	.global main
main
	mov R0,#12
	mov R1,#23
	mov R2,#35
	mov R3,R0,lsl #2
	orr R12,R1,R2
	eor R12,R12,R3
	mvn R12,R12
	and R12,R1,R12
	.end
