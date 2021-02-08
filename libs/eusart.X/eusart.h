/* 
 * File:   eusart.h
 * Author: Daniel Barrientos Pineda
 * Comments: for configuring and using EUSART of PIC16F887
 */

#ifndef __EUSART__
#define	__EUSART__

#include <xc.h> 
#include <stdint.h>


void eusart_init_tx(void);

void eusart_enable_tx_isr(void);

void eusart_disable_tx_isr(void);

void eusart_init_rx(void);

void eusart_enable_rx_isr(void);

void eusart_disable_rx_isr(void);

#endif	/* __EUSART__ */

