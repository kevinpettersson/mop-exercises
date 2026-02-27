#include <stdio.h>
#include <stdint.h>

void assignment1(); 
void whackamole(); 
void SysTick_Handler(); 

#define FAILED(n)                       \
    do {                                \
        printf("\nFAILED: %s", n);      \
        assignment_passed = 0;          \
    } while(0);                

void check_assignment1()
{
    printf("Checking assignment 1.1...");
    int assignment_passed = 1; 
    if(*((volatile uint32_t *)0xE000F000) != 0b10100) FAILED("STK_CTLR is not properly written.");
    if(*((volatile uint32_t *)0xE000F004) != 0) FAILED("STK_SR is not properly written.");
    if(*((volatile uint32_t *)0xE000F008) != 1) FAILED("STK_CNTL is not properly written.");
    if(*((volatile uint32_t *)0xE000F00C) != 2) FAILED("STK_CNTH is not properly written.");
    if(*((volatile uint32_t *)0xE000F010) != 3) FAILED("STK_CMPL is not properly written.");
    if(*((volatile uint32_t *)0xE000F014) != 4) FAILED("STK_CMPH is not properly written.");
    if(assignment_passed) printf("PASSED!\n");
}

void check_assignment2_1()
{
    printf("Checking assignment 2.1...");
    int assignment_passed = 1; 
    /* Note: IENR is write-to-set, and cannot be read. Reading ISR instead. */
    if(*((volatile uint32_t *)0xE000E000) != (1 << 12)) FAILED("Systick interrupt is not enabled in PFIC.");
    uintptr_t value;
    __asm__ volatile ("csrr %0, mtvec" : "=r"(value));
    if(value != (uintptr_t)SysTick_Handler) FAILED("Systick_Handler is not properly set in mtvec.");
    if(assignment_passed) printf("PASSED!\n");
}

void check_assignment2_2()
{
    printf("Checking assignment 2.3...");
    int assignment_passed = 1; 
    if((*((volatile uint32_t *)0xE000F000) & 0b100000) == 0) FAILED("INIT is not set, so the timer might not start on 0.");
    // Does not matter if they count up or down. 
    //if((*((volatile uint32_t *)0xE000F000) & 0b010000) == 0) FAILED("MODE is not set, so the timer will count DOWN, not UP.");
    if((*((volatile uint32_t *)0xE000F000) & 0b001000) == 0) FAILED("STRE is not set, so the timer will not reload.");
    if((*((volatile uint32_t *)0xE000F000) & 0b000100) == 0) FAILED("STCLK is not set, so the timer will use 148/8Mhz.");
    if((*((volatile uint32_t *)0xE000F000) & 0b000010) == 0) FAILED("STIE is not set, so interrupts will not be generated.");
    if((*((volatile uint32_t *)0xE000F000) & 0b000001) != 0) FAILED("STE is set, so the timer is already running!.");
    if(*((volatile uint32_t *)0xE000F004) != 0) FAILED("STK_SR is not 0, so you won't get an interrupt.");
    // STK_CMPL is not readable on the simulator right now, so cannot check. 
    // if(*((volatile uint32_t *)0xE000F010) != 144000*10) FAILED("STK_CMPL is not set to 10ms.");
    if(*((volatile uint32_t *)0xE000F014) != 0) FAILED("STK_CMPH should be initialized to 0.");
    if(assignment_passed) printf("PASSED!\n");
}

int main(void)
{
    assignment1(); 
    check_assignment1(); 
    whackamole(); 
}
