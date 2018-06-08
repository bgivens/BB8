/*
 * 9dof_UART_report.h
 *
 *  Created on: Mar 10, 2018
 *      Author: Brandon
 */

#ifndef LSM9DS0_UART_REPORT_H_
#define LSM9DS0_UART_REPORT_H_

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
//#include "inc/hw_gpio.h"
//#include "inc/hw_i2c.h"
//#include "driverlib/gpio.h"
#include "driverlib/rom.h"
//#include "driverlib/i2c.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "utils/uartstdio.h"
#include "priorities.h"
#include "LSM9DS0.h"
#include "FreeRTOS.h"
#include "task.h"

uint32_t LSM9DS0ReportTaskInit(void);

#endif /* 9DOF_UART_REPORT_H_ */
