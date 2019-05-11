/*
 * OSTasks.c
 *
 *  Created on: Apr 19, 2019
 *      Author: uidg9626
 */
#include"avr/io.h"
#include"OSTasks.h"
#include"Timer.h"
#include"Int.h"
#include"Adc.h"
#include"pwm.h"
#include"DisplayLCD.h"
#include"IR.h"
#include"butons.h"

void TaskInitialization(void)
{
	InterruptsInit();
	Timer0Init();
	AdcInit();
	InitButons();
	IRInit();
	DisplayLCDInit();
	DisplayLCDGoTo(0,0);
	DisplayLCDPrintString("Hello World!");
}



void Task1MS(void)
{

}

void Task10MS(void)
{

}

void Task100MS(void)
{

}

void Task1000MS(void)
{


}

void TaskLcd(void)
{
	DisplayLCDHandling();
}

