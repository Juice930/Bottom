	.global main
	.text ;define codigo del programa y lo ubica en flash

SYSCTL_RCGCGPIO_R 	.field 0x400FE608,32    ;REGISTRO DEL RELOJ

GPIO_PORTN_DIR_H	.field 0x4005F400,32	;Dirección del Puerto H
GPIO_PORTN_DEN_H 	.field 0x4005F51C,32	;Enable del Puerto H
GPIO_PORTN_DATA_H	.field 0x4005F00C,32	;Datos del Puerto H

GPIO_PORTN_DIR_K 	.field 0x40061400,32 ; Registro de Dirección K
GPIO_PORTN_DEN_K	.field 0x4006151C,32 ; Registro de habilitación K
GPIO_PORTN_DATA_K 	.field 0x40061280,32 ; Registro de Datos K

LEDS	.EQU 0x0
LED3	.EQU 0x1
LED4	.EQU 0x2
LED33	.EQU 0x3

main
	LDR R1,SYSCTL_RCGCGPIO_R
	LDR R0, [R1]
	ORR R0, R0, #0x0280 ; Habilitar el reloj de los puertos H y K
	STR R0, [R1]

	NOP
	NOP ; se da tiempo para que el reloj se habilite

;---------------------------------------------------------------
; CONFIGURACION PUERTO H
;---------------------------------------------------------------
	LDR R1,GPIO_PORTN_DEN_H ; Habilita al puerto digital F
	MOV R0, #0XFF ; 1 significa que habilita E/S
	STR R0,[R1]
	LDR R1,GPIO_PORTN_DIR_H ; Configura la dirección del registro
	MOV R0, #0x03 ; PH0 PH1 salidas
	STR R0,[R1]

;---------------------------------------------------------------
; CONFIGURACION PUERTO K
;---------------------------------------------------------------

	LDR R2,GPIO_PORTN_DEN_K ; Habilita al puerto digital K
	MOV R0, #0X50 ; 1 significa que habilita E/S
	STR R0,[R2]
	LDR R1,GPIO_PORTN_DIR_K ; Configura la dirección del registro K
	MOV R0, #0X00 ; PJ1 PJ2 entradas
	STR R0,[R1]

;---------------------------------------------------------------
; RUTINA DE LECTURA ESCRITURA
;---------------------------------------------------------------

loop:		LDR R1,GPIO_PORTN_DATA_K 	;Lee los botones
			LDR R0,[R1]			        ;Guarda los botones

			CMP R0,#LED33
			BEQ PRENDELED33

			CMP R0,#LED3
			BEQ PRENDELED3

			CMP R0,#LED4
			BEQ PRENDELED4

			CMP R0,#LEDS
			BEQ PRENDELEDS

;Apaga los leds
PRENDELED33:LDR R1,GPIO_PORTN_DATA_H ; apunta al Puerto de datos F
			mov R5,#0x0
			STR R5,[R1] ; Escribe en el registro del datos del Puerto F el valor de J
			B loop

;AQUI SE PRENDE LED 3
PRENDELED3: LDR R1,GPIO_PORTN_DATA_H ; apunta al Puerto de datos F
			mov R5,#0x01
			STR R5,[R1] ; Escribe en el registro del datos del Puerto F el valor de J
			B loop

;AQUI SE PRENDE LED 4
PRENDELED4: LDR R1,GPIO_PORTN_DATA_H ; apunta al Puerto de datos F
			mov R5,#0x02
			STR R5,[R1] ; Escribe en el registro del datos del Puerto F el valor de J
			B loop

;AQUI SE PRENDEN LOS 2 LEDS, LED 3 Y LED4
PRENDELEDS: LDR R1,GPIO_PORTN_DATA_H ; apunta al Puerto de datos F
			mov R5,#0x03
			STR R5,[R1] ; Escribe en el registro del datos del Puerto F el valor de J
			B loop
			.end
