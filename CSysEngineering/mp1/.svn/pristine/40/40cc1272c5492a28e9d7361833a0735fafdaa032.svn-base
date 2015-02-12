
.data					# section declaration

        # Useful offset constants for accessing members of a 
        # struct mp1_blink_struct structure
        LOCATION   = 0
        ON_CHAR    = 2
        OFF_CHAR   = 3 
        ON_LENGTH  = 4
        OFF_LENGTH = 6
        COUNTDOWN  = 8
        STATUS     = 10
        NEXT       = 12

        STRUCT_SIZE = 16
	MAX_LOCATION = 1999
# Pointer to head of list (initialized to NULL)
mp1_list_head:
        .long   0

.text					# section declaration

# Export the function symbol names

.global mp1_rtc_tasklet
.global mp1_ioctl

# void mp1_poke(void);
#
# Interface: Register-based arguments (not C-style)
#    Inputs: %cl  - The byte you wish to write
#            %eax - Offset from the start of video memory that you wish
#                   to write to
#   Outputs: Text-mode video screen is written to at location %eax with
#            the byte in %cl
# Registers: Clobbers EDX
mp1_poke:
        
	movl    vmem_base_addr(,1),%edx
	movb    %cl,(%edx,%eax,1)
	ret
#---
#returns : nothing
#args : none
#clobbers : %eax, %ecx
mp1_rtc_tasklet:
	PUSHL %EBP
	MOVL %ESP, %EBP
	PUSHL %EBX			#save callee-saved reg
	PUSHL %ESI
	PUSHL %EDI
	
	MOVL mp1_list_head, %ESI
	CMPL $0, %ESI			#null check for head ptr
	je done_tasklet

change_fields:
	DECW COUNTDOWN(%ESI)
	CMPW $0, COUNTDOWN(%ESI)	#dec countdown to see if we switch
	je examine_status

go_to_next:
	MOVL NEXT(%ESI), %ESI
	CMPL $0, %ESI			#ptr = ptr->next
	je done_tasklet
	jmp change_fields

examine_status:
	CMPW $1, STATUS(%ESI)
	jne turn_on
	CMPW $1, STATUS(%ESI)
	jne go_to_next

turn_off:
	MOVB OFF_CHAR(%ESI), %CL
	XORL %EAX, %EAX
	MOVW LOCATION(%ESI), %AX
	ADDW %AX, %AX
	call mp1_poke
	MOVW $0, STATUS(%ESI)
	MOVW OFF_LENGTH(%ESI), %AX
	MOVW %AX, COUNTDOWN(%ESI)
	jmp go_to_next

turn_on:
	MOVB ON_CHAR(%ESI), %CL
	XORL %EAX, %EAX
	MOVW LOCATION(%ESI), %AX
	ADDW %AX, %AX
	call mp1_poke
	MOVW $1, STATUS(%ESI)
	MOVW ON_LENGTH(%ESI), %AX
	MOVW %AX, COUNTDOWN(%ESI)
	jmp go_to_next

done_tasklet:
	POPL %EDI
	POPL %ESI			#restore callee-saved reg
	POPL %EBX
	leave
	ret
#---
# dispatcher table, arguments, return values, and clobbers depend on com num
mp1_ioctl:
	CMPL $3, 8(%ESP)			#if command number >3 ret -1
	ja ret_neg
	CMPL $0, 4(%ESP)
	je ret_neg
	MOVL 8(%ESP), %ECX
	jmp *dispatcher_table(,%ECX,4)		#jump table for com num <= 3

ret_neg:
	MOVL $-1, %EAX
	ret

