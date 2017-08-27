//Bibliotecas a usar.
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "inc/tm4c1294ncpdt.h"
#include "driverlib/timer.h"
#include "driverlib/sysctl.h"

volatile uint32_t pulsoJ0 = 0;
volatile uint32_t pulsoJ1 = 0;

int cont=0;

void confPWM_F1(uint32_t T,uint32_t D){
//    SYSCTL_RCGCPWM_R |= SYSCTL_RCGCPWM_R0;
    GPIO_PORTF_AHB_DIR_R=0x02;
    GPIO_PORTF_AHB_AFSEL_R|=0x02;
    GPIO_PORTF_AHB_DEN_R|=0x02;
    GPIO_PORTF_AHB_PCTL_R = (GPIO_PORTF_AHB_PCTL_R & 0xFFFFFF0F)+0x00000060;
    GPIO_PORTF_AHB_AMSEL_R &= ~0x02;
//    while((SYSCTL_PRPWM_R & SYSCTL_PRPWM_R0) == 0){};
    PWM0_CC_R |= PWM_CC_USEPWM;
    PWM0_CC_R &= ~PWM_CC_PWMDIV_M;
    PWM0_CC_R += PWM_CC_PWMDIV_64;
    PWM0_0_CTL_R = 0;
    PWM0_0_GENB_R = (PWM_0_GENB_ACTCMPAD_ONE|PWM_0_GENB_ACTLOAD_ZERO);
    PWM0_0_LOAD_R = T - 1;//Solo es necesario cambiar este
    PWM0_0_CMPB_R = D - 1;

    PWM0_0_CTL_R |= PWM_0_CTL_ENABLE;//Y este
    //PWM0_1_CTL_R &= ~PWM_1_CTL_ENABLE;//Y este
    //PWM0_2_CTL_R &= ~PWM_2_CTL_ENABLE;//Y este
    //PWM0_3_CTL_R &= ~PWM_3_CTL_ENABLE;//Y este

    PWM0_ENABLE_R = PWM_ENABLE_PWM0EN;
}

void confPWM_F2(uint32_t T, uint32_t D){
    //SYSCTL_RCGCPWM_R |= SYSCTL_RCGCPWM_R0;
    GPIO_PORTF_AHB_DIR_R=0x04;
    GPIO_PORTF_AHB_AFSEL_R |= 0x04;
    GPIO_PORTF_AHB_DEN_R |= 0x04;
    GPIO_PORTF_AHB_PCTL_R = (GPIO_PORTF_AHB_PCTL_R & 0xFFFFFFF0)+0x00000600;
    GPIO_PORTF_AHB_AMSEL_R &= ~0x04;
    //while((SYSCTL_PRPWM_R & SYSCTL_PRPWM_R0) == 0){};
    PWM0_CC_R |= PWM_CC_USEPWM;
    PWM0_CC_R &= ~PWM_CC_PWMDIV_M;
    PWM0_CC_R += PWM_CC_PWMDIV_64;
    PWM0_1_CTL_R = 0;
    PWM0_1_GENA_R = (PWM_1_GENA_ACTCMPAD_ONE|PWM_1_GENA_ACTLOAD_ZERO);
    PWM0_1_LOAD_R = T - 1;//Solo es necesario cambiar este
    PWM0_1_CMPA_R = D - 1;
    //PWM0_0_CTL_R &= ~PWM_0_CTL_ENABLE;//Y este
    PWM0_1_CTL_R |= PWM_1_CTL_ENABLE;//Y este
    //PWM0_2_CTL_R &= ~PWM_2_CTL_ENABLE;//Y este
    //PWM0_3_CTL_R &= ~PWM_3_CTL_ENABLE;//Y este

    PWM0_ENABLE_R = PWM_ENABLE_PWM2EN;
}

