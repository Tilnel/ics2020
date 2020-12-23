#ifndef ARCH_H__
#define ARCH_H__

struct Context {
  uintptr_t eax, ecx, ebx, edx, esp0, ebp, esi, edi, esp;
  //uintptr_t esp, cs, edi, esi, ebp, esp0, edx, ebx, ecx, eax;
  void *cr3;
  int irq;  
  uintptr_t cs, dd;
};

#define GPR1 eax
#define GPR2 eip
#define GPR3 eip
#define GPR4 eip
#define GPRx eip

#endif
