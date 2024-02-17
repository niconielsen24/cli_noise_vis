#ifndef _CONSOLE_UTILS_
#define _CONSOLE_UTILS_

#include <windows.h>

void clearConsole();

void setCursorInvis(HANDLE hConsole);

int box(HANDLE hConsole, int left, int top, int width, int height);

#endif