; ce programme est pour l'assembleur RealView (Keil)
	thumb
	area  moncode, code, readonly
	export son_callback
	extern etat
	import Type_Etat
		

son_callback	proc
	
;typedef struct {
;int position;		// 0
;int taille;		// 4
;void * son;		// 8
;int resolution;	// 12
;int periode_ticks;	// 16
;} type_etat;

;/*
E_POS	equ	0
E_TAI	equ	4
E_SON	equ	8
E_RES	equ	12
E_PER	equ	16
		end
;*/
	
GPIOB_BSRR	equ	0x40010C10	; Bit Set/Reset register
	ldr	r4,=etat
	ldr	r4,[r4]
; mise a 1 de PB1
	ldr	r3, =GPIOB_BSRR
	cmp 	r4, #0x00000002
	beq 	eq	
	bne	ne
return
	str	r4,[r3]
	ldr	r5,=etat
	str	r4,[r5]
	bx	lr
	endp
	
eq
	mov	r4,#0x00020000
	b	return

ne	mov	r4,#0x00000002
	b	return
	
	end
		
		