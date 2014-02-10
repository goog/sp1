[FORMAT "WCOFF"]
[INSTRSET "i486p"]
[BITS 32]

[FILE "naskfunc.nas"]
	GLOBAL _io_hlt
	GLOBAL _write_mem8 ;
	GLOBAL _io_cli,_io_sti,_io_stihlt
	GLOBAL _io_in8,_io_in16,_io_in32
	GLOBAL _io_out8,_io_out16,_io_out32
	GLOBAL _io_load_eflags,_io_store_eflags
	GLOBAL _load_gdtr,_load_idtr,_load_tr
	GLOBAL _load_cr0,_store_cr0
	GLOBAL	_memtest_sub,_taskswitch4,_taskswitch3, _farjmp
	GLOBAL	_asm_inthandler21, _asm_inthandler2c,_asm_inthandler20,_asm_inthandler27
	EXTERN	_inthandler21,_inthandler2c,_inthandler20,_inthandler27
[SECTION .text]
_io_hlt:    ; void io_hlt(void);
	HLT
	RET

_write_mem8: ; void write_mem8(int addr, int data)  ;
	MOV ECX,[ESP+4] ; move the address to 32bits counter
	MOV AL,[ESP+8]  ; write 8bits data
	MOV [ECX],AL
	RET


_io_cli:  ;   void io_cli(void);

	CLI
	RET

_io_sti: ; void io_sti(void);
	STI
	RET

_io_stihlt:  ; void io_stihlt(void);
	STI
	HLT
	RET


_io_in8:  ; int io_in8(int port);
	MOV EDX,[ESP+4]
	MOV EAX,0
	IN AL,DX
	RET 


_io_in16:  ; int io_in16(int port);
	MOV EDX,[ESP+4]
	MOV EAX,0
	IN AX,DX
	RET 


_io_in32:  ; int io_in32(int port);
	MOV EDX,[ESP+4]
	IN EAX,DX
	RET 

_io_out8:  ; void io_out8(int port,int data);
	MOV EDX,[ESP+4]
	MOV AL,[ESP+8]
	OUT DX,AL
	RET


_io_out16:  ; void io_out16(int port,int data);
	MOV EDX,[ESP+4]
	MOV EAX,[ESP+8]
	OUT DX,AX
	RET

_io_out32:  ; void io_out32(int port,int data);
	MOV EDX,[ESP+4]
	MOV EAX,[ESP+8]
	OUT DX,EAX
	RET

_io_load_eflags:  ; int io_load_eflags(void);
	PUSHFD
	POP EAX
	RET


_io_store_eflags:  ; void io_store_eflags(int eflags);
	MOV EAX,[ESP+4]
	PUSH EAX
	POPFD
	RET

_load_gdtr:	; void load_gdtr(int limit, int addr);
		MOV AX,[ESP+4]		; limit
		MOV [ESP+6],AX
		LGDT [ESP+6]
		RET

_load_idtr:		; void load_idtr(int limit, int addr);
		MOV AX,[ESP+4]		; limit
		MOV [ESP+6],AX
		LIDT [ESP+6]
		RET
_load_cr0:		; int load_cr0(void);
		MOV		EAX,CR0
		RET

_store_cr0:		; void store_cr0(int cr0);
		MOV		EAX,[ESP+4]
		MOV		CR0,EAX
		RET

_asm_inthandler21:
	PUSH ES
	PUSH DS
	PUSHAD
	MOV EAX,ESP
	PUSH EAX
	MOV AX,SS 
	MOV DS,AX
	MOV ES,AX
	CALL _inthandler21
	POP EAX
	POPAD
	POP DS
	POP ES
	IRETD

_asm_inthandler2c:
	PUSH	ES
	PUSH	DS
	PUSHAD
	MOV		EAX,ESP
	PUSH	EAX
	MOV		AX,SS
	MOV		DS,AX
	MOV		ES,AX
	CALL	_inthandler2c
	POP		EAX
	POPAD
	POP		DS
	POP		ES
	IRETD

_asm_inthandler20:
	PUSH	ES
	PUSH	DS
	PUSHAD
	MOV	EAX,ESP
	PUSH	EAX
	MOV	AX,SS
	MOV	DS,AX
	MOV	ES,AX
	CALL	_inthandler20
	POP		EAX
	POPAD
	POP		DS
	POP		ES
	IRETD

_asm_inthandler27:
	PUSH	ES
	PUSH	DS
	PUSHAD
	MOV	EAX,ESP
	PUSH	EAX
	MOV	AX,SS
	MOV	DS,AX
	MOV	ES,AX
	CALL	_inthandler27
	POP		EAX
	POPAD
	POP		DS
	POP		ES
	IRETD

_memtest_sub: ; unsigned int memtest_sub(unsigned int start, unsigned int end)
	PUSH EDI
	PUSH ESI
	PUSH EBX
	MOV ESI,0xaa55aa55
	MOV EDI,0x55aa55aa
	MOV EAX,[ESP+12+4]

mts_loop:
	MOV EBX,EAX
	ADD EBX,0xffc
	MOV EDX,[EBX]
	MOV [EBX],ESI
	XOR DWORD [EBX],0xffffffff
	CMP EDI,[EBX]
	JNE mts_fin
	XOR DWORD [EBX],0xffffffff
	CMP ESI,[EBX]
	JNE mts_fin
	MOV [EBX],EDX
	ADD EAX,0x1000
	CMP EAX,[ESP+12+8]
	JBE mts_loop
	POP EBX
	POP ESI
	POP EDI

mts_fin:
	MOV [EBX],EDX
	POP EBX
	POP ESI
	POP EDI
	RET

_load_tr:
	LTR [ESP+4]
	RET

_taskswitch4:
	JMP 4*8:0
	RET
_taskswitch3:
	JMP 3*8:0
	RET


_farjmp:
	JMP FAR [ESP+4]
	RET
