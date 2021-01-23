#include <common.h>

#if defined(MULTIPROGRAM) && !defined(TIME_SHARING)
# define MULTIPROGRAM_YIELD() yield()
#else
# define MULTIPROGRAM_YIELD()
#endif

#define NAME(key) \
  [AM_KEY_##key] = #key,

static char dispinfo[256];
size_t fs_lseek(int fd, size_t offset, int whence);

static const char *keyname[256] __attribute__((used)) = {
  [AM_KEY_NONE] = "NONE",
  AM_KEYS(NAME)
};
AM_GPU_CONFIG_T gpuconf;

size_t serial_write(const void *buf, size_t offset, size_t len) {
  printf("before this\n");
  yield();
  printf("after this\n");
  for (int i = 0; i < len; i++) putch(((char *)buf)[i]); 
  return len;
}

size_t events_read(void *buf, size_t offset, size_t len) {
  AM_INPUT_KEYBRD_T ev = io_read(AM_INPUT_KEYBRD);
  if (ev.keycode == AM_KEY_NONE) { memset(buf, 0, len); return 0; }
  return sprintf(buf, "%s %s", ev.keydown ? "kd" : "ku", keyname[ev.keycode], len);
}

size_t dispinfo_read(void *buf, size_t offset, size_t len) {
  AM_GPU_CONFIG_T gpuconfig = io_read(AM_GPU_CONFIG);
  // assert(0);
  // printf("%s\n", dispinfo);
  sscanf(dispinfo, "WIDTH: %d\nHEIGHT: %d\n", &gpuconf.width, &gpuconf.height);
  return sprintf(buf, "WIDTH: %d\nHEIGHT: %d\n", &gpuconfig.width, &gpuconfig.height);
}

size_t dispinfo_write(const void *buf, size_t offset, size_t len) {
  strncpy(dispinfo, buf, len);
  return strlen(dispinfo);
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
  printf("before this\n");
  yield();
  printf("after this\n");
  sscanf(dispinfo, "WIDTH: %d\nHEIGHT: %d\n", &gpuconf.width, &gpuconf.height);
  // AM_GPU_CONFIG_T gpuconfig = io_read(AM_GPU_CONFIG);
  int w = gpuconf.width; 
  offset = fs_lseek(5, 0, 1);
  int y = offset / 4 / w;
  int x = offset / 4 % w;
  io_write(AM_GPU_FBDRAW, x, y, (uint32_t *)buf, len / 4, 1, true);
  return 0;
}

int sys_gettimeofday(struct timeval *tv, struct timezone *tz) {
  tv->tv_sec = io_read(AM_TIMER_UPTIME).us / 1000000;
  tv->tv_usec = io_read(AM_TIMER_UPTIME).us % 1000000;
  return 0;
}

void init_device() {
  gpuconf = io_read(AM_GPU_CONFIG);
  Log("Initializing devices...");
  ioe_init();
}
