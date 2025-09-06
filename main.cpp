#include <iostream>
#include <vector>
#include <ctime>
#include <cctype>

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#else
#include <termios.h> // cho tcflush, TCIFLUSH
#include <unistd.h>  // cho STDIN_FILENO
#include <fcntl.h>
#include <sys/time.h>
#include <sys/types.h>
#endif

#include <stdio.h>

using namespace std;

#define WIDTH 40
#define HEIGHT 20
int score = 0;

/*
 * Enum
 */
enum class Direction
{
    up,
    right,
    down,
    left
};

// Each point is a part of the snake
struct Point
{
    int x;
    int y;
};
std::vector<Point> snake = {
    {WIDTH / 2 + 2, HEIGHT / 2},
    {WIDTH / 2 + 1, HEIGHT / 2},
    {WIDTH / 2, HEIGHT / 2},
    {WIDTH / 2 - 1, HEIGHT / 2},
    {WIDTH / 2 - 2, HEIGHT / 2}};
Direction direction = Direction::right;
Point prevTail;

void displayScore();
void setBufferedInput(bool);
void resetSnake();
void showEndMenu();
void startGame();
void move();
void drawBox();
void drawHeadnTail();
#ifdef _WIN32
// Windows-specific implementations
void clearSnake();
void drawSnakePart(Point);
void drawSnake();
void genApple();

void ShowConsoleCursor(bool showFlag)
{
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
    cursorInfo.bVisible = showFlag;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}
void setBufferedInput(bool enable)
{
    // Không cần cài đặt trên Windows với _getch()
}
void flushInput()
{
    // Không cần flush trên Windows với _getch()
}

#else
void ShowConsoleCursor(bool showFlag)
{
    if (showFlag)
        printf("\e[?25h");
    else
        printf("\e[?25l");
    fflush(stdout);
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
    tcflush(STDIN_FILENO, TCIFLUSH);
}
#endif

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

void drawBox()
{
    clearScreen();
    for (size_t i = 0; i <= WIDTH; i++)
    {
        gotoxy(i, 0);
        std::cout << '=' << std::flush;
        gotoxy(i, HEIGHT);
        std::cout << '=' << std::flush;
    }
    for (size_t i = 1; i < HEIGHT; i++)
    {
        gotoxy(0, i);
        std::cout << '|' << std::flush;
        gotoxy(WIDTH, i);
        std::cout << '|' << std::flush;
    }
}

void gotoxy(int x, int y)
{
    printf("\033[%d;%dH", y + 1, x + 1);
    fflush(stdout);
}

// Move snake function
void move()

void drawSnakePart(Point p)
{
    gotoxy(p.x, p.y);
    std::cout << BODY << std::flush;
}

void setBufferedInput(bool enable)
{
    Point newHead = snake[0];
    switch (direction)
    {
    case Direction::up:
        newHead.y--;
        break;
    case Direction::down:
        newHead.y++;
        break;
    case Direction::left:
        newHead.x--;
        break;
    case Direction::right:
        newHead.x++;
        break;
    }
    prevTail = snake.back();
    snake.insert(snake.begin(), newHead);
    snake.pop_back();
}

// Show end menu section
void clearScreen()
{
    system("cls");
}
void showEndMenu()
{
    clearScreen();
    gotoxy(0, 0);
    std::cout << "End game!" << std::endl;
    std::cout << "Your score: " << score << std::endl;
    std::cout << "Do you want to play again ([y]/[n]): " << std::flush;
    setBufferedInput(true);
    flushInput();
    char option;
    std::cin >> option;
    option = tolower(option);
    setBufferedInput(false);
    if (option == 'y')
    {
        // reset lại rắn và start game
    }
    else
    {
        setBufferedInput(true);
        clearScreen();
        exit(0);
    }
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

void clearSnake()
{
    for (size_t i = 0; i < snake.size(); i++)
    {
        gotoxy(snake[i].x, snake[i].y);
        std::cout << ' ' << std::flush;
    }
}

void showStartMenu()
{
    clearScreen();
    std::cout << "Welcome to snake game!" << std::endl;
    std::cout << "\nControl the snake to eat the apples (O) that appear on the screen\nAvoid crashing into the walls or your own body — doing so will end the game.\nHow long can you survive? Let's find out!" << std::endl;
    std::cout << "\n\nOptions:" << std::endl;
    std::cout << "1. Start" << std::endl;
    std::cout << "2. Quit" << std::endl;
    std::cout << "Your choice: " << std::flush;
    setBufferedInput(true);
    flushInput(); // xoá input cũ
    int option;
    std::cin >> option;
    flushInput(); // xoá \n còn lại
    setBufferedInput(false);
    if (option == 1)
    {
        clearScreen();
        std::cout << "Choose your level (1 - 5): " << std::flush;
        int t;
        std::cin >> t;
        if (t < 1 || t > 5)
            t = 1;
        speed = 300 - t * 50;
        clearScreen();
        std::cout << "Tip: While playing game, you can press 'q' to quit" << std::endl;
        gotoxy(0, 3);
        std::cout << "Ready!" << std::flush;
#ifdef _WIN32
        Sleep(1000);
#else
        usleep(1000000);
#endif
        for (size_t i = 3; i > 0; i--)
        {
            gotoxy(0, 3);
            std::cout << i << "         " << std::flush;
#ifdef _WIN32
            Sleep(1000);
#else
            usleep(1000000);
#endif
        }
        gotoxy(0, 3);
        std::cout << "GO!" << std::flush;
#ifdef _WIN32
        Sleep(1000);
#else
        usleep(1000000);
#endif
        startGame();
    }
    else
    {
        setBufferedInput(true);
        clearScreen();
        exit(0);
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

void genApple() {
    int x, y;
    bool validPosition;
    do {
        validPosition = true;  // Assume position is valid until proven otherwise
        x = rand() % (WIDTH - 2) + 1;  // Random x-coordinate within bounds
        y = rand() % (HEIGHT - 2) + 1; // Random y-coordinate within bounds
        for (size_t i = 0; i < snake.size(); i++) {
            if (snake[i].x == x && snake[i].y == y) {
                validPosition = false;  // Position overlaps with snake
                break;
            }
        }
    } while (!validPosition);  // Repeat until a valid position is found
    apple.x = x;
    apple.y = y;
}

void drawHeadnTail()
{
    gotoxy(snake[0].x, snake[0].y);
    char headChar;
    switch (direction)
    {
    case Direction::up:
        headChar = '^';
        break;
    case Direction::down:
        headChar = 'v';
        break;
    case Direction::left:
        headChar = '<';
        break;
    case Direction::right:
        headChar = '>';
        break;
    }
    std::cout << headChar << std::flush;

    if (snake.size() > 1)
    {
        gotoxy(snake[1].x, snake[1].y);
        std::cout << '*' << std::flush;
    }

    gotoxy(prevTail.x, prevTail.y);
    std::cout << ' ' << std::flush;
}

bool isBiteItself()
{
    Point head = snake[0];
    for (size_t i = 1; i < snake.size(); i++)
        if (head.x == snake[i].x && head.y == snake[i].y)
            return true;
    return false;
}