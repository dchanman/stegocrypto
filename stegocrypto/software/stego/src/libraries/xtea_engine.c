/**
 * XTEA: eXtended Tiny Encryption Algorithm
 * Reference: https://en.wikipedia.org/wiki/XTEA
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "xtea_engine.h"

#define DELTA 0x9e3779b9
#define NUM_ROUNDS	32

static int xtea_engine_char_array_to_xtea_array(const char * chars, const int char_length, unsigned int ** uint32s, int * uint32_length);
static int xtea_engine_xtea_array_to_char_array(const unsigned int * uint32ts, const int uint32_length, char ** chars, int * char_length);

void xtea_engine_encipher(unsigned int data[2], const unsigned int key[4]) {
	unsigned int d0 = data[0];
	unsigned int d1 = data[1];
	unsigned int sum = 0;

	int i;
	for (i = 0; i < NUM_ROUNDS; i++) {
		d0 += (((d1 << 4) ^ (d1 >> 5)) + d1) ^ (sum + key[sum & 3]);
        sum += DELTA;
        d1 += (((d0 << 4) ^ (d0 >> 5)) + d0) ^ (sum + key[(sum >> 11) & 3]);
	}

	data[0] = d0;
	data[1] = d1;
}

void xtea_engine_decipher(unsigned int data[2], const unsigned int key[4]) {
	unsigned int d0 = data[0];
	unsigned int d1 = data[1];
	unsigned int sum = DELTA * NUM_ROUNDS;

	int i;
	for (i = 0; i < NUM_ROUNDS; i++) {
		d1 -= (((d0 << 4) ^ (d0 >> 5)) + d0) ^ (sum + key[(sum >> 11) & 3]);
        sum -= DELTA;
        d0 -= (((d1 << 4) ^ (d1 >> 5)) + d1) ^ (sum + key[sum & 3]);
	}

	data[0] = d0;
	data[1] = d1;
}

void xtea_engine_encipher_str(const char * data, const int length, char ** output, int * output_length, const unsigned int key[4]) {
	int i;
	unsigned int * temp;
	int templength;

	xtea_engine_char_array_to_xtea_array(data, length, &temp, &templength);

	/* Encipher */
	for (i = 0; i < templength; i += 2)
		xtea_engine_encipher(&temp[i], key);

	xtea_engine_xtea_array_to_char_array(temp, templength, output, output_length);

	free(temp);
}

void xtea_engine_decipher_str(const char * data, const int length, char ** output, int * output_length, const unsigned int key[4]) {
	int i;
	unsigned int * temp;
	int templength;

	xtea_engine_char_array_to_xtea_array(data, length, &temp, &templength);

	/* Decipher */
	for (i = 0; i < templength; i+= 2)
		xtea_engine_decipher(&temp[i], key);

	xtea_engine_xtea_array_to_char_array(temp, templength, output, output_length);
	
	free(temp);
}

static int xtea_engine_char_array_to_xtea_array(const char * chars, const int char_length, unsigned int ** uint32s, int * uint32_length) {
	char * padded_char_buffer;
	
	/**
	 * Our XTEA array needs block sizes of 64 bits 
	 * 64 bits  = 8 chars
	 */
	int new_char_length = char_length + (8 - char_length % 8);

	padded_char_buffer = malloc(sizeof(char) * new_char_length);
	if (padded_char_buffer == NULL) {
		printf("[%s] Out of memory!\n", __func__);
		return 1;		
	}
	memset(padded_char_buffer, '\0', new_char_length);

	/* Copy chars into our padded buffer. Remaining space will be filled with \0 */
	memcpy(padded_char_buffer, chars, sizeof(char) * char_length);

	/* HACKY: cast our padded buffer into a unsigned int buffer. The sizes should match because of math */
	*uint32s = (unsigned int *)padded_char_buffer;
	*uint32_length = (new_char_length / 4);

	return 0;
}

static int xtea_engine_xtea_array_to_char_array(const unsigned int * uint32ts, const int uint32_length, char ** chars, int * char_length) {
	char * output;
	int output_length;
	
	/* There are 4 chars in a unsigned int */
	output_length = uint32_length * 4;
	output = malloc(sizeof(char) * output_length);
	if (output == NULL) {
		printf("[%s] Out of memory!\n", __func__);
		return 1;
	}
	
	memcpy(output, uint32ts, sizeof(char) * output_length);

	*chars = output;
	*char_length = output_length;

	return 0;
}
