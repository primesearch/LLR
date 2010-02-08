# Compile the LLR source files for Linux 32 bits architecture

D = ./linuxllr

CC = gcc
CFLAGS = -I. -I./gwnum -O2 -march=i486 -malign-double

CPP = g++
CPPFLAGS = -I. -I./gwnum -O2 -march=i486 -malign-double

COMPOBJS = lprime.o lmenu.o

compil: $(COMPOBJS)
	mv $(COMPOBJS) $(D)

.c.o:
	$(CC) $(CFLAGS) -c $<

.cpp.o:
	$(CPP) $(CPPFLAGS) -c $<
