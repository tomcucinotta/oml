NAME=oml_exceptions
CFLAGS=-Wall -Wformat -fPIC 
LDFLAGS=-shared

all: lib$(NAME).so doc

lib$(NAME).so: oml_exceptions.o
	gcc $(LDFLAGS) -o $@ $^

doc: oml_exceptions.h oml_exceptions.c Doxygen.template
	doxygen Doxygen.template

clean:
	rm -f *~ *.bak *.o *.so *.a
	rm -rf doc/*

dep:
	makedepend $(CFLAGS) -- -Y -- *.c

# DO NOT DELETE

oml_exceptions.o: oml_exceptions.h
