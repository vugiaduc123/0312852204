#include <iostream>
#include <windows.h>
#include <termios.h> // cho tcflush, TCIFLUSH
#include <unistd.h>  // cho STDIN_FILENO
#include <cstdlib>
#include <conio.h>
#include <time.h>

using namespace std;

int main()
{
    return 0;
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