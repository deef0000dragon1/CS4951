#include "lcd.h"

/*
 * I swear to fucking god.
 * This code will kill me.
 * I spent hours transcribing this from a tome that would make mortals go mad.
 * I did it so you didn't have to.
 * You're welcome.
 * Now, let me end it all. Thanks.
 */
 
void LCD_init(){
	delay_1us_(5000);
	*(RCC_AHB1) |= 0b0110;
	
	*(GPIO_B) &= ~(0x3F); //clear the lower 6 bits
	*(GPIO_B) |= (0x15); //make them all outputs
	
	//same thing but in gpio C
	*(GPIO_C) &= ~(0xFF<<16);
	*(GPIO_C) |= (0x55<<16);
	
	LCD_cmd(0x30); //8bit mode
	LCD_cmd(0x28); //4bit mode
	LCD_clear(); //clear with delay
	LCD_home(); //home with delay
	LCD_cmd(0x06); //entry mode, move right
	LCD_cmd(0x0C); //display on with cursor blinking
}

void LCD_cmd(uint32_t cmd){
	*(GPIO_B + GPIO_BSRR) = LCD_RWCLR + LCD_RSCLR; //rw and rs values
	LCD_exec(cmd);
}

void LCD_data(uint32_t data){
	*(GPIO_B + GPIO_BSRR) = LCD_RWCLR + LCD_RSSET; //rw clr, rs set
	LCD_exec(data);
}

void LCD_exec(uint32_t datacomm){
	LCD_setUpperNibble(datacomm);
	LCD_latch();
	LCD_setLowerNibble(datacomm);
	LCD_latch();
	delay_1us_(400);
}

void LCD_setUpperNibble(uint32_t datacomm){
	datacomm &= ~(0xF); //mask lower nibble
	datacomm <<= 4; //shift to pins
	datacomm |= 0xF000000;
	*(GPIO_B + GPIO_BSRR) = datacomm;
}

void LCD_setLowerNibble(uint32_t datacomm){
	datacomm &= ~(0xF0); //mask upper nibble
	datacomm <<= 8; //shift to pins
	datacomm |= 0xF000000;
	*(GPIO_B + GPIO_BSRR) = datacomm;
}

void LCD_latch(){
	*(GPIO_B + GPIO_BSRR) = LCD_ESET;
	delay_1us_(1);
	*(GPIO_B + GPIO_BSRR) = LCD_ECLR;
	delay_1us_(1);
}

void LCD_clear(){
	LCD_cmd(0x1);
	delay_1us_(1500);
}

void LCD_home(){
	LCD_cmd(0x2);
	delay_1us_(1500);
}

uint32_t LCD_printNum(int num){
	char str[5];
	sprintf(str,"%d",num);
	return LCD_printString(str);
}

uint32_t LCD_printString(char str[]){
	uint32_t i = 0;
	while(str[i]!='\0'){
		LCD_data(str[i]);
		i++;
	}
	return i;
}

//ben i'm going to have you shot for this.
void LCD_setPosition(uint32_t row, uint32_t col){
	uint32_t address = 0;
	if(row == 1){
		address = 0x40;
	}
	address += col;
	address |= 0x80;
	LCD_cmd(address);
	delay_1us_(37);
}
