*
* Programa para calcular la DFT
* utilizando el algoritmo de Goertzel
* con entrada de datos en formato de
* punto fijo a 32 bits
*
		.global _goertzel

		.global _spXk, _spYk		; vectores

		.data

WDCR 	.set 	07029h 		; Direccion del reg. de control WatchDog
N 		.set 	1000 		; Longitud de la secuencia de entrada

_spXk		.space	N*32
_spYk		.space	N*32

Nflo 	.set 	1000.0 		; Frecuencia de muestreo (no se qpd pero abr N flotante)
;xn		.space  N*16		; Secuencia original en Q12 a 16 bits
xnf		.space 	N*32 		; Espacio reservado para la secuencia de entrada a la transformada DFT (xn ya en float 32)
Qi 		.set 	4096.0 		; Valor para hacer la conversion a formato de punto fijo de la senal generada
							; qi = 2^(bits para parte fraccionaria)
							; este valor es para Qi=12 -> 2^12=4096
arg 	.float 	0.0 		; Localidad de memoria auxiliar
cos_arg	.float 	0.0 		; Localidad de memoria auxiliar
;spXk 	.space 	N*32 		; Espacio reservado para guardar el espectro
k 		.float 	0.0 		; Dominio discreto de la frecuencia
kint 	.set 	500 		; contador de control ciclo k (kint = N/2)

_goertzel:
* Deshabilitacion del WatchDog
		EALLOW 						; Habilita la escritura de registros protegidos
		MOVL 		XAR1, 	#WDCR 	; Registro XAR1 apunta dir WDCR
		MOV 		*XAR1, 	#0068h 	; Desactiva WatchDog
		EDIS 						; Deshabilita escritura a registros protegidos
;		SETC    	SXM             ; extensión de signo habilitada


