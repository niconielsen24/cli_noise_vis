#ifndef _NOISE_UTILS_
#define _NOISE_UTILS_

#include <windows.h>

double map(double value, double fromMin, double fromMax, double toMin, double toMax);

COORD calculateCenter(int consoleWidth, int consoleHeight, int bufferWidth, int bufferHeight);

float clamp(float val);

#endif