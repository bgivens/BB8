/*
 * main_motor.c
 *
 *  Created on: Mar 1, 2018
 *      Author: Brandon Givens
 */

#include <main_motor.h>

uint32_t ui32SysClock;

//*****************************************************************************
//
// The stack size for the LED toggle task.
//
//*****************************************************************************
#define MAINMOTORTASKSTACKSIZE        	128         // Stack size in words
#define PWM_FREQUENCY 					55
volatile uint32_t ui32Load;
volatile uint32_t ui32PWMClock;
volatile uint8_t ui8Adjust = 83;
volatile uint8_t ui8Adjust_interval = 5;

int32_t map(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

static void MainMotorTask(void *pvParameters)
{
    //delay 5 seconds
    while(1)
    {
    	//TODO: Check if if-statements can be optimized
    	ui8Adjust = vertical_joystick_data;
    	if(vertical_joystick_data > 0)
    	{
        	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2, ui8Adjust);
        	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_3, 0);
    	}
    	else if(vertical_joystick_data < 0)
    	{
    		PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2, 0);
    		PWMPulseWidthSet(PWM0_BASE, PWM_OUT_3, ui8Adjust);
    	}
    	else if(vertical_joystick_data == 0)
    	{
    		PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2, 0);
    		PWMPulseWidthSet(PWM0_BASE, PWM_OUT_3, 0);
    	}

    	vTaskDelay(100);
    }
}

uint32_t MainMotorTaskInit(void)
{
	//
    // Set the PWM clock to the system clock.
    //
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
    GPIOPinConfigure(GPIO_PB4_M0PWM2);
    GPIOPinConfigure(GPIO_PB5_M0PWM3);


    //
    // Configure the PWM function for this pin.
    // Consult the data sheet to see which functions are allocated per pin.
    //
    GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_4);
    GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_5);

    //
    // Configure the PWM0 to count up/down without synchronization.
    //
    ui32PWMClock = SysCtlClockGet() / 64;
    ui32Load = (ui32PWMClock / PWM_FREQUENCY) - 1;
    PWMGenConfigure(PWM0_BASE, PWM_GEN_1, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC |
    		PWM_GEN_MODE_DB_NO_SYNC);

    //
    // Set the PWM period to 250Hz.  To calculate the appropriate parameter
    // use the following equation: N = (1 / f) * SysClk.  Where N is the
    // function parameter, f is the desired frequency, and SysClk is the
    // system clock frequency.
    // In this case you get: (1 / 250Hz) * 16MHz = 64000 cycles.  Note that
    // the maximum period you can set is 2^16.
    //
    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_1, 400);

    //
    // Set PWM0 to a duty cycle of 25%.  You set the duty cycle as a function
    // of the period.  Since the period was set above, you can use the
    // PWMGenPeriodGet() function.  For this example the PWM will be high for
    // 25% of the time or 16000 clock ticks (64000 / 4).
    //
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, 0);
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2, 0);

    //
    // Enable the PWM0 Bit0 (PD0) output signal.
    //
    PWMOutputState(PWM0_BASE, PWM_OUT_2_BIT | PWM_OUT_3_BIT, true);
    //PWMOutputState(PWM0_BASE, PWM_OUT_2_BIT, true);


    //
    // Enable the PWM generator block.
    //
    PWMGenEnable(PWM0_BASE, PWM_GEN_1);

    //Configuring GPIO Port C for motor driver enable pins
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5);
    GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5, GPIO_PIN_4 | GPIO_PIN_5);


    //
    // Create the LED task.
    //
    if(xTaskCreate(MainMotorTask, (const portCHAR *)"Servo", MAINMOTORTASKSTACKSIZE, NULL, tskIDLE_PRIORITY + PRIORITY_MAINMOTOR_TASK, NULL) != pdTRUE)
    {
    	return(1);
    }

    //
    // Success.
    //
    return(0);
}
