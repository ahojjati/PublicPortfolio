//File containing source code for terminal driver

#include "terminal.h"

term_t* curr_terminal;
term_t terminal[NUM_TERM];

void putc_storage(uint8_t c,term_t* terminal);
void putc_screen(uint8_t c,term_t* terminal);
void storage_scroll(term_t* terminal);
void video_mem_scroll(term_t* terminal);

/* Function to handle system calls for reading from the terminal
 * PARAMETERS: fd - not used in this function
 *			   buf - a pointer to a buffer in which to write the data
 *			   nbytes - not used by this function
 * RETURNS: the number of bytes successfully read into the buffer
 * INFO: This function reads bytes from the line buffer up until the
 *		 newline character (\n).  If no newline character is present
 *       than the entire buffer is read.  This function also clears
 *		 the line buffer.  It is intended to be called mainly
 *		 by the shell.
 */
int32_t terminal_read(int32_t fd, void * buf, int32_t nbytes)
{
	cli();
 	clear_line_buffer(current_process->terminal);
 	current_process->terminal->term_read = 1;
 	sti();
 	while(current_process->terminal->term_read == 1)
 	{

 	}
 	current_process->terminal->terminal_read_ret = current_process->terminal->line_buf_index;
 	memcpy(buf,(void*)current_process->terminal->line_buf,BUFFER_SIZE);
 	clear_line_buffer(current_process->terminal);
	return current_process->terminal->terminal_read_ret;
}

/* Function to handle system calls for writing to the terminal
 * PARAMETERS: fd - not used in this function
 			   buf - a pointer to a buffer which contains the data to write
 			   nbytes - the number of bytes to write from the buffer
 * RETURNS: the number of bytes successfully written, or -1 on failure
 * INFO: This function sets the write_lock to 1 at the beginning of
 *       exec
 ution, and clears it before returning.  This will
 *       allow user programs to block keyboard echoes while trying
 *       to output information to the terminal.
 */
int32_t terminal_write(int32_t fd, const void * buf, int32_t nbytes)
{
	int32_t i;
	uint8_t* _buf = (uint8_t*)buf;
	//Check for valid pointer and valid n_bytes
	if(buf == NULL || nbytes < 0)
		return -1;
    else
    {
    	current_process->terminal->write_lock = 1;
        for(i=0; i<nbytes; i++)
        {
        	if(current_process->terminal == curr_terminal)
       		{
       			screen_add_char(_buf[i],current_process->terminal);
       		}
            storage_add_char(_buf[i],current_process->terminal);
        }
       	clear_line_buffer(current_process->terminal);
       	current_process->terminal->write_lock = 0;
        return i;
    }
}

/* Function to initialize the terminal
 * PARAMETERS: none
 * RETURNS: 0 on success, -1 on failure
 * INFO: clears the screen, executes the shell,
 */
int32_t terminal_open()
{
	return -1;
}

/* Function to close the terminal
 * PARAMETERS: none
 * RETURNS: -1 always, since the terminal should never be closed
 * INFO: This function does nothing, since it should not be possible
 *       for a system call to close the terminal
 */
int32_t terminal_close()
{
	//Terminal cannot be closed, so return -1
	return -1;
}

/* Function to clear the screen and put the cursor at the top
 * PARAMETERS: terminal - current terminal to clear storage.  Terminal will either be curr_terminal or current_process->terminal depending on if call is system call driven or interrupt driven respectively
 * RETURNS: void
 * INFO: This function clears storage associated with the terminal.
  * Side Effects: clears all data in video memory
 */
void clear_screen(term_t* terminal)
{
    int32_t i;
    for(i=0; i<(NUM_ROWS)*(NUM_COLS); i++)
    {
        *(uint8_t *)(terminal->video_mem + (i << 1)) = ' ';
        *(uint8_t *)(terminal->video_mem + (i << 1) + 1) = ATTRIB;
    }
	terminal->wrap_flag = 0;
	terminal->screen_x = 0;
	terminal->screen_y = 0;
}

/* Function to clear storage 
 * PARAMETERS: terminal - current terminal to clear storage.  Terminal will either be curr_terminal or current_process->terminal depending on if call is system call driven or interrupt driven respectively
 * RETURNS: void
 * INFO: This function clears storage associated with the terminal.
  * Side Effects: clears all data in storage buffer
 */
void clear_storage(term_t* terminal)
{
    int32_t i;
    for(i=0; i<NUM_ROWS*NUM_COLS; i++) {
        *(uint8_t *)(terminal->storage + (i << 1)) = ' ';
        *(uint8_t *)(terminal->storage + (i << 1) + 1) = ATTRIB;
    }
}


