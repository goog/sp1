#include "bootpack.h"
#include <stdio.h>
#include <string.h>

struct FILEINFO {
	unsigned char name[8], ext[3], type;
	char reserve[10];
	unsigned short time, date, clustno;
	unsigned int size;
};

int cons_newline(int cursor_y, struct SHEET *sheet);
void task_console(struct SHEET * sht,unsigned int memtotal);

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
	struct SHEET *sht_back, *sht_mouse, *sht_win,*sht_cons;
	unsigned char *buf_back, buf_mouse[256],*buf_win;
	int i;
	int fifobuf[128];
	char s[40];
	struct FIFO fifo;
	fifo_init(&fifo,128,fifobuf,0);
	static char keytable0[0x80] = {
		0,   0,   '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '^', 0,   0,
		'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '@', '[', 0,   0,   'A', 'S',
		'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', ':', 0,   0,   ']', 'Z', 'X', 'C', 'V',
		'B', 'N', 'M', ',', '.', '/', 0,   '*', 0,   ' ', 0,   0,   0,   0,   0,   0,
		0,   0,   0,   0,   0,   0,   0,   '7', '8', '9', '-', '4', '5', '6', '+', '1',
		'2', '3', '0', '.', 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		0,   0,   0,   0x5c, 0,  0,   0,   0,   0,   0,   0,   0,   0,   0x5c, 0,  0
	};
	static char keytable1[0x80] = {
		0,   0,   '!', 0x22, '#', '$', '%', '&', 0x27, '(', ')', '~', '=', '~', 0,   0,
		'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '`', '{', 0,   0,   'A', 'S',
		'D', 'F', 'G', 'H', 'J', 'K', 'L', '+', '*', 0,   0,   '}', 'Z', 'X', 'C', 'V',
		'B', 'N', 'M', '<', '>', '?', 0,   '*', 0,   ' ', 0,   0,   0,   0,   0,   0,
		0,   0,   0,   0,   0,   0,   0,   '7', '8', '9', '-', '4', '5', '6', '+', '1',
		'2', '3', '0', '.', 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		0,   0,   0,   '_', 0,   0,   0,   0,   0,   0,   0,   0,   0,   '|', 0,   0
	};
	
	
	init_gdtidt();
	init_pic();
	io_sti();
	
	init_pit(); // timer
	io_out8(PIC0_IMR, 0xf8); /* PIC1 (11111000) irq0 */
	io_out8(PIC1_IMR, 0xef); //11101111 , allow irq12 
	struct TIMER *timer;
	timer = timer_alloc();
	timer_set(timer,50,&fifo,1);
	
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
	struct TASK *curr,*console;
	curr = task_init(memman);
	fifo.task = curr;
	taskrun(curr,1,0); // set to level 1

	// sheet console
	unsigned char *buf_cons;	
	sht_cons = sheet_alloc(shtctl);
	buf_cons = (unsigned char *) memman_alloc_4k(memman, 256*165);
	sheet_setbuf(sht_cons,buf_cons,256,165,-1);
	make_shell(buf_cons,256,165,"console",0);
	//make_textbox8(sht_cons, 8,28,240,128,COL8_000084);
	console = task_alloc();
	console->tss.esp = memman_alloc_4k(memman,64*1024) + 64 * 1024 -8;
	console->tss.eip = (int) &task_console;
	console->tss.es = 1*8;
	console->tss.cs = 2*8;
	console->tss.ss = 1*8;
	console->tss.ds = 1*8;
	console->tss.fs = 1*8;
	console->tss.gs = 1*8;
	*((int *) (console->tss.esp + 4)) = (int) sht_cons; // memory share
	*((int *) (console->tss.esp + 8)) = (int) memtotal;
	taskrun(console,2,2); // level2,different priorities 


	sheet_move(sht_back,0,0);// move background sheet 
	sheet_move(sht_mouse,mx,my);
	sheet_move(sht_win,8,56);
	sheet_move(sht_cons,100,200);
	sheet_updown(sht_back,0); // height 0, at the bottom
	sheet_updown(sht_cons,1);
	sheet_updown(sht_win,2);
	sheet_updown(sht_mouse,3);

	// tab key
	int key_to=0;
	int key_shift = 0;
	int key_leds = ((binfo->leds) >> 4) & 7; // caps,numlock,scrolllock

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
		{
		//sprintf(s, "%02X", i-256);
		//boxfill8(buf_back, binfo->scrnx, COL8_008400, 0, 16, 15, 31);
		//putfonts8_asc(buf_back, binfo->scrnx, 0, 16, COL8_FFFFFF, s);
		//sheet_refresh(sht_back,0,16,16,32);
		if(i<256 +0x80)
			{
			if(key_shift == 0)
				s[0]= keytable0[i-256];
			else
				s[0]= keytable1[i-256];
			}
		else 
			s[0] =0;
		if(s[0] >= 'A' && s[0]<= 'Z')
		{
			if(((key_leds & 4) == 0 && key_shift == 0) || 
			   ((key_leds & 4) == 1 && key_shift != 0))
			       s[0]+=0x20;
		}

		if (s[0] != 0) { // common character
				if (key_to == 0) {
					if (cursor_x < 128) {
							s[1] = 0;
							bps(sht_win, cursor_x, 28, COL8_FFFFFF, COL8_000000, s, 1);
							cursor_x += 8;
						}
				} 
				else// send to console
					fifo_put(&console->fifo, s[0] + 256);
				}
		if(i == 256 + 0x0e && cursor_x > 8)  // backspace key
			{
				if(key_to == 0)				
				{
					bps(sht_win,cursor_x,28,COL8_FFFFFF,COL8_000000," ",1);
					cursor_x -=8;
				}
				else
				{
					fifo_put(&console->fifo,8+256);
				}
			}
		
		if(i == 256 + 0x1c)
			if(key_to != 0)
				fifo_put(&console->fifo,10+256);
		
		if(i == 256 + 0x0f)  // Tab key
			{
				if(key_to == 0)
				{
					key_to =1;
					make_wtitle8(buf_win, sht_win->bxsize,"task a",0);
					shell_title(buf_cons, sht_cons->bxsize,"console",1);
					cursor_c = -1;
					boxfill8(sht_win ->buf, sht_win->bxsize, COL8_FFFFFF,cursor_x,28,cursor_x+7,43);
					fifo_put(&console->fifo,2); // console cursor on				
				}				
				else
				{
					key_to =0;
					make_wtitle8(buf_win, sht_win->bxsize,"task a",1);
					shell_title(buf_cons, sht_cons->bxsize,"console",0);
					cursor_c = COL8_000000;
					fifo_put(&console->fifo,3); // console cursor off
				}
				sheet_refresh(sht_win,0,0,sht_win->bxsize,21);
				sheet_refresh(sht_cons,0,0,sht_cons->bxsize,21);
			}
		if (i == 256 + 0x2a)
			key_shift |= 1;
		if (i == 256 + 0x36)
			key_shift |= 2;
		if (i == 256 + 0xaa)
			key_shift &= ~1;
		if (i == 256 + 0xb6) 
			key_shift &= ~2;
		if (i == 256 + 0x3a) // CapsLock
			key_leds ^= 4;		
		if (i == 256 + 0x45) // NumLock 
			key_leds ^= 2;		
		if (i == 256 + 0x46) // ScrollLock 
			key_leds ^= 1;
					
		if(cursor_c >= 0)  // show cursor
			boxfill8(sht_win ->buf, sht_win->bxsize, cursor_c,cursor_x,28,cursor_x+7,43);
		sheet_refresh(sht_win,cursor_x,28,cursor_x+8,44);

		}

		else if(511 < i && i < 768)
		{
		
		if(mouse_decode(&mdec,i-512) != 0) 
		  {	//sprintf(s, "[lcr %4d %4d]", mdec.x,mdec.y);
			
			//if((mdec.btn & 0x01) != 0) s[1]='L';
		    //if((mdec.btn & 0x02) != 0) s[3]='R';
		    //if((mdec.btn & 0x04) != 0) s[2]='C';	
			//boxfill8(buf_back, binfo->scrnx, COL8_008484, 32, 16, 32+15*8-1, 31); // after filling,show ok
			//putfonts8_asc(buf_back, binfo->scrnx, 32, 16, COL8_FFFFFF, s);
		 	//sheet_refresh(sht_back,32,16,32+15*8,32);
			// the move of mouse 
			mx +=mdec.x;
			my +=mdec.y;
			if(mx <0) mx = 0;
			if(my <0) my = 0;
			if(mx > binfo->scrnx -1 )  mx =  binfo->scrnx -1;
			if(my > binfo->scrny -1 )  my =  binfo->scrny -1;  // control the mouse move area
		 	//sprintf(s, "(%3d,%3d)", mx,my);
			//boxfill8(buf_back, binfo->scrnx, COL8_008484, 0, 0,79, 15);
			//putfonts8_asc(buf_back, binfo->scrnx, 0, 0, COL8_FFFFFF, s);
			//sheet_refresh(sht_back,0,0,80,16);
			sheet_move(sht_mouse,mx,my);
		 	if((mdec.btn & 0x01) != 0)
				sheet_move(sht_win,mx-80,my-8);
		  }
		}

		else if(i == 1)
			{
				timer_set(timer,50,&fifo,0);
				if(cursor_c >= 0)
				{
					boxfill8(sht_win->buf,sht_win->bxsize, COL8_000000, cursor_x, 28,cursor_x+7, 43);
					sheet_refresh(sht_win,cursor_x,28,cursor_x + 8, 44);
				}
			}
		else if(i==0)
			{
				timer_set(timer,50,&fifo,1);
				if(cursor_c >= 0)
				{
					boxfill8(sht_win->buf,sht_win->bxsize, COL8_FFFFFF,cursor_x, 28,cursor_x + 7, 43);
					sheet_refresh(sht_win,cursor_x,28,cursor_x + 8, 44);
				}
			}
			
	     }

	}
}


// sht: a sheet
void task_console(struct SHEET * sht,unsigned int memtotal)
{
//struct FIFO fifo;
struct TIMER *timer;
int i, fifobuf[128],x,y;
char s[30],cmdline[30];
struct MEMMAN * memman = (struct MEMMAN *) MEMMAN_ADDR;
struct FILEINFO *finfo = (struct FILEINFO *) (ADR_DISKIMG + 0x002600);
int count=0,cursor_x = 16,cursor_y =28, cursor_c = -1;
struct TASK *task = task_now();
timer = timer_alloc();
timer_set(timer,50,&task->fifo,1);
fifo_init(&task->fifo,128,fifobuf,task);
bps(sht, 8, 28, COL8_000084, COL8_FFFFFF, "$", 1);

for(;;)
{
	count++;
	io_cli();
	if(fifo_status(&task->fifo) == 0)
	{
		task_sleep(task);
		io_sti();
	}
	else
	{
	i = fifo_get(&task->fifo);
	io_sti();
	if(i <= 1)
	{	
		if(i== 1)
		{
			timer_set(timer,50,&task->fifo,0);
			if(cursor_c >= 0)
				cursor_c = COL8_FFFFFF;	
		}
		else
		{
			timer_set(timer,50,&task->fifo,1);
			if(cursor_c >= 0)
				cursor_c = COL8_000084;	
		}
		
	}
	if(i ==2 )
		cursor_c = COL8_FFFFFF;
	
	if(i ==3)    // CURSOR off
	{
		boxfill8(sht->buf,sht->bxsize, COL8_000084,cursor_x, 28,cursor_x + 7, 43);
		cursor_c = -1;	
	}
	if (256 <= i && i <= 511)
	{
				if (i == 8 + 256) {
					if (cursor_x > 16) {
						bps(sht, cursor_x, 28,  COL8_000084,COL8_FFFFFF, " ", 1);
						cursor_x -= 8;
					}
				} 
				else if(i == 10 + 256) {  // enter key
					bps(sht, cursor_x, cursor_y,  COL8_000084,COL8_FFFFFF, " ", 1); // disable cursor
					cmdline[cursor_x/8 -2] = 0; // construct string
					cursor_y = cons_newline(cursor_y, sht); // next line
					if (strcmp(cmdline,"mem") == 0) {
						sprintf(s, "total   %dMB", memtotal / (1024 * 1024));
						bps(sht, 8, cursor_y,  COL8_000084, COL8_FFFFFF,s, 30);
						cursor_y = cons_newline(cursor_y, sht);
						sprintf(s, "free %dKB", memman_total(memman) / 1024);
						bps(sht, 8, cursor_y, COL8_000084,  COL8_FFFFFF,s, 30);
						cursor_y = cons_newline(cursor_y, sht);
					} 
					else if (strcmp(cmdline,"clear") == 0)
					{
						for(y = 28; y < 28 + 112; y++)
							for (x = 8; x < 8 + 240; x++)
								sht->buf[x + y * sht->bxsize] = COL8_000084;
						sheet_refresh(sht,8,28, 8 + 240,112 + 28);
						cursor_x =16;cursor_y =28;
						
					}
					
					else if (strcmp(cmdline, "dir") == 0) {
						for (x = 0; x < 224; x++) {
							if (finfo[x].name[0] == 0x00) {
								break;
							}
							if (finfo[x].name[0] != 0xe5) {
								if ((finfo[x].type & 0x18) == 0) {
									sprintf(s, "filename.ext   %7d", finfo[x].size);
									for (y = 0; y < 8; y++) {
										s[y] = finfo[x].name[y];
									}
									s[ 9] = finfo[x].ext[0];
									s[10] = finfo[x].ext[1];
									s[11] = finfo[x].ext[2];
									bps(sht, 8, cursor_y,  COL8_000000,COL8_FFFFFF, s, 30);
									cursor_y = cons_newline(cursor_y, sht);
								}
							}
						}
						cursor_y = cons_newline(cursor_y, sht);
					} 
					else if (cmdline[0] != 0) {
						bps(sht, 8, cursor_y,  COL8_000000,COL8_FFFFFF, "Bad command.", 12);
						cursor_y = cons_newline(cursor_y, sht);
					}
					
					cursor_x = 8;
					bps(sht, cursor_x, cursor_y,  COL8_000084,COL8_FFFFFF, "$", 1);
					cursor_x +=8;
				} 
				else
				{
					
					if(cursor_x < 240) {
						s[0] = i - 256;
						s[1] = 0;
						cmdline[cursor_x/8 -2] = i-256;
						bps(sht, cursor_x, cursor_y,  COL8_000084,COL8_FFFFFF, s, 1);
						cursor_x += 8;
					}
					else
					{
						cursor_x = 0;
						cursor_y +=16;
						s[0] = i - 256;
						s[1] = 0;
						cmdline[cursor_x/8 -2] = i-256;
						bps(sht, cursor_x, cursor_y,  COL8_000084,COL8_FFFFFF, s, 1);
						cursor_x += 8;	
					}
				}
	
	
	}
	if(cursor_c >= 0)
		boxfill8(sht->buf,sht->bxsize, cursor_c,cursor_x, cursor_y,cursor_x + 7, cursor_y+15);
	sheet_refresh(sht,cursor_x,cursor_y,cursor_x + 8, cursor_y + 16);
}
}
}



int cons_newline(int cursor_y, struct SHEET *sheet)
{
	int x, y;
	if(cursor_y < 28 + 112) {
		cursor_y += 16;
	} else {
		for (y = 28; y < 28 + 112; y++) {
			for (x = 8; x < 8 + 240; x++) {
				sheet->buf[x + y * sheet->bxsize] = sheet->buf[x + (y + 16) * sheet->bxsize];
			}
		}
		for (y = 28 + 112; y < 28 + 128; y++) {
			for (x = 8; x < 8 + 240; x++) {
				sheet->buf[x + y * sheet->bxsize] = COL8_000084; // fill color at last line
			}
		}
		sheet_refresh(sheet, 8, 28, 8 + 240, 28 + 128);
	}
	return cursor_y;
}
