#include "sched.h"

static pcb_t* queue[SHELL_PROCESSES];

/* 
 * init_sched
 *   DESCRIPTION: Initializes the queue array of pcb_t pointer to point to NULL
 *				  The structure will later be used to store pointers to pcb_t
 *				  structs that are going to run in a round robin fashion
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none 
 *   SIDE EFFECTS: sets up queue array
 */

void init_sched()
{
	int i;
	// Sets all pcb_t pointers to NULL
	for(i = 0; i<QUEUE_SIZE;i++)
	{
		queue[i] = NULL;
	}
		which_process = 0;
	return;
}

/* 
 * void sched()
 *   DESCRIPTION: Controls the context switching and process switching.  Stores the state of 
 * 				  the current process prior to the interreupt call.  Then restores the next
 *				  processes registers so next program can run correctly.  Preforms stack switching
 *				  and context switching to go to the next process. Interrupt driven
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: Stores current process and preforms task swithing to next process
 */

void sched()
{

//IF loop to check if current process is NULL
 asm volatile("movl %0, %%ecx;": :"c"(current_process));
 asm volatile("cmp $0, %ecx;");

 // If current process is NULL jump to end of label, else free esp,ebp,CR3, and esp0 for previous process so it can resume later
 asm volatile("je current_process_null");
 asm volatile("movl %%esp, %0;" : "=c" (current_process->registers.esp));
 asm volatile("movl %%ebp, %0;" : "=c" (current_process->registers.ebp));
 asm volatile("movl %%CR3, %0": "=c"(current_process->registers.cr3));
 current_process->registers.esp0 = tss.esp0;

 asm volatile("current_process_null:");

 if (current_process != NULL && current_process->terminal == curr_terminal)
 {
 	memcpy((void*)current_process->terminal->video_mem, (void*)current_process->terminal->storage, NUM_ROWS*NUM_COLS*2);
 }

//Cycles through the 3 process pointers which corresponds to the 3 shells
queue[which_process] = current_process;
which_process ++;
which_process %= 3;
current_process = queue[which_process];

// Calls execute shells for the initial 3 shells, occupado never goes back to 0 for shell processes and after extecute shell is called, they will sit in while loops so they will be called in the future
	if(queue[which_process] == NULL && (running_process[0].occupado==0 || running_process[1].occupado == 0 || running_process [2].occupado == 0))
	{
		send_eoi(0);
		execute_shell();
	}
//After 3 initial shells are set up, perform context switching on the children processes of each shell
	else
	{
		send_eoi(0);
		//Restores  stack frame,CR3, and tss of process that we will change to
		asm volatile("movl %0, %%CR3": :"c" (current_process->registers.cr3));
		tss.esp0 = current_process->registers.esp0;
		asm volatile("movl %0 , %%esp" : :"c" (current_process->registers.esp));
		asm volatile("movl %0 , %%ebp" : :"c" (current_process->registers.ebp));
		return;
	}
return;
}
