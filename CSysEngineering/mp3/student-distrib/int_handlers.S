.text

	USER_DS = 0x002B
	KERNEL_DS = 0x0018

#associated vector values with each IRQ
	IRQ_START = 0X20
	IRQ0_VECTOR = IRQ_START
	IRQ1_VECTOR = (IRQ_START +1)
	IRQ2_VECTOR = (IRQ_START +2)
	IRQ3_VECTOR = (IRQ_START +3)
	IRQ4_VECTOR = (IRQ_START +4)
	IRQ5_VECTOR = (IRQ_START +5)
	IRQ6_VECTOR = (IRQ_START +6)
	IRQ7_VECTOR = (IRQ_START +7)
	IRQ8_VECTOR = (IRQ_START +8)
	IRQ9_VECTOR = (IRQ_START +9)
	IRQ10_VECTOR = (IRQ_START +10)
	IRQ11_VECTOR = (IRQ_START +11)
	IRQ12_VECTOR = (IRQ_START +12)
	IRQ13_VECTOR = (IRQ_START +13)
	IRQ14_VECTOR = (IRQ_START +14)
	IRQ15_VECTOR = (IRQ_START +15)

#associated IRQ number constants
	IRQ0 = 0
	IRQ1 = 1
	IRQ2 = 2
	IRQ3 = 3
	IRQ4 = 4
	IRQ5 = 5
	IRQ6 = 6
	IRQ7 = 7
	IRQ8 = 8
	IRQ9 = 9
	IRQ10 = 10
	IRQ11 = 11
	IRQ12 = 12
	IRQ13 = 13
	IRQ14 = 14
	IRQ15 = 15

#macro to save all registers onto the stack
   #define SAVE_ALL \
    cld;\
    pushl %es;\
    pushl %ds;\
    pushl %eax;\
    pushl %ebp;\
    pushl %edi;\
    pushl %esi;\
    pushl %edx;\
    pushl %ecx;\
    pushl  %ebx;\
    movl $KERNEL_DS,%edx;\
    movl %edx,%ds;\
	movl %edx,%es;

#macro to save all registers onto the stack
    #define RESTORE_ALL \
    popl %ebx;\
	popl %ecx;\
	popl %edx;\
    popl %esi;\
    popl %edi;\
    popl %ebp;\
    popl %eax;\
    popl %ds;\
 	popl %es;\
 	addl $4, %esp;\

#the assembly functions defined in this file to be made available in all other files
	.globl divide_error,debug,nmi,int3,overflow,bounds,invalid_op,device_not_available
	.globl doublefault_fn,coprocessor_segment_overrun,invalid_TSS,segment_not_present
	.globl stack_segment,general_protection,page_fault,floating_point,alignment_check
	.globl machine_check,simd_coprocessor_error
	.globl master_irq0,master_irq1,master_irq2,master_irq3,master_irq4,master_irq5,master_irq6,master_irq7
	.globl slave_irq0,slave_irq1,slave_irq2,slave_irq3,slave_irq4,slave_irq5,slave_irq6,slave_irq7
	.globl system_call

#the C functions that we'll be using/calling within the assembly file
	.extern _halt,_execute,_read,_write,_open,_close,_getargs,_vidmap,_set_handler,_sigreturn,_init_pcb
	.extern divide_error_c,debug_c,nmi_c,int3_c,overflow_c,bounds_c,invalid_op_c,device_not_available_c
	.extern doublefault_fn_c,coprocessor_segment_overrun_c,invalid_TSS_c,segment_not_present_c
	.extern stack_segment_c,general_protection_c,page_fault_c,floating_point_c,alignment_check_c
	.extern machine_check_c,simd_coprocessor_error_c
	.extern master_irq0_c,keyboard_handler_c,master_irq2_c,master_irq3_c,master_irq4_c,master_irq5_c,master_irq6_c,master_irq7_c
	.extern rtc_handler_c,slave_irq0_c,slave_irq1_c,slave_irq2_c,slave_irq3_c,slave_irq4_c,slave_irq5_c,slave_irq6_c,slave_irq7_c

