#include "bootpack.h"
#include <stdio.h>
// reorder the header files then ok

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
	extern char hankaku[4096];

	int i;
	char s[40];
	init_gdtidt();
	init_pic();
	io_sti();
	
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
	
	/* initlize the os background */
	init_screen(vram,xsize,ysize);
	putfont8(binfo->vram, binfo->scrnx, 8, 8, COL8_FFFFFF, hankaku+'A'*16);
	putfont8(binfo->vram, binfo->scrnx, 16, 8, COL8_FFFFFF, hankaku+'B'*16);
	putfont8(binfo->vram, binfo->scrnx, 24, 8, COL8_FFFFFF, hankaku+'C'*16);
	putfont8(binfo->vram, binfo->scrnx, 40, 8, COL8_FFFFFF, hankaku+'1'*16);
	putfont8(binfo->vram, binfo->scrnx, 48, 8, COL8_FFFFFF, hankaku+'2'*16);
	putfonts8_asc(binfo->vram,binfo->scrnx,60,50,COL8_FFFFFF,"hello os.");
        // interaction input
	sprintf(s,"scrnx= %d",binfo->scrnx);
	putfonts8_asc(binfo->vram,binfo->scrnx,16,64,COL8_FFFFFF,s);
	char mcursor[256],keybuf[32],mousebuf[128];
	int mx, my;
	mx = (binfo->scrnx - 16) / 2;
	my = (binfo->scrny - 28 - 16) / 2;
	// mouse image
	struct MOUSE_DEC mdec;
	enable_mouse(&mdec); 
	init_mouse_cursor8(mcursor,COL8_008484);
	putblock8_8(binfo->vram,binfo->scrnx,16,16,mx,my,mcursor,16);
	

	sprintf(s, "memory %dMB free %dkb", memtotal/(1024*1024), memman_total(memman)/1024);
	putfonts8_asc(binfo->vram, binfo->scrnx, 0, 132, COL8_FFFFFF, s);

	io_out8(PIC0_IMR, 0xf9); /* PIC1�ƃL�[�{�[�h������(11111001) */
	io_out8(PIC1_IMR, 0xef); //11101111 , allow irq12 

	// keyboard 
	fifo8_init(&keyfifo,32,keybuf);
	fifo8_init(&mousefifo,128,mousebuf);
	for(;;)
	{
	io_cli(); // forbid all interrupts 
	if(fifo8_status(&keyfifo) + fifo8_status(&mousefifo)== 0) 
		io_stihlt();  // read data until it's empty
	else {
		if(fifo8_status(&keyfifo))
		{
		i=fifo8_get(&keyfifo);
		io_sti(); // open interrupts
		sprintf(s, "%02X", i);
		boxfill8(binfo->vram, binfo->scrnx, COL8_008484, 0, 16, 15, 31);
		putfonts8_asc(binfo->vram, binfo->scrnx, 0, 16, COL8_FFFFFF, s);

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
			boxfill8(binfo->vram, binfo->scrnx, COL8_008484, 32, 16, 32+15*8-1, 31); // after filling,show ok
			putfonts8_asc(binfo->vram, binfo->scrnx, 32, 16, COL8_FFFFFF, s);
		 	// the move of mouse 
			boxfill8(binfo->vram, binfo->scrnx, COL8_008484, mx, my, mx+15,my+15);
			mx +=mdec.x;
			my +=mdec.y;
			if(mx <0) mx = 0;
			if(my <0) my = 0;
			if(mx > binfo->scrnx -16 ) mx =  binfo->scrnx -16;
			if(my > binfo->scrny -16 ) mx =  binfo->scrny -16;
		 	sprintf(s, "(%3d,%3d)", mx,my);
			boxfill8(binfo->vram, binfo->scrnx, COL8_008484, 0, 0,79, 15);
			putfonts8_asc(binfo->vram, binfo->scrnx, 0, 0, COL8_FFFFFF, s);
			putblock8_8(binfo->vram,binfo->scrnx,16,16,mx,my,mcursor,16);
		 
		  }
		
		
		}
	     }

	}
}





#define EFLAGS_AC_BIT		0x00040000
#define CR0_CACHE_DISABLE	0x60000000

unsigned int memtest(unsigned int start, unsigned int end)
{
	char flg486 = 0;
	unsigned int eflg, cr0, i;

	/* 386���A486�ȍ~�Ȃ̂��̊m�F */
	eflg = io_load_eflags();
	eflg |= EFLAGS_AC_BIT; /* AC-bit = 1 */
	io_store_eflags(eflg);
	eflg = io_load_eflags();
	if ((eflg & EFLAGS_AC_BIT) != 0) { /* 386�ł�AC=1�ɂ��Ă�����0�ɖ߂��Ă��܂� */
		flg486 = 1;
	}
	eflg &= ~EFLAGS_AC_BIT; /* AC-bit = 0 */
	io_store_eflags(eflg);

	if (flg486 != 0) {
		cr0 = load_cr0();
		cr0 |= CR0_CACHE_DISABLE; /* �L���b�V���֎~ */
		store_cr0(cr0);
	}

	i = memtest_sub(start, end);

	if (flg486 != 0) {
		cr0 = load_cr0();
		cr0 &= ~CR0_CACHE_DISABLE; /* �L���b�V������ */
		store_cr0(cr0);
	}

	return i;
}

