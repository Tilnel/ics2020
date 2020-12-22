#ifndef ARCH_H__
#define ARCH_H__

struct Context {
  //uintptr_t eax, ecx, ebx, edx, esp0, ebp, esi, edi, eflags, cs, esp;
  uintptr_t esp, cs, edi, esi, ebp, esp0, edx, ebx, ecx, eax;
  int irq;
  void *cr3;
};

#define GPR1 eax
#define GPR2 eip
#define GPR3 eip
#define GPR4 eip
#define GPRx eip

#endif
