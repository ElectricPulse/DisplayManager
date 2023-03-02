COBJS = $(shell find -name '*.c' | tr "\n" " ")
OOBJS = $(shell find -name '*.c' | sed 's#\.c#\.o#' | tr "\n" " ")

1:
	gcc -lpam -g -o main $(COBJS)
