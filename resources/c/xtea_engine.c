/**
 * XTEA: eXtended Tiny Encryption Algorithm
 * Reference: https://en.wikipedia.org/wiki/XTEA
*/

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
//#include "xtea_engine.h"

#define KEY_SCHEDULE_CONSTRAINT 0x9e3779b9
#define NUM_ROUNDS	32

void xtea_engine_encipher(uint32_t data[2], uint32_t const key[4]) {
	uint32_t sum = 0;
	uint32_t d0 = data[0];
	uint32_t d1 = data[1];
	uint32_t delta = KEY_SCHEDULE_CONSTRAINT;

	int i;
	for (i = 0; i < NUM_ROUNDS; i++) {
		d0 += (((d1 << 4) ^ (d1 >> 5)) + d1) ^ (sum + key[sum & 3]);
        sum += delta;
        d1 += (((d0 << 4) ^ (d0 >> 5)) + d0) ^ (sum + key[(sum >> 11) & 3]);
	}

	data[0] = d0;
	data[1] = d1;
}

void xtea_engine_decipher(uint32_t data[2], uint32_t const key[4]) {
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

	if (argc == 3) {
		d1 = atoi(argv[1]);
		d2 = atoi(argv[2]);
	}

	test(d1, d2);
	return 0;
}