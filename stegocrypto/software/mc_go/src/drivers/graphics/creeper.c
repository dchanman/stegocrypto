/*
 * creeper.c
 *
 *  Created on: February 12, 2016
 *  Updated on: February 12, 2016
 *      Author: Logan
 */

#include <stdio.h>
#include "Colours.h"
#include "graphics.h"

//colours used for creeper
#define CREEPER_COLOUR_1 BLACK
#define CREEPER_COLOUR_2 PALE_GREEN
#define CREEPER_COLOUR_3 LIGHT_GREEN
#define CREEPER_COLOUR_4 LIME_GREEN
#define CREEPER_COLOUR_5 GREEN

//pixel width of creeper bitmap
#define pixelWidth 14

/* Static Functions */
static int creeper_head_generator(int x, int y, int pixelSize);
static int creeper_body_generator(int x, int y, int pixelSize);
static void creeper_legs_generator(int x, int y, int pixelSize);


void creeper_generator(int x, int y, int width) {
	int pixelSize = width / pixelWidth;
	int headShift = 2 * pixelSize;
	int bodyShift = 3 * pixelSize;
	printf("Pixel size is %d!\n", pixelSize);

	y = creeper_head_generator(x + headShift, y, pixelSize);
	y = creeper_body_generator(x + bodyShift, y, pixelSize);
	creeper_legs_generator(x, y, pixelSize);
}

/*
 * generates creeper head
 * x - top left x coordinate
 * y - top left y coordinate
 * pixelSize - width and length of each pixel box
 */
static int creeper_head_generator(int x, int y, int pixelSize) {
	int i, j;
	int tempX = x;

	int headColours[10][10] = {{CREEPER_COLOUR_5, CREEPER_COLOUR_2, CREEPER_COLOUR_3, CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_2, CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_3, CREEPER_COLOUR_4}, //row 1
							   {CREEPER_COLOUR_4, CREEPER_COLOUR_5, CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_2, CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_4}, //row 2
							   {CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_1, CREEPER_COLOUR_1, CREEPER_COLOUR_4, CREEPER_COLOUR_5, CREEPER_COLOUR_1, CREEPER_COLOUR_1, CREEPER_COLOUR_4, CREEPER_COLOUR_3}, //row 3
							   {CREEPER_COLOUR_3, CREEPER_COLOUR_4, CREEPER_COLOUR_1, CREEPER_COLOUR_1, CREEPER_COLOUR_2, CREEPER_COLOUR_4, CREEPER_COLOUR_1, CREEPER_COLOUR_1, CREEPER_COLOUR_4, CREEPER_COLOUR_5}, //row 4
							   {CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_3, CREEPER_COLOUR_4, CREEPER_COLOUR_1, CREEPER_COLOUR_1, CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_5, CREEPER_COLOUR_5}, //row 5
							   {CREEPER_COLOUR_2, CREEPER_COLOUR_5, CREEPER_COLOUR_4, CREEPER_COLOUR_1, CREEPER_COLOUR_1, CREEPER_COLOUR_1, CREEPER_COLOUR_1, CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_5}, //row 6
							   {CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_3, CREEPER_COLOUR_1, CREEPER_COLOUR_1, CREEPER_COLOUR_1, CREEPER_COLOUR_1, CREEPER_COLOUR_4, CREEPER_COLOUR_5, CREEPER_COLOUR_5}, //row 7
							   {CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_1, CREEPER_COLOUR_3, CREEPER_COLOUR_4, CREEPER_COLOUR_1, CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_5}, //row 8
							   {CREEPER_COLOUR_5, CREEPER_COLOUR_4, CREEPER_COLOUR_3, CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_5, CREEPER_COLOUR_4, CREEPER_COLOUR_2, CREEPER_COLOUR_4}, //row 9
							   {CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_5, CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_2, CREEPER_COLOUR_4, CREEPER_COLOUR_5, CREEPER_COLOUR_4}}; //row 10

	//implements graphics based on colors from color matrix headColours
	for (i = 0; i < 10; i++) {
		for (j = 0; j < 10; j++) {
			graphics_draw_rectangle_filled(x, y, pixelSize, pixelSize, headColours[i][j]);
			x += pixelSize;
		}
		x = tempX;
		y += pixelSize;
	}

	return y;
}

/*
 * generates creeper body
 * x - top left x coordinate
 * y - top left y coordinate
 * pixelSize - width and length of each pixel box
 */
static int creeper_body_generator(int x, int y, int pixelSize) {
	int i, j;
	int tempX = x;

	int bodyColours[11][8] = {{CREEPER_COLOUR_3, CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_2, CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_4}, //row
							  {CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_3, CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_5}, //row 2
							  {CREEPER_COLOUR_2, CREEPER_COLOUR_5, CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_4}, //row 3
							  {CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_3, CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_5}, //row 4
							  {CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_3, CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_4}, //row 5
							  {CREEPER_COLOUR_5, CREEPER_COLOUR_4, CREEPER_COLOUR_3, CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_5, CREEPER_COLOUR_2}, //row 6
							  {CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_4}, //row 7
							  {CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_5, CREEPER_COLOUR_4}, //row 8
							  {CREEPER_COLOUR_5, CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_2, CREEPER_COLOUR_4, CREEPER_COLOUR_3, CREEPER_COLOUR_4, CREEPER_COLOUR_4}, //row 9
							  {CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_4}, //row 10
							  {CREEPER_COLOUR_2, CREEPER_COLOUR_5, CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_3, CREEPER_COLOUR_4, CREEPER_COLOUR_5}}; //row 11

	//implements graphics based on colors from color matrix bodyColours
	for (i = 0; i < 11; i++) {
		for (j = 0; j < 8; j++) {
			graphics_draw_rectangle_filled(x, y, pixelSize, pixelSize, bodyColours[i][j]);
			x += pixelSize;
		}
		x = tempX;
		y += pixelSize;
	}

	return y;
}

/*
 * generates creeper legs
 * x - top left x coordinate
 * y - top left y coordinate
 * pixelSize - width and length of each pixel box
 */
static void creeper_legs_generator(int x, int y, int pixelSize) {
	int i, j;
	int tempX = x;

	int legsColours[4][14] = {{CREEPER_COLOUR_5, CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_2, CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_5}, //row 1
							  {CREEPER_COLOUR_2, CREEPER_COLOUR_4, CREEPER_COLOUR_5, CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_3, CREEPER_COLOUR_4, CREEPER_COLOUR_4}, //row 2
							  {CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_4}, //row 3
							  {CREEPER_COLOUR_1, CREEPER_COLOUR_5, CREEPER_COLOUR_1, CREEPER_COLOUR_5, CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_4, CREEPER_COLOUR_5, CREEPER_COLOUR_1, CREEPER_COLOUR_5, CREEPER_COLOUR_1}}; //row 4

	//implements graphics based on colors from color matrix legsColours
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 14; j++) {
			graphics_draw_rectangle_filled(x, y, pixelSize, pixelSize, legsColours[i][j]);
			x += pixelSize;
		}
		x = tempX;
		y += pixelSize;
	}
}
