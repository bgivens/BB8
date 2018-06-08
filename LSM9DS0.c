/*
 * LSM9DS0.c
 *	Methods for interacting with the LSM9DS0 9-DOF module via I2C for the Tiva C TM4C123G microcontroller.
 *
 *	To wire:
 *	LSM9DS0 -> TM4C123G
 *	VIN -> 3.3V
 *	GND -> GND
 *	SCL -> PB2
 *	SDA -> PB3
 *
 *
 *  Created on: Mar 3, 2018
 *      Author: Brandon Givens
 *      Contact: brandon.a.givens@gmail.com
 */

#include "LSM9DS0.h"

//***************************************************************************************************
//	Function: I2CReceive
//
//	Queries an I2C slave device for data from a specified register
//
//	Parameters:
//		uint8_t device_address	: 	The I2C address of the slave device
//		uint8_t data_reg		:	Register on the slave device from which
//									data is to be read
//
//	Returns:
//		uint8_t 				:	8-bit data stored in data_reg
//
//***************************************************************************************************
uint8_t I2CReceive(uint8_t device_address, uint8_t data_reg)
{
    //	Specify that we are writing (a register address) to the
    //	slave device
    I2CMasterSlaveAddrSet(I2C0_BASE, device_address, false);

    //	Specify register to be read
    I2CMasterDataPut(I2C0_BASE, data_reg);

    //	Send control byte and register address byte to slave device
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);

    //	Wait for MCU to finish transaction
    while(I2CMasterBusy(I2C0_BASE));

    //	Specify that we are going to read from slave device
    I2CMasterSlaveAddrSet(I2C0_BASE, device_address, true);

    //	Send control byte and read from the register we specified
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);

    //	Wait for MCU to finish transaction
    while(I2CMasterBusy(I2C0_BASE));

    //	Return data pulled from the specified register
    return I2CMasterDataGet(I2C0_BASE);
}

//***************************************************************************************************
//	Function: I2C_regWrite
//
//	Writes data to a specific register of an I2C device
//
//	Parameters:
//		uint16_t device_address		: 	The I2C address of the slave device
//		uint16_t device_register	:	Register on the slave device to which
//										data is to be written
//		uint8_t	 device_data		: 	Data to be written to device register
//
//	Returns:
//		None
//
//***************************************************************************************************
void I2C_regWrite(uint16_t device_address, uint16_t device_register, uint8_t device_data)
{
   //	Specify that we want to communicate to device address with an intended write to bus
   I2CMasterSlaveAddrSet(I2C0_BASE, device_address, false);

   //	Register to be read
   I2CMasterDataPut(I2C0_BASE, device_register);

   //	Send control byte and register address byte to slave device
   I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);

   //	Wait for MCU to finish transaction
   while(I2CMasterBusy(I2C0_BASE));

   //	Specify data to be written to the above mentioned device_register
   I2CMasterDataPut(I2C0_BASE, device_data);

   //	Wait while checking for MCU to complete the transaction
   I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);

   //	Wait for MCU & device to complete transaction
   while(I2CMasterBusy(I2C0_BASE));
}

//***************************************************************************************************
//	Function: readAccel
//
//	Reads acceleration data and returns a vector containing data for x, y, z axis
//
//	Parameters:
//		None
//
//	Returns:
//		lsm9ds0Vector_t	: Vector containing accelerometer data for x, y, z axis
//
//***************************************************************************************************
lsm9ds0Vector_t readAccel()
{
	//	Reading device data.
	//	The LSM9DS0 stores 16bit data in two 8 bit registers, one register containing the
	//	upper 8-bits of the data, the other register containing the lower 8-bits of data
	uint8_t xlo = I2CReceive(LSM9DS0_ADDRESS_ACCELMAG, LSM9DS0_REGISTER_OUT_X_L_A);
	int16_t xhi = I2CReceive(LSM9DS0_ADDRESS_ACCELMAG, LSM9DS0_REGISTER_OUT_X_H_A);
	uint8_t ylo = I2CReceive(LSM9DS0_ADDRESS_ACCELMAG, LSM9DS0_REGISTER_OUT_Y_L_A);
	int16_t yhi = I2CReceive(LSM9DS0_ADDRESS_ACCELMAG, LSM9DS0_REGISTER_OUT_Y_H_A);
	uint8_t zlo = I2CReceive(LSM9DS0_ADDRESS_ACCELMAG, LSM9DS0_REGISTER_OUT_Z_L_A);
	int16_t zhi = I2CReceive(LSM9DS0_ADDRESS_ACCELMAG, LSM9DS0_REGISTER_OUT_Z_H_A);

	// Shifting bits and bitwise or-ing hi and lo data to form proper integers
	xhi <<= 8; xhi |= xlo;
	yhi <<= 8; yhi |= ylo;
	zhi <<= 8; zhi |= zlo;

	//	Forming vector object containing data to return
	lsm9ds0Vector_t accelData;
	accelData.x = xhi;
	accelData.y = yhi;
	accelData.z = zhi;
	return accelData;
}

