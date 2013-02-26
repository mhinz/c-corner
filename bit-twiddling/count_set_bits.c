#include <stdio.h>
#include <inttypes.h>

int main(void)
{
    uint32_t num = 0x32;
    uint32_t res;

    for (res = 0; num; num >>= 1)
        res += (num & 1);

    printf("%" PRIu32 "\n", res);

    return 0;
}
