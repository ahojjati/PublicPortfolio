//contains the handler for the rtc
//possibly an initialization function 
// anything rtc related

#include "i8259.h"
#include "lib.h"
#include "rtc.h"

// flag used to determine when at interrupt comes in from the RTC
static uint32_t interrupt_flag = 0;

/*
 * void rtc_init()
 * Description: sets up the RTC and enables the IRQs on the appropriate PICs
 * Inputs: none
 * Outputs: none
 * Returns: none
 * Side Effects: Enables IRQs 2 and 8; initializes the frequency and divsisor for RTC 
 */
void rtc_init()
{
	uint32_t flags;
	uint8_t prev;

	cli_and_save(flags);

	// open appropriate IRQs
	enable_irq(RTC_IRQ);
	enable_irq(SLAVE);

	// grab previous value of register B
	outb(RTC_REG_B, RTC_PORT);
	prev = inb(RTC_DATA);

	// write to register B and turn on bit 6 of it
	outb(RTC_REG_B, RTC_PORT);
	outb(prev | BIT_SIX, RTC_DATA);

	// set flag low for rtc_read purposese
	interrupt_flag = 0;

	restore_flags(flags);
	
	
}

/*
 * void rtc_handler_c()
 * Description: handler for the RTC that calls test_interrupts
 * Inputs: none
 * Outputs: none
 * Returns: none
 * Side Effects: clears regsister C in RTC so RTC interrupts can come again
 */
void rtc_handler_c()
{
	//set flag high so rtc_read call can move on
	interrupt_flag = 1;

	// clear register C
	outb(RTC_REG_C, RTC_PORT);
	inb(RTC_DATA);

	send_eoi(RTC_IRQ);
}


/*
 * int32_t rtc_open()
 * Description: sets the RTC interrupt frequency to 2Hz
 * Inputs: none
 * Outputs: none
 * Returns: 0 on succcess, -1 on failure (?)
 * Side Effects: shifts contents of register A down appropriately so 2Hz interrupt generated
 */
 int32_t rtc_open()
 {
 	uint32_t flags;
 	cli_and_save(flags);

 	// Set the divisor value in register A so that interrupts come at 2 Hz
 	outb(RTC_REG_A, RTC_PORT);
	outb(SLOW, RTC_DATA);

	restore_flags(flags);

	return SUCCESS;

 }

 /*
 * int32_t rtc_close()
 * Description: returns 0
 * Inputs: none
 * Outputs: none
 * Returns: 0 constantly
 * Side Effects: none
 */
 int32_t rtc_close()
 {
 	return SUCCESS;
 }

 /*
 * int32_t rtc_read(int32_t fd, void*buffer, int32_t count)
 * Description: wait for an interrupt to be generated by RTC before returning
 * Inputs: fd - Garbage
 *		   buffer - Garbage
 *		   count - Garbage
 * Outputs: none
 * Returns: 0 on succcess, -1 on failure (?)
 * Side Effects: resets the interrupt_flag back to 0
 */
 int32_t rtc_read(int32_t fd, void* buffer, int32_t count)
 {
 	interrupt_flag = 0;
 	while(!interrupt_flag) 
 	{
 		// wait for interrupt and do nothing
 	}
 	return SUCCESS;
 }

 /*
 * int32_t rtc_write(int32_t fd, const void* buffer, int32_t count)
 * Description: sets the RTC interrupt frequency to whatever is specifed in buffer
 * Inputs: fd - Garbage
 *		   buffer - pointer to buffer that contains the value at which we wish to set the frequencies at
 *		   count - Garbage
 * Outputs: none
 * Returns: 0 on succcess, -1 on failure (?)
 * Side Effects: shifts contents of register A down appropriately so new frequency interrupt generated
 * Note: Wrting to register A (rate); frequency = 32768 >> (rate - 1);
 */
 int32_t rtc_write(int32_t fd, const void* buffer, int32_t count)
 {
 	const int32_t * buf = buffer;
 	int32_t frequency;
 	uint32_t rate = DIVISOR_START;
 	uint8_t value = 0;

 	// check to see if passed in pointer is NULL
 	if (buffer == NULL)
 		return FAILURE;

 	frequency = *buf;

 	// Limit user capabilities of setting frequency from 2 Hz to 1024 Hz and powers of 2
 	if(frequency >= RTC_MIN && frequency <= RTC_MAX && frequency%2 == 0)
 	{
 		uint32_t flags;
 		cli_and_save(flags);

 		// shift rate the appropriate amount of times and determine whether frequency is a power of 2
 		while (frequency != 2 && frequency%2 == 0)
 		{
 			rate -= 1;
 			frequency = frequency >> 1;
 		}
 		// when jumping out of while loop, check to make sure that final value of frequency is 2
 		if (frequency%2 != 0)
 		{
 			return FAILURE;
 		} 
 		value = (uint8_t)rate;

 		// write to register A in RTC to set the appropriate frequency
 		outb(RTC_REG_A, RTC_PORT);
 		outb(value, RTC_DATA);

 		restore_flags(flags);
 		return SUCCESS;
 	}
 	return FAILURE;
}
