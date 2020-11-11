#include <am.h>
#include <nemu.h>

#define KEYDOWN_MASK 0x8000

void __am_input_keybrd(AM_INPUT_KEYBRD_T *kbd) {
  //kbd->keydown = 0;
  //kbd->keycode = AM_KEY_NONE;
  if(kbd->keydown) outl(kbd->keycode, KBD_ADDR);
  else outl(AM_KEY_NONE, KBD_ADDR);
}
