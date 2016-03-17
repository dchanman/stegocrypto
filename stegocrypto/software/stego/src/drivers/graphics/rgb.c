/*
 * rgb.c
 *
 *  Created on: Mar 17, 2016
 *      Author: derek
 */

#include "rgb.h"
#include "Colours.h"

#define RGB_THRESHOLD	64

static int rgb[2][2][2] = {
		/* 000, 001      010, 011 */
		{{BLACK, BLUE}, {GREEN, CYAN}},
		/* 100, 101      110, 111 */
		{{RED, MAGENTA}, {YELLOW, WHITE}}
};

/* Janky conversion from RGB bytes to the hardcoded colours we use */
int rgb_to_8bit(unsigned char red_byte, unsigned char green_byte, unsigned char blue_byte) {

	int r = (red_byte < RGB_THRESHOLD ? 0 : 1);
	int g = (green_byte < RGB_THRESHOLD ? 0 : 1);
	int b = (blue_byte < RGB_THRESHOLD ? 0 : 1);

	return rgb[r][g][b];
}

