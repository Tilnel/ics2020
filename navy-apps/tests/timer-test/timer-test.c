#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

int main() {
    struct timeval tv;
    struct timezone tz;
    int sec = 1;
    int j = 0;
    while (1) {
        if (j == 100000) {
            gettimeofday(&tv, &tz);
            if (sec != tv.tv_sec)
                printf("DIO: %d %d seconds passed.\n", sec, tv.tv_sec);
            sec = tv.tv_sec;
            j = 0;
        } else
            j++;
    }
    return 0;
}
