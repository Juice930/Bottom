#include <stdbool.h>
#include <stdint.h>
#define NVIC_ST_CTRL_R (*((volatile unsigned long *)0xE000E010)) //apuntador del registro que permite configurar las acciones del temporizador
#define NVIC_ST_RELOAD_R (*((volatile unsigned long *)0xE000E014)) //apuntador del registro que contiene el valor de inicio del contador
#define NVIC_ST_CURRENT_R (*((volatile unsigned long *)0xE000E018)) //apuntador del registro que presenta el estado de la cuenta actual
#define NVIC_ST_CTRL_COUNT 0x00010000 // bandera de cuenta flag
#define NVIC_ST_CTRL_CLK_SRC 0x00000004 // Clock Source
#define NVIC_ST_CTRL_INTEN 0x00000002 // Habilitador de interrupción
#define NVIC_ST_CTRL_ENABLE 0x00000001 // Modo del contador

#define GPIO_PORTN_DATA_R (*((volatile unsigned long *)0x4006400C))
#define GPIO_PORTN_DIR_R (*((volatile unsigned long *)0x40064400))
#define GPIO_PORTN_DEN_R (*((volatile unsigned long *)0x4006451C))

#define GPIO_PORTE_DATA_R (*((volatile unsigned long *)0x4005C004))
#define GPIO_PORTE_DIR_R (*((volatile unsigned long *)0x4005C400))
#define GPIO_PORTE_DEN_R (*((volatile unsigned long *)0x4005C51C))

#define SYSCTL_RCGC2_R (*((volatile unsigned long *)0x400FE608))
#define SYSCTL_PRGPIO_R (*((volatile unsigned long *)0x400FEA08))
#define SYSCTL_RCGC2_GPION 0x00001000 // bit de estado del reloj de puerto N

#define length 12
int Notas[]={494,659,784,698,659,784,698,698,659,523,587,494};//B4,E5,G5,F5,E5,G5,E5,F5,E5,C5,D5,B4

//#define length 4
//int Notas[]={1,5,2,10};
// Inizializa el SysTick
void SysTick_Init(void){
NVIC_ST_CTRL_R = 0; // Desahabilita el SysTick durante la configuración
NVIC_ST_RELOAD_R = 0xFFFF; // Se establece el valor de cuenta deseado en RELOAD_R
NVIC_ST_CURRENT_R = 0; // Se escribe al registro current para limpiarlo
NVIC_ST_CTRL_R = 0x00000005; // Se Habilita el SysTick y se selecciona la fuente de reloj
}

void Frecuenciador(uint32_t f){
    uint32_t i;
    for(i=0;i<3*f;i++){
        NVIC_ST_RELOAD_R=16000000/(2*f)-1;
        NVIC_ST_CURRENT_R=0;
        while((NVIC_ST_CTRL_R&0x00010000)==0){};
        GPIO_PORTN_DATA_R = GPIO_PORTN_DATA_R^0x03;
        GPIO_PORTE_DATA_R = !GPIO_PORTE_DATA_R;
    }
}

int main(void){
uint32_t i;
SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPION; // activa reloj puerto N
SysTick_Init(); // inicializo SysTick
GPIO_PORTN_DIR_R |= 0x0F; // puerto N de salida
GPIO_PORTN_DEN_R |= 0x0F; // habilita el puerto N
GPIO_PORTN_DATA_R = 0x02; // enciende 1 led

GPIO_PORTE_DIR_R |= 0x01;
GPIO_PORTE_DEN_R |= 0x01;

while(1){
    for(i=0;i<length;i++)
        Frecuenciador(Notas[i]);
}
}
