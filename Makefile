LIBS=$(shell pkg-config --libs gstreamer-1.0 gtk4)
CFLAGS=$(shell pkg-config --cflags gstreamer-1.0 gtk4)

#1: compile
#	./main 1>log 2>&1 &\
	sleep 0.1; \
	if [ -s log ]; then \
		cat log | less; \
	fi 

1:
	gcc -g -fdiagnostics-color=always $(CFLAGS) -l pam -Wall -o main main.c gtk.c x-server.c pam/login.c pam/helper.c $(LIBS)


