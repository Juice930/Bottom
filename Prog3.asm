	.global main
main

	MOV R0,#0xAAAA      ;mover #0xAAAA a R0
    MOV R1,#0X9999      ;mover #0x9999 a R1

    AND R5, R1, #0XFF00  ;R5 = R1 & #0XFF00

    AND R1, R1, R0       ;R1 = R1 & R0

    AND R1, R1, R5, LSR #1  ; R1 = R1&(R5>>1);

    MOV R4,#0xAAAA      ;mover #0xAAAA a R0
    MOV R3,#0X9999      ;mover #0x9999 a R1

    AND R4, R3, #1       ;R4 = R3 & 0X00000001, no pone bandera

    MOV R4,#0xAAAA      ;mover #0xAAAA a R0
    MOV R3,#0X9999      ;mover #0x9999 a R1

    ANDS R4, R3, #1       ;R4 = R3 & 0X00000001, pone bandera


    MOV R0,#0xAAAA      ;mover 0 a R0
    ORR R0, R0, #0x0055  ; R0= R0 |0X0055

    .end
