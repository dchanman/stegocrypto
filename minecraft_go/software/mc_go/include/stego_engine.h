/*
 * stego_engine.h
 *
 *  Created on: Mar 15, 2016
 *      Author: derek
 */

#ifndef STEGO_ENGINE_H_
#define STEGO_ENGINE_H_

#define STEGO_ENGINE_BMP_HEADER_SIZE 14

/**
 * Hides the bytes inside <data> inside a bitmap file <imagedata>.
 *
 * This is the steganography encoding step. A bitmap file is modified
 * slightly so that the pixels used to store RGB data is also used to
 * secretly hide other forms of binary data. This means you could hide
 * messages in plain sight in the bitmap picture and nobody would be
 * able to tell just from looking at the image.
 *
 * @param[inout] imagedata - char array containing a bitmap file, modified upon return
 * @param[in] data - char array representing data to hide inside the bitmap file
 * @param[in] datalength - the length of the data array
 */
void stego_engine_embed(char * imagedata, const char * data, const int datalength);

/**
 * Extracts the encoded bytes inside a bitmap file.
 *
 * This is the steganography decoding step. This extracts the data from
 * the modified bitmap file resulting from the function above.
 *
 * Calling this function on an image file that does not contain embedded
 * data results in undefined output.
 *
 * @param[in] imagedata - char array containing a bitmap file that has embedded steganographic data
 * @param[out] data - the raw embedded data hidden in the bitmap file that has been extracted
 * @param[out] datalength - the length of the extracted data
 *
 */
void stego_engine_extract(const char * imagedata, char ** data, int * datalength);

/**
 * Extracts information from a bitmap file's header.
 *
 * @param[in] header - char array containing a bitmap header
 * @param[out] imagefilesize - size of the entire bitmap file
 * @param[out] data_start_offset - byte offset indicating where the bitmap RGB data begins
 */
void stego_engine_process_header(const char * header, int * imagefilesize, int * data_start_offset);

#endif /* STEGO_ENGINE_H_ */
