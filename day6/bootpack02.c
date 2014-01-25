
void io_hlt(void);
void write_mem8(int addr,int data);

void io_cli(void);
void io_out8(int port,int data);
int io_load_eflags(void);
void io_store_eflags(int eflags);


/*  color setting */
void init_palette(void);
void set_palette(int start,int end,unsigned char * rgb);

#define COL8_000000		0
#define COL8_FF0000		1
#define COL8_00FF00		2
#define COL8_FFFF00		3
#define COL8_0000FF		4
#define COL8_FF00FF		5
#define COL8_00FFFF		6
#define COL8_FFFFFF		7
#define COL8_C6C6C6		8
#define COL8_840000		9
#define COL8_008400		10
#define COL8_848400		11
#define COL8_000084		12
#define COL8_840084		13
#define COL8_008484		14
#define COL8_848484		15
void boxfill8(unsigned char *vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1);

void HariMain(void)
{
	int i;
	char *p;
	for(i=0xa0000;i<=0xaffff;i++)
		{
		//write_mem8(i,i&0x0f);
		p = (char *) i;
		*p = i &0x0f;
		}
	io_hlt();  /* execute the _io_hlt assemble function*/
}


void init_palette(void)
{
	static unsigned char table_rgb[16*3] = {
	0x00,0x00,0x00,  /* 0: black */
	0xff,0x00,0x00, /* 1 : 亮红*/
	0x00,0xff,0x00, /*  2:  亮绿 */
	0xff,0xff,0x00,
	0x00,0x00,0xff, /* 4,亮蓝  */
	0xff,0x00,0xff, /* 5 亮紫 */
	0x00,0xff,0xff, 
	0xff,0xff,0xff, /*  7 white */
	0xc6,0xc6,0xc6, /* 8 亮灰 */
	0x84,0x00,0x00,
	0x00,0x84,0x00, /* 10 black green */
	0x84,0x84,0x00, /* 11 black yellow*/
	0x00,0x00,0x84, /* 12 暗青 */
	0x84,0x00,0x84, /* 13 */
	0x00,0x84,0x84, /*  14 浅暗蓝 */
	0x84,0x84,0x84  /*black gray */
	};
	set_palette(0,15,table_rgb);
	return;

}


void set_palette(int start,int end,unsigned char * rgb)
{
	int i, eflags;
	eflags = io_load_eflags() ; //record inter flags
	io_cli();
	io_out8(0x03c8,start);  // the id of palette is start 
	for(i= start;i <= end; i++)
	{
		io_out8(0x03c9,rgb[0]/4);
		io_out8(0x03c9,rgb[1]/4);
		io_out8(0x03c9,rgb[2]/4);
		rgb +=3;
	
	}

	io_store_eflags(eflags);  /* 恢复中断许可*/
	return;



}


void boxfill8(unsigned char *vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1)
{


int x,y;
for(y=y0;y<=y1;y++)
	for(x=x0;x<=x1;x++)
		vram[y*xsize + x] = c

return;

}
