/*
 * Display7S.c
 *
 *  Created on: Apr 16, 2019
 *      Author: uid20916
 */
#include<avr/io.h>
#include<avr/interrupt.h>
#include"Display7S.h"

/* Define Segment port */
#define DISPLAY_SEG_DIR 	DDRB
#define DISPLAY_SEG_PORT 	PORTB

/*Define Selections port */
#define DISPLAY_SEL_DIR		DDRC
#define DISPLAY_SEL_PORT	PORTC

const uint8_t Digits[10]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};

uint8_t FrameBuffer[4]={0xff,0xff,0xff,0xff};

void Display7SInit(void)
{
	DISPLAY_SEG_DIR = 0xFF;
	DISPLAY_SEG_PORT = 0xFF;

	DISPLAY_SEL_DIR |=0x0F;
	DISPLAY_SEL_PORT |=0x0F;



}

void Display7SSync(void)
{
	static uint8_t SyncIndex;
	TCNT0 =178;

	DISPLAY_SEG_PORT = 0xFF;
	DISPLAY_SEG_PORT = FrameBuffer[SyncIndex];
	DISPLAY_SEL_PORT |=0xFF;
	DISPLAY_SEL_PORT &=(~(1<<SyncIndex));
	SyncIndex++;
	SyncIndex=SyncIndex%4;
}


void Display7SPrint(uint16_t Number)
{
	uint8_t NumberIndex;
	for(NumberIndex=0;NumberIndex<4;NumberIndex++)
	{
		if((Number==0) && (NumberIndex>0))
		{
			FrameBuffer[NumberIndex] = 0xFF;
		}
		else
		{
			FrameBuffer[NumberIndex]=Digits[Number%10];
			Number /=10;
		}
	}
}



