CC=gcc
CFLAGS=-I.
LDFLAGS=-pthread
DEPS = 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

tlcontrolmake: main.o
	$(CC) -o tlcontrol main.o $(LDFLAGS)
