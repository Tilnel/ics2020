#include <common.h>
#include <sys/time.h>

#if defined(MULTIPROGRAM) && !defined(TIME_SHARING)
# define MULTIPROGRAM_YIELD() yield()
#else
# define MULTIPROGRAM_YIELD()
#endif

#define NAME(key) \
  [AM_KEY_##key] = #key,

static const char *keyname[256] __attribute__((used)) = {
  [AM_KEY_NONE] = "NONE",
  AM_KEYS(NAME)
};

size_t serial_write(const void *buf, size_t offset, size_t len) {
  for (int i = 0; i < len; i++) putch(((char *)buf)[i]); 
  return 0;
}

size_t events_read(void *buf, size_t offset, size_t len) {
  AM_INPUT_KEYBRD_T ev = io_read(AM_INPUT_KEYBRD);
  if (ev.keycode == AM_KEY_NONE) { memset(buf, 0, len); return 0; }
  return sprintf(buf, "%s %s", ev.keydown ? "kd" : "ku", keyname[ev.keycode], len);
}

size_t dispinfo_read(void *buf, size_t offset, size_t len) {
  AM_GPU_FBDRAW_T ctl;
  ctl = io_read(AM_GPU_FBDRAW);
  sprintf(buf, "WIDTH: %d\nHEIGHT: %d\n", ctl.w, ctl.h);
  return 0;
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
  return 0;
}

int sys_gettimeofday(struct timeval *tv, struct timezone *tz) {
  tv->tv_sec = io_read(AM_TIMER_UPTIME).us / 1000000;
  tv->tv_usec = io_read(AM_TIMER_UPTIME).us % 1000000;
  return 0;
}

void init_device() {
  Log("Initializing devices...");
  ioe_init();
}
