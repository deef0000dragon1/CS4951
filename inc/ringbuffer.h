/*
 * ringbuffer.h
 *
 *  Created on: Oct 7, 2018
 *      Author: gregersonnd
 */

#ifndef Koehler_Ring_buffer_driver
#define Koehler_Ring_buffer_driver

#define bitsPerSecond 1920
#define SwapTimeMS 20
#define BUF_SIZE 50

typedef struct myBuffer{
	unsigned int put;
	unsigned int get;
	char buffer[BUF_SIZE];
}RingBuffer;

//will put element on buffer. Blocks.
void ringbufferPut(RingBuffer* buffer, char element);
//Will get element from buffer. Blocks if empty
char ringbufferGet(RingBuffer* buffer);
//will return true if the buffer can accept a character
int ringbufferHasSpace(RingBuffer* buffer);
//will return true if the buffer can output a characte without blocking
int ringbufferHasElement(RingBuffer* buffer);


#endif /* RINGBUFFER_H_ */
