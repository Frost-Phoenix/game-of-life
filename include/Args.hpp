#pragma once

#include "TUI/Term.hpp"


namespace Args {
    void ParseArgs(int argc, char* argv[], int& fps, int& randomness, Term::Color& cells_color);
}