/* Helper function which clears out the line buffer
 * PARAMETERS: none
 * RETURNS: void
 * INFO: This function fills the line buffer with all spaces
 */
void clear_line_buffer(term_t* terminal)
{
	int i;
	for (i = 0; i < BUFFER_SIZE; ++i)
	{
		terminal->line_buf[i] = 0;
	}
	terminal->line_buf_index = 0;
	return;

}

/* Helper function which writes a single charcter to the line buffer
 * PARAMETERS: c - the character to write
 * RETURNS: 0 on success, -1 on failure (ie. line buf is full)
 * INFO: This function will not write to the line buffer if it is
 *       already full.
 */
void line_buffer_add(uint8_t c,term_t* terminal)
{
	if(terminal->line_buf_index < BUFFER_SIZE)
	{
		terminal->line_buf[terminal->line_buf_index] = c;
		terminal->line_buf_index++;
		return;
	}
	else
		return;
}


/* line_buffer_delete
 * Description: deletes a character from the line buffer if there is at least 1 character in the buffer
 * Inputs: terminal - current terminal .  Terminal will either be curr_terminal or current_process->terminal depending on if call is system call driven or interrupt driven respectively
 * Outputs: none
 * Returns: none
 * Side Effects: delets a character from the line buffer
 */
void line_buffer_delete(term_t* terminal)
{
	if(terminal->line_buf_index == 0)
		return ;
	else
	{
		terminal->line_buf_index--;
		terminal->line_buf[terminal->line_buf_index] = 0;
		return ;
	}
}

/* Function that adds characters to storage
 * PARAMETERS: c - the character to write
 *				terminal - current terminal to scroll.  Terminal will either be curr_terminal or current_process->terminal depending on if call is system call driven or interrupt driven respectively
 * RETURNS: 0 on success, -1 on failure (ie. line buf is full)
 * INFO: I pretty much made a truth table because there were too many conflicting edge cases. 
 */

