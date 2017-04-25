CC = clang
CFLAGS = -Wall -Wextra -g -O0
LDFLAGS =

CFLAGS += `pkg-config --cflags glib-2.0`
LDFLAGS += `pkg-config --libs glib-2.0`

all: csv2bin bincat

csv2bin: csv2bin.o rec.o
	$(CC) $(LDFLAGS) -o $@ $^

csv2bin.o: csv2bin.c
	$(CC) -c $(CFLAGS) -o $@ $^

rec.o: rec.c
	$(CC) -c $(CFLAGS) -o $@ $^

bincat: bincat.o recv.o
	$(CC) $(LDFLAGS) -o $@ $^

bincat.o: bincat.c
	$(CC) -c $(CFLAGS) -o $@ $^

recv.o: recv.c
	$(CC) -c $(CFLAGS) -o $@ $^

clean:
	-rm csv2bin csv2bin.o rec.o bincat bincat.o recv.o
