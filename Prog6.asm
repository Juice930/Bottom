	.global main
	; .data ; define variables globales

	.text ;define codigo del programa y lo ubica en flash

SYSCTL_RCGCGPIO_R .field 0x400FE608,32 ; REGISTRO DEL RELOJ
GPIO_PORTN_DIR_N .field 0x40064400,32 ; Registro de Dirección N
GPIO_PORTN_DEN_N .field 0x4006451C,32 ; Registro de habilitación N
GPIO_PORTN_DATA_N .field 0x4006400C,32 ; Registro de Datos N
LEDS .EQU 0x03 ; se asigna el valor 0x20 a el simbolo BIT5
LEDS2 .EQU 0X00

main
	LDR R1,SYSCTL_RCGCGPIO_R ; 1) activar el reloj del puerto N y J
	LDR R0, [R1]
	ORR R0, R0, #0x1000 ; se valida el bit 12 para habilitar el reloj
	STR R0, [R1]
	NOP
	NOP ; se da tiempo para que el reloj se habilite

	movw R6,#0xA120
	movt R6,#0x0007

	LDR R1,GPIO_PORTN_DIR_N ; Valida la dirección del registro
	MOV R0, #0Xff ; PN0 PN1 salidas
	STR R0,[R1]
	LDR R1,GPIO_PORTN_DEN_N ; Habilita al puerto digital N
	MOV R0, #0XFF ; 1 significa que habilita E/S
	STR R0,[R1]
	LDR R1,GPIO_PORTN_DATA_N ; apunta al Puerto de datos N
	AND R0,#LEDS

salto
	STR R0,[R1] ; Escribe en el registro del datos del Puerto N
	mov R5,#0
	B Loop
jump
	MOV R2, #LEDS2
	STR R2,[R1]
	mov R5,#0
	B ReLop
Loop
	add R5,R5,#1
	cmp	R5,R6
	bne Loop
	B jump
ReLop
	add R5,R5,#1
	cmp	R5,R6
	bne ReLop
	B salto

	;movw R6,#0x0240
	;movt R6,#0x00f4
	.end
