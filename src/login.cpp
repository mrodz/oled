#include <Arduino.h>
#include <cstring>
#include <mbedtls/aes.h>

#include "login.hpp"

unsigned char *encrypt_any_length_string(const char *input, uint8_t *key, uint8_t *iv)
{
	int padded_input_len = 0;
	int input_len = strlen(input) + 1;
	int modulo16 = input_len % 16;
	if (input_len < 16)
		padded_input_len = 16;
	else
		padded_input_len = (strlen(input) / 16 + 1) * 16;

	char *padded_input = new char[padded_input_len];

	if (padded_input == nullptr)
	{
		Serial.println("Failed to allocate memory");
		return;
	}

	memcpy(padded_input, input, strlen(input));
	uint8_t pkc5_value = (17 - modulo16);
	for (int i = strlen(input); i < padded_input_len; i++)
	{
		padded_input[i] = pkc5_value;
	}
	unsigned char *encrypt_output = new unsigned char[padded_input_len];
	mbedtls_aes_context aes;
	mbedtls_aes_init(&aes);
	mbedtls_aes_setkey_enc(&aes, key, 256);
	mbedtls_aes_crypt_cbc(&aes, MBEDTLS_AES_ENCRYPT, padded_input_len, iv,
						  (unsigned char *)padded_input, encrypt_output);

	return encrypt_output;
}

#define MAX_USERS 4

User users[MAX_USERS];

User::User(char *name, char *password_hash)
{
	strncpy(this->name, name, MAX_USERNAME_LEN);
	this->name[MAX_USERNAME_LEN - 1] = '\0';

	this->password_hash = password_hash;
}

User::~User() {
	delete[] this->password_hash;
}