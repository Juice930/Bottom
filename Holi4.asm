      .global main
main
     ; Adicción no signada

      MOV R4, #0x20000000   ;R4 = Dirección base de los operandos
      LDR R1,[R4]           ;R1 = valor guardado en 0x20000000 (poner 0xFFFFF000)
      ADD R0, R1, #10       ;R0 = R1 + 10
      ADDS R0, R1, #10
      ADDS R0, R1, #0xFF
	  ADDS R0, R1, #0xFF00
    ; Substracción no signada
      SUBS R2, R1,#0x0F
      SUBS  R2, R1,#0xFF00
      SUBS  R2, R1,#0XFFFFFFFF
      ;-------------------------------------------------------------------------------
      ; Adición signadas
      LDR  R5, [R4,#4]      ;R5 = valor guardado en 0x20000004 (poner 0x7FFFF000)
      ADD R0, R5, #10
      ADDS R2, R5, #10
      ADDS R2, R5, #0xFF
	  ADDS R2, R5, #0xFF00
  ; Substracción  signada
      SUBS R2, R5,#0x0F
      SUBS  R2, R5,#0xFF00
      MOV  R2,#0XAA
      SUBS R2, R2,#0xFF00
      SUBS R2,R2,R5

      NOP
      .end
