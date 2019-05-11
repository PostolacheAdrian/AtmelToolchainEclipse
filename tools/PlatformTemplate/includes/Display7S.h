/*
 * Display7S.h
 *
 *  Created on: Apr 16, 2019
 *      Author: uid20916
 */

#ifndef INCLUDES_DISPLAY7S_H_
#define INCLUDES_DISPLAY7S_H_
#include<stdint.h>

extern void Display7SInit(void);
extern void Display7SPrint(uint16_t Number);
extern void Display7SSync(void);

#endif /* INCLUDES_DISPLAY7S_H_ */