dispatcher_table:
.long mp1_ioctl_add, mp1_ioctl_remove, mp1_ioctl_find, mp1_ioctl_sync
#---
#returns : 0 on success, -1 on failure
#args : mp1_blink_struct
#clobbers : %eax, %ecx
mp1_ioctl_add:
	PUSHL %EBP
	MOVL %ESP, %EBP
	PUSHL %EBX			#save callee saved registers
	PUSHL %ESI
	PUSHL %EDI


	
	MOVL 8(%EBP), %ESI
	PUSHL $STRUCT_SIZE
	call mp1_malloc			#allocating memory in kernal space
	ADDL $4, %ESP
	CMPL $0, %EAX
	je ret_neg_one

	MOVL %EAX, %EDI
	PUSHL $STRUCT_SIZE
	PUSHL %ESI
	PUSHL %EAX			#copy struct to kernal space
	call mp1_copy_from_user
	ADDL $12, %ESP
	CMPL $0, %EAX
	jne free_memory

	CMPW $MAX_LOCATION, LOCATION(%EDI)	#check valid location
	ja free_memory

	XORL %EAX, %EAX
	MOVW ON_LENGTH(%EDI), %AX
	MOVW %AX, COUNTDOWN(%EDI)	#countdown = on_length
	MOVW $1, STATUS(%EDI)		#status = 1

	MOVL mp1_list_head, %EAX
	MOVL %EAX, NEXT(%EDI)		#set list head to the new struct
	MOVL %EDI, mp1_list_head

	XORL %EAX, %EAX
	MOVW LOCATION(%EDI), %AX
	ADDL %EAX, %EAX			#poke it
	MOVB ON_CHAR(%EDI), %CL
	call mp1_poke

	MOVL $0, %EAX
	jmp done_add

free_memory:
	PUSHL %EDI
	call mp1_free			#free the memory if bad allocation
	ADDL $4, %ESP

ret_neg_one:
	MOVL $-1, %EAX

done_add:
	POPL %EDI
	POPL %ESI			#restore callee-saved registers
	POPL %EBX
	leave
	ret
#---
mp1_ioctl_remove:
	PUSHL %EBP
	MOVL %ESP, %EBP		#save callee-saved registers
	PUSHL %EBX
	PUSHL %ESI
	PUSHL %EDI

	CMPL $0, 8(%EBP)
	je ret_neg_one_remove
	PUSHW 8(%EBP)
	call traverse		#traverse the list looking for location
	ADDL $2, %ESP

	CMPL $-1, %EAX
	je ret_neg_one_remove		#if not found return -1

	CMPL $-1, %EDX
	je move_head_pointer		#if we are at head then move head
	MOVL NEXT(%EAX), %ECX		#otherwise move previous
	MOVL %ECX, NEXT(%EDX)
	jmp free_memory_remove

move_head_pointer:
	MOVL NEXT(%EAX), %ECX
	MOVL %ECX, mp1_list_head

free_memory_remove:
	PUSHL %EAX
	call mp1_free			#free memory
	ADDL $4, %ESP

	MOVL $0, %EAX

done_remove:
	POPL %EDI
	POPL %ESI			#restore callee-saved reg
	POPL %EBX
	leave
	ret

ret_neg_one_remove:
	MOVL $-1, %EAX
	jmp done_remove

#---
#returns : input on success, -1 on failure
#args : mp1_blink_struct
#clobbers : %eax, %ecx, %edx
mp1_ioctl_find:
	PUSHL %EBP
	MOVL %ESP, %EBP			#save callee-saved reg
	PUSHL %EBX
	PUSHL %ESI
	PUSHL %EDI
	
	XORL %EAX, %EAX
	XORL %ECX, %ECX			#clear eax, ecx, and edx
	XORL %EDX, %EDX

	MOVL 8(%EBP), %EDI
	CMPL $0, %EDI
	je ret_neg_one_find		#traverse the linked list
	PUSHW LOCATION(%EDI)
	call traverse
	ADDL $2, %ESP
	CMPL $-1, %EAX
	je ret_neg_one_find

	MOVL %EAX, %ESI
	PUSHL $STRUCT_SIZE
	PUSHL %ESI
	PUSHL %EDI
	call mp1_copy_to_user		#copy the struct found to user-space
	ADDL $12, %ESP
	CMPL $0, %EAX
	jne ret_neg_one_find
	MOVL %EDI, %EAX
	jmp done_find

