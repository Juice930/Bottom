#define GPIO_PORTN_DATA_BITS_R  0x40064000
#define GPIO_PORTN_DIR_R        (*((volatile unsigned int *)0x40064400))
#define GPIO_PORTN_DATA_R       (*((volatile unsigned int*)0x400643FC))
#define GPIO_PORTN_DEN_R        (*((volatile unsigned int *)0x4006451C))
#define PORTbit1 0x08
#define PORTbit0 0x04
#define PORTNbits (GPIO_PORTN_DATA_BITS_R|PORTbit1|PORTbit0)
#define PN10 (*(volatile unsigned int *)PORTNbits)   //#define PF21 (*(volatile uint32)0x4005D044)

#define GPIO_PORTJ_DATA_BITS_R  0x4006000C
#define GPIO_PORTJ_DIR_R        (*(volatile unsigned int *)0x40060400)
#define GPIO_PORTJ_DEN_R        (*(volatile unsigned int *)0x4006051C)
#define GPIO_PORTJ_PUR_R        (*(volatile unsigned int *)0x40060510)
#define PORTbit1 0x08
#define PORTbit0 0x04
#define PORTNbits (GPIO_PORTN_DATA_BITS_R|PORTbit1|PORTbit0)
#define PJ0  (*(volatile unsigned int *)PORTNbits)

#define SYSCTL_RCGCGPIO_R  (*(volatile unsigned int *)0x400FE608)
#define SYSCTL_PRGPIO_R    (*(volatile unsigned int *)0x400FEA08)
//#define GPIO_PORTN_DATA_J   *((volatile unsigned long *)0x4006000C) //Registro de Datos J
void main(void){
    // Habilita reloj en Puerto N y J
    SYSCTL_RCGCGPIO_R |=0x1120 ;   //(a)

    // Espera a que esté listo el puerto
    while((SYSCTL_PRGPIO_R & 0x1120)==0);

    GPIO_PORTN_DIR_R |=  0x03;
    GPIO_PORTN_DEN_R |=  0xFF;

    GPIO_PORTJ_DIR_R |=  0x00;
    GPIO_PORTJ_PUR_R |=  0x03;
    GPIO_PORTJ_DEN_R |=  0xFF;

    // valor inicial de los pines (hay Leds conectados en la tarjeta)
    PN10 = 0x00;    // PN1 = 0, PN0 = 0

    while(1)
        if(PJ0!=3)//!PJ0 es para quemientras esté apachurrado
            PN10 = PJ0;
}
