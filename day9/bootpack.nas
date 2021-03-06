[FORMAT "WCOFF"]
[INSTRSET "i486p"]
[OPTIMIZE 1]
[OPTION 1]
[BITS 32]
	EXTERN	_init_gdtidt
	EXTERN	_init_pic
	EXTERN	_io_sti
	EXTERN	_init_keyboard
	EXTERN	_memman_init
	EXTERN	_memman_free
	EXTERN	_init_palette
	EXTERN	_init_screen
	EXTERN	_hankaku
	EXTERN	_putfont8
	EXTERN	_putfonts8_asc
	EXTERN	_sprintf
	EXTERN	_enable_mouse
	EXTERN	_init_mouse_cursor8
	EXTERN	_putblock8_8
	EXTERN	_memman_total
	EXTERN	_io_out8
	EXTERN	_keyfifo
	EXTERN	_fifo8_init
	EXTERN	_mousefifo
	EXTERN	_io_cli
	EXTERN	_fifo8_status
	EXTERN	_fifo8_get
	EXTERN	_mouse_decode
	EXTERN	_boxfill8
	EXTERN	_io_stihlt
	EXTERN	_io_load_eflags
	EXTERN	_io_store_eflags
	EXTERN	_memtest_sub
	EXTERN	_load_cr0
	EXTERN	_store_cr0
[FILE "bootpack.c"]
[SECTION .data]
LC0:
	DB	"hello os.",0x00
LC1:
	DB	"scrnx= %d",0x00
LC2:
	DB	"memory %dMB free %dkb",0x00
LC4:
	DB	"[lcr %4d %4d]",0x00
LC5:
	DB	"(%3d,%3d)",0x00
LC3:
	DB	"%02X",0x00
[SECTION .text]
	ALIGN	2
	GLOBAL	_HariMain
_HariMain:
	PUSH	EBP
	MOV	EBP,ESP
	PUSH	EDI
	PUSH	ESI
	PUSH	EBX
	LEA	ESI,DWORD [-60+EBP]
	SUB	ESP,496
	MOV	EBX,2
	CALL	_init_gdtidt
	CALL	_init_pic
	CALL	_io_sti
	CALL	_init_keyboard
	PUSH	-1073741825
	PUSH	4194304
	CALL	_memtest
	PUSH	3932160
	MOV	DWORD [-500+EBP],EAX
	CALL	_memman_init
	PUSH	647168
	PUSH	4096
	PUSH	3932160
	CALL	_memman_free
	MOV	EAX,DWORD [-500+EBP]
	SUB	EAX,4194304
	PUSH	EAX
	PUSH	4194304
	PUSH	3932160
	CALL	_memman_free
	ADD	ESP,36
	CALL	_init_palette
	MOVSX	EAX,WORD [4084]
	MOV	DWORD [-496+EBP],EAX
	MOVSX	EAX,WORD [4086]
	PUSH	EAX
	PUSH	DWORD [-496+EBP]
	PUSH	DWORD [4088]
	CALL	_init_screen
	PUSH	_hankaku+1040
	PUSH	7
	PUSH	8
	PUSH	8
	MOVSX	EAX,WORD [4084]
	PUSH	EAX
	PUSH	DWORD [4088]
	CALL	_putfont8
	ADD	ESP,36
	PUSH	_hankaku+1056
	PUSH	7
	PUSH	8
	PUSH	16
	MOVSX	EAX,WORD [4084]
	PUSH	EAX
	PUSH	DWORD [4088]
	CALL	_putfont8
	PUSH	_hankaku+1072
	PUSH	7
	PUSH	8
	PUSH	24
	MOVSX	EAX,WORD [4084]
	PUSH	EAX
	PUSH	DWORD [4088]
	CALL	_putfont8
	ADD	ESP,48
	PUSH	_hankaku+784
	PUSH	7
	PUSH	8
	PUSH	40
	MOVSX	EAX,WORD [4084]
	PUSH	EAX
	PUSH	DWORD [4088]
	CALL	_putfont8
	PUSH	_hankaku+800
	PUSH	7
	PUSH	8
	PUSH	48
	MOVSX	EAX,WORD [4084]
	PUSH	EAX
	PUSH	DWORD [4088]
	CALL	_putfont8
	ADD	ESP,48
	PUSH	LC0
	PUSH	7
	PUSH	50
	PUSH	60
	MOVSX	EAX,WORD [4084]
	PUSH	EAX
	PUSH	DWORD [4088]
	CALL	_putfonts8_asc
	MOVSX	EAX,WORD [4084]
	PUSH	EAX
	PUSH	LC1
	PUSH	ESI
	CALL	_sprintf
	ADD	ESP,36
	PUSH	ESI
	PUSH	7
	PUSH	64
	PUSH	16
	MOVSX	EAX,WORD [4084]
	PUSH	EAX
	PUSH	DWORD [4088]
	CALL	_putfonts8_asc
	MOVSX	EAX,WORD [4084]
	LEA	ECX,DWORD [-16+EAX]
	MOV	EAX,ECX
	CDQ
	IDIV	EBX
	MOV	EDI,EAX
	MOVSX	EAX,WORD [4086]
	LEA	ECX,DWORD [-44+EAX]
	MOV	EAX,ECX
	CDQ
	IDIV	EBX
	MOV	DWORD [-504+EBP],EAX
	LEA	EBX,DWORD [-316+EBP]
	LEA	EAX,DWORD [-492+EBP]
	PUSH	EAX
	CALL	_enable_mouse
	PUSH	14
	PUSH	EBX
	CALL	_init_mouse_cursor8
	ADD	ESP,36
	PUSH	16
	PUSH	EBX
	PUSH	DWORD [-504+EBP]
	PUSH	EDI
	PUSH	16
	PUSH	16
	MOVSX	EAX,WORD [4084]
	PUSH	EAX
	PUSH	DWORD [4088]
	CALL	_putblock8_8
	ADD	ESP,32
	PUSH	3932160
	CALL	_memman_total
	SHR	DWORD [-500+EBP],20
	SHR	EAX,10
	MOV	DWORD [ESP],EAX
	PUSH	DWORD [-500+EBP]
	PUSH	LC2
	PUSH	ESI
	CALL	_sprintf
	PUSH	ESI
	PUSH	7
	PUSH	132
	PUSH	0
	MOVSX	EAX,WORD [4084]
	PUSH	EAX
	PUSH	DWORD [4088]
	CALL	_putfonts8_asc
	ADD	ESP,40
	PUSH	249
	PUSH	33
	CALL	_io_out8
	PUSH	239
	PUSH	161
	CALL	_io_out8
	LEA	EAX,DWORD [-348+EBP]
	PUSH	EAX
	PUSH	32
	PUSH	_keyfifo
	CALL	_fifo8_init
	LEA	EAX,DWORD [-476+EBP]
	PUSH	EAX
	PUSH	128
	PUSH	_mousefifo
	CALL	_fifo8_init
	ADD	ESP,40
