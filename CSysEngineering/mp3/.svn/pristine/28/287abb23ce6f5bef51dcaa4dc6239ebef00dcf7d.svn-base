#ifndef _PCB_STRUCT_H
#define _PCB_STRUCT_H

#include "lib.h"
#include "types.h"
#include "file_sys_struct.h"

#define NUM_FID 		8
#define BUFF_SIZE 		128
#define PCB_SIZE		222
#define NUM_PROCESSES	8
#define SHELL_PROCESSES	3
#define PCB_TOTAL		(PCB_SIZE*NUM_PROCESSES)


#ifndef ASM

//structure for the terminal
typedef struct term_t
{
	uint8_t line_buf[BUFF_SIZE];		//1024
	uint8_t line_buf_index;		//1
	//used in edge case management
	uint32_t wrap_flag;			//4
	uint32_t screen_x;			//4
	uint32_t screen_y;			//4
	uint8_t* video_mem;		//4
	//storage for when that terminal is not active
	uint8_t* storage;			//4
	//set high when the terminal is being read from
	volatile uint32_t term_read; //4
	//not really used
	uint32_t write_lock;			//4
	uint32_t terminal_read_ret;

}__attribute__((packed)) term_t;	// Total = 1053

//register file struct used to save registers to and restore from
typedef struct reg_desc
{
	uint32_t eax;
	uint32_t ebx;
	uint32_t ecx;
	uint32_t edx;
	uint32_t edi;
	uint32_t esi;
	uint32_t ebp;
	uint32_t esp;
	uint32_t cr3;
	uint32_t esp0;
	uint32_t eip;
	uint32_t cs;
	uint32_t eflags;
	uint32_t ss;
} __attribute__((packed)) reg_t; //44 bytes

//pcb struct, describes a process. there are 8 of these
typedef struct pcb_t
{
	//says whether this structure is occupied by a running process
	uint8_t occupado;				//1   
	reg_t registers;			    //48
	uint32_t return_esp;			
	uint32_t return_ebp;
	uint32_t temp_register;
	struct pcb_t* parent;			//4 			running process parent to be equal to pid that spawned process
	struct pcb_t* child;			//4 			running process to be -1,current process child to be equal to pid of new process
	uint32_t pid; 					//4 			running process(next process)
	uint8_t argument[BUFF_SIZE];	//128
	f_desc_t fid[NUM_FID];			//16 			running process stdin stdout
	uint32_t return_address;		//4
	uint32_t fail_address;			//4
	int32_t  return_value;			//4
	//whether or not we are to return to a fail address on _halt()
	int8_t failure;					//1
	//terminal that this process belongs to
	term_t* terminal;				//4
} __attribute__((packed)) pcb_t; //222 bytes
//directory entry structure

#endif
#endif
