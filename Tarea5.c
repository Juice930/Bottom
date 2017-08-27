#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#define CLKENA      *(volatile unsigned long *) 0x400FE608

#define NVICEN1     *(volatile uint32_t *)      0xE000E104  // Registro de habilitaci�n de interrupci�n PJ
#define NVICEN2     *(volatile uint32_t *)      0xE000E108  // Registro de habilitaci�n de interrupci�n PM
#define NVICPRI8    *(volatile uint32_t *)      0xE000E420  //Registro de prioridad de interrupci�n para el bit 35
#define NVICPRI12   *(volatile uint32_t *)      0xE000E430  //Registro de prioridad de interrupci�n para el bit 51
#define NVICPRI13   *(volatile uint32_t *)      0xE000E434  //Registro de prioridad de interrupci�n para el bit 53
#define NVICPRI18   *(volatile uint32_t *)      0xE000E448  //Registro de prioridad para el bit 72

#define PortJDir    *(volatile unsigned long *) 0x40060400
#define PortJPUR    *(volatile unsigned long *) 0x40060510
#define PortJDen    *(volatile unsigned long *) 0x4006051C
#define PortJData   *(volatile unsigned long *) 0x4006000C
#define PortJIS     *(volatile unsigned long *) 0x40060404  //Interrupt Sense
#define PortJIBE    *(volatile unsigned long *) 0x40060408  //Interrupt Both Edges
#define PortJIEV    *(volatile unsigned long *) 0x4006040C  //Interrupt Event
#define PortJIM     *(volatile unsigned long *) 0x40060410  //Interrupt Mask
#define PortJICR    *(volatile unsigned long *) 0x4006041C  //Interrupt Clear

#define PortKDir    *(volatile unsigned long *) 0x40061400
#define PortKDen    *(volatile unsigned long *) 0x4006151C
#define PortKData   *(volatile unsigned long *) 0x4006103C //4 segmentos de aqu�

#define PortLDir    *(volatile unsigned long *) 0x40062400
#define PortLDen    *(volatile unsigned long *) 0x4006251C
#define PortLPUR    *(volatile unsigned long *) 0x40062510
#define PortLData   *(volatile unsigned long *) 0x4006203C  //3 segmentos de aqu�
/*#define PortLIS     *(volatile unsigned long *) 0x40062404  //Interrupt Sense
#define PortLIBE    *(volatile unsigned long *) 0x40062408  //Interrupt Both Edges
#define PortLIEV    *(volatile unsigned long *) 0x4006240C  //Interrupt Event
#define PortLIM     *(volatile unsigned long *) 0x40062410  //Interrupt Mask
#define PortLICR    *(volatile unsigned long *) 0x4006241C  //Interrupt Clear*/

#define PortMData   *(volatile unsigned long *) 0x40063004  //1 bot�n
#define PortMDir    *(volatile unsigned long *) 0x40063400
#define PortMPUR    *(volatile unsigned long *) 0x40063510
#define PortMDen    *(volatile unsigned long *) 0x4006351C
#define PortMIS     *(volatile unsigned long *) 0x40063404  //Interrupt Sense
#define PortMIBE    *(volatile unsigned long *) 0x40063408  //Interrupt Both Edges
#define PortMIEV    *(volatile unsigned long *) 0x4006340C  //Interrupt Event
#define PortMIM     *(volatile unsigned long *) 0x40063410  //Interrupt Mask
#define PortMICR    *(volatile unsigned long *) 0x4006341C  //Interrupt Clear

#define Timer3          *(volatile uint32_t *)  0x400FE604  //Registro del TIMER
#define Timer3CFG       *(volatile uint32_t *)  0x40033000  //Configuraci�n del TIMER, aqu� se elige entre 16 o 32 bits
#define Timer3TAMR      *(volatile uint32_t *)  0x40033004  //Configuraci�n del conteo del TIMER
#define Timer3CTL       *(volatile uint32_t *)  0x4003300C  //Habilitaci�n del TIMER
#define Timer3ICR       *(volatile uint32_t *)  0x40033024  //Control de las banderas de interrupciones
#define Timer3TAILR     *(volatile uint32_t *)  0x40033028  //L�mite de conteo
#define Timer3TAPR      *(volatile uint32_t *)  0x40033038  //Divisor de frecuencia del contador
#define Timer3IMR       *(volatile uint32_t *)  0x40033018  //M�scara de las interrupciones del TIMER


int n=0;

void deco(void){//Enciende los LEDs necesarios para mostrar los n�meros
    if (n==0){
        PortKData=0;
        PortLData=1;
    }
    else if (n==1){
        PortKData=9;
        PortLData=7;
    }
    else if (n==2){
        PortKData=2;
        PortLData=2;
    }
    else if (n==3){
        PortKData=0;
        PortLData=6;
    }
    else if (n==4){
        PortKData=9;
        PortLData=4;
    }
    else if (n==5){
        PortKData=4;
        PortLData=4;
    }
    else if (n==6){
        PortKData=4;
        PortLData=0;
    }
    else if (n==7){
        PortKData=1;
        PortLData=7;
    }
    else if (n==8){
        PortKData=0;
        PortLData=0;
    }
    else{
        PortKData=1;
        PortLData=4;
    }
}

