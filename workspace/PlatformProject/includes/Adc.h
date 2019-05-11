/*
 * Adc.h
 *
 *  Created on: Apr 17, 2019
 *      Author: uidg9626
 */

#ifndef INCLUDES_ADC_H_
#define INCLUDES_ADC_H_
#include<stdint.h>
/* Number of ADC channels to be configured */
#define ADC_NO_OF_CHANNELS 2
/* Define ADC channels buffer indexes */
#define ADC_POT1	0
#define ADC_POT2	1

extern void AdcInit(void);
extern uint16_t AdcGetValue(uint8_t ChannelIndex);
extern void AdcInterruptHandler(void);


#endif /* INCLUDES_ADC_H_ */
