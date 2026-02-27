#ifndef RCC_H
#define RCC_H
#include "stm32f411xe.h"

#define HSE_TIMEOUT 0x1000
#define PLL_TIMEOUT 0x1000

typedef enum {
    RCC_OK = 0,
    RCC_ERROR = 1
} RCC_ErrorStatus;

RCC_ErrorStatus ClockInit(void);

void MCO1_Init(void);


#endif

