#ifndef _PAGE_H
#define _PAGE_H


#include "types.h"
#include "lib.h"
#include "x86_desc.h"

#define ENABLE_MASK        							0x80000000
#define NUM_TABLE_ENTRIES 							1024
#define FOUR_MD										0x10000010
#define RESERVED_CR4_SET_TO_ZERO					0x000007FF
#define KERNEL_ADDRESS								1
#define BOOL_KB_PAGE_TRUE							1
#define BOOL_KB_PAGE_FALSE							0	
#define VIDEO_MEM_VIRTUAL							0xB8000
#define VIDEO_MEM_20_MOST_SIG_BITS_PAGE_TAB			0xB8000 >> 12
#define	USER 										1
#define SUPERVISOR 									0
#define BIT_ON										1
#define BIT_OFF										0
#define DPL_KERNEL									0
#define DPL_USER									3
#define RESERVED									0
#define PROCESS_ADDRESS1							2
#define PROCESS_ADDRESS2							3
#define PROCESS_VIRT_ADDR							32
#define CURR_PROC_ADDR          0x08000000


#ifndef ASM
////////////////////////////////STRUCTURES/////////////////////////////////////
// Page directory entries


////////////////////////////////Prototypes/////////////////////////////////////

//Sets up 2 directory entries and 1 page table
void init_paging();

//Sets up a page dir table
void init_page_dir(page_dir_entry* page_directory);

//Adds a page dir entry to the page dir table specified
void add_page_dir_entry(page_dir_entry* page_directory,uint8_t table_entry,uint32_t ptr_to_page,uint8_t kb_page,uint32_t user_supervisor);

//Sets up a page directory table
void init_page_table(page_table_entry* page_tabl);

//Adds a page table entry to the page table specified
void add_page_table_entry(page_table_entry* page_table,uint32_t table_entry,uint32_t ptr_to_page, uint32_t user_supervisor);

//setup paging for the current process
void setup_page_process(uint32_t pcb_num);

//switch the page directory for the process
void process_switch_page_dir(int32_t pid);


#endif
#endif

