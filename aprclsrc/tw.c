#include <stdio.h>
#include <stdlib.h>
#include "gmp.h"
#include "mpz_aprcl.h"

// Wieferich prime test executable, to be called by LLR as a child process.

int main (int argc, char **argv) {
	int retcode = 0, retcode2 = 0;
	mpz_t n;
	mpz_t pwn;
	mpz_t res;
	mpz_t nm1;
	mpz_t a;

	if (argc < 2)
		return (10);		// No string argument...
	if (mpz_init_set_str (n, argv[1], 10) != 0) {
		mpz_clear (n);
		return (11);		// Invalid numeric string...
	}
	if (argc < 3) {			// No more parameters
		mpz_init_set_ui (a, 2);
	}
	else if (mpz_init_set_str (a, argv[2], 10) != 0) {
		mpz_clear (n);
		mpz_clear (a);
		return (12);		// Invalid numeric string...
	}
	mpz_init_set_ui(res, 1);	// Init res to 1
	mpz_init_set(nm1, n);
	mpz_init_set(pwn, n);		// init pwn to n^1 = n
	mpz_sub_ui(nm1, nm1, 1);	// compute n-1
	while (mpz_cmp_ui(res, 1) == 0) {
		mpz_powm(res, a, nm1, pwn);	// Compute a^(n-1) modulo pwn
		if (mpz_cmp_ui(res, 1) == 0)	// May be positive...
			retcode2++;
		mpz_mul (pwn, pwn, n);		// Set pwn to the next power of n.
	}
	retcode = retcode2;			// Default return...
	if (retcode2 >= 2) {			// May be positive...
		retcode = mpz_aprcl (n);	// Prime test, without any message
		if (retcode == 0)
			retcode = -retcode2;	// W-positive, but composite...
		else if (retcode != -1)		// No APRCL error...
			retcode = retcode2;	// Wieferich prime found if code >= 2
	}
	mpz_clear (n);
	mpz_clear (a);
	mpz_clear (res);
	mpz_clear (nm1);
	mpz_clear (pwn);
	return (retcode);
}
