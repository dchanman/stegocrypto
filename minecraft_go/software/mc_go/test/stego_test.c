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
static int stego_engine_import_image(const char *filename, char **data_out, int * length, int * data_start_offset);
static int stego_engine_export_image(const char * filename, const char * data, const int length);

static int stego_engine_import_image(const char * filename, char ** data_out, int * length, int * data_start_offset) {
	char header[STEGO_ENGINE_BMP_HEADER_SIZE];
	char * filedata;
	short int fh;
	int i;
	int read;

	sdcard_init();

	if (sdcard_open(&fh, filename)) {
		printf("Could not open\n");
		return -1;
	}

	if (sdcard_read(fh, header, sizeof(header)) < 0) {
		printf("Could not read\n");
		return -1;
	}

	for (i = 0; i < 14; i++) {
		printf("%02X ", header[i] & 0x00FF);
	}

	stego_engine_process_header(header, length, data_start_offset);

	printf("Filesize: %d bytes\n", *length);

	filedata = malloc((*length)*sizeof(char));
	if (filedata == NULL) {
		printf("Not enough memory to import file data");
		return -1;
	}
	memset(filedata, '\0', (*length)*sizeof(char));

	printf("Sufficient memory available\n");

	/* Copy in the whole file */
	sdcard_close(fh);
	if (sdcard_open(&fh, filename)) {
		printf("Could not open\n");
		return -1;
	}

	read = sdcard_read(fh, filedata, *length);
	if (read < (*length)) {
		printf("Could not read entire file, read <%d> of length <%d>\n", read, *length);
		goto err;
	}

	*data_out = filedata;

	sdcard_close(fh);

	return 0;

err:
	sdcard_close(fh);
	return -1;
}

static int stego_engine_export_image(const char * filename, const char * data, const int length) {
	short int fh;
	int wrote;

	if (sdcard_open(&fh, filename) != 0) {
		printf("Could not open\n");
		return -1;
	}

	wrote = sdcard_write(fh, data, length);
	if (wrote != length) {
		printf("Did not write full .bmp file: wrote <%d> of <%d> bytes\n", wrote, length);
		return -1;
	}

	if (sdcard_close(fh) != 0) {
		printf("Could not close SDCard file\n");
		return -1;
	}

	return 0;
}

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

	result = stego_engine_import_image(filename, &imagedata, &imagelength, &offset);
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
	stego_engine_export_image(output_filename, imagedata, imagelength);

	printf("Done exporting, let's try opening it again\n");
	free(imagedata);
	result = stego_engine_import_image(output_filename, &imagedata, &imagelength, &offset);
	if (result != 0) {
		printf("Error with importing image\n");
		return result;
	}

	free(imagedata);
	free(extracted);
	return result;
}
