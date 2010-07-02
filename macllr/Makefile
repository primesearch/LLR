# Makefile for MacIntel OS X llr or sllr

ENVP= MACOSX_DEPLOYMENT_TARGET=10.4

CC = gcc
LFLAGS = -Wl,-syslibroot,/Developer/SDKs/MacOSX10.4u.sdk -arch i386 -mmacosx-version-min=10.4
LIBMAC = ../gwnum/release/gwnum.a
LIBS   = $(LIBMAC) -lm -lstdc++
LIBS2  = $(LIBMAC) -lm -lstdc++ -static

FACTOROBJ = factor32p.o factor32lp.o
COMPOBJS = lprime.o lmenu.o
EXE = llr

# insert proper dummy file to get _GWDATA data segment aligned
# on a 32 byte boundary.  Use proper dummyNN.o to do the alignment

# DUMMY1   = ../gwnum/gwnum.ld
# DUMMY2   = ../gwnum/gwnum.ld

#########################################################################

all: gwnum $(EXE)

$(EXE): security  $(FACTOROBJ) $(COMPOBJS) $(LIBMAC)
	ar -x $(LIBMAC) multutil.o
	$(ENVP) $(CC) $(LFLAGS) -o $(EXE) $(FACTOROBJ) $(COMPOBJS) multutil.o $(LIBS)

security:
	[ ! -e ../security.h ] && touch ../security.h || true
	[ ! -e ../security.c ] && touch ../security.c || true

$(COMPOBJS):
	cd .. ; make -f makemac

gwnum:
	cd ../gwnum ; make -f makemac

clean:
	rm -f $(EXE) $(COMPOBJS)

allclean:
	rm -f $(EXE) $(COMPOBJS)
	cd ../gwnum ; make -f makemac clean
