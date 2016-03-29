/*
 * bitmap_display.c
 *
 *  Created on: Mar 17, 2016
 *      Author: derek
 */

#include <stdlib.h>
#include <stdio.h>
#include "bitmap.h"
#include "sdcard.h"
#include "touchscreen.h"

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
		/* Hacky: By default, make the image index increment */
		printf("[%s] Waiting for timer or touch...\n", __func__);
		//pixel.x = 401;
		touchscreen_get_press(&pixel, 3000);
		imageindex++; /* TODO: fix the touchscreen... */
		//if (pixel.x > 400)
		//	imageindex++;
		//else
		//	imageindex--;

		/* Make sure our index is always valid */
		imageindex = imageindex % numfiles;

		printf("[%s] Drawing (%d) <%s>\n", __func__, imageindex, filenames[imageindex]);

		bitmap = NULL;
		result = bitmap_import_image(filenames[imageindex], &bitmap, &imagefilesize, &bitmap_data_offset);
		if (result != 0) {
			printf("Error: Could not import image from SD card\n");
			continue;
		}

		result = bitmap_draw_centered_fullscreen(bitmap);
		//bitmap_draw(bitmap);

		if (bitmap != NULL)
			free(bitmap);

	}
	return 0;
}




