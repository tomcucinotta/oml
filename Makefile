PACKAGE:=oml
VERSION:=$(shell cat VERSION)
TEST_PROGS=$(patsubst %.c,%,$(wildcard test-*.c))
LIB_SRCS=$(patsubst test-%.c,,$(wildcard *.c oml_exceptions-dlex/*.c))
LIB_OBJS=$(patsubst %.c,%.o,$(LIB_SRCS))
OML_LIB=liboml.so
CFLAGS+=-Wall -Wformat -fPIC -fsanitize=address -std=gnu99 -I. -Ioml_exceptions-dlex
LDLIBS=-L. -loml -lrt

all: lib $(TEST_PROGS)

lib: $(OML_LIB)

TEST_LOG_FILE=tests-log.txt

test: $(TEST_PROGS)
	@$(foreach prog,$(TEST_PROGS),printf "%30s" "Running $(prog) ... " ; printf "\n%30s\n" "RUNNING TEST $(prog)" >> $(TEST_LOG_FILE); if LD_LIBRARY_PATH="$$LD_LIBRARY_PATH:." ./$(prog) >>$(TEST_LOG_FILE) 2>> $(TEST_LOG_FILE); then echo "Ok"; else echo "Failed (see $(TEST_LOG_FILE))"; fi; )

GEN_FILES:=oml_svector_find.h test-svector-find.c oml_list_find.h test-list-find.c
GEN_FILES:=$(patsubst %.h,%_namespace.h,$(filter-out %_namespace.h %.c,$(GEN_FILES) $(wildcard *.h)))

dump-vars:
	echo $(GEN_FILES)

gen: $(GEN_FILES)

oml_svector_find.h: oml_vector_find.h
	echo "/* DO NOT EDIT\n * THIS FILE IS AUTOMATICALLY GENERATED\n */\n\n" | cat - $< > $@
	replace.pl -r 's/_vector/_svector/g' $@
	replace.pl -r 's/_VECTOR/_SVECTOR/g' $@

test-svector-find.c: test-vector-find.c
	echo "/* DO NOT EDIT\n * THIS FILE IS AUTOMATICALLY GENERATED\n */\n\n" | cat - $< > $@
	replace.pl -r 's/_vector/_svector/g' $@
	replace.pl -r 's/_VECTOR/_SVECTOR/g' $@

oml_list_find.h: oml_vector_find.h
	echo "/* DO NOT EDIT\n * THIS FILE IS AUTOMATICALLY GENERATED\n */\n\n" | cat - $< > $@
	replace.pl -r 's/_vector/_list/g' $@
	replace.pl -r 's/_VECTOR/_LIST/g' $@

test-list-find.c: test-vector-find.c
	echo "/* DO NOT EDIT\n * THIS FILE IS AUTOMATICALLY GENERATED\n */\n\n" | cat - $< > $@
	replace.pl -r 's/_vector/_list/g' $@
	replace.pl -r 's/_VECTOR/_LIST/g' $@

.PHONY: doc
doc:
	doxygen

$(OML_LIB): $(LIB_OBJS)
	gcc -shared $^ -o $@

%_namespace.h: %.h
	@echo Generating $@...
	@echo "/* DO NOT EDIT: Automatically generated from Makefile */" > $@
	@echo "" >> $@
	@echo "#ifndef __$(shell echo $(@:.h=) | tr [:lower:] [:upper:])__" >> $@
	@echo "#  define __$(shell echo $(@:.h=) | tr [:lower:] [:upper:])__" >> $@
	@echo "#include \"$<\"" >> $@
	@echo "" >> $@
	@grep '^#define oml_[a-zA-Z_0-9]* ' $< | sed -e 's/#define oml_\([a-zA-Z_0-9]*\).*/#define \1 oml_\1/' >> $@
	@grep '^#define oml_[a-zA-Z_0-9]*(' $< | sed -e 's/#define oml_\([a-zA-Z_0-9]*\)\((.*)\).*/#define \1\2 oml_\1\2/' >> $@
	@echo "" >> $@
	@echo "#endif /* __$(shell echo $(@:.h=) | tr [:lower:] [:upper:])__ */ " >> $@

test-%: test-%.o
	gcc $(CFLAGS) $< -o $@ $(LDLIBS)

%.o: %.c
	gcc $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(OML_LIB) $(TEST_PROGS) $(GEN_FILES) *.o *~ *.bak *.d

.PHONY: distclean
distclean: clean
	rm -rf $(shell find . -type d -name CVS)

.PHONY: ChangeLog
ChangeLog:
	cvs2cl -P

README: VERSION
	sed -i -e 's/Version .*/Version '`cat VERSION`'/' README

dist: dist-src

dist-src: README
	rm -rf /tmp/$(PACKAGE)-$(VERSION) && mkdir /tmp/$(PACKAGE)-$(VERSION) && cp -r * /tmp/$(PACKAGE)-$(VERSION) && cd /tmp/$(PACKAGE)-$(VERSION) && make distclean && cd .. && tar -czf $(PACKAGE)-$(VERSION).tar.gz $(PACKAGE)-$(VERSION) && echo "Distribution archive in /tmp/$(PACKAGE)-$(VERSION).tar.gz"

.PHONY: dep
dep:
	makedepend -- -Y *.c

# DO NOT DELETE

oml_debug.o: oml_debug.h
oml_malloc.o: oml_debug.h oml_malloc.h
oml_scope.o: oml_scope.h
test-class-serialize.o: test-myclass.h
test-dltree.o: oml_debug.h oml_dltree.h oml_malloc.h
test-eheap.o: oml_debug.h oml_eheap.h oml_malloc.h oml_common.h
test-func.o: oml_debug.h oml_func.h
test-heap.o: oml_debug.h oml_heap.h oml_malloc.h oml_common.h
test-list.o: oml_debug.h oml_list.h oml_malloc.h oml_list_find.h oml_common.h
test-lstack.o: oml_debug.h oml_lstack.h oml_list.h oml_malloc.h
test-lstack.o: oml_list_find.h oml_common.h
test-map.o: oml_debug.h oml_map.h oml_malloc.h oml_common.h oml_pair.h
test-map.o: oml_list.h oml_list_find.h
test-oml-scope.o: oml_scope.h
test-pair.o: oml_debug.h oml_pair.h oml_common.h
test-queue.o: oml_debug.h oml_vqueue.h oml_malloc.h
test-scope.o: oml_scope.h
test-scoped-list.o: oml_scoped_list.h oml_scope.h oml_list.h oml_debug.h
test-scoped-list.o: oml_malloc.h oml_list_find.h oml_common.h
test-set.o: oml_debug.h oml_set.h oml_malloc.h oml_common.h oml_list.h
test-set.o: oml_list_find.h oml_dump.h oml_foreach.h
test-svector.o: oml_debug.h oml_svector.h oml_malloc.h
test-sync-nest.o: oml_debug.h
test-sync.o: oml_sync.h oml_debug.h oml_func.h
test-theap.o: oml_debug.h oml_theap.h oml_malloc.h oml_common.h
test-vector-find.o: oml_debug.h oml_vector_find.h oml_vector.h oml_malloc.h
test-vector-find.o: oml_common.h
test-vector.o: oml_debug.h oml_vector.h oml_malloc.h test-vector-factory.h
test-vstack.o: oml_debug.h oml_vstack.h oml_vector.h oml_malloc.h
