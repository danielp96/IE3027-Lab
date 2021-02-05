/* 
 * File:   d7s.c
 * Author: Daniel Barrientos Pineda
 * Comments: for using up to 4 7 segment displays
 */


#include "d7s.h"


#define D7S_P0
#define D7S_P1
#define D7S_P2
#define D7S_P3



uint8_t d7s_bin2hex(uint8_t num)
{
    num &= 0x0F;

    uint8_t hex = 0;

    switch (num)
    {
        case 0x0:
            hex = 0x7E;
            break;

        case 0x1:
            hex = 0x30;
            break;

        case 0x2:
            hex = 0x6D;
            break;
        
        case 0x3:
            hex = 0x79;
            break;
        
        case 0x4:
            hex = 0x33;
            break;
        
        case 0x5:
            hex = 0x5B;
            break;
        
        case 0x6:
            hex = 0x5F;
            break;
        
        case 0x7:
            hex = 0x70;
            break;
        
        case 0x8:
            hex = 0x7F;
            break;
        
        case 0x9:
            hex = 0x7B;
            break;
        
        case 0xA:
            hex = 0x77;
            break;
        
        case 0xB:
            hex = 0x1F;
            break;
        
        case 0xC:
            hex = 0x4E;
            break;
        
        case 0xD:
            hex = 0x3D;
            break;
        
        case 0xE:
            hex = 0x4F;
            break;

        case 0xF:
            hex = 0x47;
            break;

        default:
            hex = 0xFF;
    }

    return hex;
}

void d7s_2display(uint8_t* port, uint8_t data, bool sel)
{
    if (sel)
    {
        *port = d7s_bin2hex(data);
    }
    else
    {
        *port = d7s_bin2hex(data >> 4);
    }
}
