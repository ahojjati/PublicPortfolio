#ifndef _FILE_SYS_STRUCT_H
#define _FILE_SYS_STRUCT_H

#include "lib.h"
#include "types.h"

#define BOOT_BLOCK_RESERVED_BYTES 52
#define DIR_ENTRY_RESERVED_BYTES 24
#define FILENAME_BYTES 32
#define MAX_NUM_DIR_ENTRIES 63
#define NTH_DATA_BLOCK_MAX 1023
#define BLOCK_SIZE	4096

#ifndef ASM

typedef struct f_ops
{
	int32_t (*open)();
	int32_t (*read)(int32_t,void*,int32_t);
	int32_t (*write)(int32_t,const void*,int32_t);
	int32_t (*close)();
} __attribute__((packed)) f_ops_t; //16 bytes


typedef struct index_node
{
	uint32_t length_bytes;
	uint32_t nth_data_block[NTH_DATA_BLOCK_MAX];
}__attribute__((__packed__)) index_node_t;

typedef struct f_desc
{
	f_ops_t * fops;
	index_node_t * inode;
	uint32_t offs;
	uint32_t flags;
} __attribute__((packed)) f_desc_t; //16 bytes

typedef struct directory_entry
{
	uint8_t file_name[FILENAME_BYTES];
	uint32_t file_type;
	uint32_t inode_num;
	uint8_t reserved[DIR_ENTRY_RESERVED_BYTES];
}  __attribute__((__packed__)) dentry_t;

//boot_block structure
typedef struct boot_block
{
	uint32_t num_dir_entries;
	uint32_t num_inodes;
	uint32_t num_data_blocks;
	uint8_t reserved[BOOT_BLOCK_RESERVED_BYTES];
	dentry_t dir_entry[MAX_NUM_DIR_ENTRIES];
} __attribute__((__packed__)) boot_block_t;

typedef struct read_descriptor_t
{
	uint32_t written;
	uint32_t count;
	int32_t error;
	uint8_t * buf;
} __attribute__((__packed__)) read_desc_t;

#endif
#endif
