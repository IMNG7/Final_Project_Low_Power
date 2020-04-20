/*
 * @File Name: Load_PM.c
 * @Description: This .h file contains functions for the Load Power Management
 *  Created on: Feb 8, 2020
 *      Author: Nitik gupta
 */

#ifndef SRC_LOAD_PM_H_
#define SRC_LOAD_PM_H_
#include "gpio.h"
#include "em_gpio.h"
#include "stdbool.h"
#include "Timer_Module.h"


#define Sensor_Enable_PORT 3		// Sensor Port and Pins
#define Sensor_Enable_PIN 15
#define SDA_Enable_PORT 2			// I2C pins on EFR32
#define SDA_Enable_PIN 10
#define SCK_Enable_PORT 2
#define SCK_Enable_PIN 11

//@Brief: Enable the pins of the I2C Sensor
void Enable_Device();
// @Brief: Disable the pins of the I2C Sensor and the I2C Pins
void Disable_Device();
#endif /* SRC_LOAD_PM_H_ */
