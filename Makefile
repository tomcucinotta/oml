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

GEN_FILES=oml_svector_find.h test-svector-find.c oml_list_find.h test-list-find.c

gen: $(GEN_FILES)

oml_svector_find.h: oml_vector_find.h
	cp $< $@
	replace.pl -r 's/_vector/_svector/g' $@
	replace.pl -r 's/_VECTOR/_SVECTOR/g' $@

test-svector-find.c: test-vector-find.c
	cp $< $@
	replace.pl -r 's/_vector/_svector/g' $@
	replace.pl -r 's/_VECTOR/_SVECTOR/g' $@

oml_list_find.h: oml_vector_find.h
	cp $< $@
	replace.pl -r 's/_vector/_list/g' $@
	replace.pl -r 's/_VECTOR/_LIST/g' $@

test-list-find.c: test-vector-find.c
	cp $< $@
	replace.pl -r 's/_vector/_list/g' $@
	replace.pl -r 's/_VECTOR/_LIST/g' $@

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
	rm -f $(OML_LIB) $(TEST_PROGS) $(GEN_FILES) *.o *~ *.bak *.d

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
test-heap.o: oml_debug.h oml_heap.h oml_malloc.h oml_common.h
test-list.o: oml_debug.h oml_list.h oml_malloc.h
test-list-find.o: oml_debug.h oml_list_find.h oml_list.h oml_malloc.h
test-list-find.o: oml_common.h
test-lstack.o: oml_debug.h oml_lstack.h oml_list.h oml_malloc.h
test-map.o: oml_debug.h oml_map.h oml_malloc.h oml_common.h oml_pair.h
test-map.o: oml_list.h oml_list_find.h
test-queue.o: oml_debug.h oml_vqueue.h oml_malloc.h
test-svector-find.o: oml_debug.h oml_svector_find.h oml_svector.h
test-svector-find.o: oml_malloc.h oml_common.h
test-theap.o: oml_debug.h oml_theap.h oml_malloc.h oml_common.h
test-vector.o: oml_debug.h oml_vector.h oml_malloc.h
test-vector-find.o: oml_debug.h oml_vector_find.h oml_vector.h oml_malloc.h
test-vector-find.o: oml_common.h
test-vstack.o: oml_debug.h oml_vstack.h oml_vector.h oml_malloc.h
