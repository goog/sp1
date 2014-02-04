[FORMAT "WCOFF"]
[INSTRSET "i486p"]
[OPTIMIZE 1]
[OPTION 1]
[BITS 32]
	EXTERN	_fifo8_init
	EXTERN	_init_gdtidt
	EXTERN	_init_pic
	EXTERN	_io_sti
	EXTERN	_keyfifo
	EXTERN	_mousefifo
	EXTERN	_init_pit
	EXTERN	_io_out8
	EXTERN	_timer_alloc
	EXTERN	_timer_set
	EXTERN	_init_keyboard
	EXTERN	_memtest
	EXTERN	_memman_init
	EXTERN	_memman_free
	EXTERN	_init_palette
	EXTERN	_shtctl_init
	EXTERN	_sheet_alloc
	EXTERN	_memman_alloc_4k
	EXTERN	_sheet_setbuf
	EXTERN	_init_screen
	EXTERN	_enable_mouse
	EXTERN	_init_mouse_cursor8
	EXTERN	_make_window8
	EXTERN	_sheet_move
	EXTERN	_sheet_updown
	EXTERN	_sprintf
	EXTERN	_putfonts8_asc
	EXTERN	_memman_total
	EXTERN	_sheet_refresh
	EXTERN	_timerctl
	EXTERN	_boxfill8
	EXTERN	_io_cli
	EXTERN	_fifo8_status
	EXTERN	_fifo8_get
	EXTERN	_mouse_decode
[FILE "bootpack.c"]
[SECTION .data]
LC0:
	DB	"counter",0x00
LC1:
	DB	"%3d %3d",0x00
LC2:
	DB	"memory %dMB free %dkb",0x00
LC3:
	DB	"%010d",0x00
LC8:
	DB	"3s",0x00
LC7:
	DB	"10",0x00
LC5:
	DB	"[lcr %4d %4d]",0x00
LC6:
	DB	"(%3d,%3d)",0x00
