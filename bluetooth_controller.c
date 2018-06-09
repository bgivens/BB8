/*
 * bluetooth_controller.c
 *
 *  Created on: May 27, 2018
 *      Author: Brandon
 */

#include "bluetooth_controller.h"

#define BTCONTROLLERSTACKSIZE		128

int data[3];

extern void BT_Controller_ISR(void)
{
	UARTIntClear(UART1_BASE, UART_INT_RX);

 	if(UARTCharsAvail(UART1_BASE) == true)
	{
		data[0] = UARTCharGetNonBlocking(UART1_BASE);

		//Waiting for next character to come in. The TM4C123XL is fast enough where it's done processing
		//the first character before the second one arrives, making parsing the data potentially difficult
		while(UARTCharsAvail(UART1_BASE)!=true)
		{
		}
		data[1] = UARTCharGetNonBlocking(UART1_BASE);
		while(UARTCharsAvail(UART1_BASE)!=true)
		{
		}
		data[2] = UARTCharGetNonBlocking(UART1_BASE);

		horizontal_joystick_data = (data[0]-48)*100+(data[1]-48)*10+(data[2]-48);
		if(horizontal_joystick_data > 100) //TODO:Refactor code so that negative integers can be send from controller side, and dealt with appropriately
		{								   //Rather than acting like 0-100 is positive and 101-200 is negative
			horizontal_joystick_data = horizontal_joystick_data - 100;
			horizontal_joystick_data = -horizontal_joystick_data;
		}

		//Waiting for other joystick data
		while(UARTCharsAvail(UART1_BASE)!=true)
		{
		}
		data[0] = UARTCharGetNonBlocking(UART1_BASE);

		while(UARTCharsAvail(UART1_BASE)!=true)
		{
		}
		data[1] = UARTCharGetNonBlocking(UART1_BASE);

		while(UARTCharsAvail(UART1_BASE)!=true)
		{
		}
		data[2] = UARTCharGetNonBlocking(UART1_BASE);

		vertical_joystick_data = (data[0]-48)*100+(data[1]-48)*10+(data[2]-48);
		if(vertical_joystick_data > 100)
		{
			vertical_joystick_data = vertical_joystick_data - 100;
			vertical_joystick_data = -vertical_joystick_data;
		}

		UARTprintf("Horizontal Data: %d\n", horizontal_joystick_data);
		UARTprintf("Vertical Data  : %d\n", vertical_joystick_data);
	}
}

uint32_t BT_Controller_Init(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);

	UARTConfigSetExpClk(UART1_BASE, SysCtlClockGet(), 9600,
						(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
						UART_CONFIG_PAR_NONE));

	GPIOPinConfigure(GPIO_PB0_U1RX);
	GPIOPinConfigure(GPIO_PB1_U1TX);
	GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);

	UARTEnable(UART1_BASE);
	UARTIntEnable(UART1_BASE, UART_INT_RX);
	UARTIntRegister(UART1_BASE, BT_Controller_ISR);

	vertical_joystick_data = 0;
	horizontal_joystick_data = 0;

	return(0);
}
