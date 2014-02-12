#include "bootpack.h"
#include "stdio.h"
#include "string.h"

// sht: a sheet
void task_console(struct SHEET * sht,unsigned int memtotal)
{
//struct FIFO fifo;
struct TIMER *timer;
int i, fifobuf[128];
char s[30],cmdline[30];
struct CONSOLE cons;
cons.sht = sht;
cons.cur_x =  8;
cons.cur_y = 28;
cons.cur_c = -1;

struct MEMMAN * memman = (struct MEMMAN *) MEMMAN_ADDR;
int *fat = (int *) memman_alloc_4k(memman, 4 * 2880);
file_readfat(fat, (unsigned char *) (ADR_DISKIMG + 0x000200));
* (int *) 0x0fec = (int) &cons;
int count=0;
struct TASK *task = task_now();
timer = timer_alloc();
timer_set(timer,50,&task->fifo,1);
fifo_init(&task->fifo,128,fifobuf,task);
cons_putchar(&cons, '$', 1);

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
			if(cons.cur_c >= 0)
				cons.cur_c = COL8_FFFFFF;	
		}
		else
		{
			timer_set(timer,50,&task->fifo,1);
			if(cons.cur_c >= 0)
				cons.cur_c = COL8_000084;	
		}
		
	}
	if(i ==2 )
		cons.cur_c = COL8_FFFFFF;
	
	if(i ==3)    // cursor off
	{
		boxfill8(sht->buf,sht->bxsize, COL8_000084,cons.cur_x, cons.cur_y,cons.cur_x + 7, cons.cur_y+15);
		cons.cur_c = -1;
	}
	if (256 <= i && i <= 511)  // keyboard data
	{
				if(i == 8 + 256) 
				{  // backspace
					if (cons.cur_x >= 16) {
						cons_putchar(&cons, ' ', 0); // erase the cursor
						cons.cur_x -= 8;
					}
				} 
				else if(i == 10 + 256) {  // enter key
					cons_putchar(&cons, ' ', 0); // disable cursor
					cmdline[cons.cur_x/8 -2] = 0; // construct string
					cons_newline(&cons); // next line
					cons_runcmd(cmdline,&cons,fat,memtotal);
					cons_putchar(&cons, '$', 1);
					
				} 
						
				else // general character
				{
					
					if(cons.cur_x < 240) {
						cmdline[cons.cur_x/8 -2] = i-256;
						cons_putchar(&cons, i-256, 1);
					}
					//else
					//{
						//cons.cur_x = 16;
						//cons.cur_y +=16;
						//cmdline[cons.cur_x/8 -2] = i-256; // 16 starts char
						//cons_putchar(&cons, i-256, 1);
					//}
				}
	
	
	}
	if(cons.cur_c >= 0)
		boxfill8(sht->buf,sht->bxsize, cons.cur_c,cons.cur_x, cons.cur_y,cons.cur_x + 7, cons.cur_y+15);
	sheet_refresh(sht,cons.cur_x,cons.cur_y,cons.cur_x + 8, cons.cur_y + 16);
}
}
}


void cons_putchar(struct CONSOLE *cons, int chr, char move)
{
	char s[2];
	s[0] = chr;
	s[1] = 0;
	if (s[0] == 0x09) {	// tab
		for (;;) {
			bps(cons->sht, cons->cur_x, cons->cur_y,  COL8_000084, COL8_FFFFFF," ", 1);
			cons->cur_x += 8;
			if (cons->cur_x == 8 + 240) {
				cons_newline(cons);
			}
			if (((cons->cur_x - 8) & 0x1f) == 0) {
				break;	// 32n
			}
		}
	} 
	
	else if (s[0] == 0x0a) {	// enter
		cons_newline(cons);
	} 
	else if (s[0] == 0x0d) 
	{
		
	} 
	
	else {
		bps(cons->sht, cons->cur_x, cons->cur_y,  COL8_000084,COL8_FFFFFF, s, 1);
		if (move != 0) {
			cons->cur_x += 8;
			if (cons->cur_x == 8 + 240) {
				cons_newline(cons);
			}
		}
	}
	return;
}

