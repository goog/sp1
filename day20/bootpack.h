/* asmhead.nas */
struct BOOTINFO { /* 0x0ff0-0x0fff */
	char cyls; /* ブートセクタはどこまでディスクを読んだのか */
	char leds; /* ブート時のキーボードのLEDの状態 */
	char vmode; /* ビデオモード  何ビットカラーか */
	char reserve;
	short scrnx, scrny;
	char *vram;
};
#define ADR_BOOTINFO	0x00000ff0
#define ADR_DISKIMG     0x00100000

/* naskfunc.nas */
void io_hlt(void);
void io_cli(void);
void io_sti(void);
void io_stihlt(void);
void io_out8(int port, int data);
int io_in8(int port);
int io_load_eflags(void);
void io_store_eflags(int eflags);
void load_gdtr(int limit, int addr);
void load_idtr(int limit, int addr);
void asm_inthandler20(void);
void asm_inthandler21(void);
void asm_inthandler27(void);
void asm_inthandler2c(void);
void asm_cons_putchar(void);
int load_cr0(void);
void store_cr0(int cr0);
void load_tr(int tr);
void taskswitch4(void);
void taskswitch3(void);
void farjmp(int eip, int cs);
void farcall(int eip, int cs);
unsigned int memtest_sub(unsigned int start, unsigned int end);
void asm_hrb_api(void);

/* graphic.c */
void init_palette(void);
void set_palette(int start, int end, unsigned char *rgb);
void boxfill8(unsigned char *vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1);
void init_screen(char *vram, int x, int y);
void putfont8(char *vram, int xsize, int x, int y, char c, char *font);
void putfonts8_asc(char *vram, int xsize, int x, int y, char c, unsigned char *s);
void init_mouse_cursor8(char *mouse, char bc);
void putblock8_8(char *vram, int vxsize, int pxsize,
	int pysize, int px0, int py0, char *buf, int bxsize);
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

/* dsctbl.c */
struct SEGMENT_DESCRIPTOR {
	short limit_low, base_low;
	char base_mid, access_right;
	char limit_high, base_high;
};
struct GATE_DESCRIPTOR {
	short offset_low, selector;
	char dw_count, access_right;
	short offset_high;
};
void init_gdtidt(void);
void set_segmdesc(struct SEGMENT_DESCRIPTOR *sd, unsigned int limit, int base, int ar);
void set_gatedesc(struct GATE_DESCRIPTOR *gd, int offset, int selector, int ar);
#define ADR_IDT			0x0026f800
#define LIMIT_IDT		0x000007ff
#define ADR_GDT			0x00270000
#define LIMIT_GDT		0x0000ffff
#define ADR_BOTPAK		0x00280000
#define LIMIT_BOTPAK	0x0007ffff
#define AR_DATA32_RW	0x4092
#define AR_CODE32_ER	0x409a
#define AR_INTGATE32	0x008e
#define AR_TSS32	0x0089


/* int.c */

void init_pic(void);
void inthandler21(int *esp);
void inthandler27(int *esp);
void inthandler2c(int *esp);
#define PIC0_ICW1		0x0020
#define PIC0_OCW2		0x0020
#define PIC0_IMR		0x0021
#define PIC0_ICW2		0x0021
#define PIC0_ICW3		0x0021
#define PIC0_ICW4		0x0021
#define PIC1_ICW1		0x00a0
#define PIC1_OCW2		0x00a0
#define PIC1_IMR		0x00a1
#define PIC1_ICW2		0x00a1
#define PIC1_ICW3		0x00a1
#define PIC1_ICW4		0x00a1

/* fifo.c 32bit */
struct FIFO{
	int *buf;
	int p, q, size, free, flags;
	struct TASK *task;
};
void fifo_init(struct FIFO *fifo, int size, int  *buf,struct TASK *task);
int fifo_put(struct FIFO *fifo, int  data);
int fifo_get(struct FIFO *fifo);
int fifo_status(struct FIFO *fifo);


/* keyboard.c */
void wait_KBC_sendready(void);
void init_keyboard(struct FIFO *fifo, int data);

/*mouse.c */
struct MOUSE_DEC{
	unsigned char buf[3],phase;
	int x,y,btn;
};
void enable_mouse(struct FIFO *fifo, int data0, struct MOUSE_DEC * mdec);
int mouse_decode(struct MOUSE_DEC *mdec,unsigned char dat);

// memory 
#define MEMMAN_FREES  4090
#define MEMMAN_ADDR 0x003c0000
struct FREEINFO {             // one item
	unsigned int addr,size;
};


struct MEMMAN {
	int frees,maxfrees,lostsize,losts;
	struct FREEINFO free[MEMMAN_FREES];
};

unsigned int memtest(unsigned int start, unsigned int end);
void memman_init(struct MEMMAN *man);
unsigned int memman_total(struct MEMMAN *man );
unsigned int memman_alloc(struct MEMMAN * man, unsigned int size);
unsigned int memman_alloc_4k(struct MEMMAN * man, unsigned int size);
int memman_free(struct MEMMAN * man, unsigned int addr, unsigned int size );
int memman_free_4k(struct MEMMAN * man, unsigned int addr, unsigned int size );

