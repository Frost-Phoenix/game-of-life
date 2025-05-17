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
    GameOfLife(int randomness);

    void Step();
    void ClearGrig();
    void toggleCell(Pos pos);

    void ResizeGrid(Size size);
    void GenerateRandomGrid();

    int GetNbRows() const;
    int GetNbCols() const;
    int GetNbGenerations() const;
    const std::vector<std::vector<CellState>>& GetGrid() const;

private:
    int generations;
    int randomness;

    Size size;
    std::vector<std::vector<CellState>> grid;

    int GetNbNeighbors(Pos pos);
    bool IsPosInBounds(Pos pos);
};