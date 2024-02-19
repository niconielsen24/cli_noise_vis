#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include <limits.h>

#include "rogueutil.h"          // cloned from "https://github.com/sakhmatd/rogueutil"
#include "open-simplex-noise.h" // cloned from "https://github.com/smcameron/open-simplex-noise-in-c"
#include "utils.h"

int main(int argc, char *argv[]) {

    int noise_ctx_seed = 0;
    float size_factor  = 0;
    setCursorVisibility(0);
    
    if (argc == 3)
    {
        const char * nptr = argv[1];
        char * endptr = NULL;
        errno = 0;
        noise_ctx_seed = strtol(nptr, &endptr, 10);
        if (nptr == endptr) {
            fprintf(stderr, " Seed : %i  invalid  (no digits found, 0 returned)\n", noise_ctx_seed);
            return EXIT_FAILURE;
        } else if (errno == ERANGE && noise_ctx_seed == INT_MIN) {
            fprintf(stderr, " Seed : %i  invalid  (underflow occurred)\n", noise_ctx_seed);
            return EXIT_FAILURE;
        } else if (errno == ERANGE && noise_ctx_seed == INT_MAX) {
            fprintf(stderr, " Seed : %i  invalid  (overflow occurred)\n", noise_ctx_seed);
            return EXIT_FAILURE;
        } else if (errno != 0 && noise_ctx_seed == 0) {
            fprintf(stderr, " Seed : %i  invalid  (unspecified error occurred)\n", noise_ctx_seed);        
            return EXIT_FAILURE;
        }

        const char * nptr_1 = argv[2];
        char * endptr_1 = NULL;
        errno = 0;
        size_factor = strtof(nptr_1, &endptr_1);
        if (nptr_1 == endptr_1) {
            fprintf(stderr,  " Size factor : %f  invalid  (no digits found, 0 returned)\n", size_factor);
            return EXIT_FAILURE;
        } else if (errno == ERANGE) {
            fprintf(stderr, " Size factor : %f  invalid  (overflow/underflow occurred)\n", size_factor);
            return EXIT_FAILURE;
        }
    } else {
        char *usage =  
        "\n\nUsage :"
        "\n\n   *Example run : 'noise.exe <int noise seed> <float size_factor>'"
        "\n    If no parameters are provided defaults will be used"
        "\n\n   *Default values :"
        "\n         Noise seed = 12345"
        "\n         Size facor = 0.7 (eg. 0.7 times width and height of the console size)"
        "\n\n   *In general a size factor of 0.7 is the max the stack can handle"
        "\n   *Escape key will exit the program"
        "\n\n       <<< Press any key to continue >>>";
        cls();
        printf("%s",usage);
        getkey();
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