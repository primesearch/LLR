# Makefile for Linux llr or sllr

LFLAGS = -Wl
LIBELF = ../gwnum/linux64/gwnum.a
LIBS   = $(LIBELF) ../gwnum/gwnum.ld -lm  -lpthread -lstdc++
LIBS2  = $(LIBELF) ../gwnum/gwnum.ld -lm  -lpthread -lstdc++ -static

FACTOROBJ = factor64p.o factor64lp.o
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
	cd .. ; make -f make64

gwnum:
	cd ../gwnum ; make -f make64

clean:
	rm -f $(EXE) $(EXE2) $(COMPOBJS)

allclean:
	rm -f $(EXE) $(EXE2) $(COMPOBJS)
	cd ../gwnum ; make -f make64 clean
