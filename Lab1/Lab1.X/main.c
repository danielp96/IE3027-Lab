/*
 * File:   main.c
 * Author: daniel
 *
 * Created on 22 de enero de 2021, 08:47 AM
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
#include <stdbool.h>

//******************************************************************************
// Defines
//******************************************************************************

#define _XTAL_FREQ 8000000

#define push_start PORTDbits.RD0
#define push_p1    PORTDbits.RD1
#define push_p2    PORTDbits.RD2


//******************************************************************************
// Variables
//******************************************************************************

bool b_start_push = false;
bool b_p1_push = false;
bool b_p2_push = false;
bool b_game_running = false;

//******************************************************************************
// function declarations
//******************************************************************************

void setup(void);
void semaforo(void);

//******************************************************************************
// Main
//******************************************************************************

void main(void)
{
    setup();
    while(1) 
    {
        __delay_ms(250);
        //**********************************************************************
        // Debouncing
        //**********************************************************************
        
        b_start_push = false;
        b_p1_push = false;
        b_p2_push = false;
        
        if (push_start)
        {
            b_start_push = true;
        }
        
        if (push_p1)
        {
            b_p1_push = true;
        }
        
        if (push_p2)
        {
            b_p2_push = true;
        }
        
        //**********************************************************************
        // Start game
        //**********************************************************************
        if (b_start_push && !b_game_running)
        {
            b_game_running = true;
            semaforo();
        }
        
        //**********************************************************************
        // Player input
        //**********************************************************************
        
        if (b_p1_push && b_game_running)
        {
            PORTA = !PORTA ? 1 : PORTA << 1;
        }
        
        if (b_p2_push && b_game_running)
        {
            PORTB = !PORTB ? 1 : PORTB << 1;
        }
        
        //**********************************************************************
        // Winner determination
        //**********************************************************************
        
        if ((0x80 == PORTA) || (0x80 == PORTB))
        {
            b_game_running = false;
            
            PORTA = 0x80 == PORTA ? 0xFF : 0;
            PORTB = 0x80 == PORTB ? 0xFF : 0;
            
            PORTC = 0;
        }
    }
}

//******************************************************************************
// Configure input output registers and ports
//******************************************************************************
void setup(void)
{
    ANSEL  = 0x00;
    ANSELH = 0x00;
    
    TRISA = 0x00; // player 1
    TRISB = 0x00; // player 2
    TRISC = 0x00; // semaforo
    TRISD = 0x07; // push buttons
    
    PORTA = 0;
    PORTB = 0;
    PORTC = 0;
    
    return;
}

void semaforo(void)
{
    PORTA = 0;
    PORTB = 0;
    
    PORTC = 1;
    __delay_ms(750);

    PORTC <<= 1;
    __delay_ms(750);
    

    PORTC <<= 1;
}