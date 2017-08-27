#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#define CLKENA      *(volatile unsigned long *) 0x400FE608

#define NVICEN0     *(volatile uint32_t *)      0xE000E100  // Registro de habilitación de interrupción PD
#define NVICEN1     *(volatile uint32_t *)      0xE000E104  // Registro de habilitación de interrupción PJ
#define NVICEN2     *(volatile uint32_t *)      0xE000E108  // Registro de habilitación de interrupción PM

#define NVICPRI0    *(volatile uint32_t *)      0xE000E400  //Registro de prioridad de interrupción para el bit 3
#define NVICPRI8    *(volatile uint32_t *)      0xE000E420  //Registro de prioridad de interrupción para el bit 35
#define NVICPRI12   *(volatile uint32_t *)      0xE000E430  //Registro de prioridad de interrupción para el bit 51
#define NVICPRI13   *(volatile uint32_t *)      0xE000E434  //Registro de prioridad de interrupción para el bit 53
#define NVICPRI18   *(volatile uint32_t *)      0xE000E448  //Registro de prioridad para el bit 72

#define PortDData   *(volatile unsigned long *) 0x4005B004  //1 botón
#define PortDDir    *(volatile unsigned long *) 0x4005B400
#define PortDPUR    *(volatile unsigned long *) 0x4005B510
#define PortDDen    *(volatile unsigned long *) 0x4005B51C
#define PortDIS     *(volatile unsigned long *) 0x4005B404  //Interrupt Sense
#define PortDIBE    *(volatile unsigned long *) 0x4005B408  //Interrupt Both Edges
#define PortDIEV    *(volatile unsigned long *) 0x4005B40C  //Interrupt Event
#define PortDIM     *(volatile unsigned long *) 0x4005B410  //Interrupt Mask
#define PortDICR    *(volatile unsigned long *) 0x4005B41C  //Interrupt Clear

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
#define PortKData   *(volatile unsigned long *) 0x4006103C //4 segmentos de aquí

#define PortLDir    *(volatile unsigned long *) 0x40062400
#define PortLDen    *(volatile unsigned long *) 0x4006251C
#define PortLPUR    *(volatile unsigned long *) 0x40062510
#define PortLData   *(volatile unsigned long *) 0x4006203C  //3 segmentos de aquí

#define PortMData   *(volatile unsigned long *) 0x40063004  //1 botón
#define PortMDir    *(volatile unsigned long *) 0x40063400
#define PortMPUR    *(volatile unsigned long *) 0x40063510
#define PortMDen    *(volatile unsigned long *) 0x4006351C
#define PortMIS     *(volatile unsigned long *) 0x40063404  //Interrupt Sense
#define PortMIBE    *(volatile unsigned long *) 0x40063408  //Interrupt Both Edges
#define PortMIEV    *(volatile unsigned long *) 0x4006340C  //Interrupt Event
#define PortMIM     *(volatile unsigned long *) 0x40063410  //Interrupt Mask
#define PortMICR    *(volatile unsigned long *) 0x4006341C  //Interrupt Clear

#define Timer3          *(volatile uint32_t *)  0x400FE604  //Registro del TIMER
#define Timer3CFG       *(volatile uint32_t *)  0x40033000  //Configuración del TIMER, aquí se elige entre 16 o 32 bits
#define Timer3TAMR      *(volatile uint32_t *)  0x40033004  //Configuración del conteo del TIMER
#define Timer3CTL       *(volatile uint32_t *)  0x4003300C  //Habilitación del TIMER
#define Timer3ICR       *(volatile uint32_t *)  0x40033024  //Control de las banderas de interrupciones
#define Timer3TAILR     *(volatile uint32_t *)  0x40033028  //Límite de conteo
#define Timer3TAPR      *(volatile uint32_t *)  0x40033038  //Divisor de frecuencia del contador
#define Timer3IMR       *(volatile uint32_t *)  0x40033018  //Máscara de las interrupciones del TIMER


