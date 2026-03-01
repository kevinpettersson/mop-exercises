#include <stdio.h>
#include <stdint.h>
#include "peripherals.h"
void blink(void);

      
extern volatile GPIO_t* GPIO_D;
extern volatile GPIO_t* GPIO_E;
extern volatile SysTick_t * systick;

#define DEFINE_1 ((volatile uint32_t*) 0x40010008)
#define DEFINE_2 ((volatile uint32_t*) 0x4001000C)
#define DEFINE_3 ((volatile uint32_t*) 0x40010010)
#define DEFINE_4 ((volatile uint32_t*) 0x40010014)

#define DEFINE_5 ((volatile uint32_t*) 0x40010400)
#define DEFINE_6 ((volatile uint32_t*) 0x40010404)
#define DEFINE_7 ((volatile uint32_t*) 0x40010408)
#define DEFINE_8 ((volatile uint32_t*) 0x4001040C)
#define DEFINE_9 ((volatile uint32_t*) 0x40010410)
#define DEFINE_10 ((volatile uint32_t*) 0x40010414)

#define DEFINE_11 ((volatile uint32_t*) 0xE000E000)
#define DEFINE_12 ((volatile uint32_t*) 0xE000E004)
#define DEFINE_13 ((volatile uint32_t*) 0xE000E100)
#define DEFINE_14 ((volatile uint32_t*) 0xE000E104)

extern void * vector_table; 

#define FAILED(n)                       \
    do {                                \
        printf("\nFAILED: %s", n);      \
        assignment_passed = 0;          \
    } while(0);  

void check_assignment_1()
{
    printf("Checking assignment 1...");
    int assignment_passed = 1; 
    if(((GPIO_E->CFGLR >> 7*4) & 0xF) != 0x8) FAILED("PE7 not configured as input with pull-up/down");
    if((GPIO_E->OUTDR & 0b10000000) != 0b10000000) FAILED("PE7 not pulled up");
    if(assignment_passed) printf("PASSED!\n");
}

void check_assignment_2()
{
    printf("Checking assignment 2...");
    int assignment_passed = 1; 
    if((*DEFINE_1 != 0) || (*DEFINE_3 != 0) || (*DEFINE_4 != 0) || (*DEFINE_2 & 0x0FFF) != 0) 
        FAILED("You have changed the AFIO_EXTICR configuration for pins other than PE7");
    if((*DEFINE_2 & 0xF000) != 0x4000) FAILED("PE7 not routed to EXTI7");
    if(assignment_passed) printf("PASSED!\n");
}

void check_assignment_3()
{
    printf("Checking assignment 3...");
    int assignment_passed = 1; 
    if((*DEFINE_7 & 0b10000000) != 0b10000000) FAILED("Rising edge trigger not enabled for EXTI7");
    if((*DEFINE_8 & 0b10000000) != 0) FAILED("Falling edge trigger should be disabled for EXTI7");
    if((*DEFINE_5 & 0b10000000) != 0b10000000) FAILED("Interrupt not enabled for EXTI7");
    if(assignment_passed) printf("PASSED!\n");
}

void check_assignment_4()
{
    printf("Checking assignment 4...");
    int assignment_passed = 1; 
    if((*DEFINE_12 & (1<<7)) != (1<<7)) FAILED("Interrupt for EXTI7 not enabled in PFIC");
    if((*DEFINE_12 & ~(1<<7)) != 0) FAILED("You have enabled interrupts other than EXTI7 in PFIC");
    if(assignment_passed) printf("PASSED!\n");
}

void check_assignment_5()
{
    printf("Checking assignment 5...");
    int assignment_passed = 1; 
    uint32_t mtvec;
    __asm("csrr %0, mtvec" : "=r" (mtvec));
    uint32_t addr = (uint32_t)(uintptr_t)&vector_table;
    if((uint32_t)mtvec !=  ((uint32_t)addr | 1)) FAILED("mtvec is not set up to address of vector table | 0x1");
    if(assignment_passed) printf("PASSED!\n");
}

int main(void)
{
    *DEFINE_1 = 0; 
    *DEFINE_2 = 0; 
    *DEFINE_3 = 0; 
    *DEFINE_4 = 0; 
    blink(); 
}
