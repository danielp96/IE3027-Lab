/*
 * File:   main.c
 * Author: daniel
 *
 * Created on 18 de febrero de 2021, 09:12 AM
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
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "lcd.h"
#include "eusart.h"
#include "spi.h"

//******************************************************************************
// Defines
//******************************************************************************

#define _XTAL_FREQ 4000000

#define tmr_preload 6;

#define SS1 6
#define SS2 5
#define SS3 3

//******************************************************************************
// Variables
//******************************************************************************

uint8_t pot_data  = 3;
uint8_t push_data = 7;
uint8_t temp_data = 9;
uint8_t spi_data  = 0;

uint8_t uart_data = 0;
uint8_t uart_num  = 1;
uint8_t str_pos   = 0;
bool    next_uart = 0;

uint8_t slave_num = 1;
bool   next_slave = 0;


// IMPORTANT
// for some reason if strings have the same length
// the last char of next string overwrites the first char previous string
char* str_pot_a[7];
char* str_pot_b[6];
char* str_pot_c[5];

// IMPORTANT
// compilers dies from "complex expression" if not done like this >:v
char* stra = (char*)str_pot_a;
char* strb = (char*)str_pot_b;
char* strc = (char*)str_pot_c;
//******************************************************************************
// function declarations
//******************************************************************************

void setup(void);
void display(void);
void set_next_slave(void);
void prepare_uart_data(void);

//******************************************************************************
// Main
//******************************************************************************

void main(void)
{
    setup();
    while(1) 
    {

        set_next_slave();

        // normally this would be in the isr but goes too fast
        // and doesnt change slaves
        if (PIR1bits.SSPIF)
        {
            SSPBUF = 0xAA;
            PIR1bits.SSPIF = 0;
        }


        prepare_uart_data();

        if (next_slave)
        {
            sprintf(stra, "A%.3i:", pot_data);
            sprintf(strb, "B%.3i:", push_data);
            sprintf(strc, "C%.3i:", temp_data);
        }

        display();

        PORTB = slave_num;

    }
}

void __interrupt() isr(void)
{

    if (PIE1bits.TXIE && PIR1bits.TXIF)
    {
        TXREG = (volatile unsigned char)uart_data;
        next_uart = true;

        // disable uart interrupt to avoid sending same character multiple times
        PIE1bits.TXIE = 0;
    }

    if (SSPSTATbits.BF)
    {
        spi_data = SSPBUF;
        SSPSTATbits.BF = 0;
        next_slave = true;
        slave_num++;
        slave_num &= 0x03;
    }
}

void prepare_uart_data(void)
{
    if (!next_uart)
    {
        return;
    }

    next_uart = false;

    switch (uart_num)
    {
        case 1:
            uart_data = (uint8_t)str_pot_a[str_pos];
            break;

        case 2:
            uart_data = (uint8_t)str_pot_b[str_pos];
            break;

        case 3:
            uart_data = (uint8_t)str_pot_c[str_pos];
            break;

        default:
            uart_num = 0;
            uart_data = 0x00;
            break;
    }
    str_pos++;

    if (5 == str_pos)
    {
        str_pos = 0;
        uart_num++;
    }

    PIE1bits.TXIE = 1;
}

void set_next_slave(void)
{
    if (!next_slave)
    {
        return;
    }

    next_slave = false;

    if (1 == slave_num)
    {
        pot_data = spi_data;
        PORTA = SS2;
        return;
    }
    
    if (2 == slave_num)
    {
        push_data = spi_data;
        PORTA = SS3;
        return;
    }
    
    if (3 == slave_num)
    {
        temp_data = spi_data;
        PORTA = SS1;
        return;
    }
}

void display(void)
{
    char* header_str = "  S1:  S2:  S3: ";
    lcd_move_cursor(0,0);
    lcd_write_string(header_str);
    lcd_move_cursor(1,0);

    lcd_write_char(stra[1]);
    lcd_write_char('.');
    lcd_write_char(stra[2]);
    lcd_write_char(stra[3]);
    lcd_write_char('V');
    lcd_write_string("  ");

    lcd_write_char(strb[1]);
    lcd_write_char(strb[2]);
    lcd_write_char(strb[3]);
    lcd_write_string("  ");

    lcd_write_char(strc[1]);
    lcd_write_char(strc[2]);
    lcd_write_char(strc[3]);
    lcd_write_char('C');
}

//******************************************************************************
// Configuration
//******************************************************************************
void setup(void)
{
    ANSEL  = 0x03;
    ANSELH = 0x00;
    
    TRISA = 0x00;
    TRISB = 0x00;
    TRISC = 0x90;
    TRISD = 0x00;
    TRISE = 0x00;
    
    PORTA = SS1;
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

    TMR0 = tmr_preload;

    INTCONbits.T0IF = 0;

    lcd_init();
    lcd_cmd(0x0c); // turn off cursor

    eusart_init_tx();
    eusart_enable_tx_isr();

    spi_init(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);

    SSPBUF = 0x00;

    return;
}