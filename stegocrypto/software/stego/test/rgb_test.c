/*
 * rgb_test.c
 *
 *  Created on: Mar 17, 2016
 *      Author: derek
 */
#include "rgb.h"
#include "Colours.h"

static int testcount = 0;
static int testpassed = 0;

static void test_rgb_helper(char r, char g, char b, int expected_colour) {
	int colour;

	testcount++;

	colour = rgb_to_8bit(r, g, b);
	if (colour != expected_colour) {
		printf("Test #%d: Expected <%d>, got <%d>\n", testcount, expected_colour, colour);
		return;
	}

	testpassed++;
}

void rgb_test() {
	/*              R     G     B     Colour */
	test_rgb_helper(0x00, 0x00, 0x00, BLACK);
	test_rgb_helper(0x00, 0x00, 0xFF, BLUE);
	test_rgb_helper(0x00, 0xFF, 0x00, GREEN);
	test_rgb_helper(0x00, 0xFF, 0xFF, CYAN);
	test_rgb_helper(0xFF, 0x00, 0x00, RED);
	test_rgb_helper(0xFF, 0x00, 0xFF, MAGENTA);
	test_rgb_helper(0xFF, 0xFF, 0x00, YELLOW);
	test_rgb_helper(0xFF, 0xFF, 0xFF, WHITE);

	printf("%d/%d tests passed\n", testpassed, testcount);
}
