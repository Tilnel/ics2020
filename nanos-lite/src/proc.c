#include <proc.h>
extern void naive_uload(PCB *pcb, const char *filename);
void context_kload(PCB *p, void (*entry)(void *), void *arg);
int context_uload(PCB *p, const char *filename, char *const argv[],
                  char *const envp[]);
Context *kcontext(Area kstack, void (*entry)(void *), void *arg);
uintptr_t loader(PCB *pcb, const char *filename);

#define MAX_NR_PROC 4

static int cnt = 1;
static PCB pcb[MAX_NR_PROC] __attribute__((used)) = {};
static PCB pcb_boot = {};
PCB *current = NULL;

void switch_boot_pcb() { current = &pcb_boot; }

void hello_fun(void *arg) {
    int j = 1;
    while (1) {
        if (!(j & 0xfff))
            Log("Hello World from Nanos-lite with arg '%p' for the %dth time!",
                (uintptr_t)arg, j >> 12);
        j++;
        yield();
    }
}

void init_proc() {
    // static char *argvv[] = {"/bin/nterm"};
    // context_uload(&pcb[1], "/bin/nterm", argvv, NULL);

    static char *argvv[] = {"/bin/pal", "--skip"};
    context_uload(&pcb[1], "/bin/pal", argvv, NULL);

    context_uload(&pcb[2], "/bin/bird", NULL, NULL);

    context_uload(&pcb[3], "/bin/nslider", NULL, NULL);

    context_kload(&pcb[0], hello_fun, "abc");

    switch_boot_pcb();

    Log("Initializing processes...");
}

Context *schedule(Context *prev) {
    current->cp = prev;
    current = (current == &pcb[0])? &pcb[cnt] : &pcb[0];
    // current = &pcb[1];
    return current->cp;
}

void context_kload(PCB *p, void (*entry)(void *), void *arg) {
    Area kstack = {.start = p->stack, .end = p->stack + 8 * PGSIZE};
    p->cp = kcontext(kstack, entry, arg);
}

void setargs(PCB *p, char *const argv[], char *const envp[]) {
    char *args = (char *)p->cp->eax - 0x100;
    int len = 0;
    int argc = 0;
    while((uint32_t)argv[argc] >= 0x100000 && (uint32_t)argv[argc] <= 0x8000000 && argv[argc][0]) len += strlen(argv[argc]) + 1, argc++;
    char *str = (char *)p->cp->eax - len;
    int pos = 0;
    for (int i = 0; i < argc; i++) {
        strcpy(str + pos, argv[i]);
        ((uintptr_t *)args)[i] = (uintptr_t)str + pos;
        pos += strlen(argv[i]) + 1;
    }
    for (int i = argc; i < 5; i++) {
        ((uintptr_t *)args)[i] = 0;
    }
    uintptr_t pargc = (uintptr_t)args - 4;
    ((uint32_t *)pargc)[0] = argc;

    p->cp->eax = pargc;
    // printf("argc %d\n", argc);
}

int context_uload(PCB *p, const char *filename, char *const argv[],
                  char *const envp[]) {
    protect(&p->as);
    Area kstack = {.start = p->stack, .end = p->stack + 8 * PGSIZE};

    void *entry = (void *)loader(p, filename);
    if (!entry)
        return -1;
    Log("Jump to %x\n", entry); 
    Log("Running user proc %d\n.", cnt);
    void *stack = new_page(8);
    p->max_brk = p->max_brk > (uintptr_t)stack + 8 * PGSIZE ? p->max_brk : (uintptr_t)stack + 8 * PGSIZE;
    for (int i = 0; i < 8; i++) {
        map(&p->as, p->as.area.end - (8 - i) * PGSIZE, stack + i * PGSIZE, 0);
    }
    p->cp = ucontext(&(p->as), kstack, entry);
    p->cp->eax = (uintptr_t)stack + 8 * PGSIZE;
    setargs(p, argv, envp);
    return 0;
}

int sys_execve(const char *filename, char *const argv[], char *const envp[]) {
    char *args[5];
    int argc = 0;
    while(argv[argc] && argv[argc][0]) {
        args[argc] = malloc(strlen(argv[argc]) + 1);
        strcpy(args[argc], argv[argc]);
        argc++;
    }
    if (context_uload(&pcb[cnt + 1], filename, args, envp) == -1)
        return -2;
    switch_boot_pcb();
    // cnt++;
    yield();
    return -2;
}