//***************************************************************************************************
//	Function: readGyro
//
//	Reads gyroscope data and returns a vector containing data for x, y, z axis
//
//	Parameters:
//		None
//
//	Returns:
//		lsm9ds0Vector_t	: Vector containing gyroscope data for x, y, z axis
//
//***************************************************************************************************
lsm9ds0Vector_t readGyro()
{
	// Similar process as accelData()
	uint8_t xlo = I2CReceive(LSM9DS0_ADDRESS_GYRO, LSM9DS0_REGISTER_OUT_X_L_G);
	int16_t xhi = I2CReceive(LSM9DS0_ADDRESS_GYRO, LSM9DS0_REGISTER_OUT_X_H_G);
	uint8_t ylo = I2CReceive(LSM9DS0_ADDRESS_GYRO, LSM9DS0_REGISTER_OUT_Y_L_G);
	int16_t yhi = I2CReceive(LSM9DS0_ADDRESS_GYRO, LSM9DS0_REGISTER_OUT_Y_H_G);
	uint8_t zlo = I2CReceive(LSM9DS0_ADDRESS_GYRO, LSM9DS0_REGISTER_OUT_Z_L_G);
	int16_t zhi = I2CReceive(LSM9DS0_ADDRESS_GYRO, LSM9DS0_REGISTER_OUT_Z_H_G);

	xhi <<= 8; xhi |= xlo;
	yhi <<= 8; yhi |= ylo;
	zhi <<= 8; zhi |= zlo;

	lsm9ds0Vector_t gyroData;
	gyroData.x = xhi;
	gyroData.y = yhi;
	gyroData.z = zhi;
	return gyroData;
}

//***************************************************************************************************
//	Function: readMag
//
//	Reads magnetometer data and returns a vector containing data for x, y, z axis
//
//	Parameters:
//		None
//
//	Returns:
//		lsm9ds0Vector_t	: Vector containing magnetometer data for x, y, z axis
//
//***************************************************************************************************
lsm9ds0Vector_t readMag()
{
	// Similar process as accelData()
	uint8_t xlo = I2CReceive(LSM9DS0_ADDRESS_ACCELMAG, LSM9DS0_REGISTER_OUT_X_L_M);
	int16_t xhi = I2CReceive(LSM9DS0_ADDRESS_ACCELMAG, LSM9DS0_REGISTER_OUT_X_H_M);
	uint8_t ylo = I2CReceive(LSM9DS0_ADDRESS_ACCELMAG, LSM9DS0_REGISTER_OUT_Y_L_M);
	int16_t yhi = I2CReceive(LSM9DS0_ADDRESS_ACCELMAG, LSM9DS0_REGISTER_OUT_Y_H_M);
	uint8_t zlo = I2CReceive(LSM9DS0_ADDRESS_ACCELMAG, LSM9DS0_REGISTER_OUT_Z_L_M);
	int16_t zhi = I2CReceive(LSM9DS0_ADDRESS_ACCELMAG, LSM9DS0_REGISTER_OUT_Z_H_M);

	xhi <<= 8; xhi |= xlo;
	yhi <<= 8; yhi |= ylo;
	zhi <<= 8; zhi |= zlo;
	lsm9ds0Vector_t magData;
	magData.x = xhi;
	magData.y = yhi;
	magData.z = zhi;
	return magData;
}

