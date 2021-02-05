/* 
 * File:   adc.c
 * Author: Daniel Barrientos Pineda
 * Comments: for configuring and using adc of PIC16F887
 */

#include "adc.h"

// hard coded config for now
// TODO: configurable config later
void adc_config()
{
    ADCON0 = 0b10000001;

    ADCON1bits.ADFM  = 0;
    ADCON1bits.VCFG1 = 0;
    ADCON1bits.VCFG0 = 0;

    return;
}

void adc_start()
{
    ADCON0bits.GO = 1;

    return;
}

// channel must be 0-15
void adc_select_channel(int channel)
{
    ADCON0bits.CHS0 = (channel >> 0) & 0x01;
    ADCON0bits.CHS1 = (channel >> 1) & 0x01;
    ADCON0bits.CHS2 = (channel >> 2) & 0x01;
    ADCON0bits.CHS3 = (channel >> 3) & 0x01;

    return;
}

void adc_isr_enable()
{
    INTCONbits.PEIE = 1;
    PIE1bits.ADIE = 1;
}

void adc_isr_disable()
{
    PIE1bits.ADIE = 0;
}

void adc_wait()
{
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("NOP");
    asm("NOP");

    return;
}
