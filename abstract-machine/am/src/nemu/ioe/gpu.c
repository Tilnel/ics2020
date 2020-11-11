#include <am.h>
#include <nemu.h>

#define SYNC_ADDR (VGACTL_ADDR + 4)

void __am_gpu_init() {
  /*int i;
  int w = inw(VGACTL_ADDR);
  int h = inw(VGACTL_ADDR + 2);
  uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
  for (i = 0; i < w * h; i ++) fb[i] = i;
  outl(SYNC_ADDR, 1);
  */
}

void __am_gpu_config(AM_GPU_CONFIG_T *cfg) {
  *cfg = (AM_GPU_CONFIG_T) {
    .present = true, .has_accel = false,
    .width = 400, .height = 300,
    .vmemsz = 0
  };
}

void __am_gpu_fbdraw(AM_GPU_FBDRAW_T *ctl) {
  int W = inw(VGACTL_ADDR);
  int H = inw(VGACTL_ADDR + 2);
  if (ctl->sync) {
    outl(SYNC_ADDR, 1);
    int x = ctl->x, y = ctl->y, w = ctl->w, h = ctl->h;
    uint32_t *pixels = ctl->pixels; 
    uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
    for (int i = 0; i < h && y + i < H; i ++) {
      for (int j = 0; j < w && x + j < W; j ++) {
        fb[(i + y) * W + j + x] = *pixels;

      }
      pixels += W;
    }
  }
}

void __am_gpu_status(AM_GPU_STATUS_T *status) {
  status->ready = true;
}