void storage_add_char(uint8_t c, term_t* terminal)
{
	if(terminal->line_buf_index < BUFFER_SIZE)
	{
		//Read these if statments like a truth table
		if(c != '\n' && terminal->screen_x != 0 && terminal->wrap_flag !=1 && terminal->screen_y != NUM_ROWS-1 && terminal->screen_x != NUM_COLS-1) 
		{	
			putc_storage(c,terminal);
			return;
		}

		if(c != '\n' && terminal->screen_x != 0 && terminal->wrap_flag !=1 && terminal->screen_y != NUM_ROWS-1 && terminal->screen_x == NUM_COLS-1)
		{
			putc_storage(c,terminal);
			putc_storage(10,terminal); //emulate a new line
			terminal->wrap_flag = 1;
			return;
		}

		if(c != '\n' && terminal->screen_x != 0 && terminal->wrap_flag !=1 && terminal->screen_y == NUM_ROWS-1 && terminal->screen_x != NUM_COLS-1)
		{
			putc_storage(c,terminal);
			return;

		}

		if(c != '\n' && terminal->screen_x != 0 && terminal->wrap_flag !=1 && terminal->screen_y == NUM_ROWS-1 && terminal->screen_x == NUM_COLS-1)
		{
			putc_storage(c,terminal);
			putc_storage(10,terminal); //emulate a new line
			terminal->screen_y = NUM_ROWS-1;
			storage_scroll(terminal);
			terminal->wrap_flag = 1;
			return;

		}

		if(c != '\n' && terminal->screen_x != 0 && terminal->wrap_flag ==1 && terminal->screen_y != NUM_ROWS-1 && terminal->screen_x != NUM_COLS-1)
		{
			putc_storage(c,terminal);
			return;
		}

		if(c != '\n' && terminal->screen_x != 0 && terminal->wrap_flag ==1 && terminal->screen_y != NUM_ROWS-1 && terminal->screen_x == NUM_COLS-1)
		{
			putc_storage(c,terminal);
			putc_storage(10,terminal); //emulate a new line
			terminal->wrap_flag = 1;
			return;
		}

		if(c != '\n' && terminal->screen_x != 0 && terminal->wrap_flag ==1 && terminal->screen_y == NUM_ROWS-1 && terminal->screen_x != NUM_COLS-1)
		{
			putc_storage(c,terminal);
			return;
		}

		if(c != '\n' && terminal->screen_x != 0 && terminal->wrap_flag ==1 && terminal->screen_y == NUM_ROWS-1 && terminal->screen_x == NUM_COLS-1)
		{
			putc_storage(c,terminal);
			putc_storage(10,terminal); //emulate a new line
			terminal->screen_y = NUM_ROWS-1;
			storage_scroll(terminal);
			terminal->wrap_flag = 1;
			return;
		}

		if(c != '\n' && terminal->screen_x == 0 && terminal->wrap_flag !=1 && terminal->screen_y != NUM_ROWS-1 && terminal->screen_x != NUM_COLS-1)
		{
			putc_storage(c,terminal);
			return;
		}

		if(c != '\n' && terminal->screen_x == 0 && terminal->wrap_flag !=1 && terminal->screen_y == NUM_ROWS-1 && terminal->screen_x != NUM_COLS-1)
		{
			putc_storage(c,terminal);
			return;
		}


		if(c != '\n' && terminal->screen_x == 0 && terminal->wrap_flag ==1 && terminal->screen_y != NUM_ROWS-1 && terminal->screen_x != NUM_COLS-1)
		{
			putc_storage(c,terminal);
			return;
		}


		if(c != '\n' && terminal->screen_x == 0 && terminal->wrap_flag ==1 && terminal->screen_y == NUM_ROWS-1 && terminal->screen_x != NUM_COLS-1)
		{
			putc_storage(c,terminal);
			return;
		}


		if(c == '\n' && terminal->screen_x != 0 && terminal->wrap_flag !=1 && terminal->screen_y != NUM_ROWS-1 && terminal->screen_x != NUM_COLS-1)
		{
			terminal->wrap_flag = 0;
			putc_storage(c,terminal);
			return;
		}

		if(c == '\n' && terminal->screen_x != 0 && terminal->wrap_flag !=1 && terminal->screen_y != NUM_ROWS-1 && terminal->screen_x == NUM_COLS-1)
		{
			terminal->wrap_flag = 0;
			putc_storage(c,terminal);
			return;
		}

		if(c == '\n' && terminal->screen_x != 0 && terminal->wrap_flag !=1 && terminal->screen_y == NUM_ROWS-1 && terminal->screen_x != NUM_COLS-1)
		{
			terminal->wrap_flag = 0;
			putc_storage(c,terminal);
			terminal->screen_y = NUM_ROWS-1;
			storage_scroll(terminal);
			return;
		}

		if(c == '\n' && terminal->screen_x != 0 && terminal->wrap_flag !=1 && terminal->screen_y == NUM_ROWS-1 && terminal->screen_x == NUM_COLS-1)
		{
			terminal->wrap_flag = 0;
			putc_storage(c,terminal);
			terminal->screen_y = NUM_ROWS-1;
			storage_scroll(terminal);
			return;
		}

		if(c == '\n' && terminal->screen_x != 0 && terminal->wrap_flag ==1 && terminal->screen_y != NUM_ROWS-1 && terminal->screen_x != NUM_COLS-1)
		{
			terminal->wrap_flag = 0;
			putc_storage(c,terminal);
			return;
		}

		if(c == '\n' && terminal->screen_x != 0 && terminal->wrap_flag ==1 && terminal->screen_y != NUM_ROWS-1 && terminal->screen_x == NUM_COLS-1)
		{
			terminal->wrap_flag = 0;
			putc_storage(c,terminal);
			return;
		}

		if(c == '\n' && terminal->screen_x != 0 && terminal->wrap_flag ==1 && terminal->screen_y == NUM_ROWS-1 && terminal->screen_x != NUM_COLS-1)
		{
			terminal->wrap_flag = 0;
			putc_storage(c,terminal);
			terminal->screen_y = NUM_ROWS-1;
			storage_scroll(terminal);
			return;
		}

		if(c == '\n' && terminal->screen_x != 0 && terminal->wrap_flag ==1 && terminal->screen_y == NUM_ROWS-1 && terminal->screen_x == NUM_COLS-1)
		{
			terminal->wrap_flag = 0;
			putc_storage(c,terminal);
			terminal->screen_y = NUM_ROWS-1;
			storage_scroll(terminal);
			return;
		}

		if(c == '\n' && terminal->screen_x == 0 && terminal->wrap_flag !=1 && terminal->screen_y != NUM_ROWS-1 && terminal->screen_x != NUM_COLS-1)
		{
			terminal->wrap_flag = 0;
			putc_storage(c,terminal);
			return;
		}


		if(c == '\n' && terminal->screen_x == 0 && terminal->wrap_flag !=1 && terminal->screen_y == NUM_ROWS-1 && terminal->screen_x != NUM_COLS-1)
		{
			terminal->wrap_flag = 0;
			putc_storage(c,terminal);
			terminal->screen_y = NUM_ROWS-1;
			storage_scroll(terminal);
			return;
		}


		if(c == '\n' && terminal->screen_x == 0 && terminal->wrap_flag ==1 && terminal->screen_y != NUM_ROWS-1 && terminal->screen_x != NUM_COLS-1)
		{
			terminal->wrap_flag = 0;
			return;
		}


		if(c == '\n' && terminal->screen_x == 0 && terminal->wrap_flag ==1 && terminal->screen_y == NUM_ROWS-1 && terminal->screen_x != NUM_COLS-1)
		{
			terminal->wrap_flag = 0;
			return;
		}
	}
	else
	{
		if(c == '\n' && terminal->screen_x != 0 && terminal->wrap_flag !=1 && terminal->screen_y != NUM_ROWS-1 && terminal->screen_x != NUM_COLS-1)
		{
			terminal->wrap_flag = 0;
			putc_storage(c,terminal);
			return;
		}

		if(c == '\n' && terminal->screen_x != 0 && terminal->wrap_flag !=1 && terminal->screen_y != NUM_ROWS-1 && terminal->screen_x == NUM_COLS-1)
		{
			terminal->wrap_flag = 0;
			putc_storage(c,terminal);
			return;
		}

		if(c == '\n' && terminal->screen_x != 0 && terminal->wrap_flag !=1 && terminal->screen_y == NUM_ROWS-1 && terminal->screen_x != NUM_COLS-1)
		{
			terminal->wrap_flag = 0;
			putc_storage(c,terminal);
			terminal->screen_y = NUM_ROWS-1;
			storage_scroll(terminal);
			return;
		}

		if(c == '\n' && terminal->screen_x != 0 && terminal->wrap_flag !=1 && terminal->screen_y == NUM_ROWS-1 && terminal->screen_x == NUM_COLS-1)
		{
			terminal->wrap_flag = 0;
			putc_storage(c,terminal);
			terminal->screen_y = NUM_ROWS-1;
			storage_scroll(terminal);
			return;
		}

		if(c == '\n' && terminal->screen_x != 0 && terminal->wrap_flag ==1 && terminal->screen_y != NUM_ROWS-1 && terminal->screen_x != NUM_COLS-1)
		{
			terminal->wrap_flag = 0;
			putc_storage(c,terminal);
			return;
		}

		if(c == '\n' && terminal->screen_x != 0 && terminal->wrap_flag ==1 && terminal->screen_y != NUM_ROWS-1 && terminal->screen_x == NUM_COLS-1)
		{
			putc_storage(c,terminal);
			terminal->wrap_flag = 0;
			
			return;
		}

		if(c == '\n' && terminal->screen_x != 0 && terminal->wrap_flag ==1 && terminal->screen_y == NUM_ROWS-1 && terminal->screen_x != NUM_COLS-1)
		{
			terminal->wrap_flag = 0;
			putc_storage(c,terminal);
			terminal->screen_y = NUM_ROWS-1;
			storage_scroll(terminal);
			return;
		}

		if(c == '\n' && terminal->screen_x != 0 && terminal->wrap_flag ==1 && terminal->screen_y == NUM_ROWS-1 && terminal->screen_x == NUM_COLS-1)
		{
			terminal->wrap_flag = 0;
			putc_storage(c,terminal);
			terminal->screen_y = NUM_ROWS-1;
			storage_scroll(terminal);
			return;
		}

		if(c == '\n' && terminal->screen_x == 0 && terminal->wrap_flag !=1 && terminal->screen_y != NUM_ROWS-1 && terminal->screen_x != NUM_COLS-1)
		{
			terminal->wrap_flag = 0;
			putc_storage(c,terminal);
			return;
		}


		if(c == '\n' && terminal->screen_x == 0 && terminal->wrap_flag !=1 && terminal->screen_y == NUM_ROWS-1 && terminal->screen_x != NUM_COLS-1)
		{
			terminal->wrap_flag = 0;
			putc_storage(c,terminal);
			terminal->screen_y = NUM_ROWS-1;
			storage_scroll(terminal);
			return;
		}


		if(c == '\n' && terminal->screen_x == 0 && terminal->wrap_flag ==1 && terminal->screen_y != NUM_ROWS-1 && terminal->screen_x != NUM_COLS-1)
		{
			terminal->wrap_flag = 0;
			return;
		}


		if(c == '\n' && terminal->screen_x == 0 && terminal->wrap_flag ==1 && terminal->screen_y == NUM_ROWS-1 && terminal->screen_x != NUM_COLS-1)
		{
			terminal->wrap_flag = 0;
			return;
		}
	}
	return;
}



