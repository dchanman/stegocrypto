/*
 * stegocrypto_engine.c
 *
 *  Created on: Mar 31, 2016
 *      Author: derek
 */

#include <stdlib.h>
#include "stego_engine.h"
#include "geoencryption_engine.h"

//#define STEGOCRYPTO_DEBUG
#ifdef STEGOCRYPTO_DEBUG
static void cryptodump(const char * encrypted, const int encrypted_length) {
	int i;

	printf("[%s] Cryptodump:\n", __func__);
	for (i = 0; i < encrypted_length; i++) {
		printf("%c", encrypted[i]);
	}
	printf("\n");
}
#endif

void stegocrypto_engine_init() {
	geoencryption_engine_init();
}

void stegocrypto_engine_embed(char * imagedata, const char * data, const int datalength, const float longitude_key, const float latitude_key) {
	char * encrypted_data;
	int encrypted_data_length;

	/* First encrypt our data */
	geoencryption_engine_encrypt(data, datalength, &encrypted_data, &encrypted_data_length, longitude_key, latitude_key);

	/* Now embed it */
	stego_engine_embed(imagedata, encrypted_data, encrypted_data_length);

#ifdef STEGOCRYPTO_DEBUG
	cryptodump(data, datalength);
	cryptodump(encrypted_data, encrypted_data_length);
#endif

	free(encrypted_data);
}

void stegocrypto_engine_extract(const char * imagedata, char ** decrypted, int * decrypted_length, char * longitude_key_out, char * latitude_key_out) {
	char * encrypted_data;
	int encrypted_data_length;

	/* First extract our data */
	stego_engine_extract(imagedata, &encrypted_data, &encrypted_data_length);

	/* Now decrypt it */
	geoencryption_engine_decrypt(encrypted_data, encrypted_data_length, decrypted, decrypted_length, longitude_key_out, latitude_key_out);

	free(encrypted_data);
}
