#pragma once

#include <string>


enum GameState {
    Playing,
    Insert,
};

struct Pos {
    int row;
    int col;

    Pos() = default;
    Pos(int row, int col);
};


std::string repeat(const std::string& str, size_t n);