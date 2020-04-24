/*
 * @File Name:Timer_Module.c
 *	@Brief: All the functions involving initalization of timer, oscillator, etc.
 *  Created on: Jan 30, 2020
 *      Author: gupta
 */
#include "Timer_Module.h"
#include "stdint.h"
CMU_Osc_TypeDef Oscillator_Type;
uint32_t Divisor;
extern uint8_t flag;
// Structure for the initialization of the Letimer
LETIMER_Init_TypeDef LETIMER_Init_Struct=
{
  .enable = false,         /**< Start counting when initialization completes. */
  .debugRun = false,       /**< Counter shall keep running during debug halt. */
  .comp0Top=false,       /**< Load COMP0 register into CNT when counter underflows. */
  .bufTop=false,         /**< Load COMP1 into COMP0 when REP0 reaches 0. */
  .out0Pol=0,        /**< Idle value for output 0. */
  .out1Pol=0,        /**< Idle value for output 1. */
  .ufoa0=letimerUFOANone,          /**< Underflow output 0 action. */
  .ufoa1=letimerUFOANone,          /**< Underflow output 1 action. */
  .repMode=letimerRepeatFree,        /**< Repeat mode. */
  //.topValue=(pow(2,16)-1)       /**< Top value. Counter wraps when top value matches counter value is reached. */
};
/*
 * @Name: Oscillator_init
 * @Brief: Initializing the Oscillator according the energy mode
 * @Arguments: Energy mode Required
 * @returns: None
 */
void Oscillator_Init(SLEEP_EnergyMode_t Energy_Mode)
{
	if(Energy_Mode==sleepEM1 || Energy_Mode==sleepEM2 || Energy_Mode==sleepEM0)	//
	{
		Oscillator_Type=cmuOsc_LFXO;
	}
	else if(Energy_Mode==sleepEM3 )
	{
		Oscillator_Type=cmuOsc_ULFRCO;
	}
	CMU_OscillatorEnable(Oscillator_Type,true,true);
	CMU_ClockSelectSet(cmuClock_LFA,Oscillator_Type);
	CMU_ClockEnable(cmuClock_LFA,true);
	CMU_ClockEnable(cmuClock_LETIMER0,true);
}
/*
 * @Name: Letimer_init
 * @Brief: Initializing the Letimer
 * @Arguments: None
 * @returns: None
 */
void Letimer_init()
{	uint32_t time1;
	Divisor=Get_Divisor();
	CMU_ClockDivSet(cmuClock_LETIMER0,Divisor);
	LETIMER_Init(LETIMER0,&LETIMER_Init_Struct);
	time1=Time_To_Count(Time_Period);
	time1=Max_Count-time1;
	LETIMER_CompareSet(LETIMER0,0,time1);
}
/*
 * @Name: Sleep_init
 * @Brief: Initializing the Sleep mode according the energy mode
 * @Arguments: Energy mode Required
 * @returns: None
 */
void Sleep_Init(SLEEP_EnergyMode_t Energy_Mode)
{
	if(Energy_Mode==sleepEM1)
	{
		SLEEP_SleepBlockBegin(sleepEM2);
	}
	else if(Energy_Mode==sleepEM2)
	{
		SLEEP_SleepBlockBegin(sleepEM3);
	}
	else
	{
		//Nothing Here
	}
}
/*
 * @Name: Time_To_Count
 * @Brief: Convert the time to the count value for the timer according to the frequency and divider
 * @Arguments: time in ms
 * @returns: the count value
 */
uint32_t Time_To_Count(uint16_t time)
{	uint32_t count;
	uint16_t freq;
	if(Oscillator_Type==cmuOsc_LFXO)
	{
		freq=LFreq;
	}
	else if(Oscillator_Type==cmuOsc_ULFRCO)
	{
		freq=LFreq;
	}
	count=(time*freq)/Divisor;
	return count/1000;	// Divide by 1000 to because time passed in ms
}
/*
 * @Name: Get_Divisor
 * @Brief: Receives the divider for the frequency required
 * @Arguments: None
 * @returns: Divider Value
 */
uint16_t Get_Divisor()
{	uint8_t i=1;
	uint32_t count;
	uint16_t freq;
	if(Oscillator_Type==cmuOsc_LFXO)
	{
		freq=LFreq;
	}
	else if(Oscillator_Type==cmuOsc_ULFRCO)
	{
		freq=LFreq;
	}
	count=(Time_Period)*freq/i;
	while(count/1000>Max_Count) // Divide by 1000 to because time passed in ms
	{
		i*=2;
		count=((Time_Period)*freq/i);
	}
	return i;
}
/*
 * @Name: Wait_milli
 * @Brief: Blocks the timer for time given
 * @Arguments: time_milli
 * @returns: None
 */
void Wait_milli(uint16_t time_milli)
{
	uint32_t cur_cnt = LETIMER_CounterGet(LETIMER0);
	while(LETIMER_CounterGet(LETIMER0) >= (cur_cnt - Time_To_Count(time_milli)));
}
/*
 * @Name: Wait_micro
 * @Brief: Blocks the timer for time given
 * @Arguments: time_micro
 * @returns: None
 */
void us_wait(uint32_t time_micro)
{
	uint32_t cur_cnt = LETIMER_CounterGet(LETIMER0);
	time_micro=time_micro/1000;
	while(LETIMER_CounterGet(LETIMER0) >= (cur_cnt - Time_To_Count(time_micro)));
}
/*
 * @Name: LETIMER0_IRQHandler
 * @Brief: IRQ handler for the letimer0
 * @Arguments: None
 * @returns: None
 */
void LETIMER0_IRQHandler(void)
{
	LETIMER_IntClear(LETIMER0,0x05);
	flag=1;
	LETIMER_CounterSet(LETIMER0,65535);
}
