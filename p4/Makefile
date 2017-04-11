CC = clang
CFLAGS = -Wall -Wextra
LDFLAGS =

CFLAGS += `pkg-config --cflags glib-2.0`
LDFLAGS += `pkg-config --libs glib-2.0`

all: t4

t4: main.o rec.o pspt.o
	$(CC) $(LDFLAGS) -o $@ $^

main.o: main.c
	$(CC) -c $(CFLAGS) -o $@ $^ 

rec.o: rec.c
	$(CC) -c $(CFLAGS) -o $@ $^ 

pspt.o: pspt.c
	$(CC) -c $(CFLAGS) -o $@ $^ 

clean:
	-rm t4 main.o rec.o pspt.o s* o*
