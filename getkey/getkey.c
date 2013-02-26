/*-----------------------------------------*/
/*--  Abusing the terminal's "raw mode"  --*/
/*-----------------------------------------*/

#define _POSIX_SOURCE
#define _BSD_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <time.h>


static inline int getkey(void)
{
    int             ret;
    int             key;
    struct termios  orig_term;
    struct termios  new_term;

    ret = tcgetattr(STDIN_FILENO, &orig_term);
    if (ret == -1) {
        perror("tcgetattr");
        exit(EXIT_FAILURE);
    }

    memcpy(&new_term, &orig_term, sizeof (struct termios));

    new_term.c_lflag     &= ~(ECHO | ICANON);
    new_term.c_cc[VTIME]  = 0;
    new_term.c_cc[VMIN]   = 0;

    ret = tcsetattr(STDIN_FILENO, TCSANOW, &new_term);
    if (ret == -1) {
        perror("tcsetattr");
        exit(EXIT_FAILURE);
    }

    /* Of course we can't test for feof anymore. */
    key = fgetc(stdin);
    if (ferror(stdin)) {
        fprintf(stderr, "The error indicator for stdin was set!\n");
        exit(EXIT_FAILURE);
    }

    ret = tcsetattr(STDIN_FILENO, TCSANOW, &orig_term);
    if (ret == -1) {
        perror("tcsetattr");
        exit(EXIT_FAILURE);
    }

    return key;
}

int main(void)
{
    int key;
    int count = 1;

    srand(time(NULL));

    while (1) {
        key = getkey();
        /* ESC, q, EOT */
        if (key == 0x1b || key == 0x71 || key == 0x04)
            break;
        else {
            key = rand() % 0x7f;
            printf("%c", ((key < 0x21) ? (key + 0x21) : key));
        }

        count %= 60;
        if (!count++)
            puts("");

        usleep(25000);
    }

    return EXIT_SUCCESS;
}
