# Makefile for Linux llr or sllr

CC = gcc
LFLAGS = -Wl
LIBELF = ../gwnum/gwnum.a
LIBS   = $(LIBELF) -lm -lstdc++ -lpthread
LIBS2  = $(LIBELF) -lm -lstdc++ -lpthread -static

FACTOROBJ = factor32p.o factor32lp.o
COMPOBJS = lprime.o lmenu.o
EXE = llr
EXE2 = sllr

# insert proper dummy file to get _GWDATA data segment aligned
# on a 32 byte boundary.  Use proper dummyNN.o to do the alignment

DUMMY1   = ../gwnum/gwnum.ld
DUMMY2   = ../gwnum/gwnum.ld

#########################################################################

all: gwnum $(EXE) $(EXE2)

$(EXE): security  $(FACTOROBJ) $(COMPOBJS) $(LIBELF)
	$(CC) $(LFLAGS) -o $(EXE) $(FACTOROBJ) $(COMPOBJS) $(DUMMY1) $(LIBS)

$(EXE2): security  $(FACTOROBJ) $(COMPOBJS) $(LIBELF)
	$(CC) $(LFLAGS) -o $(EXE2) $(FACTOROBJ) $(COMPOBJS) $(DUMMY2) $(LIBS2)

security:
	[ ! -e ../security.h ] && touch ../security.h || true
	[ ! -e ../security.c ] && touch ../security.c || true

$(COMPOBJS):
	cd .. ; make

gwnum:
	cd ../gwnum ; make

clean:
	rm -f $(EXE) $(EXE2) $(COMPOBJS)

allclean:
	rm -f $(EXE) $(EXE2) $(COMPOBJS)
	cd ../gwnum ; make clean
