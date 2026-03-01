///////////////////////////////////////////////////////////////////////////////
// Lecture 09 - Exercise 01
// A complicated light-switch. 
// ============================================================================
// When you start this program, you shall see the lights on the bargraph
// blinking. Your job is to turn them off. You will do this using interrupts,
// without modifying the original program. 
//
// When the highest switch (bit 7) on the DIPSWITCH goes from 0 to 1, the
// lights should stop blinking, when it foes from 1 to 0, nothing should 
// happen, but when it goes from 0 to 1 again the lights should start blinking
// again. This will be implemented using interrupts. 
// 
// Follow the lecture notes or slides as you do these assignments so you
// remember what you are doing. 
//
// Connect: 
// Console to USART 1 as usual
// Bargraph to GPIOD[0:7]
// Dipswitch to GPIOE[0:7]
///////////////////////////////////////////////////////////////////////////////
#include <stdint.h>
#include <stdio.h>
#include "peripherals.h"

volatile GPIO_t* GPIO_D = (GPIO_t*) 0x40011400;
volatile GPIO_t* GPIO_E = (GPIO_t*) 0x40011800;
volatile SysTick_t * systick = (SysTick_t*) 0xE000F000;
volatile int light_on = 1;

void init_interrupts(void);

////////////////////////////////////////////////////////////////////////////////
// Insert your macrodefinitions or structs for AFIO, EXTI, and PFIC up here
// when you need them. 
////////////////////////////////////////////////////////////////////////////////
void check_assignment_1();
void check_assignment_2();
void check_assignment_3();
void check_assignment_4();
void check_assignment_5();

////////////////////////////////////////////////////////////////////////////////
// Assignment 7: Write the interrupt handler. 
//               All this handler has to do is to toggle the state of the 
//               light_on variable, and clear the interrupt flag for EXTI7. 
////////////////////////////////////////////////////////////////////////////////

__attribute__((interrupt("machine"))) 
void exti5_9_handler()
{
    // <your code here>
}

void blink()
{    
    ///////////////////////////////////////////////////////////////////////////
    // Configure Bargraph (do not modify this part)
    ///////////////////////////////////////////////////////////////////////////
    GPIO_D->CFGLR = 0x22222222; // Set PD0-7 as output, push-pull, 2 MHz
    GPIO_D->OUTDR = 0; // Turn off all lights

    ////////////////////////////////////////////////////////////////////////////
    // Assignment 1: Configure the GPIO ports
    //               The DIL swithces are simple switches where one end is 
    //               connected to ground and the other is connected to the 
    //               GPIO pin. Thus, they have to be configured as input pins
    //               with PULL-UP. 
    ////////////////////////////////////////////////////////////////////////////
    GPIO_E->CFGLR = 0x88888888; // Set PE0-7 as input, pull-up
    GPIO_E->OUTDR = 0xFF;       // Set pull up 0-7
    // To test this step, you can temporarily uncomment the next line. This is 
    // an endless loop that will read the state of the switches and output it to the
    // bargraph, so you can verify that you have configured the GPIO ports correctly.
    //while(1) GPIO_D->OUTDR = GPIO_E->INDR;

    check_assignment_1();

    typedef struct {
        uint32_t ECR;
        uint32_t PCFR;
        uint32_t EXTICR1;
        uint32_t EXTICR2;
        uint32_t EXTICR3;
        uint32_t EXTICR4;
        uint32_t PCFR2;
    } AFIO;

    ///////////////////////////////////////////////////////////////////////////
    // Assignment 2: Routing the right pin to EXTI.
    //               Since we want to trigger an interrupt when the state of PE7 
    //               changes, we need to configure the AFIO so that Port E is 
    //               routed for EXTI7. (see lecture notes or slides)
    // 
    //               (you will need to create a macro or struct for AFIO for 
    //                this one)
    ///////////////////////////////////////////////////////////////////////////
    AFIO *afio = (AFIO *)0x40010000;
    afio->EXTICR2 = (0b0100 << 12);

    check_assignment_2();

    ///////////////////////////////////////////////////////////////////////////
    // Assignment 3: Configuring EXTI
    //               Now that Pin 7 of Port E is routed to EXTI7, we need to 
    //               configure EXTI so that it triggers an interrupt when the 
    //               state of the pin goes from 0 to 1 (rising edge).
    //
    //               Check the lecture notes os slides. 
    //               
    //               (you will need to create a macro or struct for the EXTI 
    //                registers for this one)
    ///////////////////////////////////////////////////////////////////////////
    typedef struct {
        uint32_t INTENR;
        uint32_t EVENR;
        uint32_t RTENR;
        uint32_t FTENR;
        uint32_t SWIEVR;
        uint32_t INTFR;
    } EXTI;

    EXTI *exti = (EXTI *)0x40010400;
    exti->RTENR |= (1 << 7);
    exti->FTENR |= (0 << 7);
    exti->INTENR = (1 << 7);
    // Configure EXTI_RTENR, EXTI_FTENR, and EXTI_INTENR for pin 7
    check_assignment_3();

    ///////////////////////////////////////////////////////////////////////////
    // Assignment 4: Configuring PFIC
    //               If everything is right so far, an interrupt request will 
    //               be triggered from the EXTI module to the PFIC when the 
    //               state of PE7 goes from 0 to 1. 
    //               We must now configure the PFIC so that it enables that 
    //               interrupt and sends it on to the CPU. 
    // 
    //               You will need to configure PFIC_IENR1 for this, and you
    //               will have to check the vector table in the quickguide
    //               to see which interrupt number corresponds to EXTI7.
    ///////////////////////////////////////////////////////////////////////////
    uint32_t *ptr = (uint32_t *)(0xE000E000 + 0x104);

    *ptr |= (1 << (39 % 32));


    check_assignment_4(); 

    ///////////////////////////////////////////////////////////////////////////
    // Assignment 5: Initializing the vector table
    //               If everything is correct, and someone flips the switch 
    //               from 0 to 1, an interrupt request will be sent to the CPU.
    //               The CPU will then look at the CSR register `mtvec` to find
    //               our which address to jump to, and in which way (direct or
    //               vectored mode). 
    // 
    //               At the moment, mtvec is empty, so the CPU will crash and 
    //               burn. Implement the function init_interrupts in 
    //               assignment.s to fix this. 
    ///////////////////////////////////////////////////////////////////////////
    init_interrupts(); 
    check_assignment_5(); 

    ///////////////////////////////////////////////////////////////////////////
    // The program below blinks a light and should not be modified
    ///////////////////////////////////////////////////////////////////////////
    while(1)
    {
        systick->CTLR = 0; // Disable systick and clear configuration. 
        systick->CNT = 0; // Clear the current count
        systick->CMP = 14400000; 
        systick->SR = 0; // Clear the status register
        systick->CTLR = 0b100101; 
        while (systick->SR == 0);
        GPIO_D->OUTDR ^= 0xFF; // Toggle PD0
        while(!light_on); // Just wait if light is turned off
    }
}
