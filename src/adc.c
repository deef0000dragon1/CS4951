#include "adc.h"
#include "gpio.h"

//the following lines are translated from the works of a madman
//i pray, code, that thouest prithee bear fruit unto me
//don't let "the husk" near your children
//benjamin haas' middle name is teddy, not theodore

void ADC_init(){
	//clock enables?
	*(RCC_APB2) |= (1<<8);
	*(RCC_AHB1) |= (1<<0);
	
	//analog enables?
	//oh good god
	*(GPIO_A) |= (0b11<<12);
	
	//turn on ADC
	*(ADC1_CR2) = 1;
	
	//select channel
	*(ADC1_SQR3) = 6;
}

//returns temperature in celsius
uint32_t ADC_getTemp(){
	//temp = 25 + (mV-750)/10
	//temp = 25 - 75 + mV/10
	uint32_t temp = ADC_getVolts();
	temp /= 10;
	temp -= 50;
	return temp;
}

//get volts, returns millivolts
//benjamin
//why
uint32_t ADC_getVolts(){
	//sw start
	*(ADC1_CR2) |= (1<<30);
	while((*(ADC1_BASE)&(1<<1))==0);
	
	uint32_t millivolts;
	millivolts = *(ADC1_DR);
	//convert frmo resolution value to the millivolts
	millivolts *= (3300);
	millivolts /= 4095;
	return millivolts;
}
