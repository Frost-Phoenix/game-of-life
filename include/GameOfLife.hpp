#pragma once

#include <cstddef>
#include <vector>


enum CellState {
    DEAD,
    ALIVE,
};

class GameOfLife {
public:
    GameOfLife();

    void ResizeGrid();
    void GenerateRandomGrid();

    size_t GetNbRows() const;
    size_t GetNbCols() const;
    const std::vector<std::vector<CellState>>& GetGrid() const;

private:
    size_t nb_rows;
    size_t nb_cols;

    std::vector<std::vector<CellState>> grid;
};