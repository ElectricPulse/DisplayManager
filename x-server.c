#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

static pid_t x_server_pid;

void start_x_server(const char *display, const char *virtual_terminal){
	x_server_pid = fork();
	if (x_server_pid == 0) {
		char cmd[64];
		snprintf(cmd, sizeof(cmd), "/usr/bin/X %s %s", display, virtual_terminal);
		execl("/bin/bash", "/bin/bash", "-c", cmd, NULL);
		printf("Failed to start X server\n");
		exit(1);
	} else {
		sleep(1);
	}
}	

void stop_x_server() {
	if(x_server_pid != 0) 
		kill(x_server_pid, SIGKILL);		
}

void sig_handler(int signo) {
	stop_x_server();
}

