.text

.global _start
_start:
	popl %ecx   ## argc

vnext:
	popl %ecx
	test %ecx,%ecx   ##空指针表明结束
	jz exit
	movl %ecx,%ebx
	xorl %edx,%edx


strlen:
	movb (%ebx),%al  ## al is what ?
	inc %edx
	inc %ebx 
	test %al,%al
	jnz strlen
	movb $10,-1(%ebx)
	movl $4,%eax
	movl $1,%ebx  ## stdout
	int $0x80
	jmp vnext

exit:
	movl $1,%eax  ## exit signal
	xorl %ebx,%ebx
	int $0x80

	ret
