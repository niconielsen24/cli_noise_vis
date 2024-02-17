#include <windows.h>

double map(double value, double fromMin, double fromMax, double toMin, double toMax) {
    return (value - fromMin) * (toMax - toMin) / (fromMax - fromMin) + toMin;
}

COORD calculateCenter(int consoleWidth, int consoleHeight, int bufferWidth, int bufferHeight) {
    COORD center;
    center.X = (consoleWidth - bufferWidth) / 2;
    center.Y = (consoleHeight - bufferHeight) / 2;
    return center;
}

float clamp(float val) {
    return min(max(-1.0, val),1.0);
}