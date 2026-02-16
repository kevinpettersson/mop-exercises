
#include <stdio.h>
#include <stdint.h>
///////////////////////////////////////////////////////////////////////////////
// Lab Preparation 2
// ============================================================================
// In Lab 2 you will need to know how the ASCII display works, so in this lab 
// you will write a program that interacts with it, on the simulator. 
//
// YOU ARE ALSO EXPECTED TTUREO HAVE DONE THE EXERCISE FOR LEC 05, so you know 
// how the keyboard input works. 
//
// IMPORTANT: Before you do this lab, go through Lecture06, Excercise 02, where
// you learn how to create short delays using systick. This exercise will not
// test that your delay functions work correctly, and if they don't work
// it might still work on the simulator but not on hardware. 
//
// In this assignment there are check_assignment_x() functions  already in place
// where we try to check that you have solved each assignment correctly. Once you 
// have passed an assignment, you can comment these out.
// 
// Please see the quickguide for a summary of the ASCII Display.
// 
// In the simulator IO Setup, connect 
// GPIO E(0-15)/SPI Display -> 18 LC/TFT Display   AND
// Serial Communications Interface 1 -> 06 Console
///////////////////////////////////////////////////////////////////////////////

#include <stdint.h>

void check_assignment_1(); 
void check_assignment_2_1(uint8_t command_or_data);
void check_assignment_2_2();
void check_assignment_4_1();
void check_assignment_4_2();
void check_assignment_4_3();
void check_assignment_4_4();
void check_assignment_4_5();
void check_assignment_5();

///////////////////////////////////////////////////////////////////////////////
// Register macros. 
///////////////////////////////////////////////////////////////////////////////

#define STK_BASE 0xE000F000
#define STK_CTLR (*(volatile uint32_t*) (STK_BASE + 0x00))
#define STK_SR   (*(volatile uint32_t*) (STK_BASE + 0x04))
#define STK_CNTL  (*(volatile uint32_t*) (STK_BASE + 0x08))
#define STK_CNTH  (*(volatile uint32_t*) (STK_BASE + 0x0C))
#define STK_CMPL  (*(volatile uint32_t*) (STK_BASE + 0x10))
#define STK_CMPH  (*(volatile uint32_t*) (STK_BASE + 0x14))


#define GPIO_E 0x40011800
#define GPIO_E_CFGLR  (*(volatile uint32_t*) (GPIO_E + 0x0))
#define GPIO_E_CFGHR  (*(volatile uint32_t*) (GPIO_E + 0x4))
#define GPIO_E_INDR   (*(volatile short*) (GPIO_E + 0x8))
#define GPIO_E_OUTDR  (*(volatile short*) (GPIO_E + 0xC))


///////////////////////////////////////////////////////////////////////////////
// Assignment 0: If you have done Lecture06 Exercise02, you already have
//               the code for this. Implement the delay functions below.
//               They are not tested in this assignment, so make sure they
//               work using the Lecture06 Exercise02.
///////////////////////////////////////////////////////////////////////////////
uint64_t ns_to_ticks(uint64_t ns) {
    return (ns * 144) / 1000; // ns * 0.144
}

void delay(uint64_t ns) {
    uint64_t ticks = ns_to_ticks(ns);

    STK_CTLR = 0;             // disable
    STK_CNTH = 0;
    STK_CNTL = 0;
    STK_CMPL = (uint32_t)ticks;
    STK_CMPH = (uint32_t)(ticks >> 32);

    STK_CTLR = 0b101;         // enable, count down

    while ((STK_SR & 0x1) == 0); // wait for count flag

    STK_CTLR = 0;             // disable
    STK_SR = 0;               // clear flag
}

void delay_us(uint32_t us) { delay((uint64_t)us * 1000); }
void delay_ms(uint32_t ms) { delay_us((uint32_t)ms * 1000); }


