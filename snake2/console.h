#ifndef CONSOLE_H
#define CONSOLE_H

#include <windows.h>

class Console
{
private:
    int translateArrowKey(int code) const;
    void setCursorVisible(bool visible) const;

public:
    enum Key
    {
        ERR_KEY = -1,
        KEY_UP_CODE = 1000,
        KEY_DOWN_CODE = 1001,
        KEY_LEFT_CODE = 1002,
        KEY_RIGHT_CODE = 1003
    };

    void init() const;
    void clear() const;

    // Non-blocking input.
    // If no key is waiting, this returns ERR_KEY immediately.
    int getKey() const;

    void sleep(int ms) const;

    void hideCursor() const;
    void showCursor() const;

    void getSize(int& width, int& height) const;
    void drawChar(int x, int y, char ch) const;
    void moveCursor(int x, int y) const;
};

#endif
