CC = clang
CFLAGS = -Wall -Wextra
LDFLAGS =

GLIB_CFLAGS = `pkg-config --cflags glib-2.0`
GLIB_LDFLAGS = `pkg-config --libs glib-2.0`

all: cli ser

cli: cli.o
	$(CC) $(LDFLAGS) -o $@ $^

cli.o: cli.c
	$(CC) -c $(CFLAGS) -o $@ $^ 

ser: ser.o
	$(CC) $(LDFLAGS) $(GLIB_LDFLAGS) -o $@ $^

ser.o: ser.c
	$(CC) -c $(CFLAGS) $(GLIB_CFLAGS) -o $@ $^ 

clean:
	-rm ser cli ser.o cli.o
