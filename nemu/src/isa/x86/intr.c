#include <cpu/exec.h>
#include "local-include/rtl.h"

void raise_intr(DecodeExecState *s, uint32_t NO, vaddr_t ret_addr) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * That is, use ``NO'' to index the IDT.
   */
  assert(0);
  *s0 = vaddr_read(cpu.ldtr.base + 8 * NO, 2);
  *s1 = vaddr_read(cpu.ldtr.base + 8 * NO + 6, 2);
  ret_addr = (*s0 << 16) + *s1;
  rtl_push(s, &cpu.eflags);
  rtl_push(s, &cpu.cs);
  rtl_push(s, &cpu.pc);
 
  // TODO();
}

void query_intr(DecodeExecState *s) {
  TODO();
}