void confPWM_G0(uint32_t T, uint32_t D){                // Procedimiento de configuración con base al ejemplo de la página 1678 de la hoja técnica.
      //SYSCTL_RCGCPWM_R |= SYSCTL_RCGCPWM_R0;            // 1) Activación del reloj del módulo PWM0.
    GPIO_PORTG_AHB_DIR_R=0x01;
      GPIO_PORTG_AHB_AFSEL_R |= 0x01;          //jj         // 3) Habilitación de función alterna del bit 3 del puerto F.
      GPIO_PORTG_AHB_DEN_R |= 0x01;                     // Habilitación del bit 3 del puerto F como bit de salida digital.
      GPIO_PORTG_AHB_PCTL_R = (GPIO_PORTG_AHB_PCTL_R & 0xFFFFFFF0)+0x00000006;//4) Configuración de PF3 como MOPPWM3.
      GPIO_PORTG_AHB_AMSEL_R &= ~0x01;                  // Deshabilitar PF3 como salida analógica.
      //while((SYSCTL_PRPWM_R & SYSCTL_PRPWM_R0) == 0){}; // Esperar a que se active reloj del módulo PWM0.
                                                        // 5) Configuración de división de frecuencia al PWM0.
      PWM0_CC_R |= PWM_CC_USEPWM;                       // a) En el registro PWM_O_CC, habilitar uso de divisor de frecuencia del PWM0.
      PWM0_CC_R &= ~PWM_CC_PWMDIV_M;                    // b) Limpiar el bit 0x07 del registro PWM_O_CC, para ignorar cualquier otra división de frecuencia anterior.
      PWM0_CC_R += PWM_CC_PWMDIV_64;                    // c) Configurar divisor entre 2.
                                                        // 6) Configuración del generador del PW0 para cuenta descendente.
      PWM0_2_CTL_R  = 0;                                // a) Escribir cero en el registro PWM0CTL.
      PWM0_2_GENA_R |= PWM_2_GENA_ACTCMPBD_ONE;         // b) PF3 en alto cuando la cuenta sea lgual al comparador B.
      PWM0_2_GENA_R |= PWM_2_GENA_ACTLOAD_ZERO;         // c) PF3 en bajo cuando se alcance a la cuenta ingresada en LOAD.
      PWM0_2_LOAD_R = T - 1;                            // 7) Cuentas necesarias para llegar a cero. Estas equivalen al periodo de la señal.
      PWM0_2_CMPA_R = D - 1;                            // 8) Cuentas necesarias para hacer la comparación. Estas equivalen al ciclo de trabajo de la señal.

      //PWM0_0_CTL_R &= ~PWM_0_CTL_ENABLE;//Y este
      //PWM0_1_CTL_R &= ~PWM_1_CTL_ENABLE;//Y este
      PWM0_2_CTL_R |= PWM_2_CTL_ENABLE;//Y este
      //PWM0_3_CTL_R &= ~PWM_3_CTL_ENABLE;//Y este              // 9) Habilitación del generador del PWM1.
      //PWM0_ENABLE_R = PWM_ENABLE_PWM3EN;              // 10) Habilitación de la salida de la señal en MOPPWM3.
}

void confPWM_K4(uint32_t T, uint32_t D){                // Procedimiento de configuración con base al ejemplo de la página 1678 de la hoja técnica.
      //SYSCTL_RCGCPWM_R |= SYSCTL_RCGCPWM_R0;            // 1) Activación del reloj del módulo PWM0.
    GPIO_PORTK_DIR_R=0x10;
      GPIO_PORTK_AFSEL_R |= 0x10;                   // 3) Habilitación de función alterna del bit 3 del puerto F.
      GPIO_PORTK_DEN_R |= 0x10;                     // Habilitación del bit 3 del puerto F como bit de salida digital.
      GPIO_PORTK_PCTL_R = (GPIO_PORTK_PCTL_R & 0xFFF0FFFF)+0x000600000;//4) Configuración de PF3 como MOPPWM3.
      GPIO_PORTK_AMSEL_R &= ~0x10;                  // Deshabilitar PF3 como salida analógica.
      //while((SYSCTL_PRPWM_R & SYSCTL_PRPWM_R0) == 0){}; // Esperar a que se active reloj del módulo PWM0.
                                                        // 5) Configuración de división de frecuencia al PWM0.
      PWM0_CC_R |= PWM_CC_USEPWM;                       // a) En el registro PWM_O_CC, habilitar uso de divisor de frecuencia del PWM0.
      PWM0_CC_R &= ~PWM_CC_PWMDIV_M;                    // b) Limpiar el bit 0x07 del registro PWM_O_CC, para ignorar cualquier otra división de frecuencia anterior.
      PWM0_CC_R += PWM_CC_PWMDIV_64;                    // c) Configurar divisor entre 2.
                                                        // 6) Configuración del generador del PW0 para cuenta descendente.
      PWM0_3_CTL_R  = 0;                                // a) Escribir cero en el registro PWM0CTL.
      PWM0_3_GENA_R |= PWM_3_GENA_ACTCMPBD_ONE;         // b) PF3 en alto cuando la cuenta sea lgual al comparador B.
      PWM0_3_GENA_R |= PWM_3_GENA_ACTLOAD_ZERO;         // c) PF3 en bajo cuando se alcance a la cuenta ingresada en LOAD.
      PWM0_3_LOAD_R = T - 1;                            // 7) Cuentas necesarias para llegar a cero. Estas equivalen al periodo de la señal.
      PWM0_3_CMPA_R = D - 1;                            // 8) Cuentas necesarias para hacer la comparación. Estas equivalen al ciclo de trabajo de la señal.
      //PWM0_0_CTL_R &= ~PWM_0_CTL_ENABLE;//Y este
      //PWM0_1_CTL_R &= ~PWM_1_CTL_ENABLE;//Y este
      //PWM0_2_CTL_R &= ~PWM_2_CTL_ENABLE;//Y este
      PWM0_3_CTL_R |=  PWM_3_CTL_ENABLE;//Y este    // 9) Habilitación del generador del PWM1.
      //PWM0_ENABLE_R = PWM_ENABLE_PWM3EN;              // 10) Habilitación de la salida de la señal en MOPPWM3.
}

