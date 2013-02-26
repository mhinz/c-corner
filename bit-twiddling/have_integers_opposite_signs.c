#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>

/*
 *   00000000 00000000 00000000 00000100 (4)
 * ^ 00000000 00000000 00000000 00001000 (8)
 *   -----------------------------------
 *   00000000 00000000 00000000 00001100 (12)
 *
 *
 *   00000000 00000000 00000000 00000100 (4)
 * ^ 11111111 11111111 11111111 11111100 (-4)
 *   -----------------------------------
 *   11111111 11111111 11111111 11111000 (-8)
 */

int main(void)
{
    const int32_t a = 4;
    const int32_t b = 8;
    const int32_t c = -4;
    bool res;

    res = (a ^ b) < 0;
    printf("Do %" PRId32 " and %" PRId32 " have opposite signs?  %s\n", a, b, res ? "Yes" : "No");

    res = (a ^ c) < 0;
    printf("Do %" PRId32 " and %" PRId32 " have opposite signs?  %s\n", a, c, res ? "Yes" : "No");

    return 0;
}
