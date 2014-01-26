; haribote-ipl
; TAB=4

		ORG		0x7c00			; ‚±‚ÌƒvƒƒOƒ‰ƒ€‚ª‚Ç‚±‚É“Ç‚Ýž‚Ü‚ê‚é‚Ì‚©


		JMP		entry
		DB		0x90
		DB		"HARIBOTE"		; ƒu[ƒgƒZƒNƒ^‚Ì–¼‘O‚ðŽ©—R‚É‘‚¢‚Ä‚æ‚¢i8ƒoƒCƒgj
		DW		512				; 1ƒZƒNƒ^‚Ì‘å‚«‚³i512‚É‚µ‚È‚¯‚ê‚Î‚¢‚¯‚È‚¢j
		DB		1				; ƒNƒ‰ƒXƒ^‚Ì‘å‚«‚³i1ƒZƒNƒ^‚É‚µ‚È‚¯‚ê‚Î‚¢‚¯‚È‚¢j
		DW		1				; FAT‚ª‚Ç‚±‚©‚çŽn‚Ü‚é‚©i•’Ê‚Í1ƒZƒNƒ^–Ú‚©‚ç‚É‚·‚éj
		DB		2				; FAT‚ÌŒÂ”i2‚É‚µ‚È‚¯‚ê‚Î‚¢‚¯‚È‚¢j
		DW		224				; ƒ‹[ƒgƒfƒBƒŒƒNƒgƒŠ—Ìˆæ‚Ì‘å‚«‚³i•’Ê‚Í224ƒGƒ“ƒgƒŠ‚É‚·‚éj
		DW		2880			; ‚±‚Ìƒhƒ‰ƒCƒu‚Ì‘å‚«‚³i2880ƒZƒNƒ^‚É‚µ‚È‚¯‚ê‚Î‚¢‚¯‚È‚¢j
		DB		0xf0			; ƒƒfƒBƒA‚Ìƒ^ƒCƒvi0xf0‚É‚µ‚È‚¯‚ê‚Î‚¢‚¯‚È‚¢j
		DW		9				; FAT—Ìˆæ‚Ì’·‚³i9ƒZƒNƒ^‚É‚µ‚È‚¯‚ê‚Î‚¢‚¯‚È‚¢j
		DW		18				; 1ƒgƒ‰ƒbƒN‚É‚¢‚­‚Â‚ÌƒZƒNƒ^‚ª‚ ‚é‚©i18‚É‚µ‚È‚¯‚ê‚Î‚¢‚¯‚È‚¢j
		DW		2				; ƒwƒbƒh‚Ì”i2‚É‚µ‚È‚¯‚ê‚Î‚¢‚¯‚È‚¢j
		DD		0				; ƒp[ƒeƒBƒVƒ‡ƒ“‚ðŽg‚Á‚Ä‚È‚¢‚Ì‚Å‚±‚±‚Í•K‚¸0
		DD		2880			; ‚±‚Ìƒhƒ‰ƒCƒu‘å‚«‚³‚ð‚à‚¤ˆê“x‘‚­
		DB		0,0,0x29		; ‚æ‚­‚í‚©‚ç‚È‚¢‚¯‚Ç‚±‚Ì’l‚É‚µ‚Ä‚¨‚­‚Æ‚¢‚¢‚ç‚µ‚¢
		DD		0xffffffff		; ‚½‚Ô‚ñƒ{ƒŠƒ…[ƒ€ƒVƒŠƒAƒ‹”Ô†
		DB		"HARIBOTEOS "	; ƒfƒBƒXƒN‚Ì–¼‘Oi11ƒoƒCƒgj
		DB		"FAT12   "		; ƒtƒH[ƒ}ƒbƒg‚Ì–¼‘Oi8ƒoƒCƒgj
		RESB	18				; ‚Æ‚è‚ ‚¦‚¸18ƒoƒCƒg‚ ‚¯‚Ä‚¨‚­


entry:
		MOV		AX,0
		MOV		SS,AX
		MOV		SP,0x7c00
		MOV		DS,AX
                ;ds = 0 , read a sector
		MOV		AX,0x0820
		MOV		ES,AX             ; make extra segment 0820
		MOV		CH,0			; ÖùÃæ0
		MOV		DH,0			; ´ÅÍ·0
		MOV		CL,2			; ÉÈÇø2

reader:
		MOV		AH,0x02			; AH=0x02 
		MOV		AL,1
		MOV		BX,0
		MOV		DL,0x00			; Aƒhƒ‰ƒCƒu
		INT		0x13			;call BIOS interrupt
		JNC		next
		JC		error

next:
	MOV AX,ES
	ADD AX,0x0020
	MOV ES,AX   ; es increase 0x20, 512B
	ADD CL,1
	CMP CL,18
	JBE reader

fin:
		HLT						; ‰½‚©‚ ‚é‚Ü‚ÅCPU‚ð’âŽ~‚³‚¹‚é
		JMP		fin				; –³ŒÀƒ‹[ƒv

error:
		MOV		SI,msg
putloop:
		MOV		AL,[SI]
		ADD		SI,1			; SI‚É1‚ð‘«‚·
		CMP		AL,0
		JE		fin
		MOV		AH,0x0e			; ˆê•¶Žš•\Ž¦ƒtƒ@ƒ“ƒNƒVƒ‡ƒ“
		MOV		BX,15			; ƒJƒ‰[ƒR[ƒh
		INT		0x10			; ƒrƒfƒIBIOSŒÄ‚Ño‚µ
		JMP		putloop
msg:
		DB		0x0a, 0x0a		; ‰üs‚ð2‚Â
		DB		"load error"
		DB		0x0a			; ‰üs
		DB		0

		RESB	0x7dfe-$		; 0x7dfe‚Ü‚Å‚ð0x00‚Å–„‚ß‚é–½—ß

		DB		0x55, 0xaa
