# Compile the APRCL source files for Linux 64 bits architecture

D = ../

CC = gcc
CFLAGS = -I../ -O2 -static
sources = $(D)/aprcl.c $(D)/mpz_aprcl.c
LIBS = libgmp.a
EXE = aprcl

$(EXE): $(sources)
	$(CC) $(CFLAGS) -o $(EXE) $(sources) $(LIBS)

clean:
	rm $(EXE)
