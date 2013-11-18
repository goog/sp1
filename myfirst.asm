section .data
	hello:     db 'Hello world!',10    ; 'Hello world!' plus a linefeed character
	helloLen:  equ $-hello

section .text
	global _start
	
_start:
	mov eax,4
	mov ebx,1
	mov ecx,hello
	mov edx,helloLen
	int 0x80
	
	; for exit 
	mov eax,1
	mov ebx,0
	int 0x80
	