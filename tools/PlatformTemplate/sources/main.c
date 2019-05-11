/*
 * main.c
 *
 *  Created on: Jan 18, 2019
 *      Author: uidg9626
 */
#include<avr/io.h>
#include<stdint.h>
#include"OS.h"

int main (void)
{
	/* Initialization code */
	InitOS();
	StartOS();
	PORTB= 0x00;
	while(1)
	{
		/* Nothing to do */
	}
	return 0;
}

