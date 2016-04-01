/*
 * stegocrypto_test.c
 *
 *  Created on: Mar 31, 2016
 *      Author: derek
 */

#include <stdio.h>
#include <string.h>
#include "sdcard.h"
#include "bitmap.h"
#include "stegocrypto.h"
#include "rgb.h"

void stegocrypto_test_embed() {
	char * imagedata;
	int imagedata_length;
	int offset;

	const char * msg = "hello this is my secret\n";
	char * extracted;
	int extracted_length;

	/* Initialize everything */
	geoencryption_engine_init();
	sdcard_init();
	rgb_init();

	printf("[%s] Importing image...\n", __func__);

	bitmap_import_image("test.bmp", &imagedata, &imagedata_length, &offset);
	bitmap_draw_centered_fullscreen(imagedata);

	printf("[%s] Embedding data... <%s>\n", __func__);

	/* Embed */
	stegocrypto_engine_embed(imagedata, msg, strlen(msg)+1, -123.251, 49.261);
	bitmap_draw_centered_fullscreen(imagedata);

	printf("[%s] Extracting data...", __func__);

	/* Extract */
	stegocrypto_engine_extract(imagedata, &extracted, &extracted_length);
	bitmap_draw_centered_fullscreen(imagedata);

	printf("[%s] Extracted <%s>\n", __func__, extracted);

	if (strcmp(extracted, msg) == 0)
		printf("[%s] SUCCESS!\n", __func__);
	else
		printf("[%s] FAILURE!\n", __func__);

	printf("[%s] Saving data...\n", __func__);
	bitmap_export_image("output.bmp", imagedata, imagedata_length);

	free(imagedata);
}
