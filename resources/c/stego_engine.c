#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "stego_engine.h"

/* Uncomment one of these to select between compiling for HW or compiling for SW implementation */
/* Hardware option */
//#define STEGO_ENGINE_HARDWARE_ACCELERATION
/* Software option */
#undef STEGO_ENGINE_HARDWARE_ACCELERATION

/* Software vs Hardware implemented functions */
void stego_engine_embed_hw(char * imagedata, const char * data, const int datalength);
void stego_engine_extract_hw(const char * imagedata, char ** data, int * datalength);
void stego_engine_embed_sw(char * imagedata, const char * data, const int datalength);
void stego_engine_extract_sw(const char * imagedata, char ** data, int * datalength);

void stego_engine_embed(char * imagedata, const char * data, const int datalength) {
#ifdef STEGO_ENGINE_HARDWARE_ACCELERATION
	stego_engine_embed_hw(imagedata, data, datalength);
#else
	stego_engine_embed_sw(imagedata, data, datalength);
#endif
}

void stego_engine_extract(const char * imagedata, char ** data, int * datalength) {
#ifdef STEGO_ENGINE_HARDWARE_ACCELERATION
	stego_engine_extract_hw(imagedata, data, datalength);
#else
	stego_engine_extract_sw(imagedata, data, datalength);
#endif
}

/* TODO: Refactor me into a bitmap library */
void stego_engine_process_header(const char * header, int * imagefilesize, int * data_start_offset) {

	/* Check the magic numbers at the start of our file to make sure this is a .bmp file */
	if (header[0] != 0x42 || header[1] != 0x4D) {
		printf("Error: Not a .bmp file\n");
		return;
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
}

void stego_engine_embed_sw(char * imagedata, const char * data, const int datalength) {
	int i;
	int k;
	int imageoffset;
	int imagelength;
	char byte;
	char bit;
	int bitcount;


	//ImageData points to address space. In hardware we should increment the address until image_offset = 0 
	//then read data from that address space. 

	if (imagedata == NULL || data == NULL) {
		printf("NULL pointer\n");
		return;
	}

	/* Process the header */
	stego_engine_process_header(imagedata, &imagelength, &imageoffset);

	/* Make sure out bitmap file has enough space to hold our data */
	if (datalength * 8 > (imagelength - imageoffset)) {
		printf("Not enough space in the .bmp! Aborting...\n");
		return;
	}

	/* We will be using i to track the offset of the image data we are writing to */
	i = imageoffset;

	/* First, write the 32-bit datalength */
	byte = datalength;
	for (bitcount = 0; bitcount < 32; bitcount++) {
		bit = 0x0001 & byte;

		/* If we have a 0 bit, make the byte in the image an even value */
		if (bit == 1)
			imagedata[i] |= 0x01;
		/* If we have a 1 bit, make the byte in the image an odd value */
		else
			imagedata[i] &= 0xFE;

		/* Increment the imagedata index */
		i++;

		/* Bitshift the byte to prepare the next bit */
		byte = byte >> 1;
	}

	/* Now write the data into the image */
	for (k = 0; k < datalength; k++) {
		byte = data[k];

		/* We store 8 bits of data inside 8 bytes of RGB */
		for (bitcount = 0; bitcount < 8; bitcount++) {
			bit = 0x0001 & byte;

			/* If we have a 0 bit, make the byte in the image an even value */
			if (bit == 1)
				imagedata[i] |= 0x01;
			/* If we have a 1 bit, make the byte in the image an odd value */
			else
				imagedata[i] &= 0xFE;

			/* Increment the imagedata index */
			i++;

			/* Bitshift the byte to prepare the next bit */
			byte = byte >> 1;
		}
	}
}

void stego_engine_extract_sw(const char * imagedata, char ** data, int * datalength) {
	int i;
	int j;
	int k;
	int offset;
	char byte;
	int imagelength; /* unused, just a placeholder in header function */

	if (imagedata == NULL || data == NULL || datalength == NULL) {
		printf("NULL pointer\n");
		return;
	}

	stego_engine_process_header(imagedata, &imagelength, &offset);

	i = offset;

	/* Process the length of our encoded data */
	*datalength = 0;
	for (k = 0; k < 32; k++) {
		*datalength += (imagedata[i] << k);
		i++;
	}

	/* Create a buffer to hold the data we're about to extract */
	*data = malloc((*datalength)*sizeof(char));
	if (*data == NULL) {
		printf("Out of memory\n");
		return;
	}

	/* Now extract the data */
	for (j = 0; j < *datalength; j++) {
		byte = 0;

		/* 8 bits of data is stored across 8 bytes of RGB */
		for (k = 0; k < 8; k++) {
			byte += (imagedata[i] << k);
			i++;
		}

		(*data)[j] = byte;
	}
}

void stego_engine_embed_hw(char * imagedata, const char * data, const int datalength) {
	/* JAY DO YOUR STUFF HERR */
}

void stego_engine_extract_hw(const char * imagedata, char ** data, int * datalength) {
	/* SUCH COLLAB. WOW */
}
