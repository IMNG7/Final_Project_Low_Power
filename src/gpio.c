/*
 * gpio.c
 *
 *  Created on: Dec 12, 2018
 *      Author: Dan Walkes
 */
#include "gpio.h"
#include "em_gpio.h"
#include "stdbool.h"
#include "log.h"
#include <string.h>


/**
 * TODO: define these.  See the radio board user guide at https://www.silabs.com/documents/login/user-guides/ug279-brd4104a-user-guide.pdf
 * and GPIO documentation at https://siliconlabs.github.io/Gecko_SDK_Doc/efm32g/html/group__GPIO.html
 */
#define	LED0_port 5
#define LED0_pin 4
#define LED1_port 5
#define LED1_pin 5
#define LCD_EXTCOMIN_port 3
#define LCD_EXTCOMIN_pin 13


void gpioInit()
{
	GPIO_DriveStrengthSet(LED0_port, gpioDriveStrengthWeakAlternateStrong);
	//GPIO_DriveStrengthSet(LED0_port, gpioDriveStrengthWeakAlternateWeak);
	GPIO_PinModeSet(LED0_port, LED0_pin, gpioModePushPull, false);
	GPIO_DriveStrengthSet(LED1_port, gpioDriveStrengthWeakAlternateStrong);
	//GPIO_DriveStrengthSet(LED1_port, gpioDriveStrengthWeakAlternateWeak);
	GPIO_PinModeSet(LED1_port, LED1_pin, gpioModePushPull, false);
	GPIO_PinModeSet(Push_Button_Port0, Push_Button_Pin0, gpioModeInputPullFilter, true);
	GPIO_PinModeSet(Push_Button_Port1, Push_Button_Pin1, gpioModeInputPullFilter, true);
	GPIO_PinModeSet(Interrupt_port, Interrupt_pin, gpioModeInputPullFilter, true);
}

void gpioLed0SetOn()
{
	GPIO_PinOutSet(LED0_port,LED0_pin);
}
void gpioLed0SetOff()
{
	GPIO_PinOutClear(LED0_port,LED0_pin);
}
void gpioLed1SetOn()
{
	GPIO_PinOutSet(LED1_port,LED1_pin);
}
void gpioLed1SetOff()
{
	GPIO_PinOutClear(LED1_port,LED1_pin);
}
//@name:gpioEnableDisplay
//@brief:Enables the EXTCOMIN pin
//@returns:void
//@param:none
void gpioEnableDisplay()
{
	GPIO_PinModeSet(LCD_EXTCOMIN_port, LCD_EXTCOMIN_pin, gpioModePushPull, false);
}
//@name:gpioSetDisplayExtcomin
//@brief:Set the pin
//@returns:void
//@param:previous check
void gpioSetDisplayExtcomin(bool high)
{
	if(high)
	{
		GPIO_PinOutSet(LCD_EXTCOMIN_port,LCD_EXTCOMIN_pin);
	}
	else
	{
		GPIO_PinOutClear(LCD_EXTCOMIN_port,LCD_EXTCOMIN_pin);
	}
}
void Button_Handler()
{
	LOG_INFO("\n\r Inside Button Interrupt");
}