LC4:
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
	LEA	EAX,DWORD [-596+EBP]
	SUB	ESP,644
	LEA	EDI,DWORD [-540+EBP]
	PUSH	EAX
	LEA	EAX,DWORD [-508+EBP]
	PUSH	8
	PUSH	EAX
	CALL	_fifo8_init
	LEA	EAX,DWORD [-604+EBP]
	PUSH	EAX
	PUSH	8
	PUSH	EDI
	CALL	_fifo8_init
	LEA	EDX,DWORD [-572+EBP]
	LEA	EAX,DWORD [-612+EBP]
	PUSH	EAX
	PUSH	8
	PUSH	EDX
	CALL	_fifo8_init
	ADD	ESP,36
	CALL	_init_gdtidt
	CALL	_init_pic
	CALL	_io_sti
	LEA	EAX,DWORD [-300+EBP]
	PUSH	EAX
	PUSH	32
	PUSH	_keyfifo
	CALL	_fifo8_init
	LEA	EAX,DWORD [-428+EBP]
	PUSH	EAX
	PUSH	128
	PUSH	_mousefifo
	CALL	_fifo8_init
	CALL	_init_pit
	PUSH	248
	PUSH	33
	CALL	_io_out8
	ADD	ESP,32
	PUSH	239
	PUSH	161
	CALL	_io_out8
	CALL	_timer_alloc
	MOV	EBX,EAX
	CALL	_timer_alloc
	MOV	ESI,EAX
	CALL	_timer_alloc
	PUSH	1
	MOV	DWORD [-640+EBP],EAX
	LEA	EAX,DWORD [-508+EBP]
	PUSH	EAX
	PUSH	1000
	PUSH	EBX
	CALL	_timer_set
	PUSH	1
	PUSH	EDI
	PUSH	300
	PUSH	ESI
	CALL	_timer_set
	LEA	EDX,DWORD [-572+EBP]
	ADD	ESP,40
	PUSH	1
	PUSH	EDX
	PUSH	50
	PUSH	DWORD [-640+EBP]
	CALL	_timer_set
	CALL	_init_keyboard
	PUSH	-1073741825
	PUSH	4194304
	CALL	_memtest
	PUSH	3932160
	MOV	ESI,EAX
	CALL	_memman_init
	PUSH	647168
	PUSH	4096
	PUSH	3932160
	CALL	_memman_free
	LEA	EAX,DWORD [-4194304+ESI]
	SHR	ESI,20
	ADD	ESP,40
	PUSH	EAX
	PUSH	4194304
	PUSH	3932160
	CALL	_memman_free
	CALL	_init_palette
	MOVSX	EDI,WORD [4086]
	MOVSX	EBX,WORD [4084]
	PUSH	EDI
	MOV	DWORD [-616+EBP],EBX
	MOVSX	EAX,WORD [4084]
	PUSH	EAX
	PUSH	DWORD [4088]
	PUSH	3932160
	CALL	_shtctl_init
	MOV	EBX,EAX
	PUSH	EAX
	CALL	_sheet_alloc
	ADD	ESP,32
	PUSH	EBX
	MOV	DWORD [-620+EBP],EAX
	CALL	_sheet_alloc
	PUSH	EBX
	LEA	EBX,DWORD [-268+EBP]
	MOV	DWORD [-624+EBP],EAX
	CALL	_sheet_alloc
	MOVSX	EDX,WORD [4086]
	MOV	DWORD [-628+EBP],EAX
	MOVSX	EAX,WORD [4084]
	IMUL	EAX,EDX
	PUSH	EAX
	PUSH	3932160
	CALL	_memman_alloc_4k
	PUSH	8320
	PUSH	3932160
	MOV	DWORD [-632+EBP],EAX
	CALL	_memman_alloc_4k
	PUSH	-1
	MOV	DWORD [-636+EBP],EAX
	MOVSX	EAX,WORD [4086]
	PUSH	EAX
	MOVSX	EAX,WORD [4084]
	PUSH	EAX
	PUSH	DWORD [-632+EBP]
	PUSH	DWORD [-620+EBP]
	CALL	_sheet_setbuf
	ADD	ESP,44
	PUSH	99
	PUSH	16
	PUSH	16
	PUSH	EBX
	PUSH	DWORD [-624+EBP]
	CALL	_sheet_setbuf
	PUSH	-1
	PUSH	52
	PUSH	160
	PUSH	DWORD [-636+EBP]
	PUSH	DWORD [-628+EBP]
	CALL	_sheet_setbuf
	ADD	ESP,40
	PUSH	EDI
	PUSH	DWORD [-616+EBP]
	PUSH	DWORD [-632+EBP]
	CALL	_init_screen
	LEA	EAX,DWORD [-588+EBP]
	PUSH	EAX
	CALL	_enable_mouse
	PUSH	99
	PUSH	EBX
	MOV	EBX,2
	CALL	_init_mouse_cursor8
	PUSH	LC0
	PUSH	52
	PUSH	160
	PUSH	DWORD [-636+EBP]
	CALL	_make_window8
	ADD	ESP,40
	MOVSX	EAX,WORD [4084]
	LEA	ECX,DWORD [-16+EAX]
	MOV	EAX,ECX
	CDQ
	IDIV	EBX
	MOV	DWORD [-648+EBP],EAX
	MOVSX	EAX,WORD [4086]
	PUSH	0
	LEA	ECX,DWORD [-44+EAX]
	PUSH	0
	MOV	EAX,ECX
	CDQ
	IDIV	EBX
	PUSH	DWORD [-620+EBP]
	MOV	DWORD [-652+EBP],EAX
	LEA	EBX,DWORD [-476+EBP]
	CALL	_sheet_move
	PUSH	DWORD [-652+EBP]
	PUSH	DWORD [-648+EBP]
	PUSH	DWORD [-624+EBP]
	CALL	_sheet_move
	PUSH	72
	PUSH	80
	PUSH	DWORD [-628+EBP]
	CALL	_sheet_move
	ADD	ESP,36
	PUSH	0
	PUSH	DWORD [-620+EBP]
	CALL	_sheet_updown
	PUSH	1
	PUSH	DWORD [-628+EBP]
	CALL	_sheet_updown
	PUSH	2
	PUSH	DWORD [-624+EBP]
	CALL	_sheet_updown
	PUSH	DWORD [-652+EBP]
	PUSH	DWORD [-648+EBP]
	PUSH	LC1
	PUSH	EBX
	CALL	_sprintf
	ADD	ESP,40
	PUSH	EBX
	PUSH	7
	PUSH	0
	PUSH	0
	MOVSX	EAX,WORD [4084]
	PUSH	EAX
	PUSH	DWORD [4088]
	CALL	_putfonts8_asc
	PUSH	3932160
	CALL	_memman_total
	SHR	EAX,10
	MOV	DWORD [ESP],EAX
	PUSH	ESI
	PUSH	LC2
	PUSH	EBX
	CALL	_sprintf
	ADD	ESP,40
	PUSH	EBX
	PUSH	7
	PUSH	32
	PUSH	0
	MOVSX	EAX,WORD [4084]
	PUSH	EAX
	PUSH	DWORD [-632+EBP]
	CALL	_putfonts8_asc
	PUSH	48
	MOVSX	EAX,WORD [4084]
	PUSH	EAX
	PUSH	0
