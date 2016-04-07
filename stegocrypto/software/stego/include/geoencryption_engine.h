/*
 * geoencryption_engine.h
 *
 *  Created on: Mar 29, 2016
 *      Author: derek
 */

#ifndef GEOENCRYPTION_ENGINE_H_
#define GEOENCRYPTION_ENGINE_H_

/**
 * Initializes the geoencryption engine and its dependencies
 */
void geoencryption_engine_init();

/**
 * Encrypt data using a specified longitude and latitude as the encryption key.
 *
 * WARNING:
 * Note that floats have terrible precision. Give a few extra decimal points whenever possible
 * Example: If you want 126.26, you should provide 126.261.
 * The microcontroller might give you 125.25999 if you truncate too early.
 *
 * Note that encrypted output uses allocated memory. You need to free the buffer pointed to
 * by encrypted when you are finished.
 *
 * @params[in] data - data to be encrypted
 * @params[in] data_length
 * @params[out] encrypted - encrypted data
 * @params[out] encrypted_length - length of encrypted data, not guaranteed to be same value as data_length!
 * @params[in] longitude
 * @params[in] latitude
 */
void geoencryption_engine_encrypt(const char * data, const int data_length, char ** encrypted, int * encrypted_length, float longitude, float latitude);

/**
 * Decrypt data using longitude and latitude from the GPS as the encryption key.
 *
 * Note that decrypted output uses allocated memory. You need to free the buffer pointed to
 * by decrypted when you are finished.
 *
 * @params[in] data - data to be decrypted
 * @params[in] data_length
 * @params[out] decrypted - plaintext data
 * @params[out] decrypted_length - length of plaintext data, not guaranteed to be same value as data_length!
 * @params[out] longitude_out - stores the longitude string of the key used. You may set this to NULL if you don't care about this output
 * @params[out] latitude_out - stores the latitude string of the key used. You may set this to NULL if you don't care about this output
 */
void geoencryption_engine_decrypt(const char * data, const int data_length, char ** decrypted, int * decrypted_length, char * longitude_out, char * latitude_out);

#endif /* GEOENCRYPTION_ENGINE_H_ */
