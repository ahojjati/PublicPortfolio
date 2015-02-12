/*idt_handlers.c - all of the interrupt handler functions
*/

#include "int_handlers_c.h"


/* Exception handler function for divide by zero
 * PARAMETERS: none
 * RETURNS: void
 * INFO:
 */
void divide_error_c()
{
	terminal_write((int32_t)0,(void*)"Divide Error Exception!\n",(int32_t)strlen("Divide Error Exception!\n"));
	_halt((uint8_t)256);
	return;
}

/* Exception handler for debug
 * PARAMETERS: nonefpa
 * RETURNS: void
 * INFO:
 */
void debug_c()
{	
	terminal_write((int32_t)0,(void*)"Debug Exception!\n",(int32_t)strlen("Debug Exception!\n"));
	_halt((uint8_t)256);	
	return;
}

/* Exception handler for non maskable interrupt
 * PARAMETERS: none
 * RETURNS: void
 * INFO:
 */
void nmi_c()
{
	terminal_write((int32_t)0,(void*)"Non Maskable interrupt!\n",(int32_t)strlen("Non Maskable interrupt!\n"));
	_halt((uint8_t)256);
	return;
}

/* Exception handler for Breakpoint Exception
 * PARAMETERS: none
 * RETURNS: void
 * INFO:
 */
void int3_c()
{
	terminal_write((int32_t)0,(void*)"Breakpoint Exception!\n",(int32_t)strlen("Breakpoint Exception!\n"));
	_halt((uint8_t)256);
	return;
}
	
/* Exception handler for Overflow Exception
 * PARAMETERS: none
 * RETURNS: void
 * INFO:
 */
void overflow_c()
{
	terminal_write((int32_t)0,(void*)"Overflow Exception!\n",(int32_t)strlen("Overflow Exception!\n"));
	_halt((uint8_t)256);
	return;
}

/* Exception handler for Bound Range Esceeded Exception
 * PARAMETERS: none
 * RETURNS: void
 * INFO:
 */
void bounds_c()
{
	terminal_write((int32_t)0,(void*)"Bound Range Exceeded Exception!\n",(int32_t)strlen("Bound Range Exceeded Exception!\n"));
	_halt((uint8_t)256);
	return;
}

/* Exception handler for Invalid Opcode Exception
 * PARAMETERS: none
 * RETURNS: void
 * INFO:
 */
void invalid_op_c()
{
	terminal_write((int32_t)0,(void*)"Invalid Opcode Exception!\n",(int32_t)strlen("Invalid Opcode Exception!\n"));
	_halt((uint8_t)256);
	return;
}

/* Exception handler for Device not available exception
 * PARAMETERS: none
 * RETURNS: void
 * INFO:
 */
void device_not_available_c()
{
	terminal_write((int32_t)0,(void*)"Device not Available Exception!\n",(int32_t)strlen("Device not Available Exception!\n"));
	_halt((uint8_t)256);
	return;
}

/* Exception handler for Double Fault Exception
 * PARAMETERS: none
 * RETURNS: void
 * INFO:
 */
void doublefault_fn_c()
{
	terminal_write((int32_t)0,(void*)"Double Fault Exception!\n",(int32_t)strlen("Double Fault Exception!\n"));
	_halt((uint8_t)256);
	return;
}

/* Exception handler for Coprocessor Segment
 * PARAMETERS: none
 * RETURNS: void
 * INFO:
 */
void coprocessor_segment_overrun_c()
{
	terminal_write((int32_t)0,(void*)"Coprocessor Segment Overrun!\n",(int32_t)strlen("Coprocessor Segment Overrun!\n"));
	_halt((uint8_t)256);
	return;
}

/* Exception handler for Invalid TSS exception
 * PARAMETERS: none
 * RETURNS: void
 * INFO:
 */
void invalid_TSS_c()
{
	terminal_write((int32_t)0,(void*)"Invalid TSS Exception!\n",(int32_t)strlen("Invalid TSS Exception!\n"));
	_halt((uint8_t)256);
	return;
}

/* Exception handler for Segment Not Present
 * PARAMETERS: none
 * RETURNS: void
 * INFO:
 */
void segment_not_present_c()
{
	terminal_write((int32_t)0,(void*)"Segment Not Present!\n",(int32_t)strlen("Segment Not Present!\n"));
	_halt((uint8_t)256);
	return;
}

/* Exception handler for Stack Fault Exception
 * PARAMETERS: none
 * RETURNS: void
 * INFO:
 */
void stack_segment_c()
{	
	terminal_write((int32_t)0,(void*)"Stack Fault Exception!\n",(int32_t)strlen("Stack Fault Exception!\n"));
	_halt((uint8_t)256);
	return;
}

/* Exception handler for General Protection Exception
 * PARAMETERS: none
 * RETURNS: void
 * INFO:
 */
void general_protection_c()
{
	terminal_write((int32_t)0,(void*)"General Protection Exception!\n",(int32_t)strlen("General Protection Exception!\n"));
	_halt((uint8_t)256);
	return;
}

