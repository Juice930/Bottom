	.global main
main
      MOV R0,#0xAAAA      ;mover 0 a R0
      MOV R1,#0X20000000  ;carga el número 0x20000000 en R1

      STR R0,[R1,#4]      ;el valor de 32 bits de R0 se almacena  en la dirección R1+4 Ri,#4 dirección indexada
      						;STR instrucción de acceso a memoria
      STRH R2,[R1],#8    ;el valor de 16-bits de R2 se almacena en la dirección apuntada por R1, despues R1=R1+8
							;H significa que se guarda para 16 bits
      MOV R3,#8           ;mover FF a R3

      STRB R2,[R1, R3]    ;el valor de 8-bits de R2 se almacena en la dirección apuntada por R1+R3

      STRB R2,[R1, #1]!   ;R1=R1+1, el valor de 8-bits de R2 se almacena en la dirección apuntada por R1+1


      .end
