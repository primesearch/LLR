# Compile the LLR source files for Linux 32 bits architecture

D = ./linuxllr

CC = gcc
CFLAGS = -I. -I./gwnum -O2 -march=i486 -malign-double

CPP = g++
CPPFLAGS = -I. -I./gwnum -O2 -march=i486 -malign-double

COMPOBJS = lprime.o lmenu.o

compile: $(COMPOBJS)
	mv $(COMPOBJS) $(D)

lprime.o: lprime.c
	$(CC) $(CFLAGS) -c lprime.c

lmenu.o: lmenu.c
	$(CC) $(CFLAGS) -c lmenu.c

.cpp.o:
	$(CPP) $(CPPFLAGS) -c $<
