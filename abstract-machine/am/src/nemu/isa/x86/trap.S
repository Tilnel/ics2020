#----|------------entry------------|---irq id---|-----handler-----|
.globl __am_vecsys;    __am_vecsys: pushl $0x80; jmp __am_asm_trap
.globl __am_vectrap;  __am_vectrap: pushl $0x81; jmp __am_asm_trap
.globl __am_irq0;        __am_irq0: pushl   $32; jmp __am_asm_trap
.globl __am_vecnull;  __am_vecnull: pushl   $-1; jmp __am_asm_trap


__am_asm_trap:

  pushal

  pushl $0

  movl 0x2c(%esp), %eax
  andl $0x3, %eax
  je b
  movl %esp, 0x34(%esp)
  movl ksp, %esp

  b:
  pushl %esp

  call __am_irq_handle

  movl %eax, %esp

  addl $4, %esp

  popal

  addl $4, %esp

  iret
