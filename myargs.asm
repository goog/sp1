section .text
	global _start

_start:
	pop	eax		; Get the number of arguments
	pop	ebx		; Get the program name
	pop	ebx		; Get the first actual argument ("foo")
	pop	ecx		; "bar"
	pop	edx		; "42"

	mov	eax,1
	mov	ebx,0
	int	80h		; Exit
