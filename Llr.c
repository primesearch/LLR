/*----------------------------------------------------------------------
| This file contains routines and global variables that are common for
| all operating systems the program has been ported to.  It is included
| in one of the source code files of each port.  See Llr.h for the
| common #defines and common routine definitions.
+---------------------------------------------------------------------*/

#define	FFTTRYMAX 0
 
#define CHECK_IF_ANY_ERROR(X,J,N,K) \
/* If the sum of the output values is an error (such as infinity) */\
/* then raise an error. */\
\
		if (gw_test_illegal_sumout ()) {\
			sprintf (buf, ERRMSG0L, J, N, ERRMSG1A);\
			OutputBoth (buf);\
			sleep5 = TRUE;\
			goto error;\
		}\
\
/* Check that the sum of the input numbers squared is approximately */\
/* equal to the sum of unfft results.  Since this check may not */\
/* be perfect, check for identical results after a restart. */\
\
		if (gw_test_mismatched_sums ()) {\
			static unsigned long last_bit[10] = {0};\
			static double last_suminp[10] = {0.0};\
			static double last_sumout[10] = {0.0};\
			double suminp, sumout;\
			suminp = gwsuminp (X);\
			sumout = gwsumout (X);\
			if (J == last_bit[K] &&\
			    suminp == last_suminp[K] &&\
			    sumout == last_sumout[K]) {\
				writeResults (ERROK);\
				saving = 1;\
			} else {\
				char	msg[80], sic[80], soc[80];\
				sprintf (sic, "%.16g", suminp);\
				sprintf (soc, "%.16g", sumout);\
				if (strcmp(sic, soc)) {\
				    sprintf (msg, ERRMSG1B, suminp, sumout);\
				    sprintf (buf, ERRMSG0L, J, N, msg);\
				    if (nextffttaken >= FFTTRYMAX)	OutputBoth (buf);\
				    last_bit[K] = J;\
				    last_suminp[K] = suminp;\
				    last_sumout[K] = sumout;\
					sleep5 = TRUE;\
				    goto trynextfft;\
				}\
			}\
		}\
\
/* Check for excessive roundoff error  */\
\
		if (MAXERR > 0.40) {\
			static unsigned long last_bit[10] = {0};\
			static double last_maxerr[10] = {0.0};\
			if (J == last_bit[K] &&\
			    MAXERR == last_maxerr[K]) {\
				OutputBoth (ERROK);\
				GWERROR = 0;\
				if (K != 6) {\
					saving = 1;\
				}\
				else {\
					OutputBoth (ERRMSG6);\
					maxerr_recovery_mode = 1;\
					sleep5 = FALSE;\
					goto error;\
				}\
			} else {\
				char	msg[80];\
				sprintf (msg, ERRMSG1C, MAXERR);\
				sprintf (buf, ERRMSG0L, J, N, msg);\
				OutputBoth (buf);\
				maxerr_point = last_bit[K] = J;\
				last_maxerr[K] = MAXERR;\
				sleep5 = FALSE;\
				goto error;\
			}\
		}\
\
		if (ERRCHK) {\
			if (MAXERR < reallyminerr && J > 30)\
				reallyminerr = MAXERR;\
			if (MAXERR > reallymaxerr)\
				reallymaxerr = MAXERR;\
		} 

// Some ABC format strings

char ckstring[] = "(2^$a$b)^2-2";	// Carol/Kynea
char cwstring[] = "$a*$b^$a$c";		// Cullen/Woodall
char ffstring[] = "$a*2^$b+1";		// FermFact output
char gmstring[] = "4^$a+1";		// Gaussian Mersenne norm

// Fixed k forms for k*b^n+/-1
char fkpstring[] = "%d*$a^$b+%d";
char fkmstring[] = "%d*$a^$b-%d";
char fkastring[]  = "%d*$a^$b$c";

// Fixed b forms for k*b^n+/-1
char fbpstring[]  = "$a*%d^$b+%d";
char fbmstring[]  = "$a*%d^$b-%d";
char fbastring[] = "$a*%d^$b$c";

// Fixed n forms for k*b^n+/-1
char fnpstring[] = "$a*$b^%d+%d";
char fnmstring[] = "$a*$b^%d-%d";
char fnastring[]  = "$a*$b^%d$c";

// Any form of k*b^n+/-1
char abcpstring[]  = "$a*$b^$c+%d";
char abcmstring[]  = "$a*$b^$c-%d";
char abcastring[] = "$a*$b^$c$d";

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

char JUNK[]="Copyright 1996-2002 Just For Fun Software, All rights reserved";
char	INI_FILE[80] = {0};
char	RESFILE[80] = {0};
char	LOGFILE[80] = {0};
char	EXTENSION[8] = {0};
char	username[100] = {0};

int volatile ERRCHK = 0;
unsigned int PRIORITY = 1;
unsigned int CPU_AFFINITY = 99;
EXTERNC unsigned int volatile CPU_TYPE = 0;
EXTERNC int ZERO_PADDED_FFT;
unsigned long volatile ITER_OUTPUT = 0;
unsigned long volatile ITER_OUTPUT_RES = 99999999;
unsigned long volatile DISK_WRITE_TIME = 30;
int	TWO_BACKUP_FILES = 1;
int	RUN_ON_BATTERY = 1;
int	TRAY_ICON = TRUE;
int	HIDE_ICON = FALSE;
unsigned int PRECISION = 2;
int	CUMULATIVE_TIMING = 0;
int	HIGH_RES_TIMER = 0; 

extern double MAXDIFF;
double maxdiffmult = 1.0;

/* PRP and LLR global variables */

#define	sgkbufsize 20000

giant	N = NULL;		/* Number being tested */
giant	NP = NULL;		/* Number being tested */
giant	M = NULL;		/* Gaussian Mersenne modulus = N*NP */
giant	gk = NULL;		/* k multiplier */

unsigned long Nlen = 0;	/* Bit length of number being LLRed or PRPed */
unsigned long klen = 0;	/* Number of bits of k multiplier */

/* Global variables for factoring */

EXTERNC unsigned long FACPASS = 0;
EXTERNC unsigned long FACTEST = 0;
EXTERNC unsigned long FACHSW = 0;	/* High word of found factor */
EXTERNC unsigned long FACMSW = 0;	/* Middle word of found factor */
EXTERNC unsigned long FACLSW = 0;	/* Low word of found factor */
EXTERNC void *FACDATA = NULL;		/* factor data is kept in a global */

/* Other gwnum globals */

EXTERNC gwnum	GW_RANDOM;			/* A random number used in */
									/* gwsquare_carefully and gwmul_carefully */
EXTERNC double ADDIN_VALUE;

EXTERNC void gw_random_number (gwnum);/* Generate random FFT data */

giant testn, testnp, testf, testx;
unsigned long facn = 0, facnp = 0;
int resn = 0, resnp = 0;
int k_is_square = 0;
char facnstr[80], facnpstr[80];

void setupf();
int factor64();
void* aligned_malloc (unsigned long, unsigned long);
void  aligned_free (void *);

static unsigned long mask;

unsigned int skiptest = 0;
unsigned int verbose = 0;
unsigned int abonillsum = 0;
unsigned int abonmismatch = 0;
unsigned int testgm = 0;
unsigned int testgq = 0;
unsigned int testfac = 0;
unsigned int abonroundoff = 0;
unsigned int sleep5 = 0;
unsigned int maxerr_recovery_mode = 0;
unsigned int maxerr_point = 0;
unsigned long interimFiles, interimResidues, throttle, facfrom, facto;
unsigned long factored = 0, eliminated = 0;
unsigned long pdivisor = 1000000, pquotient = 1;

double smargin = 0.0;

int is_valid_double(double);
int genProthBase(giant, unsigned long);

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

/* Sleep five minutes before restarting */

char ERROK[] = "Disregard last error.  Result is reproducible and thus not a hardware problem.\n";
char ERRMSG0L[] = "Iter: %ld/%ld, %s";
char ERRMSG0[] = "Bit: %ld/%ld, %s"; 
char ERRMSG1A[] = "ERROR: ILLEGAL SUMOUT\n";
char ERRMSG1B[] = "ERROR: SUM(INPUTS) != SUM(OUTPUTS), %.16g != %.16g\n";
char ERRMSG1C[] = "ERROR: ROUND OFF (%.10g) > 0.40\n";
char ERRMSG2[] = "Possible hardware failure, consult the readme file.\n";
char ERRMSG3[] = "Continuing from last save file.\n";
char ERRMSG4[] = "Waiting five minutes before restarting.\n";
char ERRMSG5[] = "Fatal Error, test of %s aborted\n";
char ERRMSG6[] = "For added safety, redoing iteration using a slower, more reliable method.\n";
char WRITEFILEERR[] = "Error writing intermediate file: %s\n";

void	trace(int n) {			// Debugging tool...
	char buf[100];
	sprintf(buf, "OK until number %d\n", n);
	OutputBoth (buf); 	
}

void clearline (int size) {
	char buf[256];
	int i;

	for (i=0; i<256; i++)
		buf[i] = '\0';
	for (i=0; i<size; i++)
		buf[i] = ' ';
	buf[size-1] = '\r';
	OutputStr(buf);
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

double timers[10];			/* Up to ten separate timers */

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
	if (HIGH_RES_TIMER) { 
		timers[i] -= getHighResTimer (); 
	} else { 
		struct _timeb timeval; 
		_ftime (&timeval); 
		timers[i] -= (double) timeval.time * 1000.0 + timeval.millitm; 
	} 
} 
 
void end_timer ( 
	int	i) 
{ 
	if (HIGH_RES_TIMER) { 
		timers[i] += getHighResTimer (); 
	} else { 
		struct _timeb timeval; 
		_ftime (&timeval); 
		timers[i] += (double) timeval.time * 1000.0 + timeval.millitm; 
	} 
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
	else 
		return (timers[i] / 1000.0); 
} 
 
#define TIMER_NL	0x1
#define TIMER_CLR	0x2
#define TIMER_OPT_CLR	0x4

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

		time (&this_time);
		strcpy (tbuf, ctime (&this_time)+4);
		tbuf[12] = 0;
		sprintf (buf, "[%s] ", tbuf);
		OutputStr (buf);
}

void OutputTimeStampOnDisk ()
{
	time_t	this_time;
	char	buf[40];

		time (&this_time);
		strftime (buf, 40, "[%Y-%m-%d %H:%M:%S]\n", localtime(&this_time));
		writeResults (buf);
}

/* Determine the CPU speed either empirically or by user overrides. */ 
/* getCpuType must be called prior to calling this routine. */ 
 
void getCpuSpeed (void) 
{ 
	int	temp; 
 
/* Guess the CPU speed using the RDTSC instruction */ 
 
	guessCpuSpeed (); 

/* Now let the user override the cpu speed from the local.ini file */ 
 
	if (IniGetInt (INI_FILE, "CpuOverride", 0)) { 
		temp = IniGetInt (INI_FILE, "CpuSpeed", 99); 
		if (temp != 99) CPU_SPEED = temp; 
	} 
 
/* Make sure the cpu speed is reasonable */ 
 
	if (CPU_SPEED > 50000) CPU_SPEED = 50000; 
	if (CPU_SPEED < 25) CPU_SPEED = 25; 
} 
 
/* Set the CPU flags based on the CPUID data.  Also, the */ 
/* advanced user can override our guesses. */ 
 
unsigned int SAVE_CPU_FLAGS;

void getCpuInfo (void) 
{ 
	int	temp; 
 
/* Get the CPU info using CPUID instruction */ 
 
	guessCpuType (); 

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
	temp = IniGetInt (INI_FILE, "CpuSupportsSSE", 99); 
	if (temp == 0) CPU_FLAGS &= ~CPU_SSE; 
	if (temp == 1) CPU_FLAGS |= CPU_SSE; 
	temp = IniGetInt (INI_FILE, "CpuSupportsSSE2", 99); 
	if (temp == 0) CPU_FLAGS &= ~CPU_SSE2; 
	if (temp == 1) CPU_FLAGS |= CPU_SSE2; 
 
SAVE_CPU_FLAGS = CPU_FLAGS;		// Duplicate these values

/* Now get the CPU speed */ 
 
	getCpuSpeed (); 
} 
 
/* Format a long or very long textual cpu description */ 
 
void getCpuDescription ( 
	char	*buf,			/* A 512 byte buffer */ 
	int	long_desc)		/* True for a very long description */ 
{ 
 
/* Recalculate the CPU speed in case speed step has changed the original */ 
/* settings. */ 
 
	getCpuSpeed (); 
 
/* Now format a pretty CPU description */ 
 
	sprintf (buf, "%s\nCPU speed: %.2f MHz\n", CPU_BRAND, CPU_SPEED); 
	if (CPU_FLAGS) { 
		strcat (buf, "CPU features: "); 
		if (CPU_FLAGS & CPU_RDTSC) strcat (buf, "RDTSC, "); 
		if (CPU_FLAGS & CPU_CMOV) strcat (buf, "CMOV, "); 
		if (CPU_FLAGS & CPU_PREFETCH) strcat (buf, "PREFETCH, "); 
		if (CPU_FLAGS & CPU_MMX) strcat (buf, "MMX, "); 
		if (CPU_FLAGS & CPU_SSE) strcat (buf, "SSE, "); 
		if (CPU_FLAGS & CPU_SSE2) strcat (buf, "SSE2, "); 
		strcpy (buf + strlen (buf) - 2, "\n"); 
	} 
	strcat (buf, "L1 cache size: "); 
	if (CPU_L1_CACHE_SIZE < 0) strcat (buf, "unknown\n"); 
	else sprintf (buf + strlen (buf), "%d KB\n", CPU_L1_CACHE_SIZE); 
	strcat (buf, "L2 cache size: "); 
	if (CPU_L2_CACHE_SIZE < 0) strcat (buf, "unknown\n"); 
	else sprintf (buf + strlen (buf), "%d KB\n", CPU_L2_CACHE_SIZE); 
	if (! long_desc) return; 
	strcat (buf, "L1 cache line size: "); 
	if (CPU_L1_CACHE_LINE_SIZE < 0) strcat (buf, "unknown\n"); 
	else sprintf (buf+strlen(buf), "%d bytes\n", CPU_L1_CACHE_LINE_SIZE); 
	strcat (buf, "L2 cache line size: "); 
	if (CPU_L2_CACHE_LINE_SIZE < 0) strcat (buf, "unknown\n"); 
	else sprintf (buf+strlen(buf), "%d bytes\n", CPU_L2_CACHE_LINE_SIZE); 
	if (CPU_L1_DATA_TLBS > 0) 
		sprintf (buf + strlen (buf), "L1 TLBS: %d\n", CPU_L1_DATA_TLBS); 
	if (CPU_L2_DATA_TLBS > 0) 
		sprintf (buf + strlen (buf), "%sTLBS: %d\n", 
			 CPU_L1_DATA_TLBS > 0 ? "L2 " : "", 
			 CPU_L2_DATA_TLBS); 
} 
 
/* Determine if a number is prime */

int isPrime (
	unsigned long p)
{
	unsigned long i;
	for (i = 2; i * i <= p; i = (i + 1) | 1)
		if (p % i == 0) return (FALSE);
	return (TRUE);
}

/* Determine the names of the INI files */

void nameIniFiles (
	int	named_ini_files)
{
	char	buf[120];

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

/* Let the user rename these files and pick a different working directory */

	IniGetString (INI_FILE, "WorkingDir", buf, sizeof(buf), NULL);
	IniGetString (INI_FILE, "results.txt", RESFILE, 80, RESFILE);
	IniGetString (INI_FILE, "prime.log", LOGFILE, 80, LOGFILE);
	IniGetString (INI_FILE, "prime.ini", INI_FILE, 80, INI_FILE);
	if (buf[0]) {
		_chdir (buf);
		IniFileOpen (INI_FILE, 0);
	}
}

/* Read the INI files */

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
 
/* Guess the CPU type if it isn't known.  Otherwise, validate it. */ 
 
	getCpuInfo (); 
 
/* Other oddball options */ 
 
	CUMULATIVE_TIMING = IniGetInt (INI_FILE, "CumulativeTiming", 0); 
	HIGH_RES_TIMER = isHighResTimerAvailable (); 
} 
 
/*----------------------------------------------------------------------
| Portable routines to read and write ini files!  NOTE:  These only
| work if you open no more than 5 ini files.  Also you must not
| change the working directory at any time during program execution.
+---------------------------------------------------------------------*/

struct IniLine {
	char	*keyword;
	char	*value;
	int	active;
};
struct IniCache {
	char	*filename;
	int	immediate_writes;
	int	dirty;
	unsigned int num_lines;
	unsigned int array_size;
	struct IniLine **lines;
};

void growIniLineArray (
	struct IniCache *p)
{
	struct IniLine **newlines;

	if (p->num_lines != p->array_size) return;

	newlines = (struct IniLine **)
		malloc ((p->num_lines + 100) * sizeof (struct IniLine **));
	if (p->num_lines) {
		memcpy (newlines, p->lines, p->num_lines * sizeof (struct IniLine *));
		free (p->lines);
	}
	p->lines = newlines;
	p->array_size = p->num_lines + 100;
}

struct IniCache *openIniFile (
	char	*filename,
	int	forced_read)
{
static	struct IniCache *cache[10] = {0};
	struct IniCache *p;
	FILE	*fd;
	unsigned int i;
	char	line[80];
	char	*val;

/* See if file is cached */

	for (i = 0; i < 10; i++) {
		p = cache[i];
		if (p == NULL) {
			p = (struct IniCache *) malloc (sizeof (struct IniCache));
			p->filename = (char *) malloc (strlen (filename) + 1);
			strcpy (p->filename, filename);
			p->immediate_writes = 1;
			p->dirty = 0;
			p->num_lines = 0;
			p->array_size = 0;
			p->lines = NULL;
			forced_read = 1;
			cache[i] = p;
			break;
		}
		if (strcmp (filename, p->filename) == 0)
			break;
	}

/* Skip reading the ini file if appropriate */

	if (!forced_read) return (p);
	if (p->dirty) return (p);

/* Free the data if we've already read some in */

	for (i = 0; i < p->num_lines; i++) {
		free (p->lines[i]->keyword);
		free (p->lines[i]->value);
		free (p->lines[i]);
	}
	p->num_lines = 0;

/* Read the IniFile */
	
	fd = fopen (filename, "r");
	if (fd == NULL) return (p);

	while (fgets (line, 80, fd)) {
		if (line[strlen(line)-1] == '\n') line[strlen(line)-1] = 0;
		if (line[0] == 0) continue;
		if (line[strlen(line)-1] == '\r') line[strlen(line)-1] = 0;
		if (line[0] == 0) continue;

		val = strchr (line, '=');
		if (val == NULL) {
			char	buf[130];
			sprintf (buf, "Illegal line in INI file: %s\n", line);
			OutputSomewhere (buf);
			continue;
		}
		*val++ = 0;

		growIniLineArray (p);
		
/* Allocate and fill in a new line structure */

		i = p->num_lines++;
		p->lines[i] = (struct IniLine *) malloc (sizeof (struct IniLine));
		p->lines[i]->keyword = (char *) malloc (strlen (line) + 1);
		p->lines[i]->value = (char *) malloc (strlen (val) + 1);
		p->lines[i]->active = TRUE;
		strcpy (p->lines[i]->keyword, line);
		strcpy (p->lines[i]->value, val);
	}
	fclose (fd);

	return (p);
}

