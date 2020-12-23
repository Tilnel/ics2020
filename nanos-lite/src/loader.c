#include <proc.h>
#include <elf.h>
extern size_t ramdisk_write(const void *buf, size_t offset, size_t len);
extern size_t ramdisk_read(void *buf, size_t offset, size_t len);
extern size_t get_ramdisk_size();

#ifdef __LP64__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif

static uintptr_t loader(PCB *pcb, const char *filename) {
  // TODO();
  Elf_Ehdr eh;
  Elf_Phdr ph;
  ramdisk_read((void *)(&eh), 0, sizeof(eh));
  for (int i = 0; i < eh.e_phnum; i++) {
    ramdisk_read((void *)(&ph), i * eh.e_phentsize, eh.e_phentsize);
    if (ph.p_type == 1) {
      ramdisk_read((void *)ph.p_vaddr, ph.p_offset, ph.p_filesz);
      memset((void *)ph.p_vaddr + ph.p_filesz, 0, ph.p_memsz - ph.p_filesz);
    }
  }
  return eh.e_entry;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %p", entry);
  ((void(*)())entry) ();
}

