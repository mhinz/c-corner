#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>

int main(void)
{
    uint32_t num;
    bool     res;

    num = 4096;
    res = !(num & (num - 1));
    printf("Is %" PRIu32 " a power of 2?  %s\n", num, res ? "Yes" : "No");

    num = 3986;
    res = !(num & (num - 1));
    printf("Is %" PRIu32 " a power of 2?  %s\n", num, res ? "Yes" : "No");

    num = 0;
    res = !(num & (num - 1));
    printf("Is %" PRIu32 " a power of 2?  %s\n", num, res ? "Yes" : "No");

    // Wait, what?  0 is no power of 2!
    res = num && !(num & (num - 1));
    printf("Is %" PRIu32 " a power of 2?  %s\n", num, res ? "Yes" : "No");

    return 0;
}
