#include <stdbool.h>
#include <stdint.h>

#define GPIOCLK      *(volatile int*) 0x400FE608 //Habilitaci�n del reloj

#define NVICEN1     *(volatile int*) 0xE000E104 //Registro de interrupci�n del NVIC para el Puerto J

#define PortJData   *(volatile int*) 0x4006000C //Datos del puerto J
#define PortJDir    *(volatile int*) 0x40060400 //Direcci�n del puerto J
#define PortJIS     *(volatile int*) 0x40060404 //Interrupt Sense del puerto J
#define PortJIBE    *(volatile int*) 0x40060408 //Interrupci�n por ambos flancos del puerto J
#define PortJIEV    *(volatile int*) 0x4006040C //Interrupci�n por evento del puerto J
#define PortJIM     *(volatile int*) 0x40060410 //M�scara de interrupci�n del puerto J
#define PortJICR    *(volatile int*) 0x4006041C //Clear de interrupciones del puerto J
#define PortJPUR    *(volatile int*) 0x40060510 //Resistencias de Pull-up para el puerto J
#define PortJDEN    *(volatile int*) 0x4006051C //Digital Enable del puerto J

#define PortNData   *(volatile int*) 0x4006400C //Datos del puerto N
#define PortNDir    *(volatile int*) 0x40064400 //Direcci�n del puerto N
#define PortNDEN    *(volatile int*) 0x4006451C //Digital Enable del puerto N

void Delay(int t){
    int i;
    for(i=0;i<t;i++){};
}

void PortJHandler(void){    //Funci�n llamada si hubo interrupci�n en el puerto J
    int i;
    if(PortJData==2){       //Si se apret� el bot�n 1
            PortNData^=1;   //Encendemos el bot�n 1
            Delay(11000000);//Damos un delay largooo
            PortNData^=1;   //Apagamos el bot�n 1
         }
    PortJICR=3;     //Limpio banderas de interrupci�n
}

void PortInit(void){
    GPIOCLK=0x1100;             //Habilito el reloj para los puertos J y N
    while(GPIOCLK!=0x1100){};   //Espero a que el reloj est� listo

    PortJDir=0;     //Habilito al puerto J como entrada
    PortJIS&=~3;    //No quiero que el puerto J sea sensible a niveles de voltaje
    PortJIBE&=~3;   //Ni a ambos flancos
    PortJIEV&=~3;   //S�lo que detecte flanco de bajada
    PortJIM|=3;     //Desenmascaro la interrupci�n
    PortJPUR=3;     //Habilito unas resistencias de pullup para el puerto J
    PortJDEN=3;     //Habilito el puerto J
    PortJICR=3;     //Limpio banderas de interrupci�n

    PortNDir=3;     //Habilito al puerto N como salida
    PortNDEN=3;     //Habilito al puerto N

    NVICEN1=1<<(51-32); //La interrupci�n 51 es la del puerto J, la habilito en el NVIC
    PortNData=0;        //Apagamos los leds
}

void main(void) {
    int i;
    PortInit(); //Funci�n para inicializar los puertos
    while(1)
        if(PortJData==1){       //Si se apret� el bot�n 2 entonces...
            for(i=0;i<5*2;i++){ //Hacemos 10 veces este cacho de c�digo
                PortNData^=2;   //Invertimos el Led 2
                Delay(1100000); //Damos un delay
            }
            while(PortJData==1){};//Esperamos a que se suelte el bot�n
        }
}
