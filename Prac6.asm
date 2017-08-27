	.global main
main
	movw R0,#0x0004
	movt R0,#0x2000

	movw R2,#0x0020
	movt R2,#0x2000

	mov R4,#1

	mov R1,#0x25
	str R1,[R0]
	add R0,R0,#4

	mov R1,#0x99
	str R1,[R0]
	add R0,R0,#4

	mov R1,#0x29
	str R1,[R0]
	add R0,R0,#4

	mov R1,#0x2A
	str R1,[R0]
	add R0,R0,#4

	mov R1,#0x80
	str R1,[R0]
	add R0,R0,#4

	mov R1,#0x3C
	str R1,[R0]
	add R0,R0,#4
	mov R6,R0
	movw R0,#0x0004
	movt R0,#0x2000
selec
	ldr R3,[R0]
	cmp R6,R0
	beq fin
	and R5,R3,R4
	cmp R5,#1
	beq guardar
	add R0,R0,#4
	b selec
guardar
	str R3,[R2]
	add R2,R2,#4
	add R0,R0,#4
	b selec

fin b fin
	.end
