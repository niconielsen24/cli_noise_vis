#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

#include "rogueutil.h" // copied from "https://github.com/sakhmatd/rogueutil"
#include "open-simplex-noise.h" // copied from "https://github.com/smcameron/open-simplex-noise-in-c"
#include "utils.h"

int main(int argc, char *argv[]) {

    int noise_ctx_seed = 0;
    float size_factor  = 0;
    if (argc == 3)
    {
        noise_ctx_seed = strtol(argv[1], NULL, 10);
        size_factor    = strtof(argv[2],NULL);
    } else {
        noise_ctx_seed = 12345;
        size_factor    = 0.7;
    }

    double noise_val;
    bool should_continue = true;
    char * density = "@$B86G4HTU#?l!*;:+<>=^~-,.`        ";

    int height  = (int)(trows() * size_factor);
    int depth   = 500;
    int width   = (int)(tcols() * size_factor); 

    int center_y = (trows() - height) / 2; 
    int center_x = (tcols() - width)  / 2; 

    char grid[depth][height * (width + center_x)];

    struct osn_context *ctx;
    open_simplex_noise(noise_ctx_seed, &ctx);

    for (int i = 0; i < depth; i++)
    {
        for (int j = 0; j < height; j++)
        {
            for (int z = 0; z < (width + center_x); z++)
            {
                noise_val = open_simplex_noise3(ctx,(double) (i+0.0001)/100,(double) (j+0.0001)/100,(double) (z+0.0001)/100);
                if (z < center_x)   grid[i][j * (width + center_x) + z] = ' ';
                if (z >= center_x)  grid[i][j * (width + center_x) + z] = density[(int)map(noise_val,-1.0,1.0,0.0,(double) strlen(density))];;
                if (z == center_x || z == center_x + width - 2) grid[i][j * (width + center_x) + z] = '|';
                if (j == 0 && z >= center_x)                    grid[i][j * (width + center_x) + z] = '-';
                if (j == height - 1 && z >= center_x)           grid[i][j * (width + center_x) + z] = '-';
                if (
                    (j == height - 1 && z == center_x)              ||
                    (j == height - 1 && z == width + center_x - 2)  ||
                    (j == 0 && z == center_x)                       ||
                    (j == 0 && z == width + center_x - 2)
                    )                                           grid[i][j * (width + center_x) + z] = '+';
            }
            grid[i][(j == 0) ? width + center_x - 1 : (j * (width + center_x)) - 1] = '\n';
        }
        grid[i][(height * (width + center_x)) - 1] = '\0';
    }

    while (should_continue){
        for (int i = 0; i < depth; i++)
        {
            cls();
            for (int k = 0; k < center_y; k++) printf("%c",'\n');
            printf("%s",grid[i]);
            if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
                should_continue = false;
                break;
            }
            Sleep(30);
        }
    }

    cls();
    open_simplex_noise_free(ctx);

    return EXIT_SUCCESS;
}