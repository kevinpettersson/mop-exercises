.section .text

.global InitInterrupts
.extern SysTick_Handler

InitInterrupts: 
###############################################################################
# Assignment 2.1: This function is called from the whackamole game loop. 
#                 Here, you need to: 
#                 1. Initialize the CSR register "mtvec" to point at the 
#                    SysTick_Handler, in the C code. 
#                 2. Enable interrupts from SysTick, by setting the 
#                    corresponding bit in the PFIC_IENR1 register (see Vector 
#                    Table and PFIC in the quickguide).
# Hint: Read lecture 08 to remember that CSR registers are accessed using csrw and
#       csrr instructions (which are in the QuickGuide).
###############################################################################
    la t0, SysTick_Handler  # load adress of SysTick_Handler into t0
    csrw mtvec, t0          # save adress in mtvec register

    li t1, 0xE000E100          # base adress of PFIC + offset 0x100 to IERN1
    li t2, (1 << 12)           # shift 1 to bit 12
    sw t2, 0(t1)               # write to enable interupt offset 112
    ret             
