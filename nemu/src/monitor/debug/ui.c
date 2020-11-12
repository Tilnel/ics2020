#include <isa.h>
#include "expr.h"
#include "watchpoint.h"
#include <memory/paddr.h> 
#include <monitor/monitor.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint64_t);
int is_batch_mode();

/* We use the `readline' library to provide more flexibility to read from stdin. */
static char* rl_gets() {
  static char *line_read = NULL;

  if (line_read) {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(nemu) ");

  if (line_read && *line_read) {
    add_history(line_read);
  }

  return line_read;
}

static int cmd_c(char *args) {
  cpu_exec(-1);
  return 0;
}


static int cmd_q(char *args) {
  nemu_state.state = NEMU_QUIT;
  //exit(0);
  return -1;
}

static int cmd_help(char *args);

static int cmd_si(char *args);

static int cmd_info(char *args);

static int cmd_p(char *args);

static int cmd_x(char *args);

static int cmd_w(char *args);

static int cmd_d(char *args);

static struct {
  char *name;
  char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "Display informations about all supported commands", cmd_help },
  { "c", "Continue the execution of the program", cmd_c },
  { "q", "Exit NEMU", cmd_q },
  { "si", "Let the program step into N instructions and then pause execution. When N is not given, the default is 1.", cmd_si },
  { "info", "Display the state of registers or th watchpoint.", cmd_info },
  { "p", "Calculate the value of the expression.", cmd_p },
  { "x", "Calculate the value of the expression as the start address, and display N continuous 4-bits in hexadecimal form.", cmd_x },
  { "w", "When the value of the expression changes, suspend the program.", cmd_w },
  { "d", "Delete the watchpoint numbered N.", cmd_d },
  /* TODO: Add more commands */

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i ++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else {
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}

static int cmd_si(char *args) {
  char *arg = strtok(args, " ");
  int i;

  if (arg == NULL) {
    cpu_exec(1);
  }
  else {
    i = atoi(arg);
    cpu_exec(i);
  }
  return 0;
}

static int cmd_info(char *args){
  char *arg = strtok(args, " ");
  if (arg == NULL) {
    printf("No Arguments!\n");
  }
  else {
    if (arg[0] == 'r'){
      isa_reg_display();
    }
    else if (arg[0] == 'w') {
      wp_display();
    }
    else {
      printf("Unavailable command!\n");
      printf("Usage: info r: display the status of registers.\n");
      printf("       info w: display the status of watchpoints.\n");
    }
  }
  return 0;
}

static int cmd_p(char *args){
  bool flag = true;
  sword_t answer = expr(args, &flag);
  if (flag) printf("%x\n", answer);  
  return 0;
}

static int cmd_x(char *args){
  char *arg = strtok(args, " ");
  char *arg2 = arg + strlen(arg) + 1;
  int times = atoi(arg);
  paddr_t addr = (paddr_t)strtol(arg2, NULL, 0);
  for (int i = 0; i < times; i++){
    printf("%x\n",(uint32_t)paddr_read(addr + i, 1)); 
       
  }
  return 0;
}

static int cmd_w(char *args){
  char *arg = strtok(args, " ");
  bool flag = true;
  expr(arg, &flag);
  
  if (flag) {
    new_wp(arg);
  } else {
    printf("Please input an available expression.\n");
  }
  return 0;
}

static int cmd_d(char *args){
  char *arg = strtok(args, " ");
  bool flag = true;
  int id = expr(arg, &flag);
  free_wp(id);

  return 0;
}



void ui_mainloop() {
  char lastbuf[256];
  lastbuf[0] = '\0';
  if (is_batch_mode()) {
    cmd_c(NULL);
    return;
  }

  for (char *str; (str = rl_gets());) {// != NULL; ) {
    char *str_end = str + strlen(str);
    if (str != NULL) strcpy(lastbuf, str);
    printf("%s", str);
    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    if (cmd == NULL) { 
      strcpy(str, lastbuf); 
      cmd = strtok(str, " "); 
      if (cmd == NULL) { 
        lastbuf[0] = '\0';
        continue; 
      }
    }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end) {
      args = NULL;
    }

#ifdef HAS_IOE
    extern void sdl_clear_event_queue();
    sdl_clear_event_queue();
#endif

    int i;
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(cmd, cmd_table[i].name) == 0) {
        if (cmd_table[i].handler(args) < 0) {return; }
        break;
      }
    }

    if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
  }
}
