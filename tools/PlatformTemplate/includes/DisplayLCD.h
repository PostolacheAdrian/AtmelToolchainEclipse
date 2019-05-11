/*
 * DisplayLCD.h
 *
 *  Created on: Apr 12, 2019
 *      Author: uidg9626
 */

#ifndef INCLUDES_DISPLAYLCD_H_
#define INCLUDES_DISPLAYLCD_H_
#include<stdint.h>
extern void DisplayLCDInit(void);
extern void DisplayLCDPrintString(char *string);
extern void DisplayLCDGoTo(uint8_t column, uint8_t line);
extern void IntegerToAscii(int32_t Value, uint8_t Base, uint8_t Digits, char *Text);
extern void DisplayLCDHandling(void);

#endif /* INCLUDES_DISPLAYLCD_H_ */
