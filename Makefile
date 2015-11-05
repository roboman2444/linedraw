CC = gcc
LDFLAGS = -lGL -lGLEW -lglfw -lm
CFLAGS = -Wall -Ofast -fstrict-aliasing -march=native
OBJECTS = linedraw.o

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

linedraw: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@ $(LDFLAGS)

debug:	CFLAGS= -Wall -O0 -g  -fstrict-aliasing -march=native
debug: 	$(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o linedraw-$@ $(LDFLAGS)


clean:
	@echo cleaning oop
	@rm -f $(OBJECTS)
purge:
	@echo purging oop
	@rm -f $(OBJECTS)
	@rm -f linedraw
	@rm -f linedraw-debug
