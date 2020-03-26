CC=g++
CFLAGS=-I.
LDFLAGS=-pthread
DEPS = 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

tlcontrolmake: main.o config_file_reader.o
	$(CC) -o tlcontrol main.o config_file_reader.o $(LDFLAGS)
