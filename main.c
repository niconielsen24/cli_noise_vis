#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

#include "console_utils.h"
#include "open-simplex-noise.h" // copied from "https://github.com/smcameron/open-simplex-noise-in-c"
#include "utils.h"  

float clamp(float val) {
    return min(max(-1.0, val),1.0);
}

int main(int argc, char *argv[]) {

    int noise_ctx_seed = 12345;
    float size_factor = 0.7;
    if (argc == 3)
    {
        noise_ctx_seed = strtol(argv[1], NULL, 10);
        size_factor = strtof(argv[2],NULL);
    }

    clearConsole();
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hConsole == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "Error while creating console handle\n");
        return EXIT_FAILURE;
    }

    CONSOLE_SCREEN_BUFFER_INFO console_screen_buffer_info;
    if (!GetConsoleScreenBufferInfo(hConsole,&console_screen_buffer_info)){
        fprintf(stderr, "Failed to get console screen buffer information\n");
        return EXIT_FAILURE;
    }

    int height = console_screen_buffer_info.dwSize.Y * size_factor;
    int depth = 1000;
    int width = console_screen_buffer_info.dwSize.X * size_factor; 
    char * density = "*===<>++++***---... ";

    CHAR_INFO (*info_buffer)[height * width] = malloc(sizeof(CHAR_INFO[depth][height * width]));
    if (info_buffer == NULL) {
        fprintf(stderr, "Failed to allocate buffer");
        return EXIT_FAILURE;
    }

    struct osn_context *ctx;
    open_simplex_noise(noise_ctx_seed ,&ctx);

    double noise_val;

    for (int i = 0; i < depth; i++)
    {
        for (int j = 0; j < height; j++)
        {
            for (int z = 0; z < width; z++)
            {
                noise_val = open_simplex_noise3(ctx,(double) (i+0.0001)/100,(double) (j+0.0001)/100,(double) (z+0.0001)/100);
                info_buffer[i][(j * width) + z].Char.AsciiChar = density[(int)map(noise_val,-1.0,1.0,0.0,(double) strlen(density))];
                info_buffer[i][(j * width) + z].Attributes = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
            }
        }
    }

    COORD bufferSize = { width, height };
    COORD bufferCoord = { 0, 0 };
    COORD center = calculateCenter(console_screen_buffer_info.dwSize.X, console_screen_buffer_info.dwSize.Y, width, height);
    SMALL_RECT writeRegion = { center.X, center.Y, center.X + width - 1, center.Y + height - 1 };
    setCursorInvis(hConsole);

    bool should_continue = true;
    while (should_continue){
        for (int i = 0; i < depth; i++)
        {
            clearConsole();
            box(hConsole,center.X-1,center.Y-1,width+2,height+2);
            WriteConsoleOutput(hConsole, info_buffer[i], bufferSize, bufferCoord, &writeRegion);
            if (GetAsyncKeyState(VK_ESCAPE) & 0x01) should_continue = !should_continue;
            Sleep(30);
        }
    }

    open_simplex_noise_free(ctx);
    free(info_buffer);

    return EXIT_SUCCESS;
}