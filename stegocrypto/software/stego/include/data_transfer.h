/*
 * data_transfer.h
 *
 *  Created on: Mar 29, 2016
 *      Author: Logan
 */

#ifndef DATA_TRANSFER_H_
#define DATA_TRANSFER_H_

//constants for expected message lengths
#define DATA_CONNECTION_CHECK_LENGTH 1
#define DATA_LENGTH_CONST 1

//constants to determine stage for case statement
#define STAGE_WAIT 1
#define STAGE_STRING_LENGTH 2
#define STAGE_MESSAGE_LENGTH 3
#define STAGE_FINAL 4

/*
 * Initialize the bluetooth
 */
//void data_transfer_encode_mode();

/*
 * Initialize the bluetooth
 */
//void data_transfer_decode_mode();

/*
 * Initialize the bluetooth
 */
void data_transfer_init();

/*
 * Receives a message from the serial port over bluetooth
 *
 * @param recv_msg - pointer to a message that will be received
 */
void data_transfer_receive(unsigned char ** recv_msg);

/*
 * Sends a message using the serial port over bluetooth
 *
 * @param sent_msg - pointer to a message that will be sent
 * @param send_msg_length - pointer to length of message
 * @param length_of_send_msg_length - pointer to number of characters in send_msg_length
 */
void data_transfer_send(unsigned char *imagedata, const int datalength);

#endif /* DATA_TRANSFER_H_ */
