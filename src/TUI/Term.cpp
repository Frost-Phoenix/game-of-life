#include "TUI/Term.hpp"

#include <csignal>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <string>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

#include "common.hpp"

using std::string;
using std::cout, std::flush;


namespace Term {

    /******************************************************
     *                     Private                        *
     ******************************************************/

    termios original_termios;
    termios raw;

    static sig_atomic_t resized = false;

    void SaveTerminalState() {
        tcgetattr(STDIN_FILENO, &original_termios);
    }

    void RestoreTerminalState() {
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_termios);
    }

    void EnterRawMode() {
        raw = original_termios;
        raw.c_lflag &= ~(ECHO | ICANON); // No echo, no canonical mode
        raw.c_cc[VMIN] = 0;
        raw.c_cc[VTIME] = 1; // Timeout for read()

        tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
    }

    void HandleSignal(int sig) {
        switch (sig) {
            case SIGWINCH: resized = true; break;
            case SIGINT:
            case SIGTERM:
                Restore();
                std::exit(EXIT_SUCCESS);
                break;
        }
    }

    void SetupSignalHandlers() {
        std::signal(SIGINT, HandleSignal);   // Ctrl+C
        std::signal(SIGTERM, HandleSignal);  // kill
        std::signal(SIGWINCH, HandleSignal); // window resize
    }

    KeyEvent ReadKey() {
        char chr;
        ssize_t n = read(STDIN_FILENO, &chr, 1);
        if (n <= 0) return { .type = KeyType::UNKNOWN };

        if (chr != '\x1B') {
            switch (chr) {
                case '\x1B': return { .type = KeyType::ESCAPE };
                default:     return { .type = KeyType::CHARACTER, .chr = chr };
            }
        }

        // Escape sequence
        char seq[3];
        if (read(STDIN_FILENO, &seq[0], 1) != 1) return { .type = KeyType::ESCAPE };
        if (read(STDIN_FILENO, &seq[1], 1) != 1) return { .type = KeyType::ESCAPE };

        if (seq[0] == '[') {
            switch (seq[1]) {
                case 'A': return { .type = KeyType::ARROW_UP };
                case 'B': return { .type = KeyType::ARROW_DOWN };
                case 'C': return { .type = KeyType::ARROW_RIGHT };
                case 'D': return { .type = KeyType::ARROW_LEFT };
            }
        }

        return { .type = KeyType::UNKNOWN };
    }

    /******************************************************
     *                      Public                        *
     ******************************************************/

    void Init() {
        SaveTerminalState();
        SetupSignalHandlers();
        EnterRawMode();

        cout << "\033[?1049h"; // Enter alternate buffer
        cout << "\033[?25l";   // Hide cursor
    }

    void Restore() {
        cout << "\033[?25h";   // Show cursor
        cout << "\033[?1049l"; // Leave alternate buffer

        RestoreTerminalState();
    }

    void Clear() {
        cout << "\033[H\033[J" << flush;
    }

    void SetBold() {
        cout << "\033[1m" << flush;
    }

    void SetTextColor(Color color) {
        cout << "\033[" << static_cast<int>(color) << "m" << flush;
    }

    void SetBgColor(Color color) {
        cout << "\033[" << static_cast<int>(color) + 10 << "m" << flush;
    }

    void SetCursorPos(int row, int col) {
        cout << "\033[" << row + 1 << ";" << col + 1 << "H" << flush;
    }

    void ResetFormating() {
        cout << "\033[0m" << flush;
    }

    void SynchronizedOutputStart() {
        cout << "\033[?2026h";
    }

    void SynchronizedOutputEnd() {
        cout << "\033[?2026l" << flush;
    }

    Size GetTermSize() {
        struct winsize w;

        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

        return Size(w.ws_row, w.ws_col);
    }

    bool GetInput(KeyEvent& keyEvent) {
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);

        struct timeval timeout = { .tv_sec = 0, .tv_usec = 0 };

        int result = select(STDIN_FILENO + 1, &readfds, nullptr, nullptr, &timeout);

        if (result > 0 && FD_ISSET(STDIN_FILENO, &readfds)) {
            keyEvent = ReadKey();
            return true;
        }

        return false;
    }

    bool WasResized() {
        if (resized) {
            resized = false;
            return true;
        }

        return false;
    }
} // namespace Term