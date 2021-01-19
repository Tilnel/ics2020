#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

int main(int argc, char *argv[], char *envp[]);
extern char **environ;
void call_main(uintptr_t *args) {
  char *empty[] =  {NULL };
  environ = empty;
  assert(0);
  exit(main(0, empty, empty));
}

// void call_main(int argc, char *argv[], char *envp[]) {
//   printf("%call s\n", argv[1]);
//   exit(main(argc, argv, envp));
// }