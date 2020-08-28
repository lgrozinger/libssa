CC=gcc
CFLAGS=-Wall -g -fPIC -O2 -I. -Iinclude
LDFLAGS=-lm -lgsl -lgslcblas
UNITS = dg ll pq heap dm dmd nrm nrmd ssa
OBJS = $(patsubst %, build/%.o, $(UNITS)) 
VPATH = src/
PREFIX = /usr/local/lib


build/%.o:%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(LDFLAGS)

build : $(OBJS)
	$(CC) -shared -o libssa.so $(OBJS) $(LDFLAGS)

install : build
	-rm $(PREFIX)/libssa.so
	ln -s $(shell pwd)/libssa.so $(PREFIX)/libssa.so

.PHONY : test
test :
	$(CC) $(CFLAGS) -o build/check_ll test/check_ll.c src/ll.c -lcheck -lm -lrt -lpthread -lsubunit
	$(CC) $(CFLAGS) -o build/check_pq test/check_pq.c src/pq.c src/ll.c -lcheck -lm -lrt -lpthread -lsubunit
	$(CC) $(CFLAGS) -o build/check_ssa test/check_ssa.c src/ssa.c -lcheck -lm -lrt -lpthread -lsubunit
	$(CC) $(CFLAGS) -o build/check_heap test/check_heap.c src/heap.c -lcheck -lm -lrt -lpthread -lsubunit	
	./build/check_ll
	./build/check_pq
	./build/check_ssa
	./build/check_heap

.PHONY : clean
clean :
	-rm -r -f build/*.o
	-rm -r -f build/check_ll build/check_heap build/check_pq build/check_ssa build/check_heap
	-rm -f libssa.o
