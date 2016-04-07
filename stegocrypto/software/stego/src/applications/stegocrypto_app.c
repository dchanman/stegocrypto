/*
 * stegocrypto_app.c
 *
 *  Created on: Apr 7, 2016
 *      Author: derek
 */

#include <stdio.h>
#include <stdlib.h>
#include "general.h"
#include "data_transfer.h"
#include "stegocrypto.h"
#include "bitmap.h"
#include "sdcard.h"

static void stegocrypto_app_debug_dump_string(const unsigned char * data, const int length) {
	char debug[64];
	int debug_len;

	memset(debug, '\0', sizeof(debug));
	debug_len = (
	sizeof(debug) < length + 1 ? sizeof(debug) : length + 1);
	snprintf(debug, debug_len, "%s", data);
	printf("[%s] Received data: %s\n", __FILE__, debug);
}

static void stegocrypto_app_encrypt() {
	unsigned char * data;
		int data_length;
		unsigned char * image;
		int image_length;
		unsigned char * longitude;
		int longitude_length;
		float longf;
		unsigned char * latitude;
		float latf;
		int latitude_length;
		boolean result;

		printf("[%s]\n", __func__);

		/* Get data */
		result = data_transfer_receive(&data, &data_length);
		if (!result) {
			return;
		}
		stegocrypto_app_debug_dump_string(data, data_length);

		/* Get longitude */
		result = data_transfer_receive(&longitude, &longitude_length);
		if (!result) {
			free(data);
			return;
		}
		stegocrypto_app_debug_dump_string(longitude, longitude_length);
		longf = atof(longitude);

		/* Get latitude */
		result = data_transfer_receive(&latitude, &latitude_length);
		if (!result) {
			free(data);
			free(longitude);
			return;
		}
		stegocrypto_app_debug_dump_string(latitude, latitude_length);
		latf = atof(latitude);


		/* Get image */
		result = data_transfer_receive(&image, &image_length);
		if (!result) {
			printf("[%s]: Timed out while receiving image file\n", __func__);
			free(data);
			free(longitude);
			free(latitude);
			return;
		}
		printf("[%s]: Done receiving image (size %d)\n", __func__, image_length);

		/* Save it */
		printf("[%s] Received image data, exporting...\n", __func__);
		bitmap_export_image("raw.bmp", image, image_length);

		/* Encrypt */
		printf("[%s] Encrypting with long <%0.4f> latf <%0.4f>...\n", __func__, longf, latf);
		stegocrypto_engine_embed(image, data, data_length, longf, latf);

		/* Send it back */
		printf("[%s] Sending back embedded data...\n", __func__);
		result = data_transfer_send(image, image_length);

		/* Save it */
		printf("[%s] Received image data, exporting...\n", __func__);
		bitmap_export_image("enc.bmp", image, image_length);

		/* Draw it */
		printf("[%s] Drawing...\n", __func__);
		bitmap_draw_centered_fullscreen(image);

		/* Clean up */
		printf("[%s]: Cleaning up...\n", __func__);
		free(data);
		free(image);
		free(longitude);
		free(latitude);

		printf("[%s]: Done!\n", __func__);
}


static void stegocrypto_app_decrypt() {
		unsigned char * image;
		int image_length;
		char * extracted;
		int extracted_length;
		boolean result;

		printf("[%s]\n", __func__);

		/* Get image */
		result = data_transfer_receive(&image, &image_length);
		if (!result) {
			printf("[%s]: Timed out while receiving image file\n", __func__);
			return;
		}
		printf("[%s]: Done receiving image (size %d)\n", __func__, image_length);

		/* Save it */
		printf("[%s] Received image data, exporting...\n", __func__);
		bitmap_export_image("raw.bmp", image, image_length);

		/* Decrypt */
		printf("[%s] Decrypting...\n", __func__);
		stegocrypto_engine_extract(image, &extracted, &extracted_length);
		stegocrypto_app_debug_dump_string(extracted, extracted_length);

		/* Send it back */
		printf("[%s] Sending back embedded data...\n", __func__);
		result = data_transfer_send(extracted, extracted_length);

		/* Draw it */
		printf("[%s] Drawing...\n", __func__);
		bitmap_draw_centered_fullscreen(image);

		/* Clean up */
		printf("[%s]: Cleaning up...\n", __func__);
		free(image);
		free(extracted);

		printf("[%s]: Done!\n", __func__);
}

void stegocrypto_app_main() {
	unsigned char opt;
	boolean result;

	/* Initialize everything */
	data_transfer_init();
	stegocrypto_engine_init();
	sdcard_init();

	printf("StegoCrypto App\n");

	while (1) {
		result = bluetooth_get_char_timeout(&opt, 3000);
		if (!result)
			continue;

		if (opt == 'E') {
			stegocrypto_app_encrypt();
		} else if (opt == 'D'){
			stegocrypto_app_decrypt();
		} else {
			printf("[%s] Received unknown opt <%c>\n", __func__, opt);
		}
	}
}
