/*
 * main_motor.h
 *
 *  Created on: Mar 1, 2018
 *      Author: Brandon
 */

#ifndef MAIN_MOTOR_H_
#define MAIN_MOTOR_H_

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

//*****************************************************************************
//
// Prototypes for the Main Motor task.
//
//*****************************************************************************

extern int32_t vertical_joystick_data;

extern uint32_t MainMotorTaskInit(void);

#endif /* MAIN_MOTOR_H_ */
