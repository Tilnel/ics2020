#include <proc.h>
#include <elf.h>
size_t ramdisk_write(const void *buf, size_t offset, size_t len);
size_t ramdisk_read(void *buf, size_t offset, size_t len);
size_t get_ramdisk_size();

#ifdef __LP64__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif

uintptr_t loader(PCB *pcb, const char *filename) {
  // TODO();
  Elf_Ehdr eh;
  Elf_Phdr ph;

  int fd = fs_open(filename, 0, 0); // 0b11, rw
  if (fd == -1) return 0;
  // ramdisk_read(&eh, 0, sizeof(Elf_Ehdr));
  fs_read(fd, &eh, sizeof(Elf_Ehdr));

  for (int i = 0; i < eh.e_phnum; i++) {
    fs_lseek(fd, eh.e_phoff + i * sizeof(Elf_Phdr), 0);
    fs_read(fd, &ph, sizeof(Elf_Phdr));

    size_t size = ph.p_memsz;
    int nr_page = (size + PGSIZE - 1) / PGSIZE + 1;
    void *page = new_page(nr_page);
    uintptr_t pos = ph.p_vaddr & 0xfffff000;

    if (ph.p_type == 1) {
      for (int j = 0; j < nr_page; j++) {
        map(&pcb->as, (void *)pos + j * PGSIZE, page + j * PGSIZE, 0);
        // printf("%x\n", pos + j * PGSIZE);
      }
      pcb->max_brk = pcb->max_brk > pos + nr_page * PGSIZE ? pcb->max_brk : pos + nr_page * PGSIZE;
      fs_lseek(fd, ph.p_offset, 0);
      fs_read(fd, (void *)((uintptr_t)ph.p_vaddr & 0xfff) + (uintptr_t)page, ph.p_filesz);
      memset((void *)(ph.p_vaddr & 0xfff) + ph.p_filesz + (uintptr_t)page, 0, ph.p_memsz - ph.p_filesz);
    }
  }
  return eh.e_entry;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %p", entry);
  ((void(*)())entry) ();
}
