#include <cstdlib>
#include <ctime>

#include "GameOfLife.hpp"
#include "TUI/TUI.hpp"
#include "TUI/Term.hpp"


using Term::KeyType, Term::KeyEvent;


int main() {
    srand(time(nullptr));

    GameOfLife gameOfLife;

    TUI tui;
    tui.Init(gameOfLife);
    tui.Render(gameOfLife);

    KeyEvent pressedKey;

    bool runnig = true;
    while (runnig) {
        if (Term::WasResized()) {
            gameOfLife.ResizeGrid();
            tui.Resize(gameOfLife);
            tui.Render(gameOfLife);

            continue;
        }

        if (Term::GetInput(pressedKey)) {
            switch (pressedKey.type) {
                case KeyType::CHARACTER:
                    switch (pressedKey.character) {
                        case 'q': runnig = false; break;
                        default:  break;
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
    }

    tui.Exit();
}
