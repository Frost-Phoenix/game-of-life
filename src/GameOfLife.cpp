#include "GameOfLife.hpp"

#include <cstddef>
#include <cstdlib>

#include "TUI/Term.hpp"


/******************************************************
 *                     Private                        *
 ******************************************************/


/******************************************************
 *                      Public                        *
 ******************************************************/

GameOfLife::GameOfLife() {
    ResizeGrid();
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

const std::vector<std::vector<CellState>>& GameOfLife::GetGrid() const {
    return grid;
}