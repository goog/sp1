#include "bootpack.h"
#include <stdio.h>
// reorder the header files then ok

extern struct FIFO8 keyfifo;
extern struct FIFO8 mousefifo;

#define PORT_KEYDAT 0x0060
#define PORT_KEYSTA 0x0064
#define PORT_KEYCMD 0x0064
#define KEYSTA_SEND_NOTREADY 0x02
#define KEYCMD_WRITE_MODE 0x60
#define KBC_MODE 0x47
#define KEYCMD_SENDTO_MOUSE 0xd4
#define MOUSECMD_ENABLE 0xf4

void wait_KBC_sendready(void)
{
for(;;)
{
	if((io_in8(PORT_KEYSTA) & KEYSTA_SEND_NOTREADY) == 0 )
		break;
}
return;
}

void init_keyboard(void)
{
	wait_KBC_sendready();
	io_out8(PORT_KEYCMD,KEYCMD_WRITE_MODE);  // mode setting instruction
	wait_KBC_sendready();
	io_out8(PORT_KEYDAT,KBC_MODE);
	return;
}

struct MOUSE_DEC{
unsigned char buf[3],phase;
int x,y,btn;
};

void enable_mouse(struct MOUSE_DEC * mdec)
{
wait_KBC_sendready();
io_out8(PORT_KEYCMD,KEYCMD_SENDTO_MOUSE);
wait_KBC_sendready();
io_out8(PORT_KEYDAT,MOUSECMD_ENABLE);
// phrase 0, wait for 0xfa
mdec->phase = 0;
return;
}

int mouse_decode(struct MOUSE_DEC *mdec,unsigned char dat)
{
if(mdec->phase == 0)
{
	if(dat == 0xfa)
		mdec->phase = 1;
	return 0;
}

if(mdec->phase == 1)
{
if((dat & 0xc8) == 0x08)   //11001000
	{mdec->buf[0] = dat;
	mdec->phase =2;
	}
return 0;
}


if(mdec->phase == 2)
{
mdec->buf[1] = dat;
mdec->phase =3;
return 0;
}

if(mdec->phase == 3)
{
mdec->buf[2] = dat;
mdec->phase =1;
mdec->btn = mdec->buf[0] & 0x07;
mdec->x = mdec->buf[1];
mdec->y = mdec->buf[2];

if((mdec->buf[0] & 0x10) != 0)
	mdec->x |= 0xffffff00;


if((mdec->buf[0] & 0x20) != 0)
	mdec->y |= 0xffffff00;
mdec->y = - mdec->y;
return 1;
}
return -1;
}

void HariMain(void)
{
	char *vram;
	int xsize;
	int ysize;
	//short *binfo_scrnx,*binfo_scrny;
	//int *binfo_vram;
	
	struct BOOTINFO *binfo;
	extern char hankaku[4096];

	int i;
	char s[40];
	init_gdtidt();
	init_pic();
	io_sti();
	
	init_keyboard();
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
