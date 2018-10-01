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
	*(GPIO_C + GPIO_MODER) &= ~(0x3 << 0);
	*(GPIO_C + GPIO_MODER) |= (0x0 << 0);
	//set up the interrupt for it
	*(RCC_APB2) |= 1<<14;

	//syscfg allows interrupts in hardware
	*(SYSCFG_CR1) &= ~(0x7 << 0);
	*(SYSCFG_CR1) |= (0x2 << 0);
	//imr allows the interrupt in exti
	*(EXTI_BASE + EXTI_IMR) |= 0x1;
	//rising edge and falling edge enable
	*(EXTI_BASE + EXTI_RTSR) |= 0x1;
	*(EXTI_BASE + EXTI_FTSR) |= 0x1;
	//clear the pending flag if any
	*(EXTI_BASE + EXTI_PR) |= 0x1;
	//enable EXTi0 IMPORTANT!!!
	*(NVIC_ISER0) |= (1<<6);
	//enable TIM2
	*(NVIC_ISER0) |= (1<<28);

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
		if((*(GPIO_C + GPIO_IDR))&1){
			globalState = BUSY;
			setLED();
		}else{
			globalState = COLLISION;
			setLED();
		}
	}
}

void pinISR()
{
	//edge of manchester coding

	//set the timer to 1.1 ms
	resetTimer();
	//set the state to busy - we are getting information
	globalState = BUSY;

	//update LEDs
	setLED();
	//TODO: add to vector table

	//clear interrupt flag
	*(EXTI_BASE + EXTI_PR) |= (1<<0);
}

//sets the proper LED for the proper state
//idle is green, busy is orange, collision is red
void setLED(void)
{

	switch (globalState)
	{
	case IDLE:
		//set the green led A5
		*(GPIO_A + GPIO_BSRR) |= 0x1 << 5;

		//clear the orange and red leds, a6 and a7
		*(GPIO_A + GPIO_BSRR) |= 0x1 << 22;
		*(GPIO_A + GPIO_BSRR) |= 0x1 << 23;
		break;
	case BUSY:
		//set the orange led A14
		*(GPIO_A + GPIO_BSRR) |= 0x1 << 6;

		//clear the green and red leds, a13 and a15
		*(GPIO_A + GPIO_BSRR) |= 0x1 << 21;
		*(GPIO_A + GPIO_BSRR) |= 0x1 << 23;
		break;
	case COLLISION:
		//set the red led A15
		*(GPIO_A + GPIO_BSRR) |= 0x1 << 7;

		//clear the green and orange leds, a13 and a14
		*(GPIO_A + GPIO_BSRR) |= 0x1 << 21;
		*(GPIO_A + GPIO_BSRR) |= 0x1 << 22;
		break;
	}
}

void initializeTimer()
{
	//enables timer
	*(RCC_APB1) |= 1;


	//set one pulse mode and climbing mode
	*(TIM_2) |= (1 << 3);
	*(TIM_2) &= ~(1 << 4);

	//Enable Interupt
	*(TIM_2 + TIM_DIER) |= (1 << 6);

	//set maximum to 44K
	*(TIM_2 + TIM_ARR) = (44000);

	//Set timer value
	*(TIM_2 + TIM_CNT) = (0);

	//turn on timer
	//set the timer time
	//set timer interupt (timerISR)
}

void timerISR()
{
	//force clear interupt flag. 
	*(TIM_2 + TIM_SR) &= ~(1 << 6);

	//determine pin state
	if (*(GPIO_C + GPIO_IDR)&1) {
		//if 1, set idle
		globalState = IDLE;
	}else{
		//if 0, set colission
		globalState = COLLISION;
	}

	//set leds
	setLED();
	//timeout on the manch encoding
	resetTimer();

}

void resetTimer()
{

	//disable
	*(TIM_2) &= ~(1 << 0);

	//set one pulse mode and climbing mode
	//*(TIM2) |= (1 << 3);
	//*(TIM2) &= ~(1 << 4);

	//Enable Interupt
	//*(TIM2 + TIM_DIER) |= (1 << 6);

	//set maximum to 44K
	//*(TIM2 + TIM_ARR) = (44000);

	//Set timer value
	*(TIM_2 + TIM_CNT) = (0);

	//Enable Timer
	*(TIM_2) |= (1 << 0);

}
