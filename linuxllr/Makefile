# Makefile for Linux mprime
#
# NOTE: To convert the included object files to ELF, your binutils 
# installation must be able to read the coff-i386 object format.  
#
# Assuming your binutils are dynamically linked, you can add COFF support
# by configuring binutils with the switch '--enable-targets=i386-go32'
# and rebuilding libbfd.so .

CC = gcc
CFLAGS = -I.. -I../gwnum -O2 -march=i486 -malign-double

CPP = g++
CPPFLAGS = -I.. -I../gwnum -O2 -march=i486 -malign-double

LFLAGS = -Wl
LIBS   = ../gwnum/gwnum.a -lm -lstdc++
LIBS2  = ../gwnum/gwnum.a -lm -lstdc++ -static

FACTOROBJ = factor32p.o
DUMMYOBJS = dummy0.o dummy4.o dummy8.o dummy12.o dummy16.o dummy20.o dummy24.o dummy28.o
LINUXOBJS = lprime.o lmenu.o
EXE = llr
EXE2 = sllr

# insert proper dummy file to get _GWDATA data segment aligned
# on a 32 byte boundary.  Use proper dummyNN.o to do the alignment

DUMMY1   = ../gwnum/gwnum.ld
DUMMY2   = ../gwnum/gwnum.ld

#########################################################################

$(EXE): security  $(FACTOROBJ) $(LINUXOBJS) $(DUMMYOBJS)
	$(CC) $(LFLAGS) -o $(EXE) $(FACTOROBJ) $(LINUXOBJS) $(DUMMY1) $(LIBS)

$(EXE2): security  $(FACTOROBJ) $(LINUXOBJS) $(DUMMYOBJS)
	$(CC) $(LFLAGS) -o $(EXE2) $(FACTOROBJ) $(LINUXOBJS) $(DUMMY2) $(LIBS2)

security:
	[ ! -e ../security.h ] && touch ../security.h || true
	[ ! -e ../security.c ] && touch ../security.c || true

clean:
	rm -f $(EXE) $(EXE2) $(LINUXOBJS) $(FACTOROBJ) $(DUMMYOBJS)

$(FACTOROBJ) $(DUMMYOBJS): %.o : %.obj
	objcopy -Oelf32-i386 -Icoff-go32 $< $@

.c.o:
	$(CC) $(CFLAGS) -c $<

.cpp.o:
	$(CPP) $(CPPFLAGS) -c $<
