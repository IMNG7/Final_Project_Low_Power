/*
 * VNCL4040_I2C_Comm.h
 *
 *  Created on: Apr 18, 2020
 *      Author: gupta
 */

#ifndef SRC_VNCL4040_I2C_COMM_H_
#define SRC_VNCL4040_I2C_COMM_H_

#include "i2cspm.h"
#include "stdint.h"
#include "log.h"
#include "Timer_Module.h"
#include "Load_PM.h"
#define VNCL4040_I2C_BUS_ADDRESS 0x60
#define SI7021_Read_Temperature_Mode	0xF3
#define PORT_SDA_LOCATION	16
#define PORT_SCL_LOCATION	14
#define Single_Byte 1
#define Double_Byte 2

//@Brief: Initializing I2C according to the I2C Configuring
void i2c_initialize();
//@Brief: Writing to the registers from the I2C
void i2c_transfer_write(uint8_t* message);
//@Brief: Reading from the registers from the I2C
void i2c_transfer_read(uint8_t* tmp);
void i2c_transfer_cmd_reg_write(uint8_t* cmd_reg,uint8_t* data);
//@Brief: Measuring the temperature from the inbuilt temperature sensor
float measure_temp();
//@Brief: Converting the Raw Data to celcius
float convert_to_celcius(uint16_t temp);

#endif /* SRC_VNCL4040_I2C_COMM_H_ */
