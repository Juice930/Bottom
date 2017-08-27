	.global main
main
	MOV R10,#04 ; a R10 se va a guardar el valor 04 direccionamiento inmediato
	LDR R8,[R10] ;carga en registro desde memoria
	MOV R8,#00 ;direccionamiento inmediato

	.end
