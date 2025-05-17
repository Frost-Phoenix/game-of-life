#pragma once

#include <string>

#include "GameOfLife.hpp"
#include "TUI/ScreenBuffer.hpp"
#include "Term.hpp"
#include "common.hpp"


class TUI {
public:
    TUI(Term::Color cells_color);

    void Init();
    void Exit();

    void Resize(Size size);
    bool IsScreenBigEnough();

    void Render(GameOfLife& gameOfLife, GameState game_state, Pos selected_cell, bool paused, int FPS);


private:
    Size size;
    ScreenBuffer screenBuffer;

    void RenderBox(Pos pos, int width, int height, Term::Color name_color, const std::string& name = "");
    void RenderBoxGame(int width, GameState game_state, bool paused);
    void RenderBoxKeybinds(GameState game_state);
    void RenderBoxStats(int FPS, int nb_generations);
};
