CC = gcc
CFLAGS = -Wall -Wextra -Wpointer-arith -Wstrict-prototypes -std=gnu89 -fPIC -MMD -MP
LDFLAGS = -shared

#added
LIBS=-lpthread

all: libmylab.so mytests

#This builds a shared library.
libmylab.so: lab.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

#This builds an executable it depends on the library
#added $(LIBS)
mytests: mytests.o libmylab.so
	$(CC) $(CFLAGS) $(LIBS) $< -L. -Wl,-rpath=. -lmylab -o $@



.PHONY: clean
clean:
	$(RM) *.o *.d *.gcno *.gcda *.gcov libmylab.so mytests

-include *.d
