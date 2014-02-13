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
	GLOBAL	_memtest_sub,_taskswitch4,_taskswitch3, _farjmp, _asm_cons_putchar,_farcall
	GLOBAL	_asm_inthandler21, _asm_inthandler2c,_asm_inthandler20,_asm_inthandler27,_asm_inthandler0c
	GLOBAL	_asm_hrb_api,_start_app, _asm_end_app
	EXTERN	_inthandler21,_inthandler2c,_inthandler20,_inthandler27,_cons_putchar,_hrb_api,_inthandler0c
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

_asm_inthandler20:
		PUSH	ES
		PUSH	DS
		PUSHAD
		MOV		AX,SS
		CMP		AX,1*8
		JNE		.from_app
;	OSª®¢Ä¢éÆ«ÉèÜêœÌÅÙÚ¡ÜÅÇšè
		MOV		EAX,ESP
		PUSH	SS				; èÜêœÆ«ÌSSðÛ¶
		PUSH	EAX				; èÜêœÆ«ÌESPðÛ¶
		MOV		AX,SS
		MOV		DS,AX
		MOV		ES,AX
		CALL	_inthandler20
		ADD		ESP,8
		POPAD
		POP		DS
		POP		ES
		IRETD
.from_app:
;	Avª®¢Ä¢éÆ«ÉèÜêœ
		MOV		EAX,1*8
		MOV		DS,AX			; Æè ŠžDSŸ¯OSpÉ·é
		MOV		ECX,[0xfe4]		; OSÌESP
		ADD		ECX,-8
		MOV		[ECX+4],SS		; èÜêœÆ«ÌSSðÛ¶
		MOV		[ECX  ],ESP		; èÜêœÆ«ÌESPðÛ¶
		MOV		SS,AX
		MOV		ES,AX
		MOV		ESP,ECX
		CALL	_inthandler20
		POP		ECX
		POP		EAX
		MOV		SS,AX			; SSðAvpÉß·
		MOV		ESP,ECX			; ESPàAvpÉß·
		POPAD
		POP		DS
		POP		ES
		IRETD

_asm_inthandler21:
		PUSH	ES
		PUSH	DS
		PUSHAD
		MOV		AX,SS
		CMP		AX,1*8
		JNE		.from_app
;	OSª®¢Ä¢éÆ«ÉèÜêœÌÅÙÚ¡ÜÅÇšè
		MOV		EAX,ESP
		PUSH	SS				; èÜêœÆ«ÌSSðÛ¶
		PUSH	EAX				; èÜêœÆ«ÌESPðÛ¶
		MOV		AX,SS
		MOV		DS,AX
		MOV		ES,AX
		CALL	_inthandler21
		ADD		ESP,8
		POPAD
		POP		DS
		POP		ES
		IRETD
.from_app:
;	Avª®¢Ä¢éÆ«ÉèÜêœ
		MOV		EAX,1*8
		MOV		DS,AX			; Æè ŠžDSŸ¯OSpÉ·é
		MOV		ECX,[0xfe4]		; OSÌESP
		ADD		ECX,-8
		MOV		[ECX+4],SS		; èÜêœÆ«ÌSSðÛ¶
		MOV		[ECX  ],ESP		; èÜêœÆ«ÌESPðÛ¶
		MOV		SS,AX
		MOV		ES,AX
		MOV		ESP,ECX
		CALL	_inthandler21
		POP		ECX
		POP		EAX
		MOV		SS,AX			; SSðAvpÉß·
		MOV		ESP,ECX			; ESPàAvpÉß·
		POPAD
		POP		DS
		POP		ES
		IRETD

_asm_inthandler27:
		PUSH	ES
		PUSH	DS
		PUSHAD
		MOV		AX,SS
		CMP		AX,1*8
		JNE		.from_app
;	OSª®¢Ä¢éÆ«ÉèÜêœÌÅÙÚ¡ÜÅÇšè
		MOV		EAX,ESP
		PUSH	SS				; èÜêœÆ«ÌSSðÛ¶
		PUSH	EAX				; èÜêœÆ«ÌESPðÛ¶
		MOV		AX,SS
		MOV		DS,AX
		MOV		ES,AX
		CALL	_inthandler27
		ADD		ESP,8
		POPAD
		POP		DS
		POP		ES
		IRETD
.from_app:
;	Avª®¢Ä¢éÆ«ÉèÜêœ
		MOV		EAX,1*8
		MOV		DS,AX			; Æè ŠžDSŸ¯OSpÉ·é
		MOV		ECX,[0xfe4]		; OSÌESP
		ADD		ECX,-8
		MOV		[ECX+4],SS		; èÜêœÆ«ÌSSðÛ¶
		MOV		[ECX  ],ESP		; èÜêœÆ«ÌESPðÛ¶
		MOV		SS,AX
		MOV		ES,AX
		MOV		ESP,ECX
		CALL	_inthandler27
		POP		ECX
		POP		EAX
		MOV		SS,AX			; SSðAvpÉß·
		MOV		ESP,ECX			; ESPàAvpÉß·
		POPAD
		POP		DS
		POP		ES
		IRETD

_asm_inthandler2c:
		PUSH	ES
		PUSH	DS
		PUSHAD
		MOV		AX,SS
		CMP		AX,1*8
		JNE		.from_app
