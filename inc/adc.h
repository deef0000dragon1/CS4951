#ifndef ADC_H
#define ADC_H

#include <stdio.h>
#include <stdlib.h>

#define ADC1_BASE (volatile uint32_t*) 0x40012000
#define ADC1_CR2 (volatile uint32_t*) 0x40012008
#define ADC1_SQR3 (volatile uint32_t*) 0x40012034
#define ADC1_DR (volatile uint32_t*) 0x4001204C

extern void ADC_init();
extern uint32_t ADC_getTemp();
extern uint32_t ADC_getVolts();

#endif
