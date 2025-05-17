#include "Args.hpp"

#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>
#include <tuple>

#include "TUI/Term.hpp"

using namespace std;
using Term::Color;


namespace Args {

    /******************************************************
     *                     Private                        *
     ******************************************************/

    const tuple<Color, string> colors[] = {
        { Color::BLACK, "Black" },   { Color::RED, "Red" },   { Color::GREEN, "Green" },
        { Color::YELLOW, "Yellow" }, { Color::BLUE, "Blue" }, { Color::MAGENTA, "Magenta" },
        { Color::WHITE, "White" },
    };

    void PrintOption(string opt_short, string opt_long, string description) {
        Term::SetTextColor(Color::GREEN);
        cout << "    " << opt_short;
        Term::SetTextColor(Color::DEFAULT);
        cout << ", ";
        Term::SetTextColor(Color::GREEN);
        cout << opt_long;
        Term::SetTextColor(Color::DEFAULT);
        cout << description << "\n";
    }

    void PrintKeybind(string key, string description) {
        Term::SetTextColor(Color::GREEN);
        cout << "    [" << key << "]";
        Term::SetTextColor(Color::DEFAULT);
        cout << description << "\n";
    }

    void PrintColors() {
        int color_id = 0;
        for (auto& [color, color_name] : colors) {
            cout << "    " << color_id << ": ";
            Term::SetTextColor(color);
            cout << color_name << "\n";
            Term::ResetFormating();
            color_id++;
        }
    }

    void Help() {
        cout << "Game of Life - A terminal-based Conway's Game of Life simulation\n\n";

        Term::SetTextColor(Color::YELLOW);
        cout << "USAGE: ";
        Term::SetTextColor(Color::DEFAULT);
        cout << "game-of-life";
        Term::ResetFormating();
        cout << " [OPTIONS]\n";

        Term::SetTextColor(Color::YELLOW);
        cout << "\nOPTIONS:\n";
        PrintOption("-f", "--fps <FPS>", "\t\t     Set the speed of the simulation [default: 10]");
        PrintOption("-r", "--randomness <RANDOMNESS>",
                    "    Set the percentage of randomness [default: 25%]");
        PrintOption("-c", "--color <COLOR_ID>", "\t     Set the cells color [default: 7 white]");
        PrintOption("-h", "--help", "\t\t\t     Show this help message and exit");

        Term::SetTextColor(Color::YELLOW);
        cout << "\nKEYBINDS:\n";
        Term::SetTextColor(Color::DEFAULT);
        PrintKeybind("q", "\t\t   Quit");
        Term::SetTextColor(Color::YELLOW);
        cout << "\n  Default:\n";
        Term::SetTextColor(Color::DEFAULT);
        PrintKeybind("s", "\t\t   Step the simulation");
        PrintKeybind("p", "\t\t   Toggle pause status");
        PrintKeybind("i", "\t\t   Enter Insert mode");
        PrintKeybind("(shift) =", "    +1 (+5) FPS");
        PrintKeybind("(shift) -", "    -1 (-5) FPS");
        Term::SetTextColor(Color::YELLOW);
        cout << "\n  Insert:\n";
        Term::SetTextColor(Color::DEFAULT);
        PrintKeybind("c", "\t\t   Clear the grid");
        PrintKeybind("t", "\t\t   Toggle cell under the cursor");
        PrintKeybind("r", "\t\t   Generate random grid");
        PrintKeybind("h,j,k,l", "\t   Move the cursor");
        PrintKeybind("Arrows", "\t   Move the cursor");
        PrintKeybind("Escape", "\t   Quit insert mode");

        Term::SetTextColor(Color::YELLOW);
        cout << "\nColors:\n";
        Term::ResetFormating();
        PrintColors();

        Term::SetTextColor(Color::YELLOW);
        cout << "\nNotes:\n";
        Term::SetTextColor(Color::DEFAULT);
        cout << "    - Grid size is determine by the terminal size\n";
        cout << "    - Colors index goes from 0 to 7\n";
    }

    void ExitError(string message, string found = "") {
        Term::SetTextColor(Color::RED);
        Term::SetBold();
        cout << "error: ";
        Term::ResetFormating();
        cout << message;

        if (found != "") {
            cout << " '";
            Term::SetTextColor(Color::YELLOW);
            cout << found;
            Term::ResetFormating();
            cout << "'";
        }

        cout << "\n\nFor more information try '";
        Term::SetTextColor(Color::GREEN);
        cout << "--help";
        Term::ResetFormating();
        cout << "'\n";

        exit(EXIT_SUCCESS);
    }

    /******************************************************
     *                      Public                        *
     ******************************************************/
    void ParseArgs(int argc, char* argv[], int& fps, int& randomness, Term::Color& cells_color) {
        for (int i = 1; i < argc; ++i) {
            string arg = argv[i];

            if (arg == "-h" || arg == "--help") {
                Help();
                exit(0);
            } else if (arg == "-f" || arg == "--fps") {
                if (i + 1 == argc) {
                    ExitError("FPS not provided");
                }
                try {
                    fps = stoi(argv[++i]);
                    if (fps <= 0) {
                        ExitError("FPS must be grater than zero");
                    }
                } catch (const exception& e) {
                    ExitError("invalid FPS", string(argv[i]));
                }
            } else if (arg == "-r" || arg == "--randomness") {
                if (i + 1 == argc) {
                    ExitError("Randomness not provided");
                }
                try {
                    randomness = stoi(argv[++i]);
                    if (randomness < 0 || randomness > 100) {
                        ExitError("Randomness must be between 0 and 100");
                    }
                } catch (const exception& e) {
                    ExitError("invalid randomness", string(argv[i]));
                }
            } else if (arg == "-c" || arg == "--color") {
                if (i + 1 == argc) {
                    ExitError("Color not provided");
                }
                try {
                    int index = stoi(argv[++i]);
                    if (index < 0 || index > 7) {
                        ExitError("Color index must be between 0 and 7");
                    }
                    cells_color = get<0>(colors[index]);
                } catch (const exception& e) {
                    ExitError("invalid color index", string(argv[i]));
                }
            } else {
                ExitError("unexpected argument", arg);
            }
        }
    }
} // namespace Args