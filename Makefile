PACKAGE:=oml
VERSION:=$(shell cat VERSION)
TEST_PROGS=$(patsubst %.c,%,$(wildcard test-*.c))
LIB_SRCS=$(patsubst test-%.c,,$(wildcard *.c))
LIB_OBJS=$(patsubst %.c,%.o,$(LIB_SRCS))
OML_LIB=liboml.so
CFLAGS=-Wall

all: $(OML_LIB) $(TEST_PROGS)

TEST_LOG_FILE=tests-log.txt

test: $(TEST_PROGS)
	@$(foreach prog,$(TEST_PROGS),printf "%30s" "Running $(prog) ... " ; printf "\n%30s\n" "RUNNING TEST $(prog)" >> $(TEST_LOG_FILE); if LD_LIBRARY_PATH="$$LD_LIBRARY_PATH:." ./$(prog) >>$(TEST_LOG_FILE) 2>> $(TEST_LOG_FILE); then echo "Ok"; else echo "Failed (see $(TEST_LOG_FILE))"; fi; )

doc:
	doxygen

$(OML_LIB): $(LIB_OBJS)
	gcc -shared $^ -o $@

test-%: test-%.o
	gcc $(CFLAGS) -L. -loml $< -o $@

%.o: %.c
	gcc $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(OML_LIB) $(TEST_PROGS) *.o *~ *.bak *.d

.PHONY: distclean
distclean: clean
	rm -rf $(shell find . -type d -name CVS)

vars:
	echo TEST_PROGS=$(TEST_PROGS)
	echo LIB_SRCS=$(LIB_SRCS)
	echo LIB_OBJS=$(LIB_OBJS)

dist: dist-src

dist-src:
	rm -rf /tmp/$(PACKAGE)-$(VERSION) && mkdir /tmp/$(PACKAGE)-$(VERSION) && cp -r * /tmp/$(PACKAGE)-$(VERSION) && cd /tmp/$(PACKAGE)-$(VERSION) && make distclean && cd .. && tar -czf $(PACKAGE)-$(VERSION).tar.gz $(PACKAGE)-$(VERSION) && echo "Distribution archive in /tmp/$(PACKAGE)-$(VERSION).tar.gz"

# DO NOT DELETE

oml_debug.o: oml_debug.h
oml_malloc.o: oml_debug.h oml_malloc.h
test-eheap.o: oml_debug.h oml_eheap.h oml_malloc.h oml_common.h
test-heap.o: oml_debug.h oml_heap.h oml_malloc.h
test-list.o: oml_debug.h oml_list.h oml_malloc.h
test-lstack.o: oml_debug.h oml_lstack.h oml_list.h oml_malloc.h
test-queue.o: oml_debug.h
test-theap.o: oml_debug.h oml_theap.h oml_malloc.h oml_common.h
test-vstack.o: oml_debug.h oml_vstack.h oml_vector.h oml_malloc.h
