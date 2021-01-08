#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>

int main() {
  struct timeval *tv;
  struct timezone *tz;
  int sec = 0;
  while (1) {
	gettimeofday(tv, tz);
	if (sec != tv->tv_sec) printf("It's the %d's second.\n", sec); 
	sec = tv->tv_sec;
  }
  return 0;
}