void writeIniFile (
	struct IniCache *p)
{
	int	fd;
	unsigned int j;
	char	buf[100];

/* Delay writing the file unless this INI file is written */
/* to immediately */

	if (!p->immediate_writes) {
		p->dirty = 1;
		return;
	}

/* Create and write out the INI file */

	fd = _open (p->filename, _O_CREAT | _O_TRUNC | _O_WRONLY | _O_TEXT, 0666);
	if (fd < 0) return;
	for (j = 0; j < p->num_lines; j++) {
		strcpy (buf, p->lines[j]->keyword);
		strcat (buf, "=");
		strcat (buf, p->lines[j]->value);
		strcat (buf, "\n");
		_write (fd, buf, strlen (buf));
	}
	p->dirty = 0;
	_close (fd);
}

void truncated_strcpy (
	char	*buf,
	unsigned int bufsize,
	char	*val)
{
	if (strlen (val) >= bufsize) {
		memcpy (buf, val, bufsize-1);
		buf[bufsize-1] = 0;
	} else {
		strcpy (buf, val);
	}
}

void IniGetString (
	char	*filename,
	char	*keyword,
	char	*val,
	unsigned int val_bufsize,
	char	*default_val)
{
	struct IniCache *p;
	unsigned int i;

/* Open ini file */

	p = openIniFile (filename, 1);

/* Look for the keyword */

	for (i = 0; ; i++) {
		if (i == p->num_lines) {
			if (default_val == NULL) {
				val[0] = 0;
			} else {
				truncated_strcpy (val, val_bufsize, default_val);
			}
			return;
		}
		if (p->lines[i]->active &&
		    stricmp (keyword, p->lines[i]->keyword) == 0) break;
	}

/* Copy info from the line structure to the user buffers */

	truncated_strcpy (val, val_bufsize, p->lines[i]->value);
}

long IniGetInt (
	char	*filename,
	char	*keyword,
	long	default_val)
{
	char	buf[20], defval[20];
	sprintf (defval, "%ld", default_val);
	IniGetString (filename, keyword, buf, 20, defval);
	return (atol (buf));
}

void IniWriteString (
	char	*filename,
	char	*keyword,
	char	*val)
{
	struct IniCache *p;
	unsigned int i, j;

/* Open ini file */
	p = openIniFile (filename, 1);
/* Look for the keyword */

	for (i = 0; ; i++) {
		if (i == p->num_lines ||
		    stricmp (p->lines[i]->keyword, "Time") == 0) {

/* Ignore request if we are deleting line */

			if (val == NULL) return;

/* Make sure the line array has room for the new line */

			growIniLineArray (p);

/* Shuffle entries down to make room for this entry */

			for (j = p->num_lines; j > i; j--)
				p->lines[j] = p->lines[j-1];

/* Allocate and fill in a new line structure */

			p->lines[i] = (struct IniLine *) malloc (sizeof (struct IniLine));
			p->lines[i]->keyword = (char *) malloc (strlen (keyword) + 1);
			strcpy (p->lines[i]->keyword, keyword);
			p->lines[i]->value = NULL;
			p->num_lines++;
			break;
		}
		if (p->lines[i]->active &&
		    stricmp (keyword, p->lines[i]->keyword) == 0) {
			if (val != NULL && strcmp (val, p->lines[i]->value) == 0) return;
			break;
		}
	}
/* Delete the line if requested */

	if (val == NULL) {
		IniDeleteLine (filename, i+1);
		return;
	}
/* Replace the value associated with the keyword */

	free (p->lines[i]->value);
	p->lines[i]->value = (char *) malloc (strlen (val) + 1);
	strcpy (p->lines[i]->value, val);

/* Write the INI file back to disk */
	writeIniFile (p);
}

void IniWriteInt (
	char	*filename,
	char	*keyword,
	long	val)
{
	char	buf[20];
	sprintf (buf, "%ld", val);
	IniWriteString (filename, keyword, buf);
}

void IniFileOpen (
	char	*filename,
	int	immediate_writes)
{
	struct IniCache *p;
	p = openIniFile (filename, 1);
	p->immediate_writes = immediate_writes;
}

void IniFileClose (
	char	*filename)
{
	struct IniCache *p;
	p = openIniFile (filename, 0);
	if (p->dirty) {
		p->immediate_writes = 1;
		writeIniFile (p);
		p->immediate_writes = 0;
	}
}

int IniFileWritable (
	char	*filename)
{
	struct IniCache *p;
	int	fd;
	unsigned int j;
	char	buf[100];

/* Create and write out the INI file */

	p = openIniFile (filename, 0);
	fd = _open (p->filename, _O_CREAT | _O_TRUNC | _O_WRONLY | _O_TEXT, 0666);
	if (fd < 0) return (FALSE);
	for (j = 0; j < p->num_lines; j++) {
		strcpy (buf, p->lines[j]->keyword);
		strcat (buf, "=");
		strcat (buf, p->lines[j]->value);
		strcat (buf, "\n");
		if (_write (fd, buf, strlen (buf)) != (int) strlen (buf)) {
			_close (fd);
			return (FALSE);
		}
	}
	if (p->num_lines == 0) {
		if (_write (fd, "DummyLine=XXX\n", 14) != 14) {
			_close (fd);
			return (FALSE);
		}
		p->dirty = 1;
	}
	_close (fd);
	return (TRUE);
}

unsigned int IniGetNumLines (
	char	*filename)
{
	struct IniCache *p;
	p = openIniFile (filename, 0);
	return (p->num_lines);
}

void IniGetLineAsString (
	char	*filename,
	unsigned int line,
	char	*keyword,
	unsigned int keyword_bufsize,
	char	*val,
	unsigned int val_bufsize)
{
	struct IniCache *p;

/* Open ini file */

	p = openIniFile (filename, 0);

/* Copy info from the line structure to the user buffers */

	truncated_strcpy (keyword, keyword_bufsize, p->lines[line-1]->keyword);
	truncated_strcpy (val, val_bufsize, p->lines[line-1]->value);
}

void IniGetLineAsInt (
	char	*filename,
	unsigned int line,
	char	*keyword,
	unsigned int keyword_bufsize,
	long	*val)
{
	char	buf[20];
	IniGetLineAsString (filename, line, keyword, keyword_bufsize, buf, 20);
	*val = atol (buf);
}

void IniReplaceLineAsString (
	char	*filename,
	unsigned int line,
	char	*keyword,
	char	*val)
{
	IniDeleteLine (filename, line);
	IniInsertLineAsString (filename, line, keyword, val);
}

void IniReplaceLineAsInt (
	char	*filename,
	unsigned int line,
	char	*keyword,
	long	val)
{
	char	buf[20];
	sprintf (buf, "%ld", val);
	IniReplaceLineAsString (filename, line, keyword, buf);
}

void IniInsertLineAsString (
	char	*filename,
	unsigned int line,
	char	*keyword,
	char	*val)
{
	struct IniCache *p;
	unsigned int i;

/* Open ini file, do not reread it as that could change the line numbers! */

	p = openIniFile (filename, 0);

/* Adjust line number if it doesn't make sense */

	if (line == 0) line = 1;
	if (line > p->num_lines+1) line = p->num_lines+1;

/* Make sure the line array has room for the new line */

	growIniLineArray (p);

/* Shuffle lines down in the array to make room for the new line */

	for (i = p->num_lines; i >= line; i--) p->lines[i] = p->lines[i-1];
	p->num_lines++;

/* Allocate and fill in a new line structure */

	p->lines[line-1] = (struct IniLine *) malloc (sizeof (struct IniLine));
	p->lines[line-1]->keyword = (char *) malloc (strlen (keyword) + 1);
	p->lines[line-1]->value = (char *) malloc (strlen (val) + 1);
	p->lines[line-1]->active = TRUE;
	strcpy (p->lines[line-1]->keyword, keyword);
	strcpy (p->lines[line-1]->value, val);

/* Write the INI file back to disk */

	writeIniFile (p);
}

void IniInsertLineAsInt (
	char	*filename,
	unsigned int line,
	char	*keyword,
	long	val)
{
	char	buf[20];
	sprintf (buf, "%ld", val);
	IniInsertLineAsString (filename, line, keyword, buf);
}

void IniAppendLineAsString (
	char	*filename,
	char	*keyword,
	char	*val)
{
	struct IniCache *p;
	p = openIniFile (filename, 0);
	IniInsertLineAsString (filename, p->num_lines+1, keyword, val);
}

void IniAppendLineAsInt (
	char	*filename,
	char	*keyword,
	long	val)
{
	char	buf[20];
	sprintf (buf, "%ld", val);
	IniAppendLineAsString (filename, keyword, buf);
}

void IniDeleteLine (
	char	*filename,
	unsigned int line)
{
	struct IniCache *p;
	unsigned int i;

/* Open ini file, do not reread it as that could change the line numbers! */

	p = openIniFile (filename, 0);
	if (line == 0 || line > p->num_lines) return;

/* Free the data associated with the given line */

	free (p->lines[line-1]->keyword);
	free (p->lines[line-1]->value);
	free (p->lines[line-1]);

/* Delete the line from the lines array */

	for (i = line; i < p->num_lines; i++) p->lines[i-1] = p->lines[i];
	p->num_lines--;

/* Write the INI file back to disk */

	writeIniFile (p);
}

void IniDeleteAllLines (
	char	*filename)
{
	struct IniCache *p;
	unsigned int i;

/* Open ini file! */

	p = openIniFile (filename, 0);

/* Free the data associated with the given line */

	for (i = 0; i < p->num_lines; i++) {
		free (p->lines[i]->keyword);
		free (p->lines[i]->value);
		free (p->lines[i]);
	}
	p->num_lines = 0;

/* Write the INI file back to disk */

	writeIniFile (p);
}

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
			sprintf (buf+25, " - ver %s]\n", VERSION);
			_write (fd, buf, strlen (buf));
		}

/* Output the message */

		_write (fd, str, strlen (str));
	}

/* Display message about full log file */
	
	else {
		char	*fullmsg = "Prime.log file full.  Please delete it.\n";
		OutputStr (fullmsg);
		if (filelen < 251000)
			_write (fd, fullmsg, strlen (fullmsg));
	}
	_close (fd);
}

int gmodi (unsigned long, giant);
void uldivg (unsigned long, giant);


/* Generate temporary file name */

void tempFileName ( 
	char	*buf, char c) 
{ 
	int remainder;
 
	remainder = gmodi(19999981, N);
	sprintf (buf, "%01c%07li", c, remainder % 10000000); 
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
		if (verbose)
			_write (fd, buf, 27);
	}

/* Output the message */

	if (_write (fd, msg, strlen (msg)) < 0) goto fail;
	_close (fd);
	return (TRUE);

/* On a write error, close file and return error flag */

fail:	_close (fd);
	return (FALSE);
}


/* Read and write intermediate results to a file */

int read_gwnum (
	int	fd,
	gwnum	g,
	long	*sum)
{
	giant	tmp;
	long	i, len, bytes;

	tmp = popg ((Nlen >> 5) + 10);
	if (_read (fd, &len, sizeof (long)) != sizeof (long)) return (FALSE);
	bytes = len * sizeof (long);
	if (_read (fd, tmp->n, bytes) != bytes) return (FALSE);
	tmp->sign = len;
	*sum += len;
	for (i = 0; i < len; i++) *sum += tmp->n[i];
	gianttogw (tmp, g);
	pushg (1);
	return (TRUE);
}

int write_gwnum (
	int	fd,
	gwnum	g,
	long	*sum)
{
	giant	tmp;
	long	i, len, bytes;

	tmp = popg ((Nlen >> 5) + 10);
	gwtogiant (g, tmp);
	len = tmp->sign;
	if (_write (fd, &len, sizeof (long)) != sizeof (long)) return (FALSE);
	bytes = len * sizeof (long);
	if (_write (fd, tmp->n, bytes) != bytes) return (FALSE);
	*sum += len;
	for (i = 0; i < len; i++) *sum += tmp->n[i];
	pushg (1);
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

int writeToFile (
	char	*filename,
	unsigned long j,
	gwnum	x,
	gwnum	y)
{
	char	newfilename[16];
	int	fd;
	unsigned long magicnum, version;
	long	sum = 0;

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

	if (! write_gwnum (fd, x, &sum)) goto writeerr;
	if (y != NULL && ! write_gwnum (fd, y, &sum)) goto writeerr; 

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
	char	*filename,
	unsigned long *j,
	gwnum	x,
	gwnum	y)
{
	int	fd;
	unsigned long magicnum, version;
	long	sum = 0, i;

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

	if (! read_gwnum (fd, x, &sum)) goto readerr;
	if (y != NULL && ! read_gwnum (fd, y, &sum)) goto readerr; 

/* Read and compare the checksum */

	if (_read (fd, &i, sizeof (long)) != sizeof (long)) goto readerr;
	if (i != sum) goto readerr;
	_close (fd);
	return (TRUE);

/* An error occured.  Delete the current intermediate file. */
/* Set stage to -1 to indicate an error. */

readerr:
	OutputStr ("Error reading LLR or PRP save file.\n");
	_close (fd);
error:
	_unlink (filename);
	return (FALSE);
}


int gmwriteToFile (
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
	long	sum = 0;

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

	if (! write_gwnum (fd, x, &sum)) goto writeerr;
	if (y != NULL && ! write_gwnum (fd, y, &sum)) goto writeerr; 

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

	if (! read_gwnum (fd, x, &sum)) goto readerr;
	if (y != NULL && ! read_gwnum (fd, y, &sum)) goto readerr; 

/* Read and compare the checksum */

	if (_read (fd, &i, sizeof (long)) != sizeof (long)) goto readerr;
	if (i != sum) goto readerr;
	_close (fd);
	return (TRUE);

/* An error occured.  Delete the current intermediate file. */
/* Set stage to -1 to indicate an error. */

readerr:
	OutputStr ("Error reading LLR or PRP save file.\n");
	_close (fd);
error:
	_unlink (filename);
	return (FALSE);
}

char res64[17]; /* VP : This variable has been made global */

/* Test if a gwnum is zero */

int
gwiszero(
	gwnum 	gg,
	int 	N
)
{
	int 	j, err_code;
	long	val;

	for(j=0; j<N; ++j)
	{
		err_code = get_fft_value (gg, j, &val);
		if (err_code) return (err_code);
		if (val)
			return 0;
	}
	return 1;
}

/* Set a gwnum to zero */

void gwzero (gwnum gg, int N) {
	int j;
	for(j=0; j<N; ++j)
		set_fft_value (gg, j, 0);
	return;
}

/* Print some words of a gwnum */

int
gwprint(
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
		err_code = get_fft_value (gg, j, &val);
		if (err_code) return (err_code);
		sprintf (buf, "%d ", val);
		if (val)
			OutputStr (buf);
	}
	OutputStr ("\n");
	return 0;
}

#define gw_add()	(*GWPROCPTRS[4])()

#define gw_sub()	(*GWPROCPTRS[6])()

void gwaddn3 (			/* Add two numbers normalizing always */
	gwnum	s1,		/* Source #1 */
	gwnum	s2,		/* Source #2 */
	gwnum	d)		/* Destination */
{

	ASSERTG (((unsigned long *) s1)[-1] >= 1);
	ASSERTG (((unsigned long *) s2)[-1] >= 1);
	ASSERTG (((unsigned long *) s1)[-7] == 0);
	ASSERTG (((unsigned long *) s2)[-7] == 0);

/* Set the has-been-partially-FFTed flag */

	((unsigned long *) d)[-7] = 0;

/* Now do the add */

	SRCARG = s1;
	SRC2ARG = s2;
	DESTARG = d;
	gw_add ();
	((unsigned long *) d)[-1] = 1;
}

void gwsubn3 (			/* Compute s1 - s2 normalizing always */
	gwnum	s1,		/* Source #1 */
	gwnum	s2,		/* Source #2 */
	gwnum	d)		/* Destination */
{

	ASSERTG (((unsigned long *) s1)[-1] >= 1);
	ASSERTG (((unsigned long *) s2)[-1] >= 1);
	ASSERTG (((unsigned long *) s1)[-7] == 0);
	ASSERTG (((unsigned long *) s2)[-7] == 0);

/* Set the has-been-partially-FFTed flag */

	((unsigned long *) d)[-7] = 0;

/* Now do the subtract */

	SRCARG = s2;
	SRC2ARG = s1;
	DESTARG = d;
	gw_sub ();
	((unsigned long *) d)[-1] = 1;
}


/* Multiply numbers using a slower method that will have reduced */
/* round-off error on non-random input data.  Caller must make sure the */
/* input numbers have not been partially or fully FFTed. */

void gwmul_carefully (
	gwnum s, gwnum t)					/* Source and destination */
{
	gwnum	tmp1, tmp2, tmp3, tmp4;
	double	saved_addin_value;

/* Generate a random number, if we have't already done so */

	if (GW_RANDOM == NULL) {
		GW_RANDOM = gwalloc ();
		gw_random_number (GW_RANDOM);
		gwsquare (GW_RANDOM);		// To force a normalization...
	}

/* Save and clear the addin value */

	saved_addin_value = ADDIN_VALUE;
	ADDIN_VALUE = 0.0;

/* Now do the multiply using four multiplies and adds */

	tmp1 = gwalloc ();
	tmp2 = gwalloc ();
	tmp3 = gwalloc ();
	tmp4 = gwalloc ();
	gwcopy (s, tmp4);
	gwstartnextfft (0);					/* Disable POSTFFT */
	gwaddn3 (s, GW_RANDOM, tmp1);		/* Compute s+random */
	gwaddn3 (t, GW_RANDOM, tmp3);		/* Compute t+random */
	gwfft (GW_RANDOM, tmp2);
	gwfftmul (tmp2, tmp4);				/* Compute s*random */
	gwfftmul (tmp2, t);					/* Compute t*random */
	gwfftfftmul (tmp2, tmp2, tmp2);		/* Compute random^2 */
	ADDIN_VALUE = saved_addin_value;	/* Restore the addin value */
	gwmul (tmp1, tmp3);					/* Compute (s+random)*(t+random) */
	gwsubn3 (tmp3, tmp2, tmp3);			/* Subtract random^2 */
	gwsubn3 (tmp3, t, tmp3);
	gwsubn3 (tmp3, tmp4, t);

/* Free memory and return */

	gwfree (tmp1);
	gwfree (tmp2);
	gwfree (tmp3);
	gwfree (tmp4);
}


