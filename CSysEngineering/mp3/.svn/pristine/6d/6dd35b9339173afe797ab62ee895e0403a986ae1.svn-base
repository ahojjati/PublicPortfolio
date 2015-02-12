/*idt_handlers.h - all of the interrupt handler functions
*/

#ifndef _INT_HANDLERS_C_H
#define _INT_HANDLERS_C_H

#include "lib.h"
#include "i8259.h"
#include "sched.h"
#include "system_call.h"


#ifndef ASM

//Intels interrupts
void divide_error_c();
void debug_c();
void nmi_c();
void int3_c();
void overflow_c();
void bounds_c();
void invalid_op_c();
void device_not_available_c();
void doublefault_fn_c();
void coprocessor_segment_overrun_c();
void invalid_TSS_c();
void segment_not_present_c();
void stack_segment_c();
void general_protection_c();
void page_fault_c();
void floating_point_c();
void alignment_check_c();
void machine_check_c();
void simd_coprocessor_error_c();

//Pic master irq line interrupts
void master_irq0_c();
void master_irq2_c();
void master_irq3_c();
void master_irq4_c();
void master_irq5_c();
void master_irq6_c();
void master_irq7_c();

//Pic slave irq line interrupts
void slave_irq1_c();
void slave_irq2_c();
void slave_irq3_c();
void slave_irq4_c();
void slave_irq5_c();
void slave_irq6_c();
void slave_irq7_c();

#endif
#endif
