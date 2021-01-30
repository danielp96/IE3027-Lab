/*
 * File:   main.c
 * Author: daniel
 *
 * Created on 29 de enero de 2021, 01:14 PM
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

//******************************************************************************
// Defines
//******************************************************************************

#define _XTAL_FREQ 4000000
#define tmr_preload 6;
#define push_debounce_time 200


//******************************************************************************
// Variables
//******************************************************************************

uint8_t push_counter = 0;
uint8_t portb_flags  = 0;
uint8_t push_timer   = 0;
uint8_t mux_timer    = 0;

//******************************************************************************
// function declarations
//******************************************************************************

void setup(void);
void push_logic(void);

//******************************************************************************
// Main
//******************************************************************************

void main(void)
{
    setup();
    while(1) 
    {
        //**********************************************************************
        // Text goes here
        //**********************************************************************
        push_logic();

        PORTD = push_counter;
    }
}

void __interrupt() isr(void)
{
    if (INTCONbits.RBIF)
    {
        INTCONbits.RBIF = 0;
        portb_flags = PORTB;
    }

    if (INTCONbits.T0IF)
    {
        INTCONbits.T0IF = 0;
        TMR0 = tmr_preload;
        push_timer++;
        mux_timer++;

        PORTC = mux_timer;
    }
}

void push_logic(void)
{
    if (push_debounce_time != push_timer)
    {
        return;
    }

    push_timer = 0;

    if (0x01 == portb_flags)
    {
        push_counter++;
    }

    if (0x02 == portb_flags)
    {
        push_counter--;
    }

    return;
}

//******************************************************************************
// Configuration
//******************************************************************************
void setup(void)
{
    ANSEL  = 0x01;
    ANSELH = 0x00;
    
    TRISA = 0x01;
    TRISB = 0x03;
    TRISC = 0x00;
    TRISD = 0x00;
    TRISE = 0x00;
    
    PORTA = 0;
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
    PORTE = 0;

    // portb interrupts
    IOCB = 0x03;
    INTCONbits.RBIE = 1;
    INTCONbits.T0IE = 1;
    INTCONbits.GIE = 1;
    
    OSCCON = 0b01100001;

    OPTION_REGbits.T0CS = 0;
    OPTION_REGbits.PSA = 0;
    OPTION_REGbits.PS2 = 0;
    OPTION_REGbits.PS1 = 0;
    OPTION_REGbits.PS0 = 1;

    TMR0 = tmr_preload;

    INTCONbits.T0IF = 0;

    return;
}