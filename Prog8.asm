; PROGRAMA LECTURA Y ESCRITURA DE PUERTOS PARALELOS
;  Lee PJ0 y PJ1 y escribe el dato leido en PN0 y PN1

  .global main

 ;   .thumb               ; se usara instrucciones thumb
 ;  .data                    ; define variables globales
       .text                 ;define codigo del programa y lo ubica en flash

SYSCTL_RCGCGPIO_R  .field 0x400FE608,32    ; REGISTRO DEL RELOJ

GPIO_PORTN_DIR_N     .field 0x40064400,32    ; Registro de Dirección N
GPIO_PORTN_DEN_N     .field 0x4006451C,32    ; Registro de habilitación N
GPIO_PORTN_DATA_N    .field 0x4006400C,32    ; Registro de Datos N

GPIO_PORTN_DIR_J     .field 0x40060400,32    ; Registro de Dirección J
GPIO_PORTN_PUR_J     .field 0x40060510,32    ; Registro de pull-up J
GPIO_PORTN_DEN_J     .field 0x4006051C,32    ; Registro de habilitación J
GPIO_PORTN_DATA_J    .field 0x4006000C,32    ; Registro de Datos J

SW     .EQU 0X3
main


      LDR R1,SYSCTL_RCGCGPIO_R      ; 1) activar el reloj del puerto N y J
      LDR R0, [R1]
      ORR R0, R0, #0x1100           ; se valida el bit 12 para habilitar el reloj N y J
      STR R0, [R1]
      NOP
      NOP                           ; se da tiempo para que el reloj se habilite
;---------------------------------------------------------------
;                    CONFIGURACION PUERTO J
;---------------------------------------------------------------

      LDR R1,GPIO_PORTN_DIR_J   ; Configura la dirección del registro J
      MOV R0, #0X00             ; PJ1 PJ2 entradas
      STR R0,[R1]

      LDR R1,GPIO_PORTN_PUR_J   ; Configura los resistores pull_up J
      MOV R0, #0X03             ; habilita weak pull up para PJ0 PJ1
      STR R0,[R1]


      LDR R1,GPIO_PORTN_DEN_J   ; Habilita al puerto digital J
      MOV R0, #0XFF             ; 1 significa que habilita E/S
      STR R0,[R1]

;---------------------------------------------------------------
;                    CONFIGURACION PUERTO N
;---------------------------------------------------------------


      LDR R1,GPIO_PORTN_DIR_N       ; Configura la dirección del registro
      MOV R0, #0X03                ; PN0 PN1 salidas
      STR R0,[R1]

      LDR R1,GPIO_PORTN_DEN_N       ; Habilita al puerto digital N
      MOV R0, #0XFF                 ; 1 significa que habilita E/S
      STR R0,[R1]

;---------------------------------------------------------------
;                    RUTINA DE LECTURA ESCRITURA
;---------------------------------------------------------------
loop
	LDR R1,GPIO_PORTN_DATA_J   ; apunta al Puerto de datos J
    LDR R0,[R1]
    MOV R5,R0
    CMP R0,#SW
    BLNE sal
	B loop

sal
	LDR R1,GPIO_PORTN_DATA_N	; apunta al Puerto de datos N
    STR R5,[R1]             	; Escribe en el registro del datos del Puerto N el valor de J
    BX LR
; -----------------
    .end
