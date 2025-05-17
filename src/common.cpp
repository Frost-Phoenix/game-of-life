#include "common.hpp"

#include <string>

using std::string;


Size::Size(int nb_rows, int nb_cols) {
    this->nb_rows = nb_rows;
    this->nb_cols = nb_cols;
}

Pos::Pos(int row, int col) {
    this->row = row;
    this->col = col;
}