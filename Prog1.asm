      .global main
main
      MOV R1,#4         ; carga el número 4 en R10
      LDRB R0,[R1]      ; carga 8-bits no signados desde la dirección apuntada por R1 a R0
      LDRH R2,[R1]      ; carga 16-bits no signados desde la dirección apuntada por R1 a R2
      MOV R0,#255        ; mover 0 a R0

      MOV R1,#1396
      LDRSB R0,[R1,#10]  ;carga 8-bits signados desde la dirección (R1+8) a R0
      LDR  R0,[R1,#10]   ;carga 32-bits desde la dirección R1 a R0, R0= R1 +10

      .end
