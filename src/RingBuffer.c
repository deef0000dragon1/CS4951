#include <stdio.h>
#include <string.h>

#include "ringbuffer.h"

void ringbufferPut(RingBuffer * buffer, char element){
	while(!ringbufferHasSpace(buffer)){}

	buffer->buffer[buffer->put] = element;

	buffer->put = (buffer->put + 1)%BUF_SIZE;
}

char ringbufferGet(RingBuffer * buffer){

	while (!ringbufferHasElement(buffer)){}

	char c = buffer->buffer[buffer->get];

	buffer->get = (buffer->get + 1)%BUF_SIZE;

	return c;
}

int ringbufferHasSpace(RingBuffer * buffer){


	if(buffer->get == 0 && buffer->put == BUF_SIZE-1){
		return 0;
	}

	if((buffer->get - buffer->put) == 1){
		return 0;
	}

	return 1;
}

int ringbufferHasElement(RingBuffer * buffer){
	if (buffer->put != (buffer->get)){
		return 1;
	}

	return 0;
}
