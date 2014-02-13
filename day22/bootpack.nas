[FORMAT "WCOFF"]
[INSTRSET "i486p"]
[OPTIMIZE 1]
[OPTION 1]
[BITS 32]
	EXTERN	_fifo_init
	EXTERN	_init_gdtidt
	EXTERN	_init_pic
	EXTERN	_io_sti
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
	EXTERN	_make_textbox8
	EXTERN	_task_init
	EXTERN	_taskrun
	EXTERN	_make_shell
	EXTERN	_task_alloc
	EXTERN	_task_console
	EXTERN	_sheet_move
	EXTERN	_sheet_updown
	EXTERN	_io_cli
	EXTERN	_fifo_status
	EXTERN	_fifo_get
	EXTERN	_boxfill8
	EXTERN	_sheet_refresh
	EXTERN	_mouse_decode
	EXTERN	_keytable0.0
	EXTERN	_bps
	EXTERN	_cons_putstr0
	EXTERN	_asm_end_app
	EXTERN	_make_wtitle8
	EXTERN	_shell_title
	EXTERN	_fifo_put
	EXTERN	_keytable1.1
	EXTERN	_task_sleep
[FILE "bootpack.c"]
[SECTION .data]
_keytable0.0:
	DB	0
	DB	0
	DB	49
	DB	50
	DB	51
	DB	52
	DB	53
	DB	54
	DB	55
	DB	56
	DB	57
	DB	48
	DB	45
	DB	94
	DB	0
	DB	0
	DB	81
	DB	87
	DB	69
	DB	82
	DB	84
	DB	89
	DB	85
	DB	73
	DB	79
	DB	80
	DB	64
	DB	91
	DB	0
	DB	0
	DB	65
	DB	83
	DB	68
	DB	70
	DB	71
	DB	72
	DB	74
	DB	75
	DB	76
	DB	59
	DB	58
	DB	0
	DB	0
	DB	93
	DB	90
	DB	88
	DB	67
	DB	86
	DB	66
	DB	78
	DB	77
	DB	44
	DB	46
	DB	47
	DB	0
	DB	42
	DB	0
	DB	32
	DB	0
	DB	0
	DB	0
	DB	0
	DB	0
	DB	0
	DB	0
	DB	0
	DB	0
	DB	0
	DB	0
	DB	0
	DB	0
	DB	55
	DB	56
	DB	57
	DB	45
	DB	52
	DB	53
	DB	54
	DB	43
	DB	49
	DB	50
	DB	51
	DB	48
	DB	46
	DB	0
	DB	0
	DB	0
	DB	0
	DB	0
	DB	0
	DB	0
	DB	0
	DB	0
	DB	0
	DB	0
	DB	0
	DB	0
	DB	0
	DB	0
	DB	0
	DB	0
	DB	0
	DB	0
	DB	0
	DB	0
	DB	0
	DB	0
	DB	0
	DB	0
	DB	0
	DB	0
	DB	0
	DB	0
	DB	0
	DB	0
	DB	92
	DB	0
	DB	0
	DB	0
	DB	0
	DB	0
	DB	0
	DB	0
	DB	0
	DB	0
	DB	92
	DB	0
	DB	0
_keytable1.1:
	DB	0
	DB	0
	DB	33
	DB	34
	DB	35
	DB	36
	DB	37
	DB	38
	DB	39
	DB	40
	DB	41
	DB	126
	DB	61
	DB	126
	DB	0
	DB	0
	DB	81
	DB	87
	DB	69
	DB	82
	DB	84
	DB	89
	DB	85
	DB	73
	DB	79
	DB	80
	DB	96
	DB	123
	DB	0
	DB	0
	DB	65
	DB	83
	DB	68
	DB	70
	DB	71
	DB	72
	DB	74
	DB	75
	DB	76
	DB	43
	DB	42
	DB	0
	DB	0
	DB	125
	DB	90
	DB	88
	DB	67
	DB	86
	DB	66
	DB	78
	DB	77
	DB	60
	DB	62
	DB	63
	DB	0
	DB	42
	DB	0
	DB	32
	DB	0
	DB	0
	DB	0
	DB	0
	DB	0
	DB	0
	DB	0
	DB	0
	DB	0
	DB	0
	DB	0
	DB	0
	DB	0
	DB	55
	DB	56
	DB	57
	DB	45
	DB	52
	DB	53
	DB	54
	DB	43
	DB	49
	DB	50
	DB	51
	DB	48
	DB	46
	DB	0
	DB	0
	DB	0
	DB	0
	DB	0
	DB	0
	DB	0
	DB	0
	DB	0
	DB	0
	DB	0
	DB	0
	DB	0
	DB	0
	DB	0
	DB	0
	DB	0
	DB	0
	DB	0
	DB	0
	DB	0
	DB	0
	DB	0
	DB	0
	DB	0
	DB	0
	DB	0
	DB	0
	DB	0
	DB	0
	DB	0
	DB	95
	DB	0
	DB	0
	DB	0
	DB	0
	DB	0
	DB	0
	DB	0
	DB	0
	DB	0
	DB	124
	DB	0
	DB	0
