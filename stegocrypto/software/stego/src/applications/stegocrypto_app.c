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

void stegocrypto_app_main() {
	unsigned char * data;
	int data_length;
	unsigned char * image;
	int image_length;
	unsigned char * longitude;
	int longitude_length;
	unsigned char * latitiude;
	int latitiude_length;
	boolean result;

	char debug[64];
	int debug_len;

	/* Initialize everything */
	data_transfer_init();
	stegocrypto_engine_init();
	sdcard_init();

	printf("StegoCrypto App\n");

	while (1) {
		/* Get data */
		result = data_transfer_receive(&data, &data_length);
		if (!result) {
			continue;
		}
		memset(debug, '\0', sizeof(debug));
		debug_len = (sizeof(debug) < data_length + 1 ? sizeof(debug) : data_length + 1);
		snprintf(debug, debug_len, "%s", data);
		printf("[%s] Received data: %s\n", __func__, debug);


		/* Get image */
		result = data_transfer_receive(&image, &image_length);
		if (!result) {
			printf("[%s]: Timed out while receiving image file\n", __func__);
			free(data);
			continue;
		}
		printf("[%s]: Done receiving image (size %d)\n", __func__, image_length);

		/* Echo the data */
		result = data_transfer_send(data, data_length);
		if (!result) {
			printf("[%s]: Timed out echoing data\n", __func__);
		}

		/* Save it */
		printf("[%s] Received image data, exporting...\n", __func__);
		bitmap_export_image("recent.bmp", image, image_length);

		/* Draw it */
		printf("[%s] Drawing...\n", __func__);
		bitmap_draw_centered_fullscreen(image);


//		/* Get longitude */
//		result = data_transfer_receive(&longitude, &longitude_length);
//		if (!result) {
//			free(data);
//			free(image);
//			continue;
//		}
//
//		/* Get latitude */
//		result = data_transfer_receive(&latitiude, &latitiude_length);
//		if (!result) {
//			free(data);
//			free(image);
//			free(longitude);
//			continue;
//		}

		/* Encrypt */


		/* Clean up */
		printf("[%s]: Cleaning up...\n", __func__);
		free(data);
		free(image);
		free(longitude);
		free(latitiude);

		printf("[%s]: Done!\n", __func__);


	}
}
