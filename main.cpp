#include <iostream>
#include <windows.h>
#include <termios.h> // cho tcflush, TCIFLUSH
#include <unistd.h>  // cho STDIN_FILENO
#include <cstdlib>
#include <conio.h>
#include <time.h>

using namespace std;
void displayScore();
void setBufferedInput(bool);
void resetSnake();

void ShowConsoleCursor(bool showFlag)
{
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
    cursorInfo.bVisible = showFlag;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

int main()
{
    setBufferedInput(false);
    showStartMenu();
    setBufferedInput(true);
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

void displayScore()
{
    gotoxy(WIDTH + 5, 2);
    std::cout << "Your score: " << score << std::flush;
    gotoxy(WIDTH + 5, 8);
    std::cout << "Up: w        " << std::flush;
    gotoxy(WIDTH + 5, 9);
    std::cout << "Right: d     " << std::flush;
    gotoxy(WIDTH + 5, 10);
    std::cout << "Down: s      " << std::flush;
    gotoxy(WIDTH + 5, 11);
    std::cout << "Left: a      " << std::flush;
    gotoxy(WIDTH + 5, 14);
    std::cout << "Note: Press q to quit" << std::flush;
}

void resetSnake()
{
    score = 0;
    direction = Direction::right;
    snake.clear();
    snake.push_back({ WIDTH / 2 + 2, HEIGHT / 2 });
    snake.push_back({ WIDTH / 2 + 1, HEIGHT / 2 });
    snake.push_back({ WIDTH / 2, HEIGHT / 2 });
    snake.push_back({ WIDTH / 2 - 1, HEIGHT / 2 });
    snake.push_back({ WIDTH / 2 - 2, HEIGHT / 2 });
}