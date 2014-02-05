#include "bootpack.h"

/* b:background color, c : char color len: length*/
int bps(struct SHEET *sht, int x,int y, int b,int c, char *s , int len)
{
boxfill8(sht->buf,sht->bxsize,b,x,y,x+8*len -1,y+15);
putfonts8_asc(sht->buf,sht->bxsize,x,y, c,s);
sheet_refresh(sht,x,y,x+8*len,y+16);
return 1;
}