#Intels interrupt wrappers so IRET works correctly

# void divide_error(void);
#
# Interface: Stack-based arguments (parameters on stack)
#    Inputs: none
#   Outputs: none
#  Function: calls appropriate c function for which the exception should go through
divide_error:
	pushl %eax
	pushl %ecx
	pushl %edx
	pushf 
	call divide_error_c
	jmp restore_return

# void debug(void);
#
# Interface: Stack-based arguments (parameters on stack)
#    Inputs: none
#   Outputs: none
#  Function: calls appropriate c function for which the exception should go through
debug:
	pushl %eax
	pushl %ecx
	pushl %edx
	pushf 
	call debug_c
	jmp restore_return

# void nmi(void);
#
# Interface: Stack-based arguments (parameters on stack)
#    Inputs: none
#   Outputs: none
#  Function: calls appropriate c function for which the exception should go through
nmi:
	pushl %eax
	pushl %ecx
	pushl %edx
	pushf 
	call nmi_c
	jmp restore_return

# void int3(void);
#
# Interface: Stack-based arguments (parameters on stack)
#    Inputs: none
#   Outputs: none
#  Function: calls appropriate c function for which the exception should go through
int3:
	pushl %eax
	pushl %ecx
	pushl %edx
	pushf 
	call int3_c
	jmp restore_return

# void overflow(void);
#
# Interface: Stack-based arguments (parameters on stack)
#    Inputs: none
#   Outputs: none
#  Function: calls appropriate c function for which the exception should go through
overflow:
	pushl %eax
	pushl %ecx
	pushl %edx
	pushf 
	call overflow_c
	jmp restore_return

# void bounds(void);
#
# Interface: Stack-based arguments (parameters on stack)
#    Inputs: none
#   Outputs: none
#  Function: calls appropriate c function for which the exception should go through
bounds:
	pushl %eax
	pushl %ecx
	pushl %edx
	pushf 
	call bounds_c
	jmp restore_return

# void invalid_op(void);
#
# Interface: Stack-based arguments (parameters on stack)
#    Inputs: none
#   Outputs: none
#  Function: calls appropriate c function for which the exception should go through
invalid_op:
	pushl %eax
	pushl %ecx
	pushl %edx
	pushf 
	call invalid_op_c
	jmp restore_return

# void device_not_available(void);
#
# Interface: Stack-based arguments (parameters on stack)
#    Inputs: none
#   Outputs: none
#  Function: calls appropriate c function for which the exception should go through
device_not_available:
	pushl %eax
	pushl %ecx
	pushl %edx
	pushf 
	call device_not_available_c
	jmp restore_return

# void doublefault_fn(void);
#
# Interface: Stack-based arguments (parameters on stack)
#    Inputs: none
#   Outputs: none
#  Function: calls appropriate c function for which the exception should go through
doublefault_fn:
	pushl %eax
	pushl %ecx
	pushl %edx
	pushf 
	call doublefault_fn_c
	jmp restore_return

# void coprocessor_segment_overrun(void);
#
# Interface: Stack-based arguments (parameters on stack)
#    Inputs: none
#   Outputs: none
#  Function: calls appropriate c function for which the exception should go through
coprocessor_segment_overrun:
	pushl %eax
	pushl %ecx
	pushl %edx
	pushf 
	call coprocessor_segment_overrun_c
	jmp restore_return

# void invalid_TSS(void);
#
# Interface: Stack-based arguments (parameters on stack)
#    Inputs: none
#   Outputs: none
#  Function: calls appropriate c function for which the exception should go through
invalid_TSS:
	pushl %eax
	pushl %ecx
	pushl %edx
	pushf 
	call invalid_TSS_c
	jmp restore_return

# void segment_not_present(void);
#
# Interface: Stack-based arguments (parameters on stack)
#    Inputs: none
#   Outputs: none
#  Function: calls appropriate c function for which the exception should go through
segment_not_present:
	pushl %eax
	pushl %ecx
	pushl %edx
	pushf 
	call segment_not_present_c
	jmp restore_return