void conteo(void){  //Llamar a esta funci�n si hay interrupci�n de TIMER
    n=(n+1)%10;     //n es una variable global que lleva el conteo
    deco();         //Habilitamos los leds correspondientes para mostrar el n�mero
    Timer3ICR=1;    //Resetea la bandera de interrupci�n
}
void reset(void){   //Llamar a esta funci�n si hay interrupci�n de Puerto M
    n=0;            //Reiniciamos el contador
    deco();         //Habilitamos los leds correspondientes para mostrar el n�mero
    PortMICR=1;     //Limpiamos las banderas
}
void delaychange(){ //Llamar a esta funci�n si hay interrupci�n en el puerto J
    int Bot=PortJData;
    Timer3CTL=0;           //Deshabilita timer
    if (Bot==2)
        //Timer3TAPR=0xFF;//Escal� la frecuencia para 1 s
        Timer3TAILR=16000000;    //Esta instrucci�n no funciona
    else if (Bot==1)
        //Timer3TAPR=0x7F;//Aqu� la escal� para 1/2 s
        Timer3TAILR=16000000/2;     //Esta tampoco
    Timer3CTL=1;    //Habilita timer
    PortJICR=3;     //Reseteamos las banderas
}
void timer_ini(int time){   //80 MHz
    Timer3CTL=0;           //Deshabilita timer
    Timer3|=0x08;           //Para activar el reloj del Timer3
    //Timer3CFG=4;            //Reloj de 16 bits
    Timer3CFG=0;
    Timer3TAMR=0x00000002;  //Contador ascendente peri�dico
    Timer3ICR=1;            //Resetea la bandera de interrupci�n
    //Timer3TAPR=0xFF;        //120 MHz/240=0.5 MHz
    Timer3TAPR=0;
    Timer3IMR=0;            //Enmascara al TIMER
    Timer3TAILR=time-1;       //Cuenta m�x
    Timer3CTL=1;
}
void port_ini(void){
    CLKENA=0x0F00;//Puerto J,K y L
    while(CLKENA!=0x0F00){};
    PortJDir=0;     //Entradas
    PortJDen=3;     //Habilitar 2 Entradas
    PortJPUR=3;     //2 Pull-up Resistors
    PortJIS&=~3;    //No queremos detectar niveles de voltaje
    PortJIBE&=~3;   //Detectar both-edges
    PortJIEV&=~3;   //Queremos habilitar el evento cuando sea falling edge
    PortJICR=3;     //Limpiamos las banderas
    PortJIM|=3;     //Desenmascaramos los botones

    PortKDir=0xF;   //Queremos 4 salidas
    PortKDen=0xF;   //Habilitamos 4 salidas

    PortLDir=7;    //MSB es entrada, los dem�s son salidas
    PortLDen=7;

    PortMDir=0;     //Entradas
    PortMDen=1;     //Habilitar 1 Entrada
    PortMPUR=1;     //Habilitamos un bot�n de salida
    PortMIS&=~1;    //No queremos detectar niveles de voltaje
    PortMIBE&=~1;   //No detectar both-edges
    PortMIEV&=~1;   //Queremos habilitar el evento cuando sea falling edge
    PortMICR=1;     //Limpiamos las banderas
    PortMIM|=1;     //Desenmascaramos los botones

    timer_ini(16000000);

    /*NVICPRI18 &= 0xFFFFFF00;//Prioridad 0 para Reset
    NVICPRI8  = (NVICPRI8 & 0x00FFFFFF) | 0x40000000 ;//Prioridad 2 para TIMER
    NVICPRI12 = (NVICPRI12 & 0x00FFFFFF) | 0x20000000 ; //Prioridad 1 para puerto J, botones*/

    NVICPRI8 &= 0x00FFFFFF;                             //Prioridad 0 para TIMER
    NVICPRI12 = (NVICPRI12 & 0x00FFFFFF) | 0x40000000;  //Prioridad 2 para puerto J, botones
    //NVICPRI13 = (NVICPRI13 & 0xFFFF00FF) | 0x00000200;  //Prioridad 1 para RESET en L
    NVICPRI18 = (NVICPRI18 & 0xFFFFFF00) | 0x00000002;  //Prioridad 1 para RESET en M

    NVICEN1= 1<<(51-32);    //Habilitamos la interrupci�n 51 de NVIC    Puerto J
    //NVICEN1= 1<<(53-32);    //Habilitamos la interrupci�n 53 de NVIC    Puerto L
    NVICEN1= 1<<(35-32);    //Habilitamos la interrupci�n 35 de NVIC,   TIMER
    NVICEN2= 1<<(72-64);    //Habilitamos la interrupci�n 72 de NVIC    Puerto M

    Timer3IMR|=1;//Desenmascara al TIMER, lo hice al final para que no llamara a la interrupci�n antes de tiempo
}
void main(void){
	port_ini();
	while(1){};
}
