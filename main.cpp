#include <iostream>
#include <windows.h>
#include <cstdlib>
#include <conio.h>
#include <time.h>

using namespace std;

void setBufferedInput(bool);

int main()
{
    return 0;
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