# void stack_segment(void);
#
# Interface: Stack-based arguments (parameters on stack)
#    Inputs: none
#   Outputs: none
#  Function: calls appropriate c function for which the exception should go through
stack_segment:
	pushl %eax
	pushl %ecx
	pushl %edx
	pushf 
	call stack_segment_c
	jmp restore_return

# void general_protection(void);
#
# Interface: Stack-based arguments (parameters on stack)
#    Inputs: none
#   Outputs: none
#  Function: calls appropriate c function for which the exception should go through
general_protection:
	pushl %eax
	pushl %ecx
	pushl %edx
	pushf 
	call general_protection_c
	jmp restore_return

# void page_fault(void);
#
# Interface: Stack-based arguments (parameters on stack)
#    Inputs: none
#   Outputs: none
#  Function: calls appropriate c function for which the exception should go through
page_fault:
	pushl %eax
	pushl %ecx
	pushl %edx
	pushf 
	call page_fault_c
	jmp restore_return

# void floating_point(void);
#
# Interface: Stack-based arguments (parameters on stack)
#    Inputs: none
#   Outputs: none
#  Function: calls appropriate c function for which the exception should go through
floating_point:
	pushl %eax
	pushl %ecx
	pushl %edx
	pushf 
	call floating_point_c
	jmp restore_return

# void alignment_check(void);
#
# Interface: Stack-based arguments (parameters on stack)
#    Inputs: none
#   Outputs: none
#  Function: calls appropriate c function for which the exception should go through
alignment_check:
	pushl %eax
	pushl %ecx
	pushl %edx
	pushf 
	call alignment_check_c
	jmp restore_return

# void machine_check(void);
#
# Interface: Stack-based arguments (parameters on stack)
#    Inputs: none
#   Outputs: none
#  Function: calls appropriate c function for which the exception should go through
machine_check:
	pushl %eax
	pushl %ecx
	pushl %edx
	pushf 
	call machine_check_c
	jmp restore_return

# void simd_coprocess_error(void);
#
# Interface: Stack-based arguments (parameters on stack)
#    Inputs: none
#   Outputs: none
#  Function: calls appropriate c function for which the exception should go through
simd_coprocessor_error:
	pushl %eax
	pushl %ecx
	pushl %edx
	pushf 
	call simd_coprocessor_error_c
	jmp restore_return

	#Pic master irq line interrupts wrapper so IRET works correctly

# void master_irq0(void);
#
# Interface: Stack-based arguments (parameters on stack)
#    Inputs: none
#   Outputs: none
#  Function: pushes necessary information onto stack to call interrupt jumptable 
#   		 which links to appropriate interrupt handler
master_irq0:
	pushl $IRQ0_VECTOR-256
	SAVE_ALL
	movl $IRQ0,%edx
	jmp common_interrupt

# void master_irq1(void);
#
# Interface: Stack-based arguments (parameters on stack)
#    Inputs: none
#   Outputs: none
#  Function: pushes necessary information onto stack to call interrupt jumptable 
#   		 which links to appropriate interrupt handler
master_irq1: 
	pushl $IRQ1_VECTOR-256
	SAVE_ALL
	movl $IRQ1,%edx
	jmp common_interrupt

# void master_irq2(void);
#
# Interface: Stack-based arguments (parameters on stack)
#    Inputs: none
#   Outputs: none
#  Function: pushes necessary information onto stack to call interrupt jumptable 
#   		 which links to appropriate interrupt handler
master_irq2:
	pushl $IRQ2_VECTOR-256
	SAVE_ALL
	movl $IRQ2,%edx
	jmp common_interrupt

# void master_irq3(void);
#
# Interface: Stack-based arguments (parameters on stack)
#    Inputs: none
#   Outputs: none
#  Function: pushes necessary information onto stack to call interrupt jumptable 
#   		 which links to appropriate interrupt handler
master_irq3:
	pushl $IRQ3_VECTOR-256
	SAVE_ALL
	movl $IRQ3,%edx
	jmp common_interrupt