L27:
	PUSH	0
	PUSH	DWORD [-620+EBP]
	CALL	_sheet_refresh
	ADD	ESP,44
L2:
	PUSH	DWORD [_timerctl]
	PUSH	LC3
	LEA	EBX,DWORD [-476+EBP]
	PUSH	EBX
	LEA	ESI,DWORD [-508+EBP]
	LEA	EDI,DWORD [-540+EBP]
	CALL	_sprintf
	PUSH	43
	PUSH	119
	PUSH	28
	PUSH	40
	PUSH	8
	PUSH	160
	PUSH	DWORD [-636+EBP]
	CALL	_boxfill8
	ADD	ESP,40
	PUSH	EBX
	PUSH	0
	PUSH	28
	PUSH	40
	PUSH	160
	PUSH	DWORD [-636+EBP]
	CALL	_putfonts8_asc
	PUSH	44
	PUSH	120
	PUSH	28
	PUSH	40
	PUSH	DWORD [-628+EBP]
	CALL	_sheet_refresh
	ADD	ESP,44
	CALL	_io_cli
	PUSH	_keyfifo
	CALL	_fifo8_status
	PUSH	_mousefifo
	MOV	EBX,EAX
	CALL	_fifo8_status
	PUSH	ESI
	ADD	EBX,EAX
	CALL	_fifo8_status
	PUSH	EDI
	ADD	EBX,EAX
	CALL	_fifo8_status
	LEA	EDX,DWORD [-572+EBP]
	ADD	EBX,EAX
	PUSH	EDX
	CALL	_fifo8_status
	ADD	ESP,20
	LEA	EAX,DWORD [EAX+EBX*1]
	TEST	EAX,EAX
	JE	L29
	PUSH	_keyfifo
	CALL	_fifo8_status
	POP	ECX
	TEST	EAX,EAX
	JNE	L30
	PUSH	_mousefifo
	CALL	_fifo8_status
	POP	EDX
	TEST	EAX,EAX
	JNE	L31
	PUSH	ESI
	CALL	_fifo8_status
	POP	EBX
	TEST	EAX,EAX
	JNE	L32
	PUSH	EDI
	CALL	_fifo8_status
	POP	ECX
	TEST	EAX,EAX
	JNE	L33
	LEA	EDX,DWORD [-572+EBP]
	PUSH	EDX
	CALL	_fifo8_status
	POP	EDX
	TEST	EAX,EAX
	JE	L2
	LEA	EBX,DWORD [-572+EBP]
	PUSH	EBX
	CALL	_fifo8_get
	MOV	EBX,EAX
	CALL	_io_sti
	POP	EAX
	TEST	EBX,EBX
	JE	L24
	MOV	EAX,DWORD [-640+EBP]
	MOV	BYTE [12+EAX],0
	PUSH	111
	PUSH	15
	PUSH	96
	PUSH	8
	PUSH	7
L28:
	MOVSX	EAX,WORD [4084]
	PUSH	EAX
	PUSH	DWORD [-632+EBP]
	CALL	_boxfill8
	MOV	EBX,DWORD [-640+EBP]
	ADD	ESP,28
	MOVZX	EAX,BYTE [12+EBX]
	PUSH	EAX
	LEA	EAX,DWORD [-572+EBP]
	PUSH	EAX
	PUSH	50
	PUSH	EBX
	CALL	_timer_set
	PUSH	111
	PUSH	15
	PUSH	96
	PUSH	8
	PUSH	DWORD [-620+EBP]
	CALL	_sheet_refresh
	ADD	ESP,36
	JMP	L2
L24:
	MOV	EDX,DWORD [-640+EBP]
	MOV	BYTE [12+EDX],1
	PUSH	111
	PUSH	15
	PUSH	96
	PUSH	8
	PUSH	14
	JMP	L28
L33:
	PUSH	EDI
	CALL	_fifo8_get
	CALL	_io_sti
	PUSH	LC8
	PUSH	7
	PUSH	80
L26:
	PUSH	0
	MOVSX	EAX,WORD [4084]
	PUSH	EAX
	PUSH	DWORD [-632+EBP]
	CALL	_putfonts8_asc
	PUSH	100
	PUSH	100
	PUSH	0
	PUSH	0
	PUSH	DWORD [-620+EBP]
	CALL	_sheet_refresh
	ADD	ESP,48
	JMP	L2
L32:
	PUSH	ESI
	CALL	_fifo8_get
	CALL	_io_sti
	PUSH	LC7
	PUSH	7
	PUSH	64
	JMP	L26
