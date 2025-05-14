#pragma once

#include <string>

#include "GameOfLife.hpp"
#include "TUI/ScreenBuffer.hpp"
#include "Term.hpp"
#include "common.hpp"


class TUI {
public:
    void Init(GameOfLife& gameOfLife);
    void Exit();

    void Resize(GameOfLife& gameOfLife);

    void Render(GameOfLife& gameOfLife);

    ScreenBuffer screenBuffer;

private:
    size_t nb_rows;
    size_t nb_cols;

    void RenderBox(Pos pos, int width, int height, Term::Color color, const std::string& name = "");
    void RenderBoxGame();

    bool IsScreenBigEnough();
};