# void master_irq4(void);
#
# Interface: Stack-based arguments (parameters on stack)
#    Inputs: none
#   Outputs: none
#  Function: pushes necessary information onto stack to call interrupt jumptable 
#   		 which links to appropriate interrupt handler
master_irq4:
	pushl $IRQ4_VECTOR-256
	SAVE_ALL
	movl $IRQ4,%edx
	jmp common_interrupt

# void master_irq5(void);
#
# Interface: Stack-based arguments (parameters on stack)
#    Inputs: none
#   Outputs: none
#  Function: pushes necessary information onto stack to call interrupt jumptable 
#   		 which links to appropriate interrupt handler
master_irq5:
	pushl $IRQ5_VECTOR-256
	SAVE_ALL
	movl $IRQ5,%edx
	jmp common_interrupt

# void master_irq6(void);
#
# Interface: Stack-based arguments (parameters on stack)
#    Inputs: none
#   Outputs: none
#  Function: pushes necessary information onto stack to call interrupt jumptable 
#   		 which links to appropriate interrupt handler
master_irq6:
	pushl $IRQ6_VECTOR-256
	SAVE_ALL
	movl $IRQ6,%edx
	jmp common_interrupt

# void master_irq7(void);
#
# Interface: Stack-based arguments (parameters on stack)
#    Inputs: none
#   Outputs: none
#  Function: pushes necessary information onto stack to call interrupt jumptable 
#   		 which links to appropriate interrupt handler
master_irq7:
	pushl $IRQ7_VECTOR-256
	SAVE_ALL
	movl $IRQ7,%edx
	jmp common_interrupt

	#Pic slave irq line interrupts

# void slave_irq0(void);
#
# Interface: Stack-based arguments (parameters on stack)
#    Inputs: none
#   Outputs: none
#  Function: pushes necessary information onto stack to call interrupt jumptable 
#   		 which links to appropriate interrupt handler
slave_irq0:		
	pushl $IRQ8_VECTOR-256
	SAVE_ALL
	movl $IRQ8,%edx
	jmp common_interrupt

# void slave_irq1(void);
#
# Interface: Stack-based arguments (parameters on stack)
#    Inputs: none
#   Outputs: none
#  Function: pushes necessary information onto stack to call interrupt jumptable 
#   		 which links to appropriate interrupt handler
slave_irq1:
	pushl $IRQ9_VECTOR-256
	SAVE_ALL
	movl $IRQ9,%edx
	jmp common_interrupt

# void slave_irq2(void);
#
# Interface: Stack-based arguments (parameters on stack)
#    Inputs: none
#   Outputs: none
#  Function: pushes necessary information onto stack to call interrupt jumptable 
#   		 which links to appropriate interrupt handler
slave_irq2:
	pushl $IRQ10_VECTOR-256
	SAVE_ALL
	movl $IRQ10,%edx
	jmp common_interrupt

# void slave_irq3(void);
#
# Interface: Stack-based arguments (parameters on stack)
#    Inputs: none
#   Outputs: none
#  Function: pushes necessary information onto stack to call interrupt jumptable 
#   		 which links to appropriate interrupt handler
slave_irq3:
	pushl $IRQ11_VECTOR-256
	SAVE_ALL
	movl $IRQ11,%edx
	jmp common_interrupt

# void slave_irq4(void);
#
# Interface: Stack-based arguments (parameters on stack)
#    Inputs: none
#   Outputs: none
#  Function: pushes necessary information onto stack to call interrupt jumptable 
#   		 which links to appropriate interrupt handler
slave_irq4:
	pushl $IRQ12_VECTOR-256
	SAVE_ALL
	movl $IRQ12,%edx
	jmp common_interrupt

# void slave_irq5(void);
#
# Interface: Stack-based arguments (parameters on stack)
#    Inputs: none
#   Outputs: none
#  Function: pushes necessary information onto stack to call interrupt jumptable 
#   		 which links to appropriate interrupt handler
slave_irq5:
	pushl $IRQ13_VECTOR-256
	SAVE_ALL
	movl $IRQ13,%edx
	jmp common_interrupt

