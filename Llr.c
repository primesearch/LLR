/*----------------------------------------------------------------------
| This file contains routines and global variables that are common for
| all operating systems the program has been ported to.  It is included
| in one of the source code files of each port.  See Llr.h for the
| common #defines and common routine definitions.
+---------------------------------------------------------------------*/
 
#define CHECK_IF_ANY_ERROR(X,J,N,K) \
		checknumber = K;\
		restarting = FALSE;\
		will_try_larger_fft = FALSE;\
/* If the sum of the output values is an error (such as infinity) */\
/* then raise an error. */\
\
		if (gw_test_illegal_sumout (gwdata)) {\
			sprintf (buf, ERRMSG0L, J, N, ERRMSG1A);\
			clearline(100);\
			OutputBoth (buf);\
			sleep5 = TRUE;\
			goto error;\
		}\
\
/* Check that the sum of the input numbers squared is approximately */\
/* equal to the sum of unfft results.  Since this check may not */\
/* be perfect, check for identical results after a restart. */\
\
		if (gw_test_mismatched_sums (gwdata)) {\
			double suminp, sumout;\
			lasterr_point = J;\
			suminp = gwsuminp (gwdata, X);\
			sumout = gwsumout (gwdata, X);\
			if (J == last_bit[K] &&\
			    suminp == last_suminp[K] &&\
			    sumout == last_sumout[K]) {\
				clearline(100);\
				OutputBoth (ERROK);\
				saving = 1;\
			} else {\
				char	msg[80], sic[80], soc[80];\
				sprintf (sic, "%.16g", suminp);\
				sprintf (soc, "%.16g", sumout);\
				if (strcmp(sic, soc)) {\
				    sprintf (msg, ERRMSG1B, suminp, sumout);\
				    sprintf (buf, ERRMSG0L, J, N, msg);\
					clearline(100);\
				    OutputBoth (buf);\
				    last_bit[K] = J;\
				    last_suminp[K] = suminp;\
				    last_sumout[K] = sumout;\
					sleep5 = TRUE;\
				    goto error;\
				}\
			}\
		}\
\
/* Check for excessive roundoff error  */\
\
		if (gw_get_maxerr(gwdata) > maxroundoff) {\
			lasterr_point = J;\
			if (J == last_bit[K] &&\
			    gw_get_maxerr(gwdata) == last_maxerr[K] && !abonroundoff && !care) {\
				clearline(100);\
				OutputBoth (ERROK);\
				gwdata->GWERROR = 0;\
				clearline(100);\
				IniWriteInt(INI_FILE, "Error_Count", error_count = IniGetInt(INI_FILE, "Error_Count", 0) + 1);\
				if (error_count > MAX_ERROR_COUNT) {\
					OutputBoth (ERRMSG9);\
					IniWriteString(INI_FILE, "Error_Count", NULL);\
					will_try_larger_fft = TRUE;\
					last_bit[K]  = 0;\
					last_maxerr[K]  = 0.0;\
					maxerr_recovery_mode[K] = FALSE;\
				}\
				else {\
					OutputBoth (ERRMSG6);\
					maxerr_recovery_mode[K] = TRUE;\
				}\
				sleep5 = FALSE;\
				restarting = TRUE;\
				goto error;\
			} else {\
				char	msg[80];\
				sprintf (msg, ERRMSG1C, gw_get_maxerr(gwdata), maxroundoff);\
				sprintf (buf, ERRMSG0L, J, N, msg);\
				clearline(100);\
				OutputBoth (buf);\
				if (J == last_bit[K])\
					will_try_larger_fft = TRUE;\
				if (will_try_larger_fft) {\
					OutputBoth (ERRMSG8);\
					IniWriteString(INI_FILE, "Error_Count", NULL);\
					last_bit[K]  = 0;\
					last_maxerr[K]  = 0.0;\
				}\
				else {\
					last_bit[K] = J;\
					last_maxerr[K] = gw_get_maxerr(gwdata);\
				}\
				maxerr_recovery_mode[K] = FALSE;\
				sleep5 = FALSE;\
				restarting = TRUE;\
				goto error;\
			}\
		}\
\
		if (ERRCHK) {\
			if (gw_get_maxerr(gwdata) < reallyminerr && J > 30)\
				reallyminerr = gw_get_maxerr(gwdata);\
			if (gw_get_maxerr(gwdata) > reallymaxerr)\
				reallymaxerr = gw_get_maxerr(gwdata);\
		} 

// Some ABC format strings

char ckstring[] = "(2^$a$b)^2-2";	// Carol/Kynea
char cwstring[] = "$a*$b^$a$c";		// Cullen/Woodall
char ffstring[] = "$a*2^$b+1";		// FermFact output
char ffmstring[] = "$a*2^$b-1";		// Lei output
char gmstring[] = "4^$a+1";			// Gaussian Mersenne norms
char gfstring[] = "$a^$b+1";		// Special primality test for generalized Fermat numbers
char spstring[] = "(2^$a+1)/3";		// Special SPRP test for Wagstaff numbers
char dpstring[] = "DivPhi($a*$b^$c+1)"; // like-PRP test for DivPhi(...,2)
char repustring[] = "(10^$a-1)/9";	// PRP test for repunits numbers
char grepustring[] = "($a^$b-1)/($a-1)";// PRP test for generalized repunits numbers
char diffnumpstring[] = "$a^$b-$a^$c+%d";// If $b>$c, it is [$a^($b-$c)-1]*$a^$c+%d so, form K*B^N+C
char diffnummstring[] = "$a^$b-$a^$c-%d";// If $b>$c, it is [$a^($b-$c)-1]*$a^$c-%d so, form K*B^N+C
char diffnumstring[] = "$a^$b-$a^$c$d";	// General diffnum format
// Fixed k and c forms for k*b^n+c

char fkpstring[] = ""$LLF"*$a^$b+%d";
char fkmstring[] = ""$LLF"*$a^$b-%d";

// Fixed b and c forms for k*b^n+c

char fbpstring[]  = "$a*"$LLF"^$b+%d";
char fbmstring[]  = "$a*"$LLF"^$b-%d";

// Fixed n and c forms for k*b^n+c

char fnpstring[] = "$a*$b^%lu+%d";
char fnmstring[] = "$a*$b^%lu-%d";

// Fixed k forms for k*b^n+c

char fkastring[]  = ""$LLF"*$a^$b$c";

// Fixed b forms for k*b^n+c

char fbastring[] = "$a*"$LLF"^$b$c";

// Fixed n forms for k*b^n+c

char fnastring[]  = "$a*$b^%lu$c";

// Fixed c forms for k*b^n+c

char abcpstring[]  = "$a*$b^$c+%d";
char abcmstring[]  = "$a*$b^$c-%d";

// General k*b^n+c format 

char abcastring[] = "$a*$b^$c$d";

// General (k*b^n+c)/d format 

char abcadstring[] = "($a*$b^$c$d)/$e";

// Test the primality of a number given as a string

char numberstring[] = "$a";

// Test if $a is a base $b Wieferich prime

char wftstring[] = "$a$b";

// Search for base $c Wieferich primes in the range $a to $b

char wfsstring[] = "$a$b$c";



/* Process a number from newpgen output file */
/* NEWPGEN output files use the mask as defined below: */

#define MODE_PLUS    0x01	/* k.b^n+1 */
#define MODE_MINUS   0x02	/* k.b^n-1 */
#define MODE_2PLUS   0x04	/* k.b^(n+1)+1 (*) */
#define MODE_2MINUS  0x08	/* k.b^(n+1)-1 (*) */
#define MODE_4PLUS   0x10	/* k.b^(n+2)+1 (*) */
#define MODE_4MINUS  0x20	/* k.b^(n+2)-1 (*) */
#define MODE_PRIMORIAL 0x40	/* PRIMORIAL - can't handle this */
#define MODE_PLUS5  0x80	/* k.b^n+5 */
#define MODE_AP	    0x200	/* 2^n+2k-1 */
#define MODE_PLUS7  0x800	/* k.b^n+7 */
#define MODE_2PLUS3 0x1000	/* 2k.b^n+3 */
#define MODE_DUAL 0x8000
#define MODE_PLUS_DUAL 0x8001	/* b^n+k */
#define MODE_MINUS_DUAL 0x8002	/* b^n-k */
#define MODE_NOTGENERALISED 0x400


/* Define the world/group/owner read/write attributes for creating files */
/* I've always used 0666 in Unix (everyone gets R/W access), but MSVC 8 */
/* now refuses to work with that setting -- insisting on 0600 instead. */

#ifdef _WIN32
#define	CREATE_FILE_ACCESS	0600
#else
#define	CREATE_FILE_ACCESS	0666
#endif

#define IBSIZE 300

char	greatbuf[10001] = {0};
char	INI_FILE[80] = {0};
char	SVINI_FILE[80] = {0};
char	RESFILE[80] = {0};
char	LOGFILE[80] = {0};
char	EXTENSION[8] = {0};;
char	HARDWARE_GUID[33] = {0};
int volatile ERRCHK = 0;
unsigned int PRIORITY = 1;
unsigned int CPU_AFFINITY = 99;
EXTERNC unsigned int volatile CPU_TYPE = 0;
unsigned long volatile ITER_OUTPUT = 0;
unsigned long volatile ITER_OUTPUT_RES = 99999999;
unsigned long volatile DISK_WRITE_TIME = 30;
unsigned long INTERIM_FILES = 0;
unsigned long INTERIM_RESIDUES = 0;
int	CLASSIC_OUTPUT = 0;
int	OUTPUT_ROUNDOFF = 0;
int	CUMULATIVE_ROUNDOFF = 1;
int	TWO_BACKUP_FILES = 1;
int	NUM_BACKUP_FILES = 3;
int	NUM_JACOBI_BACKUP_FILES = 2;
int	RUN_ON_BATTERY = 1;
int	TRAY_ICON = TRUE;
int	HIDE_ICON = FALSE;
double UNOFFICIAL_CPU_SPEED = 0.0;
unsigned int WORKTODO_COUNT = 0;/* Count of valid work lines */
unsigned int ROLLING_AVERAGE = 0;
unsigned int PRECISION = 2;
unsigned long NUM_CPUS = 1;	/* Number of CPUs/Cores in the computer */
unsigned long NUM_WORKER_THREADS = 1;
hwloc_topology_t hwloc_topology;	/* Hardware topology */
uint32_t CPU_TOTAL_L1_CACHE_SIZE = 0;	/* Sum of all the L1 caches in KB as determined by hwloc */
uint32_t CPU_TOTAL_L2_CACHE_SIZE = 0;	/* Sum of all the L2 caches in KB as determined by hwloc */
uint32_t CPU_TOTAL_L3_CACHE_SIZE = 0;	/* Sum of all the L3 caches in KB as determined by hwloc */
uint32_t CPU_TOTAL_L4_CACHE_SIZE = 0;	/* Sum of all the L4 caches in KB as determined by hwloc */
uint32_t CPU_NUM_L1_CACHES = 0;		/* Number of L1 caches as determined by hwloc */
uint32_t CPU_NUM_L2_CACHES = 0;		/* Number of L2 caches as determined by hwloc */
uint32_t CPU_NUM_L3_CACHES = 0;		/* Number of L3 caches as determined by hwloc */
uint32_t CPU_NUM_L4_CACHES = 0;		/* Number of L4 caches as determined by hwloc */
int	CPU_L2_CACHE_INCLUSIVE = -1;	/* 1 if inclusive, 0 if exclusive, -1 if not known */
int	CPU_L3_CACHE_INCLUSIVE = -1;	/* 1 if inclusive, 0 if exclusive, -1 if not known */
int	CPU_L4_CACHE_INCLUSIVE = -1;	/* 1 if inclusive, 0 if exclusive, -1 if not known */
unsigned int NUM_NUMA_NODES = 1;	/* Number of NUMA nodes in the computer */
unsigned int NUM_THREADING_NODES = 1;	/* Number of nodes where it might be beneficial to keep a worker's threads in the same node */
int	CUMULATIVE_TIMING = 0;
int	HIGH_RES_TIMER = 0; 
int	usingDivPhi_m = 0;
int	RDTSC_TIMING = 1;

extern double MAXDIFF;
double maxdiffmult = 1.0;

/* PRP and LLR global variables */

#if defined (_CONSOLE)
HANDLE hConsole;		// Handle to Console attributes
#endif

#define	sgkbufsize 20000

giant	N = NULL;		/* Number being tested */
giant	NP = NULL;		/* Number being tested */
giant	M = NULL;		/* Gaussian Mersenne modulus = N*NP */
giant	gk = NULL;		/* k multiplier */
giant	gb = NULL;		/* Generalized Fermat base may be a large integer... */

unsigned long Nlen = 0;	/* Bit length of number being LLRed or PRPed */
unsigned long klen = 0;	/* Number of bits of k multiplier */
unsigned long OLDFFTLEN = 0; /* previous value of FFTLEN, used by setuponly option */
unsigned long ndiff = 0;/* used for b^n-b^m+c number processing */
unsigned long gformat;	/* used for b^n-b^m+c number processing */

struct work_unit *w = NULL;	// Work to do data as described in Prime95

/* Global variables for factoring */

#ifndef	WIN32
EXTERNC unsigned long _CPU_FLAGS = 0;
#endif

EXTERNC unsigned long FACPASS = 0;
EXTERNC unsigned long FACTEST = 0;
EXTERNC unsigned long FACHSW = 0;	/* High word of found factor */
EXTERNC unsigned long FACMSW = 0;	/* Middle word of found factor */
EXTERNC unsigned long FACLSW = 0;	/* Low word of found factor */
EXTERNC void *FACDATA = NULL;		/* factor data is kept in a global */
EXTERNC void *SRCARG = NULL;

/* Other gwnum globals */

EXTERNC void gw_random_number (gwhandle*, gwnum);/* Generate random FFT data */

giant testn, testnp, testf, testx;
unsigned long facn = 0, facnp = 0;
int resn = 0, resnp = 0;
char facnstr[80], facnpstr[80];
char m_pgen_input[IBSIZE], m_pgen_output[IBSIZE], oldm_pgen_input[IBSIZE];
char keywords[10][IBSIZE], values[10][IBSIZE];
char multiplier[IBSIZE], base[IBSIZE], exponent[IBSIZE], exponent2[IBSIZE], addin[IBSIZE], divisors[IBSIZE];
char inifilebuf[IBSIZE];
char sgd[sgkbufsize];
char sgb[sgkbufsize];
char bpfstring[sgkbufsize];

#ifndef X86_64

void setupf();
int factor64();
void psetupf();
int pfactor64();

#endif

//void* aligned_malloc (unsigned long, unsigned long);
void  aligned_free (void *);

void md5 (
	char output[33],
	const char *string);

/* Calculate the 32-byte hex string for the hardware GUID.  We use the */
/* output of the CPUID function to generate this.  We don't include the */
/* cache size information because when a new processor comes out the CPUID */
/* does not recognize the cache size.  When a new version of prime95 is */
/* released that does recognize the cache size a different hardware GUID */
/* would be generated. */

void calc_hardware_guid (void)
{
	char	buf[500];

	sprintf (buf, "%s%d", CPU_BRAND, CPU_SIGNATURE);
	md5 (HARDWARE_GUID, buf);

/* Sometimes a user might want to run the program on several machines. */
/* Typically this is done by carrying the program and files around on a */
/* portable media such as a USB memory stick.  In this case, */
/* we need to defeat the code that automatically detects hardware changes. */
/* The FixedHardwareUID INI option tells us to get the Windows and */
/* hardware hashes from the INI file rather than calculating them. */

	if (IniGetInt (INI_FILE, "FixedHardwareUID", 0)) {
		IniGetString (INI_FILE, "HardwareGUID", HARDWARE_GUID, sizeof (HARDWARE_GUID), HARDWARE_GUID);
		IniWriteString (INI_FILE, "HardwareGUID", HARDWARE_GUID);
	}
}

static unsigned long last_bit[10] = {0};				// Bit or iter. currently tested on operaton N� index.
static double last_suminp[10] = {0.0};
static double last_sumout[10] = {0.0};
static double last_maxerr[10] = {0.0};					// Current Roundoff on operation N� index.
static double maxroundoff = 0.40;						// Largest acceptable Roundoff Error
static double fpart = 0.0;

static unsigned long mask;

unsigned int Fermat_only = FALSE;

unsigned int strong = FALSE;
unsigned int quotient = FALSE;
unsigned int vrbareix = FALSE;
unsigned int dualtest = FALSE;
unsigned int bpsw = FALSE;
unsigned int verbose = FALSE;
unsigned int setuponly = FALSE;
unsigned int nosaving = FALSE;
unsigned int abonillsum = FALSE;
unsigned int abonmismatch = FALSE;
unsigned int testgm = FALSE;
unsigned int testgq = FALSE;
unsigned int testfac = FALSE;
unsigned int nofac = FALSE;
unsigned int general = FALSE;
unsigned int eps2 = FALSE;
unsigned int debug = FALSE;
unsigned int restarting = FALSE;				// Actually set by Roundoff error condition ; reset by the macro.
unsigned int care = FALSE;						// Set after a careful iteration ; reset after a normal one.
unsigned int postfft = TRUE;
unsigned int nbfftinc = 0;						// Number of required FFT increments.
unsigned int maxfftinc = 5;						// Maximum accepted FFT increments.
unsigned int aborted = FALSE;
unsigned int generic = FALSE;
unsigned int abonroundoff = FALSE;				// Abort the test in a Roundoff error occurs.
unsigned int will_try_larger_fft = FALSE;		// Set if unrecoverable error or too much errors ; reset by the macro.
unsigned int checknumber = 0;					// N� of currently checked gwnum operation ; only displayed in abort message
unsigned int error_count = 0;					// Number of reproducible errors that previously occured.
unsigned int sleep5 = FALSE, showdigits = FALSE;
unsigned int maxerr_recovery_mode [10] = {0};	// Set if a reproducible error occured in the current operation.
unsigned int lasterr_point = 0;					// Last bit or iteration that caused an error.
unsigned long primolimit = 30000;
unsigned long nextifnear = FALSE;
unsigned long maxaprcl = 200;
unsigned long interimFiles, interimResidues, throttle, facfrom, facto;
unsigned long factored = 0, eliminated = 0;
unsigned long pdivisor = 1000000, pquotient = 1;
unsigned long bpf[30], bpc[30], vpf[30];		// Base prime factors, cofactors, power of p.f.
unsigned long *t = NULL,*ta = NULL;				// Precrible arrays, dynamically allocated.
unsigned long smallprime[168] =					// Small primes < 1000
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
unsigned long smallphi[10] = {1,2,4,6,10,12,16,18,22,28};
giantstruct*		gbpf[30] = {NULL};			// Large integer prime factors
giantstruct*		gbpc[30] = {NULL};			// Large integer cofactors
unsigned long nrestarts = 0;					// Nb. of restarts for an N+1 or N-1 prime test
unsigned long nbdg = 0, globalb = 2;			// number of digits ; base of the candidate in a global
double	globalk = 1.0;							// k value of the candidate in a global
double	pcfftlim = 0.5;							// limit for gwnear_fft_limit function to return TRUE.
int readlg, writelg;							// Values returned by low level I/O


double timers[10] = {0.0};			/* Up to five separate timers */

double smargin = 0.0;

int is_valid_double(double);
int genProthBase(giant, uint32_t);
long generalLucasBase(giant , uint32_t *, uint32_t *);
unsigned long gcd (
	unsigned long,
	unsigned long);

int ispower (unsigned long,
	unsigned long);

/* Utility output routines */

void LineFeed ()
{
#ifndef WIN32
	OutputStr ("\r");
#elif defined (_CONSOLE)
	OutputStr ("\r");
#else
	OutputStr ("\n");
#endif
}

void OutputNum (
	unsigned long num)
{
	char	buf[20];
	sprintf (buf, "%lu", num);
	OutputStr (buf);
}

#define MAX_ERROR_COUNT 5

char ERROK[] = "Disregard last error.  Result is reproducible and thus not a hardware problem.\n";
char ERRMSG0L[] = "Iter: %ld/%ld, %s";
char ERRMSG0[] = "Bit: %ld/%ld, %s"; 
char ERRMSG1A[] = "ERROR: ILLEGAL SUMOUT\n";
char ERRMSG1B[] = "ERROR: SUM(INPUTS) != SUM(OUTPUTS), %.16g != %.16g\n";
char ERRMSG1C[] = "ERROR: ROUND OFF (%.10g) > %.10g\n";
char ERRMSG2[] = "Possible hardware failure, consult the readme file.\n";
char ERRMSG3[] = "Continuing from last save file.\n";
char ERRMSG4[] = "Waiting five minutes before restarting.\n";
char ERRMSG5[] = "Fatal Error, Check Number = %d, test of %s aborted\n";
char ERRMSG6[] = "For added safety, redoing iteration using a slower, more reliable method.\n";
char ERRMSG7[] = "Fatal Error, divisibility test of %d^(N-1)-1 aborted\n";
char ERRMSG8[] = "Unrecoverable error, Restarting with next larger FFT length...\n";
//char ERRMSG9[] = "Too much errors while using the standard method ; continuing with a slower, more reliable one.\n";
char ERRMSG9[] = "Too much errors ; Restarting with next larger FFT length...\n";
char ERRMSG60[] = "ERROR: Comparing double-check values failed.  Rolling back to iteration %lu.\n";
char ERRMSG70[] = "ERROR: Comparing Gerbicz checksum values failed.  Rolling back to iteration %lu.\n";
char ERRMSG80[] = "ERROR: Invalid FFT data.  Restarting from last save file.\n";
char ERRMSG90[] = "ERROR: Invalid PRP state.  Restarting from last save file.\n";
char WRITEFILEERR[] = "Error writing intermediate file: %s\n";

void	trace(unsigned long n) {			// Debugging tool...
	char buf[100];
	sprintf(buf, "OK until number %lu\n", n);
	if (verbose)
		OutputBoth (buf); 
	else
		OutputStr (buf); 	
}

void clearline (int size) {
	char buf[256];
	int i;

	for (i=0; i<256; i++)
		buf[i] = '\0';
	for (i=0; i<size; i++)
		buf[i] = ' ';
	buf[size-1] = '\r';
#if !defined(WIN32) || defined(_CONSOLE)
	OutputStr(buf);
#endif
}

int	digitstrcmp (const char *s1, const char *s2) {
	if (strlen (s1) < strlen (s2))
		return (-1);
	else if (strlen (s1) > strlen (s2))
		return (1);
	else 
		return (strcmp (s1, s2));
}


int SleepFive ()
{
	int	i;

	OutputStr (ERRMSG4);
	BlinkIcon (10);			/* Blink icon for 10 seconds */
	Sleep (10000);
	ChangeIcon (IDLE_ICON);		/* Idle icon for rest of 5 minutes */
	for (i = 0; i < 290; i++) {
		Sleep (1000);
		if (escapeCheck ()) return (FALSE);
	}
	ChangeIcon (WORKING_ICON);	/* And back to the working icon */
	return (TRUE);
}

/* Generate the scaling factors for ITER_OUTPUT in the rare cases where the user */
/* has used some undoc.txt settings to change how often the title is output or to */
/* make the frequency roughly the same in all windows even if using different FFT sizes. */

void calc_output_frequencies (
	gwhandle *gwdata,		/* Handle to the gwnum code */
	double	*output_frequency,	/* Calculated adjustment to ITER_OUTPUT */
	double	*output_title_frequency)/* Calculated adjustment to ITER_OUTPUT for title */
{
	int	scaled_freq, title_freq;
	double	exp, temp;

	/* Check the flag that says scale ITER_OUTPUT so that messages */
	/* appear at roughly same rate for all FFT sizes (scale factor */
	/* should be 1.0 if testing M50000000). */
	scaled_freq = (int) IniGetInt (INI_FILE, "ScaleOutputFrequency", 0);
	if (!scaled_freq) {
		*output_frequency = 1.0;
	} else {
		*output_frequency = gwmap_to_timing (1.0, 2, 50000000, -1) /
				    gwmap_to_timing (gwdata->k, gwdata->b, gwdata->n, gwdata->c);
		if (gwget_num_threads (gwdata) > 1 && NUM_WORKER_THREADS < NUM_CPUS)
			*output_frequency /= 1.8 * (gwget_num_threads (gwdata) - 1);
		/* For prettier output (outputs likely to be a multiple of a power of 10), round the */
		/* output frequency to the nearest (10,15,20,25,30,40,...90) times a power of ten */
		exp = floor (_log10 (*output_frequency));
		temp = *output_frequency * pow (10.0, -exp);
		if (temp < 1.25) temp = 1.0;
		else if (temp <1.75) temp = 1.5;
		else if (temp < 2.25) temp = 2.0;
		else if (temp < 2.75) temp = 2.5;
		else temp = floor (temp + 0.5);
		*output_frequency = temp * pow (10.0, exp);
	}

	/* Calculate the title frequency as a fraction of the output frequency */
	title_freq = (int) IniGetInt (INI_FILE, "TitleOutputFrequency", 1);
	if (title_freq < 1) title_freq = 1;
	*output_title_frequency = *output_frequency / (double) title_freq;
}

/* Truncate a percentage to the requested number of digits. */
/* Truncating prevents 99.5% from showing up as 100% complete. */

double trunc_percent (
	double	percent)
{
	if (percent > 100.0) percent = 100.0;
	percent -= 0.5 * pow (10.0, - (double) PRECISION);
	if (percent < 0.0) return (0.0);
	return (percent);
}

 
//  Test if a string contains only valid digits. 
 
int isDigitString(char *s) { 
    while (*s) { 
	if (!isdigit(*s)) return (FALSE); 
	s++; 
    } 
    return (TRUE); 
} 
 
/* Routines used to time code chunks */

void clear_timers () {
	int	i;
	for (i = 0; i < 10; i++) timers[i] = 0.0;
}

void clear_timer (
	int	i)
{
	timers[i] = 0.0;
}

void start_timer ( 
	int	i) 
{ 
	if (i > 4)
		return;
	if (timers[i+5] != 0.0)			// to avoid double start...
		return;
	if (HIGH_RES_TIMER) { 
		timers[i] -= getHighResTimer (); 
	} /* else if (CPU_FLAGS & CPU_RDTSC) {		// 16/06/12 CPU_SPEED no more used...
		uint32_t hi, lo;
		rdtsc (&hi, &lo);
		timers[i] -= (double) hi * 4294967296.0 + lo;
	} */ else { 
		struct _timeb timeval; 
		_ftime (&timeval); 
		timers[i] -= (double) timeval.time * 1000.0 + timeval.millitm; 
	} 
	timers[i+5] = 1.0;				// to show that timers[i] is already started
} 
 
void end_timer ( 
	int	i) 
{ 
	if (i > 4)
		return;
	if (timers[i+5] == 0.0)			// to avoid double end...
		return;
	if (HIGH_RES_TIMER) { 
		timers[i] += getHighResTimer (); 
	} /* else if (CPU_FLAGS & CPU_RDTSC) {		// 16/06/12 CPU_SPEED no more used...
		uint32_t hi, lo;
		rdtsc (&hi, &lo);
		timers[i] += (double) hi * 4294967296.0 + lo;
	} */ else { 
		struct _timeb timeval; 
		_ftime (&timeval); 
		timers[i] += (double) timeval.time * 1000.0 + timeval.millitm; 
	} 
	timers[i+5] = 0.0;				// to show that timers[i] is ended
} 
 
void divide_timer (
	int	i,
	int	j)
{
	timers[i] = timers[i] / j;
}

double timer_value ( 
	int	i) 
{ 
	if (HIGH_RES_TIMER) 
		return (timers[i] / getHighResTimerFrequency ()); 
//	else if (CPU_FLAGS & CPU_RDTSC)
//		return (timers[i] / CPU_SPEED / 1000000.0);		// 16/06/12 CPU_SPEED no more used...
	else 
		return (timers[i] / 1000.0); 
} 
 
#define TIMER_NL	0x1
#define TIMER_CLR	0x2
#define TIMER_OPT_CLR	0x4
#define TIMER_MS	0x8

void print_timer (
	int	i,
	int	flags)
{ 
	char	buf[40]; 
	double	t; 
 
	t = timer_value (i); 
	if (t >= 1.0) 
		sprintf (buf, "%.3f sec.", t); 
	else 
		sprintf (buf, "%.3f ms.", t * 1000.0); 
	OutputStr (buf); 
	if (flags & TIMER_NL) LineFeed();
	if (flags & TIMER_CLR) timers[i] = 0.0; 
	if ((flags & TIMER_OPT_CLR) && !CUMULATIVE_TIMING) timers[i] = 0.0; 
} 

void write_timer (		// JP 23/11/07
	char* buf,
	int	i, 
	int	flags) 
{ 
	double	t; 
 
	t = timer_value (i); 
	if (t >= 1.0)  
		sprintf (buf, "%.3f sec.", t); 
	else 
		sprintf (buf, "%.3f ms.", t * 1000.0); 
 
	if (flags & TIMER_NL)
           strcat(buf, "\n");

	if (flags & TIMER_CLR) timers[i] = 0.0; 
	if ((flags & TIMER_OPT_CLR) && !CUMULATIVE_TIMING) timers[i] = 0.0; 
} 

void OutputTimeStamp ()
{
	time_t	this_time;
	char	tbuf[40], buf[40];

//	if (TIMESTAMPING) {
		time (&this_time);
		strcpy (tbuf, ctime (&this_time)+4);
		tbuf[12] = 0;
		sprintf (buf, "[%s] ", tbuf);
		OutputStr (buf);
//	}
}

void OutputBTimeStamp ()
{
	time_t	this_time;
	char	tbuf[40], buf[40];

//	if (TIMESTAMPING) {
		time (&this_time);
		strcpy (tbuf, ctime (&this_time)+4);
		tbuf[12] = 0;
		sprintf (buf, "[%s]\n", tbuf);
		OutputBoth (buf);
//	}
}

/* Determine the CPU speed either empirically or by user overrides. */
/* getCpuType must be called prior to calling this routine. */

void getCpuSpeed (void)
{
	int	temp, old_cpu_speed, report_new_cpu_speed;

/* Guess the CPU speed using the RDTSC instruction */

	guessCpuSpeed ();

/* Now let the user override the cpu speed from the local.ini file */

	temp = IniGetInt (INI_FILE, "CpuSpeed", 99);
	if (temp != 99) CPU_SPEED = temp;

/* Make sure the cpu speed is reasonable */

	if (CPU_SPEED > 50000) CPU_SPEED = 50000;
	if (CPU_SPEED < 25) CPU_SPEED = 25;

/* Set the unofficial CPU speed.  The unofficial CPU speed is the */
/* last CPU speed measurement.  The official CPU speed is the one */
/* reported to the server. */

	UNOFFICIAL_CPU_SPEED = CPU_SPEED;

/* If CPU speed is much less than the official CPU speed, then set a new */
/* official CPU speed only after several slower measurements. */
/* The reason for this is that erroneously (due to one aberrant CPU speed */
/* calculation) reducing the speed we report to the server may result */
/* in erroneously unreserving exponents. */

	report_new_cpu_speed = FALSE;
	old_cpu_speed = IniGetInt (INI_FILE, "OldCpuSpeed", 0);
	if (CPU_SPEED < (double) old_cpu_speed * 0.97) {
		if (IniGetInt (INI_FILE, "NewCpuSpeedCount", 0) <= 5) {
			if (CPU_SPEED > (double) IniGetInt (INI_FILE, "NewCpuSpeed", 0))
				IniWriteInt (INI_FILE, "NewCpuSpeed", (int) (CPU_SPEED + 0.5));
			IniWriteInt (INI_FILE, "NewCpuSpeedCount", IniGetInt (INI_FILE, "NewCpuSpeedCount", 0) + 1);
			CPU_SPEED = old_cpu_speed;
		} else {
			if (CPU_SPEED < (double) IniGetInt (INI_FILE, "NewCpuSpeed", 0))
				CPU_SPEED = (double) IniGetInt (INI_FILE, "NewCpuSpeed", 0);
			report_new_cpu_speed = TRUE;
		}
	}

/* If CPU speed is close to last reported CPU speed, then use it. */
/* tell the server, recalculate new completion dates, and reset the */
/* rolling average.  Don't do this on the first run (before the Welcome */
/* dialog has been displayed). */

	else if (CPU_SPEED < (double) old_cpu_speed * 1.03) {
		IniWriteInt (INI_FILE, "NewCpuSpeedCount", 0);
		IniWriteInt (INI_FILE, "NewCpuSpeed", 0);
	}

/* If CPU speed is much larger than the speed reported to the server, then */
/* use this new speed and tell the server. */

	else {
		report_new_cpu_speed = TRUE;
	}

/* Report a new CPU speed.  Remember the new CPU speed, tell the server, */
/* recalculate new completion dates, and reset the rolling average in */
/* such a way as to reduce the chance of spurious unreserves.  Don't */
/* do this on the first run (before the Welcome dialog has been displayed). */

	if (report_new_cpu_speed) {
		IniWriteInt (INI_FILE, "OldCpuSpeed", (int) (CPU_SPEED + 0.5));
		IniWriteInt (INI_FILE, "NewCpuSpeedCount", 0);
		IniWriteInt (INI_FILE, "NewCpuSpeed", 0);
		if (old_cpu_speed) {
			if (WORKTODO_COUNT) {
				ROLLING_AVERAGE = (int) (ROLLING_AVERAGE * old_cpu_speed / CPU_SPEED);
				if (ROLLING_AVERAGE < 1000) ROLLING_AVERAGE = 1000;
			}
			else
				ROLLING_AVERAGE = 1000;
			IniWriteInt (INI_FILE, "RollingAverage", ROLLING_AVERAGE);
			IniWriteInt (INI_FILE, "RollingStartTime", 0);
		}
	}
}

/* Set the CPU flags based on the CPUID instruction.  Also, the advanced */
/* user can override our guesses. */

void getCpuInfo (void)
{
	int	depth, i, temp;
			
/* Get the CPU info using CPUID instruction */	

	guessCpuType ();

/* New in version 29!  Use hwloc info to determine NUM_CPUS and CPU_HYPERTHREADS.  Also get number of NUMA nodes */
/* which we may use later on to allocate memory from the proper NUMA node. */
/* We still allow overriding these settings using the INI file. */

	NUM_CPUS = hwloc_get_nbobjs_by_type (hwloc_topology, HWLOC_OBJ_CORE);
	if (NUM_CPUS < 1) NUM_CPUS = hwloc_get_nbobjs_by_type (hwloc_topology, HWLOC_OBJ_PU);
	if (NUM_CPUS < 1) NUM_CPUS = 1;				// Shouldn't happen
	CPU_HYPERTHREADS = hwloc_get_nbobjs_by_type (hwloc_topology, HWLOC_OBJ_PU) / NUM_CPUS;
	if (CPU_HYPERTHREADS < 1) CPU_HYPERTHREADS = 1;		// Shouldn't happen
	NUM_NUMA_NODES = hwloc_get_nbobjs_by_type (hwloc_topology, HWLOC_OBJ_NUMANODE);
	if (NUM_NUMA_NODES < 1 || NUM_CPUS % NUM_NUMA_NODES != 0) NUM_NUMA_NODES = 1;

/* New in version 29.5, get L1/L2/L3/L4 total cache size for use in determining torture test FFT sizes. */
/* Overwrite cpuid's linesize and associativity with hwloc's */

	CPU_TOTAL_L1_CACHE_SIZE = CPU_NUM_L1_CACHES = 0;
	CPU_TOTAL_L2_CACHE_SIZE = CPU_NUM_L2_CACHES = 0;
	CPU_TOTAL_L3_CACHE_SIZE = CPU_NUM_L3_CACHES = 0;
	CPU_TOTAL_L4_CACHE_SIZE = CPU_NUM_L4_CACHES = 0;
	CPU_L2_CACHE_INCLUSIVE = -1;
	CPU_L3_CACHE_INCLUSIVE = -1;
	CPU_L4_CACHE_INCLUSIVE = -1;
#if HWLOC_API_VERSION >= 0x00020000
	for (depth = 0; depth < hwloc_topology_get_depth (hwloc_topology); depth++) {
		for (i = 0; i < (int) hwloc_get_nbobjs_by_depth (hwloc_topology, depth); i++) {
			hwloc_obj_t obj;
			const char *inclusive;

			obj = hwloc_get_obj_by_depth (hwloc_topology, depth, i);
			if (obj == NULL || obj->attr == NULL) break; // can't happen
			if (obj->type == HWLOC_OBJ_L1CACHE) {
				CPU_TOTAL_L1_CACHE_SIZE += (uint32_t) (obj->attr->cache.size >> 10);
				CPU_NUM_L1_CACHES++;
				if (obj->attr->cache.linesize > 0) CPU_L1_CACHE_LINE_SIZE = obj->attr->cache.linesize;
				if (obj->attr->cache.associativity > 0) CPU_L1_SET_ASSOCIATIVE = obj->attr->cache.associativity;
			}
			else if (obj->type == HWLOC_OBJ_L2CACHE) {
				CPU_TOTAL_L2_CACHE_SIZE += (uint32_t) (obj->attr->cache.size >> 10);
				CPU_NUM_L2_CACHES++;
				if (obj->attr->cache.linesize > 0) CPU_L2_CACHE_LINE_SIZE = obj->attr->cache.linesize;
				if (obj->attr->cache.associativity > 0) CPU_L2_SET_ASSOCIATIVE = obj->attr->cache.associativity;
				inclusive = hwloc_obj_get_info_by_name (obj, "Inclusive");
				if (inclusive != NULL) CPU_L2_CACHE_INCLUSIVE = atoi (inclusive);
			}
			else if (obj->type == HWLOC_OBJ_L3CACHE) {
				CPU_TOTAL_L3_CACHE_SIZE += (uint32_t) (obj->attr->cache.size >> 10);
				CPU_NUM_L3_CACHES++;
				if (obj->attr->cache.linesize > 0) CPU_L3_CACHE_LINE_SIZE = obj->attr->cache.linesize;
				if (obj->attr->cache.associativity > 0) CPU_L3_SET_ASSOCIATIVE = obj->attr->cache.associativity;
				inclusive = hwloc_obj_get_info_by_name (obj, "Inclusive");
				if (inclusive != NULL) CPU_L3_CACHE_INCLUSIVE = atoi (inclusive);
			}
			else if (obj->type == HWLOC_OBJ_L4CACHE) {
				CPU_TOTAL_L4_CACHE_SIZE += (uint32_t) (obj->attr->cache.size >> 10);
				CPU_NUM_L4_CACHES++;
				inclusive = hwloc_obj_get_info_by_name (obj, "Inclusive");
				if (inclusive != NULL) CPU_L4_CACHE_INCLUSIVE = atoi (inclusive);
			}
		}
	}
#endif

/* Overwrite the cache info calculated via CPUID as hwloc's info is more detailed and I believe more reliable. */
/* We are transitioning away from using the cache size global variables computed by the CPUID code. */

	if (CPU_NUM_L1_CACHES) CPU_L1_CACHE_SIZE = CPU_TOTAL_L1_CACHE_SIZE / CPU_NUM_L1_CACHES;
	if (CPU_NUM_L2_CACHES) CPU_L2_CACHE_SIZE = CPU_TOTAL_L2_CACHE_SIZE / CPU_NUM_L2_CACHES;
	if (CPU_NUM_L3_CACHES) CPU_L3_CACHE_SIZE = CPU_TOTAL_L3_CACHE_SIZE / CPU_NUM_L3_CACHES;

/* If hwloc could not figure out the cache sizes, use the cache sizes as determined by CPUID. */
/* Note that the CPUID code in gwnum is not good at determining the number of L2 and L3 caches. */
/* Fortunately, it should be rare that we rely on the CPUID code. */

	if (CPU_NUM_L1_CACHES == 0 && CPU_L1_CACHE_SIZE > 0) CPU_TOTAL_L1_CACHE_SIZE = CPU_L1_CACHE_SIZE * NUM_CPUS, CPU_NUM_L1_CACHES = NUM_CPUS;
	if (CPU_NUM_L2_CACHES == 0 && CPU_L2_CACHE_SIZE > 0) CPU_TOTAL_L2_CACHE_SIZE = CPU_L2_CACHE_SIZE, CPU_NUM_L2_CACHES = 1;
	if (CPU_NUM_L3_CACHES == 0 && CPU_L3_CACHE_SIZE > 0) CPU_TOTAL_L3_CACHE_SIZE = CPU_L3_CACHE_SIZE, CPU_NUM_L3_CACHES = 1;

/* Calculate hardware GUID (global unique identifier) using the CPUID info. */
/* Well, it isn't unique but it is about as good as we can do and still have */
/* portable code.  Do this calculation before user overrides values */
/* derived from CPUID results. */

	calc_hardware_guid ();

/* Let the user override the cpu flags from the local.ini file */

	temp = IniGetInt (INI_FILE, "CpuSupportsRDTSC", 99);
	if (temp == 0) CPU_FLAGS &= ~CPU_RDTSC;
	if (temp == 1) CPU_FLAGS |= CPU_RDTSC;
	temp = IniGetInt (INI_FILE, "CpuSupportsCMOV", 99);
	if (temp == 0) CPU_FLAGS &= ~CPU_CMOV;
	if (temp == 1) CPU_FLAGS |= CPU_CMOV;
	temp = IniGetInt (INI_FILE, "CpuSupportsPrefetch", 99);
	if (temp == 0) CPU_FLAGS &= ~CPU_PREFETCH;
	if (temp == 1) CPU_FLAGS |= CPU_PREFETCH;
	temp = IniGetInt (INI_FILE, "CpuSupportsPrefetchw", 99);
	if (temp == 0) CPU_FLAGS &= ~CPU_PREFETCHW;
	if (temp == 1) CPU_FLAGS |= CPU_PREFETCHW;
	temp = IniGetInt (INI_FILE, "CpuSupportsPrefetchwt1", 99);
	if (temp == 0) CPU_FLAGS &= ~CPU_PREFETCHWT1;
	if (temp == 1) CPU_FLAGS |= CPU_PREFETCHWT1;
	temp = IniGetInt (INI_FILE, "CpuSupportsSSE", 99);
	if (temp == 0) CPU_FLAGS &= ~CPU_SSE;
	if (temp == 1) CPU_FLAGS |= CPU_SSE;
	temp = IniGetInt (INI_FILE, "CpuSupportsSSE2", 99);
	if (temp == 0) CPU_FLAGS &= ~CPU_SSE2;
	if (temp == 1) CPU_FLAGS |= CPU_SSE2;
	temp = IniGetInt (INI_FILE, "CpuSupportsSSE4", 99);
	if (temp == 0) CPU_FLAGS &= ~CPU_SSE41;
	if (temp == 1) CPU_FLAGS |= CPU_SSE41;
	temp = IniGetInt (INI_FILE, "CpuSupports3DNow", 99);
	if (temp == 0) CPU_FLAGS &= ~(CPU_3DNOW + CPU_3DNOW_PREFETCH);
	if (temp == 1) CPU_FLAGS |= (CPU_3DNOW + CPU_3DNOW_PREFETCH);
	temp = IniGetInt (INI_FILE, "CpuSupportsAVX", 99);
	if (temp == 0) CPU_FLAGS &= ~(CPU_AVX | CPU_FMA3);
	if (temp == 1) CPU_FLAGS |= CPU_AVX;
	temp = IniGetInt (INI_FILE, "CpuSupportsFMA3", 99);
	if (temp == 0) CPU_FLAGS &= ~CPU_FMA3;
	if (temp == 1) CPU_FLAGS |= (CPU_AVX | CPU_FMA3);
	temp = IniGetInt (INI_FILE, "CpuSupportsFMA4", 99);
	if (temp == 0) CPU_FLAGS &= ~CPU_FMA4;
	if (temp == 1) CPU_FLAGS |= CPU_FMA4;
	temp = IniGetInt (INI_FILE, "CpuSupportsAVX2", 99);
	if (temp == 0) CPU_FLAGS &= ~CPU_AVX2;
	if (temp == 1) CPU_FLAGS |= CPU_AVX2;
	temp = IniGetInt (INI_FILE, "CpuSupportsAVX512F", 99);
	if (temp == 0) CPU_FLAGS &= ~CPU_AVX512F;
	if (temp == 1) CPU_FLAGS |= CPU_AVX512F;

/* Let the user override the L1/L2/L3/L4 cache size in local.txt file */

	CPU_TOTAL_L1_CACHE_SIZE = IniGetInt (INI_FILE, "CpuL1TotalCacheSize", CPU_TOTAL_L1_CACHE_SIZE);
	CPU_NUM_L1_CACHES = IniGetInt (INI_FILE, "CpuL1NumCaches", CPU_NUM_L1_CACHES);

	CPU_TOTAL_L2_CACHE_SIZE = IniGetInt (INI_FILE, "CpuL2TotalCacheSize", CPU_TOTAL_L2_CACHE_SIZE);
	CPU_NUM_L2_CACHES = IniGetInt (INI_FILE, "CpuL2NumCaches", CPU_NUM_L2_CACHES);
	CPU_L2_CACHE_SIZE = IniGetInt (INI_FILE, "CpuL2CacheSize", CPU_L2_CACHE_SIZE);
	CPU_L2_CACHE_LINE_SIZE = IniGetInt (INI_FILE, "CpuL2CacheLineSize", CPU_L2_CACHE_LINE_SIZE);
	CPU_L2_SET_ASSOCIATIVE = IniGetInt (INI_FILE, "CpuL2SetAssociative", CPU_L2_SET_ASSOCIATIVE);
	CPU_L2_CACHE_INCLUSIVE = IniGetInt (INI_FILE, "CpuL2CacheInclusive", CPU_L2_CACHE_INCLUSIVE);

	CPU_TOTAL_L3_CACHE_SIZE = IniGetInt (INI_FILE, "CpuL3TotalCacheSize", CPU_TOTAL_L3_CACHE_SIZE);
	CPU_NUM_L3_CACHES = IniGetInt (INI_FILE, "CpuL3NumCaches", CPU_NUM_L3_CACHES);
	CPU_L3_CACHE_SIZE = IniGetInt (INI_FILE, "CpuL3CacheSize", CPU_L3_CACHE_SIZE);
	CPU_L3_CACHE_LINE_SIZE = IniGetInt (INI_FILE, "CpuL3CacheLineSize", CPU_L3_CACHE_LINE_SIZE);
	CPU_L3_SET_ASSOCIATIVE = IniGetInt (INI_FILE, "CpuL3SetAssociative", CPU_L3_SET_ASSOCIATIVE);
	CPU_L3_CACHE_INCLUSIVE = IniGetInt (INI_FILE, "CpuL3CacheInclusive", CPU_L3_CACHE_INCLUSIVE);

	CPU_TOTAL_L4_CACHE_SIZE = IniGetInt (INI_FILE, "CpuL4TotalCacheSize", CPU_TOTAL_L4_CACHE_SIZE);
	CPU_NUM_L4_CACHES = IniGetInt (INI_FILE, "CpuL4NumCaches", CPU_NUM_L4_CACHES);
	CPU_L4_CACHE_INCLUSIVE = IniGetInt (INI_FILE, "CpuL4CacheInclusive", CPU_L4_CACHE_INCLUSIVE);

/* Let the user override the CPUID brand string.  It should never be necessary. */
/* However, one Athlon owner's brand string became corrupted with illegal characters. */
	
	IniGetString (INI_FILE, "CpuBrand", CPU_BRAND, sizeof(CPU_BRAND), CPU_BRAND);

/* Allow overriding the hwloc's generated values for number of physical processors, hyperthreads, and NUMA nodes. */

	NUM_CPUS = IniGetInt (INI_FILE, "NumCPUs", NUM_CPUS);
	if (NUM_CPUS == 0) NUM_CPUS = 1;
	CPU_HYPERTHREADS = IniGetInt (INI_FILE, "CpuNumHyperthreads", CPU_HYPERTHREADS);
	if (CPU_HYPERTHREADS == 0) CPU_HYPERTHREADS = 1;
	NUM_NUMA_NODES = IniGetInt (INI_FILE, "NumNUMANodes", NUM_NUMA_NODES);
	if (NUM_NUMA_NODES == 0) NUM_NUMA_NODES = 1;

/* Apply sane corrections if NUM_CPUs was reduced significantly) */

	if (CPU_NUM_L1_CACHES > NUM_CPUS) CPU_NUM_L1_CACHES = NUM_CPUS;
	if (CPU_NUM_L2_CACHES > NUM_CPUS) CPU_NUM_L2_CACHES = NUM_CPUS;
	if (CPU_NUM_L3_CACHES > NUM_CPUS) CPU_NUM_L3_CACHES = NUM_CPUS;
	if (CPU_NUM_L4_CACHES > NUM_CPUS) CPU_NUM_L4_CACHES = NUM_CPUS;
	if (NUM_NUMA_NODES > NUM_CPUS) NUM_NUMA_NODES = NUM_CPUS;

/* Let user override the CPU architecture */

	CPU_ARCHITECTURE = IniGetInt (INI_FILE, "CpuArchitecture", CPU_ARCHITECTURE);

/* We also compute the number of "threading nodes".  That is, number of nodes where we don't want to split a worker's threads */
/* across 2 threading nodes.  For example, there may well be a performance penalty if a worker's threads are on two different */
/* physical CPUS or access data from two different L3 caches. */
// bug -- may need much more sophisticated hwloc code here, such as handling asymetric cores or caches per threading node

	NUM_THREADING_NODES = hwloc_get_nbobjs_by_type (hwloc_topology, HWLOC_OBJ_PACKAGE);
	if (CPU_NUM_L3_CACHES > NUM_THREADING_NODES) NUM_THREADING_NODES = CPU_NUM_L3_CACHES;
	NUM_THREADING_NODES = IniGetInt (INI_FILE, "NumThreadingNodes", NUM_THREADING_NODES);
	if (NUM_THREADING_NODES < 1 || NUM_CPUS % NUM_THREADING_NODES != 0) NUM_THREADING_NODES = 1;

/* Now get the CPU speed */

	getCpuSpeed ();
}

/* Print the machine topology as discovered by hwloc library */

void topology_print_children (
	hwloc_obj_t obj,
        int depth)
{
	char type[32], attr[1024], cpuset[256], topbuf[1500];
	unsigned int i;
	if (obj == NULL) return;  // Shouldn't happen
	hwloc_obj_type_snprintf (type, sizeof(type), obj, 0);
	sprintf (topbuf, "%*s%s", 2*depth, " ", type);
	if (obj->os_index != (unsigned) -1)
		sprintf (topbuf+strlen(topbuf), "#%u", obj->os_index);
	hwloc_obj_attr_snprintf (attr, sizeof(attr), obj, ", ", 1 /* verbose */);
	if (obj->type == HWLOC_OBJ_CORE || obj->type == HWLOC_OBJ_PU)
		hwloc_bitmap_snprintf (cpuset, sizeof(cpuset), obj->cpuset);
	else
		cpuset[0] = 0;
	if (attr[0] && cpuset[0]) sprintf (topbuf+strlen(topbuf), " (%s, cpuset: %s)", attr, cpuset);
	else if (attr[0]) sprintf (topbuf+strlen(topbuf), " (%s)", attr);
	else if (cpuset[0]) sprintf (topbuf+strlen(topbuf), " (cpuset: %s)", cpuset);
	strcat (topbuf, "\n");
	for (i = 0; i < obj->arity; i++) {
		if (IniGetInt (INI_FILE, "OutputTopology", 0)&&(depth==0))
			if (verbose)
				OutputBoth ("Machine topology as determined by hwloc library:\n");
			else
				OutputStr ("Machine topology as determined by hwloc library:\n");
		topology_print_children (obj->children[i], depth + 1);
	}
	if (IniGetInt (INI_FILE, "OutputTopology", 0)) {
		if (verbose) {
			OutputBoth (topbuf);
		}
		else {
			OutputStr (topbuf);
		}
	}
}

/* Format a long or very long textual cpu description */

void getCpuDescription (
	char	*buf,			/* A 512 byte buffer */
	int	long_desc)		/* True for a very long description */
{

/* Recalculate the CPU speed in case speed step has changed the original settings. */

	getCpuSpeed ();

/* Now format a pretty CPU description */

	sprintf (buf, "%s\nCPU speed: %.2f MHz", CPU_BRAND, UNOFFICIAL_CPU_SPEED);
	if (NUM_CPUS > 1 && CPU_HYPERTHREADS > 1)
		sprintf (buf + strlen (buf), ", %lu hyperthreaded cores", NUM_CPUS);
	else if (NUM_CPUS > 1)
		sprintf (buf + strlen (buf), ", %lu cores", NUM_CPUS);
	else if (CPU_HYPERTHREADS > 1)
		sprintf (buf + strlen (buf), ", with hyperthreading");
	strcat (buf, "\n");
	if (CPU_FLAGS) {
		strcat (buf, "CPU features: ");
		if (CPU_FLAGS & CPU_RDTSC) strcat (buf, "RDTSC, ");
		if (CPU_FLAGS & CPU_CMOV) strcat (buf, "CMOV, ");
		if (CPU_FLAGS & CPU_MMX) strcat (buf, "MMX, ");
		if (CPU_FLAGS & CPU_3DNOW) strcat (buf, "3DNow!, ");
		else if (CPU_FLAGS & CPU_3DNOW_PREFETCH) strcat (buf, "3DNow! Prefetch, ");
		else if (CPU_FLAGS & CPU_PREFETCHW) strcat (buf, "Prefetchw, ");
		else if (CPU_FLAGS & CPU_PREFETCH) strcat (buf, "Prefetch, ");
		if (CPU_FLAGS & CPU_SSE) strcat (buf, "SSE, ");
		if (CPU_FLAGS & CPU_SSE2) strcat (buf, "SSE2, ");
		if (CPU_FLAGS & CPU_SSE41) strcat (buf, "SSE4, ");
		if (CPU_FLAGS & CPU_AVX) strcat (buf, "AVX, ");
		if (CPU_FLAGS & CPU_AVX2) strcat (buf, "AVX2, ");
		if (CPU_FLAGS & (CPU_FMA3 | CPU_FMA4)) strcat (buf, "FMA, ");
		if (CPU_FLAGS & CPU_AVX512F) strcat (buf, "AVX512F, ");
		strcpy (buf + strlen (buf) - 2, "\n");
	}

	strcat (buf, "L1 cache size: ");
	if (CPU_NUM_L1_CACHES <= 0) strcat (buf, "unknown");
	if (CPU_NUM_L1_CACHES > 1) sprintf (buf + strlen (buf), "%dx", CPU_NUM_L1_CACHES);
	if (CPU_NUM_L1_CACHES >= 1) sprintf (buf + strlen (buf), "%d KB", CPU_TOTAL_L1_CACHE_SIZE / CPU_NUM_L1_CACHES);
	
	strcat (buf, ", L2 cache size: ");
	if (CPU_NUM_L2_CACHES <= 0) strcat (buf, "unknown");
	if (CPU_NUM_L2_CACHES > 1) sprintf (buf + strlen (buf), "%dx", CPU_NUM_L2_CACHES);
	if (CPU_NUM_L2_CACHES >= 1) {
		if ((CPU_TOTAL_L2_CACHE_SIZE / CPU_NUM_L2_CACHES) & 0x3FF)
			sprintf (buf + strlen (buf), "%d KB", CPU_TOTAL_L2_CACHE_SIZE / CPU_NUM_L2_CACHES);
		else
			sprintf (buf + strlen (buf), "%d MB", CPU_TOTAL_L2_CACHE_SIZE / CPU_NUM_L2_CACHES / 1024);
	}

	if (CPU_NUM_L3_CACHES > 0) {
		strcat (buf, CPU_NUM_L4_CACHES > 0 ? "\n" : ", ");
		strcat (buf, "L3 cache size: ");
		if (CPU_NUM_L3_CACHES > 1) sprintf (buf + strlen (buf), "%dx", CPU_NUM_L3_CACHES);
		if ((CPU_TOTAL_L3_CACHE_SIZE / CPU_NUM_L3_CACHES) & 0x3FF)
			sprintf (buf + strlen (buf), "%d KB", CPU_TOTAL_L3_CACHE_SIZE / CPU_NUM_L3_CACHES);
		else
			sprintf (buf + strlen (buf), "%d MB", CPU_TOTAL_L3_CACHE_SIZE / CPU_NUM_L3_CACHES / 1024);
	}

	if (CPU_NUM_L4_CACHES > 0) {
		strcat (buf, ", L4 cache size: ");
		if (CPU_NUM_L4_CACHES > 1) sprintf (buf + strlen (buf), "%dx", CPU_NUM_L4_CACHES);
		if ((CPU_TOTAL_L4_CACHE_SIZE / CPU_NUM_L4_CACHES) & 0x3FF)
			sprintf (buf + strlen (buf), "%d KB", CPU_TOTAL_L4_CACHE_SIZE / CPU_NUM_L4_CACHES);
		else
			sprintf (buf + strlen (buf), "%d MB", CPU_TOTAL_L4_CACHE_SIZE / CPU_NUM_L4_CACHES / 1024);
	}

	if (! long_desc) return;
	strcat (buf, "\nL1 cache line size: ");
	if (CPU_L1_CACHE_LINE_SIZE < 0) strcat (buf, "unknown");
	else sprintf (buf+strlen(buf), "%d bytes", CPU_L1_CACHE_LINE_SIZE);
	strcat (buf, ", L2 cache line size: ");
	if (CPU_L2_CACHE_LINE_SIZE < 0) strcat (buf, "unknown");
	else sprintf (buf+strlen(buf), "%d bytes", CPU_L2_CACHE_LINE_SIZE);
	strcat (buf, "\n");
	topology_print_children (hwloc_get_root_obj (hwloc_topology), 0);
}

/* Determine if a small number is prime */

int isPrime (
	unsigned long p)
{
	unsigned long i;
	if (p < 2)													// 1 is not prime.
		return (FALSE);
	for (i = 2; (i < (1<<16)) && (i*i <= p); i = (i + 1) | 1)	// i >= 2^16 is forbidden
		if (p % i == 0) return (FALSE);
	return (TRUE);
}

/*------------------------------------------------------------------------------
| Portable routines to read and write ini files that are not in gwnum library...
+-------------------------------------------------------------------------------*/

void save_IniFile (char *filename, char *savedfilename) {
	struct IniCache *p;	
	p = openIniFile (filename, 1);		// open and read the source IniFile.
	p->filename = savedfilename;		// put the target filename in the structure.
	writeIniFile (p);					// Write the target.
	p->filename = filename;				// Restore the structure in memory.
}

void IniFileOpen (
	char	*filename,
	int	immediate_writes)
{
	struct IniCache *p;
	p = openIniFile (filename, 1);
	p->immediate_writes = immediate_writes;
}

// Determine the names of the INI files.

void nameIniFiles (
	int	named_ini_files)
{
	char	buf[120];
	int		lchd;

	hwloc_topology_init (&hwloc_topology);
	hwloc_topology_load (hwloc_topology);
        
	if (named_ini_files < 0) {
		strcpy (INI_FILE, "llr.ini");
		strcpy (RESFILE, "lresults.txt");
		strcpy (LOGFILE, "lprime.log");
		strcpy (EXTENSION, "");
	} else {
		sprintf (INI_FILE, "llr%04d.ini", named_ini_files);
		sprintf (RESFILE, "lresu%04d.txt", named_ini_files);
		sprintf (LOGFILE, "lprim%04d.log", named_ini_files);
		sprintf (EXTENSION, ".%03d", named_ini_files);
	}

// Let the user rename these files and pick a different working directory

	IniGetString (INI_FILE, "WorkingDir", buf, sizeof(buf), NULL);
	IniGetString (INI_FILE, "results.txt", RESFILE, 80, RESFILE);
	IniGetString (INI_FILE, "prime.log", LOGFILE, 80, LOGFILE);
	IniGetString (INI_FILE, "prime.ini", INI_FILE, 80, INI_FILE);
	if (buf[0]) {
		lchd = _chdir (buf);
		IniFileOpen (INI_FILE, 0);
	}
}

// Read the INI files.

void readIniFiles () 
{ 
	int	temp; 
 
	getCpuInfo (); 
 
	PRECISION = (unsigned int) IniGetInt (INI_FILE, "PercentPrecision", 2); 
	if (PRECISION > 6) PRECISION = 6; 
 
	ITER_OUTPUT = IniGetInt (INI_FILE, "OutputIterations", 10000); 
	if (ITER_OUTPUT <= 0) ITER_OUTPUT = 1; 
	ITER_OUTPUT_RES = IniGetInt (INI_FILE, "ResultsFileIterations", 
				     99999999); 
	if (ITER_OUTPUT_RES < 1000) ITER_OUTPUT_RES = 1000; 
	DISK_WRITE_TIME = IniGetInt (INI_FILE, "DiskWriteTime", 30); 
	TWO_BACKUP_FILES = (int) IniGetInt (INI_FILE, "TwoBackupFiles", 1); 
	RUN_ON_BATTERY = (int) IniGetInt (INI_FILE, "RunOnBattery", 1); 
 
	temp = (int) IniGetInt (INI_FILE, "ErrorCheck", 0); 
	ERRCHK = (temp != 0); 
	PRIORITY = (unsigned int) IniGetInt (INI_FILE, "Priority", 1); 
	CPU_AFFINITY = (unsigned int) IniGetInt (INI_FILE, "Affinity", 99); 
	HIDE_ICON = (int) IniGetInt (INI_FILE, "HideIcon", 0); 
	TRAY_ICON = (int) IniGetInt (INI_FILE, "TrayIcon", 1); 
 
// Guess the CPU type if it isn't known.  Otherwise, validate it.
        
	getCpuInfo (); 
 
// Other oddball options
 
	CUMULATIVE_TIMING = IniGetInt (INI_FILE, "CumulativeTiming", 0); 
	HIGH_RES_TIMER = isHighResTimerAvailable (); 
} 
 
/*---------------------------------------------------------------------------*/

/* Output string to screen or results file */

void OutputSomewhere (
	char	*buf)
{
	if (NO_GUI) writeResults (buf);
	else OutputStr (buf);
}

/* Output string to both the screen and results file */

void OutputBoth (
	char	*buf)
{
	OutputStr (buf);
	writeResults (buf);
}

/* Output message to screen and prime.log file */

void LogMsg (
	char	*str)
{
	int	fd;
	unsigned long filelen;
static	time_t	last_time = 0;
	time_t	this_time;

/* Output it to the screen */

	OutputStr (str);

/* Open the log file and position to the end */

	fd = _open (LOGFILE, _O_TEXT | _O_RDWR | _O_CREAT, 0666);
	if (fd < 0) {
		OutputStr ("Unable to open log file.\n");
		return;
	}
	filelen = _lseek (fd, 0L, SEEK_END);

/* Output to the log file only if it hasn't grown too big */

	if (filelen < 250000) {

/* If it has been at least 5 minutes since the last time stamp */
/* was output, then output a new timestamp */

		time (&this_time);
		if (this_time - last_time > 300) {
			char	buf[48];
			last_time = this_time;
			buf[0] = '[';
			strcpy (buf+1, ctime (&this_time));
			sprintf (buf+25, " - ver %s]\n", LLR_VERSION);
			writelg = _write (fd, buf, strlen (buf));
		}

/* Output the message */

		writelg = _write (fd, str, strlen (str));
	}

/* Display message about full log file */
	
	else {
		char	*fullmsg = "Prime.log file full.  Please delete it.\n";
		OutputStr (fullmsg);
		if (filelen < 251000)
			writelg = _write (fd, fullmsg, strlen (fullmsg));
	}
	_close (fd);
}

int gmodi (uint32_t, giant);
void uldivg (uint32_t, giant);


/* Generate temporary file name */
 
void tempFileName ( 
	char	*buf, char c, giant NN) 
{ 
	int remainder;
 
	remainder = gmodi(19999981, NN);
	sprintf (buf, "%c%07i", c, remainder % 10000000); 
} 

/* See if the given file exists */

int fileExists (
	char	*filename)
{
	int	fd;
	fd = _open (filename, _O_RDONLY | _O_BINARY);
	if (fd < 0) return (0);
	_close (fd);
	return (1);
}

/* Open the results file and write a line to the end of it. */

int writeResults (
	char	*msg)
{
static	time_t	last_time = 0;
	time_t	this_time;
	int	fd;

	if (IniGetInt (INI_FILE, "NoLresultFile", 0))
		return (TRUE);

/* Open file, position to end */

	fd = _open (RESFILE, _O_TEXT | _O_RDWR | _O_CREAT | _O_APPEND, 0666);
	if (fd < 0) {
		LogMsg ("Error opening the results file ; see result below :\n");
		LogMsg (msg);			// Log the unwrited message (Darren Bedwell'request)
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
		if (verbose || restarting)
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

// There are (at least) 8 PRP or Prime residue types for testing N=(k*b^n+c)/d:

#define	PRP_TYPE_FERMAT		1	// Fermat PRP.  Calculate a^(N-1) mod N.  PRP if result = 1
#define	PRP_TYPE_SPRP		2	// SPRP variant.  Calculate a^((N-1)/2) mod N.  PRP if result = +/-1
#define	PRP_TYPE_FERMAT_VAR	3	// Type 1 variant,b=2,d=1. Calculate a^(N-c) mod N.  PRP if result = a^-(c-1)
#define	PRP_TYPE_SPRP_VAR	4	// Type 2 variant,b=2,d=1. Calculate a^((N-c)/2) mod N.  PRP if result = +/-a^-((c-1)/2)
#define	PRP_TYPE_COFACTOR	5	// Cofactor variant.  Calculate a^(N*d-1) mod N*d.  PRP if result = a^(d-1) mod N
#define PROTH_TYPE			6	// Proth prime variant. Calculate a^((N-1)/2) mod N with Jacobi (a,N) = -1. Prime iff result = -1
#define	GMN_TYPE			7	// A special variant of Proth prime test
#define	WAGSTAFF_TYPE		8	// A special variant of Cofactor SPRP test 

// Primenet encourages programs to return type 1 PRP residues as that has been the standard for prime95, PFGW, LLR for many years.

#define PRP_MAGICNUM		0x87f2a91b
#define PRP_VERSION		4

#ifndef SEC1
#define SEC1(p)			0UL
#define SEC2(p,hi,lo,u,e)	0UL
#define SEC3(p)			0UL
#define SEC4(p)			0UL
#define SEC5(p,b1,b2)		0UL
#endif

/* Compare the final giant in a PRP run.  Different PRP residue types check for different final values */

int isPRPg (
	giant	v,			/* Final result of PRP powering */
	giant	N,			/* Number we are PRPing, (k*b^n+c)/known_factors */
	struct work_unit *w,		/* Number being tested */
	unsigned int prp_base,		/* PRP base */
	int	prp_residue_type)	/* Type of PRP test performed */
{

	int	result = FALSE;
	giant kbnc, known_factors, reduced_v, compare_val;

/* Standard Fermat PRP, test for one */

	if (prp_residue_type == PRP_TYPE_FERMAT) return (isone (v));

/* SPRP test is PRP if result is one or minus one */

	if (prp_residue_type == PRP_TYPE_SPRP || prp_residue_type == PROTH_TYPE) {
		if (prp_residue_type == PRP_TYPE_SPRP && isone (v))
			return (TRUE);
		subg (v, N); result = isone (N); addg (v, N);
		return (result);
	}

/* Handle the cofactor case.  We calculated v = a^(N*KF-1) mod (N*KF).  We have a PRP if (v mod N) = (a^(KF-1)) mod N */

	if (prp_residue_type == PRP_TYPE_COFACTOR) {
		// Calculate k*b^n+c
		if (w->k > 0.0) {
			kbnc = allocgiant ((w->n + (unsigned long)_log2(w->k)) + 5);
			known_factors = allocgiant ((w->n + (unsigned long)_log2(w->k)) + 5);
			reduced_v = allocgiant ((w->n + (unsigned long)_log2(w->k)) + 5);
			compare_val = allocgiant ((w->n + (unsigned long)_log2(w->k)) + 5);
			dbltog (w->k, kbnc);
		}
		else {
			kbnc = allocgiant ((w->n + bitlen(gk)) + 5);
			known_factors = allocgiant ((w->n + bitlen(gk)) + 5);
			reduced_v = allocgiant ((w->n + bitlen(gk)) + 5);
			compare_val = allocgiant ((w->n + bitlen(gk)) + 5);
			gtog (gk, kbnc);
		}
		gshiftleft (w->n, kbnc);
		iaddg (w->c, kbnc);
		gtog (kbnc, known_factors);
		divg (N, known_factors);
		ulsubg (1, known_factors);
		ultog (prp_base, compare_val);
		powermodg (compare_val, known_factors, kbnc);
		modg (N, compare_val);
		modg (kbnc, v);
		gtog (v, reduced_v);
		modg (N, reduced_v);
		result = (gcompg (reduced_v, compare_val) == 0);
		free (kbnc);
		free (known_factors);
		free (reduced_v);
		free (compare_val);
	}

/* Handle the weird cases -- Fermat and SPRP variants, one of which gpuOwl uses 

	else {
		mpz_t	power;

/* Calculate the compare_value 

		mpz_init (power);
		if (prp_residue_type == PRIMENET_PRP_TYPE_FERMAT_VAR) mpz_set_si (power, - (w->c - 1));
		else mpz_set_si (power, - (w->c - 1) / 2);
		mpz_set_ui (compare_val, prp_base);
		mpz_powm (compare_val, compare_val, power, mpz_N);
		mpz_clear (power);

/* Now do the comparison(s) 

		result = mpz_eq (mpz_v, compare_val);
		if (prp_residue_type == PRIMENET_PRP_TYPE_SPRP_VAR && !result) {
			mpz_sub (compare_val, mpz_N, compare_val);	// Negate compare val
			result = mpz_eq (mpz_v, compare_val);
		}
	}

/* Cleanup and return 

	mpz_clear (mpz_v);
	mpz_clear (mpz_N);
	mpz_clear (compare_val);*/
	return (result);
}

/* Mul giant by a power of the PRP or Proth base.  Used in optimizing PRPs for k*2^n+c numbers. */

void basemulg (
	giant	v,			/* Giant to multiply by base^power */
	struct work_unit *w,		/* Number being tested */
	unsigned int prp_base,		/* PRP base */
	int	power)			/* Desired power of the PRP base */
{
	giant	modulus, prp_base_power;

/* If power is zero, then multiply by base^0 is a no-op */

	if (power == 0) return;

/* Generate the modulus (k*b^n+c), b is known to be 2 */
	if (w->k > 0.0) {
		modulus = allocgiant ((w->n + (unsigned long)_log2(w->k)) + 5);
		prp_base_power = allocgiant ((w->n + (unsigned long)_log2(w->k)) + 5);
		dbltog (w->k, modulus);
	}
	else {
		modulus = allocgiant ((w->n + bitlen(gk)) + 5);
		prp_base_power = allocgiant ((w->n + bitlen(gk)) + 5);
		gtog (gk, modulus);
	}
	gshiftleft (w->n, modulus);
	iaddg (w->c, modulus);

/* Calculate prp_base^power mod k*b^n+c */

	ultog (prp_base, prp_base_power);
	powermod (prp_base_power, abs(power), modulus);
	if (power < 0)
		invg (modulus, prp_base_power);	// to avoid using powermod with a negative exponent (probably due to a bug in giants.c).

/* Multiply by prp_base_power to get the final result */

	mulg (prp_base_power, v);
	modg (modulus, v);

/* Cleanup and return */

	free (prp_base_power);
	free (modulus);
}


/* Data structure used in reading save files and their backups as well as */
/* renaming bad save files. */

typedef struct read_save_file_state {
	int	thread_num;
	int	read_attempt;
	int	a_save_file_existed;
	int	a_non_bad_save_file_existed;
	int	num_original_bad_files;
	int	num_save_files_renamed;
	char	base_filename[80];
	char	current_filename[80];
} readSaveFileState;

/* Data structure used in writing save files and their backups */

typedef struct write_save_file_state {
	char	base_filename[80];
	int	num_ordinary_save_files;
	int	num_special_save_files;		/* Example: Number of save files to keep that passed the Jacobi error check */
	uint64_t special;			/* Bit array for which ordinary save files are special */
} writeSaveFileState;

/* Open the save file for writing.  Either overwrite or generate a temporary */
/* file name to write to, where we will rename the file after the file is */
/* successully written. */

int openWriteSaveFile (
	writeSaveFileState *state)
{
	char	output_filename[32];
	int	fd;

/* If we are allowed to create multiple intermediate files, then use a .write extension */
/* The value 99, not accessible via the GUI, is a special value meaning overwrite the */
/* existing save file -- a very dangerous choice.  You might use this for a floppy or */
/* small USB stick installation where there is no room for two save files. */
/* NOTE: This behavior is different than v24 where when the user selected one save */
/* file, then he got the dangerous overwrite option. */

	if (state->num_ordinary_save_files == 99)
		strcpy (output_filename, state->base_filename);
	else
		sprintf (output_filename, "%s.write", state->base_filename);

/* Now save to the intermediate file */

	fd = _open (output_filename, _O_BINARY | _O_WRONLY | _O_TRUNC | _O_CREAT, CREATE_FILE_ACCESS);
	return (fd);
}

/* Close the save file we finished writing.  If necessary, delete old */
/* save file, and rename just written save file. */

void closeWriteSaveFile (
	writeSaveFileState *state,
	int	fd)
{
	char	dest_filename[32], src_filename[32];
	int	rename_count;

/* Flush data to disk and close the save file. */

	_commit (fd);
	_close (fd);

/* If no renaming is needed, we're done */

	if (state->num_ordinary_save_files == 99) return;

/* Save files that are special will be one step further down the chain after renaming */

	state->special <<= 1;

/* Decide how many save files need renaming (does the last ordinary file deserve to move into the special save files?) */

	rename_count = bittst (&state->special, state->num_ordinary_save_files) ?
			       state->num_ordinary_save_files + state->num_special_save_files : state->num_ordinary_save_files;

/* Delete the last file in the rename chain */

	if (rename_count == 1) strcpy (dest_filename, state->base_filename);
	else if (rename_count == 2) sprintf (dest_filename, "%s.bu", state->base_filename);
	else sprintf (dest_filename, "%s.bu%d", state->base_filename, rename_count-1);
	_unlink (dest_filename);

/* Perform the proper number of renames */

	while (rename_count--) {
		if (rename_count == 0) sprintf (src_filename, "%s.write", state->base_filename);
		else if (rename_count == 1) strcpy (src_filename, state->base_filename);
		else if (rename_count == 2) sprintf (src_filename, "%s.bu", state->base_filename);
		else sprintf (src_filename, "%s.bu%d", state->base_filename, rename_count-1);
		rename (src_filename, dest_filename);
		strcpy (dest_filename, src_filename);
	}
}

/* Mark the current save file as special (a super good save file -- Jacobi or Gerbicz checked) */

void setWriteSaveFileSpecial (
	writeSaveFileState *state)
{
	state->special |= 1;
}

/* Close and delete the save file we were writing.  This is done */
/* when an error occurs while writing the save file. */

void deleteWriteSaveFile (
	writeSaveFileState *state,
	int	fd)
{
	char	output_filename[32];

/* Close and delete the save file */

	_close (fd);
	if (state->num_ordinary_save_files == 99)
		strcpy (output_filename, state->base_filename);
	else
		sprintf (output_filename, "%s.write", state->base_filename);
	_unlink (output_filename);
}

/* Delete save files when work unit completes. */

void unlinkSaveFiles (
	writeSaveFileState *state)
{
	int	i, maxbad;
	char	unlink_filename[80];

	maxbad = IniGetInt (INI_FILE, "MaxBadSaveFiles", 10);
	for (i = 1; i <= maxbad; i++) {
		sprintf (unlink_filename, "%s.bad%d", state->base_filename, i);
		_unlink (unlink_filename);
	}
	if (state->num_ordinary_save_files != 99) {
		for (i = 1; i < state->num_ordinary_save_files + state->num_special_save_files; i++) {
			if (i == 1) sprintf (unlink_filename, "%s.bu", state->base_filename);
			else sprintf (unlink_filename, "%s.bu%d", state->base_filename, i);
			_unlink (unlink_filename);
		}
	}
	if (state->base_filename[0] == 'p') {
		sprintf (unlink_filename, "q%s", state->base_filename+1);
		_unlink (unlink_filename);
		sprintf (unlink_filename, "r%s", state->base_filename+1);
		_unlink (unlink_filename);
	}
	sprintf (unlink_filename, "%s.write", state->base_filename);
	_unlink (unlink_filename);
	_unlink (state->base_filename);
}

/* Format a message for writing to the results file and sending to the */
/* server.  We prepend the assignment ID to the message.  If operating */
/* offline, then prepend other information. */

void formatMsgForResultsFile (
	char	*buf,		/* Msg to prepend to, 200 characters max */
	struct work_unit *w)
{
	char	newbuf[2000];

/* Output a USERID/COMPID prefix for result messages 

	if (!USERID[0])
		strcpy (newbuf, buf);
	else if (!COMPID[0])
		sprintf (newbuf, "UID: %s, %s", USERID, buf);
	else
		sprintf (newbuf, "UID: %s/%s, %s", USERID, COMPID, buf);

/* Output the assignment ID too.  That alone should be enough info to */
/* credit the correct userID.  However, we still output the user ID as it */
/* is far more human friendly than an assignment ID. */

	if (w->assignment_uid[0])
		sprintf (newbuf + strlen (newbuf) - 1,
			 ", AID: %s\n", w->assignment_uid);

/* Now truncate the message to 200 characters */

	newbuf[200] = 0;
	strcpy (buf, newbuf);
}

/* Read and write intermediate results to a file */

int read_gwnum (
	gwhandle *gwdata,
	ghandle *gdata,
	int	fd,
	gwnum	g,
	long	*sum)
{
	giant	tmp;
	long	i, len, bytes;

	tmp = popg (gdata, ((unsigned long) gwdata->bit_length >> 4) + 10);
	if (_read (fd, &len, sizeof (long)) != sizeof (long)) return (FALSE);
	bytes = len * sizeof (long);
	if (_read (fd, tmp->n, bytes) != bytes) return (FALSE);
	tmp->sign = len;
	*sum += len;
	for (i = 0; i < len; i++) *sum += tmp->n[i];
	gianttogw (gwdata, tmp, g);
	pushg (gdata, 1);
	return (TRUE);
}

int write_gwnum (
	gwhandle *gwdata,
	ghandle *gdata,
	int	fd,
	gwnum	g,
	long	*sum)
{
	giant	tmp;
	long	i, len, bytes;

	tmp = popg (gdata, ((unsigned long) gwdata->bit_length >> 4) + 10);
	gwtogiant (gwdata, g, tmp);
	len = tmp->sign;
	if (_write (fd, &len, sizeof (long)) != sizeof (long)) return (FALSE);
	bytes = len * sizeof (long);
	if (_write (fd, tmp->n, bytes) != bytes) return (FALSE);
	*sum += len;
	for (i = 0; i < len; i++) *sum += tmp->n[i];
	pushg (gdata, 1);
	return (TRUE);
}

int read_long (
	int	fd,
	unsigned long *val,
	long	*sum)
{
	if (_read (fd, val, sizeof (long)) != sizeof (long)) return (FALSE);
	*sum += *val;
	return (TRUE);
}

int write_long (
	int	fd,
	unsigned long val,
	long	*sum)
{
	if (_write (fd, &val, sizeof (long)) != sizeof (long)) return (FALSE);
	*sum += val;
	return (TRUE);
}


int read_long298 (
	int	fd,
	unsigned long *val,
	unsigned long *sum)
{
	uint32_t tmp;

	if (_read (fd, &tmp, sizeof (uint32_t)) != sizeof (uint32_t))
		return (FALSE);
	if (sum != NULL) *sum = (uint32_t) (*sum + tmp);
	*val = tmp;
	return (TRUE);
}

int write_long298 (
	int	fd,
	unsigned long val,
	unsigned long *sum)
{
	uint32_t tmp;

	tmp = (uint32_t) val;
	if (_write (fd, &tmp, sizeof (uint32_t)) != sizeof (uint32_t))
		return (FALSE);
	if (sum != NULL) *sum = (uint32_t) (*sum + tmp);
	return (TRUE);
}


int read_slong (
	int	fd,
	long	*val,
	unsigned long *sum)
{
	int32_t tmp;

	if (_read (fd, &tmp, sizeof (int32_t)) != sizeof (int32_t))
		return (FALSE);
	if (sum != NULL) *sum = (uint32_t) (*sum + (uint32_t) tmp);
	*val = tmp;
	return (TRUE);
}

int write_slong (
	int	fd,
	long	val,
	unsigned long *sum)
{
	int32_t tmp;

	tmp = (int32_t) val;
	if (_write (fd, &tmp, sizeof (int32_t)) != sizeof (int32_t))
		return (FALSE);
	if (sum != NULL) *sum = (uint32_t) (*sum + (uint32_t) tmp);
	return (TRUE);
}

int read_double (
	int	fd,
	double *val,
	long	*sum)
{
	if (_read (fd, val, sizeof (double)) != sizeof (double)) return (FALSE);
	*sum += (long)floor(*val);
	return (TRUE);
}

int write_double (
	int	fd,
	double val,
	long	*sum)
{
	if (_write (fd, &val, sizeof (double)) != sizeof (double)) return (FALSE);
	*sum += (long)floor(val);
	return (TRUE);
}
int read_double298 (
	int	fd,
	double	*val,
	unsigned long *sum)
{
	if (_read (fd, val, sizeof (double)) != sizeof (double))
		return (FALSE);
	if (sum != NULL) *sum = (uint32_t) (*sum + (uint32_t) *val);
	return (TRUE);
}

int write_double298 (
	int	fd,
	double	val,
	unsigned long *sum)
{
	if (_write (fd, &val, sizeof (double)) != sizeof (double))
		return (FALSE);
	if (sum != NULL) *sum += (uint32_t) (*sum + (uint32_t) val);
	return (TRUE);
}

/* Routines to read and write the common header portion of all save files */
/* The save file format is: */
/*	u32		magic number  (different for ll, p-1, prp, tf, ecm) */
/*	u32		version number */
/*	double		k in k*b^n+c */
/*	u32		b in k*b^n+c */
/*	u32		n in k*b^n+c */
/*	s32		c in k*b^n+c */
/*	double		pct complete */
/*	char(11)	stage */
/*	char(1)		pad */
/*	u32		checksum of all following data */

/* Read and test the "magic number" in the first 4 bytes of the file. */
/* We use this to detect save files created by this program prior to the */
/* common header format. */

/* Routines to read and write a byte array from and to a save file */

int read_array (
	int	fd,
	char	*buf,
	unsigned long len,
	unsigned long *sum)
{
	unsigned long i;
	unsigned char *ubuf;

	if ((unsigned long)_read (fd, buf, len) != len) return (FALSE);
	ubuf = (unsigned char *) buf;
	if (sum != NULL)
		for (i = 0; i < len; i++)
			*sum = (uint32_t) (*sum + ubuf[i]);
	return (TRUE);
}

int write_array (
	int	fd,
	const char *buf,
	unsigned long len,
	unsigned long *sum)
{
	unsigned long i;
	unsigned char *ubuf;

	if (len == 0) return (TRUE);
	if ((unsigned long)_write (fd, buf, len) != len) return (FALSE);
	ubuf = (unsigned char *) buf;
	if (sum != NULL)
		for (i = 0; i < len; i++)
			*sum = (uint32_t) (*sum + ubuf[i]);
	return (TRUE);
}

#define CHECKSUM_OFFSET	48

int read_checksum (
	int	fd,
	unsigned long *sum)
{
	_lseek (fd, CHECKSUM_OFFSET, SEEK_SET);
	if (!read_long298 (fd, sum, NULL)) return (FALSE);
	return (TRUE);
}

int write_checksum (
	int	fd,
	unsigned long sum)
{
	_lseek (fd, CHECKSUM_OFFSET, SEEK_SET);
	if (!write_long298 (fd, sum, NULL)) return (FALSE);
	return (TRUE);
}

int read_magicnum (
	int	fd,
	unsigned long magicnum)
{
	unsigned long filenum;

/* Read the magic number from the first 4 bytes */

	_lseek (fd, 0, SEEK_SET);
	if (!read_long298 (fd, &filenum, NULL)) return (FALSE);

/* Return TRUE if the magic number matches the caller's desired magic number */

	return (filenum == magicnum);
}

/* Read the rest of the common header */

int read_header (
	int	fd,
	unsigned long *version,
	struct work_unit *w,
	unsigned long *sum)
{
	double	k;
	unsigned long b, n;
	long	c;
	char	pad;
	char	stage[11];
	double	pct_complete;
	unsigned long trash_sum;

/* Skip past the magic number in the first 4 bytes */

	_lseek (fd, sizeof (uint32_t), SEEK_SET);

/* Read the header */

	if (!read_long298 (fd, version, NULL)) return (FALSE);
	if (!read_double298 (fd, &k, NULL)) return (FALSE);
	if (!read_long298 (fd, &b, NULL)) return (FALSE);
	if (!read_long298 (fd, &n, NULL)) return (FALSE);
	if (!read_slong (fd, &c, NULL)) return (FALSE);
	if (!read_array (fd, stage, 11, NULL)) return (FALSE);
	if (!read_array (fd, &pad, 1, NULL)) return (FALSE);
	if (!read_double298 (fd, &pct_complete, NULL)) return (FALSE);
	if (sum == NULL) sum = &trash_sum;
//	_lseek (fd, CHECKSUM_OFFSET, SEEK_SET);
        if (_read (fd, sum, sizeof (unsigned long)) != sizeof (unsigned long)) return (FALSE);
        
/* Validate the k,b,n,c values */

	if (k != w->k || b != w->b || n != w->n || c != w->c) return (FALSE);

/* Set the work unit's stage and pct_complete fields */

	stage[10] = 0;
	strcpy (w->stage, stage);
	if (pct_complete < 0.0) pct_complete = 0.0;
	if (pct_complete > 1.0) pct_complete = 1.0;
	w->pct_complete = pct_complete;

/* Return success */

	return (TRUE);
}

int write_header (
	int	fd,
	unsigned long magicnum,
	unsigned long version,
	struct work_unit *w)
{
	char	pad = 0;
	unsigned long sum = 0;

	if (!write_long298 (fd, magicnum, NULL)) return (FALSE);
	if (!write_long298 (fd, version, NULL)) return (FALSE);
	if (!write_double298 (fd, w->k, NULL)) return (FALSE);
	if (!write_long298 (fd, w->b, NULL)) return (FALSE);
	if (!write_long298 (fd, w->n, NULL)) return (FALSE);
	if (!write_slong (fd, w->c, NULL)) return (FALSE);
	if (!write_array (fd, w->stage, 11, NULL)) return (FALSE);
	if (!write_array (fd, &pad, 1, NULL)) return (FALSE);
	if (!write_double298 (fd, w->pct_complete, NULL)) return (FALSE);
//	if (!write_long298 (fd, sum, NULL)) return (FALSE);
//	_lseek (fd, CHECKSUM_OFFSET, SEEK_SET);
	if (_write (fd, &sum, sizeof (unsigned long)) != sizeof (unsigned long)) return (FALSE);
	return (TRUE);
}


int read_gwnum298 (
	int	fd,
	gwhandle *gwdata,
	gwnum	g,
	unsigned long *sum)
{
	giant	tmp;
	unsigned long i, len, giantlen, bytes;

	if (!read_long298 (fd, &len, sum)) return (FALSE);
	if (len == 0) return (FALSE);

	giantlen = ((int) gwdata->bit_length >> 5) + 10;
	if (len > giantlen) return (FALSE);
	tmp = popg (&gwdata->gdata, giantlen);
	if (tmp == NULL) return (FALSE);	// BUG - we should return some other error code
						// otherwise caller will likely delete save file.

	bytes = len * sizeof (uint32_t);
	if ((unsigned long)_read (fd, tmp->n, bytes) != bytes) goto errexit;
	if (len && tmp->n[len-1] == 0) goto errexit;
	tmp->sign = len;
	*sum = (uint32_t) (*sum + len);
	for (i = 0; i < len; i++) *sum = (uint32_t) (*sum + tmp->n[i]);
	gianttogw (gwdata, tmp, g);
	pushg (&gwdata->gdata, 1);
	return (TRUE);

// Free memory and return failure

errexit:
	pushg (&gwdata->gdata, 1);
	return (FALSE);
}

int write_gwnum298 (
	int	fd,
	gwhandle *gwdata,
	gwnum	g,
	unsigned long *sum)
{
	giant	tmp;
	unsigned long i, len, bytes;

	tmp = popg (&gwdata->gdata, ((int) gwdata->bit_length >> 5) + 10);
	if (tmp == NULL) return (FALSE);
	if (gwtogiant (gwdata, g, tmp)) goto err;
	len = tmp->sign;
	if (len == 0) goto err;
	if (!write_long298 (fd, len, sum)) goto err;
	bytes = len * sizeof (uint32_t);
	if ((unsigned long)_write (fd, tmp->n, bytes) != bytes) goto err;
	*sum = (uint32_t) (*sum + len);
	for (i = 0; i < len; i++) *sum = (uint32_t) (*sum + tmp->n[i]);
	pushg (&gwdata->gdata, 1);
	return (TRUE);
err:	pushg (&gwdata->gdata, 1);
	return (FALSE);
}

int writeToFile (
	gwhandle *gwdata,
	ghandle *gdata,
	char	*filename,
	unsigned long j,
	gwnum	x,
	gwnum	y)
{
	char	newfilename[16];
	int	fd;
	unsigned long magicnum, version;
	long	sum = 0, i;

	if (nosaving)				// Requested new option... 11/02/11
		return (TRUE);

/* If we are allowed to create multiple intermediate files, then */
/* write to a file called yNNNNNNN. */

	strcpy (newfilename, filename);
	if (TWO_BACKUP_FILES) newfilename[0] = 'y';

/* Create the intermediate file */

	fd = _open (newfilename, _O_BINARY|_O_WRONLY|_O_TRUNC|_O_CREAT, 0666);
	if (fd < 0) return (FALSE);

/* Write the file header. */

	magicnum = 0x9f2b3cd4;
	if (_write (fd, &magicnum, sizeof (long)) != sizeof (long))
		goto writeerr;
	version = 1;
	if (_write (fd, &version, sizeof (long)) != sizeof (long))
		goto writeerr;

/* Write the file data */

	if (! write_long (fd, j, &sum)) goto writeerr;

/* Write the data values */

	if (! write_gwnum (gwdata, gdata, fd, x, &sum)) goto writeerr;
	if (y != NULL && ! write_gwnum (gwdata, gdata, fd, y, &sum)) goto writeerr; 

/* Save the five timers */

	for (i=0; i<5; i++) {
		if (timers[i+5] != 0.0) {// if the timer was running
			end_timer (i);			// update and save it
			if (! write_double (fd, timers[i], &sum)) {
				start_timer (i);	// and then, restart it, even if write is in error!
				goto writeerr;
			}
			if (! write_double (fd, timers[i+5], &sum)) { // save the timer status
				start_timer (i);	// and then, restart it, even if write is in error!
				goto writeerr;
			}
			start_timer (i);	// and then, restart it!
		}
		else {
			if (! write_double (fd, timers[i], &sum)) goto writeerr;	// save the timer
			if (! write_double (fd, timers[i+5], &sum)) goto writeerr;	// save its status
		}
	}

/* Write the checksum */

	if (_write (fd, &sum, sizeof (long)) != sizeof (long)) goto writeerr;

	_commit (fd);
	_close (fd);

/* Now rename the intermediate files */

	if (TWO_BACKUP_FILES) {
		_unlink (filename);
		rename (newfilename, filename);
	}
	return (TRUE);

/* An error occured.  Close and delete the current file. */

writeerr:
	_close (fd);
	_unlink (newfilename);
	return (FALSE);
}

int readFromFile (
	gwhandle *gwdata,
	ghandle *gdata,
	char	*filename,
	unsigned long *j,
	gwnum	x,
	gwnum	y)
{
	int	fd;
	unsigned long magicnum, version;
	long	sum = 0, i;
	char	errmsg[100];

/* Open the intermediate file */

	fd = _open (filename, _O_BINARY | _O_RDONLY);
	if (fd < 0) goto error;

/* Read the file header */

	if (_read (fd, &magicnum, sizeof (long)) != sizeof (long))
		goto readerr;
	if (magicnum != 0x9f2b3cd4) goto readerr;

	if (_read (fd, &version, sizeof (long)) != sizeof (long)) goto readerr;
	if (version != 1 && version != 2) goto readerr;

/* Read the file data */

	if (! read_long (fd, j, &sum)) goto readerr;

/* Read the values */

	if (! read_gwnum (gwdata, gdata, fd, x, &sum)) goto readerr;
	if (y != NULL && ! read_gwnum (gwdata, gdata, fd, y, &sum)) goto readerr; 


/* Read the five timers and their status */

	for (i=0; i<5; i++) {
		if (! read_double (fd, &timers[i], &sum)) goto readerr;
		if (! read_double (fd, &timers[i+5], &sum)) goto readerr;
		start_timer (i);	// and restart them! (J.P. 10/04/20)
	}

/* Read and compare the checksum */

	if (_read (fd, &i, sizeof (long)) != sizeof (long)) goto readerr;
	if (i != sum) goto readerr;

	_close (fd);
	return (TRUE);

/* An error occured.  Delete the current intermediate file. */
/* Set stage to -1 to indicate an error. */

readerr:
	sprintf (errmsg,"Error reading %s intermediate file.\n", filename);
	OutputStr (errmsg);
	_close (fd);
error:
	_unlink (filename);
	return (FALSE);
}

int writeToFileB (
	gwhandle *gwdata,
	ghandle *gdata,
	char	*filename,
	unsigned long j,
	unsigned long B,
	unsigned long nr,
	unsigned long *bpf,
	gwnum	x,
	gwnum	y)
{
	char	newfilename[16];
	int	fd;
	unsigned long magicnum, version;
	long	sum = 0, i;

	if (nosaving)				// Requested new option... 11/02/11
		return (TRUE);

/* If we are allowed to create multiple intermediate files, then */
/* write to a file called yNNNNNNN. */

	strcpy (newfilename, filename);
	if (TWO_BACKUP_FILES) newfilename[0] = 'y';

/* Create the intermediate file */

	fd = _open (newfilename, _O_BINARY|_O_WRONLY|_O_TRUNC|_O_CREAT, 0666);
	if (fd < 0) return (FALSE);

/* Write the file header. */

	magicnum = 0x9f2b3cd4;
	if (_write (fd, &magicnum, sizeof (long)) != sizeof (long))
		goto writeerr;
	version = 1;
	if (_write (fd, &version, sizeof (long)) != sizeof (long))
		goto writeerr;

/* Write the file data */

	if (! write_long (fd, j, &sum)) goto writeerr;
	if (! write_long (fd, B, &sum)) goto writeerr;
	if (! write_long (fd, nr, &sum)) goto writeerr;
	for (i=0; i<30; i++) {
		if (! write_long (fd, bpf[i], &sum)) goto writeerr;
	}

/* Write the data values */

	if (! write_gwnum (gwdata, gdata, fd, x, &sum)) goto writeerr;
	if (y != NULL && ! write_gwnum (gwdata, gdata, fd, y, &sum)) goto writeerr; 

/* Save the five timers */

	for (i=0; i<5; i++) {
		if (timers[i+5] != 0.0) {// if the timer was running
			end_timer (i);			// update and save it
			if (! write_double (fd, timers[i], &sum)) {
				start_timer (i);	// and then, restart it, even if write is in error!
				goto writeerr;
			}
			if (! write_double (fd, timers[i+5], &sum)) { // save the timer status
				start_timer (i);	// and then, restart it, even if write is in error!
				goto writeerr;
			}
			start_timer (i);	// and then, restart it!
		}
		else {
			if (! write_double (fd, timers[i], &sum)) goto writeerr;	// save the timer
			if (! write_double (fd, timers[i+5], &sum)) goto writeerr;	// save its status
		}
	}

/* Write the checksum */

	if (_write (fd, &sum, sizeof (long)) != sizeof (long)) goto writeerr;

	_commit (fd);
	_close (fd);

/* Now rename the intermediate files */

	if (TWO_BACKUP_FILES) {
		_unlink (filename);
		rename (newfilename, filename);
	}
	return (TRUE);

/* An error occured.  Close and delete the current file. */

writeerr:
	_close (fd);
	_unlink (newfilename);
	return (FALSE);
}

int readFromFileB (
	gwhandle *gwdata,
	ghandle *gdata,
	char	*filename,
	unsigned long *j,
	unsigned long *B,
	unsigned long *nr,
	unsigned long *bpf,
	gwnum	x,
	gwnum	y)
{
	int	fd;
	unsigned long magicnum, version;
	long	sum = 0, i;
	char	errmsg[100];

/* Open the intermediate file */

	fd = _open (filename, _O_BINARY | _O_RDONLY);
	if (fd < 0) goto error;

/* Read the file header */

	if (_read (fd, &magicnum, sizeof (long)) != sizeof (long))
		goto readerr;
	if (magicnum != 0x9f2b3cd4) goto readerr;

	if (_read (fd, &version, sizeof (long)) != sizeof (long)) goto readerr;
	if (version != 1 && version != 2) goto readerr;

/* Read the file data */

	if (! read_long (fd, j, &sum)) goto readerr;
	if (! read_long (fd, B, &sum)) goto readerr;
	if (! read_long (fd, nr, &sum)) goto readerr;
	for (i=0; i<30; i++) {
		if (! read_long (fd, &bpf[i], &sum)) goto readerr;
	}

/* Read the values */

	if (! read_gwnum (gwdata, gdata, fd, x, &sum)) goto readerr;
	if (y != NULL && ! read_gwnum (gwdata, gdata, fd, y, &sum)) goto readerr; 

/* Read the five timers and their status */

	for (i=0; i<5; i++) {
		if (! read_double (fd, &timers[i], &sum)) goto readerr;
		if (! read_double (fd, &timers[i+5], &sum)) goto readerr;
		start_timer (i);	// and restart them! (J.P. 10/04/20)
	}

/* Read and compare the checksum */

	if (_read (fd, &i, sizeof (long)) != sizeof (long)) goto readerr;
	if (i != sum) goto readerr;

	_close (fd);
	return (TRUE);

/* An error occured.  Delete the current intermediate file. */
/* Set stage to -1 to indicate an error. */

readerr:
	sprintf (errmsg,"Error reading %s intermediate file.\n", filename);
	OutputStr (errmsg);
	_close (fd);
error:
	_unlink (filename);
	return (FALSE);
}

int gmwriteToFile (
	gwhandle *gwdata,
	ghandle *gdata,
	char	*filename,
	unsigned long j,
	unsigned long ubx,
	unsigned long uby,
	gwnum	x,
	gwnum	y)
{
	char	newfilename[16];
	int	fd;
	unsigned long magicnum, version;
	long	sum = 0, i;

	if (nosaving)				// Requested new option... 11/02/11
		return (TRUE);

/* If we are allowed to create multiple intermediate files, then */
/* write to a file called yNNNNNNN. */

	strcpy (newfilename, filename);
	if (TWO_BACKUP_FILES) newfilename[0] = 'y';

/* Create the intermediate file */

	fd = _open (newfilename, _O_BINARY|_O_WRONLY|_O_TRUNC|_O_CREAT, 0666);
	if (fd < 0) return (FALSE);

/* Write the file header. */

	magicnum = 0x9f2b3cd4;
	if (_write (fd, &magicnum, sizeof (long)) != sizeof (long))
		goto writeerr;
	version = 1;
	if (_write (fd, &version, sizeof (long)) != sizeof (long))
		goto writeerr;

/* Write the file data */

	if (! write_long (fd, j, &sum)) goto writeerr;
	if (! write_long (fd, ubx, &sum)) goto writeerr;
	if (! write_long (fd, uby, &sum)) goto writeerr;

/* Write the data values */

	if (! write_gwnum (gwdata, gdata, fd, x, &sum)) goto writeerr;
	if (y != NULL && ! write_gwnum (gwdata, gdata, fd, y, &sum)) goto writeerr; 

/* Save the five timers */

	for (i=0; i<5; i++) {
		if (timers[i+5] != 0.0) {// if the timer was running
			end_timer (i);			// update and save it
			if (! write_double (fd, timers[i], &sum)) {
				start_timer (i);	// and then, restart it, even if write is in error!
				goto writeerr;
			}
			if (! write_double (fd, timers[i+5], &sum)) { // save the timer status
				start_timer (i);	// and then, restart it, even if write is in error!
				goto writeerr;
			}
			start_timer (i);	// and then, restart it!
		}
		else {
			if (! write_double (fd, timers[i], &sum)) goto writeerr;	// save the timer
			if (! write_double (fd, timers[i+5], &sum)) goto writeerr;	// save its status
		}
	}

/* Write the checksum */

	if (_write (fd, &sum, sizeof (long)) != sizeof (long)) goto writeerr;

	_commit (fd);
	_close (fd);

/* Now rename the intermediate files */

	if (TWO_BACKUP_FILES) {
		_unlink (filename);
		rename (newfilename, filename);
	}
	return (TRUE);

/* An error occured.  Close and delete the current file. */

writeerr:
	_close (fd);
	_unlink (newfilename);
	return (FALSE);
}

int gmreadFromFile (
	gwhandle *gwdata,
	ghandle *gdata,
	char	*filename,
	unsigned long *j,
	unsigned long *ubx,
	unsigned long *uby,
	gwnum	x,
	gwnum	y)
{
	int	fd;
	unsigned long magicnum, version;
	long	sum = 0, i;
	char	errmsg[100];

/* Open the intermediate file */

	fd = _open (filename, _O_BINARY | _O_RDONLY);
	if (fd < 0) goto error;

/* Read the file header */

	if (_read (fd, &magicnum, sizeof (long)) != sizeof (long))
		goto readerr;
	if (magicnum != 0x9f2b3cd4) goto readerr;

	if (_read (fd, &version, sizeof (long)) != sizeof (long)) goto readerr;
	if (version != 1 && version != 2) goto readerr;

/* Read the file data */

	if (! read_long (fd, j, &sum)) goto readerr;
	if (! read_long (fd, ubx, &sum)) goto readerr;
	if (! read_long (fd, uby, &sum)) goto readerr;

/* Read the values */

	if (! read_gwnum (gwdata, gdata, fd, x, &sum)) goto readerr;
	if (y != NULL && ! read_gwnum (gwdata, gdata, fd, y, &sum)) goto readerr; 

/* Read the five timers and their status */

	for (i=0; i<5; i++) {
		if (! read_double (fd, &timers[i], &sum)) goto readerr;
		if (! read_double (fd, &timers[i+5], &sum)) goto readerr;
		start_timer (i);	// and restart them! (J.P. 10/04/20)
	}

/* Read and compare the checksum */

	if (_read (fd, &i, sizeof (long)) != sizeof (long)) goto readerr;
	if (i != sum) goto readerr;

	_close (fd);
	return (TRUE);

/* An error occured.  Delete the current intermediate file. */
/* Set stage to -1 to indicate an error. */

readerr:
	sprintf (errmsg,"Error reading %s intermediate file.\n", filename);
	OutputStr (errmsg);
	_close (fd);
error:
	_unlink (filename);
	return (FALSE);
}

int LwriteToFile (					// To save a Lucas sequence matrix and its Discriminant
	gwhandle *gwdata,
	ghandle *gdata,
	char	*filename,
	unsigned long j,
	unsigned long D,
	unsigned long nr,
	unsigned long *bpf,
	gwnum	x,
	gwnum	y,
	gwnum	z,
	gwnum	t)
{
	char	newfilename[16];
	int	fd;
	unsigned long magicnum, version;
	long	sum = 0, i;

	if (nosaving)				// Requested new option... 11/02/11
		return (TRUE);

/* If we are allowed to create multiple intermediate files, then */
/* write to a file called yNNNNNNN. */

	strcpy (newfilename, filename);
	if (TWO_BACKUP_FILES) newfilename[0] = 'y';

/* Create the intermediate file */

	fd = _open (newfilename, _O_BINARY|_O_WRONLY|_O_TRUNC|_O_CREAT, 0666);
	if (fd < 0) return (FALSE);

/* Write the file header. */

	magicnum = 0x9f2b3cd4;
	if (_write (fd, &magicnum, sizeof (long)) != sizeof (long))
		goto writeerr;
	version = 1;
	if (_write (fd, &version, sizeof (long)) != sizeof (long))
		goto writeerr;

/* Write the file data */

	if (! write_long (fd, j, &sum)) goto writeerr;
	if (! write_long (fd, D, &sum)) goto writeerr;
	if (! write_long (fd, nr, &sum)) goto writeerr;
	for (i=0; i<30; i++) {
		if (! write_long (fd, bpf[i], &sum)) goto writeerr;
	}

/* Write the data values */

	if (! write_gwnum (gwdata, gdata, fd, x, &sum)) goto writeerr;
	if (y != NULL && ! write_gwnum (gwdata, gdata, fd, y, &sum)) goto writeerr; 
	if (z != NULL && ! write_gwnum (gwdata, gdata, fd, z, &sum)) goto writeerr; 
	if (t != NULL && ! write_gwnum (gwdata, gdata, fd, t, &sum)) goto writeerr; 

/* Save the five timers */

	for (i=0; i<5; i++) {
		if (timers[i+5] != 0.0) {// if the timer was running
			end_timer (i);			// update and save it
			if (! write_double (fd, timers[i], &sum)) {
				start_timer (i);	// and then, restart it, even if write is in error!
				goto writeerr;
			}
			if (! write_double (fd, timers[i+5], &sum)) { // save the timer status
				start_timer (i);	// and then, restart it, even if write is in error!
				goto writeerr;
			}
			start_timer (i);	// and then, restart it!
		}
		else {
			if (! write_double (fd, timers[i], &sum)) goto writeerr;	// save the timer
			if (! write_double (fd, timers[i+5], &sum)) goto writeerr;	// save its status
		}
	}

/* Write the checksum */

	if (_write (fd, &sum, sizeof (long)) != sizeof (long)) goto writeerr;

	_commit (fd);
	_close (fd);

/* Now rename the intermediate files */

	if (TWO_BACKUP_FILES) {
		_unlink (filename);
		rename (newfilename, filename);
	}
	return (TRUE);

/* An error occured.  Close and delete the current file. */

writeerr:
	_close (fd);
	_unlink (newfilename);
	return (FALSE);
}

int LreadFromFile (						// To restore a Lucas sequence matrix
	gwhandle *gwdata,
	ghandle *gdata,
	char	*filename,
	unsigned long *j,
	unsigned long *D,
	unsigned long *nr,
	unsigned long *bpf,
	gwnum	x,
	gwnum	y,
	gwnum	z,
	gwnum	t)
{
	int	fd;
	unsigned long magicnum, version;
	long	sum = 0, i;
	char	errmsg[100];

/* Open the intermediate file */

	fd = _open (filename, _O_BINARY | _O_RDONLY);
	if (fd < 0) goto error;

/* Read the file header */

	if (_read (fd, &magicnum, sizeof (long)) != sizeof (long))
		goto readerr;
	if (magicnum != 0x9f2b3cd4) goto readerr;

	if (_read (fd, &version, sizeof (long)) != sizeof (long)) goto readerr;
	if (version != 1 && version != 2) goto readerr;

/* Read the file data */

	if (! read_long (fd, j, &sum)) goto readerr;
	if (! read_long (fd, D, &sum)) goto readerr;
	if (! read_long (fd, nr, &sum)) goto readerr;
	for (i=0; i<30; i++) {
		if (! read_long (fd, &bpf[i], &sum)) goto readerr;
	}

/* Read the values */

	if (! read_gwnum (gwdata, gdata, fd, x, &sum)) goto readerr;
	if (y != NULL && ! read_gwnum (gwdata, gdata, fd, y, &sum)) goto readerr; 
	if (z != NULL && ! read_gwnum (gwdata, gdata, fd, z, &sum)) goto readerr; 
	if (t != NULL && ! read_gwnum (gwdata, gdata, fd, t, &sum)) goto readerr; 

/* Read the five timers and their status */

	for (i=0; i<5; i++) {
		if (! read_double (fd, &timers[i], &sum)) goto readerr;
		if (! read_double (fd, &timers[i+5], &sum)) goto readerr;
		start_timer (i);	// and restart them! (J.P. 10/04/20)
	}

/* Read and compare the checksum */

	if (_read (fd, &i, sizeof (long)) != sizeof (long)) goto readerr;
	if (i != sum) goto readerr;

	_close (fd);
	return (TRUE);

/* An error occured.  Delete the current intermediate file. */
/* Set stage to -1 to indicate an error. */

readerr:
	sprintf (errmsg,"Error reading %s intermediate file.\n", filename);
	OutputStr (errmsg);
	_close (fd);
error:
	_unlink (filename);
	return (FALSE);
}

/* Increment the error counter.  The error counter is one 32-bit field containing 5 values.  Prior to version 29.3, this was */
/* a one-bit flag if this is a continuation from a save file that did not track error counts, a 7-bit count of errors that were */
/* reproducible, a 8-bit count of ILLEGAL SUMOUTs or zeroed FFT data or corrupt units_bit, a 8-bit count of convolution errors */
/* above 0.4, and a 8-bit count of SUMOUTs not close enough to SUMINPs. */
/* NOTE:  The server considers an LL run clean if the error code is XXaaYY00 and XX = YY and aa is ignored.  That is, repeatable */
/* round off errors and all ILLEGAL SUMOUTS are ignored. */
/* In version 29.3, a.k.a. Wf in result lines, the 32-bit field changed.  See comments in the code below. */

void inc_error_count (
	int	type,
	unsigned long *error_count)
{
	unsigned long addin, orin, maxval;

	addin = orin = 0;
	if (type == 0) addin = 1, maxval = 0xF;				// SUMINP != SUMOUT
	else if (type == 4) addin = 1 << 4, maxval = 0x0F << 4;		// Jacobi error check
	else if (type == 1) addin = 1 << 8, maxval = 0x3F << 8;		// Roundoff > 0.4
	else if (type == 5) orin = 1 << 14;				// Zeroed FFT data
	else if (type == 6) orin = 1 << 15;				// Units bit, counter, or other value corrupted
	else if (type == 2) addin = 1 << 16, maxval = 0xF << 16;	// ILLEGAL SUMOUT
	else if (type == 7) addin = 1 << 20, maxval = 0xF << 20;	// High reliability (Gerbicz or dblchk) PRP error
	else if (type == 3) addin = 1 << 24, maxval = 0x3F << 24;	// Repeatable error

	if (addin && (*error_count & maxval) != maxval) *error_count += addin;
	*error_count |= orin;
}

/* Create a message if the non-repeatable error count is more than zero */
/* Returns TRUE if the non-repeatable error count is more than zero. */

int make_error_count_message (
	unsigned long error_count,
	int	message_type,		/* 1 = very small, 2 = one line, 3 = multi-line, 0x8000 = confidence always excellent */
	char	*buf,
	int	buflen)
{
	int	count_repeatable, count_suminp, count_roundoff, count_illegal_sumout, count_total;
	int	count_jacobi, count_gerbicz, count_bad_errors;
	int	force_high_confidence;
	char	local_buf[400], counts_buf[200], confidence[25];

/* Massage input argument */

	force_high_confidence = message_type & 0x8000;
	message_type = message_type & 0x7FFF;

/* Parse the error counts variable */

	count_repeatable = (error_count >> 24) & 0x3F;
	count_illegal_sumout = (error_count >> 16) & 0xF;
	count_roundoff = (error_count >> 8) & 0x3F;
	count_jacobi = (error_count >> 4) & 0xF;
	count_gerbicz = (error_count >> 20) & 0xF;
	count_suminp = error_count & 0xF;

/* Return if no hardware errors have occurred */

	count_total = count_illegal_sumout + count_suminp + count_roundoff + count_jacobi + count_gerbicz;
	if (count_total - count_repeatable == 0) return (FALSE);

/* Format the error counts */

	counts_buf[0] = 0;

	if (message_type == 1) {
		sprintf (counts_buf, ", errors: %d", count_total);
	}

	if (message_type == 2) {
		sprintf (counts_buf, "%d error%s", count_total, count_total > 1 ? "s": "");
		if (count_repeatable == 1) {
			strcpy (local_buf, ", 1 was repeatable (not an error)");
			strcat (counts_buf, local_buf);
		} else if (count_repeatable > 1) {
			sprintf (local_buf, ", %d%s were repeatable (not errors)",
				 count_repeatable, count_repeatable == 0x3F ? " or more" : "");
			strcat (counts_buf, local_buf);
		}
	}

	if (message_type == 3) {
		if (count_jacobi >= 1) {
			sprintf (local_buf, "%d%s Jacobi error%s, ",
				 count_jacobi, count_jacobi == 0xF ? " or more" : "", count_jacobi > 1 ? "s" : "");
			strcat (counts_buf, local_buf);
		}
		if (count_gerbicz >= 1) {
			sprintf (local_buf, "%d%s Gerbicz/double-check error%s, ",
				 count_gerbicz, count_gerbicz == 0xF ? " or more" : "", count_gerbicz > 1 ? "s" : "");
			strcat (counts_buf, local_buf);
		}
		if (count_roundoff >= 1) {
			sprintf (local_buf, "%d%s ROUNDOFF > 0.4, ",
				 count_roundoff, count_roundoff == 0x3F ? " or more" : "");
			strcat (counts_buf, local_buf);
		}
		if (count_suminp >= 1) {
			sprintf (local_buf, "%d%s SUM(INPUTS) != SUM(OUTPUTS), ",
				 count_suminp, count_suminp == 0xF ? " or more" : "");
			strcat (counts_buf, local_buf);
		}
		if (count_illegal_sumout >= 1) {
			sprintf (local_buf, "%d%s ILLEGAL SUMOUT/bad FFT data, ",
				 count_illegal_sumout, count_illegal_sumout == 0xF ? " or more" : "");
			strcat (counts_buf, local_buf);
		}
		counts_buf[strlen(counts_buf)-2] = 0;
		if (count_repeatable >= 1) {
			if (count_repeatable == 1)
				strcpy (local_buf, "of which 1 was repeatable (not a hardware error)");
			else
				sprintf (local_buf, "of which %d were repeatable (not hardware errors)", count_repeatable);
			if (strlen (counts_buf) <= 40) strcat (counts_buf, " ");
			else strcat (counts_buf, "\n");
			strcat (counts_buf, local_buf);
		}
		strcat (counts_buf, ".\n");
	}

/* Guess our confidence in the end result */

	count_bad_errors = count_jacobi + count_suminp + count_roundoff - count_repeatable;
	if (force_high_confidence) count_bad_errors = 0;
	strcpy (confidence, count_bad_errors == 0 ? "excellent" :
			    count_bad_errors <= 3 ? "fair" :
			    count_bad_errors <= 6 ? "poor" : "very poor");

/* Put it all together to form our full message */

	if (message_type == 1) {
		sprintf (local_buf, ", %s, confidence: %s", counts_buf, confidence);
	}
	if (message_type == 2) {
		if (count_jacobi || count_gerbicz) sprintf (local_buf, "Hardware errors!  %s.  Confidence in end result is %s.\n", counts_buf, confidence);
		else sprintf (local_buf, "Possible hardware errors!  %s.  Confidence in end result is %s.\n", counts_buf, confidence);
	}
	if (message_type == 3) {
		if (count_jacobi || count_gerbicz) strcpy (local_buf, "Hardware errors have occurred during the test!");
		else strcpy (local_buf, "Possible hardware errors have occurred during the test!");
		if (strlen (counts_buf) <= 25) strcat (local_buf, " ");
		else strcat (local_buf, "\n");
		strcat (local_buf, counts_buf);
		sprintf (local_buf+strlen(local_buf), "Confidence in final result is %s.\n", confidence);
	}

/* Copy as much of our result as possible to the caller's buffer */

	if ((int) strlen (local_buf) >= buflen) local_buf[buflen-1] = 0;
	strcpy (buf, local_buf);
	return (TRUE);
}


/* Do a squaring very carefully.  This is done after a normal */
/* iteration gets a roundoff error above 0.40.  This careful iteration */
/* will not generate a roundoff error. */

void careful_squaring (
	gwhandle *gwdata,
	gwnum s,
	long addin)		
{
	gwnum	hi, lo;
	unsigned long i;

/* Copy the data to hi and lo.  Zero out half the FFT data in each. */

	hi = gwalloc (gwdata);
	lo = gwalloc (gwdata);
	gwcopy (gwdata, s, hi);
	gwcopy (gwdata, s, lo);
	for (i = 0; i < gwdata->FFTLEN/2; i++)
		set_fft_value (gwdata, hi, i, 0);
	for ( ; i < gwdata->FFTLEN; i++)
		set_fft_value (gwdata, lo, i, 0);

/* Clear the addin value */

	if (abs(gwdata->c) == 1)
		gwsetaddin (gwdata, 0);

/* Now do the squaring using three multiplies and adds */

	gwstartnextfft (gwdata, FALSE);
	gwfft (gwdata, hi, hi);
	gwfft (gwdata, lo, lo);
	gwfftfftmul (gwdata, lo, hi, s);
	gwfftfftmul (gwdata, hi, hi, hi);
	if (abs(gwdata->c) == 1)
		gwsetaddin (gwdata, addin);	/* Set the addin value */
	gwfftfftmul (gwdata, lo, lo, lo);
	gwadd (gwdata, s, s);
	gwadd (gwdata, hi, s);
	gwadd (gwdata, lo, s);

/* Since our error recovery code cannot cope with an error during a careful */
/* iteration, make sure the error variable is cleared.  This shouldn't */
/* ever happen, but two users inexplicably ran into this problem. */

	gw_clear_error (gwdata);

/* Free memory and return */

	gwfree (gwdata, hi);
	gwfree (gwdata, lo);
}


void careful_multiply (
	gwhandle *gwdata,
	gwnum s,
	gwnum t,
	long addin)		
{
	gwnum	hi, lo, hi2, lo2, u;
	unsigned long i;

/* Copy the data to hi, lo, hi2 and lo2.  Zero out half the FFT data in each. */

	hi = gwalloc (gwdata);
	lo = gwalloc (gwdata);
	hi2 = gwalloc (gwdata);
	lo2 = gwalloc (gwdata);
	u = gwalloc (gwdata);

	gwcopy (gwdata, s, hi);
	gwcopy (gwdata, s, lo);
	gwcopy (gwdata, t, hi2);
	gwcopy (gwdata, t, lo2);

	for (i = 0; i < gwdata->FFTLEN/2; i++) {
		set_fft_value (gwdata, hi, i, 0);
		set_fft_value (gwdata, hi2, i, 0);
	}

	for ( ; i < gwdata->FFTLEN; i++) {
		set_fft_value (gwdata, lo, i, 0);
		set_fft_value (gwdata, lo2, i, 0);
	}

/* Clear the addin value */

	if (abs(gwdata->c) == 1)
		gwsetaddin (gwdata, 0);

/* Now do the multiply using four multiplies and adds */

	gwstartnextfft (gwdata, FALSE);
	gwfft (gwdata, hi, hi);
	gwfft (gwdata, lo, lo);
	gwfft (gwdata, hi2, hi2);
	gwfft (gwdata, lo2, lo2);
	gwfftfftmul (gwdata, hi, hi2, t);
	gwfftfftmul (gwdata, lo, lo2, u);
	gwfftfftmul (gwdata, hi, lo2, hi);
	if (abs(gwdata->c) == 1)
		gwsetaddin (gwdata, addin);	/* Set the addin value */
	gwfftfftmul (gwdata, lo, hi2, lo);
	gwadd (gwdata, u, t);
	gwadd (gwdata, hi, t);
	gwadd (gwdata, lo, t);

/* Since our error recovery code cannot cope with an error during a careful */
/* iteration, make sure the error variable is cleared.  This shouldn't */
/* ever happen, but two users inexplicably ran into this problem. */

	gw_clear_error (gwdata);

/* Free memory and return */

	gwfree (gwdata, hi);
	gwfree (gwdata, lo);
	gwfree (gwdata, hi2);
	gwfree (gwdata, lo2);
	gwfree (gwdata, u);
}

/* Set a gwnum to zero */

void gwzero (gwhandle *gwdata, gwnum gg) {
	unsigned long j;

	for(j=0; j<gwdata->FFTLEN; ++j)
		set_fft_value (gwdata, gg, j, 0);
	return;
}


/* Test if a gwnum is zero */

#define	MAX_NZ_COUNT 16

int
gwpiszero(
	gwhandle *gwdata,
	gwnum 	gg,
	giant	extmod
)
{
	unsigned long j;
	int 	err_code;
	long	val, result, count = 0;
	giant	gtest;
	ghandle *gdata;

	if (!gwdata->GENERAL_MOD && !quotient)
		for(j=0; j<gwdata->FFTLEN; ++j) {
			err_code = get_fft_value (gwdata, gg, j, &val);
			if (err_code) return (-err_code);
			if (val && count > MAX_NZ_COUNT)
				return 0;		// Too much non zero words, the gwnum is not zero.
			else if (val)
				count++;		// Update the count of non-zero words.
		}
	if (count || gwdata->GENERAL_MOD || quotient) {	// The gwnum may be zero but needs a more accurate test...
		gdata = &gwdata->gdata;
		gtest = popg (gdata, gwdata->FFTLEN);	// Allocate memory for the giant
		gwtogiant (gwdata, gg, gtest);
		modg (extmod, gtest);
		result = isZero (gtest);
		pushg (gdata, 1);						// Free memory
		return (result);
	}
	else
		return 1;			// The gwnum is zero
}

/* Test two gwnums for equality *

int	gwpequal (gwhandle *gwdata, gwnum gw1, gwnum gw2) {
	gwnum gwdiff;
	unsigned long savenormcount1, savenormcount2;
	int result;

	gwdiff = gwalloc (gwdata);						// Reserve memory for the difference
	savenormcount1 = ((unsigned long *) gw1) [-1];	// Save the norm. counts
	savenormcount2 = ((unsigned long *) gw2) [-1];
	((unsigned long *) gw1) [-1] = ((unsigned long *) gw2) [-1] = 100; // Set high counts
	gwsub3 (gwdata, gw1, gw2, gwdiff);				// to force a normalization...
	result = gwpiszero (gwdata, gwdiff);				// Test for zero difference
	((unsigned long *) gw1) [-1] = savenormcount1;	// Restore the norm. counts
	((unsigned long *) gw2) [-1] = savenormcount2;
	gwfree (gwdata, gwdiff);						// Free memory
	return (result);
}


*/

// Print some words of a gwnum.

int
gwprint(
	gwhandle *gwdata,
	gwnum 	gg,
	int 	N
)
{
	int 	j, err_code;
	long	val;
	char buf[20];

	OutputStr ("\n");
	for(j=0; j<N; ++j)
	{
		err_code = get_fft_value (gwdata, gg, j, &val);
		if (err_code) return (err_code);
		if (val) {
			sprintf (buf, "%ld ", val);
			OutputBoth (buf);
		}
	}
	OutputBoth ("\n");
	return 0;
}

int setupok (gwhandle *gwdata, int errcode)		// Test if the call to gwsetup is successful
{
	char buff[256];
	if (!errcode)
		return TRUE;
	else {
		sprintf (buff, "Fatal error at setup : ");
		gwerror_text (gwdata, errcode, buff+strlen(buff), 255-strlen(buff));
		strcpy(buff+strlen(buff), "\n");
		OutputBoth (buff);
		return FALSE;
	}
}


char res64[40]; /* VP : This variable has been made global */
char alt_res64[40]; /* JP 01/07/20 : This variable has been made global */

#ifndef X86_64

/* Make a string out of a 96-bit value (a found factor) */

void makestr (
	unsigned long hsw,
	unsigned long msw,
	unsigned long lsw,
	char	*buf)			/* An 80 character output buffer */
{
	int	i, j, k, carry;
	unsigned long x[3];
	char	pow[80];

	x[0] = hsw; x[1] = msw; x[2] = lsw;

	if (!x[0] && !x[1] && !x[2]) {
		buf[0] = '0';
		buf[1] = 0;
		return;
	}

	for (i = 0; i < 79; i++) pow[i] = '0', buf[i] = '0';
	pow[78] = '1';
	pow[79] = buf[79] = 0;

	for (i = 3; i--; ) {
		for (j = 0; j < 32; j++) {
			if (x[i] & 1) {
				carry = 0;
				for (k = 79; k--; ) {
					buf[k] = buf[k] - '0' +
						pow[k] - '0' + carry;
					carry = buf[k] / 10;
					buf[k] %= 10;
					buf[k] += '0';
				}
			}
			carry = 0;
			for (k = 79; k--; ) {
				pow[k] = (pow[k] - '0') * 2 + carry;
				carry = pow[k] / 10;
				pow[k] %= 10;
				pow[k] += '0';
			}
			x[i] >>= 1;
		}
	}
	while (buf[0] == '0') strcpy (buf, buf+1);
}



/* Determine how much we should factor (in bits) */
/* Don't let feeble 486 any Cyrix machines factor higher than 2^62 */
/* as this would execute some really slow floating point code */

unsigned int factorLimit (
	unsigned long p,
	int	work_type)
{
	unsigned int test, override, twop = p + (p>>1);	// ~1.5*p

	// I am assuming that the time elapsed for a Proth test on the Gaussian-Mersenne Norm
	// which exponent is p is approximatively the time elapsed by a LL test which exponent is 2*p. -JP-

	if (twop > FAC80) test = 80;	/* Test all 80 bit factors */
	else if (twop > FAC79) test = 79;	/* Test all 79 bit factors */
	else if (twop > FAC78) test = 78;	/* Test all 78 bit factors */
	else if (twop > FAC77) test = 77;	/* Test all 77 bit factors */
	else if (twop > FAC76) test = 76;	/* Test all 76 bit factors */
	else if (twop > FAC75) test = 75;	/* Test all 75 bit factors */
	else if (twop > FAC74) test = 74;	/* Test all 74 bit factors */
	else if (twop > FAC73) test = 73;	/* Test all 73 bit factors */
	else if (twop > FAC72) test = 72;	/* Test all 72 bit factors */
	else if (twop > FAC71) test = 71;	/* Test all 71 bit factors */
	else if (twop > FAC70) test = 70;	/* Test all 70 bit factors */
	else if (twop > FAC69) test = 69;	/* Test all 69 bit factors */
	else if (twop > FAC68) test = 68;	/* Test all 68 bit factors */
	else if (twop > FAC67) test = 67;	/* Test all 67 bit factors */
	else if (twop > FAC66) test = 66;	/* Test all 66 bit factors */
	else if (twop > FAC65) test = 65;	/* Test all 65 bit factors */
	else if (twop > FAC64) test = 64;	/* Test all 64 bit factors */
	else if (twop > FAC63) test = 63;	/* Test all 63 bit factors */
	else if (twop > FAC62) test = 62;	/* Test all 62 bit factors */
	else if (twop > FAC61) test = 61;	/* Test all 61 bit factors */
	else if (twop > FAC60) test = 60;	/* Test all 60 bit factors */
	else if (twop > FAC59) test = 59;	/* Test all 59 bit factors */
	else if (twop > FAC58) test = 58;	/* Test all 58 bit factors */
	else if (twop > FAC57) test = 57;	/* Test all 57 bit factors */
	else if (twop > FAC56) test = 56;	/* Test all 56 bit factors */
	else test = 40;			/* Test all 40 bit factors */
	if (work_type == WORK_FACTOR) {
		override = (unsigned int)
			IniGetInt (INI_FILE, "FactorOverride", 99);
		if (override != 99) test = override;
	}
	if (work_type == WORK_DBLCHK) test--;
	return (test);
}

/* Prepare for making a factoring run */

void factorSetup (unsigned long p)
{

/* Allocate 1MB memory for factoring */

	if (FACDATA == NULL) FACDATA = aligned_malloc (1000000, 65536);

/* Call the factoring setup assembly code */

	
	FACLSW = p;
	SRCARG = FACDATA;
	setupf ();

/* If using the SSE2 factoring code, do more initialization */
/* We need to initialize much of the following data: */
/*	XMM_BITS30		DD	0,3FFFFFFFh,0,3FFFFFFFh
	XMM_INITVAL		DD	0,0,0,0
	XMM_INVFAC		DD	0,0,0,0
	XMM_I1			DD	0,0,0,0
	XMM_I2			DD	0,0,0,0
	XMM_F1			DD	0,0,0,0
	XMM_F2			DD	0,0,0,0
	XMM_F3			DD	0,0,0,0
	XMM_TWO_120_MODF1	DD	0,0,0,0
	XMM_TWO_120_MODF2	DD	0,0,0,0
	XMM_TWO_120_MODF3	DD	0,0,0,0
	XMM_INIT120BS		DD	0,0
	XMM_INITBS		DD	0,0
	XMM_BS			DD	0,0
	XMM_SHIFTER		DD	64 DUP (0)
	TWO_TO_FACSIZE_PLUS_62	DQ	0.0
	SSE2_LOOP_COUNTER	DD	0 */
/* The address to XMM_BITS30 was returned in SRCARG. */


// #ifndef X86_64 // useless
	if (CPU_FLAGS & CPU_SSE2) {
		unsigned long i, bits_in_factor;
		unsigned long *xmm_data;

/* Compute the number of bits in the factors we will be testing */

		if (FACHSW) bits_in_factor = 64, i = FACHSW;
		else if (FACMSW) bits_in_factor = 32, i = FACMSW;
		else return;
		while (i) bits_in_factor++, i >>= 1;

/* Factors 63 bits and below use the non-SSE2 code */

//		if (bits_in_factor <= 63) return; -JP-

/* Set XMM_SHIFTER values (the first shifter value is not used). */
/* Also compute the initial value. */

		p = 2*p;			// This code accepts an even exponent! -JP-

		xmm_data = (unsigned long *) SRCARG;
		for (i = 0; p > bits_in_factor + 59; i++) {
			xmm_data[52+i*2] = (p & 1) ? 1 : 0;
			p >>= 1;
		}
		xmm_data[4] =			/* XMM_INITVAL */
		xmm_data[6] = p >= 90 ? 0 : (1 << (p - 60));
		xmm_data[44] = 62 - (120 - bits_in_factor);/* XMM_INIT120BS */
		xmm_data[46] = 62 - (p - bits_in_factor);/* XMM_INITBS */
		xmm_data[116] = i;		/* SSE2_LOOP_COUNTER */
		*(double *)(&xmm_data[114]) = pow ((double) 2.0, (int) (bits_in_factor + 62));
						/* TWO_TO_FACSIZE_PLUS_62 */

/* Set XMM_BS to 60 - (120 - fac_size + 1) as defined in factor64.mac */

		xmm_data[48] = bits_in_factor - 61;
	}
// #endif // useless
}


/* Cleanup after making a factoring run */

void factorDone (void)
{

/* Free factoring data */

	aligned_free (FACDATA);
	FACDATA = NULL;
}

/* Wrapper code that verifies any factors found by the assembly code */

int factorAndVerify (
	unsigned long p)
{
	unsigned long hsw, msw;
	int	res;
	char testbuf[256], facstr[256];

/* Remember starting point in case of an error */

	hsw = FACHSW;
	msw = FACMSW;

/* Call assembly code */

#ifndef	WIN32
	_CPU_FLAGS = CPU_FLAGS;	// needed by assembler code...
#endif

loop:
	res = factor64 ();


/* If a factor was not found, return. */

	if (res == 2) {
		return (2);
	}


/* Otherwise verify the factor. */

	if (FACTEST) {						// if we are in test mode, print the factor.
		makestr (FACHSW, FACMSW, FACLSW, facstr);
		if (FACTEST > 70)
			sprintf (testbuf, "%s, TEST = %lu\n", facstr, FACTEST);
		else
			sprintf (testbuf, "%s, TEST = %lu, PASS = %lu\n", facstr, FACTEST, FACPASS);
		OutputBoth(testbuf);
	}

	if (FACHSW || FACMSW || FACLSW > 1) {	// Verifying the factor...
		itog ((int) FACHSW, testf);
		gshiftleft (32, testf);
		uladdg (FACMSW, testf);
		gshiftleft (32, testf);
		uladdg (FACLSW, testf);
		itog (4, testx);
		powermod (testx, p, testf);
		uladdg (1, testx);
		if(gcompg (testx, testf) == 0) {
			if (!resn) {
				gtog (N, testn);
				modg (testf, testn);
				if ((resn = isZero (testn))) {
					makestr (FACHSW, FACMSW, FACLSW, facnstr);
				}
			}
			if (!resnp) {
				gtog (NP, testnp);
				modg (testf, testnp);
				if ((resnp = isZero (testnp))) {
					makestr (FACHSW, FACMSW, FACLSW, facnpstr);
				}
			}
			if ((!resn || !resnp) && FACTEST <= 70) {
				goto loop;
			}
			return (1);
		}
	}

/* If factor is no good, print an error message, sleep, and */
/* restart the factoring code. */

	OutputBoth ("ERROR: Incorrect factor found.\n");
	if (! SleepFive ()) return (FALSE);
	FACHSW = hsw;
	FACMSW = msw;
	factorSetup (p);
	goto loop;
//	return(1);
}

/* Compute percent completion of a factoring job */

double facpct (
	short	pass,
	unsigned int bits,
	unsigned long endpthi,
	unsigned long endptlo)
{
	double	startpt, endpt, current;

	current = FACHSW * 4294967296.0 + FACMSW;
	endpt = endpthi * 4294967296.0 + endptlo;
	if (current > endpt) current = endpt;
        if (bits < 32) bits = 32;
        startpt = pow ((double) 2.0, (int) (bits-32));
	return ((pass + (current - startpt) / (endpt - startpt)) / 16.0 * 100.0);
}


/* Trial factor a Gaussian-Mersenne candidate prior to running a Proth test */

char FACMSG[] = "Factoring GM%%ld to 2^%%d is %%.%df%%%% complete.";

int primeFactor (
	unsigned long p,		/* Exponent to factor */
	unsigned int bits,		/* How far already factored in bits */
	int	*result,		/* Returns true if factor found */
	int	work_type,		/* Work type from worktodo.ini file */
	int	fd)			/* Continuation file handle or zero */
{
	int	continuation, old_style;
	long	write_time = DISK_WRITE_TIME * 60;
	unsigned int test_bits;
	unsigned long endpthi, endptlo;
	short	pass;
	time_t	start_time, current_time;
	char	filename[20];
	char	buf[256];

/* Clear all timers */

	clear_timers ();

/* Get the current time */

	time (&start_time);

/* Init temporary file name */

	tempFileName (filename, 'f', N);

/* Determine how much we should factor (in bits) */

	test_bits = factorLimit (p, work_type);
	if (test_bits < 32) test_bits = 32;

/* By default we do not do the old style of factoring which was 16 passes */
/* each pass testing from bits to test_bits */

	old_style = FALSE;

/* Is this a continuation?  If so, read continuation file. */
/* There are two types of continuation files, handle both */

	if (fd) {
		short	shortdummy;
		long	longdummy;
		if (_read (fd, &shortdummy, sizeof (short)) != sizeof (short))
			goto readerr;
		if (_read (fd, &longdummy, sizeof (long)) != sizeof (long))
			goto readerr;
		if (_read (fd, &pass, sizeof (short)) != sizeof (short))
			goto readerr;
		pass--;
		if (_read (fd, &shortdummy, sizeof (short)) != sizeof (short))
			goto readerr;
		*result = shortdummy;

		if (pass < 900) {
			unsigned long startpt;
			FACHSW = 0;
			if (_read (fd, &FACMSW, sizeof (long)) != sizeof (long))
				goto readerr;
			if (_read (fd, &startpt, sizeof (long)) != sizeof (long))
				goto readerr;
			endpthi = 0;
			if (_read (fd, &endptlo, sizeof (long)) != sizeof (long))
				goto readerr;
			old_style = TRUE;
		} else {
			if (_read (fd, &shortdummy, sizeof (short)) != sizeof (short))
				goto readerr;
			old_style = shortdummy;
			if (_read (fd, &shortdummy, sizeof (short)) != sizeof (short))
				goto readerr;
			bits = shortdummy;
			if (_read (fd, &pass, sizeof (short)) != sizeof (short))
				goto readerr;
			if (_read (fd, &FACHSW, sizeof (long)) != sizeof (long))
				goto readerr;
			if (_read (fd, &FACMSW, sizeof (long)) != sizeof (long))
				goto readerr;
			if (_read (fd, &endpthi, sizeof (long)) != sizeof (long))
				goto readerr;
			if (_read (fd, &endptlo, sizeof (long)) != sizeof (long))
				goto readerr;
		}
		_close (fd);
		continuation = TRUE;
	} else {
		*result = FALSE;
		continuation = FALSE;
	}

/* Is prime already factored enough?  If so, return.  However, if we are */
/* processing a continuation file, then return the result from the file. */
/* This bizarre case happens when someone lowers the FactorOverride value */
/* in the middle of a factoring job. */

	if (bits >= test_bits) {
		if (fd) goto done;
		return (TRUE);
	}

/* Init the title */

	sprintf (buf, "Factoring GM%ld", p);
	title (buf);
	sprintf (buf, "%s factoring GM%ld to 2^%d\n",
		 fd ? "Resuming" : "Starting", p, test_bits);
	OutputStr(buf);

/* Loop testing larger and larger factors until we've tested to the */
/* appropriate number of bits.  Advance one bit at a time to minimize wasted */
/* time looking for a second factor after a first factor is found. */

	while (test_bits > bits) {
	    unsigned int end_bits;
	    unsigned long iters, iters_r;
	    int	stopping, saving;

/* Advance one bit at a time to minimize wasted time looking for a */
/* second factor after a first factor is found. */

	    end_bits = (bits < 50) ? 50 : bits + 1;
	    if (old_style) {
		    unsigned long x;
		    if (endpthi || endptlo == 0xFFFFFFFF) {
			    end_bits = 64;
			    endpthi = 1;
			    endptlo = 0;
		    }
		    else for (x = endptlo, end_bits = 31; x; x >>= 1) end_bits++;
	    }
	    if (end_bits > test_bits) end_bits = test_bits;

/* Compute the ending point for each pass */

	    if (!continuation) {
			if (end_bits < 64) {
				endpthi = 0;
				endptlo = 1L << (end_bits-32);
			} else {
				endpthi = 1L << (end_bits-64);
				endptlo = 0;
			}
	    }

/* Sixteen passes.  Two for the 1 or 5 mod 8 factors times two for the */
/* 1 or 2 mod 3 factors times four for the 1, 2, 3, or 4 mod 5 factors. */

	    iters_r = 0;
	    iters = 0;
	    if (! continuation) pass = 0;
	    for ( ; pass < 16; pass++) {

/* Set the starting point only if we are not resuming from */
/* a continuation file.  For no particularly good reason we */
/* quickly redo trial factoring for factors below 2^50. */

			if (continuation)
				continuation = FALSE;
			else {
//				if (bits < 50) {	// Bug ?
				if (bits < 32) {
					FACHSW = 0;
					FACMSW = 0;
				} else if (bits < 64) {
					FACHSW = 0;
					FACMSW = 1L << (bits-32);
				} else {
					FACHSW = 1L << (bits-64);
					FACMSW = 0;
				}
			}

/* Only test for factors less than 2^32 on the first pass */

			if (FACHSW == 0 && FACMSW == 0 && pass != 0) FACMSW = 1;

/* Setup the factoring program */

			FACPASS = pass;
			factorSetup (p);

/* Loop until all factors tested or factor found */

			for ( ; ; ) {
				int	res;

/* Use this opportunity to perform other miscellaneous tasks that may */
/* be required by this particular OS port */

//				doMiscTasks ();

/* Do a chunk of factoring */

				start_timer (0);
				res = factorAndVerify (p);
				end_timer (0);
				if (res != 2) break;

/* Set flag if we are saving or stopping */

				stopping = stopCheck ();
				if ((iters_r & 0x7F) == 0 && !stopping) {
					time (&current_time);
					saving = (current_time-start_time > write_time);
				} else
					saving = 0;

/* Output informative message */

				if (++iters >= ITER_OUTPUT) {
					char	fmt_mask[80];
					double	percent;
					percent = facpct (pass, bits, endpthi, endptlo);
					percent = trunc_percent (percent);
					sprintf (fmt_mask, FACMSG, PRECISION);
					sprintf (buf, fmt_mask, p, end_bits, percent);
					OutputTimeStamp ();
					OutputStr (buf);
					OutputStr ("  Time: ");
					print_timer (0, TIMER_NL | TIMER_OPT_CLR);
					sprintf (fmt_mask, "%%.%df%%%%", PRECISION);
					sprintf (buf, fmt_mask, percent);
					title (buf);
					iters = 0;
				}

/* Output informative message */

				if (++iters_r >= ITER_OUTPUT_RES ||
					(NO_GUI && stopping)) {
					char	fmt_mask[80];
					double	percent;
					percent = facpct (pass, bits, endpthi, endptlo);
					percent = trunc_percent (percent);
					sprintf (fmt_mask, FACMSG, PRECISION);
					sprintf (buf, fmt_mask, p, end_bits, percent);
					strcat (buf, "\n");
					writeResults (buf);
					iters_r = 0;
				}

/* If an escape key was hit, write out the results and return */

				if (stopping || saving) {
					short	shortdummy;
					long	longdummy;
					fd = _creat (filename, CREATE_FILE_ACCESS);
					_close (fd);
					fd = _open (filename, _O_BINARY | _O_RDWR);
					shortdummy = 2;
					writelg = _write (fd, &shortdummy, sizeof (short));
					longdummy = 0;
					writelg = _write (fd, &longdummy, sizeof (long));
					shortdummy = 999;
					writelg = _write (fd, &shortdummy, sizeof (short));
					shortdummy = *result;
					writelg = _write (fd, &shortdummy, sizeof (short));
					shortdummy = old_style;
					writelg = _write (fd, &shortdummy, sizeof (short));
					shortdummy = bits;
					writelg = _write (fd, &shortdummy, sizeof (short));
					writelg = _write (fd, &pass, sizeof (short));
					writelg = _write (fd, &FACHSW, sizeof (long));
					writelg = _write (fd, &FACMSW, sizeof (long));
					writelg = _write (fd, &endpthi, sizeof (long));
					writelg = _write (fd, &endptlo, sizeof (long));
					_commit (fd);
					_close (fd);
					if (stopping) {
						factorDone ();
						return (FALSE);
					}
					start_time = current_time;
				}

/* Test for completion */

				if (FACHSW > endpthi ||
					(FACHSW == endpthi && FACMSW >= endptlo))
					goto nextpass;
			}


/* Set flag indicating a factor has been found */

			*result = TRUE;

/* We used to continue factoring to find a smaller factor in a later pass. */
/* However, there was a bug - restarting from the save file skipped the */
/* further factoring AND the time it takes to search for smaller factors */
/* is getting longer and longer as we factor deeper and deeper.  Therefore, */
/* in version 20 I've elected to no longer search for smaller factors. */
/* The one exception is users that are using FactorOverride to locate */
/* small factors. */

			if (work_type == WORK_FACTOR ||
				IniGetInt (INI_FILE, "FactorOverride", 99) > 60) break;

			if (FACMSW != 0xFFFFFFFF) {
				endpthi = FACHSW;
				endptlo = FACMSW+1;
			} else {
				endpthi = FACHSW+1;
				endptlo = 0;
			}

/* Do next of the 16 passes */

nextpass:	;
	    }

/* If we've found a factor, then we are done factoring */

	    if (*result) break;

/* Advance the how far factored variable */

	    bits = end_bits;
	    old_style = FALSE;
	}

done:

/* Delete the continuation file */

	_unlink (filename);

/* Clear rolling average start time in case we've */
/* interrupted a Lucas-Lehmer test. */

//	clearRollingStart ();

/* All done */

	factorDone ();
	return (TRUE);

/* Return a kludgy error code to indicate an error reading intermediate file */

readerr:_close (fd);
	*result = 999;
	return (TRUE);
}


/* Factor a prime using factors of the specified size */

char NOFAC[] = "GM%ld no factor from 2^%d to 2^%d\n";

int primeSieve (
	unsigned long startp,	// The prime to be factored
//	unsigned long endp,
	unsigned short minbits,
	unsigned short maxbits,
	char *str,		// N as a string
	char *strp,		// NP as a string
	int	*result,	/* Returns true if factor found */
	int	fd)			/* Continuation file */
{
	long	write_time = DISK_WRITE_TIME * 60;
	char	filename[20], buf[80];
//	int	increasing;
	int	continuation;
	unsigned long p, endpthi, endptlo, iters, iters_r;
	unsigned short bits;
	short	pass;
	int	stopping, saving;
	time_t	start_time, current_time;

/* Clear all timers */

//	clear_timers ();

/* Get the current time */

	time (&start_time);

/* Is this a continuation?  If so, read continuation file. */

	if (fd) {
		short type;
		readlg = _read (fd, &type, sizeof (short));
		readlg = _read (fd, &p, sizeof (long));			// dummy
		readlg = _read (fd, &p, sizeof (long));
		readlg = _read (fd, &pass, sizeof (short));
		readlg = _read (fd, &bits, sizeof (short));
		readlg = _read (fd, &FACHSW, sizeof (long));
		readlg = _read (fd, &FACMSW, sizeof (long));
		readlg = _read (fd, &factored, sizeof (long));
		readlg = _read (fd, &eliminated, sizeof (long));
		_close (fd);
		continuation = TRUE;
	} else
		continuation = FALSE;

/* Init filename */

	tempFileName (filename, 'g', N);

	if (!continuation) p = startp;
	if (maxbits < 32) maxbits = 32;
	if (!continuation) bits = minbits;

/* Init the title */

	sprintf (buf, "Factoring GM%ld", p);
	title (buf);
	sprintf (buf, "%s factoring GM%ld to 2^%d\n",
		 fd ? "Resuming" : "Starting", p, maxbits);
//	OutputBoth (buf);
	OutputStr(buf);


/* Loop through all the bit levels */

/* Loop testing larger and larger factors until we've tested to the */
/* appropriate number of bits.  Advance one bit at a time to minimize wasted */
/* time looking for a second factor after a first factor is found. */

		while (maxbits > bits) {
			unsigned int end_bits;

			end_bits = (bits < 50) ? 50 : bits + 1;
			if (end_bits > maxbits) end_bits = maxbits;

/* Determine how much we should factor */

			if (end_bits < 32) {
				endpthi = 0;
				endptlo = 1L;
			}
			else if (end_bits < 64) {
				endpthi = 0;
				endptlo = 1L << (end_bits-32);
			} else {
				endpthi = 1L << (end_bits-64);
				endptlo = 0;
			}

/* Sixteen passes! two for the 1 or 5 mod 8 factors times two for the */
/* 1 or 2 mod 3 factors times four for the 1, 2, 3, or 4 mod 5 factors. */

			iters = 0;
			iters_r = 0;
			if (!continuation) pass = 0;

			for ( ; pass < 16; pass++) {

				if (!continuation) {
					if (bits < 32) {
						FACHSW = 0;
						FACMSW = 0;
					} else if (bits < 64) {
						FACHSW = 0;
						FACMSW = 1L << (bits-32);
					} else {
						FACHSW = 1L << (bits-64);
						FACMSW = 0;
					}
				}

/* Only test for factors less than 2^32 on the first pass */

				if (FACHSW == 0 && FACMSW == 0 && pass != 0) FACMSW = 1;

/* Setup the factoring program */

				FACPASS = pass;
				factorSetup (p);
				continuation = FALSE;

/* Loop until all factors tested or factor found */

				for ( ; ; ) {
					int	res;

/* Factor some more */

					start_timer (0);
					res = factorAndVerify (p);
					end_timer (0);
					if (res != 2) goto bingo;

/* Set flag if we are saving or stopping */

					stopping = stopCheck ();
					if ((iters_r & 0x7F) == 0 && !stopping) {
						time (&current_time);
						saving = (current_time-start_time > write_time);
					} else
						saving = 0;

/* Output informative message */

					if (++iters >= ITER_OUTPUT) {
						char	fmt_mask[80];
						double	percent;
						percent = facpct (pass, bits, endpthi, endptlo);
						percent = trunc_percent (percent);
						sprintf (fmt_mask, FACMSG, PRECISION);
						sprintf (buf, fmt_mask, p, end_bits, percent);
						OutputTimeStamp ();
						OutputStr (buf);
						OutputStr ("  Time: ");
						print_timer (0, TIMER_NL | TIMER_OPT_CLR);
						sprintf (fmt_mask, "%%.%df%%%%", PRECISION);
						sprintf (buf, fmt_mask, percent);
						title (buf);
						iters = 0;
					}

/* Output informative message */

					if (++iters_r >= ITER_OUTPUT_RES) {
						char	fmt_mask[80];
						double	percent;
						percent = facpct (pass, bits, endpthi, endptlo);
						percent = trunc_percent (percent);
						sprintf (fmt_mask, FACMSG, PRECISION);
						sprintf (buf, fmt_mask, p, end_bits, percent);
						strcat (buf, "\n");
						writeResults (buf);
						iters_r = 0;
					}

/* If an escape key was hit, write out the results and return */

					if (stopping || saving) {
						short	four = 4;
						fd = _open (filename, _O_BINARY | _O_WRONLY | _O_TRUNC | _O_CREAT, CREATE_FILE_ACCESS);
						writelg = _write (fd, &four, sizeof (short));
						writelg = _write (fd, &p, sizeof (long)); /* dummy */
						writelg = _write (fd, &p, sizeof (long));
						writelg = _write (fd, &pass, sizeof (short));
						writelg = _write (fd, &bits, sizeof (short));
						writelg = _write (fd, &FACHSW, sizeof (long));
						writelg = _write (fd, &FACMSW, sizeof (long));
						writelg = _write (fd, &factored, sizeof (long));
						writelg = _write (fd, &eliminated, sizeof (long));
						_commit (fd);
						_close (fd);
						if (stopping) {
						factorDone ();
						return (FALSE);
						}
						start_time = current_time;
					}

/* Test for completion */

					if (FACHSW > endpthi ||
						(FACHSW == endpthi && FACMSW >= endptlo))
						break;

				}
			}

/* Advance the how far factored variable */

	    bits = end_bits;
		}

/* Output message if no factor found */

		sprintf (buf, NOFAC, p, max(minbits,1), maxbits);
		if (verbose)
			OutputBoth (buf);
		else
			OutputStr (buf);
		*result = FALSE;
		goto endfac;

/* Format and output the factor found message */

bingo:	if (!testgq)
			sprintf (buf, "%s has a factor : %s\n", str, facnstr);
		else if (!testgm)
			sprintf (buf, "%s has a factor : %s\n", strp, facnpstr);
		else
			sprintf (buf, "%s has a factor : %s and %s has a factor : %s\n", str, facnstr, strp, facnpstr);
		OutputBoth (buf);
		eliminated++;
		*result = TRUE;

endfac:


/* Delete the continuation file */

	_unlink (filename);

/* All done */

	factorDone ();
	return (TRUE);
}


/* Determine how much we should factor a 2^p+1 number (in bits) */
/* Don't let feeble 486 any Cyrix machines factor higher than 2^62 */
/* as this would execute some really slow floating point code */

unsigned int pfactorLimit (
	unsigned long p,
	int	work_type)
{
	unsigned int test, override;

	if (p > FAC80) test = 80;	/* Test all 80 bit factors */
	else if (p > FAC79) test = 79;	/* Test all 79 bit factors */
	else if (p > FAC78) test = 78;	/* Test all 78 bit factors */
	else if (p > FAC77) test = 77;	/* Test all 77 bit factors */
	else if (p > FAC76) test = 76;	/* Test all 76 bit factors */
	else if (p > FAC75) test = 75;	/* Test all 75 bit factors */
	else if (p > FAC74) test = 74;	/* Test all 74 bit factors */
	else if (p > FAC73) test = 73;	/* Test all 73 bit factors */
	else if (p > FAC72) test = 72;	/* Test all 72 bit factors */
	else if (p > FAC71) test = 71;	/* Test all 71 bit factors */
	else if (p > FAC70) test = 70;	/* Test all 70 bit factors */
	else if (p > FAC69) test = 69;	/* Test all 69 bit factors */
	else if (p > FAC68) test = 68;	/* Test all 68 bit factors */
	else if (p > FAC67) test = 67;	/* Test all 67 bit factors */
	else if (p > FAC66) test = 66;	/* Test all 66 bit factors */
	else if (p > FAC65) test = 65;	/* Test all 65 bit factors */
	else if (p > FAC64) test = 64;	/* Test all 64 bit factors */
	else if (p > FAC63) test = 63;	/* Test all 63 bit factors */
	else if (p > FAC62) test = 62;	/* Test all 62 bit factors */
	else if (p > FAC61) test = 61;	/* Test all 61 bit factors */
	else if (p > FAC60) test = 60;	/* Test all 60 bit factors */
	else if (p > FAC59) test = 59;	/* Test all 59 bit factors */
	else if (p > FAC58) test = 58;	/* Test all 58 bit factors */
	else if (p > FAC57) test = 57;	/* Test all 57 bit factors */
	else if (p > FAC56) test = 56;	/* Test all 56 bit factors */
	else test = 40;			/* Test all 40 bit factors */
	if (work_type == WORK_FACTOR) {
		override = (unsigned int)
			IniGetInt (INI_FILE, "FactorOverride", 99);
		if (override != 99) test = override;
	}
	if (work_type == WORK_DBLCHK) test--;
	return (test);
}
/* Prepare for making a 2^p+1 factoring run */

void pfactorSetup (unsigned long p)
{

/* Allocate 1MB memory for factoring */

	if (FACDATA == NULL) FACDATA = aligned_malloc (1000000, 65536);

/* Call the factoring setup assembly code */

	FACLSW = p;
	SRCARG = FACDATA;
	psetupf ();

/* If using the SSE2 factoring code, do more initialization */
/* We need to initialize much of the following data: */
/*	XMM_BITS30		DD	0,3FFFFFFFh,0,3FFFFFFFh
	XMM_INITVAL		DD	0,0,0,0
	XMM_INVFAC		DD	0,0,0,0
	XMM_I1			DD	0,0,0,0
	XMM_I2			DD	0,0,0,0
	XMM_F1			DD	0,0,0,0
	XMM_F2			DD	0,0,0,0
	XMM_F3			DD	0,0,0,0
	XMM_TWO_120_MODF1	DD	0,0,0,0
	XMM_TWO_120_MODF2	DD	0,0,0,0
	XMM_TWO_120_MODF3	DD	0,0,0,0
	XMM_INIT120BS		DD	0,0
	XMM_INITBS		DD	0,0
	XMM_BS			DD	0,0
	XMM_SHIFTER		DD	64 DUP (0)
	TWO_TO_FACSIZE_PLUS_62	DQ	0.0
	SSE2_LOOP_COUNTER	DD	0 */
/* The address to XMM_BITS30 was returned in SRCARG. */

// #ifndef X86_64 // useless
	if (CPU_FLAGS & CPU_SSE2) {
		unsigned long i, bits_in_factor;
		unsigned long *xmm_data;

/* Compute the number of bits in the factors we will be testing */

		if (FACHSW) bits_in_factor = 64, i = FACHSW;
		else if (FACMSW) bits_in_factor = 32, i = FACMSW;
		else return;
		while (i) bits_in_factor++, i >>= 1;

/* Factors 63 bits and below use the non-SSE2 code */

		if (bits_in_factor <= 63) return;

/* Set XMM_SHIFTER values (the first shifter value is not used). */
/* Also compute the initial value. */

		xmm_data = (unsigned long *) SRCARG;
		for (i = 0; p > bits_in_factor + 59; i++) {
			xmm_data[52+i*2] = (p & 1) ? 1 : 0;
			p >>= 1;
		}
		xmm_data[4] =			/* XMM_INITVAL */
		xmm_data[6] = p >= 90 ? 0 : (1 << (p - 60));
		xmm_data[44] = 62 - (120 - bits_in_factor);/* XMM_INIT120BS */
		xmm_data[46] = 62 - (p - bits_in_factor);/* XMM_INITBS */
		xmm_data[116] = i;		/* SSE2_LOOP_COUNTER */
		*(double *)(&xmm_data[114]) = pow ((double) 2.0, (int) (bits_in_factor + 62));
						/* TWO_TO_FACSIZE_PLUS_62 */

/* Set XMM_BS to 60 - (120 - fac_size + 1) as defined in factor64.mac */

		xmm_data[48] = bits_in_factor - 61;
	}
// #endif // useless
}



/* Wrapper code that verifies any factors of 2^p+1 found by the assembly code */

int pfactorAndVerify (
	unsigned long p)
{
	unsigned long hsw, msw;
	int	res;

/* Remember starting point in case of an error */

	hsw = FACHSW;
	msw = FACMSW;

/* Call assembly code */

loop:	res = pfactor64 ();

/* If a factor was not found, return. */

	if (res == 2) return (2);

/* Otherwise verify the factor. */

	if (FACHSW || FACMSW || FACLSW > 1) {
		giant	f, x;

		f = allocgiant (100);
		itog ((int) FACHSW, f);
		gshiftleft (32, f);
		uladdg (FACMSW, f);
		gshiftleft (32, f);
		uladdg (FACLSW, f);

		x = allocgiant (100);
		itog (2, x);
		powermod (x, p, f);
		uladdg (1, x);
		res = (!gcompg (f, x));
	
		free (f);
		free (x);

		if (res) {
			makestr (FACHSW, FACMSW, FACLSW, facnstr);
			return (1);
		}
	}

/* If factor is no good, print an error message, sleep, and */
/* restart the factoring code. */

	OutputBoth ("ERROR: Incorrect factor found.\n");
	if (! SleepFive ()) return (FALSE);
	FACHSW = hsw;
	FACMSW = msw;
	pfactorSetup (p);
	goto loop;
}

/* Trial factor a 2^p+1  candidate prior to running a SPRP test on (2^p+1)/3 */

char PFACMSG[] = "Factoring (2^%%ld+1)/3 to 2^%%d is %%.%df%%%% complete.";

int pprimeFactor (
	unsigned long p,		/* Exponent to factor */
	unsigned int bits,		/* How far already factored in bits */
	int	*result,		/* Returns true if factor found */
	int	work_type,		/* Work type from worktodo.ini file */
	int	fd)			/* Continuation file handle or zero */
{
	int	continuation, old_style;
	long	write_time = DISK_WRITE_TIME * 60;
	unsigned int test_bits;
	unsigned long endpthi, endptlo;
	short	pass;
	time_t	start_time, current_time;
	char	filename[20];
	char	buf[256];

/* Clear all timers */

	clear_timers ();

/* Get the current time */

	time (&start_time);

/* Init temporary file name */

	tempFileName (filename, 'f', NP);

/* Determine how much we should factor (in bits) */

	test_bits = pfactorLimit (p, work_type);
	if (test_bits < 32) test_bits = 32;

/* By default we do not do the old style of factoring which was 16 passes */
/* each pass testing from bits to test_bits */

	old_style = FALSE;

/* Is this a continuation?  If so, read continuation file. */
/* There are two types of continuation files, handle both */

	if (fd) {
		short	shortdummy;
		long	longdummy;
		if (_read (fd, &shortdummy, sizeof (short)) != sizeof (short))
			goto readerr;
		if (_read (fd, &longdummy, sizeof (long)) != sizeof (long))
			goto readerr;
		if (_read (fd, &pass, sizeof (short)) != sizeof (short))
			goto readerr;
		pass--;
		if (_read (fd, &shortdummy, sizeof (short)) != sizeof (short))
			goto readerr;
		*result = shortdummy;

		if (pass < 900) {
			unsigned long startpt;
			FACHSW = 0;
			if (_read (fd, &FACMSW, sizeof (long)) != sizeof (long))
				goto readerr;
			if (_read (fd, &startpt, sizeof (long)) != sizeof (long))
				goto readerr;
			endpthi = 0;
			if (_read (fd, &endptlo, sizeof (long)) != sizeof (long))
				goto readerr;
			old_style = TRUE;
		} else {
			if (_read (fd, &shortdummy, sizeof (short)) != sizeof (short))
				goto readerr;
			old_style = shortdummy;
			if (_read (fd, &shortdummy, sizeof (short)) != sizeof (short))
				goto readerr;
			bits = shortdummy;
			if (_read (fd, &pass, sizeof (short)) != sizeof (short))
				goto readerr;
			if (_read (fd, &FACHSW, sizeof (long)) != sizeof (long))
				goto readerr;
			if (_read (fd, &FACMSW, sizeof (long)) != sizeof (long))
				goto readerr;
			if (_read (fd, &endpthi, sizeof (long)) != sizeof (long))
				goto readerr;
			if (_read (fd, &endptlo, sizeof (long)) != sizeof (long))
				goto readerr;
		}
		_close (fd);
		continuation = TRUE;
	} else {
		*result = FALSE;
		continuation = FALSE;
	}

/* Is prime already factored enough?  If so, return.  However, if we are */
/* processing a continuation file, then return the result from the file. */
/* This bizarre case happens when someone lowers the FactorOverride value */
/* in the middle of a factoring job. */

	if (bits >= test_bits) {
		if (fd) goto done;
		return (TRUE);
	}

/* Init the title */

	sprintf (buf, "Factoring (2^%ld+1)/3", p);
	title (buf);
	sprintf (buf, "%s factoring (2^%ld+1)/3 to 2^%d\n",
		 fd ? "Resuming" : "Starting", p, test_bits);
	OutputStr(buf);

/* Loop testing larger and larger factors until we've tested to the */
/* appropriate number of bits.  Advance one bit at a time to minimize wasted */
/* time looking for a second factor after a first factor is found. */

	while (test_bits > bits) {
	    unsigned int end_bits;
	    unsigned long iters, iters_r;
	    int	stopping, saving;

/* Advance one bit at a time to minimize wasted time looking for a */
/* second factor after a first factor is found. */

	    end_bits = (bits < 50) ? 50 : bits + 1;
	    if (old_style) {
		    unsigned long x;
		    if (endpthi || endptlo == 0xFFFFFFFF) {
			    end_bits = 64;
			    endpthi = 1;
			    endptlo = 0;
		    }
		    else for (x = endptlo, end_bits = 31; x; x >>= 1) end_bits++;
	    }
	    if (end_bits > test_bits) end_bits = test_bits;

/* Compute the ending point for each pass */

	    if (!continuation) {
			if (end_bits < 64) {
				endpthi = 0;
				endptlo = 1L << (end_bits-32);
			} else {
				endpthi = 1L << (end_bits-64);
				endptlo = 0;
			}
	    }

/* Sixteen passes.  Two for the 1 or 3 mod 8 factors times two for the */
/* 1 or 2 mod 3 factors times four for the 1, 2, 3, or 4 mod 5 factors. */

	    iters_r = 0;
	    iters = 0;
	    if (! continuation) pass = 0;
	    for ( ; pass < 16; pass++) {

/* Set the starting point only if we are not resuming from */
/* a continuation file.  For no particularly good reason we */
/* quickly redo trial factoring for factors below 2^50. */

			if (continuation)
				continuation = FALSE;
			else {
//				if (bits < 50) {	// Bug ?
				if (bits < 32) {
					FACHSW = 0;
					FACMSW = 0;
				} else if (bits < 64) {
					FACHSW = 0;
					FACMSW = 1L << (bits-32);
				} else {
					FACHSW = 1L << (bits-64);
					FACMSW = 0;
				}
			}

/* Only test for factors less than 2^32 on the first pass */

			if (FACHSW == 0 && FACMSW == 0 && pass != 0) FACMSW = 1;

/* Setup the factoring program */

			FACPASS = pass;
			pfactorSetup (p);

/* Loop until all factors tested or factor found */

			for ( ; ; ) {
				int	res;

/* Use this opportunity to perform other miscellaneous tasks that may */
/* be required by this particular OS port */

//				doMiscTasks ();

/* Do a chunk of factoring */

				start_timer (0);
				res = pfactorAndVerify (p);
				end_timer (0);
				if (res != 2) break;

/* Set flag if we are saving or stopping */

				stopping = stopCheck ();
				if ((iters_r & 0x7F) == 0 && !stopping) {
					time (&current_time);
					saving = (current_time-start_time > write_time);
				} else
					saving = 0;

/* Output informative message */

				if (++iters >= ITER_OUTPUT) {
					char	fmt_mask[80];
					double	percent;
					percent = facpct (pass, bits, endpthi, endptlo);
					percent = trunc_percent (percent);
					sprintf (fmt_mask, PFACMSG, PRECISION);
					sprintf (buf, fmt_mask, p, end_bits, percent);
					OutputTimeStamp ();
					OutputStr (buf);
					OutputStr ("  Time: ");
					print_timer (0, TIMER_NL | TIMER_OPT_CLR);
					sprintf (fmt_mask, "%%.%df%%%%", PRECISION);
					sprintf (buf, fmt_mask, percent);
					title (buf);
					iters = 0;
				}

/* Output informative message */

				if (++iters_r >= ITER_OUTPUT_RES ||
					(NO_GUI && stopping)) {
					char	fmt_mask[80];
					double	percent;
					percent = facpct (pass, bits, endpthi, endptlo);
					percent = trunc_percent (percent);
					sprintf (fmt_mask, PFACMSG, PRECISION);
					sprintf (buf, fmt_mask, p, end_bits, percent);
					strcat (buf, "\n");
					writeResults (buf);
					iters_r = 0;
				}

/* If an escape key was hit, write out the results and return */

				if (stopping || saving) {
					short	shortdummy;
					long	longdummy;
					fd = _creat (filename, CREATE_FILE_ACCESS);
					_close (fd);
					fd = _open (filename, _O_BINARY | _O_RDWR);
					shortdummy = 2;
					writelg = _write (fd, &shortdummy, sizeof (short));
					longdummy = 0;
					writelg = _write (fd, &longdummy, sizeof (long));
					shortdummy = 999;
					writelg = _write (fd, &shortdummy, sizeof (short));
					shortdummy = *result;
					writelg = _write (fd, &shortdummy, sizeof (short));
					shortdummy = old_style;
					writelg = _write (fd, &shortdummy, sizeof (short));
					shortdummy = bits;
					writelg = _write (fd, &shortdummy, sizeof (short));
					writelg = _write (fd, &pass, sizeof (short));
					writelg = _write (fd, &FACHSW, sizeof (long));
					writelg = _write (fd, &FACMSW, sizeof (long));
					writelg = _write (fd, &endpthi, sizeof (long));
					writelg = _write (fd, &endptlo, sizeof (long));
					_commit (fd);
					_close (fd);
					if (stopping) {
						factorDone ();
						return (FALSE);
					}
					start_time = current_time;
				}

/* Test for completion */

				if (FACHSW > endpthi ||
					(FACHSW == endpthi && FACMSW >= endptlo))
					goto nextpass;
			}


/* Set flag indicating a factor has been found */

			*result = TRUE;

/* We used to continue factoring to find a smaller factor in a later pass. */
/* However, there was a bug - restarting from the save file skipped the */
/* further factoring AND the time it takes to search for smaller factors */
/* is getting longer and longer as we factor deeper and deeper.  Therefore, */
/* in version 20 I've elected to no longer search for smaller factors. */
/* The one exception is users that are using FactorOverride to locate */
/* small factors. */

			if (work_type == WORK_FACTOR ||
				IniGetInt (INI_FILE, "FactorOverride", 99) > 60) break;

			if (FACMSW != 0xFFFFFFFF) {
				endpthi = FACHSW;
				endptlo = FACMSW+1;
			} else {
				endpthi = FACHSW+1;
				endptlo = 0;
			}

/* Do next of the 16 passes */

nextpass:	;
	    }

/* If we've found a factor, then we are done factoring */

	    if (*result) break;

/* Advance the how far factored variable */

	    bits = end_bits;
	    old_style = FALSE;
	}

done:

/* Delete the continuation file */

	_unlink (filename);

/* Clear rolling average start time in case we've */
/* interrupted a Lucas-Lehmer test. */

//	clearRollingStart ();

/* All done */

	factorDone ();
	return (TRUE);

/* Return a kludgy error code to indicate an error reading intermediate file */

readerr:_close (fd);
	*result = 999;
	return (TRUE);
}


/* Factor a prime using factors of the specified size */

char PNOFAC[] = "(2^%ld+1)/3 no factor from 2^%d to 2^%d\n";

int pprimeSieve (
	unsigned long startp,	// The prime to be factored
	unsigned short minbits,
	unsigned short maxbits,
	int	*result,	/* Returns true if factor found */
	int	fd)			/* Continuation file */
{
	long	write_time = DISK_WRITE_TIME * 60;
	char	filename[20], buf[1000];
//	int	increasing;
	int	continuation;
	unsigned long p, endpthi, endptlo, iters, iters_r;
	unsigned short bits;
	short	pass;
	int	stopping, saving;
	time_t	start_time, current_time;

/* Clear all timers */

//	clear_timers ();

/* Get the current time */

	time (&start_time);

/* Is this a continuation?  If so, read continuation file. */

	if (fd) {
		short type;
		readlg = _read (fd, &type, sizeof (short));
		readlg = _read (fd, &p, sizeof (long));			// dummy
		readlg = _read (fd, &p, sizeof (long));
		readlg = _read (fd, &pass, sizeof (short));
		readlg = _read (fd, &bits, sizeof (short));
		readlg = _read (fd, &FACHSW, sizeof (long));
		readlg = _read (fd, &FACMSW, sizeof (long));
		readlg = _read (fd, &factored, sizeof (long));
		readlg = _read (fd, &eliminated, sizeof (long));
		_close (fd);
		continuation = TRUE;
	} else
		continuation = FALSE;

/* Init filename */

	tempFileName (filename, 'g', NP);

	if (!continuation) p = startp;
	if (maxbits < 32) maxbits = 32;
	if (!continuation) bits = minbits;

/* Init the title */

	sprintf (buf, "Factoring (2^%ld+1)/3", p);
	title (buf);
	sprintf (buf, "%s factoring (2^%ld+1)/3 to 2^%d\n",
		 fd ? "Resuming" : "Starting", p, maxbits);
//	OutputBoth (buf);
	OutputStr(buf);


/* Loop through all the bit levels */

/* Loop testing larger and larger factors until we've tested to the */
/* appropriate number of bits.  Advance one bit at a time to minimize wasted */
/* time looking for a second factor after a first factor is found. */

		while (maxbits > bits) {
			unsigned int end_bits;

			end_bits = (bits < 50) ? 50 : bits + 1;
			if (end_bits > maxbits) end_bits = maxbits;

/* Determine how much we should factor */

			if (end_bits < 32) {
				endpthi = 0;
				endptlo = 1L;
			}
			else if (end_bits < 64) {
				endpthi = 0;
				endptlo = 1L << (end_bits-32);
			} else {
				endpthi = 1L << (end_bits-64);
				endptlo = 0;
			}

/* Sixteen passes! two for the 1 or 5 mod 8 factors times two for the */
/* 1 or 2 mod 3 factors times four for the 1, 2, 3, or 4 mod 5 factors. */

			iters = 0;
			iters_r = 0;
			if (!continuation) pass = 0;

			for ( ; pass < 16; pass++) {

				if (!continuation) {
					if (bits < 32) {
						FACHSW = 0;
						FACMSW = 0;
					} else if (bits < 64) {
						FACHSW = 0;
						FACMSW = 1L << (bits-32);
					} else {
						FACHSW = 1L << (bits-64);
						FACMSW = 0;
					}
				}

/* Only test for factors less than 2^32 on the first pass */

				if (FACHSW == 0 && FACMSW == 0 && pass != 0) FACMSW = 1;

/* Setup the factoring program */

				FACPASS = pass;
				pfactorSetup (p);
				continuation = FALSE;

/* Loop until all factors tested or factor found */

				for ( ; ; ) {
					int	res;

/* Factor some more */

					start_timer (0);
					res = pfactorAndVerify (p);
					end_timer (0);
					if (res != 2) goto bingo;

/* Set flag if we are saving or stopping */

					stopping = stopCheck ();
					if ((iters_r & 0x7F) == 0 && !stopping) {
						time (&current_time);
						saving = (current_time-start_time > write_time);
					} else
						saving = 0;

/* Output informative message */

					if (++iters >= ITER_OUTPUT) {
						char	fmt_mask[80];
						double	percent;
						percent = facpct (pass, bits, endpthi, endptlo);
						percent = trunc_percent (percent);
						sprintf (fmt_mask, PFACMSG, PRECISION);
						sprintf (buf, fmt_mask, p, end_bits, percent);
						OutputTimeStamp ();
						OutputStr (buf);
						OutputStr ("  Time: ");
						print_timer (0, TIMER_NL | TIMER_OPT_CLR);
						sprintf (fmt_mask, "%%.%df%%%%", PRECISION);
						sprintf (buf, fmt_mask, percent);
						title (buf);
						iters = 0;
					}

/* Output informative message */

					if (++iters_r >= ITER_OUTPUT_RES) {
						char	fmt_mask[80];
						double	percent;
						percent = facpct (pass, bits, endpthi, endptlo);
						percent = trunc_percent (percent);
						sprintf (fmt_mask, PFACMSG, PRECISION);
						sprintf (buf, fmt_mask, p, end_bits, percent);
						strcat (buf, "\n");
						writeResults (buf);
						iters_r = 0;
					}

/* If an escape key was hit, write out the results and return */

					if (stopping || saving) {
						short	four = 4;
						fd = _open (filename, _O_BINARY | _O_WRONLY | _O_TRUNC | _O_CREAT, CREATE_FILE_ACCESS);
						writelg = _write (fd, &four, sizeof (short));
						writelg = _write (fd, &p, sizeof (long)); /* dummy */
						writelg = _write (fd, &p, sizeof (long));
						writelg = _write (fd, &pass, sizeof (short));
						writelg = _write (fd, &bits, sizeof (short));
						writelg = _write (fd, &FACHSW, sizeof (long));
						writelg = _write (fd, &FACMSW, sizeof (long));
						writelg = _write (fd, &factored, sizeof (long));
						writelg = _write (fd, &eliminated, sizeof (long));
						_commit (fd);
						_close (fd);
						if (stopping) {
						factorDone ();
						return (FALSE);
						}
						start_time = current_time;
					}

/* Test for completion */

					if (FACHSW > endpthi ||
						(FACHSW == endpthi && FACMSW >= endptlo))
						break;

				}
			}

/* Advance the how far factored variable */

	    bits = end_bits;
		}

/* Output message if no factor found */

		sprintf (buf, PNOFAC, p, max(minbits,1), maxbits);
		if (verbose)
			OutputBoth (buf);
		else
			OutputStr (buf);
		*result = FALSE;
		goto endfac;

/* Format and output the factor found message */

bingo:	sprintf (buf, "(2^%ld+1)/3 has a factor : %s\n", p, facnstr);
		OutputBoth (buf);
		eliminated++;
		*result = TRUE;

endfac:


/* Delete the continuation file */

	_unlink (filename);

/* All done */

	factorDone ();
	return (TRUE);
}

#endif

/* Rotate a p-bit giant right shift_count bits.  Used to undo shifted FFT data. */

int rotategp (				/* Return false if failed due to memory allocation error */
	giant	v,				/* Giant to rotate right */
	unsigned long p,		/* Exponent */
	unsigned long shift_count	/* Number of bits to rotate right */
	)
{
	giant	vlo, modulus, mvlo;


/* If rotate count is zero, no work needed */

	if (shift_count >= p)				// only to be careful...
		shift_count %= p;				// 31/10/20

	if (shift_count == 0) return (1);

/* Convert current iteration to binary */

	vlo = allocgiant ((p >> 4) + 5);		// JP 24/04/20
	if (vlo == NULL) return (0);
	mvlo = allocgiant ((p >> 4) + 5);		// JP 06/05/20
	if (mvlo == NULL) return (0);
	modulus = allocgiant ((p >> 4) + 5);	// JP 24/04/20
	if (modulus == NULL) return (0);
	dbltog (w->k, modulus);					// modulus is 2^p-1 (Mersenne),2^p+1 (Wagstaff or Gaussian Mersenne norm),k*2^p+1 Proth or k*2^p-1 Riesel
	gshiftleft (p, modulus);
	iaddg (w->c, modulus);
	setone (vlo);
	gshiftleft (shift_count, vlo);			// inverse of 2^shift_count modulo k*2^p+c
	invg (modulus,vlo);
	gtog (modulus, mvlo);
	if ((w->c>0) && ((w->prp_residue_type==GMN_TYPE)?shift_count&2:shift_count&1))
		subg (vlo, mvlo);					// change the sign if necessary
	else
		gtog (vlo, mvlo);
	mulg (mvlo, v);							// restore unshifted v
	modg (modulus, v);
	free (vlo);
	free (modulus);
	free (mvlo);
	return (1);
}

/* Format the ETA for output to the worker window */

void formatETA (
	double	howlong,		/* how long to complete (in seconds) */
	char	*buf)
{
	double days, hours, minutes, seconds;
	days = floor (howlong / 86400.0);  howlong -= days * 86400.0;
	hours = floor (howlong / 3600.0);  howlong -= hours * 3600.0;
	minutes = floor (howlong / 60.0);  howlong -= minutes * 60.0;
	seconds = floor (howlong);
	if (days >= 3.0)
		sprintf (buf, ", ETA: %dd %02d:%02d", (int) days, (int) hours, (int) minutes);
	else
		sprintf (buf, ", ETA: %02d:%02d:%02d", (int) (days * 24.0 + hours), (int) minutes, (int) seconds);
}

/* Compare two (possibly shifted) gwnums for equality.  Used in PRP error-checking. */

int areTwoPRPValsEqual (
	gwhandle *gwdata,
	unsigned long p,		/* Mersenne exponent (for shifting) */		
	gwnum	val1,			/* Value #1 */
	unsigned long units_bit1,	/* Shift count #1 */
	gwnum	val2,			/* Value #2 */
	unsigned long units_bit2	/* Shift count #2 */
)
{
	giant	tmp1, tmp2;
	int	diff, err_code1, err_code2;
	tmp1 = popg (&gwdata->gdata, ((unsigned long) gwdata->bit_length >> 4) + 5);
	err_code1 = gwtogiant (gwdata, val1, tmp1) || isZero (tmp1);
	rotategp (tmp1, p, units_bit1);
	tmp2 = popg (&gwdata->gdata, ((unsigned long) gwdata->bit_length >> 4) + 5);
	err_code2 = gwtogiant (gwdata, val2, tmp2) || isZero (tmp2);
	rotategp (tmp2, p, units_bit2);
	diff = gcompg (tmp1, tmp2);
	pushg (&gwdata->gdata, 2);
	return (err_code1 == 0 && err_code2 == 0 && !diff);
}

/* multiplication or squaring  state to be written to / read from save files */

struct program_state {
	gwnum	x;			/* The current value in our left-to-right exponentiation */
	gwnum	y;					/* used when some interim value is needed. */
	unsigned long counter;		/* Current "iteration" counter */
	unsigned long units_bit;	/* For shifting FFT data -- allows more robust double-checking */
	unsigned long units_bit2;	/* Keep the shift count for y */
	unsigned long error_count;	/* Count of any errors that have occurred during test */
	unsigned int prp_base;		/* Fermat PRP base, default is 3 */
	int	two_power_opt;			/* TRUE is power of two optimizations are enabled (N adjusted to create long run of squarings) */
	int	residue_type;			/* Type of residue to generate (5 different residue types are supported) */
	int	error_check_type;		/* 0=none, 1=Gerbicz, 2=double-checking */
	int	state;					/* State variable see definitions below */
	gwnum	alt_x;				/* When doing ERRCHK_DBLCHK, this is the alternate x value */
								/* When doing ERRCHK_GERBICZ, this is the comparison checksum value being calculated */
	unsigned long alt_units_bit;/* When doing ERRCHK_DBLCHK, this is the alternate shift count */
	gwnum	u0;					/* Saved first value of the Gerbicz checksum function */
	gwnum	d;					/* Last computed value of the Gerbicz checksum function */
	unsigned long L;			/* Iterations between multiplies in computing Gerbicz checksum */
	unsigned long start_counter;	/* Counter at start of current Gerbicz or double-check block */
	unsigned long next_mul_counter;	/* Counter when next Gerbicz multiply takes place */
	unsigned long end_counter;	/* Counter when current Gerbicz or double-check block ends */
	giant gx;					/* x result of the test in giant form */
	giant gy;					/* y result of the test in giant form */
} ps;

#define ERRCHK_NONE		0	/* No high-reliability error-checking */
#define ERRCHK_GERBICZ	1	/* Gerbicz high-reliability error-checking -- very low overhead */
#define ERRCHK_DBLCHK	2	/* Run test twice, comparing residues along the way. Highly-reliable, very expensive. */

#define STATE_NORMAL				 0	/* Normal left-to-right exponentiation */
#define STATE_DCHK_PASS1			10	/* Do squarings for a while, then rollback counter and switch to pass 2 */
#define STATE_DCHK_PASS2			11	/* Do squarings for a while, compare vals, then switch back to pass 1 */
#define STATE_GERB_START_BLOCK		22	/* Determine how many iters are in the next Gerbicz block */
#define STATE_GERB_MID_BLOCK		23	/* Do squarings for L iterations */
#define STATE_GERB_MID_BLOCK_MULT	24	/* Do checksum multiply after the L-th squaring (except last block) */
#define STATE_GERB_END_BLOCK		25	/* After L^2 squarings, do alt squarings to compute 2nd Gerbicz compare value */
#define STATE_GERB_END_BLOCK_MULT	26	/* After L alt squarings, do one last mul to compute checksum #2 */
#define STATE_GERB_FINAL_MULT		27	/* Do one last mul to compute checksum #1 and then compare checksum values */

/* Write intermediate PRP results to a file */
/* The PRP save file format is: */
/*	u32		magic number  (different for ll, p-1, prp, tf, ecm) */
/*	u32		version number */
/*	double		pct complete */
/*	char(11)	stage */
/*	char(1)		pad */
/*	u32		checksum of following data */
/*	u32		error_count */
/*	u32		iteration counter */
/*	u32		prp base (version number >= 2) */
/*	u32		shift count (version number >= 2) */
/*	u32		power-of-two optimization was used (version number >= 2) */
/*	u32		residue type (version number >= 3) */
/*	u32		error-check type (version number >= 3) */
/*	u32		state (version number >= 3) */
/*	u32		alternate shift count (version number >= 3) */
/*	u32		L - iterations between Gerbicz multiplies (version number >= 3) */
/*	u32		error-checking start counter (version number >= 3) */
/*	u32		error-checking next Gerbicz multiply counter (version number >= 3) */
/*	u32		error-checking end counter (version number >= 3) */
/*	gwnum		FFT data for x (u32 len, array u32s) */
/*	gwnum		FFT data for alt_x (u32 len, array u32s) (version number >= 3) */
/*	gwnum		FFT data for u0 (u32 len, array u32s) (version number >= 3) */
/*	gwnum		FFT data for d (u32 len, array u32s) (version number >= 3) */

/* Prepare for writing save files */

void writeSaveFileStateInit (
	writeSaveFileState *state,
	char	*filename,
	int	num_special_save_files)
{
	strcpy (state->base_filename, filename);
	state->num_ordinary_save_files = NUM_BACKUP_FILES;
	state->num_special_save_files = num_special_save_files;
	state->special = 0;			/* Init with "no ordinary files are special" */
}

int writeSaveFile (
	gwhandle *gwdata,
	writeSaveFileState *write_save_file_state,
	struct work_unit *w,
	struct program_state *ps)
{
	int	fd, i;
	unsigned long sum = 0;

/* Now save to the intermediate file */
	fd = openWriteSaveFile (write_save_file_state);
	if (fd < 0) return (FALSE);

	if (!write_header (fd, PRP_MAGICNUM, PRP_VERSION, w)) goto err;

	if (!write_long298 (fd, ps->error_count, &sum)) goto err;
	if (!write_long298 (fd, ps->counter, &sum)) goto err;
	if (!write_long298 (fd, ps->prp_base, &sum)) goto err;
	if (!write_long298 (fd, ps->units_bit, &sum)) goto err;
	if (!write_long298 (fd, ps->units_bit2, &sum)) goto err;
	if (!write_long298 (fd, ps->two_power_opt, &sum)) goto err;
	if (!write_long298 (fd, ps->residue_type, &sum)) goto err;
	if (!write_long298 (fd, ps->error_check_type, &sum)) goto err;
	if (!write_long298 (fd, ps->state, &sum)) goto err;
	if (!write_long298 (fd, ps->alt_units_bit, &sum)) goto err;
	if (!write_long298 (fd, ps->L, &sum)) goto err;
	if (!write_long298 (fd, ps->start_counter, &sum)) goto err;
	if (!write_long298 (fd, ps->next_mul_counter, &sum)) goto err;
	if (!write_long298 (fd, ps->end_counter, &sum)) goto err;
	if (!write_gwnum298 (fd, gwdata, ps->x, &sum)) goto err;
	if (!write_gwnum298 (fd, gwdata, ps->y, &sum)) goto err;

	if (ps->state != STATE_NORMAL && ps->state != STATE_GERB_MID_BLOCK && ps->state != STATE_GERB_MID_BLOCK_MULT) {
		if (!write_gwnum298 (fd, gwdata, ps->alt_x, &sum)) goto err;
	}

	if (ps->state != STATE_NORMAL && ps->state != STATE_DCHK_PASS1 && ps->state != STATE_DCHK_PASS2 &&
	    ps->state != STATE_GERB_START_BLOCK && ps->state != STATE_GERB_FINAL_MULT) {
		if (!write_gwnum298 (fd, gwdata, ps->u0, &sum)) goto err;
	}

	if (ps->state != STATE_NORMAL && ps->state != STATE_DCHK_PASS1 && ps->state != STATE_DCHK_PASS2 &&
	    ps->state != STATE_GERB_START_BLOCK) {
		if (!write_gwnum298 (fd, gwdata, ps->d, &sum)) goto err;
	}
	
/* Save the five timers */

	for (i=0; i<5; i++) {
		if (timers[i+5] != 0.0) {// if the timer was running
			end_timer (i);			// update and save it
			if (! write_double (fd, timers[i], (long*)&sum)) {
				start_timer (i);	// and then, restart it, even if write is in error!
				goto err;
			}
			if (! write_double (fd, timers[i+5], (long*)&sum)) { // save the timer status
				start_timer (i);	// and then, restart it, even if write is in error!
				goto err;
			}
			start_timer (i);	// and then, restart it!
		}
		else {
			if (! write_double (fd, timers[i], (long*)&sum)) goto err;	// save the timer
			if (! write_double (fd, timers[i+5], (long*)&sum)) goto err;	// save its status
		}
	}
	
	_lseek (fd, CHECKSUM_OFFSET, SEEK_SET);
	if (_write (fd, &sum, sizeof (unsigned long)) != sizeof (unsigned long)) return (FALSE);

	closeWriteSaveFile (write_save_file_state, fd);
	return (TRUE);

/* An error occured.  Delete the current file. */

err:	deleteWriteSaveFile (write_save_file_state, fd);
	return (FALSE);
}

/* Read the data portion of an intermediate PRP results file */

int readPRPSaveFile (
	gwhandle *gwdata,		/* Handle to gwnum */
	char	*filename,		/* Save file name */
	struct work_unit *w,		/* Work unit */
	struct program_state *ps)		/* PRP state structure to read and fill in */
{
	int	fd, i;
	unsigned long savefile_prp_base, sum, filesum, version;
	fd = _open (filename, _O_BINARY | _O_RDONLY);
	if (fd <= 0) return (FALSE);
	if (!read_magicnum (fd, PRP_MAGICNUM)) goto err;
	if (!read_header (fd, &version, w, &filesum)) goto err;
	if (version == 0 || version > PRP_VERSION) goto err;

	sum = 0;
	if (!read_long298 (fd, &ps->error_count, &sum)) goto err;
	if (!read_long298 (fd, &ps->counter, &sum)) goto err;

	if (version <= 1) {
		savefile_prp_base = 3;
		ps->units_bit = 0;
		ps->two_power_opt = FALSE;
	} else {
		unsigned long savefile_two_power_opt;
		if (!read_long298 (fd, &savefile_prp_base, &sum)) goto err;
		if (!read_long298 (fd, &ps->units_bit, &sum)) goto err;
		if (!read_long298 (fd, &ps->units_bit2, &sum)) goto err;
		if (!read_long298 (fd, &savefile_two_power_opt, &sum)) goto err;
		ps->two_power_opt = savefile_two_power_opt;
	}
	if (savefile_prp_base != ps->prp_base) goto err;

	if (version <= 2) {
		ps->state = STATE_NORMAL;
		ps->error_check_type = ERRCHK_NONE;
	} else {
		unsigned long savefile_state, savefile_residue_type, savefile_error_check_type;
		// We might be able to handle some mismatched residue types, for now don't since this should never happen
		if (!read_long298 (fd, &savefile_residue_type, &sum)) goto err;
		if (savefile_residue_type != (unsigned long)ps->residue_type) goto err;
		// We could handle some mismatched error check types by looking at the state
		// variable, for now don't since this should never happen
		if (!read_long298 (fd, &savefile_error_check_type, &sum)) goto err;
		if (savefile_error_check_type != (unsigned long)ps->error_check_type) goto err;
		if (!read_long298 (fd, &savefile_state, &sum)) goto err;
		ps->state = savefile_state;
		if (!read_long298 (fd, &ps->alt_units_bit, &sum)) goto err;
		if (!read_long298 (fd, &ps->L, &sum)) goto err;
		if (!read_long298 (fd, &ps->start_counter, &sum)) goto err;
		if (!read_long298 (fd, &ps->next_mul_counter, &sum)) goto err;
		if (!read_long298 (fd, &ps->end_counter, &sum)) goto err;
	}

	// In version 3, we did not delay the final multiply in calculation of checksum #1.
	// We must ignore some save files because the version 3 and version 4 states are subtly different.
	if (version == 3 && (ps->state == STATE_GERB_MID_BLOCK_MULT ||
			     ps->state == STATE_GERB_END_BLOCK ||
			     ps->state == STATE_GERB_END_BLOCK_MULT)) goto err;

	// All PRP states wrote an x value
	if (!read_gwnum298 (fd, gwdata, ps->x, &sum)) goto err;
	if (!read_gwnum298 (fd, gwdata, ps->y, &sum)) goto err;

	// In version 3, we only wrote x to the save file at Gerbicz start block.  In version 4, we write x and the
	// identical alt_x.  There is added error protection by always having at least two gwnum values in memory.
	if (version == 3 && ps->state == STATE_GERB_START_BLOCK) {
		gwcopy (gwdata, ps->x, ps->alt_x);
		ps->alt_units_bit = ps->units_bit;
	}
	else if (ps->state != STATE_NORMAL && ps->state != STATE_GERB_MID_BLOCK && ps->state != STATE_GERB_MID_BLOCK_MULT) {
		if (!read_gwnum298 (fd, gwdata, ps->alt_x, &sum)) goto err;
	}

	// Most PRP Gerbicz states wrote a u0 value
	if (ps->state != STATE_NORMAL && ps->state != STATE_DCHK_PASS1 && ps->state != STATE_DCHK_PASS2 &&
	    ps->state != STATE_GERB_START_BLOCK && ps->state != STATE_GERB_FINAL_MULT) {
		if (!read_gwnum298 (fd, gwdata, ps->u0, &sum)) goto err;
	}

	// Most PRP Gerbicz states wrote a d value
	if (ps->state != STATE_NORMAL && ps->state != STATE_DCHK_PASS1 && ps->state != STATE_DCHK_PASS2 &&
	    ps->state != STATE_GERB_START_BLOCK) {
		if (!read_gwnum298 (fd, gwdata, ps->d, &sum)) goto err;
	}
	
/* Read the five timers and their status, and restart them! */

	for (i=0; i<5; i++) {
		if (! read_double (fd, &timers[i], (long*)&sum)) goto err;
		if (! read_double (fd, &timers[i+5], (long*)&sum)) goto err;
		start_timer (i);
	}

	// Validate checksum and return
	
	if (filesum != sum) goto err;
	_close (fd);
	return (TRUE);
err:	_close (fd);
	return (FALSE);
}

#define BIT 1
#define ITER 0

void writeresidue (
	gwhandle *gwdata,
	gwnum s,			// The gwnum data
	giant m,			// The current external modulus
	giant t,			// A temporary giant file	
	char *b,			// The output buffer
	const char *str,	// The tested number as a string
	const int bit,		// The iteration bit number
	const int kind		// Bit or Iteration
)
{
	char restr[20];

	gwtogiant (gwdata, s, t);	// The modulo reduction is done here
	modg (m, t);				// External modulus and gwnum's one may be different...
	if (abs(t->sign) < 1)		// make a 32 bit residue correct !!
		sprintf (restr, "%08lX%08lX", (unsigned long)0, (unsigned long)0);
	else if (abs(t->sign) < 2)
		sprintf (restr, "%08lX%08lX", (unsigned long)0, (unsigned long)t->n[0]);
	else
		sprintf (restr, "%08lX%08lX", (unsigned long)t->n[1], (unsigned long)t->n[0]);
	sprintf (b, "%s interim residue %s at %s %d\n", str, restr, kind? "bit" : "iteration", bit);
	if (verbose)
		OutputBoth (b);
	else
		OutputStr (b);
}

// Compute the number of digits of a large integer.

unsigned long gnbdg (giant nb, unsigned long digitbase) {
	giant gnbmax;
	unsigned long templ;
	if (digitbase==2)
		return (bitlen (nb));
	else {
		templ = (unsigned long)(floor((double)bitlen (nb) * log (2.0) /log ((double)digitbase))); // default value
		gnbmax = allocgiant (abs (nb->sign) + 2);
		ultog (digitbase, gnbmax);
		power (gnbmax, templ);				// compute digitbase^templ as a comparand
		while (gcompg (gnbmax, nb) < 0) {
			templ++;						// adjust the value if it is too small
			ulmulg (digitbase, gnbmax);		// and adjust the comparand
		}
		free (gnbmax);
		return (templ);
	}
}

int readFilew (
	char *data, char	*filename)
{
	int	fd;
	fd = _open (filename, _O_RDONLY | _O_BINARY);
	if (fd < 0) return (FALSE);
	if (!_read (fd, data, 1000))
		return (FALSE);
	_close (fd);
	return (TRUE);
}

int writeFilew (
	char	*msg, char *filename)
{
	int	fd;

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

#if defined (WIN32) || defined (__linux__) || defined (__FreeBSD__) || defined (__APPLE__)

int aprcltest (int prptest, int verbose)		// Primality test using compiled APRCL code
{
	int retcode;
	mpz_t n1;
	mpz_init (n1);

	if (mpz_set_str (n1, greatbuf, 10) != 0)
		return (6);								// Invalid numeric string...
	if (prptest)
		retcode = mpz_strongbpsw_prp (n1);		// Strong BPSW PRP test
	else
		retcode = mpz_aprtcle (n1, verbose);	// Prime test possibly with print out.
	return ((retcode == -1)? 7 : retcode);		// Result code returned (-1 if in error...)
}

int gmpwftest (char *nstr, char *bstr)			// Wieferich test using APRCL subroutines and gmp library.
{
	int retcode = 0, retcode2 = 0;
	mpz_t n;
	mpz_t pwn;
	mpz_t res;
	mpz_t nm1;
	mpz_t a;

	if (nstr == NULL)
		return (10);				// No string argument...
	if (mpz_init_set_str (n, nstr, 10) != 0) {
		mpz_clear (n);
		return (11);				// Invalid numeric string...
	}
	if (bstr == NULL) {				// No more parameters
		mpz_init_set_ui (a, 2);
	}
	else if (mpz_init_set_str (a, bstr, 10) != 0) {
		mpz_clear (n);
		mpz_clear (a);
		return (12);				// Invalid numeric string...
	}
	mpz_init_set_ui(res, 1);		// Init. res to 1
	mpz_init_set(nm1, n);
	mpz_init_set(pwn, n);			// init pwn to n^1 = n
	mpz_sub_ui(nm1, nm1, 1);		// compute n-1
	while (mpz_cmp_ui(res, 1) == 0) {
		mpz_powm(res, a, nm1, pwn);	// Compute a^(n-1) modulo pwn
		if (mpz_cmp_ui(res, 1) == 0)	// May be positive...
			retcode2++;
		mpz_mul (pwn, pwn, n);		// Set pwn to the next power of n.
	}
	retcode = retcode2;				// Default return...
	if (retcode2 >= 2) {			// May be positive...
		retcode = mpz_aprcl (n);	// Prime test, without any message
		if (retcode == 0)
			retcode = -retcode2;	// W-positive, but composite...
		else if (retcode != -1)		// No APRCL error...
			retcode = retcode2;		// Wieferich prime found if code >= 2
	}
	mpz_clear (n);
	mpz_clear (a);
	mpz_clear (res);
	mpz_clear (nm1);
	mpz_clear (pwn);
	return (retcode);
}

/* Generate temporary file name */
 
void tempFilenamew ( 
	char	*buf, char* c, mpz_t NN) 
{ 
	unsigned long remainder;
 
	remainder = mpz_fdiv_ui (NN, 19999981);
	sprintf (buf, "%s%07lu.txt", c, remainder % 10000000); 
} 

int gmpwfsearch (char *sstart, char *sstop, char *sbase) {
	int retcode = 0, first, outfd;
	unsigned long b;
	unsigned int sverbose = verbose;
	int resaprcl, stopping;
	unsigned long m = 0, phi = 1, prevphi = 0, i = 0, j = 0, ndp=9, np, startmod;
	char lbuf [1000] = {0}, lbuf2 [1000] = {0}, sresume[1000] = {0};
	char TEMP_FILE[80] = {0};
	char outputfile[80];
	mpz_t start;
	mpz_t stop;
	mpz_t n;
	mpz_t sqn;
	mpz_t res;
	mpz_t nm1;
	mpz_t a;
	unsigned long *t = NULL,*ta = NULL;	// Precrible arrays, dynamically allocated.

	if (mpz_init_set_str (start, sstart, 10) != 0) {
		sprintf (lbuf, "%s : Invalid numeric string for start...\n", sstart);
		if (verbose)
			OutputBoth (lbuf);
		else
			OutputStr (lbuf);
		return (4);			// Invalid numeric string...
	}
	if (mpz_init_set_str (stop, sstop, 10) != 0) {
		sprintf (lbuf, "%s : Invalid numeric string for stop...\n", sstop);
		if (verbose)
			OutputBoth (lbuf);
		else
			OutputStr (lbuf);
		return (4);			// Invalid numeric string...
	}
	if (mpz_init_set_str (a, sbase, 10) != 0) {
		sprintf (lbuf, "%s : Invalid numeric string for the base...\n", sbase);
		if (verbose)
			OutputBoth (lbuf);
		else
			OutputStr (lbuf);
		return (5);			// Invalid numeric string...
	}
	mpz_get_str (lbuf2, 10, a);
	sprintf (TEMP_FILE, "wf%s_%s.txt", lbuf2, sstart);
	if (fileExists (TEMP_FILE)) {
		readFilew (sresume, TEMP_FILE);
		sprintf (lbuf, "Resuming at n = %s\n", sresume);
		if (verbose)
			OutputBoth (lbuf);
		else
			OutputStr (lbuf);
		mpz_set_str (start, sresume, 10);
	}
	else {
		sprintf (lbuf, "Starting Wieferich prime search base %s from n = %s to %s\n", sbase, sstart, sstop);
		verbose = TRUE;				// To force a time stamp
		OutputBoth (lbuf);
		verbose = sverbose;			// Restore the verbose option
		first = TRUE;
	}
	for (np=ndp;np>=3;np--) {
		for (i=0;i<np;i++) {
			prevphi = phi;
			phi *= smallphi[i];
		}
		t = (unsigned long*)aligned_malloc((phi+10)*sizeof(unsigned long), 8);
		ta = (unsigned long*)aligned_malloc((prevphi+10)*sizeof(unsigned long), 8);
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
	IniGetString (INI_FILE, "PgenOutputFile", outputfile, 80, NULL);
	if (mpz_cmp_ui (start, smallprime[np-1]) <= 0) {
		for (i=0;i<np;i++) {
			mpz_set_ui (n, smallprime[i]);
			if (mpz_cmp(n, stop) <= 0) {
				mpz_set(nm1, n);
				mpz_set(sqn, n);
				mpz_mul (sqn, n, n);			// Set sqn to the square of n.
				mpz_sub_ui(nm1, nm1, 1);
				mpz_powm(res, a, nm1, sqn);
				if (mpz_cmp_ui(res, 1) == 0) {	// Positive result!
					mpz_get_str (lbuf2, 10, n);
					sprintf (lbuf,"%s is a base %lu Wieferich prime!!\n", lbuf2, b);
					verbose = TRUE;				// To force a time stamp
					OutputStr("\033[7m");
					OutputBoth(lbuf);
					OutputStr("\033[0m");
					verbose = sverbose;			// Restore the verbose option
					outfd = _open (outputfile, _O_TEXT | _O_RDWR | _O_APPEND | _O_CREAT, 0666);
					if (outfd) {
						if (first && (IniGetInt (INI_FILE, "PgenLine", 1) == 1)) {	// write the relevant header
							writelg = _write (outfd, "ABC$a$b\n", 8);
							first = FALSE;
						}
						sprintf (lbuf, "%s %s\n", lbuf2, sbase); 
						writelg = _write (outfd, lbuf, strlen (lbuf));
						_close (outfd);
					}
					if(IniGetInt(INI_FILE, "StopOnSuccess", 0)) {
						return (2);
					}
				}
			}
			else {
				sprintf (lbuf, "Range %s to %s completed for Wieferich base %s.\n", sstart, sstop, sbase);
				verbose = TRUE;				// To force a time stamp
				OutputBoth (lbuf);
				verbose = sverbose;			// Restore the verbose option
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
	for (i=0; ; i++) {
		if (i>=(np+phi)) {
			mpz_get_str (lbuf2, 10, n);
			writeFilew (lbuf2, TEMP_FILE);		// Make a checkpoint
			resaprcl = mpz_aprcl (n);			// Prime test, without any message
			if (resaprcl == 2)
				sprintf (lbuf, "Tested up to %s which is prime!\n", lbuf2);
			else
				sprintf (lbuf, "Tested up to %s\n", lbuf2);
			if (verbose)
				OutputBoth (lbuf);
			else
				OutputStr (lbuf);
			i = np;
			mpz_add_ui (start, start, m);
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
			stopping = escapeCheck ();
			if (stopping) {
				mpz_get_str (lbuf2, 10, n);
				writeFilew (lbuf2, TEMP_FILE);
				sprintf (lbuf, "stopping at n = %s\n", lbuf2);
				if (verbose)
					OutputBoth (lbuf);
				else
					OutputStr (lbuf);
				return (7);
			}
			mpz_set(nm1, n);
			mpz_sub_ui(nm1, nm1, 1);
			mpz_powm(res, a, nm1, n);		// res = a^nm1 (mod. n)
			if (mpz_cmp_ui(res, 1) != 0)
				continue;					// n is composite...
			else {							// n may be prime, test further
				mpz_set(sqn, n);
				mpz_mul (sqn, n, n);		// Set sqn to the square of n.
				mpz_powm(res, a, nm1, sqn);	// res = a^nm1 (mod. sqn)
			}
			if (mpz_cmp_ui(res, 1) == 0) {	// May be positive...
				retcode = mpz_aprcl (n);	// Prime test, without any message
				mpz_get_str (lbuf2, 10, n);
				if (retcode == -1) {
					sprintf (lbuf, "An error occurred in the APRCL prime test of %s...\n", lbuf2);
					if (verbose)
						OutputBoth (lbuf);
					else
						OutputStr (lbuf);
					retcode = 6;			// The test is in error...
				}
				else if (retcode == 0) {
					retcode = 1;			// W-positive, but composite...
					sprintf (lbuf, "%s is W-positive, but composite...\n", lbuf2);
					if (verbose)
						OutputBoth (lbuf);
					else
						OutputStr (lbuf);
				}
				else if (retcode == 2) {
					sprintf (lbuf,"%s is a base %lu Wieferich prime!!\n", lbuf2, b);
					verbose = TRUE;				// To force a time stamp
					OutputStr("\033[7m");
					OutputBoth(lbuf);
					OutputStr("\033[0m");
					verbose = sverbose;			// Restore the verbose option
					outfd = _open (outputfile, _O_TEXT | _O_RDWR | _O_APPEND | _O_CREAT, 0666);
					if (outfd) {
						if (first) {		// write the relevant header
							writelg = _write (outfd, "ABC $a$b\n", 9);
							first = FALSE;
						}
						sprintf (lbuf, "%s %s\n", lbuf2, sgb); 
						writelg = _write (outfd, lbuf, strlen (lbuf));
						_close (outfd);
					}
					if(IniGetInt(INI_FILE, "StopOnSuccess", 0)) {
						return (retcode);
					}

				}
				else {
					sprintf (lbuf, "Unexpected result while APRCL testing %s...\n", lbuf2);
					if (verbose)
						OutputBoth (lbuf);
					else
						OutputStr (lbuf);
				}
			}
			else
				continue;
		}
		else
			break;
	}
	_unlink (TEMP_FILE);
	sprintf (lbuf, "Range %s to %s completed for Wieferich base %s.\n", sstart, sstop, sbase);
	verbose = TRUE;				// To force a time stamp
	OutputBoth (lbuf);
	verbose = sverbose;			// Restore the verbose option
	return (retcode);
}

int gmpSearchWieferich (char *sstart, char *sstop, char *sbase)
{
	int retcode;
	if (sbase == NULL)
		sbase = "2";
	retcode = gmpwfsearch (sstart, sstop, sbase);
	if ((retcode == 7)||(retcode == 11)||(retcode == 3)||(retcode == 4)||(retcode == 5)||(retcode == 10))
		return (FALSE);
	if((retcode==2) && IniGetInt(INI_FILE, "StopOnSuccess", 0)) {
		return (FALSE);
	}
	return (TRUE);
}

#else

	STARTUPINFO aprstinfo;
	PROCESS_INFORMATION aprprinfo;

int aprcltest (int prptest, int verbose)		// Primality test using external APRCL program as a child process
{
	int ofd;
	unsigned long exitcode, errcode;
	char errbuf[100];
	char titre[] = "APRT-CLE Primality Test";
	char line[100] = {0};
	ofd = _open ("__pc__", _O_TEXT | _O_RDWR | _O_APPEND | _O_CREAT, 0666);
	if (ofd) {
		writelg = _write (ofd, greatbuf, strlen (greatbuf));
		_close (ofd);
	}
	else
		return (8);				// Could not create the file...

//	Initialisation de la structure wfstinfo avec les seules donn�es requises

	aprstinfo.cb = sizeof (STARTUPINFO);
	aprstinfo.lpDesktop = NULL;
	aprstinfo.lpTitle = titre;
	aprstinfo.dwFlags = STARTF_USESHOWWINDOW;
	aprstinfo.lpReserved2 = NULL;

	exitcode = STILL_ACTIVE;

// Construire la ligne de commande

	if (prptest)
		sprintf (line, "aprcl __pc__ prp");
	else if (verbose == 1)
		sprintf (line, "aprcl __pc__ prime progress");
	else if (verbose == 2)
		sprintf (line, "aprcl __pc__ prime details");
	else
		sprintf (line, "aprcl __pc__");

// Creer le processus fils

	if (!CreateProcess (				// Tentative de creation du processus fils
		"aprcl.exe",
		line,
		NULL,
		NULL,
		FALSE,
		0L,
		NULL,
		NULL,
		&aprstinfo,
		&aprprinfo))

	{
		errcode = GetLastError ();		// Echec...
		sprintf (errbuf, "Error %lu while trying to create new process\n", errcode);
		OutputStr (errbuf);
		_unlink ("__pc__");
		return (9);
	}

	while (exitcode == STILL_ACTIVE) {
		GetExitCodeProcess(aprprinfo.hProcess, &exitcode);
	}
	_unlink ("__pc__");
	return (exitcode);
}

	STARTUPINFO wftstinfo;
	PROCESS_INFORMATION wftprinfo;

int gmpwftest (char *nstr, char *bstr)				// Wieferich test using external WF program as a child process
{
	unsigned long exitcode, errcode;
	char errbuf[100];
	char titre[] = "GMP-APRCL Wieferich Test";
	char line[100] = {0};
//	Initialisation de la structure wfstinfo avec les seules donn�es requises

	wftstinfo.cb = sizeof (STARTUPINFO);
	wftstinfo.lpDesktop = NULL;
	wftstinfo.lpTitle = titre;
	wftstinfo.dwFlags = STARTF_USESHOWWINDOW;
	wftstinfo.lpReserved2 = NULL;

	exitcode = STILL_ACTIVE;

// Construire la ligne de commande

	if (bstr != NULL)
		sprintf (line, "tw %s %s", nstr, bstr);
	else
		sprintf (line, "tw %s", nstr);

// Creer le processus fils

	if (!CreateProcess (				// Tentative de creation du processus fils
		"tw.exe",
		line,
		NULL,
		NULL,
		FALSE,
		0L,
		NULL,
		NULL,
		&wftstinfo,
		&wftprinfo))

	{	
		errcode = GetLastError ();		// Echec...
		sprintf (errbuf, "Error %lu while trying to create new process\n", errcode);
		OutputStr (errbuf);
		return (-1000000000);
	}

	while (exitcode == STILL_ACTIVE) {
		GetExitCodeProcess(wftprinfo.hProcess, &exitcode);
	}
	return (exitcode);
}

#if defined (_CONSOLE)
	STARTUPINFO wfstinfo;
	PROCESS_INFORMATION wfprinfo;
#else
	extern	STARTUPINFO wfstinfo;
	extern	PROCESS_INFORMATION wfprinfo;
#endif

int gmpwfsearch (char *sstart, char *sstop, char *sbase) {

	unsigned long exitcode, errcode;
	char errbuf[100];
	char titre[] = "GMP-APRCL Wieferich Search";
	char line[100] = {0};

//	Initialisation de la structure wfstinfo avec les seules donn�es requises

	wfstinfo.cb = sizeof (STARTUPINFO);
	wfstinfo.lpDesktop = NULL;
	wfstinfo.lpTitle = titre;
	wfstinfo.dwFlags = STARTF_USESHOWWINDOW;
	wfstinfo.lpReserved2 = NULL;

	exitcode = STILL_ACTIVE;

// Construire la ligne de commande

	if (sbase != NULL)
		sprintf (line, "llrwfsrch %s %s %s", sstart, sstop, sbase);
	else
		sprintf (line, "llrwfsrch %s %s", sstart, sstop);

// Creer le processus fils

	if (!CreateProcess (				// Tentative de creation du processus fils
		"llrwfsrch.exe",
		line,
		NULL,
		NULL,
		FALSE,
		0L,
		NULL,
		NULL,
		&wfstinfo,
		&wfprinfo))

	{
		errcode = GetLastError ();		// Echec...
		sprintf (errbuf, "Error %lu while trying to create new process\n", errcode);
		OutputStr (errbuf);
		return (10);
	}

	while (exitcode == STILL_ACTIVE) {
		GetExitCodeProcess(wfprinfo.hProcess, &exitcode);
	}
	return (exitcode);
}


int gmpSearchWieferich (char *sstart, char *sstop, char *sbase)
{
	int retcode, i = 0, first, outfd;
	unsigned int sverbose = verbose;
	char lbuf [1000] = {0}, lbuf2 [1000] = {0};
	char TEMP_FILE[80] = {0}, RESULT_FILE[80] = {0};
	char outputfile[80];
	IniGetString (INI_FILE, "PgenOutputFile", outputfile, 80, NULL);
	if (sbase == NULL)
		sbase = "2";
	sprintf (TEMP_FILE, "wf%s_%s.txt", sbase, sstart);
	sprintf (RESULT_FILE,"wf%s_results.txt", sbase);
	if (fileExists (TEMP_FILE)) {
		readFilew (lbuf2, TEMP_FILE);
		sprintf (lbuf, "Resuming at n = %s\n", lbuf2);
		if (verbose)
			OutputBoth (lbuf);
		else
			OutputStr (lbuf);
	}
	else {
		sprintf (lbuf, "Starting Wieferich prime search base %s from n = %s to %s\n", sbase, sstart, sstop);
		verbose = TRUE;				// To force a time stamp
		OutputBoth (lbuf);
		verbose = sverbose;			// Restore the verbose option
		first = TRUE;
	}
	title ("Wieferich prime search in progress.");
	while (TRUE) {
		retcode = gmpwfsearch (sstart, sstop, sbase);
		clearline(100);
		if (fileExists (TEMP_FILE))
			readFilew (lbuf2, TEMP_FILE);
		if (retcode == 8) {
			sprintf (lbuf, "Tested up to %s\n", lbuf2);
		}
		else if (retcode == 9) {
			sprintf (lbuf, "Tested up to %s which is prime!\n", lbuf2);
		}
		else if ((retcode == 7)||(retcode == 11)) {
			sprintf (lbuf, "Stopping at n = %s\n", lbuf2);
		}
		else if (retcode == 2) {
			readFilew (lbuf2, RESULT_FILE);
			_unlink (RESULT_FILE);
			sprintf (lbuf, "%s is a base %s Wieferich prime!!\n", lbuf2, sbase);
		}
		else if (retcode == 1) {
			readFilew (lbuf2, RESULT_FILE);
			_unlink (RESULT_FILE);
			sprintf (lbuf, "%s is W-positive, but composite...\n", lbuf2);
		}
		else if (retcode == 6) {
			readFilew (lbuf2, RESULT_FILE);
			_unlink (RESULT_FILE);
			sprintf (lbuf, "An error occurred in the APRCL prime test of %s...\n", lbuf2);
		}
		else if (retcode == 0) {
			sprintf (lbuf, "Range %s to %s completed for Wieferich base %s.\n", sstart, sstop, sbase);
			verbose = TRUE;				// To force a time stamp
			OutputBoth (lbuf);
			verbose = sverbose;			// Restore the verbose option
			_unlink (RESULT_FILE);
			_unlink (TEMP_FILE);
			break;
		}
		else if ((retcode == 3)||(retcode == 4)||(retcode == 5)) {
			sprintf (lbuf, "At least one input parameter is invalid or missing...\n");
		}
		else if (retcode == 10) {
			sprintf (lbuf, "Wieferich prime searching not available...\n");
		}
		else {
			readFilew (lbuf2, RESULT_FILE);
			_unlink (RESULT_FILE);
			sprintf (lbuf, "Unexpected result while APRCL testing %s...\n", lbuf2);
		}
		if (retcode == 2) {
			clearline(100);
			verbose = TRUE;				// To force a time stamp
#if defined (_CONSOLE)
			hConsole = GetStdHandle(STD_OUTPUT_HANDLE);	// Access to Console attributes
			SetConsoleTextAttribute(hConsole, BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
			OutputBoth(lbuf);
			SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
#else
			OutputBoth(lbuf);
#endif
			verbose = sverbose;			// Restore the verbose option
			outfd = _open (outputfile, _O_TEXT | _O_RDWR | _O_APPEND | _O_CREAT, 0666);
			if (outfd) {
				if (first && (IniGetInt (INI_FILE, "PgenLine", 1) == 1)) {	// write the relevant header
					writelg = _write (outfd, "ABC $a$b\n", 9);
					first = FALSE;
				}
				sprintf (lbuf, "%s %s\n", lbuf2, sgb); 
				writelg = _write (outfd, lbuf, strlen (lbuf));
				_close (outfd);
			}
			clearline(100);
			if(IniGetInt(INI_FILE, "StopOnSuccess", 0)) {
				return (FALSE);
			}
		}
		else {
			if (verbose)
				OutputBoth (lbuf);
			else
				OutputStr (lbuf);
		}
		if ((retcode == 7)||(retcode == 11)||(retcode == 3)||(retcode == 4)||(retcode == 5)||(retcode == 10))
			return (FALSE);
	}
	return (TRUE);
}

#endif

int saprcltest (char *str, int prptest, int verbose) {
	int i;
	for (i=0;i<10000;i++) {
		greatbuf[i] = str[i];
		if (!str[i])
			break;
	}
	return (aprcltest(prptest, verbose));
}

int gaprcltest (giant N, int prptest, int verbose) {
	if (gnbdg (N,10) > 10000)
		return (-1);						// Number too large...
	if (N->sign == 1) {						// Trial divisions test is sufficient for this small number...
		return (isPrime (N->n[0]) ? 12 : 10);
	}
	gtoc (N, greatbuf, 10000);
	return (aprcltest(prptest, verbose));
}


int MakePrimoInput (giant N, char *str) {
	char buffer[100], primofilename[11];
	FILE *fout;

	tempFileName (primofilename, 't', N);	// Construct an input file name for Primo
	sprintf (primofilename + strlen(primofilename), ".in");
	fout = fopen (primofilename, "w");
	if (fout != NULL) {
		fprintf (fout, "[candidate]\n");	// Write the Primo input file
		fprintf (fout, "N=%s\n", str);
		fclose (fout);
		sprintf (buffer,"Candidate saved in file %s for further test with Primo.\n", primofilename);
		OutputBoth(buffer);
	}
	return (fout != NULL);
}


/* Test if M divides a^(N-1) - 1 -- gwsetup has already been called. */

int isexpdiv (
	gwhandle *gwdata,
	ghandle *gdata,
	unsigned long a,
	giant N,
	giant M,
	int	*res)
{
	unsigned long bit, bitpos, firstone = 0, iters;
	gwnum	x;
	giant	tmp;
	char	filename[20], buf[sgkbufsize+256], fft_desc[256], oldres64[17];
	long	write_time = DISK_WRITE_TIME * 60;
	int	echk, saving, stopping;
	time_t	start_time, current_time;
	double	reallyminerr = 1.0;
	double	reallymaxerr = 0.0;

	if (!IniGetInt(INI_FILE, "Testdiff", 0))	// Unless test of MAXDIFF explicitly required
		gwdata->MAXDIFF = 1.0E80;				// Diregard MAXDIFF...

/* Init, subtract 1 from N to compute a^(N-1) mod M */

	iaddg (-1, N);

	Nlen = bitlen (N);

	*res = TRUE;		/* Assume the residue is one */

/* Init filename */

	tempFileName (filename, 'z', N);

/* Allocate memory */

	x = gwalloc (gwdata);

	tmp = popg (gdata, (Nlen >> 3) + 8);

/* Optionally resume from save file and output a message */
/* indicating we are resuming a test */

	if (fileExists (filename) && readFromFile (gwdata, gdata, filename, &bit, x, NULL)) {
		char	fmt_mask[80];
		double	pct;
		pct = trunc_percent (bit * 100.0 / Nlen);
		sprintf (fmt_mask,
			 "Resuming divisibility test of %%d^(N-1)-1 at bit %%ld [%%.%df%%%%]\n",
			 PRECISION);
		sprintf (buf, fmt_mask, a, bit, pct);
//		OutputStr (buf);
//		if (verbose || restarting)
//			writeResults (buf);
	}

/* Otherwise, output a message indicating we are starting test */

	else {
		clear_timers ();	// Init. timers
		sprintf (buf, "Starting divisibility test of %lu^(N-1)-1\n", a);
//		OutputStr (buf);
//		if (verbose || restarting)
//			writeResults (buf);
		bit = 1;
		dbltogw (gwdata, (double) a, x);
	}

/* Get the current time */

	start_timer (0);
	start_timer (1);
	time (&start_time);

/* Output a message about the FFT length */

	topology_print_children (hwloc_get_root_obj (hwloc_topology), 0);
	gwfft_description (gwdata, fft_desc);
	sprintf (buf, "Using %s\n", fft_desc);

//	OutputStr (buf);
//	if (verbose || restarting) {
//		writeResults (buf);
//	}
	ReplaceableLine (1);	/* Remember where replaceable line is */

/* Init the title */

	title ("Divisibility test in progress...");

/* Do the PRP test */

	gwsetmulbyconst (gwdata, a);
	iters = 0;
	while (bit < Nlen) {

/* Error check the last 50 iterations, before writing an */
/* intermediate file (either user-requested stop or a */
/* 30 minute interval expired), and every 128th iteration. */

		stopping = stopCheck ();
		echk = stopping || ERRCHK || (bit <= 50) || (bit >= Nlen-50) || gwnear_fft_limit (gwdata, pcfftlim);
		if (((bit & 127) == 0) || (bit == 1) || (bit == (lasterr_point-1))) {
			echk = 1;
			time (&current_time);
			saving = ((current_time - start_time > write_time) || (bit == 1) || (bit == (lasterr_point-1)));
		} else
			saving = 0;

/* Process this bit */

		gwstartnextfft (gwdata, postfft && !debug && !stopping && !saving && bit != lasterr_point && (bit+26 < Nlen) && (bit > 26) && !maxerr_recovery_mode[6]);

		bitpos = Nlen-bit-1;
		if (bitval (N, bitpos)) {
			gwsetnormroutine (gwdata, 0, echk, 1);
		} else {
			gwsetnormroutine (gwdata, 0, echk, 0);
		}

		if ((bit+25 < Nlen) && (bit > 25) && ((bit != lasterr_point) || !maxerr_recovery_mode[6])) {
			gwsquare (gwdata, x);
			care = FALSE;
		}
		else {
			gwsquare_carefully (gwdata, x);
			care = TRUE;
		}

		if (debug && (bit < 50)) {
			gwtogiant (gwdata,x, tmp);		// The modulo reduction is done here
			if (abs(tmp->sign) < 2)			// make a 32 bit residue correct !!
				sprintf (buf, 
				 "%lu^(N-1)-1 interim residue %08lX%08lX at bit %lu\n",
				 a, (unsigned long)0, (unsigned long)tmp->n[0], bit);
			else
				sprintf (buf, 
				 "%lu^(N-1)-1 interim residue %08lX%08lX at bit %lu\n",
				 a, (unsigned long)tmp->n[1], (unsigned long)tmp->n[0], bit);
			OutputBoth (buf);
		}
		CHECK_IF_ANY_ERROR (x, (bit), Nlen, 6);

/* That iteration succeeded, bump counters */

		if (bit == lasterr_point)
			saving = 1;					// Be sure to restart after this recovery iteration!
		bit++;
		iters++;

/* Print a message every so often */

		if (bit % ITER_OUTPUT == 0) {
			char	fmt_mask[80];
			double	pct;
			pct = trunc_percent (bit * 100.0 / Nlen);
			sprintf (fmt_mask, "%%.%df%%%% of %%ld", PRECISION);
			sprintf (buf, fmt_mask, pct, Nlen);
			title (buf);
			ReplaceableLine (2);	/* Replace line */
			sprintf (fmt_mask,
				 "%%d^(N-1)-1, bit: %%ld / %%ld [%%.%df%%%%]",
				 PRECISION);
			sprintf (buf, fmt_mask, a, bit, Nlen, pct);
			OutputStr (buf);
			if (ERRCHK && bit > 30) {
				OutputStr (".  Round off: ");
				sprintf (buf, "%10.10f", reallyminerr);
				OutputStr (buf);
				sprintf (buf, " to %10.10f", reallymaxerr);
				OutputStr (buf);
			}
			end_timer (0);
			if (CUMULATIVE_TIMING) {
				OutputStr (".  Time thusfar: ");
			} else {
				OutputStr (".  Time per bit: ");
				divide_timer (0, iters);
				iters = 0;
			}
			print_timer (0, TIMER_NL | TIMER_OPT_CLR);
			start_timer (0);
		}

/* Print a results file message every so often */

		if (bit % ITER_OUTPUT_RES == 0 || (NO_GUI && stopping)) {
			sprintf (buf, "Bit %ld / %ld\n", bit, Nlen);
			writeResults (buf);
		}

/* Write results to a file every DISK_WRITE_TIME minutes */
/* On error, retry in 10 minutes (it could be a temporary */
/* disk-full situation) */

		if (saving || stopping) {
			write_time = DISK_WRITE_TIME * 60;
			saving = FALSE;
			if (! writeToFile (gwdata, gdata, filename, bit, x, NULL)) {
				sprintf (buf, WRITEFILEERR, filename);
				OutputBoth (buf);
				if (write_time > 600) write_time = 600;
			}	
			time (&start_time);

/* If an escape key was hit, write out the results and return */

			if (stopping) {
				iaddg (1, N);	// Restore the modulus
				pushg (gdata, 1);
				gwfree (gwdata, x);
				*res = FALSE;
				return (FALSE);
			}
		}

/* Output the 64-bit residue at specified interims.  Also output the */
/* residues for the next iteration so that we can compare our */
/* residues to programs that start counter at zero or one. */

		if (interimResidues && bit % interimResidues < 2) {
				gwtogiant (gwdata,x, tmp);			// The modulo reduction is done here
			if (abs(tmp->sign) < 2)			// make a 32 bit residue correct !!
				sprintf (buf, 
				 "%lu^(N-1)-1 interim residue %08lX%08lX at bit %lu\n",
				 a, (unsigned long)0, (unsigned long)tmp->n[0], bit);
			else
				sprintf (buf, 
				 "%lu^(N-1)-1 interim residue %08lX%08lX at bit %lu\n",
				 a, (unsigned long)tmp->n[1], (unsigned long)tmp->n[0], bit);
			OutputBoth (buf);
		}

/* Write a save file every "interimFiles" iterations. */

		if (interimFiles && bit % interimFiles == 0) {
			char	interimfile[20];
			sprintf (interimfile, "%.8s.%03lu",
				 filename, bit / interimFiles);
			if (! writeToFile (gwdata, gdata, interimfile, bit, x, NULL)) {
				sprintf (buf, WRITEFILEERR, interimfile);
				OutputBoth (buf);
			}
		}
	}

/* See if the residue is one.  If not, format a 64-bit residue. */
/* Old versions of PRP used a non-standard 64-bit residue, computing */
/* a^N-a mod N rather than the more standard a^(N-1) mod N.  Since */
/* some projects recorded these non-standard residues, output that */
/* residue too.  Note that some really old versions printed out the */
/* 32-bit chunks of the non-standard residue in reverse order. */

	clearline (100);

	gwtogiant (gwdata, x, tmp);
	if (!isone (tmp)) {
		*res = FALSE;	/* Residue not one */
		if (abs(tmp->sign) < 2)	// make a 32 bit residue correct !!
			sprintf (res64, "%08lX%08lX", (unsigned long)0, (unsigned long)tmp->n[0]);
		else
			sprintf (res64, "%08lX%08lX", (unsigned long)tmp->n[1], (unsigned long)tmp->n[0]);
		imulg (a, tmp); specialmodg (gwdata, tmp); ulsubg (a, tmp);
		if (abs(tmp->sign) < 2)	// make a 32 bit residue correct !!
			sprintf (oldres64, "%08lX%08lX", (unsigned long)0, (unsigned long)tmp->n[0]);
		else
			sprintf (oldres64, "%08lX%08lX", (unsigned long)tmp->n[1], (unsigned long)tmp->n[0]);
	}

/* Print results.  Do not change the format of this line as Jim Fougeron of */
/* PFGW fame automates his QA scripts by parsing this line. */

	sprintf (buf, "End of divisibility test of %lu^(N-1)-1\n", a);

	pushg (gdata, 1);
	gwfree (gwdata, x);

/* Output the final timings */

	end_timer (1);
	sprintf (buf+strlen(buf)-1, "  Time: ");
	ReplaceableLine (2);	/* Replace line */
	write_timer (buf+strlen(buf), 1, TIMER_CLR | TIMER_NL); 
//	if (verbose)
//		OutputBoth (buf);
//	else
//		OutputStr (buf);

/* Cleanup and return */

	iaddg (1, N);					// Restore the modulus
	Nlen = bitlen (N);
	_unlink (filename);
	lasterr_point = 0;				// Reset the last error point.
	return (TRUE);

/* An error occured, sleep if required, then try restarting at last save point. */

error:
	iaddg (1, N);					// Restore the value of N
	Nlen = bitlen (N);
	pushg (gdata, 1);
	gwfree (gwdata, x);
	*res = FALSE;					// To avoid credit message...

	if ((abonillsum && gw_test_illegal_sumout(gwdata)) || 
		(abonmismatch && gw_test_mismatched_sums (gwdata)) || 
		(abonroundoff && gw_get_maxerr(gwdata) > maxroundoff)) {	// Abort...
		aborted = TRUE;
		sprintf (buf,ERRMSG7,a);
		OutputBoth (buf);
		_unlink (filename);
		if(IniGetInt(INI_FILE, "StopOnAbort", 0)) {
			IniWriteInt (INI_FILE, "PgenLine", IniGetInt(INI_FILE, "PgenLine", 0) + 1);	// Point on the next line
			return (FALSE);
		}
		else
			return (TRUE);
	}

/* Output a message saying we are restarting */

	if (sleep5) OutputBoth (ERRMSG2);
	OutputBoth (ERRMSG3);

/* Sleep five minutes before restarting */

	if (sleep5 && ! SleepFive ()) {
		return (FALSE);
	}

/* Restart */

	if (will_try_larger_fft) {
		IniWriteInt(INI_FILE, "FFT_Increment", nbfftinc = (IniGetInt(INI_FILE, "FFT_Increment", 0) + 1));
		if (nbfftinc == maxfftinc)
			abonroundoff = TRUE;	// Don't accept any more Roundoff error.
	}
	return (-1);
}

/* Test for an N+1 Frobenius probable prime -- gwsetup has already been called. */

int commonFrobeniusPRP (
	gwhandle *gwdata,
	ghandle *gdata,
	long P,
	long Q,
	int	*res, char *str)
{
	unsigned long bit, firstone = 0, iters, D, bitv;
	gwnum x, y, gwA, gw2, gwQ;
	giant	tmp, tmp2, tmp3, A;
//	struct	gwasm_data *asm_data;
	char	filename[20], buf[sgkbufsize+256], fft_desc[256];
	long	write_time = DISK_WRITE_TIME * 60;
	int	echk, saving, stopping;
	time_t	start_time, current_time;
	double	reallyminerr = 1.0;
	double	reallymaxerr = 0.0;

	if (!IniGetInt(INI_FILE, "Testdiff", 0))	// Unless test of MAXDIFF explicitly required
		gwdata->MAXDIFF = 1.0E80;				// Diregard MAXDIFF...

/* Allocate memory */

	x = gwalloc (gwdata);
	y = gwalloc (gwdata);
	gwA = gwalloc (gwdata);
	gw2 = gwalloc (gwdata);
	gwQ = gwalloc (gwdata);
	
	Nlen = bitlen (N);
	tmp = popg (gdata, (Nlen >> 3) + 3);
	tmp2 = popg (gdata, (Nlen >> 3) + 3);
	tmp3 = popg (gdata, (Nlen >> 3) + 3);
	A = popg (gdata, (Nlen >> 3) + 3);

	D = P*P - 4*Q;

//	Compute needed large integer and gwnum constants

	dbltogw (gwdata, 2.0, gw2);
	itog (Q, tmp);		// Compute gwQ for Frobenius test
	if (Q < 0)
		addg (N, tmp);	// only a positive giant can be converted to gwnum
	gianttogw (gwdata, tmp, gwQ);
	gtog (tmp, A);		// Compute A = P*P*Q^-1 - 2 mod N
	invg (N, A);
	ulmulg (P*P, A);
	ulsubg (2, A);
	modg (N, A);
	gianttogw (gwdata, A, gwA);


	*res = TRUE;		/* Assume it is a probable prime */

/* Init file name */

	tempFileName (filename, 'F', N);

/* Optionally resume from save file and output a message */
/* indicating we are resuming a Frobenius test */

	if (fileExists (filename) && readFromFile (gwdata, gdata, filename, &bit, x, y)) {
		char	fmt_mask[80];
		double	pct;
		pct = trunc_percent (bit * 100.0 / Nlen);
		sprintf (fmt_mask,
			 "Resuming Frobenius sequence at bit %%ld [%%.%df%%%%]\n",
			 PRECISION);
		sprintf (buf, fmt_mask, bit, pct);
		start_timer (0);
		start_timer (1);
		goto Frobeniusresume;
	}
	else {

/* Init, compute (N+1)/2 to compute x and y mod N */

		gtog (N, tmp3);
		iaddg (1, tmp3);
		gshiftright (1, tmp3);
		Nlen = bitlen (tmp3);

		tempFileName (filename, 'L', N);	// Reinit file name

/* Optionally resume from save file and output a message */
/* indicating we are resuming a Lucas test */

		if (fileExists (filename) && readFromFile (gwdata, gdata, filename, &bit, x, y)) {
			char	fmt_mask[80];
			double	pct;
			pct = trunc_percent (bit * 100.0 / Nlen);
			sprintf (fmt_mask,
				"Resuming Lucas sequence at bit %%ld [%%.%df%%%%]\n",
				PRECISION);
			sprintf (buf, fmt_mask, bit, pct);
			OutputStr (buf);
			if (verbose || restarting)
				writeResults (buf);
		}

/* Otherwise, output a message indicating we are starting a Lucas test */

		else {
//			clear_timers ();	// Init. timers
			sprintf (buf, "Starting Lucas sequence\n");
			OutputStr (buf);
			if (verbose || restarting)
				writeResults (buf);

			bit = 1;
			gwcopy (gwdata, gw2, x);		// Initial values
			gwcopy (gwdata, gwA, y);
		}
	}


/* Get the current time */

	start_timer (0);
	start_timer (1);
	time (&start_time);

/* Output a message about the FFT length */

	topology_print_children (hwloc_get_root_obj (hwloc_topology), 0);
	gwfft_description (gwdata, fft_desc);
	sprintf (buf, "Using %s, P = %ld, Q = %ld\n", fft_desc, P, Q);

	OutputStr (buf);
	if (verbose || restarting) {
		writeResults (buf);
	}
	ReplaceableLine (1);	/* Remember where replaceable line is */

/* Init the title */

	title ("Lucas PRP test in progress...");

/* Do the PRP test */

	iters = 0;
	while (bit <= Nlen) {

/* Error check the first and last 50 iterations, before writing an */
/* intermediate file (either user-requested stop or a */
/* 30 minute interval expired), and every 128th iteration. */

		stopping = stopCheck ();
		echk = stopping || ERRCHK || (bit <= 50) || (bit >= Nlen-50) || gwnear_fft_limit (gwdata, pcfftlim);
		if (((bit & 127) == 0) || (bit == 1) || (bit == (lasterr_point-1))) {
			echk = 1;
			time (&current_time);
			saving = ((current_time - start_time > write_time) || (bit == 1) || (bit == (lasterr_point-1)));
		} else
			saving = 0;

/* Process this bit */

		gwsetnormroutine (gwdata, 0, echk, 0);
		gwstartnextfft (gwdata, FALSE);

		if ((bitv = bitval (tmp3, Nlen-bit))) {
			if (abs(gwdata->c)==1)				// Warning : gwsetaddin must not be used if abs(c) != 1 !!
				gwsetaddin (gwdata, 0);
			if ((bit+26 < Nlen) && (bit > 26) &&
				((bit != lasterr_point) || !maxerr_recovery_mode[1])) {
				gwsafemul (gwdata, y, x);
				care = FALSE;
			}
			else {
				gwmul_carefully (gwdata, y, x);
				care = TRUE;
			}
			if (debug && (bit < 50))
				writeresidue (gwdata, x, N, tmp, buf, str, bit, BIT);
			CHECK_IF_ANY_ERROR(x, (bit), Nlen, 1)
			gwsub3 (gwdata, x, gwA, x);
			if (abs(gwdata->c)==1)
				gwsetaddin (gwdata, -2);		// Warning : gwsetaddin must not be used if abs(c) != 1 !!
			if ((bit+26 < Nlen) && (bit > 26) &&
				((bit != lasterr_point) || !maxerr_recovery_mode[2])) {
				gwsquare (gwdata, y);
				care = FALSE;
			}
			else {
				gwsquare_carefully (gwdata, y);
				care = TRUE;
			}
			if (debug && (bit < 50))
				writeresidue (gwdata, y, N, tmp, buf, str, bit, BIT);
			CHECK_IF_ANY_ERROR(y, (bit), Nlen, 2)
			if (abs(gwdata->c)!=1)
				gwsubquick (gwdata, gw2, y);	// gwsetaddin has not been used because abs(c) != 1 !!
		}
		else {
			if (abs(gwdata->c)==1)
				gwsetaddin (gwdata, 0);			// Warning : gwsetaddin must not be used if abs(c) != 1 !!
			if ((bit+26 < Nlen) && (bit > 26) &&
				((bit != lasterr_point) || !maxerr_recovery_mode[3])) {
				gwsafemul (gwdata, x, y);
				care = FALSE;
			}
			else {
				gwmul_carefully (gwdata, x, y);
				care = TRUE;
			}
			if (debug && (bit < 50))
				writeresidue (gwdata, y, N, tmp, buf, str, bit, BIT);
			CHECK_IF_ANY_ERROR(y, (bit), Nlen, 3)
			gwsub3 (gwdata, y, gwA, y);
			if (abs(gwdata->c)==1)
				gwsetaddin (gwdata, -2);		// Warning : gwsetaddin must not be used if abs(c) != 1 !!
			if ((bit+26 < Nlen) && (bit > 26) &&
				((bit != lasterr_point) || !maxerr_recovery_mode[4])) {
				gwsquare (gwdata, x);
				care = FALSE;
			}
			else {
				gwsquare_carefully (gwdata, x);
				care = TRUE;
			}
			if (debug && (bit < 50))
				writeresidue (gwdata, x, N, tmp, buf, str, bit, BIT);
			CHECK_IF_ANY_ERROR(x, (bit), Nlen, 4)
			if (abs(gwdata->c)!=1)
				gwsubquick (gwdata, gw2, x);	// gwsetaddin has not been used because abs(c) != 1 !!
		}

 /* That iteration succeeded, bump counters */

		if (bit == lasterr_point)
			saving = 1;					// Be sure to restart after this recovery iteration!
		bit++;
		iters++;

/* Print a message every so often */

		if (bit % ITER_OUTPUT == 0) {
			char	fmt_mask[80];
			double	pct;
			pct = trunc_percent (bit * 100.0 / Nlen);
			sprintf (fmt_mask, "%%.%df%%%% of %%ld", PRECISION);
			sprintf (buf, fmt_mask, pct, Nlen);
			title (buf);
			ReplaceableLine (2);	/* Replace line */
			sprintf (fmt_mask,
				 "%%s, bit: %%ld / %%ld [%%.%df%%%%]",
				 PRECISION);
			sprintf (buf, fmt_mask, str, bit, Nlen, pct);
			OutputStr (buf);
			if (ERRCHK && bit > 30) {
				OutputStr (".  Round off: ");
				sprintf (buf, "%10.10f", reallyminerr);
				OutputStr (buf);
				sprintf (buf, " to %10.10f", reallymaxerr);
				OutputStr (buf);
			}
			end_timer (0);
			if (CUMULATIVE_TIMING) {
				OutputStr (".  Time thusfar: ");
			} else {
				OutputStr (".  Time per bit: ");
				divide_timer (0, iters);
				iters = 0;
			}
			print_timer (0, TIMER_NL | TIMER_OPT_CLR);
			start_timer (0);
		}

/* Print a results file message every so often */

		if (bit % ITER_OUTPUT_RES == 0 || (NO_GUI && stopping)) {
			sprintf (buf, "Bit %ld / %ld\n", bit, Nlen);
			writeResults (buf);
		}

/* Write results to a file every DISK_WRITE_TIME minutes */
/* On error, retry in 10 minutes (it could be a temporary */
/* disk-full situation) */

		if (saving || stopping) {
			write_time = DISK_WRITE_TIME * 60;
			saving = FALSE;
			if (! writeToFile (gwdata, gdata, filename, bit, x, y)) {
				sprintf (buf, WRITEFILEERR, filename);
				OutputBoth (buf);
				if (write_time > 600) write_time = 600;
			}	
			time (&start_time);

/* If an escape key was hit, write out the results and return */

			if (stopping) {
				pushg (gdata, 3);
				gwfree (gwdata, x);				// Clean up
				gwfree (gwdata, y);
				gwfree (gwdata, gwA);
				gwfree (gwdata, gw2);
				gwfree (gwdata, gwQ);
				*res = FALSE;
				return (FALSE);
			}
		}

/* Output the 64-bit residue at specified interims.  Also output the */
/* residues for the next iteration so that we can compare our */
/* residues to programs that start counter at zero or one. */

		if (interimResidues && bit % interimResidues < 2)
			writeresidue (gwdata, x, N, tmp, buf, str, bit, BIT);

/* Write a save file every "interimFiles" iterations. */

		if (interimFiles && bit % interimFiles == 0) {
			char	interimfile[20];
			sprintf (interimfile, "%.8s.%03lu",
				 filename, bit / interimFiles);
			if (! writeToFile (gwdata, gdata, interimfile, bit, x, y)) {
				sprintf (buf, WRITEFILEERR, interimfile);
				OutputBoth (buf);
			}
		}
	}

/* See if we've found a Lucas probable prime.  If not, format a 64-bit residue. */

	clearline (100);
	lasterr_point = 0;				// Reset last error point.

	gwtogiant (gwdata, x, tmp);		// V(m)
	gwtogiant (gwdata, y, tmp2);	// V(m+1)
	mulg (A, tmp);					// A*V(m)
	gshiftleft (1, tmp2);			// 2*V(m+1)
	subg (tmp2, tmp);				// A*V(m)-2*V(m+1)
	modg (N, tmp);					// External modulus and gwnum's one may be different...

	if (!isZero (tmp)) {
		*res = FALSE;				// N is composite.
		if (abs(tmp->sign) < 2)		// make a 32 bit residue correct !!
			sprintf (res64, "%08lX%08lX", (unsigned long)0, (unsigned long)tmp->n[0]);
		else
			sprintf (res64, "%08lX%08lX", (unsigned long)tmp->n[1], (unsigned long)tmp->n[0]);

		if (IniGetInt(INI_FILE, "LucasPRPtest", 0))
			if (bpsw)
				sprintf (buf, "%s is not prime(P = %ld, Q = %ld), BPSW RES64: %s", str, P, Q, res64);
			else
				sprintf (buf, "%s is not prime(P = %ld, Q = %ld), Lucas RES64: %s", str, P, Q, res64);
		else
			if (bpsw)
				sprintf (buf, "%s is Fermat PSP, but composite!! (P = %ld, Q = %ld), BPSW RES64: %s", str, P, Q, res64);
			else
				sprintf (buf, "%s is Fermat PSP, but composite!! (P = %ld, Q = %ld), Lucas RES64: %s", str, P, Q, res64);
	}

	if (*res) {						// N may be prime ; do now the Frobenius PRP test
		_unlink (filename);			// Remove Lucas save file
		tempFileName (filename, 'F', N);	// Reinit file name
		bit = 1;
		gwcopy (gwdata, gwQ, y);	// Init : y = Q
		if (IniGetInt(INI_FILE, "LucasPRPtest", 0))
			if (bpsw)
				sprintf (buf, "%s is BPSW PRP, Starting Frobenius test sequence\n", str);
			else
				sprintf (buf, "%s is Lucas PRP, Starting Frobenius test sequence\n", str);
		else
			if (bpsw)
				sprintf (buf, "%s is Fermat and BPSW PRP, Starting Frobenius test sequence\n", str);
			else
				sprintf (buf, "%s is Fermat and Lucas PRP, Starting Frobenius test sequence\n", str);
		if (! writeToFile (gwdata, gdata, filename, bit, x, y)) {
			sprintf (buf, WRITEFILEERR, filename);
			OutputBoth (buf);
		}			// Make a save file to avoid a false restart after a roundoff error...

Frobeniusresume:

	clearline (100);

		OutputStr (buf);
		if (verbose || restarting)
			writeResults (buf);
		gtog (N, tmp3);				// compute (N-1)/2 to compute y mod N
		ulsubg (1, tmp3);
		gshiftright (1, tmp3);
		Nlen = bitlen (tmp3);

/* Get the current time */

		time (&start_time);

/* Output a message about the FFT length */

		topology_print_children (hwloc_get_root_obj (hwloc_topology), 0);
		gwfft_description (gwdata, fft_desc);
		sprintf (buf, "Using %s, Q = %ld\n", fft_desc, Q);

		OutputStr (buf);
		if (verbose || restarting) {
			writeResults (buf);
		}
		ReplaceableLine (1);	/* Remember where replaceable line is */

/* Init the title */

		title ("Frobenius PRP test in progress...");

/* Do the PRP test */
		
		if (abs(gwdata->c)==1)
			gwsetaddin (gwdata, 0);			// Warning : gwsetaddin must not be used if abs(c) != 1 !!
		if (abs(Q) <= GWMULBYCONST_MAX)
			gwsetmulbyconst (gwdata, Q);
		else
			gwsetmulbyconst (gwdata, 1);	// mulbyconst does not work if the constant is too big...
		iters = 0;
		gwstartnextfft (gwdata, FALSE);
		while (bit < Nlen) {

/* Error check the first and last 50 iterations, before writing an */
/* intermediate file (either user-requested stop or a */
/* 30 minute interval expired), and every 128th iteration. */

			stopping = stopCheck ();
			echk = stopping || ERRCHK || (bit <= 50) || (bit >= Nlen-50) || gwnear_fft_limit (gwdata, pcfftlim);
			if (((bit & 127) == 0) || (bit == 1) || (bit == (lasterr_point-1))) {
				echk = 1;
				time (&current_time);
				saving = ((current_time - start_time > write_time) || (bit == 1) || (bit == (lasterr_point-1)));
			} else
				saving = 0;

/* Process this bit */

//			gwstartnextfft (gwdata, postfft && !debug && !stopping && !saving && bit != lasterr_point && (bit+26 < Nlen) && (bit > 26) && !maxerr_recovery_mode[6]);

			if ((bitv = bitval (tmp3, Nlen-bit-1))) {
				gwsetnormroutine (gwdata, 0, echk, 1);
			} else {
				gwsetnormroutine (gwdata, 0, echk, 0);
			}
			if ((bit+25 < Nlen) && (bit > 25) && ((bit != lasterr_point) || !maxerr_recovery_mode[6])) {
				gwsquare (gwdata, y);
				care = FALSE;
			}
			else {
				gwsquare_carefully (gwdata, y);
				care = TRUE;
			}

			if (debug && (bit < 50))
				writeresidue (gwdata, y, N, tmp, buf, str, bit, BIT);
			CHECK_IF_ANY_ERROR (y, (bit), Nlen, 6);

			if (bitv && (abs(Q) > GWMULBYCONST_MAX)) {
				gwsafemul (gwdata, gwQ, y);		// multiply by Q if abs(Q) is too big...
				care = FALSE;
			}
/* That iteration succeeded, bump counters */

			if (bit == lasterr_point)
				saving = 1;					// Be sure to restart after this recovery iteration!
			bit++;
			iters++;

/* Print a message every so often */

			if (bit % ITER_OUTPUT == 0) {
				char	fmt_mask[80];
				double	pct;
				pct = trunc_percent (bit * 100.0 / Nlen);
				sprintf (fmt_mask, "%%.%df%%%% of %%ld", PRECISION);
				sprintf (buf, fmt_mask, pct, Nlen);
				title (buf);
				ReplaceableLine (2);	/* Replace line */
				sprintf (fmt_mask,
					 "%%s, bit: %%ld / %%ld [%%.%df%%%%]",
					 PRECISION);
				sprintf (buf, fmt_mask, str, bit, Nlen, pct);
				OutputStr (buf);
				if (ERRCHK && bit > 30) {
					OutputStr (".  Round off: ");
					sprintf (buf, "%10.10f", reallyminerr);
					OutputStr (buf);
					sprintf (buf, " to %10.10f", reallymaxerr);
					OutputStr (buf);
				}
				end_timer (0);
				if (CUMULATIVE_TIMING) {
					OutputStr (".  Time thusfar: ");
				} else {
					OutputStr (".  Time per bit: ");
					divide_timer (0, iters);
					iters = 0;
				}
				print_timer (0, TIMER_NL | TIMER_OPT_CLR);
				start_timer (0);
			}

/* Print a results file message every so often */

			if (bit % ITER_OUTPUT_RES == 0 || (NO_GUI && stopping)) {
				sprintf (buf, "Bit %ld / %ld\n", bit, Nlen);
				writeResults (buf);
			}

/* Write results to a file every DISK_WRITE_TIME minutes */
/* On error, retry in 10 minutes (it could be a temporary */
/* disk-full situation) */

			if (saving || stopping) {
				write_time = DISK_WRITE_TIME * 60;
				saving = FALSE;
				if (! writeToFile (gwdata, gdata, filename, bit, x, y)) {
					sprintf (buf, WRITEFILEERR, filename);
					OutputBoth (buf);
					if (write_time > 600) write_time = 600;
				}	
				time (&start_time);

/* If an escape key was hit, write out the results and return */

				if (stopping) {
					pushg (gdata, 4);
					gwfree (gwdata, x);
					gwfree (gwdata, y);
					gwfree (gwdata, gwA);
					gwfree (gwdata, gw2);
					gwfree (gwdata, gwQ);
					*res = FALSE;
					return (FALSE);
				}
			}

/* Output the 64-bit residue at specified interims.  Also output the */
/* residues for the next iteration so that we can compare our */
/* residues to programs that start counter at zero or one. */

			if (interimResidues && bit % interimResidues < 2)
				writeresidue (gwdata, y, N, tmp, buf, str, bit, BIT);

/* Write a save file every "interimFiles" iterations. */

			if (interimFiles && bit % interimFiles == 0) {
				char	interimfile[20];
				sprintf (interimfile, "%.8s.%03lu",
				 filename, bit / interimFiles);
				if (! writeToFile (gwdata, gdata, interimfile, bit, x, y)) {
					sprintf (buf, WRITEFILEERR, interimfile);
					OutputBoth (buf);
				}
			}
		}
		gwsetnormroutine (gwdata, 0, 1, 0);
		gwmul_carefully (gwdata, x, y);	// y = B*V(m)
		care = TRUE;
//		gwmul (gwdata, x, y);			// y = B*V(m) ; gwmul not reliable enough here...
		CHECK_IF_ANY_ERROR (y, (Nlen), Nlen, 6);
		gwsubquick (gwdata, gw2, y);
		gwtogiant (gwdata, y, tmp);
		modg (N, tmp);					// External modulus and gwnum's one may be different...
		if (!isZero (tmp)) {
			*res = FALSE;				// N is Lucas PSP, but composite!!
			if (abs(tmp->sign) < 2)		// make a 32 bit residue correct !!
				sprintf (res64, "%08lX%08lX", (unsigned long)0, (unsigned long)tmp->n[0]);
			else
				sprintf (res64, "%08lX%08lX", (unsigned long)tmp->n[1], (unsigned long)tmp->n[0]);

			if (IniGetInt(INI_FILE, "LucasPRPtest", 0))
				if (bpsw)
					sprintf (buf, "%s is BPSW PSP (P = %ld, Q = %ld), but composite!!. Frobenius RES64: %s", str, P, Q, res64);
				else
					sprintf (buf, "%s is Lucas PSP (P = %ld, Q = %ld), but composite!!. Frobenius RES64: %s", str, P, Q, res64);
			else
				if (bpsw)
					sprintf (buf, "%s is Fermat and BPSW PSP (P = %ld, Q = %ld), but composite!!. Frobenius RES64: %s", str, P, Q, res64);
				else
					sprintf (buf, "%s is Fermat and Lucas PSP (P = %ld, Q = %ld), but composite!!. Frobenius RES64: %s", str, P, Q, res64);
		}
	}

/* Print results.  */

	clearline (100);

	if (*res) {
		if (IniGetInt(INI_FILE, "LucasPRPtest", 0)) {
			if (bpsw)
				sprintf (buf, "%s is BPSW and Frobenius PRP! (P = %ld, Q = %ld, D = %ld)", str, P, Q, D);
			else
				sprintf (buf, "%s is Lucas and Frobenius PRP! (P = %ld, Q = %ld, D = %ld)", str, P, Q, D);
		}
		else {
			if (bpsw)
				sprintf (buf, "%s is Fermat, BPSW and Frobenius PRP! (P = %ld, Q = %ld, D = %ld)", str, P, Q, D);
			else
				sprintf (buf, "%s is Fermat, Lucas and Frobenius PRP! (P = %ld, Q = %ld, D = %ld)", str, P, Q, D);
		}
	}

	pushg (gdata, 4);
	gwfree (gwdata, x);				// Clean up
	gwfree (gwdata, y);
	gwfree (gwdata, gwA);
	gwfree (gwdata, gw2);
	gwfree (gwdata, gwQ);

/* Update the output file */

	if ((*res && IniGetInt (INI_FILE, "OutputPrimes", 0)) ||
	    (!*res && IniGetInt (INI_FILE, "OutputComposites", 0)))
		writeResults (buf);


#if defined(WIN32) && !defined(_CONSOLE)

	sprintf (buf+strlen(buf), "  Time : "); 
	ReplaceableLine (2);	/* Replace line */ 

#else

	clearline(100);

	if (*res) {
#if defined (_CONSOLE)
		hConsole = GetStdHandle(STD_OUTPUT_HANDLE);	// Access to Console attributes
		SetConsoleTextAttribute(hConsole, BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
		OutputBoth(buf);
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
#else
		OutputStr("\033[7m");
		OutputBoth(buf);
		OutputStr("\033[0m");
#endif
	}
	else
		OutputBoth(buf);

	sprintf (buf, "  Time : "); 

#endif

/* Output the final timings */

	end_timer (1);
	write_timer (buf+strlen(buf), 1, TIMER_CLR | TIMER_NL); 
	if ((*res && IniGetInt (INI_FILE, "OutputPrimes", 0)) ||
	    (!*res && IniGetInt (INI_FILE, "OutputComposites", 0)))
		OutputStr (buf);
	else
		OutputBoth (buf);

/* Cleanup and return */

	Nlen = bitlen (N);
	_unlink (filename);
	lasterr_point = 0;
	return (TRUE);

/* An error occured, sleep if required, then try restarting at last save point. */

error:
	Nlen = bitlen (N);
	pushg (gdata, 4);
	gwfree (gwdata, x);				// Clean up
	gwfree (gwdata, y);
	gwfree (gwdata, gwA);
	gwfree (gwdata, gw2);
	gwfree (gwdata, gwQ);
	*res = FALSE;

	if ((abonillsum && gw_test_illegal_sumout(gwdata)) || 
		(abonmismatch && gw_test_mismatched_sums (gwdata)) || 
		(abonroundoff && gw_get_maxerr(gwdata) > maxroundoff)) {	// Abort...
		aborted = TRUE;
		sprintf (buf, ERRMSG5, checknumber, str);
		OutputBoth (buf);
		_unlink (filename);
		if(IniGetInt(INI_FILE, "StopOnAbort", 0)) {
			IniWriteInt (INI_FILE, "PgenLine", IniGetInt(INI_FILE, "PgenLine", 0) + 1);	// Point on the next line
			return (FALSE);
		}
		else
			return (TRUE);
	}


/* Output a message saying we are restarting */

	if (sleep5) OutputBoth (ERRMSG2);
	OutputBoth (ERRMSG3);

/* Sleep five minutes before restarting */

	if (sleep5 && ! SleepFive ()) {
		return (FALSE);
	}

/* Restart */

	if (will_try_larger_fft) {
		IniWriteInt(INI_FILE, "FFT_Increment", nbfftinc = (IniGetInt(INI_FILE, "FFT_Increment", 0) + 1));
		if (nbfftinc == maxfftinc)
			abonroundoff = TRUE;	// Don't accept any more Roundoff error.
	}
	return (-1);
}

/* This code is adapted from George Woltman's prp one in Version 29.8 of Prime95 */
/* Thanks to George's work, it was not difficult to implement it in LLR such as  */
/* the Gerbicz error checking is now available by default for all base two tests */


int GerbiczTest (
	gwhandle *gwdata,
	ghandle *gdata,
	unsigned long a,
	int	*res,
	char *str,
	struct work_unit *w)
{
	unsigned long explen, final_counter, iters, errchk, bit, init_units_bit = 0, init_alt_units_bit = 0; 
	unsigned long restart_error_count = 0;		/* On a restart, use this error count rather than the one from a save file */
	int	have_res2048;
	unsigned long final_counter_y, max_counter, loopshift = 0, last_counter = 0xFFFFFFFF;	/* Iteration of last error */
	long		restart_counter = -1;			/* On a restart, this specifies how far back to rollback save files */
	giant	exp, tmp, tmp2, tmp3; 
//	struct	program_state ps;
	int		interim_counter_off_one, interim_mul, mul_final, PositiveResult;
	int		first_iter_msg, echk, near_fft_limit, stopping; 
	double	inverse_explen;
	double	reallyminerr = 1.0; 
	double	reallymaxerr = 0.0; 
	double	allowable_maxerr, output_frequency, output_title_frequency;
	char	string_rep[80];
	char	filename[20], buf[sgkbufsize+256], 
		fft_desc[256], res2048[513]; 
	long	write_time = DISK_WRITE_TIME * 60; 
	int	string_rep_truncated;
	int	error_count_messages;
//	readSaveFileState read_save_file_state; /* Manage savefile names during reading */
	writeSaveFileState write_save_file_state; /* Manage savefile names during writing */

/* Init program state */

	memset (&ps, 0, sizeof (ps));
	if (w->prp_base)
		ps.prp_base = w->prp_base;
	else
		ps.prp_base = a;

	if (w->prp_residue_type)
		ps.residue_type = w->prp_residue_type;
	else
		ps.residue_type = IniGetInt (INI_FILE, "PRPResidueType", PRP_TYPE_COFACTOR);
	if (w->known_factors == NULL && ps.residue_type == PRP_TYPE_COFACTOR)
		ps.residue_type = PRP_TYPE_FERMAT;

/* Set flag if we will perform power-of-two optimizations.  These optimizations reduce the number of mul-by-small constants */
/* by computing a^(k*2^n) which gives us a long run of simple squarings.  These squarings let us do Gerbicz error checking. */

	ps.two_power_opt = (!IniGetInt (INI_FILE, "PRPStraightForward", 0) && w->b == 2 &&
			    (w->known_factors == NULL || ps.residue_type == PRP_TYPE_COFACTOR));

	interim_counter_off_one = (ps.two_power_opt && w->k == 1.0 && w->c < 0);
	interim_mul = (ps.two_power_opt && w->c < 0);


/* Flag the PRP tests that require multiplying the final a^exp to account for c */

	if (ps.two_power_opt && (ps.residue_type == PRP_TYPE_FERMAT || ps.residue_type == PRP_TYPE_COFACTOR))
		mul_final = w->c - 1;
	else if (ps.two_power_opt && ps.residue_type == PRP_TYPE_SPRP)
		mul_final = (w->c - 1) / 2;
	else
		mul_final = 0;

/* Determine what highly-reliable error-checking will be done (if any) */

	errchk = IniGetInt (INI_FILE, "ErrorChecking", 1);
	if (errchk == 0) ps.error_check_type = ERRCHK_NONE;
	if (errchk == 1) ps.error_check_type = ps.two_power_opt ? ERRCHK_GERBICZ : ERRCHK_NONE;
	if (errchk == 2) ps.error_check_type = ps.two_power_opt ? ERRCHK_GERBICZ : ERRCHK_DBLCHK;
	if (errchk == 3) ps.error_check_type = ERRCHK_DBLCHK;

/* Calculate the exponent we will use to do our left-to-right binary exponentiation */

	if (ps.residue_type == GMN_TYPE) {
		exp = allocgiant ((((unsigned long) (2*w->n * _log2 (w->b))) >> 4) + 5);
		tmp = allocgiant ((((unsigned long) (2*w->n * _log2 (w->b))) >> 4) + 5);
		tmp2 = allocgiant ((((unsigned long) (2*w->n * _log2 (w->b))) >> 4) + 5);
	}
	else {
		exp = allocgiant ((((unsigned long) (w->n * _log2 (w->b))) >> 4) + 5);
		tmp = allocgiant ((((unsigned long) (w->n * _log2 (w->b))) >> 4) + 5);
		tmp2 = allocgiant ((((unsigned long) (w->n * _log2 (w->b))) >> 4) + 5);
	}

/* As a small optimization, base 2 numbers are computed as a^(k*2^n) or a^(k*2^(n-1)) mod N with the final result */
/* multiplied by a^(c-1).  This eliminates tons of mul-by-consts at the expense of lots of bookkeepping headaches */
/* and one squaring if k=1 and c<0. */

	if (ps.two_power_opt) {
		int	gerbicz_squarings;
		if (ps.residue_type == PRP_TYPE_FERMAT ||
		    ps.residue_type == PRP_TYPE_FERMAT_VAR ||
		    ps.residue_type == PRP_TYPE_COFACTOR)
			gerbicz_squarings = w->n;
		else
			gerbicz_squarings = w->n - 1;
		ultog (2, exp);
		power (exp, gerbicz_squarings);
		if (w->k > 0.0)
			dblmulg (w->k, exp);
		else
			mulg (gk, exp);
	}

/* PRP co-factor test.  Do a PRP test on k*b^n+c rather than (k*b^n+c)/known_factors. */

	else if (ps.residue_type == PRP_TYPE_COFACTOR) {
		ultog (w->b, exp);
		power (exp, w->n);
		if (w->k > 0.0)
			dblmulg (w->k, exp);
		else
			mulg (gk, exp);
		iaddg (w->c, exp);
		iaddg (-1, exp);
	}

/* Standard PRP test.  Subtract 1 from N to compute a^(N-1) mod N */

	else {
		gtog (N, exp);
		iaddg (-1, exp);
	}

/* Get the exact bit length of the binary exponent.  We will perform bitlen(exp)-1 squarings for the PRP test. */


	explen = Nlen = bitlen (exp);
	final_counter = explen - 1;
	final_counter_y = (final_counter/2)-1;	// Warning : ps.counter starts from 0 !

/* Allocate memory */

	ps.x = gwalloc (gwdata);
	ps.y = gwalloc (gwdata);
	if (ps.error_check_type == ERRCHK_GERBICZ || ps.error_check_type == ERRCHK_DBLCHK)
		ps.alt_x = gwalloc (gwdata);
	if (ps.error_check_type == ERRCHK_GERBICZ) {
		ps.u0 = gwalloc (gwdata);
		ps.d = gwalloc (gwdata);
	}

/* Set the proper starting value and state if no save file was present */

	if (ps.counter == 0) {
		/* For base == 2 numbers, k == 1 and c == +1 or -1 we support FFT data shifting */
		if (w->b == 2 && (w->k == 1.0) && (abs(w->c) == 1) && w->n > 1000 && IniGetInt (INI_FILE, "Shifting", 1)) {
			// Generate a random initial shift count
			srand ((unsigned) time (NULL));
			init_units_bit = (rand () << 16) + rand ();
			if (CPU_FLAGS & CPU_RDTSC) { uint32_t hi,lo; rdtsc(&hi,&lo); init_units_bit += lo; }
			// Let user override random initial shift count
			init_units_bit = IniGetInt (INI_FILE, "InitialShiftCount", init_units_bit);
			// Initial shift count can't be larger than n-64 (the -64 avoids wraparound in setting intial value)
			init_units_bit = init_units_bit % ((ps.residue_type == GMN_TYPE)?2*(w->n - 64):(w->n - 64));
			tmp3 = popg (&gwdata->gdata, ((unsigned long) gwdata->bit_length >> 4) + 5);
			ultog (ps.prp_base, tmp3);
			gshiftleft (init_units_bit, tmp3);
			ps.units_bit = init_units_bit;
			gianttogw (gwdata, tmp3, ps.x);
			pushg (&gwdata->gdata, 1);
		} else {
			ps.units_bit = ps.alt_units_bit = init_units_bit = 0;
			dbltogw (gwdata, (double) ps.prp_base, ps.x);
		}

		gwcopy (gwdata, ps.x, ps.y);	// temporary...
		ps.units_bit2 = 0;

/* The easy state case is no high-reliability error-checking */

		if (ps.error_check_type == ERRCHK_NONE) {
			ps.state = STATE_NORMAL;
			ps.start_counter = 0;			// Value not used
			ps.end_counter = 0;				// Value not used
			ps.alt_units_bit = ps.units_bit; // JP
		}

/* The next easiest case is double-the-work error-checking comparing residues at specified intervals */

		else if (ps.error_check_type == ERRCHK_DBLCHK) {
			ps.state = STATE_DCHK_PASS1;
			ps.start_counter = 0;
			ps.end_counter = IniGetInt (INI_FILE, "DoublecheckCompareInterval", 100000);
			if (ps.end_counter > final_counter) ps.end_counter = final_counter;
			if (ps.units_bit == 0) {
				gwcopy (gwdata, ps.x, ps.alt_x);
				ps.alt_units_bit = 0;
			} else {
				gwadd3 (gwdata, ps.x, ps.x, ps.alt_x);
				ps.alt_units_bit = ps.units_bit + 1;
				if (ps.alt_units_bit >= ((ps.residue_type == GMN_TYPE)?2*(w->n):w->n)) {
					ps.alt_units_bit -= (ps.residue_type == GMN_TYPE)?2*(w->n):w->n;
				}
			}
		}

/* The final case of high-reliability error-checking is Gerbicz error checking, described below. */
/* See http://mersenneforum.org/showthread.php?t=22471 for background information. */
/* In a nutshell, if PRPing k*2^n+c we calculate (prp_base^k)^(2^n) which ends with a long string of squarings. */
/* Let u[0] = (prp_base^k), our n squarings are defined as:
	u[i]=u[0]^(2^i) mod mp, for i=0..n
   We define a "checksum" function below, which is updated every L-th squaring:
	d[t]=prod(i=0,t,u[L*i]) mod mp
   The key idea is that the checksum function can be calculated two different ways, thus the two can be compared to detect an error:
	d[t]=d[t-1]*u[L*t] mod mp		(checksum #1)
	d[t]=u[0]*d[t-1]^(2^L) mod mp		(checksum #2)
   The larger L we choose, the lower the error-checking overhead cost.  For L of 1000, we catch errors every 1 million iterations
   with an overhead of just 0.2%. */
/* For extra protection, we always keep two copies of the gwnum in memory.  If either one "goes bad" error checking will catch this. */

		else if (ps.error_check_type == ERRCHK_GERBICZ) {
			// Both STATE_DCHK_PASS1 and STATE_GERB_START_BLOCK expect alt_x to be a copy of x
			gwcopy (gwdata, ps.x, ps.alt_x);
			ps.alt_units_bit = ps.units_bit;
			// We first compute (prp_base^k) by double-checking 
			if (w->k != 1.0) {
				ps.state = STATE_DCHK_PASS1;
				ps.start_counter = 0;
				if (w->k > 0.0)
					ps.end_counter = (int) _log2 (w->k);
				else
					ps.end_counter = bitlen (gk);
			} else {
				ps.state = STATE_GERB_START_BLOCK;
			}
		}
	}

/* Init vars for Test/Status and CommunicateWithServer */

	if (ps.residue_type == PROTH_TYPE || ps.residue_type == GMN_TYPE)
		strcpy (w->stage, "Proth");
	else
		strcpy (w->stage, "PRP");
	inverse_explen = 1.0 / (double) final_counter;
	w->pct_complete = (double) ps.counter * inverse_explen;
	calc_output_frequencies (gwdata, &output_frequency, &output_title_frequency);

/* If we are near the maximum exponent this fft length can test, then we */
/* will error check all iterations */

	near_fft_limit = gwnear_fft_limit (gwdata, pcfftlim);


/* Figure out the maximum round-off error we will allow.  By default this is 27/64 when near the FFT limit and 26/64 otherwise. */
/* We've found that this default catches errors without raising too many spurious error messages.  We let the user override */
/* this default for user "Never Odd Or Even" who tests exponents well beyond an FFT's limit.  He does his error checking by */
/* running the first-test and double-check simultaneously. */

	allowable_maxerr = IniGetFloat (INI_FILE, "MaxRoundoffError", (float) (near_fft_limit ? 0.421875 : 0.40625));

/* Get setting for verbosity of hardware error messages.  Force output of "confidence is excellent" when error checking. */

	error_count_messages = IniGetInt (INI_FILE, "ErrorCountMessages", 3);
	if (ps.error_check_type != ERRCHK_NONE) error_count_messages |= 0x8000;

/* Init filename */

	tempFileName (filename, 'z', N);

/* Init the write save file state.  This remembers which save files are Gerbicz-checked.  Do this initialization */
/* before the restart for roundoff errors so that error recovery does not destroy thw write save file state. */

	writeSaveFileStateInit (&write_save_file_state, filename, NUM_JACOBI_BACKUP_FILES);

/* Get the current time */

/* Format the string representation of the test number */

	if (w->known_factors == NULL) {
		strcpy (string_rep, /*gwmodulo_as_string (gwdata)*/str); // 02/07/20
		string_rep_truncated = FALSE;
	} else {
		if (strchr (gwmodulo_as_string (gwdata), '^') == NULL)
			strcpy (string_rep, gwmodulo_as_string (gwdata));
		else
			sprintf (string_rep, "(%s)", gwmodulo_as_string (gwdata));
		if (strlen (w->known_factors) < 40) {
			char	*p;
			strcat (string_rep, "/");
			strcat (string_rep, w->known_factors);
			while ((p = strchr (string_rep, ',')) != NULL) *p = '/';
			string_rep_truncated = FALSE;
		} else {
			strcat (string_rep, "/known_factors");
			string_rep_truncated = TRUE;
		}
	}

/* Optionally resume from save file and output a message */
/* indicating we are resuming a test */

	if (fileExists (filename) && readPRPSaveFile (gwdata, filename, w, &ps)) {
		char	fmt_mask[80];
		double	pct;

		if (restart_counter < 0 || ps.counter <= (unsigned long) restart_counter)
			first_iter_msg = TRUE;
		bit = ps.counter+1;
		pct = trunc_percent (bit * 100.0 / Nlen);
		if (ps.residue_type == PROTH_TYPE || ps.residue_type == GMN_TYPE)
			sprintf (fmt_mask,
				"Resuming Proth prime test of %%s at bit %%ld [%%.%df%%%%]\n",
				PRECISION);
		else if (ps.residue_type == PRP_TYPE_SPRP)
			sprintf (fmt_mask,
				"Resuming Strong PRP test of %%s at bit %%ld [%%.%df%%%%]\n",
				PRECISION);
		else
			sprintf (fmt_mask,
				"Resuming Fermat PRP test of %%s at bit %%ld [%%.%df%%%%]\n",
				PRECISION);
		sprintf (buf, fmt_mask, string_rep, bit, pct);
		OutputStr (buf);
		if (verbose || restarting)
			writeResults (buf);
	}

/* Otherwise, output a message indicating we are starting test */

	else {
		clear_timers ();	// Make all timers clean...
		start_timer (2);
		if (ps.residue_type == PROTH_TYPE || ps.residue_type == GMN_TYPE)
			if (showdigits)
				sprintf (buf, "Starting Proth prime test of %s (%lu decimal digits)\n", string_rep, nbdg);
			else
				sprintf (buf, "Starting Proth prime test of %s\n", string_rep);
		else if (ps.residue_type == PRP_TYPE_SPRP)
				sprintf (buf, "Starting Strong PRP test of %s\n", string_rep);
		else
				sprintf (buf, "Starting Fermat PRP test of %s\n", string_rep);
		OutputStr (buf);
		if (verbose || restarting)
			writeResults (buf);
		ps.counter = 0;
		ps.error_count = 0;
		first_iter_msg = TRUE;
		bit = 1;
	}

	start_timer (0);	// Start global timer.
	start_timer (1);	// Start loop timer

/* Output a message about the FFT length and the Proth base. */

	topology_print_children (hwloc_get_root_obj (hwloc_topology), 0);
	gwfft_description (gwdata, fft_desc);
	sprintf (buf, "Using %s, a = %lu\n", fft_desc, a);

	OutputStr (buf);
	if (verbose || restarting)
		writeResults (buf);

	ReplaceableLine (1);	/* Remember where replaceable line is */

/* Get setting for verbosity of hardware error messages.  Force output of "confidence is excellent" when error checking. */

	error_count_messages = IniGetInt (INI_FILE, "ErrorCountMessages", 3);
	if (ps.error_check_type != ERRCHK_NONE) error_count_messages |= 0x8000;

/* Do the Proth or PRP test */

	gwsetmulbyconst (gwdata, a);
	iters = 0;
	if (ps.residue_type == GMN_TYPE) {
		loopshift = (ps.counter >= final_counter_y) ? final_counter_y : 0;
		max_counter = (ps.counter >= final_counter_y) ? final_counter : final_counter_y;
	}
	else {
		max_counter = final_counter;
		loopshift = 0;
	}
	while (ps.counter < final_counter) {
		gwnum	x;			/* Pointer to number to square */
		unsigned long *units_bit;	/* Pointer to units_bit to update */
		int	saving, saving_highly_reliable, sending_residue, interim_residue, interim_file, stop_reason;
		int	actual_frequency, have_to_multiply = 0;

/* If this is the first iteration of a Gerbicz error-checking block, then */
/* determine "L" -- the number of squarings between each Gerbicz multiplication */
/* We end this Gerbicz block after L^2 iterations.  */
/* If there aren't many iterations left, revert to simple double-checking. */

		if (ps.state == STATE_GERB_START_BLOCK) {
			int	iters_left = final_counter - ps.counter;
			if (iters_left < 49) {
				ps.state = STATE_DCHK_PASS1;
				ps.start_counter = ps.counter;
				ps.end_counter = final_counter;
				if (ps.alt_units_bit) {
					gwadd3 (gwdata, ps.alt_x, ps.alt_x, ps.alt_x);
					ps.alt_units_bit = ps.alt_units_bit + 1;
					if (ps.alt_units_bit >= ((ps.residue_type == GMN_TYPE)?2*(w->n):w->n)) {
						ps.alt_units_bit -= (ps.residue_type == GMN_TYPE)?2*(w->n):w->n;
					}
				}
			} else {
				int	gerbicz_block_size;
				double	adjustment;
				ps.state = STATE_GERB_MID_BLOCK;
				adjustment = IniGetFloat (INI_FILE, "PRPGerbiczCompareIntervalAdj", 1.0);
				if (adjustment < 0.001 || adjustment > 1.0) adjustment = 0.5;
				gerbicz_block_size = (int) (adjustment * IniGetInt (INI_FILE, "GerbiczCompareInterval", 1000000));
				if (gerbicz_block_size < 25) gerbicz_block_size = 25;
				if (gerbicz_block_size > iters_left) gerbicz_block_size = iters_left;
				ps.L = (unsigned long) sqrt ((double) gerbicz_block_size);
				ps.start_counter = ps.counter;
				ps.next_mul_counter = ps.counter + ps.L;
				ps.end_counter = ps.counter + ps.L * ps.L;
				gwswap (ps.alt_x, ps.u0);		// Set u0 to a copy of x
				gwcopy (gwdata, ps.x, ps.d);		// Set d[0] to a copy of x
				if (IniGetInt (INI_FILE, "GerbiczVerbosity", 1) > 1) {
					sprintf (buf, "Start Gerbicz block of size %ld at iteration %ld.\n", ps.L * ps.L, ps.start_counter+1);
					OutputBoth (buf);
				}
			}
		}

/* Save if we are stopping, right after we pass an errored iteration, several iterations before retesting */
/* an errored iteration so that we don't have to backtrack very far to do a gwsquare_carefully iteration */
/* (we don't do the iteration immediately before because a save operation may change the FFT data and make */
/* the error non-reproducible), and finally save if the save file timer has gone off. */

		stop_reason = stopping = stopCheck ();
		saving = stopping || (ps.counter == last_counter-8) || (ps.counter == last_counter);
		saving_highly_reliable = FALSE;

/* Round off error check the first and last 50 iterations, before writing a save file, near an FFT size's limit, */
/* or check every iteration option is set, and every 128th iteration. */

		echk = ERRCHK || ps.counter < 50 || ps.counter >= final_counter-50 || saving ||
		       (ps.error_check_type == ERRCHK_NONE && (near_fft_limit || ((ps.counter & 127) == 0)));
		gw_clear_maxerr (gwdata);

/* Check if we should output residue to screen, or create an intermediate save file */

		sending_residue = (ps.state == STATE_NORMAL || ps.state == STATE_DCHK_PASS1 || ps.state == STATE_GERB_MID_BLOCK) &&
				  ps.counter > 0 &&
				  ((ps.counter+1-interim_counter_off_one) == 500000 ||
				   ((ps.counter+1-interim_counter_off_one) % 5000000 == 0 && IniGetInt (INI_FILE, "SendInterimResidues", 1)));
		interim_residue = INTERIM_RESIDUES &&
				  (ps.state == STATE_NORMAL || ps.state == STATE_DCHK_PASS1 || ps.state == STATE_GERB_MID_BLOCK) &&
				  (ps.counter > 0 && (ps.counter+1-interim_counter_off_one) % INTERIM_RESIDUES == 0);
		interim_file = INTERIM_FILES &&
			       (ps.state == STATE_NORMAL || ps.state == STATE_DCHK_PASS2 || ps.state == STATE_GERB_MID_BLOCK) &&
			       (ps.counter > 0 && (ps.counter+1) % INTERIM_FILES == 0);

/* Do one PRP or Proth iteration */

		timers[1] = 0.0;
		start_timer (1);

/* If we are doing one of the Gerbicz multiplies (not a squaring), then handle that here */

		if (ps.state == STATE_GERB_MID_BLOCK_MULT) {
			gwstartnextfft (gwdata, 0);		/* Do not start next forward FFT */
			gwsetnormroutine (gwdata, 0, 1, 0);	/* Always roundoff error check multiplies */
			gwsafemul (gwdata, ps.x, ps.d);	/* "Safe" multiply that does not change ps.x */
			x = ps.d;				/* Set pointer for checking roundoff errors, sumouts, etc. */
		} else if (ps.state == STATE_GERB_END_BLOCK_MULT) {
			gwstartnextfft (gwdata, 0);		/* Do not start next forward FFT */
			gwsetnormroutine (gwdata, 0, 1, 0);	/* Always roundoff error check multiplies */
			gwmul (gwdata, ps.u0, ps.alt_x);	/* Multiply to calc checksum #2.  u0 value can be destroyed. */
			x = ps.alt_x;				/* Set pointer for checking roundoff errors, sumouts, etc. */
		} else if (ps.state == STATE_GERB_FINAL_MULT) {
			gwcopy (gwdata, ps.x, ps.u0);		// Copy x (before using it) for next Gerbicz block
			gwstartnextfft (gwdata, 0);		/* Do not start next forward FFT */
			gwsetnormroutine (gwdata, 0, 1, 0);	/* Always roundoff error check multiplies */
			gwsafemul (gwdata, ps.u0, ps.d);	/* "Safe" multiply to compute final d[t] value (checksum #1) */
			x = ps.d;				/* Set pointer for checking roundoff errors, sumouts, etc. */
		}

/* Otherwise, do a squaring iteration */

		else {

/* Use state to decide which number we are squaring */

			if (ps.state == STATE_NORMAL || ps.state == STATE_DCHK_PASS1 || ps.state == STATE_GERB_MID_BLOCK) {
				x = ps.x;
				units_bit = &ps.units_bit;
			} else {			// (ps.state == STATE_DCHK_PASS2 || ps.state == STATE_GERB_END_BLOCK) {
				x = ps.alt_x;
				units_bit = &ps.alt_units_bit;
			}

/* Decide if we can start the next forward FFT.  This is faster, but leaves the result in an "unsavable-to-disk" state. */

			gwstartnextfft (gwdata,
					!saving && !maxerr_recovery_mode && ps.counter != ps.end_counter-1 &&
					ps.counter > (35+loopshift) && ps.counter < (max_counter-35) &&
					!sending_residue && !interim_residue && !interim_file);

/* Process this bit.  Use square carefully the first and last 30 iterations. */
/* This should avoid any pathological non-random bit pattterns.  Also square */
/* carefully during an error recovery. This will protect us from roundoff */
/* errors up to (1.0 - 0.40625). */

			if (have_to_multiply = bitval (exp, final_counter-ps.counter-1)) {
				gwsetnormroutine (gwdata, 0, echk, 1);
			} else {
				gwsetnormroutine (gwdata, 0, echk, 0);
			}
			if (maxerr_recovery_mode[6] && ps.counter == last_counter) {
				gwsquare_carefully (gwdata, x);
				maxerr_recovery_mode[6] = 0;
				last_counter = 0xFFFFFFFF;
				echk = 0;
			} else if (ps.counter < (30+loopshift) || ps.counter >= (max_counter-30))
				gwsquare_carefully (gwdata, x);
			else
				gwsquare (gwdata, x);

			(*units_bit) <<= 1;
			if ((*units_bit) >= ((ps.residue_type == GMN_TYPE)?2*(w->n):w->n)) {
				(*units_bit) -= (ps.residue_type == GMN_TYPE)?2*(w->n):w->n;
			}
			if ((ps.residue_type == GMN_TYPE) && (ps.counter == final_counter_y)) {
				gwcopy (gwdata, x, ps.y);
				ps.units_bit2 = *units_bit;
				loopshift = final_counter_y;
				max_counter = final_counter;
			}

		}

// introduce an error every random # iterations when debugging highly reliable error checking

//#define INTRODUCE_ERRORS
#ifdef INTRODUCE_ERRORS
		if ((rand () & 0x7FFF) == 134)  // one out of 32768
			*x += 5.0;
#endif
		
/* End iteration timing and increase count of iterations completed */

		end_timer (1);
		timers[0] += timers[1];
		iters++;

/* Update min/max round-off error */

		if (echk) {
			if (ps.counter > 30 && gw_get_maxerr (gwdata) < reallyminerr) reallyminerr = gw_get_maxerr (gwdata);
			if (gw_get_maxerr (gwdata) > reallymaxerr) reallymaxerr = gw_get_maxerr (gwdata);
		}

/* If the sum of the output values is an error (such as infinity) then raise an error. */
/* This kind of error is apt to persist, so always restart from last save file. */

		if (gw_test_illegal_sumout (gwdata)) {
			sprintf (buf, ERRMSG0, ps.counter+1, final_counter, ERRMSG1A);
			OutputBoth (buf);
			inc_error_count (2, &ps.error_count);
			last_counter = ps.counter;		/* create save files before and after this iteration */
			restart_counter = -1;			/* rollback to any save file */
			sleep5 = TRUE;
			goto restart;
		}

/* Check for excessive roundoff error.  If round off is too large, repeat the iteration to see if this was */
/* a hardware error.  If it was repeatable then repeat the iteration using a safer, slower method.  This can */
/* happen when operating near the limit of an FFT.  NOTE: with the introduction of Gerbicz error-checking we */
/* ignore some of these errors as the Gerbicz check will catch any problems later.  However, if the round off */
/* error is really large, then results are certainly corrupt and we roll back immmediately. */

		if (echk && gw_get_maxerr (gwdata) > allowable_maxerr) {
			if (ps.counter == last_counter && gw_get_maxerr (gwdata) == last_maxerr[6]) {
				OutputBoth (ERROK);
				inc_error_count (3, &ps.error_count);
				gw_clear_error (gwdata);
				OutputBoth (ERRMSG5);
				maxerr_recovery_mode[6] = 1;
				restart_counter = ps.counter;		/* rollback to this iteration or earlier */
				sleep5 = FALSE;
				goto restart;
			} else {
				char	msg[100];
				sprintf (msg, ERRMSG1C, gw_get_maxerr (gwdata), allowable_maxerr);
				sprintf (buf, ERRMSG0, ps.counter+1, final_counter, msg);
				OutputBoth (buf);
				inc_error_count (1, &ps.error_count);
				if (ps.error_check_type == ERRCHK_NONE ||
				    gw_get_maxerr (gwdata) > IniGetFloat (INI_FILE, "RoundoffRollbackError", (float) 0.475)) {
					last_counter = ps.counter;
					last_maxerr[6] = gw_get_maxerr (gwdata);
					restart_counter = ps.counter;		/* rollback to this iteration or earlier */
					sleep5 = FALSE;
					goto restart;
				}
			}
		}

/* Update counter, percentage complete */

		ps.counter++;
		w->pct_complete = (double) ps.counter * inverse_explen;
		if (ps.error_check_type == ERRCHK_DBLCHK) {
			unsigned long true_counter;
			true_counter = ps.start_counter + ((ps.counter - ps.start_counter) >> 1);
			if (ps.state == STATE_DCHK_PASS2) true_counter += ((ps.end_counter - ps.start_counter) >> 1);
			w->pct_complete = (double) true_counter * inverse_explen;
		}

/* Output the title every so often */

		actual_frequency = (int) (ITER_OUTPUT * output_title_frequency);
		if (actual_frequency < 1) actual_frequency = 1;
		if (ps.counter % actual_frequency == 0 || first_iter_msg) {
			if ((ps.residue_type == PROTH_TYPE)||(ps.residue_type == GMN_TYPE))
				sprintf (buf, "%.*f%% of Proth %s", (int) PRECISION, 100.0*trunc_percent (w->pct_complete), string_rep);
			else if (ps.residue_type == PRP_TYPE_SPRP)
				sprintf (buf, "%.*f%% of Strong PRP %s", (int) PRECISION, 100.0*trunc_percent (w->pct_complete), string_rep);
			else
				sprintf (buf, "%.*f%% of Fermat PRP %s", (int) PRECISION, 100.0*trunc_percent (w->pct_complete), string_rep);
			title (buf);
		}

//		ReplaceableLine (2);	/* Replace line */ 

/* Print a message every so often */

		actual_frequency = (int) (ITER_OUTPUT * output_frequency);
		if (actual_frequency < 1) actual_frequency = 1;
		if ((ps.counter % actual_frequency == 0 && ps.state != STATE_GERB_MID_BLOCK_MULT &&
		     ps.state != STATE_GERB_END_BLOCK && ps.state != STATE_GERB_END_BLOCK_MULT &&
		     ps.state != STATE_GERB_FINAL_MULT) || first_iter_msg) {
			sprintf (buf, "Iteration: %ld / %ld [%.*f%%]",
				 ps.counter, final_counter, (int) PRECISION, 100.0*trunc_percent (w->pct_complete));
			/* Append a short form total errors message */
			if ((error_count_messages & 0xFF) == 1)
				make_error_count_message (ps.error_count, error_count_messages, buf + strlen (buf),
							  (int) (sizeof (buf) - strlen (buf)));
			/* Truncate first message */
			if (first_iter_msg) {
				strcat (buf, ".\r");	// JP 17/04/20
				clear_timer (0);
				first_iter_msg = FALSE;
			}
			/* In v28.5 and later, format a consise message including the ETA */
			else if (!CLASSIC_OUTPUT) {
				double speed;
				/* Append roundoff error */
				if ((OUTPUT_ROUNDOFF || ERRCHK) && reallymaxerr >= 0.001) {
					sprintf (buf+strlen(buf), ", roundoff: %5.3f", reallymaxerr);
					if (!CUMULATIVE_ROUNDOFF) reallyminerr = 1.0, reallymaxerr = 0.0;
				}
				/* Append ms/iter */
				speed = timer_value (0) / (double) iters;
				sprintf (buf+strlen(buf), ", ms/iter: %6.3f", speed * 1000.0);
				clear_timer (0);
				iters = 0;
				/* Append ETA */
				formatETA ((final_counter - ps.counter) * speed, buf+strlen(buf));
#if defined (_CONSOLE) || defined (__linux__) || defined (__FreeBSD__) || defined (__APPLE__)
				strcat (buf, "\r");
#else
				strcat (buf, "\n");
#endif
			}
			/* Format the classic (pre-v28.5) message */
			else {
				/* Append optional roundoff message */
				if (ERRCHK && ps.counter > 30) {
					sprintf (buf+strlen(buf), ".  Round off: %10.10f to %10.10f", reallyminerr, reallymaxerr);
					if (!CUMULATIVE_ROUNDOFF) reallyminerr = 1.0, reallymaxerr = 0.0;
				}
				if (CUMULATIVE_TIMING) {
					strcat (buf, ".  Total time: ");
					print_timer (0, TIMER_NL);
				} else {
					strcat (buf, ".  Per iteration time: ");
					divide_timer (0, iters);
					print_timer (0, TIMER_NL | TIMER_CLR);
					iters = 0;
				}
			}

			OutputStr (buf);


/* Output a verbose message showing the error counts.  This way a user is likely to */
/* notice a problem without reading the results.txt file. */

			if ((error_count_messages & 0xFF) >= 2 &&
			    make_error_count_message (ps.error_count, error_count_messages, buf, sizeof (buf)))
				OutputStr (buf);
		}

		ReplaceableLine (2);	/* Replace line */ 

/* Print a results file message every so often */

		if ((ps.counter % ITER_OUTPUT_RES == 0 && ps.state != STATE_GERB_MID_BLOCK_MULT &&
		     ps.state != STATE_GERB_END_BLOCK && ps.state != STATE_GERB_END_BLOCK_MULT &&
		     ps.state != STATE_GERB_FINAL_MULT) || (NO_GUI && stop_reason)) {
			sprintf (buf, "Iteration %ld / %ld\n", ps.counter, final_counter);
			writeResults (buf);
		}

/* If double-checking, at end of pass 1 rollback counter and start computing alt_x. */
/* If double-checking, at end of pass 2 compare values and move onto next block. */

		if (ps.state == STATE_DCHK_PASS1) {
			if (ps.counter < ps.end_counter)
				;		// Do next iteration
			else if (ps.counter == ps.end_counter) {	// Switch to alt_x computations
				ps.state = STATE_DCHK_PASS2;
				ps.counter = ps.start_counter;
			} else {					// Can't happen
				OutputBoth (ERRMSG9);
				inc_error_count (6, &ps.error_count);
				restart_counter = -1;			/* rollback to any save file */
				sleep5 = FALSE;
				goto restart;
			}
		}

		if (ps.state == STATE_DCHK_PASS2) {
			if (ps.counter < ps.end_counter)
				;		// Do next iteration
			else if (ps.counter == ps.end_counter) {	// Switch to alt_x computations
				if (!areTwoPRPValsEqual (gwdata, (ps.residue_type == GMN_TYPE)?2*(w->n):w->n, ps.x, ps.units_bit, ps.alt_x, ps.alt_units_bit)) {
					sprintf (buf, ERRMSG60, ps.start_counter);
					OutputBoth (buf);
					inc_error_count (7, &ps.error_count);
					restart_counter = ps.start_counter;		/* rollback to this iteration */
					sleep5 = FALSE;
					goto restart;
				}
				/* If doing a full double-check, start next block of iterations */
				if (ps.error_check_type == ERRCHK_DBLCHK) {
					ps.state = STATE_DCHK_PASS1;
					ps.start_counter = ps.counter;
					ps.end_counter = ps.start_counter + IniGetInt (INI_FILE, "PRPDoublecheckCompareInterval", 100000);
					if (ps.end_counter > final_counter) ps.end_counter = final_counter;
				}
				/* Otherwise, we're doing the first or last few iterations of a Gerbicz error-check. */
				/* Set state to start next Gerbicz block (in case we just computed prp_base^k). */
				else {
					ps.state = STATE_GERB_START_BLOCK;
				}
				/* We've reached a verified iteration, create a save file and mark it highly reliable. */
				/* But if there are less than 1000 iterations left on a reliable machine */
				/* don't bother creating the save file. */
				if (final_counter - ps.counter >= 1000 || ps.error_count > 0) {
					saving = TRUE;
					saving_highly_reliable = TRUE;
				}
			} else {					// Can't happen
				OutputBoth (ERRMSG9);
				inc_error_count (6, &ps.error_count);
				restart_counter = -1;			/* rollback to any save file */
				sleep5 = FALSE;
				goto restart;
			}
		}

/* If Gerbicz error-checking, handle all the possible Gerbicz states.  See if this is an L-th iteration that needs */
/* to do a checksum multiply.  Also check if this is the L^2-th iteration where we switch to an alternate method to */
/* compute checksum #2. */
/* NOTE: We do not need to worry about shift_counts in the checksum as both checksums will end up with the same shift count. */

		// Just did a normal PRP squaring

		if (ps.state == STATE_GERB_MID_BLOCK) {
			if (ps.counter < ps.next_mul_counter)
				;		// Do next iteration
			else if (ps.counter == ps.end_counter) {	// Delay last checksum #1 multiply, start checksum #2 calculation
				if (IniGetInt (INI_FILE, "GerbiczVerbosity", 1) > 1) OutputStr ("Start Gerbicz error check.\n");
				// At end of Gerbicz block, switch to "L" squarings of alt_x to create Gerbicz checksum #2 value
				gwcopy (gwdata, ps.d, ps.alt_x);	// Copy d[t-1] to alt_x
				ps.state = STATE_GERB_END_BLOCK;	// Squaring alt_x state
				ps.counter -= ps.L;			// L squarings
			} else if (ps.counter == ps.next_mul_counter) {	// Do a checksum #1 multiply next
				// Back counter up by one and do one multiply in the computation of Gerbicz checksum #1 value
				ps.state = STATE_GERB_MID_BLOCK_MULT;
				ps.counter -= 1;
			} else {					// Can't happen
				OutputBoth (ERRMSG9);
				inc_error_count (6, &ps.error_count);
				restart_counter = -1;			/* rollback to any save file */
				sleep5 = FALSE;
				goto restart;
			}
		}

		// Just did a a checksum #1 multiply at the end of a block of L normal squarings


		else if (ps.state == STATE_GERB_MID_BLOCK_MULT) {
			if (ps.counter < ps.end_counter) {		// In middle of Gerbicz block, do another "L" squarings
				ps.state = STATE_GERB_MID_BLOCK;
				ps.next_mul_counter += ps.L;
			} else {					// Can't happen
				OutputBoth (ERRMSG9);
				inc_error_count (6, &ps.error_count);
				restart_counter = -1;			/* rollback to any save file */
				sleep5 = FALSE;
				goto restart;
			}
		}

		// Just did a checksum #2 squaring
		else if (ps.state == STATE_GERB_END_BLOCK) {
			if (ps.counter < ps.end_counter)
				;		// Do next iteration in computing checksum #2
			else if (ps.counter == ps.end_counter) {	// Next do final multiply in computing checksum #2
				ps.state = STATE_GERB_END_BLOCK_MULT;
				ps.counter -= 1;
			} else {					// Can't happen
				OutputBoth (ERRMSG9);
				inc_error_count (6, &ps.error_count);
				restart_counter = -1;			/* rollback to any save file */
				sleep5 = FALSE;
				goto restart;
			}
		}

		// Just did the checksum #2 multiply at the end of L checksum #2 squarings
		else if (ps.state == STATE_GERB_END_BLOCK_MULT) {
			if (ps.counter == ps.end_counter) {		// Next do final multiply in computing checksum #1
				ps.state = STATE_GERB_FINAL_MULT;
				ps.counter -= 1;
			} else {					// Can't happen
				OutputBoth (ERRMSG9);
				inc_error_count (6, &ps.error_count);
				restart_counter = -1;			/* rollback to any save file */
				sleep5 = FALSE;
				goto restart;
			}
		}

		// Just did the final checksum #1 multiply which we delayed until checksum #2 completed
		else if (ps.state == STATE_GERB_FINAL_MULT) {
			double	gerbicz_block_size_adjustment;
			// We adjust the compare interval size downward when errors occur and upwards when they dont.
			// That way buggy machines will lose fewer iterations when rolling back.
			gerbicz_block_size_adjustment = IniGetFloat (INI_FILE, "PRPGerbiczCompareIntervalAdj", 1.0);
			if (gerbicz_block_size_adjustment < 0.001 || gerbicz_block_size_adjustment > 1.0) gerbicz_block_size_adjustment = 0.5;
			// Compare alt_x, d (the two Gerbicz checksum values that must match)
			if (!areTwoPRPValsEqual (gwdata, (ps.residue_type == GMN_TYPE)?2*(w->n):w->n, ps.alt_x, 0, ps.d, 0)) {
				sprintf (buf, ERRMSG70, ps.start_counter);
				OutputBoth (buf);
				gerbicz_block_size_adjustment *= 0.25;		/* This will halve next L */
				if (gerbicz_block_size_adjustment < 0.001) gerbicz_block_size_adjustment = 0.001;
				IniWriteFloat (INI_FILE, "PRPGerbiczCompareIntervalAdj", (float) gerbicz_block_size_adjustment);
				inc_error_count (7, &ps.error_count);
				restart_counter = ps.start_counter;		/* rollback to this iteration */
				sleep5 = FALSE;
				goto restart;
			}
			if (IniGetInt (INI_FILE, "GerbiczVerbosity", 1)) {
				clearline(100);
				sprintf (buf, "Gerbicz error check passed at iteration %ld.\r", ps.counter);
				OutputStr (buf);
			}
			gerbicz_block_size_adjustment *= 1.0473;		/* 30 good blocks to double L */
			if (gerbicz_block_size_adjustment > 1.0) gerbicz_block_size_adjustment = 1.0;
			IniWriteFloat (INI_FILE, "PRPGerbiczCompareIntervalAdj", (float) gerbicz_block_size_adjustment);
			/* Start next Gerbicz block.  Both x and alt_x must be identical at start of next block. */
			ps.state = STATE_GERB_START_BLOCK;
			gwswap (ps.alt_x, ps.u0);
			ps.alt_units_bit = ps.units_bit;
			/* We've reached a verified iteration, create a save file and mark it highly reliable. */
			/* But if there are less than 1000 iterations left on a reliable machine, don't bother creating the save file. */
			if (final_counter - ps.counter >= 1000 || gerbicz_block_size_adjustment < 1.0) {
				saving = TRUE;
				saving_highly_reliable = TRUE;
			}
		}

/* Write results to a file every DISK_WRITE_TIME minutes */

		if (saving) {
			if (! writeSaveFile (gwdata, &write_save_file_state, w, &ps)) {
				sprintf (buf, WRITEFILEERR, filename);
				OutputBoth (buf);
			}
			// Mark save files that contain verified computations.  This will keep the save file
			// for a longer period of time (i.e. will not be replaced by a save file that does
			// not also contain verified computations).
			if (saving_highly_reliable)
				setWriteSaveFileSpecial (&write_save_file_state);
			saving = FALSE;
			saving_highly_reliable = FALSE;
		}

/* If an escape key was hit, write out the results and return */

		if (stop_reason) {
			stop_reason = FALSE;
			if (ps.residue_type == PROTH_TYPE || ps.residue_type == GMN_TYPE)
				sprintf (buf, "Stopping Proth prime test of %s at iteration %ld [%.*f%%]\n",
					string_rep, ps.counter, (int) PRECISION, trunc_percent (100.0*w->pct_complete));
			else if (ps.residue_type == PRP_TYPE_SPRP)
				sprintf (buf, "Stopping Strong PRP test of %s at iteration %ld [%.*f%%]\n",
					string_rep, ps.counter, (int) PRECISION, trunc_percent (100.0*w->pct_complete));
				else
				sprintf (buf, "Stopping Fermat PRP test of %s at iteration %ld [%.*f%%]\n",
					string_rep, ps.counter, (int) PRECISION, trunc_percent (100.0*w->pct_complete));
			clearline(100);
			OutputStr (buf);
			goto exit;
		}

/* Output the 64-bit residue at specified interims. */

		if (interim_residue) {
			tmp3 = popg (&gwdata->gdata, ((unsigned long) gwdata->bit_length >> 4) + 5);
			if (gwtogiant (gwdata, x, tmp3)) {
				pushg (&gwdata->gdata, 1);
				OutputBoth (ERRMSG80);
				inc_error_count (2, &ps.error_count);
				last_counter = ps.counter;		/* create save files before and after this iteration */
				restart_counter = -1;			/* rollback to any save file */
				sleep5 = TRUE;
				goto restart;
			}
			rotategp (tmp3, (ps.residue_type == GMN_TYPE)?2*(w->n):w->n, *units_bit);
			if (interim_mul)
				basemulg (tmp3, w, ps.prp_base, -1);
			if (w->known_factors && ps.residue_type != PRP_TYPE_COFACTOR)
				modg (N, tmp3);
			if ((ps.residue_type == PROTH_TYPE)||(ps.residue_type == GMN_TYPE))
				sprintf (buf, "%s interim Proth residue %08lX%08lX at iteration %ld\n",
					string_rep, (unsigned long) tmp3->n[1], (unsigned long) tmp3->n[0],
					ps.counter - interim_counter_off_one);
			else if (ps.residue_type == PRP_TYPE_SPRP)
				sprintf (buf, "%s interim Strong PRP residue %08lX%08lX at iteration %ld\n",
					string_rep, (unsigned long) tmp3->n[1], (unsigned long) tmp3->n[0],
					ps.counter - interim_counter_off_one);
			else
				sprintf (buf, "%s interim Fermat PRP residue %08lX%08lX at iteration %ld\n",
					string_rep, (unsigned long) tmp3->n[1], (unsigned long) tmp3->n[0],
					ps.counter - interim_counter_off_one);
			OutputBoth (buf);
			pushg (&gwdata->gdata, 1);
		}

/* Write a save file every INTERIM_FILES iterations. */

		if (interim_file) {
			char	interimfile[32];
			writeSaveFileState state;
			sprintf (interimfile, "%s.%03ld", filename, ps.counter / INTERIM_FILES);
			writeSaveFileStateInit (&state, interimfile, 0);
			state.num_ordinary_save_files = 99;
			writeSaveFile (gwdata, &state, w, &ps);
		}
		bit++;
	}

/* Free up some memory */

	gwfree (gwdata, ps.u0);
	gwfree (gwdata, ps.d);

/* Make sure PRP state is valid.  We cannot be in the middle of a double-check or in the middle of a Gerbicz block */

	if (ps.state != STATE_NORMAL && ps.state != STATE_DCHK_PASS1 && ps.state != STATE_GERB_START_BLOCK) {
		OutputBoth (ERRMSG90);
		inc_error_count (6, &ps.error_count);
		restart_counter = -1;			/* rollback to any save file */
		sleep5 = FALSE;
		goto restart;
	}

/* See if we've found a probable prime.  If not, format a 64-bit residue. */

	if (gwtogiant (gwdata, ps.x, tmp)) {
		OutputBoth (ERRMSG8);
		inc_error_count (2, &ps.error_count);
		restart_counter = -1;			/* rollback to any save file */
		sleep5 = TRUE;
		goto restart;
	}

	rotategp (tmp, (ps.residue_type == GMN_TYPE)?2*(w->n):w->n, ps.units_bit);
	if (mul_final)
		basemulg (tmp, w, ps.prp_base, mul_final);
	if (w->known_factors && ps.residue_type != PRP_TYPE_COFACTOR)
		modg (N, tmp);
	if (ps.residue_type != GMN_TYPE) {
		PositiveResult = isPRPg (tmp, N, w, ps.prp_base, ps.residue_type);
		if (!PositiveResult) {
			if (ps.residue_type == PROTH_TYPE)
				uladdg (1, tmp);	// to match with V3.8.23 Proth residue
			sprintf (res64, "%08lX%08lX", (unsigned long) ((tmp->sign > 1) ? tmp->n[1] : 0), (unsigned long) tmp->n[0]);
			have_res2048 = (tmp->sign > 64);
			if (have_res2048) {
				int i;
				for (i = 63; i >= 0; i--) sprintf (res2048+504-i*8, "%08lX", (unsigned long) tmp->n[i]);
			}
		}
	}

/* If we are doing highly reliable error checking, then make sure the calculation of the final residue was error free! */
/* Perform the same calculations above but use alt_x. */

	if (ps.state == STATE_DCHK_PASS1 || ps.state == STATE_GERB_START_BLOCK) {
		int	alt_match, alt_isProbablePrime;
		if (gwtogiant (gwdata, ps.alt_x, tmp2)) {
			OutputBoth (ERRMSG8);
			inc_error_count (2, &ps.error_count);
			restart_counter = -1;			/* rollback to any save file */
			sleep5 = TRUE;
			goto restart;
		}
		rotategp (tmp2, (ps.residue_type == GMN_TYPE)?2*(w->n):w->n, ps.alt_units_bit);
		if (mul_final)
			basemulg (tmp2, w, ps.prp_base, mul_final);
		if (w->known_factors && ps.residue_type != PRP_TYPE_COFACTOR)
			modg (N, tmp2);
		if (ps.residue_type != GMN_TYPE) {
			alt_isProbablePrime = isPRPg (tmp2, N, w, ps.prp_base, ps.residue_type);
			alt_match = (PositiveResult == alt_isProbablePrime);
			if (alt_match && !alt_isProbablePrime) {
				if (ps.residue_type == PROTH_TYPE)
					uladdg (1, tmp2);	// to match with V3.8.23 Proth residue
				sprintf (alt_res64, "%08lX%08lX", (unsigned long) ((tmp2->sign > 1) ? tmp2->n[1] : 0), (unsigned long) tmp2->n[0]);
				alt_match = !strcmp (res64, alt_res64);
			}
		}
		else {
			alt_match = TRUE;	// Test only x == alt-x
		}
		if (!alt_match || (gcompg (tmp, tmp2)!=0)) {
			OutputBoth (ERRMSG80);
			inc_error_count (2, &ps.error_count);
			restart_counter = -1;			/* rollback to any save file */
			sleep5 = TRUE;
			goto restart;
		}
		gwfree (gwdata, ps.alt_x);
	}

	if (ps.residue_type == GMN_TYPE) {
		ps.gx = tmp;
		if (gwtogiant (gwdata, ps.y, tmp2)) {
			OutputBoth (ERRMSG8);
			inc_error_count (2, &ps.error_count);
			restart_counter = -1;			/* rollback to any save file */
			sleep5 = TRUE;
			goto restart;
		}
		rotategp (tmp2, (ps.residue_type == GMN_TYPE)?2*(w->n):w->n, ps.units_bit2);
		ps.gy = tmp2;

	/* Delete the continuation files. */

		unlinkSaveFiles (&write_save_file_state);
	
	}
	else {

		/* Print results */

		if (PositiveResult) {
			if (ps.residue_type == PROTH_TYPE)
				sprintf (buf, "%s is prime", string_rep);
			else	if (ps.residue_type == PRP_TYPE_SPRP) {
				sprintf (buf, "%s is a Strong Probable prime", string_rep);
				if (ps.prp_base != 3)
					sprintf (buf+strlen(buf), " (%u-PRP)", ps.prp_base);
			}
			else {
				sprintf (buf, "%s is a Fermat Probable prime", string_rep);
				if (ps.prp_base != 3)
					sprintf (buf+strlen(buf), " (%u-PRP)", ps.prp_base);
			}

			strcat (buf, "!");
			sprintf (buf+strlen(buf), " (%lu decimal digits)", nbdg); 
			*res = TRUE;
		}
		else {
			sprintf (buf, "%s is not prime.  ", string_rep);
			if (ps.prp_base != 3)
				sprintf (buf+strlen(buf), "Base-%u ", ps.prp_base);
			if (ps.residue_type == PROTH_TYPE)
				sprintf (buf+strlen(buf), "Proth ");
			else if (ps.residue_type != PRP_TYPE_FERMAT)
				sprintf (buf+strlen(buf), "Type-%d ", ps.residue_type);
			sprintf (buf+strlen(buf), "RES64: %s.", res64);
			*res = FALSE;
		}

/* Print known factors */

		if (string_rep_truncated) {
			char	*bigbuf;
			bigbuf = (char *) malloc (strlen (w->known_factors) + 100);
			if (bigbuf != NULL) {
				sprintf (bigbuf, "Known factors used for PRP test were: %s\n", w->known_factors);
				OutputStr (bigbuf);
				if ((PositiveResult && IniGetInt (INI_FILE, "OutputPrimes", 0)) ||
			    (!PositiveResult && IniGetInt (INI_FILE, "OutputComposites", 0)))
					writeResults (bigbuf);
				free (bigbuf);
			}
		}

	/* Delete the continuation files. */

		unlinkSaveFiles (&write_save_file_state);
	

		clearline (100);

		free (tmp);
		free (tmp2);
		gwfree (gwdata, ps.x);
		gwfree (gwdata, ps.y);


#if defined(WIN32) && !defined(_CONSOLE)

		sprintf (buf+strlen(buf), "  Time : "); 
		ReplaceableLine (2);	/* Replace line */ 

#else

		clearline(100);

		if (*res) {
#if defined (_CONSOLE)
			hConsole = GetStdHandle(STD_OUTPUT_HANDLE);	// Access to Console attributes
			SetConsoleTextAttribute(hConsole, BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
			OutputBoth(buf);
			SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
#else
			OutputStr("\033[7m");
			OutputBoth(buf);
			OutputStr("\033[0m");
#endif
		}
		else
			OutputBoth(buf);

		sprintf (buf, "  Time : "); 

#endif

/* Output the final timings */

		end_timer (2);
		write_timer (buf+strlen(buf), 2, TIMER_CLR | TIMER_NL); 
		OutputBoth(buf);

/* Cleanup and return */

	}

	IniWriteString(INI_FILE, "FFT_Increment", NULL);
	lasterr_point = 0;
	return (TRUE);

/* An error occured, output a message saying we are restarting, sleep, */
/* then try restarting at last save point. */

restart:
	if (sleep5) 
		OutputBoth (ERRMSG2);
	OutputBoth (ERRMSG3);

/* Save the incremented error count to be used in the restart rather than the error count read from a save file */

	restart_error_count = ps.error_count;


/* Sleep five minutes before restarting */

	if (sleep5 && ! SleepFive ()) {
		return (FALSE);
	}

/* Return so that last continuation file is read in */

	free (exp);
	return (-1);;

exit:
	free (tmp);
	free (tmp2);
	gwfree (gwdata, ps.x);
	gwfree (gwdata, ps.y);
	free (exp);
	*res = FALSE;		// To avoid credit message !
	return (FALSE);
}

/* Test for a (strong) Fermat probable prime -- gwsetup has already been called. */

int commonPRP (
	gwhandle *gwdata,
	ghandle *gdata,
	unsigned long a,
	int	*res, char *str)
{
	unsigned long bit, bitpos, firstone = 0, iters;
	gwnum	x, y, gwminusone, gwone;
	giant	tmp, tmp2;
	char	filename[20], buf[sgkbufsize+256], fft_desc[256], oldres64[17];
	long	write_time = DISK_WRITE_TIME * 60;
	int	echk, saving, stopping, zres;
	time_t	start_time, current_time;
	double	reallyminerr = 1.0;
	double	reallymaxerr = 0.0;

	if (!IniGetInt(INI_FILE, "Testdiff", 0))	// Unless test of MAXDIFF explicitly required
		gwdata->MAXDIFF = 1.0E80;				// Diregard MAXDIFF...

	Nlen = bitlen (N);
	tmp = popg (gdata, (Nlen >> 4) + 3);
	tmp2 = popg (gdata, (Nlen >> 4) + 3);

/* Init, subtract 1 from N to compute a^(N-1) mod N */

	gtog (N, tmp2);
	iaddg (-1, tmp2);
	while (bitval (tmp2, firstone) == 0)	// position of first one bit in N-1
		firstone++;

	nbdg = gnbdg (N, 10);	// Compute the number of decimal digits of the tested number.
	*res = TRUE;			/* Assume it is a probable prime */

/* Init filename */

	tempFileName (filename, 'z', N);

/* Allocate memory */

	x = gwalloc (gwdata);
	y = gwalloc (gwdata);
	gwminusone = gwalloc (gwdata);
	gwone = gwalloc (gwdata);

	dbltogw (gwdata, 1.0, gwone);
	gianttogw (gwdata, tmp2, gwminusone);

/* Optionally resume from save file and output a message */
/* indicating we are resuming a test */

	if (fileExists (filename) && readFromFile (gwdata, gdata, filename, &bit, x, NULL)) {
		char	fmt_mask[80];
		double	pct;
		pct = trunc_percent (bit * 100.0 / Nlen);
		sprintf (fmt_mask,
			 "Resuming probable prime test of %%s at bit %%ld [%%.%df%%%%]\n",
			 PRECISION);
		sprintf (buf, fmt_mask, str, bit, pct);
		OutputStr (buf);
		if (verbose || restarting)
			writeResults (buf);
	}

/* Otherwise, output a message indicating we are starting test */

	else {
		clear_timers ();	// Init. timers
 		if(!usingDivPhi_m) {
			if (showdigits)
				sprintf (buf, "Starting probable prime test of %s (%lu decimal digits)\n", str, nbdg);
			else
				sprintf (buf, "Starting probable prime test of %s\n", str);
			OutputStr (buf);
  		}
		if (verbose || restarting)
			writeResults (buf);
		bit = 1;
		dbltogw (gwdata, (double) a, x);
	}

/* Get the current time */

	start_timer (0);
	start_timer (1);
	time (&start_time);

/* Output a message about the FFT length */

	topology_print_children (hwloc_get_root_obj (hwloc_topology), 0);
	gwfft_description (gwdata, fft_desc);
	sprintf (buf, "Using %s, a = %lu\n", fft_desc, a);

	OutputStr (buf);
	if (verbose || restarting) {
		writeResults (buf);
	}
	ReplaceableLine (1);	/* Remember where replaceable line is */

/* Init the title */
        if (strong)
            title ("Strong Fermat PRP test in progress...");
        else
            title ("Fermat PRP test in progress...");

/* Do the PRP test */

	gwsetmulbyconst (gwdata, a);
	iters = 0;
	while (bit < Nlen) {

/* Error check the first and last 50 iterations, before writing an */
/* intermediate file (either user-requested stop or a */
/* 30 minute interval expired), and every 128th iteration. */

		stopping = stopCheck ();
		echk = stopping || ERRCHK || (bit <= 50) || (bit >= Nlen-50) || gwnear_fft_limit (gwdata, pcfftlim);
		if (((bit & 127) == 0) || (bit == 1) || (bit == (lasterr_point-1))) {
			echk = 1;
			time (&current_time);
			saving = ((current_time - start_time > write_time) || (bit == 1) || (bit == (lasterr_point-1)));
		} else
			saving = 0;

/* Process this bit */

		gwstartnextfft (gwdata, postfft && !debug && !stopping && !saving && bit != lasterr_point && (bit+26 < Nlen) && (bit > 26) && !maxerr_recovery_mode[6]);
		bitpos = Nlen-bit-1;
		if (bitval (tmp2, bitpos)) {
			gwsetnormroutine (gwdata, 0, echk, 1);
		} else {
			gwsetnormroutine (gwdata, 0, echk, 0);
		}

		if ((bit+25 < Nlen) && (bit > 25) && ((bit != lasterr_point) || !maxerr_recovery_mode[6])) {
			gwsquare (gwdata, x);
			care = FALSE;
		}
		else {
			gwsquare_carefully (gwdata, x);
			care = TRUE;
		}

		if (debug && (bit < 50))
			writeresidue (gwdata, x, N, tmp, buf, str, bit, BIT);
		CHECK_IF_ANY_ERROR (x, (bit), Nlen, 6);

/* That iteration succeeded, bump counters */

		if (bit == lasterr_point)
			saving = 1;					// Be sure to restart after this recovery iteration!
		bit++;
		iters++;

/* Print a message every so often */

		if (bit % ITER_OUTPUT == 0) {
			char	fmt_mask[80];
			double	pct;
			pct = trunc_percent (bit * 100.0 / Nlen);
			sprintf (fmt_mask, "%%.%df%%%% of %%ld", PRECISION);
			sprintf (buf, fmt_mask, pct, Nlen);
			title (buf);
			ReplaceableLine (2);	/* Replace line */
			sprintf (fmt_mask,
				 "%%s, bit: %%ld / %%ld [%%.%df%%%%]",
				 PRECISION);
			sprintf (buf, fmt_mask, str, bit, Nlen, pct);
			OutputStr (buf);
			if (ERRCHK && bit > 30) {
				OutputStr (".  Round off: ");
				sprintf (buf, "%10.10f", reallyminerr);
				OutputStr (buf);
				sprintf (buf, " to %10.10f", reallymaxerr);
				OutputStr (buf);
			}
			end_timer (0);
			if (CUMULATIVE_TIMING) {
				OutputStr (".  Time thusfar: ");
			} else {
				OutputStr (".  Time per bit: ");
				divide_timer (0, iters);
				iters = 0;
			}
			print_timer (0, TIMER_NL | TIMER_OPT_CLR);
			start_timer (0);
		}

/* Print a results file message every so often */

		if (bit % ITER_OUTPUT_RES == 0 || (NO_GUI && stopping)) {
			sprintf (buf, "Bit %ld / %ld\n", bit, Nlen);
			writeResults (buf);
		}

/* Write results to a file every DISK_WRITE_TIME minutes */
/* On error, retry in 10 minutes (it could be a temporary */
/* disk-full situation) */

		if (saving || stopping) {
			write_time = DISK_WRITE_TIME * 60;
			saving = FALSE;
			if (! writeToFile (gwdata, gdata, filename, bit, x, NULL)) {
				sprintf (buf, WRITEFILEERR, filename);
				OutputBoth (buf);
				if (write_time > 600) write_time = 600;
			}	
			time (&start_time);

/* If an escape key was hit, write out the results and return */

			if (stopping) {
				pushg (gdata, 2);
				gwfree (gwdata, x);
				gwfree (gwdata, y);
				gwfree (gwdata, gwminusone);
				gwfree (gwdata, gwone);
				*res = FALSE;
				return (FALSE);
			}
		}

/* Output the 64-bit residue at specified interims.  Also output the */
/* residues for the next iteration so that we can compare our */
/* residues to programs that start counter at zero or one. */

		if (interimResidues && bit % interimResidues < 2)
				writeresidue (gwdata, x, N, tmp, buf, str, bit, BIT);

/* Write a save file every "interimFiles" iterations. */

		if (interimFiles && bit % interimFiles == 0) {
			char	interimfile[20];
			sprintf (interimfile, "%.8s.%03lu",
				 filename, bit / interimFiles);
			if (! writeToFile (gwdata, gdata, interimfile, bit, x, NULL)) {
				sprintf (buf, WRITEFILEERR, interimfile);
				OutputBoth (buf);
			}
		}
		if (strong) {
			if (bitpos == firstone) {
				gwsub3 (gwdata, gwone, x, y);
//				zres = gwequal(gwdata, x, gwone);	// Test for x == 1 mod N
				zres = gwpiszero (gwdata, y, N);
				if (zres == 1)						// success.
					break;
				gwadd3 (gwdata, gwone, x, y);
//				zres = gwequal(gwdata, x, gwminusone);// Test for x == -1 mod N
				zres = gwpiszero (gwdata, y, N);
				if (zres == 1)						// success.
					break;
			}
			else if (bitpos && bitpos < firstone) {
				gwadd3 (gwdata, gwone, x, y);
//				zres = gwequal(gwdata, x, gwminusone);// Test for x == -1 mod N
				zres = gwpiszero (gwdata, y, N);
				if (zres == 1)						// success.
					break;
			}
		}
	}

/* See if we've found a probable prime.  If not, format a 64-bit residue. */
/* Old versions of PRP used a non-standard 64-bit residue, computing */
/* 3^N-3 mod N rather than the more standard 3^(N-1) mod N.  Since */
/* some projects recorded these non-standard residues, output that */
/* residue too.  Note that some really old versions printed out the */
/* 32-bit chunks of the non-standard residue in reverse order. */

	clearline (100);
	if (strong && bitpos) {
		sprintf (buf, "%s is base %lu-Strong Fermat PRP! (%lu decimal digits)", str, a, nbdg);
	}
	else {
		gwtogiant (gwdata, x, tmp);
		modg (N, tmp);			// External modulus and gwnum's one may be different...
		if (!isone (tmp)) {
			*res = FALSE;	/* Not a prime */
			if (abs(tmp->sign) < 2)	// make a 32 bit residue correct !!
				sprintf (res64, "%08lX%08lX", (unsigned long)0, (unsigned long)tmp->n[0]);
			else
				sprintf (res64, "%08lX%08lX", (unsigned long)tmp->n[1], (unsigned long)tmp->n[0]);
			imulg (a, tmp); modg (N, tmp); ulsubg (a, tmp);
			if (abs(tmp->sign) < 2)	// make a 32 bit residue correct !!
				sprintf (oldres64, "%08lX%08lX", (unsigned long)0, (unsigned long)tmp->n[0]);
			else
				sprintf (oldres64, "%08lX%08lX", (unsigned long)tmp->n[1], (unsigned long)tmp->n[0]);
 			if (usingDivPhi_m)
 				sprintf (buf, "%s does not divide %lu^%s^%d-1", str, a, sgb, usingDivPhi_m);
 			else
			if (IniGetInt (INI_FILE, "OldRes64", 1))
				sprintf (buf, "%s is not prime.  RES64: %s.  OLD64: %s", str, res64, oldres64);
			else
				sprintf (buf, "%s is not prime.  RES64: %s", str, res64);
		}
		else {
			if (strong) {
				*res = FALSE;	/* Not a prime */
				sprintf (buf, "%s is not prime, although base %lu-Fermat PSP!!", str, a);
			}
 			else if (usingDivPhi_m)
 				sprintf (buf, "%s Divides %lu^%s^%d-1", str, a, sgb, usingDivPhi_m);
			else
				sprintf (buf, "%s is base %lu-Fermat PRP! (%lu decimal digits)", str, a, nbdg);
		}
	}

/* Print results.  Do not change the format of this line as Jim Fougeron of */
/* PFGW fame automates his QA scripts by parsing this line. */

	pushg (gdata, 2);
	gwfree (gwdata, x);
	gwfree (gwdata, y);
	gwfree (gwdata, gwminusone);
	gwfree (gwdata, gwone);

/* Update the output file */

	if ((*res && IniGetInt (INI_FILE, "OutputPrimes", 0)) ||
	    (!*res && IniGetInt (INI_FILE, "OutputComposites", 0)))
		writeResults (buf);


#if defined(WIN32) && !defined(_CONSOLE)

	sprintf (buf+strlen(buf), "  Time : "); 
	ReplaceableLine (2);	/* Replace line */ 

#else

	clearline(100);

	if (*res) {
#if defined (_CONSOLE)
		hConsole = GetStdHandle(STD_OUTPUT_HANDLE);	// Access to Console attributes
		SetConsoleTextAttribute(hConsole, BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
		OutputBoth(buf);
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
#else
		OutputStr("\033[7m");
		OutputBoth(buf);
		OutputStr("\033[0m");
#endif
	}
	else
		OutputBoth(buf);

	sprintf (buf, "  Time : "); 

#endif

/* Output the final timings */

	end_timer (1);
	write_timer (buf+strlen(buf), 1, TIMER_CLR | TIMER_NL); 
	if ((*res && IniGetInt (INI_FILE, "OutputPrimes", 0)) ||
	    (!*res && IniGetInt (INI_FILE, "OutputComposites", 0)))
		OutputStr (buf);
	else
		OutputBoth (buf);

/* Cleanup and return */

	_unlink (filename);
	lasterr_point = 0;
	return (TRUE);

/* An error occured, sleep if required, then try restarting at last save point. */

error:
	pushg (gdata, 2);
	gwfree (gwdata, x);
	gwfree (gwdata, y);
	gwfree (gwdata, gwone);
	gwfree (gwdata, gwminusone);
	*res = FALSE;					// To avoid credit mesage...

	if ((abonillsum && gw_test_illegal_sumout(gwdata)) || 
		(abonmismatch && gw_test_mismatched_sums (gwdata)) || 
		(abonroundoff && gw_get_maxerr(gwdata) > maxroundoff)) {	// Abort...
		aborted = TRUE;
		sprintf (buf, ERRMSG5, checknumber, str);
		OutputBoth (buf);
		_unlink (filename);
		if(IniGetInt(INI_FILE, "StopOnAbort", 0)) {
			IniWriteInt (INI_FILE, "PgenLine", IniGetInt(INI_FILE, "PgenLine", 0) + 1);	// Point on the next line
			return (FALSE);
		}
		else
			return (TRUE);
	}

/* Output a message saying we are restarting */

	if (sleep5) OutputBoth (ERRMSG2);
	OutputBoth (ERRMSG3);

/* Sleep five minutes before restarting */

	if (sleep5 && ! SleepFive ()) {
		return (FALSE);
	}

/* Restart */

	if (will_try_larger_fft) {
		IniWriteInt(INI_FILE, "FFT_Increment", nbfftinc = (IniGetInt(INI_FILE, "FFT_Increment", 0) + 1));
		if (nbfftinc == maxfftinc)
			abonroundoff = TRUE;	// Don't accept any more Roundoff error.
	}
	return (-1);
}

/* Test for 2*N+1 prime, knowing that N is prime -- gwsetup has already been called. */

int commonCC1P (
	gwhandle *gwdata,
	ghandle *gdata,
	unsigned long a,
	int	*res, char *str)
{
	unsigned long bit, iters, nreduced, gcdn;
	gwnum	x;
	giant	exponent, tmp;
	char	filename[20], buf[sgkbufsize+256], fft_desc[256], oldres64[17];
	long	write_time = DISK_WRITE_TIME * 60;
	int	echk, saving, stopping;
	time_t	start_time, current_time;
	double	reallyminerr = 1.0;
	double	reallymaxerr = 0.0;

/* First, test if gcd ((a^2-1), N) is one... */

	nreduced = gmodi (a*a-1, N);
	if (!nreduced)
		gcdn = a*a-1;
	else
		gcdn = gcd (a*a-1, nreduced);
	if (gcdn != 1) {
		sprintf (buf, "%s has a small factor : %lu!!\n", str, gcdn);
		OutputBoth (buf);
		*res = FALSE;
		return (TRUE);
	}

	if (!IniGetInt(INI_FILE, "Testdiff", 0))	// Unless test of MAXDIFF explicitly required
		gwdata->MAXDIFF = 1.0E80;				// Diregard MAXDIFF...

	Nlen = bitlen (N);
	nbdg = gnbdg (N, 10); // Compute the number of decimal digits of the tested number.

/* Allocate memory */

	x = gwalloc (gwdata);

	tmp = popg (gdata, (Nlen >> 5) + 3);
	exponent = popg (gdata, (Nlen >> 5) + 3);


/* Init, subtract 1 from N to compute a^(N-1) mod N */

	gtog (N, exponent);
	iaddg (-1, exponent);
//	Nlen = bitlen (N);
	*res = TRUE;		/* Assume it is a prime */

/* Init filename */

	tempFileName (filename, 'z', N);

/* Optionally resume from save file and output a message */
/* indicating we are resuming a test */

	if (fileExists (filename) && readFromFile (gwdata, gdata, filename, &bit, x, NULL)) {
		char	fmt_mask[80];
		double	pct;
		pct = trunc_percent (bit * 100.0 / Nlen);
		sprintf (fmt_mask,
			 "Resuming Pocklington prime test of %%s at bit %%ld [%%.%df%%%%]\n",
			 PRECISION);
		sprintf (buf, fmt_mask, str, bit, pct);
		OutputStr (buf);
		if (verbose || restarting)
			writeResults (buf);
	}

/* Otherwise, output a message indicating we are starting test */

	else {
		clear_timers ();	// Init. timers
		if (setuponly) {
			if (gwdata->FFTLEN != OLDFFTLEN) {
				OutputBoth (str); 
				OutputBoth (" : "); 
			}
		}
		else {
			if (showdigits)
				sprintf (buf, "Starting Pocklington prime test of %s (%lu decimal digits)\n", str, nbdg);
			else
				sprintf (buf, "Starting Pocklington prime test of %s\n", str);
			OutputStr (buf);
		if (verbose || restarting)
			writeResults (buf);
		}
		bit = 1;
		dbltogw (gwdata, (double) a, x);
	}

/* Get the current time */

	start_timer (0);
	start_timer (1);
	time (&start_time);

/* Output a message about the FFT length */

	topology_print_children (hwloc_get_root_obj (hwloc_topology), 0);
	gwfft_description (gwdata, fft_desc);
	sprintf (buf, "Using %s, a = %lu\n", fft_desc, a);

	OutputStr (buf);
	if (verbose || restarting) {
		writeResults (buf);
	}
	ReplaceableLine (1);	/* Remember where replaceable line is */

/* Init the title */

	title ("Pocklington prime test in progress...");

/* Do the PRP test */

	gwsetmulbyconst (gwdata, a);

	iters = 0;
	while (bit < Nlen) {

/* Error check the first and last 50 iterations, before writing an */
/* intermediate file (either user-requested stop or a */
/* 30 minute interval expired), and every 128th iteration. */

		stopping = stopCheck ();
		echk = stopping || ERRCHK || (bit <= 50) || (bit >= Nlen-50) || gwnear_fft_limit (gwdata, pcfftlim);
		if (((bit & 127) == 0) || (bit == 1) || (bit == (lasterr_point-1))) {
			echk = 1;
			time (&current_time);
			saving = ((current_time - start_time > write_time) || (bit == 1) || (bit == (lasterr_point-1)));
		} else
			saving = 0;

/* Process this bit */

		gwstartnextfft (gwdata, postfft && !debug && !stopping && !saving && bit != lasterr_point && (bit+26 < Nlen) && (bit > 26) && !maxerr_recovery_mode[6]);

		if (bitval (exponent, Nlen-bit-1)) {
			gwsetnormroutine (gwdata, 0, echk, 1);
		} else {
			gwsetnormroutine (gwdata, 0, echk, 0);
		}
		if ((bit+25 < Nlen) && (bit > 25) && ((bit != lasterr_point) || !maxerr_recovery_mode[6])) {
			gwsquare (gwdata, x);
			care = FALSE;
		}
		else {
			gwsquare_carefully (gwdata, x);
			care = TRUE;
		}

		if (debug && (bit < 50))
			writeresidue (gwdata, x, N, tmp, buf, str, bit, BIT);
		CHECK_IF_ANY_ERROR (x, (bit), Nlen, 6);

/* That iteration succeeded, bump counters */

		if (bit == lasterr_point)
			saving = 1;					// Be sure to restart after this recovery iteration!
		bit++;
		iters++;

/* Print a message every so often */

		if (bit % ITER_OUTPUT == 0) {
			char	fmt_mask[80];
			double	pct;
			pct = trunc_percent (bit * 100.0 / Nlen);
			if (strlen (str) < 40) {
				sprintf (fmt_mask, "%%.%df%%%% of %%s", PRECISION);
				sprintf (buf, fmt_mask, pct, str);
			}
			else {
				sprintf (fmt_mask, "%%.%df%%%% of %%ld", PRECISION);
				sprintf (buf, fmt_mask, pct, Nlen);
			}
			title (buf);
			ReplaceableLine (2);	/* Replace line */
			sprintf (fmt_mask,
				 "%%s, bit: %%ld / %%ld [%%.%df%%%%]",
				 PRECISION);
			sprintf (buf, fmt_mask, str, bit, Nlen, pct);
			OutputStr (buf);
			if (ERRCHK && bit > 30) {
				OutputStr (".  Round off: ");
				sprintf (buf, "%10.10f", reallyminerr);
				OutputStr (buf);
				sprintf (buf, " to %10.10f", reallymaxerr);
				OutputStr (buf);
			}
			end_timer (0);
			if (CUMULATIVE_TIMING) {
				OutputStr (".  Time thusfar: ");
			} else {
				OutputStr (".  Time per bit: ");
				divide_timer (0, iters);
				iters = 0;
			}
			print_timer (0, TIMER_NL | TIMER_OPT_CLR);
			start_timer (0);
		}

/* Print a results file message every so often */

		if (bit % ITER_OUTPUT_RES == 0 || (NO_GUI && stopping)) {
			sprintf (buf, "Bit %ld / %ld\n", bit, Nlen);
			writeResults (buf);
		}

/* Write results to a file every DISK_WRITE_TIME minutes */
/* On error, retry in 10 minutes (it could be a temporary */
/* disk-full situation) */

		if (saving || stopping) {
			write_time = DISK_WRITE_TIME * 60;
			saving = FALSE;
			if (! writeToFile (gwdata, gdata, filename, bit, x, NULL)) {
				sprintf (buf, WRITEFILEERR, filename);
				OutputBoth (buf);
				if (write_time > 600) write_time = 600;
			}
			time (&start_time);

/* If an escape key was hit, write out the results and return */

			if (stopping) {
				pushg (gdata, 2);
				gwfree (gwdata, x);
				*res = FALSE;
				return (FALSE);
			}
		}

/* Output the 64-bit residue at specified interims.  Also output the */
/* residues for the next iteration so that we can compare our */
/* residues to programs that start counter at zero or one. */

		if (interimResidues && bit % interimResidues < 2)
				writeresidue (gwdata, x, N, tmp, buf, str, bit, BIT);

/* Write a save file every "interimFiles" iterations. */

		if (interimFiles && bit % interimFiles == 0) {
			char	interimfile[20];
			sprintf (interimfile, "%.8s.%03lu",
				 filename, bit / interimFiles);
			if (! writeToFile (gwdata, gdata, interimfile, bit, x, NULL)) {
				sprintf (buf, WRITEFILEERR, interimfile);
				OutputBoth (buf);
			}
		}
	}

/* See if we've found a probable prime.  If not, format a 64-bit residue. */
/* Old versions of PRP used a non-standard 64-bit residue, computing */
/* 3^N-3 mod N rather than the more standard 3^(N-1) mod N.  Since */
/* some projects recorded these non-standard residues, output that */
/* residue too.  Note that some really old versions printed out the */
/* 32-bit chunks of the non-standard residue in reverse order. */

	clearline (100);

	gwtogiant (gwdata, x, tmp);
	if (!isone (tmp)) {
		*res = FALSE;	/* Not a prime */
		if (abs(tmp->sign) < 2)	// make a 32 bit residue correct !!
			sprintf (res64, "%08lX%08lX", (unsigned long)0, (unsigned long)tmp->n[0]);
		else
			sprintf (res64, "%08lX%08lX", (unsigned long)tmp->n[1], (unsigned long)tmp->n[0]);
		imulg (3, tmp); specialmodg (gwdata, tmp); ulsubg (3, tmp);
		if (abs(tmp->sign) < 2)	// make a 32 bit residue correct !!
			sprintf (oldres64, "%08lX%08lX", (unsigned long)0, (unsigned long)tmp->n[0]);
		else
			sprintf (oldres64, "%08lX%08lX", (unsigned long)tmp->n[1], (unsigned long)tmp->n[0]);
	}

/* Print results.  Do not change the format of this line as Jim Fougeron of */
/* PFGW fame automates his QA scripts by parsing this line. */

	if (*res)
		sprintf (buf, "%s is prime! (%lu decimal digits)", str, nbdg);
	else if (IniGetInt (INI_FILE, "OldRes64", 0))
		sprintf (buf, "%s is not prime.  RES64: %s.  OLD64: %s", str, res64, oldres64);
	else
		sprintf (buf, "%s is not prime.  RES64: %s", str, res64);

#if defined(WIN32) && !defined(_CONSOLE)

	sprintf (buf+strlen(buf), "  Time : "); 
	ReplaceableLine (2);	/* Replace line */ 

#else

	clearline(100);

	if (*res) {
#if defined (_CONSOLE)
		hConsole = GetStdHandle(STD_OUTPUT_HANDLE);	// Access to Console attributes
		SetConsoleTextAttribute(hConsole, BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
		OutputBoth(buf);
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
#else
		OutputStr("\033[7m");
		OutputBoth(buf);
		OutputStr("\033[0m");
#endif
	}
	else
		OutputBoth(buf);

	sprintf (buf, "  Time : "); 

#endif


/* Output the final timings */

	end_timer (1);
	write_timer (buf+strlen(buf), 1, TIMER_CLR | TIMER_NL); 
	OutputBoth (buf);

/* Cleanup and return */

	pushg (gdata, 2);
	gwfree (gwdata, x);
	_unlink (filename);
	lasterr_point = 0;
	return (TRUE);

/* An error occured, sleep if required, then try restarting at last save point. */

error:
	pushg (gdata, 2);
	gwfree (gwdata, x);
	*res = FALSE;					// To avoid credit mesage...

	if ((abonillsum && gw_test_illegal_sumout(gwdata)) || 
		(abonmismatch && gw_test_mismatched_sums (gwdata)) || 
		(abonroundoff && gw_get_maxerr(gwdata) > maxroundoff)) {	// Abort...
		aborted = TRUE;
		sprintf (buf, ERRMSG5, checknumber, str);
		OutputBoth (buf);
		_unlink (filename);
		if(IniGetInt(INI_FILE, "StopOnAbort", 0)) {
			IniWriteInt (INI_FILE, "PgenLine", IniGetInt(INI_FILE, "PgenLine", 0) + 1);	// Point on the next line
			return (FALSE);
		}
		else
			return (TRUE);
	}

/* Output a message saying we are restarting */

	if (sleep5) OutputBoth (ERRMSG2);
	OutputBoth (ERRMSG3);

/* Sleep five minutes before restarting */

	if (sleep5 && ! SleepFive ()) {
		return (FALSE);
	}

/* Restart */

	if (will_try_larger_fft) {
		IniWriteInt(INI_FILE, "FFT_Increment", nbfftinc = (IniGetInt(INI_FILE, "FFT_Increment", 0) + 1));
		if (nbfftinc == maxfftinc)
			abonroundoff = TRUE;	// Don't accept any more Roundoff error.
	}
	return (-1);
}

/* Test for 2*N-1 prime, knowing that N is prime -- gwsetup has already been called. */

int commonCC2P (
	gwhandle *gwdata,
	ghandle *gdata,
	unsigned long P,
	int *res,
	char *str)
{
	char	filename[20], buf[sgkbufsize+256], fft_desc[256]; 
	unsigned long bits, explen, iters, bit, bitv, D, mask=0x80000000, frestart=FALSE;
	unsigned long nreduced, gcdn;
	long	write_time = DISK_WRITE_TIME * 60;
	int	echk, saving, stopping;
	time_t	start_time, current_time;
	double	reallyminerr = 1.0;
	double	reallymaxerr = 0.0;
	giant exponent, tmp, tmp2, tmp3;
	gwnum x, y, gwinvD;

/* First, test if gcd (U(2), N) is one... */

	nreduced = gmodi (P, N);
	if (!nreduced)
		gcdn = P;
	else
		gcdn = gcd (P, nreduced);
	if (gcdn != 1) {
		sprintf (buf, "%s has a small factor : %lu!!\n", str, gcdn);
		OutputBoth (buf);
		*res = FALSE;
		return (TRUE);
	}

	if (!IniGetInt(INI_FILE, "Testdiff", 0))	// Unless test of MAXDIFF explicitly required
		gwdata->MAXDIFF = 1.0E80;				// Diregard MAXDIFF...

	x = gwalloc (gwdata);					// allocate memory for the gwnums
	y = gwalloc (gwdata);
	gwinvD = gwalloc (gwdata);

	bits = bitlen (N);
	nbdg = gnbdg (N, 10); // Compute the number of decimal digits of the tested number.

	exponent = allocgiant ((bits >> 4) + 8);	// Allocate memory for exponent
	tmp = allocgiant ((bits >> 3) + 8);		// Allocate memory for tmp
	tmp2 = allocgiant ((bits >> 4) + 8);		// Allocate memory for tmp2
	tmp3 = allocgiant ((bits >> 4) + 8);		// Allocate memory for tmp3

	gtog (N, exponent);
	uladdg (1, exponent);					// exponent = modulus + 1

	explen = bitlen (exponent);

	*res = TRUE;							/* Assume a positive result */

/* Init filename */

	tempFileName (filename, 'L', N);

/* Optionally resume from save file and output a message */
/* indicating we are resuming a test */

	if (fileExists (filename) && readFromFile (gwdata, gdata, filename, &bit, x, y)) {
		char	fmt_mask[80];
		double	pct;
		pct = trunc_percent (bit * 100.0 / explen);
		sprintf (fmt_mask,
			 "Resuming Lucas sequence at bit %%ld [%%.%df%%%%]\n",
			 PRECISION);
		sprintf (buf, fmt_mask, bit, pct);
		OutputStr (buf);
		if (verbose || restarting)
			writeResults (buf);
		D = P*P-4;
	}

/* Otherwise, output a message indicating we are starting test */

	else {
		clear_timers ();	// Init. timers
		D = P*P-4;
		if (showdigits)
			sprintf (buf, "Starting Lucas sequence (%lu decimal digits)\n", nbdg);
		else
			sprintf (buf, "Starting Lucas sequence\n");
		OutputStr (buf);
		if (verbose || restarting)
			writeResults (buf);
		bit = 1;
		dbltogw (gwdata, 2.0, x);
		dbltogw (gwdata, (double)P, y);
	}

	ultog (D, tmp3);						// Compute the inverse of D modulo N
	invg (N, tmp3);
	gianttogw (gwdata, tmp3, gwinvD);		// Convert it to gwnum
//	gwfft (gwdata, gwinvD, gwinvD);			// And fft it

/* Get the current time */

	start_timer (0);
	start_timer (1);
	time (&start_time);

/* Output a message about the FFT length */

	topology_print_children (hwloc_get_root_obj (hwloc_topology), 0);
	gwfft_description (gwdata, fft_desc);
	sprintf (buf, "Using %s, P = %lu\n", fft_desc, P);

	OutputStr (buf);
	if (verbose || restarting) {
		writeResults (buf);
	}

	title ("Morrison prime test in progress...");

	ReplaceableLine (1);	/* Remember where replaceable line is */

	iters = 0;
	gwsetnormroutine (gwdata, 0, 1, 0);

	while (bit <= explen) {

/* Error check the first and last 50 iterations, before writing an */
/* intermediate file (either user-requested stop or a */
/* 30 minute interval expired), and every 128th iteration. */

		stopping = stopCheck ();
		echk = stopping || ERRCHK || (bit <= 50) || (bit >= Nlen-50) || gwnear_fft_limit (gwdata, pcfftlim);
		if (((bit & 127) == 0) || (bit == 1) || (bit == (lasterr_point-1))) {
			echk = 1;
			time (&current_time);
			saving = ((current_time - start_time > write_time) || (bit == 1) || (bit == (lasterr_point-1)));
		} else
			saving = 0;

/* Process this bit */

		gwsetnormroutine (gwdata, 0, echk, 0);
//		gwstartnextfft (postfft && !debug && !stopping && !saving && bit != lasterr_point && (bit+26 < Nlen) && (bit > 26));

		gwstartnextfft (gwdata, FALSE);

		if ((bitv = bitval (exponent, explen-bit))) {
			if (abs(gwdata->c) == 1)
				gwsetaddin (gwdata, -(int)P);
			if ((bit+26 < explen) && (bit > 26) &&
				((bit != lasterr_point) || !maxerr_recovery_mode[1])) {
				gwsafemul (gwdata, y, x);
				care = FALSE;
			}
			else {
				gwmul_carefully (gwdata, y, x);
				care = TRUE;
			}
			if (debug && (bit < 50))
				writeresidue (gwdata, x, N, tmp, buf, str, bit, BIT);
			CHECK_IF_ANY_ERROR(x, (bit), explen, 1)
			if (abs(gwdata->c) != 1)
				gwsmalladd (gwdata, -(double)P, x);
			if (abs(gwdata->c) == 1)
				gwsetaddin (gwdata, -2);
			if ((bit+26 < explen) && (bit > 26) &&
				((bit != lasterr_point) || !maxerr_recovery_mode[2])) {
				gwsquare (gwdata, y);
				care = FALSE;
			}
			else {
				gwsquare_carefully (gwdata, y);
				care = TRUE;
			}
			if (debug && (bit < 50))
				writeresidue (gwdata, y, N, tmp, buf, str, bit, BIT);
			CHECK_IF_ANY_ERROR(y, (bit), explen, 2)
			if (abs(gwdata->c) != 1)
				gwsmalladd (gwdata, -2.0, y);
		}
		else {
			if (abs(gwdata->c) == 1)
				gwsetaddin (gwdata, -(int)P);
			if ((bit+26 < explen) && (bit > 26) &&
				((bit != lasterr_point) || !maxerr_recovery_mode[3])) {
				gwsafemul (gwdata, x, y);
				care = FALSE;
			}
			else {
				gwmul_carefully (gwdata, x, y);
				care = TRUE;
			}
			if (debug && (bit < 50))
				writeresidue (gwdata, y, N, tmp, buf, str, bit, BIT);
			CHECK_IF_ANY_ERROR(y, (bit), explen, 3)
			if (abs(gwdata->c) != 1)
				gwsmalladd (gwdata, -(double)P, y);
			if (abs(gwdata->c) == 1)
				gwsetaddin (gwdata, -2);
			if ((bit+26 < explen) && (bit > 26) &&
				((bit != lasterr_point) || !maxerr_recovery_mode[4])) {
				gwsquare (gwdata, x);
				care = FALSE;
			}
			else {
				gwsquare_carefully (gwdata, x);
				care = TRUE;
			}
			if (debug && (bit < 50))
				writeresidue (gwdata, x, N, tmp, buf, str, bit, BIT);
			CHECK_IF_ANY_ERROR(x, (bit), explen, 4)
			if (abs(gwdata->c) != 1)
				gwsmalladd (gwdata, -2.0, x);
		}

 /* That iteration succeeded, bump counters */

		if (bit == lasterr_point)
			saving = 1;					// Be sure to restart after this recovery iteration!
		bit++;
		iters++;

/* Print a message every so often */

		if (bit % ITER_OUTPUT == 0) {
			char	fmt_mask[80];
			double	pct;
			pct = trunc_percent (bit * 100.0 / explen);
			sprintf (fmt_mask, "%%.%df%%%% of %%ld", PRECISION);
			sprintf (buf, fmt_mask, pct, Nlen);
			title (buf);
			ReplaceableLine (2);	/* Replace line */
			sprintf (fmt_mask,
				 "%%s, bit: %%ld / %%ld [%%.%df%%%%]",
				 PRECISION);
			sprintf (buf, fmt_mask, str, bit, explen, pct);
			OutputStr (buf);
			if (ERRCHK && bit > 30) {
				OutputStr (".  Round off: ");
				sprintf (buf, "%10.10f", reallyminerr);
				OutputStr (buf);
				sprintf (buf, " to %10.10f", reallymaxerr);
				OutputStr (buf);
			}
			end_timer (0);
			if (CUMULATIVE_TIMING) {
				OutputStr (".  Time thusfar: ");
			} else {
				OutputStr (".  Time per bit: ");
				divide_timer (0, iters);
				iters = 0;
			}
			print_timer (0, TIMER_NL | TIMER_OPT_CLR);
			start_timer (0);
		}

/* Print a results file message every so often */

		if (bit % ITER_OUTPUT_RES == 0 || (NO_GUI && stopping)) {
			sprintf (buf, "Bit %ld / %ld\n", bit, explen);
			writeResults (buf);
		}

/* Write results to a file every DISK_WRITE_TIME minutes */
/* On error, retry in 10 minutes (it could be a temporary */
/* disk-full situation) */

		if (saving || stopping) {
			write_time = DISK_WRITE_TIME * 60;
			saving = FALSE;
			if (! writeToFile (gwdata, gdata, filename, bit, x, y)) {
				sprintf (buf, WRITEFILEERR, filename);
				OutputBoth (buf);
				if (write_time > 600) write_time = 600;
			}	
			time (&start_time);

/* If an escape key was hit, write out the results and return */

			if (stopping) {
				free (tmp);
				free (tmp2);
				free (tmp3);
				gwfree (gwdata, x);				// Clean up
				gwfree (gwdata, y);
				gwfree (gwdata, gwinvD);
				*res = FALSE;
				return (FALSE);
			}
		}

/* Output the 64-bit residue at specified interims.  Also output the */
/* residues for the next iteration so that we can compare our */
/* residues to programs that start counter at zero or one. */

		if (interimResidues && bit % interimResidues < 2)
				writeresidue (gwdata, x, N, tmp, buf, str, bit, BIT);

/* Write a save file every "interimFiles" iterations. */

		if (interimFiles && bit % interimFiles == 0) {
			char	interimfile[20];
			sprintf (interimfile, "%.8s.%03lu",
				 filename, bit / interimFiles);
			if (! writeToFile (gwdata, gdata, interimfile, bit, x, y)) {
				sprintf (buf, WRITEFILEERR, interimfile);
				OutputBoth (buf);
			}
		}
	}

	clearline (100);

	care = TRUE;					// All errors are now flagged as unrecoverable...

	if (abs(gwdata->c) == 1)
		gwsetaddin (gwdata, 0);			// Reset addin constant.
	gwsetnormroutine (gwdata, 0, 1, 1);	// set mul. by const.
	gwsetmulbyconst (gwdata, 2);
//	gwfftmul (gwdata, gwinvD, y);	// y = D^-1*2*V(N+2) modulo N
	gwmul_carefully (gwdata, gwinvD, y);// y = D^-1*2*V(N+2) modulo N
	CHECK_IF_ANY_ERROR(y, (explen), explen, 4)
	gwsetmulbyconst (gwdata, P);
//	gwfftmul (gwdata, gwinvD, x);	// x = D^-1*P*V(N+1) modulo N
	gwmul_carefully (gwdata, gwinvD, x);// x = D^-1*P*V(N+1) modulo N
	CHECK_IF_ANY_ERROR(x, (explen), explen, 4)
	gwsub (gwdata, x, y);			// y = D^-1*(2*V(N+2)-P*V(N+1)) = U(N+1) modulo N
	gwsetnormroutine (gwdata, 0, 1, 0);	// reset mul by const
	gwtogiant (gwdata, y, tmp);		// tmp = U(N+1) modulo N


	if (!isZero (tmp)) {
		*res = FALSE;				// Not a prime.
		if (abs(tmp->sign) < 2)		// make a 32 bit residue correct !!
			sprintf (res64, "%08lX%08lX", (unsigned long)0, (unsigned long)tmp->n[0]);
		else
			sprintf (res64, "%08lX%08lX", (unsigned long)tmp->n[1], (unsigned long)tmp->n[0]);
		sprintf (buf, "%s is not prime. Lucas RES64: %s", str, res64);
	}
	else
		sprintf (buf, "%s is prime! (%lu decimal digits)", str, nbdg);


#if defined(WIN32) && !defined(_CONSOLE)

	sprintf (buf+strlen(buf), "  Time : "); 
	ReplaceableLine (2);	/* Replace line */ 

#else

	clearline(100);

	if (*res) {
#if defined (_CONSOLE)
		hConsole = GetStdHandle(STD_OUTPUT_HANDLE);	// Access to Console attributes
		SetConsoleTextAttribute(hConsole, BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
		OutputBoth(buf);
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
#else
		OutputStr("\033[7m");
		OutputBoth(buf);
		OutputStr("\033[0m");
#endif
	}
	else
		OutputBoth(buf);

	sprintf (buf, "  Time : "); 

#endif

/* Output the final timings */

	end_timer (1);
	write_timer (buf+strlen(buf), 1, TIMER_CLR | TIMER_NL); 
	OutputBoth (buf);

	gwfree (gwdata, x);				// Clean up
	gwfree (gwdata, y);
	gwfree (gwdata, gwinvD);
	free (tmp);
	free (tmp2);
	free (tmp3);
	free (exponent);
	_unlink (filename);
	lasterr_point = 0;
	return TRUE;

/* An error occured, sleep if required, then try restarting at last save point. */

error:

	gwfree (gwdata, x);				// Clean up
	gwfree (gwdata, y);
	gwfree (gwdata, gwinvD);
	free (tmp);
	free (tmp2);
	free (tmp3);
	free (exponent);
	*res = FALSE;

	if ((abonillsum && gw_test_illegal_sumout(gwdata)) || 
		(abonmismatch && gw_test_mismatched_sums (gwdata)) || 
		(abonroundoff && gw_get_maxerr(gwdata) > maxroundoff)) {	// Abort...
		aborted = TRUE;
		sprintf (buf, ERRMSG5, checknumber, str);
		OutputBoth (buf);
		_unlink (filename);
		if(IniGetInt(INI_FILE, "StopOnAbort", 0)) {
			IniWriteInt (INI_FILE, "PgenLine", IniGetInt(INI_FILE, "PgenLine", 0) + 1);	// Point on the next line
			return (FALSE);
		}
		else
			return (TRUE);
	}

/* Output a message saying we are restarting */

	if (sleep5) OutputBoth (ERRMSG2);
	OutputBoth (ERRMSG3);

/* Sleep five minutes before restarting */

	if (sleep5 && ! SleepFive ()) {
		return (FALSE);
	}

/* Restart */

	if (will_try_larger_fft) {
		IniWriteInt(INI_FILE, "FFT_Increment", nbfftinc = (IniGetInt(INI_FILE, "FFT_Increment", 0) + 1));
		if (nbfftinc == maxfftinc)
			abonroundoff = TRUE;	// Don't accept any more Roundoff error.
	}
	return (-1);
}

/* Test if k*2^n+c is a probable prime. */

int fastIsPRP (
	double	k,				/* k in k*b^n+c */
	unsigned long b,		/* b in k*b^n+c */
	unsigned long n,		/* n in k*b^n+c */
	signed long c,			/* c in k*b^n+c */
	char *str,
	int	*res)
{
	int	retval, a;
	gwhandle *gwdata;
	ghandle *gdata;
//	struct gwasm_data *asm_data;

/* Setup the assembly code. */

	gwdata = (gwhandle*) malloc(sizeof(gwhandle));

	if (bpsw)
		a = IniGetInt (INI_FILE, "FBase", 2);
	else
		a = IniGetInt (INI_FILE, "FBase", 3);
 	if (usingDivPhi_m) 
		a = 2;

	// init work_unit used by the Gerbicz code

	w->k = k;
	w->n = n;
	w->b = b;
	w->c = c;
	w->prp_base = a;
	w->prp_residue_type = strong? PRP_TYPE_SPRP : PRP_TYPE_FERMAT;
	if (quotient) 
		w->prp_residue_type = PRP_TYPE_COFACTOR;
	else
		w->known_factors = NULL;

	do {
restart:
		gwinit (gwdata);
		gdata = &gwdata->gdata;
 		gwset_num_threads (gwdata, IniGetInt(INI_FILE, "ThreadsPerTest", 1));
		gwset_larger_fftlen_count(gwdata, (char)IniGetInt(INI_FILE, "FFT_Increment", 0)); 
		gwsetmaxmulbyconst (gwdata, a);
		if (!setupok (gwdata, gwsetup (gwdata, k, b, n, c))) {
			free (gwdata);
			return FALSE;
		}

//		Restart with next FFT length if we are too near the limit...

		if (nextifnear && gwnear_fft_limit (gwdata, pcfftlim)) {
			OutputBoth ("Current FFT beeing too near the limit, next FFT length is forced...\n");
			IniWriteInt(INI_FILE, "FFT_Increment", IniGetInt(INI_FILE, "FFT_Increment", 0) + 1);
			gwdone (gwdata);
			goto restart;
		}

/* Do the PRP test */
		if (b == 2)
			retval = GerbiczTest (gwdata, gdata, a, res, str, w);
		else
			retval = commonPRP (gwdata, gdata, a, res, str);
		gwdone (gwdata);
	} while (retval == -1);

/* Clean up and return */

	free (gwdata);
	return (retval);
}


/* Test if k*b^n+c is the next prime in a Cunningham chain of the first kind. */

int fastIsCC1P (
	double	k,				/* k in k*b^n+c */
	unsigned long b,		/* b in k*b^n+c */
	unsigned long n,		/* n in k*b^n+c */
	signed long c,			/* c in k*b^n+c */
	char *str,
	int	*res)
{
	int	retval, a;
	gwhandle *gwdata;
	ghandle *gdata;

/* Setup the assembly code. */

	gwdata = (gwhandle*) malloc(sizeof(gwhandle));

	a = IniGetInt (INI_FILE, "FBase", 3);

	do {
restart:
		gwinit (gwdata);
		gdata = &gwdata->gdata;
 		gwset_num_threads (gwdata, IniGetInt(INI_FILE, "ThreadsPerTest", 1));
		gwset_larger_fftlen_count(gwdata, (char)IniGetInt(INI_FILE, "FFT_Increment", 0));
		gwsetmaxmulbyconst (gwdata, a);
		if (!setupok (gwdata, gwsetup (gwdata, k, b, n, c))) {
			free (gwdata);
			return FALSE;
		}

//		Restart with next FFT length if we are too near the limit...

		if (nextifnear && gwnear_fft_limit (gwdata, pcfftlim)) {
			OutputBoth ("Current FFT beeing too near the limit, next FFT length is forced...\n");
			IniWriteInt(INI_FILE, "FFT_Increment", IniGetInt(INI_FILE, "FFT_Increment", 0) + 1);
			gwdone (gwdata);
			goto restart;
		}

/* Do the Pocklington test */

		retval = commonCC1P (gwdata, gdata, a, res, str);
		gwdone (gwdata);
	} while (retval == -1);

/* Clean up and return */

	if (retval == TRUE)		// If not stopped by user...
		IniWriteString(INI_FILE, "FFT_Increment", NULL);
	free (gwdata);
	return (retval);
}

/* Test if k*2^n+c is the next prime in a Cunningham chain of the second kind. */

int fastIsCC2P (
	double	k,				/* k in k*b^n+c */
	unsigned long b,		/* b in k*b^n+c */
	unsigned long n,		/* n in k*b^n+c */
	signed long c,			/* c in k*b^n+c */
	char *str,
	int	*res)
{
	char	buf[sgkbufsize+256]; 
	int	retval, P;
	gwhandle *gwdata;
	ghandle *gdata;

/* Setup the assembly code. */

	gwdata = (gwhandle*) malloc(sizeof(gwhandle));

//	Compute P for the Morrison test (Q = 1)

	P = genLucasBaseP (N, IniGetInt (INI_FILE, "PBase", 3));
	if (P < 0) {
		if (P == -1)
			sprintf (buf, "Cannot compute P to test %s...\nThis is surprising, please, let me know that!!\nMy E-mail is jpenne@free.fr\n", str);
		else
			sprintf (buf, "%s has a small factor : %d !!\n", str, abs(P));
		OutputBoth (buf);
		free (gwdata);
		return (TRUE); 
	}

	do {
restart:
		gwinit (gwdata);
		gdata = &gwdata->gdata;
 		gwset_num_threads (gwdata, IniGetInt(INI_FILE, "ThreadsPerTest", 1));
		gwset_larger_fftlen_count(gwdata, (char)IniGetInt(INI_FILE, "FFT_Increment", 0));
		gwsetmaxmulbyconst(gwdata, P);
		if (!setupok (gwdata, gwsetup (gwdata, k, b, n, c))) {
			free (gwdata);
			return FALSE;
		}

//		Restart with next FFT length if we are too near the limit...

		if (nextifnear && gwnear_fft_limit (gwdata, pcfftlim)) {
			OutputBoth ("Current FFT beeing too near the limit, next FFT length is forced...\n");
			IniWriteInt(INI_FILE, "FFT_Increment", IniGetInt(INI_FILE, "FFT_Increment", 0) + 1);
			gwdone (gwdata);
			goto restart;
		}

/* Do the Morrison test */

		retval = commonCC2P (gwdata, gdata, P, res, str);
		gwdone (gwdata);
	} while (retval == -1);

/* Clean up and return */

	if (retval == TRUE)		// If not stopped by user...
		IniWriteString(INI_FILE, "FFT_Increment", NULL);
	free (gwdata);
	return (retval);
}

/* Test if k*b^n+c (or a factor of it) is a Frobenius probable prime. */

int fastIsFrobeniusPRP (
	double	k,				/* k in k*b^n+c */
	unsigned long b,		/* b in k*b^n+c */
	unsigned long n,		/* n in k*b^n+c */
	signed long c,			/* c in k*b^n+c */
	char *str,
	int	*res)
{
	char	buf[sgkbufsize+256]; 
	int	retval;
	gwhandle *gwdata;
	ghandle *gdata;
	long P = 3, Q = 0;
	long D, sign1, sign2, abs_d, sqrtabs_d;

/* Setup the assembly code. */

	gwdata = (gwhandle*) malloc(sizeof(gwhandle));


	if (bpsw) {
		P = 1;
		sign1 = N->n[0]&2? -1 : 1;			// sign of kronecker (-1, N)
		for (abs_d = 5;abs_d <= 2147483647;abs_d += 2) {
			sign2 = abs_d&2 ? -1 : 1;		// requested sign of D
			if ((D = isLucasBaseQ (N, abs_d, ((sign2 == -1) && (sign1 == -1))? 1 : -1)) == TRUE) {
				sqrtabs_d = (int)floor(sqrt((double)abs_d));
				D = sign2*abs_d;
				Q = (1-D)/4;
				if ((abs (Q) == 1) || ((globalk == 1.0) && ispower((unsigned long)abs (Q), globalb)) || (sqrtabs_d * sqrtabs_d == abs_d))
					continue;		// Avoid abs(Q) == 1 , k == 1 and abs(Q) power of b, or D perfect square...
				else
					break;
			}
			else if (D != FALSE) {
				sprintf (buf, "%s has a small factor : %d !!\n", str, abs(D));
				OutputBoth (buf);
				free (gwdata);
				return (TRUE); 
			}
		}
		if (D == FALSE) {
			sprintf (buf, "Unable to further test %s ; it may be a perfect square !!\n", str);
			OutputBoth (buf);
			free (gwdata);
			return (TRUE); 
		}
	}
	else {

		P = IniGetInt (INI_FILE, "PBase", 3);

		D = generalLucasBase (N, (uint32_t*)&P, (uint32_t*)&Q);
		if (D < 0) {
			if (D == -1)
				sprintf (buf, "Cannot compute D to test %s...\nThis is surprising, please, let me know that!!\nMy E-mail is jpenne@free.fr\n", str);
			else
				sprintf (buf, "%s has a small factor : %d !!\n", str, abs(D));
			OutputBoth (buf);
			free (gwdata);
			return (TRUE); 
		}
	}

	do {
restart:
		gwinit (gwdata);
		gdata = &gwdata->gdata;
 		gwset_num_threads (gwdata, IniGetInt(INI_FILE, "ThreadsPerTest", 1));
		gwset_larger_fftlen_count(gwdata, (char)IniGetInt(INI_FILE, "FFT_Increment", 0));
		if (abs(Q) <= GWMULBYCONST_MAX)
			gwsetmaxmulbyconst (gwdata, max (3, abs(Q)));
		if (!setupok (gwdata, gwsetup (gwdata, k, b, n, c))) {
			free (gwdata);
			return FALSE;
		}

//		Restart with next FFT length if we are too near the limit...

		if (nextifnear && gwnear_fft_limit (gwdata, pcfftlim)) {
			OutputBoth ("Current FFT beeing too near the limit, next FFT length is forced...\n");
			IniWriteInt(INI_FILE, "FFT_Increment", IniGetInt(INI_FILE, "FFT_Increment", 0) + 1);
			gwdone (gwdata);
			goto restart;
		}

/* Do the Frobenius PRP test */

		retval = commonFrobeniusPRP (gwdata, gdata, P, Q, res, str);
		gwdone (gwdata);

	} while (retval == -1);

/* Clean up and return */
	
//	if (retval == TRUE)		// If not stopped by user...
//		IniWriteString(INI_FILE, "FFT_Increment", NULL);
	free (gwdata);
	return (retval);
}

/* Test if N is a Frobenius probable prime.  The number N can be of ANY form. */

int slowIsFrobeniusPRP (
	char	*str,		/* string representation of N */
	int	*res)
{
	char	buf[sgkbufsize+256]; 
	int	retval;
	gwhandle *gwdata;
	ghandle *gdata;

	long P = 3, Q = 0;
	long D, sign1, sign2, abs_d, sqrtabs_d;

/* Setup the assembly code. */

	gwdata = (gwhandle*) malloc(sizeof(gwhandle));

	if (bpsw) {
		P = 1;
		sign1 = N->n[0]&2? -1 : 1;			// sign of kronecker (-1, N)
		for (abs_d = 5;abs_d <= 2147483647;abs_d += 2) {
			sign2 = abs_d&2 ? -1 : 1;		// requested sign of D
			if ((D = isLucasBaseQ (N, abs_d, ((sign2 == -1) && (sign1 == -1))? 1 : -1)) == TRUE) {
				sqrtabs_d = (int)floor(sqrt((double)abs_d));
				D = sign2*abs_d;
				Q = (1-D)/4;
				if ((abs (Q) == 1) || ((globalk == 1.0) && ispower((unsigned long)abs (Q), globalb)) || (sqrtabs_d * sqrtabs_d == abs_d))
					continue;		// Avoid abs(Q) == 1 , k == 1 and abs(Q) power of b, or D perfect square...
				else
					break;
			}
			else if (D != FALSE) {
				sprintf (buf, "%s has a small factor : %d !!\n", str, abs(D));
				OutputBoth (buf);
				free (gwdata);
				return (TRUE); 
			}
		}
		if (D == FALSE) {
			sprintf (buf, "Unable to further test %s ; it may be a perfect square !!\n", str);
			OutputBoth (buf);
			free (gwdata);
			return (TRUE); 
		}
	}
	else {
		P = IniGetInt (INI_FILE, "PBase", 3);

		D = generalLucasBase (N, (uint32_t*)&P, (uint32_t*)&Q);
		if (D < 0) {
			if (D == -1)
				sprintf (buf, "Cannot compute D to test %s...\nThis is surprising, please, let me know that!!\nMy E-mail is jpenne@free.fr\n", str);
			else
				sprintf (buf, "%s has a small factor : %d !!\n", str, abs(D));
			OutputBoth (buf);
			free (gwdata);
			return (TRUE); 
		}
	}

	do {
restart:
		gwinit (gwdata);
		gdata = &gwdata->gdata;
 		gwset_num_threads (gwdata, IniGetInt(INI_FILE, "ThreadsPerTest", 1));
		gwset_larger_fftlen_count(gwdata, (char)IniGetInt(INI_FILE, "FFT_Increment", 0));
		if (abs(Q) <= GWMULBYCONST_MAX)
			gwsetmaxmulbyconst (gwdata, max (3, abs(Q)));
		gwdata->force_general_mod = TRUE;
		if (!setupok (gwdata, gwsetup_general_mod_giant (gwdata, N))) {
			free (gwdata);
			return FALSE;
		}

//		Restart with next FFT length if we are too near the limit...

		if (nextifnear && gwnear_fft_limit (gwdata, pcfftlim)) {
			OutputBoth ("Current FFT beeing too near the limit, next FFT length is forced...\n");
			IniWriteInt(INI_FILE, "FFT_Increment", IniGetInt(INI_FILE, "FFT_Increment", 0) + 1);
			gwdone (gwdata);
			goto restart;
		}

/* Do the Frobenius PRP test */

		retval = commonFrobeniusPRP (gwdata, gdata, P, Q, res, str);
		gwdone (gwdata);

	} while (retval == -1);

/* Clean up and return */

//	if (retval == TRUE)		// If not stopped by user...
//		IniWriteString(INI_FILE, "FFT_Increment", NULL);
	free (gwdata);
	return (retval);
}

/* Test if N is a Wieferich prime.  The number N can be of ANY form. 
	Memory for N and M = N^2 must be previously allocated */

int gwslowIsWieferich (
	char	*str,		/* string representation of N */
	int	*res, int shownegs)
{
	int	a, retval, resaprcl;
	char	buf[sgkbufsize+256]; 
	gwhandle *gwdata;
	ghandle *gdata;
//	M = allocgiant ((bitlen (N) >> 3) + 8);
	nbdg = gnbdg (N, 10); // Compute the number of decimal digits of the tested number.

	gtog (N, M);
	squareg (M);

/* Setup the gwnum code */

	gwdata = (gwhandle*) malloc(sizeof(gwhandle));

	a = IniGetInt (INI_FILE, "FBase", 2);

	do {
restart:
		gwinit (gwdata);
		gdata = &gwdata->gdata;
 		gwset_num_threads (gwdata, IniGetInt(INI_FILE, "ThreadsPerTest", 1));
		gwset_larger_fftlen_count(gwdata, (char)IniGetInt(INI_FILE, "FFT_Increment", 0));
		gwsetmaxmulbyconst (gwdata, a);
		if (!setupok (gwdata, gwsetup_general_mod_giant (gwdata, M))) {
			free (gwdata);
			return FALSE;
		}

//		Restart with next FFT length if we are too near the limit...

		if (nextifnear && gwnear_fft_limit (gwdata, pcfftlim)) {
			OutputBoth ("Current FFT beeing too near the limit, next FFT length is forced...\n");
			IniWriteInt(INI_FILE, "FFT_Increment", IniGetInt(INI_FILE, "FFT_Increment", 0) + 1);
			gwdone (gwdata);
			goto restart;
		}

/* Do the divisibility test */

		retval = isexpdiv (gwdata, gdata, a, N, M, res);
		gwdone (gwdata);

	} while (retval == -1);

	if (retval) {
		if (*res) {
			if (nbdg <= 1000) {
				resaprcl = gaprcltest (N,0,0);
				if ((resaprcl == 12) || (resaprcl == 2))
					sprintf (buf, "%s is a Base %d Wieferich prime!! (%lu decimal digits)\n", str, a, nbdg);
				else
					sprintf (buf, "%s is not a Base %d Wieferich prime, because not prime! (%lu decimal digits)\n", str, a, nbdg);
			}
			else {
				sprintf (buf, "%s may be a Base %d Wieferich prime,if prime! (%lu decimal digits)\n", str, a, nbdg);
			}
		}
		else {
			sprintf (buf, "%s is not a Base %d Wieferich prime. RES64: %s\n", str, a, res64);
		}

		if (shownegs || *res)
			OutputBoth (buf);
	}



/* Clean up and return */

//	free (M);
	if (retval == TRUE)		// If not stopped by user...
		IniWriteString(INI_FILE, "FFT_Increment", NULL);
	free (gwdata);
	return (retval);
}

int	gmpisWieferich (
	char	*str,	// string representation of N
	char	*base,	// string representation of the base
	int	*res)
{
	char buf [100];
	int retval = TRUE, retcode;
	retcode = gmpwftest (str, base);
	if (base == NULL)
		base = "2";
	if (retcode >= 2) {
		*res = TRUE;
		sprintf (buf, "%s is a base %s Wieferich prime!! (exp. = %d)\n", str, base, retcode);
	}
	else {
		*res = FALSE;
		if (retcode == 0)
			sprintf (buf, "%s is composite.\n", str);
		else if (retcode == 1)
			sprintf (buf, "%s is a base %s W-negative...\n", str, base);
		else if (retcode == -1000000000) {
			retval = FALSE;
			sprintf (buf, "Wieferich prime test not available for %s\n", str);
		}
		else if (retcode <= -2)
			sprintf (buf, "%s is a base %s W-positive, but composite... (exp. = %d)\n", str, base, -retcode);
		else if (retcode == -1) {
			retval = FALSE;
			sprintf (buf, "The gmpw test of %s got an error...\n", str);
		}
	}
	if (*res) {
		clearline (100);
#if defined (__linux__) || defined (__FreeBSD__) || defined (__APPLE__)
		OutputStr("\033[7m");
		OutputBoth (buf);
		OutputStr("\033[0m");
#else
#if defined (_CONSOLE)
		hConsole = GetStdHandle(STD_OUTPUT_HANDLE);	// Access to Console attributes
		SetConsoleTextAttribute(hConsole, BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
		OutputBoth(buf);
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
		clearline (100);
#else
		OutputBoth(buf);
#endif
#endif
		clearline (100);
	}
	else
		if (verbose)
			OutputBoth (buf);
		else
			OutputStr (buf);
	return (retval);
}

/* Test if N is a probable prime.  The number N can be of ANY form. */

int slowIsPRP (
	char	*str,		/* string representation of N */
	int	*res)
{
	int	retval, a;
	gwhandle *gwdata;
	ghandle *gdata;

/* Setup the gwnum code */

	gwdata = (gwhandle*) malloc(sizeof(gwhandle));

	if (bpsw)
		a = IniGetInt (INI_FILE, "FBase", 2);
	else
		a = IniGetInt (INI_FILE, "FBase", 3);


	// init work_unit used by the Gerbicz code

	w->prp_base = a;
	w->prp_residue_type = strong? PRP_TYPE_SPRP : PRP_TYPE_FERMAT;
	if (quotient) 
		w->prp_residue_type = PRP_TYPE_COFACTOR;
	else
		w->known_factors = NULL;

	do {
restart:
		gwinit (gwdata);
		gdata = &gwdata->gdata;
 		gwset_num_threads (gwdata, IniGetInt(INI_FILE, "ThreadsPerTest", 1));
		gwset_larger_fftlen_count(gwdata, (char)IniGetInt(INI_FILE, "FFT_Increment", 0));
		gwsetmaxmulbyconst (gwdata, a);
		if (!setupok (gwdata, gwsetup_general_mod_giant (gwdata, N))) {
			free (gwdata);
			return FALSE;
		}

//		Restart with next FFT length if we are too near the limit...

		if (nextifnear && gwnear_fft_limit (gwdata, pcfftlim)) {
			OutputBoth ("Current FFT beeing too near the limit, next FFT length is forced...\n");
			IniWriteInt(INI_FILE, "FFT_Increment", IniGetInt(INI_FILE, "FFT_Increment", 0) + 1);
			gwdone (gwdata);
			goto restart;
		}

/* Do the PRP test */
		if (w->b == 2) {
			retval = GerbiczTest (gwdata, gdata, a, res, str, w);
		}
		else {
			retval = commonPRP (gwdata, gdata, a, res, str);
		}
		gwdone (gwdata);
	} while (retval == -1);

/* Clean up and return */

//	if (retval == TRUE)		// If not stopped by user...
//		IniWriteString(INI_FILE, "FFT_Increment", NULL);
	free (gwdata);
	return (retval);
}

/* Test if N is the next prime in a CC1 chain.  The number N can be of ANY form. */

int slowIsCC1P (
	char	*str,		/* string representation of N */
	int	*res)
{
	int	retval, a;
	gwhandle *gwdata;
	ghandle *gdata;

/* Setup the gwnum code */

	gwdata = (gwhandle*) malloc(sizeof(gwhandle));

	a = IniGetInt (INI_FILE, "FBase", 3);

	do {
restart:
		gwinit (gwdata);
		gdata = &gwdata->gdata;
 		gwset_num_threads (gwdata, IniGetInt(INI_FILE, "ThreadsPerTest", 1));
		gwset_larger_fftlen_count(gwdata, (char)IniGetInt(INI_FILE, "FFT_Increment", 0));
		gwsetmaxmulbyconst (gwdata, a);
		if (!setupok (gwdata, gwsetup_general_mod_giant (gwdata, N))) {
			free (gwdata);
			return FALSE;
		}

//		Restart with next FFT length if we are too near the limit...

		if (nextifnear && gwnear_fft_limit (gwdata, pcfftlim)) {
			OutputBoth ("Current FFT beeing too near the limit, next FFT length is forced...\n");
			IniWriteInt(INI_FILE, "FFT_Increment", IniGetInt(INI_FILE, "FFT_Increment", 0) + 1);
			gwdone (gwdata);
			goto restart;
		}

/* Do the Pocklington test */

		retval = commonCC1P (gwdata, gdata, a, res, str);
		gwdone (gwdata);
	} while (retval == -1);

/* Clean up and return */

	if (retval == TRUE)		// If not stopped by user...
		IniWriteString(INI_FILE, "FFT_Increment", NULL);
	free (gwdata);
	return (retval);
}

/* Test if N is the next prime in a CC2 chain.  The number N can be of ANY form. */

int slowIsCC2P (
	char	*str,				/* string representation of N */
	int	*res)
{
	char	buf[sgkbufsize+256]; 
	int	retval, P;
	gwhandle *gwdata;
	ghandle *gdata;

/* Setup the gwnum code */

	gwdata = (gwhandle*) malloc(sizeof(gwhandle));

//	Compute P for the Morrison test (Q = 1)

	P = genLucasBaseP (N, IniGetInt (INI_FILE, "PBase", 3));
	if (P < 0) {
		if (P == -1)
			sprintf (buf, "Cannot compute P to test %s...\nThis is surprising, please, let me know that!!\nMy E-mail is jpenne@free.fr\n", str);
		else
			sprintf (buf, "%s has a small factor : %d !!\n", str, abs(P));
		OutputBoth (buf);
		free (gwdata);
		return (TRUE); 
	}

	do {
restart:
		gwinit (gwdata);
		gdata = &gwdata->gdata;
 		gwset_num_threads (gwdata, IniGetInt(INI_FILE, "ThreadsPerTest", 1));
		gwset_larger_fftlen_count(gwdata, (char)IniGetInt(INI_FILE, "FFT_Increment", 0));
		gwsetmaxmulbyconst(gwdata, P);
		if (!setupok (gwdata, gwsetup_general_mod_giant (gwdata, N))) {
			free (gwdata);
			return FALSE;
		}

//		Restart with next FFT length if we are too near the limit...

		if (nextifnear && gwnear_fft_limit (gwdata, pcfftlim)) {
			OutputBoth ("Current FFT beeing too near the limit, next FFT length is forced...\n");
			IniWriteInt(INI_FILE, "FFT_Increment", IniGetInt(INI_FILE, "FFT_Increment", 0) + 1);
			gwdone (gwdata);
			goto restart;
		}

/* Do the Morrison test */

		retval = commonCC2P (gwdata, gdata, P, res, str);
		gwdone (gwdata);
	} while (retval == -1);

/* Clean up and return */

	if (retval == TRUE)		// If not stopped by user...
		IniWriteString(INI_FILE, "FFT_Increment", NULL);
	free (gwdata);
	return (retval);
}

/* Test if a small N is a probable prime. */
/* Compute b^(N-1) mod N */

int isProbablePrime (void)
{
	int	retval;
	giant	x;

	if (isone (N)) return (FALSE);
	x = allocgiant (N->sign + 8);
	itog (IniGetInt (INI_FILE, "FBase", 3), x);
	powermodg (x, N, N);
	iaddg (-IniGetInt (INI_FILE, "FBase", 3), x);
	retval = isZero (x);
	free (x);
	return (retval);
}

int isPRPinternal (
	char *str,
	double dk, 
	giant gb,
	unsigned long n,
	int incr,
	int *res)
{
//	J.P. shadow        char buf[100];
	char	filename[20], buf[sgkbufsize+256];	// Lei - not need such long char
	unsigned long retval, fcontinue = FALSE, smallbase = 0;

	if (abs(gb->sign) == 1)						// Test if the base is a small integer
		smallbase = gb->n[0];
	w->k = dk;
	w->b = smallbase;
	w->n = n;
	w->c = incr;								// Transmit c to Gerbicz code!

	tempFileName (filename, 'L', N);			// See if resuming a Lucas or Frobenius PRP test
	fcontinue = fileExists (filename);
	tempFileName (filename, 'F', N);
	fcontinue = fcontinue || fileExists (filename);


	if (dk >= 1.0) {
		if (fcontinue || IniGetInt(INI_FILE, "LucasPRPtest", 0)) {
			if (!fcontinue)
				clear_timers ();				// Init. timers
			retval = fastIsFrobeniusPRP (dk, smallbase, n, incr, str, res);
		}
		else {
			retval = fastIsPRP (dk, smallbase, n, incr, str, res);
			if (retval && *res && !Fermat_only && !IniGetInt(INI_FILE, "FermatPRPtest", 0))
				retval = fastIsFrobeniusPRP (dk, smallbase, n, incr, str, res);
			else if (retval == TRUE)		// If not stopped by user...
				IniWriteString(INI_FILE, "FFT_Increment", NULL);
		}
	}
	else if (Nlen < 50) {
		*res = isProbablePrime();
		if (*res)
		{
#ifndef	WIN32
			OutputStr("\033[7m");
#endif
			sprintf (buf, "%s is a probable prime.\n", str);
			OutputBoth(buf);
#ifndef	WIN32
			OutputStr("\033[0m");
#endif
		}
		else {
			sprintf (buf, "%s is not prime.\n", str);
			OutputBoth (buf);
		}
		retval = TRUE;
	}
	else {
		if (fcontinue || IniGetInt(INI_FILE, "LucasPRPtest", 0)) {
			if (!fcontinue)
				clear_timers ();				// Init. timers
			retval = slowIsFrobeniusPRP (str, res);
		}
		else {
			retval = slowIsPRP (str, res);
			if (retval && *res && !Fermat_only && !IniGetInt(INI_FILE, "FermatPRPtest", 0))
				retval = slowIsFrobeniusPRP (str, res);
			else if (retval == TRUE)		// If not stopped by user...
				IniWriteString(INI_FILE, "FFT_Increment", NULL);
		}
	}
	return retval;
}

#define NPG		0					// NEWPGEN output format, not AP mode
#define NPGAP	1					// NEWPGEN output format, AP mode
#define	ABCC	2					// Carol ABC format
#define	ABCK	3					// Kynea ABC format
#define ABCCW	4					// Cullen/Woodall ABC format
#define ABCFF	5					// FermFact output ABC format
#define ABCGM	6					// Gaussian Mersenne ABC format
#define ABCLEI  7					// Lei ABC format
#define ABCSP	8					// (2^n+1)/3 ABC format
#define NPGCC1	9					// Cunningham chain first kind
#define NPGCC2	10					// Cunningham chain second kind


// New ABC formats for k*b^n+c

#define ABCFKGS		11				// Fixed k:  b and n specified on each input line
#define ABCFKAS		12				// Fixed k:  b, n, and c specified on each input line
#define ABCFBGS		13				// Fixed b:  k and n specified on each input line
#define ABCFBAS		14				// Fixed b:  k, n, and c specified on each input line
#define ABCFNGS		15				// Fixed n:  k and b specified on each input line
#define ABCFNAS		16				// Fxied n:  k, b, and c specified on each input line
#define ABCVARGS	17				// k, b, and n specified on each input line
#define ABCVARAS	18				// k, b, n, and c specified on each input line
#define ABCVARAQS	19				// k, b, n, c and divisor specified on each input line
#define	ABCRU		20				// (10^n-1)/9 Repunits
#define	ABCGRU		21				// (b^n-1)/(b-1) Generalized Repunits
#define ABCGF		22				// ABC format for generalized Fermat numbers
#define ABCDN		23				// b^n-b^m+c format, m < n <= 2*m
#define ABCDNG		24				// General b^n-b^m+c format, m < n <= 2*m
#define ABCWFT		25				// Format used for Wieferich test
#define ABCWFS		26				// Format used for Wieferich search
#define ABCGPT		27				// Format used for General prime test (APRCL)
#define ABCDP		28				// Format used for DivPhi()

int IsPRP (							// General PRP test
	unsigned long format, 
	char *sgk,
	char *sgb,
	giant gb,
	unsigned long n, 
	int incr,
	unsigned long shift,
	int	*res) 
{  
	char	str[sgkbufsize+256], sgk1[sgkbufsize], buf[sgkbufsize+256]; 
	unsigned long bits, retval, smallbase = 0;
	int resaprcl;
	double dk;
	giant gd, gr;
	gwhandle *gwdata;
	ghandle *gdata;

/* Setup the gwnum code. */

	gwdata = (gwhandle*) malloc(sizeof(gwhandle));
	gwinit (gwdata);
	gdata = &gwdata->gdata;
	strong = IniGetInt (INI_FILE, (char*)"StrongFermat", 0);
	fpart = 0.0;							// clear this value
	if (abs(gb->sign) == 1)					// Test if the base is a small integer
		smallbase = gb->n[0];

	if (format == ABCRU || format == ABCGRU) {	// Repunits or Generalized Repunits
		if (smallbase)
			sprintf (str, "(%lu^%lu-1)/%lu", smallbase, n, smallbase-1);
		else
			sprintf (str, "(%s^%lu-1)/(%s-1)", sgb, n, sgb);
		gk = allocgiant (1);
		setone (gk);
	}
 	else if (format == ABCDP) {				// DivPhi
 		if (smallbase)
 			sprintf (str, "%s*%lu^%lu+1", sgk, smallbase, n);
 		else
 			sprintf (str, "%s*%s^%lu+1", sgk, sgb, n);
		gk = allocgiant (strlen(sgk)/2 + 8);	// Allocate one byte per decimal digit + spares
 		ctog (sgk, gk);						// Convert k string to giant
 		strong = 0;
 	}
	else if (!(format == ABCC || format == ABCK)) {
		gk = allocgiant (strlen(sgk)/2 + 8);	// Allocate one byte per decimal digit + spares
		ctog (sgk, gk);						// Convert k string to giant
		gshiftleft (shift, gk);				// Shift k multiplier if requested
		gtoc (gk, sgk1, sgkbufsize);		// Updated k string
		if (mask & MODE_DUAL) {
			sprintf (str, "%s^%lu%c%d", sgb, n, incr < 0 ? '-' : '+', abs(incr));
		}
		else if (format != NPGAP) {			// Not MODE_AP
			if (!strcmp(sgk1, "1"))
				if (format == ABCVARAQS) {
					char *p;
					while ((p = strchr (sgd,'.'))!=NULL)
						*p = '/';
					sprintf (str, "(%s^%lu%c%d)/%s", sgb, n, incr < 0 ? '-' : '+', abs(incr), sgd);
				}
				else
					sprintf (str, "%s^%lu%c%d", sgb, n, incr < 0 ? '-' : '+', abs(incr));
			else
				if (format == ABCVARAQS) {
					char *p;
					while ((p = strchr (sgd,'.'))!=NULL)
						*p = '/';
					sprintf (str, "(%s*%s^%lu%c%d)/%s", sgk1, sgb, n, incr < 0 ? '-' : '+', abs(incr), sgd);
				}
				else
					if ((n != 0) || (incr != 0))
						sprintf (str, "%s*%s^%lu%c%d", sgk1, sgb, n, incr < 0 ? '-' : '+', abs(incr));
					else
						sprintf (str, "%s", sgk1);
		}
		else {								// MODE_AP
			if (!strcmp(sgk1, "1"))
				sprintf (str, "%s^%lu+1", sgb, n);
			else
				sprintf (str, "%s^%lu+2*%s-1", sgb, n, sgk1);
		}
	}
	else {
		gk = allocgiant ((n>>4)+8);
		setone (gk);						// Compute k multiplier
		gshiftleft (n-2, gk);				// Warning : here, n is exponent+1 !
		if (format == ABCK) {
			uladdg (1, gk);
			sprintf (str, "%s*2^%lu%c1 = (2^%lu+1)^2 - 2", sgk, n, '-', n-1);
		}
		else {
			ulsubg (1, gk);
			sprintf (str, "%s*2^%lu%c1 = (2^%lu-1)^2 - 2", sgk, n, '-', n-1);
		}
	}

	if ((gformat == ABCDN) || (gformat == ABCDNG)) {// Compute gk = gb^(n-m)-1
		bits = ndiff*bitlen (gb);
		gk = allocgiant ((bits >> 4) + 8);
		gtog (gb, gk);
		power (gk, ndiff);
		iaddg (-1, gk);
		sprintf (str, "%s^%lu-%s^%lu%c%d", sgb, n+ndiff, sgb, n, incr < 0 ? '-' : '+', abs(incr));
	}

	if (smallbase)
		bits = (unsigned long) ((n * log((double) smallbase)) / log(2.0) + bitlen(gk));
	else
		bits = n * bitlen(gb) + bitlen(gk);
	N =  allocgiant ((bits >> 4) + 8);		// Allocate memory for N

//	Compute the number we are testing.

 	if (format == ABCDP) {
 		globalk = dk = (double)gk->n[0]; 
 		Fermat_only = TRUE;
 		for(usingDivPhi_m = n; usingDivPhi_m > 0; usingDivPhi_m--) {
 			sprintf (buf, "Starting test if %s divides Phi(%s^%d,2)\n", str, sgb, usingDivPhi_m);
			OutputStr (buf);
 			gtog (gb, N);
 			power (N, usingDivPhi_m);
 			iaddg (1, N);
 			retval = isPRPinternal (str, dk, gb, n, incr, res);
 			if(!*res) break;
 		}
 		if(usingDivPhi_m<(int)n) {
			sprintf(buf, "Conclusion: %s Divides Phi(%s^%d,2)\n", str, sgb, usingDivPhi_m+1);
			OutputBoth (buf);
		}
 		Fermat_only = usingDivPhi_m = FALSE;
 		free (N);
 		free (gk);
 		return retval;
 	}

	gtog (gb, N);
	power (N, n);

	if (format == NPGAP) {				// mode AP
		addg(gk, N);
		addg(gk, N);
	}
	else {								// not mode AP
		mulg (gk, N); 
	}

	iaddg (incr, N);

	if (format == ABCRU || format == ABCGRU) {
		if (!isPrime (n)) {
			sprintf (buf, "%s is not prime because %lu is not prime!\n", str, n);
			OutputBoth (buf);
			*res = FALSE;
			free (N);
			free (gk);
			return TRUE;
		}
		iaddg (-1, gb);
		divg (gb, N);				// Divide N by (base-1)
		iaddg (1, gb);
		quotient = TRUE;
//		strong = FALSE;				// Do a simple Fermat PRP test (not strong).
	}
	else if (format == ABCVARAQS) {
		char factor[sgkbufsize+256], *p, *p2;
		gd = allocgiant (strlen(sgd)/2 + 8);	// Allocate one byte per decimal digit + spares
		gr = allocgiant ((bits >> 4) + 8);	// Allocate memory for the remainder
		p = sgd;
		while (TRUE) {
			strcpy (factor, p);
			if ((p2 = strchr (p,'/'))!=NULL) {
				p = &p2[1];
				if ((p2 = strchr (factor,'/'))!=NULL)
					*p2 = 0;
			}
			if (!isDigitString(factor)) {
				sprintf (buf,"invalid digit string : %s\n", factor);
				OutputBoth (buf);
				*res = FALSE;
				free (gr);
				free (gd);
				free (N);
				free (gk);
				return TRUE;
			}
			ctog (factor, gd);						// Convert divisor string to giant
			gtog (N, gr);
			modg (gd, gr);
			if (!isZero(gr)) {
				sprintf (buf, "%s is not an integer!\n", str);
				OutputBoth (buf);
				*res = FALSE;
				free (gr);
				free (gd);
				free (N);
				free (gk);
				return TRUE;
			}
			else {
				divg (gd, N);
			}
			if (p2 == NULL)
				break;
		}
		w->prp_residue_type = PRP_TYPE_COFACTOR;
		w->known_factors = sgd;
		quotient = TRUE;
//		strong = FALSE;			// Do a simple Fermat PRP test (not strong).
	}

	if ((nbdg = gnbdg(N, 10)) < 200) {		// Attempt an APRCL test...
		start_timer(1);
		if (nbdg > maxaprcl)
			resaprcl = gaprcltest (N, 1, 0);	// Make only a Strong BPSW PRP test
		else if (debug)
			resaprcl = gaprcltest (N, 0, 2);	// Primality test while showing progress 
		else
			resaprcl = gaprcltest (N, 0, 0);	// Primality test silently done
		end_timer (1);
		if (resaprcl == 10) {
			sprintf (buf,"%s is not prime. (Trial divisions)", str);
		}
		else if (resaprcl == 12)
			sprintf (buf,"%s is prime! (%lu decimal digits, Trial divisions)", str, nbdg);
		else if (resaprcl == 0)
			goto PRPCONTINUE;					// Continue the PRP test to get the residue...
		else if (resaprcl == 1)
			sprintf (buf,"%s is a probable BPSW prime! (%lu decimal digits, APRCL test) ", str, nbdg);
		else if (resaprcl == 2)
			sprintf (buf,"%s is prime! (%lu decimal digits, APRCL test)", str, nbdg);
		else if (resaprcl == 6)
			sprintf (buf,"Invalid numerical string in %s\n", str);
		else if (resaprcl == 7) {
			sprintf (buf,"APRCL error while testing %s...\n", str);
			if (verbose)
				OutputBoth(buf);
			else
				OutputStr (buf);
			goto PRPCONTINUE;					// Continue the PRP test to get the residue...
		}
		else {
			if (resaprcl == 9)
				sprintf (buf, "APRCL primality test not available for %s\n", str);
			else
				sprintf (buf,"Unexpected return value : %d, while APRCL testing %s...\n", resaprcl, str);
			if (verbose)
				OutputBoth(buf);
			else
				OutputStr (buf);
			goto PRPCONTINUE;					// Continue the PRP test to get the residue...
		}
		*res = ((resaprcl == 1)||(resaprcl == 2)||(resaprcl == 12));

#if defined(WIN32) && !defined(_CONSOLE)

		sprintf (buf+strlen(buf), "  Time : "); 

#else

		clearline(100);

		if (*res) {
#if defined (_CONSOLE)
		hConsole = GetStdHandle(STD_OUTPUT_HANDLE);	// Access to Console attributes
		SetConsoleTextAttribute(hConsole, BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
		OutputBoth(buf);
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
#else
		OutputStr("\033[7m");
		OutputBoth(buf);
		OutputStr("\033[0m");
#endif
		}
		else
			OutputBoth(buf);

		sprintf (buf, "  Time : "); 

#endif

/* Output the final timings for APRCL test */

		write_timer (buf+strlen(buf), 1, TIMER_CLR | TIMER_NL); 
		OutputBoth (buf);
		if (resaprcl == 1)
			MakePrimoInput (N, str);
		retval = TRUE; 
	}
	else {
PRPCONTINUE:
		Nlen = bitlen (N); 
		klen = bitlen(gk);
		fpart = (1.0-(double)klen/Nlen)*100.0;



		if (klen > 53 || generic || !smallbase) {	// we must use generic reduction
			dk = 0.0;
		}
		else {								// we can use DWT ; compute the multiplier as a double
			dk = (double)gk->n[0];
			if (gk->sign > 1)
				dk += 4294967296.0*(double)gk->n[1];
		}

		globalk = dk;
		retval = isPRPinternal (str, dk, gb, n, incr, res);
		if (*res) {
			if (klen > 1) {
				sprintf (buf, "(Factorized part = %4.2f%%)\n", fpart);
				OutputBoth (buf);
			}
			if (nbdg < primolimit)
				MakePrimoInput (N, str);
		}

	}

//	strong = TRUE;			// Restore Strong Fermat PRP test
	quotient = FALSE;		// Reset quotient flag

	if (format == ABCVARAQS) {
		free (gr);
		free (gd);
	}
	free (N);
	if (gk!=NULL)
		free (gk);
	return retval;
}

int IsCCP (	// General test for the next prime in a Cunningham chain
	unsigned long format, 
	char *sgk,
	char *sgb,
	giant gb,
	unsigned long n, 
	int incr,
	unsigned long shift,
	int	*res) 
{  
	char	str[sgkbufsize+256], sgk1[sgkbufsize], buf[sgkbufsize+256]; 
	unsigned long bits, retval, smallbase = 0;
	double dk;
	int resaprcl;



	if (abs(gb->sign) == 1)				// Test if the base is a small integer
		smallbase = gb->n[0];

	gk = allocgiant (strlen(sgk)/2 + 8);	// Allocate one byte per decimal digit + spares
	ctog (sgk, gk);						// Convert k string to giant
	gshiftleft (shift, gk);				// Shift k multiplier if requested
	gtoc (gk, sgk1, sgkbufsize);		// Updated k string
	if (mask & MODE_DUAL) {
		sprintf (str, "%s^%lu%c%d", sgb, n, incr < 0 ? '-' : '+', abs(incr));
	}
	else {
		if (!strcmp(sgk1, "1"))
			sprintf (str, "%s^%lu%c%d", sgb, n, incr < 0 ? '-' : '+', abs(incr));
		else
				sprintf (str, "%s*%s^%lu%c%d", sgk1, sgb, n, incr < 0 ? '-' : '+', abs(incr));
	}
	
	if (smallbase)	
		bits = (unsigned long) ((n * log((double) smallbase)) / log(2.0) + bitlen(gk));
	else
		bits = n * bitlen(gb) + bitlen(gk);
	N =  allocgiant ((bits >> 4) + 8);		// Allocate memory for N

//	Compute the number we are testing.

	gtog (gb, N);
	power (N, n);
	mulg (gk, N);
	iaddg (incr, N);

	if ((nbdg = gnbdg(N, 10)) < 200) {			// Attempt an APRCL test for this small number...
		start_timer(1);
			resaprcl = gaprcltest (N, 0, 0);	// Primality test silently done
		end_timer (1);
		if (resaprcl == 10) {
			sprintf (buf,"%s is not prime. (Trial divisions)", str);
		}
		else if (resaprcl == 12)
			sprintf (buf,"%s is prime! (%lu decimal digits, Trial divisions)", str, nbdg);
		else if (resaprcl == 0) {
			goto CCPCONTINUE;					// Continue the CCP test to get the residue...
		}
		else if (resaprcl == 2)
			sprintf (buf,"%s is prime! (%lu decimal digits, APRCL test)", str, nbdg);
		else if (resaprcl == 6)
			sprintf (buf,"Invalid numerical string in %s\n", str);
		else if (resaprcl == 7) {
			sprintf (buf,"APRCL error while testing %s...\n", str);
			if (verbose)
				OutputBoth(buf);
			else
				OutputStr (buf);
			goto CCPCONTINUE;					// Continue the CCP test
		}
		else {
			if (resaprcl == 9)
				sprintf (buf, "APRCL primality test not available for %s\n", str);
			else
				sprintf (buf,"Unexpected return value : %d, while APRCL testing %s...\n", resaprcl, str);
			if (verbose)
				OutputBoth(buf);
			else
				OutputStr (buf);
			goto CCPCONTINUE;					// Continue the CCP test
		}
		*res = ((resaprcl == 2)||(resaprcl == 12));

#if defined(WIN32) && !defined(_CONSOLE)

		sprintf (buf+strlen(buf), "  Time : "); 

#else

		clearline(100);

		if (*res) {
#if defined (_CONSOLE)
		hConsole = GetStdHandle(STD_OUTPUT_HANDLE);	// Access to Console attributes
		SetConsoleTextAttribute(hConsole, BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
		OutputBoth(buf);
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
#else
		OutputStr("\033[7m");
		OutputBoth(buf);
		OutputStr("\033[0m");
#endif
		}
		else
			OutputBoth(buf);

		sprintf (buf, "  Time : "); 

#endif

/* Output the final timings for APRCL test */

		write_timer (buf+strlen(buf), 1, TIMER_CLR | TIMER_NL); 
		OutputBoth (buf);
		retval = TRUE; 
	}
	else {
CCPCONTINUE:
		klen = bitlen(gk);

		if (klen > 53 || generic || !smallbase) {	// we must use generic reduction
			dk = 0.0;
		}
		else {										// we can use DWT ; compute the multiplier as a double
			dk = (double)gk->n[0];
			if (gk->sign > 1)
				dk += 4294967296.0*(double)gk->n[1];
		}

		globalk = dk;

		if (dk >= 1.0)
			if (format == NPGCC1)
				retval = fastIsCC1P (dk, smallbase, n, incr, str, res);
			else if (format == NPGCC2)
				retval = fastIsCC2P (dk, smallbase, n, incr, str, res);
			else
				retval = FALSE;
		else
			if (format == NPGCC1)
				retval = slowIsCC1P (str, res);
			else if (format == NPGCC2)
				retval = slowIsCC2P (str, res);
			else
				retval = FALSE;

	}
	free (N);
	free (gk);
	return retval;
}

unsigned long gcd (
	unsigned long x,
	unsigned long y)
{
	unsigned long w;

	if (!x || x==y)
		return y;
	if (!y)
		return x;

	if (x < y) {
		w = x;
		x = y;
		y = w;
	}

	while (1) {
		x = x%y;
		if (!x)
			break;
		w = x;
		x = y;
		y = w;
	}

	return (y);
}

void findbpf (unsigned long base) {		// find all prime factors of the base
	unsigned long b, p;
	int i;

	for (i=0; i<30; i++)
		bpf[i] = bpc[i] = vpf[i] = 0;	// clean up

// bpf[i] : base prime factor, vpf[i] : its exponent, bpc[i] = base/bpf[i]
// A 32 bits integer can have at most 9 distinct prime factors.

	i = 0;

	b = base;					// copy of base, to be completely factored...

	if (!(base & 1)) {			// divisor two?
		bpf[i] = 2;
		while (!(b & 1)) {
			vpf[i]++;			// compute the power of two
			b >>= 1;
		}
		bpc[i++] = base/2;
		if (isPrime (b)) {		// b may be the last prime factor!
			bpf[i] = b;
			vpf[i] = 1;
			bpc[i] = base/b;
			return;
		}
	}

	if (!(base%3)) {			// divisor three?
		bpf[i] = 3;
		while (!(b % 3)) {
			vpf[i]++;			// compute the power of three
			b /= 3;
		}
		bpc[i++] = base/3;
		if (isPrime (b)) {		// b may be the last prime factor!
			bpf[i] = b;
			vpf[i] = 1;
			bpc[i] = base/b;
			return;
		}
	}

	p = 5;

	while (p*p <= base) {		// other divisors?
		if (!(base%p) && isPrime (p)) {
			bpf[i] = p;
			while (!(b % p)) {
				vpf[i]++;		// compute the power of p
				b /= p;
			}
			bpc[i++] = base/p;
			if (isPrime (b)) {	// b may be the last prime factor!
				bpf[i] = b;
				vpf[i] = 1;
				bpc[i] = base/b;
				return;
			}
		}
		p += 2;
		if (!(base%p) && isPrime (p)) {
			bpf[i] = p;
			while (!(b % p)) {
				vpf[i]++;		// compute the power of p
				b /= p;
			}
			bpc[i++] = base/p;
			if (isPrime (b)) {	// b may be the last prime factor!
				bpf[i] = b;
				vpf[i] = 1;
				bpc[i] = base/b;
				return;
			}
		}
		p += 4;
	}

	if (i == 0 || base < 4) {	// the base is prime!
			bpf[0] = base;
			vpf[0] = bpc[0] = 1;
	}

}

int findgbpf (giant gbase) {		// find all prime factors of a large integer base
	unsigned long p, pmax;
	int i, gbits;
	double db;
	giant b;
	gbits = bitlen (gbase);
	b = allocgiant (2*abs(gbase->sign) + 8);
	for (i=0; i<30; i++) {
		bpf[i]  = vpf[i] = 0;	// clean up
		if (gbpc[i] != NULL) {
			free (gbpc[i]);
			gbpc[i] = NULL;
		}
		if (gbpf[i] != NULL) {
			free (gbpf[i]);
			gbpf[i] = NULL;
		}
	}

// bpf[i] or gbpf[i] : base prime factor, vpf[i] : its exponent, gbpc[i] = gbase/bpf[i] or gbase/gbpf[i]
// We expect the base has no more than 30 distinct prime factors.

	i = 0;
	gtog (gbase, b);			// copy of base, to be completely factored...

	if (!bitval (gbase, 0)) {	// divisor two?
		bpf[i] = 2;
		while (!bitval (b, 0)) {
			vpf[i]++;			// compute the exponent of two
			gshiftright (1, b);
		}
		gbpc[i] = allocgiant (2*abs(gbase->sign) + 8);
		gtog (gbase, gbpc[i]);
		gshiftright (1, gbpc[i]);
		i++;
		if ((b->sign == 1) &&  isPrime (b->n[0])) {	// b may be the last prime factor!
			bpf[i] = b->n[0];
			vpf[i] = 1;
			gbpc[i] = allocgiant (2*abs(gbase->sign) + 8);
			gtog (gbase, gbpc[i]);
			uldivg (b->n[0], gbpc[i]);
			free (b);
			return TRUE;
		}
	}
	if (!gmodi (3, gbase)) {	// divisor three?
		bpf[i] = 3;
		while (!gmodi (3, b)) {
			vpf[i]++;			// compute the exponent of three
			uldivg (3, b);
		}
		gbpc[i] = allocgiant (2*abs(gbase->sign) + 8);
		gtog (gbase, gbpc[i]);
		uldivg (3, gbpc[i]);
		i++;
		if ((b->sign == 1) &&  isPrime (b->n[0])) {	// b may be the last prime factor!
			bpf[i] = b->n[0];
			vpf[i] = 1;
			gbpc[i] = allocgiant (2*abs(gbase->sign) + 8);
			gtog (gbase, gbpc[i]);
			uldivg (b->n[0], gbpc[i]);
			free (b);
			return TRUE;
		}
	}
	if (bitlen(b) > 53) {				// The cofactor is still a large integer...
		pmax = 1<<20;
	}
	else {								// Compute the cofactor as a double
		db = (double)b->n[0];
		if (b->sign > 1)
			db += 4294967296.0*(double)b->n[1];
		pmax = (unsigned long)floor (sqrt (db));
		if (pmax > (1<<20))
			pmax = 1<<20;				// 2**40 > 2**32, so, a cofactor not larger than 32bit must be prime!
	}

	p = 5;

	while (p <= pmax) {			// other divisors?
		if (isPrime(p) && !gmodi (p, gbase)) {
			bpf[i] = p;
			while (!gmodi (p, b)) {
				vpf[i]++;		// compute the exponent of p
				uldivg (p, b);
			}
			gbpc[i] = allocgiant (2*abs(gbase->sign) + 8);
			gtog (gbase, gbpc[i]);
			uldivg (p, gbpc[i]);
			i++;
			if ((b->sign == 1) &&  isPrime (b->n[0])) {	// b may be the last prime factor!
				bpf[i] = b->n[0];
				vpf[i] = 1;
				gbpc[i] = allocgiant (2*abs(gbase->sign) + 8);
				gtog (gbase, gbpc[i]);
				uldivg (b->n[0], gbpc[i]);
				free (b);
				return TRUE;
			}
		}
		p += 2;
		if (isPrime(p) && !gmodi (p, gbase)) {
			bpf[i] = p;
			while (!gmodi (p, b)) {
				vpf[i]++;		// compute the exponent of p
				uldivg (p, b);
			}
			gbpc[i] = allocgiant (2*abs(gbase->sign) + 8);
			gtog (gbase, gbpc[i]);
			uldivg (p, gbpc[i]);
			i++;
			if ((b->sign == 1) &&  isPrime (b->n[0])) {	// b may be the last prime factor!
				bpf[i] = b->n[0];
				vpf[i] = 1;
				gbpc[i] = allocgiant (2*abs(gbase->sign) + 8);
				gtog (gbase, gbpc[i]);
				uldivg (b->n[0], gbpc[i]);
				free (b);
				return TRUE;
			}
		}
		p += 4;
	}
	if (isone(b)) {						// The factorization is complete
		free(b);
		return TRUE;
	}
	else if (abs(b->sign) == 1) {		// The cofactor is a small integer prime !
		bpf[i] = b->n[0];
		vpf[i] = 1;
		gbpc[i] = allocgiant (2*abs(gbase->sign) + 8);
		gtog (gbase, gbpc[i]);
		uldivg (bpf[i], gbpc[i]);
		free (b);
		return TRUE;
	}
	else {
		bpf[i] = vpf[i] = 1;			// To signal a large integer cofactor
		gbpc[i] = allocgiant (2*abs(gbase->sign) + 8);
		gtog (gbase, gbpc[i]);
		divg (b, gbpc[i]);
		gbpf[i] = allocgiant (2*abs(gbase->sign) + 8);
		if (bitlen(b) <= 40 || (gaprcltest (b, 0, 0) == 2)) {
			gtog (b, gbpf[i]);			// The cofactor is prime !
			free (b);
			return TRUE;
		}
		else {
			gfact (b, gbpf[i], 0, 0, debug);// Try to factorize using R.Crandall code
			gtog (gbase, gbpc[i]);
			divg (gbpf[i], gbpc[i]);
			i++;
			bpf[i] = vpf[i] = 1;			// To signal a large integer cofactor
			gbpc[i] = allocgiant (2*abs(gbase->sign) + 8);
			gtog (gbase, gbpc[i]);
			divg (b, gbpc[i]);
			gbpf[i] = allocgiant (2*abs(gbase->sign) + 8);
			gtog (b, gbpf[i]);
			free (b);
			if ((bitlen(gbpf[i-1]) <= 40) && (bitlen(gbpf[i]) <= 40))
				return TRUE;				// The two factors are prime !
			else {
				if ((gaprcltest (gbpf[i-1], 0, 0) == 2) && (gaprcltest (gbpf[i], 0 ,0) == 2))
					return TRUE;			// The two factors are prime !
				else
					return FALSE;			// The factors must be factorized further...
			}
		}
	}
}

int Lucasequence (
	gwhandle *gwdata,
	ghandle *gdata,
	giant modulus,
	giant exponent,
	unsigned long P,
	giant gb,
	int jmin,
	int jmax,
	char *str,
	char *buf,
	int	*res)
{
	unsigned long bit, iters, D, bitv;
	unsigned long bits, frestart=FALSE, explen;
	unsigned long maxrestarts;
	gwnum x, y, gwinvD, gwinv2;
	gwnum a11, a12, a21, a22, b11, b12, b21, b22, c11, c12, c21, c22, p, pp, s;
	giant	tmp, tmp2;
	char	filename[20], fft_desc[256];
	long	write_time = DISK_WRITE_TIME * 60;
	long	j;
	int	echk, saving, stopping;
	time_t	start_time, current_time;
	double	reallyminerr = 1.0;
	double	reallymaxerr = 0.0;


	maxrestarts = IniGetInt(INI_FILE, "MaxRestarts", 100);

	if (!IniGetInt(INI_FILE, "Testdiff", 0))	// Unless test of MAXDIFF explicitly required
		gwdata->MAXDIFF = 1.0E80;				// Diregard MAXDIFF...

// restart:

	*res = TRUE;								/* Assume a positive result */

	/* Get the current time */

	time (&start_time);

/* Allocate memory */

	bits = bitlen (exponent);
	nbdg = gnbdg (N, 10); // Compute the number of decimal digits of the tested number.

	x = gwalloc (gwdata);
	y = gwalloc (gwdata);
	gwinvD = gwalloc (gwdata);
	gwinv2 = gwalloc (gwdata);

	a11 = gwalloc (gwdata);		// allocate memory for matrix computing...
	a12 = gwalloc (gwdata);
	a21 = gwalloc (gwdata);
	a22 = gwalloc (gwdata);
	b11 = gwalloc (gwdata);
	b12 = gwalloc (gwdata);
	b21 = gwalloc (gwdata);
	b22 = gwalloc (gwdata);
	c11 = gwalloc (gwdata);
	c12 = gwalloc (gwdata);
	c21 = gwalloc (gwdata);
	c22 = gwalloc (gwdata);
	p = gwalloc (gwdata);
	pp = gwalloc (gwdata);
	s = gwalloc (gwdata);

	tmp = popg (gdata, (bits >> 4) + 8);
	tmp2 = popg (gdata, (bits >> 4) + 8);

/* Init, */


	gtog (exponent, tmp2);
	divg (gb, tmp2);				// tmp2 = exponent/base

	explen = bitlen (tmp2);

/* Init filename */

	tempFileName (filename, 'L', N);

/* Optionally resume from save file and output a message */
/* indicating we are resuming a test */

	if (fileExists (filename) && readFromFileB (gwdata, gdata, filename, &bit, &P, &nrestarts, bpf, x, y)) {
		char	fmt_mask[80];
		double	pct;
		pct = trunc_percent (bit * 100.0 / explen);
		sprintf (fmt_mask,
			 "Resuming Lucas sequence at bit %%ld [%%.%df%%%%]\n",
			 PRECISION);
		sprintf (buf, fmt_mask, bit, pct);
		OutputStr (buf);
		if (verbose || restarting)
			writeResults (buf);	
		D = P*P - 4;
	}

/* Otherwise, output a message indicating we are starting test */

	else {
		OutputStr (buf);
		if (verbose || restarting)
			writeResults (buf);	
		D = P*P - 4;
		bit = 1;
		dbltogw (gwdata, 2.0, x);			// Initial values
		dbltogw (gwdata, (double)P, y);
	}

	ultog (D, tmp);						// Compute inverse of D modulo N
	invg (modulus, tmp);
	gianttogw (gwdata, tmp, gwinvD);	// Convert it to gwnum
//	gwfft (gwdata, gwinvD, gwinvD);		// And fft it
	gtog (modulus, tmp);				// Compute inverse of 2 modulo N
	iaddg (1, tmp);						// Which is (N+1)/2
	gshiftright (1, tmp);
	gianttogw (gwdata, tmp, gwinv2);	// Convert it to gwnum
//	gwfft (gwdata, gwinv2, gwinv2);		// And fft it

/* Output a message about the FFT length */

	topology_print_children (hwloc_get_root_obj (hwloc_topology), 0);
	gwfft_description (gwdata, fft_desc);
	sprintf (buf, "Using %s, P = %lu\n", fft_desc, P);

	OutputStr (buf);
	if (verbose || restarting) {
		writeResults (buf);
	}
	ReplaceableLine (1);	/* Remember where replaceable line is */

/* Init the title */

	title ("Computing Lucas sequence...");

/* Main loop... */

	iters = 0;
	start_timer (0);
	start_timer (1);
	while (bit <= explen) {

/* Error check the last 50 iterations, before writing an */
/* intermediate file (either user-requested stop or a */
/* 30 minute interval expired), and every 128th iteration. */

		stopping = stopCheck ();
		echk = stopping || ERRCHK || (bit <= 50) || (bit >= explen-50) || gwnear_fft_limit (gwdata, pcfftlim);
		if (((bit & 127) == 0) || (bit == 1) || (bit == (lasterr_point-1))) {
			echk = 1;
			time (&current_time);
			saving = ((current_time - start_time > write_time) || (bit == 1) || (bit == (lasterr_point-1)));
		} else
			saving = 0;

/* Process this bit */

//		gwstartnextfft (postfft && !debug && !stopping && !saving && bit != lasterr_point && (bit+26 < explen) && (bit > 26));

		gwsetnormroutine (gwdata, 0, echk, 0);
		gwstartnextfft (gwdata, FALSE);

		if ((bitv = bitval (tmp2, explen-bit))) {
			if (abs(gwdata->c) == 1)
				gwsetaddin (gwdata, -(int)P);
			if ((bit+26 < explen) && (bit > 26) &&
				((bit != lasterr_point) || !maxerr_recovery_mode[1])) {
				gwsafemul (gwdata, y, x);
				care = FALSE;
			}
			else {
				gwmul_carefully (gwdata, y, x);
				care = TRUE;
			}
			if (debug && (bit < 50))
				writeresidue (gwdata, x, N, tmp, buf, str, bit, BIT);
			CHECK_IF_ANY_ERROR(x, (bit), explen, 1)
			if (abs(gwdata->c) != 1)
				gwsmalladd (gwdata, -(double)P, x);
			if (abs(gwdata->c) == 1)
				gwsetaddin (gwdata, -2);
			if ((bit+26 < explen) && (bit > 26) &&
				((bit != lasterr_point) || !maxerr_recovery_mode[2])) {
				gwsquare (gwdata, y);
				care = FALSE;
			}
			else {
				gwsquare_carefully (gwdata, y);
				care = TRUE;
			}
			if (debug && (bit < 50))
				writeresidue (gwdata, y, N, tmp, buf, str, bit, BIT);
			CHECK_IF_ANY_ERROR(y, (bit), explen, 2)
			if (abs(gwdata->c) != 1)
				gwsmalladd (gwdata, -2.0, y);
		}
		else {
			if (abs(gwdata->c) == 1)
				gwsetaddin (gwdata, -(int)P);
			if ((bit+26 < explen) && (bit > 26) &&
				((bit != lasterr_point) || !maxerr_recovery_mode[3])) {
				gwsafemul (gwdata, x, y);
				care = FALSE;
			}
			else {
				gwmul_carefully (gwdata, x, y);
				care = TRUE;
			}
			if (debug && (bit < 50))
				writeresidue (gwdata, y, N, tmp, buf, str, bit, BIT);
			CHECK_IF_ANY_ERROR(y, (bit), explen, 3)
			if (abs(gwdata->c) != 1)
				gwsmalladd (gwdata, -(double)P, y);
			if (abs(gwdata->c) == 1)
				gwsetaddin (gwdata, -2);
			if ((bit+26 < explen) && (bit > 26) &&
				((bit != lasterr_point) || !maxerr_recovery_mode[4])) {
				gwsquare (gwdata, x);
				care = FALSE;
			}
			else {
				gwsquare_carefully (gwdata, x);
				care = TRUE;
			}
			if (debug && (bit < 50))
				writeresidue (gwdata, x, N, tmp, buf, str, bit, BIT);
			CHECK_IF_ANY_ERROR(x, (bit), explen, 4)
			if (abs(gwdata->c) != 1)
				gwsmalladd (gwdata, -2.0, x);
		}

 /* That iteration succeeded, bump counters */

		if (bit == lasterr_point)
			saving = 1;					// Be sure to restart after this recovery iteration!
		bit++;
		iters++;

/* Print a message every so often */

		if (bit % ITER_OUTPUT == 0) {
			char	fmt_mask[80];
			double	pct;
			pct = trunc_percent (bit * 100.0 / explen);
			sprintf (fmt_mask, "%%.%df%%%% of %%ld", PRECISION);
			sprintf (buf, fmt_mask, pct, explen);
			title (buf);
			ReplaceableLine (2);	/* Replace line */
			sprintf (fmt_mask,
				 "%%s, bit: %%ld / %%ld [%%.%df%%%%]",
				 PRECISION);
			sprintf (buf, fmt_mask, str, bit, explen, pct);
			OutputStr (buf);
			if (ERRCHK && bit > 30) {
				OutputStr (".  Round off: ");
				sprintf (buf, "%10.10f", reallyminerr);
				OutputStr (buf);
				sprintf (buf, " to %10.10f", reallymaxerr);
				OutputStr (buf);
			}
			end_timer (0);
			if (CUMULATIVE_TIMING) {
				OutputStr (".  Time thusfar: ");
			} else {
				OutputStr (".  Time per bit: ");
				divide_timer (0, iters);
				iters = 0;
			}
			print_timer (0, TIMER_NL | TIMER_OPT_CLR);
			start_timer (0);
		}

/* Print a results file message every so often */

		if (bit % ITER_OUTPUT_RES == 0 || (NO_GUI && stopping)) {
			sprintf (buf, "Bit %ld / %ld\n", bit, explen);
			writeResults (buf);
		}

/* Write results to a file every DISK_WRITE_TIME minutes */
/* On error, retry in 10 minutes (it could be a temporary */
/* disk-full situation) */

		if (saving || stopping) {
			saving = FALSE;
			write_time = DISK_WRITE_TIME * 60;
			if (! writeToFileB (gwdata, gdata, filename, bit, P, nrestarts, bpf, x, y)) {
				sprintf (buf, WRITEFILEERR, filename);
				OutputBoth (buf);
				if (write_time > 600) write_time = 600;
			}	
			time (&start_time);

/* If an escape key was hit, write out the results and return */

			if (stopping) {
				pushg (gdata, 2);
				gwfree (gwdata, x);				// Clean up
				gwfree (gwdata, y);
				gwfree (gwdata, gwinvD);
				gwfree (gwdata, gwinv2);
				gwfree (gwdata, a11);			// Clean up the matrix
				gwfree (gwdata, a12);
				gwfree (gwdata, a21);
				gwfree (gwdata, a22);
				gwfree (gwdata, b11);
				gwfree (gwdata, b12);
				gwfree (gwdata, b21);
				gwfree (gwdata, b22);
				gwfree (gwdata, c11);
				gwfree (gwdata, c12);
				gwfree (gwdata, c21);
				gwfree (gwdata, c22);
				gwfree (gwdata, p);
				gwfree (gwdata, pp);
				gwfree (gwdata, s);
				*res = FALSE;
				return (FALSE);
			}
		}

/* Output the 64-bit residue at specified interims.  Also output the */
/* residues for the next iteration so that we can compare our */
/* residues to programs that start counter at zero or one. */

		if (interimResidues && bit % interimResidues < 2)
				writeresidue (gwdata, x, N, tmp, buf, str, bit, BIT);

/* Write a save file every "interimFiles" iterations. */

		if (interimFiles && bit % interimFiles == 0) {
			char	interimfile[20];
			sprintf (interimfile, "%.8s.%03lu",
				 filename, bit / interimFiles);
			if (! writeToFileB (gwdata, gdata, interimfile, bit, P, nrestarts, bpf, x, y)) {
				sprintf (buf, WRITEFILEERR, interimfile);
				OutputBoth (buf);
			}
		}
	}

	care = TRUE;						// All following errors are considered unrecoverable...

	// Compute the matrix at (N+1)/base

	if (abs(gwdata->c) == 1)
		gwsetaddin (gwdata, 0);			// Reset addin constant.
	gwcopy (gwdata, x, a22);			// a22 = V((N+1)/base)
	gwcopy (gwdata, y, a12);			// a12 = V((N+1)/base+1)
	gwcopy (gwdata, y, a21);			// a21 = V((N+1)/base+1)
	gwcopy (gwdata, y, a11);			// a11 = V((N+1)/base+1)
	gwcopy (gwdata, x, y);				// Now, y = V((N+1)/base)
	gwsetnormroutine (gwdata, 0, 1, 1);	// set mul. by const.
	gwsetmulbyconst (gwdata, 2);
//	gwfftmul (gwdata, gwinvD, a21);		// a21 = D^-1*2*V((N+1)/base+1) modulo N
	gwmul_carefully (gwdata, gwinvD, a21);// a21 = D^-1*2*V((N+1)/base+1) modulo N
	CHECK_IF_ANY_ERROR(a21, (explen), explen, 6)
	gwsetmulbyconst (gwdata, P);
//	gwfftmul (gwdata, gwinvD, x);		// x =  D^-1*P*V((N+1)/base) modulo N
	gwmul_carefully (gwdata, gwinvD, x);// x =  D^-1*P*V((N+1)/base) modulo N
	CHECK_IF_ANY_ERROR(x, (explen), explen, 6)
	gwsub (gwdata, x, a21);				// a21 = D^-1*(2*V((N+1)/base+1)-P*V((N+1)/base)) = U(N+1)/base modulo N
//	gwfftmul (gwdata, gwinvD, a11);		// a11 = D^-1*P*V((N+1)/base+1) modulo N
	gwmul_carefully (gwdata, gwinvD, a11);// a11 = D^-1*P*V((N+1)/base+1) modulo N
	CHECK_IF_ANY_ERROR(a11, (explen), explen, 6)
	gwsetmulbyconst (gwdata, 2);
//	gwfftmul (gwdata, gwinvD, y);		// xx = D^-1*2*V((N+1)/base) modulo N
	gwmul_carefully (gwdata, gwinvD, y);// xx = D^-1*2*V((N+1)/base) modulo N
	CHECK_IF_ANY_ERROR(y, (explen), explen, 6)
	gwsub (gwdata, y, a11);				// a11 = D^-1*(P*V((N+1)/base+1)-2*V((N+1)/base)) = U((N+1)/base+1) modulo N
	gwsetnormroutine (gwdata, 0, 1, 0);	// reset mul by const
//	gwfftmul (gwdata, gwinv2, a22);		// a22 = 2^-1*V((N+1)/base)
	gwmul_carefully (gwdata, gwinv2, a22);// a22 = 2^-1*V((N+1)/base)
	CHECK_IF_ANY_ERROR(a22, (explen), explen, 6)
//	gwfftmul (gwdata, gwinv2, a12);		// a12 = 2^-1*V((N+1)/base+1)
	gwmul_carefully (gwdata, gwinv2, a12);// a12 = 2^-1*V((N+1)/base+1)
	CHECK_IF_ANY_ERROR(a12, (explen), explen, 6)
	gwsetmulbyconst (gwdata, P);
	gwsetnormroutine (gwdata, 0, 1, 1);	// set mul. by const.
	gwcopy (gwdata, a11, x);			// x = U((N+1)/base+1)
//	gwfftmul (gwdata, gwinv2, x);		// x = 2^-1*P*U((N+1)/base+1)
	gwmul_carefully (gwdata, gwinv2, x);// x = 2^-1*P*U((N+1)/base+1)
	CHECK_IF_ANY_ERROR(x, (explen), explen, 6)
	gwsub (gwdata, x, a12);				// a12 = 2^-1(V((N+1)/base+1)-P*U((N+1)/base+1))
	gwcopy (gwdata, a21, x);			// x = U((N+1)/base)
//	gwfftmul (gwdata, gwinv2, x);		// x = 2^-1*P*U((N+1)/base)
	gwmul_carefully (gwdata, gwinv2, x);// x = 2^-1*P*U((N+1)/base)
	CHECK_IF_ANY_ERROR(x, (explen), explen, 6)
	gwsub (gwdata, x, a22);				// a22 = 2^-1(V((N+1)/base)-P*U((N+1)/base))
	gwsetnormroutine (gwdata, 0, 1, 0);	// reset mul by const

//	gwtogiant (gwdata, a21, tmp);		// tmp = U((N+1)/base) modulo N

	gwcopy (gwdata, a11, c11);			// Save the current matrix
	gwcopy (gwdata, a12, c12);
	gwcopy (gwdata, a21, c21);
	gwcopy (gwdata, a22, c22);

//	gwfft (gwdata, a11, b11);			// Copy (and fft) the current matrix
//	gwfft (gwdata, a12, b12);
//	gwfft (gwdata, a21, b21);
//	gwfft (gwdata, a22, b22);

	gwcopy (gwdata, a11, b11);			// Copy the current matrix
	gwcopy (gwdata, a12, b12);
	gwcopy (gwdata, a21, b21);
	gwcopy (gwdata, a22, b22);

	explen = bitlen (gb);
	lasterr_point = 0;					// Reset last error point.
	bit = 1;

	while (bit < explen) {					// Finish to compute U(N+1)

// Square the matrix

		gwcopy (gwdata, a12, p);			// a12-->p
		gwcopy (gwdata, a12, pp);			// a12-->pp
		gwadd3 (gwdata, a11, a22, s);		// a11+a12-->s
//		gwfft (gwdata, s, s);
//		gwmul (gwdata, a21, p);				// a21*a12-->p
		gwmul_carefully (gwdata, a21, p);	// a21*a12-->p
		CHECK_IF_ANY_ERROR(p, (bit), explen, 6)
//		gwsquare (gwdata, a22);				// a22*a22-->a22
		gwsquare_carefully (gwdata, a22);	// a22*a22-->a22
		CHECK_IF_ANY_ERROR(a22, (bit), explen, 6)
//		gwfftfftmul (gwdata, s, a21, a21);	// (a11+a22)*a21-->a21 T
		gwmul_carefully (gwdata, s, a21);	// (a11+a22)*a21-->a21 T
		CHECK_IF_ANY_ERROR(a21, (bit), explen, 6)
		gwadd (gwdata, p, a22);				// a21*a12+a22*a22-->a22 T
//		gwfftmul (gwdata, s, a12);			// (a11+a22)*a12-->a12 T
		gwmul_carefully (gwdata, s, a12);	// (a11+a22)*a12-->a12 T
		CHECK_IF_ANY_ERROR(a12, (bit), explen, 6)
//		gwsquare (gwdata, a11);				// a11*a11-->a11
		gwsquare_carefully (gwdata, a11);	// a11*a11-->a11
		CHECK_IF_ANY_ERROR(a11, (bit), explen, 6)
		gwadd (gwdata, p, a11);				// a21*a12+a11*a11-->a11 T

// Multiply it if required

		if (bitval (gb, explen-bit-1)) {
			gwcopy (gwdata, a11, p);		// a11-->p
			gwcopy (gwdata, a21, pp);		// a21-->pp
//			gwfftmul (gwdata, b11, a11);	// b11*a11-->a11
			gwmul_carefully (gwdata, b11, a11);// b11*a11-->a11
			CHECK_IF_ANY_ERROR(a11, (bit), explen, 6)
//			gwfftmul (gwdata, b12, pp);		// b12*a21-->pp
			gwmul_carefully (gwdata, b12, pp);// b12*a21-->pp
			CHECK_IF_ANY_ERROR(pp, (bit), explen, 6)
			gwadd (gwdata, pp, a11);		// b11*a11+b12*a21-->a11 T
//			gwfftmul (gwdata, b21, p);		// b21*a11-->p
			gwmul_carefully (gwdata, b21, p);// b21*a11-->p
			CHECK_IF_ANY_ERROR(p, (bit), explen, 6)
//			gwfftmul (gwdata, b22, a21);	// b22*a21-->a21
			gwmul_carefully (gwdata, b22, a21);// b22*a21-->a21
			CHECK_IF_ANY_ERROR(a21, (bit), explen, 6)
			gwadd (gwdata, p, a21);			// b21*a11+b22*a21-->a21 T
			gwcopy (gwdata, a12, p);		// a12-->p
			gwcopy (gwdata, a22, pp);		// a22-->pp
//			gwfftmul (gwdata, b11, a12);	// b11*a12-->a12
			gwmul_carefully (gwdata, b11, a12);// b11*a12-->a12
			CHECK_IF_ANY_ERROR(a12, (bit), explen, 6)
//			gwfftmul (gwdata, b12, pp);		// b12*a22-->pp
			gwmul_carefully (gwdata, b12, pp);// b12*a22-->pp
			CHECK_IF_ANY_ERROR(pp, (bit), explen, 6)
			gwadd (gwdata, pp, a12);		// b11*a12+b12*a22-->a12 T
//			gwfftmul (gwdata, b21, p);		// b21*a12-->p
			gwmul_carefully (gwdata, b21, p);// b21*a12-->p
			CHECK_IF_ANY_ERROR(p, (bit), explen, 6)
//			gwfftmul (gwdata, b22, a22);	// b22*a22-->a22
			gwmul_carefully (gwdata, b22, a22);// b22*a22-->a22
			CHECK_IF_ANY_ERROR(a22, (bit), explen, 6)
			gwadd (gwdata, p, a22);			// b21*a12+b22*a22-->a22 T

		}
		bit++;
	}

	clearline (100);

	gwtogiant (gwdata, a21, tmp);			// tmp = U(N+1) modulo N
	ReplaceableLine (2);					/* Replace line */

	if (!isZero (tmp)) {
		*res = FALSE;						/* Not a prime */
		if (abs(tmp->sign) < 2)				// make a 32 bit residue correct !!
			sprintf (res64, "%08lX%08lX", (unsigned long)0, (unsigned long)tmp->n[0]);
		else
			sprintf (res64, "%08lX%08lX", (unsigned long)tmp->n[1], (unsigned long)tmp->n[0]);
		if (IniGetInt(INI_FILE, "Verify", 0))
			sprintf (buf, "%s is not prime. P = %lu, Lucas RES64: %s", str, P, res64);
		else
			sprintf (buf, "%s is not prime, although Fermat PSP! P = %lu, Lucas RES64: %s", str, P, res64);
	}
	else {
		sprintf (buf, "%s may be prime, trying to compute gcd's\n", str);
		OutputStr (buf);
		for (j=jmax; j>=jmin; j--) {
			if (bpf[j] == 0)			// base prime factor already tested
				continue;
			gwcopy (gwdata, c11, a11);			// Computing U((N+1)/q)
			gwcopy (gwdata, c12, a12);
			gwcopy (gwdata, c21, a21);
			gwcopy (gwdata, c22, a22);

			explen = bitlen (gbpc[j]);
			lasterr_point = 0;					// Reset last error point.
			bit = 1;
			
			while (bit < explen) {

// Square the matrix

				gwcopy (gwdata, a12, p);			// a12-->p
				gwcopy (gwdata, a12, pp);			// a12-->pp
				gwadd3 (gwdata, a11, a22, s);		// a11+a12-->s
//				gwfft (gwdata, s, s);
//				gwmul (gwdata, a21, p);				// a21*a12-->p
				gwmul_carefully (gwdata, a21, p);	// a21*a12-->p
				CHECK_IF_ANY_ERROR(p, (bit), explen, 6)
//				gwsquare (gwdata, a22);				// a22*a22-->a22
				gwsquare_carefully (gwdata, a22);	// a22*a22-->a22
				CHECK_IF_ANY_ERROR(a22, (bit), explen, 6)
//				gwfftfftmul (gwdata, s, a21, a21);	// (a11+a22)*a21-->a21 T
				gwmul_carefully (gwdata, s, a21);	// (a11+a22)*a21-->a21 T
				CHECK_IF_ANY_ERROR(a21, (bit), explen, 6)
				gwadd (gwdata, p, a22);				// a21*a12+a22*a22-->a22 T
//				gwfftmul (gwdata, s, a12);			// (a11+a22)*a12-->a12 T
				gwmul_carefully (gwdata, s, a12);	// (a11+a22)*a12-->a12 T
				CHECK_IF_ANY_ERROR(a12, (bit), explen, 6)
//				gwsquare (gwdata, a11);				// a11*a11-->a11
				gwsquare_carefully (gwdata, a11);	// a11*a11-->a11
				CHECK_IF_ANY_ERROR(a11, (bit), explen, 6)
				gwadd (gwdata, p, a11);				// a21*a12+a11*a11-->a11 T

// Multiply it if required

				if (bitval (gbpc[j], explen-bit-1)) {
					gwcopy (gwdata, a11, p);		// a11-->p
					gwcopy (gwdata, a21, pp);		// a21-->pp
//					gwfftmul (gwdata, b11, a11);	// b11*a11-->a11
					gwmul_carefully (gwdata, b11, a11);// b11*a11-->a11
					CHECK_IF_ANY_ERROR(a11, (bit), explen, 6)
//					gwfftmul (gwdata, b12, pp);		// b12*a21-->pp
					gwmul_carefully (gwdata, b12, pp);// b12*a21-->pp
					CHECK_IF_ANY_ERROR(pp, (bit), explen, 6)
					gwadd (gwdata, pp, a11);		// b11*a11+b12*a21-->a11 T
//					gwfftmul (gwdata, b21, p);		// b21*a11-->p
					gwmul_carefully (gwdata, b21, p);// b21*a11-->p
					CHECK_IF_ANY_ERROR(p, (bit), explen, 6)
//					gwfftmul (gwdata, b22, a21);	// b22*a21-->a21
					gwmul_carefully (gwdata, b22, a21);// b22*a21-->a21
					CHECK_IF_ANY_ERROR(a21, (bit), explen, 6)
					gwadd (gwdata, p, a21);			// b21*a11+b22*a21-->a21 T
					gwcopy (gwdata, a12, p);		// a12-->p
					gwcopy (gwdata, a22, pp);		// a22-->pp
//					gwfftmul (gwdata, b11, a12);	// b11*a12-->a12
					gwmul_carefully (gwdata, b11, a12);// b11*a12-->a12
					CHECK_IF_ANY_ERROR(a12, (bit), explen, 6)
//					gwfftmul (gwdata, b12, pp);		// b12*a22-->pp
					gwmul_carefully (gwdata, b12, pp);// b12*a22-->pp
					CHECK_IF_ANY_ERROR(pp, (bit), explen, 6)
					gwadd (gwdata, pp, a12);		// b11*a12+b12*a22-->a12 T
//					gwfftmul (gwdata, b21, p);		// b21*a12-->p
					gwmul_carefully (gwdata, b21, p);// b21*a12-->p
					CHECK_IF_ANY_ERROR(p, (bit), explen, 6)
//					gwfftmul (gwdata, b22, a22);	// b22*a22-->a22
					gwmul_carefully (gwdata, b22, a22);// b22*a22-->a22
					CHECK_IF_ANY_ERROR(a22, (bit), explen, 6)
					gwadd (gwdata, p, a22);			// b21*a12+b22*a22-->a22 T

				}
				bit++;
			}
			gwtogiant (gwdata, a21, tmp);
			if (isZero (tmp)) {
				if (bpf[j] == 1) {
					gtoc(gbpf[j], bpfstring, strlen(sgb));
					sprintf (buf, "%s may be prime, but N divides U((N+1)/%s), P = %lu\n", str, bpfstring, P);
				}
				else
					sprintf (buf, "%s may be prime, but N divides U((N+1)/%lu), P = %lu\n", str, bpf[j], P);
				OutputStr (buf);
				if (verbose)
					writeResults (buf);	
				frestart = TRUE;
				_unlink (filename);
				continue;
//				break;
			}
			else {
				gcdg (modulus, tmp);
				if (isone (tmp)) {
					if (bpf[j] == 1) {
						gtoc(gbpf[j], bpfstring, strlen(sgb));
						sprintf (buf, "U((N+1)/%s) is coprime to N!\n", bpfstring);
					}
					else
						sprintf (buf, "U((N+1)/%lu) is coprime to N!\n", bpf[j]);
					OutputStr (buf);
					if (verbose)
						writeResults (buf);	
					bpf[j] = 0;
				}
				else {
					*res = FALSE;	/* Not a prime */
					if (IniGetInt(INI_FILE, "Verify", 0))
						sprintf (buf, "%s is not prime, although Lucas PSP!! (P = %lu)", str, P);
					else
						sprintf (buf, "%s is not prime, although Fermat and Lucas PSP!! (P = %lu)", str, P);
					break;
				}
			}
		}
	}

	if (*res && !frestart)
		sprintf (buf, "%s is prime! (%lu decimal digits, P = %lu)", str, nbdg, P);

	pushg (gdata, 2);
	gwfree (gwdata, x);			// Clean up
	gwfree (gwdata, y);
	gwfree (gwdata, gwinvD);
	gwfree (gwdata, gwinv2);
	gwfree (gwdata, a11);		// Clean up the matrix
	gwfree (gwdata, a12);
	gwfree (gwdata, a21);
	gwfree (gwdata, a22);
	gwfree (gwdata, b11);
	gwfree (gwdata, b12);
	gwfree (gwdata, b21);
	gwfree (gwdata, b22);
	gwfree (gwdata, c11);
	gwfree (gwdata, c12);
	gwfree (gwdata, c21);
	gwfree (gwdata, c22);
	gwfree (gwdata, p);
	gwfree (gwdata, pp);
	gwfree (gwdata, s);

/* Cleanup and return */

	_unlink (filename);
	lasterr_point = 0;

	if (frestart)
		return -2;
	else {
		IniWriteString(INI_FILE, "FFT_Increment", NULL);
		return TRUE;
	}

/* An error occured, sleep if required, then try restarting at last save point. */

error:
	pushg (gdata, 2);
	gwfree (gwdata, x);				// Clean up
	gwfree (gwdata, y);
	gwfree (gwdata, gwinvD);
	gwfree (gwdata, gwinv2);
	gwfree (gwdata, a11);			// Clean up the matrix
	gwfree (gwdata, a12);
	gwfree (gwdata, a21);
	gwfree (gwdata, a22);
	gwfree (gwdata, b11);
	gwfree (gwdata, b12);
	gwfree (gwdata, b21);
	gwfree (gwdata, b22);
	gwfree (gwdata, c11);
	gwfree (gwdata, c12);
	gwfree (gwdata, c21);
	gwfree (gwdata, c22);
	gwfree (gwdata, p);
	gwfree (gwdata, pp);
	gwfree (gwdata, s);
	*res = FALSE;

	if ((abonillsum && gw_test_illegal_sumout(gwdata)) || 
		(abonmismatch && gw_test_mismatched_sums (gwdata)) || 
		(abonroundoff && gw_get_maxerr(gwdata) > maxroundoff)) {	// Abort...
		aborted = TRUE;
		sprintf (buf, ERRMSG5, checknumber, str);
		OutputBoth (buf);
		_unlink (filename);
		if(IniGetInt(INI_FILE, "StopOnAbort", 0)) {
			IniWriteInt (INI_FILE, "PgenLine", IniGetInt(INI_FILE, "PgenLine", 0) + 1);	// Point on the next line
			return (FALSE);
		}
		else
			return (TRUE);
	}

/* Output a message saying we are restarting */

	if (sleep5) OutputBoth (ERRMSG2);
	OutputBoth (ERRMSG3);

/* Sleep five minutes before restarting */

	if (sleep5 && ! SleepFive ()) {
		return (FALSE);
	}

/* Restart */

	if (will_try_larger_fft) {
		IniWriteInt(INI_FILE, "FFT_Increment", nbfftinc = (IniGetInt(INI_FILE, "FFT_Increment", 0) + 1));
		if (nbfftinc == maxfftinc)
			abonroundoff = TRUE;	// Don't accept any more Roundoff error.
	}
	return (-1);
}


int plusminustest ( 
	char *sgk,
	char *sgb,
	giant gb,
	unsigned long n, 
	int incr,
	unsigned long shift,
	int	*res) 
{ 
	char	filename[20], buf[sgkbufsize+256], str[sgkbufsize+256], sgk1[sgkbufsize], fft_desc[256], oldres64[17]; 
	unsigned long base, bits, explen, iters, bit, a, frestart=FALSE;
	unsigned long newa, maxrestarts, P, factorized_part = 0;
	uint32_t hi = 0, lo = 0, nincr = 1;
	double dk;
	giant grem, tmp, tmp2, tmp3;
	gwnum x, y;
	long	write_time = DISK_WRITE_TIME * 60;
	int	resaprcl, echk, saving, stopping, D, jmin, jmax, j, retval, Psample, factorized;
	time_t	start_time, current_time;
	double	reallyminerr = 1.0;
	double	reallymaxerr = 0.0;

	gwhandle *gwdata;
	ghandle *gdata;
	gwdata = (gwhandle*) malloc(sizeof(gwhandle));
	gwinit (gwdata);			// to be sure about giant usage...
	gdata = &gwdata->gdata;

	if ((gformat == ABCDN) || (gformat == ABCDNG)) {// Compute gk = gb^(n-m)-1
		bits = ndiff*bitlen (gb);
		gk = allocgiant ((bits >> 4) + 8);
		gtog (gb, gk);
		power (gk, ndiff);
		iaddg (-1, gk);
		sprintf (str, "%s^%lu-%s^%lu%c%d", sgb, n+ndiff, sgb, n, incr < 0 ? '-' : '+', abs(incr));
	}
	else {
		gk = allocgiant (strlen(sgk)/2 + 8);	// Allocate one byte per decimal digit + spares
		ctog (sgk, gk);						// Convert k string to giant
		grem = allocgiant (2*abs(gk->sign) + 8);	// place for mod (gk, gb)
		gshiftleft (shift, gk);				// Shift k multiplier if requested
		gtoc (gk, sgk1, sgkbufsize);		// Updated k string
		if (!strcmp(sgk1, "1"))
			sprintf (str, "%s^%lu%c%d", sgb, n, incr < 0 ? '-' : '+', abs(incr));
		else
			sprintf (str, "%s*%s^%lu%c%d", sgk1, sgb, n, incr < 0 ? '-' : '+', abs(incr));
	}

	bits = n * bitlen(gb) + bitlen(gk); 
	N =  allocgiant ((bits >> 4) + 8);		// Allocate memory for N

//	Be sure the base does not divide the gk multiplier :

	if ((gformat != ABCDN) && (gformat != ABCDNG)) {
		while (!isone(gk)) {
			gtog (gk,grem);
			modg (gb, grem);
			if (!isZero(grem))
				break;
			divg (gb, gk);
			n++;
		}
		free (grem);
	}

//	Compute the number we are testing.

	gtog (gb, N);
	power (N, n);

	Nlen = bitlen (N);					// Bit length of base^n

	mulg (gk, N); 

	iaddg (incr, N);

	klen = bitlen(gk);

	if (klen > 53 || generic || (abs(gb->sign) > 1)) {	// we must use generic reduction
		dk = 0.0;
	}
	else {								// we can use DWT ; compute the multiplier as a double
		dk = (double)gk->n[0];
		if (gk->sign > 1)
			dk += 4294967296.0*(double)gk->n[1];
		if (abs(gb->sign) == 1) {
			base = gb->n[0];
			globalb = base;
		}
	}

	globalk = dk;

	if ((nbdg = gnbdg(N, 10)) < 200) {			// Attempt an APRCL test for this small number...
		start_timer(1);
			resaprcl = gaprcltest (N, 0, 0);	// Primality test silently done
		end_timer (1);
		if (resaprcl == 10) {
			sprintf (buf,"%s is not prime. (Trial divisions)", str);
		}
		else if (resaprcl == 12)
			sprintf (buf,"%s is prime! (%lu decimal digits, Trial divisions)", str, nbdg);
		else if (resaprcl == 0) {
			goto PLMCONTINUE;					// Continue the PLM test to get the residue...
		}
		else if (resaprcl == 2)
			sprintf (buf,"%s is prime! (%lu decimal digits, APRCL test)", str, nbdg);
		else if (resaprcl == 6)
			sprintf (buf,"Invalid numerical string in %s\n", str);
		else if (resaprcl == 7) {
			sprintf (buf,"APRCL error while testing %s...\n", str);
			if (verbose)
				OutputBoth(buf);
			else
				OutputStr (buf);
			goto PLMCONTINUE;					// Continue the PLM test
		}
		else {
			if (resaprcl == 9)
				sprintf (buf, "APRCL primality test not available for %s\n", str);
			else
				sprintf (buf,"Unexpected return value : %d, while APRCL testing %s...\n", resaprcl, str);
			if (verbose)
				OutputBoth(buf);
			else
				OutputStr (buf);
			goto PLMCONTINUE;					// Continue the PLM test
		}
		*res = ((resaprcl == 2)||(resaprcl == 12));

#if defined(WIN32) && !defined(_CONSOLE)

		sprintf (buf+strlen(buf), "  Time : "); 

#else

		clearline(100);

		if (*res) {
#if defined (_CONSOLE)
		hConsole = GetStdHandle(STD_OUTPUT_HANDLE);	// Access to Console attributes
		SetConsoleTextAttribute(hConsole, BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
		OutputBoth(buf);
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
#else
		OutputStr("\033[7m");
		OutputBoth(buf);
		OutputStr("\033[0m");
#endif
		}
		else
			OutputBoth(buf);

		sprintf (buf, "  Time : "); 

#endif

/* Output the final timings for APRCL test */

		write_timer (buf+strlen(buf), 1, TIMER_CLR | TIMER_NL); 
		OutputBoth (buf);
		free (gk);
		free (N);
		return (TRUE); 
	}

PLMCONTINUE:

	if (klen > Nlen) {
		if ((gformat == ABCDN) || (gformat == ABCDNG))
		    sprintf(buf, "%s^%lu-1 > %s^%lu, so, only a PRP test is done for %s.\n", sgb, ndiff, sgb, n, str);
		else
		    sprintf(buf, "%s > %s^%lu, so, only a PRP test is done for %s.\n", sgk, sgb, n, str);
	    OutputBoth(buf);
		Nlen = bitlen (N);					// Bit length of N
		fpart = (1.0-(double)klen/Nlen)*100.0;
		retval = isPRPinternal (str, dk, gb, n, incr, res);
		if (res) {
			sprintf (buf, "(Factorized part = %4.2f%%)\n", fpart);
			OutputBoth (buf);
			if (nbdg < primolimit)
				MakePrimoInput (N, str);
		}
		free(gk);
		free(N);
		return retval;
	}

	Nlen = bitlen (N);					// Bit length of N

	nbdg = gnbdg (N, 10);				// Compute the number of decimal digits of the tested number.
	factorized = findgbpf (gb);			// Factorize the base if possible...
	for (jmax=29; (jmax>0) && !bpf[jmax]; jmax--);

	jmin = 0;							// Choose the minimum required factorized part.
	if (jmax) {							// The base is composite...
		for (j=jmax; j>0; j--) {
			if (bpf[j] == 1)
				factorized_part += n*vpf[j]*bitlen(gbpf[j]);
			else
				factorized_part += (unsigned long)floor(n*vpf[j]*log ((double)bpf[j])/log(2.0));
			if ((2*factorized_part) > Nlen)
				break;
		}
		jmin = j;
		sprintf (buf, "Base factorized as : ");

		for (j=0; j<=jmax; j++) {
			if (j<jmax) {
				if (bpf[j] == 1) {
					gtoc(gbpf[j], buf+strlen(buf), strlen(sgb));
				}
				else
					sprintf (buf+strlen(buf), "%lu", bpf[j]);
				if (vpf[j]>1)
					sprintf (buf+strlen(buf), "^%lu*", vpf[j]);
				else
					sprintf (buf+strlen(buf), "*");
			}
			else {
				if (bpf[j] == 1) {
					gtoc(gbpf[j], buf+strlen(buf), strlen(sgb));
				}
				else
					sprintf (buf+strlen(buf), "%lu", bpf[j]);
				if (vpf[j]>1)
					sprintf (buf+strlen(buf), "^%lu\n", vpf[j]);
				else
					sprintf (buf+strlen(buf), "\n");
			}
		}
		if (!setuponly) {
			if (verbose)
				OutputBoth(buf);
			else
				OutputStr(buf);
		}
	}


	if (factorized)
		sprintf (buf, "Base prime factor(s) taken : ");
	else
		sprintf (buf, "Base cofactor : ");

	for (j=jmin; j<=jmax; j++) {
		if (j<jmax)
			if (bpf[j] == 1) {
				gtoc(gbpf[j], buf+strlen(buf), strlen(sgb));
				sprintf (buf+strlen(buf), ", ");
			}
			else
				sprintf (buf+strlen(buf), "%lu, ", bpf[j]);
		else
			if (bpf[j] == 1) {
				gtoc(gbpf[j], buf+strlen(buf), strlen(sgb));
				if (factorized)
					sprintf (buf+strlen(buf), "\n");
				else
					sprintf (buf+strlen(buf), " (Must be proven prime or factorized externally)\n");
			}
			else
				sprintf (buf+strlen(buf), "%lu\n", bpf[j]);
	}

	if (!setuponly) {
		if (verbose)
			OutputBoth(buf);
		else
			OutputStr(buf);
	}

	maxrestarts = IniGetInt(INI_FILE, "MaxRestarts", 100);
	nrestarts = IniGetInt (INI_FILE, "NRestarts", 0);
	if (!(a = IniGetInt (INI_FILE, "FermatBase", 0)))
		a = IniGetInt (INI_FILE, "FBase", 3);// The base for the PRP and Pocklington tests
	if (!(P = IniGetInt (INI_FILE, "LucasBaseP", 0))) {
		Psample = genLucasBaseP (N, IniGetInt (INI_FILE, "PBase", 3));
		if (Psample < 0) {
			if (Psample == -1)
				sprintf (buf, "Cannot compute P to test %s...\nThis is surprising, please, let me know that!!\nMy E-mail is jpenne@free.fr\n", str);
			else
				sprintf (buf, "%s has a small factor : %d !!\n", str, abs(Psample));
			OutputBoth (buf);
			free(gk);
			free(N);
			return (TRUE); 
		}
		else
			P = Psample;
	}
											// The Discriminant for the Morrison test
	D = P*P-4;								// D = P^2 - 4*Q with Q = 1


 restart:

/* Setup the gwnum code */

	gwinit (gwdata);
	gdata = &gwdata->gdata;
 	gwset_num_threads (gwdata, IniGetInt(INI_FILE, "ThreadsPerTest", 1));

	*res = TRUE;

	M = allocgiant ((bits >> 4) + 8);		// Allocate memory for M
	tmp = allocgiant ((bits >> 4) + 8);	// Allocate memory for tmp
	tmp2 =allocgiant ((bits >> 4) + 8);	// Allocate memory for tmp2
	tmp3 =allocgiant ((bits >> 4) + 8);	// Allocate memory for tmp3
	gtog (N, tmp);
	ulsubg (1, tmp);					// tmp = N-1

	gwset_larger_fftlen_count(gwdata, (char)IniGetInt(INI_FILE, "FFT_Increment", 0));
	if (incr == +1) {
		gwsetmaxmulbyconst (gwdata, a);
		divg (gb, tmp);					// tmp = (N-1)/base
		explen = bitlen (tmp);
		if (dk >= 1.0) {
			if (!setupok (gwdata, gwsetup (gwdata, dk, base, n, +1))) {
				free(gk);
				free(N);
				free (M);
				free (tmp);
				free (tmp2);
				free (tmp3);
				*res = FALSE;		// Not proven prime...
				free (gwdata);
				return FALSE; 
			}
		}
		else {
			if (!setupok (gwdata, gwsetup_general_mod_giant (gwdata, N))) {
				free(gk);
				free(N);
				free (M);
				free (tmp);
				free (tmp2);
				free (tmp3);
				*res = FALSE;		// Not proven prime...
				free (gwdata);
				return FALSE; 
			}
		}
	}
	else {
		gwsetmaxmulbyconst (gwdata, max(a, P));
		gtog (N, M);
		iaddg (1, M);
		explen = bitlen (tmp);
		if (dk >= 1.0) {
			if (!setupok (gwdata, gwsetup (gwdata, dk, base, n, -1))) {
				free(gk);
				free(N);
				free (M);
				free (tmp);
				free (tmp2);
				free (tmp3);
				*res = FALSE;		// Not proven prime...
				free (gwdata);
				return FALSE; 
			}
		}
		else {
			if (!setupok (gwdata, gwsetup_general_mod_giant (gwdata, N))) {
				free(gk);
				free(N);
				free (M);
				free (tmp);
				free (tmp2);
				free (tmp3);
				*res = FALSE;		// Not proven prime...
				free (gwdata);
				return FALSE; 
			}
		}
		tempFileName (filename, 'L', N);
		if (fileExists (filename)) {	// Resuming a Lucas sequence...
			goto DoLucas;
		}
		else if (IniGetInt(INI_FILE, "Verify", 0) || IniGetInt(INI_FILE, "PRPdone", 0)) {
			clear_timers ();			// Init. timers
			sprintf (buf, "Starting Lucas sequence for %s...\n", str);
			goto DoLucas;				// Starting directly a Lucas sequence...
		}
	}

//	Restart with next FFT length if we are too near the limit...

	if (nextifnear && gwnear_fft_limit (gwdata, pcfftlim)) {
		OutputBoth ("Current FFT beeing too near the limit, next FFT length is forced...\n");
		IniWriteInt(INI_FILE, "FFT_Increment", IniGetInt(INI_FILE, "FFT_Increment", 0) + 1);
		free (M);
		free (tmp);
		free (tmp2);
		free (tmp3);
		gwdone (gwdata);
		goto restart;
	}

/* Init filename */

	tempFileName (filename, 'z', N);

/* Allocate memory */

	x = gwalloc (gwdata);
	y = gwalloc (gwdata);

/* Optionally resume from save file and output a message */
/* indicating we are resuming a test */

	if (fileExists (filename) && readFromFileB (gwdata, gdata, filename, &bit, &a, &nrestarts, bpf, x, y)) {
		char	fmt_mask[80];
		double	pct;
		pct = trunc_percent (bit * 100.0 / explen);
		sprintf (fmt_mask,
			 "Resuming N%%c%%d prime test of %%s at bit %%ld [%%.%df%%%%]\n",
			 PRECISION);
		sprintf (buf, fmt_mask, incr < 0 ? '+' : '-', abs(incr), str, bit, pct);
		OutputStr (buf);
		if (verbose || restarting)
			writeResults (buf);	
	}

/* Otherwise, output a message indicating we are starting test */

	else {
		if (setuponly) {
			if ((gwdata->FFTLEN != OLDFFTLEN)||debug) {
				OutputBoth (str);
				OutputBoth (" : ");
			}
		}
		else {
			if (frestart) {
				sprintf (buf, "Restarting N%c%d prime test of %s\n", incr < 0 ? '+' : '-', abs(incr), str);
				frestart = FALSE;
			}
			else {
				clear_timers ();		// Init. timers
				if (showdigits)
					sprintf (buf, "Starting N%c%d prime test of %s (%lu decimal digits)\n", incr < 0 ? '+' : '-', abs(incr), str, nbdg);
				else
					sprintf (buf, "Starting N%c%d prime test of %s\n", incr < 0 ? '+' : '-', abs(incr), str);
			}
		OutputStr (buf);
		if (verbose || restarting)
			writeResults (buf);	
		}
		bit = 1;
		dbltogw (gwdata, (double)a, x);
	}

/* Get the current time */

	start_timer (0);
	start_timer (1);
	time (&start_time);

/* Output a message about the FFT length */

	topology_print_children (hwloc_get_root_obj (hwloc_topology), 0);
	gwfft_description (gwdata, fft_desc);
	sprintf (buf, "Using %s, a = %lu\n", fft_desc,a);

	if (setuponly) {
		if ((gwdata->FFTLEN != OLDFFTLEN)||debug) {
			OutputBoth(buf);
			OLDFFTLEN = gwdata->FFTLEN;
		}
	}
	else if (verbose || restarting){
		OutputBoth(buf);
	}
	else {
		OutputStr(buf);
	}
	if (setuponly) {
		stopping = stopCheck ();
		free(gk);
		free(N);
                free(M);
		free(tmp);
		free(tmp2);
		free(tmp3);
		gwfree (gwdata, x);
		gwfree (gwdata, y);
		gwdone(gwdata);
		*res = FALSE;
		end_timer (1);
		free (gwdata);
		return(!stopping);
	}
	LineFeed ();
	ReplaceableLine (1);    /* Remember where replaceable line is */

/* Init the title */

	if (incr > 0)
		title ("Pocklington prime test in progress...");
	else
		title ("Fermat PRP test in progress...");

/* Do the PRP test */

	gwsetmulbyconst (gwdata, a);
	if (abs(gwdata->c) == 1)
		gwsetaddin(gwdata,0);
	iters = 0;
	
	while (bit < explen) {

/* Error check the first and last 50 iterations, before writing an */
/* intermediate file (either user-requested stop or a */
/* 30 minute interval expired), and every 128th iteration. */

		stopping = stopCheck ();
		echk = stopping || ERRCHK || (bit <= 50) || (bit >= explen-50) || gwnear_fft_limit (gwdata, pcfftlim);
		if (((bit & 127) == 0) || (bit == 1) || (bit == (lasterr_point-1))) {
			echk = 1;
			time (&current_time);
			saving = ((current_time - start_time > write_time) || (bit == 1) || (bit == (lasterr_point-1)));
		} else
			saving = 0;

/* Process this bit */

		gwstartnextfft (gwdata, postfft && !debug && !stopping && !saving && bit != lasterr_point && (bit+26 < explen) && (bit > 26) && !maxerr_recovery_mode[6]);

		if (bitval (tmp, explen-bit-1)) {
			gwsetnormroutine (gwdata, 0, echk, 1);
		} else {
			gwsetnormroutine (gwdata, 0, echk, 0);
		}

		if ((bit+25 < explen) && (bit > 25) && ((bit != lasterr_point) || !maxerr_recovery_mode[6])) {
			gwsquare (gwdata, x);
			care = FALSE;
		}
		else {
			gwsquare_carefully (gwdata, x);
			care = TRUE;
		}

		if (debug && (bit < 50))
			writeresidue (gwdata, x, N, tmp2, buf, str, bit, BIT);
		CHECK_IF_ANY_ERROR (x, (bit), explen, 6);

/* That iteration succeeded, bump counters */

		if (bit == lasterr_point)
			saving = 1;					// Be sure to restart after this recovery iteration!
		bit++;
		iters++;

/* Print a message every so often */

		if (bit % ITER_OUTPUT == 0) {
			char	fmt_mask[80];
			double	pct;
			pct = trunc_percent (bit * 100.0 / explen);
			sprintf (fmt_mask, "%%.%df%%%% of %%ld", PRECISION);
			sprintf (buf, fmt_mask, pct, explen);
			title (buf);
			ReplaceableLine (2);	/* Replace line */
			sprintf (fmt_mask,
				 "%%s, bit: %%ld / %%ld [%%.%df%%%%]",
				 PRECISION);
			sprintf (buf, fmt_mask, str, bit, explen, pct);
			OutputStr (buf);
			if (ERRCHK && bit > 30) {
				OutputStr (".  Round off: ");
				sprintf (buf, "%10.10f", reallyminerr);
				OutputStr (buf);
				sprintf (buf, " to %10.10f", reallymaxerr);
				OutputStr (buf);
			}
			end_timer (0);
			if (CUMULATIVE_TIMING) {
				OutputStr (".  Time thusfar: ");
			} else {
				OutputStr (".  Time per bit: ");
				divide_timer (0, iters);
				iters = 0;
			}
			print_timer (0, TIMER_NL | TIMER_OPT_CLR);
			start_timer (0);
		}

/* Print a results file message every so often */

		if (bit % ITER_OUTPUT_RES == 0 || (NO_GUI && stopping)) {
			sprintf (buf, "Bit %ld / %ld\n", bit, explen);
			writeResults (buf);
		}

/* Write results to a file every DISK_WRITE_TIME minutes */
/* On error, retry in 10 minutes (it could be a temporary */
/* disk-full situation) */

		if (saving || stopping) {
			write_time = DISK_WRITE_TIME * 60;
			saving = FALSE;
			if (! writeToFileB (gwdata, gdata, filename, bit, a, nrestarts, bpf, x, y)) {
				sprintf (buf, WRITEFILEERR, filename);
				OutputBoth (buf);
				if (write_time > 600) write_time = 600;
			}	
			time (&start_time);

/* If an escape key was hit, write out the results and return */

			if (stopping) {
				free (N);
				free (gk);
				free (M);
				free (tmp);
				free (tmp2);
				free (tmp3);
				gwfree (gwdata, x);
				gwfree (gwdata, y);
				gwdone (gwdata);
				*res = FALSE;
				free (gwdata);
				return (FALSE);
			}
		}

/* Output the 64-bit residue at specified interims.  Also output the */
/* residues for the next iteration so that we can compare our */
/* residues to programs that start counter at zero or one. */

		if (interimResidues && bit % interimResidues < 2)
			writeresidue (gwdata, x, N, tmp2, buf, str, bit, BIT);

/* Write a save file every "interimFiles" iterations. */

		if (interimFiles && bit % interimFiles == 0) {
			char	interimfile[20];
			sprintf (interimfile, "%.8s.%03lu",
				 filename, bit / interimFiles);
			if (! writeToFileB (gwdata, gdata, interimfile, bit, a, nrestarts, bpf, x, y)) {
				sprintf (buf, WRITEFILEERR, interimfile);
				OutputBoth (buf);
			}
		}
	}

	clearline (100);
	lasterr_point = 0;							// Reset last error point.

	if (incr == +1) {
		care = TRUE;							// All following errors are considered unrecoverable...
		bit = 1;
		explen = bitlen (gb);
		gwcopy (gwdata, x, y);
		gwstartnextfft(gwdata, FALSE);
		gwsetnormroutine (gwdata, 0, 1, 0);
		while (bit < explen) {
//			gwsquare (gwdata, x);
			gwsquare_carefully (gwdata, x);
			CHECK_IF_ANY_ERROR (x, (bit), explen, 6);
			if (bitval (gb, explen-bit-1)) {
//				gwsafemul (gwdata, y, x);
				gwmul_carefully (gwdata, y, x);
				CHECK_IF_ANY_ERROR (x, (bit), explen, 6);
			} 
			bit++;
		}
	}

	lasterr_point = 0;				// Reset last error point.

/* See if we've found a probable prime.  If not, format a 64-bit residue. */
/* Old versions of PRP used a non-standard 64-bit residue, computing */
/* 3^N-3 mod N rather than the more standard 3^(N-1) mod N.  Since */
/* some projects recorded these non-standard residues, output that */
/* residue too.  Note that some really old versions printed out the */
/* 32-bit chunks of the non-standard residue in reverse order. */

	ReplaceableLine (2);	/* Replace line */
	gwtogiant (gwdata, x, tmp);
	if (!isone (tmp)) {
		*res = FALSE;	/* Not a prime */
		if (abs(tmp->sign) < 2)	// make a 32 bit residue correct !!
			sprintf (res64, "%08lX%08lX", (unsigned long)0, (unsigned long)tmp->n[0]);
		else
			sprintf (res64, "%08lX%08lX", (unsigned long)tmp->n[1], (unsigned long)tmp->n[0]);
		imulg (a, tmp); specialmodg (gwdata, tmp); ulsubg (a, tmp);
		if (abs(tmp->sign) < 2)	// make a 32 bit residue correct !!
			sprintf (oldres64, "%08lX%08lX", (unsigned long)0, (unsigned long)tmp->n[0]);
		else
			sprintf (oldres64, "%08lX%08lX", (unsigned long)tmp->n[1], (unsigned long)tmp->n[0]);
		if (IniGetInt (INI_FILE, "OldRes64", 1))
			sprintf (buf, "%s is not prime.  RES64: %s.  OLD64: %s", str, res64, oldres64);
		else
			sprintf (buf, "%s is not prime.  RES64: %s", str, res64);
	}

	if (*res) {
		end_timer (1);
		_unlink (filename);
		if (!factorized) {
			gwfree (gwdata, x);
			gwfree (gwdata, y);
			gwdone (gwdata);
			sprintf (buf, "%s is a Probable Prime (Base incompletely factorized).", str);
		}
		else if (incr == -1) {				// Morrison test ; start the Lucas sequence
			gwfree (gwdata, x);
			gwfree (gwdata, y);
			sprintf (buf, "%s may be prime. Starting Lucas sequence...\n", str);
			IniWriteInt(INI_FILE, "PRPdone", 1);
DoLucas:
			do {
				retval = Lucasequence (gwdata, gdata, N, M, P, gb, jmin, jmax, str, buf, res);
				gwdone (gwdata);
				if (retval == -2) {			// Restart required using next base
					nrestarts++;
					if (nrestarts > maxrestarts) {
						sprintf (buf, "Giving up after %lu restarts...\n", nrestarts);
						frestart = FALSE;
						*res = FALSE;		// Not proven prime...
						retval = TRUE;
						break;
					}
					IniWriteInt (INI_FILE, "NRestarts", nrestarts);
					P = genLucasBaseP (N, P+1);
					IniWriteInt (INI_FILE, "LucasBaseP", P);
					D = P*P-4;
				}
				if (retval < 0)	{		// Restart required for any reason
					sprintf (buf, "Restarting Lucas sequence with P = %lu\n", P);
								// Setup again the gwnum code.
					gwinit (gwdata);
					gdata = &gwdata->gdata;
 					gwset_num_threads (gwdata, IniGetInt(INI_FILE, "ThreadsPerTest", 1));
					gwset_larger_fftlen_count(gwdata, (char)IniGetInt(INI_FILE, "FFT_Increment", 0));
					gwsetmaxmulbyconst (gwdata, max(a, P));
					if (dk >= 1.0) {
						if (!setupok (gwdata, gwsetup (gwdata, dk, base, n, -1))) {
							free(gk);
							free(N);
							free (M);
							free (tmp);
							free (tmp2);
							free (tmp3);
							*res = FALSE;		// Not proven prime...
							free (gwdata);
							return FALSE; 
						}
					}
					else {
						if (!setupok (gwdata, gwsetup_general_mod_giant (gwdata, N))) {
							free(gk);
							free(N);
							free (M);
							free (tmp);
							free (tmp2);
							free (tmp3);
							*res = FALSE;		// Not proven prime...
							free (gwdata);
							return FALSE; 
						}
					}
				}
			}	while (retval < 0);
			if (retval == FALSE) {				// Lucas sequence stopped.
				free (N);
				free (gk);
				free (M);
				free (tmp);
				free (tmp2);
				free (tmp3);
				*res = FALSE;
				free (gwdata);
				return FALSE;
			}
		}
		else {						// Pocklington test ; compute the gcd's
			sprintf (buf, "Computing GCD'S...");
			title (buf);
			sprintf (buf, "%s may be prime, trying to compute gcd's\n", str);
			OutputStr (buf);
			for (j=jmax; j>=jmin; j--) {
				if (bpf[j] == 0)	// base prime factor already tested
					continue;
				gwcopy (gwdata, y, x);		// Computing a^((N-1)/q)
				bit = 1;
				lasterr_point = 0;			// Reset last error point.
				explen = bitlen (gbpc[j]);
				while (bit < explen) {
//					gwsquare (gwdata, x);
					gwsquare_carefully (gwdata, x);
					CHECK_IF_ANY_ERROR (x, (bit), explen, 6);
					if (bitval (gbpc[j], explen-bit-1)) {
//						gwsafemul (gwdata, y, x);
						gwmul_carefully (gwdata, y, x);
						CHECK_IF_ANY_ERROR (x, (bit), explen, 6);
					}
					bit++;
				}
				gwtogiant (gwdata, x, tmp);
				if (isone (tmp)) {
					if (frestart)
						continue;
					if (a==2)		// Choose prime bases to have less restarts...
						newa = 3;
					else {
						if (!(a&1))
							newa = a + 1;
						else
							newa = a + 2;
						while (!isPrime(newa))
							newa += 2;
					}
					nrestarts++;
					if (nrestarts > maxrestarts) {
						if (bpf[j] == 1) {
							gtoc(gbpf[j], bpfstring, strlen(sgb));
							sprintf (buf, "%s may be prime, but N divides %lu^((N-1)/%s))-1, giving up after %lu restarts...", str, a, bpfstring, maxrestarts);
						}
						else
							sprintf (buf, "%s may be prime, but N divides %lu^((N-1)/%lu))-1, giving up after %lu restarts...", str, a, bpf[j], maxrestarts);
						frestart = FALSE;
						*res = FALSE;		// Not proven prime...
					}
					else {
						if (bpf[j] == 1) {
							gtoc(gbpf[j], bpfstring, strlen(sgb));
							sprintf (buf, "%s may be prime, but N divides %lu^((N-1)/%s))-1, restarting with a=%lu", str, a, bpfstring, newa);
						}
						else
							sprintf (buf, "%s may be prime, but N divides %lu^((N-1)/%lu))-1, restarting with a=%lu", str, a, bpf[j], newa);
						a = newa;
						IniWriteInt (INI_FILE, "NRestarts", nrestarts);
						IniWriteInt (INI_FILE, "FermatBase", a);
						frestart = TRUE;
					}
				}
				else {
					ulsubg (1, tmp);
					gcdg (N, tmp);
					if (isone (tmp)) {
						if (bpf[j] == 1) {
							gtoc(gbpf[j], bpfstring, strlen(sgb));
							sprintf (buf, "%lu^((N-1)/%s)-1 is coprime to N!\n", a, bpfstring);
						}
						else
							sprintf (buf, "%lu^((N-1)/%lu)-1 is coprime to N!\n", a, bpf[j]);
						OutputStr (buf);
						if (verbose)
							writeResults (buf);
						bpf[j] = 0;			// success for this prime factor of the base, continue
					}
					else {
						*res = FALSE;		/* Not a prime */
						sprintf (buf, "%s is not prime, although %lu Fermat PSP!!.", str, a);
						break;				// No need to continue...
					}
				}
			}
			if (*res && !frestart)
				sprintf (buf, "%s is prime! (%lu decimal digits)", str, nbdg);
			gwfree (gwdata, x);
			gwfree (gwdata, y);
			gwdone (gwdata);
		}
	}
	else {
		gwfree (gwdata, x);
		gwfree (gwdata, y);
		gwdone (gwdata);
	}

	if (!frestart) {
		free (N);
		free (gk);
	}
	free (M);
	free (tmp);
	free (tmp2);
	free (tmp3);
//	gwfree (gwdata, x);
//	gwfree (gwdata, y);

#if defined(WIN32) && !defined(_CONSOLE)

	sprintf (buf+strlen(buf), "  Time : "); 
//	ReplaceableLine (2);	/* Replace line */ 

#else // cllr, linux or Mac Intel

	clearline(100);

	if (*res) {
#if defined (_CONSOLE)
		hConsole = GetStdHandle(STD_OUTPUT_HANDLE);	// Access to Console attributes
		SetConsoleTextAttribute(hConsole, BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
		OutputBoth(buf);
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
#else
		OutputStr("\033[7m");
		OutputBoth(buf);
		OutputStr("\033[0m");
#endif
	}
	else
		OutputBoth(buf);

	sprintf (buf, "  Time : "); 

#endif

/* Output the final timings */

	end_timer (1);
//	sprintf (buf+strlen(buf)-1, "  Time: ");
	write_timer (buf+strlen(buf), 1, TIMER_CLR | TIMER_NL); 
	if (!frestart) {
		OutputBoth (buf);
		IniWriteString (INI_FILE, "NRestarts", NULL);
		if (incr == 1)
			IniWriteString (INI_FILE, "FermatBase", NULL);
		else
			IniWriteString (INI_FILE, "LucasBaseP", NULL);
	}
	else {
		OutputStr (buf);
		if (incr == 1)
			writeResults (buf);
	}

/* Cleanup and return */

//	gwdone (gwdata);
	_unlink (filename);
	lasterr_point = 0;
	if (frestart)
		goto restart;
	if (IniGetInt(INI_FILE, "PRPdone", 0))
		IniWriteString(INI_FILE, "PRPdone", NULL);
	IniWriteString(INI_FILE, "FFT_Increment", NULL);
	free (gwdata);
	return (TRUE);

/* An error occured, sleep if required, then try restarting at last save point. */

error:
	free (M);
	free (tmp);
	free (tmp2);
	free (tmp3);
	gwfree (gwdata, x);
	gwfree (gwdata, y);
	*res = FALSE;

	if ((abonillsum && gw_test_illegal_sumout(gwdata)) || 
		(abonmismatch && gw_test_mismatched_sums (gwdata)) || 
		(abonroundoff && gw_get_maxerr(gwdata) > maxroundoff)) {	// Abort...
		aborted = TRUE;
		sprintf (buf, ERRMSG5, checknumber, str);
		OutputBoth (buf);
		free (N);
		free (gk);
		_unlink (filename);
		if (IniGetInt(INI_FILE, "PRPdone", 0))
			IniWriteString(INI_FILE, "PRPdone", NULL);
		IniWriteString(INI_FILE, "FFT_Increment", NULL);
		gwdone (gwdata);
		free (gwdata);
		if(IniGetInt(INI_FILE, "StopOnAbort", 0)) {
			IniWriteInt (INI_FILE, "PgenLine", IniGetInt(INI_FILE, "PgenLine", 0) + 1);	// Point on the next line
			return (FALSE);
		}
		else
			return (TRUE);
	}

	gwdone (gwdata);

	/* Output a message saying we are restarting */

	if (sleep5) OutputBoth (ERRMSG2);
	OutputBoth (ERRMSG3);

/* Sleep five minutes before restarting */

	if (sleep5 && ! SleepFive ()) { 
		free (gwdata);
		return (FALSE);
	}

/* Restart */

	if (will_try_larger_fft) {
		IniWriteInt(INI_FILE, "FFT_Increment", nbfftinc = (IniGetInt(INI_FILE, "FFT_Increment", 0) + 1));
		if (nbfftinc == maxfftinc)
			abonroundoff = TRUE;	// Don't accept any more Roundoff error.
	}
	goto restart;

}

/*
 	Primality testing of k*2^n-1 numbers with the Lucas Lehmer Riesel
	algorithm, using the gwnums for fast multiplications and squarings.
	Second attempt for a full IBDWT version, using Colin Percival method improved
	by George Woltman.
	Jean Penn�, May 2004.
*/

int isLLRP ( 
	unsigned long format, 
	char *sgk,
	unsigned long b_else,	// Lei
	unsigned long n, 
	unsigned long binput,		// Lei
	unsigned long ninput,		// Lei
	unsigned long shift,
	int	*res) 
{ 
	unsigned long iters, index; 
	unsigned long p, gksize, j, k; 
	unsigned long mask, last, bit, bits; 
	long	vindex, retval;
	gwhandle *gwdata;
	ghandle *gdata;
	gwnum	x, y; 
	giant	tmp, gbinput;
	char	filename[20], buf[sgkbufsize+256], str[sgkbufsize+256],
			sgk1[sgkbufsize], fft_desc[256]; 
	long	write_time = DISK_WRITE_TIME * 60; 
	int		resaprcl, echk, saving, stopping, v1, first = 1, inc = -1; 
	time_t	start_time, current_time; 
	double	reallyminerr = 1.0; 
	double	reallymaxerr = 0.0; 
	double	dk;

// Lei
	double ddk;
	unsigned long idk = 0;
	giant gk1;
// Lei end

// init. gwnum code
	gwdata = (gwhandle*) malloc(sizeof(gwhandle));
	gwinit (gwdata);
	gdata = &gwdata->gdata;

	if (!(format == ABCC || format == ABCK)) {
// Lei
		if (b_else != 1) {					// Compute the length of b_else^ninput
			ddk = (double) b_else;
			ddk = ninput * log (ddk)/log (2.0);
			idk = (long) ddk + 1;
		}
		else
			idk = 0;
// Lei end
		if ((format == ABCDN) || (format == ABCDNG)) {	// Compute gk = gb^(n-m)-1
			gksize = ndiff*(unsigned long)ceil(log ((double)binput)/log (2.0))+idk;// initial gksize
			gk = allocgiant ((gksize >> 4) + 8);		// Allocate space for gk
			ultog (binput, gk);
			power (gk, ndiff);
			iaddg (-1, gk);
			sprintf (str, "%lu^%lu-%lu^%lu-1", binput, ninput+ndiff, binput, ninput);
		}
		else {
			gksize = 8*strlen(sgk) + idk;		// J. P. Initial gksize
			gk = allocgiant ((gksize >> 4) + 8);	// Allocate space for gk
			ctog (sgk, gk);						// Convert k string to giant
		}

		klen = bitlen(gk);			// Bit length of initial k multiplier

		if (klen > 53 || generic) {	// we must use generic reduction
			dk = 0.0;
		}
		else {						// we can use DWT ; compute the multiplier as a double
			dk = (double)gk->n[0];
			if (gk->sign > 1)
				dk += 4294967296.0*(double)gk->n[1];
		}

// Lei
		if (b_else != 1) {					// Compute the big multiplier
			gk1 = allocgiant ((gksize>>4) + 8);
			ultog (b_else, gk1);		
			power (gk1, ninput);
			mulg (gk1, gk);
			free (gk1);
		}
// Lei end

		if (shift > 0) {
			gshiftleft (shift, gk);			// Shift k multiplier if requested
			dk *= (double) (1<<shift);		// Update dk... J.P. 11/02/11
			if (b_else != 1)
				strcpy (sgk1, sgk);			// Lei, J.P.
			else
				gtoc (gk, sgk1, sgkbufsize);// Updated k string
		}
		else {
			strcpy (sgk1, sgk);
		}
		if ((format != ABCDN) && (format != ABCDNG)) {
			if (b_else != 1)	// Lei, J.P.
				sprintf (str, "%s*%lu^%lu%c1", sgk, binput, ninput, '-');// Number N to test, as a string
			else
				sprintf (str, "%s*2^%lu%c1", sgk1, n, '-');	// Number N to test, as a string
		}

//	gk must be odd for the LLR test, so, adjust gk and n if necessary.


		while (!bitval(gk, 0)) {
			gshiftright (1, gk);	// update k as a giant
			n++;
		}
	}
	else {
		gk = allocgiant ((n>>4)+8);
		setone (gk);						// Compute k multiplier
		gshiftleft (n-2, gk);				// Warning : here, n is exponent+1 !
		if (format == ABCK) {
			uladdg (1, gk);
			sprintf (str, "%s*2^%lu%c1 = (2^%lu+1)^2 - 2", sgk, n, '-', n-1);
		}
		else {
			ulsubg (1, gk);
			sprintf (str, "%s*2^%lu%c1 = (2^%lu-1)^2 - 2", sgk, n, '-', n-1);
		}
	}

	klen = bitlen(gk);					// Bit length ok k multiplier
	if (klen > 53 || generic) {	// we must use generic reduction. 12/06/20
		dk = 0.0;
	}
	else {						// we can use DWT ; compute the multiplier as a double
		dk = (double)gk->n[0];
		if (gk->sign > 1)
			dk += 4294967296.0*(double)gk->n[1];
	}
	bits = n + klen;					// Bit length of N
	N =  allocgiant ((bits >> 4) + 8);	// Allocate memory for N

//	Compute the number we are testing.

	setone (N);
	gshiftleft (n, N);
	mulg (gk, N); 
	ulsubg (1, N);

	Nlen = bitlen (N); 
	nbdg = gnbdg (N, 10); // Compute the number of decimal digits of the tested number.

	globalk = dk;
	if ((nbdg = gnbdg(N, 10)) < 200) {			// Attempt an APRCL test for this small number...
		start_timer(1);
		resaprcl = gaprcltest (N, 0, 0);	// Primality test silently done
		end_timer (1);
		if (resaprcl == 10) {
			sprintf (buf,"%s is not prime. (Trial divisions)", str);
		}
		else if (resaprcl == 12)
			sprintf (buf,"%s is prime! (%lu decimal digits, Trial divisions)", str, nbdg);
		else if (resaprcl == 0) {
			goto LLRCONTINUE;					// Continue the LLR test to get the residue...
		}
		else if (resaprcl == 2)
			sprintf (buf,"%s is prime! (%lu decimal digits, APRCL test)", str, nbdg);
		else if (resaprcl == 6)
			sprintf (buf,"Invalid numerical string in %s\n", str);
		else if (resaprcl == 7) {
			sprintf (buf,"APRCL error while testing %s...\n", str);
			if (verbose)
				OutputBoth(buf);
			else
				OutputStr (buf);
			goto LLRCONTINUE;					// Continue the LLR test
		}
		else {
			if (resaprcl == 9)
				sprintf (buf, "APRCL primality test not available for %s\n", str);
			else
				sprintf (buf,"Unexpected return value : %d, while APRCL testing %s...\n", resaprcl, str);
			if (verbose)
				OutputBoth(buf);
			else
				OutputStr (buf);
			goto LLRCONTINUE;					// Continue the LLR test
		}
		*res = ((resaprcl == 2)||(resaprcl == 12));

#if defined(WIN32) && !defined(_CONSOLE)

		sprintf (buf+strlen(buf), "  Time : "); 

#else

		clearline(100);

		if (*res) {
#if defined (_CONSOLE)
		hConsole = GetStdHandle(STD_OUTPUT_HANDLE);	// Access to Console attributes
		SetConsoleTextAttribute(hConsole, BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
		OutputBoth(buf);
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
#else
		OutputStr("\033[7m");
		OutputBoth(buf);
		OutputStr("\033[0m");
#endif
		}
		else
			OutputBoth(buf);

		sprintf (buf, "  Time : "); 


#endif

/* Output the final timings for APRCL test */

		write_timer (buf+strlen(buf), 1, TIMER_CLR | TIMER_NL); 
		OutputBoth (buf);
		free (gk);
		free (N);
		return (TRUE); 
	}

LLRCONTINUE:

	if (klen > n) {
		if ((format == ABCDN) || (format == ABCDNG))
			sprintf(buf, "2^%lu-1 > 2^%lu, so we can only do a PRP test for %s.\n", ndiff, n, str);
		else
			sprintf(buf, "%s > 2^%lu, so we can only do a PRP test for %s.\n", sgk, n, str);
	    OutputBoth(buf);
// Lei
		if ((format == ABCDN) || (format == ABCDNG))
			sprintf (str, "%lu^%lu-%lu^%lu-1", binput, ninput+ndiff, binput, ninput);
		else
			sprintf (str, "%s*%lu^%lu%c1", sgk, binput, ninput, '-');     // Number N to test, as a string
		gbinput = allocgiant (2);
		gbinput->sign = 1;
		gbinput->n[0] = 2;
		fpart = (1.0-(double)klen/Nlen)*100.0;
		retval = isPRPinternal (str, dk, gbinput, n, -1, res);
// Lei end
		if (res) {
			sprintf (buf, "(Factorized part = %4.2f%%)\n", fpart);
			OutputBoth (buf);
			if (nbdg < primolimit)
				MakePrimoInput (N, str);
		}

		free(gbinput);
		free(gk);
		free(N);
		return retval;
	}

	if ((v1 = gen_v1(gk, n, general, eps2, debug)) < 0) {
		if (v1 == -1)
			sprintf (buf, "Cannot compute V1 to test %s...\nThis is surprising, please, let me know that!!\nMy E-mail is jpenne@free.fr\n", str);
		else
			sprintf (buf, "%s has a small factor : %d !!\n", str, abs(v1));
		OutputBoth (buf); 
		free(gk);
		free(N);
		*res = FALSE;
		return(TRUE);
	}

	if (!IniGetInt(INI_FILE, "Verify", 0) && !IniGetInt(INI_FILE, "PRPdone", 0) && ((Nlen/klen < 10.0) || IniGetInt (INI_FILE, "ErrorChecking", 1))) {
								// We have better to do ad first a Gerbicz PRP test.
// Lei
		strcpy (buf, str);
		if ((format == ABCDN) || (format == ABCDNG))
			sprintf (str, "%lu^%lu-%lu^%lu-1", binput, ninput+ndiff, binput, ninput);
		else
			sprintf (str, "%s*%lu^%lu%c1", sgk, binput, ninput, '-');     // Number N to test, as a string
		Fermat_only = TRUE;
		gbinput = allocgiant (2);
		gbinput->sign = 1;
		gbinput->n[0] = 2;
		fpart = 0.0;
		retval = isPRPinternal (str, dk, gbinput, n, -1, res);
		free(gbinput);
		Fermat_only = FALSE;
// Lei end
		if (!*res) {
			free(gk);
			free(N);
			return retval;
		}
		IniWriteInt(INI_FILE, "PRPdone", 1);
		strcpy (str, buf);	// Lei
	}


	if(abs(gk->sign) == 1) {	// k is a "small" integer
	    k = gk->n[0];
	}
	else {
		k = 0;					// to indicate that k is a big integer
	}

restart: 

	gwinit (gwdata);
	gdata = &gwdata->gdata;
 	gwset_num_threads (gwdata, IniGetInt(INI_FILE, "ThreadsPerTest", 1));

	vindex = 1;					// First attempt

	p = Nlen; 

	*res = TRUE;		/* Assume it is prime */ 

	gwset_larger_fftlen_count(gwdata, (char)IniGetInt(INI_FILE, "FFT_Increment", 0));
	if (dk >= 1.0) {
		if (!setupok (gwdata, gwsetup (gwdata, dk, binput, ninput, -1))) { 
			free(gk);
			free(N);
			free(gwdata);
			*res = FALSE;
			return FALSE;
		}
		w->k = dk;
		w->n = ninput;
	}
	else {
		if (!setupok (gwdata, gwsetup_general_mod_giant (gwdata, N))) {
			free(gk);
			free(N);
			free(gwdata);
			*res = FALSE;
			return FALSE;
		}

	}

//	Restart with next FFT length if we are too near the limit...

	if (nextifnear && gwnear_fft_limit (gwdata, pcfftlim)) {
		OutputBoth ("Current FFT beeing too near the limit, next FFT length is forced...\n");
		IniWriteInt(INI_FILE, "FFT_Increment", IniGetInt(INI_FILE, "FFT_Increment", 0) + 1);
		gwdone (gwdata);
		goto restart;
	}

	if (!IniGetInt(INI_FILE, "Testdiff", 0))	// Unless test of MAXDIFF explicitly required
		gwdata->MAXDIFF = 1.0E80;				// Diregard MAXDIFF...

	x = gwalloc (gwdata); 
	y = gwalloc (gwdata);
 
	tmp =  popg (gdata, (Nlen >> 5) + 3); 

	last = n-1;

 	gwsetnormroutine (gwdata, 0, ERRCHK, 0); 

/* Init filename */ 

	tempFileName (filename, 'z', N); 
 
/* Init the title */ 
 
	title ("L.L.R. prime test in progress...");
 
/* Optionally resume from save file and output a message */ 
/* indicating we are resuming a test */ 
 
	if (fileExists (filename) && readFromFile (gwdata, gdata, filename, &j, x, NULL)) { 
		char	fmt_mask[80]; 
		double	pct; 
		pct = trunc_percent (j * 100.0 / n); 
		sprintf (fmt_mask, 
			"Resuming LLR test of %%s at iteration %%ld [%%.%df%%%%]\n", 
			PRECISION); 
		sprintf (buf, fmt_mask, str, j, pct); 
		OutputStr (buf); 
		if (verbose || restarting)
			writeResults (buf);
		start_timer (0); 
		start_timer (1); 
		time (&start_time); 
	} 
 

/* Otherwise, output a message indicating we are starting test, */ 
/* or resuming the computing of U0. */
 
	else { 
	    if (k==1) {
			if (!isPrime (n)) {
				sprintf (buf, "The Mersenne number %s is not prime because %lu is not prime.\n", str, n); 
				OutputBoth (buf); 
				pushg (gdata, 1); 
				free(gk);
				free(N);
				gwfree (gwdata, x); 
				gwfree (gwdata, y);
				gwdone(gwdata);
				*res = FALSE;
				end_timer (1); 
				free (gwdata);
				return(TRUE);
			}
			sprintf (buf, 
				"Prime95 or Mprime are much better to test this Mersenne number !!\n");
			if (verbose)
				OutputBoth(buf);
			else
				OutputStr(buf);
			if (showdigits)
				sprintf (buf, "Starting Lucas Lehmer prime test of %s (%lu decimal digits)\n", str, nbdg);
			else
				sprintf (buf, "Starting Lucas Lehmer prime test of %s\n", str);
			if (verbose)
				OutputBoth(buf);
			else
				OutputStr (buf); 
			topology_print_children (hwloc_get_root_obj (hwloc_topology), 0);
			gwfft_description (gwdata, fft_desc);
			sprintf (buf, "%s\n", fft_desc);
			if (verbose)
				OutputBoth(buf);
			else
				OutputStr (buf); 
			v1 = 4;
			dbltogw (gwdata, (double) v1, x);
			clear_timers ();		// Init. timers
			start_timer (0); 
			start_timer (1); 
			time (&start_time); 
			goto MERSENNE;
	    }
	    filename[0] = 'u';
	    if (fileExists (filename) && readFromFile (gwdata, gdata, filename, &j, x, y)) { 
			char	fmt_mask[80]; 
			double	pct; 
			pct = trunc_percent (100.0 - j * 100.0 / klen); 
			sprintf (fmt_mask, 
			 "Resuming test of %%s (computing U0) at iteration %%ld [%%.%df%%%%]", 
			 PRECISION); 
			sprintf (buf, fmt_mask, str, klen - j, pct); 
			OutputStr (buf); 
			OutputStr ("\n");
			if (verbose || restarting) {
				strcat (buf, "\n");
				writeResults (buf);
			}
			ReplaceableLine (1);	/* Remember where replacable line is */ 
			start_timer (0); 
			start_timer (1); 
			time (&start_time); 
	    } 
	    else {
			clear_timers ();		// Init. timers
			start_timer (0); 
			start_timer (1); 
			time (&start_time); 
			if (setuponly) {
				if ((gwdata->FFTLEN != OLDFFTLEN)||debug) {
					OutputBoth (str); 
					OutputBoth (" : "); 
				}
			}
			else {
				if (showdigits)
					sprintf (buf, "Starting Lucas Lehmer Riesel prime test of %s (%lu decimal digits)\n", str, nbdg);
				else
					sprintf (buf, "Starting Lucas Lehmer Riesel prime test of %s\n", str);
				OutputStr (buf); 
				if (verbose || restarting)
					writeResults (buf);
			}
			topology_print_children (hwloc_get_root_obj (hwloc_topology), 0);
			gwfft_description (gwdata, fft_desc);
			sprintf (buf, "Using %s\n", fft_desc);

			if (setuponly) {
				if ((gwdata->FFTLEN != OLDFFTLEN)||debug) {
					OutputBoth(buf);
					OLDFFTLEN = gwdata->FFTLEN;
				}
			}
			else if (verbose || restarting)
				OutputBoth(buf);
			else {
				OutputStr(buf);
			}
			if (setuponly) {
				stopping = stopCheck (); 
				pushg (gdata, 1); 
				free(gk);
				free(N);
				gwfree (gwdata, x); 
				gwfree (gwdata, y);
				gwdone(gwdata);
				*res = FALSE;
				end_timer (1); 
				free (gwdata);
				return(!stopping);
			}

			sprintf (buf, "V1 = %d ; Computing U0...", v1);
			OutputStr (buf); 
			LineFeed();
			ReplaceableLine (1);				/* Remember where replacable line is */ 
			gwstartnextfft (gwdata, FALSE);		/* Disable POSTFFT, to be sure... */
			dbltogw (gwdata, (double) v1, x); 
			if (debug)
				writeresidue (gwdata, x, N, tmp, buf, str, 0, ITER);
			gwcopy (gwdata, x, y);
			if (debug)
				writeresidue (gwdata, y, N, tmp, buf, str, 0, ITER);
			gwsetnormroutine (gwdata, 0, 1, 0);
			gwsetaddin (gwdata, -2);
			if ((1 != lasterr_point) || !maxerr_recovery_mode[0]) {
				gwsquare (gwdata, y);
				care = FALSE;
			}
			else {
				gwsquare_carefully (gwdata, y);
				care = TRUE;
			}
			if (debug)
				writeresidue (gwdata, y, N, tmp, buf, str, 1, ITER);
			CHECK_IF_ANY_ERROR(y, 1, klen, 0)
			if (1 == lasterr_point)
				saving = 1;					// Be sure to restart after this recovery iteration!
			j = klen - 2;
 	    }
					/* Computing u0 (cf Hans Riesel) */
	    iters = 0; 
	    while (j>0) {
 
/* Process this iteration */ 

			mask = 1<<j;

			if (k)
				bit = (k&mask);
			else
				bit = bitval (gk, j);
 
			index = klen-j--;
			iters++;

/* Error check the first 50 iterations, before writing an */ 
/* intermediate file (either user-requested stop or a */ 
/* 30 minute interval expired), and every 128th iteration. */ 
 
			stopping = stopCheck (); 
			echk = stopping || ERRCHK || (index <= 50) || gwnear_fft_limit (gwdata, pcfftlim); 
			if (((index & 127) == 0) || (index == 2) || (index == (lasterr_point-1))) {
				echk = 1;
				time (&current_time);
				saving = ((current_time - start_time > write_time) || (index == 2) || (index == (lasterr_point-1)));
			} else
				saving = 0;

			gwsetnormroutine (gwdata, 0, echk, 0);

			if (bit) {
				gwsetaddin (gwdata, -v1);
				if ((index != lasterr_point) || !maxerr_recovery_mode[1]) {
					gwsafemul (gwdata, y, x);
					care = FALSE;
				}
				else {
					gwmul_carefully (gwdata, y, x);
					care = TRUE;
				}
				if (debug && (index < 30))
					writeresidue (gwdata, x, N, tmp, buf, str, index, ITER);
				CHECK_IF_ANY_ERROR(x, (index), klen, 1)
				gwsetaddin (gwdata, -2);
				if ((index != lasterr_point) || !maxerr_recovery_mode[2]) {
					gwsquare (gwdata, y);
					care = FALSE;
				}
				else {
					gwsquare_carefully (gwdata, y);
					care = TRUE;
				}
				if (debug && (index < 30))
					writeresidue (gwdata, y, N, tmp, buf, str, index, ITER);
				CHECK_IF_ANY_ERROR(y, (index), klen, 2)
			}
			else {
				gwsetaddin (gwdata, -v1);
				if ((index != lasterr_point) || !maxerr_recovery_mode[3]) {
					gwsafemul (gwdata, x, y);
					care = FALSE;
				}
				else {
					gwmul_carefully (gwdata, x, y);
					care = TRUE;
				}
				if (debug && (index < 30))
					writeresidue (gwdata, y, N, tmp, buf, str, index, ITER);
				CHECK_IF_ANY_ERROR(y, (index), klen, 3)
				gwsetaddin (gwdata, -2);
				if ((index != lasterr_point) || !maxerr_recovery_mode[4]) {
					gwsquare (gwdata, x);
					care = FALSE;
				}
				else {
					gwsquare_carefully (gwdata, x);
					care = TRUE;
				}
				if (debug && (index < 30))
					writeresidue (gwdata, x, N, tmp, buf, str, index, ITER);
				CHECK_IF_ANY_ERROR(x, (index), klen, 4)
			}

			if (index == lasterr_point)
				saving = 1;					// Be sure to restart after this recovery iteration!

/* Print a message every so often */ 
 
			if (index % ITER_OUTPUT == 0) { 
				char	fmt_mask[80]; 
				double	pct; 
				pct = trunc_percent (100.0 - j * 100.0 / klen); 
				if (strlen (str) < 40) {
					sprintf (fmt_mask, "%%s, %%.%df%%%% of %%ld", PRECISION); 
					sprintf (buf, fmt_mask, str, pct, klen); 
				}
				else {
					sprintf (fmt_mask, "%%.%df%%%% of %%ld", PRECISION); 
					sprintf (buf, fmt_mask, pct, klen); 
				}
				title (buf); 
				ReplaceableLine (2);	/* Replace line */ 
				sprintf (fmt_mask, 
				 "%%s, iteration : %%ld / %%ld [%%.%df%%%%]", 
				 PRECISION); 
				sprintf (buf, fmt_mask, str, index, klen, pct); 
				OutputStr (buf); 
				if (ERRCHK && index > 30) { 
					OutputStr (".  Round off: "); 
					sprintf (buf, "%10.10f", reallyminerr); 
					OutputStr (buf); 
					sprintf (buf, " to %10.10f", reallymaxerr); 
					OutputStr (buf); 
				} 
				end_timer (0); 
				if (CUMULATIVE_TIMING) { 
					OutputStr (".  Time thusfar : "); 
				} 
				else { 
					OutputStr (".  Time per iteration : "); 
					divide_timer (0, iters); 
					iters = 0; 
				} 
				print_timer (0, TIMER_NL | TIMER_OPT_CLR); 
				start_timer (0); 
			} 
 
/* Print a results file message every so often */ 
 
			if (index % ITER_OUTPUT_RES == 0 || (NO_GUI && stopping)) { 
				sprintf (buf, "Iteration %ld / %ld\n", index, klen); 
				writeResults (buf); 
			} 
 
/* Write results to a file every DISK_WRITE_TIME minutes */ 
/* On error, retry in 10 minutes (it could be a temporary */ 
/* disk-full situation) */ 
 
			if (saving || stopping) { 
				write_time = DISK_WRITE_TIME * 60; 
				saving = FALSE;
				if (! writeToFile (gwdata, gdata, filename, j, x, y)) { 
					sprintf (buf, WRITEFILEERR, filename); 
					OutputBoth (buf); 
					if (write_time > 600) write_time = 600; 
				} 
				time (&start_time); 
 
/* If an escape key was hit, write out the results and return */ 
 
				if (stopping) {
					pushg (gdata, 1); 
					free(gk);
					free(N);
					gwfree (gwdata, x); 
					gwfree (gwdata, y);
					gwdone(gwdata);
					free (gwdata);
					*res = FALSE;	// JP 09/08/16
					return (FALSE); 
				}
			} 
	    }

		gwsetaddin (gwdata, -v1);
		if ((klen != lasterr_point) || !maxerr_recovery_mode[5]) {
			gwmul (gwdata, y, x);
			care = FALSE;
		}
		else {
			gwmul_carefully (gwdata, y, x);
			care = TRUE;
		}
		if (debug)
			writeresidue (gwdata, x, N, tmp, buf, str, klen, ITER);
		CHECK_IF_ANY_ERROR(x, klen, klen, 5)

		ReplaceableLine (2);	/* Replace line */ 
#ifdef WIN32
		sprintf (buf, "V1 = %d ; Computing U0...done.\n", v1);
#else
		sprintf (buf, "V1 = %d ; Computing U0...done.", v1);
#endif
		OutputStr(buf);
		if (verbose || restarting) {
			sprintf (buf, "V1 = %d ; Computing U0...done.\n", v1);
			writeResults (buf); 
		}
							/* End of x = u0 computing */
		_unlink (filename);	/* Remove the save file */
	    filename[0] = 'z';	/* restore filename which was modified... */
		lasterr_point = 0;	// Reset last error point.
	    sprintf (buf, "Starting Lucas-Lehmer loop..."); 
	    OutputStr (buf); 
            LineFeed();

MERSENNE:
		j = 1;
	} 


/* Do the Lucas Lehmer Riesel Prime test */ 

	ReplaceableLine (1);	/* Remember where replacable line is */  
	iters = 0; 

	gwsetaddin (gwdata, -2);

	while (j<last) { 

/* Error check the first and last 50 iterations, before writing an */ 
/* intermediate file (either user-requested stop or a */ 
/* 30 minute interval expired), and every 128th iteration. */ 
		stopping = stopCheck (); 
		echk = stopping || ERRCHK || (j <= 50) || (j >= last - 50) || gwnear_fft_limit (gwdata, pcfftlim); 
		if (((j & 127) == 0) || (j == 1) || (j == (lasterr_point-1))) {
			echk = 1;
			time (&current_time);
			saving = ((current_time - start_time > write_time) || (j == 1) || (j == (lasterr_point-1)));
		} else
			saving = 0;

/* Process this iteration */ 

		gwsetnormroutine (gwdata, 0, echk, 0);

		gwstartnextfft (gwdata, postfft && !debug && !stopping && !saving && j != lasterr_point &&  !((interimFiles && (j+1) % interimFiles == 0)) &&
			!(interimResidues && ((j+1) % interimResidues < 2)) && 
			(j >= 30) && (j < last - 31) && !maxerr_recovery_mode[6]); 

		if ((j > 30) && (j < last - 30) && ((j != lasterr_point) || !maxerr_recovery_mode[6])) {
			gwsquare (gwdata, x);
			care = FALSE;
		}
		else {
			gwsquare_carefully (gwdata, x);
			care = TRUE;
		}

		if (debug && (j < 30))
			writeresidue (gwdata, x, N, tmp, buf, str, j, ITER);
		CHECK_IF_ANY_ERROR(x, j, last, 6)
		if (j == lasterr_point)
			saving = 1;					// Be sure to restart after this recovery iteration!
		j++; 
		iters++; 
 
 
/* Print a message every so often */ 
 
		if (j % ITER_OUTPUT == 0) { 
			char	fmt_mask[80]; 
			double	pct; 
			pct = trunc_percent (j * 100.0 / n); 
			if (strlen (str) < 40) {
				sprintf (fmt_mask, "%%.%df%%%% of %%s", PRECISION); 
				sprintf (buf, fmt_mask, pct, str); 
			}
			else {
				sprintf (fmt_mask, "%%.%df%%%% of %%ld", PRECISION); 
				sprintf (buf, fmt_mask, pct, n); 
			}
			title (buf); 
			ReplaceableLine (2);	/* Replace line */ 
			sprintf (fmt_mask, 
				 "%%s, iteration : %%ld / %%ld [%%.%df%%%%]", 
				 PRECISION); 
			sprintf (buf, fmt_mask, str, j, n, pct); 
			OutputStr (buf); 
			if (ERRCHK && j > 30) { 
				OutputStr (".  Round off: "); 
				sprintf (buf, "%10.10f", reallyminerr); 
				OutputStr (buf); 
				sprintf (buf, " to %10.10f", reallymaxerr); 
				OutputStr (buf); 
			} 
			end_timer (0); 
			if (CUMULATIVE_TIMING) { 
				OutputStr (".  Time thusfar : "); 
			} 
			else { 
				OutputStr (".  Time per iteration : "); 
				divide_timer (0, iters); 
				iters = 0; 
			} 
			print_timer (0, TIMER_NL | TIMER_OPT_CLR); 
			start_timer (0); 
		} 
 
/* Print a results file message every so often */ 
 
		if (j % ITER_OUTPUT_RES == 0 || (NO_GUI && stopping)) { 
			sprintf (buf, "Iteration %ld / %ld\n", j, n); 
			writeResults (buf); 
		} 
 
/* Write results to a file every DISK_WRITE_TIME minutes */ 
/* On error, retry in 10 minutes (it could be a temporary */ 
/* disk-full situation) */ 
 
		if (saving || stopping) { 
			write_time = DISK_WRITE_TIME * 60; 
			saving = FALSE;
			if (! writeToFile (gwdata, gdata, filename, j, x, NULL)) { 
				sprintf (buf, WRITEFILEERR, filename); 
				OutputBoth (buf); 
				if (write_time > 600) write_time = 600; 
			} 
			time (&start_time); 

 
/* If an escape key was hit, write out the results and return */ 
 
			if (stopping) {
				pushg (gdata, 1); 
				free(gk);
				free(N);
				gwfree (gwdata, x); 
				gwfree (gwdata, y);
				gwdone(gwdata);
				free (gwdata);
				*res = FALSE;	// JP 09/08/16
				return (FALSE); 
			}
		} 

/* Output the 64-bit residue at specified interims.  Also output the */
/* residues for the next iteration so that we can compare our */
/* residues to programs that start counter at zero or one. */

		if (interimResidues && j % interimResidues < 2)
			writeresidue (gwdata, x, N, tmp, buf, str, j, ITER);

/* Write a save file every "interimFiles" iterations. */

		if (interimFiles && j % interimFiles == 0) {
			char	interimfile[20];
			sprintf (interimfile, "%.8s.%03lu",
				 filename, j / interimFiles);
			if (! writeToFile (gwdata, gdata, interimfile, j, x, NULL)) {
				sprintf (buf, WRITEFILEERR, interimfile);
				OutputBoth (buf);
			}
		}
	} 

	clearline (100);

	gwtogiant (gwdata, x, tmp);

	if (!isZero (tmp)) { 
		*res = FALSE;				/* Not a prime */ 
		if (abs(tmp->sign) < 2)		// make a 32 bit residue correct !!
			sprintf (res64, "%08lX%08lX", (unsigned long)0, (unsigned long)tmp->n[0]); 
		else
			sprintf (res64, "%08lX%08lX", (unsigned long)tmp->n[1], (unsigned long)tmp->n[0]); 
	} 
 
/* Print results and cleanup */ 

	if (*res) 
		sprintf (buf, "%s is prime! (%lu decimal digits)", str, nbdg); 
	else
		sprintf (buf, "%s is not prime.  LLR Res64: %s", str, res64); 

#if defined(WIN32) && !defined(_CONSOLE)

	sprintf (buf+strlen(buf), "  Time : "); 
	ReplaceableLine (2);	/* Replace line */ 

#else

	clearline(100);

	if (*res) {
#if defined (_CONSOLE)
		hConsole = GetStdHandle(STD_OUTPUT_HANDLE);	// Access to Console attributes
		SetConsoleTextAttribute(hConsole, BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
		OutputBoth(buf);
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
#else
		OutputStr("\033[7m");
		OutputBoth(buf);
		OutputStr("\033[0m");
#endif
	}
	else
		OutputBoth(buf);

	sprintf (buf, "  Time : "); 

#endif

	end_timer (1); 
	write_timer (buf+strlen(buf), 1, TIMER_CLR | TIMER_NL); 
	OutputBoth (buf); 

	pushg (gdata, 1); 
	free(gk);
	free(N);
	gwfree (gwdata, x); 
	gwfree (gwdata, y);
	gwdone (gwdata); 
	filename[0] = 'z';
	_unlink (filename); 
	if (IniGetInt(INI_FILE, "PRPdone", 0))
		IniWriteString(INI_FILE, "PRPdone", NULL);
	IniWriteString(INI_FILE, "FFT_Increment", NULL);
	lasterr_point = 0;
	free (gwdata);
	return (TRUE); 
 
/* An error occured, sleep if required, then try restarting at last save point. */ 

error:
	pushg (gdata, 1); 
	gwfree (gwdata, x); 
	gwfree (gwdata, y); 
	*res = FALSE;

	if ((abonillsum && gw_test_illegal_sumout(gwdata)) || 
		(abonmismatch && gw_test_mismatched_sums (gwdata)) || 
		(abonroundoff && gw_get_maxerr(gwdata) > maxroundoff)) {	// Abort...
		aborted = TRUE;
		sprintf (buf, ERRMSG5, checknumber, str);
		OutputBoth (buf);
		free(gk);
		free(N);
		filename[0] = 'u';
		_unlink (filename);
		filename[0] = 'z';
		_unlink (filename); 
		if (IniGetInt(INI_FILE, "PRPdone", 0))
			IniWriteString(INI_FILE, "PRPdone", NULL);
		gwdone (gwdata); 
		free (gwdata);
		if(IniGetInt(INI_FILE, "StopOnAbort", 0)) {
			IniWriteInt (INI_FILE, "PgenLine", IniGetInt(INI_FILE, "PgenLine", 0) + 1);	// Point on the next line
			return (FALSE);
		}
		else
			return (TRUE);
	}

	gwdone (gwdata);

/* Output a message saying we are restarting */ 
 
	if (sleep5) OutputBoth (ERRMSG2); 
	OutputBoth (ERRMSG3); 
 
/* Sleep five minutes before restarting */ 
 
	if (sleep5 && ! SleepFive ()) {
		free (gwdata);
		return (FALSE); 
	}

/* Restart */ 
 
	if (will_try_larger_fft) {
		IniWriteInt(INI_FILE, "FFT_Increment", nbfftinc = (IniGetInt(INI_FILE, "FFT_Increment", 0) + 1));
		if (nbfftinc == maxfftinc)
			abonroundoff = TRUE;	// Don't accept any more Roundoff error.
	}
	goto restart; 
} 


int isLLRW ( 
	unsigned long format, 
	char *sgk,
	unsigned long n,
	unsigned long shift,
	int	*res) 
{ 
	unsigned long bits, gksize; 
	long retval;
	char str[sgkbufsize+256], sgk1[sgkbufsize]; 

	gksize = strlen(sgk);
	gk = allocgiant ((gksize>>1) + 8);	// Allocate one byte per decimal digit + spares
	ctog (sgk, gk);						// Convert k string to giant

	if (shift > 0) {
		gshiftleft (shift, gk);			// Shift k multiplier if requested
		gtoc (gk, sgk1, sgkbufsize);	// Updated k string
	}
	else
		strcpy (sgk1, sgk);

	sprintf (str, "%s*2^%lu%c1", sgk1, n, '-');	// Number N to test, as a string

	bits = n + bitlen(gk);				// Bit length of N
	N =  allocgiant ((bits>>4) + 8);		// Allocate memory for N

//	Compute the number we are testing.

	setone (N);
	gshiftleft (n, N);
	mulg (gk, N); 
	iaddg (-1, N);
	M = allocgiant ((bitlen (N) >> 3) + 8);
	retval = gwslowIsWieferich (str, res, TRUE);
	free (M);
	free (gk);
	free (N);
	return retval;
} 


int isProthW ( 
	unsigned long format, 
	char *sgk,
	unsigned long n,
	unsigned long shift,
	int	*res) 
{ 
	unsigned long bits, gksize;
	long retval;
	char	str[sgkbufsize+256], sgk1[sgkbufsize]; 

	gksize = strlen(sgk);
	gk = allocgiant ((gksize>>1) + 8);	// Allocate one byte per decimal digit + spares
	ctog (sgk, gk);						// Convert k string to giant

	if (shift > 0) {
		gshiftleft (shift, gk);			// Shift k multiplier if requested
		gtoc (gk, sgk1, sgkbufsize);	// Updated k string
	}
	else
		strcpy (sgk1, sgk);

	sprintf (str, "%s*2^%lu%c1", sgk1, n, '+');	// Number N to test, as a string

	bits = n + bitlen(gk);				// Bit length of N
	N =  allocgiant ((bits>>4) + 8);		// Allocate memory for N

//	Compute the number we are testing.

	setone (N);
	gshiftleft (n, N);
	mulg (gk, N); 
	iaddg (1, N);
	M = allocgiant ((bitlen (N) >> 3) + 8);

	retval =  gwslowIsWieferich (str, res, TRUE);

	free (M);
	free (gk);
	free (N);
	return retval;
} 


int isProthP ( 
	unsigned long format, 
	char *sgk,
        unsigned long b_else,	// Lei
	unsigned long n,
	unsigned long binput,		// Lei
	unsigned long ninput,		// Lei
	unsigned long shift,
	int	*res) 
{ 
	unsigned long gksize; 
	unsigned long p; 
	unsigned long bits; 
	long	a, retval;
	gwhandle *gwdata;
	ghandle *gdata;
	giant	gbinput; 
	char	buf[sgkbufsize+256], 
		str[sgkbufsize+256], sgk1[sgkbufsize]; 
	long	write_time = DISK_WRITE_TIME * 60; 
	int	resaprcl, inc = +1; 
	double dk;

// Lei
	double ddk;
	unsigned long idk = 0;
	giant gk1;
// Lei end

// init. gwnum code
	gwdata = (gwhandle*) malloc(sizeof(gwhandle));
	gwinit (gwdata);
	gdata = &gwdata->gdata;

// Lei
	if (b_else != 1) {					// Compute the length of b_else^ninput
		ddk = (double) b_else;
		ddk = ninput * log (ddk)/log (2.0);
		idk = (long) ddk + 1;
	}
	else
		idk = 0;
// Lei end
	if ((format == ABCDN) || (format == ABCDNG)) {	// Compute gk = gb^(n-m)-1
		gksize = ndiff*(unsigned long)ceil(log ((double)binput)/log (2.0))+idk;// initial gksize
		gk = allocgiant ((gksize >> 4) + 8);	// Allocate space for gk
		ultog (binput, gk);
		power (gk, ndiff);
		iaddg (-1, gk);
		sprintf (str, "%lu^%lu-%lu^%lu+1", binput, ninput+ndiff, binput, ninput);
	}
	else {
		gksize = 8*strlen(sgk) + idk;		// J. P. Initial gksize
		gk = allocgiant ((gksize >> 4)  + 8);	// Allocate space for gk
		ctog (sgk, gk);						// Convert k string to giant
	}
	klen = bitlen(gk);					// Length of initial k multiplier
	if (klen > 53 || generic) {			// we must use generic reduction
		dk = 0.0;
	}
	else {								// we can use DWT, compute k as a double
		dk = (double)gk->n[0];
		if (gk->sign > 1)
			dk += 4294967296.0*(double)gk->n[1];
	}

// Lei
	if (b_else != 1) {					// Compute the big multiplier
		gk1 = allocgiant ((gksize>>1) + 8);
		ultog (b_else, gk1);
		power (gk1, ninput);
		mulg (gk1, gk);
		free (gk1);
	}
// Lei end

	if (shift > 0) {
		gshiftleft (shift, gk);			// Shift k multiplier if requested
		dk *= (double) (1<<shift);		// Update dk... J.P. 11/02/11
		if (b_else != 1)
			strcpy (sgk1, sgk);			// Lei, J.P.
		else
			gtoc (gk, sgk1, sgkbufsize);// Updated k string
	}
	else
		strcpy (sgk1, sgk);

	if ((format != ABCDN) && (format != ABCDNG)) {
		if (b_else != 1)	// Lei, J.P.
			sprintf (str, "%s*%lu^%lu%c1", sgk, binput, ninput, '+');// Number N to test, as a string
		else
			sprintf (str, "%s*2^%lu%c1", sgk1, n, '+');	// Number N to test, as a string
	}


	bits = n + bitlen(gk);				// Bit length of N
	N =  allocgiant ((bits>>4) + 8);		// Allocate memory for N

//	Compute the number we are testing.

	setone (N);
	gshiftleft (n, N);
	mulg (gk, N); 
	iaddg (1, N);

//	gk must be odd for the Proth test, so, adjust gk and n if necessary.

	while (bitval(gk, 0) == 0) {
	    gshiftright (1, gk);			// update k as a giant
	    n++;							// update the exponent
	}

	Nlen = bitlen (N); 
	klen = bitlen(gk);

	if (klen > 53 || generic) {			// we must use generic reduction
		dk = 0.0;
	}
	else {								// we can use DWT, compute k as a double
		dk = (double)gk->n[0];
		if (gk->sign > 1)
			dk += 4294967296.0*(double)gk->n[1];
	}

	if ((nbdg = gnbdg(N, 10)) < 200) {			// Attempt an APRCL test for this small number...
		start_timer(1);
			resaprcl = gaprcltest (N, 0, 0);	// Primality test silently done
		end_timer (1);
		if (resaprcl == 10) {
			sprintf (buf,"%s is not prime. (Trial divisions)", str);
		}
		else if (resaprcl == 12)
			sprintf (buf,"%s is prime! (%lu decimal digits, Trial divisions)", str, nbdg);
		else if (resaprcl == 0) {
			goto PRCONTINUE;					// Continue the PROTH test to get the residue...
		}
		else if (resaprcl == 2)
			sprintf (buf,"%s is prime! (%lu decimal digits, APRCL test)", str, nbdg);
		else if (resaprcl == 6)
			sprintf (buf,"Invalid numerical string in %s\n", str);
		else if (resaprcl == 7) {
			sprintf (buf,"APRCL error while testing %s...\n", str);
			if (verbose)
				OutputBoth(buf);
			else
				OutputStr (buf);
			goto PRCONTINUE;					// Continue the PROTH test
		}
		else {
			if (resaprcl == 9)
				sprintf (buf, "APRCL primality test not available for %s\n", str);
			else
				sprintf (buf,"Unexpected return value : %d, while APRCL testing %s...\n", resaprcl, str);
			if (verbose)
				OutputBoth(buf);
			else
				OutputStr (buf);
			goto PRCONTINUE;					// Continue the PROTH test
		}
		*res = ((resaprcl == 2)||(resaprcl == 12));

#if defined(WIN32) && !defined(_CONSOLE)

		sprintf (buf+strlen(buf), "  Time : "); 

#else

		clearline(100);

		if (*res) {
#if defined (_CONSOLE)
		hConsole = GetStdHandle(STD_OUTPUT_HANDLE);	// Access to Console attributes
		SetConsoleTextAttribute(hConsole, BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
		OutputBoth(buf);
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
#else
		OutputStr("\033[7m");
		OutputBoth(buf);
		OutputStr("\033[0m");
#endif
		}
		else
			OutputBoth(buf);

		sprintf (buf, "  Time : "); 

#endif

/* Output the final timings for APRCL test */

		write_timer (buf+strlen(buf), 1, TIMER_CLR | TIMER_NL); 
		OutputBoth (buf);
		free (gk);
		free (N);
		return (TRUE); 
	}

PRCONTINUE:

	globalk = dk;

	if (klen > n) {
		if ((format == ABCDN) || (format == ABCDNG))
		    sprintf(buf, "2^%lu-1 > 2^%lu, so we can only do a PRP test for %s.\n", ndiff, n, str);
		else
		    sprintf(buf, "%s > 2^%lu, so we can only do a PRP test for %s.\n", sgk, n, str);
	    OutputBoth(buf);

// Lei
		if ((format == ABCDN) || (format == ABCDNG))
			sprintf (str, "%lu^%lu-%lu^%lu+1", binput, ninput+ndiff, binput, ninput);
		else
			sprintf (str, "%s*%lu^%lu%c1", sgk, binput, ninput, '+');     // Number N to test, as a string
		gbinput = allocgiant (2);
		gbinput->sign = 1;
		gbinput->n[0] = 2;
		fpart = (1.0-(double)klen/Nlen)*100.0;
		retval = isPRPinternal (str, dk, gbinput, n, 1, res);
// Lei end
		if (res) {
			sprintf (buf, "(Factorized part = %4.2f%%)\n", fpart);
			OutputBoth (buf);
			if (nbdg < primolimit)
				MakePrimoInput (N, str);
		}

		free(gbinput);
		free(gk);
		free(N);
		return (retval);
	}

/* Init the title */ 
 
	title ("Proth prime test in progress...");
 
/* Compute the base for the Proth algorithm. */
 
if ((a = genProthBase(gk, (uint32_t)n)) < 0) {
	if (a == -1)
		sprintf (buf, "Cannot compute a to test %s...\nThis is surprising, please, let me know that!!\nMy E-mail is jpenne@free.fr\n", str);
	else
		sprintf (buf, "%s has a small factor : %d !!\n", str, abs(a));
	OutputBoth (buf); 
	*res = FALSE;
	free(gk);
	free(N);
	return(TRUE);
}



	nbdg = gnbdg (N, 10);	// Compute the number of decimal digits of the tested number.
	gwinit (gwdata);
	gdata = &gwdata->gdata;
 	gwset_num_threads (gwdata, IniGetInt(INI_FILE, "ThreadsPerTest", 1));

	gwsetmaxmulbyconst (gwdata, a);

	p = Nlen; 

//	*res = TRUE;						/* Assume it is a prime */ 
	w->k = dk;		// init work_unit for the Gerbicz test.
	w->n = n;
	w->b = 2;
	w->c = 1;
	w->prp_base = a;
	w->prp_residue_type = PROTH_TYPE;

	do {
restart:
		if (dk == 0.0) {			// generic modular reduction needed...
			gwset_larger_fftlen_count(gwdata, (char)IniGetInt(INI_FILE, "FFT_Increment", 0));
			if (!setupok (gwdata, gwsetup_general_mod_giant (gwdata, N))) {
				free(gk);
				free(N);
				free(gwdata);
				*res = FALSE;
				return FALSE;
			}

//	Restart with next FFT length if we are too near the limit...

			if (nextifnear && gwnear_fft_limit (gwdata, pcfftlim)) {
				OutputBoth ("Current FFT beeing too near the limit, next FFT length is forced...\n");
				IniWriteInt(INI_FILE, "FFT_Increment", IniGetInt(INI_FILE, "FFT_Increment", 0) + 1);
				gwdone (gwdata);
				goto restart;
			}
		}			// end dk == 0.0
		else {
restart2:
			gwset_larger_fftlen_count(gwdata, (char)IniGetInt(INI_FILE, "FFT_Increment", 0));
			if (!setupok (gwdata, gwsetup (gwdata, dk, binput, ninput, +1))) {
				free(gk);
				free(N);
				free(gwdata);
				*res = FALSE;
				return FALSE;
			}

//		Restart with next FFT length if we are too near the limit...

			if (nextifnear && gwnear_fft_limit (gwdata, pcfftlim)) {
				OutputBoth ("Current FFT beeing too near the limit, next FFT length is forced...\n");
				IniWriteInt(INI_FILE, "FFT_Increment", IniGetInt(INI_FILE, "FFT_Increment", 0) + 1);
				gwdone (gwdata);
				goto restart2;
			}
		}	// end dk != 0.0 (not generic reduction mode)

/* Do the Proth test */

			retval = GerbiczTest (gwdata, gdata, a, res, str, w);
			gwdone (gwdata);
	} while (retval == -1);

/* Clean up and return */

		free (gk);
		free (N);
		free (gwdata);
		return (retval);
} 

#define LOWFACTORLIMIT 10000	// To factor lower exponent candidates is not useful...
int res1, res2;
long	a;

/********************************* GAUSSIAN-MERSENNE PRIME SEARCH CODE *****************************************

The main purpose of this code is the seach for non-real Gaussian-Mersenne primes.

A Gaussian-Mersenne number can be written as : GM(p) = (1+/-i)^p - 1
It is easy to prove that such a number may be prime in the Gauss ring, only if p is prime,
so we are only considering odd prime values for the exponent p.

More generally, a non-real Gaussian integer x+i*y is prime iff its norm x^2+y^2 is a rational prime.

The norm of GM(p) is N(p) = 2^p + sign*2^((p+1)/2) + 1
where sign = +1 if p = 3 or 5 modulo 8, -1 if p = 1 or 7 modulo 8.

This norm can be rewritten as N(p) = [2^((p-1)/2) + sign]*2^((p+1)/2) + 1, which shows that
N(p) is a Proth number, so the Proth algorithm may be used to prove its primality.
A drawback is that the multiplier in the [] increases so rapidly that it should require using generic mode...

The algorithm implemented below avoids this drawback, and has been suggested to me by Harsh Aggarwal.

The starting point is the Aurifeuillian factorization of M(p) = 4^p+1 :

M(p) = 4^p+1 = (2^p + 2^((p+1)/2) + 1)(2^p - 2^((p+1)/2) + 1)

One of these two factors is the norm N(p) of GM(p) while the other, N'(p) is the norm of another
Gaussian integer : GF(p) = (1+/-i)^p + 1
p beeing odd, such a number is always divisible by 2+/-i, so, N'(p) (like M(p)) has always the trivial factor 5.
But, p beeing prime, GQ(p) = GF(p)/(2+/-i) may be a Gaussian prime, iff N'/5 is a rational prime...

Now, the idea is to run the Proth algorithm, but doing the squarings modulo M(p), and then doing the modulo N
reduction only on the final result. Then, the performances for a given p may be approximatively the same as
for a Lucas-Lehmer test with exponent 1.4*p.

Moreover, using an interim result of the main loop, we have all the info to get the PRP test result of N'/5!

Here is the algorithm in pseudo - C :

GMtest() {

Compute M(p), N(p), N'(p), N'/5;
Compute a such Jacobi(a, N) = -1; // (the Proth base)

x = a;

for (i=1; i<=p-1; i++) {		// This main loop implies only squarings modulo 2^(2*p) + 1, which is optimal!
	x = x*x modulo M;			// One iteration of the Proth algorithm
	if (i == (p-1)/2) y = x;	// Keep this interim value of x
}

// We have now x = a^(2^(p-1)) modulo M and y = a^(2^((p-1)/2)) modulo M.
// To do the Proth test, we need now to compute R = a^(N-1)/2 nodulo N;
// But, (N-1)/2 = 2^(p-1) + (sign)*2^((p-1)/2), which shows us how to complete :

if (sign == 1)
	R = x*y modulo N;
else if (sign == -1)
	R = x*(y^-1 modulo N) modulo N;

	if (R == -1 modulo N)
		printf("N is prime!");
	else
		printf("N is not prime.");

// The PRP test on N'/5 is slightly more complicated :
// We need to test if a^(N'/5) == a modulo N'/5, which implies a^N' == a^5 modulo N'/5 and finally :
// R' = a^(N'-1] = a^4 modulo N'/5 (a and N'/5 beeing co-prime).
// But, N'-1 = 2*[2^(p-1) - (sign)*2^((p-1)/2)], so :

if (sign == 1)
	R' = x*x*y^-1*y^-1; // all computed modulo N'/5
else if (sign == -1)
	R' = x*x*y*y;		// all computed modulo N'/5
		
	if (R' == a^4 modulo N'/5)
		printf("N'/5 is a^4-PRP!");
	else
		printf("N'/5 is not prime.");

} // End

Finally, to be efficient, this prime search needs eliminating as many candidates as possible by prefactoring.
To do that I adapted the George Woltman's "factor32.asm" code to find the factors of 4^p+1, and then, of
N and/or N'/5.
This feature is included here, and allows to factor up to 2^86, if really needed.
Also, there is an option to do factoring only jobs.

Jean Penn�, March 30 2006

New :

Now, both Gerbicz error checking and random initial shift on the Proth base
	are used for these tests!

Jean Penn�, June 24 2020

****************************************************************************************************************/

int isGMNP ( 
	char *sgk,
	unsigned long n,
	int	*res) 
{ 
	unsigned long nbdg1, nbdg2; 
	unsigned long abits = 0; 
	unsigned long bits, howfar = 0; 
	gwhandle *gwdata;
	ghandle *gdata;
	giant	tmp, tmp2, apow4; 
	char	filename[20], buf[sgkbufsize+256], str[sgkbufsize+256], strp[sgkbufsize+256]; 
	long	write_time = DISK_WRITE_TIME * 60; 
	int		sign, fisok = 0, fres = 0, fhandle = 0, inc = +1, resaprcl1 = 0, resaprcl2 = 0; 
	int		retval;
	double	reallyminerr = 1.0; 
	double	reallymaxerr = 0.0; 
	double dk;

//	init. gwnum code

	gwdata = (gwhandle*) malloc(sizeof(gwhandle));
	gwinit (gwdata);
	gdata = &gwdata->gdata;

	if ((!facto && !isPrime (n)) || n == 2) {
		sprintf (buf, "Gaussian-Mersenne prime test not done because %lu is not an odd prime.\n", n); 
		OutputBoth (buf); 
		*res = FALSE;
		return(TRUE);
	}

	sign = (((n&7) == 3) || ((n&7) == 5))? 1 : 0;	// 1 if positive, 0 if negative
	sprintf (str, "2^%lu%c%s+1",  n, (sign) ? '+' : '-', sgk);	// Number N to test, as a string
	sprintf (strp, "(2^%lu%c%s+1)/5",  n, (sign) ? '-' : '+', sgk);	// Number N' to test, as a string

	bits = 2*n;							// Bit length of M = N*N'
	M = allocgiant ((bits>>4) + 8);		// Allocate memory for M = N*N'
	N = allocgiant ((bits>>5) + 8);		// Allocate memory for N
	NP = allocgiant ((bits>>5) + 8);		// Allocate memory for N'
	gk = allocgiant ((bits>>5) + 8);		// Allocate memory for gk
	testn =  allocgiant ((bits>>3) + 16);	// For factoring
	testnp = allocgiant ((bits>>3) + 16);
	testf  = allocgiant (100);
	testx  = allocgiant (100);

//	gk is the multiplier when N is written as gk*2^exponent + 1
//	N = 2^n + s*2^((n+1)/2) + 1 = (2^((n-1)/2) + s)*2^((n+1)/2) + 1
//	So, gk = 2^((n-1)/2) + s and exponent = (n+1)/2, where s is +1 or -1
//	It is only used to compute the Proth base.

//	Compute the numbers we are testing or using.

	setone (N);
	gshiftleft (n, N);				// N  = 2^n
	iaddg (1, N);					// N  = 2^n + 1
	gtog (N, NP);					// N' = 2^n + 1
	setone (M);
	gshiftleft ((n+1)/2, M);		// M  = 2^((n+1)/2)
	gtog (M, gk);
	gshiftright (1, gk);			// gk  = 2^((n-1)/2)

	if (sign) {						// n = 3 or 5 modulo 8
		addg (M, N);				// N  = 2^n + 2^((n+1)/2) + 1
		iaddg (1, gk);				// gk  = 2^((n-1)/2) + 1
		subg (M, NP);				// N' = 2^n - 2^((n+1)/2) + 1
	}
	else {							// n = 1 or 7 modulo 8
		subg (M, N);				// N  = 2^n - 2^((n+1)/2) + 1
		iaddg (-1, gk);				// gk  = 2^((n-1)/2) - 1
		addg (M, NP);				// N' = 2^n + 2^((n+1)/2) + 1
	}

	uldivg ((uint32_t)5, NP);		// NP = N'/5
	setone (M);
	gshiftleft (2*n, M);			// M  = 2^(2*n)
	iaddg (1, M);					// M  = N*N' = 2^(2*n) + 1

	Nlen = 2*n+1; 
	nbdg1 = gnbdg (N, 10);			// Size of the two candidates
	nbdg2 = gnbdg (NP, 10);

	res1 = res2 = 0;			// Clear the results...		

	if (!facto && (nbdg1 < 2000) && (nbdg2 < 2000)) {
		a = 0;						// N and NP are small numbers, so we may make APRCL tests...
		res1 = res2 = 0;			// Clear the results...		
		start_timer(1);				// Beginning the test of the GMN...
		if (nbdg1 < 100) {			// Attempt an APRCL test only on M.N. less than 100 digits large...
			resaprcl1 = gaprcltest (N, 0, 0);// Primality test silently done
			res1 = ((resaprcl1 == 2)||(resaprcl1 == 12));
			if (resaprcl1 == 10)
				sprintf (buf,"%s is not prime. (Trial divisions)\n", str);
			else if (resaprcl1 == 12)
				sprintf (buf,"%s is prime! (%lu decimal digits, Trial divisions)", str, nbdg1);
			else if (resaprcl1 == 0)
				sprintf (buf,"%s is not prime. (APRCL test)\n", str);
			else if (resaprcl1 == 2)
				sprintf (buf,"%s is prime! (%lu decimal digits, APRCL test)", str, nbdg1);
			else if (resaprcl1 == 6)
				sprintf (buf,"Invalid numerical string in %s\n", str);
			else if (resaprcl1 == 7)
				sprintf (buf,"APRCL error while testing %s...\n", str);
			else {
				if (resaprcl1 == 9)
				sprintf (buf, "APRCL primality test not available for %s\n", str);
				else
					sprintf (buf,"Unexpected return value : %d, while APRCL testing %s...\n", resaprcl1, str);
			}
			if (res1) {
#if !defined(WIN32) || defined(_CONSOLE)
#if defined (_CONSOLE)
				hConsole = GetStdHandle(STD_OUTPUT_HANDLE);	// Access to Console attributes
				SetConsoleTextAttribute(hConsole, BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
				OutputBoth(buf);
				SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
				OutputBoth("\n");
#else
				OutputStr("\033[7m");
				OutputBoth(buf);
				OutputStr("\033[0m");
				OutputBoth("\n");
#endif
#endif
			}
			else {
				if (verbose)
					OutputBoth(buf);
				else
					OutputStr (buf);
			}

		}
		if (nbdg2 > maxaprcl)					// Now testing the cofactor, if not too large...
			resaprcl2 = gaprcltest (NP, 1, 0);	// Make only a Strong BPSW PRP test
		else if (debug)
			resaprcl2 = gaprcltest (NP, 0, 2);	// Primality test while showing progress 
		else
			resaprcl2 = gaprcltest (NP, 0, 0);	// Primality test silently done
		end_timer (1);
		res2 = ((resaprcl2 == 1)||(resaprcl2 == 2)||(resaprcl2 == 12));
		if (resaprcl2 == 10)
			sprintf (buf,"%s is not prime. (Trial divisions)", strp);
		else if (resaprcl2 == 12)
			sprintf (buf,"%s is prime! (%lu decimal digits, Trial divisions)", strp, nbdg2);
		else if (resaprcl2 == 0) {				// Not prime message may be useless here...
			if (nbdg1 < 100)
				sprintf (buf,"%s is not prime. (APRCL test)", strp);
			else if ((N->sign > 1)||(NP->sign > 1))
				goto COFCONTINUE;				// Continue the PRP test to get the residue...
		}
		else if (resaprcl2 == 1)				// BPSW PRP candidate
			sprintf (buf,"%s is a probable BPSW prime! (%lu decimal digits, APRCL test) ", strp, nbdg2);
		else if (resaprcl2 == 2)
			sprintf (buf,"%s is prime! (%lu decimal digits, APRCL test)", strp, nbdg2);
		else if (resaprcl2 == 6)
			sprintf (buf,"Invalid numerical string in %s\n", strp);
		else if (resaprcl2 == 7) {
			sprintf (buf,"APRCL error while testing %s...\n", strp);
		}
		else {
			if (resaprcl2 == 9)
				sprintf (buf, "APRCL primality test not available for %s\n", strp);
			else
				sprintf (buf,"Unexpected return value : %d, while APRCL testing %s...\n", resaprcl2, strp);
		}
#if defined(WIN32) && !defined(_CONSOLE)

		sprintf (buf+strlen(buf), "  Time : "); 

#else

		clearline(100);

		if (res2) {
#if defined (_CONSOLE)
		hConsole = GetStdHandle(STD_OUTPUT_HANDLE);	// Access to Console attributes
		SetConsoleTextAttribute(hConsole, BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
		OutputBoth(buf);
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
#else
		OutputStr("\033[7m");
		OutputBoth(buf);
		OutputStr("\033[0m");
#endif
		}
		else
			OutputBoth(buf);

		sprintf (buf, "  Time : "); 
#endif


/* Output the final timings for APRCL test */

		write_timer (buf+strlen(buf), 1, TIMER_CLR | TIMER_NL); 
		OutputBoth (buf);
		if (resaprcl2 == 1)										// BPSW PRP candidate
			MakePrimoInput (NP, strp);
		*res = (res1 || res2);
		if ((nbdg1 >= 100)&&((N->sign > 1)||(NP->sign > 1)))	// APRCL test not done on N
			goto COFCONTINUE;									// Continue the test to get the residue(s)...
		free(N);
		free(NP);
		free(M);
		free(gk);
		free(testn);
		free(testnp);
		free(testf);
		free(testx);

		return (TRUE); 
	}
	else {
		resaprcl1 = resaprcl2 = 0;
	}

COFCONTINUE:

//	res1 = res2 = 1;				// Assume N and NP are prime...

#ifndef X86_64

	strcpy (facnstr, "1");
	strcpy (facnpstr, "1");

	FACTEST = testfac;


	if (facto) {				// There is a request for a prefactoring only job...

		resn = !testgm;
		resnp = !testgq;
		factored++;

		if (!facfrom && facn != 0 && facnp != 0)
			facfrom = 32;

		if (!testgq && facn > 1) {
			eliminated++;
			sprintf (buf, "%s has a factor : %lu\n", str, facn);
			OutputBoth (buf);
			*res = res1 = res2 = FALSE;
			free(N);
			free(NP);
			free(M);
			free(gk);
			free(testn);
			free(testnp);
			free(testf);
			free(testx);
			return (TRUE); 
		}
		else if (!testgm && facnp > 1) {
			eliminated++;
			sprintf (buf, "%s has a factor : %lu\n", strp, facnp);
			OutputBoth (buf);
			*res = res1 = res2 = FALSE;
			free(N);
			free(NP);
			free(M);
			free(gk);
			free(testn);
			free(testnp);
			free(testf);
			free(testx);
			return (TRUE); 
		}

		if (facnp > 0 && facn > 1) {
			resn = TRUE;
			sprintf (facnstr, "%lu", facn);
		}

		if (facnp > 1) {
			resnp = TRUE;
			sprintf (facnpstr, "%lu", facnp);
		}

		/* Init filename */

		tempFileName (filename, 'g', N);

		if (fileExists (filename)) {
			/* Open the intermediate file */
			fhandle = _open (filename, _O_BINARY | _O_RDONLY);
		}

		fres = resn && resnp;

		if (fres || n <  LOWFACTORLIMIT || primeSieve (n, (unsigned short)facfrom, (unsigned short)facto, str, strp, &fres, fhandle)) {
			*res = !fres;
			res1 = res2 = FALSE;
			free(N);
			free(NP);
			free(M);
			free(gk);
			free(testn);
			free(testnp);
			free(testf);
			free(testx);
			return (TRUE); 
		}
		else {
			*res = res1 = res2 = FALSE;
			free(N);
			free(NP);
			free(M);
			free(gk);
			free(testn);
			free(testnp);
			free(testf);
			free(testx);
			return (FALSE); 
		}
	}

	if (testfac > 70) {

		FACMSW = FACHSW = 0;
		if (N->sign > 1)
			FACMSW = N->n[1];
		if (N->sign > 2)
			FACHSW = N->n[2];

		factorSetup(n);


		FACLSW = N->n[0];
		resn = resnp = 0;

		if (factorAndVerify(n) == 1) {
			sprintf (buf, "%s has a factor : %s\n", str, facnstr);
			OutputBoth (buf);
		}
		else {
			sprintf (buf, "%s : no factor found.\n", str);
			OutputBoth (buf);
		}


		FACMSW = FACHSW = 0;
		if (NP->sign > 1)
			FACMSW = NP->n[1];
		if (NP->sign > 2)
			FACHSW = NP->n[2];

		factorSetup(n);

		FACLSW = NP->n[0];
		resn = resnp = 0;

		if (factorAndVerify(n) == 1) {
			sprintf (buf, "%s has a factor : %s\n", strp, facnpstr);
			OutputBoth (buf);
		}
		else {
			sprintf (buf, "%s : no factor found.\n", strp);
			OutputBoth (buf);
		}

		*res = res1 = res2 = FALSE;
		free(N);
		free(NP);
		free(M);
		free(gk);
		free(testn);
		free(testnp);
		free(testf);
		free(testx);
		return (TRUE);
	}

#else

	if (facto) {				// There is a request for a prefactoring only job...
		sprintf (buf, "Prefactoring 64bit code is not yet available ; please use 32bit version.\n");
		OutputBoth (buf);
		*res = res1 = res2 = FALSE;
		free(N);
		free(NP);
		free(M);
		free(gk);
		free(testn);
		free(testnp);
		free(testf);
		free(testx);
		return (TRUE);

	}

#endif

// Test if we are resuming a prime test or doing setup only.

	if (setuponly||IniGetInt(INI_FILE, "Verify", 0)||IniGetInt(INI_FILE, "Noprefactoring", 0))
		goto resume;

	tempFileName (filename, 'z', N);
	if (fileExists (filename))
		goto resume;

#ifndef X86_64

// If not resuming, prefactor if necessary.

	if (facn <= 1 || facnp <= 1) {
		if (!testgq && facn > 1) {
			sprintf (buf, "%s has a factor : %lu\n", str, facn);
			OutputBoth (buf);
			*res = res1 = res2 = FALSE;
			free(N);
			free(NP);
			free(M);
			free(gk);
			free(testn);
			free(testnp);
			free(testf);
			free(testx);
			return (TRUE); 
		}
		else if (!testgm && facnp > 1) {
			sprintf (buf, "%s has a factor : %lu\n", strp, facnp);
			OutputBoth (buf);
			*res = res1 = res2 = FALSE;
			free(N);
			free(NP);
			free(M);
			free(gk);
			free(testn);
			free(testnp);
			free(testf);
			free(testx);
			return (TRUE); 
		}


		if (facn == 0 || facnp == 0)	// Prepare to trial factor the number
			howfar = 0;
		else
			howfar = 32;

		resn = !testgm;
		resnp = !testgq;

		if (facnp > 0 && facn > 1) {
			resn = TRUE;
			sprintf (facnstr, "%lu", facn);
		}

		if (facnp > 1) {
			resnp = TRUE;
			sprintf (facnpstr, "%lu", facnp);
		}

		/* Init filename */

		tempFileName (filename, 'f', N);

		if (fileExists (filename)) {
			/* Open the intermediate file */
			fhandle = _open (filename, _O_BINARY | _O_RDONLY);
		}

		fres = FALSE;

		if(n < LOWFACTORLIMIT || (fisok = primeFactor (n, howfar, &fres, WORK_FACTOR, fhandle))) {
			if (fres) {
				if (!testgq)
					sprintf (buf, "%s has a factor : %s\n", str, facnstr);
				else if (!testgm)
					sprintf (buf, "%s has a factor : %s\n", strp, facnpstr);
				else
					sprintf (buf, "%s has a factor : %s and %s has a factor : %s\n", str, facnstr, strp, facnpstr);
				*res = FALSE;
				OutputBoth (buf);
				*res = res1 = res2 = FALSE;
				free(N);
				free(NP);
				free(M);
				free(gk);
				free(testn);
				free(testnp);
				free(testf);
				free(testx);
				return (TRUE); 
			}
		}
		else {
			*res = res1 = res2 = FALSE;
			free(N);
			free(NP);
			free(M);
			free(gk);
			free(testn);
			free(testnp);
			free(testf);
			free(testx);
			return (FALSE); 
		}
		if (testfac >= 16) {
			*res = res1 = res2 = FALSE;
			free(N);
			free(NP);
			free(M);
			free(gk);
			free(testn);
			free(testnp);
			free(testf);
			free(testx);
			return (TRUE); 
		}

	}
	else {
		sprintf (buf, "%s has a factor : %lu and %s has a factor : %lu\n", str, facn, strp, facnp);
		*res = res1 = res2 = FALSE;
		OutputBoth (buf);
		free(N);
		free(NP);
		free(M);
		free(gk);
		free(testn);
		free(testnp);
		free(testf);
		free(testx);
		return (TRUE); 
	}

#else
	else {
		sprintf (buf, "Prefactoring not done, due to 64bit code not yet available.\n");
		OutputBoth (buf);
	}

#endif

goto primetest;

resume:

resaprcl1 = resaprcl2 = 0;

primetest:

 	dk = 1.0;						// k == 1 for the modulo N*N'

	globalk = dk;

/* Compute the base for the Proth algorithm. */
 
	if ((a = genProthBase(gk, ((uint32_t)n+1)/2)) < 0) {
		if (a == -1)
			sprintf (buf, "Cannot compute a to test %s...\nThis is surprising, please, let me know that!!\nMy E-mail is jpenne@free.fr\n", str);
		else
			sprintf (buf, "%s has a small factor : %d !!\n", str, abs(a));
		OutputBoth (buf); 
		*res = res1 = res2 = FALSE;
		free(gk);
		free(N);
		free(NP);
		free(M);
		free(testn);
		free(testnp);
		free(testf);
		free(testx);
		return(TRUE);
	}

/* More initializations... */

	tmp = allocgiant((Nlen >> 4) + 8);
	tmp2 = allocgiant((Nlen >> 4) + 8);
	apow4 = allocgiant(16);
	itog (a, apow4);
	imulg (a, apow4);
	imulg (a, apow4);
	imulg (a, apow4);

	*res = TRUE;						/* Assume it is a prime */ 

	w->prp_residue_type = GMN_TYPE;
	w->known_factors = NULL;
	w->k = dk;
	w->n = n;
	w->b = 2;
	w->c = 1;

	do {

restart:

		gwinit (gwdata);
		gdata = &gwdata->gdata;
 		gwset_num_threads (gwdata, IniGetInt(INI_FILE, "ThreadsPerTest", 1));

		gwsetmaxmulbyconst (gwdata, a);


/* Assume intermediate results of the length of N*N'. */ 

		gwset_larger_fftlen_count(gwdata, (char)IniGetInt(INI_FILE, "FFT_Increment", 0));
		if (!setupok (gwdata, gwsetup (gwdata, dk, 2, 2*n, +1))) { 	// Setup the DWT mode
			*res = res1 = res2 = FALSE;
			free(gk);
			free(N);
			free(NP);
			free(M);
			free(testn);
			free(testnp);
			free(testf);
			free(testx);
			free(gwdata);
			return FALSE;
		}


//	Restart with next FFT length if we are too near the limit...

		if (nextifnear && gwnear_fft_limit (gwdata, pcfftlim)) {
			OutputBoth ("Current FFT beeing too near the limit, next FFT length is forced...\n");
			IniWriteInt(INI_FILE, "FFT_Increment", IniGetInt(INI_FILE, "FFT_Increment", 0) + 1);
			gwdone (gwdata);
			goto restart;
		}

		retval = GerbiczTest (gwdata, gdata, a, res, str, w);

	}	while (retval == -1);

	if (retval == FALSE)
		goto EXIT;

	clearline (100);

	gtog (ps.gx, tmp);
	specialmodg (gwdata, tmp);
	gtog (ps.gy, tmp2);
	specialmodg (gwdata, tmp2);
	if (sign) {
		mulg (tmp2, tmp);
		modg (N, tmp);
		iaddg (1, tmp);				// Compute the (unnormalized) residue
	}
	else {
		invg (N, tmp2);
		mulg (tmp2, tmp);
		modg (N, tmp);
		iaddg (1, tmp);
	}

/* See if we've found a Proth prime.  If not, format a 64-bit residue. */

	if (resaprcl1 != 2)	{			// Exclude this output if previous APRCL positive result
		if (gcompg (N, tmp) != 0) {
			res1 = FALSE;				/* Not a prime */
			if (abs(tmp->sign) < 2)		// make a 32 bit residue correct !!
				sprintf (res64, "%08lX%08lX", (unsigned long)0, (unsigned long)tmp->n[0]);
			else
				sprintf (res64, "%08lX%08lX", (unsigned long)tmp->n[1], (unsigned long)tmp->n[0]);
		}
		else {
			res1 = TRUE;
		}


/* Print results.  Do not change the format of this line as Jim Fougeron of */
/* PFGW fame automates his QA scripts by parsing this line. */

		if (res1) {
#if defined (_CONSOLE)
			sprintf (buf, "%s is prime! (%lu decimal digits)", str, nbdg1);
#else
			if ((resaprcl2 != 1) && (resaprcl2 != 2))
				sprintf (buf, "%s is prime! (%lu decimal digits)\n", str, nbdg1);
			else
				sprintf (buf, "%s is prime! (%lu decimal digits)", str, nbdg1);
#endif
		}
		else {
			if ((resaprcl2 != 1) && (resaprcl2 != 2))
				sprintf (buf, "%s is not prime.  Proth RES64: %s\n", str, res64);
			else
				sprintf (buf, "%s is not prime.  Proth RES64: %s", str, res64);
		}

#if defined(WIN32) && !defined(_CONSOLE)

		if ((resaprcl2 != 1) && (resaprcl2 != 2))
			OutputBoth (buf);				// Avoid a double display...

#else

		clearline(100);

		if (res1) {
			if ((resaprcl2 != 1) && (resaprcl2 != 2)) {
#if defined (_CONSOLE)
				hConsole = GetStdHandle(STD_OUTPUT_HANDLE);	// Access to Console attributes
				SetConsoleTextAttribute(hConsole, BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
				OutputBoth(buf);			// Avoid a double display...
				SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
				OutputBoth("\n");
#else
				OutputStr("\033[7m");
				OutputBoth(buf);			// Avoid a double display...
				OutputStr("\033[0m");
#endif
			}
		}
		else {
			if ((resaprcl2 != 1) && (resaprcl2 != 2))
				OutputBoth(buf);				// Avoid a double display...
		}

		if ((resaprcl2 != 1) && (resaprcl2 != 2))
			sprintf (buf, "  Time : "); 	// Avoid a double display...

#endif
	}									// End Exclude...
	gtog (ps.gx, tmp);
	specialmodg (gwdata, tmp);
	gtog (ps.gy, tmp2);
	specialmodg (gwdata, tmp2);

	if (sign) {
		mulg (tmp, tmp);
		modg (NP, tmp);
		mulg (tmp2, tmp2);
		mulg (apow4, tmp2);
		modg (NP, tmp2);
	}
	else {
		mulg (tmp, tmp);
		modg (NP, tmp);
		mulg (tmp2, tmp);
		modg (NP, tmp);
		mulg (tmp2, tmp);
		modg (NP, tmp);
		gtog (apow4, tmp2);
		modg (NP, tmp2);
	}

	if ((resaprcl2 != 1) && (resaprcl2 != 2)) {	// Exclude this output if previous APRCL positive result

		if (gcompg (tmp2, tmp) != 0) {
			subg (tmp2, tmp);
			res2 = FALSE;				/* Not a prime */
			if (abs(tmp->sign) < 2)		// make a 32 bit residue correct !!
				sprintf (res64, "%08lX%08lX", (unsigned long)0, (unsigned long)tmp->n[0]);
			else
				sprintf (res64, "%08lX%08lX", (unsigned long)tmp->n[1], (unsigned long)tmp->n[0]);
		}
		else
			res2 = TRUE;


/* Print results.  Do not change the format of this line as Jim Fougeron of */
/* PFGW fame automates his QA scripts by parsing this line. */


		if (res2)
			sprintf (buf, "%s is %lu-PRP! (%lu decimal digits)", strp, a, nbdg2);
		else
			sprintf (buf, "%s is not prime.  RES64: %s", strp, res64);

#ifdef WIN32

		sprintf (buf+strlen(buf), "  Time: ");

#else

		if (res2) {
#if defined (_CONSOLE)
		hConsole = GetStdHandle(STD_OUTPUT_HANDLE);	// Access to Console attributes
		SetConsoleTextAttribute(hConsole, BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
		OutputBoth(buf);
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
#else
		OutputStr("\033[7m");
		OutputBoth(buf);
		OutputStr("\033[0m");
#endif
		}
		else
			OutputBoth (buf);
		sprintf (buf, "  Time: ");

#endif

	}								// End Exclude...
	else
		sprintf (buf+strlen(buf), "  Time: ");

/* Output the final timings */

	if (!(resaprcl1 == 2) || !((resaprcl2 == 2) || (resaprcl2 == 1))) {
		end_timer (2);
		write_timer (buf+strlen(buf), 2, TIMER_CLR | TIMER_NL); 
		OutputBoth (buf);
		if (res2 && (resaprcl2 != 1) && (resaprcl2 != 2) && (nbdg2 < primolimit))
			MakePrimoInput (NP, strp);
	}

	*res = (res1 || res2);

/* Cleanup and return */

	free(ps.gx);
	free(ps.gy);

EXIT:
	free(tmp);
	free(tmp2);
	free(apow4);
	free(gk);
	free(N);
	free(NP);
	free(M);
	free(testn);
	free(testnp);
	free(testf);
	free(testx);
	gwdone (gwdata);
	IniWriteString(INI_FILE, "FFT_Increment", NULL);
	lasterr_point = 0;
	free (gwdata);
	return (retval);
} 


/************************************** Strong Fermat PRP test code for Wagstaff numbers ************************************

Wagstaff numbers are numbers of the form W(n) = (2^n+1)/3 where n is an odd integer.

****************************************************************************************************************************/


int isWSPRP ( 
	char *sgk,
	unsigned long n,
	int	*res) 
{ 
	unsigned long iters; 
	unsigned long ubx, uby, a, atemp, abits = 0;
	uint32_t hi, lo;
	unsigned long bit, bits, expx, howfar = 0, dovrbareix = vrbareix; 
	gwhandle *gwdata;
	ghandle *gdata;
	gwnum	x, y; 
	giant	tmp, tmp2, gx0; 
	char	filename[20], buf[sgkbufsize+256], fft_desc[256], oldres64[17]; 
	long	write_time = DISK_WRITE_TIME * 60; 
	int	resaprcl, echk, saving, stopping, fisok = 0, fres = 0, fhandle = 0, inc = +1; 
	time_t	start_time, current_time; 
	double	reallyminerr = 1.0; 
	double	reallymaxerr = 0.0; 

	gwdata = (gwhandle*) malloc(sizeof(gwhandle));
	gwinit (gwdata);
	gdata = &gwdata->gdata;

	if ((!facto && !isPrime (n)) || n == 2) {
		sprintf (buf, "(2^%lu+1)/3 SPRP test not done because %lu is not an odd prime.\n", n, n); 
		OutputBoth (buf); 
		*res = FALSE;
		return(TRUE);
	}

	bits = n;							// Bit length of NP
	Nlen = bits + 1;					// for read/write intermediate files
	M = allocgiant ((bits>>4) + 8);		// Allocate memory for M
	NP = allocgiant ((bits>>4) + 8);		// Allocate memory for NP
	testn =  allocgiant ((bits>>3) + 16);	// For factoring
	testf  = allocgiant (100);
	testx  = allocgiant (100);

//	Compute the numbers we are testing or using.

	setone (M);
	gshiftleft (n, M);				// M  = 2^n
	iaddg (1, M);					// M  = 2^n + 1
	gtog (M, NP);					// NP  = 2^n + 1
	uldivg (3, NP);					// NP  = (2^n + 1)/3

	if (!facto && ((nbdg = gnbdg(NP, 10)) < 200)) {	// Attempt an APRCL test...
		start_timer(1);
		if (nbdg > maxaprcl)
			resaprcl = gaprcltest (NP, 1, 0);			// Make only a Strong BPSW PRP test
		else if (debug)
			resaprcl = gaprcltest (NP, 0, 2);			// Primality test while showing progress 
		else
			resaprcl = gaprcltest (NP, 0, 0);			// Primality test silently done
		end_timer (1);
		if (resaprcl == 10)
			sprintf (buf,"%s is not prime. (Trial divisions)", sgk);
		else if (resaprcl == 12)
			sprintf (buf,"%s is prime! (%lu decimal digits, Trial divisions)", sgk, nbdg);
		else if (resaprcl == 0)
			goto WSTFCONTINUE;							// Continue the PRP test to get the residue...
		else if (resaprcl == 1)
			sprintf (buf,"%s is a probable BPSW prime! (%lu decimal digits, APRCL test) ", sgk, nbdg);
		else if (resaprcl == 2)
			sprintf (buf,"%s is prime! (%lu decimal digits, APRCL test)", sgk, nbdg);
		else if (resaprcl == 6)
			sprintf (buf,"Invalid numerical string in %s\n", sgk);
		else if (resaprcl == 7) {
			sprintf (buf,"APRCL error while testing %s...\n", sgk);
			if (verbose)
				OutputBoth(buf);
			else
				OutputStr (buf);
			goto WSTFCONTINUE;							// Continue the PRP test to get the residue...
		}
		else {
			if (resaprcl == 9)
				sprintf (buf, "APRCL primality test not available for %s\n", sgk);
			else
				sprintf (buf,"Unexpected return value : %d, while APRCL testing %s...\n", resaprcl, sgk);
			if (verbose)
				OutputBoth(buf);
			else
				OutputStr (buf);
			goto WSTFCONTINUE;							// Continue the PRP test to get the residue...
		}
		*res = ((resaprcl == 1)||(resaprcl == 2)||(resaprcl == 12));

#if defined(WIN32) && !defined(_CONSOLE)

		sprintf (buf+strlen(buf), "  Time : "); 

#else

		clearline(100);

		if (*res) {
#if defined (_CONSOLE)
		hConsole = GetStdHandle(STD_OUTPUT_HANDLE);	// Access to Console attributes
		SetConsoleTextAttribute(hConsole, BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
		OutputBoth(buf);
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
#else
		OutputStr("\033[7m");
		OutputBoth(buf);
		OutputStr("\033[0m");
#endif
		}
		else
			OutputBoth(buf);

		sprintf (buf, "  Time : "); 

#endif

/* Output the final timings for APRCL test */

		write_timer (buf+strlen(buf), 1, TIMER_CLR | TIMER_NL); 
		OutputBoth (buf);
		if (resaprcl == 1)
			MakePrimoInput (NP, sgk);
		free(NP);
		free(M);
		free(testn);
		free(testf);
		free(testx);
		return (TRUE); 
	}

WSTFCONTINUE:

#ifndef X86_64

	if (facto) {				// There is a request for a prefactoring only job...

		factored++;

		/* Init filename */

		tempFileName (filename, 'g', NP);

		if (fileExists (filename)) {
			/* Open the intermediate file */
			fhandle = _open (filename, _O_BINARY | _O_RDONLY);
		}

		fres = 0;

		if (n <  LOWFACTORLIMIT || pprimeSieve (n, (unsigned short)facfrom, (unsigned short)facto, &fres, fhandle)) {
			*res = !fres;
			free(NP);
			free(M);
			free(testn);
			free(testf);
			free(testx);
			return (TRUE); 
		}
		else {
			*res = FALSE;
			free(NP);
			free(M);
			free(gk);
			free(testn);
			free(testf);
			free(testx);
			return (FALSE); 
		}
	}

#else

	if (facto) {				// There is a request for a prefactoring only job...
		sprintf (buf, "Prefactoring 64bit code is not yet available ; please use 32bit version.\n");
		OutputBoth (buf);
		*res = FALSE;
		free(NP);
		free(M);
		free(testn);
		free(testf);
		free(testx);
		return (TRUE); 
	}

#endif

// Test if we are resuming a PRP test.

	tempFileName (filename, 's', NP);
	if (fileExists (filename)) {				// Resuming a Fermat SPRP test
		dovrbareix = FALSE;
		goto process;
	}

	tempFileName (filename, 'z', NP);
	if (fileExists (filename)) {				// Resuming a Vrba-Reix test
		dovrbareix = TRUE;
		goto process;
	}

	// If not resuming, prefactor if necessary.

	if (nofac || IniGetInt(INI_FILE, "Verify", 0))
		goto process;

#ifndef X86_64

	// Prepare to trial factor the number

	howfar = facfrom;

	/* Init filename */

	tempFileName (filename, 'f', NP);

	if (fileExists (filename)) {				// Open the intermediate file
		fhandle = _open (filename, _O_BINARY | _O_RDONLY);
	}

	fres = FALSE;

	if(n < LOWFACTORLIMIT || (fisok = pprimeFactor (n, howfar, &fres, WORK_TEST, fhandle))) {
		if (fres) {
			sprintf (buf, "(2^%ld+1)/3 has a factor : %s\n", n, facnstr);
			*res = FALSE;
			OutputBoth (buf);
			free(NP);
			free(M);
			free(testn);
			free(testf);
			free(testx);
			return (TRUE); 
		}
	}
	else {
		*res = FALSE;
		free(NP);
		free(M);
		free(testn);
		free(testf);
		free(testx);
		return (FALSE); 
	}

#else

	sprintf (buf, "Prefactoring not done, due to 64bit code not yet available.\n");
	OutputBoth (buf);

#endif

process:


restart:

	nbdg = gnbdg (NP, 10); // Compute the number of decimal digits of the tested number.

	gwinit (gwdata);
	gdata = &gwdata->gdata;
 	gwset_num_threads (gwdata, IniGetInt(INI_FILE, "ThreadsPerTest", 1));

	globalk = 1.0;

	if (dovrbareix) {						// Compute the seed for the Vrba-Reix test
		gx0 =  allocgiant ((bits >> 4) + 8);	// Allocate memory for gx0
		gtog (NP, gx0);						// gx0 = NP
		uladdg (3, gx0);					// gx0 = N+3
		gshiftright (1, gx0);				// gx0 = (N+3)/2 = 3/2 mod N = 3*2^(-1) mod N
		expx = n-1;
		tempFileName (filename, 'z', NP);	// Set the filename to zxxxxxxx
	}
	else {									// Set he base for the SPRP test
		a = IniGetInt (INI_FILE, "FBase", 3);
		gwsetmaxmulbyconst (gwdata, a);
		expx = n;
		tempFileName (filename, 's', NP);	// Set the filename to sxxxxxxx
	}

	*res = TRUE;						/* Assume it is a prime */ 

	gwset_larger_fftlen_count(gwdata, (char)IniGetInt(INI_FILE, "FFT_Increment", 0));
	if (!setupok (gwdata, gwsetup (gwdata, 1.0, 2, n, +1))) { 	// Setup the DWT mode
		*res = FALSE;
		free(NP);
		free(M);
		if (dovrbareix)
			free (gx0);
		free(testn);
		free(testf);
		free(testx);
		free(gwdata);
		return (FALSE); 
	}


//	Restart with next FFT length if we are too near the limit...

	if (nextifnear && gwnear_fft_limit (gwdata, pcfftlim)) {
		OutputBoth ("Current FFT beeing too near the limit, next FFT length is forced...\n");
		IniWriteInt(INI_FILE, "FFT_Increment", IniGetInt(INI_FILE, "FFT_Increment", 0) + 1);
		if (dovrbareix)
			free (gx0);
		gwdone (gwdata);
		goto restart;
	}

/* More initializations... */

	tmp = popg(gdata, (bits >> 4) + 8);
	tmp2 = popg(gdata, (bits >> 4) + 8);

/* Get the current time */
/* Allocate memory */

	x = gwalloc (gwdata);
	y = gwalloc (gwdata);

/* Optionally resume from save file and output a message */
/* indicating we are resuming a test */

/* ubx and uby are the current units bit positions, in x and y, respectively.*/

	if (fileExists (filename) && gmreadFromFile (gwdata, gdata, filename, &bit, &ubx, &uby, x, y)) {
		char	fmt_mask[80];
		double	pct;
		pct = trunc_percent (bit * 100.0 / expx);
		if (dovrbareix)
			sprintf (fmt_mask,
				"Resuming Vrba-Reix test of %%s at bit %%ld [%%.%df%%%%]\n",
				 PRECISION);
		else
			sprintf (fmt_mask,
				"Resuming SPRP test of %%s at bit %%ld [%%.%df%%%%]\n",
				 PRECISION);
		sprintf (buf, fmt_mask, sgk, bit, pct);
		OutputStr (buf);
		if (verbose || restarting)
			writeResults (buf);
	}

/* Otherwise, output a message indicating we are starting test */

	else {
		clear_timers ();		// Init. timers
		if (showdigits) {
			if (dovrbareix)
				sprintf (buf, "Starting Vrba-Reix test of %s (%lu decimal digits)\n", sgk, nbdg);
			else
				sprintf (buf, "Starting SPRP test of %s (%lu decimal digits)\n", sgk, nbdg);
		}
		else {
			if (dovrbareix)
				sprintf (buf, "Starting Vrba-Reix test of %s\n", sgk);
			else
				sprintf (buf, "Starting SPRP test of %s\n", sgk);
		}
		OutputStr (buf);
		if (verbose || restarting)
			writeResults (buf);
		bit = 1;

/* Compute a random shift for the initial value */

		srand ((unsigned int) time (NULL));
		ubx = (rand() << 16) + rand();
		if (CPU_FLAGS & CPU_RDTSC) { rdtsc(&hi,&lo); ubx += lo; }
		if (dovrbareix) {
			ubx = ubx % (bits);			// To be sure that the shift is not too large...
		}
		else {
			atemp = a;
			while (atemp) {				// Compute the bit length of the Fermat base a
				atemp >>= 1;
				abits++;
			}
			ubx = ubx % (bits-abits);	// To be sure that the shift is not too large...
		}
		uby = 0;

/* Compute the left shifted initial value */

		if (dovrbareix) {
			gtog (gx0, tmp2);
			gshiftleft (ubx, tmp2);
			specialmodg (gwdata, tmp2);
		}
		else {
			itog (a, tmp2);
			gshiftleft (ubx, tmp2);
		}
		gianttogw (gwdata, tmp2, x);
		gwcopy (gwdata, x, y);
	}

	start_timer (0);
	start_timer (1);
	time (&start_time);				// Get current time

/* Output a message about the FFT length. */

	topology_print_children (hwloc_get_root_obj (hwloc_topology), 0);
	gwfft_description (gwdata, fft_desc);
	sprintf (buf, "Using %s\n", fft_desc);

	OutputStr (buf);
	if (verbose || restarting) {
		writeResults (buf);
	}
	ReplaceableLine (1);	/* Remember where replaceable line is */

/* Init the title */

	if (dovrbareix)
		title ("Wagstaff numbers Vrba-Reix test in progress...");
	else
		title ("Wagstaff numbers SPRP test in progress...");

/* Do the PRP test */

	iters = 0;
	while (bit <= expx) {

/* Error check the first and last 50 iterations, before writing an */
/* intermediate file (either user-requested stop or a */
/* 30 minute interval expired), and every 128th iteration. */

		stopping = stopCheck ();
		echk = stopping || ERRCHK || (bit <= 50) || (bit >= expx-50) || gwnear_fft_limit (gwdata, pcfftlim);
		if (((bit & 127) == 0) || (bit == 1) || (bit == (lasterr_point-1))) {
			echk = 1;
			time (&current_time);
			saving = ((current_time - start_time > write_time) || (bit == 1) || (bit == (lasterr_point-1)));
		} else
			saving = 0;

/* Process this bit */

		gwstartnextfft (gwdata, postfft && !debug && !stopping && !saving && bit != lasterr_point && !((interimFiles && (bit+1) % interimFiles == 0)) &&
			!(interimResidues && ((bit+1) % interimResidues < 2)) && 
			(bit >= 30) && (bit < expx-31) && !maxerr_recovery_mode[6]);


		gwsetnormroutine (gwdata, 0, echk, 0);


		ubx <<= 1;
		if (ubx >= bits) 
			ubx -= bits;								// Compute the doubled shift modulo n

		if (dovrbareix)	{								// Fix-up the addin constant
			if (ubx&1)									// See if a change of sign is needed
				gwsetaddinatpowerofb (gwdata, 2, ubx);
			else
				gwsetaddinatpowerofb (gwdata, -2, ubx);
		}

		if ((bit > 30) && (bit < expx-30) && ((bit != lasterr_point) || !maxerr_recovery_mode[6])) {
			gwsquare (gwdata, x);
			care = FALSE;
		}
		else {
			gwsquare_carefully (gwdata, x);
			care = TRUE;
		}

		if (!dovrbareix && bit == (expx - 1)) {
			gwcopy (gwdata, x, y);
			uby = ubx;
		}


		CHECK_IF_ANY_ERROR (x, (bit), expx, 6);

/* That iteration succeeded, bump counters */

		if (bit == lasterr_point)
			saving = 1;					// Be sure to restart after this recovery iteration!
		bit++;
		iters++;

/* Print a message every so often */

		if (bit % ITER_OUTPUT == 0) {
			char	fmt_mask[80];
			double	pct;
			pct = trunc_percent (bit * 100.0 / expx);
			sprintf (fmt_mask, "%%.%df%%%% of %%ld", PRECISION);
			sprintf (buf, fmt_mask, pct, expx);
			title (buf);
			ReplaceableLine (2);	/* Replace line */
			sprintf (fmt_mask,
				 "%%s, bit: %%ld / %%ld [%%.%df%%%%]",
				 PRECISION);
			sprintf (buf, fmt_mask, sgk, bit, expx, pct);
			OutputStr (buf);
			if (ERRCHK && bit > 30) {
				OutputStr (".  Round off: ");
				sprintf (buf, "%10.10f", reallyminerr);
				OutputStr (buf);
				sprintf (buf, " to %10.10f", reallymaxerr);
				OutputStr (buf);
			}
			end_timer (0);
			if (CUMULATIVE_TIMING) {
				OutputStr (".  Time thusfar: ");
			} else {
				OutputStr (".  Time per bit: ");
				divide_timer (0, iters);
				iters = 0;
			}
			print_timer (0, TIMER_NL | TIMER_OPT_CLR);
			start_timer (0);
		}

/* Print a results file message every so often */

		if (bit % ITER_OUTPUT_RES == 0 || (NO_GUI && stopping)) {
			sprintf (buf, "Bit %ld / %ld\n", bit, expx);
			writeResults (buf);
		}

/* Write results to a file every DISK_WRITE_TIME minutes */
/* On error, retry in 10 minutes (it could be a temporary */
/* disk-full situation) */

		if (saving || stopping) {
			write_time = DISK_WRITE_TIME * 60;
			saving = FALSE;
			if (! gmwriteToFile (gwdata, gdata, filename, bit, ubx, uby, x, y)) {
				sprintf (buf, WRITEFILEERR, filename);
				OutputBoth (buf);
				if (write_time > 600) write_time = 600;
			}
			time (&start_time);


/* If an escape key was hit, write out the results and return */

			if (stopping) {
				pushg(gdata, 2);
				free(NP);
				free(M);
				free(testn);
				free(testf);
				free(testx);
				if (dovrbareix)
					free (gx0);
				gwfree (gwdata, x);
				gwfree (gwdata, y);
				gwdone (gwdata);
				*res = FALSE;
				free (gwdata);
				return (FALSE);
			}
		}

/* Output the 64-bit residue at specified interims.  Also output the */
/* residues for the next iteration so that we can compare our */
/* residues to programs that start counter at zero or one. */

		if (interimResidues && bit % interimResidues < 2) {

			setone (tmp2);					// Restore the value of x from the shifted one.
			gshiftleft (ubx, tmp2);
			invg (M,tmp2);
			gtog (M, tmp);
			if (ubx&1)						// View if a sign change on x is necessary.
				subg (tmp2, tmp);
			else
				gtog (tmp2, tmp);

			gwtogiant (gwdata, x, tmp2);	// The modulo M reduction is done here
			mulg (tmp2, tmp);
			specialmodg (gwdata, tmp);

			modg (NP, tmp);
			if (!dovrbareix)
				ulsubg (a*a, tmp);			// Compute the (unnormalized) residue modulo NP
			if (abs(tmp->sign) < 2)			// make a 32 bit residue correct !!
				sprintf (buf, 
				 "%s interim residue %08lX%08lX at iteration %lu\n",
				 sgk, (unsigned long)0, (unsigned long)tmp->n[0], bit);
			else
				sprintf (buf, 
				 "%s interim residue %08lX%08lX at iteration %lu\n",
				 sgk, (unsigned long)tmp->n[1], (unsigned long)tmp->n[0], bit);
			OutputBoth (buf);
		}

/* Write a save file every "interimFiles" iterations. */

		if (interimFiles && bit % interimFiles == 0) {
			char	interimfile[20];
			sprintf (interimfile, "%.8s.%03lu",
				 filename, bit / interimFiles);
			if (! gmwriteToFile (gwdata, gdata, interimfile, bit, ubx, uby, x, y)) {
				sprintf (buf, WRITEFILEERR, interimfile);
				OutputBoth (buf);
			}
		}
	}

	clearline (100);

	setone (tmp2);					// Restore the value of x from the shifted one.
	gshiftleft (ubx, tmp2);
	invg (M,tmp2);
	gtog (M, tmp);
	if (ubx&1)						// View if a sign change on x is necessary.
		subg (tmp2, tmp);
	else
		gtog (tmp2, tmp);

	gwtogiant (gwdata, x, tmp2);	// The modulo M reduction is done here
	mulg (tmp2, tmp);
	specialmodg (gwdata, tmp);		// Result modulo M


	if (dovrbareix) {
		modg (NP, tmp);
		subg (gx0, tmp);
	}
	else {
		itog (a*a, tmp2);
		invg (NP, tmp2);		// a^(-2) modulo NP
		mulg (tmp2, tmp);		// tmp = a^(2^n-2) = a^(3*(NP-1)) --> the very base is a^3 !!
		modg (NP, tmp);			// Compute the (unnormalized) residue modulo NP
	}

/* Do the Strong PRP test. If the number is proven composite, format a 64-bit residue. */

	if ((!dovrbareix && !isone (tmp)) || (dovrbareix && !isZero (tmp))) {
		*res = FALSE;				/* Not a prime */
		if (abs(tmp->sign) < 2)		// make a 32 bit residue correct !!
			sprintf (res64, "%08lX%08lX", (unsigned long)0, (unsigned long)tmp->n[0]);
		else
			sprintf (res64, "%08lX%08lX", (unsigned long)tmp->n[1], (unsigned long)tmp->n[0]);
		imulg (a*a*a, tmp); modg (NP, tmp); ulsubg (a*a*a, tmp);
		if (abs(tmp->sign) < 2)		// make a 32 bit residue correct !!
			sprintf (oldres64, "%08lX%08lX", (unsigned long)0, (unsigned long)tmp->n[0]);
		else
			sprintf (oldres64, "%08lX%08lX", (unsigned long)tmp->n[1], (unsigned long)tmp->n[0]);
		if (vrbareix && !dovrbareix)
			if (IniGetInt (INI_FILE, "OldRes64", 1))
				sprintf (buf, "%s is not prime, although Vrba-Reix PSP!!  RES64: %s.  OLD64: %s", sgk, res64, oldres64);
			else
				sprintf (buf, "%s is not prime, although Vrba-Reix PSP!!  RES64: %s", sgk, res64);
		else if (!vrbareix && !dovrbareix)
			if (IniGetInt (INI_FILE, "OldRes64", 1))
				sprintf (buf, "%s is not prime.  RES64: %s.  OLD64: %s", sgk, res64, oldres64);
			else
				sprintf (buf, "%s is not prime.  RES64: %s", sgk, res64);
		else if (!vrbareix && dovrbareix)
			sprintf (buf, "%s is not prime, although Strong Fermat PSP!!  Vrba-Reix RES64: %s", sgk, res64);
		else
			sprintf (buf, "%s is not prime.  Vrba-Reix RES64: %s", sgk, res64);
	}
	else if (!dovrbareix) {			// May be a prime, continue the SPRP test
		setone (tmp2);				// Restore the value of y from the shifted one.
		gshiftleft (uby, tmp2);
		invg (M,tmp2);
		gtog (M, tmp);
		if (uby&1)					// View if a sign change on y is necessary.
			subg (tmp2, tmp);
		else
			gtog (tmp2, tmp);

		gwtogiant (gwdata, y, tmp2);		// The modulo M reduction is done here
		mulg (tmp2, tmp);
		specialmodg (gwdata, tmp);

		modg (NP, tmp);
		iaddg (a, tmp);
		if (gcompg (NP, tmp) != 0 && (tmp->sign != 1 || tmp->n[0] != 2*a)) {
			*res = FALSE;			/* Not a prime */
			if (vrbareix)
				sprintf (buf, "%s is not prime, although Vrba-Reix and Base %lu - Fermat PSP!!", sgk, a*a*a);
			else
				sprintf (buf, "%s is not prime, although Base %lu - Fermat PSP!!", sgk, a*a*a);
		}
		else {
			sprintf (buf, "%s is Base %lu - Strong Fermat PRP! (%lu decimal digits)", sgk, a*a*a, nbdg);
		}
	}
	else
		sprintf (buf, "%s is Vrba-Reix PRP! (%lu decimal digits)", sgk, nbdg);




#if defined(WIN32) && !defined(_CONSOLE)

	sprintf (buf+strlen(buf), "  Time : "); 
	ReplaceableLine (2);	/* Replace line */ 

#else

	clearline(100);

	if (*res) {
#if defined (_CONSOLE)
		hConsole = GetStdHandle(STD_OUTPUT_HANDLE);	// Access to Console attributes
		SetConsoleTextAttribute(hConsole, BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
		OutputBoth(buf);
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
#else
		OutputStr("\033[7m");
		OutputBoth(buf);
		OutputStr("\033[0m");
#endif
	}
	else
		OutputBoth(buf);

	sprintf (buf, "  Time : "); 

#endif

/* Output the final timings */

	end_timer (1);
	write_timer (buf+strlen(buf), 1, TIMER_CLR | TIMER_NL); 
	OutputBoth (buf);

/* Cleanup and return */


	pushg(gdata, 2);
	gwfree (gwdata, x);
	gwfree (gwdata, y);
	IniWriteString(INI_FILE, "FFT_Increment", NULL);
	lasterr_point = 0;
	gwdone (gwdata);
	_unlink (filename);
	if (dualtest && *res) {				// If dual test required and positive result
		if (vrbareix && dovrbareix) {
			free (gx0);
			dovrbareix = FALSE;
			goto restart;				// Do now a Fermat SPRP test
		}
		else if (!vrbareix && !dovrbareix) {
			dovrbareix = TRUE;
			goto restart;				// Do now a Vrba-Reix test
		}
	}
	if (*res && (nbdg < primolimit))
		MakePrimoInput (NP, sgk);
	free(NP);
	free(M);
	free(testn);
	free(testf);
	free(testx);
	if (dovrbareix)
		free (gx0);
	free (gwdata);
	return (TRUE);

/* An error occured, sleep if required, then try restarting at last save point. */

error:
	pushg(gdata, 2);
	gwfree (gwdata, x);
	gwfree (gwdata, y);

	if ((abonillsum && gw_test_illegal_sumout(gwdata)) || 
		(abonmismatch && gw_test_mismatched_sums (gwdata)) || 
		(abonroundoff && gw_get_maxerr(gwdata) > maxroundoff)) {	// Abort...
		aborted = TRUE;
		sprintf (buf, ERRMSG5, checknumber, sgk);
		OutputBoth (buf);
		*res = FALSE;
		free(NP);
		free(M);
		free(testn);
		free(testf);
		free(testx);
		if (dovrbareix)
			free (gx0);
		_unlink (filename);
		gwdone (gwdata);
		free (gwdata);
		if(IniGetInt(INI_FILE, "StopOnAbort", 0)) {
			IniWriteInt (INI_FILE, "PgenLine", IniGetInt(INI_FILE, "PgenLine", 0) + 1);	// Point on the next line
			return (FALSE);
		}
		else
			return (TRUE);
	}

	gwdone (gwdata);

/* Output a message saying we are restarting */

	if (sleep5) OutputBoth (ERRMSG2);
	OutputBoth (ERRMSG3);

/* Sleep five minutes before restarting */

	if (sleep5 && ! SleepFive ()) {
		free (gwdata);
		*res = FALSE;
		return (FALSE);
	}

/* Restart */

	if (will_try_larger_fft) {
		IniWriteInt(INI_FILE, "FFT_Increment", nbfftinc = (IniGetInt(INI_FILE, "FFT_Increment", 0) + 1));
		if (nbfftinc == maxfftinc)
			abonroundoff = TRUE;	// Don't accept any more Roundoff error.
	}
	goto restart;
}


static unsigned __int64 li, smallbase, smallk, lastfactor;

int ispoweroftwo (
	unsigned long n)
{
	if (!n)
		return (FALSE);
	while (!(n&1))
		n = n >> 1;
	return (n == 1);
}

int process_num (
	unsigned long format,
	char *sgk,
	char *sgb,
	unsigned long n,
	int	incr,
	unsigned long shift,
	int	*res)
{
	int	retval;
	char outbuf[sgkbufsize+256];

// Lei -remove a line and replace
//	unsigned long ninput = n, binput = base;
	unsigned long ninput = n, base, binput, b_2up = 1, b_else = 1, superPRP = 1;
	long mult;
// Lei end

	gformat = format; // save format in a global.

	if((mult = IniGetInt(INI_FILE, "StopOnPrimedK", 0))) {
		sprintf (outbuf, "ks%s", sgk);
		if(IniGetInt(INI_FILE, outbuf, 0) >= mult) {// is the count for this k value reached ?
			*res = FALSE;							// then, skip this test
			return TRUE;
		}
	}
	else if((mult = IniGetInt(INI_FILE, "StopOnPrimedN", 0))) {
		sprintf (outbuf, "ns%lu", n);
		if(IniGetInt(INI_FILE, outbuf, 0) >= mult) {// is the count for this n value reached ?
			*res = FALSE;							// then, skip this test
			return TRUE;
		}
	}
	else if((mult = IniGetInt(INI_FILE, "StopOnPrimedB", 0))) {
		sprintf (outbuf, "bs%s", sgb);
		if(IniGetInt(INI_FILE, outbuf, 0) >= mult) {// is the count for this base value reached ?
			*res = FALSE;							// then, skip this test
			return TRUE;
		}
	}

	if (format == ABCGM)
		return (isGMNP (sgk, n, res));	// Do the primality test of a Gaussian Mersenne norm

	if (format == ABCSP)				// Do the PRP test of a Wagstaff number
		return (isWSPRP (sgk, n, res));

	gb = allocgiant (strlen(sgb)/2 + 8);	// Allocate one byte per decimal digit + spares
	ctog (sgb, gb);						// Convert b string to giant
	if (gb->sign == 1) {				// Test if the base is a small integer...
		binput = base = gb->n[0];		// Then, get the base in an unsigned long
		while (!(base&1) && base > 2) {	// Divide the base by two as far as possible
			base >>= 1;
			n += ninput;
		}

		if (base != 2) {				// Test if the base was a power of two

// Lei
			n -= ninput;
			b_else = base;				// Is odd...
			b_2up = binput / b_else;	// binput = b_else*b_2up
			if ((b_2up > b_else) && (!((format == ABCC) || (format == ABCK) || (format == ABCRU)
												|| (format == ABCGRU) || (format == ABCVARAQS)))) {
				superPRP = 0;			// Then b_2up^ninput > b_else^ninput
			}							// N = k*binput^ninput+c = (k*b_else^ninput)*2^n+c
			else {
// Lei end
				base = binput;			// Do not modify because PRP will be forced...
				n = ninput;
			}
		}

		globalb = base;					// Keep the base of the candidate in a global

// Lei mod
 		if (format == ABCDP) {
			retval = IsPRP (format, sgk, sgb, gb, n, incr, shift, res);
			free (gb); return (retval);
		}
		if (((base == 2) || (superPRP == 0)) && !IniGetInt (INI_FILE, "ForcePRP", 0) && ((incr == -1) || (incr == +1)) && (format != ABCVARAQS)) {
			if (incr == -1)
				retval = IniGetInt(INI_FILE, "TestW", 0) ? isLLRW (format, sgk, n, shift, res) : isLLRP (format, sgk, b_else, n, binput, ninput, shift, res);
			else
				retval = IniGetInt(INI_FILE, "TestW", 0) ? isProthW (format, sgk, n, shift, res) : isProthP (format, sgk, b_else, n, binput, ninput, shift, res);
		}
// end Lei mod

		else if ((format == NPGCC1 || format == NPGCC2) && !IniGetInt (INI_FILE, "ForcePRP", 0)) {
			retval = IsCCP (format, sgk, sgb, gb, n, incr, shift, res);
		}
		else if (!IniGetInt (INI_FILE, "ForcePRP", 0) && (incr == +1 || incr == -1) && (format != ABCVARAQS) && 
		(format != ABCRU) && (format != ABCGRU))
			retval = plusminustest (sgk, sgb, gb, n, incr, shift, res);
		else  {
			retval = IsPRP (format, sgk, sgb, gb, n, incr, shift, res);
		}
		free (gb);
		return (retval);
	}			// End gb is a small interger
	else if ((format == NPGCC1 || format == NPGCC2) && !IniGetInt (INI_FILE, "ForcePRP", 0)) {
		retval = IsCCP (format, sgk, sgb, gb, n, incr, shift, res);
	}
	else if (!IniGetInt (INI_FILE, "ForcePRP", 0) && (incr == +1 || incr == -1) && (format != ABCVARAQS) && 
	(format != ABCRU) && (format != ABCGRU))
		retval = plusminustest (sgk, sgb, gb, n, incr, shift, res);
	else  {
//		Fermat_only = TRUE;     // JP 30/01/17
		retval = IsPRP (format, sgk, sgb, gb, n, incr, shift, res);
//		Fermat_only = FALSE;    // JP 30/01/17
        }
	free (gb);
	return (retval);
}

char	outpf[] = "gqplus.res", outmf[] = "gqminus.res";
char	gqpstring[] = "ABC (2^$a+2^(($a+1)/2)+1)/5\n";
char	gqmstring[] = "ABC (2^$a-2^(($a+1)/2)+1)/5\n";


int primeContinue ()
{

	int	work, nargs, hiline, completed = FALSE;
	unsigned long format, shift, begline, rising_ns, rising_ks, last_processed_n, m;
	char *pinput;

/* Set appropriate priority */

	SetPriority ();

/* Case off the work type */

	work = IniGetInt (INI_FILE, "Work", 0);

/* Handle a sieving program output file */

	if (work == 0) {
	    char	inputfile[80], outputfile[80], oldinputfile[80], cmaxroundoff[10], cpcfftlim[10], sgk[sgkbufsize], buff[sgkbufsize+256];
		char	hbuff[sgkbufsize+256], outbuf[sgkbufsize+256], last_processed_k[sgkbufsize+256], sstart[1000], sstop[1000];
	    FILE *fd;
	    unsigned long i, chainlen, n, nfudge, nn;
	    int	firstline, line, hline, resultline,
			outfd, outfdp, outfdm, res, incr, sign, argcnt, validheader = FALSE;
	    char c;

	    IniGetString (INI_FILE, "PgenInputFile", inputfile, IBSIZE, NULL);
	    IniGetString (INI_FILE, "OldInputFile", oldinputfile, IBSIZE, inputfile);	// Default it to PgenInputFile! JP 26/02/17
	    IniGetString (INI_FILE, "PgenOutputFile", outputfile, IBSIZE, NULL);
	    IniGetString (INI_FILE, "MaxRoundOff", cmaxroundoff, 5, "0.40");
	    IniGetString (INI_FILE, "PercentFFTLimit", cpcfftlim, 5, "0.50");
	    IniGetString (INI_FILE, "SpecialCommand", buff, IBSIZE, NULL);
//		IniWriteString(INI_FILE, "SpecialCommand", NULL);
		maxroundoff = atof (cmaxroundoff);
		pcfftlim = atof (cpcfftlim);
		if (!strcmp (inputfile, oldinputfile))
			firstline = IniGetInt (INI_FILE, "PgenLine", 1);		// Continuing on the same file
		else
			firstline = 1;											// Processing a new file
		last_processed_n = (unsigned long)IniGetInt(INI_FILE, "Last_Processed_n", 0);
		IniGetString(INI_FILE, "Last_Processed_k",last_processed_k, sgkbufsize, NULL);
	    hline = IniGetInt (INI_FILE, "HeaderLine", 0);
	    verbose = IniGetInt (INI_FILE, "Verbose", 0);
	    setuponly = IniGetInt (INI_FILE, "SetupOnly", 0);
		showdigits = IniGetInt (INI_FILE, "ShowDigits", 0);
	    nosaving = IniGetInt (INI_FILE, "NoSaveFile", 0);
		begline = IniGetInt(INI_FILE, "BegLine", 0);
		testgm  = IniGetInt(INI_FILE, "TestGM", 1);
		testgq  = IniGetInt(INI_FILE, "TestGQ", 0);
		testfac  = IniGetInt(INI_FILE, "TestFac", 0);
		facfrom =  IniGetInt(INI_FILE, "FacFrom", 0);
		facto =  IniGetInt(INI_FILE, "FacTo", 0);
		general =  IniGetInt(INI_FILE, "Vgene", 0);
		eps2 =  IniGetInt(INI_FILE, "Veps2", 0);
		debug =  IniGetInt(INI_FILE, "Debug", 0);
		postfft =  IniGetInt(INI_FILE, "Postfft", 1);
		generic =  IniGetInt(INI_FILE, "ForceGeneric", 0);
		vrbareix  = IniGetInt(INI_FILE, "VrbaReixTest", 0);
		dualtest = IniGetInt(INI_FILE, "DualTest", 0);
		bpsw = IniGetInt(INI_FILE, "BPSW", 0);
		hiline =  IniGetInt(INI_FILE, "HiLine", 0);
		rising_ns =  IniGetInt(INI_FILE, "Rising_ns", 0);
		rising_ks =  IniGetInt(INI_FILE, "Rising_ks", 0);
		nofac =  IniGetInt(INI_FILE, "NoPrefactoring", 0);
		maxaprcl = IniGetInt(INI_FILE, "MaxAprcl", 200);
		primolimit = IniGetInt(INI_FILE, "PrimoLimit", 30000);
		nextifnear = IniGetInt(INI_FILE, "NextFFTifNearLimit", 0);
		maxfftinc = IniGetInt(INI_FILE, "MaxFFTinc", 5);

/* A new option to create interim save files every N iterations. */
/* This allows two machines to simultanously work on the same exponent */
/* and compare results along the way. */

		interimFiles = IniGetInt (INI_FILE, "InterimFiles", 0);
		interimResidues = IniGetInt (INI_FILE, "InterimResidues", interimFiles);

/* Option to slow down the program by sleeping after every iteration.  You */
/* might use this on a laptop or a computer running in a hot room to keep */
/* temperatures down and thus reduce the chance of a hardware error.  */

		throttle = IniGetInt (INI_FILE, "Throttle", 0);

OPENFILE :
//	    fd = fopen (inputfile, "r");

		for (i=0;i<5;i++) {
			if ((fd = fopen (inputfile, "r")) != NULL)
				break;
		}
		if (fd==NULL) {
			OutputBoth ("Could not open the input file...\n");
			IniWriteInt (INI_FILE, "WorkDone", 1);
			return (FALSE);
	    }

		sprintf (SVINI_FILE, "save_%s", INI_FILE);		// set the name of the backup Ini File

// Process each line in the output file

		for (line=0; ; line++) {

// Set termination on error conditions

			abonillsum = IniGetInt(INI_FILE, "Abortonillsum", 0);
			abonmismatch = IniGetInt(INI_FILE, "Abortonmismatch", 0);
			abonroundoff = IniGetInt(INI_FILE, "Abortonroundoff", 0);
			if (IniGetInt(INI_FILE, "Abortonerror", 0))
				abonillsum = abonmismatch = abonroundoff = 1;

// Blank the input line

			for (i=0; i<(sgkbufsize+256); i++)
				buff[i] = ' ';
			buff[sgkbufsize+255] = '\n';

// Read the line, break at EOF

			if (fgets (buff, sgkbufsize+256, fd) == NULL) {
				IniWriteInt (INI_FILE, "WorkDone", 1);
				rising_ns = rising_ks = FALSE;
				break;
			}
			else
				IniWriteInt (INI_FILE, "WorkDone", 0);

// Skip this line if requested (we processed it on an earlier run)
// (but don't ignore last header line found!)

			if (hiline && line > hiline) {
				IniWriteInt (INI_FILE, "WorkDone", 1);
				break;
			}

			if (!strncmp (buff, "ABC", 3)) {	// ABC format header found

				sprintf (hbuff, "%s", buff);	// Save the header
				IniWriteInt (INI_FILE, "HeaderLine", line);	// Save the header line number
				hline = line;
				validheader = TRUE;				// Assume it is valid...

				for (pinput=buff+3; *pinput && isspace(*pinput); pinput++);

				for (i=0;i<strlen(pinput);i++)
					if (isspace(pinput[i]))
						pinput[i] = '\0';		// Suppress the EOL characters if necessary.

				if (!strcmp (pinput, grepustring)) {
					format = ABCGRU;
				}
				else if (!strcmp (pinput, abcadstring)) {
					format = ABCVARAQS;
				}
				else if (!strcmp (pinput, diffnumstring))
					format = ABCDNG;
				else if (!strcmp (pinput, ckstring)) {
					format = ABCK;
				}
 				else if (!strcmp (pinput, dpstring)) {
 					format = ABCDP;
 				}
				else if (!strcmp (pinput, repustring)) {
					format = ABCRU;
				}
				else if (!strcmp (pinput, cwstring)) {
					format = ABCCW;
				}
				else if (!strcmp (pinput, abcastring)) {
					format = ABCVARAS;
				}
				else if (!strcmp (pinput, ffstring)) {
					format = ABCFF;
				}
				else if (!strcmp (pinput, gmstring)) {
					format = ABCGM;
				}
				else if (!strcmp (pinput, spstring)) {
					format = ABCSP;
				}
				else if (!strcmp (pinput, gfstring)) {
					format = ABCGF;
				}
				else if (!strcmp (pinput, wfsstring)) {
					format = ABCWFS;
				}
				else if (!strcmp (pinput, wftstring)) {
					format = ABCWFT;
				}
				else if (!strcmp (pinput, numberstring)) {
					format = ABCGPT;
				}
				else if (sscanf(pinput, fnpstring, &n, &incr) == 2) {
					format = ABCFNGS;
				}
				else if (sscanf(pinput, fnmstring, &n, &incr) == 2) {
					format = ABCFNGS;
					incr = - incr;
				}
				else if (sscanf(pinput, fnpstring, &n) == 1) { 
					format = ABCFNAS;
				}
				else if (sscanf(pinput, abcpstring, &incr) == 1) {
					format = ABCVARGS;
				}
				else if (sscanf(pinput, abcmstring, &incr) == 1) {
					format = ABCVARGS;
					incr = - incr;
				}
				else if (sscanf(pinput, diffnumpstring, &incr) == 1) {
					format = ABCDN;
				}
				else if (sscanf(pinput, diffnummstring, &incr) == 1) {
					format = ABCDN;
					incr = - incr;
				}
				else if (sscanf(pinput, fkpstring, &smallk, &incr) == 2) {
					sprintf (sgk, $LLF, smallk);	// unsigned fixed k...	
					format = ABCFKGS;
				}
				else if (sscanf(pinput, fkmstring, &smallk, &incr) == 2) {
					sprintf (sgk, $LLF, smallk);	// unsigned fixed k...	
					format = ABCFKGS;
					incr = - incr;
				}
				else if (sscanf(pinput, fkpstring, &smallk) == 1) { 
					sprintf (sgk, $LLF, smallk);	// unsigned fixed k...	
					format = ABCFKAS;
				}
				else if (sscanf(pinput, fbpstring, &smallbase, &incr) == 2) {
					sprintf (sgb, $LLF, smallbase);	// unsigned fixed base...	
					format = ABCFBGS;
				}
				else if (sscanf(pinput, fbmstring, &smallbase, &incr) == 2) {
					sprintf (sgb, $LLF, smallbase);	// unsigned fixed base...	
					format = ABCFBGS;
					incr = - incr;
				}
				else if (sscanf(pinput, fbastring, &smallbase) == 1) { 
					sprintf (sgb, $LLF, smallbase);	// unsigned fixed base...	
					format = ABCFBAS;
				}
				else {
					OutputBoth ("Invalid ABC format, next data lines will be flushed...\n");
					validheader = FALSE;		// Invalid header found...
				}

				if (format == ABCGM) {
					if (!facto)
						sprintf (pinput+strlen (gmstring),
							" // Let GM(p) = (1+/-i)^p-1, GQ(p) = ((1+/-i)^p+1)/(2+/-i) if p>3, (1+/-i)^p+1 if p<=3\n");
					if (!facto && !fileExists (outpf)) {
						outfdp = _open (outpf, _O_TEXT | _O_RDWR | _O_CREAT, 0666);
						if (outfdp) {
							writelg = _write (outfdp, gqpstring, strlen (gqpstring));
							_close (outfdp);
						}	
					}
					if (!facto && !fileExists (outmf)) {
						outfdm = _open (outmf, _O_TEXT | _O_RDWR | _O_CREAT, 0666);
						if (outfdm) {
							writelg = _write (outfdm, gqmstring, strlen (gqmstring));
							_close (outfdm);
						}
					}
				}
				continue;				// Read next line, but do not change PgenLine!
			}							// End ABC format header found

			else if (((argcnt = sscanf (buff, $LLF":%c:%lu:"$LLF":%lu\n", &li, &c, &chainlen, &smallbase, &mask)) > 1) || !line) {
				if (argcnt < 4) {
					OutputBoth ("Missing or invalid NewPGen header, next data lines will be flushed...\n");
					validheader = FALSE;				// Invalid NewPGen header...
				}
				else {
					sprintf (sgb, $LLF, smallbase);	// Newpgen format admits only unsigned base...	
					validheader = TRUE;
					if (argcnt == 4)
						mask = 0;
					strcpy (hbuff, buff);			// Save the header
					IniWriteInt (INI_FILE, "HeaderLine", line);	// Save the header line number
					hline = line;
					format = NPG;
					if (mask & 0x40) {
						OutputStr ("Primorial NewPgen files are not supported...\n");
						validheader = FALSE;
					}
					if (chainlen == 0) chainlen = 1;
				}
				continue;				// Read next line, but do not change PgenLine!
			}							// End NewPGen header found

			else {						// Processing a data line
				if (((!rising_ns && !rising_ks) || (rising_ns && rising_ks)) && (line < firstline))
					continue;			// Skip this line if requested (we processed it on an earlier run)

				if (!validheader)
					continue;			// Flush data until a valid header is found...

				shift = 0;				// Only one value for the k multiplier

				if (format == NPG) {	// NEWPGEN output

// THIS SECTION IS FOR BACKWARDS COMPATIBILITY WITH PREVIOUS PRP.EXE
// That version used the one character code to determine what to do.
// The new version uses the mask field.

					if (mask == 0 || IniGetInt (INI_FILE, "UseCharCode", 0)) {

// The variable c is a one character code as follows:
//  P : k.b^n+1 (Plus)
//  M : k.b^n-1 (Minus)
//  T: k.b^n+-1 (Twin)
//  S: k.b^n-1; k.b^(n+1)-1 (SG (CC 1st kind len 2))
//  C: k.b^n+1; k.b^(n+1)+1 (CC 2nd kind len 2)
//  B: k.b^n+-1; k.b^(n+1)+-1 (BiTwin)
//  J: k.b^n+-1; k.b^(n+1)-1 (Twin/SG)
//  K: k.b^n+-1; k.b^(n+1)+1 (Twin/CC)
//  Y : k.b^n+1 + others (Lucky Plus)
//  Z : k.b^n-1 + others (Lucky Minus)
//  1: CC 1st kind chain
//  2: CC 2nd kind chain
//  3: BiTwin chain
// Undo the increment of n that newpgen did on types 1, 2, 3
// Map P, M, Y, Z, T, S, C, B to their more generic counterparts

						nfudge = 0;
						if (c == '1') nfudge = 1;
						if (c == '2') nfudge = 1;
						if (c == '3') nfudge = 1;
						if (c == 'P') c = '2', chainlen = 1;
						if (c == 'M') c = '1', chainlen = 1;
//						if (c == 'Y') c = '2', chainlen = 1;
//						if (c == 'Z') c = '1', chainlen = 1;
						if (c == 'T') c = '3', chainlen = 1;
						if (c == 'S') c = '1', chainlen = 2;
						if (c == 'C') c = '2', chainlen = 2;
						if (c == 'B') c = '3', chainlen = 2;


// Process each line in the newpgen output file

// allow k to be a big integer
						if (sscanf (buff+begline, "%s %lu", sgk, &n) != 2)
							continue;				// Skip invalid line

						if (!isDigitString(sgk))
							continue;				// Skip invalid line

						if (rising_ns && !rising_ks && (n <= last_processed_n))
							continue;				// Skip already processed n's


						if (rising_ks && !rising_ns && (digitstrcmp (sgk, last_processed_k) <= 0))
							continue;				// Skip already processed k's

						if ((rising_ns && !rising_ks) || (!rising_ns && rising_ks))
							fclose (fd);			// Unlock the file during the test...

// Test numbers according to the c variable

						nn = n;
						if (c == 'Y') {
							nn--;
						}
						if (c == 'Z') {
							nn--;
						}

						for (i = 0; i < chainlen; i++) {
							if (c == '1' || c == '3') {
								if (! process_num (format, sgk, sgb, n - nfudge + i, -1, shift, &res))
									goto done;
								if (!res)
									break;
								if (c == '1')
									format = NPGCC1;
							}
							if (c == '2' || c == '3') {
								if (! process_num (format, sgk, sgb, n - nfudge + i, +1, shift, &res))
									goto done;
								if (!res)
									break;
								if (c == '2')
									format = NPGCC2;
							}
							if (c == 'J') {	// Twin/SG
								int	res2;
								if (! process_num (format, sgk, sgb, n, -1, shift, &res))
									goto done;
								if (!res)
									break;
								if (! process_num (format, sgk, sgb, n+1, -1, shift, &res))
									goto done;
								if (! process_num (format, sgk, sgb, n, +1, shift, &res2))
									goto done;
								res |= res2;
								format = NPG;
								break;
							}
							if (c == 'K') {	// Twin/CC
								int	res2;
								if (! process_num (format, sgk, sgb, n, +1, shift, &res))
									goto done;
								if (!res)
									break;
								if (! process_num (format, sgk, sgb, n, -1, shift, &res))
									goto done;
								if (! process_num (format, sgk, sgb, n+1, +1, shift, &res2))
									goto done;
								res |= res2;
								format = NPG;
								break;
							}
							if (c == 'Y') {	// Lucky Plus
								int	res2;
								if (! process_num (format, sgk, sgb, nn+1, +1, shift, &res))
									goto done;
								if (!res)
									break;
								if (! process_num (format, sgk, sgb, nn+1, -1, shift, &res))
									goto done;
								if (! process_num (format, sgk, sgb, nn, +1, shift, &res2))
									goto done;
								res |= res2;
								if (! process_num (format, sgk, sgb, nn+2, +1, shift, &res2))
									goto done;
								res |= res2;
								format = NPG;
								break;
							}
							if (c == 'Z') {	// Lucky Minus
								int	res2;
								if (! process_num (format, sgk, sgb, nn+1, -1, shift, &res))
									goto done;
								if (!res)
									break;
								if (! process_num (format, sgk, sgb, nn+1, +1, shift, &res))
									goto done;
								if (! process_num (format, sgk, sgb, nn, -1, shift, &res2))
									goto done;
								res |= res2;
								if (! process_num (format, sgk, sgb, nn+2, -1, shift, &res2))
									goto done;
								res |= res2;
								format = NPG;
								break;
							}
							if (c == 'A') {	// AP mode
								format = NPGAP;
								if (! process_num (format, sgk, sgb, n, -1, shift, &res))
									goto done;
								format = NPG;
								if (!res) break;
							}
						}		// End loop on chain length
						format = NPG;
					}			// End of old section


// THIS IS THE NEW SECTION.  It uses both the mask field and the
// character code to determine what to do

					else {

// NEWPGEN output files use the mask as defined below:
// #define MODE_PLUS    0x01	/* k.b^n+1
// #define MODE_MINUS   0x02	/* k.b^n-1
// #define MODE_2PLUS   0x04	/* k.b^(n+1)+1 (*)
// #define MODE_2MINUS  0x08	/* k.b^(n+1)-1 (*)
// #define MODE_4PLUS   0x10	/* k.b^(n+2)+1 (*)
// #define MODE_4MINUS  0x20	/* k.b^(n+2)-1 (*)
// #define MODE_PRIMORIAL 0x40	/* PRIMORIAL - can't handle this
// #define MODE_PLUS5  0x80	/* k.b^n+5
// #define MODE_AP	    0x200	/* 2^n+2k-1
// #define MODE_PLUS7  0x800	/* k.b^n+7
// #define MODE_2PLUS3 0x1000	/* 2k.b^n+3
// #define MODE_DUAL 0x8000
// #define MODE_PLUS_DUAL 0x8001	/* b^n+k
// #define MODE_MINUS_DUAL 0x8002	/* b^n-k
// #define MODE_NOTGENERALISED 0x400
// Those entries that have a (*) next to them are modified if the
// MODE_NOTGENERALISED flag is set.  If it is set, they are changed
// as follows
// MODE_2PLUS      2k.b^n+1
// MODE_2MINUS     2k.b^n-1
// MODE_4PLUS      4k.b^n+1
// MODE_4MINUS     4k.b^n-1
// Similarly, longer chains are affected in the same way (so if the base
// is 3 and we are after a CC of the 1st kind of length 4, rather that
// looking at k.3^n-1 & k.3^(n+1)-1 & k.3^(n+2)-1 & k.3^(n+3)-1 we look
// at k.3^n-1 & 2k.3^n-1 & 4k.3^n-1 & 8k.3^n-1).

// allow k to be a big integer

						if (sscanf (buff+begline, "%s %lu", sgk, &n) != 2)
							continue;	// Skip invalid line

						if (!isDigitString(sgk))
							continue;	// Skip invalid line


						if (rising_ns && !rising_ks && (n <= last_processed_n))
							continue;				// Skip already processed n's

						if (rising_ks && !rising_ns && (digitstrcmp (sgk, last_processed_k) <= 0))
							continue;				// Skip already processed k's

						if ((rising_ns && !rising_ks) || (!rising_ns && rising_ks))
							fclose (fd);			// Unlock the file during the test...

// Undo the increment of n that newpgen did on types 1, 2, 3

						nn = n;
//						if (c == '1' || c == '2' || c == '3')
//							nn--;

						if (c == 'S')
							chainlen = 2;
						if (c == 'C')
							chainlen = 2;
						if (c == 'B')
							chainlen = 2;

						if ((mask & MODE_PLUS) && (mask & MODE_2MINUS) &&
								(mask & MODE_2PLUS) && (mask & MODE_4PLUS)) {
							nn--;
						}
						if ((mask & MODE_MINUS) && (mask & MODE_2MINUS) &&
								(mask & MODE_2PLUS) && (mask & MODE_4MINUS)) {
							nn--;
						}

// Test numbers according to the mask variable
// The J and K types (Twin/CC and Twin/SG) are special in that they
// are output if either a Twin OR a CC/SG is found

						shift = 0;

						for (i = 0; i < chainlen; i++) {
							if ((mask & MODE_MINUS) && (mask & MODE_PLUS) &&
								(mask & MODE_2MINUS)) {	// Twin/SG
								int	res2;
								if (! process_num (format, sgk, sgb, nn, -1, shift, &res))
									goto done;
								if (!res)
									break;
								if (! process_num (format, sgk, sgb, nn+1, -1, shift, &res))
									goto done;
								if (! process_num (format, sgk, sgb, nn, +1, shift, &res2))
									goto done;
								res |= res2;
								break;
							}
							if ((mask & MODE_MINUS) && (mask & MODE_PLUS) &&
								(mask & MODE_2PLUS)) {	// Twin/CC
								int	res2;
								if (! process_num (format, sgk, sgb, nn, +1, shift, &res))
									goto done;
								if (!res)
									break;
								if (! process_num (format, sgk, sgb, nn, -1, shift, &res))
									goto done;
								if (! process_num (format, sgk, sgb, nn+1, +1, shift, &res2))
									goto done;
								res |= res2;
								break;
							}
							if ((mask & MODE_PLUS) && (mask & MODE_2MINUS) &&
								(mask & MODE_2PLUS) && (mask & MODE_4PLUS)) {	// Lucky Plus
								int	res2;
								if (! process_num (format, sgk, sgb, nn+1, +1, shift, &res))
									goto done;
								if (!res)
									break;
								if (! process_num (format, sgk, sgb, nn+1, -1, shift, &res))
									goto done;
								if (! process_num (format, sgk, sgb, nn, +1, shift, &res2))
									goto done;
								res |= res2;
								if (! process_num (format, sgk, sgb, nn+2, +1, shift, &res2))
									goto done;
								res |= res2;
								break;
							}
							if ((mask & MODE_MINUS) && (mask & MODE_2MINUS) &&
								(mask & MODE_2PLUS) && (mask & MODE_4MINUS)) {	// Lucky Minus
								int	res2;
								if (! process_num (format, sgk, sgb, nn+1, -1, shift, &res))
									goto done;
								if (!res)
									break;
								if (! process_num (format, sgk, sgb, nn+1, +1, shift, &res))
									goto done;
								if (! process_num (format, sgk, sgb, nn, -1, shift, &res2))
									goto done;
								res |= res2;
								if (! process_num (format, sgk, sgb, nn+2, -1, shift, &res2))
									goto done;
								res |= res2;
								break;
							}
							if (mask & MODE_MINUS) {
								if (mask & MODE_DUAL) {
									if (! process_num (format, "1", sgb, nn, -atoi(sgk), shift, &res))
										goto done;
								}
								else {
									if (! process_num (format, sgk, sgb, nn, -1, shift, &res))
										goto done;
								}
								if (!res) {
//									i = chainlen;
									break;
								}
							}
							if (mask & MODE_PLUS) {
								if (mask & MODE_DUAL) {
									if (! process_num (format, "1", sgb, nn, atoi(sgk), shift, &res))
										goto done;
								}
								else
									if (! process_num (format, sgk, sgb, nn, +1, shift, &res))
										goto done;
								if (!res)
									break;
							}
							if (mask & MODE_PLUS5) {
								if (! process_num (format, sgk, sgb, nn, +5, shift, &res))
									goto done;
								if (!res)
									break;
							}
							if (mask & MODE_PLUS7) {
								if (! process_num (format, sgk, sgb, nn, +7, shift, &res))
									goto done;
								if (!res)
									break;
							}
							if (mask & MODE_2PLUS3) {
								shift = 1;
								format = NPGCC1;
								if (! process_num (format, sgk, sgb, nn, +3, shift, &res))
									goto done;
								shift = 0;
								format = NPG;
								if (!res)
									break;
							}
							if (mask & MODE_AP) {
								format = NPGAP;
								if (! process_num (format, sgk, sgb, nn, -1, shift, &res))
									goto done;
								format = NPG;
								if (!res)
									break;
							}

// Bump k or n for the next iteration or for the MODE_2PLUS and
// MODE_2MINUS flags

							if (mask & MODE_NOTGENERALISED)
								shift += 1; 
							else
								nn += 1;

// If chainlength is more than 1, then we let the for loop do the work
// rather than the MODE_2PLUS, etc. flags

							if (chainlen > 1) {
								if ((mask & MODE_2MINUS) || (mask & MODE_4MINUS))
									format = NPGCC1;
								else if ((mask & MODE_2PLUS) || (mask & MODE_4PLUS))
									format = NPGCC2;
								else
									format = NPG;
								continue;
							}
							if (mask & MODE_2MINUS) {
								if (! process_num (format, sgk, sgb, nn, -1, shift, &res))
									goto done;
								if (!res)
									break;
							}
							if (mask & MODE_2PLUS) {
								if (! process_num (format, sgk, sgb, nn, +1, shift, &res))
									goto done;
								if (!res)
									break;
							}

// Bump k or n for the MODE_4PLUS and MODE_4MINUS flags

							if (mask & MODE_NOTGENERALISED)
								shift += 1; 
							else
								nn += 1;

							if (mask & MODE_4MINUS) {
								if (! process_num (format, sgk, sgb, nn, -1, shift, &res))
									goto done;
								if (!res)
									break;
							}
							if (mask & MODE_4PLUS) {
								if (! process_num (format, sgk, sgb, nn, +1, shift, &res))
									goto done;
								if (!res)
									break;
							}
						}	// End of loop on chain length
						format = NPG;
					}		// End of new section

// If all numbers tested were primes or PRPs, copy the line to the output file

					if (res) {
						resultline = IniGetInt(INI_FILE, "ResultLine", 0);
						outfd = _open (outputfile, _O_TEXT | _O_RDWR | _O_APPEND | _O_CREAT, 0666);
						if (outfd) {
							if (hline >= resultline) {	// write the relevant header
								writelg = _write (outfd, hbuff, strlen (hbuff));
							}
							sprintf (outbuf, "%s %lu\n", sgk, n);	// write the result
							writelg = _write (outfd, outbuf, strlen (outbuf));
							_close (outfd);
						}
						IniWriteInt (INI_FILE, "ResultLine", line);	// update the result line
					}
				}			// End of NewPGen format processing

				else if (format == ABCWFT) {			// Wieferich test
					if ((nargs = sscanf (buff+begline, "%s %s", sgk, sgb)) < 1)
						continue;						// Skip invalid line
					if (!isDigitString (sgk))
						continue;						// Skip invalid line
					if (nargs == 2) {
						if (!isDigitString (sgb))
							continue;					// Skip invalid line
						if (!gmpisWieferich (sgk, sgb, &res))
							goto done;
					}
					else if (!gmpisWieferich (sgk, NULL, &res))
							goto done;
					if (res) {
						resultline = IniGetInt(INI_FILE, "ResultLine", 0);
						outfd = _open (outputfile, _O_TEXT | _O_RDWR | _O_APPEND | _O_CREAT, 0666);
						if (outfd) {
							if (hline >= resultline) {	// write the relevant header
								writelg = _write (outfd, hbuff, strlen (hbuff));
							}
							if (nargs == 2)
								sprintf (outbuf, "%s %s\n", sgk, sgb); 
							else
								sprintf (outbuf, "%s\n", sgk); 
							writelg = _write (outfd, outbuf, strlen (outbuf));
							_close (outfd);
						}
						IniWriteInt (INI_FILE, "ResultLine", line);	// update the result line
					}
				}
				else if (format == ABCWFS) {			// Wieferich search
					if ((nargs = sscanf (buff+begline, "%s %s %s", sstart, sstop, sgb)) < 2)
						continue;						// Skip invalid line
					if (!isDigitString (sstart))
						continue;						// Skip invalid line
					if (!isDigitString (sstop))
						continue;						// Skip invalid line
					if (nargs == 3) {
						if (!isDigitString (sgb))
							continue;					// Skip invalid line
						if (!gmpSearchWieferich (sstart, sstop, sgb))
							goto done;
					}
					else {
						sprintf (sgb,"2");
						if (!gmpSearchWieferich (sstart, sstop, NULL))
							goto done;
					}
				}
				else if (format == ABCGPT) {			// General prime test
					if (sscanf (buff+begline, "%s", sgk) != 1)
						continue;						// Skip invalid line
					if (!isDigitString (sgk))
						continue;						// Skip invalid line
					res = saprcltest (sgk, FALSE, FALSE);
					if (res == 2) {
						sprintf (buff, "%s is prime!(APRCL test)\n", sgk);
						clearline (100);
#if defined (__linux__) || defined (__FreeBSD__) || defined (__APPLE__)
						OutputStr("\033[7m");
						OutputBoth (buff);
						OutputStr("\033[0m");
#else
#if defined (_CONSOLE)
						hConsole = GetStdHandle(STD_OUTPUT_HANDLE);	// Access to Console attributes
						SetConsoleTextAttribute(hConsole, BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
						OutputBoth(buff);
						SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
#else
						OutputBoth (buff);
#endif
#endif
						clearline (100);
						resultline = IniGetInt(INI_FILE, "ResultLine", 0);
						outfd = _open (outputfile, _O_TEXT | _O_RDWR | _O_APPEND | _O_CREAT, 0666);
						if (outfd) {
							if (hline >= resultline) {	// write the relevant header
								writelg = _write (outfd, hbuff, strlen (hbuff));
							}
							sprintf (outbuf, "%s\n", sgk); 
							writelg = _write (outfd, outbuf, strlen (outbuf));
							_close (outfd);
						}
						IniWriteInt (INI_FILE, "ResultLine", line);	// update the result line
					}
					else if (res == 0){
						sprintf (buff, "%s is not prime.(APRCL test)\n", sgk);
						OutputBoth (buff);
					}
					else if (res == 9) {
						sprintf (buff, "APRCL primality test not available for %s\n", sgk);
						if (verbose)
							OutputBoth(buff);
						else
							OutputStr (buff);
						continue;
					}
					else if (res == 7) {
						sprintf (buff,"APRCL error while testing %s...\n", sgk);
						if (verbose)
							OutputBoth(buff);
						else
							OutputStr (buff);
					}
					else {
						sprintf (buff,"Unexpected return value : %d, while APRCL testing %s...\n", res, sgk);
						if (verbose)
							OutputBoth(buff);
						else
							OutputStr (buff);
					}
				}
				else if (format == ABCCW) {		// Cullen/Woodall
					if (sscanf (buff+begline, "%lu %s %d", &n, sgb, &incr) != 3)
						continue;				// Skip invalid line
					if (!isDigitString (sgb))
						continue;				// Skip invalid line
					if (rising_ns && !rising_ks  && (n <= last_processed_n))
						continue;				// Skip already processed n's
					sprintf (sgk, "%lu", n);
					if (rising_ks && !rising_ns && (digitstrcmp (sgk, last_processed_k) <= 0))
						continue;				// Skip already processed k's
					if ((rising_ns && !rising_ks) || (!rising_ns && rising_ks))
						fclose (fd);			// Unlock the file during the test...
					if (! process_num (format, sgk, sgb, n, incr, shift, &res))
						goto done;
					if (res) {
						resultline = IniGetInt(INI_FILE, "ResultLine", 0);
						outfd = _open (outputfile, _O_TEXT | _O_RDWR | _O_APPEND | _O_CREAT, 0666);
						if (outfd) {
							if (hline >= resultline) {	// write the relevant header
								writelg = _write (outfd, hbuff, strlen (hbuff));
							}
							sprintf (outbuf, "%s %s %d\n", sgk, sgb, incr); 
							writelg = _write (outfd, outbuf, strlen (outbuf));
							_close (outfd);
						}
						IniWriteInt (INI_FILE, "ResultLine", line);	// update the result line
					}
				}
				else if (format == ABCFF)	{	// FermFact output
												// allow k to be a big integer
					if (sscanf (buff+begline, "%s %lu", sgk, &n) != 2)
						continue;				// Skip invalid line
					if (!isDigitString(sgk))
						continue;				// Skip invalid line
					if (rising_ns && !rising_ks && (n <= last_processed_n))
						continue;				// Skip already processed n's
					if (rising_ks && !rising_ns && (digitstrcmp (sgk, last_processed_k) <= 0))
						continue;				// Skip already processed k's
					sprintf (sgb, "2");
					if ((rising_ns && !rising_ks) || (!rising_ns && rising_ks))
						fclose (fd);			// Unlock the file during the test...
					if (! process_num (format, sgk, "2", n, +1, shift, &res))
						goto done;
					if (res) {
						resultline = IniGetInt(INI_FILE, "ResultLine", 0);
						outfd = _open (outputfile, _O_TEXT | _O_RDWR | _O_APPEND | _O_CREAT, 0666);
						if (outfd) {
							if (hline >= resultline) {	// write the relevant header
								writelg = _write (outfd, hbuff, strlen (hbuff));
							}
							sprintf (outbuf, "%s %lu\n", sgk, n); 
							writelg = _write (outfd, outbuf, strlen (outbuf));
							_close (outfd);
						}
						IniWriteInt (INI_FILE, "ResultLine", line);	// update the result line
					}
				}
				else if (format == ABCLEI)       {	// Lei output
											// allow k to be a big integer
					if (sscanf (buff+begline, "%s %lu", sgk, &n) != 2)
						continue;			// Skip invalid line
					if (!isDigitString(sgk))
						continue;			// Skip invalid line
					if (rising_ns && !rising_ks && (n <= last_processed_n))
						continue;				// Skip already processed n's
					if (rising_ks && !rising_ns && (digitstrcmp (sgk, last_processed_k) <= 0))
						continue;				// Skip already processed k's
					sprintf (sgb, "2");
					if ((rising_ns && !rising_ks) || (!rising_ns && rising_ks))
						fclose (fd);			// Unlock the file during the test...
					if (! process_num (format, sgk, "2", n, -1, shift, &res))
						goto done;
					if (res) {
						resultline = IniGetInt(INI_FILE, "ResultLine", 0);
						outfd = _open (outputfile, _O_TEXT | _O_RDWR | _O_APPEND | _O_CREAT, 0666);
						if (outfd) {
							if (hline >= resultline) {	// write the relevant header
								writelg = _write (outfd, hbuff, strlen (hbuff));
							}
							sprintf (outbuf, "%s %lu\n", sgk, n);
							writelg = _write (outfd, outbuf, strlen (outbuf));
							_close (outfd);
						}
						IniWriteInt (INI_FILE, "ResultLine", line);	// update the result line
					}
				}
				else if (format == ABCFKGS)	{	// Fixed k:  b and n specified on each input line
					if (sscanf (buff+begline, "%s %lu", sgb, &n) != 2)
						continue;				// Skip invalid line
					if (!isDigitString (sgb))
						continue;				// Skip invalid line
					if (rising_ns && (n <= last_processed_n))
						continue;				// Skip already processed n's
					if ((rising_ns && !rising_ks) || (!rising_ns && rising_ks))
						fclose (fd);			// Unlock the file during the test...
					if (! process_num (format, sgk, sgb, n, incr, shift, &res))
						goto done;
					if (res) {
						resultline = IniGetInt(INI_FILE, "ResultLine", 0);
						outfd = _open (outputfile, _O_TEXT | _O_RDWR | _O_APPEND | _O_CREAT, 0666);
						if (outfd) {
							if (hline >= resultline) {	// write the relevant header
								writelg = _write (outfd, hbuff, strlen (hbuff));
							}
							sprintf (outbuf, "%s %lu\n", sgb, n); 
							writelg = _write (outfd, outbuf, strlen (outbuf));
							_close (outfd);
						}
						IniWriteInt (INI_FILE, "ResultLine", line);	// update the result line
					}
				}
				else if (format == ABCFKAS)	{	// Fixed k:  b, n, and c specified on each input line
					if (sscanf (buff+begline, "%s %lu %d", sgb, &n, &incr) != 3)
						continue;				// Skip invalid line
					if (!isDigitString(sgk))
						continue;				// Skip invalid line
					if (!isDigitString (sgb))
						continue;				// Skip invalid line
					if (rising_ns && (n <= last_processed_n))
						continue;				// Skip already processed n's
					if ((rising_ns && !rising_ks) || (!rising_ns && rising_ks))
						fclose (fd);			// Unlock the file during the test...
					if (! process_num (format, sgk, sgb, n, incr, shift, &res))
						goto done;
					if (res) {
						resultline = IniGetInt(INI_FILE, "ResultLine", 0);
						outfd = _open (outputfile, _O_TEXT | _O_RDWR | _O_APPEND | _O_CREAT, 0666);
						if (outfd) {
							if (hline >= resultline) {	// write the relevant header
								writelg = _write (outfd, hbuff, strlen (hbuff));
							}
							sprintf (outbuf, "%s %lu %d\n", sgb, n, incr); 
							writelg = _write (outfd, outbuf, strlen (outbuf));
							_close (outfd);
						}
						IniWriteInt (INI_FILE, "ResultLine", line);	// update the result line
					}
				}
				else if (format == ABCFBGS)	{	// Fixed b:  k and n specified on each input line
					if (sscanf (buff+begline, "%s %lu", sgk, &n) != 2)
						continue;				// Skip invalid line
					if (!isDigitString(sgk))
						continue;				// Skip invalid line
					if (rising_ns && !rising_ks && (n <= last_processed_n))
						continue;				// Skip already processed n's
					if (rising_ks && !rising_ns && (digitstrcmp (sgk, last_processed_k) <= 0))
						continue;				// Skip already processed k's
					if ((rising_ns && !rising_ks) || (!rising_ns && rising_ks))
						fclose (fd);			// Unlock the file during the test...
					if (! process_num (format, sgk, sgb, n, incr, shift, &res))
						goto done;
					if (res) {
						resultline = IniGetInt(INI_FILE, "ResultLine", 0);
						outfd = _open (outputfile, _O_TEXT | _O_RDWR | _O_APPEND | _O_CREAT, 0666);
						if (outfd) {
							if (hline >= resultline) {	// write the relevant header
								writelg = _write (outfd, hbuff, strlen (hbuff));
							}
							sprintf (outbuf, "%s %lu\n", sgk, n); 
							writelg = _write (outfd, outbuf, strlen (outbuf));
							_close (outfd);
						}
						IniWriteInt (INI_FILE, "ResultLine", line);	// update the result line
					}
				}
				else if (format == ABCFBAS)	{	// Fixed b:  k, n, and c specified on each input line
					if (sscanf (buff+begline, "%s %lu %d", sgk, &n, &incr) != 3)
						continue;				// Skip invalid line
					if (!isDigitString(sgk))
						continue;				// Skip invalid line
					if (rising_ns && !rising_ks && (n <= last_processed_n))
						continue;				// Skip already processed n's
					if (rising_ks && !rising_ns && (digitstrcmp (sgk, last_processed_k) <= 0))
						continue;				// Skip already processed k's
					if ((rising_ns && !rising_ks) || (!rising_ns && rising_ks))
						fclose (fd);			// Unlock the file during the test...
					if (! process_num (format, sgk, sgb, n, incr, shift, &res))
						goto done;
					if (res) {
						resultline = IniGetInt(INI_FILE, "ResultLine", 0);
						outfd = _open (outputfile, _O_TEXT | _O_RDWR | _O_APPEND | _O_CREAT, 0666);
						if (outfd) {
							if (hline >= resultline) {	// write the relevant header
								writelg = _write (outfd, hbuff, strlen (hbuff));
							}
							sprintf (outbuf, "%s %lu %d\n", sgk, n, incr); 
							writelg = _write (outfd, outbuf, strlen (outbuf));
							_close (outfd);
						}
						IniWriteInt (INI_FILE, "ResultLine", line);	// update the result line
					}
				}
				else if (format == ABCFNGS)	{	// Fixed n:  k and b specified on each input line
					if (sscanf (buff+begline, "%s %s", sgk, sgb) != 2)
						continue;				// Skip invalid line
					if (!isDigitString(sgk))
						continue;				// Skip invalid line
					if (!isDigitString (sgb))
						continue;				// Skip invalid line
					if (rising_ks && !rising_ns && (digitstrcmp (sgk, last_processed_k) <= 0))
						continue;				// Skip already processed k's
					if ((rising_ns && !rising_ks) || (!rising_ns && rising_ks))
						fclose (fd);			// Unlock the file during the test...
					if (! process_num (format, sgk, sgb, n, incr, shift, &res))
						goto done;
					if (res) {
						resultline = IniGetInt(INI_FILE, "ResultLine", 0);
						outfd = _open (outputfile, _O_TEXT | _O_RDWR | _O_APPEND | _O_CREAT, 0666);
						if (outfd) {
							if (hline >= resultline) {	// write the relevant header
								writelg = _write (outfd, hbuff, strlen (hbuff));
							}
							sprintf (outbuf, "%s %s\n", sgk, sgb); 
							writelg = _write (outfd, outbuf, strlen (outbuf));
							_close (outfd);
						}
						IniWriteInt (INI_FILE, "ResultLine", line);	// update the result line
					}
				}
				else if (format == ABCFNAS)	{	// Fixed n:  k, b, and c specified on each input line
					if (sscanf (buff+begline, "%s %s %d", sgk, sgb, &incr) != 3)
						continue;				// Skip invalid line
					if (!isDigitString(sgk))
						continue;				// Skip invalid line
					if (!isDigitString (sgb))
						continue;				// Skip invalid line
					if (rising_ks && !rising_ns && (digitstrcmp (sgk, last_processed_k) <= 0))
						continue;				// Skip already processed k's
					if ((rising_ns && !rising_ks) || (!rising_ns && rising_ks))
						fclose (fd);			// Unlock the file during the test...
					if (! process_num (format, sgk, sgb, n, incr, shift, &res))
						goto done;
					if (res) {
						resultline = IniGetInt(INI_FILE, "ResultLine", 0);
						outfd = _open (outputfile, _O_TEXT | _O_RDWR | _O_APPEND | _O_CREAT, 0666);
						if (outfd) {
							if (hline >= resultline) {	// write the relevant header
								writelg = _write (outfd, hbuff, strlen (hbuff));
							}
							sprintf (outbuf, "%s %s %d\n", sgk, sgb, incr); 
							writelg = _write (outfd, outbuf, strlen (outbuf));
							_close (outfd);
						}
						IniWriteInt (INI_FILE, "ResultLine", line);	// update the result line
					}
				}
				else if (format == ABCVARGS)	{	// k, b, and n specified on each input line
					if (sscanf (buff+begline, "%s %s %lu", sgk, sgb, &n) != 3)
						continue;				// Skip invalid line
					if (!isDigitString(sgk))
						continue;				// Skip invalid line
					if (!isDigitString (sgb))
						continue;				// Skip invalid line
					if (rising_ns && !rising_ks && (n <= last_processed_n))
						continue;				// Skip already processed n's
					if (rising_ks && !rising_ns && (digitstrcmp (sgk, last_processed_k) <= 0))
						continue;				// Skip already processed k's
					if ((rising_ns && !rising_ks) || (!rising_ns && rising_ks))
						fclose (fd);			// Unlock the file during the test...
					if (! process_num (format, sgk, sgb, n, incr, shift, &res))
						goto done;
					if (res) {
						resultline = IniGetInt(INI_FILE, "ResultLine", 0);
						outfd = _open (outputfile, _O_TEXT | _O_RDWR | _O_APPEND | _O_CREAT, 0666);
						if (outfd) {
							if (hline >= resultline) {	// write the relevant header
								writelg = _write (outfd, hbuff, strlen (hbuff));
							}
							sprintf (outbuf, "%s %s %lu\n", sgk, sgb, n); 
							writelg = _write (outfd, outbuf, strlen (outbuf));
							_close (outfd);
						}
						IniWriteInt (INI_FILE, "ResultLine", line);	// update the result line
					}
				}
				else if (format == ABCVARAS)	{	// k, b, n, and c specified on each input line
					if (sscanf (buff+begline, "%s %s %lu %d", sgk, sgb, &n, &incr) != 4)
						continue;				// Skip invalid line
					if (!isDigitString(sgk))
						continue;				// Skip invalid line
					if (!isDigitString (sgb))
						continue;				// Skip invalid line
					if (rising_ns && !rising_ks && (n <= last_processed_n))
						continue;				// Skip already processed n's
					if (rising_ks && !rising_ns && (digitstrcmp (sgk, last_processed_k) <= 0))
						continue;				// Skip already processed k's
					if ((rising_ns && !rising_ks) || (!rising_ns && rising_ks))
						fclose (fd);			// Unlock the file during the test...
					if (! process_num (format, sgk, sgb, n, incr, shift, &res))
						goto done;
					if (res) {
						resultline = IniGetInt(INI_FILE, "ResultLine", 0);
						outfd = _open (outputfile, _O_TEXT | _O_RDWR | _O_APPEND | _O_CREAT, 0666);
						if (outfd) {
							if (hline >= resultline) {	// write the relevant header
								writelg = _write (outfd, hbuff, strlen (hbuff));
							}
							sprintf (outbuf, "%s %s %lu %d\n", sgk, sgb, n, incr); 
							writelg = _write (outfd, outbuf, strlen (outbuf));
							_close (outfd);
						}
						IniWriteInt (INI_FILE, "ResultLine", line);	// update the result line
					}
				}
				else if (format == ABCRU)	{	// Repunits, n is the only parameter.
					if (sscanf (buff+begline, "%lu", &n) != 1)
						continue;				// Skip invalid line
					sprintf (sgb, "10");
					sprintf (sgk, "1");
					if (rising_ns && !rising_ks && (n <= last_processed_n))
						continue;				// Skip already processed n's
					if ((rising_ns && !rising_ks) || (!rising_ns && rising_ks))
						fclose (fd);			// Unlock the file during the test...
					if (! process_num (format, "1", "10", n, -1, 0, &res))
						goto done;
					if (res) {
						resultline = IniGetInt(INI_FILE, "ResultLine", 0);
						outfd = _open (outputfile, _O_TEXT | _O_RDWR | _O_APPEND | _O_CREAT, 0666);
						if (outfd) {
							if (hline >= resultline) {	// write the relevant header
								writelg = _write (outfd, hbuff, strlen (hbuff));
							}
							sprintf (outbuf, "%lu\n", n); 
							writelg = _write (outfd, outbuf, strlen (outbuf));
							_close (outfd);
						}
						IniWriteInt (INI_FILE, "ResultLine", line);	// update the result line
					}
				}
 				else if (format == ABCDP)	{	// DivPhi, k, b, n specified on each input line
 					if (sscanf (buff+begline, "%s %s %lu", sgk, sgb, &n) != 3)
 						continue;				// Skip invalid line
 					if (!isDigitString(sgk))
 						continue;				// Skip invalid line
 					if (!isDigitString (sgb))
 						continue;				// Skip invalid line
 					incr = 1;
 					if (! process_num (format, sgk, sgb, n, 1, 0, &res))
 						goto done;
 				}
				else if (format == ABCGRU)	{	// Generalized Repunits, b, n, are the two parameters
					if (sscanf (buff+begline, "%s %lu", sgb, &n) != 2)
						continue;				// Skip invalid line
					if (!isDigitString (sgb))
						continue;				// Skip invalid line
					sprintf (sgk, "1");
					if (rising_ns && !rising_ks && (n <= last_processed_n))
						continue;				// Skip already processed n's
					if ((rising_ns && !rising_ks) || (!rising_ns && rising_ks))
						fclose (fd);			// Unlock the file during the test...
					if (! process_num (format, "1", sgb, n, -1, 0, &res))
						goto done;
					if (res) {
						resultline = IniGetInt(INI_FILE, "ResultLine", 0);
						outfd = _open (outputfile, _O_TEXT | _O_RDWR | _O_APPEND | _O_CREAT, 0666);
						if (outfd) {
							if (hline >= resultline) {	// write the relevant header
								writelg = _write (outfd, hbuff, strlen (hbuff));
							}
							sprintf (outbuf, "%s %lu\n", sgb, n); 
							writelg = _write (outfd, outbuf, strlen (outbuf));
							_close (outfd);
						}
						IniWriteInt (INI_FILE, "ResultLine", line);	// update the result line
					}
				}
				else if (format == ABCGF)	{	// Generalized Fermat, sgb, n, are the two parameters
					if (sscanf (buff+begline, "%s %lu", sgb, &n) != 2)
						continue;				// Skip invalid line
					if (!isDigitString(sgb))
						continue;				// Skip invalid line
					if (!ispoweroftwo(n))
						continue;				// Skip invalid line
					sprintf (sgk, "1");
					if (! process_num (format, "1", sgb, n, 1, 0, &res))
						goto done;
//					if (!process_gf (sgb, n, &res))
//						goto done;				// Use special process here...
					if (res) {
						resultline = IniGetInt(INI_FILE, "ResultLine", 0);
						outfd = _open (outputfile, _O_TEXT | _O_RDWR | _O_APPEND | _O_CREAT, 0666);
						if (outfd) {
							if (hline >= resultline) {	// write the relevant header
								writelg = _write (outfd, hbuff, strlen (hbuff));
							}
							sprintf (outbuf, "%s %lu\n", sgb, n);	// write the result
							writelg = _write (outfd, outbuf, strlen (outbuf));
							_close (outfd);
						}
						IniWriteInt (INI_FILE, "ResultLine", line);	// update the result line
					}
				}
				else if (format == ABCDN)	{	// b^n-b^m+c numbers ; sgb, n, m are the three parameters
					if (sscanf (buff+begline, "%s %lu %lu", sgb, &n, &m) != 3)
						continue;				// Skip invalid line
					if (!isDigitString(sgb))
						continue;				// Skip invalid line
					if (n <= m)
						continue;				// Skip invalid line
					ndiff = n-m;				// Save difference of exponents in a global
					sprintf (sgk, "1");
					if (! process_num (format, "1", sgb, m, incr, 0, &res))
						goto done;
					if (res) {
						resultline = IniGetInt(INI_FILE, "ResultLine", 0);
						outfd = _open (outputfile, _O_TEXT | _O_RDWR | _O_APPEND | _O_CREAT, 0666);
						if (outfd) {
							if (hline >= resultline) {	// write the relevant header
								writelg = _write (outfd, hbuff, strlen (hbuff));
							}
							sprintf (outbuf, "%s %lu %lu\n", sgb, n, m);	// write the result
							writelg = _write (outfd, outbuf, strlen (outbuf));
							_close (outfd);
						}
						IniWriteInt (INI_FILE, "ResultLine", line);	// update the result line
					}
				}
				else if (format == ABCDNG)	{	// b^n-b^m+c numbers ; sgb, n, m, c are the four parameters
					if (sscanf (buff+begline, "%s %lu %lu %d", sgb, &n, &m, &incr) != 4)
						continue;				// Skip invalid line
					if (!isDigitString(sgb))
						continue;				// Skip invalid line
					if (n <= m)
						continue;				// Skip invalid line
					ndiff = n-m;				// Save difference of exponents in a global
					sprintf (sgk, "1");
					if (! process_num (format, "1", sgb, m, incr, 0, &res))
						goto done;
					if (res) {
						resultline = IniGetInt(INI_FILE, "ResultLine", 0);
						outfd = _open (outputfile, _O_TEXT | _O_RDWR | _O_APPEND | _O_CREAT, 0666);
						if (outfd) {
							if (hline >= resultline) {	// write the relevant header
								writelg = _write (outfd, hbuff, strlen (hbuff));
							}
							sprintf (outbuf, "%s %lu %lu %d\n", sgb, n, m, incr);	// write the result
							writelg = _write (outfd, outbuf, strlen (outbuf));
							_close (outfd);
						}
						IniWriteInt (INI_FILE, "ResultLine", line);	// update the result line
					}
				}
				else if (format == ABCVARAQS)	{	// k, b, n, c and d specified on each input line
					if (sscanf (buff+begline, "%s %s %lu %d %s", sgk, sgb, &n, &incr, sgd) != 5)
						continue;				// Skip invalid line
					if (!isDigitString(sgk))
						continue;				// Skip invalid line
					if (!isDigitString (sgb))
						continue;				// Skip invalid line
//					if (!isDigitString(sgd))
//						continue;				// Skip invalid line
					if (rising_ns && !rising_ks && (n <= last_processed_n))
						continue;				// Skip already processed n's
					if (rising_ks && !rising_ns && (digitstrcmp (sgk, last_processed_k) <= 0))
						continue;				// Skip already processed k's
					if ((rising_ns && !rising_ks) || (!rising_ns && rising_ks))
						fclose (fd);			// Unlock the file during the test...
					if (! process_num (format, sgk, sgb, n, incr, shift, &res))
						goto done;
					if (res) {
						resultline = IniGetInt(INI_FILE, "ResultLine", 0);
						outfd = _open (outputfile, _O_TEXT | _O_RDWR | _O_APPEND | _O_CREAT, 0666);
						if (outfd) {
							if (hline >= resultline) {	// write the relevant header
								writelg = _write (outfd, hbuff, strlen (hbuff));
							}
							sprintf (outbuf, "%s %s %lu %d %s\n", sgk, sgb, n, incr, sgd); 
							writelg = _write (outfd, outbuf, strlen (outbuf));
							_close (outfd);
						}
						IniWriteInt (INI_FILE, "ResultLine", line);	// update the result line
					}
				}
				else if (format == ABCGM)	{	// Gaussian Mersenne
					if ((nargs = sscanf (buff+begline, "%lu %lu %lu", &n, &facn, &facnp)) < 1)
						continue;				// Skip invalid line
					else if (nargs == 1)		// Not prefactored.
						facn = facnp = 0;
					else if (nargs == 2) {		// Second argument is how far already factored, in bits)
						if (!facfrom)
							facfrom = facn;
						facn = facnp = 0;
					}
					if (rising_ns && !rising_ks && (n <= last_processed_n))
						continue;				// Skip already processed n's
					sprintf (sgk, "2^%lu", (n+1)/2);
					sprintf (sgb, "2");
					if ((rising_ns && !rising_ks) || (!rising_ns && rising_ks))
						fclose (fd);			// Unlock the file during the test...
					if (! process_num (format, sgk, "2", n, +1, shift, &res))
						goto done;
#ifndef X86_64
					if (facto) {				// If factoring, print a job progress message every so often
						if (n/pdivisor-pquotient == 1) {
							sprintf (outbuf, "%lu candidates factored, %lu factors found, %lu remaining\n"
								, factored, eliminated, factored - eliminated);
							OutputBoth (outbuf);
							pquotient = n/pdivisor;
						}
						else if (n/pdivisor-pquotient > 1)
							pquotient = n/pdivisor;
					}
#endif
					if (res) {
						resultline = IniGetInt(INI_FILE, "ResultLine", 0);
						sign = (((n&7) == 3) || ((n&7) == 5))? 1 : 0;	// 1 if positive, 0 if negative
						outfd = _open (outputfile, _O_TEXT | _O_RDWR | _O_APPEND | _O_CREAT, 0666);
						if (outfd) {
#ifndef X86_64
							if (facto)
								if (n >= LOWFACTORLIMIT)
									sprintf (outbuf, "%lu %lu\n", n, facto); 
								else if (facfrom)
									sprintf (outbuf, "%lu %lu\n", n, facfrom); 
								else
									sprintf (outbuf, "%lu\n", n); 
							else if (res1 && res2)
#else
							if (res1 && res2)
#endif
								if (a)
									sprintf (outbuf, "%lu (GM(%lu) is Prime in Z+iZ and the norm of GQ(%lu) is %ld-PRP.)\n", n, n, n, a); 
								else
									sprintf (outbuf, "%lu (GM(%lu) and GQ(%lu) are Prime in Z+iZ.)\n", n, n, n); 
							else if (res1)
								sprintf (outbuf, "%lu (GM(%lu) is Prime in Z+iZ.)\n", n, n); 
							else
								if (a)
									sprintf (outbuf, "%lu (The norm of GQ(%lu) is %ld-PRP.)\n", n, n, a); 
								else
									sprintf (outbuf, "%lu (GQ(%lu) is Prime in Z+iZ.)\n", n, n); 
							if (hline >= resultline) {	// write the relevant header
								writelg = _write (outfd, hbuff, strlen (hbuff));
							}
							writelg = _write (outfd, outbuf, strlen (outbuf));
							_close (outfd);
						}
						if (res2) {
							if (sign) {
								outfdm = _open (outmf, _O_TEXT | _O_RDWR | _O_APPEND | _O_CREAT, 0666);
								if (outfdm) {
									sprintf (outbuf, "%lu\n", n); 
									writelg = _write (outfdm, outbuf, strlen (outbuf));
									_close (outfdm);
								}
							}
							else {
								outfdp = _open (outpf, _O_TEXT | _O_RDWR | _O_APPEND | _O_CREAT, 0666);
								if (outfdp) {
									sprintf (outbuf, "%lu\n", n); 
									writelg = _write (outfdp, outbuf, strlen (outbuf));
									_close (outfdp);
								}
							}
						}
						IniWriteInt (INI_FILE, "ResultLine", line);	// update the result line
					}
				}
				else if (format == ABCSP)	{	// SPRP test of (2^n+1)/3 numbers
					if ((nargs = sscanf (buff+begline, "%lu %lu", &n, &facn)) < 1)
						continue;				// Skip invalid line
					else if (nargs == 1)		// Not prefactored.
						facn = facnp = 0;
					else if (nargs == 2) {		// Second argument is how far already factored, in bits)
						if (!facfrom)
							facfrom = facn;
						facn = facnp = 0;
					}
					if (rising_ns && !rising_ks  && (n <= last_processed_n))
						continue;				// Skip already processed n's
					sprintf (sgk, "(2^%lu+1)/3", n);
					sprintf (sgb, "2");
					if ((rising_ns && !rising_ks) || (!rising_ns && rising_ks))
						fclose (fd);			// Unlock the file during the test...
					if (! process_num (format, sgk, "2", n, +1, shift, &res))
						goto done;
#ifndef X86_64
					if (facto) {				// If factoring, print a job progress message every so often
						if (n/pdivisor-pquotient == 1) {
								sprintf (outbuf, "%lu candidates factored, %lu factors found, %lu remaining\n"
									, factored, eliminated, factored - eliminated);
								OutputBoth (outbuf);
							pquotient = n/pdivisor;
						}
						else if (n/pdivisor-pquotient > 1)
							pquotient = n/pdivisor;
					}
#endif
					if (res) {
						resultline = IniGetInt(INI_FILE, "ResultLine", 0);
						outfd = _open (outputfile, _O_TEXT | _O_RDWR | _O_APPEND | _O_CREAT, 0666);
#ifndef X86_64
						if (facto)
							if (n >= LOWFACTORLIMIT)
								sprintf (outbuf, "%lu %lu\n", n, facto); 
							else if (facfrom)
								sprintf (outbuf, "%lu %lu\n", n, facfrom); 
							else
								sprintf (outbuf, "%lu\n", n); 
						else
							sprintf (outbuf, "%lu\n", n); 
#else
						sprintf (outbuf, "%lu\n", n); 
#endif
						if (outfd) {
							if (hline >= resultline) {	// write the relevant header
								writelg = _write (outfd, hbuff, strlen (hbuff));
							}
							writelg = _write (outfd, outbuf, strlen (outbuf));
							_close (outfd);
						}
						IniWriteInt (INI_FILE, "ResultLine", line);	// update the result line
					}
				}
				else if (format == ABCK) {							// Carol/Kynea
					if (sscanf (buff+begline, "%lu %d", &n, &incr) != 2)
						continue;						// Skip invalid line
					if (rising_ns && !rising_ks  && (n <= last_processed_n))
						continue;				// Skip already processed n's
					if (incr == 1) {
						format = ABCK;					// Kynea number
						sprintf (sgk, "(2^%lu+1)", n-1);
					}
					else if (incr == -1) {
						format = ABCC;					// Carol number
						sprintf (sgk, "(2^%lu-1)", n-1);
					}
					else
						continue;
					sprintf (sgb, "2");
					if ((rising_ns && !rising_ks) || (!rising_ns && rising_ks))
						fclose (fd);			// Unlock the file during the test...
					if (! process_num (format, sgk, "2", n+1, -1, shift, &res))
						goto done;
					if (res) {
						resultline = IniGetInt(INI_FILE, "ResultLine", 0);
						outfd = _open (outputfile, _O_TEXT | _O_RDWR | _O_APPEND | _O_CREAT, 0666);
						if (outfd) {
							if (hline >= resultline) {	// write the relevant header
								writelg = _write (outfd, hbuff, strlen (hbuff));
							}
							sprintf (outbuf, "%lu %d\n", n, incr); 
							writelg = _write (outfd, outbuf, strlen (outbuf));
							_close (outfd);
						}
						IniWriteInt (INI_FILE, "ResultLine", line);	// update the result line
					}
				}
			}				// End processing a data line
			if ((!rising_ns && !rising_ks) || (rising_ns && rising_ks))
				IniWriteInt (INI_FILE, "PgenLine", line + 1);		// Point on the next line
			if (rising_ns && !rising_ks) {
				IniWriteInt (INI_FILE, "Last_Processed_n", n);		// Point on the next n
				last_processed_n = n;
			}
			if (rising_ks && !rising_ns) {
				IniWriteString (INI_FILE, "Last_Processed_k", sgk); // Point on the next k
				strcpy (last_processed_k, sgk);
			}
			if(n>=(unsigned long)IniGetInt(INI_FILE, "MaxN", 2147483647)) {
				break;
			}
			if (res) {
				if(IniGetInt(INI_FILE, "BeepOnSuccess", 0)) {
					do {	// If stopping on this success, beep infinitely!
#if !defined(WIN32) || defined(_CONSOLE)
						flashWindowAndBeep (20);
#else
						flashWindowAndBeep (50);
#endif
					} while (!stopCheck () && IniGetInt(INI_FILE, "StopOnSuccess", 0));
				}
				if(IniGetInt(INI_FILE, "StopOnSuccess", 0)) {
					goto done;
				}
				else if(IniGetInt(INI_FILE, "StopOnPrimedK", 0)) {
					sprintf (outbuf, "ks%s", sgk);
					IniWriteInt (INI_FILE, outbuf, 1+IniGetInt(INI_FILE, outbuf, 0));
							// Increment this k success count
					save_IniFile (INI_FILE, SVINI_FILE);	// make a backup of INI_FILE
				}
				else if(IniGetInt(INI_FILE, "StopOnPrimedN", 0)) {
					sprintf (outbuf, "ns%lu", n);
					IniWriteInt (INI_FILE, outbuf, 1+IniGetInt(INI_FILE, outbuf, 0));
							// Increment this n success count
					save_IniFile (INI_FILE, SVINI_FILE);	// make a backup of INI_FILE
				}
				else if(IniGetInt(INI_FILE, "StopOnPrimedB", 0)) {
					sprintf (outbuf, "bs%s", sgb);
					IniWriteInt (INI_FILE, outbuf, 1+IniGetInt(INI_FILE, outbuf, 0));
							// Increment this base success count
					save_IniFile (INI_FILE, SVINI_FILE);	// make a backup of INI_FILE
				}
			}
			if ((rising_ns && !rising_ks) || (!rising_ns && rising_ks))
				goto OPENFILE;
		}					// End of loop on input lines
		IniWriteString (INI_FILE, "ResultLine", NULL);		// delete the result line
		_unlink (SVINI_FILE);								// delete the backup of INI_FILE
		completed = TRUE;
done:
		if(IniGetInt(INI_FILE, "StopOnSuccess", 0) && res) {
			if ((!rising_ns && !rising_ks) || (rising_ns && rising_ks))
				IniWriteInt (INI_FILE, "PgenLine", line + 1);		// Point on the next line
			if (rising_ns && !rising_ks)
				IniWriteInt (INI_FILE, "Last_Processed_n", n);		// Point on the next n
			if (rising_ks && !rising_ns)
				IniWriteString (INI_FILE, "Last_Processed_k", sgk); // Point on the next k
		}
		else if (!aborted && ((!rising_ns && !rising_ks) || (rising_ns && rising_ks)))
			IniWriteInt (INI_FILE, "PgenLine", line);		// Point again on the current line...
		if (facto) {
			sprintf (outbuf, "%lu candidates factored, %lu factors found, %lu remaining\n"
				, factored, eliminated, factored - eliminated);
			OutputBoth (outbuf);
		}
		if ((!rising_ns && !rising_ks) || (rising_ns && rising_ks))
			fclose (fd);
		IniWriteString(INI_FILE, "OldInputFile", inputfile);		// Save the just processed input file name.
	}						// End Work == 0

// Handle an expr

	else {					// Work != 0
		OutputStr ("Expression testing not yet implemented.\n");
		IniWriteInt (INI_FILE, "WorkDone", 1);
	}
	aborted = FALSE;
	return (completed);
}