int n=0;
bool op=0;
void deco(void){//Enciende los LEDs necesarios para mostrar los números
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
void deco2(void){
    if (n==0){
        PortKData=1;
        PortLData=0;
    }
    else if (n==1){
        PortKData=0;
        PortLData=0;
    }
    else if (n==2){
        PortKData=6;
        PortLData=1;
    }
    else if (n==3){
        PortKData=0;
        PortLData=1;
    }
    else if (n==4){
        PortKData=6;
        PortLData=0;
    }
    else if (n==5){
        PortKData=7;
        PortLData=0;
    }
    else if (n==6){
        PortKData=4;
        PortLData=0;
    }
    else if (n==7){
        PortKData=9;
        PortLData=0;
    }
    else if (n==8){
        PortKData=9;
        PortLData=7;
    }
}
void portDhandler(void){
    Timer3CTL=0;
    Timer3TAILR=16000000*2;
    Timer3CTL=1;    //Habilita timer
    op=1;
    PortDICR=1;
}
void conteo(void){  //Llamar a esta función si hay interrupción de TIMER
    if (op==0){
        n=(n+1)%10;     //n es una variable global que lleva el conteo
        deco();         //Habilitamos los leds correspondientes para mostrar el número
        Timer3ICR=1;    //Resetea la bandera de interrupción
    }
    else{
        n=(n+1)%9;     //n es una variable global que lleva el conteo
        deco2();         //Habilitamos los leds correspondientes para mostrar el número
        Timer3ICR=1;    //Resetea la bandera de interrupción
    }
}
void reset(void){   //Llamar a esta función si hay interrupción de Puerto M
    n=0;            //Reiniciamos el contador
    deco();         //Habilitamos los leds correspondientes para mostrar el número
    PortMICR=1;     //Limpiamos las banderas
}
void delaychange(){ //Llamar a esta función si hay interrupción en el puerto J
    if(!op){
        int Bot=PortJData;
        Timer3CTL=0;           //Deshabilita timer
        if (Bot==2)
            //Timer3TAPR=0xFF;//Escalé la frecuencia para 1 s
            Timer3TAILR=16000000;    //Esta instrucción no funciona
        else if (Bot==1)
            //Timer3TAPR=0x7F;//Aquí la escalé para 1/2 s
            Timer3TAILR=16000000/2;     //Esta tampoco
        Timer3CTL=1;    //Habilita timer
    }
    PortJICR=3;//Reseteamos las banderas
}
void timer_ini(int time){   //80 MHz

    Timer3CTL=0;           //Deshabilita timer
    Timer3|=0x08;           //Para activar el reloj del Timer3
    //Timer3CFG=4;            //Reloj de 16 bits
    Timer3CFG=0;

    TIMER3_TAMR_R=0x00000001;
    //Timer3TAMR=0x00000002;  //Contador ascendente periódico
    TIMER3_ICR_R=1;         //Resetea la bandera de interrupción
    //Timer3TAPR=0xFF;        //120 MHz/240=0.5 MHz
    TIMER3_TAPR_R=0;
    TIMER3_IMR_R=0;            //Enmascara al TIMER
    TIMER3_TAILR_R=time-1;       //Cuenta máx
    TIMER3_CTL_R=1;
}
void port_ini(void){
    CLKENA=0x0F08;//Puerto D,J,K, L y M
    while(CLKENA!=0x0F08){};
    PortDDir=0;     //Entradas
    PortDDen=1;     //Habilitar 1 Entrada
    PortDPUR=1;     //Habilitamos un botón de salida
    PortDIS&=~1;    //No queremos detectar niveles de voltaje
    PortDIBE&=~1;   //No detectar both-edges
    PortDIEV&=~1;   //Queremos habilitar el evento cuando sea falling edge
    PortDICR=1;     //Limpiamos las banderas
    PortDIM|=1;     //Desenmascaramos los botones

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

    PortLDir=7;    //MSB es entrada, los demás son salidas
    PortLDen=7;

    PortMDir=0;     //Entradas
    PortMDen=1;     //Habilitar 1 Entrada
    PortMPUR=1;     //Habilitamos un botón de salida
    PortMIS&=~1;    //No queremos detectar niveles de voltaje
    PortMIBE&=~1;   //No detectar both-edges
    PortMIEV&=~1;   //Queremos habilitar el evento cuando sea falling edge
    PortMICR=1;     //Limpiamos las banderas
    PortMIM|=1;     //Desenmascaramos los botones

    timer_ini(16000000);

    NVICPRI0  = (NVICPRI0 & 0x00FFFFFF)|0x20FFFFFF;     //Prioridad 1 para Puerto D
    NVICPRI8 &= 0x00FFFFFF;                             //Prioridad 0 para TIMER
    NVICPRI12 = (NVICPRI12 & 0x00FFFFFF) | 0x40000000;  //Prioridad 2 para puerto J, botones
    NVICPRI18 = (NVICPRI18 & 0xFFFFFF00) | 0x00000002;  //Prioridad 1 para RESET en M
    NVICEN0=8;              //Interrupción 3 de NVIC Puerto D
    NVICEN1= 1<<(51-32);    //Habilitamos la interrupción 51 de NVIC    Puerto J
    NVICEN1= 1<<(35-32);    //Habilitamos la interrupción 35 de NVIC,   TIMER
    NVICEN2= 1<<(72-64);    //Habilitamos la interrupción 72 de NVIC    Puerto M

    Timer3IMR|=1;//Desenmascara al TIMER, lo hice al final para que no llamara a la interrupción antes de tiempo
}
void main(void){
    port_ini();
    while(1){};
}
