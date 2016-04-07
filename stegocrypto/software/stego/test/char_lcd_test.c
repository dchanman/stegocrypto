/*
 * char_lcd_test.c
 *
 *  Created on: Apr 7, 2016
 *      Author: derek
 */

#include "char_lcd.h"

void char_lcd_test() {
	char_lcd_init();

	while (1) {
		char_lcd_init();

		usleep(1000000);

		char_lcd_write("Hello");

		usleep(1000000);

		char_lcd_set_pos(1,0);
		char_lcd_write("Hello");

		usleep(1000000);

		char_lcd_init();
		char_lcd_write("Hello\nHi this is me");

		usleep(1000000);
	}
}