void angulon(int ang,int motor){
    if(ang>=-90 & ang<=90)
        if (motor==1){
            //confPWM_F1(6250,468.5+ang*(625-312)/180.);
            PWM0_0_LOAD_R = 6250 - 1;
            PWM0_0_CMPA_R = (468.5+ang*(625-312)/180.) - 1;
            PWM0_ENABLE_R |= PWM_ENABLE_PWM1EN;
        }
        else if (motor==2){
            //confPWM_F2(6250,468.5+ang*(625-312)/180.);
            PWM0_1_LOAD_R = 6250 - 1;
            PWM0_1_CMPA_R = (468.5+ang*(625-312)/180.) - 1;
            PWM0_ENABLE_R |= PWM_ENABLE_PWM2EN;
        }
        else if (motor==3){
            //confPWM_G0(6250,468.5+ang*(625-312)/180.);
            PWM0_2_LOAD_R = 6250 - 1;
            PWM0_2_CMPA_R = (468.5+ang*(625-312)/180.) - 1;
            PWM0_ENABLE_R |= PWM_ENABLE_PWM4EN;
        }
        else if (motor==4){
            //confPWM_K4(6250,468.5+ang*(625-312)/180.);
            PWM0_3_LOAD_R = 6250 - 1;
            PWM0_3_CMPA_R = (468.5+ang*(625-312)/180.) - 1;
            PWM0_ENABLE_R |= PWM_ENABLE_PWM6EN;
        }
}

int duty;
bool ida=0;
bool abrir=0;
int ang;

void object(void){
    int i;
    for(i=0;i<100;i++){
        angulon(0,1);
        PWM0_ENABLE_R = PWM_ENABLE_PWM2EN;
        angulon(90,2);
        PWM0_ENABLE_R |= PWM_ENABLE_PWM3EN;
        SysCtlDelay(700000);
    }
}

void manejadorGPIOJ(void){
    if(GPIO_PORTJ_AHB_RIS_R==2){
        if (!abrir)
            ang=90;
        else
            ang=-90;
        abrir=!abrir;
        /*angulon(ang,1);
        angulon(ang,2);
        angulon(ang,3);
        angulon(ang,4);*/
    }
    if(GPIO_PORTJ_AHB_DATA_R==2)
        object();
    GPIO_PORTJ_AHB_ICR_R = 0x03;
    //SysCtlDelay(800000);
    /*if(GPIO_PORTJ_AHB_RIS_R & 0x01){        //Interrupción por PJ0.
        GPIO_PORTJ_AHB_ICR_R = 0x01;        //Se limpia la bandera de reconocimiento de interrupción.
        pulsoJ0++;
        GPIO_PORTF_AHB_DATA_R = 0x01;       //Encender led PF0.
        PWM0_ENABLE_R = PWM_ENABLE_PWM2EN;  //Habilitar salida de PWM por PF2, deshabilitar salida por PF3.
    }
    if(GPIO_PORTJ_AHB_RIS_R & 0x02){//Interrupción por PJ1.
        GPIO_PORTJ_AHB_ICR_R = 0x02;  //Se limpia la bandera de reconocimiento de interrupción.
        pulsoJ1++;
        GPIO_PORTF_AHB_DATA_R = 0x10;       //Encender led PF1.
        PWM0_ENABLE_R = PWM_ENABLE_PWM3EN;  //Habilitar salida de PWM por PF3, deshabilitar salida por PF2.
    }*/
}

