/***************************************************************************//**
 * @file
 * @brief Silicon Labs Bluetooth mesh light example
 * This example implements a Bluetooth mesh light node.
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

/* Board headers */
#include "init_mcu.h"
#include "init_board.h"
#include "init_app.h"
#include "board_features.h"
#include "retargetserial.h"

/* Bluetooth stack headers */
#include "native_gecko.h"
#include "gatt_db.h"
#include <gecko_configuration.h>
#include <mesh_sizes.h>
#include "gpiointerrupt.h"
/* Libraries containing default Gecko configuration values */
#include <em_gpio.h>
#include <em_core.h>
/* Coex header */
#include "coexistence-ble.h"

/* Device initialization header */
#include "hal-config.h"

/* Application code */
#include "app.h"

#if defined(HAL_CONFIG)
#include "bsphalconfig.h"
#else
#include "bspconfig.h"
#endif

#include "src/ble_mesh_device_type.h"
//#include "src/I2C_Comm.h"
#include "src/Load_PM.h"
#include "src/VNCL4040_I2C_Comm.h"
//#include "src/I2C_Comm_Interrupt.h"
//#include "src/State_Machine.h"
#include "src/Timer_Module.h"
#include "src/display.h"
#include "src/gpio.h"
#include "src/log.h"

/***********************************************************************************************//**
 * @addtogroup Application
 * @{
 **************************************************************************************************/

/***********************************************************************************************//**
 * @addtogroup app
 * @{
 **************************************************************************************************/

bool mesh_bgapi_listener(struct gecko_cmd_packet *evt);

/// Maximum number of simultaneous Bluetooth connections
#define MAX_CONNECTIONS 2
#define Lowest_Energy_Mode sleepEM3
/// Heap for Bluetooth stack
uint8_t bluetooth_stack_heap[DEFAULT_BLUETOOTH_HEAP(MAX_CONNECTIONS) + BTMESH_HEAP_SIZE + 1760];

/// Bluetooth advertisement set configuration
///
/// At minimum the following is required:
/// * One advertisement set for Bluetooth LE stack (handle number 0)
/// * One advertisement set for Mesh data (handle number 1)
/// * One advertisement set for Mesh unprovisioned beacons (handle number 2)
/// * One advertisement set for Mesh unprovisioned URI (handle number 3)
/// * N advertisement sets for Mesh GATT service advertisements
/// (one for each network key, handle numbers 4 .. N+3)
///
#define MAX_ADVERTISERS (4 + MESH_CFG_MAX_NETKEYS)

/// Priorities for bluetooth link layer operations
static gecko_bluetooth_ll_priorities linklayer_priorities = GECKO_BLUETOOTH_PRIORITIES_DEFAULT;

/// Bluetooth stack configuration
const gecko_configuration_t config =
{
  .bluetooth.max_connections = MAX_CONNECTIONS,
  .bluetooth.max_advertisers = MAX_ADVERTISERS,
  .bluetooth.heap = bluetooth_stack_heap,
  .bluetooth.heap_size = sizeof(bluetooth_stack_heap) - BTMESH_HEAP_SIZE,
#if defined(FEATURE_LFXO)
  .bluetooth.sleep_clock_accuracy = 100, // ppm
#elif defined(PLFRCO_PRESENT) || defined(LFRCO_PRESENT)
  .bluetooth.sleep_clock_accuracy = 500, // ppm
#endif
  .bluetooth.linklayer_priorities = &linklayer_priorities,
  .gattdb = &bg_gattdb_data,
  .btmesh_heap_size = BTMESH_HEAP_SIZE,
  .pa.config_enable = 1, // Set this to be a valid PA config
#if defined(FEATURE_PA_INPUT_FROM_VBAT)
  .pa.input = GECKO_RADIO_PA_INPUT_VBAT, // Configure PA input to VBAT
#else
  .pa.input = GECKO_RADIO_PA_INPUT_DCDC,
#endif // defined(FEATURE_PA_INPUT_FROM_VBAT)
  .max_timers = 16,
  .rf.flags = GECKO_RF_CONFIG_ANTENNA,   // Enable antenna configuration.
  .rf.antenna = GECKO_RF_ANTENNA,   // Select antenna path!
};

