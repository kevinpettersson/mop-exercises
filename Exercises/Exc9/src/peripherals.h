#ifndef PERIPHERALS_H
#define PERIPHERALS_H
#include <stdint.h>

///////////////////////////////////////////////////////////////////////////////
// Declare a structure for the SysTick Timer
///////////////////////////////////////////////////////////////////////////////
typedef struct {
    union {
        struct {
            uint32_t ste : 1; // SysTick Timer Enable
            uint32_t stie : 1; // SysTick Timer Interrupt Enable
            uint32_t stclk : 1; // SysTick Timer Clock Source
            uint32_t stre : 1; // SysTick Timer Reload Enable   
            uint32_t mode : 1; // SysTick Timer Mode 
            uint32_t init : 1; 
            uint32_t reserved : 25;
            uint32_t swie : 1; 
        };
        uint32_t CTLR; 
    };
    uint32_t SR;
    union {
        struct {
            uint32_t CNTL;
            uint32_t CNTH;
        };
        uint64_t CNT; 
    }; 
    union {
        struct {
            uint32_t CMPL;
            uint32_t CMPH;
        };
        uint64_t CMP; 
    }; 
} SysTick_t;

///////////////////////////////////////////////////////////////////////////////
// Structure for GPIO
///////////////////////////////////////////////////////////////////////////////
typedef struct {
    uint32_t CFGLR; 
    uint32_t CFGHR;
    uint16_t INDR;
    uint16_t reserved0; 
    uint16_t OUTDR;
    uint16_t reserved1;
    uint32_t BSHR;
    uint16_t BCR; 
    uint16_t reserved2;
    uint32_t LCKR;
} GPIO_t;

#endif
