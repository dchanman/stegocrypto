
/*
 * data_transfer_test.c
 *
 *  Created on: Mar 29, 2016
 *      Author: Logan
 */

//test receiving message
//#define RECEIVING_TESTING

//test sending message
//#define SENDING_TESTING

//test entire communication
//#define COMMUNICATION_TESTING

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "data_transfer.h"

void data_transfer_test() {
	data_transfer_init();

		unsigned char * recv_msg = NULL;

		data_transfer_receive(&recv_msg);
		printf("Received %s\n", recv_msg);
		printf("Done receiving\n");

		printf("Test sending\n");
		data_transfer_send(recv_msg, strlen(recv_msg) + 1);
		free(recv_msg);
		printf("Done\n");

	#ifdef SENDING_TESTING
		const unsigned char * msg = "hello this is my msg";
		data_transfer_send(msg, strlen(msg) + 1);
	#endif

	#ifdef COMMUNICATION_TESTING
		//************************************************************************
		//Encoding step
		//************************************************************************
		//Receive image and data
		unsigned char * image = NULL;
		data_transfer_receive(image);

		unsigned char * data = NULL;
		data_transfer_receive(data);

		const int datalength =  sizeof(data);

		unsigned char * longitude_msg = NULL;
		data_transfer_receive(longitude_msg);
		const double long_temp = atof(longitude_msg);
		const float longitude_key = (float) long_temp;

		unsigned char * latitude_msg = NULL;
		data_transfer_receive(latitude_msg);
		const double lat_temp = atof(latitude_msg);
		const float latitude_key = (float) lat_temp;

		//Encode data into image and then send
		stegocrypto_engine_embed(image, data, datalength, longitude_key, latitude_key);
		data_transfer_send(image, strlen(image));

		//************************************************************************
		//Decoding step
		//************************************************************************
		//Receive image with encoded data and decode
		unsigned char * imagedata = NULL;
		memset(imagedata, '\0', sizeof(imagedata));
		data_transfer_receive(imagedata);

		unsigned char * decrypted = NULL;
		memset(decrypted, '\0', sizeof(decrypted));
		int *decrypted_length;

		//Send image and data separately
		stegocrypto_engine_extract(imagedata, decrypted, decrypted_length);
		data_transfer_send(imagedata, strlen(imagedata));
		data_transfer_send(decrypted, decrypted_length);

		free(decrypted);
	#endif

	usleep(1000000);
	printf("End of test!\n");
}