L31:
	PUSH	_mousefifo
	CALL	_fifo8_get
	MOV	EBX,EAX
	CALL	_io_sti
	MOVZX	EAX,BL
	PUSH	EAX
	LEA	EAX,DWORD [-588+EBP]
	PUSH	EAX
	CALL	_mouse_decode
	ADD	ESP,12
	TEST	EAX,EAX
	JE	L2
	PUSH	DWORD [-580+EBP]
	PUSH	DWORD [-584+EBP]
	PUSH	LC5
	LEA	EDX,DWORD [-476+EBP]
	PUSH	EDX
	CALL	_sprintf
	ADD	ESP,16
	MOV	EAX,DWORD [-576+EBP]
	TEST	EAX,1
	JE	L11
	MOV	BYTE [-475+EBP],76
L11:
	TEST	EAX,2
	JE	L12
	MOV	BYTE [-473+EBP],82
L12:
	AND	EAX,4
	JE	L13
	MOV	BYTE [-474+EBP],67
L13:
	PUSH	31
	LEA	EBX,DWORD [-476+EBP]
	PUSH	151
	PUSH	16
	PUSH	32
	PUSH	14
	MOVSX	EAX,WORD [4084]
	PUSH	EAX
	PUSH	DWORD [-632+EBP]
	CALL	_boxfill8
	PUSH	EBX
	PUSH	7
	PUSH	16
	PUSH	32
	MOVSX	EAX,WORD [4084]
	PUSH	EAX
	PUSH	DWORD [-632+EBP]
	CALL	_putfonts8_asc
	ADD	ESP,52
	PUSH	32
	PUSH	152
	PUSH	16
	PUSH	32
	PUSH	DWORD [-620+EBP]
	CALL	_sheet_refresh
	MOV	EDX,DWORD [-584+EBP]
	MOV	EAX,DWORD [-580+EBP]
	ADD	ESP,20
	ADD	DWORD [-652+EBP],EAX
	ADD	DWORD [-648+EBP],EDX
	JS	L34
L14:
	CMP	DWORD [-652+EBP],0
	JS	L35
L15:
	MOVSX	EAX,WORD [4084]
	DEC	EAX
	CMP	DWORD [-648+EBP],EAX
	JLE	L16
	MOV	DWORD [-648+EBP],EAX
L16:
	MOVSX	EAX,WORD [4086]
	DEC	EAX
	CMP	DWORD [-652+EBP],EAX
	JLE	L17
	MOV	DWORD [-652+EBP],EAX
L17:
	PUSH	DWORD [-652+EBP]
	PUSH	DWORD [-648+EBP]
	PUSH	LC6
	LEA	EBX,DWORD [-476+EBP]
	PUSH	EBX
	CALL	_sprintf
	PUSH	15
	PUSH	79
	PUSH	0
	PUSH	0
	PUSH	14
	MOVSX	EAX,WORD [4084]
	PUSH	EAX
	PUSH	DWORD [-632+EBP]
	CALL	_boxfill8
	ADD	ESP,44
	PUSH	EBX
	PUSH	7
	PUSH	0
	PUSH	0
	MOVSX	EAX,WORD [4084]
	PUSH	EAX
	PUSH	DWORD [-632+EBP]
	CALL	_putfonts8_asc
	PUSH	16
	PUSH	80
	PUSH	0
	PUSH	0
	PUSH	DWORD [-620+EBP]
	CALL	_sheet_refresh
	ADD	ESP,44
	PUSH	DWORD [-652+EBP]
	PUSH	DWORD [-648+EBP]
	PUSH	DWORD [-624+EBP]
	CALL	_sheet_move
	ADD	ESP,12
	JMP	L2
L35:
	MOV	DWORD [-652+EBP],0
	JMP	L15
L34:
	MOV	DWORD [-648+EBP],0
	JMP	L14
L30:
	PUSH	_keyfifo
	CALL	_fifo8_get
	MOV	EBX,EAX
	CALL	_io_sti
	PUSH	EBX
	LEA	EBX,DWORD [-476+EBP]
	PUSH	LC4
	PUSH	EBX
	CALL	_sprintf
	PUSH	31
	PUSH	15
	PUSH	16
	PUSH	0
	PUSH	14
	MOVSX	EAX,WORD [4084]
	PUSH	EAX
	PUSH	DWORD [-632+EBP]
	CALL	_boxfill8
	ADD	ESP,44
	PUSH	EBX
	PUSH	7
	PUSH	16
	PUSH	0
	MOVSX	EAX,WORD [4084]
	PUSH	EAX
	PUSH	DWORD [-632+EBP]
	CALL	_putfonts8_asc
	PUSH	32
	PUSH	16
	PUSH	16
	JMP	L27
L29:
	CALL	_io_sti
	JMP	L2
