#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <fcntl.h>
#include "gmp.h"
#include "mpz_aprcl.h"

// Wieferich prime search stand alone executable for Windows LLR.

#define TRUE 1
#define FALSE 0
#if defined (__linux__) || defined (__FreeBSD__) || defined (__APPLE__)
#define _O_APPEND	O_APPEND
#define _O_RDONLY	O_RDONLY
#define _O_WRONLY	O_WRONLY
#define _O_RDWR		O_RDWR
#define _O_CREAT	O_CREAT
#define _O_TRUNC	O_TRUNC
#define _O_BINARY 	0
#define _O_TEXT		0
#define _read read
#define _write write
#define _open open
#define _close close
#define _unlink unlink
#endif
unsigned long m = 0, phi = 1, prevphi = 0, i = 0, j = 0, ndp=9, np, startmod;
unsigned long smallprime[168] = 
{2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,
73,79,83,89,97,101,103,107,109,113,127,131,137,139,149,151,157,163,167,173,
179,    181,    191,    193,    197,    199,    211,    223,    227,    229,
233,    239,    241,    251,    257,    263,    269,    271,    277,    281,
283,    293,    307,    311,    313,    317,    331,    337,    347,    349,
353,    359,    367,    373,    379,    383,    389,    397,    401,    409,
419,    421,    431,    433,    439,    443,    449,    457,    461,    463,
467,    479,    487,    491,    499,    503,    509,    521,    523,    541,
547,    557,    563,    569,    571,    577,    587,    593,    599,    601,
607,    613,    617,    619,    631,    641,    643,    647,    653,    659,
661,    673,    677,    683,    691,    701,    709,    719,    727,    733,
739,    743,    751,    757,    761,    769,    773,    787,    797,    809,
811,    821,    823,    827,    829,    839,    853,    857,    859,    863,
877,    881,    883,    887,    907,    911,    919,    929,    937,    941,
947,    953,    967,    971,    977,    983,    991,    997};
unsigned long smallphi[11] = {1,2,4,6,10,12,16,18,22,28,30};
int resaprcl, stopping;
int volatile THREAD_STOP = 0;
int volatile THREAD_KILL = 0;
char lbuf [1000] = {0}, lbuf2 [1000] = {0}, sresume[1000] = {0};
char TEMP_FILE[80] = {0}, RESULT_FILE[80] = {0}, MSG_FILE[] = "wfmsg.txt";

/* Signal handlers */

void sigterm_handler(int signo)
{
	THREAD_STOP = TRUE;
	if (signo != SIGINT) THREAD_KILL = TRUE;
	(void)signal(signo, sigterm_handler);
}

/* Return TRUE if we should stop calculating */

int escapeCheck ()
{
	if (THREAD_STOP) {
		THREAD_STOP = FALSE;
		return (TRUE);
	}
	return (FALSE);
}

void OutputStr (char *buf)
{
	printf ("%s", buf);
}

/* Open the results file and write a line to the end of it. */

int writeResults (
	char	*msg)
{
static	time_t	last_time = 0;
	time_t	this_time;
	int	fd, writelg;

/* Open file, position to end */

	fd = _open (RESULT_FILE, _O_TEXT | _O_RDWR | _O_CREAT | _O_APPEND, 0666);
	if (fd < 0) {
		OutputStr ("Error opening the results file.\n");
		return (FALSE);
	}

/* If it has been at least 5 minutes since the last time stamp */
/* was output, then output a new timestamp */

	time (&this_time);
	if (this_time - last_time > 300) {
		char	buf[32];
		last_time = this_time;
		buf[0] = '[';
		strcpy (buf+1, ctime (&this_time));
		buf[25] = ']';
		buf[26] = '\n';
		writelg = _write (fd, buf, 27);
	}

/* Output the message */

	if (_write (fd, msg, strlen (msg)) < 0) goto fail;
	_close (fd);
	return (TRUE);

/* On a write error, close file and return error flag */

fail:	_close (fd);
	return (FALSE);
}

/* Output string to both the screen and results file */

void OutputBoth (
	char	*buf)
{
	OutputStr (buf);
	writeResults (buf);
}

