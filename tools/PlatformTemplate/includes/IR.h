/*
 * IR.h
 *
 *  Created on: Apr 24, 2019
 *      Author: uid20916
 */

#ifndef INCLUDES_IR_H_
#define INCLUDES_IR_H_

extern void IRHandler(void);
extern void IRInit(void);
extern void IRTransmitData(uint8_t Data);
extern void IRRxHandler(void);
extern uint8_t IRReceiveData(void);
#endif /* INCLUDES_IR_H_ */
