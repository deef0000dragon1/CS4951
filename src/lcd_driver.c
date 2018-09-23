 /*
 * lcd_driver.c
 *
 * Created on: Dec 12, 2016
 * Author: carpenterme
 */
 #include "lcd_driver.h"
 #include "delay.h"
 /*
 * sends cmds to the LCD
 */
 void LCDcmd(uint32_t cmd){
 //Clear R/W and RS for sending the command
 *GPIOB_BSRR |= (LCD_RW_CLR|LCD_RS_CLR);
 LCDexec(cmd);
 }

 /*
 * Prints a character to the LCD
 */
 void LCDprt(char c){
 //sends the char using LCDdata()
 LCDdata(c);
 }

 /*
 * sends a clear cmd to the lcd
 */
 void LCDclr(){
 LCDbusyFlag();
 LCDcmd(clear);
 }

 /*
 * sends a home cmd to the lcd
 */
 void LCDhome(){
 LCDbusyFlag();
 LCDcmd(home);
 }

 /*
 * initializes the lcd
 */
 void LCDinit(){
 //5 ms delay for power-up
 delay_1us_(poweron_delay);
 //Enable clocks for GPIOB & GPIOC
 *RCC_AHB1ENR |= (RCC_GPIOBEN|RCC_GPIOCEN);
 //set all pins connected to lcd as output
 *GPIOB_MODER &= ~GPIOB_MODERBC;
 *GPIOB_MODER |= GPIOB_MODEROUT;
 LCDportinit();
 //First initialize LCD in 8 bit mode
 LCDcmd(EightBitMode);
 delay_1us_(standard_delay);
 //Second set it to 4 bit mode

 LCDcmd(FourBitMode);
 delay_1us_(standard_delay);
 //delays are no longer necessary because the busy flag is now available
 //clear display
 LCDclr();
 //home display
 LCDhome();
 //set to no shift print to the right
 LCDbusyFlag();
 LCDcmd(entry_right_noshift);
 //turn on display with no blinking cursor
 LCDbusyFlag();
 LCDcmd(on_nocursor);
 }

 /*
 * Set direction of LCD data and control ports
*/
 void LCDportinit(){
 //control ports are on GPIOC
 *GPIOC_MODER &= ~GPIOC_MODERBC;
 *GPIOC_MODER |= GPIOC_MODEROUT;
 }

 /*
 * send a byte of data to the lcd
 */
 void LCDdata(uint32_t data){
//for sending data send clear R/W and set RS
 *GPIOB_BSRR |= LCD_RW_CLR;
 *GPIOB_BSRR |= LCD_RS_SET;
 LCDexec(data);
 }

 /*
 * function used in LCDdata and LCDcmd to send the byte to the LCD
 */
 void LCDexec(uint32_t data){
 //Send upper 4 bits to LCD
 LCDsetuppernibble(data);
 LCDlatch();
 //Send lower 4 bits
 LCDsetlowernibble(data);
 LCDlatch();
 }

 /*
 * latch for the lcd
 */
 void LCDlatch(){
 //set enable to high so the data gets sent
 *GPIOB_BSRR |= LCD_E_SET;
 delay_1us_(latch_delay);
 //set it low again
 *GPIOB_BSRR |= LCD_E_CLR;
 delay_1us_(latch_delay);
 }


 /*
 * send upper nibble to the lcd used in LCDexec
 */
 void LCDsetuppernibble(uint32_t data){
 //get upper nibble by itself and send it to the BSRR
 data = data & 0xF0;
 data = data<<4;
 *GPIOC_BSRR |= (CLEAR_DATA_NIBBLE|data);
 }

 /*
 * send lower nibble to the lcd used in LCDexec
 */
 void LCDsetlowernibble(uint32_t data){
 //get lower nibble by itself and send it to the BSRR
 data = data & 0x0F;
 data = data<<8;
 *GPIOC_BSRR |= (CLEAR_DATA_NIBBLE|data);
 }

 /*
 * set position of cursor on lcd
 */
 void LCDsetcursor(uint32_t row, uint32_t column){
 uint32_t cursor = 1<<7;//1 needed in DB7
 if(column < 15){
 //only sets cursor if its within the boundaries of the lcd
 if(row < 2){
 //64 is to the next line so a it will move to the next line if row =1
 cursor += (row*64) + column;
 }
 LCDbusyFlag();
 LCDcmd(cursor);
 }
 }
 /*
* Print a string to the LCD
 */
 void LCDPrintString(char *strptr){
 char *start;
 for(start = strptr; *start != '\0'; start++){
 LCDbusyFlag(); //check if it is ok to send to LCD
 LCDprt(*start); //print char to LCD
 }
 }
 /*
 * poll busy flag of the LCD
*/
 void LCDbusyFlag(){
 uint32_t busy_flag = 1; //initialize the busy flag variable
 *GPIOC_MODER &= ~GPIOC_MODERBC; //set GPIOC to input to read busy flag
 *GPIOC_MODER |= GPIOC_MODERIN;
 *GPIOB_BSRR |= (LCD_RW_SET|LCD_RS_CLR); //Set lcd to read mode and RS low
 while(busy_flag != 0){
 *GPIOB_BSRR |= LCD_E_SET; //Set enable pin high
 delay_1us_(latch_delay); // 1us delay for timing
 busy_flag = ((*GPIOC_IDR)&(1<<busyflag_bit)); //read busy flag status and store it
 *GPIOB_BSRR |= LCD_E_CLR; //Clear enable bit
 delay_1us_(latch_delay); //1 us delay for timing

 //code used to read and throw away the lower nibbles
 *GPIOB_BSRR |= LCD_E_SET;
 delay_1us_(latch_delay);
 *GPIOB_BSRR |= LCD_E_CLR;
 delay_1us_(latch_delay);
 }
 *GPIOC_MODER &= ~GPIOC_MODERBC; //set GPIOC back to output
 *GPIOC_MODER |= GPIOC_MODEROUT;
 *GPIOB_BSRR |= LCD_RW_CLR; //Set LCD back to write mode
 }
