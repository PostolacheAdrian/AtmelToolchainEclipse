/*
 * pwm.c
 *
 *  Created on: Apr 22, 2019
 *      Author: uid20916
 */
#include<avr/io.h>
#include"pwm.h"

volatile uint8_t DutyCycle=0;

void PwmInit(void)
{
	DDRD|=0x80;
	PORTD|=0x80;
	TCNT2 =6;
	OCR2  =7;
	TCCR2 |=0x78;
	/* Start Timer with prescaler 1024 */
	TCCR2 |=0x03;
}


void PwmSetDuty(uint8_t Duty)
{
	DutyCycle = (((uint16_t)Duty*249)/100)+7;
}

void PwmIntHandler(void)
{
	TCNT2 = 6;
	OCR2 = DutyCycle;
}