L2:
	CALL	_io_cli
	PUSH	_keyfifo
	CALL	_fifo8_status
	PUSH	_mousefifo
	MOV	EBX,EAX
	CALL	_fifo8_status
	LEA	EAX,DWORD [EAX+EBX*1]
	POP	EBX
	POP	ESI
	TEST	EAX,EAX
	JE	L18
	PUSH	_keyfifo
	CALL	_fifo8_status
	POP	ECX
	TEST	EAX,EAX
	JNE	L19
	PUSH	_mousefifo
	CALL	_fifo8_status
	POP	EDX
	TEST	EAX,EAX
	JE	L2
	PUSH	_mousefifo
	CALL	_fifo8_get
	MOV	EBX,EAX
	CALL	_io_sti
	MOVZX	EAX,BL
	PUSH	EAX
	LEA	EAX,DWORD [-492+EBP]
	PUSH	EAX
	CALL	_mouse_decode
	ADD	ESP,12
	TEST	EAX,EAX
	JE	L2
	PUSH	DWORD [-484+EBP]
	PUSH	DWORD [-488+EBP]
	PUSH	LC4
	LEA	EBX,DWORD [-60+EBP]
	PUSH	EBX
	CALL	_sprintf
	ADD	ESP,16
	MOV	EAX,DWORD [-480+EBP]
	TEST	EAX,1
	JE	L11
	MOV	BYTE [-59+EBP],76
L11:
	TEST	EAX,2
	JE	L12
	MOV	BYTE [-57+EBP],82
L12:
	AND	EAX,4
	JE	L13
	MOV	BYTE [-58+EBP],67