//***************************************************************************************************
//	Function: LSM9DS0Init
//
//	Initializes I2C0 peripheral on the TM4C123G, as well as configures LSM9DS0 registers for
//	device operation
//
//	Parameters:
//		None
//
//	Returns:
//		uint32_t	: 0 is returned if initialization is successful
//
//***************************************************************************************************
uint32_t LSM9DS0Init(void)
{
	//
	// Initialize I2C Module 0
	//

	//Enable I2C module 0
	SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);

	//Reset module
	SysCtlPeripheralReset(SYSCTL_PERIPH_I2C0);

	//Enable GPIO peripheral that contains I2C 0
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

	// Configure the pin muxing for I2C0 functions on port B2 and B3.
	GPIOPinConfigure(GPIO_PB2_I2C0SCL);
	GPIOPinConfigure(GPIO_PB3_I2C0SDA);

	// Select the I2C function for these pins.
	GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_2);
	GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_3);

	// Enable and initialize the I2C0 master module.  Use the system clock for
	// the I2C0 module.  The last parameter sets the I2C data transfer rate.
	// If false the data rate is set to 100kbps and if true the data rate will
	// be set to 400kbps.
	I2CMasterInitExpClk(I2C0_BASE, SysCtlClockGet(), false);

	//Clear I2C FIFOs
	HWREG(I2C0_BASE + I2C_O_FIFOCTL) = 80008000;

	//	Enable accelerometer continuous output
	I2C_regWrite(LSM9DS0_ADDRESS_ACCELMAG, LSM9DS0_REGISTER_CTRL_REG1_XM, 0x67);
	I2C_regWrite(LSM9DS0_ADDRESS_ACCELMAG, LSM9DS0_REGISTER_CTRL_REG5_XM, 0b11110000);

	//	Enable magnetometer continuous output
	I2C_regWrite(LSM9DS0_ADDRESS_ACCELMAG, LSM9DS0_REGISTER_CTRL_REG7_XM, 0b00000000);

	//	Enable gyroscope continuous output
	I2C_regWrite(LSM9DS0_ADDRESS_GYRO, LSM9DS0_REGISTER_CTRL_REG1_G, 0x0F);
	I2C_regWrite(LSM9DS0_ADDRESS_GYRO, LSM9DS0_REGISTER_CTRL_REG2_G, 0x00);
	I2C_regWrite(LSM9DS0_ADDRESS_GYRO, LSM9DS0_REGISTER_CTRL_REG3_G, 0x88);
	I2C_regWrite(LSM9DS0_ADDRESS_GYRO, LSM9DS0_REGISTER_CTRL_REG5_G, 0x10);

	//	Configuring accelerometer range
	uint8_t regData = I2CReceive(LSM9DS0_ADDRESS_ACCELMAG, LSM9DS0_REGISTER_CTRL_REG2_XM);
	regData &= ~(0b00111000);
	regData |= LSM9DS0_ACCELRANGE_2G;
	I2C_regWrite(LSM9DS0_ADDRESS_ACCELMAG, LSM9DS0_REGISTER_CTRL_REG2_XM, regData);

	//	Configuring magnetometer gain
	regData = I2CReceive(LSM9DS0_ADDRESS_ACCELMAG, LSM9DS0_REGISTER_CTRL_REG6_XM);
	regData &= ~(0b01100000);
	regData |= LSM9DS0_MAGGAIN_2GAUSS;
	I2C_regWrite(LSM9DS0_ADDRESS_ACCELMAG, LSM9DS0_REGISTER_CTRL_REG6_XM, regData);

	//	Configuring gyroscope scale
	regData = I2CReceive(LSM9DS0_ADDRESS_GYRO, LSM9DS0_REGISTER_CTRL_REG4_G);
	regData &= ~(0b00110000);
	regData |= LSM9DS0_GYROSCALE_245DPS;
	I2C_regWrite(LSM9DS0_ADDRESS_GYRO, LSM9DS0_REGISTER_CTRL_REG4_G, regData);

    //
    // Success.
    //
    return(0);
}