/* Exception handler for Page Fault Exception
 * PARAMETERS: none
 * RETURNS: void
 * INFO:Bit 0 (P) is the Present flag.
 *		Bit 1 (R/W) is the Read/Write flag.
 *		Bit 2 (U/S) is the User/Supervisor flag.
 *
 *		US RW  P - Description
 *		0  0  0 - Supervisory process tried to read a non-present page entry
 *		0  0  1 - Supervisory process tried to read a page and caused a protection fault
 *		0  1  0 - Supervisory process tried to write to a non-present page entry
 *		0  1  1 - Supervisory process tried to write a page and caused a protection fault
 *		1  0  0 - User process tried to read a non-present page entry
 *		1  0  1 - User process tried to read a page and caused a protection fault
 *		1  1  0 - User process tried to write to a non-present page entry
 *		1  1  1 - User process tried to write a page and caused a protection fault
 *
 *		CR2 contains the linear address that caused the exception
 *		Upper 10 specify the PDE and middle 10 specify PTE
 */
void page_fault_c()
{
	terminal_write((int32_t)0,(void*)"Page Fault Exception!\n",(int32_t)strlen("Page Fault Exception!\n"));
	_halt((uint8_t)256);
	return;
}

/* Exception handler for Floating Point Exception
 * PARAMETERS: none
 * RETURNS: void
 * INFO:
 */
void floating_point_c()
{
	terminal_write((int32_t)0,(void*)"x87 FPU Floating-Point Error!\n",(int32_t)strlen("x87 FPU Floating-Point Error!\n"));
	_halt((uint8_t)256);
	return;
}

/* Exception handler for Alignment Check Exception
 * PARAMETERS: none
 * RETURNS: void
 * INFO:
 */
void alignment_check_c()
{
	terminal_write((int32_t)0,(void*)"Alignment Check Exception!\n",(int32_t)strlen("Alignment Check Exception!\n"));
	_halt((uint8_t)256);
	return;
}

/* Exception handler for machine check
 * PARAMETERS: none
 * RETURNS: void
 * INFO:
 */
void machine_check_c()
{
	terminal_write((int32_t)0,(void*)"Machine-Check Exception!\n",(int32_t)strlen("Machine-Check Exception!\n"));
	_halt((uint8_t)256);
	return;
}

/* Exception handler for Floating Point
 * PARAMETERS: none
 * RETURNS: void
 * INFO:
 */
void simd_coprocessor_error_c()
{
	terminal_write((int32_t)0,(void*)"SIMD Floating-Point Exception!\n",(int32_t)strlen("SIMD Floating-Point Exception!\n"));
	_halt((uint8_t)256);
	return;
}
/* Exception handler for master_irq2
 * PARAMETERS: none
 * RETURNS: void
 * INFO:
 */
void master_irq2_c()
{
	send_eoi(2);
	return;
}

/* Exception handler for master_irq3
 * PARAMETERS: none
 * RETURNS: void
 * INFO:
 */
void master_irq3_c()
{
	send_eoi(3);
	return;
}

/* Exception handler for master_irq4
 * PARAMETERS: none
 * RETURNS: void
 * INFO:
 */
void master_irq4_c()
{
	send_eoi(4);
	return;
}

/* Exception handler for master_irq5
 * PARAMETERS: none
 * RETURNS: void
 * INFO:
 */
void master_irq5_c()
{
	send_eoi(5);
	return;
}

/* Exception handler for master_irq6
 * PARAMETERS: none
 * RETURNS: void
 * INFO:
 */
void master_irq6_c()
{
	send_eoi(6);
	return;
}

/* Exception handler for master_irq7
 * PARAMETERS: none
 * RETURNS: void
 * INFO:
 */
void master_irq7_c()
{
	send_eoi(7);
	return;
}
/* Exception handler for slave_irq1
 * PARAMETERS: none
 * RETURNS: void
 * INFO:
 */
void slave_irq1_c()
{
	send_eoi(9);
	return;
}

/* Exception handler for slave_irq2
 * PARAMETERS: none
 * RETURNS: void
 * INFO:
 */
void slave_irq2_c()
{
	send_eoi(10);
	return;
}

/* Exception handler for slave_irq3
 * PARAMETERS: none
 * RETURNS: void
 * INFO:
 */
void slave_irq3_c()
{
	send_eoi(11);
	return;
}

/* Exception handler for slave_irq4
 * PARAMETERS: none
 * RETURNS: void
 * INFO:
 */
void slave_irq4_c()
{
	send_eoi(12);
	return;
}

/* Exception handler for slave_irq5
 * PARAMETERS: none
 * RETURNS: void
 * INFO:
 */
void slave_irq5_c()
{
	send_eoi(13);
	return;
}

/* Exception handler for slave_irq6
 * PARAMETERS: none
 * RETURNS: void
 * INFO:
 */
void slave_irq6_c()
{
	send_eoi(14);
	return;
}

/* Exception handler for slave_irq7
 * PARAMETERS: none
 * RETURNS: void
 * INFO:
 */
void slave_irq7_c()
{
	send_eoi(15);
	return;
}
