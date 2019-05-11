/*
 * int.c
 *
 *  Created on: Apr 18, 2019
 *      Author: uidg9626
 */
#include<avr/io.h>
#include<avr/interrupt.h>
#include"Adc.h"

#include"OS.h"
#include"pwm.h"
#include"IR.h"

void InterruptsInit(void)
{
	cli();

	/* Enable Iteruprs for ADC */
	ADCSRA |= 0x08;

	/* Enable Timer 2 interrupt */
	TIMSK |=0x40;

	/* Setting TIMER 0 for interrupt */
	TIMSK |=0x01;

	/* Enable Global Interrupt */
	sei();
}

ISR(TIMER2_OVF_vect)
{

	TCNT2 = 202;
	IRHandler();

}

/* Definition of ISRs */
ISR(ADC_vect)
{
	AdcInterruptHandler();

}

ISR(TIMER0_OVF_vect)
{
	TCNT0 = 56;
	OsIncrementTick();
}

ISR(TIMER1_CAPT_vect)
{
	IRRxHandler();
}
