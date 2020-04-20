/*
 *	@Name: main.c
 *	@Author Name: Nitik Gupta
 *	@Description: Main function
 */
#include "gecko_configuration.h"
#include "gpio.h"
#include "native_gecko.h"
#include "em_letimer.h"
#include "em_cmu.h"
#include "math.h"
#include "sleep.h"
#include "Timer_Module.h"
#include "em_emu.h"
//#include "I2C_Comm.h"
#include "VNCL4040_I2C_Comm.h"
#include "log.h"
#include "Load_PM.h"
#include "em_core.h"
/** LETIMER initialization structure. */
#define Lowest_Energy_Mode sleepEM2

float temperature;
uint8_t flag=0;

int appMain(gecko_configuration_t *config)
{
  // Initialize stack
  CORE_irqState_t IRQ_State;
  /*
   * ENTER YOUR SLEEP MODE HERE
   */
  const SLEEP_EnergyMode_t sleep_mode_Running=Lowest_Energy_Mode;    //Enter the Mode you want the sleep mode to be running
  /*
   *  DONT CHANGE BELOW THIS
   */
  logInit();
  gecko_init(config);
  gpioInit();
  Oscillator_Init(sleep_mode_Running);
  Letimer_init();
  NVIC_EnableIRQ(LETIMER0_IRQn);
  LETIMER_IntEnable(LETIMER0,0x05);
  Sleep_Init(sleep_mode_Running);
  GPIO_ExtIntConfig(Interrupt_port,Interrupt_pin,8,false,true,true);
  NVIC_EnableIRQ(GPIO_EVEN_IRQn);
  LETIMER_Enable(LETIMER0,true);

  /* Infinite loop */
  while (1)
  {
	  IRQ_State=CORE_EnterCritical();
	  if(flag==1)
	  {	  flag=0;
		  temperature=measure_temp();
		  logFlush();
	  }
	  CORE_ExitCritical(IRQ_State);
	  if(sleep_mode_Running!=sleepEM0)
	  {
		  SLEEP_Sleep();
	  }
  }
}
void GPIO_EVEN_IRQHandler()
{	gecko_external_signal(gecko_evt_system_external_signal_id);
	GPIO_IntClear(0x40);
}
