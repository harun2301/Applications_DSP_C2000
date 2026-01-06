*
* Ecuacion en diferencias del filtro
* y(n) = b0x(n) + b1x(n-1) + b2x(n-2) - a1y(n-1) - a2y(n-2)
*
*

		.global _filtroIIR

		.global _xn, _yn		; vectores

		.data

N		.set 	1000
_xn		.space	N*16
_yn		.space	N*16

Nb		.set	3 		;
Na 		.set 	2 		;
sumX 	.word 	0 		;
yIIR 	.word 	0 		;
memX 	.space 	Nb*16 	;
endMemX .word 	0 		;
memY 	.space 	Na*16 	;
endMemY .word 	0 		;


*==========================
*Filtrado de senial
*==========================
* AL = xn[i]
* XAR4 = &coef_b[0]
* XAR5 = &coef_a[0]
_filtroIIR:
		SETC 	SXM 			; Modo extension de signo
		SETC 	OVM 			;
		SPM 	0				; Corrimientos nulos en P

		MOV 	@sumX,	#0 		;
		MOVL 	XAR7,	#memX 	; XAR7 apunta a direccion de memX, buffer de x
		MOV 	*XAR7,	AL 		; lo que hay en AL lo pasa a donde apunta *XAR7,
								; es decir el primer parametro, o sea escribe x(n) al inicio del buffer
		MOVL 	XAR0,	#yIIR 	; XAR0 apunta a salida
		ZAPA

		MOV		T,		*XAR4++				; T = b0
		MPY		P,		T,		*XAR7++		; P = (T)(*XAR7++) = b0*xn0
		MOV		T,		*XAR4++				; T = b1
		MPYA	P,		T,		*XAR7++		; Acc = ACC + P = 0 + b0*xn0 ; P = (T)(*XAR7++) = b1*xn1
		MOV		T,		*XAR4++				; T = b2
		MPYA	P,		T,		*XAR7++		; Acc = ACC + P = b0*xn0 + b1*xn1 ; P = (T)(*XAR7++) = b2*xn2
		ADDL	ACC,	P					; ACC = = ACC + P = b0*xn0 + b1*xn1 + b2*xn2
*		RPT 	#Nb-1
*	||	MAC 	P,		*XAR4++,	*XAR7++ ; ACC=P, T=*XAR4,P=(*XAR4)(*XAR7)

		;ADDL 	ACC,	P<<PM				; Acumula último producto
		LSL 	ACC,	#4 					; mueve a la parte alta y trunca AH en formato Q12
		MOVW 	DP,		#sumX 				; mueve el apuntador de pagina
		MOV 	@sumX,	AH 					; guarda el resultado de la MAC

		RPT 	#Nb-1
	|| 	DMOV 	*--XAR7 					; Retardo en el tiempo

		MOVL 	ACC,	XAR5 				;
		ADD 	ACC,	#1 					; avanza el XAR5 para iniciar en a1
		MOVL 	XAR5,	ACC 				;

		MOVL 	XAR7,	#memY 				; apunta XAR7 al buffer de y
		ZAPA

*		RPT 	#Na-1
*	||	MAC 	P,		*XAR5++,	*XAR7++ ; ACC=P, T=*XAR5,P=(*XAR5)(*XAR7)

		MOV		T,		*XAR5++				; T = a1
		MPY		P,		T,		*XAR7++		; P = (T)(*XAR7++) = a1*yn1
		MOV		T,		*XAR5++				; T = a2
		MPYA	P,		T,		*XAR7++		; Acc = ACC + P = 0 + a1*yn1 ; P = (T)(*XAR7++) = a2*yn2
		ADDL	ACC,	P					; ACC = = ACC + P = a1*yn1 + a2*yn2

		;ADDL 	ACC,	P<<PM				; Acumula último producto
		LSL 	ACC,	#4 					; mueve a la parte alta y trunca AH en formato Q12
		ADD 	AH,		@sumX 				;

		MOV 	*XAR0,	AH 					;

		RPT 	#Na-1
	|| 	DMOV 	*--XAR7 					; Retardo en el tiempo

		MOV 	*XAR7,	AH 					; guarda salida en el buffer y
		MOVL 	ACC,	*XAR0 				; guardo en el acumulador mi salida
		SFR		ACC,	#16					; Como mi variable en C es de tipo int (16 bits) solo se regresa la parte baja AL

		LRETR