;	OSª®¢Ä¢éÆ«ÉèÜêœÌÅÙÚ¡ÜÅÇšè
		MOV		EAX,ESP
		PUSH	SS				; èÜêœÆ«ÌSSðÛ¶
		PUSH	EAX				; èÜêœÆ«ÌESPðÛ¶
		MOV		AX,SS
		MOV		DS,AX
		MOV		ES,AX
		CALL	_inthandler2c
		ADD		ESP,8
		POPAD
		POP		DS
		POP		ES
		IRETD
.from_app:
;	Avª®¢Ä¢éÆ«ÉèÜêœ
		MOV		EAX,1*8
		MOV		DS,AX			; Æè ŠžDSŸ¯OSpÉ·é
		MOV		ECX,[0xfe4]		; OSÌESP
		ADD		ECX,-8
		MOV		[ECX+4],SS		; èÜêœÆ«ÌSSðÛ¶
		MOV		[ECX  ],ESP		; èÜêœÆ«ÌESPðÛ¶
		MOV		SS,AX
		MOV		ES,AX
		MOV		ESP,ECX
		CALL	_inthandler2c
		POP		ECX
		POP		EAX
		MOV		SS,AX			; SSðAvpÉß·
		MOV		ESP,ECX			; ESPàAvpÉß·
		POPAD
		POP		DS
		POP		ES
		IRETD

_asm_inthandler0c:   ; stack exception
		STI
		PUSH	ES
		PUSH	DS
		PUSHAD
		MOV		EAX,ESP
		PUSH	EAX
		MOV		AX,SS
		MOV		DS,AX
		MOV		ES,AX
		CALL	_inthandler0c
		CMP		EAX,0
		JNE		_asm_end_app
		POP		EAX
		POPAD
		POP		DS
		POP		ES
		ADD		ESP,4
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


_asm_cons_putchar:
	STI  ; open interupt
	PUSHAD
	PUSH 1
	AND EAX,0xff  ; EAX = AL
	PUSH EAX
	PUSH DWORD [0x0fec] ; push 3 parameters, 4 bytes
	CALL _cons_putchar
	ADD ESP,12
	POPAD
	IRETD

_farcall:
	CALL FAR [ESP +4]
	RET

_asm_hrb_api:  ; int 0x40 handler
		PUSH	DS
		PUSH	ES
		PUSHAD
		MOV		EAX,1*8
		MOV		DS,AX   ; set os data segment
		MOV		ECX,[0xfe4]
		ADD		ECX,-40    ; OS esp - 40
		MOV		[ECX+32],ESP	; keep current things first, application ESP
		MOV		[ECX+36],SS		; application SS

; PUSHAD后的值复制到系统栈, application parameters copy to os's esp
		MOV		EDX,[ESP   ]
		MOV		EBX,[ESP+ 4]
		MOV		[ECX   ],EDX	; copy to hrb_api
		MOV		[ECX+ 4],EBX	; copy to hrb_api
		MOV		EDX,[ESP+ 8]
		MOV		EBX,[ESP+12]
		MOV		[ECX+ 8],EDX	; copy to hrb_api
		MOV		[ECX+12],EBX	; copy to hrb_api
		MOV		EDX,[ESP+16]
		MOV		EBX,[ESP+20]
		MOV		[ECX+16],EDX	; copy to hrb_api
		MOV		[ECX+20],EBX	; copy to hrb_api
		MOV		EDX,[ESP+24]
		MOV		EBX,[ESP+28]
		MOV		[ECX+24],EDX	; copy to hrb_api
		MOV		[ECX+28],EBX	; copy to hrb_api

		MOV		ES,AX
		MOV		SS,AX
		MOV		ESP,ECX
		STI

		CALL	_hrb_api

		MOV		ECX,[ESP+32]	; get application's ESP
		MOV		EAX,[ESP+36]	; get application's SS
		CLI
		MOV		SS,AX
		MOV		ESP,ECX   ; return to application
		POPAD
		POP		ES
		POP		DS
		IRETD

_asm_end_app:
		MOV	ESP,[EAX]  ;EAX is the address of tss.esp0
		MOV	DWORD [EAX+4],0
		POPAD
		RET

_start_app:		; void start_app(int eip, int cs, int esp, int ds);
		PUSHAD
		MOV		EAX,[ESP+36]	; application EIP (passed)
		MOV		ECX,[ESP+40]	; application CS
		MOV		EDX,[ESP+44]	; application ESP
		MOV		EBX,[ESP+48]	; application DS/SS
		MOV		[0xfe4],ESP		; OS's ESP
		CLI
		MOV		ES,BX
		MOV		SS,BX
		MOV		DS,BX
		MOV		FS,BX
		MOV		GS,BX   ; data segment setting
		MOV		ESP,EDX
		STI
		PUSH	ECX				; far-CALL cs
		PUSH	EAX				; far-CALL eip
		CALL	FAR [ESP]		;  call application

;	after application ret 

		MOV		EAX,1*8			; OS's DS
		CLI
		MOV		ES,AX
		MOV		SS,AX
		MOV		DS,AX
		MOV		FS,AX
		MOV		GS,AX
		MOV		ESP,[0xfe4]
		STI
		POPAD
		RET


