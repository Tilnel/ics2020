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
      printf("%x\n", i);

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
  uint32_t dir = (uintptr_t)va >> 22;
  uint32_t page = ((uintptr_t)va >> 12) & 0x3ff;
  // uint32_t offset = (uintptr_t)va & 0xfff;

  as->pgsize = PGSIZE;
  uint32_t *index = as->ptr;      //page index
  uint32_t *page_sheet_base = (uint32_t *)(index[dir]);
  if (((uint32_t)page_sheet_base & 1) == 0) {
    page_sheet_base = (uint32_t *)(((uint32_t)pgalloc_usr(PGSIZE) & 0xfffff000) | 1);
    index[dir] = (uint32_t)page_sheet_base;
    // printf("%x\n", index[dir]);
  }
  uint32_t page_sheet_item = ((uintptr_t)pa & 0xfffff000) | 1;
  ((uint32_t *)((uint32_t)page_sheet_base & 0xfffff000))[page] = page_sheet_item; 

}

Context* ucontext(AddrSpace *as, Area kstack, void *entry) {
  Context *ret = kstack.end - 52;
  // ret->eax = (int)kstack.end;
  ret->eip = (int)entry;
  ret->cs = 8;
  ret->cr3 = as->ptr;
  return ret;
}
