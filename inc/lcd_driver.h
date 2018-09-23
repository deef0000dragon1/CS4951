 /*
 * lcd_driver.h
 *
 * Created on: Dec 12, 2016
 * Author: carpenterme
 */

 #ifndef INC_LCD_DRIVER_H_
 #define INC_LCD_DRIVER_H_

 #include <inttypes.h>
 // RCC Register
 #define RCC_AHB1ENR (volatile uint32_t*) 0x40023830
 //Enable bits for GPIOC and GPIOB
 #define RCC_GPIOCEN 1<<2// GPIOC Enable is bit 2 in RCC_AHB1ENR
#define RCC_GPIOBEN 1<<1// GPIOB Enable is bit 1 in RCC_AHB1ENR
 //GPIO Registers
 #define GPIOB_MODER (volatile uint32_t*) 0x40020400
 #define GPIOC_MODER (volatile uint32_t*) 0x40020800
 #define GPIOC_IDR (volatile uint32_t*) 0x40020810
 #define GPIOB_BSRR (volatile uint32_t*) 0x40020418
 #define GPIOC_BSRR (volatile uint32_t*) 0x40020818

//MODER Offsets
 #define GPIOB_MODERBC 0b111111
 #define GPIOB_MODEROUT 0b010101
 #define GPIOC_MODERBC 0xFF0000
 #define GPIOC_MODEROUT 0x550000
 #define GPIOC_MODERIN 0x000000

 //LCD cmds
 #define EightBitMode 0x30
 #define FourBitMode 0x28
 #define clear 0x01
 #define home 0x02
 #define entry_right_noshift 0x06
 #define on_nocursor 0x0C

 //Delay times in us
 #define clear_home_delay 1520
 #define poweron_delay 5000
 #define latch_delay 1
 #define standard_delay 37
 //delay times in ms
 #define halfsecond 500
 //BSRR Offsets
 #define LCD_E_SET 0b100
 #define LCD_RW_SET 0b010
 #define LCD_RS_SET 0b001
 #define LCD_E_CLR 0b100<<16
 #define LCD_RW_CLR 0b010<<16
 #define LCD_RS_CLR 0b001<<16

 #define CLEAR_DATA_NIBBLE 0x0F000000
 //Busy flag is at PC11
 #define busyflag_bit 11
 //sends cmds to lcd
 void LCDcmd(uint32_t cmd);
 //sends char to lcd
 void LCDprt(char c);
//clears lcd
void LCDclr();
//send home cmd
 void LCDhome();
 //initializes lcd
 void LCDinit();
 //initializes control ports
 void LCDportinit();
 //sends byte of data to the lcd
 void LCDdata(uint32_t data);
 //function used in LCDdata to send data to lcd
 void LCDexec(uint32_t data);
 //enables sending to the lcd
 void LCDlatch();
 //sends the upper nibble of the data byte to lcd
void LCDsetuppernibble(uint32_t data);
 //sends the lower nibble of the data byte to lcd
 void LCDsetlowernibble(uint32_t data);
 //sets the cursor of lcd
 void LCDsetcursor(uint32_t row, uint32_t column);
 //prints a string to the lcd
 void LCDPrintString(char *strptr);
 //poll busy flag
 void LCDbusyFlag();
#endif /* INC_LCD_DRIVER_H_ */