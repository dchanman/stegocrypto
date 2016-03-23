/**
 * XTEA: eXtended Tiny Encryption Algorithm
 * Reference: https://en.wikipedia.org/wiki/XTEA
*/

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
//#include "xtea_engine.h"

#define KEY_SCHEDULE_CONSTRAINT 0x9e3779b9
#define NUM_ROUNDS	32

static void xtea_engine_encipher(uint32_t data[2], uint32_t const key[4]);
static void xtea_engine_decipher(uint32_t data[2], uint32_t const key[4]);

static void xtea_engine_encipher(uint32_t data[2], uint32_t const key[4]) {
	uint32_t d0 = data[0];
	uint32_t d1 = data[1];
	uint32_t delta = KEY_SCHEDULE_CONSTRAINT;
	uint32_t sum = 0;

	int i;
	for (i = 0; i < NUM_ROUNDS; i++) {
		d0 += (((d1 << 4) ^ (d1 >> 5)) + d1) ^ (sum + key[sum & 3]);
        sum += delta;
        d1 += (((d0 << 4) ^ (d0 >> 5)) + d0) ^ (sum + key[(sum >> 11) & 3]);
	}

	data[0] = d0;
	data[1] = d1;
}

static void xtea_engine_decipher(uint32_t data[2], uint32_t const key[4]) {
	uint32_t d0 = data[0];
	uint32_t d1 = data[1];
	uint32_t delta = KEY_SCHEDULE_CONSTRAINT;
	uint32_t sum = delta * NUM_ROUNDS;

	int i;
	for (i = 0; i < NUM_ROUNDS; i++) {
		d1 -= (((d0 << 4) ^ (d0 >> 5)) + d0) ^ (sum + key[(sum >> 11) & 3]);
        sum -= delta;
        d0 -= (((d1 << 4) ^ (d1 >> 5)) + d1) ^ (sum + key[sum & 3]);
	}

	data[0] = d0;
	data[1] = d1;
}

static void xtea_engine_encipher_str(char * data, int length, uint32_t const key[4]) {
	int i;

	/* We have to convert our char * into a uint32_t * */
	int data_num_bits = sizeof(char) * length;
	printf("length: %d\n", length);
	printf("data_num_bits: %d\n", data_num_bits);

	/* xtea's block size is 64 bits (2 * 32-bit uints) */
	int num_blocks = 1 + (data_num_bits / 64);
	printf("num_blocks: %d\n", num_blocks);

	/* We need an even number of blocks */
	if (num_blocks % 2 == 0)
		num_blocks++;

	/* Create a buffer to hold our data */
	char * buf = calloc(num_blocks * 2, sizeof(uint32_t));
	printf("calloc: %d, %lu\n", num_blocks * 2, sizeof(uint32_t));

	/* Copy in our data */
	for (i = 0; i < length; i++)
		buf[i] = data[i];

	/* Encipher */
	//for (i = 0; i < num_blocks; i++)
		xtea_engine_encipher(((uint32_t *)buf), key);

	/* Copy out our data */
	for (i = 0; i < length; i++)
		data[i] = buf[i];

	free(buf);
}

static void xtea_engine_decipher_str(char * data, int length, uint32_t const key[4]) {
int i;

	/* We have to convert our char * into a uint32_t * */
	int data_num_bits = sizeof(char) * length;
	printf("length: %d\n", length);
	printf("data_num_bits: %d\n", data_num_bits);

	/* xtea's block size is 64 bits (2 * 32-bit uints) */
	int num_blocks = 1 + (data_num_bits / 64);
	printf("num_blocks: %d\n", num_blocks);

	/* We need an even number of blocks */
	if (num_blocks % 2 == 0)
		num_blocks++;

	/* Create a buffer to hold our data */
	char * buf = calloc(num_blocks * 2, sizeof(uint32_t));
	printf("calloc: %d, %lu\n", num_blocks * 2, sizeof(uint32_t));

	/* Copy in our data */
	for (i = 0; i < length; i++)
		buf[i] = data[i];

	/* Decipher */
	//for (i = 0; i < num_blocks; i++)
		xtea_engine_decipher(((uint32_t *)buf), key);

	/* Copy out our data */
	for (i = 0; i < length; i++)
		data[i] = buf[i];

	free(buf);
}

void test(uint32_t data1, uint32_t data2) {
	uint32_t key[4] = {1,2,3,4};

	uint32_t buf[2];
	buf[0] = data1;
	buf[1] = data2;

	printf("Testing enciphering %d %d\n", buf[0], buf[1]);

	xtea_engine_encipher(buf, key);

	printf("Done enciphering %d %d\n", buf[0], buf[1]);	

	xtea_engine_decipher(buf, key);

	printf("Done deciphering %d %d\n", buf[0], buf[1]);	
}

int main(int argc, char ** argv) {
	uint32_t d1 = 3;
	uint32_t d2 = 26;
	char str[16];
	uint32_t key[4] = {1,2,3,4};
	sprintf(str, "hello");

	if (argc == 3) {
		d1 = atoi(argv[1]);
		d2 = atoi(argv[2]);
	}

	test(d1, d2);

	printf("\n");

	printf("str: %s\n", str);
	xtea_engine_encipher_str(str, strlen(str), key);
	printf("str: %s\n", str);
	xtea_engine_decipher_str(str, strlen(str), key);
	printf("str: %s\n", str);
	return 0;
}