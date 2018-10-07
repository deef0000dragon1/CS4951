
#include "uart_driver.h"
#include <inttypes.h>
#include <stdio.h>
#include "ringbuffer.h"

static volatile RingBuffer sendBuffer = {0, 0};
static volatile RingBuffer recieveBuffer = {0, 0};

char usart2_getch()
{
	//return the next character of the receiveBuffer
	return ringbufferGet(&recieveBuffer);
}

void usart2_putch(char c)
{
	ringbufferPut(&sendBuffer, c);

	//enable outputting interupt flag
	(*USART_CR1) = (*USART_CR1) | (1 << TXEIE);
}

void USART2_IRQHandler()
{
	uint32_t dr = (*USART_SR);
	if ((dr & (1 << RXNE)) == (1 << RXNE))
	{						  //RXNE
		char c = (*USART_DR); //get char from reg

		if (c == '\r')
		{						// If character is CR
			usart2_putch('\n'); // send it
			c = '\n';			// Return LF. fgets is terminated by LF
		}

		ringbufferPut(&recieveBuffer, c); //put char in input
		usart2_putch(c);				  //add char to echo back out.
	}

	if (~(*USART_SR) & (1 << TXE) == (1 << TXE))
	{ //TXE
		//if output uffer empty,
		//disable output flag TXE Controll
		//if not empty
		//put data in TDR

		if (!ringbufferHasElement(&sendBuffer))
		{
			//disable TXE
			(*USART_CR1) = (*USART_CR1) & ~(1 << TXEIE);
		}
		else
		{
			(*USART_DR) = ringbufferGet(&sendBuffer);
		}
	}

	//check if input or output
	//if input RXNE

	//if output ready TXE
}

void init_usart2(uint32_t baud, uint32_t sysclk)
{
	// Enable clocks for GPIOA and USART2
	*(RCC_AHB1ENR) |= (1 << GPIOAEN);
	*(RCC_APB1ENR) |= (1 << USART2EN);

	// Function 7 of PORTA pins is USART
	*(GPIOA_AFRL) &= (0xFFFF00FF);		// Clear the bits associated with PA3 and PA2
	*(GPIOA_AFRL) |= (0b01110111 << 8); // Choose function 7 for both PA3 and PA2
	*(GPIOA_MODER) &= (0xFFFFFF0F);		// Clear mode bits for PA3 and PA2
	*(GPIOA_MODER) |= (0b1010 << 4);	// Both PA3 and PA2 in alt function mode

	// Set up USART2
	//USART2_init(); //8n1 no flow control
	// over8 = 0..oversample by 16
	// M = 0..1 start bit, data size is 8, 1 stop bit
	// PCE= 0..Parity check not enabled
	// no interrupts... using polling
	*(USART_CR1) = (1 << UE) | (1 << TE) | (1 << RE); // Enable UART, Tx and Rx
	*(USART_CR2) = 0;								  // This is the default, but do it anyway
	*(USART_CR3) = 0;								  // This is the default, but do it anyway
	*(USART_BRR) = sysclk / baud;

	(*NVIC_ISER1) |= 1 << 6; //IRQ 38 is the 6th bit in the second ISR register

	(*USART_CR1) |= 1 << TXEIE | 1 << RXNEIE;

	/* I'm not sure if this is needed for standard IO*/
	//setvbuf(stderr, NULL, _IONBF, 0);
	//setvbuf(stdin, NULL, _IONBF, 0);
	setvbuf(stdout, NULL, _IONBF, 0);
}
