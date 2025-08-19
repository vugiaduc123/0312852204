#include <iostream>
#include <windows.h>
#include <cstdlib>
#include <conio.h>
#include <time.h>

using namespace std;

int main()
{
    return 0;
}

void gotoxy(int x, int y)
{
    printf("\033[%d;%dH", y + 1, x + 1);
    fflush(stdout);
}