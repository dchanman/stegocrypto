/*
 * bitmap_display.c
 *
 *  Created on: Mar 17, 2016
 *      Author: derek
 */

#include <stdlib.h>
#include <stdio.h>
#include "bitmap.h"
#include "rgb.h"
#include "graphics.h"
#include "touchscreen.h"

#define BASE	200

static const char * filenames[] = {
		"whitebox.bmp",
		"redbox.bmp",
		"greenbox.bmp",
		"fancy.bmp",
		"bluebox.bmp",
		"logan.bmp"
};

int bitmap_display() {
	char * bitmap;
	int bitmap_data_offset;
	int imagefilesize;
	int width;
	int height;
	int i;
	int result;
	unsigned int imageindex = 0;
	Pixel pixel;

	int x = 0;
	int y = 0;
	unsigned char r;
	unsigned char g;
	unsigned char b;

	touchscreen_init();

	while (1) {
		touchscreen_get_press(&pixel, 3000);
		if (pixel.x > 400)
			imageindex++;
		else
			imageindex--;

		result = bitmap_import_image(filenames[imageindex % 6], &bitmap, &imagefilesize, &bitmap_data_offset);
		if (result != 0) {
			printf("Error: Could not import image from SD card\n");
			imageindex++;
			continue;
		}

		result = bitmap_process_header(bitmap, &imagefilesize, &bitmap_data_offset, &width, &height);
		if (result != 0) {
			free(bitmap);
			continue;
		}

		printf("Image width: %d\nImage height: %d\n", width, height);
		graphics_clear_screen();

		i = bitmap_data_offset;
		/* Got some weird flippy stuff and rotation stuff with how BMP is stored */
		//for (x = height+BASE; x > BASE; x--) {
		//	for (y = BASE; y < width+BASE; y++) {
		for (y = BASE; y < height+BASE; y++) {
			for (x = BASE; x < width+BASE; x++) {
				b = 0x000000FF & (bitmap[i]);
				i++;
				g = 0x000000FF & (bitmap[i]);
				i++;
				r = 0x000000FF & (bitmap[i]);
				i++;

				graphics_write_pixel(x, y, rgb_to_8bit(r, g, b));
			}
		}

		free(bitmap);
	}
	return 0;
}




