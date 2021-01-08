#include <common.h>
#include "syscall.h"

void do_syscall(Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  a[1] = c->GPR2; 
  a[2] = c->GPR3;
  a[3] = c->GPR4;

  switch (a[0]) {
    case SYS_exit : halt(a[0]); break;
    case SYS_yield : yield(); break;
    case SYS_open : c->GPRx = fs_open((char *) a[3], a[2], a[1]); break;
    case SYS_read : c->GPRx = fs_read(a[3], (void *)a[2], a[1]); break;
    case SYS_write : c->GPRx = fs_write(a[1], (void *)a[2], a[3]); break;
    case SYS_lseek : c->GPRx = fs_lseek(a[3], a[2], a[1]); break;
    case SYS_close : c->GPRx = fs_close(a[3]); break;
    case SYS_gettimeofday : c->GPRx = sys_gettimeofday((void *)a[3], (void *)a[2]); break;
    case SYS_brk : c->GPRx = 0; break;

    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}
