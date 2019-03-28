# Makefile for MacIntel OS X llr64

ENVP= MACOSX_DEPLOYMENT_TARGET=10.5

CC = gcc
LFLAGS = -Wl -arch x86_64 -mmacosx-version-min=10.5
LIBMAC = ../gwnum/amd64/release/gwnum.a
LIBS   = $(LIBMAC) ./libgmp.a -lm -lstdc++
LIBS2  = $(LIBMAC) ./libgmp.a -lm -lstdc++ -static

COMPOBJS = lprime.o lmenu.o
EXE = llr64

# insert proper dummy file to get _GWDATA data segment aligned
# on a 32 byte boundary.  Use proper dummyNN.o to do the alignment

# DUMMY1   = ../gwnum/gwnum.ld
# DUMMY2   = ../gwnum/gwnum.ld

#########################################################################

all: gwnum $(EXE)

$(EXE): security  $(COMPOBJS) $(LIBMAC)
	$(ENVP) $(CC) $(LFLAGS) -o $(EXE) $(COMPOBJS) $(LIBS)

security:
	[ ! -e ../security.h ] && touch ../security.h || true
	[ ! -e ../security.c ] && touch ../security.c || true

$(COMPOBJS):
	cd .. ; make -f makemac64

gwnum:
	cd ../gwnum ; make -f makemac

clean:
	rm -f $(EXE) $(COMPOBJS)

allclean:
	rm -f $(EXE) $(COMPOBJS)
	cd ../gwnum ; make -f makemac clean
