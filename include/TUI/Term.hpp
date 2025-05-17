#pragma once

#include "common.hpp"


namespace Term {

    enum class Color {
        DEFAULT = 39,

        BLACK = 30,
        RED = 31,
        GREEN = 32,
        YELLOW = 33,
        BLUE = 34,
        MAGENTA = 35,
        CYAN = 36,
        WHITE = 37,

        BRIGHT_BLACK = 90,
        BRIGHT_RED = 91,
        BRIGHT_GREEN = 92,
        BRIGHT_YELLOW = 93,
        BRIGHT_BLUE = 94,
        BRIGHT_MAGENTA = 95,
        BRIGHT_CYAN = 96,
        BRIGHT_WHITE = 97,
    };

    enum class KeyType {
        CHARACTER,

        ESCAPE,

        ARROW_UP,
        ARROW_DOWN,
        ARROW_LEFT,
        ARROW_RIGHT,

        UNKNOWN,
    };

    struct KeyEvent {
        KeyType type;
        char chr = '\0';
    };

    void Init();
    void Restore();

    void Clear();
    void SetBold();
    void SetTextColor(Color color);
    void SetBgColor(Color color);
    void SetCursorPos(int row, int col);
    void ResetFormating();
    void SynchronizedOutputStart();
    void SynchronizedOutputEnd();

    Size GetTermSize();
    bool GetInput(KeyEvent& keyEvent);

    bool WasResized();

} // namespace Term