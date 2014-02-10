#include "bootpack.h"

void fifo_init(struct FIFO *fifo, int size, int  *buf,struct TASK *task)
{
fifo->size = size; // size of buffer
fifo->buf  = buf;
fifo->free = size;
fifo->task = task;
fifo->flags = 0;
fifo->p = 0;  // next write position
fifo->q = 0; // next read position
return;
}


#define FLAGS_OVERRUN 0x0001
int fifo_put(struct FIFO *fifo, int  data)
{
if(fifo->free == 0 )
	{
	fifo->flags |= FLAGS_OVERRUN;
	return -1;
	}

fifo->buf[fifo->p] = data;
fifo->p++;
if(fifo->p == fifo->size)
	fifo->p = 0;

fifo->free--;
if(fifo->task != 0)
	if(fifo->task->flags != 2)
		taskrun(fifo->task,-1,0);
return 0;
}



int fifo_get(struct FIFO *fifo)
{
int data;
if(fifo->free == fifo->size )
	return -1; // buffer empty

data = fifo->buf[fifo->q];
fifo->q++;
if(fifo->q == fifo->size)
	fifo->q = 0;

fifo->free++;
return data;
}


int fifo_status(struct FIFO *fifo)
{
return fifo->size - fifo->free;
// buffer usage
}
