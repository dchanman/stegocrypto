/*
 * geoencryption_engine.c
 *
 *  Created on: Mar 29, 2016
 *      Author: derek
 */

#include <stdio.h>
#include <math.h>
#include "general.h"
#include "gps.h"
#include "xtea_engine.h"
#include "geoencryption_engine.h"

void geoencryption_engine_init() {
	gps_init();
}

void geoencryption_engine_encrypt(const char * data, const int data_length, char ** encrypted, int * encrypted_length, float longitude, float latitude) {
	unsigned int key[4];

	key[0] = (unsigned int)longitude;
	key[1] = (unsigned int) ((longitude - (int)longitude) * 100);
	key[2] = (unsigned int)latitude;
	key[3] = (unsigned int) ((latitude - (int)latitude) * 100);

	/* Sketchy. abs takes a signed integer, but it works with an unsigned one too lol... */
	/* We need to remove the negative sign from the long/lat decimals */
	key[1] = abs(key[1]);
	key[3] = abs(key[3]);

	//printf("[%s] Key: %u %u %u %u\n", __func__, key[0], key[1], key[2], key[3]);

	xtea_engine_encipher_str(data, data_length, encrypted, encrypted_length, key);
}

void geoencryption_engine_decrypt(const char * data, const int data_length, char ** decrypted, int * decrypted_length) {
	RMC_data rmc_data;
	Location location;
	int long_deg;
	int lat_deg;
	int long_deg_decimal;
	int lat_deg_decimal;
	unsigned int key[4];

	/* Retrieve our geo key via the GPS */
	gps_get_current_rmc_data(&rmc_data, 10);
	gps_convert_rmc_to_location(&rmc_data, &location);

	/**
	 *  Convert our data to be consistent with what Android sends us:
	 *
	 *  We won't be using minutes. Rather, we'll be using degree-decimals. The conversion is a divide by 60.
	 *  We also want 2-decimal precision. We get 2-decimal precision by multiplying by 100 and truncating to an integer
	 *
	 *  We also have to remember our signs:
	 *  	Longitude
	 *  		West: negative
	 *  		East: positive
	 *  	Latitude
	 *  		South: negative
	 *  		North: positive
	 */
	long_deg = location.long_degree * (location.long_direction = 'W' ? -1 : 1);
	lat_deg = location.lat_degree* (location.long_direction = 'N' ? 1 : -1);
	long_deg_decimal = location.long_minute / 60 * 100;
	lat_deg_decimal = location.lat_minute / 60 * 100;

	key[0] = (unsigned int)long_deg;
	key[1] = (unsigned int)long_deg_decimal;
	key[2] = (unsigned int)lat_deg;
	key[3] = (unsigned int)lat_deg_decimal;

	//printf("[%s] Key: %u %u %u %u\n", __func__, key[0], key[1], key[2], key[3]);

	/* Decrypt our data */
	xtea_engine_decipher_str(data, data_length, decrypted, decrypted_length, key);
}
