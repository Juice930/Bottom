	.global main
	.data
	.text
N	.field 0xC7854C20,32
M	.field 0x20000000,32
P	.field 0x20000008,32
main
	ldr R0, N
	mov R6, #0x20000000
	str R0, [R6]
	ldrb R0, [R6]
	ldrb R1, [R6,#1]
	ldrb R2, [R6,#2]
	ldrb R3, [R6,#3]
	and R4,R1,R2
	orr R4,R0,R4
	and R4,R4,R3
	ldr R6, P
	str R4,[R6]

	.end
