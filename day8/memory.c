#include "bootpack.h"
/*
#define MEMMAN_FREES  4090
struct FREEINFO {             // one item
unsigned int addr,size;
};


struct MEMMAN {
int frees,maxfrees,lostsize,losts;
struct FREEINFO free[MEMMAN_FREES];
};
*/
#define MEMMAN_FREES  4090
void memman_init(struct MEMMAN *man )
{
	man->frees =0;    // available item number 
	man->maxfrees = 0;
	man->lostsize = 0;
	man->losts =0;
	return;
}

// total of available memory
unsigned int memman_total(struct MEMMAN *man )
{
	unsigned int i,t =0;
	for(i=0;i < man->frees; i++)
		t += man->free[i].size;
	return t;
}




// there is optical space 
unsigned int memman_alloc(struct MEMMAN * man, unsigned int size)
{

unsigned int i,a;
for(i=0;i<man->frees;i++)
{
	if( man->free[i].size  >= size)
		{
		a = man->free[i].addr;
		man->free[i].addr +=size;
		man->free[i].size -=size;
		if(man->free[i].size == 0)
		{
		man->frees--;
		for(;i<man->frees;i++)
			man->free[i] = man->free[i+1];
		}
		
		return a;
		}
}

return 0;
}



int memman_free(struct MEMMAN * man, unsigned int addr, unsigned int size )
{

int i,j;
for(i =0;i< man->frees;i++)
	if(man->free[i].addr > addr )
	break;
// free[i] < addr <  free[i+1].addr
if(i > 0)
{

	if(man->free[i-1].addr + man->free[i-1].size  == addr )
		{
		man->free[i-1].size += size;
		if(i < man->frees)
			if(addr + size == man->free[i].addr )
			{
			man->free[i-1].size +=  man->free[i].size;
			// delete free[i]
			 man->frees--;
			 for(;i<man->frees;i++) 
				 man->free[i] = man->free[i+1];
			
			}
		
		return 0;
		}
}

if(i < man->frees)
	if(addr + size == man->free[i].addr )
	{
	man->free[i].addr = addr;
	man->free[i].size += size;
	return 0;
	}



if(man->frees < MEMMAN_FREES)
{
for(j=man->frees; j > i; j--)
	man->free[j] = man->free[j-1];

man->frees++;
if(man->maxfrees < man->frees )
	man->maxfrees = man->frees;

man->free[i].addr =  addr;
man->free[i].size = size ;

return 0;

}


man->losts++;
man->lostsize+= size;
return -1 ;
}