LC0:
	DB	"task a",0x00
LC1:
	DB	"console",0x00
LC3:
	DB	0x0A," break key : ",0x0A,0x00
LC2:
	DB	" ",0x00
[SECTION .text]
	ALIGN	2
	GLOBAL	_HariMain
_HariMain:
	PUSH	EBP
	MOV	EBP,ESP
	PUSH	EDI
	PUSH	ESI
	PUSH	EBX
	LEA	EDI,DWORD [-860+EBP]
	SUB	ESP,944
	LEA	EBX,DWORD [-268+EBP]
	MOV	EAX,DWORD [-892+EBP]
	MOV	DWORD [4068],EAX
	LEA	EAX,DWORD [-780+EBP]
	PUSH	0
	PUSH	EAX
	PUSH	128
	PUSH	EDI
	CALL	_fifo_init
	CALL	_init_gdtidt
	CALL	_init_pic
	CALL	_io_sti
	CALL	_init_pit
	PUSH	248
	PUSH	33
	CALL	_io_out8
	PUSH	239
	PUSH	161
	CALL	_io_out8
	ADD	ESP,32
	CALL	_timer_alloc
	PUSH	1
	PUSH	EDI
	MOV	DWORD [-916+EBP],EAX
	PUSH	50
	PUSH	EAX
	CALL	_timer_set
	PUSH	256
	PUSH	EDI
	CALL	_init_keyboard
	PUSH	-1073741825
	PUSH	4194304
	CALL	_memtest
	ADD	ESP,32
	PUSH	3932160
	MOV	DWORD [-888+EBP],EAX
	CALL	_memman_init
	PUSH	647168
	PUSH	4096
	PUSH	3932160
	CALL	_memman_free
	MOV	EAX,DWORD [-888+EBP]
	SUB	EAX,4194304
	PUSH	EAX
	PUSH	4194304
	PUSH	3932160
	CALL	_memman_free
	CALL	_init_palette
	MOVSX	EAX,WORD [4086]
	MOVSX	EDX,WORD [4084]
	PUSH	EAX
	MOV	DWORD [-884+EBP],EAX
	MOVSX	EAX,WORD [4084]
	PUSH	EAX
	MOV	DWORD [-880+EBP],EDX
	PUSH	DWORD [4088]
	PUSH	3932160
	CALL	_shtctl_init
	ADD	ESP,44
	PUSH	EAX
	MOV	DWORD [-892+EBP],EAX
	CALL	_sheet_alloc
	PUSH	DWORD [-892+EBP]
	MOV	DWORD [-896+EBP],EAX
	CALL	_sheet_alloc
	PUSH	DWORD [-892+EBP]
	MOV	DWORD [-900+EBP],EAX
	CALL	_sheet_alloc
	MOVSX	EDX,WORD [4086]
	MOV	DWORD [-904+EBP],EAX
	MOVSX	EAX,WORD [4084]
	IMUL	EAX,EDX
	PUSH	EAX
	PUSH	3932160
	CALL	_memman_alloc_4k
	PUSH	8320
	PUSH	3932160
	MOV	ESI,EAX
	CALL	_memman_alloc_4k
	PUSH	-1
	MOV	DWORD [-912+EBP],EAX
	MOVSX	EAX,WORD [4086]
	PUSH	EAX
	MOVSX	EAX,WORD [4084]
	PUSH	EAX
	PUSH	ESI
	PUSH	DWORD [-896+EBP]
	CALL	_sheet_setbuf
	ADD	ESP,48
	PUSH	99
	PUSH	16
	PUSH	16
	PUSH	EBX
	PUSH	DWORD [-900+EBP]
	CALL	_sheet_setbuf
	PUSH	-1
	PUSH	52
	PUSH	144
	PUSH	DWORD [-912+EBP]
	PUSH	DWORD [-904+EBP]
	CALL	_sheet_setbuf
	ADD	ESP,40
	PUSH	DWORD [-884+EBP]
	PUSH	DWORD [-880+EBP]
	PUSH	ESI
	CALL	_init_screen
	LEA	EAX,DWORD [-876+EBP]
	PUSH	EAX
	PUSH	512
	PUSH	EDI
	CALL	_enable_mouse
	PUSH	99
	PUSH	EBX
	MOV	EBX,2
	CALL	_init_mouse_cursor8
	ADD	ESP,32
	PUSH	1
	PUSH	LC0
	PUSH	52
	PUSH	144
	PUSH	DWORD [-912+EBP]
	CALL	_make_window8
	PUSH	7
	PUSH	16
	PUSH	128
	PUSH	28
	PUSH	8
	PUSH	DWORD [-904+EBP]
	CALL	_make_textbox8
	ADD	ESP,44
	MOVSX	EAX,WORD [4084]
	MOV	DWORD [-928+EBP],8
	LEA	ECX,DWORD [-16+EAX]
	MOV	EAX,ECX
	CDQ
	IDIV	EBX
	MOV	DWORD [-920+EBP],EAX
	MOVSX	EAX,WORD [4086]
	PUSH	3932160
	LEA	ECX,DWORD [-44+EAX]
	MOV	EAX,ECX
	CDQ
	IDIV	EBX
	MOV	DWORD [-932+EBP],7
	MOV	DWORD [-924+EBP],EAX
	CALL	_task_init
	PUSH	0
	PUSH	1
	MOV	DWORD [-940+EBP],EAX
	PUSH	EAX
	MOV	DWORD [-836+EBP],EAX
	CALL	_taskrun
	PUSH	DWORD [-892+EBP]
	CALL	_sheet_alloc
	PUSH	42240
	PUSH	3932160
	MOV	DWORD [-908+EBP],EAX
	CALL	_memman_alloc_4k
	PUSH	-1
	PUSH	165
	MOV	DWORD [-944+EBP],EAX
	PUSH	256
	PUSH	EAX
	PUSH	DWORD [-908+EBP]
	CALL	_sheet_setbuf
	ADD	ESP,48
	PUSH	0
	PUSH	LC1
	PUSH	165
	PUSH	256
	PUSH	DWORD [-944+EBP]
	CALL	_make_shell
	CALL	_task_alloc
	PUSH	65536
	PUSH	3932160
	MOV	ESI,EAX
	CALL	_memman_alloc_4k
	MOV	EDX,DWORD [-908+EBP]
	ADD	EAX,65528
	MOV	DWORD [72+ESI],EAX
	MOV	DWORD [48+ESI],_task_console
	MOV	DWORD [88+ESI],8
	MOV	DWORD [92+ESI],16
	MOV	DWORD [96+ESI],8
	MOV	DWORD [100+ESI],8
	MOV	DWORD [104+ESI],8
	MOV	DWORD [108+ESI],8
	MOV	DWORD [4+EAX],EDX
	MOV	EDX,DWORD [-888+EBP]
	MOV	EAX,DWORD [72+ESI]
	MOV	DWORD [8+EAX],EDX
	PUSH	2
	PUSH	2
	PUSH	ESI
	CALL	_taskrun
	ADD	ESP,40
	PUSH	0
	PUSH	0
	PUSH	DWORD [-896+EBP]
	CALL	_sheet_move
	PUSH	DWORD [-924+EBP]
	PUSH	DWORD [-920+EBP]
	PUSH	DWORD [-900+EBP]
	CALL	_sheet_move
	PUSH	56
	PUSH	8
	PUSH	DWORD [-904+EBP]
	CALL	_sheet_move
	ADD	ESP,36
	PUSH	200
	PUSH	100
	PUSH	DWORD [-908+EBP]
	CALL	_sheet_move
	PUSH	0
	PUSH	DWORD [-896+EBP]
	CALL	_sheet_updown
	PUSH	1
	PUSH	DWORD [-908+EBP]
	CALL	_sheet_updown
	PUSH	2
	PUSH	DWORD [-904+EBP]
	CALL	_sheet_updown
	ADD	ESP,36
	PUSH	4
	PUSH	DWORD [-900+EBP]
	CALL	_sheet_updown
	POP	EDI
	MOV	DWORD [-948+EBP],0
	MOV	DWORD [-952+EBP],0
	MOV	AL,BYTE [4081]
	SAR	AL,4
	MOV	EDX,EAX
	POP	EAX
	AND	EDX,7
	MOV	DWORD [-956+EBP],EDX
