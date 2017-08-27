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
mpo	.equ	0xFFAA	;De estos parámetros depende el tiempo de lectura de los botones
tie .equ	0x0005
main
	movw R6,#mpo
	movt R6,#tie	;R6 lo usamos para los delays

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
	mov R4,#0
	mov R7,#0
	mov R10,#0x2222				;Código para entrar al modo programador
loop
	ldr R8,[R1]
	bl delay3
	;mov R11,R8		;Guardamos R8 en R11
	cmp R8,#3		;Si se apretó algo R8 no será 3
	bne boton
	b loop			;Si no volvemos a empezar a leer

boton
	;ldr R8,[R1]		;Leemos el botón, para ver hasta cuándo lo sueltan
	;cmp R8,R11
	;beq boton		;Si son iguales R8 y R11 significa que no han soltado el botón
	orr R7,R7,R8
	add R4,R4,#1
	cmp R4,#4
	beq comp
	lsl R7,R7,#4
	b loop
boton2
	mov R9,LR
	;ldr R8,[R1]		;Leemos el botón, para ver hasta cuándo lo sueltan
	;cmp R8,R11
	;beq boton2
	bl delay3
	lsl R7,R7,#4
	orr R7,R7,R8
	add R4,R4,#1
	bx R9
comp
	mov R4,#0
	cmp R7,R10
	beq prog
	cmp R7,R9
	beq abrir
	mov R7,#0
	b loop

abrir
	mov R2,#3		;Encendemos todos los LEDs
	str R2,[R0]
	mov R2,#0x11
	str R2,[R3]

	mov R7,#0

	lsl R6,#4		;Quiero hacer un delay que dure 4 veces lo que duraba el otro
	bl delay3
	lsr R6,#4	;Regresamos el delay a como estaba

	mov R2,#0		;Apagamos todos los LEDs
	str R2,[R0]
	str R2,[R3]
	b loop		;Volvemos a leer la secuencia

prog				;Modo de Programación
	mov R2,#0x10	;El LED 3 se enciende cuando estamos en este modo
	str R2,[R3]
	mov R7,#0
prog2
	ldr R8,[R1]
	;mov R11,R8
	cmp R8,#3
	beq prog2
	bl boton2
	cmp R4,#4
	bne prog2
	mov R4,#0

	cmp R7,R10 		;La contraseña no puede ser el código de reset
	beq prog		;Volvemos a pedirla
	mov R9,R7
	mov R2,#0		;Apagamos el LED del modo de programación
	str R2,[R3]
	mov R7,#0
	b loop

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
	bx LR		;Nos regresa a donde hayamos llamado a delay

	.end
