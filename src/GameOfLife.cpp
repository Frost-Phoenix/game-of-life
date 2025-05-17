#include "GameOfLife.hpp"

#include <cstdlib>
#include <vector>

#include "common.hpp"

using namespace std;


/******************************************************
 *                     Private                        *
 ******************************************************/

bool GameOfLife::IsPosInBounds(Pos pos) {
    const int row = pos.row;
    const int col = pos.col;

    return (0 <= row) && (row < size.nb_rows) && (0 <= col) && (col < size.nb_cols);
}

int GameOfLife::GetNbNeighbors(Pos pos) {
    int res = 0;

    for (int row = pos.row - 1; row <= pos.row + 1; row++) {
        for (int col = pos.col - 1; col <= pos.col + 1; col++) {
            if (row == pos.row && col == pos.col) continue;
            if (!IsPosInBounds(Pos(row, col))) continue;


            if (grid[row][col] == ALIVE) {
                res++;
            }
        }
    }

    return res;
}

/******************************************************
 *                      Public                        *
 ******************************************************/

GameOfLife::GameOfLife(int randomness) {
    this->randomness = randomness;
}

void GameOfLife::Step() {
    vector<vector<CellState>> buff(size.nb_rows, std::vector<CellState>(size.nb_cols));

    generations++;

    for (int row = 0; row < size.nb_rows; row++) {
        for (int col = 0; col < size.nb_cols; col++) {
            CellState cell_state = grid[row][col];
            CellState& new_state = buff[row][col];

            const int nb_neighbors = GetNbNeighbors(Pos(row, col));

            switch (cell_state) {
                case ALIVE: {
                    if (nb_neighbors < 2 || nb_neighbors > 3) {
                        new_state = DEAD;
                    } else {
                        new_state = ALIVE;
                    }
                    break;
                }
                case DEAD: {
                    if (nb_neighbors == 3) {
                        new_state = ALIVE;
                    } else {
                        new_state = DEAD;
                    }
                    break;
                }
            }
        }
    }

    grid = std::move(buff);
}

void GameOfLife::ClearGrig() {
    generations = 0;

    for (auto& line : grid) {
        for (auto& cell_state : line) {
            cell_state = DEAD;
        }
    }
}

void GameOfLife::toggleCell(Pos pos) {
    CellState& cell_state = grid[pos.row][pos.col];

    if (cell_state == ALIVE) {
        cell_state = DEAD;
    } else {
        cell_state = ALIVE;
    }
}

void GameOfLife::ResizeGrid(Size size) {
    this->size = size;
    this->generations = 0;

    this->grid.resize(size.nb_rows);
    for (auto& line : grid) {
        line.resize(size.nb_cols);
    }

    GenerateRandomGrid();
}

void GameOfLife::GenerateRandomGrid() {
    for (int row = 0; row < size.nb_rows; row++) {
        for (int col = 0; col < size.nb_cols; col++) {
            const bool is_alive = (rand() % 100) < randomness;

            if (is_alive) {
                grid[row][col] = ALIVE;
            } else {
                grid[row][col] = DEAD;
            }
        }
    }
}

int GameOfLife::GetNbRows() const {
    return size.nb_rows;
}

int GameOfLife::GetNbCols() const {
    return size.nb_cols;
}

int GameOfLife::GetNbGenerations() const {
    return generations;
}

const vector<vector<CellState>>& GameOfLife::GetGrid() const {
    return grid;
}