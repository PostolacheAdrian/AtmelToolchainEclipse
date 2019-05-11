/*
 * OS.c
 *
 *  Created on: Apr 19, 2019
 *      Author: uidg9626
 */
#include"OS.h"
#include"OSTasks.h"
#include<stdint.h>

#define TASK_READY 		1
#define TASK_SUSPENDED 	0

typedef struct _task
{
	void (*Function)(void);
	uint32_t Period;
	uint32_t LastTick;
	uint16_t Offset;
	uint8_t State;
}Task;

Task Tasks[OS_NO_OF_TASKS];
volatile uint32_t OsTickCounter=0;
uint8_t OsStartFlag=0;

static void ConfigureTask(uint8_t TaskIndex, void *FPtr, uint32_t Period, uint16_t Offset)
{
	Tasks[TaskIndex].Function=FPtr;
	Tasks[TaskIndex].Period=Period;
	Tasks[TaskIndex].Offset=Offset;
	Tasks[TaskIndex].LastTick=0;
	Tasks[TaskIndex].State=TASK_SUSPENDED;
}
void InitOS(void)
{
	ConfigureTask(TASK_1MS,&Task1MS,MILLISECONDS_TO_TICKS(1),0);
	ConfigureTask(TASK_10MS,&Task10MS,MILLISECONDS_TO_TICKS(10),1);
	ConfigureTask(TASK_100MS,&Task100MS,MILLISECONDS_TO_TICKS(100),3);
	ConfigureTask(TASK_1000MS,&Task1000MS,MILLISECONDS_TO_TICKS(1000),5);
	ConfigureTask(TASK_LCD,&TaskLcd,MILLISECONDS_TO_TICKS(0.1),7);
}


static void OSScheduler(void)
{
	uint8_t TaskIndex;
	OsStartFlag = 1;
	while(1)
	{
		for(TaskIndex=0;TaskIndex<OS_NO_OF_TASKS;TaskIndex++)
		{
			if(Tasks[TaskIndex].State==TASK_SUSPENDED)
			{
				if((OsTickCounter - Tasks[TaskIndex].LastTick) >= Tasks[TaskIndex].Offset)
				{
					Tasks[TaskIndex].LastTick=OsTickCounter;
					Tasks[TaskIndex].State=TASK_READY;
				}
			}
			else
			{
				if((OsTickCounter - Tasks[TaskIndex].LastTick) >= Tasks[TaskIndex].Period)
				{
					Tasks[TaskIndex].LastTick=OsTickCounter;
					Tasks[TaskIndex].Function();
				}
			}
		}
	}
}


void StartOS(void)
{
	TaskInitialization();
	OSScheduler();

}

void OsIncrementTick(void)
{
	if(OsStartFlag==1)
	{
		OsTickCounter++;
	}

}
