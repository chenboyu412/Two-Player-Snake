#include "console.h"
#include <conio.h>
#include <cstdlib>

void Console::init() const
{
    clear();
    hideCursor();
}

void Console::clear() const
{
    system("cls");
}

int Console::translateArrowKey(int code) const
{
    switch (code)
    {
    case 72:
        return KEY_UP_CODE;
    case 80:
        return KEY_DOWN_CODE;
    case 75:
        return KEY_LEFT_CODE;
    case 77:
        return KEY_RIGHT_CODE;
    default:
        return code;
    }
}

int Console::getKey() const
{
    if (!_kbhit())
    {
        return ERR_KEY;
    }

    int c = _getch();

    if (c == 0 || c == 0xE0)
    {
        int c2 = _getch();
        return translateArrowKey(c2);
    }

    return c;
}

void Console::sleep(int ms) const
{
    if (ms > 0)
    {
        Sleep(ms);
    }
}

void Console::setCursorVisible(bool visible) const
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;

    if (GetConsoleCursorInfo(hOut, &cursorInfo))
    {
        cursorInfo.bVisible = visible;
        SetConsoleCursorInfo(hOut, &cursorInfo);
    }
}

void Console::hideCursor() const
{
    setCursorVisible(false);
}

void Console::showCursor() const
{
    setCursorVisible(true);
}

void Console::getSize(int& width, int& height) const
{
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);

    width = info.srWindow.Right - info.srWindow.Left + 1;
    height = info.srWindow.Bottom - info.srWindow.Top + 1;
}

void Console::drawChar(int x, int y, char ch) const
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    COORD position;
    position.X = static_cast<SHORT>(x);
    position.Y = static_cast<SHORT>(y);

    DWORD written = 0;

    SetConsoleCursorPosition(hOut, position);
    WriteConsoleA(hOut, &ch, 1, &written, nullptr);
}

void Console::moveCursor(int x, int y) const
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    COORD position;
    position.X = static_cast<SHORT>(x);
    position.Y = static_cast<SHORT>(y);

    SetConsoleCursorPosition(hOut, position);
}
