#include "bootpack.h"
#include <stdio.h>

void init_pic(void)
{

io_out8(PIC0_IMR,0xff);  // forbid all interrupt
io_out8(PIC1_IMR,0xff);

io_out8(PIC0_ICW1,0x11);
io_out8(PIC0_ICW2,0x20);// receive by INT20-27 
io_out8(PIC0_ICW3,1 << 2); // 00000100 , the master 2th irq
io_out8(PIC0_ICW4, 0x01);

io_out8(PIC1_ICW1,0x11);
io_out8(PIC1_ICW2,0x28);
io_out8(PIC1_ICW3,2); // pci1 connect to irq2
io_out8(PIC1_ICW4, 0x01); // no buffer mode 

io_out8(PIC0_IMR,0xfb);
io_out8(PIC1_IMR,0xff);
return;
}


void inthandler27(int *esp)
{
	io_out8(PIC0_OCW2, 0x67); /* IRQ-07�t������PIC�ɒʒm */
	return;
}

int *inthandler0c(int *esp)
{
	struct CONSOLE *cons = (struct CONSOLE *) *((int *) 0x0fec);
	struct TASK *task = task_now();
	char s[30];
	cons_putstr0(cons, "\nINT 0C :\n Stack Exception.\n");
	sprintf(s, "EIP = %08X\n", esp[11]);
	cons_putstr0(cons, s);
	return &(task->tss.esp0);
}


