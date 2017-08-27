//Control del ADC de la tarjeta TIVA, hecho por Pablo Velázquez, 3 de mayo 2017
//ICN

#include<stdbool.h>
#include<stdint.h>

#define GPIOCLK         (*((volatile uint32_t *)0x400FE608)) //Registro para habilitar reloj de GPIO
#define ADCCLK          (*((volatile uint32_t *)0x400FE638)) // Registro para habilitar el reloj al ADC(p. 396)

#define PortEData       *(volatile uint32_t *)  0x4005C040   //Datos del Puerto E
#define PortEDir        *(volatile uint32_t *)  0x4005C400   //Dirección del Puerto E
#define PortEAFSel      *(volatile uint32_t *)  0x4005C420   //Alternate Function Select
#define PortEDEn        *(volatile uint32_t *)  0x4005C51C   //Habilitación digital del puerto E
#define PortEDAMSel     *(volatile uint32_t *)  0x4005C528   //Analog Function Select

#define PortKData       *(volatile uint32_t *)  0x400613FC   //Datos del Puerto E
#define PortKDir        *(volatile uint32_t *)  0x40061400   //Dirección del Puerto E
#define PortKDEn        *(volatile uint32_t *)  0x4006151C   //Habilitación digital del puerto E

#define ADC0ActSS       (*((volatile uint32_t *)0x40038000)) // Activa los secuenciadores, y el acceso a memoria (DMA)
#define ADC0PeriphConf  (*((volatile uint32_t *)0x40038FC4)) // Configuración de la tasa de muestreo
#define ADC0SSPRI       (*((volatile uint32_t *)0x40038020)) // Prioridad del secuenciador
#define ADC0EMUX        (*((volatile uint32_t *)0x40038014)) // Seleccionar el trigger
#define ADC0SSEMUX3     (*((volatile uint32_t *)0x400380B8)) // Selector de las entradas AIN[19:16] o AIN[15:0]
#define ADC0SSMUX3      (*((volatile uint32_t *)0x400380A0)) // Configuración de la entrada analógica para el Secuenciador 3
#define ADC0SSCtl3      (*((volatile uint32_t *)0x400380A4)) // Configuración de la muestra ejecutada con el Secuenciador 3
#define ADC0IM          (*((volatile uint32_t *)0x40038008)) // Máscara de interrupciones en secuenciadores
#define ADC0ISC         (*((volatile uint32_t *)0x4003800C)) // Status y clear de las condiciones de interrupción del secuenciador
#define ADC0PSSI        (*((volatile uint32_t *)0x40038028)) // Inicio del muestreo en los secuenciadores
#define ADC0RIS         (*((volatile uint32_t *)0x40038004)) // Estado de la señal de interrupción de cada secuenciador
#define ADC0SSFIFO3     (*((volatile uint32_t *)0x400380A8)) // Resultados de Conversión

#define PLLFreq0       (*((volatile uint32_t *)0x400FE160))  // Configuración del PLL
#define PLLState        (*((volatile uint32_t *)0x400FE168)) // Estado de encendido del PLL

void PortInit(void){
    GPIOCLK=0x210;           //Habilitamos el CLK del puerto E y K
    while(GPIOCLK!=0x210){}; //Esperamos a que el CLK esté listo
    PortEDir    &= ~0x10;   //Habilitamos el pin E4 como entrada
    PortEAFSel |= 0x10;     //Selecciónamos Función Alterna del puerto E
    PortEDEn    &= ~0x10;   //Deshabilitamos las funciones digitales del pin E4
    PortEDAMSel |= 0x10;    //Habilitamos las funciones analógicas del pin E4;

    PortKDir    |=0xFF;
    PortKDEn    |=0xFF;

    ADCCLK|=1;              //Habilitamos el reloj del ADC
    ADC0PeriphConf=1;       //Configuramos para que muestree a 125Ksamp/s
    ADC0SSPRI   =  0x0123;  //Prioridad 0,1,2 y 3 para los respectivos secuenciadores
    ADC0ActSS   &= ~0x0008; //Deshabilitamos el secuenciador 3
    ADC0SSMUX3  =(ADC0SSMUX3 & 0xFFFFFFF0)+9;   //Habilitamos el Canal AIN9, pin E4
    ADC0SSCtl3  =0x6;       //Deshabilitamos sensor de temperatura, habilitamos INR3,
                            //habilitamos fin de sec y muestreo no diferencial
    ADC0IM      &= ~0x0008; //Enmascaramos al ADC
    ADC0ActSS   |= 0x0008;  //Habilitamos el secuenciador

    PLLFreq0 |= 0x00800000; // Encendemos el PLL
    while(PLLState!=0x01);  // Esperamos a que el PLL fije su frecuencia
    PLLFreq0 &= ~0x00800000;// Apagamos el PLL
}
float Volt;//Variable que va de 0 a 3.3 V
int Bits;
void main(void) {
    PortInit();     //Inicializamos puertos
    while(1){
        ADC0PSSI=0x0008;    //Habilita por software el inicio del muestreo
        while(ADC0RIS!=0x08){};//Esperemos a que termine la conversión
        Bits=ADC0SSFIFO3;
        Volt=3.3*(ADC0SSFIFO3 & 0xFFF)/4096.;    //Calculamos el voltaje analógico correspondiente
        PortKData=(ADC0SSFIFO3 & 0xFF0)/16;
        ADC0ISC=0x0008;     //Limpiamos la bandera de la conversión
    }
}
