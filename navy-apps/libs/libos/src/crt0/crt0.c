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

  char* nargv[] = {*(&argc + 1), *(&argc + 2), *(&argc + 3), *(&argc + 4), *(&argc + 5)};
  exit(main(argc, nargv, envp));
}