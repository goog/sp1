NASK =  ../nask
EDIMG = ../edimg
CC1 = ../cc1
GAS2NASK = ../gas2nask
OBJ2BIM = ../obj2bim
BIM2HRB = ../bim2hrb
RULEFILE = ../haribote.rul

run: haribote.img
	qemu-system-x86_64 -fda  haribote.img

asm:ipl.nas
	../nask ipl.nas ipl.bin ipl.lst

asmhead.bin:asmhead.nas
	$(NASK) asmhead.nas asmhead.bin asmhead.lst

bootpack.gas : bootpack.c
	$(CC1) -o bootpack.gas bootpack.c

bootpack.nas : bootpack.gas
	$(GAS2NASK) bootpack.gas bootpack.nas

bootpack.obj : bootpack.nas
	$(NASK) bootpack.nas bootpack.obj bootpack.lst

naskfunc.obj : naskfunc.nas
	$(NASK) naskfunc.nas naskfunc.obj naskfunc.lst

bim : bootpack.obj naskfunc.obj
	$(OBJ2BIM) @$(RULEFILE) out:bootpack.bim stack:3136k map:bootpack.map bootpack.obj naskfunc.obj

bootpack.hrb : bootpack.bim
	$(BIM2HRB) bootpack.bim bootpack.hrb 0


sys: asmhead.bin bootpack.hrb
	cat asmhead.bin bootpack.hrb >  haribote.sys

img : ipl.bin haribote.sys
		$(EDIMG)   imgin:../fdimg0at.tek \
			   wbinimg src:ipl.bin len:512 from:0 to:0 \
			   copy from:haribote.sys to:@: imgout:haribote.img



#img: ipl.bin
#	../edimg   imgin:../fdimg0at.tek   wbinimg src:ipl.bin len:512 from:0 to:0   imgout:helloos.img

clean:
	-del ipl.lst
	-del ipl.bin

