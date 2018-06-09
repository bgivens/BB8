/*
 * bluetooth_controller.h
 *
 *  Created on: May 27, 2018
 *      Author: Brandon
 */

#ifndef BLUETOOTH_CONTROLLER_H_
#define BLUETOOTH_CONTROLLER_H_

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "utils/uartstdio.h"
#include "priorities.h"
#include "FreeRTOS.h"
#include "task.h"
#include "driverlib/uart.h"

int32_t vertical_joystick_data;
int32_t horizontal_joystick_data;

extern void BT_Controller_ISR(void);
extern uint32_t BT_Controller_Init(void);

#endif /* BLUETOOTH_CONTROLLER_H_ */
