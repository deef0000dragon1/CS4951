#ifndef DELAY_H
#define DELAY_H

#define STK_CTRL (volatile uint32_t*) 0xE000E010
#define STK_LOAD (volatile uint32_t*) 0xE000E014
#define STK_VAL (volatile uint32_t*) 0xE000E018
#define STK_ENABLE_F 0
#define STK_CLKSRC_F 2
#define STK_CNT_F 16

#include <stdint.h>

void delay_1us_(uint32_t us);

#endif
