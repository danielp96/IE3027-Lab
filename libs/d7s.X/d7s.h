/* 
 * File:   d7s.h
 * Author: Daniel Barrientos Pineda
 * Comments: for configuring and using adc of PIC16F887
 * Revision history: 
 */
 
#ifndef __D7S__
#define	__D7S__

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>

uint8_t d7s_bin2hex(uint8_t num);
void d7s_2display(uint8_t* port, uint8_t data, bool sel);


#endif	/* __D7S__ */

