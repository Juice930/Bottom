#include <stdbool.h>
#include <stdint.h>

#define SYSCTL_RCGCGPIO_R       (*((volatile uint32_t *)0x400FE608))
#define SYSCTL_PRGPIO_R         (*((volatile uint32_t *)0x400FEA08))
#define SYSCTL_RCGCADC_R        (*((volatile uint32_t *)0x400FE638))
#define SYSCTL_PRADC_R          (*((volatile uint32_t *)0x400FEA38))

#define ADC0_PC_R               (*((volatile uint32_t *)0x40038FC4))
#define ADC0_SSPRI_R            (*((volatile uint32_t *)0x40038020))
#define ADC0_ACTSS_R            (*((volatile uint32_t *)0x40038000))
#define ADC0_EMUX_R             (*((volatile uint32_t *)0x40038014))
#define ADC0_SSEMUX3_R          (*((volatile uint32_t *)0x400380B8))
#define ADC0_SSMUX3_R           (*((volatile uint32_t *)0x400380A0))
#define ADC0_SSCTL3_R           (*((volatile uint32_t *)0x400380A4))
#define ADC0_IM_R               (*((volatile uint32_t *)0x40038008))
#define ADC0_ACTSS_R            (*((volatile uint32_t *)0x40038000))
#define ADC0_ISC_R              (*((volatile uint32_t *)0x4003800C))
#define ADC0_PSSI_R             (*((volatile uint32_t *)0x40038028))
#define ADC0_RIS_R              (*((volatile uint32_t *)0x40038004))
#define ADC0_SSFIFO3_R          (*((volatile uint32_t *)0x400380A8))


void main(void){
    SYSCTL_RCGCADC_R  = 0x01;   // Clock Enable for ADC
    while((SYSCTL_PRADC_R&0x01)==0); // Delay for the clock enable

    ADC0_PC_R = 0x01;       // 125Ksamp/s
    ADC0_SSPRI_R = 0x0123;  // SS3 with the top priority
    ADC0_ACTSS_R = 0x0000;  // Disables SS3 before the configuration
    ADC0_EMUX_R = 0x0000;   // Software enable for the sampling
    ADC0_SSEMUX3_R = 0x00;  // Pins AIn(15:0)
    ADC0_SSMUX3_R = (ADC0_SSMUX3_R & 0xFFFFFFF0) + 9; // AIN9 Channel
    ADC0_SSCTL3_R = 0x000E; // Enabling the Temperature Sensor, INR3 Enable, End Sequence; No diferencial mode
    ADC0_IM_R = 0x0000;     // Disables SS3 interruptions
    ADC0_ACTSS_R |= 0x0008; // Enables SS3

    ADC0_ISC_R = 0x0008;//Clears Flags
    while{
      uint32_t adc_result;
      float TEMP;
      ADC0_PSSI_R = 0x0008;             // Starts Conversion
      while ((ADC0_RIS_R & 0x08)==0);   // ALWAYS STUCK HERE
      adc_result = (ADC0_SSFIFO3_R & 0xFFF);// Saves the conversion result to adc_result
      ADC0_ISC_R = 0x0008;              // Clears flags

      TEMP = 147.5 - ((75 * (3.3) * adc_result)/ 4096);
        }
}
