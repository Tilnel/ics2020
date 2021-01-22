#include <cpu/exec.h>
#include "local-include/rtl.h"
#define IRQ_TIMER 32

void raise_intr(DecodeExecState *s, uint32_t NO, vaddr_t ret_addr) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * That is, use ``NO'' to index the IDT.
   */
  if ((cpu.cs & 0x3) == 3) {
    *s0 = vaddr_read(cpu.gdtr.base + (cpu.tr) + 2, 2);
    *s0 = *s0 + (vaddr_read(cpu.gdtr.base + (cpu.tr) + 4, 1) << 16);
    *s1 = vaddr_read(cpu.gdtr.base + (cpu.tr) + 7, 1) << 24;
    *s2 = *s0 + *s1;    // tss struct
    
    *s0 = cpu.esp;    
    *s1 = vaddr_read(*s2 + 8, 4); // ss0
    cpu.esp = vaddr_read(*s2 + 4, 4); // esp0
    rtl_push(s, s1); // push ss0
    rtl_push(s, s0); // push usr esp
  }
  *s0 = vaddr_read(cpu.ldtr.base + 8 * NO, 2);
  *s1 = vaddr_read(cpu.ldtr.base + 8 * NO + 6, 2);
  *s2 = (*s1 << 16) + *s0;
  rtl_push(s, &cpu.eflags);
  cpu.IF = 0;
  rtl_push(s, &cpu.cs);
  rtl_push(s, &ret_addr);
  cpu.pc = *s2;
 
  // TODO();
}

void query_intr(DecodeExecState *s) {
  if (cpu.INTR == true && cpu.IF == 1) {
    cpu.INTR = false;
    raise_intr(s, IRQ_TIMER, cpu.pc);
  }
  // TODO();
}
