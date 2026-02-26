///////////////////////////////////////////////////////////////////////////////
//
// Lecture 07 - Exercise 02
// In this exercise you will train to work with Structs, Unions and Ports in C
//
//  Once again, connect Console to Serial Interface 1, do the assignments
//  and verify that all the tests pass.
//
///////////////////////////////////////////////////////////////////////////////

#include <stdint.h>
#include <string.h>

///////////////////////////////////////////////////////////////////////////////
// Assignment 1 - define a structure `BankAccount` holding (in this specific
// order): 4 bytes for `account_number`
//         40 bytes for `holder_name`      (char holder_name[40])
//         4 bytes for `balance`           (floating point)
//         8 bytes for `interest_rate`     (double precision floating)
//         1 byte for `account_type`       (a character)
//
// After you do that, return a size of the structure in function assignment1.
// Use `sizeof()` operator!
//
///////////////////////////////////////////////////////////////////////////////

struct BankAccount{
    int account_number;
    char holder_name[40];
    float balance;
    double interest_rate;
    char account_type;
} bankA;


uint32_t assignment1(){
    return sizeof(bankA);
}

///////////////////////////////////////////////////////////////////////////////
// Assignment 2 - function assignment2 receives an address in memory.
// Convert this address into a pointer to `BankAccount` structure
// and fill account with this data:
//           account_number = 123456789
//           holder_name = Birta Eiriksdottir  (use strcpy() )
//           balance = 666.66
//           interest_rate = 2.56
//           account_type = 'd'
//
// Return the pointer to your structure.
//
///////////////////////////////////////////////////////////////////////////////
struct BankAccount bankAccount;

struct BankAccount * assignment2(int address){
    bankAccount.account_number = 123456789;
    strcpy(bankAccount.holder_name, "Birta Eiriksdottir");
    bankAccount.balance = 666.66;
    bankAccount.interest_rate = 2.56;
    bankAccount.account_type = 'd';

    return &bankAccount;
};

///////////////////////////////////////////////////////////////////////////////
// Assignment 3 - at address 0x20005000 a port with two 16-bit registers
// is located. Register CREG at offset 0x0 has two fields f0 (4 bits starting 
// at bit 0) and f1 (3 bits starting at bit 6). The rest of the bits are reserved
// and should never be modified. Register XREG has offset of 0x4 and it has
// two fields XLOW and XHIGH of 8 bits each. See register map:
// _______________________________________________________________Register | Offset
// | 15 |14 |13 |12 |11 |10 |09 |08 |07 |06 |05 |04 |03 |02 |01 |00 | 
// |----------------------------------------------------------------|
// |  x | x | x | x | x | x | x |    f1     | x | x |      f0       | CREG | 0x0
// |----------------------------------------------------------------|
// |              XHIGH              |              XLOW            | XREG | 0x4
// ‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾
// Note: bits 16-31 are not shown on the map. They are reserved and you should
// add padding bits to your structure.
//
// Your task is to write a struct definition named "Port" for this port, using bitfields.
// Then, in function assignment3, create a pointer to the port and 
// write 0b101 into f1, and 0x55 into XHIGH. Other fields, or reserved bytes 
// should not be modified.
//
///////////////////////////////////////////////////////////////////////////////
typedef struct {
    unsigned int f0 : 4;        // CREG bits 0-3
    unsigned int padding : 2;   // CREG bits 4-5
    unsigned int f1: 3;         // CREG bits 6-8
    unsigned int padding2 : 7;  // CREG bits 9-15
    unsigned int padding3 : 16; // CREG bits 16-31
} CREG;

typedef struct{
    unsigned int XLOW: 8;       // XREG bits 0-7 (offset 0x4)
    unsigned int XHIGH: 8;      // XREG bits 8-15 
    unsigned int padding4: 16;  // XREG bits 16-31
} XREG;

typedef struct Port{
    union { 
        uint32_t CREG;
        CREG creg; 
    };
    uint32_t _pad0;             //offset 0x4 ?
    union {
        uint32_t XREG;
        XREG xreg;
    };
};


void assignment3(){
    struct Port *port = (struct Port *)0x20005000;

    port->creg.f1 = 0b101;
    port->xreg.XLOW = 0x55;
}

///////////////////////////////////////////////////////////////////////////////
// Assignment 4 - update the definition of Port in Assignment 3 ...so that
// the XREG register can be accessed both as a a complete 16-bit register,
// and as two separate 8-bit fields named XLOW and XHIGH.
// You need to use a Union for this!
//
// In function `assignment4` use the same port at address 0x20005000 and
// return the value of the whole XREG as uint16_t (unsigned short).  
//
///////////////////////////////////////////////////////////////////////////////


uint16_t assignment4(){
    struct Port *port = (struct Port *)0x20005000;

    uint16_t low = port->xreg.XLOW;
    uint16_t high = port->xreg.XHIGH;

    uint32_t val = (port->xreg.XHIGH << 8) | port->xreg.XLOW;

    return val;
}
