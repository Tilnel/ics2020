void init_monitor(int, char *[]);
void engine_start();
int is_exit_status_bad();
#include <stdio.h>
#include <stdint.h>
//#include "monitor/debug/expr.h"
//char buf[65536];

int main(int argc, char *argv[]) {
  /* Initialize the monitor. */
  init_monitor(argc, argv);

   
  //added, can be deleted
 /* FILE *fp = fopen("./tools/gen-expr/input", "r");
  assert (fp!= NULL);
  uint32_t ans;
  bool flag, res = true;
  while (fscanf(fp, "%u %s", &ans, buf) != EOF){
    flag = true;
    if (ans != expr(buf, &flag)) {
      printf("Wrong answer! %s == %u\n", buf, ans);
      res = false;
      break;
    }
  }
  if (res) printf("Accepted\n");
 */ 
 /* Start engine. */
    engine_start();

  return is_exit_status_bad();
}
