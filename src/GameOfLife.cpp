#include "GameOfLife.hpp"

#include <cstddef>
#include <cstdlib>
#include <vector>

#include "TUI/Term.hpp"

using namespace std;


/******************************************************
 *                     Private                        *
 ******************************************************/

bool GameOfLife::IsPosInBounds(Pos pos) {
    const int row = pos.row;
    const int col = pos.col;

    return (0 <= row) && (row < (int)nb_rows) && (0 <= col) && (col < (int)nb_cols);
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

GameOfLife::GameOfLife() {
    ResizeGrid();
}

void GameOfLife::Step() {
    vector<vector<CellState>> buff(nb_rows, std::vector<CellState>(nb_cols));

    for (size_t row = 0; row < nb_rows; row++) {
        for (size_t col = 0; col < nb_cols; col++) {
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

void GameOfLife::ResizeGrid() {
    const Term::Size term_size = Term::GetTermSize();

    // Leave a 10% margin around the game area
    this->nb_rows = term_size.rows - (term_size.rows / 5);
    this->nb_cols = term_size.cols - (term_size.cols / 5);
    this->nb_rows *= 2; // 2 pixel in one character

    this->grid.resize(nb_rows);
    for (auto& line : grid) {
        line.resize(nb_cols);
    }

    GenerateRandomGrid();
}

void GameOfLife::GenerateRandomGrid() {
    for (size_t row = 0; row < nb_rows; row++) {
        for (size_t col = 0; col < nb_cols; col++) {
            const bool is_alive = (rand() % 4) == 0;

            if (is_alive) {
                grid[row][col] = ALIVE;
            } else {
                grid[row][col] = DEAD;
            }
        }
    }
}

size_t GameOfLife::GetNbRows() const {
    return nb_rows;
}

size_t GameOfLife::GetNbCols() const {
    return nb_cols;
}

const vector<vector<CellState>>& GameOfLife::GetGrid() const {
    return grid;
}