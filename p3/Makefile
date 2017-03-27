CC = clang
CFLAGS = -Wall -Wextra
LDFLAGS =

CFLAGS += `pkg-config --cflags glib-2.0`
LDFLAGS += `pkg-config --libs glib-2.0`

all: t3

t3: main.o rec.o
	$(CC) $(LDFLAGS) -o $@ $^

main.o: main.c
	$(CC) -c $(CFLAGS) -o $@ $^ 

rec.o: rec.c
	$(CC) -c $(CFLAGS) -o $@ $^ 

clean:
	-rm t3 main.o rec.o
