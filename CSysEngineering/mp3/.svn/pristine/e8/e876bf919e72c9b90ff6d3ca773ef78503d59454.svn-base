/* init_idt.h - Holds constants and various different macros that 
init_idt.c uses
*/


#ifndef _INIT_IDT_H
#define _INIT_IDT_H

#include "lib.h"
#include "int_handlers.h" 
#include "x86_desc.h"
#include "keyboard.h"
#include "rtc.h"
#include "page.h"

#define BIT_ON			1
#define BIT_OFF			0
#define DPL_KERNEL		0
#define DPL_USER		3

#ifndef ASM

//Initializes the Interrupt Descriptor Table
void init_idt();

//Initializes intel interrupts inside Interrupt Descriptor table
void setup_intel_interrupts(uint8_t vector,uint32_t* handler);

//Initializes Pic IRQ inside Interrupt Descriptor table
void setup_irq_interrupts(uint8_t vector, uint32_t* handler);

//Initializes system calls inside Interrupt Descriptor table
void setup_system_calls(uint8_t wvector, uint32_t* handler);



#endif
#endif /* _INIT_IDT_H */
