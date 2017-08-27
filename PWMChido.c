//Bibliotecas a usar.
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "inc/tm4c1294ncpdt.h"
#include "driverlib/timer.h"
#include "driverlib/sysctl.h"


int op=0;
void confPWM_F2(uint32_t T, uint32_t D){
      GPIO_PORTF_AHB_AFSEL_R |= 0x04;
      GPIO_PORTF_AHB_DEN_R |= 0x04;
      GPIO_PORTF_AHB_PCTL_R = (GPIO_PORTF_AHB_PCTL_R&0xFFFFF0FF)+0x00000600;
      GPIO_PORTF_AHB_AMSEL_R &= ~0x04;
      PWM0_CC_R |= PWM_CC_USEPWM;
      PWM0_CC_R &= ~PWM_CC_PWMDIV_M;
      PWM0_CC_R += PWM_CC_PWMDIV_64;
      PWM0_1_CTL_R = 0;
      PWM0_1_GENA_R = (PWM_1_GENA_ACTCMPAD_ONE|PWM_1_GENA_ACTLOAD_ZERO);
      PWM0_1_LOAD_R = T - 1;
      PWM0_1_CMPA_R = D - 1;
      PWM0_1_CTL_R |= PWM_1_CTL_ENABLE;
}

void confPWM_F3(uint32_t T, uint32_t D){
      GPIO_PORTF_AHB_AFSEL_R |= 0x08;
      GPIO_PORTF_AHB_DEN_R |= 0x08;
      GPIO_PORTF_AHB_PCTL_R = (GPIO_PORTF_AHB_PCTL_R&0xFFFF0FFF)+0x00006000;
      GPIO_PORTF_AHB_AMSEL_R &= ~0x08;

      PWM0_CC_R |= PWM_CC_USEPWM;
      PWM0_CC_R &= ~PWM_CC_PWMDIV_M;
      PWM0_CC_R += PWM_CC_PWMDIV_64;

      PWM0_1_CTL_R = 0;
      PWM0_1_GENB_R |= PWM_1_GENB_ACTCMPBD_ONE;
      PWM0_1_GENB_R |= PWM_1_GENB_ACTLOAD_ZERO;
      PWM0_1_LOAD_R = T - 1;
      PWM0_1_CMPB_R = D - 1;
      PWM0_1_CTL_R |= PWM_1_CTL_ENABLE;
}

void timer3_ini(int time){
    TIMER3_CTL_R=0;
    TIMER3_CFG_R=0;
    TIMER3_TAMR_R=0x00000001;
    TIMER3_ICR_R=1;
    TIMER3_TAPR_R=0;
    TIMER3_IMR_R=0;             //Enmascara al TIMER
    TIMER3_TAILR_R=time-1;      //Cuenta máx
    TIMER3_CTL_R=1;
}

void GPIOPortE_Handler(void){
    timer3_ini(8000000);
    TIMER3_IMR_R|=1;
    GPIO_PORTE_AHB_ICR_R=0x3;
}

