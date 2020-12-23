#ifndef ARCH_H__
#define ARCH_H__

struct Context {
  void    *cr3;
  uint32_t ds, eax, ebx, ecx, edx,
           esp0, esi, edi, ebp,
           eip, cs, eflags, esp, ss3;
};

#define GPR1 edi
#define GPR2 esi
#define GPR3 edx
#define GPR4 ecx
#define GPRx eax

#endif
