#include <iostream>
#include <windows.h>
#include <termios.h> // cho tcflush, TCIFLUSH
#include <unistd.h>  // cho STDIN_FILENO
#include <cstdlib>
#include <conio.h>
#include <time.h>

using namespace std;

void setBufferedInput(bool);

void ShowConsoleCursor(bool showFlag)
{
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
    cursorInfo.bVisible = showFlag;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

int main()
{
    return 0;
}

void gotoxy(int x, int y)
{
    printf("\033[%d;%dH", y + 1, x + 1);
    fflush(stdout);
}

void setBufferedInput(bool enable)
{
    // Không cần cài đặt trên Windows với _getch()
}

void setBufferedInput(bool enable)
{
    static bool enabled = true;
    static struct termios oldt;
    struct termios newt;
    if (enable && !enabled)
    {
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        enabled = true;
    }
    else if (!enable && enabled)
    {
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        newt.c_cc[VMIN] = 1;
        newt.c_cc[VTIME] = 0;
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        enabled = false;
    }
}

// Delete Buffer Input
void flushInput()
{
    // Không cần flush trên Windows với _getch()
}

void flushInput()
{
    tcflush(STDIN_FILENO, TCIFLUSH);
}