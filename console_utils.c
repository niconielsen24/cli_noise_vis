#include <windows.h>

void clearConsole(){
    system("cls");
}

void setCursorInvis(HANDLE hConsole){
    CONSOLE_CURSOR_INFO consoleCursorInfo;
    GetConsoleCursorInfo(hConsole, &consoleCursorInfo);
    consoleCursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole, &consoleCursorInfo);
}

int box(HANDLE hConsole, int left, int top, int width, int height) {

    COORD topleft = {left, top};
    DWORD written;

    FillConsoleOutputCharacterA(hConsole, '-', width, topleft, &written);

    topleft.Y += height - 1;

    FillConsoleOutputCharacterA(hConsole, '-', width, topleft, &written);

    for (int i = 1; i < height - 1; ++i) {
        topleft.Y = top + i;
        topleft.X = left;
        FillConsoleOutputCharacterA(hConsole, '|', 1, topleft, &written);
        topleft.X = left + width - 1;
        FillConsoleOutputCharacterA(hConsole, '|', 1, topleft, &written);
    }

    topleft.X = left;
    topleft.Y = top;
    FillConsoleOutputCharacterA(hConsole, '+', 1, topleft, &written);
    topleft.X = left + width - 1;
    FillConsoleOutputCharacterA(hConsole, '+', 1, topleft, &written);
    topleft.Y = top + height - 1;
    FillConsoleOutputCharacterA(hConsole, '+', 1, topleft, &written);
    topleft.X = left;
    FillConsoleOutputCharacterA(hConsole, '+', 1, topleft, &written);

    return EXIT_SUCCESS;
}