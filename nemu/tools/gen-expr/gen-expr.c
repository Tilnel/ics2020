#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

// this should be enough
static char buf[65536] = {};
static char code_buf[65536 + 128] = {}; // a little larger than `buf`
static char *code_format =
"#include <stdio.h>\n"
"int main() { "
"  unsigned result = %s; "
"  printf(\"%%u\", result); "
"  return 0; "
"}";

uint32_t choose (uint32_t n) {
  return rand()%n;
}

static int id = 0;
static inline void gen_rand_op() {
  switch (choose(4)) {
    case 0: buf[id] = '+'; break;
    case 1: buf[id] = '-'; break;
    case 2: buf[id] = '*'; break;
    case 3: buf[id] = '/'; break;
    default: assert(0);
  }
  id++;
}

static inline void gen_rand_expr() {
  uint32_t num;
  switch (choose(3)) {
    case 0:
      num = choose(4294967295);
     // num = choose(65536);
      id += sprintf(buf + id, "%u", num);
      break;
    case 1:
      buf[id]='(';
      id++;
      gen_rand_expr(); 
      buf[id]=')';
      id++;
      break;
    default: gen_rand_expr(); gen_rand_op(); gen_rand_expr(); break;
  }
  //buf[id] = '\0';
}

int main(int argc, char *argv[]) {
  int seed = time(0);
  srand(seed);
  int loop = 1;
  if (argc > 1) {
    sscanf(argv[1], "%d", &loop);
  }
  int i;
  for (i = 0; i < loop; i ++) {
    id = 0;
    gen_rand_expr();
    buf[id] = '\0';
    sprintf(code_buf, code_format, buf);

    FILE *fp = fopen("/tmp/.code.c", "w");
    assert(fp != NULL);
    fputs(code_buf, fp);
    fclose(fp);

    int ret = system("gcc /tmp/.code.c -o /tmp/.expr");
    if (ret != 0) continue;

    fp = popen("/tmp/.expr", "r");
    assert(fp != NULL);

    int result;
    fscanf(fp, "%d", &result);
    pclose(fp);

    printf("%u %s\n", result, buf);
  }
  return 0;
}
