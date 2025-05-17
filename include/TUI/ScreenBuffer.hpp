#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include "GameOfLife.hpp"
#include "TUI/Term.hpp"
#include "common.hpp"


struct Cell {
    std::string chr = " ";
    Term::Color fg = Term::Color::WHITE;
    Term::Color bg = Term::Color::DEFAULT;
    bool bold;

    Cell() = default;
    Cell(char chr, Term::Color fg = Term::Color::WHITE, bool bold = false);
    Cell(std::string chr, Term::Color fg = Term::Color::WHITE, bool bold = false);
};

class ScreenBuffer {
public:
    static constexpr int MIN_WIDTH = 80;
    static constexpr int MIN_HEIGHT = 24;

    ScreenBuffer(Term::Color cells_color);

    void Clear();
    void Resize(Size size);

    void DrawChar(Pos pos, Cell cell);
    void DrawString(Pos pos, const std::string& str, Term::Color color = Term::Color::WHITE, bool bold = false);
    void DrawGrid(GameOfLife& gameOfLife, GameState game_state, Pos selected_cell);

    void Render();
    void RenderTooSmallMessage();

private:
    Term::Color cells_color;

    Size size;
    std::vector<std::vector<Cell>> buffer;

    Pos GetCenteredPos();
    int GetColOffset(size_t stringLength, int termWidth);
};
