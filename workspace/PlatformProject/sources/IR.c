/*
 * IR.c
 *
 *  Created on: Apr 24, 2019
 *      Author: uid20916
 */

#include<avr/io.h>
#include<stdint.h>
#include"IR.h"
#include"Timer.h"
/* Defines for TX */
#define DATA_BIT_PERIOD			120
#define CTRL_BIT_PERIOD			240
#define BIT_ACTIVE_PULSE(percent,period) ((percent*period)/100)
/* Defines for RX */
#define RX_TIMMING_TOOLERANCE 200
#define PERIOD_TO_TICKS(period)		((period*0.000027)/0.0000005)

/* Variables for TX */
volatile uint8_t IRData=0xAA;
volatile uint8_t StartFlag=1;
volatile uint8_t DataFlag=0;

/*Variables for RX */
volatile uint16_t RxCounter;
volatile uint8_t  Falling = 1;
volatile uint8_t RxStart = 0;
volatile uint8_t IRRxData;
volatile uint8_t RxDataStart = 0;


static uint8_t BitTransmit(uint8_t ActivePulse,uint8_t BitPeriod)
{
	static uint8_t PeriodCounter;
	uint8_t TransmitDone = 0;
	if(PeriodCounter < BitPeriod)
	{
		PeriodCounter++;
		if(PeriodCounter == ActivePulse)
		{
			/* Stop pulses generation */
			TCCR2 &=0xCF;
		}
	}
	else
	{
		/* Start pulses generation */
		TCCR2 |=0x30;
		TransmitDone = 1;
		PeriodCounter=0;
	}
	return TransmitDone;
}

static uint8_t ByteTransmit(uint8_t Data)
{
	static uint8_t DataIndex=7;
	uint8_t TransmitDone=0;
	uint8_t Status=0;
	if((IRData &(1<< DataIndex)) == 0)
	{
		Status=BitTransmit(BIT_ACTIVE_PULSE(20,DATA_BIT_PERIOD),DATA_BIT_PERIOD);
	}
	else
	{
		Status=BitTransmit(BIT_ACTIVE_PULSE(80,DATA_BIT_PERIOD),DATA_BIT_PERIOD);
	}
	if(Status==1)
	{
		DataIndex--;
		if(DataIndex == 255)
		{
			TransmitDone=1;
			DataIndex=7;
		}
	}

	return TransmitDone;
}

static uint8_t BitReceive(uint16_t MeasuredPeriod,uint16_t Threshold)
{
	uint8_t Status=0;
	if((MeasuredPeriod >= (Threshold-RX_TIMMING_TOOLERANCE)) && (MeasuredPeriod <= (Threshold+RX_TIMMING_TOOLERANCE)))
	{
		Status = 1;
	}
	return Status;
}

static uint8_t ByteReceive(void)
{
	static uint8_t BitIndex=7;
	uint8_t BitStatus=0;
	uint8_t ByteStatus=0;

	if(BitReceive(RxCounter,6400)==1)
	{
		IRRxData |= (1<<BitIndex);
		BitStatus = 1;
	}
	else
	{
		if(BitReceive(RxCounter,1600)==1)
		{
			IRRxData &= (~(1<<BitIndex));
			BitStatus=1;
		}
	}
	if(BitStatus == 1)
	{
		BitIndex--;
		if(BitIndex == 255)
		{
			BitIndex =7;
			ByteStatus = 1;
		}
	}
	return ByteStatus;
}


void IRHandler(void)
{
	if(StartFlag == 1)
	{
		if(BitTransmit(BIT_ACTIVE_PULSE(80,CTRL_BIT_PERIOD),CTRL_BIT_PERIOD)==1)
		{
			StartFlag = 0;
			DataFlag = 1;
		}
	}
	else
	{
		if(DataFlag == 1)
		{
			if(ByteTransmit(IRData)==1)
			{
				DataFlag = 0;
			}
		}
		else
		{
			if(BitTransmit(BIT_ACTIVE_PULSE(20,CTRL_BIT_PERIOD),CTRL_BIT_PERIOD)==1)
			{
				TCCR2 &=0xF8;
				StartFlag = 1;
			}
		}
	}

}

void IRRxHandler(void)
{
	if(Falling==1)
	{
		RxCounter = ICR1;
		/* Set trigger to rissing */
		TCCR1B |=0x40;
	}
	else
	{
		/* Set trigger to falling */
		TCCR1B &=0xBF;
		RxCounter = (ICR1 - RxCounter);
		if(RxStart == 0)
		{
			if(BitReceive(RxCounter,12372)==1)
			{
				RxStart = 1;
				RxDataStart= 1;
			}
		}
		else
		{
			if(RxDataStart==1)
			{
				if(ByteReceive()==1)
				{
					RxDataStart=0;
				}
			}
			else
			{
				if(BitReceive(RxCounter,3000)==1)
				{
					RxStart = 0;
				}
			}
		}
	}

	Falling^=1;
}

void IRInit(void)
{
	Timer2Init();
	TCNT1=0;
	TCCR1A |= 0x00;
	TIMSK |=0x20;
	TCCR1B |= 0x02;
}

void IRTransmitData(uint8_t Data)
{
	IRData = Data;
	TCCR2 |=0x02;
}

uint8_t IRReceiveData(void)
{
	return IRRxData;
}
