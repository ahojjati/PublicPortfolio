
#include "vec.h"


/*
 
 
 
 
 
 
 
 */
int32_t push_back(pcb_t * proc, task_q * q)
{
	if (q == NULL || q->length >= 8 || proc == NULL)
	{
		return -1;
	}
	(q->queue)[q->length] = proc;
	q->length ++;
	if (q->length == 1)
	{
		q->beginning = (q->queue)[0];
		q->end = (q->queue)[0];
	}
	else
	{
		q->end = (q->queue)[q->length - 1];
	}
}

/*
 
 
 
 
 
 
 
 */
int32_t switch_task(task_q * q)
{
	int i;
	if (q == NULL || q->length <= 1)
	{
		return -1;
	}
	//some pointer manipulation so we don't have
	//to copy everything when we switch tasks
	for (i = 0; i < q->length - 1; ++i)
	{
		(q->queue)[i]++;
	}
	(q->queue)[length-1] = q->beginning;
}

/*
 
 
 
 
 
 
 
 */
int32_t remove_task(uint32_t pid)
{
	int i;
	for (i = 0; i < (q->length); ++i)
	{
		if (((q->queue)[i]) != NULL && ((q->queue)[i])->PID == pid)
		{
			
		}
	}
}

/*
 
 
 
 
 
 
 
 */

int32_t remove_task(pcb_t *)
{

}

/*
 
 
 
 
 
 
 
 */

int32_t remove_all_tasks()
{

}
