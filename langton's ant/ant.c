/*
 * ant.c by Marco Hinz <mhinz@spline.de>
 *
 * Simple implementation of Langton's Ant.
 */

#define _BSD_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define ROWS  50
#define COLS  50
#define GENS  15000
#define MSECS 25000

enum { WHITE, BLACK };
enum { LEFT, RIGHT, UP, DOWN };

static char world[ROWS][COLS];

typedef struct movement {
    int  x;
    int  y;
    char dir;
} mm;

static inline void step(mm *cur)
{
    switch (world[cur->y][cur->x]) {
        case WHITE:
            world[cur->y][cur->x] = BLACK;
            switch (cur->dir) {
                case LEFT:
                    cur->y--;
                    cur->dir = UP;
                    break;
                case RIGHT:
                    cur->y++;
                    cur->dir = DOWN;
                    break;
                case UP:
                    cur->x++;
                    cur->dir = RIGHT;
                    break;
                case DOWN:
                    cur->x--;
                    cur->dir = LEFT;
                    break;
            }
            break;
        case BLACK:
            world[cur->y][cur->x] = WHITE;
            switch (cur->dir) {
                case LEFT:
                    cur->y++;
                    cur->dir = DOWN;
                    break;
                case RIGHT:
                    cur->y--;
                    cur->dir = UP;
                    break;
                case UP:
                    cur->x--;
                    cur->dir = LEFT;
                    break;
                case DOWN:
                    cur->x++;
                    cur->dir = RIGHT;
            }
    }

    for (size_t y = 0; y < ROWS; y++) {
        puts("");
        for (size_t x = 0; x < COLS; x++)
            if (cur->y == y && cur->x == x)
                printf("%c ", '@');
            else
                printf("%c ", (world[y][x]) ? '=' : '.');
    }
}

int main(void)
{
    mm cell = {
        .x   = 20,
        .y   = 20,
        .dir = RIGHT,
    };

    for (size_t i = 0; i < GENS; i++) {
        if (usleep(MSECS) == -1) {
            fprintf(stderr, "usleep failed");
            exit(1);
        }
        step(&cell);
        printf("Step: %d\n", i);
    }

    puts("Done.");
}