/* Function that adds characters to the screen
 * PARAMETERS: c - the character to write
 *				terminal - current terminal to scroll.  Terminal will either be curr_terminal or current_process->terminal depending on if call is system call driven or interrupt driven respectively
 * RETURNS: 0 on success, -1 on failure (ie. line buf is full)
 * INFO: I pretty much made a truth table because there were too many conflicting edge cases. 
 */
void screen_add_char(uint8_t c,term_t* terminal)
{
	if(terminal->line_buf_index < BUFFER_SIZE)
	{
		//Read these if statments like a truth table
		if(c != '\n' && terminal->screen_x != 0 && terminal->wrap_flag !=1 && terminal->screen_y != NUM_ROWS-1 && terminal->screen_x != NUM_COLS-1) 
		{	
			putc_screen(c,terminal);
			return;
		}

		if(c != '\n' && terminal->screen_x != 0 && terminal->wrap_flag !=1 && terminal->screen_y != NUM_ROWS-1 && terminal->screen_x == NUM_COLS-1)
		{
			putc_screen(c,terminal);
			putc_screen(10,terminal); //emulate a new line
			return;
		}

		if(c != '\n' && terminal->screen_x != 0 && terminal->wrap_flag !=1 && terminal->screen_y == NUM_ROWS-1 && terminal->screen_x != NUM_COLS-1)
		{
			putc_screen(c,terminal);
			return;

		}

		if(c != '\n' && terminal->screen_x != 0 && terminal->wrap_flag !=1 && terminal->screen_y == NUM_ROWS-1 && terminal->screen_x == NUM_COLS-1)
		{
			putc_screen(c,terminal);
			putc_screen(10,terminal); //emulate a new line
			video_mem_scroll(terminal);
			return;

		}

		if(c != '\n' && terminal->screen_x != 0 && terminal->wrap_flag ==1 && terminal->screen_y != NUM_ROWS-1 && terminal->screen_x != NUM_COLS-1)
		{
			putc_screen(c,terminal);
			return;
		}

		if(c != '\n' && terminal->screen_x != 0 && terminal->wrap_flag ==1 && terminal->screen_y != NUM_ROWS-1 && terminal->screen_x == NUM_COLS-1)
		{
			putc_screen(c,terminal);
			putc_screen(10,terminal); //emulate a new line
			return;
		}

		if(c != '\n' && terminal->screen_x != 0 && terminal->wrap_flag ==1 && terminal->screen_y == NUM_ROWS-1 && terminal->screen_x != NUM_COLS-1)
		{
			putc_screen(c,terminal);
			return;
		}

		if(c != '\n' && terminal->screen_x != 0 && terminal->wrap_flag ==1 && terminal->screen_y == NUM_ROWS-1 && terminal->screen_x == NUM_COLS-1)
		{
			putc_screen(c,terminal);
			putc_screen(10,terminal); //emulate a new line
			video_mem_scroll(terminal);
			return;
		}

		if(c != '\n' && terminal->screen_x == 0 && terminal->wrap_flag !=1 && terminal->screen_y != NUM_ROWS-1 && terminal->screen_x != NUM_COLS-1)
		{
			putc_screen(c,terminal);
			return;
		}

		if(c != '\n' && terminal->screen_x == 0 && terminal->wrap_flag !=1 && terminal->screen_y == NUM_ROWS-1 && terminal->screen_x != NUM_COLS-1)
		{
			putc_screen(c,terminal);
			return;
		}


		if(c != '\n' && terminal->screen_x == 0 && terminal->wrap_flag ==1 && terminal->screen_y != NUM_ROWS-1 && terminal->screen_x != NUM_COLS-1)
		{
			putc_screen(c,terminal);
			return;
		}


		if(c != '\n' && terminal->screen_x == 0 && terminal->wrap_flag ==1 && terminal->screen_y == NUM_ROWS-1 && terminal->screen_x != NUM_COLS-1)
		{
			putc_screen(c,terminal);
			return;
		}


		if(c == '\n' && terminal->screen_x != 0 && terminal->wrap_flag !=1 && terminal->screen_y != NUM_ROWS-1 && terminal->screen_x != NUM_COLS-1)
		{
			putc_screen(c,terminal);
			return;
		}

		if(c == '\n' && terminal->screen_x != 0 && terminal->wrap_flag !=1 && terminal->screen_y != NUM_ROWS-1 && terminal->screen_x == NUM_COLS-1)
		{
			putc_screen(c,terminal);
			return;
		}

		if(c == '\n' && terminal->screen_x != 0 && terminal->wrap_flag !=1 && terminal->screen_y == NUM_ROWS-1 && terminal->screen_x != NUM_COLS-1)
		{
			putc_screen(c,terminal);
			video_mem_scroll(terminal);
			return;
		}

		if(c == '\n' && terminal->screen_x != 0 && terminal->wrap_flag !=1 && terminal->screen_y == NUM_ROWS-1 && terminal->screen_x == NUM_COLS-1)
		{
			putc_screen(c,terminal);
			video_mem_scroll(terminal);
			return;
		}

		if(c == '\n' && terminal->screen_x != 0 && terminal->wrap_flag ==1 && terminal->screen_y != NUM_ROWS-1 && terminal->screen_x != NUM_COLS-1)
		{
			putc_screen(c,terminal);
			return;
		}

		if(c == '\n' && terminal->screen_x != 0 && terminal->wrap_flag ==1 && terminal->screen_y != NUM_ROWS-1 && terminal->screen_x == NUM_COLS-1)
		{
			putc_screen(c,terminal);
			return;
		}

		if(c == '\n' && terminal->screen_x != 0 && terminal->wrap_flag ==1 && terminal->screen_y == NUM_ROWS-1 && terminal->screen_x != NUM_COLS-1)
		{
			putc_screen(c,terminal);
			video_mem_scroll(terminal);
			return;
		}

		if(c == '\n' && terminal->screen_x != 0 && terminal->wrap_flag ==1 && terminal->screen_y == NUM_ROWS-1 && terminal->screen_x == NUM_COLS-1)
		{
			putc_screen(c,terminal);
			video_mem_scroll(terminal);
			return;
		}

		if(c == '\n' && terminal->screen_x == 0 && terminal->wrap_flag !=1 && terminal->screen_y != NUM_ROWS-1 && terminal->screen_x != NUM_COLS-1)
		{
			putc_screen(c,terminal);
			return;
		}


		if(c == '\n' && terminal->screen_x == 0 && terminal->wrap_flag !=1 && terminal->screen_y == NUM_ROWS-1 && terminal->screen_x != NUM_COLS-1)
		{
			putc_screen(c,terminal);
			video_mem_scroll(terminal);
			return;
		}
	}
	else
	{
		if(c == '\n' && terminal->screen_x != 0 && terminal->wrap_flag !=1 && terminal->screen_y != NUM_ROWS-1 && terminal->screen_x != NUM_COLS-1)
		{
			putc_screen(c,terminal);
			return;
		}

		if(c == '\n' && terminal->screen_x != 0 && terminal->wrap_flag !=1 && terminal->screen_y != NUM_ROWS-1 && terminal->screen_x == NUM_COLS-1)
		{
			putc_screen(c,terminal);
			return;
		}

		if(c == '\n' && terminal->screen_x != 0 && terminal->wrap_flag !=1 && terminal->screen_y == NUM_ROWS-1 && terminal->screen_x != NUM_COLS-1)
		{
			putc_screen(c,terminal);
			video_mem_scroll(terminal);
			return;
		}

		if(c == '\n' && terminal->screen_x != 0 && terminal->wrap_flag !=1 && terminal->screen_y == NUM_ROWS-1 && terminal->screen_x == NUM_COLS-1)
		{
			putc_screen(c,terminal);
			video_mem_scroll(terminal);
			return;
		}

		if(c == '\n' && terminal->screen_x != 0 && terminal->wrap_flag ==1 && terminal->screen_y != NUM_ROWS-1 && terminal->screen_x != NUM_COLS-1)
		{
			putc_screen(c,terminal);
			return;
		}

		if(c == '\n' && terminal->screen_x != 0 && terminal->wrap_flag ==1 && terminal->screen_y != NUM_ROWS-1 && terminal->screen_x == NUM_COLS-1)
		{
			putc_screen(c,terminal);			
			return;
		}

		if(c == '\n' && terminal->screen_x != 0 && terminal->wrap_flag ==1 && terminal->screen_y == NUM_ROWS-1 && terminal->screen_x != NUM_COLS-1)
		{
			putc_screen(c,terminal);
			video_mem_scroll(terminal);
			return;
		}

		if(c == '\n' && terminal->screen_x != 0 && terminal->wrap_flag ==1 && terminal->screen_y == NUM_ROWS-1 && terminal->screen_x == NUM_COLS-1)
		{
			putc_screen(c,terminal);
			video_mem_scroll(terminal);
			return;
		}

		if(c == '\n' && terminal->screen_x == 0 && terminal->wrap_flag !=1 && terminal->screen_y != NUM_ROWS-1 && terminal->screen_x != NUM_COLS-1)
		{
			putc_screen(c,terminal);
			return;
		}


		if(c == '\n' && terminal->screen_x == 0 && terminal->wrap_flag !=1 && terminal->screen_y == NUM_ROWS-1 && terminal->screen_x != NUM_COLS-1)
		{
			putc_screen(c,terminal);
			video_mem_scroll(terminal);
			return;
		}
	}
	return;
}

