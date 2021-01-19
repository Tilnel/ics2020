#include <proc.h>
extern void naive_uload(PCB *pcb, const char *filename);
void context_kload(PCB *p, void (*entry)(void *), void *arg);
void context_uload(PCB *p, char *filename);
Context* kcontext(Area kstack, void (*entry)(void *), void *arg);
uintptr_t loader(PCB *pcb, const char *filename);

#define MAX_NR_PROC 4
static int times = 0;

static PCB pcb[MAX_NR_PROC] __attribute__((used)) = {};
static PCB pcb_boot = {};
PCB *current = NULL;

void switch_boot_pcb() { current = &pcb_boot; }

void hello_fun(void *arg) {
    int j = 1;
    while (1) {
        Log("Hello World from Nanos-lite with arg '%p' for the %dth time!",
            (uintptr_t)arg, j);
        j++;
        yield();
    }
}

void init_proc() {
    context_kload(&pcb[0], hello_fun, "abc");
    context_uload(&pcb[1], "/bin/pal");
    switch_boot_pcb();

    Log("Initializing processes...");

    // load program here
    // naive_uload(NULL, "/bin/pal/pal-x86");
    naive_uload(NULL, "/bin/dummy"); // pal/pal-x86");
}

Context *schedule(Context *prev) {
    current->cp = prev;
    current = &pcb[times & 1];
    times++;
    return current->cp;
}

void context_kload(PCB *p, void (*entry)(void *), void *arg) {
  p->as.area.end = p->stack + 32768;
  p->as.area.start = p->stack;
  p->cp = kcontext(p->as.area, entry, arg);
}

void context_uload(PCB *p, char *filename) {
    void *entry = (void *)loader(p, filename);
    ucontext(&(p->as), pcb[0].as.area, entry);
}