#include "bootpack.h"

#define PIT_CTRL 0x0043
#define PIT_CNT0 0x0040
#define TIMER_FLAG_ALLOC  1
#define TIMER_FLAG_USING  2

struct TIMERCTL timerctl;

void init_pit(void)
{
int i;
struct TIMER *t;
io_out8(PIT_CTRL,0x34);
io_out8(PIT_CNT0,0x9c);
io_out8(PIT_CNT0,0x2e);
timerctl.count =0;
timerctl.next = 0xffffffff;

for(i=0;i<MAX_TIMER;i++)
{
timerctl.timers0[i].flag =0; // unused 
}
t = timer_alloc();              //link list 
t->timeout = 0xffffffff;
t->flag = TIMER_FLAG_USING;
t->next = 0;
timerctl.t0 = t;
timerctl.next = 0xffffffff;
return;
}

struct TIMER * timer_alloc(void)
{
int i;
for(i=0;i< MAX_TIMER;i++)
{
if(timerctl.timers0[i].flag == 0)
{
	timerctl.timers0[i].flag = TIMER_FLAG_ALLOC;
	return &timerctl.timers0[i];
}
}
return 0;
}

void timer_free(struct TIMER * timer)
{
	timer->flag =0;
}



void inthandler20(int *esp)
{
struct TIMER *t;
char ts = 0;

io_out8(PIC0_OCW2,0x60);
timerctl.count++;
if(timerctl.next > timerctl.count)
	return;

t = timerctl.t0; // points to the first element
for(;;)
{
if(t->timeout > timerctl.count)
	break;
// timeout
t->flag = TIMER_FLAG_ALLOC;
if(t == mt_timer)
	ts =1;
else
	fifo_put(t->fifo,t->data);
t = t->next;
}
timerctl.t0 = t;
timerctl.next = timerctl.t0->timeout;
if(ts != 0)
	mt_switch();
return;
}


void timer_set(struct TIMER * timer,unsigned int timeout,struct FIFO *fifo,int data)
{
int e;
struct TIMER *s,*t;
timer->timeout = timerctl.count + timeout;  // compare with count
timer->flag = TIMER_FLAG_USING;
timer->fifo = fifo;
timer->data = data;
e = io_load_eflags();
io_cli();
t = timerctl.t0;
if(timeout <= t->timeout)
{
timerctl.t0 = timer;
timer->next = t;  
timerctl.next = timeout;
io_store_eflags(e);
return;
}

for(;;)
{
	s = t;  // s:p t:p+1
 	t = t->next;
	if(timeout <= t->timeout)
	{
	s->next = timer;
	timer->next = t;
	io_store_eflags(e);
	return;
	}
}

}
