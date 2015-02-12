/*page.c sets up page tables and directories*/

#include "page.h"

//page table and directory defined in x86_desc.S
extern page_table_entry page_tab[PAGE_TABLE_SIZE];
extern page_dir_entry page_dir[PAGE_DIR_SIZE];
extern page_dir_entry page_dir_proc[NUM_PROCESSES][PAGE_DIR_SIZE];
uint32_t process_available;


/*init_paging
 * Description: Initializes the page directory table,page table,video memory, and kernel code
 *               with pointter to a page table or physical memory
 * Inputs: none
 * Outputs: none
 * Returns: none
 * Side Effects: Initializes the page directory table,page table,video memory, and kernel code
 *               with pointter to a page table or physical memory
 */
void init_paging()
{
    unsigned int cr0=0;
    unsigned int cr4=0;
    uint32_t page_table_msb20 = (uint32_t)page_tab >> 12;
    uint32_t i;
    // sets up the page directory
    init_page_dir(page_dir);

    // Populate entry 0 and 1 of page dir
    add_page_dir_entry(page_dir,0,page_table_msb20,BOOL_KB_PAGE_TRUE,USER);     
    add_page_dir_entry(page_dir,1,KERNEL_ADDRESS,BOOL_KB_PAGE_FALSE,SUPERVISOR);

    //init the page table for the 4kb pages
    init_page_table(page_tab);

    //Creates 4kb pages.  Reason to start at 1 is so the first 4kb block is not present in memory. This will give a page fault exception
    for(i = 1; i< NUM_TABLE_ENTRIES; i++)
    {
        add_page_table_entry(page_tab,i,i,USER);
    }


//////////////////Paging set up for processes//////////////////////////
    //initiates 8 page table directories for all 8 processes
    for ( i = 0; i < NUM_PROCESSES; ++i)
    {
        init_page_dir(page_dir_proc[i]);
    }
    // Adds 3 entries to each process. 1st for video memory, second for kernel, and third for program image
    for (i = 0; i < NUM_PROCESSES; ++i)
    {
        add_page_dir_entry(page_dir_proc[i],0,page_table_msb20,BOOL_KB_PAGE_TRUE,USER);
        add_page_dir_entry(page_dir_proc[i],1,KERNEL_ADDRESS,BOOL_KB_PAGE_FALSE,SUPERVISOR);
        add_page_dir_entry(page_dir_proc[i],PROCESS_VIRT_ADDR,i + 2,BOOL_KB_PAGE_FALSE,USER);
    }


// Allows paging process to be performed
    asm volatile ("mov %%CR4, %0":"=c"(cr4));
    cr4 |= FOUR_MD;
    asm volatile ("mov %0, %%CR4"::"c"(cr4));

    asm volatile("mov %0, %%CR3":: "c"(page_dir));

    asm volatile("mov %%CR0, %0": "=c"(cr0));
    cr0 |= ENABLE_MASK;                 
    asm volatile("mov %0, %%CR0":: "c"(cr0));
}

//if we pass in -1 or a pcb_num greater than number of processes
//we use the default directory and dereferencing the current_process
//will result in a page fault
void process_switch_page_dir(int32_t pid)
{
    if(pid == -1)
    {
    asm volatile("mov %0, %%CR3":: "c"(page_dir));
    }
    else
    {
    asm volatile("mov %0, %%CR3":: "c"(page_dir_proc[pid]));
   }
    return;
}

  
/*
 setup_page_dir
 * Description: Initializes the page directory table by setting the RW bit
 * Inputs: page_directory- page dir entry structure that is used to set up a generic directory table
 * Outputs: none
 * Returns: none
 * Side Effects: sets up a Page directory table specified
 */
void init_page_dir(page_dir_entry* page_directory)
{
    int table_entry;  // ranges from 0 to 1023
        for(table_entry=0; table_entry< NUM_TABLE_ENTRIES; table_entry++)
    {
        page_directory[table_entry].present_4mb = BIT_OFF;
        page_directory[table_entry].read_write_4mb = BIT_OFF;
        page_directory[table_entry].user_supervisor_4mb = BIT_OFF;
        page_directory[table_entry].write_through_4mb = BIT_OFF;
        page_directory[table_entry].cache_disabled_4mb = BIT_OFF;
        page_directory[table_entry].accessed_4mb = BIT_OFF;
        page_directory[table_entry].dirty_4mb = BIT_OFF;
        page_directory[table_entry].page_size_4mb = BIT_OFF;
        page_directory[table_entry].global_page_4mb  = BIT_OFF;
        page_directory[table_entry].available_4mb  = BIT_OFF;
        page_directory[table_entry].page_table_attribute_index_4mb  = BIT_OFF;
        page_directory[table_entry].reserved_4mb = BIT_OFF;
        page_directory[table_entry].page_table_base_addr_4mb = 0;
    }
}


