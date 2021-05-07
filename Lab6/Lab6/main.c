#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"

#define DELAY 10000000

#define RED GPIO_PIN_1
#define GREEN GPIO_PIN_3
#define YELLOW GPIO_PIN_1|GPIO_PIN_3
#define OFF 0x00000000

/**
 * main.c
 */
int main(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    // Timers: RCGCTIMER, RCGCWTIMER, GPIOPCTL
    // UART: RCGCUART, UARTIBRD, UARTFBRD, UARTLCRH, UARTCC, UARTDMACTL
    // USB: RCGCUSB, GPIOPCTL
    // PWM: RCGC0, RCGC2, GPIOAFSEL, GPIOPCTL, RCC, PWM0CTL, PWM0GENA, PWM0GENB, PWM0LOAD, PWM0CMPA, PWM0CMPB, PWM0CTL, PWMENABLE
    // ADC: RCGCADC, GPIODEN, GPIOAMSEL, ADCSSPRI

	while (1)
	{
	    if ( !GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4) )
	    {
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, GREEN);
            SysCtlDelay(DELAY);

            // blink
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, GREEN);
            SysCtlDelay(DELAY/2);
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, OFF);
            SysCtlDelay(DELAY/2);

            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, GREEN);
            SysCtlDelay(DELAY/2);
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, OFF);
            SysCtlDelay(DELAY/2);

            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, GREEN);
            SysCtlDelay(DELAY/2);
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, OFF);
            SysCtlDelay(DELAY/2);

            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, GREEN);
            SysCtlDelay(DELAY/2);
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, OFF);
            SysCtlDelay(DELAY/2);

            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, OFF);
            SysCtlDelay(DELAY/4);

            // yellow
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, YELLOW);
            SysCtlDelay(DELAY);
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, OFF);
            SysCtlDelay(DELAY/4);

            // red
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, RED);
            SysCtlDelay(DELAY);
	    }
	}
}

