/* 
 * File:   adc.h
 * Author: Daniel Barrientos Pineda
 * Comments: for configuring and using adc of PIC16F887
 * Revision history: 
 */

#ifndef __ADC__

#include <xc.h>


void adc_config();

void adc_start();

void adc_select_channel(int channel);

void adc_wait();

void adc_isr_enable();

void adc_isr_disable();

#endif /* __ADC__ */