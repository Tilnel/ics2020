#include <fs.h>

typedef size_t (*ReadFn) (void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn) (const void *buf, size_t offset, size_t len);
size_t ramdisk_read(void *buf, size_t offset, size_t len);

typedef struct {
  char *name;
  size_t size;
  size_t disk_offset;
  ReadFn read;
  WriteFn write;
} Finfo;

enum {FD_STDIN, FD_STDOUT, FD_STDERR, FD_FB};

size_t invalid_read(void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

size_t invalid_write(const void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

/* This is the information about all files in disk. */
static Finfo file_table[] __attribute__((used)) = {
  [FD_STDIN]  = {"stdin", 0, 0, invalid_read, invalid_write},
  [FD_STDOUT] = {"stdout", 0, 0, invalid_read, invalid_write},
  [FD_STDERR] = {"stderr", 0, 0, invalid_read, invalid_write},
#include "files.h"
};

void init_fs() {
  // TODO: initialize the size of /dev/fb
}

int fs_open(const char *pathname, int flags, int mode) {
  int i;
  for (i = 3; i * sizeof(Finfo) < sizeof(file_table); i++) {
    if (!strcmp(pathname, file_table[i].name)) {
      // file_table[i].read = (mode / 2)? ramdisk_read : invalid_read;
      // file_table[i].write = (mode & 1)? ramdisk_write : invalid_write;
      return i;
    }
  }
  assert(0);
  return 0;
}

size_t fs_read(int fd, void *buf, size_t len) {
  // assert(file_table[fd].read == 1);
  size_t offset = file_table[fd].disk_offset;
  ramdisk_read(buf, offset, len);
  return len;
}

size_t fs_write(int fd, const void *buf, size_t count) {
  // printf("%d %d %d\n", fd, (intptr_t)buf, count);
  // printf("Write got it\n");

  if (fd == 1 || fd == 2) 
    for (int i = 0; i < count; i++) putch(((char *)buf)[i]); 
  else {
    
  }
  return count;
}