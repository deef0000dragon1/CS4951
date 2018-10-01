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
	*(TIM_2 + TIM_SR) |= (1 << 6);
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

	//set the timer to 1.1 ms
	resetTimer();
	//set the state to busy - we are getting information
	globalState = BUSY;

	if(*(GPIO_C + GPIO_IDR)&1){
		globalState = IDLE;
	}else{
		globalState = COLLISION;
	}
	//update LEDs
	setLED();


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

}

void timerISR()
{


}

void resetTimer()
{



}
