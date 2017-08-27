// Programa que configura al UART
//
// U7Rx (receptor) conectado a PC4
// U7Tx (transmisor) conectado a PC5

#include <stdint.h>

#define GPIO_PORTC_AFSEL_R      (*((volatile uint32_t *)0x4005A420))
#define GPIO_PORTC_DEN_R        (*((volatile uint32_t *)0x4005A51C))
#define GPIO_PORTC_AMSEL_R      (*((volatile uint32_t *)0x4005A528))
#define GPIO_PORTC_PCTL_R       (*((volatile uint32_t *)0x4005A52C))

#define UART7_DR_R              (*((volatile uint32_t *)0x40013000))
#define UART7_FR_R              (*((volatile uint32_t *)0x40013018))

#define UART_FR_TXFF            0x00000020  // FIFO TX UART llena
#define UART_FR_RXFE            0x00000010  // FIFO RX UART vacia
#define UART7_IBRD_R            (*((volatile uint32_t *)0x40013024))
#define UART7_FBRD_R            (*((volatile uint32_t *)0x40013028))
#define UART7_LCRH_R            (*((volatile uint32_t *)0x4001302C))

#define UART_LCRH_WLEN_8        0x00000060  // palabra de 8 bits
#define UART_LCRH_FEN           0x00000010  // Habilitación de la FIFO de la UART

#define UART7_CTL_R             (*((volatile uint32_t *)0x40013030))
#define UART7_CC_R              (*((volatile uint32_t *)0x40013FC8))
#define UART_CC_CS_M            0x0000000F  // UART Baud Clock Source
#define UART_CC_CS_SYSCLK       0x00000000  // System clock (based on clock
                                            // source and divisor factor)
#define UART_CC_CS_PIOSC        0x00000005  // PIOSC
#define SYSCTL_ALTCLKCFG_R      (*((volatile uint32_t *)0x400FE138))
#define SYSCTL_ALTCLKCFG_ALTCLK_M                                             \
                                0x0000000F  // Fuente alternativa de reloj
#define SYSCTL_ALTCLKCFG_ALTCLK_PIOSC                                         \
                                0x00000000  // PIOSC
#define SYSCTL_RCGCGPIO_R       (*((volatile uint32_t *)0x400FE608))
                                                     // Gating Control
#define SYSCTL_RCGCUART_R       (*((volatile uint32_t *)0x400FE618))
                                            // Gating Control
#define SYSCTL_PRGPIO_R         (*((volatile uint32_t *)0x400FEA08))
#define SYSCTL_PRGPIO_R0        0x00000004  // GPIO Port C Peripheral Ready
#define SYSCTL_PRUART_R         (*((volatile uint32_t *)0x400FEA18))
#define SYSCTL_PRUART_R0        0x00000080  // UART Module 7 Peripheral Ready

//------------Inicialización del UART------------
// Initializa UART7 para 115,200 bauds (reloj de 16 MHz del PIOSC),
// palabra de 8 bit, no hay bits de paridad, un stop bit, los FIFOs habilitados,
// no hay interrupciones

void main(void){
                                        // activa el reloj para el UART7
  SYSCTL_RCGCUART_R |=  0x00000080;
                                        // activate el reloj para el Puerto C
  SYSCTL_RCGCGPIO_R |= 0x00000004;
                                        // Se espera a que el reloj se estabilice
  while((SYSCTL_PRUART_R&SYSCTL_PRUART_R0) == 0){};
  UART7_CTL_R &= ~0x00000001;      // se deshabilita el UART
  UART7_IBRD_R = 8;                     // IBRD = int(16,000,000 / (16 * 115,200)) = int(8.681)
  UART7_FBRD_R = 44;                    // FBRD = round(0.6806 * 64) = 44
                                        // Palabra de 8 bits (sin bits de paridad, un bit stop, FIFOs)
  UART7_LCRH_R = (UART_LCRH_WLEN_8|UART_LCRH_FEN);
                                        // UART toma su reloj del la fuente alterna como se define por SYSCTL_ALTCLKCFG_R
  UART7_CC_R = (UART7_CC_R&~UART_CC_CS_M)+UART_CC_CS_PIOSC;
                                        // la fuente de reloj alterna es el PIOSC (default)
  SYSCTL_ALTCLKCFG_R = (SYSCTL_ALTCLKCFG_R&~SYSCTL_ALTCLKCFG_ALTCLK_M)+SYSCTL_ALTCLKCFG_ALTCLK_PIOSC;

  UART7_CTL_R &= ~0x00000020;         // alta velocidad deshabilitada;divide el reloj por 16 en lugar de 8 (default)
  UART7_CTL_R |= 0x00000001;       // habilita el UART
                                        // Se espera a que el reloj se estabilice
  while((SYSCTL_PRGPIO_R&SYSCTL_PRGPIO_R0) == 0){};
  GPIO_PORTC_AFSEL_R |= 0x30;           // habilita funcion alterna en PC5-4
  GPIO_PORTC_DEN_R |= 0x30;             // habilita digital I/O en PC5-4
                                        // configura PC5-4 como UART
  GPIO_PORTC_PCTL_R = (GPIO_PORTC_PCTL_R&0xFF00FFFF)+0x00110000;
  GPIO_PORTC_AMSEL_R &= ~0x30;          // deshabilita la funcionabilidad analogica de PC

  uint32_t data[0];

  while(1){
 while((UART7_FR_R&UART_FR_TXFF) != 0); // Se espera a que el FIFO TX este vacio
   UART7_DR_R = 0x00;
   UART7_DR_R = 0x01;
   UART7_DR_R = 0x02;
   UART7_DR_R = 0x03;
   UART7_DR_R = 0x04;
   UART7_DR_R = 0x05;

    while((UART7_FR_R&UART_FR_RXFE) != 0); // Se espera a que el FIFO RX tenga un valor
     data[0] = UART7_DR_R&0xFF;;// se envía el primer valor
     while((UART7_FR_R&UART_FR_RXFE) != 0); //Se espera a que el FIFO RX tenga un valor
     data[1] = UART7_DR_R&0xFF;// se envía el segundo valor
     while((UART7_FR_R&UART_FR_RXFE) != 0); //Se espera a que el FIFO RX tenga un valor
     data[2] = UART7_DR_R&0xFF;// se envía el tercer valor
     while((UART7_FR_R&UART_FR_RXFE) != 0); //Se espera a que el FIFO RX tenga un valor
     data[3] = UART7_DR_R&0xFF;// se envía el cuarto valor
     while((UART7_FR_R&UART_FR_RXFE) != 0); //Se espera a que el FIFO RX tenga un valor
     data[4] = UART7_DR_R&0xFF;// se envía el quinto valor
     while((UART7_FR_R&UART_FR_RXFE) != 0); //Se espera a que el FIFO RX tenga un valor
     data[5] = UART7_DR_R&0xFF;// se envía el sexto valor
     }
}
