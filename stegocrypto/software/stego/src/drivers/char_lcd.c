/*
 * char_lcd.c
 *
 *  Created on: Apr 7, 2016
 *      Author: derek
 */

#include <stdlib.h>
#include <stdio.h>
#include "altera_up_avalon_character_lcd.h"
#include "system.h"

#define CHAR_LCD_PATH	"/dev"

static alt_up_character_lcd_dev * char_lcd_dev;

void char_lcd_init() {
	char_lcd_dev = alt_up_character_lcd_open_dev(CHARACTER_LCD_0_NAME);
	if (char_lcd_dev == NULL) {
		printf("[%s]: Error: Could not open char LCD device\n", __func__);
		return;
	}

	alt_up_character_lcd_init(char_lcd_dev);
	alt_up_character_lcd_cursor_off(char_lcd_dev);
}

void char_lcd_write(const char * msg) {
	if (char_lcd_dev == NULL) {
		printf("[%s]: Error: char LCD device not open\n", __func__);
		return;
	}

	alt_up_character_lcd_string(char_lcd_dev, msg);
}

void char_lcd_set_pos(int row, int col) {
	if (char_lcd_dev == NULL) {
		printf("[%s]: Error: char LCD device not open\n", __func__);
		return;
	}

	alt_up_character_lcd_set_cursor_pos(char_lcd_dev, col, row);
}
