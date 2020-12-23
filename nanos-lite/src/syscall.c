#include <common.h>
#include "syscall.h"
size_t write(int fd, const void *buf, size_t count);

void do_syscall(Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  a[1] = c->GPR2; 
  a[2] = c->GPR3;
  a[3] = c->GPR4;

  switch (a[0]) {
    case SYS_exit : halt(a[0]); break;
    case SYS_yield : yield(); break;
    case SYS_write : write(a[1], (void *)a[2], a[3]); break;

    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}
