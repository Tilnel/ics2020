#include <isa.h>
#include <memory/vaddr.h>
#include <memory/paddr.h>
paddr_t isa_mmu_translate(vaddr_t vaddr, int type, int len) {

  // return MEM_RET_FAIL;
  return MEM_RET_OK;
}

word_t vaddr_mmu_read(vaddr_t addr, int len, int type) {
  paddr_t paddr = isa_mmu_translate(addr, type, len);
  return paddr_read(paddr, len);
}

void vaddr_mmu_write(vaddr_t addr, word_t data, int len) {
  paddr_t paddr = isa_mmu_translate(addr, MEM_TYPE_WRITE, len);
  paddr_write(paddr, data, len);
}
