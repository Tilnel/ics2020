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
        if (!(j & 0xfffff))
            Log("Hello World from Nanos-lite with arg '%p' for the %dth time!",
                (uintptr_t)arg, j);
        j++;
        // printf("%d\n", j);
        yield();
    }
}

void init_proc() {
    static char *argvv[] = {"/bin/nterm"};
    context_uload(&pcb[1], "/bin/nterm", argvv, NULL);

    context_kload(&pcb[0], hello_fun, "abc");

    // static char *argvv[] = {"/bin/pal", "--skip"};
    // context_uload(&pcb[1], "/bin/pal", argvv, NULL);

    switch_boot_pcb();
    Log("Initializing processes...");
}

Context *schedule(Context *prev) {
    assert((pcb[0].cp->cs & 0x3) == 0);
    // assert((pcb[1].cp->cs & 0x3) == 3);
    current->cp = prev;
    current = (current == &pcb[0])? &pcb[cnt] : &pcb[0];
    // current = &pcb[1];
    return current->cp;
}

void context_kload(PCB *p, void (*entry)(void *), void *arg) {
    Area kstack = {.start = p->stack, .end = p->stack + 8 * PGSIZE};
    p->cp = kcontext(kstack, entry, arg);
}

void setargs(PCB *p, char *const argv[], char *const envp[], void *stack) {
    char *args = stack + 8 * PGSIZE - 0x100 + 4;
    int len = 0;
    int argc = 0;
    while((uint32_t)argv[argc] && argv[argc][0]) len += strlen(argv[argc]) + 1, argc++;
    char *str = args + 0x40;
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
    Log("Jump to %x", entry); 
    Log("Running user proc %d.", cnt);
    void *stack = new_page(8);
    p->max_brk = p->max_brk > (uintptr_t)stack + 8 * PGSIZE ? p->max_brk : (uintptr_t)stack + 8 * PGSIZE;
    for (int i = 0; i < 8; i++) {
        map(&p->as, p->as.area.end - (8 - i) * PGSIZE, stack + i * PGSIZE, 0);
    }
    p->cp = ucontext(&(p->as), kstack, entry);
    p->cp->esp3 = (uintptr_t)p->as.area.end - 0x100;
    printf("esp3 %x\n", p->cp->esp3);
    setargs(p, argv, envp, stack);
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
    cnt++;
    switch_boot_pcb();
    yield();
    return -2;
}