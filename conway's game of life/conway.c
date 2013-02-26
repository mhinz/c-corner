/*
 * conway.c by Marco Hinz
 * 
 * Simple implementation of Conway's Game of Life.
 */

#define _BSD_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define ROWS  40U
#define COLS  40U
#define GENS  100U

enum { DEAD, ALIVE };

static unsigned x;
static unsigned y;
static unsigned **world;
static unsigned **world_tmp;

static void __attribute__((noreturn)) error(char *msg)
{
    fprintf(stderr, "%s\n", msg);
    exit(EXIT_FAILURE);
}

static void genesis(void)
{
    world = malloc(ROWS * sizeof (int *));
    if (!world)
        error("out of memory");
    world_tmp = malloc(ROWS * sizeof (int *));
    if (!world_tmp)
        error("out of memory");

    for (y = 0; y < ROWS; ++y) {
        *(world + y) = malloc(COLS * sizeof (int));
        if (!*(world + y))
            error("out of memory");
        *(world_tmp + y) = malloc(COLS * sizeof (int));
        if (!*(world_tmp + y))
            error("out of memory");
    }
    for (y = 0; y < ROWS; ++y)
        for (x = 0; x < COLS; ++x)
            *(*(world + y) + x) = DEAD;
}

static void extinction(void)
{
    for (y = 0; y < ROWS; ++y) {
        free(*(world_tmp + y));
        free(*(world     + y));
    }
    free(world_tmp);
    free(world);
}

static inline void evolution(void)
{
    int sum = 0;

    for (y = 0; y < ROWS; ++y) {
        for (x = 0; x < COLS; ++x) {
            sum =
                *(*(world + ((y-1+ROWS) % ROWS)) + ((x-1+COLS) % COLS)) +
                *(*(world + ((y-1+ROWS) % ROWS)) + ((x+  COLS) % COLS)) +
                *(*(world + ((y-1+ROWS) % ROWS)) + ((x+1+COLS) % COLS)) +
                *(*(world + ((y  +ROWS) % ROWS)) + ((x-1+COLS) % COLS)) +
                *(*(world + ((y  +ROWS) % ROWS)) + ((x+1)      % COLS)) +
                *(*(world + ((y+1+ROWS) % ROWS)) + ((x-1+COLS) % COLS)) +
                *(*(world + ((y+1+ROWS) % ROWS)) + ((x+  COLS) % COLS)) +
                *(*(world + ((y+1+ROWS) % ROWS)) + ((x+1+COLS) % COLS));

            if (*(*(world + y) + x) == ALIVE && (sum < 2 || sum > 3))
                *(*(world_tmp + y) + x) = DEAD;
            else if (*(*(world + y) + x) == DEAD && sum == 3)
                *(*(world_tmp + y) + x) = ALIVE;
            else
                *(*(world_tmp + y) + x) = *(*(world + y) + x);
        }
    }
    unsigned **tmp = world;
    world = world_tmp;
    world_tmp = tmp;

    for (y = 0; y < ROWS; y++) {
        puts("");
        for (x = 0; x < COLS; x++) {
            printf("%c ", *(*(world + y) + x) ? '#' : '.');
        }
    }
}

void great_selection(void)
{
    printf("Choose a pattern:\n"
            "  0) R-pentomino\n"
            "  1) Beehive\n"
            "  2) Glider\n"
            "> ");

    char input;
    char *ret = fgets(&input, 2, stdin);
    if (!ret)
        error("fgets failed");
    int num;
    int check = sscanf(&input, "%1d", &num);

    if (feof(stdin) || ferror(stdin))
        error("sscanf returned EOF");
    else if (check != 1)
        error("sscanf() failed to return exactly one item!");

    switch (num) {
        case 0:
            // R-pentomino
            *(*(world + 14) + 15) = ALIVE;
            *(*(world + 14) + 16) = ALIVE;
            *(*(world + 15) + 14) = ALIVE;
            *(*(world + 15) + 15) = ALIVE;
            *(*(world + 16) + 15) = ALIVE;
            break;
        case 1:
            // Beehive
            *(*(world + 18) + 16) = ALIVE;
            *(*(world + 18) + 17) = ALIVE;
            *(*(world + 19) + 18) = ALIVE;
            *(*(world + 20) + 19) = ALIVE;
            *(*(world + 21) + 19) = ALIVE;
            *(*(world + 22) + 19) = ALIVE;
            *(*(world + 23) + 18) = ALIVE;
            *(*(world + 24) + 17) = ALIVE;
            *(*(world + 24) + 16) = ALIVE;
            break;
        case 2:
            // Glider
            *(*(world + 3) + 6) = ALIVE;
            *(*(world + 4) + 4) = ALIVE;
            *(*(world + 4) + 6) = ALIVE;
            *(*(world + 5) + 5) = ALIVE;
            *(*(world + 5) + 6) = ALIVE;
            break;
        default:
            error("Incorrect input");
    }
}

int main(void)
{
    const unsigned int usecs = 100000;

    genesis();
    great_selection();

    for (unsigned i = 1; i <= GENS; i++) {
        if (usleep(usecs) == -1)
            error("usleep failed");
        evolution();
        printf("Generation: %u\n", i);
    }

    extinction();
    puts("Done.");
}