/*
add_page_dir_entry
 * Description:Adds an entry to the page directory specified
 * Inputs:  page_directory - populate an entry in the specified page directory 
 *          table_entry - table entry in the Page directory. Range from 0-1023
 *          ptr_to_page - pointer to page(4kb page) or physical mem(4mb page)
 *          kb_page - 1 means that page directory entry points to a 4kb page
 *                    0 means that the page directory entry points to a page table
 * Outputs: none
 * Returns: none
 * Side Effects: sets up an entry in the Page directory table
 */
void add_page_dir_entry(page_dir_entry* page_directory,uint8_t table_entry,uint32_t ptr_to_page,uint8_t kb_page,uint32_t user_supervisor)
{
    if(kb_page)
    {
        page_directory[table_entry].present_4kb = BIT_ON;
        page_directory[table_entry].read_write_4kb = BIT_ON; 
        page_directory[table_entry].user_supervisor_4kb = user_supervisor;
        page_directory[table_entry].write_through_4kb  = BIT_OFF;
        page_directory[table_entry].cache_disabled_4kb = BIT_OFF;
        page_directory[table_entry].accessed_4kb = BIT_OFF;
        page_directory[table_entry].reserved_4kb = BIT_OFF;
        page_directory[table_entry].page_size_4kb = BIT_OFF;
        page_directory[table_entry].global_page_4kb = BIT_OFF;
        page_directory[table_entry].available_4kb = BIT_OFF;
        page_directory[table_entry].page_table_base_addr_4kb = ptr_to_page;
    }
    else
    {
        page_directory[table_entry].present_4mb = BIT_ON;
        page_directory[table_entry].read_write_4mb = BIT_ON;
        page_directory[table_entry].user_supervisor_4mb = user_supervisor;
        page_directory[table_entry].write_through_4mb = BIT_OFF;
        page_directory[table_entry].cache_disabled_4mb = BIT_OFF;
        page_directory[table_entry].accessed_4mb = BIT_OFF;
        page_directory[table_entry].dirty_4mb = BIT_OFF;
        page_directory[table_entry].page_size_4mb = BIT_ON;
        page_directory[table_entry].global_page_4mb  = BIT_OFF;
        page_directory[table_entry].available_4mb  = BIT_OFF;
        page_directory[table_entry].page_table_attribute_index_4mb  = BIT_OFF;
        page_directory[table_entry].reserved_4mb = BIT_OFF;
        page_directory[table_entry].page_table_base_addr_4mb = ptr_to_page;
    }
}

/*
setup_page_table
 * Description: Initializes the page table with by setting the RW bit
* * Inputs:  page_table - populate an entry in the specified page table
 * Outputs: none
 * Returns: none
 * Side Effects: sets up an entry in the Page table
 */

void
init_page_table(page_table_entry* page_table)
{
    int table_entry;
        for (table_entry = 0; table_entry < NUM_TABLE_ENTRIES; table_entry++) {
        page_table[table_entry].present = BIT_OFF;
        page_table[table_entry].read_write = BIT_OFF;
        page_table[table_entry].user_supervisor = BIT_OFF;
        page_table[table_entry].write_through = BIT_OFF;
        page_table[table_entry].cache_disabled = BIT_OFF;
        page_table[table_entry].accessed = BIT_OFF;
        page_table[table_entry].dirty = BIT_OFF;
        page_table[table_entry].page_table_attribute_index = BIT_OFF;
        page_table[table_entry].global_page = BIT_OFF;
        page_table[table_entry].available = BIT_OFF;
        page_table[table_entry].page_table_base_addr = 0;
    }
}


/*
add_page_table_entry
 * Description:Adds an entry to the page table specified
 * Inputs:  page_table - populate an entry in the specified page table
 *          table_entry - table entry in the Page table. Range from 0-1023
 *          ptr_to_page - pointer to a frame in physical mem
            user_supervisor - page allows user or supervisor access
 * Outputs: none
 * Returns: none
 * Side Effects: sets up an entry in the specified Page table
 */
void add_page_table_entry(page_table_entry* page_table,uint32_t table_entry,uint32_t ptr_to_page,uint32_t user_supervisor)
{
        page_table[table_entry].present = BIT_ON;
        page_table[table_entry].read_write = BIT_ON; 
        page_table[table_entry].user_supervisor = user_supervisor;
        page_table[table_entry].write_through  = BIT_OFF;
        page_table[table_entry].cache_disabled = BIT_OFF;
        page_table[table_entry].dirty = BIT_OFF;
        page_table[table_entry].page_table_attribute_index = BIT_OFF;
        page_table[table_entry].global_page = BIT_OFF;
        page_table[table_entry].available = BIT_OFF;
        page_table[table_entry].page_table_base_addr = ptr_to_page;
}


