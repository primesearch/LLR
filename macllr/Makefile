# Makefile for MacIntel OS X llr or sllr

ENVP= MACOSX_DEPLOYMENT_TARGET=10.3

CC = gcc
LFLAGS = -Wl
LIBMAC = ../gwnum/macosx/gwnum.a
LIBS   = $(LIBMAC) -lm -lstdc++
LIBS2  = $(LIBMAC) -lm -lstdc++ -static

FACTOROBJ = factor32p.o factor32lp.o
COMPOBJS = lprime.o lmenu.o
EXE = llr
EXE2 = sllr

# insert proper dummy file to get _GWDATA data segment aligned
# on a 32 byte boundary.  Use proper dummyNN.o to do the alignment

# DUMMY1   = ../gwnum/gwnum.ld
# DUMMY2   = ../gwnum/gwnum.ld

#########################################################################

all: gwnum $(EXE) $(EXE2)

$(EXE): security  $(FACTOROBJ) $(COMPOBJS) $(LIBMAC)
	ar -x $(LIBMAC) multutil.o
	$(ENVP) $(CC) $(LFLAGS) -o $(EXE) $(FACTOROBJ) $(COMPOBJS) multutil.o $(LIBS)

$(EXE2): security  $(FACTOROBJ) $(COMPOBJS) $(LIBMAC)
	ar -x $(LIBMAC) multutil.o
	$(ENVP) $(CC) $(LFLAGS) -o $(EXE2) $(FACTOROBJ) $(COMPOBJS) multutil.o $(LIBS2)

security:
	[ ! -e ../security.h ] && touch ../security.h || true
	[ ! -e ../security.c ] && touch ../security.c || true

$(COMPOBJS):
	cd .. ; make -f makemac

gwnum:
	cd ../gwnum ; make -f makemac

clean:
	rm -f $(EXE) $(EXE2) $(COMPOBJS)

allclean:
	rm -f $(EXE) $(EXE2) $(COMPOBJS)
	cd ../gwnum ; make -f makemac clean
