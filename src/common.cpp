#include "common.hpp"

#include <string>

using std::string;


Pos::Pos(int row, int col) {
    this->row = row;
    this->col = col;
}

string repeat(const string& str, size_t n) {
    string result;
    result.reserve(str.size() * n);

    for (size_t i = 0; i < n; i++) {
        result += str;
    }

    return result;
}