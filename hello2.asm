SECTION .DATA
	hello: db 'Hello world!',10
	helloLen: equ $-hello
SECTION .TEXT
GLOBAL _start
_start:
	mov eax,4 ; 'write' 系统调用
	mov ebx,1 ; 描述符1 = 屏幕
	mov ecx,hello ; 要写的字符串
	mov edx,helloLen ; 字符串长度
	int 80h ; 呼叫内核
	mov eax,1 ; 错误代码设为0
	mov ebx,0 ; 呼叫内核
	int 80h