void cons_newline(struct CONSOLE *cons)
{
	int x, y;
	struct SHEET *sheet = cons->sht;
	if (cons->cur_y < 28 + 112) {
		cons->cur_y += 16; /* ÌsÖ */
	} else {
		/* XN[ */
		for (y = 28; y < 28 + 112; y++) {
			for (x = 8; x < 8 + 240; x++) {
				sheet->buf[x + y * sheet->bxsize] = sheet->buf[x + (y + 16) * sheet->bxsize];
			}
		}
		for (y = 28 + 112; y < 28 + 128; y++) {
			for (x = 8; x < 8 + 240; x++) {
				sheet->buf[x + y * sheet->bxsize] = COL8_000084;
			}
		}
		sheet_refresh(sheet, 8, 28, 8 + 240, 28 + 128);
	}
	cons->cur_x = 8;
	return;
}

void cons_runcmd(char *cmdline, struct CONSOLE *cons, int *fat, unsigned int memtotal)
{
	if (strcmp(cmdline, "mem") == 0) {
		cmd_mem(cons, memtotal);
	} else if (strcmp(cmdline, "clear") == 0) {
		cmd_cls(cons);
	} else if (strcmp(cmdline, "ls") == 0) {
		cmd_dir(cons);
	} else if (strncmp(cmdline, "cat ", 4) == 0) {
		cmd_type(cons, fat, cmdline);
	//} else if (strcmp(cmdline, "hlt") == 0) {
	//	cmd_hlt(cons, fat);
	} else if (cmdline[0] != 0) {
		if(cmd_app(cons,fat,cmdline) == 0)
		{
			bps(cons->sht, 8, cons->cur_y,  COL8_000084,COL8_FFFFFF, "Bad command.", 12);
			cons_newline(cons);
			cons_newline(cons);
		}
	}
	return;
}

void cmd_mem(struct CONSOLE *cons, unsigned int memtotal)
{
	struct MEMMAN *memman = (struct MEMMAN *) MEMMAN_ADDR;
	char s[30];
	sprintf(s, "total   %dMB", memtotal / (1024 * 1024));
	bps(cons->sht, 8, cons->cur_y, COL8_000084,COL8_FFFFFF,  s, 30);
	cons_newline(cons);
	sprintf(s, "free %dKB", memman_total(memman) / 1024);
	bps(cons->sht, 8, cons->cur_y, COL8_000084,COL8_FFFFFF,  s, 30);
	cons_newline(cons);
	cons_newline(cons);
	return;
}

void cmd_cls(struct CONSOLE *cons)
{
	int x, y;
	struct SHEET *sheet = cons->sht;
	for (y = 28; y < 28 + 128; y++) {
		for (x = 8; x < 8 + 240; x++) {
			sheet->buf[x + y * sheet->bxsize] = COL8_000084;
		}
	}
	sheet_refresh(sheet, 8, 28, 8 + 240, 28 + 128);
	cons->cur_y = 28;
	return;
}

void cmd_dir(struct CONSOLE *cons)
{
	struct FILEINFO *finfo = (struct FILEINFO *) (ADR_DISKIMG + 0x002600);
	int i, j;
	char s[30];
	for (i = 0; i < 224; i++) {
		if (finfo[i].name[0] == 0x00) {
			break;
		}
		if (finfo[i].name[0] != 0xe5) {
			if ((finfo[i].type & 0x18) == 0) {
				sprintf(s, "filename.ext   %7d", finfo[i].size);
				for (j = 0; j < 8; j++) {
					s[j] = finfo[i].name[j];
				}
				s[ 9] = finfo[i].ext[0];
				s[10] = finfo[i].ext[1];
				s[11] = finfo[i].ext[2];
				bps(cons->sht, 8, cons->cur_y, COL8_000084,COL8_FFFFFF,  s, 30);
				cons_newline(cons);
			}
		}
	}
	cons_newline(cons);
	return;
}

