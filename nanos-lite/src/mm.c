#include <memory.h>

static void *pf = NULL;

void* new_page(size_t nr_page) {
  void *tmp = pf;
  // memset(pf, 0, nr_page * PGSIZE); 
  pf += nr_page * PGSIZE;
  return tmp;
}

static inline void* pg_alloc(int n) {
  return new_page((n + PGSIZE - 1) / PGSIZE);
}

void free_page(void *p) {
  panic("not implement yet");
}

/* The brk() system call handler. */
int mm_brk(uintptr_t brk) {
  return 0;
}

void init_mm() {
  pf = (void *)ROUNDUP(heap.start, PGSIZE);
  Log("free physical pages starting from %p", pf);

  vme_init(pg_alloc, free_page);
}
