#include <windows.h>

double map(double value, double fromMin, double fromMax, double toMin, double toMax) {
    return (value - fromMin) * (toMax - toMin) / (fromMax - fromMin) + toMin;
}

float clamp(float val) {
    return min(max(-1.0, val),1.0);
}