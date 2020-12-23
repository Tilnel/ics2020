#ifndef ARCH_H__
#define ARCH_H__

struct Context {
  void *cr3;
  uintptr_t edi, esi, ebp, esp0, edx, ebx, ecx, eax;
  // uintptr_t eax, ecx, ebx, edx, esp0, ebp, esi, edi;
  int irq;  
  uintptr_t eip, cs, eflags;
};

#define GPR1 eax
#define GPR2 eip
#define GPR3 eip
#define GPR4 eip
#define GPRx eip

#endif
