#include <sys/types.h>
#include <security/pam_appl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdarg.h>

#include <pwd.h>
#include <string.h>
#include <err.h>
#include <paths.h>

#include "helper.h"


//appdata_ptr is the data we passed to the pam conv struct
/*
	struct pam_message {
		int msg_style;
		const char *msg;
	}
	
	struct pam_response {
		char *resp;
		int resp_retcode
	}
*/
static pam_handle_t *pam_handle;

static int conv(int num_msg, const struct pam_message **msg, struct pam_response **resp, void *appdata_ptr){
	if(num_msg != 1)
		return PAM_BUF_ERR;

	char * const pass = strdup(((char **)appdata_ptr)[1]);

	int result = PAM_SUCCESS;
	


	switch(msg[0]->msg_style) {
		case PAM_PROMPT_ECHO_ON:
			printf("Unhandled PAM_PROMPT_ECHO_ON in conv\n");
			break;
		case PAM_PROMPT_ECHO_OFF:
			*resp = malloc(sizeof(struct pam_response));
			(*resp)[0].resp = pass;	
			break;
		case PAM_ERROR_MSG:
			printf("Handle PAM_ERROR_MSG in conv\n");
			break;
		case PAM_TEXT_INFO:
			printf("%s\n", msg[0]->msg);
			break;
	}

	return result;
}

static void set_env(char *name, char *value) {
    // The `+ 2` is for the '=' and the null byte
    size_t name_value_len = strlen(name) + strlen(value) + 2;
    char *name_value = malloc(name_value_len);
    snprintf(name_value, name_value_len,  "%s=%s", name, value);
    pam_putenv(pam_handle, name_value);
    free(name_value);
}


static void init_env(struct passwd *pw) {
    set_env("HOME", pw->pw_dir);
    set_env("PWD", pw->pw_dir);
    set_env("SHELL", pw->pw_shell);
    set_env("USER", pw->pw_name);
    set_env("LOGNAME", pw->pw_name);
    set_env("PATH", "/usr/local/sbin:/usr/local/bin:/usr/bin");
    set_env("MAIL", _PATH_MAILDIR);

   	size_t xauthority_len = strlen(pw->pw_dir) + strlen("/.Xauthority") + 1;
    char *xauthority = malloc(xauthority_len);
    snprintf(xauthority, xauthority_len, "%s/.Xauthority", pw->pw_dir);
    set_env("XAUTHORITY", xauthority);
    free(xauthority);
}


bool login(const char *user, const char *pass, pid_t *child_pid){
	const char *service_name = "login";

	const char *data[2] = {user, pass};
	const struct pam_conv pam_conversation = {
		conv, data
	};

	int status;

	status = pam_start(service_name, user, &pam_conversation, &pam_handle);
	if(handle_pam_status(status))
		return 0;
	
	status = pam_authenticate(pam_handle, 0);
	if(handle_pam_status(status))
		return 0;

    status = pam_acct_mgmt(pam_handle, 0);
	if(handle_pam_status(status))
		return 0;

    status = pam_setcred(pam_handle, PAM_ESTABLISH_CRED);
	if(handle_pam_status(status))
		return 0;

    status = pam_open_session(pam_handle, 0);
	if(handle_pam_status(status)){
        pam_setcred(pam_handle, PAM_DELETE_CRED);
		return 0;
	}

   struct passwd *pw = getpwnam(user);
   init_env(pw);
	
   *child_pid = fork();
    if (*child_pid == 0) {
        chdir(pw->pw_dir);
        // We don't use ~/.xinitrc because we should already be in the users home directory
       	char *cmd = "exec /bin/bash --login .xinitrc";
		//char *cmd = "sleep 15";
		setuid(pw->pw_uid);

        execl(pw->pw_shell, pw->pw_shell, "-c", cmd, NULL);
        printf("Failed to start window manager\n");
		exit(1);
    }

    return 1;
}

int logout(pam_handle_t **pam_h){
	//return pam_end(pam_h, pam_status);
}


