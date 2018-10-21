#include "gpio.h"
#include "delay.h"
#include "uart_driver.h"
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

volatile static int pinVal = 0;
volatile static int dummy = 0;

//for transmitter
volatile static int transmitChar = 0;
volatile static int bitPosTracker = 0;
volatile static int bitTracker = 0;
volatile static int sideTracker = 0;
volatile static int continueTransmission = 0;

volatile static int transmissionISRTestingMode = 0;

void initializeTimer();
void resetTimer();
void pinInit();
void pinISR();
void totalISR();
void timerISR();
void setLED();
void initTransmissionTimer();
void transmissionISR();

int main()
{
	//initialization of the gpio pins and the timer here first

	pinInit();

	//initialize the timer for interrupt

	initializeTimer();

	initTransmissionTimer();

	init_usart2(19200, 16000000);

	//enable tim6 in nvic
	*(NVIC_ISER1) |= (1 << 22);

	//enable in software IMPORTANT!!!
	asm("CPSIE i\n\t");

	//logic for the state stuff and setting up the interrupts
	globalState = IDLE;
	setLED();
	resetTimer();
	//*(EXTI_BASE + EXTI_SWIER) |= 1;

	//test the interrupt
	//*(TIM_6 + 5) |= 1;
	while (1)
		;
}

void pinInit(void)
{
	//initialize GPIO pins from SoC
	*(RCC_AHB1) |= 0xF; //enable a-d

	//Setting A pins as output
	*(GPIO_A + GPIO_MODER) &= ~(0xFFFF << 2);
	*(GPIO_A + GPIO_MODER) |= (0x5555 << 2);

	//initialize A0 for received data
	*(GPIO_A + GPIO_MODER) &= ~(0x3 << 0);
	*(GPIO_A + GPIO_MODER) |= (0x0 << 0);
	//set up the interrupt for it
	*(RCC_APB2) |= 1 << 14;
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
	*(NVIC_ISER0) |= (1 << 6);
}

void pinISR()
{

	//edge of manchester coding

	//temp disable timer
	*(STK_CTRL) &= ~(ENABLE | TICKINT);
	//GEt value from the timer
	int clockValue = *(STK_VAL);
	//set the state to busy - we are getting information
	globalState = BUSY;
	
	//update LEDs
	setLED();

	resetTimer();

	messageReceiver(clockValue);
	//Bit Value Tracking Func(timer value)
	//clear interrupt flag
	*(EXTI_BASE + EXTI_PR) |= (1 << 0);

	*(GPIO_A + GPIO_BSRR) = (0x1 << 20);
}

//sets the proper LED for the proper state
//idle is green, busy is orange, collision is red
void setLED(void)
{

	switch (globalState)
	{
	case IDLE:
		//set the green led, clear orange and red
		*(GPIO_A + GPIO_BSRR) = (0x1 << 5) | (0x1 << 22) | (0x1 << 23);
		break;
	case BUSY:
		//set the orange led A14
		*(GPIO_A + GPIO_BSRR) = (0x1 << 6) | (0x1 << 21) | (0x1 << 23);
		break;
	case COLLISION:
		//set the red led A15
		*(GPIO_A + GPIO_BSRR) = (0x1 << 7) | (0x1 << 21) | (0x1 << 22);
		break;
	}
}

void initializeTimer()
{
	*(STK_LOAD) = 18080; // number of cycles for 1.1ms.
	*(STK_VAL) = 18080;
	*(STK_CTRL) = CLKSOURCE | ENABLE | TICKINT; // System clock is clock s
}

void initTransmissionTimer()
{
	//enables timer
	*(RCC_APB1) |= (1 << 4);

	//set one pulse mode and climbing mode
	*(TIM_6) |= (1 << 3);

	//Enable Interrupt
	*(TIM_6 + TIM_DIER) |= (1 << 0);

	//clear the flag
	*(TIM_6 + TIM_SR) &= ~(1 << 0);

	//set maximum to 8k
	*(TIM_6 + TIM_ARR) = (7816);

	//Set timer value
	*(TIM_6 + TIM_CNT) = (0);

	//enable the timer
	*(TIM_6) |= 1;

	//turn on timer
	//set the timer time
	//set timer interrupt (timerISR)
}

