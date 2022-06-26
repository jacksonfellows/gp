CFLAGS = -Wall -Werror -g

all: libgp.dylib pygp

libgp.dylib: gp.o
	cc $(CFLAGS) -dynamiclib $^ -o $@

gp.o: gp.c gp.h
	cc $(CFLAGS) -O3 -ffast-math -c gp.c

gp_pre.h: gp.h
	cc $(CFLAGS) -E gp.h -o gp_pre.h

pygp: gp_pre.h
	python3 build_gp.py
