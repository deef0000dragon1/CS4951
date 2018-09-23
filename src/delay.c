#include "delay.h"

void delay_1us_(uint32_t us){
	
	*(STK_CTRL) &= ~(1<<STK_ENABLE_F);
	
	for(uint32_t i = 0; i<us; i++){
		*(STK_LOAD) = 16;
		*(STK_CTRL) |= ((1<<STK_ENABLE_F) | (1<<STK_CLKSRC_F));
		while((*(STK_CTRL) & (1<<STK_CNT_F)) != (1<<STK_CNT_F));
	}
	
	*(STK_CTRL) &= ~(1<<STK_ENABLE_F);
}