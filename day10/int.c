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

// keyboard handler
#define PORT_KEYDAT 0x0060
struct FIFO8 keyfifo;

void inthandler21(int * esp)
{
	//struct BOOTINFO *binfo = (struct BOOTINFO *)  ADR_BOOTINFO;
	unsigned char data;
	io_out8(PIC0_OCW2,0x61);
	data= io_in8(PORT_KEYDAT);
	fifo8_put(&keyfifo,data);
	return;
}

struct FIFO8 mousefifo;
void inthandler2c(int *esp)
{
	unsigned char data;
	io_out8(PIC1_OCW2,0x64); // notify 4
	io_out8(PIC0_OCW2,0x62); // notify irq2
	data = io_in8(PORT_KEYDAT);
	fifo8_put(&mousefifo,data);
	return;
}

