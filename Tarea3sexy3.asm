	.global main
	.data
	.text
N	.field 0x12345678,32
M	.field 0x87654321,32
P	.field 0xFFFFFF00,32
main
	mov R6,#0x20000000
	ldr R1,M
	mov R3,#0x00000000
	ldr R4,N
	str 	R1,[R6]
	ldrb	R3,[R6,#3]
	ldr R6,P
	and		R4,R4,R6
	orr		R4,R4,R3

	.end
