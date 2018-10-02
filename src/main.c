#include "gpio.h"
#include "delay.h"
#include <stdio.h>
#include <stdlib.h>

//a definition of an enum for the busy state
enum STATE
{
	IDLE,
	BUSY,
	COLLISION
};

//we'll need a global variable for the current state
volatile static enum STATE globalState = IDLE;

int main()
{
	//initialization of the gpio pins and the timer here first

	//initialize GPIO pins from SoC
	*(RCC_AHB1) |= 0xF;//enable a-d

	*(GPIO_A + GPIO_MODER) &= ~(0x3F << 10);
	*(GPIO_A + GPIO_MODER) |= (0x15 << 10);


	//initialize some test pin for received data
	*(GPIO_A + GPIO_MODER) &= ~(0x3 << 0);
	*(GPIO_A + GPIO_MODER) |= (0x0 << 0);
	//set up the interrupt for it
	*(RCC_APB2) |= 1<<14;

	//syscfg allows interrupts in hardware
	*(SYSCFG_CR1) &= ~(0x7 << 0);
	*(SYSCFG_CR1) |= (0x0 << 0);
	//imr allows the interrupt in exti
	*(EXTI_BASE + EXTI_IMR) |= 0x1;
	//rising edge and falling edge enable
	*(EXTI_BASE + EXTI_RTSR) |= 0x1;
	*(EXTI_BASE + EXTI_FTSR) |= 0x1;
	//clear the pending flag if any
	*(EXTI_BASE + EXTI_PR) |= 0x1;
	//enable EXTi0 IMPORTANT!!!
	*(NVIC_ISER0) |= (1<<6);

	//enable in software IMPORTANT!!!
	asm("CPSIE i\n\t");

	//initialize the timer for interrupt

	initializeTimer();

	//logic for the state stuff and setting up the interrupts
	globalState = IDLE;
	setLED();
	resetTimer();
	*(EXTI_BASE + EXTI_SWIER) |= 1;

	while(1){
		/*if(*(GPIO_A + GPIO_IDR)&1){
			globalState = IDLE;
		}else{
			globalState = COLLISION;
		}
		setLED();*/
	}
}

void pinISR()
{
	//edge of manchester coding



	//temp disable timer
	*(STK_CTRL) &= ~(ENABLE);
	//set the state to busy - we are getting information
	globalState = BUSY;

	//update LEDs
	setLED();

	//clear interrupt flag
	*(EXTI_BASE + EXTI_PR) |= (1<<0);

	resetTimer();
}

//sets the proper LED for the proper state
//idle is green, busy is orange, collision is red
void setLED(void)
{

	switch (globalState)
	{
	case IDLE:
		//set the green led, clear orange and red
		*(GPIO_A + GPIO_BSRR) = (0x1 << 5)|(0x1 << 22)|(0x1 << 23);
		break;
	case BUSY:
		//set the orange led A14
		*(GPIO_A + GPIO_BSRR) = (0x1 << 6)|(0x1 << 21)|(0x1 << 23);
		break;
	case COLLISION:
		//set the red led A15
		*(GPIO_A + GPIO_BSRR) = (0x1 << 7)|(0x1 << 21)|(0x1 << 22);
		break;
	}
}

void initializeTimer()
{
	*(STK_LOAD) = 18080;  // number of cycles for 1.1ms.
	*(STK_CTRL) = CLKSOURCE | ENABLE | TICKINT; // System clock is clock s
}

//timer has gone beyond the expected value. 
void timerISR()
{

	int pinVal = *(GPIO_A + GPIO_IDR)&1;
	//disable timer
	*(STK_CTRL) &= ~(ENABLE);

	//if input is 1, set state idle
	if(pinVal){
		globalState = IDLE;
	}else{ //else set state colission. 
		globalState = COLLISION;
	}

	setLED();
	resetTimer();
}

void resetTimer()
{
	//disable timer
	*(STK_CTRL) &= ~(ENABLE);

	*(STK_VAL) = 18080;


	//enable timer.
	*(STK_CTRL) |= ENABLE;

}
