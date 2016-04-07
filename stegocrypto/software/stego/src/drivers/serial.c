/*
 * serial.c
 *
 *  Created on: Jan 14, 2016
 *      Author: derek
 */

#include "general.h"
#include "serial.h"
#include "timer.h"

#define SERIAL_STATUS_MASK_TX_BIT	(0x02)
#define SERIAL_STATUS_MASK_RX_BIT	(0x01)

#define DEV_CONTROL	(DEV)
#define DEV_STATUS	(DEV)
#define DEV_TX_DATA	(DEV + 2)
#define DEV_RX_DATA	(DEV + 2)
#define DEV_BAUD	(DEV + 4)

#define SERIAL_TIMER_ID	0

static boolean timeout_flag;

static void serial_timeout_isr() {
	timer_clear(SERIAL_TIMER_ID);
	timeout_flag = TRUE;
}

void serial_init(unsigned volatile char * DEV, const baud_rate_t BAUD_RATE) {
	/**
	 * Set up 6850 Control Register to use
	 * * Divide by 16 clock (CR1/CR0 = 01)
	 * * 8-bit data, no parity, 1 stop bit (CR4/CR3/CR2 = 101)
	 * * RTS low, Transmitting Interrupt Disabled (CR6/CR5 = 00)
	 * * Receive Interrupt Enable - Set to 1 arbitrarily (CR7 = 1)
	 *
	 * 0b10010101 = 0x95
	 */
	*DEV_CONTROL = 0x95;

	// program baud rate generator to use 115k baud
	*DEV_BAUD = BAUD_RATE;

	/* Initialize the timer */
	timer_init(SERIAL_TIMER_ID, TIMER_ONESHOT, serial_timeout_isr);
}

void serial_put_char(unsigned volatile char * DEV, const unsigned char c) {
	/* Poll Tx bit in 6850 status register. Wait for it to become '1' */
	while ((*DEV_STATUS & SERIAL_STATUS_MASK_TX_BIT) == 0)
		/* Wait */;

	//* Write 'c' to the 6850 TxData register to output the character */
	*DEV_TX_DATA = c;
}

void serial_put_n_char(unsigned volatile char * DEV, const unsigned char * msg, const int msg_length) {
	int count;

	for (count = 0; count < msg_length; count++) {
		serial_put_char(DEV, msg[count]);
	}
}

unsigned char serial_get_char(const unsigned volatile char * DEV) {
	/* Poll Rx bit in 6850 status register. Wait for it to become '1' */
	while ((*DEV_STATUS & SERIAL_STATUS_MASK_RX_BIT) == 0)
		/* Wait */;

	/* Read received character from 6850 RxData register. */
	return (*DEV_RX_DATA);
}

void serial_get_n_char(const unsigned volatile char * DEV, unsigned char * recv_msg, const int recv_msg_length) {
	int count;

	for (count = 0; count < recv_msg_length; count++) {
		recv_msg[count] = serial_get_char(DEV);
	}
}

boolean serial_get_char_timeout(const unsigned volatile char * DEV, unsigned char * char_out, const int timeout_millis) {
	/* Set up the timer */
		timer_stop(SERIAL_TIMER_ID);
		if (timeout_millis > 0) {
			timer_set(SERIAL_TIMER_ID, timeout_millis);
			timer_start(SERIAL_TIMER_ID);
		}

		timeout_flag = FALSE;
		while (timeout_flag == FALSE && serial_test_for_received_data(DEV) == FALSE)
			/* Poll until an event or until we time out */;

		/* If timeout */
		if (timeout_flag == TRUE) {
			return FALSE;
		}

		*char_out = (*DEV_RX_DATA);
		return TRUE;
}

boolean serial_get_n_char_timeout(const unsigned volatile char * DEV, unsigned char * recv_msg, const int recv_msg_length, const int timeout_millis) {
	int count;

	for (count = 0; count < recv_msg_length; count++) {
		 if (!serial_get_char_timeout(DEV, &recv_msg[count], timeout_millis))
			 return FALSE;
	}
	return TRUE;
}

int serial_test_for_received_data(const unsigned volatile char * DEV) {
	/* Test Rx bit in 6850 serial comms chip status register */
	/* If RX bit is set, return TRUE, otherwise return FALSE */
	return ((*DEV_STATUS & SERIAL_STATUS_MASK_RX_BIT) ? 1 : 0);
}

