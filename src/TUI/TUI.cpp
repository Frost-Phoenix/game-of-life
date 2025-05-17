#include "TUI/TUI.hpp"

#include <cmath>
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

void TUI::RenderBox(Pos pos, int width, int height, Color name_color, const string& name) {
    const int row = pos.row;
    const int col = pos.col;

    Color color = Color::BRIGHT_WHITE;

    ScreenBuffer& sb = screenBuffer;

    int inner_width = width - 2;

    // Top border
    if (!name.empty()) {
        size_t name_len = name.length();

        int name_start = col + 1 + (inner_width - name_len) / 2;
        int left_len = name_start - (col + 1);
        int right_len = inner_width - name_len - left_len;

        sb.DrawChar(Pos(row, col), Cell(top_left, color, true));
        for (int i = 0; i < left_len - 1; ++i) {
            sb.DrawChar(Pos(row, col + 1 + i), Cell(horizontal, color, true));
        }

        sb.DrawChar(Pos(row, col + left_len), Cell(top_right, color, true));
        sb.DrawString(Pos(row, name_start), name, name_color, true);
        sb.DrawChar(Pos(row, name_start + name_len), Cell(top_left, color, true));

        // Draw horizontal line after name
        for (int i = 0; i < right_len - 1; ++i) {
            sb.DrawChar(Pos(row, name_start + name_len + 1 + i), Cell(horizontal, color, true));
        }

        // Draw right border
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

void TUI::RenderBoxGame(int width, GameState game_state, bool paused) {
    string box_name = "Game of Life";
    if (game_state == Insert) {
        box_name += " (insert)";
    } else if (paused) {
        box_name += " (paused)";
    }

    Color color = Color::BRIGHT_WHITE;

    if (game_state == Insert) {
        color = Color::CYAN;
    } else if (paused) {
        color = Color::MAGENTA;
    }

    RenderBox(Pos(0, 0), width, size.nb_rows, color, box_name);
}

void TUI::RenderBoxKeybinds(GameState game_state) {
    const int nb_rows = size.nb_rows;
    const int nb_cols = size.nb_cols;

    RenderBox(Pos(0, nb_cols - 22), 22, nb_rows - nb_rows / 3, Color::BRIGHT_WHITE, "Keybinds");

    ScreenBuffer& sb = screenBuffer;

    int nb_lines = 12;

    int row = (nb_rows - nb_rows / 3) / 2 - (nb_lines / 2);
    int col_offset = nb_cols - 22;

    { // Quit
        sb.DrawChar(Pos(row, col_offset + 9), Cell('Q', Color::RED, true));
        sb.DrawString(Pos(row, col_offset + 10), "uit");
    }
    row += 2;

    if (game_state == Playing) {
        { // Step
            sb.DrawChar(Pos(row, col_offset + 9), Cell('S', Color::BLUE, true));
            sb.DrawString(Pos(row, col_offset + 10), "tep");
        }
        row++;
        { // Toggle Pause
            sb.DrawString(Pos(row, col_offset + 5), "Toggle ");
            sb.DrawChar(Pos(row, col_offset + 12), Cell('P', Color::MAGENTA, true));
            sb.DrawString(Pos(row, col_offset + 13), "ause");
        }
        row += 2;
        { // Increase FPS
            sb.DrawString(Pos(row, col_offset + 3), "Increase FPS: ");
            sb.DrawChar(Pos(row, col_offset + 17), Cell('=', Color::GREEN, true));
        }
        row++;
        { // Decrease FPS
            sb.DrawString(Pos(row, col_offset + 3), "Decrease FPS: ");
            sb.DrawChar(Pos(row, col_offset + 17), Cell('-', Color::RED, true));
        }
        row += 2;
        { // Insert mode
            sb.DrawChar(Pos(row, col_offset + 5), Cell('I', Color::CYAN, true));
            sb.DrawString(Pos(row, col_offset + 6), "nsert mode");
        }
        row += 3;
        { // Current mode
            sb.DrawString(Pos(row, col_offset + 6), "Mode: ", Color::WHITE, true);
            sb.DrawString(Pos(row, col_offset + 12), "Play");
        }
    } else if (game_state == Insert) {
        { // Clear
            sb.DrawChar(Pos(row, col_offset + 8), Cell('C', Color::BLUE, true));
            sb.DrawString(Pos(row, col_offset + 9), "lear");
        }
        row++;
        { // Toggle Cell state
            sb.DrawChar(Pos(row, col_offset + 5), Cell('T', Color::MAGENTA, true));
            sb.DrawString(Pos(row, col_offset + 6), "oggle Cell");
        }
        row++;
        { // Generate Random Grid
            sb.DrawChar(Pos(row, col_offset + 5), Cell('R', Color::GREEN, true));
            sb.DrawString(Pos(row, col_offset + 6), "andom Grid");
        }
        row += 2;
        { // Movement keys
            sb.DrawChar(Pos(row, col_offset + 6), Cell("↑", Color::WHITE, true));
            sb.DrawChar(Pos(row, col_offset + 15), Cell("h", Color::WHITE, true));
            row++;
            sb.DrawChar(Pos(row, col_offset + 4), Cell("←", Color::WHITE, true));
            sb.DrawChar(Pos(row, col_offset + 6), Cell("↓", Color::WHITE, true));
            sb.DrawChar(Pos(row, col_offset + 8), Cell("→", Color::WHITE, true));
            sb.DrawChar(Pos(row, col_offset + 15), Cell("h", Color::WHITE, true));
            sb.DrawString(Pos(row, col_offset + 13), "h j l", Color::WHITE, true);
        }
        row += 2;
        { // Quit insert
            sb.DrawString(Pos(row, col_offset + 2), "Play Mode: ", Color::WHITE, true);
            sb.DrawString(Pos(row, col_offset + 13), "Escape", Color::RED, true);
        }
        row += 2;
        { // Current mode
            sb.DrawString(Pos(row, col_offset + 5), "Mode: ", Color::WHITE, true);
            sb.DrawString(Pos(row, col_offset + 11), "Insert");
        }
    }
}

void TUI::RenderBoxStats(int FPS, int nb_generations) {
    const int nb_rows = size.nb_rows;
    const int nb_cols = size.nb_cols;

    Pos pos(nb_rows - nb_rows / 3, nb_cols - 22);
    RenderBox(pos, 22, nb_rows / 3, Color::BRIGHT_WHITE, "Stats");

    int row = (nb_rows - nb_rows / 3) + (nb_rows / 3 / 2) - 1;
    { // FPS counter
        int col = (nb_cols - 22) + 7;

        screenBuffer.DrawString(Pos(row, col), "FPS: ", Color::WHITE, true);
        screenBuffer.DrawString(Pos(row, col + 5), std::to_string(FPS));
    }
    row++;
    { // Generations counter
        int col = (nb_cols - 22) + 3;

        screenBuffer.DrawString(Pos(row, col), "Generations: ", Color::WHITE, true);
        screenBuffer.DrawString(Pos(row, col + 13), std::to_string(nb_generations));
    }
}

/******************************************************
 *                      Public                        *
 ******************************************************/

TUI::TUI(Color cells_color)
    : screenBuffer(cells_color) {
}

void TUI::Init() {
    Term::Init();
}

void TUI::Exit() {
    Term::Restore();
}

void TUI::Resize(Size size) {
    this->size = size;

    screenBuffer.Resize(size);
}

bool TUI::IsScreenBigEnough() {
    const Size termSize = Term::GetTermSize();

    return termSize.nb_rows >= screenBuffer.MIN_HEIGHT && termSize.nb_cols >= screenBuffer.MIN_WIDTH;
}

void TUI::Render(GameOfLife& gameOfLife, GameState game_state, Pos selected_cell, bool paused, int FPS) {
    if (!IsScreenBigEnough()) {
        screenBuffer.RenderTooSmallMessage();
        return;
    }

    screenBuffer.Clear();

    RenderBoxGame(gameOfLife.GetNbCols() + 2, game_state, paused);
    screenBuffer.DrawGrid(gameOfLife, game_state, selected_cell);
    RenderBoxKeybinds(game_state);
    RenderBoxStats(FPS, gameOfLife.GetNbGenerations());

    screenBuffer.Render();
}