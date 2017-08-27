	.global main
	; .data ; define variables globales

	.text ;define codigo del programa y lo ubica en flash

main
	movw R6,#0x0	;Dirección actual de la palabra de 32 bits
	movt R6,#0x2000

	movw R5,#0x0010	;Dirección final a la que queremos buscar, para revisar 20 bytes
	movt R5,#0x2000

	ldr R1,[R6]		;Cargamos a R1 los 32 bits que usaremos
	movw R2,#0x0	;R2 es una máscara para seleccionar un byte
	movt R2,#0xFF00
	and R4,R2,R1	;El byte más grande termina en R4, por eso
					;aquí cargamos en R4 el byte más grande hasta
					;el momento,(el primero)

buscar
	cmp R1,#0		;Si R1 es 0 significa que ya nos lo acabamos
	beq siguiente	;Tenemos que ir a siguiente para llenarlo de valores
	and R3,R2,R1	;R3 tiene el valor del byte actual
	lsl R1,#8		;Cada iteración va recorriendo a R1 hasta que se acaba
	cmp R4,R3		;R4<R3?
	bmi mayor		;Si R3 es mayor nos va a mandar a la subrutina para guardarlo

	b buscar		;Si no es mayor seguimos buscando

mayor
	bcs buscar		;Si C=0 la operación de resta no fue ejecutada correctamente
	mov R4,R3		;Actualizamos el valor más grande
	b buscar

siguiente
	cmp R5,R6		;Si las direcciones son iguales termina la ejecución
	beq fin			;Nos manda al final
	add R6,R6,#4	;Si no fueron iguales recorremos la dirección.
	ldr R1,[R6]		;Y cargamos a R1 los nuevos 32 bits
	b buscar		;Para volver a buscar
fin B	fin			;Loop infinito
	.end
