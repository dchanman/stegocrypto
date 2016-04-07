/*
 * char_lcd.h
 *
 *  Created on: Apr 7, 2016
 *      Author: derek
 */

#ifndef CHAR_LCD_H_
#define CHAR_LCD_H_

void char_lcd_init();

void char_lcd_write(const char * msg);

void char_lcd_set_pos(int row, int col);


#endif /* CHAR_LCD_H_ */
