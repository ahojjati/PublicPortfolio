
#ifndef _RTC_H
#define _RTC_H


//contains the handler for the rtc
//possibly an initialization function 
// anything rtc related

#define RTC_PORT			0x70
#define RTC_DATA			0x71
#define RTC_REG_A			0x0A
#define RTC_REG_B			0x0B
#define RTC_REG_C			0x0C
#define BIT_SIX				0x40
#define RTC_IRQ				8
#define SLAVE 				2
#define SLOW 				0x0F
#define SUCCESS				0
#define FAILURE				-1
#define RTC_MAX				1024
#define RTC_MIN				2
#define DIVISOR_START		15

#ifndef ASM

//initialize the rtc; CP1
void rtc_init();

// rtc handler/testing for test_interrupts; CP1
void rtc_handler_c();

// set frequency of RTC to 2Hz; CP2
int32_t rtc_open();

//return 0; CP2
int32_t rtc_close();

// wait for an RTC interrupt and then return; CP2
int32_t rtc_read(int32_t, void* buffer, int32_t);

// set RTC interrupts to a new frequencys; pulls from buffer the new parameter to set to; CP2
int32_t rtc_write(int32_t fd, const void* buffer, int32_t count);

#endif

#endif
