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
	*(RCC_AHB1) |= 0x1;

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

void pinISR()
{
	//edge of manchester coding

	//set the timer to 1.15 ms

	//set the state to idle, cuz nothing is really happening

	//TODO: add to vector table
}

//sets the proper LED for the proper state
//idle is green, busy is orange, collision is red
void setLED(void)
{

	switch (globalState)
	{
	case IDLE:
		//set the green led A13
		*(GPIO_A + GPIO_BSRR) |= 0x1 << 13;

		//clear the orange and red leds, a14 and a15
		*(GPIO_A + GPIO_BSRR) |= 0x1 << 30;
		*(GPIO_A + GPIO_BSRR) |= 0x1 << 31;
		break;
	case BUSY:
		//set the orange led A14
		*(GPIO_A + GPIO_BSRR) |= 0x1 << 14;

		//clear the green and red leds, a13 and a15
		*(GPIO_A + GPIO_BSRR) |= 0x1 << 29;
		*(GPIO_A + GPIO_BSRR) |= 0x1 << 31;
		break;
	case COLLISION:
		//set the red led A15
		*(GPIO_A + GPIO_BSRR) |= 0x1 << 15;

		//clear the green and orange leds, a13 and a14
		*(GPIO_A + GPIO_BSRR) |= 0x1 << 29;
		*(GPIO_A + GPIO_BSRR) |= 0x1 << 30;
		break;
	}
}

void initializeTimer()
{
	//enables timer
	*(RCC_APB1) |= 1;


	//set one pulse mode and climbing mode
	*(TIM2) |= (1 << 3);
	*(TIM2) &= ~(1 << 4);

	//Enable Interupt
	*(TIM2 + TIM_DIER) |= (1 << 6);

	//set maximum to 44K
	*(TIM2 + TIM_ARR) = (44000);

	//Set timer value
	*(TIM2 + TIM_CNT) = (0);

	//turn on timer
	//set the timer time
	//set timer interupt (timerISR)
}

void timerISR()
{
	//force clear interupt flag. 
	*(TIM2 + TIM_SR) &= ~(1 << 6);

	//determine pin state
	if (*(GPIO_A + GPIO_ODR)) {
		//if 1, set idle
		globalState = IDLE;
	}else{
		//if 0, set colission
		globalState = COLLISION;
	}


	//timeout on the manch encoding
	

}

void restTimer()
{

	//disable
	*(TIM2) &= ~(1 << 0);

	//set one pulse mode and climbing mode
	//*(TIM2) |= (1 << 3);
	//*(TIM2) &= ~(1 << 4);

	//Enable Interupt
	//*(TIM2 + TIM_DIER) |= (1 << 6);

	//set maximum to 44K
	//*(TIM2 + TIM_ARR) = (44000);

	//Set timer value
	*(TIM2 + TIM_CNT) = (0);

	//Enable Timer
	*(TIM2) |= (1 << 0);

}
