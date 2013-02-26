#include <stdio.h>
#include <inttypes.h>
#include <limits.h>

int main(void)
{
    const int32_t  num  = -40;
    const int32_t  mask = num >> (sizeof (int) * CHAR_BIT - 1);
    uint32_t res;

    res = (num + mask) ^ mask;

    printf("The absolute value of %" PRId32 " is %" PRIu32 ".\n", num, res);

    return 0;
}
