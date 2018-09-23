#include "gpio.h"
#include "delay.h"
#include <stdio.h>
#include <stdlib.h>

//a definition of an enum for the busy state
enum STATE {IDLE, BUSY, COLLISION};

//we'll need a global variable for the current state
volatile static enum STATE globalState = IDLE;

int main(){
	//initialization of the gpio pins and the timer here first

	//initialize GPIO pins from SoC
		*(GPIO_A + GPIO_MODER) &= ~(0x3F << 26);
		*(GPIO_A + GPIO_MODER) &= ~(0x3 << 0);

	//initialize some test pin for received data
		*(GPIO_A + GPIO_MODER) |= (0x15 << 26);
		*(GPIO_A + GPIO_MODER) |= (0x0 << 0);
	//set up the interrupt for it
		*(SYSCFG_CR1) &= ~(0x7 << 0);
		*(EXTI_BASE + EXTI_IMR) |= 0x1;
		*(EXTI_BASE + EXTI_RTSR) |= 0x1;
		*(EXTI_BASE + EXTI_FTSR) |= 0x1;
	//initialize the timer for interrupt


	//logic for the state stuff and setting up the interrupts
}

void pinISR(){
	//edge of manchester coding

	//set the timer to 1.1 ms

	//set the state to idle, cuz nothing is really happening
	
	//TODO: add to vector table
}

void timerISR(){
	//timeout on the manch encoding
	
	//if the level is 1, state is idle - otherwise, collision
}

//sets the proper LED for the proper state
//idle is green, busy is orange, collision is red
void setLED(void){

	switch(globalState){
		case IDLE:
			break;
		case BUSY:
			break;
		case COLLISION:
			break;
	}

}
