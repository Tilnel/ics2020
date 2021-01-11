#include <fs.h>

typedef size_t (*ReadFn) (void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn) (const void *buf, size_t offset, size_t len);
size_t ramdisk_read(void *buf, size_t offset, size_t len);
size_t ramdisk_write(const void *buf, size_t offset, size_t len);
size_t serial_write(const void *buf, size_t offset, size_t len);
size_t events_read(void *buf, size_t offset, size_t len);
size_t fb_write(const void *buf, size_t offset, size_t len);
size_t dispinfo_read(void *buf, size_t offset, size_t len);
size_t dispinfo_write(const void *buf, size_t offset, size_t len);

typedef struct {
  char *name;
  size_t size;
  size_t disk_offset;
  size_t open_offset;
  ReadFn read;
  WriteFn write;
} Finfo;
AM_GPU_CONFIG_T gpuconf;

enum {FD_STDIN, FD_STDOUT, FD_STDERR, FD_EVENT, FD_DISP, FD_FB};

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
  [FD_STDIN]  = {"stdin", 0, 0, 0, invalid_read, invalid_write},
  [FD_STDOUT] = {"stdout", 0, 0, 0, invalid_read, serial_write},
  [FD_STDERR] = {"stderr", 0, 0, 0, invalid_read, serial_write},
  [FD_EVENT]  = {"/dev/events", 0, 0, 0, events_read, invalid_write},
  [FD_DISP]   = {"/proc/dispinfo", 0, 0, 0, dispinfo_read, dispinfo_write},
  [FD_FB]     = {"/dev/fb", 0, 0, 0, invalid_read, fb_write},
#include "files.h"
};

void init_fs() {
  file_table[FD_FB].size = gpuconf.width * gpuconf.height * 4;
  // TODO: initialize the size of /dev/fb
}

int fs_open(const char *pathname, int flags, int mode) {
  int i;
  for (i = 0; i <= 5; i++) {
    if (!strcmp(pathname, file_table[i].name)) return i;
  }
  for (i = 6; i * sizeof(Finfo) < sizeof(file_table); i++) {
    if (!strcmp(pathname, file_table[i].name)) {
      file_table[i].read = ramdisk_read;
      file_table[i].write = ramdisk_write;
      return i;
    }
  }
  assert(0);
  return 0;
}

size_t fs_read(int fd, void *buf, size_t len) {
  // assert(file_table[fd].read == 1);
  if (fd >= 6 && file_table[fd].open_offset > file_table[fd].size) return 0;
  size_t offset = file_table[fd].disk_offset + file_table[fd].open_offset;
  int ret = file_table[fd].read(buf, offset, len);
  fs_lseek(fd, len, 1);
  return ret;
}

size_t fs_write(int fd, const void *buf, size_t count) {
  // printf("%d %d %d\n", fd, (intptr_t)buf, count);
  // printf("Write got it\n");
  size_t offset = file_table[fd].disk_offset + file_table[fd].open_offset;
  int ret = file_table[fd].write(buf, offset, count);
  fs_lseek(fd, ret, 1);
  return ret;
}

size_t fs_lseek(int fd, size_t offset, int whence) {
  size_t tmp;
  switch(whence) {
    case SEEK_SET:
      tmp = offset; break;
    case SEEK_CUR:
      tmp = file_table[fd].open_offset + offset; break;
    case SEEK_END:
      tmp = file_table[fd].size + offset; break;
    default:
      assert(0);
  }
  // if (fd >= 6 && tmp > file_table[fd].size) { printf("0"); return -1; }
  return file_table[fd].open_offset = tmp;
}

int fs_close(int fd) {
  return 0;
}