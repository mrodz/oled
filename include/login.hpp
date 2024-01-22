#ifndef OLED_LOGIN_HPP
#define OLED_LOGIN_HPP

#define MAX_USERNAME_LEN 17

class User
{
	char name[MAX_USERNAME_LEN];
	char * password_hash;

public:
	User(char *name, char *password_hash);
	virtual ~User();
};

unsigned char *encrypt_any_length_string(const char *input, uint8_t *key, uint8_t *iv);

#endif