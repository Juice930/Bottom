#include <stdint.h> //Librerías
#include <stdbool.h>
#define CLKENA *(volatile int *) 0x400FE608         //Enable del Reloj

#define PortJData   *(volatile int *) 0x4006000C    //Datos del puerto J
#define PortJDir    *(volatile int *) 0x40060400    //Dirección del puerto J
#define PortJPUR    *(volatile int *) 0x40060510    //Resistencias de Pull-Up del puerto J
#define PortJEna    *(volatile int *) 0x4006051C    //Enable del Puerto J

#define PortFData   *(volatile int *) 0x4005D004    //Datos del Puerto N
#define PortFDir    *(volatile int *) 0x4005D400    //Dirección del puerto N
#define PortFEna    *(volatile int *) 0x4005D51C    //Enable del Puerto N

#define PortNData   *(volatile int *) 0x4006400C    //Datos del Puerto F
#define PortNDir    *(volatile int *) 0x40064400    //Dirección del puerto F
#define PortNEna    *(volatile int *) 0x4006451C    //Enable del Puerto F

void delay(void){   //Función para dar un delay de 250000 ciclos de reloj
    int n=0;        //Declaramos una variable que empiece en 0
    while(n<250000) //Mientras n sea menor a 250000
        n++;        //A n le sumamos 1
}
void main(void) {       //Función principal
    int n=0;
	CLKENA|=0x1120;     //Habilitamos el reloj para los puertos F, J y N
	while(CLKENA && 0x1120==0){};   //Esperamos a que esté listo el reloj

	PortJEna=1;   //Habilitamos un botón del puerto J
	PortJDir=0;     //Lo ponemos como entrada
	PortJPUR=1;   //Habilitamos una resistencia de Pull-Up

	PortNEna=3;   //Habilitamos los Leds del puerto N
	PortNDir=3;   //Los ponemos como salidas
	PortNData=0;    //Los apagamos
	
	PortFEna=1;   //Habilitamos los Leds del puerto N
	PortFDir=1;   //Los ponemos como salidas
	PortFData=0;    //Los apagamos
	while(1)    //loop infinito
	    if(!PortJData){     //Si el botón es apretado
	        n=(n+1)%8;      //n irá del 0 al 7
	        PortFData=n/4;  //El led se irá encendiendo dependiendo del bit más significativo
	        PortNData=n%4;  //Le sumamos a lo que haya en el puerto 1 y le hacemos mod 4
	        delay();                    //Hacemos un delay para evitar rebote
	        while(PortJData==0){};      //Esperamos a que el botón sea soltado
	    }
}
