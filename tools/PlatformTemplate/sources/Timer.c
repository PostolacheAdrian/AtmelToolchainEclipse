/*
 * Timer.c
 *
 *  Created on: Apr 19, 2019
 *      Author: uidg9626
 */
#include<avr/io.h>
#include"Timer.h"
void Timer0Init(void)
{
	TCNT0 = 56;
	TCCR0 = 0x02;
}

void Timer2Init(void)
{
	DDRD  |=0x80;
	PORTD &=0x7F;
	TCNT2 = 202;
	OCR2 = 228;
	TCCR2 |=0x78;
}
