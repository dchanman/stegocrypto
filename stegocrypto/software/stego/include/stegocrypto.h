/*
 * stegocrypto.h
 *
 *  Created on: Mar 31, 2016
 *      Author: derek
 */

#ifndef STEGOCRYPTO_H_
#define STEGOCRYPTO_H_

/**
 * Geographically encrypts and stegographically embeds data into the .bmp imagedata buffer
 *
 * @param[inout] imagedata - buffer containing a .bmp file. The RGB values will be modified to store the data
 * @param[in] data - data to be encrypted and embedded into the image file
 * @param[in] datalength - length of the data
 * @param[in] longitude_key - longitude value of the geoencryption key
 * @param[in] latitude_key - latitude value of the geoencryption key
 */
void stegocrypto_engine_embed(char * imagedata, const char * data, const int datalength, const float longitude_key, const float latitude_key);

/**
 * Geographically decrypts and stegographically extracts data from the .bmp imagedata buffer
 *
 * NOTE THAT ON SUCCESS, DECRYPTED POINTS TO AN ALLOCATED BUFFER THAT MUST BE FREED.
 *
 * @param[in] imagedata - buffer containing a .bmp file with embedded stegographic data
 * @param[out] decrypted - data extracted and decrypted
 * @param[out] decrypted_length - length of data
 */
void stegocrypto_engine_extract(const char * imagedata, char ** decrypted, int * decrypted_length);


#endif /* STEGOCRYPTO_H_ */