L2:
	LEA	EDI,DWORD [-860+EBP]
	CALL	_io_cli
	PUSH	EDI
	CALL	_fifo_status
	POP	EBX
	TEST	EAX,EAX
	JE	L54
	PUSH	EDI
	CALL	_fifo_get
	MOV	EBX,EAX
	CALL	_io_sti
	POP	EDX
	LEA	EAX,DWORD [-256+EBX]
	CMP	EAX,255
	JBE	L55
	LEA	EAX,DWORD [-512+EBX]
	CMP	EAX,255
	JBE	L56
	CMP	EBX,1
	JE	L57
	TEST	EBX,EBX
	JNE	L2
	PUSH	1
	PUSH	EDI
	PUSH	50
	PUSH	DWORD [-916+EBP]
	CALL	_timer_set
	ADD	ESP,16
	CMP	DWORD [-932+EBP],0
	JS	L2
	MOV	EAX,DWORD [-928+EBP]
	PUSH	43
	ADD	EAX,7
	MOV	EDX,DWORD [-904+EBP]
	PUSH	EAX
	PUSH	28
	PUSH	DWORD [-928+EBP]
	PUSH	7
	PUSH	DWORD [4+EDX]
	PUSH	DWORD [EDX]
L52:
	CALL	_boxfill8
	MOV	EAX,DWORD [-928+EBP]
	PUSH	44
	ADD	EAX,8
	PUSH	EAX
	PUSH	28
	PUSH	DWORD [-928+EBP]
	PUSH	DWORD [-904+EBP]
	CALL	_sheet_refresh
	ADD	ESP,48
	JMP	L2
