#include <isa.h>
#include <memory/vaddr.h>
#include <memory/paddr.h>
paddr_t isa_mmu_translate(vaddr_t vaddr, int type, int len) {
  uint32_t dir = vaddr >> 22;
  uint32_t page = (vaddr >> 12) & 0x3ff;
  uint32_t offset = vaddr & 0xfff;

  paddr_t cr3 = cpu.cr3;
  printf("%x\n", vaddr);
  paddr_t page_sheet = paddr_read(cr3 + 4 * dir, 4);
  if ((page_sheet & 1) == 0) panic("pagefault at vaddr = 0x%x", vaddr);
  paddr_t page_sheet_item = paddr_read((page_sheet & 0xfffff000) + 4 * page, 4);
  if ((page_sheet_item & 1) == 0) panic("pagefault at vaddr = 0x%x", vaddr);
  paddr_t paddr = (page_sheet_item & 0xfffff000) + offset;
  if ((paddr & 0xfff) > 4 && ((paddr + len) & 0xfff) < 4) panic("cross page at vaddr = 0x%x, paddr = 0x%x", vaddr, paddr);
  assert(paddr == vaddr);
  return paddr;
}

word_t vaddr_mmu_read(vaddr_t addr, int len, int type) {
  paddr_t paddr = isa_mmu_translate(addr, type, len);
  return paddr_read(paddr, len);
}

void vaddr_mmu_write(vaddr_t addr, word_t data, int len) {
  paddr_t paddr = isa_mmu_translate(addr, MEM_TYPE_WRITE, len);
  paddr_write(paddr, data, len);
}