void confInicial(){
    SYSCTL_RCGCGPIO_R = SYSCTL_RCGCGPIO_R5+SYSCTL_RCGCGPIO_R8+SYSCTL_RCGCGPIO_R12+SYSCTL_RCGCGPIO_R9+SYSCTL_RCGCGPIO_R6;
    while(SYSCTL_PRGPIO_R!= 0x1360){};   //Espera hasta que los relojes de GPIO´s estén activados.
    //Leds y botones del Tiva.
    GPIO_PORTJ_AHB_DIR_R &= ~0x03;       //PJ0 dirección entrada - boton SW1
    GPIO_PORTJ_AHB_DEN_R |= 0x03;        //PJ0 se habilita
    GPIO_PORTJ_AHB_PUR_R |= 0x03;        //Habilitación de resistores de pull-up en PJ0 yPJ1.
    GPIO_PORTJ_AHB_IS_R &= ~0x03;        //PJ0 y PJ1 detectados por flancos.
    GPIO_PORTJ_AHB_IBE_R &= ~0x03;       //PJ0 y PJ1 no detectados por ambos flancos.
    GPIO_PORTJ_AHB_IEV_R &= ~0x03;       //PJ0 y PJ1 detectados por flanco de bajada.
    GPIO_PORTJ_AHB_ICR_R = 0x03;         //Se limpian las banderas de reconocimiento de interrupciones en puertos PJ0 y PJ1.
    GPIO_PORTJ_AHB_IM_R |= 0x03;         //Habilitación de interrupciones de puerto PJ1.
    //Pag. 116 para ver el número de instrucción, y 146 para elegir que registros ENE y PRI corresponden a la interrupcción a manejar.
    NVIC_PRI12_R = (NVIC_PRI12_R & 0x00FFFFFF)|0x60000000; //Nivel 3 de prioridad de interrupción en GPIOJ.
    NVIC_EN1_R |= 1<<(51-32);           //Habilitación de la interrupción por GPIOJ (51) en NVIC.
    GPIO_PORTN_DIR_R = 0x03;        GPIO_PORTN_DEN_R = 0x03;        GPIO_PORTN_DATA_R = 0x00;               //Leds PN0, PN1.
    GPIO_PORTF_AHB_DIR_R = 0x11;    GPIO_PORTF_AHB_DEN_R = 0x11;    GPIO_PORTF_AHB_DATA_R = 0x00;           //Leds PF0, PF4.
    SYSCTL_RCGCPWM_R |= SYSCTL_RCGCPWM_R0;
    while((SYSCTL_PRPWM_R & SYSCTL_PRPWM_R0) == 0){};
}

void voltear(void){
    int i;
    for(i=0;i<100;i++){
        angulon(ang,1);
        PWM0_ENABLE_R = PWM_ENABLE_PWM2EN;
        angulon(ang,2);
        PWM0_ENABLE_R |= PWM_ENABLE_PWM3EN;
        SysCtlDelay(700000);
    }
}

int main(void){
    uint32_t  relojPrincipal=SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL |SYSCTL_CFG_VCO_480), 20000000);//20 MHz/64=312.5 KHz
    confInicial();
    GPIO_PORTJ_AHB_ICR_R = 0x03;
    angulon(ang,1);
    angulon(ang,2);
    angulon(ang,3);
    angulon(ang,4);
    while(1){
        //angulon(ang,1);
        //SysCtlDelay(800000);

  //      SysCtlDelay(600000);
/*
      if (cont==90)
          ida=1;
      else if (cont==-90)
          ida=0;

      if (ida==0)
          cont++;
      else
          cont--;

      angulon(cont);

*/
}

}
