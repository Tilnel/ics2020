#include <cpu/exec.h>
#include "local-include/rtl.h"
#define IRQ_TIMER 32

void raise_intr(DecodeExecState *s, uint32_t NO, vaddr_t ret_addr) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * That is, use ``NO'' to index the IDT.
   */
  *s0 = vaddr_read(cpu.ldtr.base + 8 * NO, 2);
  *s1 = vaddr_read(cpu.ldtr.base + 8 * NO + 6, 2);
  *s2 = (*s1 << 16) + *s0;
  rtl_push(s, &cpu.eflags);
  cpu.IF = 0;
  rtl_push(s, &cpu.cs);
  rtl_push(s, &ret_addr);
  s->seq_pc = *s2;
 
  // TODO();
}

void query_intr(DecodeExecState *s) {
  if (cpu.IF == 1 && cpu.INTR == true) {
    cpu.INTR = false;
    raise_intr(s, IRQ_TIMER, s->seq_pc);
    update_pc(s);
    // printf("timer\n");
  }
  // TODO();
}
