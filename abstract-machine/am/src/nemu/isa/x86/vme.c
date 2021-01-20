#include <am.h>
#include <nemu.h>
#include <klib.h>

static AddrSpace kas = {};
static void* (*pgalloc_usr)(int) = NULL;
static void (*pgfree_usr)(void*) = NULL;
static int vme_enable = 0;

static Area segments[] = {      // Kernel memory mappings
  NEMU_PADDR_SPACE
};

#define USER_SPACE RANGE(0x40000000, 0xc0000000)

bool vme_init(void* (*pgalloc_f)(int), void (*pgfree_f)(void*)) {
  pgalloc_usr = pgalloc_f;
  pgfree_usr = pgfree_f;

  kas.ptr = pgalloc_f(PGSIZE);

  int i;
  for (i = 0; i < LENGTH(segments); i ++) {
    void *va = segments[i].start;
    for (; va < segments[i].end; va += PGSIZE) {
      map(&kas, va, va, 0);
    }
  }

  set_cr3(kas.ptr);
  set_cr0(get_cr0() | CR0_PG);
  vme_enable = 1;

  return true;
}

void protect(AddrSpace *as) {
  PTE *updir = (PTE*)(pgalloc_usr(PGSIZE));
  as->ptr = updir;
  as->area = USER_SPACE;
  as->pgsize = PGSIZE;
  // map kernel space
  memcpy(updir, kas.ptr, PGSIZE);
}

void unprotect(AddrSpace *as) {
}

void __am_get_cur_as(Context *c) {
  c->cr3 = (vme_enable ? (void *)get_cr3() : NULL);
}

void __am_switch(Context *c) {
  if (vme_enable && c->cr3 != NULL) {
    set_cr3(c->cr3);
  }
}

void map(AddrSpace *as, void *va, void *pa, int prot) {
}

Context* ucontext(AddrSpace *as, Area kstack, void *entry, char *const argv[], char *const envp[]) {
  Context *ret = as->area.end - 256;
  int i = 1;
  while (argv[i++][0]);
  int argc = i - 1;
  // printf("argc %d\n", argc);
  ret->eip = (int)entry;
  ret->esp = (int)(ret);
  ret->eax = (int)as->area.end;
  ret->cs = 8;
  //copy args

  ((uint32_t *)ret)[13] = argc;  // under stack 1 byte
  ((uint32_t *)ret)[14] = (uintptr_t)argv[0];
  ((uint32_t *)ret)[15] = (uintptr_t)argv[1];
  ((uint32_t *)ret)[16] = (uintptr_t)argv[2];
  ((uint32_t *)ret)[17] = (uintptr_t)argv[3];
  ((uint32_t *)ret)[18] = (uintptr_t)argv[4];
  

  ((uint32_t *)ret)[15] = (int)envp;
  return ret;
}