L57:
	PUSH	0
	PUSH	EDI
	PUSH	50
	PUSH	DWORD [-916+EBP]
	CALL	_timer_set
	ADD	ESP,16
	CMP	DWORD [-932+EBP],0
	JS	L2
	MOV	EAX,DWORD [-928+EBP]
	PUSH	43
	ADD	EAX,7
	PUSH	EAX
	MOV	EAX,DWORD [-904+EBP]
	PUSH	28
	PUSH	DWORD [-928+EBP]
	PUSH	0
	PUSH	DWORD [4+EAX]
	PUSH	DWORD [EAX]
	JMP	L52
L56:
	MOVZX	EAX,BL
	PUSH	EAX
	LEA	EAX,DWORD [-876+EBP]
	PUSH	EAX
	CALL	_mouse_decode
	POP	EDX
	POP	ECX
	TEST	EAX,EAX
	JE	L2
	MOV	EAX,DWORD [-868+EBP]
	MOV	EDX,DWORD [-872+EBP]
	ADD	DWORD [-924+EBP],EAX
	ADD	DWORD [-920+EBP],EDX
	JS	L58
L41:
	CMP	DWORD [-924+EBP],0
	JS	L59
L42:
	MOVSX	EAX,WORD [4084]
	DEC	EAX
	CMP	DWORD [-920+EBP],EAX
	JLE	L43
	MOV	DWORD [-920+EBP],EAX
L43:
	MOVSX	EAX,WORD [4086]
	DEC	EAX
	CMP	DWORD [-924+EBP],EAX
	JLE	L44
	MOV	DWORD [-924+EBP],EAX
