/*
 * bitmap.c
 *
 *  Created on: Mar 17, 2016
 *      Author: derek
 */

#include <stdlib.h>
#include <stdio.h>
#include "bitmap.h"

int bitmap_import_image(const char * filename, char ** data_out, int * length, int * data_start_offset) {
	char header[STEGO_ENGINE_BMP_HEADER_SIZE];
	char * filedata;
	short int fh;
	int i;
	int read;
	int h;
	int w;

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

	bitmap_process_header(header, length, data_start_offset, &w, &h);

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

int bitmap_export_image(const char * filename, const char * data, const int length) {
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

int bitmap_process_header(const char * header, int * imagefilesize, int * data_start_offset, int * width, int * height) {

	/* Check the magic numbers at the start of our file to make sure this is a .bmp file */
	if (header[0] != 0x42 || header[1] != 0x4D) {
		printf("Error: Not a .bmp file\n");
		return -1;
	}

	/*
	 * The bitmap size if stored as
	 * 	high bits:	bytes 2, 3
	 * 	low bits:	bytes 4, 5
	 */
	/* Low bits */
	*imagefilesize = 0x0000FFFF & ((header[3] << 8) + (header[2]));
	/* High bits */
	*imagefilesize += 0xFFFF0000 & ((header[5] << 24) + (header[4] << 16));

	/* Process the header to find the offset where the bitmap data resides */
	/*
	 * The bitmap data offset is stored as:
	 * 	high bits:	bytes 10, 11
	 * 	low bits:	bytes 12, 13
	 */
	/* Low bits */
	*data_start_offset = 0x0000FFFF & ((header[11] << 8) + (header[10]));
	/* High bits */
	*data_start_offset += 0xFFFF0000 & ((header[13] << 24) + (header[12] << 16));

	/* Process the header to find the width and height */
	/*
	 * The bitmap data offset is stored as:
	 * 	width bits:		bytes 18-21
	 * 	height bits:	bytes 22-25
	 */
	*width = 0x0000FFFF & ((header[19] << 8) + (header[18]));
	*width += 0xFFFF0000 & ((header[21] << 24) + (header[20] << 16));
	*height = 0x0000FFFF & ((header[23] << 8) + (header[22]));
	*height += 0xFFFF0000 & ((header[25] << 24) + (header[24] << 16));

	return 0;
}

