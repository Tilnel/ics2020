#include <stdio.h>
#include <unistd.h>
#include <NDL.h>

int main() {
    int sec = 1;
    int j = 0;
    while (1) {
        if (j == 100000) {
            int now = NDL_GetTicks() / 1000000;
            if (sec != now)
                printf("DIO: %d seconds passed.\n", now);
            sec = now;
            j = 0;
        } else
            j++;
    }
    return 0;
}
