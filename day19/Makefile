NASK = ../nask
EDIMG = ../edimg
GAS2NASK = ../gas2nask
OBJ2BIM = ../obj2bim
BIM2HRB = ../bim2hrb
RULEFILE = ../haribote.rul
BIN2OBJ = ../bin2obj
MAKEFONT = ../makefont
INCPATH = ../z_tools/haribote
CC1 = ../cc1 -I$(INCPATH) -Os -Wall -quiet
OBJS_BOOTPACK = bootpack.obj naskfunc.obj hankaku.obj graphic.obj dsctbl.obj int.obj fifo.obj \
		keyboard.obj mouse.obj memory.obj sheet.obj window.obj timer.obj functions.obj \
		task.obj file.obj console.obj

run: haribote.img
	qemu-system-x86_64 -fda  haribote.img

asm:ipl.nas
	../nask ipl.nas ipl.bin ipl.lst

asmhead.bin:asmhead.nas
	$(NASK) asmhead.nas asmhead.bin asmhead.lst

%.gas: %.c
	$(CC1) -o $*.gas $*.c

%.nas: %.gas
	$(GAS2NASK) $*.gas $*.nas

%.obj: %.nas
	$(NASK) $*.nas $*.obj $*.lst

bootpack.bim : $(OBJS_BOOTPACK)
	$(OBJ2BIM) @$(RULEFILE) out:bootpack.bim stack:3136k map:bootpack.map $(OBJS_BOOTPACK)

bootpack.hrb : bootpack.bim
	$(BIM2HRB) bootpack.bim bootpack.hrb 0

hankaku.bin : hankaku.txt
	$(MAKEFONT) hankaku.txt hankaku.bin

hankaku.obj : hankaku.bin
	$(BIN2OBJ) hankaku.bin hankaku.obj _hankaku


haribote.sys: asmhead.bin bootpack.hrb
	cat asmhead.bin bootpack.hrb >  haribote.sys

img : ipl.bin haribote.sys
	$(EDIMG)   imgin:../fdimg0at.tek wbinimg src:ipl.bin len:512 from:0 to:0 \
			   copy from:haribote.sys to:@: \
			   copy from:ipl.nas to:@: \
			   copy from:hlt.hrb to:@: imgout:haribote.img

hlt.hrb : hlt.nas
	$(NASK) hlt.nas hlt.hrb hlt.lst

clean:
	-del ipl.lst
	-del ipl.bin

