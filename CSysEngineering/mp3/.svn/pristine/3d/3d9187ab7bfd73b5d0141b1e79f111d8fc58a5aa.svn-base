/*idt_handlers.h - all of the interrupt handler functions
*/

#ifndef _IDT_HANDLERS_H
#define _IDT_HANDLERS_H

#include "x86_desc.h"
#include "system_call.h"
#include "sched.h"
#include "int_handlers.h"

#ifndef ASM

/* list of assembly exception functions */

// divide_error exception 
extern uint32_t divide_error;
// debug exception
extern uint32_t debug;
// non maskable interrupt exception
extern uint32_t nmi;
// break point exception
extern uint32_t int3;
// overflow exception
extern uint32_t overflow;
// bounds exception
extern uint32_t bounds;
// invalid opcode exception
extern uint32_t invalid_op;
// coprocessor not available exception
extern uint32_t device_not_available;
// double fault exception
extern uint32_t doublefault_fn;
// coprocessor segment overrun exception
extern uint32_t coprocessor_segment_overrun;
// invalid task state segment exception
extern uint32_t invalid_TSS;
// segment not present exception
extern uint32_t segment_not_present;
// stack fault exception
extern uint32_t stack_segment;
// general protection fault exception
extern uint32_t general_protection;
// page fault exception
extern uint32_t page_fault;
// floating point exception
extern uint32_t floating_point;
// alignment check exception
extern uint32_t alignment_check;
// machine check exception
extern uint32_t machine_check;
// simd floating point exception
extern uint32_t simd_coprocessor_error;

/* Master PIC irq line interrupts assembly linkage */

// PIT interrupts
extern uint32_t master_irq0;
// keyboard interrupts
extern uint32_t master_irq1;
// Slave PIC interrupts
extern uint32_t master_irq2;
extern uint32_t master_irq3;
extern uint32_t master_irq4;
extern uint32_t master_irq5;
extern uint32_t master_irq6;
extern uint32_t master_irq7;

/* Slave PIC irq line interrupts assembly linkage */

// RTC interrupts
extern uint32_t slave_irq0;
extern uint32_t slave_irq1;
extern uint32_t slave_irq2;
extern uint32_t slave_irq3;
extern uint32_t slave_irq4;
extern uint32_t slave_irq5;
extern uint32_t slave_irq6;
extern uint32_t slave_irq7;

// assembly linkage function for when system call is made
extern uint32_t system_call;

#endif
#endif /* _IDT_HANDLERS_H */