/* storage_scroll
 * Description: Causes storage that terminal points to scroll by shifting each line up by one.  Function is called when certain edge cases are met inside screen_add_char conditionals
 * Inputs: terminal - current terminal to scroll.  Terminal will either be curr_terminal or current_process->terminal depending on if call is system call driven or interrupt driven respectively
 * Outputs: none
 * Returns: none
 * Side Effects: scrolls terminal storage up 
 */
void storage_scroll(term_t* terminal)
{
	int i, j;
	//Shift data upwards in video memory
	for(i=0; i<NUM_COLS; i++)
	{
		for(j=0; j<NUM_ROWS-1; j++)
		{
		*(uint8_t *)(terminal->storage+ ((NUM_COLS*j + i) << 1)) = *(uint8_t *)(terminal->storage+ ((NUM_COLS*(j+1) + i) << 1));
        *(uint8_t *)(terminal->storage+ ((NUM_COLS*j + i) << 1) + 1) = *(uint8_t *)(terminal->storage + ((NUM_COLS*(j+1) + i) << 1) + 1);
		}
	}
	for(i=0; i<NUM_COLS; i++)
	{
		for(j=NUM_ROWS-1; j<NUM_ROWS; j++)
			{
			*(uint8_t *)(terminal->storage+ ((NUM_COLS*j + i) << 1)) = ' ';
       		*(uint8_t *)(terminal->storage+ ((NUM_COLS*j + i) << 1) + 1) = '\a';
			}
	}
}