void	trace(int n) {			// Debugging tool...
	char buf[100];
	sprintf(buf, "OK until number %d\n", n);
	OutputStr (buf); 	
}

/* Generate temporary file name */
 
void tempFilename ( 
	char	*buf, char* c, mpz_t NN) 
{ 
	unsigned long remainder;
 
	remainder = mpz_fdiv_ui (NN, 19999981);
	sprintf (buf, "%s%07lu.txt", c, remainder % 10000000); 
} 

/* See if the given file exists */

int fileExists (
	char	*filename)
{
	int	fd;
	fd = _open (filename, _O_RDONLY | _O_BINARY);
	if (fd < 0) return (FALSE);
	_close (fd);
	return (TRUE);
}

int readFile (
	char *data, char	*filename)
{
	int	fd;
	fd = _open (filename, _O_RDONLY | _O_BINARY);
	if (fd < 0) return (FALSE);
	if (!_read (fd, data, 256))
		return (FALSE);
	_close (fd);
	return (TRUE);
}

int writeFile (
	char	*msg, char *filename)
{
	int	fd, writelg;

/* Open file, position to end */

	fd = _open (filename, _O_TEXT | _O_RDWR | _O_CREAT, 0666);
	if (fd < 0) {
		OutputStr ("Error opening the file.\n");
		return (FALSE);
	}

/* Output the message */

	if (_write (fd, msg, strlen (msg)) < 0) goto fail;
	_close (fd);
	return (TRUE);

/* On a write error, close file and return error flag */

fail:	_close (fd);
	return (FALSE);
}

void	presieve (unsigned long ndp, unsigned long *t, unsigned long *ta, unsigned long *pm, unsigned long *pphi)
{
	unsigned long n, i, j, ind, m, mm, phi, p, div;
	m = phi = 1;
	t[0] = 2;
	for (n=0; n<ndp; n++) {
		for (i=0; i<phi; i++)
			ta[i] = t[n+i];
		ind = n;
		mm = 0;
		p = t[n];
		for (i=0; i<p; i++) {
			for (j=0; j<phi; j++) {
				div = ta[j] + mm;
				if (!(div>p) || (div%p))
					t[ind++] = div;
			}
			mm += m;
		}
		m *= p;
		phi *= (p-1);
	}
	*pm = m;
	*pphi = phi;
}

