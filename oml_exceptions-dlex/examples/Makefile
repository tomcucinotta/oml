CFLAGS=-I../ -I. -Wall -Wformat
LDFLAGS=-L../ -Xlinker -rpath -Xlinker ../ -loml_exceptions -lrt

PROGRAMS=test-ex test-ex-deadline test-ex-deadl-nested test-ex-deadl-return

all: $(PROGRAMS)

main: test-queue.o queue.o

test-ex: test-ex.o

clean:
	rm -f *.o *~ *.bak $(PROGRAMS)

dep:
	makedepend $(CFLAGS) -- -Y -- *.c

# DO NOT DELETE

queue.o: queue.h
semaphore.o: semaphore.h
test-ex-deadl-nested.o: ../oml_exceptions.h
test-ex-deadline.o: ../oml_exceptions.h
test-ex.o: ../oml_exceptions.h
