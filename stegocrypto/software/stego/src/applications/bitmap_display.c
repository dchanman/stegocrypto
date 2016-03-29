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
#include "sdcard.h"
#include "touchscreen.h"

#define BASE	0

static char * filenames[32];
static int numfiles = 0;

void bitmap_get_bitmap_filenames_from_sdcard() {
	int i;

	/* Free the old buffers for filenames before getting new ones */
	for (i = 0; i < numfiles; i++) {
		free(filenames[i]);
	}

	printf("[%s]: Getting files...\n", __func__);
	sdcard_get_files(filenames, &numfiles, "/");
	printf("[%s]: Got %d files\n", __func__, numfiles);

	for (i = 0; i < numfiles; i++) {
		printf("[%s]: (%d). %s (%p)\n", __func__, i, filenames[i], &filenames[i]);
	}
}

int bitmap_draw(const char * bitmap) {
	int result;
	int imagefilesize;
	int bitmap_data_offset;
	int width;
	int height;
	int i;

	int x = 0;
	int y = 0;
	unsigned char r;
	unsigned char g;
	unsigned char b;

	result = bitmap_process_header(bitmap, &imagefilesize, &bitmap_data_offset, &width, &height);
	if (result != 0) {
		return result;
	}

	printf("Image width: %d\nImage height: %d\n", width, height);
	graphics_clear_screen();

	i = bitmap_data_offset;
	/* Gotta draw the picture backwards to make it display upright on the VGA */
	for (y = height + BASE; y > BASE; y--) {
		for (x = width + BASE; x > BASE; x--) {
			b = 0x000000FF & (bitmap[i]);
			i++;
			g = 0x000000FF & (bitmap[i]);
			i++;
			r = 0x000000FF & (bitmap[i]);
			i++;

			graphics_write_pixel(x, y, rgb_to_8bit(r, g, b));
		}
	}

	return 0;
}

int bitmap_display() {
	char * bitmap;
	int imagefilesize;
	int bitmap_data_offset;
	int result;
	unsigned int imageindex = 0;
	Pixel pixel;

	touchscreen_init();

	bitmap_get_bitmap_filenames_from_sdcard();

	while (1) {
		touchscreen_get_press(&pixel, 3000);
		if (pixel.x > 400)
			imageindex++;
		else
			imageindex--;

		/* Make sure our index is always valid */
		imageindex = imageindex % numfiles;

		result = bitmap_import_image(filenames[imageindex], &bitmap, &imagefilesize, &bitmap_data_offset);
		if (result != 0) {
			printf("Error: Could not import image from SD card\n");
			imageindex++;
			continue;
		}

		result = bitmap_draw(bitmap);

		free(bitmap);
	}
	return 0;
}




