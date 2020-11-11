#include <am.h>
#include <nemu.h>

#define KEYDOWN_MASK 0x8000

void __am_input_keybrd(AM_INPUT_KEYBRD_T *kbd) {
  if(kbd->keydown == 0)
  kbd->keycode = AM_KEY_NONE;
  else kbd->keycode = inw(KBD_ADDR);
}
