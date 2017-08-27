		.global main
valmin	 .equ	R0
lmemin   .equ	R1
cont	.equ	R2
		.data
		.text

arreglo	.word	-6,1,1,11,10,23,121,1,9,38
tamano	.word	10
main

		;obtener el tamaño del arreglo

		ADR		R3,tamano
		LDR		R3, [R3]
		SUB		R3,R3, #1

		;inicializa valor mínimo

		ADR		R4, arreglo
		LDR		valmin,[r4]
		MOV		lmemin,#0

        ;iteración sobre el arreglo

        MOV		cont,#0 ;inicializa contador
ciclo	cmp		cont,R3
		BGE		fin
		LDR		R5, [ R4, cont, LSL #2]
		CMP		valmin, R5
		blt		ciclo1
		mov		valmin,R5
		mov		lmemin,cont

ciclo1	add		cont,cont,#1
		B		ciclo

fin		B		fin

		.end