/* video_mem_scroll
 * Description: Causes video memory to scroll by shifting each line up by one.  Function is called when certain edge cases are met inside storage add char conditionals
 * Inputs: terminal - current terminal to scroll.  Terminal will either be curr_terminal or current_process->terminal depending on if call is system call driven or interrupt driven respectively
 * Outputs: none
 * Returns: none
 * Side Effects: Scrolls terminal video memory up
 */
void video_mem_scroll(term_t* terminal)
{
	int i, j;
	//Shift data upwards in video memory
	for(i=0; i<NUM_COLS; i++)
	{
		for(j=0; j<NUM_ROWS-1; j++)
		{
			*(uint8_t *)(terminal->video_mem+ ((NUM_COLS*j + i) << 1)) = *(uint8_t *)(terminal->video_mem+ ((NUM_COLS*(j+1) + i) << 1));
        	*(uint8_t *)(terminal->video_mem+ ((NUM_COLS*j + i) << 1) + 1) = *(uint8_t *)(terminal->video_mem + ((NUM_COLS*(j+1) + i) << 1) + 1);
		}
	}
	for(i=0; i<NUM_COLS; i++)
	{
		for(j=NUM_ROWS-1; j<NUM_ROWS; j++)
			{
				*(uint8_t *)(terminal->video_mem + ((NUM_COLS*j + i) << 1)) = ' ';
       		 	*(uint8_t *)(terminal->video_mem + ((NUM_COLS*j + i) << 1) + 1) = '\a';
			}
	}
}


