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

static void xtea_engine_encipher(uint32_t data[2], const uint32_t key[4]);
static void xtea_engine_decipher(uint32_t data[2], const uint32_t key[4]);
static void xtea_engine_encipher_str(const char * data, int length, char ** output, int * output_length, const uint32_t key[4]);
static void xtea_engine_decipher_str(const char * data, int length, char ** output, int * output_length, const uint32_t key[4]);
static int xtea_engine_char_array_to_xtea_array(const char * chars, const int char_length, uint32_t ** uint32s, int * uint32_length);
static int xtea_engine_xtea_array_to_char_array(const uint32_t * uint32ts, const int uint32_length, char ** chars, int * char_length);

static void xtea_engine_encipher(uint32_t data[2], const uint32_t key[4]) {
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

static void xtea_engine_decipher(uint32_t data[2], const uint32_t key[4]) {
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

static void xtea_engine_encipher_str(const char * data, int length, char ** output, int * output_length, const uint32_t key[4]) {
	int i;
	uint32_t * temp;
	int templength;

	xtea_engine_char_array_to_xtea_array(data, length, &temp, &templength);

	/* Encipher */
	for (i = 0; i < templength; i += 2)
		xtea_engine_encipher(&temp[i], key);

	xtea_engine_xtea_array_to_char_array(temp, templength, output, output_length);

	free(temp);
}

static void xtea_engine_decipher_str(const char * data, int length, char ** output, int * output_length, const uint32_t key[4]) {
	int i;
	uint32_t * temp;
	int templength;

	xtea_engine_char_array_to_xtea_array(data, length, &temp, &templength);

	/* Decipher */
	for (i = 0; i < templength; i+= 2)
		xtea_engine_decipher(&temp[i], key);

	xtea_engine_xtea_array_to_char_array(temp, templength, output, output_length);
	
	free(temp);
}

static int xtea_engine_char_array_to_xtea_array(const char * chars, const int char_length, uint32_t ** uint32s, int * uint32_length) {
	char * padded_char_buffer;
	
	/**
	 * Our XTEA array needs block sizes of 64 bits 
	 * 64 bits  = 8 chars
	 */
	int new_char_length = char_length + (8 - char_length % 8);
	//printf("new_char_length: %d\n", new_char_length);

	padded_char_buffer = malloc(sizeof(char) * new_char_length);
	if (padded_char_buffer == NULL) {
		printf("Out of memory!\n");
		return 1;		
	}
	memset(padded_char_buffer, '\0', new_char_length);

	/* Copy chars into our padded buffer. Remaining space will be filled with \0 */
	memcpy(padded_char_buffer, chars, sizeof(char) * char_length);

	/* HACKY: cast our padded buffer into a uint32_t buffer. The sizes should match because of math */
	*uint32s = (uint32_t *)padded_char_buffer;
	*uint32_length = (new_char_length / 4);

	return 0;
}

static int xtea_engine_xtea_array_to_char_array(const uint32_t * uint32ts, const int uint32_length, char ** chars, int * char_length) {
	char * output;
	int output_length;
	
	/* There are 4 chars in a uint32_t */
	output_length = uint32_length * 4;
	output = malloc(sizeof(char) * output_length);
	if (output == NULL) {
		printf("Out of memory!\n");
		return 1;
	}
	
	memcpy(output, uint32ts, sizeof(char) * output_length);

	*chars = output;
	*char_length = output_length;

	return 0;
}

void test1(uint32_t data1, uint32_t data2) {
	uint32_t key[4] = {1,2,3,4};

	uint32_t buf[2];
	buf[0] = data1;
	buf[1] = data2;

	printf("[%s] <%d> <%d>\n", __func__, data1, data2);

	printf("Testing enciphering %d %d\n", buf[0], buf[1]);

	xtea_engine_encipher(buf, key);

	printf("Done enciphering %d %d\n", buf[0], buf[1]);	

	xtea_engine_decipher(buf, key);

	printf("Done deciphering %d %d\n", buf[0], buf[1]);	
}

void test2(const char * str) {
	uint32_t * xt;
	int xt_len;
	char * str2;
	int str2_len;

	printf("\n%s: <%s>\n", __func__, str);

	xtea_engine_char_array_to_xtea_array(str, strlen(str) + 1, &xt, &xt_len);
	xtea_engine_xtea_array_to_char_array(xt, xt_len, &str2, &str2_len);

	if (strcmp(str, str2) == 0)
		printf("PASSED\n");
	else
		printf("FAILED\n");
	printf("\n");

	free(xt);
	free(str2);
}

void test3(const char * str) {
	uint32_t key[4] = {1,2,3,4};

	char * enciphered;
	int enciphered_length;
	char * deciphered;
	int deciphered_length;

	printf("\n%s: <%s>\n", __func__, str);

	xtea_engine_encipher_str(str, strlen(str), &enciphered, &enciphered_length, key);

	printf("%s\n", strcmp(str, enciphered) == 0 ? "FAILED" : "PASSED");

	xtea_engine_decipher_str(enciphered, enciphered_length, &deciphered, &deciphered_length, key);
	
	printf("%s\n", strcmp(str, deciphered) == 0 ? "PASSED" : "FAILED");

	free(enciphered);
	free(deciphered);
}

int main(int argc, char ** argv) {
	uint32_t d1 = 3;
	uint32_t d2 = 26;

	if (argc == 3) {
		d1 = atoi(argv[1]);
		d2 = atoi(argv[2]);
	}

	test1(d1, d2);
	test1(0x00000000, 0x00000000);
	test1(0xFFFFFFFF, 0x00000000);
	test1(0x00000000, 0xFFFFFFFF);
	test1(0xFFFFFFFF, 0xFFFFFFFF);

	test2("");
	test2("a");
	test2("asdf");
	test2("1234");
	test2("asdf1234");
	test2("1234asdf");
	test2("1234asdf5678jkl;");

	test3("");
	test3("a");
	test3("asdf");
	test3("1234");
	test3("asdf1234");
	test3("1234asdf");
	test3("1234asdf5678jkl;");

	test3("some random message");
	test3("Hello World!\nWhat if i have escapes too?");

	return 0;
}