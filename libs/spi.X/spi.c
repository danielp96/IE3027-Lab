 /*
 * File            : spi.c
 * Author          : Ligo George
 * Company         : electroSome
 * Project         : SPI Library for MPLAB XC8
 * Microcontroller : PIC 16F877A
 * Created on April 15, 2017, 5:59 PM
 */

#include "spi.h"

void spi_init(Spi_Type sType, Spi_Data_Sample sDataSample, Spi_Clock_Idle sClockIdle, Spi_Transmit_Edge sTransmitEdge)
{
    TRISC5 = 0;
    if(sType & 0b00000100) //If Slave Mode
    {
        SSPSTAT = sTransmitEdge;
        TRISC3 = 1;
    }
    else              //If Master Mode
    {
        SSPSTAT = sDataSample | sTransmitEdge;
        TRISC3 = 0;
    }
    
    SSPCON = sType | sClockIdle;
}

static void spi_receive_wait()
{
    while ( !SSPSTATbits.BF ); // Wait for Data Receive complete
}

void spi_write(char dat)  //Write data to SPI bus
{
    SSPBUF = dat;
}

unsigned spi_data_ready() //Check whether the data is ready to read
{
    if(SSPSTATbits.BF)
        return 1;
    else
        return 0;
}

char spi_read() //REad the received data
{
    spi_receive_wait();        // wait until the all bits receive
    return(SSPBUF); // read the received data from the buffer
}
