//Header file for terminal driver
#ifndef _TERMINAL_H
#define _TERMINAL_H

#include "lib.h"
#include "types.h"
#include "pcb_struct.h"
#include "i8259.h"
#include "file_sys.h"

#define SCREEN_SIZE NUM_COLS * NUM_ROWS
#define VMEM_START VIDEO
#define VMEM_END VIDEO + SCREEN_SIZE
#define BUFFER_SIZE 128
#define VIDEO_0 0x1000
#define VIDEO_1 0x2000
#define VIDEO_2 0x4000
#define NUM_TERM	3

#ifndef ASM

extern term_t* curr_terminal;
extern term_t terminal[NUM_TERM];

//Reads nbytes from the keyboard to the buffer
int32_t terminal_read(int32_t fd, void * buf, int32_t nbytes);

//Writes nbytes from the buffer to video memory or storage
int32_t terminal_write(int32_t fd, const void * buf, int32_t nbytes);

//Function always returns -1
int32_t terminal_open();

//Function always returns -1
int32_t terminal_close();

// Clears video memory that terminal->video_mem points to
void clear_screen(term_t* terminal);

// Clears storage that terminal->video_mem points to
void clear_storage(term_t* terminal);

//Clears line buffer associated with terminal->line_buffer 
void clear_line_buffer(term_t* terminal);

//Adds a character "c" to  terminal->line_buffer 
void line_buffer_add(uint8_t c,term_t* terminal);

//Deletes a character "c" from terminal->line_buffer
void line_buffer_delete(term_t* terminal);

//Adds a character "c" to  terminal->storage which populates storage
void storage_add_char(uint8_t c, term_t* terminal);

//Adds a character 'c' to terminal->video_mem, which populates the screen
void screen_add_char(uint8_t c,term_t* terminal);

//delete a character from the terminal->storage. Deletes character from stroage
void storage_delete_char(term_t* terminal);

//delete a character from the terminal->video memory.  Deletes character from screen
void screen_delete(term_t* terminal);

//Changes to a new terminal and copies terminal num storage to video memory
void terminal_change(int terminal_num);

// Initializes global variables and terminals structs
void init_terminals();


/******Functions below are not implemented yet******/
int32_t scroll_up(void);

int32_t scroll_down(void);

#endif /* ASM */

#endif /* TERMINAL_H */
