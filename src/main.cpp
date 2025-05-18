#include <chrono>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <unistd.h>

#include "Args.hpp"
#include "GameOfLife.hpp"
#include "TUI/TUI.hpp"
#include "TUI/Term.hpp"
#include "common.hpp"

using namespace std::chrono;
using Term::KeyType, Term::KeyEvent, Term::Color;


enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT,
};


int FPS = 10;
bool paused = true;
bool runnig = true;
int randomness = 25;

milliseconds frame_duration;
time_point<steady_clock> last_frame;

Pos selected_cell;
KeyEvent pressedKey;
GameState game_state = Playing;
Color cells_color = Color::WHITE;


void Resize(TUI& tui, GameOfLife& gameOfLife) {
    const Size term_size = Term::GetTermSize();

    // Leave a 5% margin around the game area
    Size tui_size(term_size.nb_rows - (term_size.nb_rows / 10), term_size.nb_cols - (term_size.nb_cols / 10));
    tui.Resize(tui_size);

    Size game_size((tui_size.nb_rows - 2) * 2, tui_size.nb_cols - 24);
    gameOfLife.ResizeGrid(game_size);

    selected_cell = Pos(game_size.nb_rows / 2, game_size.nb_cols / 2);
}

void UpdateFPS(int new_FPS) {
    if (new_FPS <= 0) {
        new_FPS = 1;
    }

    FPS = new_FPS;
    frame_duration = milliseconds(1000 / FPS);
}

void MoveCursor(Direction dir, GameOfLife& gameOfLife) {
    if (dir == UP && selected_cell.row > 0) {
        selected_cell.row--;
    } else if (dir == DOWN && selected_cell.row < gameOfLife.GetNbRows() - 1) {
        selected_cell.row++;
    } else if (dir == LEFT && selected_cell.col > 0) {
        selected_cell.col--;
    } else if (dir == RIGHT && selected_cell.col < gameOfLife.GetNbCols() - 1) {
        selected_cell.col++;
    }
}

void HandleInputs(TUI& tui, GameOfLife& gameOfLife) {
    if (!tui.IsScreenBigEnough()) {
        if (pressedKey.type == KeyType::CHARACTER && pressedKey.chr == 'q') {
            runnig = false;
            return;
        }
    }

    if (game_state == Playing) {
        if (pressedKey.type != KeyType::CHARACTER) {
            return;
        }

        switch (pressedKey.chr) {
            case 'q': runnig = false; break;
            case 'p': paused = !paused; break;
            case 'i':
                game_state = Insert;
                paused = true;
                break;
            case 's':
                if (paused) {
                    gameOfLife.Step();
                }
                break;
            case '=': UpdateFPS(FPS + 1); break;
            case '-': UpdateFPS(FPS - 1); break;
            case '+': UpdateFPS(FPS + 5); break;
            case '_': UpdateFPS(FPS - 5); break;
            default:  break;
        }
    } else if (game_state == Insert) {
        switch (pressedKey.type) {
            case KeyType::CHARACTER:
                switch (pressedKey.chr) {
                    case 'q': runnig = false; break;
                    case 'c': gameOfLife.ClearGrig(); break;
                    case 'r': gameOfLife.GenerateRandomGrid(); break;
                    case 't': gameOfLife.toggleCell(selected_cell); break;
                    case ' ': gameOfLife.toggleCell(selected_cell); break;
                    case 'k': MoveCursor(UP, gameOfLife); break;
                    case 'j': MoveCursor(DOWN, gameOfLife); break;
                    case 'h': MoveCursor(LEFT, gameOfLife); break;
                    case 'l': MoveCursor(RIGHT, gameOfLife); break;
                    default:  break;
                }
                break;
            case KeyType::ARROW_UP:    MoveCursor(UP, gameOfLife); break;
            case KeyType::ARROW_DOWN:  MoveCursor(DOWN, gameOfLife); break;
            case KeyType::ARROW_LEFT:  MoveCursor(LEFT, gameOfLife); break;
            case KeyType::ARROW_RIGHT: MoveCursor(RIGHT, gameOfLife); break;
            case KeyType::ESCAPE:      game_state = Playing; break;
            default:                   break;
        }
    }
}

int main(int argc, char* argv[]) {
    srand(time(nullptr));

    Args::ParseArgs(argc, argv, FPS, randomness, cells_color);

    frame_duration = milliseconds(1000 / FPS);

    GameOfLife gameOfLife(randomness);

    TUI tui(cells_color);
    tui.Init();

    Resize(tui, gameOfLife);
    tui.Render(gameOfLife, game_state, selected_cell, paused, FPS);

    last_frame = steady_clock::now();

    while (runnig) {
        if (Term::WasResized()) {
            Resize(tui, gameOfLife);
            tui.Render(gameOfLife, game_state, selected_cell, paused, FPS);

            continue;
        }

        if (!paused && game_state == Playing) {
            auto now = steady_clock::now();
            if (duration_cast<milliseconds>(now - last_frame) >= frame_duration) {
                gameOfLife.Step();
                tui.Render(gameOfLife, game_state, selected_cell, paused, FPS);

                last_frame = now;
            }
        }

        if (Term::GetInput(pressedKey)) {
            HandleInputs(tui, gameOfLife);
            tui.Render(gameOfLife, game_state, selected_cell, paused, FPS);
        }

        std::this_thread::sleep_for(milliseconds(1));
    }

    tui.Exit();
}
