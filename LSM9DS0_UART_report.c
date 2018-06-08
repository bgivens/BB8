/*
 * LSM9DS0_UART_report.c
 *
 *  Created on: Mar 10, 2018
 *      Author: Brandon
 */

#include "LSM9DS0_UART_report.h"

//TODO: Move to data report task
#define LSM9DS0TASKSTACKSIZE		128

static void LSM9DS0DataTask(void *pvParameters)
{
	while(1)
	{
		lsm9ds0Vector_t accelData = readAccel();
		lsm9ds0Vector_t gyroData = readGyro();
		lsm9ds0Vector_t magData = readMag();

		UARTprintf("Accelerometer X: %d\n", accelData.x);
		UARTprintf("Accelerometer Y: %d\n", accelData.y);
		UARTprintf("Accelerometer Z: %d\n\n", accelData.z);

		UARTprintf("Gyroscope X: %d\n", gyroData.x);
		UARTprintf("Gyroscope Y: %d\n", gyroData.y);
		UARTprintf("Gyroscope Z: %d\n\n", gyroData.z);

		UARTprintf("Magnetometer X: %d\n", magData.x);
		UARTprintf("Magnetometer Y: %d\n", magData.y);
		UARTprintf("Magnetometer Z: %d\n", magData.z);

		UARTprintf("----------------------------------------\n");

		vTaskDelay(100);
	}
}

uint32_t LSM9DS0ReportTaskInit(void)
{
	LSM9DS0Init();

	if(xTaskCreate(LSM9DS0DataTask, (const portCHAR *)"LSM9DS0Data", LSM9DS0TASKSTACKSIZE, NULL, tskIDLE_PRIORITY + PRIORITY_LSM9DS0_TASK, NULL) != pdTRUE)
	{
		return(1);
	}

	return(0);
}

