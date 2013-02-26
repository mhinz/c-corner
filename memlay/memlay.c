/*
 * Program shows its process image's memory layout.
 *
 * Since this is meant for learning purposes, I don't care for UB, memory leaks
 * or unsafe functions. I'll point them out in the comments though.
 *
 * Compatible to C89.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COLS  80
#define ROWS  6

int bss[50000];
int data[10000] = { 5 };
char *rodata = "read-only data";

int main(void)
{
    int stack = ROWS;
    char sections[ROWS][COLS];

    /*
     * ISO C forbids casting between object and function pointers.
     * We do it nevertheless.
     */
    sprintf(sections[0], "%p  %s", (void *)main,   "text"  );
    sprintf(sections[1], "%p  %s", malloc(1),      "heap"  );  /* Leak. */
    sprintf(sections[2], "%p  %s", rodata,         "rodata");
    sprintf(sections[3], "%p  %s", (void *)data,   "data"  );
    sprintf(sections[4], "%p  %s", (void *)bss,    "bss"   );
    sprintf(sections[5], "%p  %s", (void *)&stack, "stack" );
    /* We should use snprintf instead. */

    /*
     * Invoking UD like a boss: "A pointer is used to call a function whose
     * type is not compatible with the pointed-to type." (6.3.2.3)
     */
    qsort(sections, ROWS, COLS, (int (*)(const void *, const void *))strcmp);

    /* The "goes to" operator, hoho! */
    while (stack --> 0)
        puts(sections[stack]);

    return 0;
}
