# Compile the APRCL source files for Windows 32 bits architecture (cygwin)

D = ../

CC = gcc
CFLAGS = -I../ -O2
sources = $(D)/aprcl.c $(D)/mpz_aprcl.c
LIBS = libgmp.a
EXE = aprcl.exe

$(EXE): $(sources)
	$(CC) $(CFLAGS) -o $(EXE) $(sources) $(LIBS)

clean:
	rm $(EXE)
