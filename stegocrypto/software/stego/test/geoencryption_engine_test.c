/*
 * geoencryption_engine_test.c
 *
 *  Created on: Mar 29, 2016
 *      Author: derek
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "geoencryption_engine.h"

void geoencryption_engine_test() {
	const char * data = "hello";
	char * encrypted;
	int encrypted_length;
	char * decrypted;
	int decrypted_length;

	geoencryption_engine_init();

	printf("[%s] Encrypting with correct encryption key...\n", __func__);
	/* Note the extra decimal points required for floating point precision. */
	geoencryption_engine_encrypt(data, strlen(data) + 1, &encrypted, &encrypted_length, -123.251, 49.261);

	printf("[%s] Decrypting...\n", __func__);
	geoencryption_engine_decrypt(encrypted, encrypted_length, &decrypted, &decrypted_length, NULL, NULL);

	if (strncmp(data, decrypted, strlen(data)) != 0)
		printf("[%s] FAILED\n", __func__);
	else
		printf("[%s] PASSED\n", __func__);

	printf("[%s] Done. Message:\n%s\n", __func__, decrypted);

	free(encrypted);
	free(decrypted);

	printf("[%s] Encrypting with incorrect encryption key...\n", __func__);
	/* Note the extra decimal points required for floating point precision. */
	geoencryption_engine_encrypt(data, strlen(data) + 1, &encrypted, &encrypted_length, -123.261, 49.261);

	printf("[%s] Decrypting...\n", __func__);
	geoencryption_engine_decrypt(encrypted, encrypted_length, &decrypted, &decrypted_length, NULL, NULL);

	if (strncmp(data, decrypted, strlen(data)) == 0)
		printf("[%s] FAILED\n", __func__);
	else
		printf("[%s] PASSED\n", __func__);

	free(encrypted);
	free(decrypted);

	printf("[%s] Done\n", __func__);
}
