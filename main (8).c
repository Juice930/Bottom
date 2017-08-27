//*****************************************************************************
//
// TM4C129_ADC0.c - ADC0 Example
//
// Copyright (c) 2013-2014 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
//
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
//
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
//
// This is part of revision 2.1.0.12573 of the DK-TM4C129X Firmware Package.
//
//*****************************************************************************

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_adc.h"
#include "driverlib/debug.h"
#include "driverlib/adc.h"
#include "driverlib/pin_map.h"
#include "driverlib/interrupt.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/udma.h"
#include "driverlib/gpio.h"
#include "utils/uartstdio.h"

uint32_t
WaitAndReadADC(uint32_t *adcValues)
{
    ADCProcessorTrigger(ADC0_BASE,0);
    // Wait until the sample sequence has completed.
    while(!ADCIntStatus(ADC0_BASE, 0, false)) { }
    // Read the value from the ADC.
    return(ADCSequenceDataGet(ADC0_BASE, 0, adcValues));
}

void
vInitADC(void)
{
    //  Initializing of the ADC Modules needed to poll the excitation frequency
    //  And the data sincos data signals
    SysCtlPeripheralReset(SYSCTL_PERIPH_GPIOE);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    SysCtlPeripheralReset(SYSCTL_PERIPH_ADC0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    SysCtlDelay(10);

    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3 | GPIO_PIN_2);

    /*
    // ******************************************************************************
    // Initialization of ADC1 Module for Acquisition of the excitation Signal on PK3
    //      Settings:
    //      1 Msps ^= 16MHz ADC clock
    //
    // Set up the Clock at | (VCO/15)/2 = (480/30) = 16MHz | which equals 1Msp/s
    */
    ADCClockConfigSet(ADC0_BASE, ADC_CLOCK_SRC_PLL | ADC_CLOCK_RATE_EIGHTH, 30);

    // Choose Sequencer 2 and set it at the highest Priority.
    ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_ALWAYS, 0);

    // Choose Step 0 in Sequencer 2 as Data Buffer, set it as last Step and enable Interrupts
    ADCSequenceStepConfigure(ADC0_BASE,0,0, ADC_CTL_CH0);
    ADCSequenceStepConfigure(ADC0_BASE,0,1, ADC_CTL_CH1 | ADC_CTL_IE | ADC_CTL_END);

    ADCSequenceEnable(ADC0_BASE, 0);

}

//*****************************************************************************
//
// Configure the UART and its pins.  This must be called before UARTprintf().
//
//*****************************************************************************
void
ConfigureUART(void)
{
    //
    // Enable GPIOA
    //
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    //
    // Enable UART0
    //
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    //
    // Use the internal 16MHz oscillator as the UART clock source.
    //
    ROM_UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);

    //
    // Initialize the UART for console I/O.
    //
    UARTStdioConfig(0, 115200, 16000000);
}

//*****************************************************************************
//
// Configure the ADC0 and ADC1
//
//*****************************************************************************
int
main(void)
{
    uint32_t ui32SysClock;
    uint32_t ui32adcValues[2], ui32Count;

    //
    // Run from the PLL at 120 MHz.
    //
    ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480),
            120000000);

    ConfigureUART();

    //
    // Configure the ADC Channels
    //
    vInitADC();

    UARTprintf("ADC System Clock %d\n",ui32SysClock);

    ui32Count = WaitAndReadADC(ui32adcValues);
    UARTprintf("NO %d\n",ui32Count);
    UARTprintf("S0 %x\n",ui32adcValues[0]);
    UARTprintf("S1 %x\n",ui32adcValues[1]);

    while(1);
}
