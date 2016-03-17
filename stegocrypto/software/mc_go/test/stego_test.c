/*
 * stego_test.c
 *
 *  Created on: Mar 16, 2016
 *      Author: derek
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sdcard.h"
#include "stego_engine.h"

/* Test functions for importing and exporting bitmap files */

static void hexdump(const char * data, int length) {
	int i;

	for (i = 0; i < length; i++) {
		if (i % 16 == 0)
			printf("\n");

		printf("%02X ", 0x00FF & data[i]);
	}
	printf("\n");
}

int stego_engine_test(const char * filename, const char * output_filename) {
	char * imagedata = NULL;
	int imagelength;
	int offset;
	char * extracted;
	int extracted_length;
	int result;

	result = bitmap_import_image(filename, &imagedata, &imagelength, &offset);
	if (result != 0) {
		printf("Error with importing image\n");
		return result;
	}

	printf("filesize: <%d> | data offset: <%02X>\n", imagelength, offset);

	printf("Dumping bytes until data starts:\n");
	hexdump(imagedata, offset);

	printf("Dumping first 128 bytes of RGB:\n");
	hexdump(imagedata + offset, 128);

	printf("Embedding data...\n");
	const char message[] = "Hello this is my secret message, i bet you can't see it\n";
	stego_engine_embed(imagedata, message, sizeof(message));

	printf("Dumping first 128 bytes of RGB:\n");
	hexdump(imagedata + offset, 128);

	printf("Extracting data...\n");
	stego_engine_extract(imagedata, &extracted, &extracted_length);

	printf("Extracted: (%d)\n%s\n", extracted_length, extracted);

	if (strcmp(message, extracted) == 0)
		printf("Test passed!\n");
	else
		printf("Test FAILED. Expected:\n%s\n\nActual:\n%s\n", message, extracted);

	printf("Exporting...\n");
	bitmap_export_image(output_filename, imagedata, imagelength);

	printf("Done exporting, let's try opening it again\n");
	free(imagedata);
	result = bitmap_import_image(output_filename, &imagedata, &imagelength, &offset);
	if (result != 0) {
		printf("Error with importing image\n");
		return result;
	}

	free(imagedata);
	free(extracted);
	return result;
}
