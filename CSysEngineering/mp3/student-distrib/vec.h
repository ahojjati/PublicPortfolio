
#ifndef _VEC_H
#define _VEC_H

#include "x86_desc.h"


#define NUM_PROCESSES			8
#define NUM_PRIORITY			8

/*if we have only 8 total possible active active processes
there is no point in having more than 8 priority levels.*/


//structure that will allow scheduling
//we will have one for each priority level
typedef struct task_q
{
	uint8_t length = 0;
	uint8_t current = 0;
	pcb_t * queue[NUM_PROCESSES];
	pcb_t * start;
	pcb_t * end;
};

//push this task to the back of the current queue
int32_t push_back(pcb_t *, task_q);

//switch to the next task in the queue
int32_t switch_task(task_q);

//remove the task by passing in the PID
int32_t remove_task(uint32_t PID);

//remove all of the tasks currently active
int32_t remove_all_tasks();

#endif
