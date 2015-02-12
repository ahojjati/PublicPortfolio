#ifndef _FILE_SYS_H
#define _FILE_SYS_H


#include "types.h"
#include "lib.h"
#include "file_sys_struct.h"
#include "pcb_struct.h"

#define BLOCK_SIZE				4096
#define CURR_PROC_ADDR          0x08000000
#define SIZE_OF_BOOTBLOCK		4096


#ifndef ASM

/* file system directory functions for fops table */

// opens the directory
int32_t fs_dir_open();
// reads the directory file
int32_t fs_dir_read(int32_t, void *, int32_t);
// writes to the directory file
int32_t fs_dir_write(int32_t fd, const void*, int32_t);
// closes the directory
int32_t fs_dir_close();

/* file system file functions for fops table*/

// opens a file
int32_t fs_open();
// reads a file
int32_t fs_read(int32_t, void *, int32_t);
// writes to a file
int32_t fs_write(int32_t fd, const void *, int32_t);
// clsoes a file
int32_t fs_close();

/* helper functions */
// looks for a file by whatever executable name is passed in
int32_t read_dentry_by_name (const uint8_t* fname, dentry_t* dentry);
// looks for a file by whatever index value is passed in
int32_t read_dentry_by_index( uint32_t index, dentry_t* dentry);
// copies a valid file's program image (however much is desired) into a specified address 
int32_t read_data (uint32_t inode, uint32_t offset,uint8_t* buf, uint32_t length);

//init file system
void file_system_init(void*);

//calculate the index node pointer value
index_node_t* calc_inode_ptr(dentry_t* dt);

// structures for seeking, verifying, and keeping track of processes
extern boot_block_t * bootblock;
extern pcb_t * current_process;
#endif

#endif
