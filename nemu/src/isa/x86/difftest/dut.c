#include <isa.h>
#include <monitor/difftest.h>
#include "../local-include/reg.h"
#include "difftest.h"

bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc) {
  // printf("pc: %x %x  \n", ref_r->pc, cpu.pc);
  // printf("ebp: %x %x", ref_r->ebp, cpu.ebp);
  // printf("eax: %x %x", ref_r->eax, cpu.eax);
  for (int i = 0; i < 8; i++) {
    if (ref_r->gpr[i]._32 != cpu.gpr[i]._32) {
      printf("%d %x %x\n", i, ref_r->gpr[i]._32, cpu.gpr[i]._32);return false;}
  }
  if ((*ref_r).pc != cpu.pc) return false;
  //printf("cpu :OF:%x ZF:%x SF:%x CF:%x IF:%x\n", cpu.OF, cpu.ZF, cpu.SF, cpu.CF, cpu.IF);
  //printf("ref :OF:%x ZF:%x SF:%x CF:%x IF:%x\n", ref_r->OF, ref_r->ZF, ref_r->SF, ref_r->CF, ref_r->IF);
  // if ((*ref_r).OF != cpu.OF) return false;
  // if ((*ref_r).ZF != cpu.ZF) return false;
  // if ((*ref_r).SF != cpu.SF) return false;
  // if ((*ref_r).CF != cpu.CF) return false;
  // if ((*ref_r).IF != cpu.IF) return false;
  
  // printf("   edx: %x %x %x\n", ref_r->edx, cpu.edx, cpu.esp);
  return true;
}

void isa_difftest_attach() {
}
