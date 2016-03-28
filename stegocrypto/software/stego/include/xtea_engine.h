#ifndef XTEA_ENGINE_H__
#define XTEA_ENGINE_H__

/**
 * Enciphers two unsigned integers using the XTEA encryption scheme
 * and a provided key.
 *
 * @param[inout] data
 * @param[in] key
 */
void xtea_engine_encipher(unsigned int data[2], const unsigned int key[4]);

/**
 * Deciphers two unsigned integers using the XTEA encryption scheme
 * and a provided key.
 *
 * @param[inout] data
 * @param[in] key
 */
void xtea_engine_decipher(unsigned int data[2], const unsigned int key[4]);

/**
 * Produces an enciphered string using the XTEA encryption scheme
 * and a provided key.
 *
 * @param[in] data - plaintext to be encrypted
 * @param[in] length - length of the string. You may want to use strlen(data) + 1 to include the null terminator
 * @param[out] output - encrypted data. May not include a NULL terminator
 * @param[out] output_length - length of the output
 * @param[in] key
 */
void xtea_engine_encipher_str(const char * data, const int length, char ** output, int * output_length, const unsigned int key[4]);

/**
 * Produces an deciphered string using the XTEA encryption scheme
 * and a provided key.
 *
 * @param[in] data - encrypted data
 * @param[in] length - length of the encrypted array
 * @param[out] output - decrypted data
 * @param[out] output_length - length of the output
 * @param[in] key
 */
void xtea_engine_decipher_str(const char * data, const int length, char ** output, int * output_length, const unsigned int key[4]);

#endif