int main (int argc, char **argv) {
	int retcode = 0, last_i = 0;
	static time_t last_time = 0;
	time_t this_time;
	unsigned long b;
	mpz_t start;
	mpz_t stop;
	mpz_t n;
	mpz_t sqn;
	mpz_t res;
	mpz_t nm1;
	mpz_t a;
	unsigned long *t = NULL,*ta = NULL;	// Precrible arrays, dynamically allocated.

/* catch termination signals */

	(void)signal(SIGTERM, sigterm_handler);
	(void)signal(SIGINT, sigterm_handler);


	if (argc < 2) {
		return (3);			// No string argument...
	}
	if (mpz_init_set_str (start, argv[1], 10) != 0) {
		return (4);			// Invalid numeric string...
	}
	if (argc < 3) {
		return (3);			// No string argument...
	}
	if (mpz_init_set_str (stop, argv[2], 10) != 0) {
		return (4);			// Invalid numeric string...
	}
	if (argc < 4) {			// No more parameters
		mpz_init_set_ui (a, 2);
	}
	else if (mpz_init_set_str (a, argv[3], 10) != 0) {
		return (5);			// Invalid numeric string...
	}
	mpz_get_str (lbuf2, 10, a);
	sprintf (TEMP_FILE, "wf%s_%s.txt", lbuf2, argv[1]);
	sprintf (RESULT_FILE,"wf%s_results.txt", lbuf2);
	if (fileExists (TEMP_FILE)) {
		readFile (sresume, TEMP_FILE);
		mpz_set_str (start, sresume, 10);
	}
	for (np=ndp;np>=3;np--) {
		for (i=0;i<np;i++) {
			prevphi = phi;
			phi *= smallphi[i];
		}
		t = (unsigned long*)realloc(t, (phi+10)*sizeof(unsigned long));
		ta = (unsigned long*)realloc(ta, (prevphi+10)*sizeof(unsigned long));
		if ((t==NULL)||(ta==NULL)) {
			phi = 1;
			prevphi = 0;
		}
		else {
			break;
		}
	}
	mpz_init (n);
	mpz_init (sqn);
	mpz_init (nm1);
	mpz_init (res);
	b = mpz_get_ui (a);
	if (mpz_cmp_ui (start, smallprime[np-1]) <= 0) {
		for (i=0;i<np;i++) {				// Test the small primes
			mpz_set_ui (n, smallprime[i]);
			if (mpz_cmp (n, start) <= 0)		// Small prime already tested
				continue;
			if (mpz_cmp(n, stop) <= 0) {
				mpz_set(nm1, n);
				mpz_set(sqn, n);
				mpz_mul (sqn, n, n);		// Set sqn to the square of n.
				mpz_sub_ui(nm1, nm1, 1);
				mpz_powm(res, a, nm1, sqn);
				if (mpz_cmp_ui(res, 1) == 0) {// Positive result!
					mpz_get_str (lbuf2, 10, n);
					writeFile (lbuf2, TEMP_FILE);
					writeFile (lbuf2, RESULT_FILE);
					return (2);
				}
			}
			else {
				return (0);
			}
		}
	}
	presieve (np, t, ta, &m, &phi);
	startmod = mpz_fdiv_ui (start, m);
	mpz_sub_ui (start, start, startmod);
	for (i = np; i<(np+phi); i++) {
		if (t[i] >= startmod)
			break;
	}
	mpz_get_str (lbuf2, 10, start);
	last_i = 0;
	for (i; ; i++) {
		if (i>=(np+phi)) {			// End of loop on the sieve
			mpz_get_str (lbuf2, 10, n);
			resaprcl = mpz_aprcl (n);	// Prime test, without any message
			writeFile (lbuf2, TEMP_FILE);	// Save the checkpoint
			if (resaprcl == 2)
				return (9);
			else
				return (8);
		}
		mpz_set (n, start);
		mpz_add_ui (n, n, t[i]);
		if (mpz_cmp(n, stop) <= 0) {
			for (j=np; j<45;j++) {
				if (!mpz_fdiv_ui (n, smallprime[j]))
					break;
			}
			if (j<45)
				continue;
			if ((i - last_i) > 100000) {
				last_i = i;
				time (&this_time);
				if (this_time - last_time > 10) {
					last_time = this_time;
					mpz_get_str (lbuf2, 10, n);
					writeFile (lbuf2, TEMP_FILE);
				}
			}
			stopping = escapeCheck ();
			if (stopping) {
				mpz_get_str (lbuf2, 10, n);
				writeFile (lbuf2, TEMP_FILE);
				return (7);
			}
			mpz_set(nm1, n);
			mpz_sub_ui(nm1, nm1, 1);
			mpz_powm(res, a, nm1, n);		// res = a^nm1 (mod. n)
			if (mpz_cmp_ui(res, 1) != 0)
				continue;				// n is composite...
			else {					// n may be prime, test further
				mpz_set(sqn, n);
				mpz_mul (sqn, n, n);		// Set sqn to the square of n.
				mpz_powm(res, a, nm1, sqn);	// res = a^nm1 (mod. sqn)
			}
			if (mpz_cmp_ui(res, 1) == 0) {	// May be positive...
				mpz_get_str (lbuf2, 10, n);
				writeFile (lbuf2, RESULT_FILE);// Save the result
				retcode = mpz_aprcl (n);	// Prime test, without any message
				mpz_add_ui(nm1, nm1, 3);	// Compute n+2
				mpz_get_str (lbuf2, 10, nm1);
				writeFile (lbuf2, TEMP_FILE);	// Make a checkpoint
				if (retcode == -1) {
					return (6);		// The test is in error...
				}
				else if (retcode == 0) {
					return (1);		// W-positive, but composite...
				}
				else {
					return (retcode);
				}
			}
			else
				continue;
		}
		else
			break;
	}
	return (0);
}
