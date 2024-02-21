#ifndef _NOISE_UTILS_
#define _NOISE_UTILS_

#ifdef _WIN32
#include <windows.h>
    bool get_key_state(){
        return GetAsyncKeyState(VK_ESCAPE) & 0x8000;
    }

    int check_console_window_resize_event(void);

#endif
#ifdef __linux__
#include <ncurses.h>
    bool get_key_state();

    int check_console_window_resize_event(void);
#endif

typedef struct {
    int depth;
    int height;
    int width;
} BoxDim;

double map(double value, double fromMin, double fromMax, double toMin, double toMax);

float clamp(float val);

#endif