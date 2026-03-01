.section .text
.global init_interrupts
.extern exti5_9_handler
.global vector_table

###############################################################################
# Assignment 5 (continued from main.c):
#           You need to put the address of the vector_table (below) into the 
#           mtvec CSR register. Then you need to set bit 0 in that register
#           to 1, to tell the CPU that you want to use vectored interrupts.
###############################################################################
init_interrupts: 
    la t0, vector_table
    li t1, 1
    or t0, t0, t1        # set bit 0
    csrw mtvec, t0
    ret


###############################################################################
# Assignment 6:
#           Now,c if an EXTI7 interrupt occurs, the pu will jump to address
#           vector_table + (irq_number_for_the_interrupt) * 4. You have to put 
#           a jump instruction at that address so that it will jump to the 
#           interrupt handler for EXTI7.
#
#           (see the lecture notes or slides if this is unclear)
###############################################################################

.align 2
vector_table: 
.org vector_table + 39 * 4 
j exti5_9_handler    # entry 39 → EXTI9_5
