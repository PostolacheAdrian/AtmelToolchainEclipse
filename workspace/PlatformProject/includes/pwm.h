/*
 * pwm.h
 *
 *  Created on: Apr 22, 2019
 *      Author: uid20916
 */

#ifndef INCLUDES_PWM_H_
#define INCLUDES_PWM_H_
#include<stdint.h>
extern void PwmInit(void);
extern void PwmSetDuty(uint8_t Duty);
extern void PwmIntHandler(void);


#endif /* INCLUDES_PWM_H_ */
