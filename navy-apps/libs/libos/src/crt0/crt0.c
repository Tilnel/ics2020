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
  printf("argc %d\n", argc);
  printf("arg1 %s\n", argv[1]);
  exit(main(argc, argv, envp));
}