#include <stdio.h>
#include <stdlib.h>
#include "gmp.h"
#include "mpz_aprcl.h"

// APRCL prime test executable, to be called by LLR as a child process.

int main (int argc, char **argv) {
	char buf[10000]={0};
	int retcode, prptest = 0, verbose = 0;
	FILE *fd;
	mpz_t n;

	if (argc < 2)
		return (3);			// No filename argument...
	if ((fd = fopen (argv[1], "r")) == NULL)
		return (4);			// Could not open the file...
	if (fgets (buf, 10000, fd) == NULL)
		return (5);			// Unexpected EOF...
	mpz_init (n);
	if (mpz_set_str (n, buf, 10) != 0)
		return (6);			// Invalid numeric string...
	if (argc < 3) {			// No more parameters
		retcode = mpz_aprcl (n);// Prime test, without any message
		return ((retcode == -1)? 7 : retcode);
						// Result code returned (-1 if in error...)
	}
	if (!strcmp(argv[2],"prp"))	// Strong BPSW PRP test only required...
		prptest = 1;
	if (argc >= 4) {
		if (!strcmp(argv[3], "progress"))
			verbose = APRTCLE_VERBOSE1;
		else if (!strcmp(argv[3], "details"))
			verbose = APRTCLE_VERBOSE2;
		else
			verbose = APRTCLE_VERBOSE0;
	}
	if (prptest)
		retcode = mpz_strongbpsw_prp (n);	// Strong BPSW PRP test
	else
		retcode = mpz_aprtcle (n, verbose);	// Prime test possibly with print out.
	return ((retcode == -1)? 7 : retcode);	// Result code returned (-1 if in error...)
}
