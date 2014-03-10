#include "bootpack.h"

/* gdt and idt */
void init_gdtidt(void)
{
	struct SEGMENT_DESCRIPTOR *gdt = (struct SEGMENT_DESCRIPTOR *) 0x00270000;
	struct GATE_DESCRIPTOR *idt    = (struct GATE_DESCRIPTOR *) 0x0026f800;
	int i;
	// inilialize gdt 
	for(i=0;i<8192;i++)
		set_segmdesc(gdt+i,0,0,0);

	set_segmdesc(gdt+1,0xffffffff,0x00000000,0x4092); // ring0, system rw segment, all managed memory
	set_segmdesc(gdt+2,0x0007ffff,0x00280000,0x409a); // system executable, bootpack.hrb memory location
	load_gdtr(0xffff,0x00270000);

	for(i=0;i < 256; i++)
	{
		set_gatedesc(idt+i,0,0,0);
	
	}

	load_idtr(LIMIT_IDT, ADR_IDT);
	set_gatedesc(idt+0x20, (int) asm_inthandler20, 2*8, AR_INTGATE32);
	set_gatedesc(idt+0x21, (int) asm_inthandler21, 2*8, AR_INTGATE32); // in segment 2
	set_gatedesc(idt+0x27, (int) asm_inthandler27, 2*8, AR_INTGATE32);
	set_gatedesc(idt+0x2c, (int) asm_inthandler2c, 2*8, AR_INTGATE32); // in segment 2
	set_gatedesc(idt+0x40, (int) asm_hrb_api, 2*8, AR_INTGATE32);
	set_gatedesc(idt+0x0c, (int) asm_inthandler0c, 2*8, AR_INTGATE32); // in segment 2
	return;

}


void set_segmdesc(struct SEGMENT_DESCRIPTOR *sd,unsigned int limit,int base,int ar)
{
	if(limit > 0xfffff) 
	{ar |= 0x8000 ;  // Gbit =1
	limit /= 0x1000; // to page 4kb
	}
	sd->limit_low = limit&0xffff;
	sd->base_low = base & 0xffff;
	sd->base_mid = (base >> 16) & 0xff;	
	sd->base_high = (base >> 24 ) & 0xff;
	sd->access_right = ar & 0xff;
	sd->limit_high = ((limit >> 16) & 0x0f) | ((ar >> 8) & 0xf0); // low 4 bits and high 4 bits
	return;
}

void set_gatedesc(struct GATE_DESCRIPTOR *gd, int offset, int selector , int ar )
{
	gd->offset_low = offset & 0xffff;
	gd->selector = selector;
	gd->dw_count = (ar >> 8 ) & 0xff;
	gd->access_right = ar & 0xff;
	gd->offset_high = (offset >> 16)  & 0xffff;
	return;
}
