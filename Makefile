CC=gcc
CFLAGS=-Wall -g -fPIC -I. -Ilib/include
LDFLAGS=-lm -lgsl -lgslcblas
UNITS = graph ll pq dm nrm ssa
HEADERS = $(patsubst %, include/%.h, $(UNITS))
OBJS = $(patsubst %, build/%.o, $(UNITS)) 
VPATH = src/ include/
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
	$(CC) $(CFLAGS) -o check_ll test/check_ll.c src/ll.c -lcheck -lm -lrt -lpthread -lsubunit
	$(CC) $(CFLAGS) -o check_pq test/check_pq.c src/pq.c src/ll.c -lcheck -lm -lrt -lpthread -lsubunit	
	./check_ll
	./check_pq

.PHONY : clean
clean :
	-rm -r -f build/*.o
	-rm -f libssa.o
