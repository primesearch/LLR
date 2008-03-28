# Makefile for Linux llr or sllr
#
# objconv comes courtesy of Agner Fog.  Download available at
# http://www.agner.org/optimize/#objconv

CV = objconv

LFLAGS = -Wl
LIBELF = ../gwnum/Linux/gwnum.a
LIBS   = $(LIBELF) -lm -lstdc++
LIBS2  = $(LIBELF) -lm -lstdc++ -static

FACTOROBJ = factor32p.o
DUMMYOBJS = dummy0.o dummy4.o dummy8.o dummy12.o dummy16.o dummy20.o dummy24.o dummy28.o
COMPOBJS = lprime.o lmenu.o
EXE = llr
EXE2 = sllr

# insert proper dummy file to get _GWDATA data segment aligned
# on a 32 byte boundary.  Use proper dummyNN.o to do the alignment

DUMMY1   = ../gwnum/gwnum.ld
DUMMY2   = ../gwnum/gwnum.ld

#########################################################################

$(EXE): security  $(FACTOROBJ) $(COMPOBJS) $(DUMMYOBJS) $(LIBELF)
	$(CC) $(LFLAGS) -o $(EXE) $(FACTOROBJ) $(COMPOBJS) $(DUMMY1) $(LIBS)

$(EXE2): security  $(FACTOROBJ) $(COMPOBJS) $(DUMMYOBJS) $(LIBELF)
	$(CC) $(LFLAGS) -o $(EXE2) $(FACTOROBJ) $(COMPOBJS) $(DUMMY2) $(LIBS2)

security:
	[ ! -e ../security.h ] && touch ../security.h || true
	[ ! -e ../security.c ] && touch ../security.c || true

$(COMPOBJS):
	cd .. ; make -f compile

$(LIBELF):
	cd ../gwnum/Linux ; make

clean:
	rm -f $(EXE) $(EXE2) $(COMPOBJS)

allclean:
	rm -f $(EXE) $(EXE2) $(COMPOBJS)
	cd ../gwnum/Linux ; make clean

$(FACTOROBJ) $(DUMMYOBJS): %.o : %.obj
#	objcopy -Oelf32-i386 -Icoff-go32 $< $@
	$(CV) -felf32 $< $@
