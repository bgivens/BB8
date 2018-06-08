/*
 * servo.c
 *
 *  Created on: Mar 1, 2018
 *      Author: Brandon
 */

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/rom.h"
#include "driverlib/pwm.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "utils/uartstdio.h"
#include "priorities.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "servo.h"

uint32_t ui32SysClock;

//*****************************************************************************
//
// The stack size for the LED toggle task.
//
//*****************************************************************************
#define SERVOTASKSTACKSIZE        	128         // Stack size in words
#define PWM_FREQUENCY 				55
volatile uint32_t ui32Load;
volatile uint32_t ui32PWMClock;
volatile uint8_t ui8Adjust = 83;
volatile uint8_t ui8Adjust_interval = 5;

static void ServoTask(void *pvParameters)
{
    //delay 5 seconds
    while(1)
    {
    	if(ui8Adjust > 111)
    	{
    		ui8Adjust_interval = -10;
    		ui8Adjust = 111;
    	}
    	if(ui8Adjust < 45)
    	{
    		ui8Adjust_interval = 10;
    		ui8Adjust = 45;
    	}
    	ui8Adjust = ui8Adjust + ui8Adjust_interval;
    	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, ui8Adjust * ui32Load / 1000);
    	vTaskDelay(250);
    }
}

uint32_t ServoTaskInit(void)
{
	//
    // Set the PWM clock to the system clock.
    //
    //SysCtlPWMClockSet(SYSCTL_PWMDIV_1);
    SysCtlPWMClockSet(SYSCTL_PWMDIV_64);

    //
    // The PWM peripheral must be enabled for use.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);

    //
    // For this example PWM0 is used with PortB Pin6.  The actual port and
    // pins used may be different on your part, consult the data sheet for
    // more information.
    // GPIO port B needs to be enabled so these pins can be used.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

    //
    // Configure the GPIO pin muxing to select PWM00 functions for these pins.
    // This step selects which alternate function is available for these pins.
    // This is necessary if your part supports GPIO pin function muxing.
    // Consult the data sheet to see which functions are allocated per pin.
    //
    GPIOPinConfigure(GPIO_PB6_M0PWM0);

    //
    // Configure the PWM function for this pin.
    // Consult the data sheet to see which functions are allocated per pin.
    //
    GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_6);

    //
    // Configure the PWM0 to count up/down without synchronization.
    //
    ui32PWMClock = SysCtlClockGet() / 64;
    ui32Load = (ui32PWMClock / PWM_FREQUENCY) - 1;
    PWMGenConfigure(PWM0_BASE, PWM_GEN_0, PWM_GEN_MODE_DOWN);

    //
    // Set the PWM period to 250Hz.  To calculate the appropriate parameter
    // use the following equation: N = (1 / f) * SysClk.  Where N is the
    // function parameter, f is the desired frequency, and SysClk is the
    // system clock frequency.
    // In this case you get: (1 / 250Hz) * 16MHz = 64000 cycles.  Note that
    // the maximum period you can set is 2^16.
    //
    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, ui32Load);

    //
    // Set PWM0 to a duty cycle of 25%.  You set the duty cycle as a function
    // of the period.  Since the period was set above, you can use the
    // PWMGenPeriodGet() function.  For this example the PWM will be high for
    // 25% of the time or 16000 clock ticks (64000 / 4).
    //

    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, ui8Adjust * ui32Load / 1000);

    //
    // Enable the PWM0 Bit0 (PD0) output signal.
    //
    PWMOutputState(PWM0_BASE, PWM_OUT_0_BIT, true);

    //
    // Enable the PWM generator block.
    //
    PWMGenEnable(PWM0_BASE, PWM_GEN_0);

    //
    // Create the LED task.
    //
    if(xTaskCreate(ServoTask, (const portCHAR *)"Servo", SERVOTASKSTACKSIZE, NULL, tskIDLE_PRIORITY + PRIORITY_SERVO_TASK, NULL) != pdTRUE)
    {
    	return(1);
    }

    //
    // Success.
    //
    return(0);
}
