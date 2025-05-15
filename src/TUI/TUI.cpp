#include "TUI/TUI.hpp"

#include <cstddef>
#include <string>

#include "GameOfLife.hpp"
#include "TUI/ScreenBuffer.hpp"
#include "TUI/Term.hpp"
#include "common.hpp"

using std::string;
using Term::Color;

const string horizontal = "─";
const string vertical = "│";
const string top_left = "┌";
const string top_right = "┐";
const string bottom_left = "└";
const string bottom_right = "┘";


/******************************************************
 *                     Private                        *
 ******************************************************/

// , Color color = Color::WHITE
void TUI::RenderBox(Pos pos, int width, int height, Color color, const string& name) {
    const int row = pos.row;
    const int col = pos.col;

    ScreenBuffer& sb = screenBuffer;

    int inner_width = width - 2;

    // Top border
    if (!name.empty()) {
        size_t name_len = name.length();
        int right_len = inner_width - name_len - 2;

        sb.DrawChar(Pos(row, col), Cell(top_left, color, true));
        sb.DrawChar(Pos(row, col + 1), Cell(horizontal, color, true));
        sb.DrawChar(Pos(row, col + 2), Cell(top_right, color, true));

        sb.DrawString(Pos(row, col + 3), name, color, true);
        sb.DrawChar(Pos(row, col + 3 + name_len), Cell(top_left, color, true));

        for (int i = 0; i < right_len; ++i) {
            sb.DrawChar(Pos(row, col + 4 + name_len + i), Cell(horizontal, color, true));
        }

        sb.DrawChar(Pos(row, col + width - 1), Cell(top_right, color, true));
    } else {
        sb.DrawChar(Pos(row, col), Cell(top_left));
        for (int i = 0; i < inner_width; ++i) {
            sb.DrawChar(Pos(row, col + 1 + i), Cell(horizontal, color, true));
        }
        sb.DrawChar(Pos(row, col + width - 1), Cell(top_right, color, true));
    }

    // Vertical sides
    for (int i = 1; i < height - 1; ++i) {
        sb.DrawChar(Pos(row + i, col), Cell(vertical, color, true));
        for (int j = 1; j < width - 1; ++j) {
            sb.DrawChar(Pos(row + i, col + j), Cell(' '));
        }
        sb.DrawChar(Pos(row + i, col + width - 1), Cell(vertical, color, true));
    }

    // Bottom border
    sb.DrawChar(Pos(row + height - 1, col), Cell(bottom_left, color, true));
    for (int i = 0; i < inner_width; ++i) {
        sb.DrawChar(Pos(row + height - 1, col + 1 + i), Cell(horizontal, color, true));
    }
    sb.DrawChar(Pos(row + height - 1, col + width - 1), Cell(bottom_right, color, true));
}

void TUI::RenderBoxGame() {
    RenderBox(Pos(0, 0), nb_cols, nb_rows, Color::BRIGHT_WHITE, "Game of Life");
}

bool TUI::IsScreenBigEnough() {
    const Term::Size termSize = Term::GetTermSize();

    return termSize.rows >= screenBuffer.MIN_HEIGHT && termSize.cols >= screenBuffer.MIN_WIDTH;
}

/******************************************************
 *                      Public                        *
 ******************************************************/

void TUI::Init(GameOfLife& gameOfLife) {
    Term::Init();
    Resize(gameOfLife);
}

void TUI::Exit() {
    Term::Restore();
}

void TUI::Resize(GameOfLife& gameOfLife) {
    this->nb_rows = (gameOfLife.GetNbRows() / 2) + 2; // +2 for borders
    this->nb_cols = gameOfLife.GetNbCols() + 2;       // +2 for borders

    screenBuffer.Resize(nb_rows, nb_cols);
}

void TUI::Render(GameOfLife& gameOfLife) {
    if (!IsScreenBigEnough()) {
        screenBuffer.RenderTooSmallMessage();
        return;
    }

    screenBuffer.Clear();

    RenderBoxGame();
    screenBuffer.DrawGrid(gameOfLife);

    screenBuffer.Render();
}