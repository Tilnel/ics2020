#include <am.h>
#include <x86.h>
#include <klib.h>

#define NR_IRQ         256     // IDT size
#define NR_SEG         6       // GDT size
#define SEG_KCODE      1
#define SEG_KDATA      2

static SegDesc gdt[NR_SEG] = {};
static TSS32 tss = {};
void *ksp = &tss.esp0;

static Context* (*user_handler)(Event, Context*) = NULL;
void __am_get_cur_as(Context *c);
void __am_switch(Context *c);
void __am_irq0();
void __am_vecsys();
void __am_vectrap();
void __am_vecnull();


Context* __am_irq_handle(Context *c) {
  assert((uintptr_t)c->cr3 < 0x100000);
  // printf("%x\n", *(int *)ksp);
  __am_get_cur_as(c);
  if (user_handler) {
    Event ev = {0};
    switch (c->irq) {
      case 0x20 : ev.event = EVENT_IRQ_TIMER; break;
      case 0x80 : ev.event = EVENT_SYSCALL; break;
      case 0x81 : ev.event = EVENT_YIELD; break;
      default: ev.event = EVENT_ERROR; break;
    }

    c = user_handler(ev, c);
    assert(c != NULL);
  }
  // __am_switch(c);
  // printf("%x\n", c->cr3);
  // printf("%x\n", c->cs);
  // printf("%x\n", c->esp0);
  // printf("%x\n", c->eip);
  printf("setcr3 to %x\n", get_cr3());
  return c;
}

bool cte_init(Context*(*handler)(Event, Context*)) {
  // initialize GDT
  gdt[1] = SEG32(STA_X | STA_R,    0,     0xffffffff, DPL_KERN);
  gdt[2] = SEG32(STA_W,            0,     0xffffffff, DPL_KERN);
  gdt[3] = SEG32(STA_X | STA_R,    0,     0xffffffff, DPL_USER);
  gdt[4] = SEG32(STA_W ,           0,     0xffffffff, DPL_USER);
  gdt[5] = SEG32(STS_T32A | STA_R, &tss,  sizeof(tss) - 1, DPL_KERN);
  set_gdt(gdt, sizeof(gdt[0]) * NR_SEG);

  // initialize TSS
  tss.ss0 = KSEL(2);
  set_tr(KSEL(5));

  static GateDesc32 idt[NR_IRQ];

  // initialize IDT
  for (unsigned int i = 0; i < NR_IRQ; i ++) {
    idt[i]  = GATE32(STS_TG, KSEL(SEG_KCODE), __am_vecnull, DPL_KERN);
  }

  // ----------------------- interrupts ----------------------------
  idt[32]   = GATE32(STS_IG, KSEL(SEG_KCODE), __am_irq0,    DPL_KERN);
  // ---------------------- system call ----------------------------
  idt[0x80] = GATE32(STS_TG, KSEL(SEG_KCODE), __am_vecsys,  DPL_USER);
  idt[0x81] = GATE32(STS_TG, KSEL(SEG_KCODE), __am_vectrap, DPL_KERN);

  set_idt(idt, sizeof(idt));

  // register event handler
  user_handler = handler;

  return true;
}


Context* kcontext(Area kstack, void (*entry)(void *), void *arg) {
  Context *ret = (void *)(kstack.end - 128);
  tss.esp0 = (uintptr_t) kstack.end;
  ret->cs = KSEL(1);
  ret->cr3 = (void *)get_cr3();
  ret->eax = (int)kstack.end;
  ret->eip = (int)entry;
  ret->esp = (int)(ret);
  ret->eflags |= 0x200;
  ((uint32_t *)ret)[16] = (int)arg;  // under stack 1 byte
  return ret;
}

void yield() {
  // printf("Yield got it\n");
  asm volatile("int $0x81");
}

void *sbrk(intptr_t increment) {
  return (void *)0;
}

bool ienabled() {
  return false;
}

void iset(bool enable) {
}
