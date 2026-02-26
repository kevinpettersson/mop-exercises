#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

extern uint32_t assignment1();
extern struct BankAccount * assignment2(int address);
extern void assignment3();
extern uint16_t assignment4();

struct BankAccountTest {
    int accountNumber;      
    char holderName[40];   
    float balance;         
    double interestRate;  
    char accountType; 
};
struct BankAccountTest account_test = {123456789, "Birta Eiriksdottir", 666.66, 2.56, 'd'}; 

struct PortTest{
    struct {
        unsigned int f0: 4;
        unsigned int _res1: 2;
        unsigned int f1: 3;
        unsigned int _res2: 7;
        unsigned int _res3: 16;
    };

    union{
        uint16_t XREG;
        struct{
            uint8_t XLOW;
            uint8_t XHIGH;
        };
    };

    unsigned short _res4;
};

















































    ///////////////////////////////////////////////////////////////////////////////
    //////////////////     Spoilers above             /////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////

int main(void)
{
    ///////////////////////////////////////////////////////////////////////////////
    // Test assignment 1
    ///////////////////////////////////////////////////////////////////////////////
    printf("Testing assignment 1...\n");
    uint32_t student_size = assignment1();
    if (student_size == sizeof(struct BankAccountTest)){
        printf("PASSED!\n\n");
    }
    else {
        printf("Received an unexpected structure size");
        printf("FAILED!\n\n");
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Test assignment 2
    ///////////////////////////////////////////////////////////////////////////////
    printf("Testing assignment 2...\n");
    void *student_struct = malloc(100);
    memset(student_struct, 0, 100);
    void *student_return = assignment2((int)student_struct);
    if (memcmp(student_return, &account_test, sizeof(account_test)) == 0){
        printf("PASSED!\n\n");
    }
    else if (student_return != student_struct){
        printf("Wrong address is returned...");
        printf("FAILED!\n\n");
    } 
    else{
        printf("Wrong memory content is returned...");
        printf("FAILED!\n\n");
    }
    ///////////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////////
    // Test assignment 3
    ///////////////////////////////////////////////////////////////////////////////
    printf("Testing assignment 3...\n");
    int address = 0x20005000;
    memset((void *)address, (char)0x00, sizeof(struct PortTest)+4);
    memset((void *)address, (char)0xAA, sizeof(struct PortTest));
    struct PortTest* port_test = (struct PortTest *) address;
    port_test->f1 = 0b0000;
    port_test->XHIGH = 0x0;

    assignment3();

    int a3_flag = 1;
    if (port_test->f1 != 0x5 || port_test->XHIGH != 0x55){
        printf("Required fields are not set correctly\n");
        a3_flag = 0;
    }
    if (port_test->XLOW != 0xAA || port_test->f0 != 0xA || port_test->_res1 != 0x2 ||
        port_test->_res2 != 0x55 || port_test->_res3 != 0xAAAA){
        printf("Other fields rather than f1 or XHIGH are modified\n");
        a3_flag = 0;
    }

    if (a3_flag){
        printf("PASSED\n\n");
    }
    else {
        printf("FAILED!\n\n");
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Test assignment 4
    ///////////////////////////////////////////////////////////////////////////////
    printf("Testing assignment 4...\n");

    uint16_t a4_ret = assignment4();

    if (a4_ret != 0x55AA){
        printf("FAILED!\n\n");
    }
    else{
        printf("PASSED!\n\n");
    }
}
