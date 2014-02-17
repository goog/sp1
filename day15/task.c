#include "bootpack.h"

#define TIMER_FLAG_ALLOC  1
#define TIMER_FLAG_USING  2
struct TIMER *mt_timer;
int mt_tr;

void timer_settime(struct TIMER *timer, unsigned int timeout)
{
	int e;
	struct TIMER *t, *s;
	timer->timeout = timeout + timerctl.count;
	timer->flag = TIMER_FLAG_USING;
	e = io_load_eflags();
	io_cli();
	t = timerctl.t0;
	if (timer->timeout <= t->timeout) {
		// become first link list element
		timerctl.t0 = timer;
		timer->next = t;
		timerctl.next = timer->timeout;
		io_store_eflags(e);
		return;
	}

	for (;;) {
		s = t;
		t = t->next;
		if (timer->timeout <= t->timeout) {
			// insert 
			s->next = timer;
			timer->next = t;
			io_store_eflags(e);   // stop interrupts on the stage of setting timers' list
			return;
		}
	}
}


// multiple task
void mt_init(void)
{	
mt_timer = timer_alloc();
timer_settime(mt_timer,2);
mt_tr = 3*8;
return;	
}


void mt_switch(void)
{
	if(mt_tr == 3*8)
		mt_tr = 4*8;
	else
		mt_tr = 3*8;
	timer_settime(mt_timer,2);
	farjmp(0,mt_tr);
	return;
}
