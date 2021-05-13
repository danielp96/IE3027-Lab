
#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/uart.h"


#define RED 0x02
#define GREEN 0x08
#define BLUE 0x04
#define OFF 0x00


void tmr0_handler(void);

uint8_t val = 0;
uint8_t color = OFF;
char data = ' ';

int main(void)
{
    // clock config
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

    // portf config
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);

    // timer0 config
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    TimerLoadSet(TIMER0_BASE, TIMER_A, (SysCtlClockGet() / 4) - 1);

    // timer interrupt
    IntEnable(INT_TIMER0A);
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT); //timeout
    IntMasterEnable(); // global isr

    // enable timer
    TimerEnable(TIMER0_BASE, TIMER_A);

    // uart config
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA); // enable A
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0); // enable uart0
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1); // uart controls pins
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE)); // config uart
    UARTIntClear(UART0_BASE, UART_INT_RX | UART_INT_RT | UART_INT_TX | UART_INT_FE | UART_INT_PE | UART_INT_BE | UART_INT_OE | UART_INT_RI | UART_INT_CTS | UART_INT_DCD | UART_INT_DSR); // clear isr
    IntEnable(INT_UART0); // enable usart isr
    UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT); // enable uart isr on RX and TX

	while (1)
	{
	    // if data received and pin off (val==1)
	    if (data != ' ' && val)
	    {
	        if (data == 'r')
	        {
	            color = (color==RED)? OFF : RED;
	        }

            if (data == 'g')
            {
                color = (color==GREEN)? OFF : GREEN;
            }

            if (data == 'b')
            {
                color = (color==BLUE)? OFF : BLUE;
            }

	        data = ' ';
	    }
	}
}

void tmr0_handler(void)
{
    // clear timer
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    // do things
    GPIOPinWrite(GPIO_PORTF_BASE, color, val);

    val = ~val;
}

void uart0_handler(void)
{
    // clear uart isr
    UARTIntClear(UART0_BASE, UART_INT_RX | UART_INT_RT);

    // get uart data
    data = UARTCharGet(UART0_BASE);

    // for testing, sends uppercase of received
    //UARTCharPut(UART0_BASE, data - 0x20);
}