///////////////////////////////////////////////////////////////////////////////
// Assignment 1: Configure GPIO Port E so that all pins [0:15] are set as 
//               output, 2MHz, push-pull.
///////////////////////////////////////////////////////////////////////////////
void init_gpio_port_e()
{
    int config = 0x22222222; // set 0b0010 for all pins 0-15

    GPIO_E_CFGLR = config;
    GPIO_E_CFGHR = config;
}

///////////////////////////////////////////////////////////////////////////////
// Assignment 2: Implement the write cycle. This will be used to write both 
//               commands and data to the display. 
//    
//               Look at the "Writing a command or data to the display" 
//               timing diagram in the quickguide, to see how this works.
// 
// Input: command_or_data - the byte to write to the display. This can be 
//                          either a command or data, depending on the state 
//                          of the control lines.
///////////////////////////////////////////////////////////////////////////////
void ascii_write_controller(uint8_t command_or_data)
{
    // The control lines (RS and RW) are set before calling this function.
    // According to the quickguide, you need to wait 40ns after setting the
    // control lines before initiating a write to the display. 
    //
    // 40ns is the time it takes to execute 8 instructions, so that time 
    // will already have passed by the time the processor has entered this
    // function. 
    delay(40);
    ///////////////////////////////////////////////////////////////////////////
    // Assignment 2.1: Set E = 1 to start the write cycle
    //                 Then write the command or data to GPIOE_OUTDR[8..15].
    //                 Hint: Make sure your data write does not overwrite 
    //                 the control bits in GPIOE_OUTDR[0..7]!
    ///////////////////////////////////////////////////////////////////////////
    GPIO_E_OUTDR |= (1 << 2);
    int16_t tmp = GPIO_E_OUTDR;
    tmp &= ~(0xFF << 8);    
    tmp |= (command_or_data << 8);
    GPIO_E_OUTDR = tmp;
    check_assignment_2_1(command_or_data);

    ///////////////////////////////////////////////////////////////////////////
    // Assignment 2.2: Wait for at least  max(tsu2, tw) = 230ns, then set 
    //                 E = 0 to end the write cycle.
    ///////////////////////////////////////////////////////////////////////////
    delay(230);
    GPIO_E_OUTDR &= ~(1 << 2);
    check_assignment_2_2();

    // Finally, wait for th = 10ns before returning from the function. This 
    //is the time it takes for the display to process the command or data after 
    // the write cycle has ended.
    //
    // 10ns is the time it takes to execute 2 instructions, so that time will
    // already have passed by the this function returns. 
    delay(10);
}

///////////////////////////////////////////////////////////////////////////////
// Assignment 3: Implement code for writing a command to the display.
///////////////////////////////////////////////////////////////////////////////
void ascii_write_command(uint8_t command)
{
    // TODO: Set RS = 0 and RW = 0 to indicate a command write, then call
    //       ascii_write_controller to write the command to the display.
    GPIO_E_OUTDR &= ~(0b11);
    ascii_write_controller(command);
}

