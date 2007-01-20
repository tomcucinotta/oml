TEST_PROGS=$(patsubst %.c,%,$(wildcard test-*.c))
LIB_SRCS=$(patsubst test-%.c,,$(wildcard *.c))
LIB_OBJS=$(patsubst %.c,%.o,$(LIB_SRCS))
OML_LIB=liboml.so
CFLAGS=-Wall

all: $(OML_LIB) $(TEST_PROGS)

$(OML_LIB): $(LIB_OBJS)
	gcc -shared $^ -o $@

test-%: test-%.o
	gcc $(CFLAGS) -L. -loml $< -o $@

%.o: %.c
	gcc $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OML_LIB) $(TEST_PROGS) *.o *~ *.bak *.d

vars:
	echo TEST_PROGS=$(TEST_PROGS)
	echo LIB_SRCS=$(LIB_SRCS)
	echo LIB_OBJS=$(LIB_OBJS)

# DO NOT DELETE

oml_debug.o: oml_debug.h
oml_malloc.o: oml_debug.h oml_malloc.h
test-eheap.o: oml_debug.h oml_eheap.h oml_malloc.h oml_common.h
test-theap.o: oml_theap.h oml_debug.h
