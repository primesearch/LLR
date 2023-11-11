# Makefile for MacIntel OS X llr

ENVP= MACOSX_DEPLOYMENT_TARGET=10.5

CC = gcc
LFLAGS = -Wl -mmacosx-version-min=10.5 -arch i386
LIBMAC = ../gwnum/gwnum.a # JP 08/11/23
LIBS   = $(LIBMAC) ./libgmp.a -lm ./libhwloc.a -lstdc++
LIBS2  = $(LIBMAC) ./libgmp.a -lm -lstdc++ -static

FACTOROBJ = factor32p.o factor32lp.o
COMPOBJS = lprime.o lmenu.o
EXE = llr

# insert proper dummy file to get _GWDATA data segment aligned
# on a 32 byte boundary.  Use proper dummyNN.o to do the alignment

# DUMMY1   = ../gwnum/gwnum.ld
# DUMMY2   = ../gwnum/gwnum.ld

#########################################################################

all: gwnum $(EXE)

$(EXE): security  $(COMPOBJS) $(FACTOROBJ) $(LIBMAC)
	$(ENVP) $(CC) $(LFLAGS) -o $(EXE) $(COMPOBJS) $(FACTOROBJ) $(LIBS)

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
