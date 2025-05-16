#include "TUI/ScreenBuffer.hpp"

#include <cstddef>
#include <iostream>
#include <ostream>
#include <string>
#include <unistd.h>

#include "GameOfLife.hpp"
#include "TUI/Term.hpp"
#include "common.hpp"

using std::string, Term::Color;
using std::cout, std::to_string;


Cell::Cell(char chr, Color fg, bool bold) {
    this->chr = chr;
    this->fg = fg;
    this->bold = bold;
}

Cell::Cell(string chr, Color fg, bool bold) {
    this->chr = chr;
    this->fg = fg;
    this->bold = bold;
}

/******************************************************
 *                     Private                        *
 ******************************************************/

Pos ScreenBuffer::GetCenteredPos() {
    const Term::Size termSize = Term::GetTermSize();

    int row = (termSize.rows / 2) - (nb_rows / 2);
    int col = (termSize.cols / 2) - (nb_cols / 2);

    return Pos(row, col);
}

int ScreenBuffer::GetColOffset(size_t stringLength, int termWidth) {
    return (termWidth / 2) - (stringLength / 2);
}

/******************************************************
 *                      Public                        *
 ******************************************************/

void ScreenBuffer::Clear() {
    for (auto& row : buffer) {
        for (auto& cell : row) {
            cell = Cell(' ');
        }
    }
}

void ScreenBuffer::Resize(size_t nb_rows, size_t nb_cols) {
    this->nb_rows = nb_rows;
    this->nb_cols = nb_cols;

    buffer.resize(nb_rows);

    for (auto& line : buffer) {
        line.resize(nb_cols);
    }

    Clear();
}

void ScreenBuffer::DrawChar(Pos pos, Cell cell) {
    const int row = pos.row;
    const int col = pos.col;

    if (!(0 <= row && row < (int)nb_rows && 0 <= col && col < (int)nb_cols)) {
        return;
    }

    buffer[row][col] = cell;
}

void ScreenBuffer::DrawString(Pos pos, const string& str, Color color, bool bold) {
    const int row = pos.row;
    const int col = pos.col;

    for (size_t i = 0; i < str.size(); i++) {
        const int offset = static_cast<int>(i);

        DrawChar(Pos(row, col + offset), Cell(str[i], color, bold));
    }
}

void ScreenBuffer::DrawGrid(GameOfLife& gameOfLife, GameState game_state, Pos selected_cell) {
    const auto& grid = gameOfLife.GetGrid();
    const size_t grid_width = gameOfLife.GetNbCols();
    const size_t grid_height = gameOfLife.GetNbRows();

    for (size_t row = 0; row + 1 < grid_height; row += 2) {
        for (size_t col = 0; col < grid_width; col++) {
            CellState state_top = grid[row][col];
            CellState state_bottom = grid[row + 1][col];

            Pos pos(row / 2 + 1, col + 1);

            Cell cell(" ");

            if (state_top == ALIVE && state_bottom == ALIVE) {
                cell.chr = "█";
            } else if (state_top == ALIVE) {
                cell.chr = "▀";
            } else if (state_bottom == ALIVE) {
                cell.chr = "▄";
            }

            if (game_state == Insert
                && ((int)row == selected_cell.row || (int)(row + 1) == selected_cell.row)
                && (int)col == selected_cell.col) {

                bool top_pixel = selected_cell.row % 2 == 0;

                cell.bg = Color::YELLOW;

                if (top_pixel) {
                    if (cell.chr == "█") {
                        cell.chr = "▄";
                    } else if (cell.chr == "▀") {
                        cell.fg = Color::YELLOW;
                        cell.bg = Color::DEFAULT;
                    } else if (cell.chr == " ") {
                        cell.chr = "▀";
                        cell.fg = Color::YELLOW;
                        cell.bg = Color::DEFAULT;
                    }
                } else {
                    if (cell.chr == "█") {
                        cell.chr = "▀";
                    } else if (cell.chr == "▄") {
                        cell.fg = Color::YELLOW;
                        cell.bg = Color::DEFAULT;
                    } else if (cell.chr == " ") {
                        cell.chr = "▄";
                        cell.fg = Color::YELLOW;
                        cell.bg = Color::DEFAULT;
                    }
                }
            }

            DrawChar(pos, cell);
        }
    }
}

void ScreenBuffer::Render() {
    const Pos startPos = GetCenteredPos();

    Term::SynchronizedOutputStart();
    Term::Clear();

    Term::SetTextColor(buffer[0][0].fg);
    Term::SetTextColor(buffer[0][0].bg);
    Color current_fg = Color::WHITE;
    Color current_bg = Color::DEFAULT;
    bool bold_active = false;

    int offset = 0;

    for (const auto& row : buffer) {
        Term::SetCursorPos(startPos.row + offset, startPos.col);

        for (const auto& cell : row) {
            if (cell.fg != current_fg) {
                Term::SetTextColor(cell.fg);
                current_fg = cell.fg;
            }
            if (cell.bg != current_bg) {
                Term::SetBgColor(cell.bg);
                current_bg = cell.bg;
            }
            if (cell.bold && !bold_active) {
                Term::SetBold();
                bold_active = true;
            } else if (!cell.bold && bold_active) {
                Term::ResetFormating();
                Term::SetTextColor(current_fg);
                Term::SetBgColor(current_bg);
                bold_active = false;
            }
            cout << cell.chr;
        }
        offset++;
    }

    Term::ResetFormating();
    Term::SynchronizedOutputEnd();
}

void ScreenBuffer::RenderTooSmallMessage() {
    Term::SynchronizedOutputStart();
    Term::Clear();

    const Term::Size termSize = Term::GetTermSize();

    const std::string lines[] = {
        "Terminal size too small:",
        "Minimal size:",
        "Width = " + to_string(MIN_WIDTH) + " Height = " + to_string(MIN_HEIGHT),
    };

    const int LineNb = 5;
    int row = (termSize.rows / 2) - (LineNb / 2);
    int col = 0;

    /* print lines[0] */
    col = GetColOffset(lines[0].size(), termSize.cols);
    Term::SetBold();
    Term::SetCursorPos(row, col);
    cout << lines[0];
    row += 1;

    /* print current terminal size*/
    const string termWidth = to_string(termSize.cols);
    const string termHeight = to_string(termSize.rows);
    const string line = "Width = " + termWidth + " Height = " + termHeight;
    col = GetColOffset(line.length(), termSize.cols);
    Term::ResetFormating();
    Term::SetCursorPos(row, col);
    cout << "Width = ";
    if (termSize.cols < MIN_WIDTH) {
        Term::SetTextColor(Color::RED);
    } else {
        Term::SetTextColor(Color::GREEN);
    }
    cout << termSize.cols;
    Term::ResetFormating();
    cout << " Height = ";
    if (termSize.rows < MIN_HEIGHT) {
        Term::SetTextColor(Color::RED);
    } else {
        Term::SetTextColor(Color::GREEN);
    }
    cout << termSize.rows;
    row += 2;

    /* print lines[1] */
    col = GetColOffset(lines[1].size(), termSize.cols);
    Term::ResetFormating();
    Term::SetBold();
    Term::SetCursorPos(row, col);
    cout << lines[1];
    row += 1;

    /* print lines[2] */
    col = GetColOffset(lines[2].size(), termSize.cols);
    Term::ResetFormating();
    Term::SetCursorPos(row, col);
    cout << lines[2];

    Term::ResetFormating();
    Term::SynchronizedOutputEnd();
}
