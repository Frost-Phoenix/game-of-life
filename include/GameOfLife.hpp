#pragma once

#include <cstddef>
#include <vector>

#include "common.hpp"


enum CellState {
    DEAD,
    ALIVE,
};

class GameOfLife {
public:
    GameOfLife(size_t nb_rows, size_t nb_cols);

    void Step();
    void ClearGrig();
    void toggleCell(Pos pos);

    void ResizeGrid(size_t nb_rows, size_t nb_cols);
    void GenerateRandomGrid();

    size_t GetNbRows() const;
    size_t GetNbCols() const;
    int GetNbGenerations() const;
    const std::vector<std::vector<CellState>>& GetGrid() const;

private:
    int generations;

    size_t nb_rows;
    size_t nb_cols;
    std::vector<std::vector<CellState>> grid;

    int GetNbNeighbors(Pos pos);
    bool IsPosInBounds(Pos pos);
};