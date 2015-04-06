# Compile the APRCL source files for Mac 64 bits architecture

D = ../

CC = gcc
CFLAGS = -I../ -O2 -Wl,-syslibroot,/Developer/SDKs/MacOSX10.5.sdk -arch x86_64 -mmacosx-version-min=10.5
sources = $(D)/aprcl.c $(D)/mpz_aprcl.c
LIBS = libgmp.a
EXE = aprcl

$(EXE): $(sources)
	$(CC) $(CFLAGS) -o $(EXE) $(sources) $(LIBS)

clean:
	rm $(EXE)
