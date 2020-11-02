# Makefile for Linux llr64 or sllr64

CC = gcc

LFLAGS =
LIBELF = ../gwnum/gwnum.a ./libgmp.a ./libhwloc.a
LIBS   = $(LIBELF)  ../gwnum/gwnum.ld -lm  -lpthread -lstdc++
LIBS2  = $(LIBELF)  ../gwnum/gwnum.ld -lm  -lpthread -lstdc++ -static

FACTOROBJ = factor64p.o factor64lp.o
COMPOBJS = lprime.o lmenu.o
EXE = llr64
EXE2 = sllr64

# insert proper dummy file to get _GWDATA data segment aligned
# on a 32 byte boundary.  Use proper dummyNN.o to do the alignment

DUMMY1   = ../gwnum/gwnum.ld
DUMMY2   = ../gwnum/gwnum.ld

#########################################################################

all: gwnum $(EXE) $(EXE2)

$(EXE): security  $(COMPOBJS) $(LIBELF)
	$(CC) $(LFLAGS) -o $(EXE) $(COMPOBJS) $(DUMMY1) $(LIBS)

$(EXE2): security $(COMPOBJS) $(LIBELF)
	$(CC) $(LFLAGS) -o $(EXE2) $(COMPOBJS) $(DUMMY2) $(LIBS2)

security:
	[ ! -e ../security.h ] && touch ../security.h || true
	[ ! -e ../security.c ] && touch ../security.c || true

$(COMPOBJS):
	cd .. ; make -f make64

gwnum:
	cd ../gwnum ; make -f make64

clean:
	rm -f $(EXE) $(EXE2) $(COMPOBJS)

allclean:
	rm -f $(EXE) $(EXE2) $(COMPOBJS) $(LIBELF)
	cd ../gwnum ; make -f make64 clean
