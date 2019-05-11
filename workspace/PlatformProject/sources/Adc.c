/*
 * Adc.c
 *
 *  Created on: Apr 17, 2019
 *      Author: uidg9626
 */
#include<avr/io.h>
#include<avr/interrupt.h>
#include"Adc.h"

typedef struct _adc_channel
{
	uint16_t AdcValue;
	uint16_t HwChannel;
}ADC_CHANNEL;

ADC_CHANNEL AdcBuffer[ADC_NO_OF_CHANNELS];

static void ConfigureAdcChannels(uint8_t BufferIndex, uint8_t HwChannel)
{
	AdcBuffer[BufferIndex].AdcValue=0;
	AdcBuffer[BufferIndex].HwChannel=HwChannel;
}

void AdcInit(void)
{
	ConfigureAdcChannels(ADC_POT1, PA1);
	ConfigureAdcChannels(ADC_POT2, PA7);

	/* Vref setting */
	ADMUX |=(0x40|AdcBuffer[0].HwChannel);
	/* Set prescaler to 16 */
	ADCSRA |=0x04;
	/* Enable ADC */
	ADCSRA |=0x80;
	sei();
	/* Start first Conversion */
	ADCSRA |=0x40;
}

uint16_t AdcGetValue(uint8_t ChannelIndex)
{

	return AdcBuffer[ChannelIndex].AdcValue;
}


void AdcInterruptHandler(void)
{
	static uint8_t AdcBufferIndex;

	AdcBuffer[AdcBufferIndex].AdcValue=ADC;
	AdcBufferIndex++;
	AdcBufferIndex %= ADC_NO_OF_CHANNELS;
	ADMUX &=0xF0;
	/* Select Channel */
	ADMUX |=AdcBuffer[AdcBufferIndex].HwChannel;
	/* Start Conversion */
	ADCSRA|=0x40;
}

