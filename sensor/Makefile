OFILES=main.o

main: $(OFILES)
	$(CC) -o $@ $^
%.c: %.o
	$(CC) -c -o $@ $<
