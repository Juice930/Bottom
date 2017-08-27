	.global main
	.text ;define codigo del programa y lo ubica en flash

SYSCTL_RCGCGPIO_R 	.field 0x400FE608,32 ; REGISTRO DEL RELOJ

GPIO_PORTN_DATA_F	.field 0x4005D044,32	;Datos del Puerto F
GPIO_PORTN_DIR_F	.field 0x4005D400,32	;Dirección del Puerto F
GPIO_PORTN_DEN_F 	.field 0x4005D51C,32	;Enable del Puerto F

GPIO_PORTN_DIR_N	.field 0x40064400,32 ; Registro de Dirección N
GPIO_PORTN_DEN_N 	.field 0x4006451C,32 ; Registro de habilitación N
GPIO_PORTN_DATA_N 	.field 0x4006400C,32 ; Registro de Datos N

GPIO_PORTN_DIR_J 	.field 0x40060400,32 ; Registro de Dirección J
GPIO_PORTN_PUR_J 	.field 0x40060510,32 ; Registro de pull-up J
GPIO_PORTN_DEN_J 	.field 0x4006051C,32 ; Registro de habilitación J
GPIO_PORTN_DATA_J 	.field 0x4006000C,32 ; Registro de Datos J
tie	.equ	0x0035	;De estos parámetros depende el tiempo de lectura de los botones
mpo	.equ	0x0A00

main
	movw R6,#mpo	;R6 lo usamos para los delays
	movt R6,#tie

	LDR R1, SYSCTL_RCGCGPIO_R 	;activamos el reloj de los puertos F,N y J
	LDR R0, [R1]
	ORR R0, R0, #0x1120
	STR R0, [R1]
	NOP
	NOP ; se da tiempo para que el reloj se habilite

	;Puerto F LEDs 3 y 4
	LDR R1,GPIO_PORTN_DIR_F ; Configura la dirección del registro
	MOV R0, #0x11 ; PN0 PN4 salidas
	STR R0,[R1]
	LDR R1,GPIO_PORTN_DEN_F ; Habilita al puerto digital F
	MOV R0, #0x11 ; 1 significa que habilita S
	STR R0,[R1]

	;Puerto N LEDS 1 y 2
	LDR R1,GPIO_PORTN_DIR_N ; Configura la dirección del registro
	MOV R0, #0X03 			; PN0 PN1 salidas
	STR R0,[R1]
	LDR R1,GPIO_PORTN_DEN_N ; Habilita al puerto digital N
	MOV R0, #0XFF 			; 1 significa que habilita E/S
	STR R0,[R1]

	;Puerto J Botones
	LDR R1,GPIO_PORTN_DIR_J ; Configura la dirección del registro J
	MOV R0, #0X00 			; PJ1 PJ2 entradas
	STR R0,[R1]
	LDR R1,GPIO_PORTN_PUR_J ; Configura los resistores pull_up J
	MOV R0, #0X03			; habilita weak pull up para PJ0 PJ1
	STR R0,[R1]
	LDR R1,GPIO_PORTN_DEN_J ; Habilita al puerto digital J
	MOV R0, #0XFF 			; 1 significa que habilita E/S
	STR R0,[R1]

	mov R9,#0xFFFF	;R9 contiene la contraseña para abrir

	ldr R0,GPIO_PORTN_DATA_N	;R1 es el apuntador de los datos del puerto N
	mov R2,#0
	str R2,[R0]					;Apagamos los LEDs que pudieran estar encendidos

	ldr R3,GPIO_PORTN_DATA_F	;R3 es esl apuntador de los datos del puerto F
	str R2,[R3]					;Apagamos los LEDs que pudieran estar encendidos

	LDR R1,GPIO_PORTN_DATA_J 	;Lee los botones (Los botones están en 1 por default)

loop
	bl delay		;Antes de leer damos un delay
	LDR R7,[R1]		;Guardamos los botones en R8
	lsl R7,R7,#4	;Recorremos el primer botón leído

	bl delay		;Un delay
	LDR R8,[R1]		;Guardamos el botón en R8
	orr R7,R7,R8	;Guardamos los botones en R7
	lsl R7,R7,#4	;Recorremos los 2 botones leídos

	bl delay		;Un delay
	LDR R8,[R1]		;Guardamos el botón en R8
	orr R7,R7,R8	;Guardamos los botones en R7
	lsl R7,R7,#4	;Recorremos los 3 botones leídos

	bl delay		;Un delay
	LDR R8,[R1]		;Guardamos el botón en R8
	orrs R7,R7,R8	;Guardamos los botones en R7, este último OR tiene activadas las banderas

	beq prog		;Si la última operación fue 0 entramos a modo de programación
	cmp R9,R7		;Si lo que hay en R7 es igual a lo que hay en R9 la contraseña es correcta
	beq abrir		;Iniciamos la secuencia de abrir

	mov R2,#0x1	;El LED 4 se enciende si la contraseña no fue ingresada
	str R2,[R3]
	bl delay3
	mov R2,#0	;Apagamos el LED 4
	str R2,[R3]


	b loop			;Si no volvemos a empezar a leer

abrir
	mov R2,#3		;Encendemos todos los LEDs
	str R2,[R0]
	mov R2,#0x11
	str R2,[R3]

	lsl R6,#4		;Quiero hacer un delay que dure 4 veces lo que duraba el otro
	bl delay3
	lsr R6,#4		;Regresamos el delay a como estaba

	mov R2,#0		;Apagamos todos los LEDs
	str R2,[R0]
	str R2,[R3]
	b loop			;Volvemos a leer la secuencia

prog				;Modo de Programación
	mov R2,#0x10	;El LED 3 se enciende cuando estamos en este modo
	str R2,[R3]

	bl delay		;Un delay
	LDR R9,[R1]		;Guardamos el botón apretado en R9
	lsl R9,R9,#4	;Recorremos el botón

	bl delay		;Un delay
	LDR R8,[R1]		;Guardamos el botón en R8
	orr R9,R9,R8	;Guardamos los botones en R9
	lsl R9,R9,#4	;Recorremos los botones

	bl delay		;Un delay
	LDR R8,[R1]		;Guardamos el botón en R8
	orr R9,R9,R8	;Guardamos los botones en R9
	lsl R9,R9,#4	;Recorremos los botones

	bl delay		;Un delay
	LDR R8,[R1]		;Guardamos el botón en R8
	orr R9,R9,R8	;Guardamos los botones en R9

	cmp R9,#0 		;La contraseña no puede ser el código de reset
	beq prog		;Volvemos a pedirla

	mov R2,#0		;Apagamos el LED del modo de programación
	str R2,[R3]

	b loop			;No sé por qué cuando quiero mandarlo a loop se traba el programa
					;Pero lo puedo mandar a que compare y también sirve

delay				;Delay con duración de 2 veces lo que haya en R6 y enciende 1 segundo al LED
	mov R2,#1		;Encendemos el LED2
	str R2,[R0]
delay2
	add R5,R5,#1	;R5 cuenta desde 0 hasta [R6]
	cmp	R5,R6
	bne delay2
	mov R5,#0		;Resetea R5

	mov R2,#0		;Apaga los LEDs
	str R2,[R0]
delay3
	add R5,R5,#1	;Vuelve a hacer un delay
	cmp	R5,R6
	bne delay3
	mov R5,#0		;Resetea R5
	bx LR			;Nos regresa a donde hayamos llamado a delay

	.end
