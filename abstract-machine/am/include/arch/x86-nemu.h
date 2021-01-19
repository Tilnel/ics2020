#ifndef ARCH_H__
#define ARCH_H__

struct Context {
  void *esp0, *cr3;
  uintptr_t edi, esi, ebp, esp, edx, ebx, ecx, eax;
  // uintptr_t eax, ecx, ebx, edx, esp0, ebp, esi, edi;
  int irq;  
  uintptr_t eip, cs, eflags;
};

#define GPR1 eax
#define GPR2 ebx
#define GPR3 ecx
#define GPR4 edx
#define GPRx eax

#endif
