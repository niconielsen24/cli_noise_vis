#include <windows.h>

#ifdef __linux__
#include <unistd.h>

bool get_key_state() {
    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, TRUE);

    int ch = getch();
    endwin();

    return ch == 27;
}
#endif

double map(double value, double fromMin, double fromMax, double toMin, double toMax) {
    return (value - fromMin) * (toMax - toMin) / (fromMax - fromMin) + toMin;
}

float clamp(float val) {
    return min(max(-1.0, val),1.0);
}

