/* 
 * File:   eusart.c
 * Author: Daniel Barrientos Pineda
 * Comments: for configuring and using EUSART of PIC16F887
 */

// hardcoded to 9600 baudrrate
// TODO: configurable baudrate
// TODO: refactor inits


#include <xc.h>

void eusart_init_tx(void)
{
    SPBRGH = 0;
    SPBRG = 25;
    TXSTAbits.BRGH = 1;

    TXSTAbits.SYNC = 0;
    RCSTAbits.SPEN = 1;

    TXSTAbits.TX9 = 0;
    TXSTAbits.TXEN = 1;
}

void eusart_enable_tx_isr(void)
{
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    PIE1bits.TXIE = 1;
}

void eusart_disable_tx_isr(void)
{
    PIE1bits.TXIE = 0;
}

void eusart_init_rx(void)
{
    SPBRGH = 0;
    SPBRG = 25;
    TXSTAbits.BRGH = 1;

    TXSTAbits.SYNC = 0;
    RCSTAbits.SPEN = 1;

    TXSTAbits.TX9 = 0;
    RCSTAbits.CREN = 1;
}

void eusart_enable_rx_isr(void)
{
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    PIE1bits.RCIE = 1;
}

void eusart_disable_rx_isr(void)
{
    PIE1bits.RCIE = 0;
}
