#include "bootpack.h"
#include <stdio.h>


#define MEMMAN_ADDR 0x003c0000
extern struct FIFO8 keyfifo;
extern struct FIFO8 mousefifo;
unsigned int memtest(unsigned int start, unsigned int end);

void HariMain(void)
{
	char *vram;
	int xsize;
	int ysize;
	unsigned int memtotal;
	struct MEMMAN *memman = (struct MEMMAN *) MEMMAN_ADDR ;
	struct BOOTINFO *binfo;
	
	struct SHTCTL *shtctl;
	struct SHEET *sht_back, *sht_mouse, *sht_win;
	unsigned char *buf_back, buf_mouse[256],*buf_win;
	int i;
	char keybuf[32],mousebuf[128];
	char s[40],timerbuf[8],timerbuf2[8],timerbuf3[8];
	struct FIFO8 timerfifo,timerfifo2,timerfifo3;
	fifo8_init(&timerfifo,8,timerbuf);
	fifo8_init(&timerfifo2,8,timerbuf2);
	fifo8_init(&timerfifo3,8,timerbuf3);

	init_gdtidt();
	init_pic();
	io_sti();
	
	fifo8_init(&keyfifo,32,keybuf);
	fifo8_init(&mousefifo,128,mousebuf);
	init_pit(); // timer
	io_out8(PIC0_IMR, 0xf8); /* PIC1 (11111001) irq0 */
	io_out8(PIC1_IMR, 0xef); //11101111 , allow irq12 
	struct TIMER *timer,*timer2,*timer3;
	timer = timer_alloc();
	timer2 = timer_alloc();
	timer3 = timer_alloc();

	timer_set(timer,1000,&timerfifo,1);
	timer_set(timer2,300,&timerfifo2,1);
	timer_set(timer3,50,&timerfifo3,1);
	
	init_keyboard();	
	// memory test
	memtotal = memtest(0x00400000, 0xbfffffff); // available maxmium  address
	memman_init(memman);
	memman_free(memman,0x00001000,0x0009e000);
	memman_free(memman,0x00400000,memtotal-0x00400000);
		
	init_palette();
	binfo = (struct BOOTINFO *)  0x0ff0;
	xsize = binfo->scrnx;
	ysize = binfo->scrny;
	vram  = binfo->vram;
	// sheet setting
	shtctl = shtctl_init(memman, binfo->vram, binfo->scrnx, binfo->scrny);
	sht_back  = sheet_alloc(shtctl);
	sht_mouse = sheet_alloc(shtctl);
	sht_win   = sheet_alloc(shtctl);
	buf_back  = (unsigned char *) memman_alloc_4k(memman, binfo->scrnx * binfo->scrny);
	buf_win  = (unsigned char *) memman_alloc_4k(memman,160 * 52); // window sheet buffer
	sheet_setbuf(sht_back, buf_back, binfo->scrnx, binfo->scrny, -1);
	sheet_setbuf(sht_mouse, buf_mouse, 16, 16, 99); // col_inv 99
	sheet_setbuf(sht_win, buf_win,160,52,-1);
	/* initlize OS background */
	init_screen(buf_back,xsize,ysize);
	
	int mx,my; // mouse position
	// mouse cursor
	struct MOUSE_DEC mdec;
	enable_mouse(&mdec); 
	init_mouse_cursor8(buf_mouse,99);
	make_window8(buf_win,160,52,"counter");
	//putfonts8_asc(buf_win,160,24,28, COL8_000000,"welcome to");
	
	mx = (binfo->scrnx - 16) / 2;
	my = (binfo->scrny - 28 - 16) / 2;

	sheet_move(sht_back,0,0);// move background sheet 
	sheet_move(sht_mouse,mx,my);
	sheet_move(sht_win,80,72);

	sheet_updown(sht_back,0); // height 0, at the bottom
	sheet_updown(sht_win,1);
	sheet_updown(sht_mouse,2); // height 2,on the top
	
	sprintf(s,"%3d %3d",mx,my);
	putfonts8_asc(binfo->vram, binfo->scrnx, 0, 0, COL8_FFFFFF, s);
	sprintf(s, "memory %dMB free %dkb", memtotal/(1024*1024), memman_total(memman)/1024);
	putfonts8_asc(buf_back, binfo->scrnx, 0, 32, COL8_FFFFFF, s);
	sheet_refresh(sht_back,0,0,binfo->scrnx,48);


	for(;;)
	{
	
	sprintf(s,"%010d",timerctl.count);
	boxfill8(buf_win,160,COL8_C6C6C6,40,28,119,43);
	putfonts8_asc(buf_win,160,40,28, COL8_000000,s);
	sheet_refresh(sht_win,40,28,120,44); // fresh area
	
	io_cli(); // forbid all interrupts
	if(fifo8_status(&keyfifo) + fifo8_status(&mousefifo) + fifo8_status(&timerfifo) +fifo8_status(&timerfifo2) + fifo8_status(&timerfifo3) == 0) 
		io_sti();  // read data until it's empty
	else {
		if(fifo8_status(&keyfifo))
		{
		i=fifo8_get(&keyfifo);
		io_sti(); // open interrupts
		sprintf(s, "%02X", i);
		boxfill8(buf_back, binfo->scrnx, COL8_008484, 0, 16, 15, 31);
		putfonts8_asc(buf_back, binfo->scrnx, 0, 16, COL8_FFFFFF, s);
		sheet_refresh(sht_back,0,16,16,32);
		}

		else if(fifo8_status(&mousefifo)!= 0)
		{
		i=fifo8_get(&mousefifo);
		io_sti(); // open interrupts
		if(mouse_decode(&mdec,i) != 0) 
		  {	sprintf(s, "[lcr %4d %4d]", mdec.x,mdec.y);
			
			if((mdec.btn & 0x01) != 0) s[1]='L';
		        if((mdec.btn & 0x02) != 0) s[3]='R';
		        if((mdec.btn & 0x04) != 0) s[2]='C';	
			boxfill8(buf_back, binfo->scrnx, COL8_008484, 32, 16, 32+15*8-1, 31); // after filling,show ok
			putfonts8_asc(buf_back, binfo->scrnx, 32, 16, COL8_FFFFFF, s);
		 	sheet_refresh(sht_back,32,16,32+15*8,32);
			// the move of mouse 
			mx +=mdec.x;
			my +=mdec.y;
			if(mx <0) mx = 0;
			if(my <0) my = 0;
			if(mx > binfo->scrnx -1 )  mx =  binfo->scrnx -1;
			if(my > binfo->scrny -1 )  my =  binfo->scrny -1;  // control the mouse move area
		 	sprintf(s, "(%3d,%3d)", mx,my);
			boxfill8(buf_back, binfo->scrnx, COL8_008484, 0, 0,79, 15);
			putfonts8_asc(buf_back, binfo->scrnx, 0, 0, COL8_FFFFFF, s);
			sheet_refresh(sht_back,0,0,80,16);
			sheet_move(sht_mouse,mx,my);
		 
		  }
		}

		else if(fifo8_status(&timerfifo) != 0)
		{
		i = fifo8_get(&timerfifo);
		io_sti();
		putfonts8_asc(buf_back, binfo->scrnx,0,64, COL8_FFFFFF,"10");
		sheet_refresh(sht_back,0,0,100,100);
		}
		
		else if(fifo8_status(&timerfifo2) != 0)
		{
		i = fifo8_get(&timerfifo2);
		io_sti();
		//sprintf(s, "[%2d]", "10");
		putfonts8_asc(buf_back, binfo->scrnx,0,80, COL8_FFFFFF,"3s");
		sheet_refresh(sht_back,0,0,100,100);
		}


		else if(fifo8_status(&timerfifo3) != 0)
		{
		i = fifo8_get(&timerfifo3);
		io_sti();
		if(i != 0)
		{
			timer3->data = 0;
			boxfill8(buf_back, binfo->scrnx, COL8_FFFFFF, 8, 96,15, 111);		
		}
		else
		{
			timer3->data = 1;
			boxfill8(buf_back, binfo->scrnx, COL8_008484, 8, 96,15, 111);	
		}
		timer_set(timer3,50,&timerfifo3,timer3->data); // keep the data
		sheet_refresh(sht_back,8, 96,15, 111);
		}
	     }

	}
}
