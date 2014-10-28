CC = gcc
CFLAGS = -I.
DEPS =
OBJ = flop.c

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

flop: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o *~ flop
