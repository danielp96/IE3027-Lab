/*
 * File:   main.c
 * Author: daniel
 *
 * Created on 26 de febrero de 2021, 08:16 AM
 */

//******************************************************************************
// PIC16F887 Configuration Bit Settings
// 'C' source line config statements
//******************************************************************************

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT
#pragma config WDTE = OFF       
#pragma config PWRTE = OFF      
#pragma config MCLRE = ON      
#pragma config CP = OFF         
#pragma config CPD = OFF        
#pragma config BOREN = OFF      
#pragma config IESO = OFF       
#pragma config FCMEN = OFF      
#pragma config LVP = OFF        

// CONFIG2
#pragma config BOR4V = BOR40V   
#pragma config WRT = OFF        

//******************************************************************************
// Includes
//******************************************************************************

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include "eusart.h"
#include "i2c.h"

//******************************************************************************
// Defines
//******************************************************************************

#define _XTAL_FREQ 4000000

#define tmr_preload 6;

//******************************************************************************
// Variables
//******************************************************************************

uint8_t uart_data = 0;
uint8_t i2c_data  = 0;



//******************************************************************************
// function declarations
//******************************************************************************

void setup(void);

//******************************************************************************
// Main
//******************************************************************************

void main(void)
{
    setup();
    while(1) 
    {
        i2c_master_start();
        i2c_master_write(0xEC); // write mode
        i2c_master_write(0xD0); // register id of sensor
        i2c_master_stop();
        __delay_ms(250);

        i2c_master_start();
        i2c_master_write(0xED); // read mode

        // register id is always 0x58
        // i2c_data should be 0x58
        i2c_data = (uint8_t)i2c_master_read(0); // read byte from sensor and end
        i2c_master_stop();
        
        PORTA = uart_data;

        PORTB = uart_data;
    }
}

void __interrupt() isr(void)
{

    if (PIE1bits.TXIE && PIR1bits.TXIF)
    {
        TXREG = i2c_data;
    }

    if (PIR1bits.RCIF)
    {
        uart_data = RCREG;
    }

}


//******************************************************************************
// Configuration
//******************************************************************************
void setup(void)
{
    ANSEL  = 0x00;
    ANSELH = 0x00;
    
    TRISA = 0x00;
    TRISB = 0x00;
    TRISC = 0x80;
    TRISD = 0x00;
    TRISE = 0x00;
    
    PORTA = 0;
    PORTB = 0xFF;
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

    TMR0 = tmr_preload;

    INTCONbits.T0IF = 0;

    eusart_init_tx();
    eusart_enable_tx_isr();

    eusart_init_rx();
    eusart_enable_rx_isr();

    i2c_master_init(100000);

    return;
}