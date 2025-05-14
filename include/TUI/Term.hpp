#pragma once


namespace Term {

    enum class Color {
        BLACK,
        RED,
        GREEN,
        YELLOW,
        BLUE,
        MAGENTA,
        CYAN,
        WHITE,
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
        char character = 0;
    };

    struct Size {
        int rows;
        int cols;
    };

    void Init();
    void Restore();

    void Clear();
    void SetBold();
    void SetTextColor(Color color);
    void SetCursorPos(int row, int col);
    void ResetFormating();
    void SynchronizedOutputStart();
    void SynchronizedOutputEnd();

    Size GetTermSize();
    bool GetInput(KeyEvent& ch);

    bool WasResized();

} // namespace Term