/* putc_storage
 * Description: populates a character to storage
  * Inputs: c - character to populate the screen with
 *		   terminal - current terminal to write character to.  Terminal will either be curr_terminal or current_process->terminal depending on if call is system call driven or interrupt driven respectively
 * Outputs: none
 * Returns: none
 * Side Effects: moves a character into storage and increments screen_x and screen_y fields
 */
void putc_storage(uint8_t c,term_t* terminal)
{
    if(c == '\n' || c == '\r') {
        terminal->screen_y++;
        terminal->screen_x=0;
    } else {
        *(uint8_t *)(terminal->storage + ((NUM_COLS*terminal->screen_y + terminal->screen_x) << 1)) = c;
        *(uint8_t *)(terminal->storage + ((NUM_COLS*terminal->screen_y + terminal->screen_x) << 1) + 1) = ATTRIB;
        terminal->screen_x++;
        terminal->screen_x %= NUM_COLS;
        terminal->screen_y = (terminal->screen_y + (terminal->screen_x / NUM_COLS)) % NUM_ROWS;
    }
}

/* putc_screen
 * Description: displays a characeter to the screen.  
 * Inputs: c - character to populate the screen with
 *		   terminal - current terminal to write character to.  erminal will either be curr_terminal or current_process->terminal depending on if call is system call driven or interrupt driven respectively
 * Outputs: outputs a character to the screen
 * Returns: none
 * Side Effects: displays a chracter to the screen 
 */
void putc_screen(uint8_t c,term_t* terminal)
{
    if(c == '\n' || c == '\r') {
    	// DO NOTHING
    } else {
        *(uint8_t *)(terminal->video_mem + ((NUM_COLS*terminal->screen_y + terminal->screen_x) << 1)) = c;
        *(uint8_t *)(terminal->video_mem + ((NUM_COLS*terminal->screen_y + terminal->screen_x) << 1) + 1) = ATTRIB;
    }
}

