CFLAGS=-I../ -I. -Wall -Wformat
LDFLAGS=-L../ -loml_exceptions

all: test-queue test-ex

main: test-queue.o queue.o

test-ex: test-ex.c

clean:
	rm -f *.o *~ *.bak

dep:
	makedepend -- -Y -- *.c

# DO NOT DELETE

queue.o: queue.h
semaphore.o: semaphore.h