void cmd_type(struct CONSOLE *cons, int *fat, char *cmdline)
{
	struct MEMMAN *memman = (struct MEMMAN *) MEMMAN_ADDR;
	struct FILEINFO *finfo = file_search(cmdline + 5, (struct FILEINFO *) (ADR_DISKIMG + 0x002600), 224);
	char *p;
	int i;
	if (finfo != 0) {
		/* t@Cª©Â©Áœê */
		p = (char *) memman_alloc_4k(memman, finfo->size);
		file_loadfile(finfo->clustno, finfo->size, p, fat, (char *) (ADR_DISKIMG + 0x003e00));
		for (i = 0; i < finfo->size; i++) {
			cons_putchar(cons, p[i], 1);
		}
		memman_free_4k(memman, (int) p, finfo->size);
	} else {
		/* t@Cª©Â©çÈ©Áœê */
		bps(cons->sht, 8, cons->cur_y, COL8_000084,COL8_FFFFFF,  "File not found.", 15);
		cons_newline(cons);
	}
	cons_newline(cons);
	return;
}

int cmd_app(struct CONSOLE *cons, int *fat,char *cmdline)
{
	struct MEMMAN *memman = (struct MEMMAN *) MEMMAN_ADDR;
	struct FILEINFO *finfo = file_search("HLT.HRB", (struct FILEINFO *) (ADR_DISKIMG + 0x002600), 224);
	struct SEGMENT_DESCRIPTOR *gdt = (struct SEGMENT_DESCRIPTOR *) ADR_GDT;
	char *p,name[18];
	int i;
	for(i=0;i <13; i++)
	{
		if(cmdline[i] <= ' ')
			break;
		name[i] = cmdline[i];
		
	}
	name[i] = 0;
	finfo = file_search(name, (struct FILEINFO *) (ADR_DISKIMG + 0x002600), 224);
	if (finfo == 0 && name[i - 1] != '.')
	{
		name[i] = '.';
		name[i+1] = 'H';
		name[i+2] = 'R';
		name[i+3] = 'B';
		name[i+4] = 0;
		// search the file again
		finfo = file_search(name, (struct FILEINFO *) (ADR_DISKIMG + 0x002600), 224);
	}
	
	if (finfo != 0) { // found file
		p = (char *) memman_alloc_4k(memman, finfo->size);
		*((int *) 0xfe8) = (int) p;
		file_loadfile(finfo->clustno, finfo->size, p, fat, (char *) (ADR_DISKIMG + 0x003e00));
		set_segmdesc(gdt + 1003, finfo->size - 1, (int) p, AR_CODE32_ER);
		farcall(0, 1003 * 8); // call
		memman_free_4k(memman, (int) p, finfo->size);
		cons_newline(cons);
		return 1;
	} 
	return 0;
}



// print, todo rename
void cons_putstr0(struct CONSOLE *cons, char *s)
{
	for (; *s != 0; s++) {
		cons_putchar(cons, *s, 1);
	}
	return;
}

void cons_putstr1(struct CONSOLE *cons, char *s, int l)
{
	int i;
	for (i = 0; i < l; i++) {
		cons_putchar(cons, s[i], 1);
	}
	return;
}


void hrb_api(int edi, int esi, int ebp, int esp, int ebx, int edx, int ecx, int eax)
{
	struct CONSOLE *cons = (struct CONSOLE *) *((int *) 0x0fec);
	int cs_base = *((int *) 0xfe8);
	if (edx == 1) {
		cons_putchar(cons, eax & 0xff, 1);
	} else if (edx == 2) {
		cons_putstr0(cons, (char *) ebx +cs_base);
	} else if (edx == 3) {
		cons_putstr1(cons, (char *) ebx + cs_base, ecx);
	}
	return;
}
