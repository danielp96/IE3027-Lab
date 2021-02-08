/* 
 * File:   adc.h
 * Author: Daniel Barrientos Pineda
 * Comments: for configuring and using adc of PIC16F887
 * Revision history: 
 */

#ifndef __ADC__

#include <xc.h>


void adc_config(void);

void adc_start(void);

void adc_select_channel(int channel);

void adc_wait(void);

void adc_isr_enable(void);

void adc_isr_disable(void);

#endif /* __ADC__ */