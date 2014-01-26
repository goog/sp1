#include "bootpack.h"
#include <stdio.h>   
// reorder the header files then ok 
void HariMain(void)
{
	char *vram;
	int xsize;
	int ysize;
	//short *binfo_scrnx,*binfo_scrny;
	//int *binfo_vram;
	
	struct BOOTINFO *binfo;
	extern char hankaku[4096];
	/*static char font_A[16] = { 0x00, 0x18, 0x18, 0x18, 0x18, 0x24, 0x24, 0x24,
				   0x24, 0x7e, 0x42, 0x42, 0x42, 0xe7, 0x00, 0x00
				 };*/
	char s[40];
	init_gdtidt();
	init_pic();
	io_sti();
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
	char mcursor[256];
	int mx, my;
	mx = (binfo->scrnx - 16) / 2;
	my = (binfo->scrny - 28 - 16) / 2;
	// mouse image 
	init_mouse_cursor8(mcursor,COL8_008484);
	putblock8_8(binfo->vram,binfo->scrnx,16,16,mx,my,mcursor,16);
	
	io_out8(PIC0_IMR, 0xf9); /* PIC1�ƃL�[�{�[�h������(11111001) */
	io_out8(PIC1_IMR, 0xef); //11101111 , allow irq12 

	for(;;)
	io_hlt();  /* execute the _io_hlt assembly function*/
}

