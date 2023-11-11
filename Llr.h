#ifndef LLRDOTH
#define LLRDOTH

/* Constants */

#define LLR_VERSION		"4.0.5"
#define LLR_VERSIONC	4,0,5

/* Global variables */

extern char INI_FILE[80];		/* Name of the prime INI file */

extern int volatile ERRCHK;			/* 1 to turn on error checking */
extern unsigned int PRIORITY;		/* Desired priority level */
extern unsigned int CPU_AFFINITY;	/* NT Processor affinity */

/* Factoring limits based on complex formulas given the speed of the */
/* factoring code vs. the speed of the Lucas-Lehmer code */
/* As an example, examine factoring to 2^68 (finding all 68-bit factors). */
/* First benchmark a machine to get LL iteration times and trial factoring */
/* times for a (16KB sieve of p=35000011). */
/*	We want to find when time spend eliminating an exponent with */
/* trial factoring equals time saved running 2 LL tests. */

/*	runs to find a factor (68) *
	#16KB sections (2^68-2^67)/p/(120/16)/(16*1024*8) *
	factoring_benchmark = 2.0 * LL test time (p * ll_benchmark)

	simplifying:

	68 * (2^68-2^67)/p/(120/16)/(16*1024*8) * facbench = 2 * p * llbench
	68 * 2^67 / p / (120/16) / 2^17 * facbench = 2 * p * lltime
	68 * 2^49 / p / (120/16) * facbench = p * lltime
	68 * 2^49 / (120/16) * facbench = p^2 * lltime
	68 * 2^53 / 120 * facbench = p^2 * lltime
	68 * 2^53 / 120 * facbench / lltime = p^2
	sqrt (68 * 2^53 / 120 * facbench / lltime) = p
*/

/* Now lets assume 30% of these factors would have been found by P-1.  So
   we only save a relatively quick P-1 test instead 2 LL tests.  Thus:
	sqrt (68 / 0.7 * 2^53 / 120 * facbench / lltime) = p
*/

/* Now factor in that 35000000 does 19 squarings, but 70000000 requires 20.
   Thus, if maxp is the maximum exponent that can be handled by an FFT size:
	sqrt (68 / 0.7 * 2^53 / 120 *
	      facbench * (1 + LOG2 (maxp/35000000) / 19) / lltime) = p
*/

/* Now factor in that errors sometimes force us to run more than 2 LL tests.
   Assume, 2.04 on average:
	sqrt (68 / 0.7 * 2^53 / 120 *
	      facbench * (1 + LOG2 (maxp/35000000) / 19) / lltime / 1.02) = p
*/

/* These breakeven points we're calculated on a 2.0 GHz P4 Northwood: */

#define FAC80	516000000L
#define FAC79	420400000L
#define FAC78	337400000L
#define FAC77	264600000L
#define FAC76	227300000L
#define FAC75	186400000L
#define FAC74	147500000L
#define FAC73	115300000L
#define FAC72	96830000L
#define FAC71	75670000L
#define FAC70	58520000L
#define FAC69	47450000L
#define FAC68	37800000L
#define FAC67	29690000L
#define FAC66	23390000L

/* These breakevens we're calculated a long time ago on unknown hardware: */

#define FAC65	13380000L
#define FAC64	8250000L
#define FAC63	6515000L
#define FAC62	5160000L
#define FAC61	3960000L
#define FAC60	2950000L
#define FAC59	2360000L
#define FAC58	1930000L
#define FAC57	1480000L
#define FAC56	1000000L

short default_work_type (void);
/* Structures and definitions dealing with the worktodo.ini file */

#define WORK_FACTOR		0
#define WORK_TEST		1
#define WORK_DBLCHK		2
#define WORK_ADVANCEDTEST	3
#define WORK_ECM		4
#define WORK_PMINUS1		5
#define WORK_PPLUS1		6
#define WORK_PFACTOR		7
#define WORK_PRP		10
#define WORK_CERT		11
#define WORK_NONE		100	/* Comment line in worktodo.ini */
#define WORK_DELETED		101	/* Deleted work_unit */

