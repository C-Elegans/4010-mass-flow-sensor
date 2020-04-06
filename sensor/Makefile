OFILES=main.o massflow.o analyzer.o adcthread.o
SHADOWFILES=$(patsubst %.c, %.o, $(wildcard shadow.c))

CFLAGS=-O0 -g
LDFLAGS=-lpthread -lm

main: $(OFILES) $(SHADOWFILES)
	$(CC) $(LDFLAGS) -o $@ $^
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OFILES)
	rm -f main
