/*
 * DisplayLCD.c
 *
 *  Created on: Apr 12, 2019
 *      Author: uidg9626
 */
#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>
#include"DisplayLCD.h"

#define LCD_DATA_PORT 	PORTC
#define LCD_DATA_DDR	DDRC

#define LCD_EN_PORT		PORTC
#define LCD_EN_DDR		DDRC
#define LCD_EN_PIN		PORTC4

#define LCD_RS_PORT		PORTC
#define LCD_RS_DDR		DDRC
#define LCD_RS_PIN		PORTC5

#define HD44780_CLEAR					0x01

#define HD44780_HOME					0x02

#define HD44780_ENTRY_MODE				0x04
#define HD44780_EM_SHIFT_CURSOR		0
#define HD44780_EM_SHIFT_DISPLAY	1
#define HD44780_EM_DECREMENT		0
#define HD44780_EM_INCREMENT		2

#define HD44780_DISPLAY_ONOFF			0x08
#define HD44780_DISPLAY_OFF			0
#define HD44780_DISPLAY_ON			4
#define HD44780_CURSOR_OFF			0
#define HD44780_CURSOR_ON			2
#define HD44780_CURSOR_NOBLINK		0
#define HD44780_CURSOR_BLINK		1

#define HD44780_DISPLAY_CURSOR_SHIFT	0x10
#define HD44780_SHIFT_CURSOR		0
#define HD44780_SHIFT_DISPLAY		8
#define HD44780_SHIFT_LEFT			0
#define HD44780_SHIFT_RIGHT			4

#define HD44780_FUNCTION_SET			0x20
#define HD44780_FONT5x7				0
#define HD44780_FONT5x10			4
#define HD44780_ONE_LINE			0
#define HD44780_TWO_LINE			8
#define HD44780_4_BIT				0
#define HD44780_8_BIT				16

#define HD44780_CGRAM_SET				0x40
#define HD44780_DDRAM_SET				0x80

#define QUEUE_SIZE		100
typedef struct _lcdraw
{
	uint8_t RS_flag;
	uint8_t Data;
}LCD_Frame;


typedef struct _lcdqueue
{
	LCD_Frame Frame[QUEUE_SIZE];
	volatile int8_t Rear;
	volatile int8_t Front;
}Queue;


Queue QueueFrames;

volatile uint8_t TransmitOn = 0;
volatile uint8_t InitializationDone=0;

static uint8_t QueueEmpty(Queue *Q)
{
	uint8_t Result;
	if (Q->Front==-1)
	{
		Result = 1;
	}
	else
	{
		Result =0;
	}
	return Result;
}




static void WriteLCD (Queue *Q)
{
	if(QueueEmpty(Q)==1)
	{
		/* Stop Transmition */
		TransmitOn = 0;
	}
	else
	{
		if(TransmitOn==1)
		{
			if(Q->Frame[Q->Front].RS_flag == 1)
			{
				LCD_RS_PORT |= (1<<LCD_RS_PIN);
			}
			else
			{
				LCD_RS_PORT &= (~(1<<LCD_RS_PIN));
			}

			/* Enable the LCD bus */
			LCD_EN_PORT |= (1<<LCD_EN_PIN);
			/* Clear Port before new data write */
			LCD_DATA_PORT &=0xF0;
			/* write High nibble */
			LCD_DATA_PORT |= (Q->Frame[Q->Front].Data >> 4);
			/* Disable the LCD bus */
			LCD_EN_PORT &= (~(1<<LCD_EN_PIN));
			/* Enable the LCD bus */
			LCD_EN_PORT |= (1<<LCD_EN_PIN);
			/* Clear Port before new data write */
			LCD_DATA_PORT &=0xF0;
			/* write Low nibble */
			LCD_DATA_PORT |= (Q->Frame[Q->Front].Data & 0x0F);
			/* Disable the LCD bus */
			LCD_EN_PORT &= (~(1<<LCD_EN_PIN));
			Q->Front++;
			if(Q->Front > Q->Rear)
			{
				Q->Front = -1;
				Q->Rear = -1;
			}
		}
	}

}

