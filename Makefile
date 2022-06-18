CFLAGS = -Wall -Werror -g

all: libgp.dylib pygp

libgp.dylib: gp.o
	cc $(CFLAGS) -dynamiclib $^ -o $@

gp.o: gp.c gp.h
	cc $(CFLAGS) -O3 -ffast-math -c gp.c

pygp:
	python3 build_gp.py
