#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "xtea_engine.h"


void test1(unsigned int data1, unsigned int data2) {
	unsigned int key[4] = {1,2,3,4};

	unsigned int buf[2];
	buf[0] = data1;
	buf[1] = data2;

	printf("[%s] <%d> <%d>\n", __func__, data1, data2);

	printf("Testing enciphering %d %d\n", buf[0], buf[1]);

	xtea_engine_encipher(buf, key);

	printf("Done enciphering %d %d\n", buf[0], buf[1]);	

	xtea_engine_decipher(buf, key);

	printf("Done deciphering %d %d\n", buf[0], buf[1]);	
}

#ifdef xtea_engine_char_array_to_xtea_array
void test2(const char * str) {
	unsigned int * xt;
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
#endif

void test3(const char * str) {
	unsigned int key[4] = {1,2,3,4};

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

void xtea_engine_test() {
	unsigned int d1 = 3;
	unsigned int d2 = 26;

	test1(d1, d2);
	test1(0x00000000, 0x00000000);
	test1(0xFFFFFFFF, 0x00000000);
	test1(0x00000000, 0xFFFFFFFF);
	test1(0xFFFFFFFF, 0xFFFFFFFF);

#ifdef test2
	test2("");
	test2("a");
	test2("asdf");
	test2("1234");
	test2("asdf1234");
	test2("1234asdf");
	test2("1234asdf5678jkl;");
#endif

	test3("");
	test3("a");
	test3("asdf");
	test3("1234");
	test3("asdf1234");
	test3("1234asdf");
	test3("1234asdf5678jkl;");

	test3("some random message");
	test3("Hello World!\nWhat if i have escapes too?");
}
