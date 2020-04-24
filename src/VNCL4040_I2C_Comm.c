/*
 * VNCL4040_I2C_Comm.c
 *
 *  Created on: Apr 18, 2020
 *      Author: gupta
 */
#include "VNCL4040_I2C_Comm.h"
I2CSPM_Init_TypeDef i2c_init_def=
{
		I2C0,                       /* Use I2C instance 0 */                       \
		gpioPortC,                  /* SCL port */                                 \
		SDA_Enable_PIN,             /* SCL pin */                                  \
		gpioPortC,                  /* SDA port */                                 \
		SCK_Enable_PIN,             /* SDA pin */                                  \
		PORT_SCL_LOCATION,             /* Location of SCL */                          \
		PORT_SDA_LOCATION,             /* Location of SDA */                          \
		0,                          /* Use currently configured reference clock */ \
	    I2C_FREQ_STANDARD_MAX,      /* Set to standard rate  */                    \
	    i2cClockHLRStandard,        /* Set to use 4:4 low/high duty cycle */       \
};
I2C_TransferSeq_TypeDef i2c_transfer_message;
I2C_TransferReturn_TypeDef return_state;
uint8_t No_Hold_Cmd=0xF3;
/*
 * @Name: i2c_initialize()
 * @Brief: Initializing I2C according to the I2C Configuring
 * @Arguments: None
 * @returns: None
 */
void i2c_initialize()
{	uint8_t tmp[2];
	uint16_t temp;
	uint8_t t1[1];
	I2CSPM_Init(&i2c_init_def);
	us_wait(80000);
	tmp[0]=0x04;
	tmp[1]=0x10;
	tmp[2]=0x00;
	i2c_transfer_cmd_reg_write(t1,tmp);
	us_wait(10000);
	tmp[0]=0x03;
	tmp[1]=0x00;
	tmp[2]=0x01;
	i2c_transfer_cmd_reg_write(t1,tmp);
	us_wait(10000);
	tmp[0]=0x00;
	tmp[1]=0x00;
	tmp[2]=0x00;
	i2c_transfer_cmd_reg_write(t1,tmp);
	us_wait(10000);
	tmp[0]=0x06;
	tmp[1]=0xFF;
	tmp[2]=0x3F;
	i2c_transfer_cmd_reg_write(t1,tmp);
	us_wait(10000);
	tmp[0]=0x07;
	tmp[1]=0x00;
	tmp[2]=0x00;
	i2c_transfer_cmd_reg_write(t1,tmp);
	us_wait(10000);
}
/*
 * @Name: i2c_transfer_write()
 * @Brief: Writing to the registers from the I2C
 * @Arguments: The value to be given
 * @returns: None
 */
void i2c_transfer_write(uint8_t* message)
{	i2c_transfer_message.addr=VNCL4040_I2C_BUS_ADDRESS<<1;
	i2c_transfer_message.flags=I2C_FLAG_WRITE;
	i2c_transfer_message.buf[0].data=message;
	i2c_transfer_message.buf[0].len=Single_Byte;
	return_state=I2CSPM_Transfer(I2C0,&i2c_transfer_message);
	if(return_state!=0)
	{
		LOG_ERROR("\n\rTransfer Failed\n\r");
	}
	else
	{
		LOG_INFO("\n\rWrite Successful\n\r");
	}
}

void i2c_transfer_cmd_reg_write(uint8_t* cmd_reg,uint8_t* data)
{	i2c_transfer_message.addr=VNCL4040_I2C_BUS_ADDRESS<<1;
	i2c_transfer_message.flags=I2C_FLAG_WRITE;
	i2c_transfer_message.buf[0].data=data;
	i2c_transfer_message.buf[0].len=3;
//	i2c_transfer_message.buf[1].data=data;
//	i2c_transfer_message.buf[1].len=Double_Byte;
	return_state=I2CSPM_Transfer(I2C0,&i2c_transfer_message);
	if(return_state!=0)
	{
		LOG_ERROR("\n\rTransfer Failed\n\r");
	}
	else
	{
		LOG_INFO("\n\rWrite Successful\n\r");
	}
}
void i2c_transfer_cmd_reg_read(uint8_t* cmd_reg,uint8_t* data)
{	i2c_transfer_message.addr=VNCL4040_I2C_BUS_ADDRESS<<1;
	i2c_transfer_message.flags=I2C_FLAG_WRITE;
	i2c_transfer_message.buf[0].data=cmd_reg;
	i2c_transfer_message.buf[0].len=Single_Byte;
	i2c_transfer_message.buf[1].data=data;
	i2c_transfer_message.buf[1].len=Double_Byte;
	return_state=I2CSPM_Transfer(I2C0,&i2c_transfer_message);
	if(return_state!=0)
	{
		LOG_ERROR("\n\rTransfer Failed\n\r");
	}
	else
	{
		LOG_INFO("\n\rRead Successful\n\r");
	}
}
/*
 * @Name: i2c_transfer_read()
 * @Brief: Reading from the registers from the I2C
 * @Arguments: The buffer to be written to
 * @returns: None
 */
void i2c_transfer_read(uint8_t* tmp)
{	i2c_transfer_message.addr=VNCL4040_I2C_BUS_ADDRESS<<1;
	i2c_transfer_message.flags=I2C_FLAG_READ;
	i2c_transfer_message.buf[0].data=tmp;
	i2c_transfer_message.buf[0].len=Double_Byte;
	return_state=I2CSPM_Transfer(I2C0,&i2c_transfer_message);
	if(return_state!=0)
	{
			LOG_ERROR("\n\rTransfer Failed\n\r");
	}
	else
	{
			LOG_INFO("\n\rRead Successful\n\r");
	}
}
/*
 * @Name: measure_temp()
 * @Brief: Measuring the temperature from the inbuilt temperature sensor
 * @Arguments: None
 * @returns: Temperature Value
 */
void measure_temp()
{
	uint8_t tmp[2];
	uint16_t temp;
	uint8_t t1[1];
	float temperature;
	//t1[0]=SI7021_Read_Temperature_Mode;
	i2c_initialize();
	us_wait(10000);
	t1[0]=0x08;
	i2c_transfer_write(t1);
	us_wait(10000);
//	tmp[0]=0x00;
//	tmp[1]=0x00;
	i2c_transfer_read(tmp);
	us_wait(10000);
	temp=(tmp[0]<<8)+tmp[1];			// there are 2 buffers for Higher and Lower byte.
	LOG_INFO("\n\r Value:%d",temp);
	//temperature=convert_to_celcius(temp);
	//return (float)temp;
}
/*
 * @Name: convert_to_celcius
 * @Brief: Converting the Raw Data to celcius
 * @Arguments: The raw value of temperature
 * @returns: The value of temperature in celcius
 */
float convert_to_celcius(uint16_t temp)
{
	float Converted;
	Converted= (175.72*(float)temp/65536)-46.85; 	// Refer the Datasheet
	return Converted;
}



