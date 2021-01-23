#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <assert.h>
// int _read(int fd, void *buf, size_t count);
int _open(const char* name, int flags, int mode);

static int evtdev = -1;
static int fbdev = -1;
static int screen_w = 0, screen_h = 0;

uint32_t NDL_GetTicks() {
  struct timeval tv;
  struct timezone tz;
  gettimeofday(&tv, &tz);  
  return tv.tv_usec / 1000 + 1000 * tv.tv_sec;
}

int NDL_PollEvent(char *buf, int len) {
  int fd = _open("/dev/events", 0, 0);
  return read(fd, buf, len);
}

void NDL_OpenCanvas(int *w, int *h) {
  if (getenv("NWM_APP")) {
    int fbctl = 4;
    fbdev = 5;
    screen_w = *w; screen_h = *h;
    char buf[64];
    int len = sprintf(buf, "%d %d", screen_w, screen_h);
    // let NWM resize the window and create the frame buffer
    write(fbctl, buf, len);
    while (1) {
      // 3 = evtdev
      int nread = read(3, buf, sizeof(buf) - 1);
      if (nread <= 0) continue;
      buf[nread] = '\0';
      if (strcmp(buf, "mmap ok") == 0) break;
    }
    close(fbctl);
  } else {
    if (*w == 0 && *h == 0) {
      *w = 640; *h = 480;
    }
    screen_w = *w;
    screen_h = *h;
    char buf[64];
    sprintf(buf, "WIDTH: %d\nHEIGHT: %d\n", *w, *h);
    write(4, buf, 64);
  }
}


void NDL_DrawRect(uint32_t *pixels, int x, int y, int w, int h) {
int y_off = (300 - screen_h) / 2;
int x_off = (400 - screen_w) / 2;
  int fd = _open("/dev/fb", 0, 0);
  if (w + x > screen_w) w = screen_w - x;
  if (h + y > screen_h) h = screen_h - y;
  for (int i = 0; i < h; i++) {
    lseek(fd, ((y_off + y + i) * screen_w + x_off + x) * 4, 0);
    write(fd, pixels + i * w, w * 4);
  }
}

void NDL_OpenAudio(int freq, int channels, int samples) {
}

void NDL_CloseAudio() {
}

int NDL_PlayAudio(void *buf, int len) {
  return 0;
}

int NDL_QueryAudio() {
  return 0;
}

int NDL_Init(uint32_t flags) {
  if (getenv("NWM_APP")) {
    evtdev = 3;
  }
  return 0;
}

void NDL_Quit() {
}