static void WriteLCDCommand(Queue *Q, uint8_t Command)
{
	if(Q->Rear < QUEUE_SIZE)
	{
		if(Q->Front == -1)
		{
			Q->Front = 0;

		}
		Q->Rear++;
		Q->Frame[Q->Rear].RS_flag = 0;
		Q->Frame[Q->Rear].Data = Command;
	}
	TransmitOn=1;
	if(InitializationDone==0)
	{
		DisplayLCDHandling();
		_delay_us(50);
	}
}

static void WriteLCDData(Queue *Q, uint8_t Data)
{
	if(Q->Rear < QUEUE_SIZE)
	{
		if(Q->Front == -1)
		{
			Q->Front = 0;
		}
		Q->Rear++;
		Q->Frame[Q->Rear].RS_flag = 1;
		Q->Frame[Q->Rear].Data = Data;
		TransmitOn=1;
	}

}

void DisplayLCDInit(void)
{
	uint8_t repeat;
	QueueFrames.Front =-1;
	QueueFrames.Rear =-1;
	LCD_DATA_DDR |= 0x0F;
	LCD_EN_DDR |=(1<< LCD_EN_PIN);
	LCD_RS_DDR |=(1<< LCD_RS_PIN);
	_delay_ms(30);
	LCD_RS_PORT &= (~(1<<LCD_RS_PIN));
	LCD_EN_PORT &= (~(1<<LCD_EN_PIN));
	for(repeat=0;repeat<3;repeat++)
	{
		LCD_EN_PORT |=(1<< LCD_EN_PIN);
		LCD_DATA_PORT &=0xF0;
		LCD_DATA_PORT |=0x03;
		LCD_EN_PORT &= (~(1<<LCD_EN_PIN));
		_delay_ms(5);

	}
	LCD_EN_PORT |=(1<< LCD_EN_PIN);
	LCD_DATA_PORT &=0xF0;
	LCD_DATA_PORT |=0x02;
	LCD_EN_PORT &= (~(1<<LCD_EN_PIN));
	_delay_ms(5);
	WriteLCDCommand(&QueueFrames,HD44780_FUNCTION_SET | HD44780_FONT5x7 | HD44780_TWO_LINE | HD44780_4_BIT);
	WriteLCDCommand(&QueueFrames,HD44780_DISPLAY_ONOFF | HD44780_DISPLAY_OFF);
	WriteLCDCommand(&QueueFrames,HD44780_CLEAR);
	_delay_ms(5);
	WriteLCDCommand(&QueueFrames,HD44780_ENTRY_MODE | HD44780_EM_SHIFT_CURSOR | HD44780_EM_INCREMENT);
	WriteLCDCommand(&QueueFrames,HD44780_DISPLAY_ONOFF | HD44780_DISPLAY_ON | HD44780_CURSOR_OFF | HD44780_CURSOR_NOBLINK);
	InitializationDone = 1;
}


void DisplayLCDPrintString(char *string)
{
	while(*string)
	{
		WriteLCDData(&QueueFrames,*string++);
	}
}

void DisplayLCDGoTo(uint8_t column, uint8_t line)
{
	WriteLCDCommand(&QueueFrames,HD44780_DDRAM_SET|(column+(0x40*line)));
}


void DisplayLCDHandling(void)
{
	WriteLCD (&QueueFrames);
}

void IntegerToAscii(int32_t Value, uint8_t Base, uint8_t Digits, char *Text)
{
	static uint8_t Chars[16]="0123456789ABCDEF";
	uint8_t TextIndex;
	uint8_t Sign=0;
	Text[Digits]='\0';
	if(Value<0)
	{
		Sign=1;
	}
	for(TextIndex=0;TextIndex<Digits;TextIndex++)
	{
		if((Value==0) && (TextIndex>0))
		{
			Text[Digits-TextIndex-1]=' ';
		}
		else
		{
			if(Value>=0)
			{
				Text[Digits-TextIndex-1] = Chars[Value%Base];
			}
			else
			{
				Text[Digits-TextIndex-1] = Chars[(~Value+1)%Base];
			}
		}
		Value=Value/Base;
		if((Value==0)&&(Sign==1)&&(Base==10))
		{
			Sign=0;
			TextIndex++;
			Text[Digits-TextIndex-1]='-';
		}
	}
}