# void slave_irq6(void);
#
# Interface: Stack-based arguments (parameters on stack)
#    Inputs: none
#   Outputs: none
#  Function: pushes necessary information onto stack to call interrupt jumptable 
#   		 which links to appropriate interrupt handler
slave_irq6:
	pushl $IRQ14_VECTOR-256
	SAVE_ALL
	movl $IRQ14,%edx
	jmp common_interrupt

# void slave_irq7(void);
#
# Interface: Stack-based arguments (parameters on stack)
#    Inputs: none
#   Outputs: none
#  Function: pushes necessary information onto stack to call interrupt jumptable 
#   		 which links to appropriate interrupt handler
slave_irq7:
	pushl $IRQ15_VECTOR-256
	SAVE_ALL
	movl $IRQ15,%edx
	jmp common_interrupt

# void restore_return(void);
#
# Interface: Stack-based arguments (parameters on stack)
#    Inputs: none
#   Outputs: none
#  Function: pops off pushed registers and returns back to whichever function in which
#			 exception was generated from
restore_return:
	popf
	popl %edx
	popl %ecx
	popl %eax
	iret

# void common_interrupt(irq_number);
#
# Interface: register based arguments
#    Inputs: irq_number - the number of the IRQ which generated the interrupt
#   Outputs: none
#  Function: moves esp and ebp into registers eax and ebx before going to jump table of interrupts
#			 and making appropriate
common_interrupt:
	movl %esp,%eax
	movl %ebp,%ebx
	call *irq_jumptable(,%edx,4)
	RESTORE_ALL
	iret

# int32_t system_call(int32_t fd, const void* buf, int32_t nbytes);
#
# Interface: register based arguments
#    Inputs: fd - file descriptor number (may not exist/be needed for specific system call)
#			 buf - pointer to generally a uint8_t or int8_t buffer (may not exist/be needed for specific system call)
#			 nbytes - number of bytes to read/write generally (may not exist/be needed for specifc system call)
#   Outputs: successful/failing response from the sytem call into eax
#  Function: moves esp and ebp into registers eax and ebx before going to jump table of interrupts
#			 and making appropriate
system_call:
	pushw %es
	pushw %ds
	pushl %eax 	#movl %eax,24(%esp) changes this value
	pushl %ebp	#ULK Page 163, PUSH ALL
	pushl %edi
	pushl %esi
	pushl %edx
	pushl %ecx
	pushl %ebx
	movl $KERNEL_DS,%esi
	movl %esi,%ds
	movl %esi,%es

	#value checking
	cmpl $0,%eax			
	je error
	cmpl $11,%eax
	jae error

	#push arguments
	pushl %edx
	pushl %ecx
	pushl %ebx

	#make the values indexes start at 0
	subl $1,%eax
	call *system_call_jumptable(,%eax,4)

#restores the stack and sets return value
	addl $12, %esp  	#removes arguments from the stack
	movl %eax,24(%esp)	#populates the EAX register that lives on the stack
	popl %ebx
	popl %ecx
	popl %edx
	popl %esi	#POP ALL
	popl %edi
	popl %ebp
	popl %eax
	popw %ds
	popw %es
	iret 

# int32_t error(void);
#
# Interface: stack based arguments
#    Inputs: none
#   Outputs: puts failure signal/code into eax (just -1 generally)
#  Function: to be called when a system call fails for some reason
error:
	movl $-1,24(%esp)  #puts -1 into eax saved on stack
	popl %ebx
	popl %ecx
	popl %edx
	popl %esi	#POP ALL
	popl %edi
	popl %ebp
	popl %eax
	popw %ds
	popw %es
	iret

# system call jumpt table
system_call_jumptable:
.long _halt,_execute,_read,_write,_open,_close,_getargs,_vidmap,_set_handler,_sigreturn

# interrrupt jump table
irq_jumptable:
.long sched,keyboard_handler_c,master_irq2_c,master_irq3_c,master_irq4_c,master_irq5_c,master_irq6_c,master_irq7_c, rtc_handler_c,slave_irq1_c,slave_irq2_c,slave_irq3_c,slave_irq4_c,slave_irq5_c,slave_irq6_c,slave_irq7_c
