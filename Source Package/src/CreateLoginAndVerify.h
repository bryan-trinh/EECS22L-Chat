#ifndef CREATE_LOGIN_AND_VERIFY

#define CREATE_LOGIN_AND_VERIFY

#define STR_LEN 64

int VerifyUserName(char* username);
int VerifyPassword(char* username, char* password);
void PrintScanOptions(int* option, char* buffer);
void AppendUser(char* username, char* password);

#endif