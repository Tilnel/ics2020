#include <monitor/difftest.h>

uint32_t pio_read_l(ioaddr_t);
uint32_t pio_read_w(ioaddr_t);
uint32_t pio_read_b(ioaddr_t);
void pio_write_l(ioaddr_t, uint32_t);
void pio_write_w(ioaddr_t, uint32_t);
void pio_write_b(ioaddr_t, uint32_t);

static inline def_EHelper(lidt) {
  *ddest = cpu.gpr[*ddest & 0x7]._32;
  *s0 = vaddr_read(*ddest, 2);
  *s1 = vaddr_read(*ddest + 2, 4);
  cpu.ldtr.size = *s0;
  cpu.ldtr.base = *s1;
  //TODO();
  print_asm_template1(lidt);
}

static inline def_EHelper(lgdt) {
  *ddest = cpu.gpr[*ddest & 0x7]._32;
  *s0 = vaddr_read(*ddest, 2);
  *s1 = vaddr_read(*ddest + 2, 4);
  cpu.gdtr.size = *s0;
  cpu.gdtr.base = *s1;
  //TODO();
  print_asm_template1(lgdt);
}

static inline def_EHelper(mov_r2cr) {
  TODO();
  print_asm("movl %%%s,%%cr%d", reg_name(id_src1->reg, 4), id_dest->reg);
}

static inline def_EHelper(mov_cr2r) {
  TODO();
  print_asm("movl %%cr%d,%%%s", id_src1->reg, reg_name(id_dest->reg, 4));

#ifndef __DIFF_REF_NEMU__
  difftest_skip_ref();
#endif
}

static inline def_EHelper(int) {
  if ((cpu.cs & 0x3) == 3) {
    printf("%x\n", cpu.gdtr.base);
    printf("%08x", vaddr_read(cpu.gdtr.base + 8 * 28, 4));
    printf("%08x", vaddr_read(cpu.gdtr.base + 8 * 28 + 4, 4));
    printf("%d\n", cpu.tr);
    *s0 = vaddr_read(cpu.gdtr.base + 8 * (cpu.tr) + 2, 2);
    *s0 = *s0 + (vaddr_read(cpu.gdtr.base + 8 * (cpu.tr) + 4, 1) << 16);
    *s1 = vaddr_read(cpu.gdtr.base + 8 * (cpu.tr) + 7, 1) << 24;
    *s2 = *s0 + *s1;    // tss struct
    
    printf("\ns2 %x\n", *s2);
    *s0 = cpu.esp;    
    *s1 = vaddr_read(*s2 + 8, 4); // ss0
    cpu.esp = vaddr_read(*s2 + 4, 4); // esp0
    rtl_push(s, s1); // push ss0
    rtl_push(s, s0); // push usr esp
  }

  rtl_push(s, &cpu.eflags);
  rtl_push(s, &cpu.cs);
  rtl_push(s, &(s->seq_pc));

  *s0 = vaddr_read(cpu.ldtr.base + 8 * (*ddest), 2);
  *s1 = vaddr_read(cpu.ldtr.base + 8 * (*ddest) + 6, 2);
  *s2 = (*s1 << 16) + *s0;
  rtl_j(s, *s2);
  //TODO();
  print_asm("int %s", id_dest->str);

#ifndef __DIFF_REF_NEMU__
  difftest_skip_dut(1, 2);
#endif
}

static inline def_EHelper(iret) {
  rtl_pop(s, s0);  
  rtl_pop(s, &cpu.cs);
  rtl_pop(s, &cpu.eflags);

  if ((cpu.cs & 0x3) == 3) {
    rtl_pop(s, s1);
    rtl_pop(s, &cpu.ss);
    cpu.esp = *s1;
  }
  
  rtl_j(s, *s0);
  // TODO();
  print_asm("iret");

#ifndef __DIFF_REF_NEMU__
  difftest_skip_ref();
#endif
}

static inline def_EHelper(in) {
  switch (id_dest->width) {
    case 1 : *s0 = pio_read_b(*dsrc1); break;  
    case 2 : *s0 = pio_read_w(*dsrc1); break;
    case 4 : *s0 = pio_read_l(*dsrc1); break;
    default : assert(0);
  } 
  operand_write(s, id_dest, s0);
  //TODO();
  print_asm_template2(in);
}

static inline def_EHelper(out) {
  switch (id_src1->width) {
    case 1 : pio_write_b(*ddest, *dsrc1); break;  
    case 2 : pio_write_w(*ddest, *dsrc1); break;
    case 4 : pio_write_l(*ddest, *dsrc1); break;
    default : assert(0);
  } 
  //TODO();
  print_asm_template2(out);
}

static inline def_EHelper(in_di) {
  switch (id_dest->width) {
    case 1 : *s0 = pio_read_b(*dsrc1); break;  
    case 2 : *s0 = pio_read_w(*dsrc1); break;
    case 4 : *s0 = pio_read_l(*dsrc1); break;
    default : assert(0);
  } 
  vaddr_write(cpu.edi, *s0, 1);
  cpu.edi++;
  //TODO();
  print_asm_template2(in);
}

static inline def_EHelper(ltr) {
  cpu.tr = reg_w(0);
}