/* sheet.c */
#define MAX_SHEETS		256
struct SHEET {
	unsigned char *buf;
	int bxsize, bysize, vx0, vy0, col_inv, height, flags;
	struct SHTCTL *ctl;
};

struct SHTCTL {
	unsigned char *vram,*map;
	int xsize, ysize, top;
	struct SHEET *sheets[MAX_SHEETS];
	struct SHEET sheets0[MAX_SHEETS];
};

struct SHTCTL *shtctl_init(struct MEMMAN *memman, unsigned char *vram, int xsize, int ysize);
struct SHEET *sheet_alloc(struct SHTCTL *ctl);
void sheet_setbuf(struct SHEET *sht, unsigned char *buf, int xsize, int ysize, int col_inv);
void sheet_updown( struct SHEET *sht, int height);
void sheet_refreshsub(struct SHTCTL *ctl,int vx0,int vy0, int vx1,int vy1,int h0,int h1);
void sheet_refreshmap(struct SHTCTL *ctl,int vx0,int vy0, int vx1,int vy1,int h0);
void sheet_refresh(struct SHEET *sht,int bx0,int by0,int bx1,int by1);
// move the sheet
void sheet_move(struct SHEET *sht, int vx0, int vy0);
void sheet_free( struct SHEET *sht);

/* window.c */
void make_window8(unsigned char *buf, int xsize, int ysize, char *title, char act);
void make_wtitle8(unsigned char *buf, int xsize, char *title, char act);
void shell_title(unsigned char *buf, int xsize, char *title, char act);
void make_shell(unsigned char *buf, int xsize, int ysize, char *title, char act);

//timer.c
#define MAX_TIMER  500
struct TIMER{
	struct TIMER *next;
	unsigned int timeout,flag;
	struct FIFO *fifo;
	unsigned char data;
};

struct TIMERCTL{
	unsigned int count,next;
	struct TIMER *t0;
	struct TIMER timers0[MAX_TIMER];
};
extern struct TIMERCTL timerctl;
void init_pit(void);
void inthandler20(int *esp);
void timer_set(struct TIMER * timer,unsigned int timeout,struct FIFO *fifo,int  data);
void timer_free(struct TIMER * timer);
struct TIMER * timer_alloc(void);

/* functions.c*/
int bps(struct SHEET *sht, int x,int y, int b,int c, char *s, int len);
void make_textbox8(struct SHEET *sht, int x0, int y0, int sx, int sy, int c); // text box 

/* mutiple task */
#define MAX_TASKS 1000
#define TASK_GDT0 3
#define MAX_TASKS_LV	100
#define MAX_TASKLEVELS	10

struct TSS32 {
	int backlink, esp0, ss0, esp1, ss1, esp2, ss2, cr3;
	int eip, eflags, eax, ecx, edx, ebx, esp, ebp, esi, edi;
	int es, cs, ss, ds, fs, gs;
	int ldtr, iomap;
};

struct TASK
{
	int sel,flags; // sel: gdt NO
	int level,priority;
	struct TSS32 tss;
	struct FIFO fifo;
};

struct TASKLEVEL {
	int running;
	int now;
	struct TASK *tasks[MAX_TASKS_LV];
};
struct TASKCTL {
	int now_lv;
	char lv_change; // whether change level at next switch
	struct TASKLEVEL level[MAX_TASKLEVELS];
	struct TASK tasks0[MAX_TASKS];
};

extern struct TIMER *task_timer;
struct TASK *task_init(struct MEMMAN *memman);
struct TASK *task_alloc(void);
void taskrun(struct TASK *task,int level,int priority);
void task_switch(void);
void task_sleep(struct TASK *task);
struct TASK * task_now(void);
void task_remove(struct TASK *task);
void task_switchsub(void);
void task_add(struct TASK *task);
void task_idle(void);

// file.c
struct FILEINFO {
	unsigned char name[8], ext[3], type;
	char reserve[10];
	unsigned short time, date, clustno;
	unsigned int size;
};
void file_readfat(int *fat, unsigned char *img);
void file_loadfile(int clustno, int size, char *buf, int *fat, char *img);
struct FILEINFO *file_search(char *name, struct FILEINFO *finfo, int max);

// console
struct CONSOLE {
	struct SHEET *sht;
	int cur_x, cur_y, cur_c;
};
void cons_putchar(struct CONSOLE *cons, int chr, char move);
void cons_newline(struct CONSOLE *cons);
void cons_runcmd(char *cmdline, struct CONSOLE *cons, int *fat, unsigned int memtotal);
void cmd_mem(struct CONSOLE *cons, unsigned int memtotal);
void cmd_cls(struct CONSOLE *cons);
void cmd_dir(struct CONSOLE *cons);
void cmd_type(struct CONSOLE *cons, int *fat, char *cmdline);
int cmd_app(struct CONSOLE *cons, int *fat,char *cmdline);
void task_console(struct SHEET * sht,unsigned int memtotal);
void hrb_api(int edi, int esi, int ebp, int esp, int ebx, int edx, int ecx, int eax);