struct work_unit {		/* One line from the worktodo file */
	int	work_type;	/* Type of work to do */
	char	assignment_uid[33]; /* Primenet assignment ID */
	char	extension[9];	/* Optional save file extension */
	double	k;		/* K in k*b^n+c */
	unsigned long b;	/* B in k*b^n+c */
	unsigned long n;	/* N in k*b^n+c */
	signed long c;		/* C in k*b^n+c */
	unsigned long minimum_fftlen;/* Minimum FFT length to use.  Zero means default fftlen. */
	double	sieve_depth;	/* How far it has been trial factored */
	double	factor_to;	/* How far we should trial factor to */
	int	pminus1ed;	/* TRUE if has been P-1 factored */
	uint64_t B1;		/* ECM, P-1, P+1 - Stage 1 bound */
	uint64_t B2;		/* ECM, P-1, P+1 - Stage 2 bound */
	uint64_t B2_start;	/* P-1 - Stage 2 start */
	int	nth_run;	/* P+1 - 1 for start 2/7, 2 for start 6/5, 3+ for random start */
	unsigned int curves_to_do; /* ECM - curves to try */
	double	curve;		/* ECM - Specific curve to test (debug tool) */
	double	tests_saved;	/* Pfactor - primality tests saved if a factor is found */
	unsigned int prp_base;	/* PRP base to use */	
	int	prp_residue_type; /* PRP residue to output -- see primenet.h */
	int	prp_dblchk;	/* True if this is a doublecheck of a previous PRP */
	int	cert_squarings; /* Number of squarings required for PRP proof certification */
	char	*known_factors;	/* ECM, P-1, P+1, PRP - list of known factors */
	char	*comment;	/* Comment line in worktodo.txt */
		/* Runtime variables */
	struct work_unit *next; /* Next in doubly-linked list */
	struct work_unit *prev; /* Previous in doubly-linked list */
	int	in_use_count;	/* Count of threads accessing this work unit */
	int	high_memory_usage;/* Set if we are using a lot of memory.  If user changes the available memory */
				/* settings, then we should stop and restart our computations. */
	char	stage[10];	/* Test stage (e.g. TF,P-1,LL) */
	double	pct_complete;	/* Percent complete (misnomer as value is between 0.0 and 1.0) */
	unsigned long fftlen;	/* FFT length in use */
	int	ra_failed;	/* Set when register assignment fails, tells us not to try registering it again. */
};
struct work_unit_array {	/* All the lines for one worker thread */
	struct work_unit *first; /* First work unit */
	struct work_unit *last;	/* Last work unit */
};


extern unsigned long volatile ITER_OUTPUT;/* Iterations between outputs */
extern unsigned long volatile ITER_OUTPUT_RES;/* Iterations between results */
					/* file outputs */
extern unsigned long volatile DISK_WRITE_TIME;
					/* Number of minutes between writing */
					/* intermediate results to disk */
extern int TWO_BACKUP_FILES;		/* TRUE for 2 backup files(qXXXXXXX) */
extern int RUN_ON_BATTERY;		/* Run program even on battery power */
extern int TRAY_ICON;			/* Display tiny tray icon */
extern int HIDE_ICON;			/* Display no icon */
extern unsigned int PRECISION;		/* Number of decimal places to output*/
					/* in percent complete lines */
extern int CUMULATIVE_TIMING;		/* True if outputting cumulative time*/

/* Common routines */


void getCpuInfo (); 
 
int isPrime (unsigned long p);


#include "gwnum/gwini.h"
#include "gwnum/gwutil.h"

struct IniCache {						// defined in gwini.c but not in gwini.h
	char	*filename;
	int	immediate_writes;
	int	dirty;
	unsigned int num_lines;
	unsigned int array_size;
	struct IniLine **lines;
};

#ifdef __cplusplus
extern "C" {
#endif

struct IniCache *openIniFile (			// defined in gwini.c but not in gwini.h
	char	*,
	int);
void writeIniFile (						// defined in gwini.c but not in gwini.h
	struct IniCache *);
void IniFileOpen (char *, int);
void nameIniFiles (int);
void readIniFiles ();

#ifdef __cplusplus
}
#endif

void OutputBoth (char *);
void OutputSomewhere (char *);
void LogMsg (char *);
void ReplaceableLine (int);

unsigned long pick_fft_length (unsigned long);

void tempFileName (char	*, char, giant);
int fileExists (char *);
int readFileHeader (char *, int *, short *, unsigned long *);
int writeResults (char	*);

int communicateWithServer ();

#ifndef X86_64

EXTERNC void setupf();
EXTERNC int factor64();
EXTERNC void psetupf();
EXTERNC int pfactor64();

#endif

//EXTERNC void* aligned_malloc (unsigned long, unsigned long);
EXTERNC void  aligned_free (void *);
//EXTERNC void  truncated_strcpy (char*, unsigned long, const char*);


/* Routines called by common routines */

void OutputStr (char *);
int isHighResTimerAvailable (void); 
double getHighResTimer (void); 
double getHighResTimerFrequency (void); 
void guessCpuType ();
void getCpuDescription (char *, int); 
unsigned long num_cpus ();
#define stopCheck escapeCheck
int escapeCheck ();
#define	WORKING_ICON	0
#define	IDLE_ICON	1
void ChangeIcon (int);
void BlinkIcon (int);


/* Common routines */

int primeContinue ();

/* Routines used to time code chunks */

extern double __timers[10];		/* 10 timers are available */

/* Routines called by common routines */

void title (char *);
void flashWindowAndBeep ();
void SetPriority ();

#endif
