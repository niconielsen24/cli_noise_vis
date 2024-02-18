#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "rogueutil.h"          // cloned from "https://github.com/sakhmatd/rogueutil"
#include "open-simplex-noise.h" // cloned from "https://github.com/smcameron/open-simplex-noise-in-c"
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
    BoxDim box = {500, (int)(trows() * size_factor), (int)(tcols() * size_factor)};
    int center_y = (trows() - box.height) / 2; 
    int center_x = (tcols() - box.width)  / 2; 
    char grid[box.depth][box.height * (box.width + center_x)];
    struct osn_context *ctx;
    open_simplex_noise(noise_ctx_seed, &ctx);

    for (int i = 0; i < box.depth; i++)
    {
        for (int j = 0; j < box.height; j++)
        {
            for (int z = 0; z < (box.width + center_x); z++)
            {
                noise_val = open_simplex_noise3(ctx,(double) (i+0.0001)/100,(double) (j+0.0001)/100,(double) (z+0.0001)/100);
                if (z < center_x)   grid[i][j * (box.width + center_x) + z] = ' ';
                if (z >= center_x)  grid[i][j * (box.width + center_x) + z] = density[(int)map(noise_val,-1.0,1.0,0.0,(double) strlen(density))];;
                if (z == center_x || z == center_x + box.width - 2) grid[i][j * (box.width + center_x) + z] = '|';
                if (j == 0 && z >= center_x)                        grid[i][j * (box.width + center_x) + z] = '-';
                if (j == box.height - 1 && z >= center_x)           grid[i][j * (box.width + center_x) + z] = '-';
                if (
                    (j == box.height - 1 && z == center_x)                  ||
                    (j == box.height - 1 && z == box.width + center_x - 2)  ||
                    (j == 0 && z == center_x)                               ||
                    (j == 0 && z == box.width + center_x - 2)
                    )                                           grid[i][j * (box.width + center_x) + z] = '+';
            }
            grid[i][(j == 0) ? box.width + center_x - 1 : (j * (box.width + center_x)) - 1] = '\n';
        }
        grid[i][(box.height * (box.width + center_x)) - 1] = '\0';
    }

    while (should_continue){
        for (int i = 0; i < box.depth; i++)
        {
            cls();
            for (int k = 0; k < center_y; k++) printf("%c",'\n');
            printf("%s",grid[i]);
            if (get_key_state()) {
                should_continue = false;
                break;
            }
            msleep(20);
        }
    }

    cls();
    open_simplex_noise_free(ctx);
    return EXIT_SUCCESS;
}