/* storage_delete_char
 * Description: deletes a character fromm storage.  If the line buffer index is 0, then it can't delete anything in the buffer. Otherwise delete from  the buffer.  Function is called
 *(				before screen_delete and it will set up the terminal fields prior to calling screen_delete
 * Inputs: terminal - current terminal to delete charcter from.  Terminal will either be curr_terminal or current_process->terminal depending on if call is interrupt or system call driven 
 * Outputs: none
 * Returns: none
 * Side Effects: deletes a character from the storage buffer
 */
void storage_delete_char(term_t* terminal)
{
	if(terminal->line_buf_index == 0)
		return ;
	if(terminal->screen_x == 0)
	{
		terminal->screen_y--;
		terminal->screen_x = NUM_COLS -1;
		 *(uint8_t *)(terminal->storage + ((NUM_COLS*terminal->screen_y + terminal->screen_x) << 1)) = ' ';
    	 *(uint8_t *)(terminal->storage + ((NUM_COLS*terminal->screen_y + terminal->screen_x) << 1) + 1) = ATTRIB;
 	}
 	else 
 	{	
 		terminal->screen_x --;
 		*(uint8_t *)(terminal->storage + ((NUM_COLS*terminal->screen_y + terminal->screen_x) << 1)) = ' ';
    	*(uint8_t *)(terminal->storage + ((NUM_COLS*terminal->screen_y + terminal->screen_x) << 1) + 1) = ATTRIB;
 	}
     return;
}

/* screen_delete
 * Description: deletes a character on the screen.  If the line buffer index is 0, then it can't delete anything on the screen. Otherwise delete from the screen.  Storage_delete_char is called
 *				first so it will actually update the terminal fields before entering this function.  
 * Inputs: terminal - current terminal to delete charcter from.  Terminal will either be curr_terminal or current_process->terminal depending on if call is interrupt or system call driven 
 * Outputs: none
 * Returns: none
 * Side Effects: deletes a character from the screen
 */
void screen_delete(term_t* terminal)
{
	if(terminal->line_buf_index == 0)
		return;

 	*(uint8_t *)(terminal->video_mem + ((NUM_COLS*terminal->screen_y + terminal->screen_x) << 1)) = ' ';
    *(uint8_t *)(terminal->video_mem + ((NUM_COLS*terminal->screen_y + terminal->screen_x) << 1) + 1) = ATTRIB;
     return;
}


/* terminal_change
 * Description: Function that controls switching from one terminal to the next.  Only intterrupt driven. Copies terminal storage
 * Inputs: none
 * Outputs: none
 * Returns: none
 * Side Effects: changes terminal by changing curr terminal pointer and copying the storage of the new curr terminal to video memory
 */
void terminal_change(int terminal_num)
{
	int i;
	curr_terminal = &terminal[terminal_num];
	for(i=0; i<NUM_ROWS*NUM_COLS; i++)
    {
        *(uint8_t *)(curr_terminal->video_mem + (i << 1)) = *(uint8_t *)(curr_terminal->storage + (i << 1));
        *(uint8_t *)(curr_terminal->video_mem + (i << 1) + 1) = *(uint8_t *)(curr_terminal->storage + (i << 1) + 1); 
    }
}

/* init_terminals
 * Description: Initializes the terminals and sets the storage field to point to its respective 4kb page
 * Inputs: none
 * Outputs: none
 * Returns: none
 * Side Effects: Initializes the terminal fields to 0 and ptrs to NULL
 */
void init_terminals()
{
	int i,j;
	for(i = 0; i< NUM_TERM; i++)
	{
		for(j=0;j<BUFF_SIZE;j++)
		{
			terminal[i].line_buf[i] = 0;
		}
		terminal[i].line_buf_index = 0;
		terminal[i].wrap_flag = 0;
		terminal[i].screen_x = 0;
		terminal[i].screen_y = 0;
		terminal[i].video_mem = (uint8_t*)VMEM_START;
		switch(i)
		{
			case 0:
			terminal[i].storage =  (uint8_t*)VIDEO_0;
			break;

			case 1:
			terminal[i].storage =  (uint8_t*)VIDEO_1;
			break;

			case 2:
			terminal[i].storage =  (uint8_t*)VIDEO_2;
			break;
		}
		clear_storage(&terminal[i]);
		terminal[i].term_read = 0;
		terminal[i].write_lock = 0;
		terminal[i].terminal_read_ret = 0;
	}
	curr_terminal = &terminal[0];
}