//timer has gone beyond the expected value.
void timerISR()
{

	//disable timer
	if (*(STK_CTRL) & (1 << 16))
	{ //check if there was a rollover since the last check.

		*(STK_CTRL) &= ~(ENABLE | TICKINT);

		switch (globalState)
		{
		case IDLE:
			globalState = IDLE;
			break;
		case COLLISION:
			globalState = COLLISION;
			break;
		case BUSY:
			if (pinVal)
			{ //if input is 1, set state idle
				globalState = IDLE;
			}
			else
			{ //else set state collision.
				globalState = COLLISION;
			}
			break;
		}
	}
	setLED();
	resetTimer();
}

void totalISR()
{
	if (*(EXTI_BASE + EXTI_PR))
	{
		pinISR();
	}
	else
	{
		timerISR();
	}
}
void resetTimer(void)
{

	pinVal = *(GPIO_A + GPIO_IDR) & 1;

	//disable timer
	*(STK_CTRL) &= ~(ENABLE | TICKINT);

	*(STK_VAL) = 18080;

	//enable timer.
	dummy = (*(STK_CTRL)&1 << 16);

	*(STK_CTRL) |= ENABLE | TICKINT;
}

void setOutputPin(int val)
{
	if (val)
	{
		*(GPIO_A + GPIO_BSRR) = (1 << 8);
	}
	else
	{
		*(GPIO_A + GPIO_BSRR) = (1 << 24);
	}
}

void transmissionISR()
{
	if (globalState != COLLISION)
	{ //if not in a colission state, begin the output check code.

		//check if new transmission (idle) or if continuing transmission
		if (globalState == IDLE || continueTransmission == 1)
		{

			//if new transmission, set continue bit so that, can transmit when line is busy from self.
			if (globalState == IDLE)
			{
				continueTransmission = 1;
			}

			if (bitPosTracker == 0)
			{ //if there is bit position left to get, get a new character and update the tracking information.

				//sync pulse high
				*(GPIO_A + GPIO_BSRR) = (0x1 << 4);
				//*(GPIO_A + GPIO_BSRR) = (0x1 << 8);

				if (transmissionISRTestingMode)
				{
					transmitChar = 'M';
				}
				else
				{
					transmitChar = usart2_getch();
				}
				bitPosTracker = 8;
				bitTracker = 0;
				sideTracker = 0;

				//sync pulse low.
				*(GPIO_A + GPIO_BSRR) = (0x1 << 20);
				//*(GPIO_A + GPIO_BSRR) = (0x1 << 24);
			}

			//if the transmission charcter is not zero, output.
			if (transmitChar != 0)
			{

				if (sideTracker == 0)
				{
					//if the side is zero, (first side,)
					bitTracker = (transmitChar >> (bitPosTracker - 1)) & 1;
					//get the new bit
					//drop the tracker by one
					if (bitTracker == 1)
					{ //and output the first side of the data.
						setOutputPin(0);
					}
					else
					{
						setOutputPin(1);
					}
					sideTracker = 1;
				}
				else
				{
					//if its not a zero, its the second side.
					if (bitTracker == 1)
					{ //just output the second side of the data.
						setOutputPin(1);
					}
					else
					{
						setOutputPin(0);
					}
					bitPosTracker--;

					sideTracker = 0;
				}
			}else{
				setOutputPin(1);
				bitPosTracker = 0;
			}
		}
		else
		{
			//if the transmission character is zero, drop the line, and make it so that it checks again next time.
			bitPosTracker = 0;
			setOutputPin(1);
		}
	}
	else
	{ // if in the colission state, drop all output and release the line.
		continueTransmission = 0;
		setOutputPin(1);
	}

	//*(TIM_6 + 5) &= ~(1<<0);
	*(TIM_6 + TIM_SR) &= ~(1 << 0);
	*(TIM_6) |= 1;
}

void messageReceiver(int clocktime){

}