uint8_t flag=0;
uint8_t Gpio_flag=0;
uint8_t Proximity_flag=0;
/*******************************************************************************************************
 * Main function.
 * Instructions to proceed for the mesh assignment.
 * 1. Add the gpio enable display function in gpio.c & gpio.h from Assignment 6.
 * 2. Complete displayUpdate() function in display.c similar to instructions in Assignment 6.
 * 3. Add your logic for loggerGetTimestamp() function in log.c from assignment 4.
 * 4. You can leverage your assignment files for timers, cmu etc. for this assignment.
 *
 * After completing above steps check for its functionality and proceed to mesh implementation.
 * 1. Use compile time switch in ble_mesh_device_type.h file to switch between publisher and subscriber.
 * 2. Add appropriate initializations in main before while loop.
 * 3. Then proceed to app.c for further instructions.
 *******************************************************************************************************/
void PB0Handler();
void PB1Handler();
void Proximity_Handler();
int main(void)
{
	const SLEEP_EnergyMode_t sleep_mode_Running=Lowest_Energy_Mode;    //Enter the Mode you want the sleep mode to be running
	CORE_irqState_t IRQ_State;
  // Initialize device
  initMcu();
  logInit();
  // Initialize board
  initBoard();
  // Initialize application
  initApp();
  initVcomEnable();

  // Minimize advertisement latency by allowing the advertiser to always
  // interrupt the scanner.
  linklayer_priorities.scan_max = linklayer_priorities.adv_min + 1;

  gecko_stack_init(&config);
  //gecko_init(config);
  gpioInit();
  Oscillator_Init(sleep_mode_Running);
    Letimer_init();
    //NVIC_EnableIRQ(LETIMER0_IRQn);
    LETIMER_IntEnable(LETIMER0,LETIMER_IFC_COMP0 | LETIMER_IFC_COMP1 | LETIMER_IFC_UF);
    Sleep_Init(sleep_mode_Running);
    //LETIMER_Enable(LETIMER0,true);
    displayInit();
    GPIOINT_Init();
    GPIO_ExtIntConfig(Push_Button_Port0,Push_Button_Pin0,6,true,false,true);
    GPIOINT_CallbackRegister(Push_Button_Pin0,PB0Handler);
    GPIO_ExtIntConfig(Push_Button_Port0,Push_Button_Pin1,7,true,false,true);
    GPIOINT_CallbackRegister(Push_Button_Pin1,PB1Handler);
    GPIO_ExtIntConfig(Interrupt_port,Interrupt_pin,8,true,false,true);
    GPIOINT_CallbackRegister(Interrupt_pin,Proximity_Handler);
    GPIO_IntEnable(1<<Push_Button_Pin0);
    GPIO_IntEnable(1<<Push_Button_Pin1);
    GPIO_IntEnable(1<<Interrupt_pin);
    NVIC_EnableIRQ(LETIMER0_IRQn);
  // Initialize the bgapi classes
  if( DeviceUsesClientModel() ){
	  gecko_bgapi_classes_init_client_lpn();
  }
  else {
	  gecko_bgapi_classes_init_server_friend();
  }

  // Initialize coexistence interface. Parameters are taken from HAL config.
  gecko_initCoexHAL();

  while (1) {
	  IRQ_State=CORE_EnterCritical();
	  	  if(flag==1)
	  	  {	  flag=0;
	  		  measure_temp();	//Change the name for submission
	  		  logFlush();
	  	  }
	  	  CORE_ExitCritical(IRQ_State);
	  	  if(sleep_mode_Running!=sleepEM0)
	  	  {
				struct gecko_cmd_packet *evt = gecko_wait_event();
				bool pass = mesh_bgapi_listener(evt);
				if (pass) {
				  handle_ecen5823_gecko_event(BGLIB_MSG_ID(evt->header), evt);
			}
	  	  }
  }
}
//void GPIO_EVEN_IRQHandler()
//{
//	gecko_external_signal(gecko_evt_system_external_signal_id);
//	if(Gpio_flag == 0 )
//	{
//		Gpio_flag =1;
//	}
//	else if(Gpio_flag == 1)
//	{
//		Gpio_flag =0;
//	}
//	GPIO_IntClear(0x40);
//}
void PB0Handler()
{
	gecko_external_signal(gecko_evt_system_external_signal_id);
	Gpio_flag=1;
	GPIO_IntClear(0x40);
}
void PB1Handler()
{
	gecko_external_signal(gecko_evt_system_external_signal_id);
	Gpio_flag=2;
	if(Proximity_flag == 2)
	{
		Proximity_flag =3;
	}
	GPIO_IntClear(0x40);
}
void Proximity_Handler()
{
	gecko_external_signal(gecko_evt_system_external_signal_id);
	Proximity_flag=1;
	GPIO_IntClear(0x40);
}