L13:
	PUSH	31
	PUSH	151
	PUSH	16
	PUSH	32
	PUSH	14
	MOVSX	EAX,WORD [4084]
	PUSH	EAX
	PUSH	DWORD [4088]
	CALL	_boxfill8
	PUSH	EBX
	PUSH	7
	PUSH	16
	PUSH	32
	MOVSX	EAX,WORD [4084]
	PUSH	EAX
	PUSH	DWORD [4088]
	CALL	_putfonts8_asc
	MOV	EAX,DWORD [-504+EBP]
	ADD	ESP,52
	ADD	EAX,15
	PUSH	EAX
	LEA	EAX,DWORD [15+EDI]
	PUSH	EAX
	PUSH	DWORD [-504+EBP]
	PUSH	EDI
	PUSH	14
	MOVSX	EAX,WORD [4084]
	PUSH	EAX
	PUSH	DWORD [4088]
	CALL	_boxfill8
	ADD	ESP,28
	MOV	EAX,DWORD [-484+EBP]
	ADD	DWORD [-504+EBP],EAX
	ADD	EDI,DWORD [-488+EBP]
	JS	L20
L14:
	CMP	DWORD [-504+EBP],0
	JS	L21
L15:
	MOVSX	EAX,WORD [4084]
	SUB	EAX,16
	CMP	EDI,EAX
	JLE	L16
	MOV	EDI,EAX
L16:
	MOVSX	EAX,WORD [4086]
	SUB	EAX,16
	CMP	DWORD [-504+EBP],EAX
	JLE	L17
	MOV	EDI,EAX
L17:
	PUSH	DWORD [-504+EBP]
	PUSH	EDI
	PUSH	LC5
	PUSH	EBX
	CALL	_sprintf
	PUSH	15
	PUSH	79
	PUSH	0
	PUSH	0
	PUSH	14
	MOVSX	EAX,WORD [4084]
	PUSH	EAX
	PUSH	DWORD [4088]
	CALL	_boxfill8
	ADD	ESP,44
	PUSH	EBX
	PUSH	7
	PUSH	0
	PUSH	0
	MOVSX	EAX,WORD [4084]
	PUSH	EAX
	PUSH	DWORD [4088]
	CALL	_putfonts8_asc
	LEA	EAX,DWORD [-316+EBP]
	PUSH	16
	PUSH	EAX
	PUSH	DWORD [-504+EBP]
	PUSH	EDI
	PUSH	16
	PUSH	16
	MOVSX	EAX,WORD [4084]
	PUSH	EAX
	PUSH	DWORD [4088]
	CALL	_putblock8_8
	ADD	ESP,56
	JMP	L2
L21:
	MOV	DWORD [-504+EBP],0
	JMP	L15
L20:
	XOR	EDI,EDI
	JMP	L14
L19:
	PUSH	_keyfifo
	CALL	_fifo8_get
	MOV	EBX,EAX
	CALL	_io_sti
	PUSH	EBX
	LEA	EBX,DWORD [-60+EBP]
	PUSH	LC3
	PUSH	EBX
	CALL	_sprintf
	PUSH	31
	PUSH	15
	PUSH	16
	PUSH	0
	PUSH	14
	MOVSX	EAX,WORD [4084]
	PUSH	EAX
	PUSH	DWORD [4088]
	CALL	_boxfill8
	ADD	ESP,44
	PUSH	EBX
	PUSH	7
	PUSH	16
	PUSH	0
	MOVSX	EAX,WORD [4084]
	PUSH	EAX
	PUSH	DWORD [4088]
	CALL	_putfonts8_asc
	ADD	ESP,24
	JMP	L2
L18:
	CALL	_io_stihlt
	JMP	L2
	ALIGN	2
	GLOBAL	_memtest
_memtest:
	PUSH	EBP
	MOV	EBP,ESP
	PUSH	ESI
	PUSH	EBX
	XOR	ESI,ESI
	CALL	_io_load_eflags
	OR	EAX,262144
	PUSH	EAX
	CALL	_io_store_eflags
	CALL	_io_load_eflags
	POP	EDX
	TEST	EAX,262144
	JE	L23
	MOV	ESI,1
L23:
	AND	EAX,-262145
	PUSH	EAX
	CALL	_io_store_eflags
	MOV	EAX,ESI
	POP	EBX
	TEST	AL,AL
	JNE	L26
L24:
	PUSH	DWORD [12+EBP]
	PUSH	DWORD [8+EBP]
	CALL	_memtest_sub
	MOV	EBX,EAX
	POP	EAX
	MOV	EAX,ESI
	POP	EDX
	TEST	AL,AL
	JNE	L27
L25:
	LEA	ESP,DWORD [-8+EBP]
	MOV	EAX,EBX
	POP	EBX
	POP	ESI
	POP	EBP
	RET
L27:
	CALL	_load_cr0
	AND	EAX,-1610612737
	PUSH	EAX
	CALL	_store_cr0
	POP	EAX
	JMP	L25
L26:
	CALL	_load_cr0
	OR	EAX,1610612736
	PUSH	EAX
	CALL	_store_cr0
	POP	ECX
	JMP	L24
