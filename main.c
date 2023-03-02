#include "main.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

int main (int argc, char **argv)
{ 
	const char *display = ":1";
	const char *virtual_terminal = "vt02";
	
	bool testing = true;
	if(argc == 1 || strcmp(argv[1], "--testing"))
		testing = false;

	if(!testing) {
		setenv("DISPLAY", display, true);
		signal(SIGSEGV, sig_handler);
		signal(SIGTRAP, sig_handler);
		start_x_server(display, virtual_terminal);
	}
	
	char **_temp;
	gtk_initialize(0,_temp);
	
	if(!testing) 
		stop_x_server();

	return 0;
} 


