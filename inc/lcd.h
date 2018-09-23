#ifndef LCD_H
#define LCD_H

#include <stdio.h>
#include <stdlib.h>
#include "gpio.h"
#include "delay.h"

#define LCD_ESET (1<<2)
#define LCD_RWSET (1<<1)
#define LCD_RSSET (1<<0)
#define LCD_ECLR (1<<18)
#define LCD_RWCLR (1<<17)
#define LCD_RSCLR (1<<16)

#define CLR_DATA_NIBBLE 0x0F00 << 16

void LCD_init();
void LCD_cmd(uint32_t cmd);
void LCD_data(uint32_t data);
void LCD_exec(uint32_t datacomm);
void LCD_setUpperNibble(uint32_t datacomm);
void LCD_setLowerNibble(uint32_t datacomm);
void LCD_latch();
void LCD_clear();
void LCD_home();
uint32_t LCD_printNum(int num);
uint32_t LCD_printString(char str[]);
void LCD_setPosition(uint32_t row, uint32_t col);

#endif
