#include <am.h>
#include <nemu.h>

#define SYNC_ADDR (VGACTL_ADDR + 4)

void __am_gpu_init() {
  int i;
  int w = 400;
  int h = 300;
  uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
  for (i = 0; i < w * h; i ++) fb[i] = i;
  outl(SYNC_ADDR, 1);
}

void __am_gpu_config(AM_GPU_CONFIG_T *cfg) {
  *cfg = (AM_GPU_CONFIG_T) {
    .present = true, .has_accel = false,
    .width = 400, .height = 300,
    .vmemsz = 0
  };
}

void __am_gpu_fbdraw(AM_GPU_FBDRAW_T *ctl) {
  if (ctl->sync) {
    outl(SYNC_ADDR, 1);
    int x = ctl->x, y = ctl->y, w = ctl->w, h = ctl->h;
    uint32_t *pixels = ctl->pixels;
    for (int i = 0; i < h && y + i < 300; i ++) {
      for (int j = 0; j < w && x + j < 400; j ++) {
        outl (FB_ADDR + (i * 400 + j) * 4, *pixels);
        pixels ++;
      }
    pixels += w;
    }
  }
}

void __am_gpu_status(AM_GPU_STATUS_T *status) {
  status->ready = true;
}
