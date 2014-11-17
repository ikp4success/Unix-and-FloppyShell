CC = gcc
CFLAGS = -I.
DEPS = parentF.h flshell.h flop.h minsc.h
OBJ = main.c

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o *~ main