L44:
	PUSH	DWORD [-924+EBP]
	PUSH	DWORD [-920+EBP]
	PUSH	DWORD [-900+EBP]
	CALL	_sheet_move
	ADD	ESP,12
	TEST	DWORD [-864+EBP],1
	JE	L2
	MOV	EAX,DWORD [-924+EBP]
	SUB	EAX,8
	PUSH	EAX
	MOV	EAX,DWORD [-920+EBP]
	SUB	EAX,80
	PUSH	EAX
	PUSH	DWORD [-904+EBP]
	CALL	_sheet_move
	ADD	ESP,12
	JMP	L2
L59:
	MOV	DWORD [-924+EBP],0
	JMP	L42
L58:
	MOV	DWORD [-920+EBP],0
	JMP	L41
L55:
	CMP	EBX,383
	JG	L8
	CMP	DWORD [-952+EBP],0
	JNE	L9
	MOV	AL,BYTE [_keytable0.0-256+EBX]
L53:
	MOV	BYTE [-828+EBP],AL
L11:
	MOV	DL,BYTE [-828+EBP]
	LEA	EAX,DWORD [-65+EDX]
	CMP	AL,25
	JA	L12
	MOV	EAX,DWORD [-956+EBP]
	AND	EAX,4
	JNE	L15
	CMP	DWORD [-952+EBP],0
	JE	L14
L15:
	DEC	EAX
	JE	L60
L12:
	MOV	AL,BYTE [-828+EBP]
	TEST	AL,AL
	JE	L16
	CMP	DWORD [-948+EBP],0
	JNE	L17
	CMP	DWORD [-928+EBP],127
	JG	L16
	PUSH	1
	LEA	EAX,DWORD [-828+EBP]
	PUSH	EAX
	PUSH	0
	PUSH	7
	PUSH	28
	PUSH	DWORD [-928+EBP]
	PUSH	DWORD [-904+EBP]
	MOV	BYTE [-827+EBP],0
	CALL	_bps
	ADD	ESP,28
	ADD	DWORD [-928+EBP],8
L16:
	CMP	EBX,270
	JE	L61
L20:
	CMP	EBX,284
	JE	L62
L24:
	CMP	EBX,271
	JE	L63
L26:
	CMP	EBX,298
	JE	L64
L29:
	CMP	EBX,310
	JE	L65
L30:
	CMP	EBX,426
	JE	L66
L31:
	CMP	EBX,438
	JE	L67
L32:
	CMP	EBX,314
	JE	L68
L33:
	CMP	EBX,325
	JE	L69
L34:
	CMP	EBX,326
	JE	L70
L35:
	CMP	EBX,315
	JE	L71
L36:
	CMP	DWORD [-932+EBP],0
	JS	L37
	MOV	EAX,DWORD [-928+EBP]
	PUSH	43
	ADD	EAX,7
	MOV	EDX,DWORD [-904+EBP]
	PUSH	EAX
	PUSH	28
	PUSH	DWORD [-928+EBP]
	MOVZX	EAX,BYTE [-932+EBP]
	PUSH	EAX
	PUSH	DWORD [4+EDX]
	PUSH	DWORD [EDX]
	CALL	_boxfill8
	ADD	ESP,28
L37:
	MOV	EAX,DWORD [-928+EBP]
	PUSH	44
	ADD	EAX,8
	PUSH	EAX
	PUSH	28
	PUSH	DWORD [-928+EBP]
	PUSH	DWORD [-904+EBP]
	CALL	_sheet_refresh
	ADD	ESP,20
	JMP	L2
L71:
	CMP	DWORD [-952+EBP],0
	JE	L36
	CMP	DWORD [24+ESI],0
	JE	L36
	PUSH	LC3
	PUSH	DWORD [4076]
	CALL	_cons_putstr0
	CALL	_io_cli
	LEA	EAX,DWORD [20+ESI]
	MOV	DWORD [56+ESI],EAX
	MOV	DWORD [48+ESI],_asm_end_app
	CALL	_io_sti
	POP	EBX
	POP	EDI
	JMP	L36
L70:
	XOR	DWORD [-956+EBP],1
	JMP	L35
L69:
	XOR	DWORD [-956+EBP],2
	JMP	L34
L68:
	XOR	DWORD [-956+EBP],4
	JMP	L33
L67:
	AND	DWORD [-952+EBP],-3
	JMP	L32
L66:
	AND	DWORD [-952+EBP],-2
	JMP	L31
L65:
	OR	DWORD [-952+EBP],2
	JMP	L30
