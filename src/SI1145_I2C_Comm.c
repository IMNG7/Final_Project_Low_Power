/*
 * VNCL4040_I2C_Comm.c
 *
 *  Created on: Apr 18, 2021
 *      Author: gupta
 */
#include <src/SI1145_I2C_Comm.h>
I2CSPM_Init_TypeDef SI1145_i2c_init_def=
{
		I2C0,                       /* Use I2C instance 0 */                       \
		gpioPortC,                  /* SCL port */                                 \
		10,             /* SCL pin */                                  \
		gpioPortC,                  /* SDA port */                                 \
		11,             /* SDA pin */                                  \
		PORT_SCL_LOCATION,             /* Location of SCL */                          \
		PORT_SDA_LOCATION,             /* Location of SDA */                          \
		0,                          /* Use currently configured reference clock */ \
	    I2C_FREQ_STANDARD_MAX,      /* Set to standard rate  */                    \
	    i2cClockHLRStandard,        /* Set to use 4:4 low/high duty cycle */       \
};
I2C_TransferSeq_TypeDef i2c_transfer_message;
I2C_TransferReturn_TypeDef return_state;
volatile extern uint8_t Proximity_flag;
/*
 * @Name: i2c_initialize()
 * @Brief: Initializing I2C according to the I2C Configuring
 * @Arguments: None
 * @returns: None
 */
