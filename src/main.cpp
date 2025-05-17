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


void Resize(TUI& tui, GameOfLife& gameOfLife, Pos& selected_cell) {
    const Term::Size term_size = Term::GetTermSize();
    // Leave a 5% margin around the game area
    int tui_nb_rows = term_size.rows - (term_size.rows / 10);
    int tui_nb_cols = term_size.cols - (term_size.cols / 10);

    int game_nb_rows = (tui_nb_rows - 2) * 2;
    int game_nb_cols = tui_nb_cols - 24;

    gameOfLife.ResizeGrid(game_nb_rows, game_nb_cols);
    tui.Resize(tui_nb_rows, tui_nb_cols);
    selected_cell = Pos(gameOfLife.GetNbRows() / 2, gameOfLife.GetNbCols() / 2);
}

int main(int argc, char* argv[]) {
    srand(time(nullptr));

    int FPS = 10;
    int randomness = 25;
    Color cells_color = Color::WHITE;

    Args::ParseArgs(argc, argv, FPS, randomness, cells_color);

    GameOfLife gameOfLife(randomness);

    TUI tui(cells_color);
    tui.Init();

    Pos selected_cell;

    Resize(tui, gameOfLife, selected_cell);

    KeyEvent pressedKey;

    auto frame_duration = milliseconds(1000 / FPS);
    auto last_frame = steady_clock::now();

    bool paused = true;
    GameState game_state = Playing;

    tui.Render(gameOfLife, game_state, selected_cell, paused, FPS);

    bool runnig = true;
    while (runnig) {
        if (Term::WasResized()) {
            Resize(tui, gameOfLife, selected_cell);
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
            if (!tui.IsScreenBigEnough()) {
                if (pressedKey.type == KeyType::CHARACTER && pressedKey.character == 'q') {
                    runnig = false;
                    break;
                }
            }
            if (game_state == Playing) {
                switch (pressedKey.type) {
                    case KeyType::CHARACTER:
                        switch (pressedKey.character) {
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
                            case '=':
                                FPS++;
                                frame_duration = milliseconds(1000 / FPS);
                                break;
                            case '-':
                                if (FPS > 1) {
                                    FPS--;
                                }
                                frame_duration = milliseconds(1000 / FPS);
                                break;
                            case '+':
                                FPS += 5;
                                frame_duration = milliseconds(1000 / FPS);
                                break;
                            case '_':
                                if (FPS > 5) {
                                    FPS -= 5;
                                }
                                frame_duration = milliseconds(1000 / FPS);
                                break;
                            default: break;
                        }
                        break;
                    default: break;
                }
            } else if (game_state == Insert) {
                switch (pressedKey.type) {
                    case KeyType::CHARACTER:
                        switch (pressedKey.character) {
                            case 'q': runnig = false; break;
                            case 'c': gameOfLife.ClearGrig(); break;
                            case 'r': gameOfLife.GenerateRandomGrid(); break;
                            case 't': gameOfLife.toggleCell(selected_cell); break;
                            case ' ': gameOfLife.toggleCell(selected_cell); break;
                            case 'k':
                                if (selected_cell.row > 0) {
                                    selected_cell.row--;
                                }
                                break;
                            case 'j':
                                if (selected_cell.row < (int)gameOfLife.GetNbRows() - 1) {
                                    selected_cell.row++;
                                }
                                break;
                            case 'h':
                                if (selected_cell.col > 0) {
                                    selected_cell.col--;
                                }
                                break;
                            case 'l':
                                if (selected_cell.col < (int)gameOfLife.GetNbCols() - 1) {
                                    selected_cell.col++;
                                }
                                break;
                            default: break;
                        }
                        break;
                    case KeyType::ARROW_UP:
                        if (selected_cell.row > 0) {
                            selected_cell.row--;
                        }
                        break;
                    case KeyType::ARROW_DOWN:
                        if (selected_cell.row < (int)gameOfLife.GetNbRows() - 1) {
                            selected_cell.row++;
                        }
                        break;
                    case KeyType::ARROW_LEFT:
                        if (selected_cell.col > 0) {
                            selected_cell.col--;
                        }
                        break;
                    case KeyType::ARROW_RIGHT:
                        if (selected_cell.col < (int)gameOfLife.GetNbCols() - 1) {
                            selected_cell.col++;
                        }
                        break;
                    case KeyType::ESCAPE: game_state = Playing; break;
                    default:              break;
                }
            }
            tui.Render(gameOfLife, game_state, selected_cell, paused, FPS);
        }

        std::this_thread::sleep_for(milliseconds(1));
    }

    tui.Exit();
}
