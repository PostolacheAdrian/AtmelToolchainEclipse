/*
 * OS.h
 *
 *  Created on: Apr 19, 2019
 *      Author: uidg9626
 */

#ifndef INCLUDES_OS_H_
#define INCLUDES_OS_H_

#define OS_NO_OF_TASKS 5

#define TASK_1MS		0
#define TASK_10MS		1
#define TASK_100MS		2
#define TASK_1000MS		3
#define TASK_LCD		4

/* Tick value in milliseconds */
#define TICK_RESOLUTION 	0.1 /* Equivalent with 10 microseconds */

/* X value will be specified in milisecods and this macro will convert the value to ticks*/
#define MILLISECONDS_TO_TICKS(x) (x/TICK_RESOLUTION)

extern void InitOS(void);
extern void StartOS(void);
extern void OsIncrementTick(void);

#endif /* INCLUDES_OS_H_ */
