#include  "file_sys.h"

//tasks to carry out:
//FILE SYSTEM DRIVER:
//  File & Directory open/close/read/write (1)
//  Read text file (1)
//  Read non-text file (1)
//  Read Large Files (1)
//  Read_Directory (1)

//initialize the file system


boot_block_t * bootblock;
pcb_t * current_process = NULL;


/*
 * void file_system_init(void * addrs)
 * DESCRIPTION : Points to the bootblock at the start of the file system image
 * INPUTS : addr - address to set the bootblock pointer to 
 * OUTPUTS : none
 * RETURN VALUE : none
 * SIDE EFFECTS : none
 *
 */
void
file_system_init(void * addr)
{
	if (addr == NULL)
		return;
    bootblock = addr;
}


// fs_dir_read and fs_read are the only two functions that
// do anything because our fs is read only

//--------------directory orwc functions--------------

/*
 * int32_t fs_dir_open()
 * DESCRIPTION : Open a directory in filesystem
 * INPUTS : none
 * OUTPUTS : none
 * RETURN VALUE : returns 0
 * SIDE EFFECTS : Absolutely nothing
 *
 */
int32_t
fs_dir_open()
{
    return 0;
}
 
/*
 * int32_t fs_dir_read(int32_t fd, void * buffer, int32_t count)
 * DESCRIPTION : Read a directory in filesystem
 * INPUTS : buffer - address to write the directory into
 *			count - number of bytes to read
 *			fd - file descriptor number
 * OUTPUTS : none
 * RETURN VALUE : number of bytes read on success and -1 on failure
 * SIDE EFFECTS : Similar to ls, allows user to read contents of directory
 *
 */
int32_t
fs_dir_read(int32_t fd, void * buffer, int32_t count)
{

	uint32_t i = 0;
	char* buf;
	dentry_t curr_directory_entry;
	f_desc_t* curr_file_desc;

	if (buffer == NULL)
		return -1;

	//char pointer to buffer
	buf = (char*)buffer; 

	curr_file_desc = &(current_process->fid)[fd];

	// The file descriptor can't be stdin or stdout
	if(fd == 0 || fd == 1)
		return -1;

	//The offset has exceeded the number of directory entries, therefore we are done reading 
	if(curr_file_desc->offs >= bootblock -> num_dir_entries)
	{
		for(i = 0; i < count; i++)
			buf[i] = 0;
		return 0;
	}

	if(read_dentry_by_index(curr_file_desc->offs,&curr_directory_entry) == -1)	
		return -1;

	//Intialize the buffer to 0 
	for(i = 0; i < count; i++)
		buf[i] = 0;

	//Copy the file name to the buffer
	for ( i = 0; i < 32 && (curr_directory_entry.file_name)[i] != '\0'; ++i)
	{
		buf[i] = (curr_directory_entry.file_name)[i];
	}


	//Increase the offset so we can grab the next directory
	curr_file_desc->offs += 1;
	return i;
}

/*
 * int32_t fs_dir_write(int32_t fd, const void* buffer, int32_t count)
 * DESCRIPTION : Write a directory to our filesystem
 * INPUTS : buffer - garbage
 *			fd -garbage
 * 			count - garbage
 * OUTPUTS : none
 * RETURN VALUE : -1 every time
 * SIDE EFFECTS : Does absolutely nothing
 *
 */
int32_t
fs_dir_write(int32_t fd, const void * buffer, int32_t count)
{
    return -1;
}

/*
 * int32_t fs_dir_close()
 * DESCRIPTION : Closes a directory in our file system
 * INPUTS : none
 * OUTPUTS : none
 * RETURN VALUE : 0, always
 * SIDE EFFECTS : Does nothing
 *
 */
int32_t
fs_dir_close()
{
    return 0;
}



//--------------file orwc functions--------------


/*
 * int32_t fs_open()
 * DESCRIPTION : Open a file
 * INPUTS : none
 * OUTPUTS : none
 * RETURN VALUE : 0 every time
 * SIDE EFFECTS : none, does nothing
 *
 */
int32_t
fs_open()
{
    return 0;
}

