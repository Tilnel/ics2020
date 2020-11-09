#include "cc.h"

static inline def_EHelper(test) {
  *ddest &= *dsrc1;
  cpu.CF = cpu.OF = 0;
  rtl_update_ZFSF(s, ddest, id_dest-> width);
  //TODO();
  print_asm_template2(test);
}

static inline def_EHelper(and) {
  *ddest &= *dsrc1;
  cpu.CF = cpu.OF = 0;
  rtl_update_ZFSF(s, ddest, s->width);
  //TODO();
  operand_write(s, id_dest, ddest);
  print_asm_template2(and);
}

static inline def_EHelper(andl) {
  exec_and(s);
  operand_write(s, id_dest, ddest);
}

static inline def_EHelper(xor) {
  *ddest ^= *dsrc1;
  cpu.CF = cpu.OF = 0;
  rtl_update_ZFSF(s, ddest, s->width);
  //TODO();
  operand_write(s, id_dest, ddest);
  print_asm_template2(xor);
}

static inline def_EHelper(or) {
  *ddest |= *dsrc1;
  cpu.CF = cpu.OF = 0;
  rtl_update_ZFSF(s, ddest, s->width);
  //TODO();
  operand_write(s, id_dest, ddest);
  print_asm_template2(or);
}

static inline def_EHelper(orl) {
  exec_or(s);
  operand_write(s, id_dest, ddest);
}

static inline def_EHelper(not) {
  //TODO();
  *ddest = ~(*ddest);
  cpu.CF = cpu.OF = 0;
  rtl_update_ZFSF(s, ddest, s->width);
  operand_write(s, id_dest, ddest);
  print_asm_template1(not);
}

static inline def_EHelper(sar) {
  *ddest = (int)(*ddest) >> (*dsrc1);
  cpu.CF = cpu.OF = 0;
  rtl_update_ZFSF(s, ddest, s->width);
  //TODO();
  // unnecessary to update CF and OF in NEMU
  operand_write(s, id_dest, ddest);
  print_asm_template2(sar);
}

static inline def_EHelper(shl) {
  *ddest <<= *dsrc1;
  cpu.CF = cpu.OF = 0;
  rtl_update_ZFSF(s, ddest, s->width);
  //TODO();
  // unnecessary to update CF and OF in NEMU
  operand_write(s, id_dest, ddest);
  print_asm_template2(shl);
}

static inline def_EHelper(shr) {
  *ddest >>= *dsrc1;
  cpu.CF = cpu.OF = 0;
  rtl_update_ZFSF(s, ddest, s->width);
  //TODO();
  // unnecessary to update CF and OF in NEMU
  operand_write(s, id_dest, ddest);
  print_asm_template2(shr);
}

static inline def_EHelper(rol) {
  TODO();
}

static inline def_EHelper(setcc) {
  uint32_t cc = s->opcode & 0xf;
  rtl_setcc(s, ddest, cc);
  operand_write(s, id_dest, ddest);

  print_asm("set%s %s", get_cc_name(cc), id_dest->str);
}
