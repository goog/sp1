run: helloos.img
	qemu-system-x86_64 -fda  helloos.img

asm: ipl.nas
	../../nask ipl.nas ipl.bin ipl.lst

img: ipl.bin
	../../edimg   imgin:../../fdimg0at.tek   wbinimg src:ipl.bin len:512 from:0 to:0   imgout:helloos.img

clean:
	-del ipl.lst
	-del ipl.bin

