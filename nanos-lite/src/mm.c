#include <memory.h>
#include <proc.h>

static void *pf = NULL;

void* new_page(size_t nr_page) {
  void *tmp = pf;
  memset(pf, 0, nr_page * PGSIZE); 
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
  // if (brk > current->max_brk) {
  //   size_t nr_page = (brk - current->max_brk + PGSIZE - 1) / PGSIZE;
  //   void *page = new_page(nr_page);
  //   for (int i = 0; i < nr_page; i++) {
  //     map(&current->as, (void *)current->max_brk + i * PGSIZE, page + i * PGSIZE, 0);
  //   }
  //   current->max_brk += PGSIZE * nr_page;
  // }
  return 0;
}

void init_mm() {
  pf = (void *)ROUNDUP(heap.start, PGSIZE);
  Log("free physical pages starting from %p", pf);

  vme_init(pg_alloc, free_page);
}
