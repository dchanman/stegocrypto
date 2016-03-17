#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "stego_engine.h"

/* Test functions for importing and exporting bitmap files */
static int stego_engine_import_image(const char *filename, char **data_out, int * length, int * data_start_offset);
static int stego_engine_export_image(const char * filename, const char * data, const int length);

static int stego_engine_import_image(const char * filename, char ** data_out, int * length, int * data_start_offset) {
	char header[STEGO_ENGINE_BMP_HEADER_SIZE];
	char * filedata;
	FILE * fh;
	int i;

	//if (sdcard_open(&fh, filename)) {
	//	printf("Could not open\n");
	//	return;
	//}
	fh = fopen(filename, "rw");
	if (fh == NULL) {
		printf("Could not open\n");
		return -1;
	}

	//if (sdcard_read(fh, header, sizeof(header)) < 0) {
	if (fread(header, sizeof(char), sizeof(header), fh) < 0) {
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
	if (fseek(fh, 0, SEEK_SET) != 0) {
		free(filedata);
		goto err;
	}

	if (fread(filedata, sizeof(char), *length, fh) < (*length)) {
		printf("Could not read entire file\n");
		goto err;
	}

	*data_out = filedata;

	//sdcard_close(fh);
	fclose(fh);
	return 0;

err:
	fclose(fh);
	return -1;
}

static int stego_engine_export_image(const char * filename, const char * data, const int length) {
	FILE * fh;

	fh = fopen(filename, "w");
	if (fh == NULL) {
		printf("Could not open\n");
		return -1;
	}

	fwrite(data, sizeof(char), length, fh);

	fclose(fh);
	return 0;
}

static void hexdump(const char * data, int length) {
	for (int i = 0; i < length; i++) {
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

	printf("Exporting...\n");
	stego_engine_export_image(output_filename, imagedata, imagelength);

	if (strcmp(message, extracted) == 0)
		printf("Test passed!\n");
	else
		printf("Test FAILED. Expected:\n%s\n\nActual:\n%s\n", message, extracted);

	printf("\nDone.\n");

	free(imagedata);
	free(extracted);
	return result;
}

int main(int argc, char **argv) {
	if (argc != 3) {
		printf("Usage: %s bmp_filename output_filename\n", argv[0]);
		return 1;
	}
	
	return stego_engine_test(argv[1], argv[2]);
}
