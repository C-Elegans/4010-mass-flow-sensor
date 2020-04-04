OFILES=main.o massflow.o analyzer.o

CFLAGS=-O0 -g
LDFLAGS=-lpthread

main: $(OFILES)
	$(CC) $(LDFLAGS) -o $@ $^
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OFILES)
	rm -f main
