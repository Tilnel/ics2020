static inline def_EHelper(mov) {
  operand_write(s, id_dest, dsrc1);
  print_asm_template2(mov);
}

static inline def_EHelper(push) {
  rtl_push(s, ddest);

//  TODO();
  print_asm_template1(push);
}

static inline def_EHelper(push_imm8) {
  rtl_sext(s, s0, ddest, 1);
  rtl_push(s, s0);
}

static inline def_EHelper(pop) {
  rtl_pop(s, ddest);
  //TODO();
  print_asm_template1(pop);
}

static inline def_EHelper(pusha) {
  if (s->isa.is_operand_size_16) {
  } else {
    *s0 = cpu.esp - 36;
    rtl_push(s, &cpu.eax);
    rtl_push(s, &cpu.ecx);
    rtl_push(s, &cpu.edx);
    rtl_push(s, &cpu.ebx);
    rtl_push(s, s0);
    rtl_push(s, &cpu.ebp);
    rtl_push(s, &cpu.esi);
    rtl_push(s, &cpu.edi);
  }
  //TODO();
  print_asm("pusha");
}

static inline def_EHelper(popa) {
  rtl_pop(s, &cpu.edi);
  rtl_pop(s, &cpu.esi);
  rtl_pop(s, &cpu.ebp);
  rtl_pop(s, s0);
  rtl_pop(s, &cpu.ebx);
  rtl_pop(s, &cpu.edx);
  rtl_pop(s, &cpu.ecx);
  rtl_pop(s, &cpu.eax);
  //TODO();
  print_asm("popa");
}

static inline def_EHelper(leave) {
  cpu.esp = cpu.ebp;
  rtl_pop(s, &cpu.ebp);
  //TODO();
  print_asm("leave");
}

static inline def_EHelper(cltd) {
  if (s->isa.is_operand_size_16) {
    if (cpu.eax >> 15 & 1) cpu.edx |= 0x0000ffff;
    else cpu.edx &= 0xffff0000;
    //TODO();
  }
  else {
    if (cpu.eax >> 31) cpu.edx = 0xffffffff;
    else cpu.edx = 0;
    //TODO();
  }
  print_asm(s->isa.is_operand_size_16 ? "cwtl" : "cltd");
}

static inline def_EHelper(cwtl) {
  if (s->isa.is_operand_size_16) {
    *s0 = cpu.eax & 0x000000ff;
    rtl_sext(s, s1, s0, 1);
    cpu.eax &= 0xffff0000;
    cpu.eax |= (*s1 & 0x0000ffff);
    //TODO();
  }
  else {
    *s0 = cpu.eax & 0x0000ffff;
    rtl_sext(s, s1, s0, 2);
    cpu.eax = *s1;
    //TODO();
  }
  print_asm(s->isa.is_operand_size_16 ? "cbtw" : "cwtl");
}

static inline def_EHelper(movsx) {
  id_dest->width = s->isa.is_operand_size_16 ? 2 : 4;
  rtl_sext(s, ddest, dsrc1, id_src1->width);
  operand_write(s, id_dest, ddest);
  print_asm_template2(movsx);
}

static inline def_EHelper(movzx) {
  id_dest->width = s->isa.is_operand_size_16 ? 2 : 4;
  rtl_zext(s, ddest, dsrc1, id_src1->width);
  operand_write(s, id_dest, dsrc1);
  print_asm_template2(movzx);
}

static inline def_EHelper(lea) {
  rtl_addi(s, ddest, s->isa.mbase, s->isa.moff);
  operand_write(s, id_dest, ddest);
  print_asm_template2(lea);
}

static inline def_EHelper(movsb) {//????????????
  *s1 = s->dest.width;
  // printf("%d\n", *s1);
  *s0 = vaddr_read(cpu.esi, *s1);
  vaddr_write(cpu.edi, *s0, *s1);
  cpu.edi += *s1;
  cpu.esi += *s1;
  print_asm_template2(mov);
}

static inline def_EHelper(xchg) {
  *s0 = *ddest;
  operand_write(s, id_dest, dsrc1);
  operand_write(s, id_src1, s0);
}

static inline def_EHelper(r2cr) {
  *dsrc1 = *ddest;
  if (s->opcode == 0x20) assert(0);
}

static inline def_EHelper(cr2r) {
  *ddest = *dsrc1;
}