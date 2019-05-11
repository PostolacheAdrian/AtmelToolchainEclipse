/*
 * butons.h
 *
 *  Created on: Apr 10, 2019
 *      Author: uidg9626
 */

#ifndef INCLUDES_BUTONS_H_
#define INCLUDES_BUTONS_H_

#include<stdint.h>

#define DEBOUNCE_TICKS 100

#define BUTON_TX 	0
#define BUTON_PLUS	1
#define BUTON_ENTER 2



typedef struct _buton
{
	volatile uint8_t *reg_out;
	volatile uint8_t *reg_in;
	volatile uint8_t *reg_dir;
	uint8_t PinNo;
	uint8_t DebCounter;
	uint8_t PrevState;
}Buton;

extern void InitButons();
extern uint8_t GetStateButon(uint8_t ButonIndex, uint8_t Mode);

#endif /* INCLUDES_BUTONS_H_ */
