#include <stdint.h>
#include <stdbool.h>

#define CLKENA      *(volatile int*) 0x400FE608 //Habilitación del reloj

#define NVICEN1     *(volatile int*) 0xE000E104 //Registro de habilitación del NVIC para la interrupción del Puerto J

#define PortJData   *(volatile int*) 0x40060004 //Datos del puerto J
#define PortJDir    *(volatile int*) 0x40060400 //Dirección del puerto J
#define PortJIS     *(volatile int*) 0x40060404 //Interrupt Sense del puerto J
#define PortJIBE    *(volatile int*) 0x40060408 //Interrupción por flanco de bajada o subida del puerto J
#define PortJIEV    *(volatile int*) 0x4006040C //Interrupción por evento del puerto J
#define PortJIM     *(volatile int*) 0x40060410 //Máscara de interrupción del puerto J
#define PortJICR    *(volatile int*) 0x4006041C //Clear de interrupciones del puerto J
#define PortJPUR    *(volatile int*) 0x40060510 //Resistencias de Pull-up para el puerto J
#define PortJDEN    *(volatile int*) 0x4006051C //Digital Enable del puerto J

#define PortNData   *(volatile int*) 0x4006400C //Datos del puerto N
#define PortNDir    *(volatile int*) 0x40064400 //Dirección del puerto N
#define PortNDEN    *(volatile int*) 0x4006451C //Digital Enable del puerto N

void Delay(void){   //Función para dar un delay
    int i;
    for(i=0;i<220000;i++){};//Hice un for que cuenta de 0 hasta 220000 para consumir tiempo
}
void JControl(void){        //La interrupción manda a llamar a esta función cuando se activa el puerto J
    PortNData^=3;           //Hago un xor para invertir la salida
    Delay();                //Hago un delay para quitar rebotes
    while(PortJData==0){};  //Espero a que se suelte el botón
    PortJICR=1;             //Limpio la bandera de interrupciones
}
void PortInit(void){
    CLKENA=0x1100;  //Habilito el reloj para los puertos J y N
    while(CLKENA!=0x1100){};//Espero a que el reloj esté listo

    PortJDir=0;     //Habilito al puerto J como entrada
    PortJIS&=~1;    //No quiero que el puerto J sea sensible a niveles de voltaje
    PortJIBE&=~1;   //Ni a ambos flancos
    PortJIEV&=~1;   //Solo que detecte flanco de bajada
    PortJIM|=1;     //Desenmascaro la interrupción
    PortJPUR=1;     //Habilito unas resistencias de pullup para el puerto J
    PortJDEN=1;     //Habilito el puerto J
    PortJICR=1;     //Limpio banderas de interrupción

    PortNDir=3;     //Habilito al puerto N como salida
    PortNDEN=3;     //Habilito al puerto N

    NVICEN1=1<<(51-32); //La interrupción 51 es la del puerto J, la habilito en el NVIC
}
void main(void) {
    PortInit();     //Inicializo los puertos
    while(1){};     //Espero a que haya interrupciones
}
