/*
 * Clock_Modes.h
 *
 *@Brief: This .h file contains all the functions required to initialize, enable perform all the functions required to complete the Assignment
 *  Created on: Jan 30, 2020
 *      Author: gupta
 */

#ifndef SRC_TIMER_MODULE_H_
#define SRC_TIMER_MODULE_H_

#include "gecko_configuration.h"
#include "gpio.h"
#include "native_gecko.h"
#include "em_letimer.h"
#include "em_cmu.h"
#include "math.h"
#include "sleep.h"
#include "SI1145_I2C_Comm.h"
 /*
   * ENTER YOUR TIME VARIABLES HERE, For the time between each I2C Read
   */
#define Time_Period 1000
/*
   * DON'T CHANGE BELOW THIS
   */
#define Max_Count 65535		// Max Count
#define ULFreq 1000			// Ultra Low Freq
#define LFreq 32768			// Low Freq
#define Min_Time 0		// Min Time allowed
#define Max_On_Time 1000		//Max Time Allowed
#define Max_Time_Period 7000
#ifndef On_Time
	#define On_Time 175		// Default 175ms On time
#endif
#ifndef Time_Period
	#define Time_Period 2250	// Default 2250 Time Period
#endif
#if (On_Time<Min_Time)
	#define On_Time 175
#endif
#if (Time_Period<Min_Time)
	#define Time_Period 2250
#endif
#if (On_Time>Max_On_Time)
	#define On_Time 175
#endif
//#if (Time_Period>Max_Time_period)
//	#define Time_Period 2250
//#endif

//@Brief: Initializing the Oscillator according the energy mode
void Oscillator_Init(SLEEP_EnergyMode_t Energy_Mode);
//@Brief: Initializing the Letimer
void Letimer_init();
//@Brief: Initializing the Sleep mode according the energy mode
void Sleep_Init(SLEEP_EnergyMode_t Energy_Mode);
//@Brief: Convert the time to the count value for the timer according to the frequency and divider
uint32_t Time_To_Count(uint16_t time);
//@Brief: Receives the divider for the frequency required
uint16_t Get_Divisor();
//@Brief: Blocks the timer for time given
void Wait_milli(uint16_t tim_mill);
//@Brief: Blocks the timer for time given
void us_wait(uint32_t time_micro);
//@Brief: IRQ handler for the letimer0
uint32_t Time_To_Count_us(uint32_t time);
#endif /* SRC_TIMER_MODULE_H_ */