* Conversion de la secuencia de datos en formato de punto fijo a punto flotante IEE754
		MOVL 		XAR0,	XAR4 	; Apuntador hacia la direccion inicial de la secuencia xn, pasa como argumento en XAR4 (antes #xn)
		MOVL		XAR1,	#xnf	; Apuntador hacia la direccion inicial de la secuencia xn en float 32
		MOVF32 		R0H,	#Qi 	; Coloca el dato de conversion a formato entero
		RPTB 		INT2FL,	#N-1 	; Ciclo de conversion de numeros enteros a flotantes
		MOV     	AL, 	*XAR0++ ; AL = xn(i) en Q12 (16 bits)
		MOV32     	R1H, 	ACC		; R1H = Q12 sign-extend a 32 bits
		NOP							; "Four alignment cycles are required after any copy from
		NOP							; a standard 28x CPU register to a floating-point register."
		NOP
		NOP
		I16TOF32 	R1H, 	R1H 	; Conversion de entero a flotante del dato xn(i)
		NOP
		NOP
		DIVF32 	R2H, 	R1H,	R0H ; Conversion de formato R2H = xn(i)/Qi
		NOP
		NOP
		NOP
		NOP
		MOV32 		*XAR1++,R2H 	; xn(i) = float IEE754 (xn(i))
INT2FL

* Calculo de la DFT por algoritmo de Goertzel
* Definicion de condiciones iniciales
		MOVF32 		R0H,	#Nflo 	; R0H = Nflo
		MOVL 		XAR0,	#xnf 	; Apuntador dirigido a la 1er loc de xnf(XAR0 BLOCK)
		MOVL 		XAR1,	XAR5 	; Apuntador dirigido a la 1er loc de spXk, pasa como argumento en XAR5 (XAR1 BLOCK) (antes #spXk)
		EINVF32		R1H, 	R0H 	; Obtiene 1/N (R1H BLOCK)
		MOV 		DP,		#k 		; Direccionamiento de pagina
		MOV 		AR2,	#kint-2 ; AR2 = Numero de repeticiones a realizar el (AR2 BLOCK)
									; ciclo k

* Inicio del ciclo de evaluacion del termino k
K_CYCLE
		MOVL 		XAR3,	#k 		; XAR3 = &k
		MOV32	 	R0H, 	*XAR3 	; R0H = k
		MPYF32 	R2H, R1H, 	R0H 	; Multiplicacion R2H = k * (1/N)
		MOVF32 		R4H,	#0.0 	; R4H = vk(n−1) (R4H BLOCK)
		MOVL 		XAR3,	#arg 	; XAR3 = &arg
		COSPUF32	R0H, 	R2H 	; R0H = cos (2pi ∗ [R2H = k/N])
		MOVF32 		R3H,	#2.0 	; R3H = 2.0
		MOV32 		*XAR3, 	R2H 	; arg = R2H = k/N
		MOVL 		XAR3,	#cos_arg; XAR3 = &cosarg
		MPYF32 	R2H, R3H,	R0H 	; R2H = 2.0 ∗ cos (2 p i ∗k/N)
		MOVF32 		R5H,	#0.0 	; R5H = vk ( n−2) (R5H BLOCK)
		MOV32 		*XAR3,	 R0H 	; cosarg = R0H = cos (2pi * [R2H = k/N])

; Ciclo para hacer el barrrido de n (1ra fase del algoritmo)
		RPTB 		N_CYCLE,#N-1
		MPYF32 	R0H, R2H, 	R4H 	; R0H = 2.0 * cos(2*pi*k/N) * vk ( n−1)
		MOV32 		R3H, 	*XAR0++ ; R3H = x(n)
		NOP
		ADDF32 	R6H, R3H, 	R0H 	; R6H = x(n) + 2.0 ∗ cos(2*pi*k/N) *vk (n−1)
		NOP
		NOP
		SUBF32 	R0H, R6H, 	R5H 	; R0H = [x(n) + 2.0 * cos(2*pi*k/N) *vk (n−1)] - vk(n−2)
		NOP
		NOP
		MOV32 		R5H, 	R4H		; R5H = R4H −> vk (n-2) = vk(n-1)
		NOP
		MOV32 		R4H, 	R0H 	; R4H = R0H −> vk (n-1) = vk(n)
		NOP
N_CYCLE

; Calculo de la parte imaginaria y real del termino k-esimo (2da fase del algoritmo)
		MOV32 		R6H, 	*XAR3 	; R6H = cos(2*pi* [R2H = k/N])
		MPYF32	R6H, R5H, 	R6H 	; R6H = vk(n-2)*cos(2*pi* [R2H=k/N])
		MOVL 		XAR3,	#arg 	; XAR3 = &arg
		MOV32 		R0H, 	*XAR3 	; R0H = arg = k/N
		SUBF32	R6H, R4H, 	R6H 	; Xr(k) = R6H = vk(n-1) - vk (n-2) * cos(2*pi*k/N)
		SINPUF32	R0H, 	R0H 	; R0H = sin(2*pi*k/N)
		MOVL 		XAR3,	#k 		; XAR3 = &k
		MOV32 		R7H, 	*XAR3 	; R7H = k
		ADDF32	R7H, R7H,	#1.00 	; R7H = R7H + 1
		NOP
		MPYF32 	R5H, R5H, 	R0H 	; Xi(k) = R5H = vk(n-2)*sin(2*pi*k/N)
		MOV32 		*XAR3, 	R7H 	; XAR3 => k = k + 1
		NOP

; Calculo del espectro del termino k-esimo
		MPYF32	R6H, R6H, 	R6H 	; R6H = [Xr(k)]^2
		NOP
		NOP
		MPYF32 	R5H, R5H, 	R5H 	; R5H = [Xi(k)]^2
		NOP
		NOP
		ADDF32 	R7H, R6H, 	R5H 	; R7H = [Xr(k)]^2 + [Xi(k)]^2
		NOP
		NOP
		SQRTF32		R6H, 	R7H 	; R6H = sqrt([Xr(k)]^2 + [Xi(k)]^2 )
		NOP
		NOP
		NOP
		MOVL 		XAR0,	#xnf
		MOV32 		*XAR1++,R6H 			; spX(k) = R6H

		BANZ 	K_CYCLE,	AR2--

; Reinicia condiciones iniciales
		MOVL 		XAR3,	#arg 	; XAR3 = &arg
		MOVF32		R0H,	#0.0	; R0H = 0.0
		NOP
		NOP
		MOV32		*XAR3,	R0H		; arg = 0.0

		MOVL 		XAR3,	#cos_arg; XAR3 = &arg
		MOVF32		R0H,	#0.0	; R0H = 0.0
		NOP
		NOP
		MOV32		*XAR3,	R0H		; arg = 0.0


		MOVL 		XAR3,	#k 		; XAR3 = &arg
		MOVF32		R0H,	#0.0	; R0H = 0.0
		NOP
		NOP
		MOV32		*XAR3,	R0H		; arg = 0.0

		LRETR
