CFILES = balls.c p1.c
TARGETS = p1
CC ?= clang 
CFLAGS = -Wall -Wextra

all: $(TARGETS)
	
$(TARGETS): $(CFILES)
	$(CC) $(CFLAGS) -o $@ $^

clean: $(TARGETS)
	rm $(TARGETS)
