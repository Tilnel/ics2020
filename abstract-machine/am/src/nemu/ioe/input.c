#include <am.h>
#include <nemu.h>

#define KEYDOWN_MASK 0x8000

void __am_input_keybrd(AM_INPUT_KEYBRD_T *kbd) {
    kbd->keycode = inw(KBD_ADDR);
    kbd->keydown = kbd->keycode >> 15;
    kbd->keycode &= 0xff;
}