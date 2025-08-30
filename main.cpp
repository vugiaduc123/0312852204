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
void clearSnake();
void drawSnakePart(Point);

void ShowConsoleCursor(bool showFlag)
{
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
    cursorInfo.bVisible = showFlag;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}
bool kbhit()
{
    struct termios oldt, newt;
    struct timeval tv = {0, 50000}; // Giảm timeout xuống 50ms
    fd_set fds;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    newt.c_cc[VMIN] = 0;
    newt.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    int result = select(STDIN_FILENO + 1, &fds, nullptr, nullptr, &tv);
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return result > 0 && FD_ISSET(STDIN_FILENO, &fds);
}

char getch()
{
    char c;
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    newt.c_cc[VMIN] = 1;
    newt.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    if (read(STDIN_FILENO, &c, 1) < 0)
        c = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return c;
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

void drawSnakePart(Point p)
{
    gotoxy(p.x, p.y);
    std::cout << BODY << std::flush;
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

void clearSnake()
{
    for (size_t i = 0; i < snake.size(); i++)
    {
        gotoxy(snake[i].x, snake[i].y);
        std::cout << ' ' << std::flush;
    }
}
void drawSnake()
{
    for (size_t i = 0; i < snake.size(); i++)
    {
        if (i == 0)
        {
            gotoxy(snake[0].x, snake[0].y);
            char headChar;
            switch (direction)
            {
                case Direction::up: headChar = '^'; break;
                case Direction::down: headChar = 'v'; break;
                case Direction::left: headChar = '<'; break;
                case Direction::right: headChar = '>'; break;
            }
            std::cout << headChar << std::flush;
        }
        else
        {
            drawSnakePart(snake[i]);
        }
    }
}