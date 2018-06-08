/*
 * bluetooth_controller.c
 *
 *  Created on: May 27, 2018
 *      Author: Brandon
 */

#include "bluetooth_controller.h"

#define BTCONTROLLERSTACKSIZE		128

static void BT_Controller_Task(void *pvParameters)
{
	while(1)
	{
 		if(UARTCharsAvail(UART1_BASE) == true)
		{
			while(UARTCharsAvail(UART1_BASE))
			{
				vertical_joystick_data = UARTCharGetNonBlocking(UART1_BASE);
				UARTprintf("%d\n", vertical_joystick_data);
			}
		}
	}
}

uint32_t BT_Controller_TaskInit(void)
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

	vertical_joystick_data = 0;
	horizontal_joystick_data = 0;

	if(xTaskCreate(BT_Controller_Task, (const portCHAR *)"Bluetooth_Controller", BTCONTROLLERSTACKSIZE, NULL, tskIDLE_PRIORITY + PRIORITY_BT_CONTROLLER_TASK, NULL) != pdTRUE)
	{
		return(1);
	}

	return(0);
}
