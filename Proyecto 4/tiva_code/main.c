
#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "inc/hw_gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"

uint8_t data = 0;

void display(uint8_t data);

/**
 * main.c
 */
int main(void)
{
    // clock config
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

    // portb config
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6); // display

    // portc config
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    GPIOPinTypeGPIOInput(GPIO_PORTC_BASE, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7); // push
    GPIOPadConfigSet(GPIO_PORTB_BASE, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD);

    // portd config
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTD_BASE + GPIO_O_CR) |= GPIO_PIN_7;
    GPIOPinConfigure(GPIO_PD7_U2TX);
    GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3); // red leds

    // porte config
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3); // green leds

    // unlock PD_7
    //HWREG(GPIO_PORTD_BASE + GPIO_O_AFSEL) |= 0x80;

    // uart config
    //SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA); // enable A
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART2); // enable uart0
    GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7); // uart controls pins
    UARTConfigSetExpClk(UART2_BASE, SysCtlClockGet(), 115200, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE)); // config uart
    UARTIntClear(UART2_BASE, UART_INT_RX | UART_INT_RT | UART_INT_TX | UART_INT_FE | UART_INT_PE | UART_INT_BE | UART_INT_OE | UART_INT_RI | UART_INT_CTS | UART_INT_DCD | UART_INT_DSR); // clear isr
    //IntEnable(INT_UART2); // enable uart isr
    //UARTIntEnable(UART2_BASE, UART_INT_RX | UART_INT_RT); // enable uart isr on RX and TX

    uint8_t old_data = 1;

    while (1)
    {
        data = GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7) >> 4;

        if (data != old_data)
        {

            GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, data);

            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, ~data);

            display( 4 - (((data >> 3) & 0x01) + ((data >> 2) & 0x01) + ((data >> 1) & 0x01) + (data & 0x01)));

            old_data = data;

            UARTCharPut(UART2_BASE, data);
        }
    }

}


void display(uint8_t data)
{
    uint8_t temp = 0x71;

    switch (data & 0x0F)
    {
        case 0:
            temp = 0x3F;
            break;

        case 1:
            temp = 0x06;
            break;

        case 2:
            temp = 0x5B;
            break;

        case 3:
            temp = 0x4F;
            break;

        case 4:
            temp = 0x66;
            break;

        default:
            temp = 0xFF;
    }

    GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, temp);
}








