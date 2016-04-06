/*
 * data_transfer.c
 *
 *  Created on: Mar 29, 2016
 *      Author: Logan
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "data_transfer.h"
#include "bitmap.h"
#include "bluetooth.h"
#include "sdcard.h"

/* Globals */
static unsigned char * READY_MSG = "R";
const unsigned char * SEND_MSG = "S";

//global variables
static unsigned int recv_msg_length = 0;
static unsigned int actual_msg_length = 0;



//static functions
//static void data_transfer_init();
//static void data_transfer_receive(unsigned char *recv_msg);
//void data_transfer_send(unsigned char *imagedata, const int datalength);
static void data_transfer_receive_communication(unsigned char *msg, const int msg_length, const int com_stage);
static void data_transfer_send_communication(unsigned char *msg, const int length, const int com_stage);

/*
void data_transfer_encode_mode() {
		//************************************************************************
		//Encoding step from data_transfer_test.c
		//************************************************************************
}
*/

/*
void data_transfer_decode_mode() {
		//************************************************************************
		//Decoding step from data_transfer_test.c
		//************************************************************************
}
*/

/*
 * Initialize the bluetooth
 */
void data_transfer_init() {
	bluetooth_init();
}

void data_transfer_receive(unsigned char ** recv_msg) {
	unsigned char temp_msg[11];
	memset(temp_msg, '\0', sizeof(temp_msg));

	//check for ready message to start communication
	printf("Waiting for ready message!\n");
	data_transfer_receive_communication(temp_msg, DATA_CONNECTION_CHECK_LENGTH, STAGE_WAIT);

	//check for length of the string length of the final message
	printf("Waiting for length of string length message!\n");
	data_transfer_receive_communication(temp_msg, DATA_LENGTH_CONST, STAGE_STRING_LENGTH);

	//check for length of the final message
	printf("Waiting for string length message!\n");
	data_transfer_receive_communication(temp_msg, recv_msg_length, STAGE_MESSAGE_LENGTH);

	//check for file
	printf("Waiting for file!\n");
	*recv_msg = malloc(sizeof(unsigned char) * actual_msg_length);
	data_transfer_receive_communication(*recv_msg, actual_msg_length, STAGE_FINAL);
}

void data_transfer_send(unsigned char * imagedata, const int datalength) {
	unsigned char *ready_msg = "S";

	unsigned char send_msg_length[10];
	memset(send_msg_length, '\0', sizeof(send_msg_length));
	unsigned char length_of_send_msg_length[2];
	memset(length_of_send_msg_length, '\0', sizeof(length_of_send_msg_length));

	//convert datalength to string
	snprintf(send_msg_length, sizeof(send_msg_length), "%d", datalength);

	//get length of send_msg_length
	snprintf(length_of_send_msg_length, sizeof(length_of_send_msg_length), "%d", strlen(send_msg_length));

	//send ready message to start communication
	bluetooth_send_command(ready_msg);
	printf("Ready message sent!\n");

	//send length of the string length of the final message
	data_transfer_send_communication(length_of_send_msg_length, strlen(length_of_send_msg_length), STAGE_STRING_LENGTH);
	printf("Length of string length message sent!\n");

	//send length of the final message
	data_transfer_send_communication(send_msg_length, strlen(send_msg_length), STAGE_MESSAGE_LENGTH);
	printf("String length message sent!\n");

	//send final message
	data_transfer_send_communication(imagedata, datalength, STAGE_FINAL);
	printf("Final message sent!\n");
}

/*
 * Receives a message using the serial port over bluetooth and sends
 * "R" when it's ready to move to the next communication stage
 *
 * @param msg - pointer to a message that will be sent
 * @param msg_length - length of message
 * @param com_stage - stage in communication
 */
static void data_transfer_receive_communication(unsigned char * msg, const int msg_length, const int com_stage) {
	int recv_check = 1;


	while(recv_check) {
		bluetooth_get_n_char(msg, msg_length);

		switch(com_stage) {
			//wait for ready message
			case STAGE_WAIT  :
				//compare message received to "R"
				if(strcmp(msg, SEND_MSG) == 0) {
					printf("Ready message received!\n\n");
					memset(msg, 0, msg_length);
					bluetooth_send_command(READY_MSG);
					recv_check = 0;
				}
				break;

			//get length of the string length of the final message
			case STAGE_STRING_LENGTH  :
				recv_msg_length = atoi(msg);
				//check that recv_msg_length is an integer > 0
				if(recv_msg_length > 0) {
					printf("Length of string length message is %d!\n\n", recv_msg_length);
					memset(msg, 0, msg_length);
					bluetooth_send_command(READY_MSG);
					recv_check = 0;
				}
				break;

			//get length of the final message
			case STAGE_MESSAGE_LENGTH  :
				actual_msg_length = atoi(msg);
				//check that recv_msg_length is an integer > 0
				if(actual_msg_length > 0) {
					printf("Length of actual message is %d!\n\n", actual_msg_length);
					memset(msg, 0, msg_length);
					bluetooth_send_command(READY_MSG);
					recv_check = 0;
				}
				break;

			//get final message
			case STAGE_FINAL  :
				//check that message received is not NULL
				if(strcmp(msg, "") != 0) {
					printf("Final message received:\n");
					printf("%s\n", msg);
					recv_msg_length = 0;
					actual_msg_length = 0;
					recv_check = 0;
				}
				break;

			//print an error if com_stage doesn't pass properly
			default :
				printf("ERROR: com_stage not being passed properly!\n");
				recv_check = 0;
				break;
		}
	}
}

/*
 * Sends a message using the serial port over bluetooth and moves to
 * the next communication stage when an "R" is received
 *
 * @param msg - pointer to a message that will be sent
 * @param msg_length - length of message
 * @param com_stage - stage in communication
 */
static void data_transfer_send_communication(unsigned char *msg, const int length, const int com_stage) {
	int send_check = 1;
	unsigned char ready_check[DATA_CONNECTION_CHECK_LENGTH];

	while(send_check) {
		bluetooth_get_n_char(ready_check, DATA_CONNECTION_CHECK_LENGTH);

		//wait for "R" message from receiver
		if(strncmp(ready_check, READY_MSG, DATA_CONNECTION_CHECK_LENGTH) == 0) {
			if(com_stage == STAGE_FINAL)
				bluetooth_send_data(msg, length);
			else
				bluetooth_send_command(msg);
			send_check = 0;
		}
	}
}