/* Do a squaring very carefully.  This is done after a normal */
/* iteration gets a roundoff error above 0.40.  This careful iteration */
/* will not generate a roundoff error. */

void careful_squaring (
	gwnum s)		
{
	gwnum	hi, lo;
	unsigned long i;
	double	saved_addin_value;

/* Copy the data to hi and lo.  Zero out half the FFT data in each. */

	hi = gwalloc ();
	lo = gwalloc ();
	gwcopy (s, hi);
	gwcopy (s, lo);
	for (i = 0; i < FFTLEN/2; i++)
		set_fft_value (hi, i, 0);
	for ( ; i < FFTLEN; i++)
		set_fft_value (lo, i, 0);

/* Save and clear the addin value */

	saved_addin_value = ADDIN_VALUE;
	ADDIN_VALUE = 0.0;

/* Now do the squaring using three multiplies and adds */

	gwstartnextfft (FALSE);
	gwfft (hi, hi);
	gwfft (lo, lo);
	gwfftfftmul (lo, hi, s);
	gwfftfftmul (hi, hi, hi);
	ADDIN_VALUE = saved_addin_value;	/* Restore the addin value */
	gwfftfftmul (lo, lo, lo);
	gwadd (s, s);
	gwadd (hi, s);
	gwadd (lo, s);

/* Since our error recovery code cannot cope with an error during a careful */
/* iteration, make sure the error variable is cleared.  This shouldn't */
/* ever happen, but two users inexplicably ran into this problem. */

//	gw_clear_error ();

/* Free memory and return */

	gwfree (hi);
	gwfree (lo);
}


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
	unsigned int test, override, twop = 2*p;

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

