#pragma once


enum GameState {
    Playing,
    Insert,
};

struct Size {
    int nb_rows;
    int nb_cols;

    Size() = default;
    Size(int nb_rows, int nb_cols);
};

struct Pos {
    int row;
    int col;

    Pos() = default;
    Pos(int row, int col);
};