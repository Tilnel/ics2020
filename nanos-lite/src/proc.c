#include <proc.h>
extern void naive_uload(PCB *pcb, const char *filename);
void context_kload(PCB *p, void (*entry)(void *), void *arg);
int context_uload(PCB *p, const char *filename, char *const argv[],
                  char *const envp[]);
Context *kcontext(Area kstack, void (*entry)(void *), void *arg);
uintptr_t loader(PCB *pcb, const char *filename);

#define MAX_NR_PROC 4

static PCB pcb[MAX_NR_PROC] __attribute__((used)) = {};
static PCB pcb_boot = {};
PCB *current = NULL;

void switch_boot_pcb() { current = &pcb_boot; }

void hello_fun(void *arg) {
    int j = 1;
    while (1) {
        if (!(j & 0xfff))
            Log("Hello World from Nanos-lite with arg '%p' for the %dth time!",
                (uintptr_t)arg, j);
        j++;
        yield();
    }
}

void init_proc() {
    // static char *argvv[] = {"/bin/nterm"};
    // context_uload(&pcb[1], "/bin/nterm", argvv, NULL);

    static char *argvv[] = {"/bin/pal", "--skip"};
    context_uload(&pcb[1], "/bin/pal", argvv, NULL);

    context_kload(&pcb[0], hello_fun, "abc");

    switch_boot_pcb();

    Log("Initializing processes...");
}

Context *schedule(Context *prev) {
    current->cp = prev;
    current = (current == &pcb[0])? &pcb[1] : &pcb[0];
    return current->cp;
}

void context_kload(PCB *p, void (*entry)(void *), void *arg) {
    p->as.area.end = p->stack + 32768;
    p->as.area.start = p->stack;
    p->cp = kcontext(p->as.area, entry, arg);
}

void setargs(PCB *p, const char *filename, char *const argv[], char *const envp[]) {
    char *args = (char *)p->cp->eax - 0x100;

    int len = 0;
    int argc = 0;
    while(argv[argc] && argv[argc][0]) len += strlen(argv[argc]) + 1, argc++;
    char *str = (char *)p->cp->eax - len;
    int pos = 0;
    for (int i = 0; i < argc; i++) {
        strcpy(str + pos, argv[i]);
        ((uintptr_t *)args)[i] = (uintptr_t)str + pos;
        pos += strlen(argv[i]) + 1;
    }
    uintptr_t pargc = (uintptr_t)args - 4;
    ((uint32_t *)pargc)[0] = argc;

    p->cp->eax = pargc;
}

int context_uload(PCB *p, const char *filename, char *const argv[],
                  char *const envp[]) {
    Area kstack;
    kstack.start = p->stack;
    kstack.end = p->stack + 32768;

    void *entry = (void *)loader(p, filename);
    if (!entry)
        return -1;
    Log("Jump to %x\n", entry); 

    p->cp = ucontext(&(p->as), kstack, entry, argv, envp);
    p->cp->eax = (uintptr_t)new_page(8);
    setargs(p, filename, argv, envp);
    return 0;
}

int sys_execve(const char *filename, char *const argv[], char *const envp[]) {
    if (context_uload(current, filename, argv, envp) == -1)
        return -2;
    switch_boot_pcb();
    // cnt++;
    yield();
    return -2;
}