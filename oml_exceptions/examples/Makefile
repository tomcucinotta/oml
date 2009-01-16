CFLAGS=-I../ -I. -Wall -Wformat
LDFLAGS=-L../ -Xlinker -rpath -Xlinker ../ -loml_exceptions

PROGRAMS=test-ex

all: $(PROGRAMS)

main: test-queue.o queue.o

test-ex: test-ex.c

clean:
	rm -f *.o *~ *.bak $(PROGRAMS)

dep:
	makedepend -- -Y -- *.c

# DO NOT DELETE

queue.o: queue.h
semaphore.o: semaphore.h
