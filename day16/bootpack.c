#include "bootpack.h"
#include <stdio.h>

void task_b_main(struct SHEET * sht_back);
// the main function 
void HariMain(void)
{
	char *vram;
	int xsize;
	int ysize;
	unsigned int memtotal;
	struct MEMMAN *memman = (struct MEMMAN *) MEMMAN_ADDR ;
	struct BOOTINFO *binfo;	
	struct SHTCTL *shtctl;
	struct SHEET *sht_back, *sht_mouse, *sht_win,*sht_winb[3];
	unsigned char *buf_back, buf_mouse[256],*buf_win,*winb;
	int i;
	int fifobuf[128];
	char s[40];
	struct FIFO fifo;
	fifo_init(&fifo,128,fifobuf,0);
	static char keytable[0x54] = {
		0,   0,   '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '^', 0,   0,
		'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '@', '[', 0,   0,   'A', 'S',
		'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', ':', 0,   0,   ']', 'Z', 'X', 'C', 'V',
		'B', 'N', 'M', ',', '.', '/', 0,   '*', 0,   ' ', 0,   0,   0,   0,   0,   0,
		0,   0,   0,   0,   0,   0,   0,   '7', '8', '9', '-', '4', '5', '6', '+', '1',
		'2', '3', '0', '.'
	};

	
	init_gdtidt();
	init_pic();
	io_sti();
	
	init_pit(); // timer
	io_out8(PIC0_IMR, 0xf8); /* PIC1 (11111001) irq0 */
	io_out8(PIC1_IMR, 0xef); //11101111 , allow irq12 
	struct TIMER *timer3;
	timer3 = timer_alloc();
	timer_set(timer3,50,&fifo,1);
	
	init_keyboard(&fifo,256);	
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
	sheet_setbuf(sht_win, buf_win,144,52,-1);


	/* initlize OS background */
	init_screen(buf_back,xsize,ysize);
	
	int mx,my; // mouse position
	// mouse cursor
	struct MOUSE_DEC mdec;
	enable_mouse(&fifo,512,&mdec); 
	init_mouse_cursor8(buf_mouse,99);
	// window a setting
	make_window8(buf_win,144,52,"task a",1);
	int cursor_x,cursor_c;  // cursor color
	make_textbox8(sht_win, 8,28,128,16,COL8_FFFFFF); // heigth is 16
	cursor_x = 8;
	cursor_c = COL8_FFFFFF;
	mx = (binfo->scrnx - 16) / 2;
	my = (binfo->scrny - 28 - 16) / 2;

	// task block
	struct TASK *b,*curr,*tw[3];
	curr = task_init(memman);
	fifo.task = curr;
	taskrun(curr,1,0); // set to level 1
	unsigned char *buf;	
	for(i=0;i<3;i++)
	{
		sht_winb[i] = sheet_alloc(shtctl);
		buf = (unsigned char *) memman_alloc_4k(memman, 144*52);
		sheet_setbuf(sht_winb[i],buf,144,52,-1);
		sprintf(s,"task_b%d",i);
		make_window8(buf,144,52,s,0);
		tw[i] = task_alloc();
		tw[i]->tss.esp = memman_alloc_4k(memman,64*1024) + 64 * 1024 -8;
		tw[i]->tss.eip = (int) &task_b_main;
		tw[i]->tss.es = 1*8;
		tw[i]->tss.cs = 2*8;
		tw[i]->tss.ss = 1*8;
		tw[i]->tss.ds = 1*8;
		tw[i]->tss.fs = 1*8;
		tw[i]->tss.gs = 1*8;
		*((int *) (tw[i]->tss.esp + 4)) = (int) sht_winb[i]; // memory share
		taskrun(tw[i],2,i+1); // level2,different priorities 
	}


	sheet_move(sht_back,0,0);// move background sheet 
	sheet_move(sht_mouse,mx,my);
	sheet_move(sht_win,8,56);
	sheet_move(sht_winb[0],168,56);
	sheet_move(sht_winb[1],8,116);
	sheet_move(sht_winb[2],168,116);


	sheet_updown(sht_back,0); // height 0, at the bottom
	sheet_updown(sht_winb[0],1);
	sheet_updown(sht_winb[1],2);
	sheet_updown(sht_winb[2],3);
	sheet_updown(sht_win,4);
	sheet_updown(sht_mouse,5);
	
	sprintf(s,"%3d %3d",mx,my);
	putfonts8_asc(binfo->vram, binfo->scrnx, 0, 0, COL8_FFFFFF, s);
	sprintf(s, "memory %dMB free %dkb", memtotal/(1024*1024), memman_total(memman)/1024);
	putfonts8_asc(buf_back, binfo->scrnx, 0, 32, COL8_FFFFFF, s);
	sheet_refresh(sht_back,0,0,binfo->scrnx,48);


	for(;;)
	{
	io_cli(); // forbid all interrupts
	if(fifo_status(&fifo) == 0) 
		{task_sleep(curr);
		io_sti();
		}
	else {
		i=fifo_get(&fifo);
		io_sti(); // open interrupts
		if(255 < i && i < 512)
		{sprintf(s, "%02X", i-256);
		boxfill8(buf_back, binfo->scrnx, COL8_008400, 0, 16, 15, 31);
		putfonts8_asc(buf_back, binfo->scrnx, 0, 16, COL8_FFFFFF, s);
		sheet_refresh(sht_back,0,16,16,32);
		if(i<256 +0x54)
			{
			if(keytable[i-256] != 0 && cursor_x < 144)
				{
					s[0]= keytable[i-256];
					s[1]= '\0';
					bps(sht_win,cursor_x,28, COL8_FFFFFF,COL8_000000,s,1); // 4th parm is background color 
					cursor_x +=8; // cursor move forward
				}
			}
		if(i == 256 + 0x0e && cursor_x > 8)
			{
				bps(sht_win,cursor_x,28,COL8_FFFFFF,COL8_000000," ",1);
				cursor_x -=8;
			
			}
		boxfill8(sht_win ->buf, sht_win->bxsize, cursor_c,cursor_x,28,cursor_x+7,43);
		sheet_refresh(sht_win,cursor_x,28,cursor_x+8,44);

		}

		else if(511 < i && i < 768)
		{
		
		if(mouse_decode(&mdec,i-512) != 0) 
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
			// move the window, vram -> sht -> move
		 	if((mdec.btn & 0x01) != 0)
				sheet_move(sht_win,mx-80,my-8);
		  }
		}

		else if(i == 1)
			{
				timer_set(timer3,50,&fifo,0);
				boxfill8(sht_win->buf,sht_win->bxsize, COL8_000000, cursor_x, 28,cursor_x+7, 43);
				sheet_refresh(sht_win,cursor_x,28,cursor_x + 8, 44);
			}
		else if(i==0)
			{
				timer_set(timer3,50,&fifo,1); // keep the data
				boxfill8(sht_win->buf,sht_win->bxsize, COL8_FFFFFF,cursor_x, 28,cursor_x + 7, 43);
				sheet_refresh(sht_win,cursor_x,28,cursor_x + 8, 44);
			}
			
	     }

	}
}


// sht: a sheet
void task_b_main(struct SHEET * sht)
{

struct FIFO fifo;
struct TIMER *timer;
int i, fifobuf[128];
int count=0;
char s[12];
fifo_init(&fifo,128,fifobuf,0);
timer = timer_alloc();
timer_set(timer,2,&fifo,2);

for(;;)
{
	count++;
	io_cli();
	if(fifo_status(&fifo) == 0)
		io_sti();
	else
	{
	i = fifo_get(&fifo);
	io_sti();
	if(i == 2)
	{	//farjmp(0,3*8);// switch to task a ;
		sprintf(s,"%10d",count);
		bps(sht,24,28, COL8_FFFFFF,COL8_000000,s,10);
		timer_set(timer,2,&fifo,2);
	}
	
	}
}

}
