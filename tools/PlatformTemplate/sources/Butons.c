/*
 * Butons.c
 *
 *  Created on: Apr 10, 2019
 *      Author: uidg9626
 */

#include<avr/io.h>
#include<stdint.h>
#include"Butons.h"

static Buton Butons[5];

static void ConfigureButon (volatile uint8_t *RegOut, volatile uint8_t *RegDir, volatile uint8_t *RegIn, uint8_t PinNo, uint8_t ButonIndex)
{
	Butons[ButonIndex].reg_dir = RegDir;
	Butons[ButonIndex].reg_out = RegOut;
	Butons[ButonIndex].reg_in = RegIn;
	Butons[ButonIndex].PinNo = PinNo;
	Butons[ButonIndex].DebCounter = 0;
	Butons[ButonIndex].PrevState = 0;
	*Butons[ButonIndex].reg_dir &= (~(1<<PinNo));
	*Butons[ButonIndex].reg_out |= (1<<PinNo);
}

void InitButons()
{
	ConfigureButon(&PORTB,&DDRB,&PINB,0,BUTON_TX);

}

uint8_t GetStateButon (uint8_t ButonIndex, uint8_t Mode)
{
	uint8_t ButonState = 0;
	if((*Butons[ButonIndex].reg_in & (1<<Butons[ButonIndex].PinNo)) == 0)
	{
		if(Butons[ButonIndex].DebCounter < DEBOUNCE_TICKS)
		{
			Butons[ButonIndex].DebCounter++;
		}
		else
		{
			if((Butons[ButonIndex].PrevState == 0) || Mode)
			{
				Butons[ButonIndex].PrevState = 1;
				ButonState = 1;
			}
		}
	}
	else
	{
		if((*Butons[ButonIndex].reg_in & (1<<Butons[ButonIndex].PinNo)) == (1<<Butons[ButonIndex].PinNo))
		{
			if(Butons[ButonIndex].DebCounter > 0)
			{
				Butons[ButonIndex].DebCounter--;
			}
			else
			{
				if((Butons[ButonIndex].PrevState == 1) || Mode)
				{
					Butons[ButonIndex].PrevState = 0;
					ButonState = 0;
				}
			}
		}
	}
	return ButonState;
}


