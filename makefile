CC=g++
CFLAGS=-I.
LDFLAGS=-lSOIL -lGL -lGLU -lglut -pthread
DEPS = 
OBJS = main.o config_file_reader.o gui.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

tlcontrolmake: $(OBJS)
	$(CC) -o tlcontrol $(OBJS) $(LDFLAGS)
