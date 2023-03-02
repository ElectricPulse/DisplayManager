#include <stdbool.h>
#include <signal.h>
void gtk_initialize(int argc, char ** argv);
void start_x_server(const char *display, const char *virtual_terminal);
void stop_x_server(void);
void sig_handler(int);
bool login(const char *, const char *, pid_t *);
bool logout(void);