#ifndef X86_64
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
#endif
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
				if (resn = isZero (testn)) {
					makestr (FACHSW, FACMSW, FACLSW, facnstr);
				}
			}
			if (!resnp) {
				gtog (NP, testnp);
				modg (testf, testnp);
				if (resnp = isZero (testnp)) {
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

	tempFileName (filename, 'f');

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
					_write (fd, &shortdummy, sizeof (short));
					longdummy = 0;
					_write (fd, &longdummy, sizeof (long));
					shortdummy = 999;
					_write (fd, &shortdummy, sizeof (short));
					shortdummy = *result;
					_write (fd, &shortdummy, sizeof (short));
					shortdummy = old_style;
					_write (fd, &shortdummy, sizeof (short));
					shortdummy = bits;
					_write (fd, &shortdummy, sizeof (short));
					_write (fd, &pass, sizeof (short));
					_write (fd, &FACHSW, sizeof (long));
					_write (fd, &FACMSW, sizeof (long));
					_write (fd, &endpthi, sizeof (long));
					_write (fd, &endptlo, sizeof (long));
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
		_read (fd, &type, sizeof (short));
		_read (fd, &p, sizeof (long));			// dummy
		_read (fd, &p, sizeof (long));
		_read (fd, &pass, sizeof (short));
		_read (fd, &bits, sizeof (short));
		_read (fd, &FACHSW, sizeof (long));
		_read (fd, &FACMSW, sizeof (long));
		_read (fd, &factored, sizeof (long));
		_read (fd, &eliminated, sizeof (long));
		_close (fd);
		continuation = TRUE;
	} else
		continuation = FALSE;

/* Init filename */

	tempFileName (filename, 'g');

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
						_write (fd, &four, sizeof (short));
						_write (fd, &p, sizeof (long)); /* dummy */
						_write (fd, &p, sizeof (long));
						_write (fd, &pass, sizeof (short));
						_write (fd, &bits, sizeof (short));
						_write (fd, &FACHSW, sizeof (long));
						_write (fd, &FACMSW, sizeof (long));
						_write (fd, &factored, sizeof (long));
						_write (fd, &eliminated, sizeof (long));
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

/* Test for a probable prime -- gwsetup has already been called. */

int commonPRP (
	unsigned long a,
	int	*res, char *str)
{
	unsigned long bit, iters;
	gwnum	x;
	giant	tmp;
	char	filename[20], buf[sgkbufsize+256], fft_desc[100], oldres64[17];
	long	write_time = DISK_WRITE_TIME * 60;
	int	echk, saving, stopping;
	time_t	start_time, current_time;
	double	reallyminerr = 1.0;
	double	reallymaxerr = 0.0;

/* Init, subtract 1 from N to compute a^(N-1) mod N */

	iaddg (-1, N);
	Nlen = bitlen (N);
	*res = TRUE;		/* Assume it is a probable prime */

	gwsetmaxmulbyconst (a);


/* Init filename */

	tempFileName (filename, 'z');

/* Get the current time */

	clear_timers ();
	start_timer (0);
	start_timer (1);
	time (&start_time);

/* Allocate memory */

	x = gwalloc ();

	tmp = popg ((Nlen >> 5) + 3);

/* Optionally resume from save file and output a message */
/* indicating we are resuming a test */

	if (fileExists (filename) && readFromFile (filename, &bit, x, NULL)) {
		char	fmt_mask[80];
		double	pct;
		pct = trunc_percent (bit * 100.0 / Nlen);
		sprintf (fmt_mask,
			 "Resuming probable prime test of %%s at bit %%ld [%%.%df%%%%]\n",
			 PRECISION);
		sprintf (buf, fmt_mask, str, bit, pct);
		OutputStr (buf);
	}

/* Otherwise, output a message indicating we are starting test */

	else {
		sprintf (buf, "Starting probable prime test of %s\n", str);
		OutputStr (buf);
		bit = 1;
		dbltogw ((double) a, x);
	}

/* Output a message about the FFT length */

	gwfft_description (fft_desc);
	sprintf (buf, "Using %s\n", fft_desc);
	OutputStr (buf);
	LineFeed();
	ReplaceableLine (1);	/* Remember where replaceable line is */

/* Init the title */

	if (strlen (str) < 40)
		title (str);
	else
		title("Running in Big k mode...");

/* Do the PRP test */

	gwsetmulbyconst (a);
	iters = 0;
	while (bit < Nlen) {

/* Error check the last 50 iterations, before writing an */
/* intermediate file (either user-requested stop or a */
/* 30 minute interval expired), and every 128th iteration. */

		stopping = stopCheck ();
		echk = stopping || ERRCHK || bit+50 >= Nlen;
		if (((bit & 127) == 0) || ((bit + 10) == maxerr_point)) {
			echk = 1;
			time (&current_time);
			saving = ((current_time - start_time > write_time) || ((bit + 10) == maxerr_point));
		} else
			saving = 0;

/* Process this bit */

		gwstartnextfft (!stopping && !saving && (bit+26 < Nlen) && (bit > 26) && !maxerr_recovery_mode);

		if (bitval (N, Nlen-bit-1)) {
			gwsetnormroutine (0, echk, 1);
		} else {
			gwsetnormroutine (0, echk, 0);
		}
		if (GENERAL_MOD || ((bit+25 < Nlen) && (bit > 25) && (!maxerr_recovery_mode || (bit != maxerr_point))))
			gwsquare (x);
		else {
			if (k_is_square)
				gwsquare_carefully (x);
			else
				careful_squaring (x);
//			careful_squaring (x);
			maxerr_recovery_mode = FALSE;
			maxerr_point = 0;
		}

/* If the sum of the output values is an error (such as infinity) */
/* then raise an error. */

		if (gw_test_illegal_sumout ()) {
			sprintf (buf, ERRMSG0, bit, Nlen, ERRMSG1A);
			OutputBoth (buf);
			sleep5 = 1;
			goto error;
		}

/* Check that the sum of the input numbers squared is approximately */
/* equal to the sum of unfft results.  Since this check may not */
/* be perfect, check for identical results after a restart. */

		if (gw_test_mismatched_sums ()) {
			static unsigned long last_bit = 0;
			static double last_suminp = 0.0;
			static double last_sumout = 0.0;
			double suminp, sumout;
			suminp = gwsuminp (x);
			sumout = gwsumout (x);
			if (bit == last_bit &&
			    suminp == last_suminp &&
			    sumout == last_sumout) {
				writeResults (ERROK);
				saving = 1;
			} else {
				char	msg[80];
				sprintf (msg, ERRMSG1B, suminp, sumout);
				sprintf (buf, ERRMSG0, bit, Nlen, msg);
				OutputBoth (buf);
				last_bit = bit;
				last_suminp = suminp;
				last_sumout = sumout;
				sleep5 = 1;
				goto error;
			}
		}

/* Check for excessive roundoff error  */

		if (MAXERR > 0.40) {
			static unsigned long last_bit = 0;
			static double last_maxerr = 0.0;
			if (bit == last_bit &&
			    MAXERR == last_maxerr) {
				OutputBoth (ERROK);
				GWERROR = 0;
				OutputBoth (ERRMSG6);
				maxerr_recovery_mode = 1;
				sleep5 = FALSE;
				goto error;
			} else {
				char	msg[80];
				sprintf (msg, ERRMSG1C, MAXERR);
				sprintf (buf, ERRMSG0, bit, Nlen, msg);
				OutputBoth (buf);
				maxerr_point = last_bit = bit;
				last_maxerr = MAXERR;
				sleep5 = FALSE;
				goto error;
			}
		}

/* Keep track of maximum and minimum round off error */

		if (ERRCHK) {
			if (MAXERR < reallyminerr && bit > 30)
				reallyminerr = MAXERR;
			if (MAXERR > reallymaxerr)
				reallymaxerr = MAXERR;
		}

/* That iteration succeeded, bump counters */

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
			if (! writeToFile (filename, bit, x, NULL)) {
				sprintf (buf, WRITEFILEERR, filename);
				OutputBoth (buf);
				if (write_time > 600) write_time = 600;
			}
			time (&start_time);

/* If an escape key was hit, write out the results and return */

			if (stopping) {
				pushg (1);
				gwfree (x);
				gwdone ();
				*res = FALSE;		// To avoid credit message !
				return (FALSE);
			}
		}

/* Output the 64-bit residue at specified interims.  Also output the */
/* residues for the next iteration so that we can compare our */
/* residues to programs that start counter at zero or one. */

		if (interimResidues && bit % interimResidues < 2) {
				gwtogiant (x, tmp);			// The modulo reduction is done here
			iaddg (1, tmp);					// Compute the (unnormalized) residue
			if (abs(tmp->sign) < 2)			// make a 32 bit residue correct !!
				tmp->n[1] = 0;
			sprintf (buf, 
				 "%s interim residue %08lX%08lX at bit %ld\n",
				 str, tmp->n[1], tmp->n[0], bit);
			OutputBoth (buf);
		}

/* Write a save file every "interimFiles" iterations. */

		if (interimFiles && bit % interimFiles == 0) {
			char	interimfile[20];
			sprintf (interimfile, "%.8s.%03d",
				 filename, bit / interimFiles);
			if (! writeToFile (interimfile, bit, x, NULL)) {
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

	gwtogiant (x, tmp);
	if (!isone (tmp)) {
		*res = FALSE;	/* Not a prime */
		if (abs(tmp->sign) < 2)	// make a 32 bit residue correct !!
			tmp->n[1] = 0;
		sprintf (res64, "%08lX%08lX", tmp->n[1], tmp->n[0]);
		imulg (3, tmp); specialmodg (tmp); ulsubg (3, tmp);
		if (abs(tmp->sign) < 2)	// make a 32 bit residue correct !!
			tmp->n[1] = 0;
		sprintf (oldres64, "%08lX%08lX", tmp->n[1], tmp->n[0]);
	}
	pushg (1);
	gwfree (x);

/* Print results.  Do not change the format of this line as Jim Fougeron of */
/* PFGW fame automates his QA scripts by parsing this line. */

	if (username[0] != '\0') {	// Test if LLRnet output format is required
		sprintf (buf, "user=%s\n", username);
		writeResults (buf);
		OutputTimeStampOnDisk ();
	}

	if (*res)
		sprintf (buf, "%s is a probable prime.", str);
	else if (IniGetInt (INI_FILE, "OldRes64", 1))
		sprintf (buf, "%s is not prime.  RES64: %s.  OLD64: %s", str, res64, oldres64);
	else
		sprintf (buf, "%s is not prime.  RES64: %s", str, res64);

#if defined(WIN32) && !defined(_CONSOLE)

	sprintf (buf+strlen(buf), "  Time : "); 
	ReplaceableLine (2);	/* Replace line */ 

#else

	clearline(100);

#ifdef _CONSOLE
	OutputBoth(buf);
#else
	if (*res) {
		OutputStr("\033[7m");
		OutputBoth(buf);
		OutputStr("\033[0m");
	}
	else
		OutputBoth(buf);
#endif

	sprintf (buf, "  Time : "); 

#endif

/* Update the output file */

// 
	if ((*res && IniGetInt (INI_FILE, "OutputPrimes", 0)) ||
	    (!*res && IniGetInt (INI_FILE, "OutputComposites", 0)))
		writeResults ("\n");

/* Output the final timings */

	end_timer (1);
	write_timer (buf+strlen(buf), 1, TIMER_CLR | TIMER_NL); 
	if ((*res && IniGetInt (INI_FILE, "OutputPrimes", 0)) ||
	    (!*res && IniGetInt (INI_FILE, "OutputComposites", 0)))
		OutputStr (buf);
	else
		OutputBoth (buf);

/* Cleanup and return */

	iaddg (1, N);					// Restore the value of N
	Nlen = bitlen (N);
	gwdone ();
	_unlink (filename);
	return (TRUE);

/* An error occured, sleep, then try restarting at last save point. */

error:
	iaddg (1, N);					// Restore the value of N
	Nlen = bitlen (N);
	pushg (1);
	gwfree (x);
	gwdone ();
	*res = FALSE;					// To avoid credit mesage...

	if ((abonillsum && gw_test_illegal_sumout()) || 
		(abonmismatch && gw_test_mismatched_sums ()) || 
		(abonroundoff && MAXERR > 0.40)) {	// Abort...
		sprintf (buf,ERRMSG5,str);
		OutputBoth (buf);
		_unlink (filename);
		return (TRUE);
	}

/* Output a message saying we are restarting */

	if (sleep5) OutputBoth (ERRMSG2);
	OutputBoth (ERRMSG3);

/* Sleep five minutes before restarting */

	if (sleep5 && ! SleepFive ()) return (FALSE);

/* Restart */

	return (-1);
}

/* Test if K*2^N+C is a probable prime. */

int fastIsPRP (
	double	k,			/* K in k*b^n+c */
	unsigned long b,		/* B in k*b^n+c */
	unsigned long n,		/* N in k*b^n+c */
	signed long c,			/* C in k*b^n+c */
	int	*res)
{
	int	retval;

/* Setup the assembly code. */

	do {
		gwsetup (k, b, n, c, 0);
		k_is_square = (floor(sqrt (k) * sqrt (k)) == k);


/* Do the PRP test */

		retval = commonPRP (3, res, gwmodulo_as_string ());
	} while (retval == -1);

/* Clean up and return */

	return (retval);
}


/* Test if N is a probable prime.  The number N can be of ANY form. */

int slowIsPRP (
	char	*str,		/* string representation of N */
	int	*res)
{
	int	retval;

/* Setup the gwnum code */

	do {
		gwsetup_general_mod_giant (N, 0);

/* Do the PRP test */

		retval = commonPRP (3, res, str);
	} while (retval == -1);
	return (retval);
}


/* Test if a small N is a probable prime. */
/* Compute 3^(N-1) mod N */

int isProbablePrime (void)
{
	int	retval;
	giant	x;

	if (isone (N)) return (FALSE);
	x = newgiant (N->sign + 8);
	itog (3, x);
	powermodg (x, N, N);
	iaddg (-3, x);
	retval = isZero (x);
	free (x);
	return (retval);
}

int isPRPinternal (
	char *str, double dk, 
	unsigned long base,
	unsigned long n,
	int incr,
	int *res)
{
	char	buf[sgkbufsize+256];
	unsigned long retval;

	if (dk >= 1.0)
		retval = fastIsPRP (dk, base, n, incr, res);
	else if (Nlen < 50) {
		*res = isProbablePrime();

		if (username[0] != '\0') {	// Test if LLRnet output format is required
			sprintf (buf, "user=%s\n", username);
			writeResults (buf);
			OutputTimeStampOnDisk ();
		}

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
	else
		retval = slowIsPRP (str, res);
	if (*res && Nlen > 50)
		OutputBoth ("Please credit George Woltman's PRP for this result!\n");
	return retval;
}

#define NPG		0					// NEWPGEN output format, not AP mode
#define NPGAP	1					// NEWPGEN output format, AP mode
#define	ABCC	2					// Carol ABC format
#define	ABCK	3					// Kynea ABC format
#define ABCCW	4					// Cullen/Woodall ABC format
#define ABCFF	5					// FermFact output ABC format
#define ABCGM	6					// Gaussian Mersenne ABC format

// New ABC formats for k*b^n+/-c
#define ABCFKGS		11				// Fixed k:  b and n specified on each input line
#define ABCFKAS		12				// Fixed k:  b, n, and c specified on each input line
#define ABCFBGS		13				// Fixed b:  k and n specified on each input line
#define ABCFBAS		14				// Fixed b:  k, n, and c specified on each input line
#define ABCFNGS		15				// Fixed n:  k and b specified on each input line
#define ABCFNAS		16				// Fxied n:  k, b, and c specified on each input line
#define ABCVARGS	17				// k, b, and n specified on each inpput line
#define ABCVARAS	18				// k, b, n, and c specified on each input line

int IsPRP (							// General PRP test
	unsigned long format, 
	char *sgk,
	unsigned long base,
	unsigned long n, 
	int incr,
	unsigned long shift,
	int	*res) 
{  
	char	buf[sgkbufsize+256], str[sgkbufsize+256], sgk1[sgkbufsize]; 
	unsigned long bits, retval;
	double dk;

	if (!(format == ABCC || format == ABCK)) {
		gk = newgiant (strlen(sgk)/2 + 8);	// Allocate one byte per decimal digit + spares
		ctog (sgk, gk);						// Convert k string to giant
		gshiftleft (shift, gk);				// Shift k multiplier if requested
		gtoc (gk, sgk1, sgkbufsize);		// Updated k string
		if (mask & MODE_DUAL) {
			sprintf (str, "%lu^%lu%c%d", base, n, incr < 0 ? '-' : '+', abs(incr));
		}
		else if (format != NPGAP) {			// Not MODE_AP
			if (!strcmp(sgk1, "1"))
				sprintf (str, "%lu^%lu%c%d", base, n, incr < 0 ? '-' : '+', abs(incr));
			else
				sprintf (str, "%s*%lu^%lu%c%d", sgk1, base, n, incr < 0 ? '-' : '+', abs(incr));
		}
		else {								// MODE_AP
			if (!strcmp(sgk1, "1"))
				sprintf (str, "%lu^%lu+1", base, n);
			else
				sprintf (str, "%lu^%lu+2*%s-1", base, n, sgk1);
		}
	}
	else {
		gk = newgiant ((n>>4)+8);
		setone (gk);						// Compute k multiplier
		gshiftleft (n-2, gk);				// Warning : here, n is exponent+1 !
		if (format == ABCK) {
			uladdg (1, gk);
			sprintf (str, "%s*2^%lu%c1 = (2^%d+1)^2 - 2", sgk, n, '-', n-1);
		}
		else {
			ulsubg (1, gk);
			sprintf (str, "%s*2^%lu%c1 = (2^%d-1)^2 - 2", sgk, n, '-', n-1);
		}
	}

	if (!IniGetInt (INI_FILE, "ForcePRP", 0)) {
		sprintf (buf, "LLR tests only k*2^n+/-1 numbers, so, we will do a PRP test of %s\n", str); 
		if (verbose)
			OutputBoth (buf); 
		else
			OutputStr (buf);
	}

	bits = (unsigned long) ((n * log(base)) / log(2) + bitlen(gk)); 
	N =  newgiant ((bits >> 4) + 8);		// Allocate memory for N

//	Compute the number we are testing.

	ultog (base, N);
	power (N, n);

	if (format == NPGAP) {				// mode AP
		addg(gk, N);
		addg(gk, N);
	}
	else {								// not mode AP
		mulg (gk, N); 
	}

	iaddg (incr, N);

	Nlen = bitlen (N); 
	klen = bitlen(gk);

	if (klen > 53) {					// we must use generic reduction
		dk = 0.0;
	}
	else {								// we can use DWT ; compute the multiplier as a double
		dk = (double)gk->n[0];
		if (gk->sign > 1)
			dk += 4294967296.0*(double)gk->n[1];
	}

	retval = isPRPinternal (str, dk, base, n, incr, res);

	free (N);
	free (gk);
	return retval;
}

/*
 	Primality testing of k*2^n-1 numbers with the Lucas Lehmer Riesel
	algorithm, using the gwnums for fast multiplications and squarings.
	Second attempt for a full IBDWT version, using Colin Percival method improved
	by George Woltman.
	Jean Penné, May 2004.
*/

int isLLRP ( 
	unsigned long format, 
	char *sgk,
	unsigned long n, 
	unsigned long shift,
	int	*res) 
{ 
	unsigned long iters, index, word2; 
	unsigned long p, fftlen, fftlmers, gksize, j, k; 
	unsigned long mask, last, bit, bits; 
	long	vindex, retval;
	long	nextffttaken = 0;
	gwnum	x, y, gwn2; 
	giant	tmp/*, gx, gy*/; 
	char	filename[20], buf[sgkbufsize+256], str[sgkbufsize+256],
			sgk1[sgkbufsize]; 
	long	write_time = DISK_WRITE_TIME * 60; 
	int		echk, saving, stopping, v1, first = 1, inc = -1; 
	time_t	start_time, current_time; 
	double	reallyminerr = 1.0; 
	double	reallymaxerr = 0.0; 
	double	dk, gwn2w;



	if (!(format == ABCC || format == ABCK)) {
		gksize = strlen(sgk);
		gk = newgiant ((gksize>>1) + 8);	// Allocate one byte per decimal digit + spares
		ctog (sgk, gk);						// Convert k string to giant
		if (shift > 0) {
			gshiftleft (shift, gk);			// Shift k multiplier if requested
			gtoc (gk, sgk1, sgkbufsize);	// Updated k string
		}
		else
			strcpy (sgk1, sgk);
		sprintf (str, "%s*2^%lu%c1", sgk1, n, '-');	// Number N to test, as a string

//	gk must be odd for the LLR test, so, adjust gk and n if necessary.

		while (bitval(gk, 0) == 0) {
			gshiftright (1, gk);	// update k as a giant
			n++;
		}

	}
	else {
		gk = newgiant ((n>>4)+8);
		setone (gk);						// Compute k multiplier
		gshiftleft (n-2, gk);				// Warning : here, n is exponent+1 !
		if (format == ABCK) {
			uladdg (1, gk);
			sprintf (str, "%s*2^%lu%c1 = (2^%d+1)^2 - 2", sgk, n, '-', n-1);
		}
		else {
			ulsubg (1, gk);
			sprintf (str, "%s*2^%lu%c1 = (2^%d-1)^2 - 2", sgk, n, '-', n-1);
		}
	}

	klen = bitlen(gk);					// Bit length ok k multiplier
	bits = n + klen;					// Bit length of N
	N =  newgiant ((bits >> 4) + 8);		// Allocate memory for N

//	Compute the number we are testing.

	setone (N);
	gshiftleft (n, N);
	mulg (gk, N); 
	ulsubg (1, N);

	Nlen = bitlen (N); 

	if (klen > 53) {		// we must use generic reduction
		dk = 0.0;
	}
	else {					// we can use DWT ; compute the multiplier as a double
		dk = (double)gk->n[0];
		if (gk->sign > 1)
			dk += 4294967296.0*(double)gk->n[1];
			k_is_square = (floor(sqrt (dk) * sqrt (dk)) == dk);
	}

	if (N->sign == 1) {		// N is a small number, so we make a simple test...

		if (username[0] != '\0') {	// Test if LLRnet output format is required
			sprintf (buf, "user=%s\n", username);
			writeResults (buf);
			OutputTimeStampOnDisk ();
		}

		if (*res = isPrime (N->n[0])) {
#ifndef WIN32
			OutputStr("\033[7m");
#endif
			sprintf (buf, "%s = %lu is prime! (trial divisions)\n", str, N->n[0]); 
			OutputBoth (buf);
#ifndef WIN32
			OutputStr("\033[0m");
#endif
			sprintf (res64, "0000000000000000");
		}
		else	{
			sprintf (buf, "%s = %lu is  not prime. (trial divisions)\n", str, N->n[0]); 
			OutputBoth (buf);
			sprintf (res64, "????????????????");
		}
		free(gk);
		free(N);
		return (TRUE); 
	}

	if (klen > n) {
	    sprintf(buf, "%s > 2^%lu, so we can only do a PRP test for %s.\n", sgk, n, str);
	    OutputBoth(buf);
		retval = isPRPinternal (str, dk, 2, n, -1, res);
		free(gk);
		free(N);
		return retval;
	}


	if (bits > 100 && bits < 10*klen && !IniGetInt(INI_FILE, "PRPdone", 0)) {
								// We have better to do ad first a PRP test.
		retval = isPRPinternal (str, dk, 2, n, -1, res);
		if (!*res) {
			free(gk);
			free(N);
			return retval;
		}
		IniWriteInt(INI_FILE, "PRPdone", 1);
	}

	if(abs(gk->sign) == 1) {	// k is a "small" integer
	    k = gk->n[0];
	}
	else {
		k = 0;					// to indicate that k is a big integer
	}
 
/* Get the current time */ 

restart: 
	vindex = 1;					// First attempt
	nextffttaken = 0;
	time (&start_time); 

/* Assume intermediate results of the length of N. */ 
/* Compute the fftlen for a Mersenne number of this size. */
	
	p = Nlen; 

	fftlmers = gwmap_to_fftlen (1.0, 2, p, -1);

	fftlen = 0;			// for first attempt...

	*res = TRUE;		/* Assume it is prime */ 

//	gwset_safety_margin (10.0);

	if (dk >= 1.0)
		gwsetup (dk, 2, n, -1, fftlen); 
	else {
		gwsetup_general_mod_giant (N, fftlen);
	}

	clear_timers (); 
	start_timer (0); 
	start_timer (1); 

	x = gwalloc (); 
	y = gwalloc ();
	gwn2 = gwalloc ();
 
	tmp =  popg ((Nlen >> 5) + 3); 

	dbltogw (2.0, gwn2);
	if (k != 1) {					// Non Mersenne
		bitaddr (n+1, &word2, &bit);
		gwn2w = *addr (gwn2, word2);
	}
	else {							// Mersenne
		word2 = 0;
		gwn2w = 2.0;
	}
	if (skiptest && klen <= 22 && gwn2w >= 0) {
		sprintf (buf, "No need to redo L. L. R. prime test of %s\n", str); 
		OutputStr (buf); 
		pushg (1); 
		free(gk);
		free(N);
		gwfree (x); 
		gwfree (y);
		gwfree (gwn2);
		gwdone();
		*res = FALSE;
		end_timer (1); 
		return(TRUE);
	}

	last = n-1;

 	gwsetnormroutine (0, ERRCHK, 0); 

/* Init filename */ 

	tempFileName (filename, 'z'); 
 
/* Init the title */ 
 
	if (strlen (str) < 40)
		title(str);
	else
		title("Running in Big k mode...");
 
/* Optionally resume from save file and output a message */ 
/* indicating we are resuming a test */ 
 
	if (fileExists (filename) && readFromFile (filename, &j, x, NULL)) { 
		char	fmt_mask[80]; 
		double	pct; 
		pct = trunc_percent (j * 100.0 / n); 
		sprintf (fmt_mask, 
			 "Resuming test of %%s at iteration %%ld [%%.%df%%%%]\n", 
			 PRECISION); 
		sprintf (buf, fmt_mask, str, j, pct); 
		OutputStr (buf); 
	} 
 

/* Otherwise, output a message indicating we are starting test, */ 
/* or resuming the computing of U0. */
 
	else { 
	    if (k==1) {
			if (!isPrime (n)) {
				sprintf (buf, "The Mersenne number %s is not prime because %d is not prime.\n", str, n); 
				OutputBoth (buf); 
				pushg (1); 
				free(gk);
				free(N);
				gwfree (x); 
				gwfree (y);
				gwfree (gwn2);
				gwdone();
				*res = FALSE;
				end_timer (1); 
				return(TRUE);
			}
		sprintf (buf, 
	      "Prime95 or Mprime are much better to test this Mersenne number !!\n");
		if (verbose)
			OutputBoth(buf);
		else
			OutputStr(buf);
		v1 = 4;
		dbltogw ((double) v1, x); 
		goto MERSENNE;
	    }
	    filename[0] = 'u';
	    if ((v1 = gen_v1(gk, n, 0, vindex)) < 0) {
			if (v1 == -1)
				sprintf (buf, "Cannot compute V1 to test %s...\nThis is surprising, please, let me know that!!\nMy E-mail is jpenne@wanadoo.fr\n", str);
			else
				sprintf (buf, "%s has a small factor : %d !!\n", str, abs(v1));
			OutputBoth (buf); 
			pushg (1); 
			free(gk);
			free(N);
			gwfree (x); 
			gwfree (y);
			gwfree (gwn2);
			gwdone();
			*res = FALSE;
			end_timer (1); 
			return(TRUE);
	    }

	    if (fileExists (filename) && readFromFile (filename, &j, x, y)) { 
			char	fmt_mask[80]; 
			double	pct; 
			pct = trunc_percent (100.0 - j * 100.0 / klen); 
			sprintf (fmt_mask, 
			 "Resuming test of %%s (computing U0) at iteration %%ld [%%.%df%%%%]", 
			 PRECISION); 
			sprintf (buf, fmt_mask, str, klen - j, pct); 
			OutputStr (buf); 
			LineFeed();
			ReplaceableLine (1);	/* Remember where replacable line is */ 
	    } 
	    else {
			if (!skiptest) {
				sprintf (buf, "Starting Lucas Lehmer Riesel prime test of %s\n", str);
				OutputStr (buf); 
			}
			else {
				sprintf (buf, "Redoing Lucas Lehmer Riesel prime test of %s\n", str); 
				OutputBoth (buf); 
			}
			if (!GENERAL_MOD)
				if (!ZERO_PADDED_FFT)
					if (!RATIONAL_FFT)
						sprintf (buf, "Using Irrational Base DWT : Mersenne fftlen = %d, Used fftlen = %d\n", fftlmers, FFTLEN);
					else
						sprintf (buf, "Using Rational Base DWT : Mersenne fftlen = %d, Used fftlen = %d\n", fftlmers, FFTLEN);
				else
					if (!RATIONAL_FFT)
						sprintf (buf, "Using Zero Padded IBDWT : Mersenne fftlen = %d, Used fftlen = %d\n", fftlmers, FFTLEN);
					else
						sprintf (buf, "Using Zero Padded RBDWT : Mersenne fftlen = %d, Used fftlen = %d\n", fftlmers, FFTLEN);
			else
				if (!RATIONAL_FFT)
					sprintf (buf, "Using General Mode (Irrational Base) : Mersenne fftlen = %d, Used fftlen = %d\n", fftlmers, FFTLEN);
				else
					sprintf (buf, "Using General Mode (Rational Base) : Mersenne fftlen = %d, Used fftlen = %d\n", fftlmers, FFTLEN);
			if (verbose)
				OutputBoth(buf);
			else {
				OutputStr(buf);
			}
			sprintf (buf, "V1 = %d ; Computing U0...", v1);
			OutputStr (buf); 
			LineFeed();
			ReplaceableLine (1);	/* Remember where replacable line is */ 
			dbltogw ((double) v1, x); 
			dbltogw ((double) v1, y); 
			gwsetaddin (-2);
			if (!GENERAL_MOD)
				if (k_is_square)
					gwsquare_carefully (y);
				else
					careful_squaring (y);
			else
				gwsquare (y);
			CHECK_IF_ANY_ERROR(y, 1, klen, 0)
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

/* Error check the last 50 iterations, before writing an */ 
/* intermediate file (either user-requested stop or a */ 
/* 30 minute interval expired), and every 128th iteration. */ 
 
			stopping = stopCheck (); 
			echk = stopping || ERRCHK || (j <= 50); 
			if ((index & 127) == 0) { 
				echk = 1; 
				time (&current_time); 
				saving = (current_time - start_time > write_time); 
			}
			else 
				saving = 0; 
			if (bit) {
				gwsetaddin (-v1);
				gwsafemul (y, x);
				CHECK_IF_ANY_ERROR(x, (index), klen, 1)
				gwsetaddin (-2);
				gwsquare (y);
				CHECK_IF_ANY_ERROR(y, (index), klen, 2)
			}
			else {
				gwsetaddin (-v1);
				gwsafemul (x, y);
				CHECK_IF_ANY_ERROR(y, (index), klen, 3)
				gwsetaddin (-2);
				gwsquare (x);
				CHECK_IF_ANY_ERROR(x, (index), klen, 4)
			}
 
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
				if (! writeToFile (filename, j, x, y)) { 
					sprintf (buf, WRITEFILEERR, filename); 
					OutputBoth (buf); 
					if (write_time > 600) write_time = 600; 
				} 
				time (&start_time); 
 
/* If an escape key was hit, write out the results and return */ 
 
				if (stopping) {
					pushg (1); 
					free(gk);
					free(N);
					gwfree (x); 
					gwfree (y);
					gwfree (gwn2);
					gwdone();
					return (FALSE); 
				}
			} 
	    }

		gwsetaddin (-v1);
		gwmul (y, x);
		CHECK_IF_ANY_ERROR(x, klen, klen, 5)
		ReplaceableLine (2);	/* Replace line */ 
		sprintf (buf, "V1 = %d ; Computing U0...done.\n", v1);
		if (verbose)
			OutputBoth (buf); 
		else
			OutputStr(buf);
							/* End of x = u0 computing */
	    filename[0] = 'z';	/* restore filename which was modified... */
MERSENNE:
	    j = 1;
	    sprintf (buf, "Starting Lucas-Lehmer loop..."); 
	    OutputStr (buf); 
	} 
	LineFeed();
	ReplaceableLine (1);	/* Remember where replacable line is */ 
 
/* Do the Lucas Lehmer Riesel Prime test */ 
 
	iters = 0; 
	gwsetaddin (-2);

	while (j<last) { 
 
/* Error check the last 50 iterations, before writing an */ 
/* intermediate file (either user-requested stop or a */ 
/* 30 minute interval expired), and every 128th iteration. */ 

		stopping = stopCheck (); 
		echk = stopping || ERRCHK || (j <= 50) || (j >= last - 50); 
		if (((j & 127) == 0) || ((j + 10) == maxerr_point)) { 
			echk = 1; 
			time (&current_time); 
			saving = ((current_time - start_time > write_time) || ((j + 10) == maxerr_point)); 
		} else 
			saving = 0; 


/* Process this iteration */ 

		gwstartnextfft (!stopping && !saving &&  !((interimFiles && (j+1) % interimFiles == 0)) &&
			!(interimResidues && ((j+1) % interimResidues < 2)) && 
			(j >= 30) && (j < last - 31) && !maxerr_recovery_mode); 

		if (GENERAL_MOD || ((j > 30) && (j < last - 30) && (!maxerr_recovery_mode || (j != maxerr_point)))) {
			gwsquare (x);
		}
		else {
			if (k_is_square)
				gwsquare_carefully (x);
			else
				careful_squaring (x);
//			careful_squaring (x);
			maxerr_recovery_mode = FALSE;
			maxerr_point = 0;
		}
		CHECK_IF_ANY_ERROR(x, j, last, 6)
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

			if (! writeToFile (filename, j, x, NULL)) { 
				sprintf (buf, WRITEFILEERR, filename); 
				OutputBoth (buf); 
				if (write_time > 600) write_time = 600; 
			} 
			time (&start_time); 

 
/* If an escape key was hit, write out the results and return */ 
 
			if (stopping) {
				pushg (1); 
				free(gk);
				free(N);
				gwfree (x); 
				gwfree (y);
				gwfree (gwn2);
				gwdone();
				return (FALSE); 
			}
		} 

/* Output the 64-bit residue at specified interims.  Also output the */
/* residues for the next iteration so that we can compare our */
/* residues to programs that start counter at zero or one. */

		if (interimResidues && j % interimResidues < 2) {
				gwtogiant (x, tmp);			// The modulo reduction is done here
			if (abs(tmp->sign) < 2)			// make a 32 bit residue correct !!
				tmp->n[1] = 0;
			sprintf (buf, 
				 "%s interim residue %08lX%08lX at iteration %ld\n",
				 str, tmp->n[1], tmp->n[0], j);
			OutputBoth (buf);
		}

/* Write a save file every "interimFiles" iterations. */

		if (interimFiles && j % interimFiles == 0) {
			char	interimfile[20];
			sprintf (interimfile, "%.8s.%03d",
				 filename, j / interimFiles);
			if (! writeToFile (interimfile, j, x, NULL)) {
				sprintf (buf, WRITEFILEERR, interimfile);
				OutputBoth (buf);
			}
		}
	} 
//	gwprint (x, FFTLEN);
	gwtogiant (x, tmp); 
//	trace (tmp->sign);
	if (!isZero (tmp)) { 
		*res = FALSE;				/* Not a prime */ 
		if (abs(tmp->sign) < 2)		// make a 32 bit residue correct !!
			tmp->n[1] = 0;
		sprintf (res64, "%08lX%08lX", tmp->n[1], tmp->n[0]); 
	} 
 
/* Print results and cleanup */ 
 

	if (username[0] != '\0') {	// Test if LLRnet output format is required
		sprintf (buf, "user=%s\n", username);
		writeResults (buf);
		OutputTimeStampOnDisk ();
	}

	if (*res) 
		sprintf (buf, "%s is prime!", str); 
	else
		sprintf (buf, "%s is not prime.  LLR Res64: %s", str, res64); 

#if defined(WIN32) && !defined(_CONSOLE)

	sprintf (buf+strlen(buf), "  Time : "); 
	ReplaceableLine (2);	/* Replace line */ 

#else

	clearline(100);

#ifdef _CONSOLE
	OutputBoth(buf);
#else
	if (*res) {
		OutputStr("\033[7m");
		OutputBoth(buf);
		OutputStr("\033[0m");
	}
	else
		OutputBoth(buf);
#endif

	sprintf (buf, "  Time : "); 

#endif

	end_timer (1); 
	write_timer (buf+strlen(buf), 1, TIMER_CLR | TIMER_NL); 
	OutputBoth (buf); 

	pushg (1); 
	free(gk);
	free(N);
	gwfree (x); 
	gwfree (y);
	gwfree (gwn2);
	gwdone (); 
	filename[0] = 'u';
	_unlink (filename);
	filename[0] = 'z';
	_unlink (filename); 
	if (IniGetInt(INI_FILE, "PRPdone", 0))
		IniWriteInt(INI_FILE, "PRPdone", 0);
	return (TRUE); 

trynextfft:	
 
/* An error occured, sleep, then try restarting at last save point. */ 

error:
	pushg (1); 
	gwfree (x); 
	gwfree (y); 
	gwfree (gwn2);
	gwdone (); 
	*res = FALSE;

	if ((abonillsum && gw_test_illegal_sumout()) || 
		(abonmismatch && gw_test_mismatched_sums ()) || 
		(abonroundoff && MAXERR > 0.40)) {	// Abort...
		sprintf (buf,ERRMSG5,str);
		OutputBoth (buf);
		free(gk);
		free(N);
		filename[0] = 'u';
		_unlink (filename);
		filename[0] = 'z';
		_unlink (filename); 
		if (IniGetInt(INI_FILE, "PRPdone", 0))
			IniWriteInt(INI_FILE, "PRPdone", 0);
		return (TRUE);
	}

/* Output a message saying we are restarting */ 
 
	if (sleep5) OutputBoth (ERRMSG2); 
	OutputBoth (ERRMSG3); 
 
/* Sleep five minutes before restarting */ 
 
	if (sleep5 && ! SleepFive ()) return (FALSE); 

/* Restart */ 
 
	goto restart; 
} 

int isProthP ( 
	unsigned long format, 
	char *sgk,
	unsigned long n,
	unsigned long shift,
	int	*res) 
{ 
	unsigned long iters, gksize; 
	unsigned long p, fftlen, fftlmers; 
	unsigned long bit, bits; 
	long	a, retval;
	gwnum	x; 
	giant	tmp, tmp2; 
	char	filename[20], buf[sgkbufsize+256], 
		str[sgkbufsize+256], fft_desc[100], sgk1[sgkbufsize]; 
	long	write_time = DISK_WRITE_TIME * 60; 
	int	echk, saving, stopping, inc = +1; 
	time_t	start_time, current_time; 
	double	reallyminerr = 1.0; 
	double	reallymaxerr = 0.0; 
	double dk;

	gksize = strlen(sgk);
	gk = newgiant ((gksize>>1) + 8);	// Allocate one byte per decimal digit + spares
	ctog (sgk, gk);						// Convert k string to giant

	if (shift > 0) {
		gshiftleft (shift, gk);			// Shift k multiplier if requested
		gtoc (gk, sgk1, sgkbufsize);	// Updated k string
	}
	else
		strcpy (sgk1, sgk);

	sprintf (str, "%s*2^%lu%c1", sgk1, n, '+');	// Number N to test, as a string

	bits = n + bitlen(gk);				// Bit length of N
	N =  newgiant ((bits>>4) + 8);		// Allocate memory for N

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

	if (klen > 53) {					// we must use generic reduction
		dk = 0.0;
	}
	else {								// we can use DWT, compute k as a double
		dk = (double)gk->n[0];
		if (gk->sign > 1)
			dk += 4294967296.0*(double)gk->n[1];
			k_is_square = (floor(sqrt (dk) * sqrt (dk)) == dk);
	}

	if (N->sign == 1) {					// N is a small number, so we make a simple test...

		if (username[0] != '\0') {	// Test if LLRnet output format is required
			sprintf (buf, "user=%s\n", username);
			writeResults (buf);
			OutputTimeStampOnDisk ();
		}

		if (*res = isPrime (N->n[0])) {
#ifndef WIN32
			OutputStr("\033[7m");
#endif
			sprintf (buf, "%s = %lu is prime! (trial divisions)\n", str, N->n[0]); 
			OutputBoth (buf);
#ifndef WIN32
			OutputStr("\033[0m");
#endif
			sprintf (res64, "0000000000000000");
		}
		else	{
			sprintf (buf, "%s = %lu is  not prime. (trial divisions)\n", str, N->n[0]); 
			OutputBoth (buf);
			sprintf (res64, "????????????????");
		}
		free(gk);
		free(N);
		return (TRUE); 
	}

	if (klen > n) {
	    sprintf(buf, "%s > 2^%lu, so we can only do a PRP test for %s.\n", sgk, n, str);
	    OutputBoth(buf);
		retval = isPRPinternal (str, dk, 2, n, 1, res);
		free(gk);
		free(N);
		return (retval);
	}

/* Init the title */ 
 
	if (strlen (str) < 40)
		title(str);
	else
		title("Running in Big k mode...");
 
/* Compute the base for the Proth algorithm. */
 
if ((a = genProthBase(gk, n)) < 0) {
	if (a == -1)
		sprintf (buf, "Cannot compute a to test %s...\nThis is surprising, please, let me know that!!\nMy E-mail is jpenne@wanadoo.fr\n", str);
	else
		sprintf (buf, "%s has a small factor : %d !!\n", str, abs(a));
	OutputBoth (buf); 
	*res = FALSE;
	free(gk);
	free(N);
	return(TRUE);
}

gwsetmaxmulbyconst (a);

restart:
	clear_timers ();
	start_timer (0);
	start_timer (1);
	time (&start_time);


/* Assume intermediate results of the length of N. */ 
/* Compute the fftlen for a Mersenne number of this size. */
	
	p = Nlen; 

	fftlmers = gwmap_to_fftlen (1.0, 2, p, -1);

	fftlen = 0;							// Let gwsetup find the fft length

	*res = TRUE;						/* Assume it is a prime */ 

	if (dk >= 1.0) {					// Setup the DWT mode
		gwsetup (dk, 2, n, +1, fftlen); 
	}
	else {								// Setup the generic mode
		gwsetup_general_mod_giant (N, fftlen);
	}

/* Init tmp = (N-1)/2 to compute a^(N-1)/2 mod N */

	tmp = popg((Nlen >> 5) + 4);
	tmp2 = popg((Nlen >> 5) + 4);
	gtog (N, tmp);
	iaddg (-1, tmp);
	gshiftright (1, tmp);
	Nlen = bitlen (tmp);

/* Init filename */

	tempFileName (filename, 'z');

/* Get the current time */
/* Allocate memory */

	x = gwalloc ();

/* Optionally resume from save file and output a message */
/* indicating we are resuming a test */

	if (fileExists (filename) && readFromFile (filename, &bit, x, NULL)) {
		char	fmt_mask[80];
		double	pct;
		pct = trunc_percent (bit * 100.0 / Nlen);
		sprintf (fmt_mask,
			 "Resuming Proth prime test of %%s at bit %%ld [%%.%df%%%%]\n",
			 PRECISION);
		sprintf (buf, fmt_mask, str, bit, pct);
		OutputStr (buf);
	}

/* Otherwise, output a message indicating we are starting test */

	else {
		sprintf (buf, "Starting Proth prime test of %s\n", str);
		OutputStr (buf);
		bit = 1;
		dbltogw ((double) a, x);
	}

/* Output a message about the FFT length and the Proth base. */

	gwfft_description (fft_desc);
	sprintf (buf, "Using %s, a = %d\n", fft_desc, a);
	OutputStr (buf);
	LineFeed();
	sprintf (buf, "Using %s, a = %d\n", fft_desc, a);
	if (verbose)
		writeResults (buf);
	ReplaceableLine (1);	/* Remember where replaceable line is */

/* Do the Proth test */

	gwsetmulbyconst (a);
	iters = 0;
	while (bit < Nlen) {

/* Error check the last 50 iterations, before writing an */
/* intermediate file (either user-requested stop or a */
/* 30 minute interval expired), and every 128th iteration. */

		stopping = stopCheck ();
		echk = stopping || ERRCHK || (bit <= 50) || (bit >= Nlen-50);
		if (((bit & 127) == 0) || ((bit + 10) == maxerr_point)) {
			echk = 1;
			time (&current_time);
			saving = ((current_time - start_time > write_time) || ((bit + 10) == maxerr_point));
		} else
			saving = 0;

/* Process this bit */

		gwstartnextfft (!stopping && !saving && !((interimFiles && (bit+1) % interimFiles == 0)) &&
			!(interimResidues && ((bit+1) % interimResidues < 2)) && 
			(bit >= 30) && (bit < Nlen-31) && !maxerr_recovery_mode);

		if (bitval (tmp, Nlen-bit-1)) {
			gwsetnormroutine (0, echk, 1);
		} else {
			gwsetnormroutine (0, echk, 0);
		}
		if (GENERAL_MOD || ((bit > 30) && (bit < Nlen-30) && (!maxerr_recovery_mode || (bit != maxerr_point))))
			gwsquare (x);
		else {
			if (k_is_square)
				gwsquare_carefully (x);
			else
				careful_squaring (x);
//			careful_squaring (x);
			if (maxerr_recovery_mode && (bit == maxerr_point)) {
				maxerr_point = 0;
				maxerr_recovery_mode = FALSE;
			}
		}


/* If the sum of the output values is an error (such as infinity) */
/* then raise an error. */

		if (gw_test_illegal_sumout ()) {
			sprintf (buf, ERRMSG0, bit, Nlen, ERRMSG1A);
			OutputBoth (buf);
			sleep5 = TRUE;
			goto error;
		}

/* Check that the sum of the input numbers squared is approximately */
/* equal to the sum of unfft results.  Since this check may not */
/* be perfect, check for identical results after a restart. */

		if (gw_test_mismatched_sums ()) {
			static unsigned long last_bit = 0;
			static double last_suminp = 0.0;
			static double last_sumout = 0.0;
			double suminp, sumout;
			suminp = gwsuminp (x);
			sumout = gwsumout (x);
			if (bit == last_bit &&
			    suminp == last_suminp &&
			    sumout == last_sumout) {
				writeResults (ERROK);
				saving = 1;
			} else {
				char	msg[80];
				sprintf (msg, ERRMSG1B, suminp, sumout);
				sprintf (buf, ERRMSG0, bit, Nlen, msg);
				OutputBoth (buf);
				last_bit = bit;
				last_suminp = suminp;
				last_sumout = sumout;
				sleep5 = TRUE;
				goto error;
			}
		}

/* Check for excessive roundoff error  */

		if (MAXERR > 0.40) {
			static unsigned long last_bit = 0;
			static double last_maxerr = 0.0;
			if (bit == last_bit &&
			    MAXERR == last_maxerr) {
				OutputBoth (ERROK);
				GWERROR = 0;
				OutputBoth (ERRMSG6);
				maxerr_recovery_mode = 1;
				sleep5 = FALSE;
				goto error;
			} else {
				char	msg[80];
				sprintf (msg, ERRMSG1C, MAXERR);
				sprintf (buf, ERRMSG0, bit, Nlen, msg);
				OutputBoth (buf);
				maxerr_point = last_bit = bit;
				last_maxerr = MAXERR;
				sleep5 = FALSE;
				goto error;
			}
		}

/* Keep track of maximum and minimum round off error */

		if (ERRCHK) {
			if (MAXERR < reallyminerr && bit > 30)
				reallyminerr = MAXERR;
			if (MAXERR > reallymaxerr)
				reallymaxerr = MAXERR;
		}

/* That iteration succeeded, bump counters */

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
			if (! writeToFile (filename, bit, x, NULL)) {
				sprintf (buf, WRITEFILEERR, filename);
				OutputBoth (buf);
				if (write_time > 600) write_time = 600;
			}
			time (&start_time);

/* If an escape key was hit, write out the results and return */

			if (stopping) {
				pushg(2);
				free(gk);
				free(N);
				gwfree (x);
				gwdone ();
				*res = FALSE;		// To avoid credit message !
				return (FALSE);
			}
		}

/* Output the 64-bit residue at specified interims.  Also output the */
/* residues for the next iteration so that we can compare our */
/* residues to programs that start counter at zero or one. */

		if (interimResidues && bit % interimResidues < 2) {
				gwtogiant (x, tmp2);			// The modulo reduction is done here
			iaddg (1, tmp2);					// Compute the (unnormalized) residue
			if (abs(tmp2->sign) < 2)			// make a 32 bit residue correct !!
				tmp2->n[1] = 0;
			sprintf (buf, 
				 "%s interim residue %08lX%08lX at bit %ld\n",
				 str, tmp2->n[1], tmp2->n[0], bit);
			OutputBoth (buf);
		}

/* Write a save file every "interimFiles" iterations. */

		if (interimFiles && bit % interimFiles == 0) {
			char	interimfile[20];
			sprintf (interimfile, "%.8s.%03d",
				 filename, bit / interimFiles);
			if (! writeToFile (interimfile, bit, x, NULL)) {
				sprintf (buf, WRITEFILEERR, interimfile);
				OutputBoth (buf);
			}
		}
	}

/* See if we've found a Proth prime.  If not, format a 64-bit residue. */

	gwtogiant (x, tmp);				// The modulo reduction is done here
	iaddg (1, tmp);					// Compute the (unnormalized) residue

	if (gcompg (N, tmp) != 0) {
		*res = FALSE;				/* Not a prime */
		if (abs(tmp->sign) < 2)		// make a 32 bit residue correct !!
			tmp->n[1] = 0;
		sprintf (res64, "%08lX%08lX", tmp->n[1], tmp->n[0]);
	}

	pushg(2);
	free(gk);
	free(N);
	gwfree (x);


/* Print results.  Do not change the format of this line as Jim Fougeron of */
/* PFGW fame automates his QA scripts by parsing this line. */


	if (username[0] != '\0') {	// Test if LLRnet output format is required
		sprintf (buf, "user=%s\n", username);
		writeResults (buf);
		OutputTimeStampOnDisk ();
	}

	if (*res)
		sprintf (buf, "%s is prime!", str); 
	else
		sprintf (buf, "%s is not prime.  Proth RES64: %s", str, res64);

#if defined(WIN32) && !defined(_CONSOLE)

	sprintf (buf+strlen(buf), "  Time : "); 
	ReplaceableLine (2);	/* Replace line */ 

#else

	clearline(100);

#ifdef _CONSOLE
	OutputBoth(buf);
#else
	if (*res) {
		OutputStr("\033[7m");
		OutputBoth(buf);
		OutputStr("\033[0m");
	}
	else
		OutputBoth(buf);
#endif

	sprintf (buf, "  Time : "); 

#endif

/* Output the final timings */

	end_timer (1);
	write_timer (buf+strlen(buf), 1, TIMER_CLR | TIMER_NL); 
	OutputBoth(buf);

/* Cleanup and return */

	gwdone ();
	_unlink (filename);
	return (TRUE);

/* An error occured, sleep, then try restarting at last save point. */

error:
	pushg(2);
	gwfree (x);
	gwdone ();
	*res = FALSE;

	if ((abonillsum && gw_test_illegal_sumout()) || 
		(abonmismatch && gw_test_mismatched_sums ()) || 
		(abonroundoff && MAXERR > 0.40)) {	// Abort...
		sprintf (buf,ERRMSG5,str);
		OutputBoth (buf);
		free(gk);
		free(N);
		_unlink (filename);
		return (TRUE);
	}

/* Output a message saying we are restarting */

	if (sleep5) OutputBoth (ERRMSG2);
	OutputBoth (ERRMSG3);

/* Sleep five minutes before restarting */

	if (sleep5 && ! SleepFive ()) return (FALSE);

/* Restart */

	goto restart;
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
for a Lucas-Lehmer test with exponent 2*p.

Moreover, using an interim result of the main loop, we have all the info to get the PRP test result of N'/5!

Here is the algorithm in pseudo - C :

GMtest() {

Compute M(p), N(p), N'(p), N'/5;
Compute a such Jacobi(a, N) = -1; // (the Proth base)

x = a;

for (i=1; i<=p-1; i++) {		// This main loop implies only squarings modulo 2^(2*p) + 1, which is optimal!
	x = x*x modulo M;
	if (x == (p-1)/2) y = x;
}

// We have now x = a^(2^(n-1)) modulo M and y = a^(2^((n-1)/2)) modulo M.
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
// We need to test if a^(N'/5) == a modulo N'/5, which implies a^N' = a^5 modulo N'/5 and finally :
// R' = a^(N'-1] = a^4 modulo N'/5 (a and N'/5 beeing co-prime).
// But, N'-1 = 2*[2^(p-1) - (sign)*2^((p-1)/2)], so :

if (sign == 1)
	R' = x*x*y^-1*y^-1; // all computed modulo N'/5
else if (sign == -1)
	R' = x*x*y*y;		// all computed modulo N'/5
		
	if (R' == a^4 modulo N'/5)
		printf("N'/5 is a-PRP!");
	else
		printf("N'/5 is not prime.");

} // End

Finally, to be efficient, this prime search needs eliminating as many candidates as possible by prefactoring.
To do that I adapted the George Woltman's "factor32.asm" code to find the factors of 4^p+1, and then, of
N and/or N'/5.
This feature is included here, and allows to factor up to 2^86, if really needed.
Also, there is an option to do factoring only jobs.

Jean Penné, March 30 2006

****************************************************************************************************************/

int isGMNP ( 
	char *sgk,
	unsigned long n,
	int	*res) 
{ 
	unsigned long iters; 
	unsigned long fftlen, ubx, uby, hi, lo, atemp, abits = 0; 
	unsigned long bit, bits, explen, expx, expy, loopshift, howfar; 
	gwnum	x, y; 
	giant	tmp, tmp2, tmp3, apow4; 
	char	filename[20], buf[sgkbufsize+256], 
		str[sgkbufsize+256], strp[sgkbufsize+256], fft_desc[100]; 
	long	write_time = DISK_WRITE_TIME * 60; 
	int	echk, saving, stopping, sign, fisok, fres, fhandle = 0, inc = +1; 
	time_t	start_time, current_time; 
	double	reallyminerr = 1.0; 
	double	reallymaxerr = 0.0; 
	double dk;

	if (!facto && !isPrime (n) || n == 2) {
		sprintf (buf, "Gaussian-Mersenne prime test not done because %d is not an odd prime.\n", n); 
		OutputBoth (buf); 
		*res = FALSE;
		return(TRUE);
	}

	sign = (((n&7) == 3) || ((n&7) == 5))? 1 : 0;	// 1 if positive, 0 if negative
	sprintf (str, "2^%lu%c%s+1",  n, (sign) ? '+' : '-', sgk);	// Number N to test, as a string
	sprintf (strp, "(2^%lu%c%s+1)/5",  n, (sign) ? '-' : '+', sgk);	// Number N' to test, as a string

	bits = 2*n;							// Bit length of M = N*N'
	M = newgiant ((bits>>4) + 8);		// Allocate memory for M = N*N'
	N = newgiant ((bits>>5) + 8);		// Allocate memory for N
	NP = newgiant ((bits>>5) + 8);		// Allocate memory for N'
	gk = newgiant ((bits>>5) + 8);		// Allocate memory for gk
	testn =  newgiant ((bits>>3) + 16);	// For factoring
	testnp = newgiant ((bits>>3) + 16);
	testf  = newgiant (100);
	testx  = newgiant (100);

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

	uldivg (5, NP);					// NP = N'/5
	setone (M);
	gshiftleft (2*n, M);			// M  = 2^(2*n)
	iaddg (1, M);					// M  = N*N' = 2^(2*n) + 1

	Nlen = 2*n+1; 

	res1 = res2 = 1;				// Assume N and NP are prime...

	if (!facto && N->sign == 1 && NP->sign == 1) {	// N and NP are small numbers, so we make simple tests...
		a = 0;

		if (username[0] != '\0') {	// Test if LLRnet output format is required
			sprintf (buf, "user=%s\n", username);
			writeResults (buf);
			OutputTimeStampOnDisk ();
		}

		if (isPrime (N->n[0])) {
#ifndef WIN32
			OutputStr("\033[7m");
#endif
			sprintf (buf, "%s = %lu is prime! (trial divisions)\n", str, N->n[0]); 
			OutputBoth (buf);
#ifndef WIN32
			OutputStr("\033[0m");
#endif
			sprintf (res64, "0000000000000000");
		}
		else	{
			sprintf (buf, "%s = %lu is  not prime. (trial divisions)\n", str, N->n[0]); 
			OutputBoth (buf);
			sprintf (res64, "????????????????");
			res1 = 0;
		}
		if (isPrime (NP->n[0])) {
#ifndef WIN32
			OutputStr("\033[7m");
#endif
			sprintf (buf, "%s = %lu is prime! (trial divisions)\n", strp, NP->n[0]); 
			OutputBoth (buf);
#ifndef WIN32
			OutputStr("\033[0m");
#endif
			sprintf (res64, "0000000000000000");
		}
		else	{
			sprintf (buf, "%s = %lu is  not prime. (trial divisions)\n", strp, NP->n[0]); 
			OutputBoth (buf);
			sprintf (res64, "????????????????");
			res2 = 0;
		}
		*res = (res1 || res2);
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

		tempFileName (filename, 'g');

		if (fileExists (filename)) {
			/* Open the intermediate file */
			fhandle = _open (filename, _O_BINARY | _O_RDONLY);
		}

		fres = resn && resnp;

		if (fres || n <  LOWFACTORLIMIT || primeSieve (n, facfrom, facto, str, strp, &fres, fhandle)) {
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

// Test if we are resuming a prime test.

	tempFileName (filename, 'z');
	if (fileExists (filename))
		goto primetest;

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

		tempFileName (filename, 'f');

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

primetest:

 	dk = 1.0;						// k == 1 for the modulo N*N'

/* Compute the base for the Proth algorithm. */
 
	if ((a = genProthBase(gk, (n+1)/2)) < 0) {
		if (a == -1)
			sprintf (buf, "Cannot compute a to test %s...\nThis is surprising, please, let me know that!!\nMy E-mail is jpenne@wanadoo.fr\n", str);
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


	gwsetmaxmulbyconst (a);

restart:
	clear_timers ();
	start_timer (0);
	start_timer (1);
	time (&start_time);

/* Assume intermediate results of the length of N*N'. */ 

	fftlen = 0;							// Let gwsetup find the fft length

	*res = TRUE;						/* Assume it is a prime */ 

	gwsetup (dk, 2, 2*n, +1, fftlen); 			// Setup the DWT mode

	expx = n-1;
	expy = expx/2;

/* More initializations... */

	tmp = popg((Nlen >> 4) + 8);
	tmp2 = popg((Nlen >> 4) + 8);
	tmp3 = popg((Nlen >> 4) + 8);
	apow4 = popg(16);
	itog (a, apow4);
	imulg (a, apow4);
	imulg (a, apow4);
	imulg (a, apow4);

/* Init filename */

	tempFileName (filename, 'z');

/* Get the current time */
/* Allocate memory */

	x = gwalloc ();
	y = gwalloc ();

/* Optionally resume from save file and output a message */
/* indicating we are resuming a test */

/* ubx and uby are the current units bit positions, in x and y, respectively.*/

	if (fileExists (filename) && gmreadFromFile (filename, &bit, &ubx, &uby, x, y)) {
		char	fmt_mask[80];
		double	pct;
		pct = trunc_percent (bit * 100.0 / expx);
		sprintf (fmt_mask,
			 "Resuming Proth prime test of %%s at bit %%ld [%%.%df%%%%]\n",
			 PRECISION);
		sprintf (buf, fmt_mask, str, bit, pct);
		OutputStr (buf);
	}

/* Otherwise, output a message indicating we are starting test */

	else {
		sprintf (buf, "Starting Proth prime test of %s\n", str);
		OutputStr (buf);
		bit = 1;
//		dbltogw ((double) a, x);

/* Compute a random shift for the initial value */

		srand ((unsigned int) time (NULL));
		ubx = (rand() << 16) + rand();
		if (CPU_FLAGS & CPU_RDTSC) { rdtsc(&hi,&lo); ubx += lo; }
		atemp = a;
		while (atemp) {						// Compute the bit length of the Proth base a
			atemp >>= 1;
			abits++;
		}
		ubx = ubx % (bits-abits);			// Be sure that the shift is not too large...
		uby = 0;


/* Compute the left shifted initial value */

		itog (a, tmp3);
		gshiftleft (ubx, tmp3);

		gianttogw (tmp3, x);
		gianttogw (M, y);
	}

/* Output a message about the FFT length and the Proth base. */

	gwfft_description (fft_desc);
	sprintf (buf, "Using %s, a = %d\n", fft_desc, a);
	OutputStr (buf);
	LineFeed();
	sprintf (buf, "Using %s, a = %d\n", fft_desc, a);
	if (verbose)
		writeResults (buf);
	ReplaceableLine (1);	/* Remember where replaceable line is */

/* Init the title */

	if (strlen (str) < 40)
		title (str);
	else					// Avoid array overflow...
		title("Running in Big k mode...");

/* Do the Proth test */

	iters = 0;
	loopshift = (bit >= expy) ? expy : 0;
	explen = (bit >= expy) ? expx : expy;
	while (bit <= expx) {

/* Error check the last 50 iterations, before writing an */
/* intermediate file (either user-requested stop or a */
/* 30 minute interval expired), and every 128th iteration. */

		stopping = stopCheck ();
		echk = stopping || ERRCHK || (bit <= (50+loopshift)) || (bit >= explen-50);
		if (((bit & 127) == 0) || ((bit + 10) == maxerr_point)) {
			echk = 1;
			time (&current_time);
			saving = ((current_time - start_time > write_time) || ((bit + 10) == maxerr_point));
		} else
			saving = 0;

/* Process this bit */

		gwstartnextfft (!stopping && !saving && !((interimFiles && (bit+1) % interimFiles == 0)) &&
			!(interimResidues && ((bit+1) % interimResidues < 2)) && 
			(bit >= (30+loopshift)) && (bit < explen-31) && !maxerr_recovery_mode);


		gwsetnormroutine (0, echk, 0);
		if (GENERAL_MOD || ((bit > (30+loopshift)) && (bit < explen-30) && (!maxerr_recovery_mode || (bit != maxerr_point))))
			gwsquare (x);
		else {
			careful_squaring (x);
			maxerr_recovery_mode = FALSE;
			maxerr_point = 0;
		}

		ubx <<= 1;
		if (ubx >= bits) ubx -= bits;		// Compute the doubled shift modulo 2*n

		if (bit == expy) {
			gwcopy (x, y);
			uby = ubx;
			loopshift = expy;
			explen = expx;
		}


/* If the sum of the output values is an error (such as infinity) */
/* then raise an error. */

		if (gw_test_illegal_sumout ()) {
			sprintf (buf, ERRMSG0, bit, explen, ERRMSG1A);
			OutputBoth (buf);
			sleep5 = TRUE;
			goto error;
		}

/* Check that the sum of the input numbers squared is approximately */
/* equal to the sum of unfft results.  Since this check may not */
/* be perfect, check for identical results after a restart. */

		if (gw_test_mismatched_sums ()) {
			static unsigned long last_bit = 0;
			static double last_suminp = 0.0;
			static double last_sumout = 0.0;
			double suminp, sumout;
			suminp = gwsuminp (x);
			sumout = gwsumout (x);
			if (bit == last_bit &&
			    suminp == last_suminp &&
			    sumout == last_sumout) {
				writeResults (ERROK);
				saving = 1;
			} else {
				char	msg[80];
				sprintf (msg, ERRMSG1B, suminp, sumout);
				sprintf (buf, ERRMSG0, bit, explen, msg);
				OutputBoth (buf);
				last_bit = bit;
				last_suminp = suminp;
				last_sumout = sumout;
				sleep5 = TRUE;
				goto error;
			}
		}

/* Check for excessive roundoff error  */

		if (MAXERR > 0.40) {
			static unsigned long last_bit = 0;
			static double last_maxerr = 0.0;
			if (bit == last_bit &&
			    MAXERR == last_maxerr) {
				OutputBoth (ERROK);
				GWERROR = 0;
				OutputBoth (ERRMSG6);
				maxerr_recovery_mode = 1;
				sleep5 = FALSE;
				goto error;
			} else {
				char	msg[80];
				sprintf (msg, ERRMSG1C, MAXERR);
				sprintf (buf, ERRMSG0, bit, explen, msg);
				OutputBoth (buf);
				maxerr_point = last_bit = bit;
				last_maxerr = MAXERR;
				sleep5 = FALSE;
				goto error;
			}
		}

/* Keep track of maximum and minimum round off error */

		if (ERRCHK) {
			if (MAXERR < reallyminerr && bit > 30)
				reallyminerr = MAXERR;
			if (MAXERR > reallymaxerr)
				reallymaxerr = MAXERR;
		}

/* That iteration succeeded, bump counters */

		bit++;
		iters++;

/* Print a message every so often */

		if (bit % ITER_OUTPUT == 0) {
			char	fmt_mask[80];
			double	pct;
			pct = trunc_percent (bit * 100.0 / expx);
			if (strlen (str) < 40) {
				sprintf (fmt_mask, "%%.%df%%%% of %%s", PRECISION);
				sprintf (buf, fmt_mask, pct, str);
			}
			else {
				sprintf (fmt_mask, "%%.%df%%%% of %%ld", PRECISION);
				sprintf (buf, fmt_mask, pct, explen);
			}
			title (buf);
			ReplaceableLine (2);	/* Replace line */
			sprintf (fmt_mask,
				 "%%s, bit: %%ld / %%ld [%%.%df%%%%]",
				 PRECISION);
			sprintf (buf, fmt_mask, str, bit, expx, pct);
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
			if (! gmwriteToFile (filename, bit, ubx, uby, x, y)) {
				sprintf (buf, WRITEFILEERR, filename);
				OutputBoth (buf);
				if (write_time > 600) write_time = 600;
			}
			time (&start_time);


/* If an escape key was hit, write out the results and return */

			if (stopping) {
				pushg(4);
				free(gk);
				free(N);
				free(NP);
				free(M);
				free(testn);
				free(testnp);
				free(testf);
				free(testx);
				gwfree (x);
				gwfree (y);
				gwdone ();
				*res = res1 = res2 = FALSE;		// To avoid credit message !
				return (FALSE);
			}
		}

/* Output the 64-bit residue at specified interims.  Also output the */
/* residues for the next iteration so that we can compare our */
/* residues to programs that start counter at zero or one. */

		if (interimResidues && bit >= expy && bit % interimResidues < 2) {

			setone (tmp3);					// Restore the value of x from the shifted one.
			gshiftleft (ubx, tmp3);
			invg (M,tmp3);
			gtog (M, testn);
			if (ubx&2)						// View if a sign change on x is necessary.
				subg (tmp3, testn);
			else
				gtog (tmp3, testn);

			gwtogiant (x, tmp3);			// The modulo reduction is done here
			mulg (tmp3, testn);
			specialmodg (testn);

			setone (tmp3);					// Restore the value of y from the shifted one.
			gshiftleft (uby, tmp3);
			invg (M,tmp3);
			gtog (M, testnp);
			if (uby&2)						// View if a sign change on y is necessary.
				subg (tmp3, testnp);
			else
				gtog (tmp3, testnp);
			gwtogiant (y, tmp3);			// The modulo reduction is done here
			mulg (tmp3, testnp);
			specialmodg (testnp);
			gtog (testn, tmp);
			gtog (testnp, tmp2);

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
			if (abs(tmp->sign) < 2)		// make a 32 bit residue correct !!
				tmp->n[1] = 0;
			sprintf (buf, 
				 "GM%ld interim residue %08lX%08lX at iteration %ld\n",
				 n, tmp->n[1], tmp->n[0], bit);
			OutputBoth (buf);
		}

/* Write a save file every "interimFiles" iterations. */

		if (interimFiles && bit % interimFiles == 0) {
			char	interimfile[20];
			sprintf (interimfile, "%.8s.%03d",
				 filename, bit / interimFiles);
			if (! gmwriteToFile (interimfile, bit, ubx, uby, x, y)) {
				sprintf (buf, WRITEFILEERR, interimfile);
				OutputBoth (buf);
			}
		}
	}

	setone (tmp3);					// Restore the value of x from the shifted one.
	gshiftleft (ubx, tmp3);
	invg (M,tmp3);
	gtog (M, testn);
	if (ubx&2)						// View if a sign change on x is necessary.
		subg (tmp3, testn);
	else
		gtog (tmp3, testn);

	gwtogiant (x, tmp3);			// The modulo reduction is done here
	mulg (tmp3, testn);
	specialmodg (testn);

	setone (tmp3);					// Restore the value of y from the shifted one.
	gshiftleft (uby, tmp3);
	invg (M,tmp3);
	gtog (M, testnp);
	if (uby&2)						// View if a sign change on y is necessary.
		subg (tmp3, testnp);
	else
		gtog (tmp3, testnp);
	gwtogiant (y, tmp3);			// The modulo reduction is done here
	mulg (tmp3, testnp);
	specialmodg (testnp);
	gtog (testn, tmp);
	gtog (testnp, tmp2);

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

	if (gcompg (N, tmp) != 0) {
		res1 = FALSE;				/* Not a prime */
		if (abs(tmp->sign) < 2)		// make a 32 bit residue correct !!
			tmp->n[1] = 0;
		sprintf (res64, "%08lX%08lX", tmp->n[1], tmp->n[0]);
	}


/* Print results.  Do not change the format of this line as Jim Fougeron of */
/* PFGW fame automates his QA scripts by parsing this line. */


	if (username[0] != '\0') {	// Test if LLRnet output format is required
		sprintf (buf, "user=%s\n", username);
		writeResults (buf);
		OutputTimeStampOnDisk ();
	}

	if (res1)
		sprintf (buf, "%s is prime!\n", str);
	else
		sprintf (buf, "%s is not prime.  Proth RES64: %s\n", str, res64);

#if defined(WIN32) && !defined(_CONSOLE)

	ReplaceableLine (2);	/* Replace line */ 
	OutputBoth (buf);

#else

	clearline(100);

#ifdef _CONSOLE
	OutputBoth(buf);
#else
	if (res1) {
		OutputStr("\033[7m");
		OutputBoth(buf);
		OutputStr("\033[0m");
	}
	else
		OutputBoth(buf);
#endif

	sprintf (buf, "  Time : "); 

#endif

	gtog (testn, tmp);
	gtog (testnp, tmp2);

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

	if (gcompg (tmp2, tmp) != 0) {
		subg (tmp2, tmp);
		res2 = FALSE;				/* Not a prime */
		if (abs(tmp->sign) < 2)		// make a 32 bit residue correct !!
			tmp->n[1] = 0;
		sprintf (res64, "%08lX%08lX", tmp->n[1], tmp->n[0]);
	}


/* Print results.  Do not change the format of this line as Jim Fougeron of */
/* PFGW fame automates his QA scripts by parsing this line. */

	if (res2)
		sprintf (buf, "%s is %d-PRP!", strp, a);
	else
		sprintf (buf, "%s is not prime.  RES64: %s", strp, res64);

#ifdef WIN32

	sprintf (buf+strlen(buf), "  Time: ");

#else

	if (res2) {
		OutputStr("\033[7m");
		OutputBoth (buf);
		OutputStr("\033[0m");
	}
	else
		OutputBoth (buf);
	sprintf (buf, "  Time: ");

#endif

/* Output the final timings */

	end_timer (1);
	write_timer (buf+strlen(buf), 1, TIMER_CLR | TIMER_NL); 
	OutputBoth (buf);

	*res = (res1 || res2);

/* Cleanup and return */


	pushg(4);
	free(gk);
	free(N);
	free(NP);
	free(M);
	free(testn);
	free(testnp);
	free(testf);
	free(testx);
	gwfree (x);
	gwfree (y);

	gwdone ();
	_unlink (filename);
	return (TRUE);

/* An error occured, sleep, then try restarting at last save point. */

error:
	pushg(4);
	gwfree (x);
	gwfree (y);
	gwdone ();

	if ((abonillsum && gw_test_illegal_sumout()) || 
		(abonmismatch && gw_test_mismatched_sums ()) || 
		(abonroundoff && MAXERR > 0.40)) {	// Abort...
		sprintf (buf,ERRMSG5,str);
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
		_unlink (filename);
		return (TRUE);
	}

/* Output a message saying we are restarting */

	if (sleep5) OutputBoth (ERRMSG2);
	OutputBoth (ERRMSG3);

/* Sleep five minutes before restarting */

	if (sleep5 && ! SleepFive ()) return (FALSE);

/* Restart */

	goto restart;
} 

static unsigned __int64 li;

int process_num (
	unsigned long format,
	char *sgk,
	unsigned long base,
	unsigned long n,
	int	incr,
	unsigned long shift,
	int	*res)
{
	int	retval;
	unsigned long ninput = n, binput = base;

	while (!(base&1) && base > 2) {	// Divide the base by two as far as possible
		base >>= 1;
		n += ninput;
	}

	if (base != 2) {				// Test if the base was a power of two
		base = binput;				// Do not modify it if no
		n = ninput;
	}

	if (format == ABCGM)
		return (isGMNP (sgk, n, res));

	if (base == 2 && !IniGetInt (INI_FILE, "ForcePRP", 0) && ((incr == -1) || (incr == +1))) {
		if (incr == -1)
			retval = isLLRP (format, sgk, n, shift, res);
		else
			retval = isProthP (format, sgk, n, shift, res);
	}
	else {
		retval = IsPRP (format, sgk, base, n, incr, shift, res);
	}
	return (retval);
}

char	outpf[] = "gqplus.res", outmf[] = "gqminus.res";
char	gqpstring[] = "ABC (2^$a+2^(($a+1)/2)+1)/5\n";
char	gqmstring[] = "ABC (2^$a-2^(($a+1)/2)+1)/5\n";


void primeContinue ()
{

	int	workdone, work, nargs;
	unsigned long format, shift;
	char *pinput;

/* If we are done, return */

	workdone = IniGetInt (INI_FILE, "WorkDone", 1);

	if (workdone == 1) return;

/* Set appropriate priority */

	SetPriority ();

/* Case off the work type */

	work = IniGetInt (INI_FILE, "Work", 0);

/* Handle a newpgen output file */

	if (work == 0) {
	    char	inputfile[80], outputfile[80], sgk[sgkbufsize], buff[sgkbufsize+256];
		char	outbuf[sgkbufsize+256];
	    FILE *fd;
	    unsigned long i, chainlen, n, base, k, nfudge, nn;
	    int	firstline, line, outfd, outfdp, outfdm, res, uptoline, incr, sign;
	    char c;

	    IniGetString (INI_FILE, "PgenInputFile", inputfile, 80, NULL);
	    IniGetString (INI_FILE, "PgenOutputFile", outputfile, 80, NULL);
	    IniGetString (INI_FILE, "LLRnetuser", username, 100, NULL);
	    firstline = IniGetInt (INI_FILE, "PgenLine", 1);
	    verbose = IniGetInt (INI_FILE, "Verbose", 0);
	    fd = fopen (inputfile, "r");

	    if (fd == NULL) {
			IniWriteInt (INI_FILE, "WorkDone", 1);
			return;
	    }

		uptoline = IniGetInt(INI_FILE, "VerifyUpToLine", 0);
		testgm  = IniGetInt(INI_FILE, "TestGM", 1);
		testgq  = IniGetInt(INI_FILE, "TestGQ", 0);
		testfac  = IniGetInt(INI_FILE, "TestFac", 0);
		facfrom =  IniGetInt(INI_FILE, "FacFrom", 0);
		facto =  IniGetInt(INI_FILE, "FacTo", 0);

		if (fgets (buff, sgkbufsize+128, fd) == NULL) {
			IniWriteInt (INI_FILE, "WorkDone", 1);
			return;
	    }


/* A new option to create interim save files every N iterations. */
/* This allows two machines to simultanously work on the same exponent */
/* and compare results along the way. */

	interimFiles = IniGetInt (INI_FILE, "InterimFiles", 0);
	interimResidues = IniGetInt (INI_FILE, "InterimResidues", interimFiles);

/* Option to slow down the program by sleeping after every iteration.  You */
/* might use this on a laptop or a computer running in a hot room to keep */
/* temperatures down and thus reduce the chance of a hardware error.  */

	throttle = IniGetInt (INI_FILE, "Throttle", 0);

		// Set termination on error conditions

		abonillsum = IniGetInt(INI_FILE, "Abortonillsum", 0);
		abonmismatch = IniGetInt(INI_FILE, "Abortonmismatch", 0);
		abonroundoff = IniGetInt(INI_FILE, "Abortonroundoff", 0);
		if (IniGetInt(INI_FILE, "Abortonerror", 0))
			abonillsum = abonmismatch = abonroundoff = 1;

		// Special code to process some ABC file formats...

		if (!strncmp (buff, "ABC", 3)) {
			for (pinput=buff+3; *pinput && isspace(*pinput); pinput++);

			if (!strncmp (pinput, cwstring, strlen (cwstring))) {
				format = ABCCW;
			}
			else if (!strncmp (pinput, ffstring, strlen (ffstring))) {
				format = ABCFF;
			}
			else if (!strncmp (pinput, gmstring, strlen (gmstring))) {
				format = ABCGM;
			}
			else if (sscanf(pinput, fkpstring, &k, &incr) == 2) {
				format = ABCFKGS;
                                sprintf(sgk, "%lu", k);
			}
			else if (sscanf(pinput, fkmstring, &k, &incr) == 2) {
				format = ABCFKGS;
				incr = - incr;
                                sprintf(sgk, "%lu", k);
			}
			else if (sscanf(pinput, fkpstring, &k) == 1) { 
				format = ABCFKAS;
                                sprintf(sgk, "%lu", k);
			}
			else if (sscanf(pinput, fbpstring, &base, &incr) == 2) {
				format = ABCFBGS;
			}
			else if (sscanf(pinput, fbmstring, &base, &incr) == 2) {
				format = ABCFBGS;
				incr = - incr;
			}
			else if (sscanf(pinput, fbpstring, &base) == 1) { 
				format = ABCFBAS;
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
			else if (!strncmp (pinput, abcastring, strlen (ffstring))) {
				format = ABCVARAS;
			}
			else if (strncmp (pinput, ckstring, strlen (ckstring))) {
				sprintf (outbuf, "%s : Unknown ABC format.\n", buff);
				OutputStr (outbuf);
				IniWriteInt (INI_FILE, "WorkDone", 1);
				return;
			}

			if (format == ABCGM) {
				if (!facto)
					sprintf (pinput+strlen (gmstring),
						" // Let GM(p) = (1+/-i)^p-1, GQ(p) = ((1+/-i)^p+1)/(2+/-i) if p>3, (1+/-i)^p+1 if p<=3\n");
				if (!facto && !fileExists (outpf)) {
					outfdp = _open (outpf, _O_TEXT | _O_RDWR | _O_CREAT, 0666);
					if (outfdp) {
						_write (outfdp, gqpstring, strlen (gqpstring));
						_close (outfdp);
					}
				}
				if (!facto && !fileExists (outmf)) {
					outfdm = _open (outmf, _O_TEXT | _O_RDWR | _O_CREAT, 0666);
					if (outfdm) {
						_write (outfdm, gqmstring, strlen (gqmstring));
						_close (outfdm);
					}
				}
			}
			if (!fileExists (outputfile)) {
				outfd = _open (outputfile, _O_TEXT | _O_RDWR | _O_CREAT, 0666);
				if (outfd) {
					_write (outfd, buff, strlen (buff));
					_close (outfd);
				}
			}

/* Process each line in the output file */

			for (line = 1; ; line++) {

/* Read the line, break at EOF */

				if (fgets (buff, sgkbufsize+256, fd) == NULL) {
					IniWriteInt (INI_FILE, "WorkDone", 1);
					break;
				}

/* Skip this line if requested (we processed it on an earlier run) */

				if (line < firstline) continue;

				shift = 0;			// Only one value for the k multiplier

				if (format == ABCCW) {			// Cullen/Woodall
					if (sscanf (buff, "%lu %lu %d", &n, &base, &incr) != 3)
						continue;				// Skip invalid line
					sprintf (sgk, "%lu", n);
					if (! process_num (format, sgk, base, n, incr, shift, &res)) goto done;
					if (res) {
						outfd = _open (outputfile, _O_TEXT | _O_RDWR | _O_APPEND | _O_CREAT, 0666);
						if (outfd) {
							sprintf (outbuf, "%lu %lu %d\n", n, base, incr); 
							_write (outfd, outbuf, strlen (outbuf));
							_close (outfd);
						}
					}
				}
				else if (format == ABCFF)	{	// FermFact output
												// allow k to be a big integer
					if (sscanf (buff, "%s %lu", sgk, &n) != 2)
						continue;				// Skip invalid line
					if (!isDigitString(sgk))
						continue;				// Skip invalid line
					if (! process_num (format, sgk, 2, n, +1, shift, &res)) goto done;
					if (res) {
						outfd = _open (outputfile, _O_TEXT | _O_RDWR | _O_APPEND | _O_CREAT, 0666);
						if (outfd) {
							sprintf (outbuf, "%s %lu\n", sgk, n); 
							_write (outfd, outbuf, strlen (outbuf));
							_close (outfd);
						}
					}
				}
				else if (format == ABCFKGS)	{	// Fixed k:  b and n specified on each input line
					if (sscanf (buff, "%lu %lu", &base, &n) != 2)
						continue;				// Skip invalid line
					if (! process_num (format, sgk, base, n, incr, shift, &res)) goto done;
					if (res) {
						outfd = _open (outputfile, _O_TEXT | _O_RDWR | _O_APPEND | _O_CREAT, 0666);
						if (outfd) {
							sprintf (outbuf, "%lu %lu\n", base, n); 
							_write (outfd, outbuf, strlen (outbuf));
							_close (outfd);
						}
					}
				}
				else if (format == ABCFKAS)	{	// Fixed k:  b, n, and c specified on each input line
					if (sscanf (buff, "%lu %lu %d", &base, &n, &incr) != 3)
						continue;				// Skip invalid line
					if (!isDigitString(sgk))
						continue;				// Skip invalid line
					if (! process_num (format, sgk, base, n, incr, shift, &res)) goto done;
					if (res) {
						outfd = _open (outputfile, _O_TEXT | _O_RDWR | _O_APPEND | _O_CREAT, 0666);
						if (outfd) {
							sprintf (outbuf, "%lu %lu %d\n", base, n, incr); 
							_write (outfd, outbuf, strlen (outbuf));
							_close (outfd);
						}
					}
				}
				else if (format == ABCFBGS)	{	// Fixed b:  k and n specified on each input line
					if (sscanf (buff, "%s %lu", sgk, &n) != 2)
						continue;				// Skip invalid line
					if (!isDigitString(sgk))
						continue;				// Skip invalid line
					if (! process_num (format, sgk, base, n, incr, shift, &res)) goto done;
					if (res) {
						outfd = _open (outputfile, _O_TEXT | _O_RDWR | _O_APPEND | _O_CREAT, 0666);
						if (outfd) {
							sprintf (outbuf, "%s %lu\n", sgk, n); 
							_write (outfd, outbuf, strlen (outbuf));
							_close (outfd);
						}
					}
				}
				else if (format == ABCFBAS)	{	// Fixed b:  k, n, and c specified on each input line
					if (sscanf (buff, "%s %lu %d", sgk, &n, &incr) != 3)
						continue;				// Skip invalid line
					if (!isDigitString(sgk))
						continue;				// Skip invalid line
					if (! process_num (format, sgk, base, n, incr, shift, &res)) goto done;
					if (res) {
						outfd = _open (outputfile, _O_TEXT | _O_RDWR | _O_APPEND | _O_CREAT, 0666);
						if (outfd) {
							sprintf (outbuf, "%s %lu %d\n", sgk, n, incr); 
							_write (outfd, outbuf, strlen (outbuf));
							_close (outfd);
						}
					}
				}
				else if (format == ABCFNGS)	{	// Fixed n:  k and b specified on each input line
					if (sscanf (buff, "%s %lu", sgk, &base) != 2)
						continue;				// Skip invalid line
					if (!isDigitString(sgk))
						continue;				// Skip invalid line
					if (! process_num (format, sgk, base, n, incr, shift, &res)) goto done;
					if (res) {
						outfd = _open (outputfile, _O_TEXT | _O_RDWR | _O_APPEND | _O_CREAT, 0666);
						if (outfd) {
							sprintf (outbuf, "%s %lu\n", sgk, base); 
							_write (outfd, outbuf, strlen (outbuf));
							_close (outfd);
						}
					}
				}
				else if (format == ABCFNAS)	{	// Fixed n:  k, b, and c specified on each input line
					if (sscanf (buff, "%s %lu %d", sgk, &base, &incr) != 3)
						continue;				// Skip invalid line
					if (!isDigitString(sgk))
						continue;				// Skip invalid line
					if (! process_num (format, sgk, base, n, incr, shift, &res)) goto done;
					if (res) {
						outfd = _open (outputfile, _O_TEXT | _O_RDWR | _O_APPEND | _O_CREAT, 0666);
						if (outfd) {
							sprintf (outbuf, "%s %lu %d\n", sgk, base, incr); 
							_write (outfd, outbuf, strlen (outbuf));
							_close (outfd);
						}
					}
				}
				else if (format == ABCVARGS)	{	// k, b, and n specified on each input line
					if (sscanf (buff, "%s %lu %lu", sgk, &base, &n) != 3)
						continue;				// Skip invalid line
					if (!isDigitString(sgk))
						continue;				// Skip invalid line
					if (! process_num (format, sgk, base, n, incr, shift, &res)) goto done;
					if (res) {
						outfd = _open (outputfile, _O_TEXT | _O_RDWR | _O_APPEND | _O_CREAT, 0666);
						if (outfd) {
							sprintf (outbuf, "%s %lu\n", sgk, base, n); 
							_write (outfd, outbuf, strlen (outbuf));
							_close (outfd);
						}
					}
				}
				else if (format == ABCVARAS)	{	// k, b, n, and c specified on each input line
					if (sscanf (buff, "%s %lu %lu %d", sgk, &base, &n, &incr) != 4)
						continue;				// Skip invalid line
					if (!isDigitString(sgk))
						continue;				// Skip invalid line
					if (! process_num (format, sgk, base, n, incr, shift, &res)) goto done;
					if (res) {
						outfd = _open (outputfile, _O_TEXT | _O_RDWR | _O_APPEND | _O_CREAT, 0666);
						if (outfd) {
							sprintf (outbuf, "%s %lu %lu %d\n", sgk, base, n, incr); 
							_write (outfd, outbuf, strlen (outbuf));
							_close (outfd);
						}
					}
				}
				else if (format == ABCGM)	{	// Gaussian Mersenne
					if ((nargs = sscanf (buff, "%lu %lu %lu", &n, &facn, &facnp)) < 1)
						continue;				// Skip invalid line
					else if (nargs == 1)		// Not prefactored.
						facn = facnp = 0;
					else if (nargs == 2) {		// Second argument is how far already factored, in bits)
						if (!facfrom)
							facfrom = facn;
						facn = facnp = 0;
					}
					sprintf (sgk, "2^%lu", (n+1)/2);
					if (! process_num (format, sgk, 2, n, +1, shift, &res)) goto done;
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
					if (res) {
						sign = (((n&7) == 3) || ((n&7) == 5))? 1 : 0;	// 1 if positive, 0 if negative
						outfd = _open (outputfile, _O_TEXT | _O_RDWR | _O_APPEND | _O_CREAT, 0666);
						if (outfd) {
							if (facto)
								if (n >= LOWFACTORLIMIT)
									sprintf (outbuf, "%lu %lu\n", n, facto); 
								else if (facfrom)
									sprintf (outbuf, "%lu %lu\n", n, facfrom); 
								else
									sprintf (outbuf, "%lu\n", n); 
							else if (res1 && res2)
								if (a)
									sprintf (outbuf, "%lu (GM(%lu) is Prime in Z+iZ and the norm of GQ(%lu) is %d-PRP.)\n", n, n, n, a); 
								else
									sprintf (outbuf, "%lu (GM(%lu) and GQ(%lu) are Prime in Z+iZ.)\n", n, n, n); 
							else if (res1)
								sprintf (outbuf, "%lu (GM(%lu) is Prime in Z+iZ.)\n", n, n); 
							else
								if (a)
									sprintf (outbuf, "%lu (The norm of GQ(%lu) is %d-PRP.)\n", n, n, a); 
								else
									sprintf (outbuf, "%lu (GQ(%lu) is Prime in Z+iZ.)\n", n, n); 
							_write (outfd, outbuf, strlen (outbuf));
							_close (outfd);
						}
						if (res2) {
							if (sign) {
								outfdm = _open (outmf, _O_TEXT | _O_RDWR | _O_APPEND | _O_CREAT, 0666);
								if (outfdm) {
									sprintf (outbuf, "%lu\n", n); 
									_write (outfdm, outbuf, strlen (outbuf));
									_close (outfdm);
								}
							}
							else {
								outfdp = _open (outpf, _O_TEXT | _O_RDWR | _O_APPEND | _O_CREAT, 0666);
								if (outfdp) {
									sprintf (outbuf, "%lu\n", n); 
									_write (outfdp, outbuf, strlen (outbuf));
									_close (outfdp);
								}
							}
						}
					}
				}
				else {							// Carol/Kynea
					if (sscanf (buff, "%lu %d", &n, &incr) != 2)
						continue;						// Skip invalid line
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
					if (! process_num (format, sgk, 2, n+1, -1, shift, &res)) goto done;
					if (res) {
						outfd = _open (outputfile, _O_TEXT | _O_RDWR | _O_APPEND | _O_CREAT, 0666);
						if (outfd) {
							sprintf (outbuf, "%lu %d\n", n, incr); 
							_write (outfd, outbuf, strlen (outbuf));
							_close (outfd);
						}
					}
				}
				IniWriteInt (INI_FILE, "PgenLine", line + 1);
				if(res && IniGetInt(INI_FILE, "StopOnSuccess", 0))
					break;
			}				// End loop
			goto done;
		}					// End ABC format

	    mask = 0;
	    sscanf (buff, $LLF":%c:%lu:%lu:%lu\n", &li, &c, &chainlen, &base, &mask);

	    if (mask & 0x40) {
			OutputStr ("Primoral NewPgen files are not supported.\n");
			return;
	    }
	    if (chainlen == 0) chainlen = 1;

	    if (! fileExists (outputfile)) {
			outfd = _open (outputfile, _O_TEXT | _O_RDWR | _O_CREAT, 0666);
			if (outfd) {
				if (mask == 0)
					sprintf (outbuf, $LLF":%c:%lu:%lu\n", li, c, chainlen, base);
				else
					sprintf (outbuf, $LLF":%c:%lu:%lu:%lu\n", li, c, chainlen, base, mask);
			_write (outfd, outbuf, strlen (outbuf));
			_close (outfd);
			}
	    }

		format = NPG;				// NEWPGEN output

/* THIS SECTION IS FOR BACKWARDS COMPATIBILITY WITH PREVIOUS PRP.EXE */
/* That version used the one character code to determine what to do. */
/* The new version uses the mask field. */

	    if (mask == 0 || IniGetInt (INI_FILE, "UseCharCode", 0)) {

/* The variable c is a one character code as follows: */
/* P : k.b^n+1 (Plus)
   M : k.b^n-1 (Minus)
   T: k.b^n+-1 (Twin)
   S: k.b^n-1; k.b^(n+1)-1 (SG (CC 1st kind len 2))
   C: k.b^n+1; k.b^(n+1)+1 (CC 2nd kind len 2)
   B: k.b^n+-1; k.b^(n+1)+-1 (BiTwin)
   J: k.b^n+-1; k.b^(n+1)-1 (Twin/SG)
   K: k.b^n+-1; k.b^(n+1)+1 (Twin/CC)
   Y : k.b^n+1 + others (Lucky Plus)
   Z : k.b^n-1 + others (Lucky Minus)
   1: CC 1st kind chain
   2: CC 2nd kind chain
   3: BiTwin chain */
/* Undo the increment of n that newpgen did on types 1, 2, 3 */
/* Map P, M, Y, Z, T, S, C, B to their more generic counterparts */

			nfudge = 0;
			if (c == '1') nfudge = 1;
			if (c == '2') nfudge = 1;
			if (c == '3') nfudge = 1;
			if (c == 'P') c = '2', chainlen = 1;
			if (c == 'M') c = '1', chainlen = 1;
//			if (c == 'Y') c = '2', chainlen = 1;
//			if (c == 'Z') c = '1', chainlen = 1;
			if (c == 'T') c = '3', chainlen = 1;
			if (c == 'S') c = '1', chainlen = 2;
			if (c == 'C') c = '2', chainlen = 2;
			if (c == 'B') c = '3', chainlen = 2;


/* Process each line in the newpgen output file */

			for (line = 1; ; line++) {

/* Read the line, break at EOF */

				if (fgets (buff, sgkbufsize+256, fd) == NULL) {
					IniWriteInt (INI_FILE, "WorkDone", 1);
					break;
				}

/* Skip this line if requested (we processed it on an earlier run) */

				if (line < firstline) continue;

					// allow k to be a big integer
				if (sscanf (buff, "%s %lu", sgk, &n) != 2)
					continue;						// Skip invalid line

				if (!isDigitString(sgk)) continue;	// Skip invalid line

				shift = 0;			// Only one value for the k multiplier

				skiptest = (line <= uptoline);

/* Test numbers according to the c variable */

				nn = n;
				if (c == 'Y') {
					nn--;
				}
				if (c == 'Z') {
					nn--;
				}

				for (i = 0; i < chainlen; i++) {
					if (c == '1' || c == '3') {
						if (! process_num (format, sgk, base, n - nfudge + i, -1, shift, &res)) goto done;
						if (!res) break;
					}
					if (c == '2' || c == '3') {
						if (! process_num (format, sgk, base, n - nfudge + i, +1, shift, &res)) goto done;
						if (!res) break;
					}
					if (c == 'J') {	// Twin/SG
						int	res2;
						if (! process_num (format, sgk, base, n, -1, shift, &res)) goto done;
						if (!res) break;
						if (! process_num (format, sgk, base, n+1, -1, shift, &res)) goto done;
						if (! process_num (format, sgk, base, n, +1, shift, &res2)) goto done;
						res |= res2;
						break;
					}
					if (c == 'K') {	// Twin/CC
						int	res2;
						if (! process_num (format, sgk, base, n, +1, shift, &res)) goto done;
						if (!res) break;
						if (! process_num (format, sgk, base, n, -1, shift, &res)) goto done;
						if (! process_num (format, sgk, base, n+1, +1, shift, &res2)) goto done;
						res |= res2;
						break;
					}
					if (c == 'Y') {	// Lucky Plus
						int	res2;
/*						if (! process_num (format, sgk, base, nn, +1, shift, &res)) goto done;
						if (!res) break;
						if (! process_num (format, sgk, base, nn+1, -1, shift, &res)) goto done;
						if (! process_num (format, sgk, base, nn+1, +1, shift, &res2)) goto done;
						res |= res2;
						if (! process_num (format, sgk, base, nn+2, +1, shift, &res2)) goto done;
						res |= res2;
						break;
*/
						if (! process_num (format, sgk, base, nn+1, +1, shift, &res)) goto done;
						if (!res) break;
						if (! process_num (format, sgk, base, nn+1, -1, shift, &res)) goto done;
						if (! process_num (format, sgk, base, nn, +1, shift, &res2)) goto done;
						res |= res2;
						if (! process_num (format, sgk, base, nn+2, +1, shift, &res2)) goto done;
						res |= res2;
						break;
					}
					if (c == 'Z') {	// Lucky Minus
						int	res2;
/*						if (! process_num (format, sgk, base, nn, -1, shift, &res)) goto done;
						if (!res) break;
						if (! process_num (format, sgk, base, nn+1, -1, shift, &res)) goto done;
						if (! process_num (format, sgk, base, nn+1, +1, shift, &res2)) goto done;
						res |= res2;
						if (! process_num (format, sgk, base, nn+2, -1, shift, &res2)) goto done;
						res |= res2;
						break;
*/
						if (! process_num (format, sgk, base, nn+1, -1, shift, &res)) goto done;
						if (!res) break;
						if (! process_num (format, sgk, base, nn+1, +1, shift, &res)) goto done;
						if (! process_num (format, sgk, base, nn, -1, shift, &res2)) goto done;
						res |= res2;
						if (! process_num (format, sgk, base, nn+2, -1, shift, &res2)) goto done;
						res |= res2;
						break;
					}
					if (c == 'A') {	// AP mode
						format = NPGAP;
						if (! process_num (format, sgk, base, n, -1, shift, &res)) goto done;
						if (!res) break;
					}
				}				// End loop on chain length

/* If all numbers tested were probable primes, copy the line to the output file */

				if (res) {
					outfd = _open (outputfile, _O_TEXT | _O_RDWR | _O_APPEND | _O_CREAT, 0666);
					if (outfd) {
						sprintf (outbuf, "%s %lu\n", sgk, n); 
						_write (outfd, outbuf, strlen (outbuf));
						_close (outfd);
					}
				}
				IniWriteInt (INI_FILE, "PgenLine", line + 1);
				if(res && IniGetInt(INI_FILE, "StopOnSuccess", 0))
					break;
			}					// End loop on input lines


/* THIS IS THE NEW SECTION.  It uses both the mask field and the */
/* character code to determine what to do */

	    }
		else {

/* NEWPGEN output files use the mask as defined below: */
/* #define MODE_PLUS    0x01	/* k.b^n+1 */
/* #define MODE_MINUS   0x02	/* k.b^n-1 */
/* #define MODE_2PLUS   0x04	/* k.b^(n+1)+1 (*) */
/* #define MODE_2MINUS  0x08	/* k.b^(n+1)-1 (*) */
/* #define MODE_4PLUS   0x10	/* k.b^(n+2)+1 (*) */
/* #define MODE_4MINUS  0x20	/* k.b^(n+2)-1 (*) */
/* #define MODE_PRIMORIAL 0x40	/* PRIMORIAL - can't handle this */
/* #define MODE_PLUS5  0x80	/* k.b^n+5 */
/* #define MODE_AP	    0x200	/* 2^n+2k-1 */
/* #define MODE_PLUS7  0x800	/* k.b^n+7 */
/* #define MODE_2PLUS3 0x1000	/* 2k.b^n+3 */
/* #define MODE_DUAL 0x8000
/* #define MODE_PLUS_DUAL 0x8001	/* b^n+k */
/* #define MODE_MINUS_DUAL 0x8002	/* b^n-k */
/* #define MODE_NOTGENERALISED 0x400
/* Those entries that have a (*) next to them are modified if the */
/* MODE_NOTGENERALISED flag is set.  If it is set, they are changed */
/* as follows: */
/* MODE_2PLUS      2k.b^n+1 */
/* MODE_2MINUS     2k.b^n-1 */
/* MODE_4PLUS      4k.b^n+1 */
/* MODE_4MINUS     4k.b^n-1 */
/* Similarly, longer chains are affected in the same way (so if the base */
/* is 3 and we are after a CC of the 1st kind of length 4, rather that */
/* looking at k.3^n-1 & k.3^(n+1)-1 & k.3^(n+2)-1 & k.3^(n+3)-1 we look */
/* at k.3^n-1 & 2k.3^n-1 & 4k.3^n-1 & 8k.3^n-1). */

/* Process each line in the newpgen output file */

			for (line = 1; ; line++) {

/* Read the line, break at EOF */

				if (fgets (buff, sgkbufsize+256, fd) == NULL) {
					IniWriteInt (INI_FILE, "WorkDone", 1);
					break;
				}

/* Skip this line if requested (we processed it on an earlier run) */

				if (line < firstline) continue;
					// allow k to be a big integer
				if (sscanf (buff, "%s %lu", sgk, &n) != 2)
					continue;						// Skip invalid line

				if (!isDigitString(sgk)) continue;	// Skip invalid line

				skiptest = (line <= uptoline);

/* Undo the increment of n that newpgen did on types 1, 2, 3 */

				nn = n;
				if (c == '1' || c == '2' || c == '3') nn--;

				if ((mask & MODE_PLUS) && (mask & MODE_2MINUS) && (mask & MODE_2PLUS) && (mask & MODE_4PLUS)) {
					nn--;
				}
				if ((mask & MODE_MINUS) && (mask & MODE_2MINUS) && (mask & MODE_2PLUS) && (mask & MODE_4MINUS)) {
					nn--;
				}

/* Test numbers according to the mask variable */
/* The J and K types (Twin/CC and Twin/SG) are special in that they */
/* are output if either a Twin OR a CC/SG is found */

				shift = 0;

				for (i = 0; i < chainlen; i++) {
					if ((mask & MODE_MINUS) && (mask & MODE_PLUS) && (mask & MODE_2MINUS)) {	// Twin/SG
						int	res2;
						if (! process_num (format, sgk, base, nn, -1, shift, &res)) goto done;
						if (!res) break;
						if (! process_num (format, sgk, base, nn+1, -1, shift, &res)) goto done;
						if (! process_num (format, sgk, base, nn, +1, shift, &res2)) goto done;
						res |= res2;
						break;
					}
					if ((mask & MODE_MINUS) && (mask & MODE_PLUS) && (mask & MODE_2PLUS)) {	// Twin/CC
						int	res2;
						if (! process_num (format, sgk, base, nn, +1, shift, &res)) goto done;
						if (!res) break;
						if (! process_num (format, sgk, base, nn, -1, shift, &res)) goto done;
						if (! process_num (format, sgk, base, nn+1, +1, shift, &res2)) goto done;
						res |= res2;
						break;
					}
					if ((mask & MODE_PLUS) && (mask & MODE_2MINUS) && (mask & MODE_2PLUS) && (mask & MODE_4PLUS)) {	// Lucky Plus
						int	res2;
/*						if (! process_num (format, sgk, base, nn, +1, shift, &res)) goto done;
						if (!res) break;
						if (! process_num (format, sgk, base, nn+1, -1, shift, &res)) goto done;
						if (! process_num (format, sgk, base, nn+1, +1, shift, &res2)) goto done;
						res |= res2;
						if (! process_num (format, sgk, base, nn+2, +1, shift, &res2)) goto done;
						res |= res2;
						break;
*/
						if (! process_num (format, sgk, base, nn+1, +1, shift, &res)) goto done;
						if (!res) break;
						if (! process_num (format, sgk, base, nn+1, -1, shift, &res)) goto done;
						if (! process_num (format, sgk, base, nn, +1, shift, &res2)) goto done;
						res |= res2;
						if (! process_num (format, sgk, base, nn+2, +1, shift, &res2)) goto done;
						res |= res2;
						break;
					}
					if ((mask & MODE_MINUS) && (mask & MODE_2MINUS) && (mask & MODE_2PLUS) && (mask & MODE_4MINUS)) {	// Lucky Minus
						int	res2;
/*						if (! process_num (format, sgk, base, nn, -1, shift, &res)) goto done;
						if (!res) break;
						if (! process_num (format, sgk, base, nn+1, -1, shift, &res)) goto done;
						if (! process_num (format, sgk, base, nn+1, +1, shift, &res2)) goto done;
						res |= res2;
						if (! process_num (format, sgk, base, nn+2, -1, shift, &res2)) goto done;
						res |= res2;
						break;
*/
						if (! process_num (format, sgk, base, nn+1, -1, shift, &res)) goto done;
						if (!res) break;
						if (! process_num (format, sgk, base, nn+1, +1, shift, &res)) goto done;
						if (! process_num (format, sgk, base, nn, -1, shift, &res2)) goto done;
						res |= res2;
						if (! process_num (format, sgk, base, nn+2, -1, shift, &res2)) goto done;
						res |= res2;
						break;
					}
					if (mask & MODE_MINUS) {
						if (mask & MODE_DUAL) {
							if (! process_num (format, "1", base, nn, -atoi(sgk), shift, &res)) goto done;
						}
						else
							if (! process_num (format, sgk, base, nn, -1, shift, &res)) goto done;
						if (!res) break;
					}
					if (mask & MODE_PLUS) {
						if (mask & MODE_DUAL) {
							if (! process_num (format, "1", base, nn, atoi(sgk), shift, &res)) goto done;
						}
						else
							if (! process_num (format, sgk, base, nn, +1, shift, &res)) goto done;
						if (!res) break;
					}
					if (mask & MODE_PLUS5) {
						if (! process_num (format, sgk, base, nn, +5, shift, &res)) goto done;
						if (!res) break;
					}
					if (mask & MODE_PLUS7) {
						if (! process_num (format, sgk, base, nn, +7, shift, &res)) goto done;
						if (!res) break;
					}
					if (mask & MODE_2PLUS3) {
						shift = 1;
						if (! process_num (format, sgk, base, nn, +3, shift, &res)) goto done;
						shift = 0;
						if (!res) break;
					}
					if (mask & MODE_AP) {
						format = NPGAP;
						if (! process_num (format, sgk, base, nn, -1, shift, &res)) goto done;
						if (!res) break;
					}

/* Bump k or n for the next iteration or for the MODE_2PLUS and */
/* MODE_2MINUS flags */

					if (mask & MODE_NOTGENERALISED) shift += 1; 
					else nn += 1;

/* If chainlength is more than 1, then we let the for loop do the work */
/* rather than the MODE_2PLUS, etc. flags */

					if (chainlen > 1) continue;

					if (mask & MODE_2MINUS) {
						if (! process_num (format, sgk, base, nn, -1, shift, &res)) goto done;
						if (!res) break;
					}
					if (mask & MODE_2PLUS) {
						if (! process_num (format, sgk, base, nn, +1, shift, &res)) goto done;
						if (!res) break;
					}

/* Bump k or n for the MODE_4PLUS and MODE_4MINUS flags */

					if (mask & MODE_NOTGENERALISED) shift += 1; 
					else nn += 1;

					if (mask & MODE_4MINUS) {
						if (! process_num (format, sgk, base, nn, -1, shift, &res)) goto done;
						if (!res) break;
					}
					if (mask & MODE_4PLUS) {
						if (! process_num (format, sgk, base, nn, +1, shift, &res)) goto done;
						if (!res) break;
					}
				}						// End of loop on chain length

/* If all numbers tested were probable primes, copy the line to the output file */

				if (res) {
					outfd = _open (outputfile, _O_TEXT | _O_RDWR | _O_APPEND | _O_CREAT, 0666);
					if (outfd) {
						sprintf (outbuf, "%s %lu\n", sgk, n); 
						_write (outfd, outbuf, strlen (outbuf));
						_close (outfd);
					}
				}
				IniWriteInt (INI_FILE, "PgenLine", line + 1);
				if(res && IniGetInt(INI_FILE, "StopOnSuccess", 0))
					break;
			}							// End of loop on input lines
		
	    }								// End of new section

done:
		if (facto) {
			sprintf (outbuf, "%lu candidates factored, %lu factors found, %lu remaining\n"
				, factored, eliminated, factored - eliminated);
			OutputBoth (outbuf);
		}
		fclose (fd);
	}

/* Handle an expr */

	else {
		OutputStr ("Expression testing not yet implemented.\n");
		IniWriteInt (INI_FILE, "WorkDone", 1);
	}
}
