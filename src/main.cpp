#include <chrono>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <unistd.h>

#include "GameOfLife.hpp"
#include "TUI/TUI.hpp"
#include "TUI/Term.hpp"
#include "common.hpp"

using namespace std::chrono;
using Term::KeyType, Term::KeyEvent;


int main() {
    srand(time(nullptr));

    GameOfLife gameOfLife;

    TUI tui;
    tui.Init(gameOfLife);
    tui.Render(gameOfLife);

    KeyEvent pressedKey;

    int FPS = 10;
    auto frame_duration = milliseconds(1000 / FPS);
    auto last_frame = steady_clock::now();

    bool paused = true;

    bool runnig = true;
    while (runnig) {
        if (Term::WasResized()) {
            gameOfLife.ResizeGrid();
            tui.Resize(gameOfLife);
            tui.Render(gameOfLife);

            continue;
        }

        if (!paused) {
            auto now = steady_clock::now();
            if (duration_cast<milliseconds>(now - last_frame) >= frame_duration) {
                gameOfLife.Step();
                tui.Render(gameOfLife);

                last_frame = now;
            }
        }

        if (Term::GetInput(pressedKey)) {
            switch (pressedKey.type) {
                case KeyType::CHARACTER:
                    switch (pressedKey.character) {
                        case 'q': runnig = false; break;
                        case 'p': paused = !paused; break;
                        case 's': gameOfLife.Step(); break;
                        case 'c': gameOfLife.ClearGrig(); break;
                        case 'r': gameOfLife.GenerateRandomGrid(); break;
                        case 't': gameOfLife.toggleCell(Pos(0, 0)); break;
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
                        default: break;
                    }
                    break;
                case KeyType::ARROW_UP:    break;
                case KeyType::ARROW_DOWN:  break;
                case KeyType::ARROW_LEFT:  break;
                case KeyType::ARROW_RIGHT: break;
                default:                   break;
            }
            tui.Render(gameOfLife);
        }

        std::this_thread::sleep_for(milliseconds(1));
    }

    tui.Exit();
}
