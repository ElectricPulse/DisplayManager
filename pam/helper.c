#include <string.h>
#include <stdio.h>
#include <security/pam_appl.h>
#include <stdlib.h>
#include "helper.h"

bool handle_pam_status(int status){
	if(status == PAM_SUCCESS)
		return 0;

	char *error;
	switch(status){
		case PAM_ABORT:
			save_error("General failure\n");
			break;
		case PAM_BUF_ERR:
			save_error("Memory buffer error\n");
			break;
		case PAM_SYSTEM_ERR:
			save_error("System error, for example a NULL pointer was passed as data\n");
			break;
		case PAM_AUTH_ERR:
			save_error("The user was not authenticated\n");
			break;
		case PAM_CRED_INSUFFICIENT:
			save_error("Non sufficient credentials\n");
			break;
		case PAM_USER_UNKNOWN:
			save_error("Unknown user\n");
			break;
		case PAM_MAXTRIES:
			save_error("Maxtries reached\n");
			break;
		case PAM_AUTHINFO_UNAVAIL:
			save_error("PAM_AUTHINFO_UNAVAIL\n");
			break;
		case PAM_PERM_DENIED:
			save_error("Error: PAM_PERM_DENIED\n");
			break;
		default: 
			save_error_int(status);
	}
	
	printf("[Error]: %s", error);
	return 1;
}