/*
 * int32_t fs_read(int32_t fd, void * buffer, int32_t count)
 * DESCRIPTION : Read a file
 * INPUTS : fd - file descriptor index of file to read
 *			buffer - populates data from file system image into buffer
 *			count - number of bytes to copy to buffer
 * OUTPUTS : none
 * RETURN VALUE : 0 on end of file, -1 on failure, or written on total written to the buffer
 * SIDE EFFECTS : puts number of bytes in buffer to count
 */
int32_t
fs_read(int32_t fd, void * buffer, int32_t count)
{
	f_desc_t * fdescriptor;
	int32_t status_written;
	uint32_t inode_number;

	// error check
	if (buffer == NULL || fd == 0 || fd == 1)
		return -1;

	fdescriptor = & (current_process->fid)[fd];

	// calculates the inode index and checks whether a valid inode_number was calculated
	inode_number = (((uint32_t)(fdescriptor->inode) - (uint32_t)bootblock - (uint32_t)BLOCK_SIZE))/(uint32_t)BLOCK_SIZE;

	if (inode_number >= bootblock->num_inodes)
		return -1;

	//stores the total_written for THIS instance read_data or status meaning 0 for end of file, -1 for failue
	status_written = read_data(inode_number, fdescriptor->offs, (uint8_t *)buffer, count);
	
	// updates offset so file read can pick up where left off
	fdescriptor->offs += status_written;

	return status_written;
}

/*
 * int32_t fs_write(int32_t fd, const void * buffer, int32_t count)
 * DESCRIPTION : Writes a file
 * INPUTS : buffer - garbage
 *			fd - garbage
 *			count - garbage
 * OUTPUTS : returns -1
 * RETURN VALUE : -1 always
 * SIDE EFFECTS : Does nothing because our fs is READ ONLY!!!
 *
 */
int32_t
fs_write(int32_t fd, const void * buffer, int32_t count)
{
    return -1;
}

/*
 * int32_t fs_close()
 * DESCRIPTION : Closes a file
 * INPUTS : none
 * OUTPUTS : none
 * RETURN VALUE : 0 every time
 * SIDE EFFECTS : this filesystem is read only, we don't need most of this
 *
 */
int32_t
fs_close()
{
    return 0;
}


//--------------------------------------------------------------------------------------------
//--------------------------------additional stuff--------------------------------------------
//--------------------------------------------------------------------------------------------

/*
 * int32_t read_dentry_by_name (const uint8_t* fname, dentry_t* dentry)
 * DESCRIPTION : Reads a file and populates a dentry_t struct if found
 * INPUTS : fname- name of file
 *			dentry - dentry_t struct
 * OUTPUTS : populated a dentry_t struct if found
 * RETURN VALUE : -1 on failure and 0 on successful
 * SIDE EFFECTS : Populates Directory Entry struct
 *
 */

int32_t
read_dentry_by_name (const uint8_t* fname, dentry_t* dentry)
{
	uint32_t i;
	uint32_t length;
	uint8_t name[FILENAME_BYTES] = {0}; // Zero all fields

	// error handle
	if (fname == NULL || dentry == NULL)
		return -1;

	length = strlen((int8_t*)fname);
	for(i = 0; i< length || i < FILENAME_BYTES; i++)
		name[i] = fname[i];

	name[FILENAME_BYTES-1] = '\0';

	// write found information into the passed in dentry location
	for(i = 0; i < bootblock->num_dir_entries; i++)
	{
		if(!strncmp((void*)name,(void*)((bootblock->dir_entry)[i]).file_name,FILENAME_BYTES-1))
		{
			memcpy(dentry, &((bootblock->dir_entry)[i]),sizeof(dentry_t));
			return 0;
		}
	}
	return -1;
}


/*
 * int32_t read_dentry_by_index (uint32_t index, dentry_t* dentry)
 * DESCRIPTION : Takes an index to an inode and populates a dentry structure
 * INPUTS : fname- index corresponding to an inode which correcponds to a directory entry
 *			dentry - dentry_t struct
 * OUTPUTS : populated a dentry_t struct if found
 * RETURN VALUE : -1 on failure and 0 on successful
 * SIDE EFFECTS :Populates Directory Entry struct
 *
 */