///////////////////////////////////////////////////////////////////////////////
// Assignment 4: Implement a function that reads the status byte from the 
//               display, to see if it is busy or ready for the next command.
//
//               Look at the "Reading status or data from the display" timing 
//               diagram in the quickguide, to see how this works.
//
// Output: 1 if busy, 0 if ready for new command.
///////////////////////////////////////////////////////////////////////////////
uint8_t ascii_read_status()
{
    ///////////////////////////////////////////////////////////////////////////
    // Assignment 4.1: Temporarily configure GPIO Port E pins [8:15] as 
    //                 input, floating.
    ///////////////////////////////////////////////////////////////////////////
    int config = 0x44444444;
    GPIO_E_CFGHR = config;
    check_assignment_4_1(); 
    ///////////////////////////////////////////////////////////////////////////
    // Assignment 4.2: Set RS = 0 (command) and RW = 1 (read) to indicate 
    //                 a status read.
    ///////////////////////////////////////////////////////////////////////////
    GPIO_E_OUTDR &= ~(1 << 0);  // RS = 0
    GPIO_E_OUTDR |= (1 << 1);   // RW = 1
    check_assignment_4_2(); 
    ///////////////////////////////////////////////////////////////////////////
    // Assignment 4.3: Set E = 1 to start the read cycle,
    //                 wait for tD = 360ns
    //                 then read the status byte from GPIO Port E INDR register
    //                 pins [8:15]
    ///////////////////////////////////////////////////////////////////////////
    GPIO_E_OUTDR |= (1 << 2);
    delay(260);
    int8_t status = (GPIO_E_INDR >> 8);
    check_assignment_4_3(); 
    ///////////////////////////////////////////////////////////////////////////
    // Assignment 4.4: Set E = 0 to end the read cycle.
    ///////////////////////////////////////////////////////////////////////////
    GPIO_E_OUTDR &= ~(1 << 2);
    check_assignment_4_4(); 

    ///////////////////////////////////////////////////////////////////////////
    // Assignment 4.5: Configure GPIO Port E pins [8:15] back to output,
    //                 2MHz, push-pull.
    ///////////////////////////////////////////////////////////////////////////
    GPIO_E_CFGHR = 0x22222222;
    check_assignment_4_5(); 

    ///////////////////////////////////////////////////////////////////////////
    // Assignment 4.6: Return the busy flag (bit 7 of the status byte)
    ///////////////////////////////////////////////////////////////////////////
    return status;
}


///////////////////////////////////////////////////////////////////////////////
// Assignment 5: Implement code for writing a chareacter (data) to the display.
///////////////////////////////////////////////////////////////////////////////
void ascii_write_data(uint8_t data)
{
    // TODO: Set RS = 1 (data) and RW = 0 (write) to indicate a command write, 
    //       then call ascii_write_controller to write the character to the 
    //       display.
    GPIO_E_OUTDR |= (1 << 0);
    GPIO_E_OUTDR &= ~(1 << 1);
    ascii_write_controller(data);
}


///////////////////////////////////////////////////////////////////////////////
// Assignment 6: Remove the training wheels!
// ============================================================================
// If you have passed all the previous assignments, and can see the password
// on the display when you run the program, then it is time to write your own
// main function that uses your functions to write to the display. 
// 
// Assignment 6.1: In the main.c file, rename "int main(void)" to 
// "int main_old(void)". 
// 
// Then uncomment the main function below and implement the missing code.
// You will need to consult the list of commands in the quickguide for this. 
///////////////////////////////////////////////////////////////////////////////

int main()
{
    // Initialize GPIO Port E (just call your function from Assignment 1
    init_gpio_port_e();
    // Read status until display is not busy.
    while(ascii_read_status() & 0x80);

    // Write a "Display Control" command that turns the display off
    ascii_write_command(0x08);
    // Read status until display is not busy.
    while(ascii_read_status() & 0x80);
    // Write a "Function Set" command that sets the function to 2 lines, 5x8 dots.
    ascii_write_command(0x38);
    // Read status until display is not busy.
    while(ascii_read_status() & 0x80);
 
    // Write a "Display Control" command that turns the dusplay on, cursor on, blink
    ascii_write_command(0x0E);

    // Read status until display is not busy.
    while(ascii_read_status() & 0x80);

    // Write a "Entry Mode Set" command that says cursor should move right and 
    // display should not shift.
    ascii_write_command(0x06);


    // Read status until display is not busy.
    while(ascii_read_status() & 0x80);

    // Write a "Clear Display" command to clear the display.
    ascii_write_command(0x01);

    // Write the name of your group to the display
    char *name = "Agile Albatrosses";
    for (int i = 0; i < 18; i++)
    {
        ascii_write_data((uint8_t)name[i]);
    }
    
}