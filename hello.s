#hello.s
.data 
	msg : .string "hello world !\n"
	len = . - msg

.text 
.global _start


_start:
	movl $1,%ebx      ##fd 
	movl $msg,%ecx    ## buf
	movl $len,%edx    ## count
	
	movl $4,%eax   ##write call
	int $0x80

	movl $0,%ebx
	movl $1,%eax   ## sys_exit
	int $0x80
