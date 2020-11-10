#include "cc.h"

static inline def_EHelper(test) {
  *s0 = *ddest & *dsrc1;
  cpu.CF = cpu.OF = 0;
  rtl_update_ZFSF(s, s0, id_dest-> width);
  //TODO();
  print_asm_template2(test);
}

static inline def_EHelper(and) {
  *s0 = *ddest & *dsrc1;
  cpu.CF = cpu.OF = 0;
  rtl_update_ZFSF(s, s0, id_dest->width);
  //TODO();
  operand_write(s, id_dest, s0);
  print_asm_template2(and);
}

static inline def_EHelper(andl) {
  exec_and(s);
  operand_write(s, id_dest, ddest);
}

static inline def_EHelper(xor) {
  *s0 = *ddest ^ *dsrc1;
  cpu.CF = cpu.OF = 0;
  rtl_update_ZFSF(s, s0, id_dest->width);
  //TODO();
  operand_write(s, id_dest, s0);
  print_asm_template2(xor);
}

static inline def_EHelper(or) {
  *s0 = *ddest | *dsrc1;
  cpu.CF = cpu.OF = 0;
  rtl_update_ZFSF(s, s0, id_dest->width);
  //TODO();
  operand_write(s, id_dest, s0);
  print_asm_template2(or);
}

static inline def_EHelper(orl) {
  exec_or(s);
  operand_write(s, id_dest, ddest);
}

static inline def_EHelper(not) {
  //TODO();
  *s0 = ~(*ddest);
  cpu.CF = cpu.OF = 0;
  rtl_update_ZFSF(s, s0, id_dest->width);
  operand_write(s, id_dest, s0);
  print_asm_template1(not);
}

static inline def_EHelper(sar) {
  *s1 = *ddest >> (id_dest->width * 8 - 1) & 1;
  *s2 = *dsrc1;
  // *s0 = (*ddest) >> (*dsrc1);
  *s0 = *ddest;
  //printf("%d\n", id_dest->width);
  for ( ; *s2 > 0; (*s2) --) {
    // *s0 &= ~(1 << (id_dest -> width * 8 - *s2));
    // *s0 |= (*s1 << (id_dest -> width * 8 - *s2));
    *s0 >>= 1;
    *s0 |= (*s1) << (id_dest->width * 8 - 1);
  }
  cpu.CF = cpu.OF = 0;
  rtl_update_ZFSF(s, s0, id_dest->width);
  //TODO();
  // unnecessary to update CF and OF in NEMU
  operand_write(s, id_dest, s0);
  print_asm_template2(sar);
}

static inline def_EHelper(shl) {
  *s0 = *ddest << *dsrc1;
  cpu.CF = cpu.OF = 0;
  rtl_update_ZFSF(s, s0, id_dest->width);
  //TODO();
  // unnecessary to update CF and OF in NEMU
  operand_write(s, id_dest, s0);
  print_asm_template2(shl);
}

static inline def_EHelper(shr) {
  *s0 = *ddest;
  *s0 >>= *dsrc1;
  cpu.CF = cpu.OF = 0;
  rtl_update_ZFSF(s, s0, id_dest->width);
  //TODO();
  // unnecessary to update CF and OF in NEMU
  //printf("%d\n", id_dest->width);
  operand_write(s, id_dest, s0);
  print_asm_template2(shr);
}

static inline def_EHelper(rol) {
  *s0 = *ddest;
  *s1 = *dsrc1;
  *s1 = *s1 & 0x1f;
  for ( ; *s1 > 0; (*s1)--) {
    *s2 = *s0 << 1;
    *s0 = (*s0 >> (id_dest->width * 8 - 1) & 1) + *s2; 
  }
  operand_write(s, id_dest, s0);
  //TODO();
}

static inline def_EHelper(bsr) {
  if (*dsrc1 == 0) cpu.ZF = 1;
  else {
    *s1 = 0;
    *s0 = *s2 = id_dest->width * 8 - 1;
    for (; *s0 >= 0; (*s0) --) {
      *s1 += (*dsrc1 >> *s0 & 1) << (*s2 - *s1);
    }
    operand_write(s, id_dest, s1);
  }
  //TODO();
}

static inline def_EHelper(setcc) {
  uint32_t cc = s->opcode & 0xf;
  rtl_setcc(s, ddest, cc);
  operand_write(s, id_dest, ddest);

  print_asm("set%s %s", get_cc_name(cc), id_dest->str);
}
