#include "bootpack.h"
#include "stdio.h"
#include "string.h"
// sht: a sheet
void task_console(struct SHEET * sht,unsigned int memtotal)
{
//struct FIFO fifo;
struct TIMER *timer;
int i, fifobuf[128],x,y;
char s[30],cmdline[30],*p;
struct MEMMAN * memman = (struct MEMMAN *) MEMMAN_ADDR;
struct SEGMENT_DESCRIPTOR *gdt = (struct SEGMENT_DESCRIPTOR *) ADR_GDT;
int *fat = (int *) memman_alloc_4k(memman, 4 * 2880);
file_readfat(fat, (unsigned char *) (ADR_DISKIMG + 0x000200));
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
					
					else if (strcmp(cmdline, "ls") == 0) {
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
					else if(strncmp(cmdline,"type ",5) == 0)
					{
						for(y=0;y < 11;y++)
							s[y]= ' ';
						y =0;
						for(x=5;y<11 && cmdline[x]!=0;x++)
						{
							if(cmdline[x] == '.' && y <=8)
								y = 8;
							else
								{
									s[y] = cmdline[x];
									if(s[y] >= 'a' && s[y]<= 'z')
										s[y]-=0x20;
									y++;	
								}
							
						}
						
						for (x = 0; x < 224; ) {
							if (finfo[x].name[0] == 0x00) {
								break;
							}
							if ((finfo[x].type & 0x18) == 0) {
								for (y = 0; y < 11; y++) {
									if (finfo[x].name[y] != s[y]) {
										goto type_next_file;
									}
								}
								break; // found file
							}
		type_next_file:
							x++;
						}
						if (x < 224 && finfo[x].name[0] != 0x00) {// have found file
							//y = finfo[x].size;
							p = (char *) memman_alloc_4k(memman,finfo[x].size);
							file_loadfile(finfo[x].clustno, finfo[x].size, p, fat, (char *) (ADR_DISKIMG + 0x003e00));
							cursor_x = 8;
							for(y= 0; y< finfo[x].size; y++) {
								s[0] = p[y];
								s[1] = 0;
								if(s[0] == 0x09)
								{
									for(;;)
									{
										bps(sht, cursor_x, cursor_y,  COL8_000084, COL8_FFFFFF," ", 1);
										cursor_x +=8;
										if(cursor_x == 8+240)
											{
											cursor_x =8;
											cursor_y = cons_newline(cursor_y,sht);
											}
										if(((cursor_x -8) & 0x1f) == 0)
											break;
									}
								}
								
								else if(s[0] == 0x0a)
									{
										cursor_x =8;
										cursor_y = cons_newline(cursor_y,sht);
									}
								else
								{	bps(sht, cursor_x, cursor_y,  COL8_000084, COL8_FFFFFF,s, 1);
									cursor_x += 8;
									if (cursor_x == 8 + 240) 
									{
										cursor_x = 8;
										cursor_y = cons_newline(cursor_y, sht);
									}
								}
							}
							memman_free_4k(memman,(int) p,finfo[x].size);
						} 
						else {
							bps(sht, 8, cursor_y,  COL8_000084,COL8_FFFFFF, "File not found.", 15);
							cursor_y = cons_newline(cursor_y, sht);
						}
						cursor_y = cons_newline(cursor_y, sht);
					}
					
					else if (strcmp(cmdline, "hlt") == 0) {
						/* execute hlt.hrb */
						for (y = 0; y < 11; y++) {
							s[y] = ' ';
						}
						s[0] = 'H';
						s[1] = 'L';
						s[2] = 'T';
						s[8] = 'H';
						s[9] = 'R';
						s[10] = 'B';
						for (x = 0; x < 224; ) {
							if (finfo[x].name[0] == 0x00) {
								break;
							}
							if ((finfo[x].type & 0x18) == 0) {
								for (y = 0; y < 11; y++) {
									if (finfo[x].name[y] != s[y]) {
										goto hlt_next_file;
									}
								}
								break;
							}
		hlt_next_file:
							x++;
						}
						if (x < 224 && finfo[x].name[0] != 0x00) {
							p = (char *) memman_alloc_4k(memman, finfo[x].size);
							file_loadfile(finfo[x].clustno, finfo[x].size, p, fat, (char *) (ADR_DISKIMG + 0x003e00));
							set_segmdesc(gdt + 1003, finfo[x].size - 1, (int) p, AR_CODE32_ER);
							farjmp(0, 1003 * 8); // start the application
							memman_free_4k(memman, (int) p, finfo[x].size);
						} else {
							bps(sht, 8, cursor_y, COL8_000084,COL8_FFFFFF, "File not found.", 15);
							cursor_y = cons_newline(cursor_y, sht);
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
