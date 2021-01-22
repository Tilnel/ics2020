#include <isa.h>
#include <memory/vaddr.h>
#include <memory/paddr.h>
paddr_t isa_mmu_translate(vaddr_t vaddr, int type, int len) {
  uint32_t dir = vaddr >> 22;
  uint32_t page = (vaddr >> 12) & 0x3ff;
  uint32_t offset = vaddr & 0xfff;

  paddr_t cr3 = cpu.cr3;
  paddr_t page_sheet = paddr_read(cr3 + 4 * dir, 4);
  if ((page_sheet & 1) == 0) 
    panic("pagefault at vaddr = 0x%x, pagesheet %x", vaddr, page_sheet);
  paddr_t page_sheet_item = paddr_read((page_sheet & 0xfffff000) + 4 * page, 4);
  if ((page_sheet_item & 1) == 0) 
    panic("pagefault at vaddr = 0x%x, pageitem %x", vaddr, page_sheet_item);
  paddr_t paddr = (page_sheet_item & 0xfffff000) + offset;
  
  // panic("cross page at vaddr = 0x%x, paddr = 0x%x", vaddr, paddr);
  assert(paddr == vaddr);
  return paddr;
}

word_t vaddr_mmu_read(vaddr_t addr, int len, int type) {
  word_t ret = 0;
  if (len > 1 && (addr & 0xfff) > 4 && ((addr + len) & 0xfff) < 4) {
    for (int i = 0; i < len; i++) {
      vaddr_t tmp = addr + i;
      paddr_t ptmp = isa_mmu_translate(tmp, type, 1);
      ret += paddr_read(ptmp, 1) << (8 * i);
    }
    return ret;
  }
  paddr_t paddr = isa_mmu_translate(addr, type, len);
  return paddr_read(paddr, len);
}

void vaddr_mmu_write(vaddr_t addr, word_t data, int len) {
  if (len > 1 && (addr & 0xfff) > 4 && ((addr + len) & 0xfff) < 4) {
    for (int i = 0; i < len; i++) {
      vaddr_t tmp = addr + i;
      paddr_t ptmp = isa_mmu_translate(tmp, 0, 1);
      word_t dat = data & 0xff;
      data = data >> 8;
      paddr_write(ptmp, dat, 1);
    }
    return;
  }
  paddr_t paddr = isa_mmu_translate(addr, 0, len);
  paddr_write(paddr, data, len);
}
