PACKAGE:=oml_exceptions
VERSION:=$(shell cat VERSION)

NAME=oml_exceptions
CFLAGS=-Wall -Wformat -fPIC 
LDFLAGS=-shared

all: lib$(NAME).so doc

lib$(NAME).so: oml_exceptions.o
	gcc $(LDFLAGS) -o $@ $^

.PHONY: doc
doc: oml_exceptions.h oml_exceptions.c Doxygen.template
	doxygen Doxygen.template

clean:
	cd examples && make clean
	rm -f *~ *.bak *.o *.so *.a
	rm -rf doc/*

.PHONY: distclean
distclean: clean
	rm -rf $(shell find . -type d -name CVS)

.PHONY: ChangeLog
ChangeLog:
	cvs2cl -P

README:
	sed -i -e 's/Version .*/Version '`cat VERSION`'/' README

dist: dist-src

dist-src: README
	rm -rf /tmp/$(PACKAGE)-$(VERSION) && mkdir /tmp/$(PACKAGE)-$(VERSION) && cp -r * /tmp/$(PACKAGE)-$(VERSION) && cd /tmp/$(PACKAGE)-$(VERSION) && make distclean && cd .. && tar -czf $(PACKAGE)-$(VERSION).tar.gz $(PACKAGE)-$(VERSION) && echo "Distribution archive in /tmp/$(PACKAGE)-$(VERSION).tar.gz"

dep:
	makedepend $(CFLAGS) -- -Y -- *.c
	cd examples && make dep

# DO NOT DELETE

oml_exceptions.o: oml_exceptions.h
