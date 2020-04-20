/*
 * @File Name: Load_PM.c
 * @Description: This .c file contains functions for the Load Power Management
 *  Created on: Feb 8, 2020
 *      Author: Nitik gupta
 */

#include "Load_PM.h"
/*
 * @Name: Enable_Device()
 * @Brief: Enable the pins of the I2C Sensor
 * @Arguments: None
 * @returns: None
 */
void Enable_Device()
{
	GPIO_PinModeSet(Sensor_Enable_PORT, Sensor_Enable_PIN, gpioModePushPull, true);
	GPIO_PinOutSet(Sensor_Enable_PORT,Sensor_Enable_PIN);
}
/*
 * @Name: Disable_Device()
 * @Brief: Disable the pins of the I2C Sensor and the I2C Pins
 * @Arguments: None
 * @returns: None
 */
void Disable_Device()
{
	GPIO_PinOutClear(Sensor_Enable_PORT,Sensor_Enable_PIN);
	GPIO_PinModeSet(Sensor_Enable_PORT, Sensor_Enable_PIN, gpioModeDisabled, false);
	GPIO_PinModeSet(SDA_Enable_PORT, SDA_Enable_PIN, gpioModeDisabled, false);
	GPIO_PinModeSet(SCK_Enable_PORT, SCK_Enable_PIN, gpioModeDisabled, false);
}
