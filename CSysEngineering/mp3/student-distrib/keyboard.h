/* contains the handler for the keyboard
possibly an initialization function 
 anything keyboard related */
#ifndef _HANDLER_H
#define _HANDLER_H

#include "lib.h"
#include "types.h"
#include "terminal.h"
#include "i8259.h"
#include "system_call.h"

#define KEYBOARD_IRQ 	1
#define KEYBOARD_PORT 	0x60
#define LEFT_SHIFT 		0x2A
#define LEFT_SHIFT_R 	0xAA
#define RIGHT_SHIFT 	0x36
#define RIGHT_SHIFT_R 	0xB6
#define CTRL 			0x1D
#define CTRL_R 			0x9D
#define ALT 			0x38
#define ALT_R 			0xB8
#define CAPS_LOCK       0x3A
#define CAPS_LOCK_R     0xBA
#define L_PRESSED       0x26
#define L_RELEASED      0xA6
#define ENTER           0x1C
#define ENTER_R         0x9C
#define BACKSPACE       0x0E
#define BACKSPACE_R     0x8E
#define F1_PRESS        0x3B
#define F2_PRESS        0x3C
#define F3_PRESS		0X3D
#define C_PRESSED       0x2E

#ifndef ASM

//initialize the keyboard IRQ
void keyboard_init();

// keyboard handler function, particularly for control keys
void keyboard_handler_c();

// keypress handler for when characters have been pressed
void key_press_read(uint8_t input);

#endif

#endif /* _HANDLER_H */
