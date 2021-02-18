/*
 * File:   main.c
 * Author: daniel
 *
 * Created on 18 de febrero de 2021, 09:07 AM
 */

//******************************************************************************
// PIC16F887 Configuration Bit Settings
// 'C' source line config statements
//******************************************************************************

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT
#pragma config WDTE = OFF       
#pragma config PWRTE = OFF      
#pragma config MCLRE = OFF      
#pragma config CP = OFF         
#pragma config CPD = OFF        
#pragma config BOREN = OFF      
#pragma config IESO = OFF       
#pragma config FCMEN = OFF      
#pragma config LVP = OFF        

// CONFIG2
#pragma config BOR4V = BOR40V   
#pragma config WRT = OFF        

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

//******************************************************************************
// Includes
//******************************************************************************

#include <xc.h>
#include <stdint.h>
#include "adc.h"

//******************************************************************************
// Defines
//******************************************************************************

#define _XTAL_FREQ 4000000

//******************************************************************************
// Variables
//******************************************************************************

uint8_t adc_data = 0;


//******************************************************************************
// function declarations
//******************************************************************************

void setup(void);
void adc_logic(void);

//******************************************************************************
// Main
//******************************************************************************

void main(void)
{
    setup();
    while(1) 
    {
        adc_logic();

        PORTB = adc_data;

    }
}

void __interrupt() isr(void)
{
    if (PIR1bits.ADIF)
    {
        PIR1bits.ADIF = 0;

        adc_data = ADRESH;
    }
}

void adc_logic(void)
{
    if (ADCON0bits.GO)
    {
        return;
    }

    adc_wait();
    adc_start();
}

//******************************************************************************
// Configuration
//******************************************************************************
void setup(void)
{
    ANSEL  = 0x03;
    ANSELH = 0x00;
    
    TRISA = 0x03;
    TRISB = 0x00;
    TRISC = 0x80;
    TRISD = 0x00;
    TRISE = 0x00;
    
    PORTA = 0;
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
    PORTE = 0;

    // interrupts
    IOCB = 0x00;
    INTCONbits.RBIE = 0;
    INTCONbits.T0IE = 0;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
    
    OSCCON = 0b01100001;

    OPTION_REGbits.T0CS = 0;
    OPTION_REGbits.PSA = 0;
    OPTION_REGbits.PS2 = 0;
    OPTION_REGbits.PS1 = 0;
    OPTION_REGbits.PS0 = 1;

    adc_config();
    adc_isr_enable();

    return;
}