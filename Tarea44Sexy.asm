	.global main
	.text ;define codigo del programa y lo ubica en flash

SYSCTL_RCGCGPIO_R 	.field 0x400FE608,32 ; REGISTRO DEL RELOJ

GPIO_PORTN_DIR_F	.field 0x4005D400,32	; Dirección del Puerto F
GPIO_PORTN_DEN_F 	.field 0x4005D51C,32	; Enable del Puerto F
GPIO_PORTN_DATA_F	.field 0x4005D044,32	; Datos del Puerto F

GPIO_PORTN_DIR_N	.field 0x40064400,32 	; Registro de Dirección N
GPIO_PORTN_DEN_N 	.field 0x4006451C,32 	; Registro de habilitación N
GPIO_PORTN_DATA_N 	.field 0x4006400C,32 	; Registro de Datos N

GPIO_PORTN_DIR_J 	.field 0x40060400,32 	; Registro de Dirección J
GPIO_PORTN_PUR_J 	.field 0x40060510,32 	; Registro de pull-up J
GPIO_PORTN_DEN_J 	.field 0x4006051C,32 	; Registro de habilitación J
GPIO_PORTN_DATA_J 	.field 0x4006000C,32 	; Registro de Datos J

main
	LDR R1,SYSCTL_RCGCGPIO_R 	;activar el reloj del puerto F,N y J
	LDR R0, [R1]
	ORR R0, R0, #0x1120 		;Se activa el reloj para los puertos F, N y J
	STR R0, [R1]

	NOP
	NOP ; se da tiempo para que el reloj se habilite


	;Puerto F LEDS 3 y 4
	LDR R1,GPIO_PORTN_DIR_F ; Configura la dirección del registro
	MOV R0, #0x11 ; PN0 PN4 salidas
	STR R0,[R1]
	LDR R1,GPIO_PORTN_DEN_F ; Habilita al puerto digital F
	MOV R0, #0X11 ; 1 significa que habilita S
	STR R0,[R1]

	;Puerto N LEDS 1 y 2

	LDR R1,GPIO_PORTN_DIR_N ; Configura la dirección del registro
	MOV R0, #0X03 			; PN0 PN1 salidas
	STR R0,[R1]
	LDR R1,GPIO_PORTN_DEN_N ; Habilita al puerto digital N
	MOV R0, #0X3			; 1 significa que habilita E/S
	STR R0,[R1]

	;Puerto J Botones

	LDR R1,GPIO_PORTN_DIR_J ; Configura la dirección del registro J
	MOV R0, #0X00			; PJ1 PJ2 entradas
	STR R0,[R1]
	LDR R1,GPIO_PORTN_PUR_J ; Configura los resistores pull_up J
	MOV R0, #0X03			; habilita weak pull up para PJ0 PJ1
	STR R0,[R1]
	LDR R1,GPIO_PORTN_DEN_J ; Habilita al puerto digital J
	MOV R0, #0XFF 			; 1 significa que habilita E/S
	STR R0,[R1]

loop
	LDR R1,GPIO_PORTN_DATA_J 	;Lee los botones (Los botones están en 1 por default)
	LDR R0,[R1]					;Guarda los botones

	CMP R0,#3	;Si los botones valen 3 vamos a cuatro
	beq cuatro	;Nos manda a cuatro si se cumple

	CMP R0,#0	;Si los botones valen 0 vamos a uno
	beq uno		;Nos manda a uno si se cumple

	CMP R0,#1	;Si los botones valen 1 vamos a dos
	beq dos		;Nos manda a dos si se cumple

				;Como no se cumplieron las otras 3 opciones solo queda tres
	LDR R1,GPIO_PORTN_DATA_N ; apunta al Puerto de datos N
	mov R5,#0
	STR R5,[R1] ;Apaga los leds

	LDR R1,GPIO_PORTN_DATA_F ; apunta al Puerto de datos F
	mov R5,#0x10
	STR R5,[R1] ;Enciende un led
	B loop		;Nos regresa al loop

uno
	LDR R1,GPIO_PORTN_DATA_N ; apunta al Puerto de datos N
	mov R5,#2	;R5 tiene lo que vamos a encender
	STR R5,[R1] ;Enciende un led

	LDR R1,GPIO_PORTN_DATA_F ; apunta al Puerto de datos F
	mov R5,#0
	STR R5,[R1] ;Apaga los leds
	B loop		;Nos regresa al loop

dos
	LDR R1,GPIO_PORTN_DATA_N ; apunta al Puerto de datos N
	mov R5,#1
	STR R5,[R1] ;Enciende un led

	LDR R1,GPIO_PORTN_DATA_F ; apunta al Puerto de datos F
	mov R5,#0
	STR R5,[R1] ;Apaga los leds
	B loop		;Nos regresa al loop

cuatro
	LDR R1,GPIO_PORTN_DATA_N ; apunta al Puerto de datos N
	mov R5,#0
	STR R5,[R1] ;Apaga los leds

	LDR R1,GPIO_PORTN_DATA_F ; apunta al Puerto de datos F
	mov R5,#0x1
	STR R5,[R1] ;Enciende un led
	B loop		;Nos regresa al loop

	.end
