CC = clang
CFLAGS = -Wall -Wextra

all: t1 t2

t1: t1.c frac.c
	$(CC) $(CFLAGS) -o $@ $^

t2: t2.c tree.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	-rm t1 t2