void confInicial(){
    SYSCTL_RCGCGPIO_R = SYSCTL_RCGCGPIO_R5+SYSCTL_RCGCGPIO_R8+SYSCTL_RCGCGPIO_R12+SYSCTL_RCGCGPIO_R3+SYSCTL_RCGCGPIO_R4;
    while((SYSCTL_PRGPIO_R& 0x1138) == 0){};   //Espera hasta que los relojes de GPIO´s estén activados.
    //Leds y botones del Tiva.

    GPIO_PORTJ_AHB_DIR_R &= ~0x03;       //PJ0 dirección entrada - boton SW1
    GPIO_PORTJ_AHB_DEN_R |= 0x03;        //PJ0 se habilita
    GPIO_PORTJ_AHB_PUR_R |= 0x03;        //Habilitación de resistores de pull-up en PJ0 yPJ1.
    GPIO_PORTJ_AHB_IS_R &= ~0x03;        //PJ0 y PJ1 detectados por flancos.
    GPIO_PORTJ_AHB_IBE_R &= ~0x03;       //PJ0 y PJ1 no detectados por ambos flancos.
    GPIO_PORTJ_AHB_IEV_R &= ~0x03;       //PJ0 y PJ1 detectados por flanco de bajada.
    GPIO_PORTJ_AHB_ICR_R = 0x03;         //Se limpian las banderas de reconocimiento de interrupciones en puertos PJ0 y PJ1.
    GPIO_PORTJ_AHB_IM_R |= 0x03;         //Habilitación de interrupciones de puerto PJ1.

    GPIO_PORTE_AHB_DIR_R = 0x00;       // (c) PK dirección entrada - F--> leds de salida E---> boton SW de reset FE = 1111 1110
    GPIO_PORTE_AHB_DEN_R |= 0x01;       //Para habilitar todos los puertos de K
    GPIO_PORTE_AHB_PUR_R |= 0x01;        //     habilita weak pull-up on PK0
    GPIO_PORTE_AHB_IS_R = 0x00;        // (d) PK0 es sensible por nivel
    GPIO_PORTE_AHB_IBE_R &= ~0x03;       //     PK0 no es sensible a dos flancos
    GPIO_PORTE_AHB_IEV_R &= ~0x03;       //     PK0 detecta eventos de flanco de bajada
    GPIO_PORTE_AHB_ICR_R = 0x03;         // (e) limpia la bandera 0
    GPIO_PORTE_AHB_IM_R |= 0x01;                 // (f) Se desenmascara la interrupcion PK0 y se envia al controlador de interrupciones

    SYSCTL_RCGCTIMER_R|=0x0C;   //Para activar el reloj del Timer3
    while(SYSCTL_RCGCTIMER_R!=0xC){};

    SYSCTL_RCGCPWM_R |= SYSCTL_RCGCPWM_R0;
    while((SYSCTL_PRPWM_R & SYSCTL_PRPWM_R0) == 0){};
    //Pag. 116 para ver el número de instrucción, y 146 para elegir que registros ENE y PRI corresponden a la interrupcción a manejar.

    NVIC_PRI1_R = (NVIC_PRI1_R & 0xFFFFFF0F) | 0x00000000; // (g) prioridad 1

    NVIC_EN0_R = 1 << (4); //Habilitamos la interrupción 72 de NVIC    Puerto M

    NVIC_PRI5_R&=0x00FFFFFF;
    NVIC_PRI8_R&= 0x00FFFFFF;
    NVIC_PRI1_R&= 0xFFFFFF0F;
    NVIC_EN1_R=1<<(35-32);    //Habilitamos la interrupción 35 de NVIC,   TIMER
    NVIC_PRI12_R = (NVIC_PRI12_R&0x00FFFFFF)|0x60000000; //Nivel 3 de prioridad de interrupción en GPIOJ.
    NVIC_EN1_R |= 1<<(51-32);           //Habilitación de la interrupción por GPIOJ (51) en NVIC.
    NVIC_EN0_R |= 1 << 4;
    NVIC_EN0_R |= 1 << 23;
    GPIO_PORTN_DIR_R = 0x03;        GPIO_PORTN_DEN_R = 0x03;        GPIO_PORTN_DATA_R = 0x00;               //Leds PN0, PN1.
    GPIO_PORTF_AHB_DIR_R = 0x11;    GPIO_PORTF_AHB_DEN_R = 0x11;    GPIO_PORTF_AHB_DATA_R = 0x00;           //Leds PF0, PF4.
}
//Pinza cerrada es -90
void esperarObjeto(void){
    PWM0_1_CMPB_R = 468.5;
    PWM0_1_CMPA_R = 468.5+90*(625-312)/180.;
    PWM0_ENABLE_R =0xC;
}

void timer3Handler(void){
    if(op==0){
        TIMER3_IMR_R=0;
        PWM0_1_CMPA_R = 468.5-90*(625-312)/180.;
        timer3_ini(20000000);
        TIMER3_IMR_R|=1;
        op++;
    }
    else if(op==1){
        TIMER3_IMR_R=0;
        PWM0_1_CMPB_R = 468.5-90*(625-312)/180.;
        timer3_ini(20000000);
        TIMER3_IMR_R|=1;
        op++;
    }
    else if(op==2){
        TIMER3_IMR_R=0;
        PWM0_1_CMPA_R = 468.5+90*(625-312)/180.;
        timer3_ini(20000000);
        TIMER3_IMR_R|=1;
        op++;
    }
    else if(op==3){
        PWM0_1_CMPB_R = 468.5+90*(625-312)/180.;
        TIMER3_IMR_R=0;
        op=0;
        GPIO_PORTE_AHB_ICR_R=0x3;
    }
}

int main(void){
    uint32_t cta=0, relojPrincipal;
    relojPrincipal=SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL |SYSCTL_CFG_VCO_480), 20000000);
    confInicial();
    GPIO_PORTE_AHB_ICR_R=0x3;
    confPWM_F2(6250,468.5);
    confPWM_F3(6250,468.5);
    esperarObjeto();
    GPIO_PORTE_AHB_ICR_R=0x3;
  while(1){}
}
