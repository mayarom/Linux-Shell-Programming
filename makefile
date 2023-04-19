CC=gcc
CFLAGS=-Wall -Wextra -Wpedantic

all: cmp copy encode decode codecA codecB stshell

cmp: cmp.o
	$(CC) $(CFLAGS) -o $@ $^

copy: copy.o
	$(CC) $(CFLAGS) -o $@ $^

encode: encode.o
	$(CC) $(CFLAGS) -o $@ $^ -ldl

decode: decode.o
	$(CC) $(CFLAGS) -o $@ $^ -ldl

codecA: codecA.o
	$(CC) $(CFLAGS) -shared -o libcodecA.so $^

codecB: codecB.o
	$(CC) $(CFLAGS) -shared -o libcodecB.so $^

stshell: stshell.o
	$(CC) $(CFLAGS) -o $@ $^ -lreadline -lpthread

cmp.o: cmp.c
	$(CC) $(CFLAGS) -c $<

copy.o: copy.c
	$(CC) $(CFLAGS) -c $<

encode.o: encode.c
	$(CC) $(CFLAGS) -c $<

decode.o: decode.c
	$(CC) $(CFLAGS) -c $<

codecA.o: codecA.c
	$(CC) $(CFLAGS) -c -fPIC $<

codecB.o: codecB.c
	$(CC) $(CFLAGS) -c -fPIC $<

stshell.o: stshell.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o cmp copy encode decode libcodecA.so libcodecB.so stshell