void SI1145_i2c_initialize()
{	uint8_t tmp[1];
	uint8_t tmp1[2];
	uint8_t t1[1];
	I2CSPM_Init(&SI1145_i2c_init_def);
	us_wait(100000);
	t1[0]=0x18;
//	tmp[0]=0x00;
	tmp[0]=0x01;
	SI1145_i2c_transfer_cmd_reg_write(t1,tmp);	// Resetting SI1145
	us_wait(10000);
	t1[0]=0x07;
//	tmp[0]=0x00;
	tmp[0]=0x17;
	SI1145_i2c_transfer_cmd_reg_write(t1,tmp); // Resetting Sequence
	us_wait(10000);
	tmp[0]=0x17;
//	tmp[1]=0x00;
	tmp[0]=0x00;
	SI1145_i2c_transfer_cmd_reg_write(t1,tmp);	// Selecting Small Diode for IR Param
//	us_wait(10000);
	t1[0]=0x18;
//	tmp[1]=0x00;
	tmp[0]=0xAE;
	SI1145_i2c_transfer_cmd_reg_write(t1,tmp);	// Selecting Small Diode for IR CMD
	us_wait(10000);
	t1[0]=0x08;
	tmp1[1]=0x00;
	tmp1[0]=0xFF;
	SI1145_i2c_transfer_cmd_reg_write_double(t1,tmp1);	// Selecting Measurement Time
	us_wait(10000);
//	t1[0]=0x1E;
//	tmp[0]=0x00;
//	tmp[1]=0x00;
//	i2c_transfer_cmd_reg_write(t1,tmp);	// Giving ADC gain 0
//	us_wait(10000);
//	t1[0]=0x1E;
//	tmp[0]=0x00;
//	tmp[1]=0x00;
//	i2c_transfer_cmd_reg_write(t1,tmp);	// Giving ADC gain 0
//	us_wait(10000);
//	t1[0]=0x1D;
//	tmp[0]=0x00;
//	tmp[1]=0x00;
//	i2c_transfer_cmd_reg_write(t1,tmp);	//Giving ADC COunter 0(According to Datasheet)
//	us_wait(10000);
	t1[0]=0x17;
//	tmp[0]=0x00;
	tmp[0]=0x20;
	SI1145_i2c_transfer_cmd_reg_write(t1,tmp);	// Enabling High Signal IR Range Param
	us_wait(10000);
	t1[0]=0x18;
//	tmp[0]=0x00;
	tmp[0]=0xBF;
	SI1145_i2c_transfer_cmd_reg_write(t1,tmp);	// Enabling High Signal IR Range Command
	us_wait(10000);
	t1[0]=0x17;
//	tmp[0]=0x00;
	tmp[0]=0xB1;
	SI1145_i2c_transfer_cmd_reg_write(t1,tmp);	// Setting Channel List Param
	us_wait(10000);
	t1[0]=0x18;
//	tmp[0]=0x00;
	tmp[0]=0xA1;
	SI1145_i2c_transfer_cmd_reg_write(t1,tmp);	// Setting Channel List Command
	us_wait(10000);
	t1[0]=0x18;
//	tmp[0]=0x00;
	tmp[0]=0x8F;
	SI1145_i2c_transfer_cmd_reg_write(t1,tmp);	// Setting AUTO mode, and enabling ALS, IR, PS mode
	us_wait(10000);
}
void SI1145_i2c_transfer_cmd_reg_write(uint8_t* cmd_reg,uint8_t* data)
{	i2c_transfer_message.addr=VNCL4040_I2C_BUS_ADDRESS<<1;
	i2c_transfer_message.flags=I2C_FLAG_WRITE_WRITE;
	i2c_transfer_message.buf[0].data=cmd_reg;
	i2c_transfer_message.buf[0].len=Single_Byte;
	i2c_transfer_message.buf[1].data=data;
	i2c_transfer_message.buf[1].len=Single_Byte;
	return_state=I2CSPM_Transfer(I2C0,&i2c_transfer_message);
	if(return_state!=0)
	{
//		LOG_ERROR("\n\rTransfer Failed\n\r");
	}
	else
	{
		//LOG_INFO("\n\rWrite Successful\n\r");
	}
}
void SI1145_i2c_transfer_cmd_reg_write_double(uint8_t* cmd_reg,uint8_t* data)
{	i2c_transfer_message.addr=VNCL4040_I2C_BUS_ADDRESS<<1;
	i2c_transfer_message.flags=I2C_FLAG_WRITE_WRITE;
	i2c_transfer_message.buf[0].data=cmd_reg;
	i2c_transfer_message.buf[0].len=Single_Byte;
	i2c_transfer_message.buf[1].data=data;
	i2c_transfer_message.buf[1].len=Double_Byte;
	return_state=I2CSPM_Transfer(I2C0,&i2c_transfer_message);
	if(return_state!=0)
	{
//		LOG_ERROR("\n\rTransfer Failed\n\r");
	}
	else
	{
		//LOG_INFO("\n\rWrite Successful\n\r");
	}
}
void SI1145_i2c_transfer_write(uint8_t* message)
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
void SI1145_i2c_transfer_cmd_reg_read(uint8_t* cmd_reg,uint8_t* data)
{	i2c_transfer_message.addr=VNCL4040_I2C_BUS_ADDRESS<<1;
	i2c_transfer_message.flags=I2C_FLAG_WRITE_READ;
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
		//LOG_INFO("\n\rRead Successful\n\r");
	}
}
/*
 * @Name: i2c_transfer_read()
 * @Brief: Reading from the registers from the I2C
 * @Arguments: The buffer to be written to
 * @returns: None
 */
void SI1145_i2c_transfer_read(uint8_t* tmp)
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
void SI1145_measure_Prox()
{
	uint8_t tmp[2];
	uint16_t temp;
	uint8_t t1[1];
	SI1145_i2c_initialize();
	us_wait(10000);
	t1[0]=0x24;
	SI1145_i2c_transfer_write(t1);
//	i2c_transfer_cmd_reg_read(t1,tmp);
	us_wait(10000);
	SI1145_i2c_transfer_read(tmp);
	us_wait(10000);
	temp=(tmp[1]<<8)+tmp[0];			// there are 2 buffers for Higher and Lower byte.
	LOG_INFO("\n\r Value:%d",temp);
	if(temp>200)
	{
//		gecko_external_signal(gecko_evt_system_external_signal_id);
//		Proximity_flag =1;
	}
//	t1[0]=0x07;
//	i2c_transfer_cmd_reg_read(t1,tmp);
//	us_wait(10000);
//	temp=(tmp[1]<<8)+tmp[0];			// there are 2 buffers for Higher and Lower byte.
//	LOG_INFO("\n\r Value:%d",temp);
}