int32_t
read_dentry_by_index (uint32_t index, dentry_t* dentry)
{
	if(index >= bootblock->num_dir_entries || dentry == NULL)
		return -1;
	//copy to memory

	dentry_t* copy_to_dentry = &(bootblock->dir_entry)[index];

	memcpy((void*)dentry, (void*)copy_to_dentry, sizeof(dentry_t));
	return 0;
}

/*
 * DESCRIPTION : Uses inode to find file, copies length into buffer starting at offset into file
 * INPUTS : inode - inode index
 *			offset - how far into file in order to start reading
 *			buf - copy the file information to
 *			length - number of bytes to copy to buf
 * OUTPUTS : populates buffer with file information starting at offset and going for length
 * RETURN VALUE : -1 on failure and 0 on end of file and count.written for how much written to buffer
 * SIDE EFFECTS :Populates buf with file data
 *
 */

int32_t read_data(uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length)
{
	//determine how far into the file block to go
	uint32_t into_block = offset % BLOCK_SIZE;
	//determine which block we need to go into
	uint32_t which_block = offset / BLOCK_SIZE;
	//helps us determine whats going on
	read_desc_t descriptor;
	//find the index of the correct data block
	uint32_t indx;
	//pointer to the inode
	index_node_t * inode_ptr = (index_node_t *)((uint32_t)bootblock + (uint32_t)BLOCK_SIZE*(inode + 1));
	//start address of data
	uint8_t * addr_data_start = (uint8_t*)(((bootblock->num_inodes + 1) * BLOCK_SIZE) + (uint32_t)bootblock);
	uint8_t * addr_data;


	// NULL check
	if (buf == NULL)
		return -1;
	//if the inode number is invalid return -1
	if(inode >= bootblock->num_inodes)
		return -1;
	//  we have read to the end of the file
	if(offset >= inode_ptr -> length_bytes)
		return 0;

	//how much of the remaining file we have to copy if length +offset > length of file or so executable copy does not page fault
	if(length+offset > inode_ptr -> length_bytes)
		length =  (inode_ptr -> length_bytes) - offset;

	// initialize the descriptor
	descriptor.written = 0;
	descriptor.count = length;
	descriptor.buf = buf;
	descriptor.error = 0;

	//find the starting address of the data block(s) we need to read from
	indx = (inode_ptr->nth_data_block)[which_block];
	addr_data = (uint8_t*)((uint32_t)(addr_data_start) + (uint32_t)(BLOCK_SIZE*indx + into_block));

	// copy to memory and update descriptor as needed
	while (descriptor.count != 0)
	{
		// case where the amount we want to copy is smaller than however much data we have access to in that data block
		if (descriptor.count < (BLOCK_SIZE - into_block))
		{
			memcpy(descriptor.buf, addr_data, descriptor.count);
			descriptor.written += descriptor.count;
			descriptor.count = 0;
		}
		// case where what we want to copy over is larger or equal to a data block 
		else
		{
			memcpy(descriptor.buf, addr_data, BLOCK_SIZE - into_block);
			descriptor.written += BLOCK_SIZE - into_block;
			descriptor.count -= BLOCK_SIZE - into_block;
			descriptor.buf += BLOCK_SIZE - into_block;
			which_block++;
			into_block =
			indx = (inode_ptr->nth_data_block)[which_block];
			addr_data = (uint8_t*)((uint32_t)(addr_data_start) + (uint32_t)(BLOCK_SIZE*indx));
		}
	}
return descriptor.written;
}

/*
 * index_node_t* calc_inode_ptr(dentry_t* dt)
 * INPUTS : dt - pointer to a dentry_t struct
 * OUTPUTS : none
 * RETURN VALUE : calculates and returns the index_node pointer value
 * SIDE EFFECTS : none
 */

index_node_t* calc_inode_ptr(dentry_t* dt)
{
		return ((index_node_t *)((uint32_t)bootblock + SIZE_OF_BOOTBLOCK + ((dt->inode_num)*sizeof(index_node_t))));
}