/*
 * bluetooth_test.c
 *
 *  Created on: 2016-01-25
 *      Author: Johnson
 */

#include <stdint.h>
#include <stdio.h>
#include "bluetooth.h"

void getAndPrintACK(){
	char ack = bluetooth_get_char();
	printf("ACK received: %c\n", ack);
}

void bluetooth_test() {
	// use the bluetooth default for now
	bluetooth_init();

	printf("Ready\n");

	char *msg = "Hello world!";
	int i;
	//char *recv_msg = "";

	while(1) {
		unsigned char temp;
		bluetooth_get_n_char(&temp, 1);
		printf("%c", temp);
		bluetooth_send_data(&temp, 1);
	}

	printf("Bluetooth test done.\n");
}
