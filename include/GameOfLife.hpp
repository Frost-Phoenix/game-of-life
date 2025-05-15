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
    GameOfLife();

    void Step();
    void ClearGrig();
    void toggleCell(Pos pos);

    void ResizeGrid();
    void GenerateRandomGrid();

    size_t GetNbRows() const;
    size_t GetNbCols() const;
    const std::vector<std::vector<CellState>>& GetGrid() const;

private:
    size_t nb_rows;
    size_t nb_cols;

    std::vector<std::vector<CellState>> grid;

    int GetNbNeighbors(Pos pos);
    bool IsPosInBounds(Pos pos);
};