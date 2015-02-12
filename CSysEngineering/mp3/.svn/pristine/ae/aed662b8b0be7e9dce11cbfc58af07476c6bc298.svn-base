#ifndef _SCHED_H
#define _SCHED_H

#include "types.h"
#include "lib.h"
#include "pcb_struct.h"
#include "system_call.h"
#include "page.h"
#include "i8259.h"

#define QUEUE_SIZE 3

#ifndef ASM

//initialize scheduling, who could've guessed?
void init_sched();

//switch to the next process in the queue continuing
//execution of that one from where it left off
void sched();

#endif
#endif
