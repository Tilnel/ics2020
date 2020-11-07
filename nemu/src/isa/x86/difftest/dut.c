#include <isa.h>
#include <monitor/difftest.h>
#include "../local-include/reg.h"
#include "difftest.h"

bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc) {
  printf("pc: %x %x  ", ref_r->pc, cpu.pc);
  printf("ebp: %x %x\n", ref_r->ebp, cpu.ebp);
  for (int i = 0; i < 8; i++) {
    if (ref_r->gpr[i]._32 != cpu.gpr[i]._32) {
      printf("%d %x %x\n", i, ref_r->gpr[i]._32, cpu.gpr[i]._32);return false;}
  }
  //if ((*ref_r).pc != cpu.pc) return false;
  // print("eax: %x %x\n", ref_r->eax, cpu.eax);
  return true;
}

void isa_difftest_attach() {
}