ret_neg_one_find:
	MOVL $-1, %EAX			#failed to find

done_find:
	POPL %EDI
	POPL %ESI			#restore callee-saved reg
	POPL %EBX
	leave
	ret

#---
#returns : 0 for success, -1 for failure
#args : unsigned long arg (really 2 shorts)
#clobbers : %ecx, %eax, %edx
mp1_ioctl_sync:
	PUSHL %EBP
	MOVL %ESP, %EBP			#save callee-saved reg
	PUSHL %EBX
	PUSHL %ESI
	PUSHL %EDI

	PUSHW 10(%EBP)
	call traverse
	ADDL $2, %ESP
	CMPL $-1, %EAX			#traversing linked-list for 1st val
	je ret_neg_one_sync
	MOVL %EAX, %ESI

	PUSHW 8(%EBP)
	call traverse
	ADDL $2, %ESP
	CMPL $-1, %EAX		#traversing the linked-list for 2nd val
	je ret_neg_one_sync
	MOVL %EAX, %EDI

	MOVW ON_LENGTH(%ESI), %AX
	MOVW %AX, ON_LENGTH(%EDI)
	MOVW OFF_LENGTH(%ESI), %AX	#copying timing values
	MOVW %AX, OFF_LENGTH(%EDI)
	MOVW COUNTDOWN(%ESI), %AX
	MOVW %AX, COUNTDOWN(%EDI)
	MOVW STATUS(%ESI), %AX		#critical so we don't lose countdown
	MOVW %AX, STATUS(%EDI)

	CMPW $1, %AX
	je turn_on_sync			#check status for syncing
	CMPW $0, %AX
	je turn_off_sync

turn_on_sync:
	XORL %EAX, %EAX		#if the status is garbage this is used
	XORL %ECX, %ECX
	MOVW LOCATION(%EDI), %AX
	ADDW %AX, %AX			#poke using the on char
	MOVB ON_CHAR(%EDI), %CL
	call mp1_poke
	MOVL $0, %EAX
	jmp done_sync

turn_off_sync:
	XORL %EAX, %EAX
	XORL %ECX, %ECX
	MOVW LOCATION(%EDI), %AX
	ADDW %AX, %AX			#poke using the off char
	MOVB OFF_CHAR(%EDI), %CL
	call mp1_poke
	MOVL $0, %EAX

done_sync:
	POPL %EDI
	POPL %ESI			#restore callee-saved reg
	POPL %EBX
	leave
	ret

ret_neg_one_sync:
	MOVL $-1, %EAX
	jmp done_sync			#return -1 on failure

#---

#returns : void * prev in edx, void * found in eax
#args : 16 bit argument location
#clobbers : eax, ecx, edx
traverse:	#traverse function to be used in remove, find, and sync
	PUSHL %EBP
	MOVL %ESP, %EBP
	PUSHL %EBX			#save callee-saved registers
	PUSHL %ESI
	PUSHL %EDI

	XORL %EAX, %EAX
	XORL %ECX, %ECX			#clear reg
	XORL %EDX, %EDX
	MOVL $-1, %EDX
	MOVW 8(%EBP), %CX		#argument is a word location
	MOVL mp1_list_head, %EAX
	CMPL $0, %EAX			#null check
	je ret_neg_traverse

compare:
	CMPW %CX, LOCATION(%EAX)	#if location is found return
	je done_traverse
	MOVL %EAX, %EDX			#edx is a pointer to previous
	MOVL NEXT(%EAX), %EAX		#eax is a pointer to what is found
	CMPL $0, %EAX
	je ret_neg_traverse
	jmp compare

ret_neg_traverse:
	XORL %EDX, %EDX
	XORL %EAX, %EAX			#return -1 on failure
	MOVL $-1, %EAX
	MOVL $-1, %EDX

done_traverse:
	POPL %EDI
	POPL %ESI			#restore callee-saved reg
	POPL %EBX
	leave
	ret

.end
