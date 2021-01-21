#include <nterm.h>
#include <stdarg.h>
#include <unistd.h>
#include <SDL.h>

char handle_key(SDL_Event *ev);

static void sh_printf(const char *format, ...) {
  static char buf[256] = {};
  va_list ap;
  va_start(ap, format);
  int len = vsnprintf(buf, 256, format, ap);
  va_end(ap);
  term->write(buf, len);
}

static void sh_banner() {
  sh_printf("Built-in Shell in NTerm (NJU Terminal)\n\n");
}

static void sh_prompt() {
  sh_printf("sh> ");
}
char path1[32], path2[32];
static void sh_handle_cmd(const char *cmd) {
  static char buf[64];
  memset(buf, 0, 64);
  buf[strcpy(buf, cmd) - 1] = 0;

  static char args[5][32] = {};
  memset(args, 0, 160);
  static char *argv[5];

  for (int i = 0; i < 5; i++) args[i][0] = 0;
  sscanf(buf, "%s %s %s %s %s", args[0], args[1], args[2], args[3], args[4]);
  static char name[16];
  strcpy(name, args[0]);
  for (int i = 0; i < 5; i++) {
    if (args[i][0] == 0) argv[i] = NULL;
    else argv[i] = args[i];
    printf("%s\n", argv[i]);
  }
  // printf("%s\n", argv[1]);
  // printf("%s", buf);
  sprintf(path1, "/bin/");
  sprintf(path2, "/usr/bin/");
  execv(name, argv);
  strcat(path1, argv[0]);
  execv(path1, argv); 
  strcat(path2, argv[0]);
  execv(path2, argv); 
}

void builtin_sh_run() {
  sh_banner();
  sh_prompt();

  while (1) {
    SDL_Event ev;
    if (SDL_PollEvent(&ev)) {
      if (ev.type == SDL_KEYUP || ev.type == SDL_KEYDOWN) {
        const char *res = term->keypress(handle_key(&ev));
        if (res) {
          sh_handle_cmd(res);
          sh_prompt();
        }
      }
    }
    refresh_terminal();
  }
}
