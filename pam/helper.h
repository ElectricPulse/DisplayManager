#include <stdbool.h>
bool handle_pam_status(int);

#define save_error(text)  {\
error = calloc(strlen(text)+1, sizeof(char));\
	strcpy(error, text); \
}

#define save_error_int(status_code_int) {\
	char status_code[2] = {status_code_int+'0', '\0'};\
	strcpy(error, status_code); \
}
