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

static const char * filenames[] = {
		"blackbox.bmp",
		"bluebox.bmp",
		"greenbox.bmp",
		"whitebox.bmp",
		"redbox.bmp",
		"fancy.bmp"
};

int bitmap_display() {
	char * bitmap;
	int bitmap_data_offset;
	int imagefilesize;
	int width;
	int height;
	int i;
	int result;

	int x = 0;
	int y = 0;

	result = bitmap_import_image(filenames[0], &bitmap, &imagefilesize, &bitmap_data_offset);
	if (result != 0) {
		printf("Error: Could not import image from SD card\n");
		return -1;
	}

	result = bitmap_process_header(bitmap, &imagefilesize, &bitmap_data_offset, &width, &height);
	if (result != 0) {
		return -1;
	}

	printf("Image width: %d\nImage height: %d\n", width, height);




	free(bitmap);
	return 0;
}




