#include <stdbool.h>
#include <stdint.h>

#define SYSCTL_RCGCGPIO_R       (*((volatile uint32_t *)0x400FE608))
#define SYSCTL_PRGPIO_R         (*((volatile unsigned long *)0x400FEA08))
#define GPIO_PORTN_DATA_R       (*((volatile uint32_t *)0x400643FC))
#define GPIO_PORTN_DIR_R        (*((volatile uint32_t *)0x40064400))
#define GPIO_PORTN_DEN_R        (*((volatile uint32_t *)0x4006451C))
#define GPIO_PORTN_DATA_R       (*((volatile uint32_t *)0x400643FC))

#define SYSCTL_RCGCTIMER_R      (*((volatile uint32_t *)0x400FE604))
#define TIMER3_ICR_R            (*((volatile uint32_t *)0x40033024))
#define TIMER3_CTL_R            (*((volatile uint32_t *)0x4003300C))
#define TIMER3_CFG_R            (*((volatile uint32_t *)0x40033000))
#define TIMER3_TAMR_R           (*((volatile uint32_t *)0x40033004))
#define TIMER3_TAILR_R          (*((volatile uint32_t *)0x40033028))
#define TIMER3_TAPR_R           (*((volatile uint32_t *)0x40033038))
#define TIMER3_ICR_R            (*((volatile uint32_t *)0x40033024))
#define TIMER3_IMR_R            (*((volatile uint32_t *)0x40033018))
#define NVIC_EN1_R              (*((volatile uint32_t *)0xE000E104))

#define SYSCTL_RCGCGPIO_R12     0x00001000  // GPIO Port N Run Mode Clock

void complementa0(void){
    GPIO_PORTN_DATA_R ^= 0x01;
}

void Timer03AIntHandler(void){
    //LIMPIA BANDERA
    TIMER3_ICR_R= 0X00000001 ; //LIMPIA BANDERA DE TIMER3
    // llama a la función complemento de leds.
    complementa0();
}

//*********************************************
// CONFIGURACIÓN PARA TIMER DE 16 BITS
 main(void) {
         SYSCTL_RCGCGPIO_R = SYSCTL_RCGCGPIO_R12; // activa reloj puerto N

         SYSCTL_RCGCTIMER_R |= 0X08; //Habilita reloj del TIMER 3
         //retardo para que el reloj alcance el PORTN Y TIMER 3
         while (SYSCTL_PRGPIO_R & 0X1000 == 0){};  // reloj listo?

         TIMER3_CTL_R=0X00000000;       //Deshabilita TIMER
         TIMER3_CFG_R= 0X00000004;      //Configurar para 16 bits   //Escalar para 24
         TIMER3_TAMR_R= 0X00000002;     //Configurar para modo periodico CUENTA HACIA ABAJO
         //TIMER3_TAMR_R= 0X00000012;   //Configurar para modo periodico CUENTA HACIA ARRIBA
         TIMER3_TAILR_R= 0xFFFA;      // Valor de recarga
         //TIMER3_TAILR_R= 0X00000F;    // Valor de recarga
         TIMER3_TAPR_R= 0x19;           // 16 MHZ /16 =1MHz //Cantidad de timer clocks
         TIMER3_ICR_R= 0X00000001 ;     //Limpia posible bandera pendiente de TIMER3
         TIMER3_IMR_R |= 0X00000001;    //Activa interrupción de TIMEOUT
         NVIC_EN1_R= 1<<(35-32);        //Habilita la interrupción de TIMER3
         TIMER3_CTL_R |= 0X00000001;    //Habilita TIMER

         // habilita el bit 0 como digital
         // configura como salida

         GPIO_PORTN_DIR_R = 0x01;
         GPIO_PORTN_DEN_R = 0x01;
         GPIO_PORTN_DATA_R = 0x00;
         while(1);
}
