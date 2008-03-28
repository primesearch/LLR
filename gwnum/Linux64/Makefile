# Makefile for Linux64 gwnum library
#

ELFOBJS = mult.o xmult1.o xmult1ax.o xmult2.o xmult2a.o xmult2ax.o xmult3.o xmult3a.o xmult4.o xmult5.o xmult6.o xmult2p.o xmult2ap.o xmult3p.o xmult3ap.o xmult4p.o xmult5p.o xmult6p.o gianthlp.o cpuidhlp.o timeit.o timeitp.o

COMPOBJS = cpuid.o gwnum.o gwutil.o gwdbldbl.o giants.o ecmstag1.o

LIB = gwnum64.a

#########################################################################

$(LIB): $(COMPOBJS) $(ELFOBJS)
	rm -f $(LIB)
	ar -qs $(LIB) $(COMPOBJS) $(ELFOBJS)

clean:
	rm -f $(LIB) $(COMPOBJS)

$(ELFOBJS):
	cd ../amd64 ; make -f Makeelf64

$(COMPOBJS):
	cd .. ; make -f Makecomp64