/*
 * gpio.h
 *
 *  Created on: Dec 12, 2018
 *      Author: Dan Walkes
 */

#ifndef SRC_GPIO_H_
#define SRC_GPIO_H_
#include <stdbool.h>

#define GPIO_SET_DISPLAY_EXT_COMIN_IMPLEMENTED 	1
#define GPIO_DISPLAY_SUPPORT_IMPLEMENTED		1
#define Push_Button_Port0 5
#define Push_Button_Pin0 6
#define Push_Button_Port1 5
#define Push_Button_Pin1 7
#define Interrupt_port 0
#define Interrupt_pin 3
void gpioInit();
void gpioLed0SetOn();
void gpioLed0SetOff();
void gpioLed1SetOn();
void gpioLed1SetOff();
void Button_Handler();
//@name:gpioEnableDisplay
//@brief:Enables the EXTCOMIN pin
void gpioEnableDisplay();
//@name:gpioSetDisplayExtcomin
//@brief:Enables the EXTCOMIN pin
void gpioSetDisplayExtcomin(bool high);
#endif /* SRC_GPIO_H_ */