L64:
	OR	DWORD [-952+EBP],1
	JMP	L29
L63:
	CMP	DWORD [-948+EBP],0
	JNE	L27
	PUSH	0
	MOV	EAX,DWORD [-904+EBP]
	PUSH	LC0
	PUSH	DWORD [4+EAX]
	PUSH	DWORD [-912+EBP]
	MOV	DWORD [-948+EBP],1
	CALL	_make_wtitle8
	MOV	EDX,DWORD [-908+EBP]
	PUSH	1
	PUSH	LC1
	PUSH	DWORD [4+EDX]
	PUSH	DWORD [-944+EBP]
	CALL	_shell_title
	MOV	EAX,DWORD [-928+EBP]
	ADD	ESP,32
	ADD	EAX,7
	MOV	DWORD [-932+EBP],-1
	PUSH	43
	PUSH	EAX
	MOV	EAX,DWORD [-904+EBP]
	PUSH	28
	PUSH	DWORD [-928+EBP]
	PUSH	7
	PUSH	DWORD [4+EAX]
	PUSH	DWORD [EAX]
	CALL	_boxfill8
	LEA	EAX,DWORD [120+ESI]
	PUSH	2
	PUSH	EAX
	CALL	_fifo_put
	ADD	ESP,36
L28:
	MOV	EDX,DWORD [-904+EBP]
	PUSH	21
	PUSH	DWORD [4+EDX]
	PUSH	0
	PUSH	0
	PUSH	EDX
	CALL	_sheet_refresh
	MOV	EAX,DWORD [-908+EBP]
	PUSH	21
	PUSH	DWORD [4+EAX]
	PUSH	0
	PUSH	0
	PUSH	EAX
	CALL	_sheet_refresh
	ADD	ESP,40
	JMP	L26
L27:
	PUSH	1
	MOV	EDX,DWORD [-904+EBP]
	PUSH	LC0
	PUSH	DWORD [4+EDX]
	PUSH	DWORD [-912+EBP]
	MOV	DWORD [-948+EBP],0
	CALL	_make_wtitle8
	MOV	EAX,DWORD [-908+EBP]
	PUSH	0
	PUSH	LC1
	PUSH	DWORD [4+EAX]
	PUSH	DWORD [-944+EBP]
	CALL	_shell_title
	LEA	EAX,DWORD [120+ESI]
	ADD	ESP,32
	MOV	DWORD [-932+EBP],0
	PUSH	3
	PUSH	EAX
	CALL	_fifo_put
	POP	EAX
	POP	EDX
	JMP	L28
L62:
	CMP	DWORD [-948+EBP],0
	JE	L24
	PUSH	266
	LEA	EAX,DWORD [120+ESI]
	PUSH	EAX
	CALL	_fifo_put
	POP	ECX
	POP	EDI
	JMP	L24
L61:
	CMP	DWORD [-948+EBP],0
	JNE	L21
	CMP	DWORD [-928+EBP],8
	JLE	L20
	PUSH	1
	PUSH	LC2
	PUSH	0
	PUSH	7
	PUSH	28
	PUSH	DWORD [-928+EBP]
	PUSH	DWORD [-904+EBP]
	CALL	_bps
	ADD	ESP,28
	SUB	DWORD [-928+EBP],8
	JMP	L20
L21:
	PUSH	264
	LEA	EAX,DWORD [120+ESI]
	PUSH	EAX
	CALL	_fifo_put
	POP	EAX
	POP	EDX
	JMP	L20
L17:
	MOVSX	EAX,AL
	ADD	EAX,256
	PUSH	EAX
	LEA	EAX,DWORD [120+ESI]
	PUSH	EAX
	CALL	_fifo_put
	POP	ECX
	POP	EDI
	JMP	L16
L60:
	CMP	DWORD [-952+EBP],0
	JE	L12
L14:
	LEA	EAX,DWORD [32+EDX]
	MOV	BYTE [-828+EBP],AL
	JMP	L12
L9:
	MOV	AL,BYTE [_keytable1.1-256+EBX]
	JMP	L53
L8:
	MOV	BYTE [-828+EBP],0
	JMP	L11
L54:
	PUSH	DWORD [-940+EBP]
	CALL	_task_sleep
	CALL	_io_sti
	POP	ECX
	JMP	L2