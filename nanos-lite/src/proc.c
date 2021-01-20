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
            (uintptr_t)arg, j);
        j++;
        yield();
    }
}

void init_proc() {
    static char *argvv[] = {"/bin/nterm"};
    context_kload(&pcb[0], hello_fun, "abc");
    context_uload(&pcb[1], "/bin/nterm", argvv, NULL);
    // context_uload(&pcb[1], "/bin/hello", argvv, NULL);
    switch_boot_pcb();

    Log("Initializing processes...");

    // load program here
    // naive_uload(NULL, "/bin/pal/pal-x86");
    // naive_uload(NULL, "/bin/dummy"); // pal/pal-x86");
}

Context *schedule(Context *prev) {
    current->cp = prev;
    current = &pcb[cnt & 1];
    cnt ++;
    return current->cp;
}

void context_kload(PCB *p, void (*entry)(void *), void *arg) {
    p->as.area.end = p->stack + 32768;
    p->as.area.start = p->stack;
    p->cp = kcontext(p->as.area, entry, arg);
}

int context_uload(PCB *p, const char *filename, char *const argv[],
                   char *const envp[]) {
    p->as.area.start = new_page(8);
    p->as.area.end = p->as.area.start + 32768;
    // printf("pile %x\n", p->as.area.end);
    printf("uld%s\n%s\n", argv[0], argv[1]);
    void *entry = (void *)loader(p, filename);
    if (!entry) return -1;
    printf("%s\n%s\n", argv[0], argv[1]);
    assert(0);
    p->cp = ucontext(&(p->as), pcb[0].as.area, entry, argv, envp);
    return 0;
}

int sys_execve(const char *filename, char *const argv[], char *const envp[]) {
    printf("cve%s\n%s\n", argv[0], argv[1]);
    if (context_uload(&(pcb[1]), filename, argv, envp) == -1) return -2;
    switch_boot_pcb();
    // cnt++;
    yield();
    return -2;
}