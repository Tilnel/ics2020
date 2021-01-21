#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

int main(int argc, char *argv[], char *envp[]);
extern char **environ;
// void call_main(uintptr_t *args) {
//   char *empty[] =  {NULL };
//   environ = empty;
//   exit(main(0, empty, empty));
//   assert(0);
// }

void call_main(int argc, char *argv[], char *envp[]) {
  char *tmp = (char *)(&argc);
  for (int i = 0; i < 256; i++) {
    putchar(tmp[i]);
  }
  printf("\n");
  printf("argc %d\n", argc);
  printf("argv0 %x\n", (char *)(&argv[0]));
  // char* nargv[] = {&argv[0], &argv[1], &arg}
  exit(main(argc, argv, envp));
}