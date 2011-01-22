#ifndef LLRDOTH
#define LLRDOTH

/* Constants */

#define VERSION		"3.8.4"

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
#define WORK_FACTOR		0
#define WORK_TEST		1
#define WORK_ADVANCEDTEST	2
#define WORK_DBLCHK		3
#define WORK_ECM		4
#define WORK_PMINUS1		5
#define WORK_PFACTOR		6
#define WORK_ADVANCEDFACTOR	7


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

void nameIniFiles (int named_ini_files);
void readIniFiles ();

void IniGetString (char *, char *, char *, unsigned int, char *);
long IniGetInt (char *, char *, long);
void IniWriteString (char *, char *, char *);
void IniWriteInt (char *, char *, long);

void IniFileOpen (char *, int);
void processTimedIniFile (char *);
void IniFileClose (char *);
unsigned int IniGetNumLines (char *);
void IniGetLineAsString (char *, unsigned int, char *, unsigned int,
			 char *, unsigned int);
void IniGetLineAsInt (char *, unsigned int, char *, unsigned int, long *);
void IniReplaceLineAsString (char *, unsigned int, char *, char *);
void IniReplaceLineAsInt (char *, unsigned int, char *, long);
void IniInsertLineAsString (char *, unsigned int, char *, char *);
void IniInsertLineAsInt (char *, unsigned int, char *, long);
void IniAppendLineAsString (char *, char *, char *);
void IniAppendLineAsInt (char *, char *, long);
void IniDeleteLine (char *, unsigned int);
void IniDeleteAllLines (char *);

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

EXTERNC void setupf();
EXTERNC int factor64();
EXTERNC void psetupf();
EXTERNC int pfactor64();
EXTERNC void* aligned_malloc (unsigned long, unsigned long);
EXTERNC void  aligned_free (void *);


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

void primeContinue ();

/* Routines used to time code chunks */

extern double __timers[10];		/* 10 timers are available */

/* Routines called by common routines */

void title (char *);
void flashWindowAndBeep ();
void SetPriority ();

struct  gwasm_data {	// gwnum V25.13
	void	*DESTARG;	/* Function argument */
	void	*DIST_TO_FFTSRCARG;/* SRCARG - DESTARG */
	void	*DIST_TO_MULSRCARG;/* SRC2ARG - DESTARG */
	void	*NORMRTN;	/* Post-multiply normalization routine */

	int32_t	ZERO_PADDED_FFT;/* True if doing a zero pad FFT */
	int32_t	POSTFFT;	/* True if assembly code can start the */
				/* FFT process on the result of a multiply */
	double	MAXERR;		/* Convolution error in a multiplication */

	void	*scratch_area;	/* Scratch area for pass 1 of SSE2 FFTs */
	void	*norm_grp_mults;/* Ptr to array #1 of normalize multipliers */
	void	*norm_col_mults;/* Ptr to array #2 of normalize multipliers */
	void	*norm_biglit_array;/* Ptr to byte array of big/lit flags */

	void	*carries;	/* Ptr to array of carries (2 pass FFT) */
	void	*plus1_premults;/* Address of 2^N+1 premultiplier data */
	intptr_t pass2gapsize;	/* Wasted bytes between pass2 data blks */
	void	*sincos1;

	void	*sincos2;
	void	*sincos3;
	void	*sincos4;
	void	*sincos5;

	intptr_t pass1blkdst;	/* Dist between blocks in pass 1 */
	void	*pass2_premults;/* Address of pass 2 premultiplier data */
	void	*xsincos_complex;/* Addr of pass2 complex sin/cos data */
	void	*sincos6;

	void	*sincos7;
	void	*sincos8;
	void	*sincos9;
	void	*sincos10;

	double	XMM_TWO[2];	/* 2.0 */
	double	XMM_HALF[2];	/* 0.5 */
	double	XMM_SQRTHALF[2];/* SQRT(0.5) */
	double	XMM_TMP1_8[16];	/* 8 XMM temporaries */

	int32_t	ffttype;	/* Type of fft (1, 2, 3, or 4) */
	int32_t	count1;		/* Counter used in common fft code */
	uint32_t ADDIN_ROW;	/* For adding a constant after multiply */
	uint32_t ADDIN_OFFSET;

	int32_t	addcount1;	/* Counter used in common fft code */
	int32_t	normval1;	/* Add / sub constants */
	int32_t	normval4;
	int32_t	ALL_COMPLEX_FFT;/* True if using all-complex FFTs */

	int32_t	cache_line_multiplier; /* Cache line multiplier from jmptab */
	int32_t	RATIONAL_FFT;	/* True if bits per FFT word is integer */
	int32_t	TOP_CARRY_NEEDS_ADJUSTING;/* True when carry out of top word */
				/* needs adjusting */
	int32_t	SPREAD_CARRY_OVER_4_WORDS;/* True when carry out of top word */
				/* must be spread over more than 2 words */

	int32_t	zero_fft;	/* TRUE if zero upper half in normalize */
	int32_t	const_fft;	/* TRUE if mul-by-const in normalize */
	double	ADDIN_VALUE;	/* Value to add in after a multiply */

	void	*norm_ptr1;
	void	*norm_ptr2;
	intptr_t normblkdst;	/* Dist between blocks in normalization */
	intptr_t normblkdst8;	/* Dist between 8 blocks in normalize */

	double	XMM_SUMOUT[2];	/* Used in normalization macros */
	double	XMM_MAXERR[2];	/* Used in normalization macros */
	double	XMM_LIMIT_BIGMAX[32]; /* Normalization constants */
	double	XMM_LIMIT_INVERSE[32];
	double	XMM_COL_MULTS[1024];
	double	XMM_TTP_FUDGE[32];
	double	XMM_TTMP_FUDGE[32];
	double	XMM_BIGVAL[2];	/* Used to round double to integer */
	double	XMM_MINUS_C[2];	/* -c stored as double */
	double	XMM_NORM012_FF[2]; /* Used in xnorm012 macros (FFTLEN/2) */
	double	XMM_LIMIT_BIGMAX_NEG[32];
	int32_t	XMM_ABSVAL[4];	/* Used to compute absolute values */
	double	XMM_P618[2];
	double	XMM_P309[2];
	double	XMM_M809[2];
	double	XMM_M262[2];
	double	XMM_M382[2];
	double	XMM_P951[2];
	double	XMM_P588[2];
	double	XMM_M162[2];
	double	DBLARG;
	double	CARRY_ADJUST7;
	double	XMM_P866[2];
	double	XMM_BIGVAL_NEG[2];
	double	CARRY_ADJUST1_HI;
	double	CARRY_ADJUST1_LO;
	double	XMM_UNUSED4[2];
	double	XMM_UNUSED5[2];
	double	XMM_P623[2];
	double	XMM_M358[2];
	double	XMM_P404[2];
	double	XMM_P975[2];
	double	XMM_P445[2];
	double	XMM_P180[2];

	uint32_t COPYZERO[8];	/* Offsets to help in gwcopyzero */

	uint32_t B_IS_2;
	uint32_t NUMARG;	/* Gwcopyzero assembly arg */
	double	ZPAD_INVERSE_K6;

	uint32_t *ASM_TIMERS;	/* Timers used for optimizing code */
	void	*sincos11;
	void	*sincos12;
	void	*zpad_addr;	/* Address of XMM_ZPAD0 */

	uint32_t FFTLEN;	/* The FFT size we are using */
	uint32_t ZPAD_TYPE;	/* 1,2,or 3 words in k (used by zero pad) */
	uint32_t BIGLIT_INCR2;	/* Offset to step in big/lit array */
	uint32_t BIGLIT_INCR4;	/* Offset to step in big/lit array */

	double	ZPAD_K6_HI;	 /* Zero padded FFT constants */
	double	ZPAD_K6_MID;
	double	ZPAD_K6_LO;
	double	ZPAD_SHIFT6;
	double	ZPAD_INVERSE_K5;
	double	ZPAD_K5_HI;
	double	ZPAD_K5_MID;
	double	ZPAD_K5_LO;
	double	ZPAD_SHIFT5;
	double	ZPAD_INVERSE_K4;
	double	ZPAD_K4_HI;
	double	ZPAD_K4_MID;
	double	ZPAD_K4_LO;
	double	ZPAD_SHIFT4;
	double	ZPAD_INVERSE_K3;
	double	ZPAD_K3_HI;
	double	ZPAD_K3_MID;
	double	ZPAD_K3_LO;
	double	ZPAD_SHIFT3;
	double	ZPAD_INVERSE_K2;
	double	ZPAD_K2_HI;
	double	ZPAD_K2_MID;
	double	ZPAD_K2_LO;
	double	ZPAD_SHIFT2;
	double	ZPAD_K1_HI;
	double	ZPAD_INVERSE_K1;
	double	ZPAD_K1_LO;
	double	ZPAD_SHIFT1;
	double	ZPAD0_7[8];

	double	XMM_BIGBIGVAL[2]; /* Used to round to big word multiple */
	double	XMM_MULCONST[2];
	double	XMM_K_TIMES_MULCONST_LO[2]; /* k*mulconst, low big word bits */
	double	XMM_K_TIMES_MULCONST_HI[2]; /* k*mulconst, top bits */
	double	XMM_MINUS_C_TIMES_MULCONST[2];
	double	XMM_K_TIMES_MULCONST_HI_1[2]; /* XMM_K_TIMES_MULCONST_HI, low bits */
	double	XMM_K_TIMES_MULCONST_HI_2[2]; /* XMM_K_TIMES_MULCONST_HI, top bits */
	double	XMM_K_LO[2];	/* k, bottom big word bits */
	double	XMM_K_HI[2];	/* k, top bits */
	double	XMM_K_HI_1[2];	/* XMM_K_HI, bottom big word bits */
	double	XMM_K_HI_2[2];	/* XMM_K_HI, top bits */

	double	INVERSE_K;	/* 1/K */
	double	K_HI;		/* Upper bits of K */
	double	K_LO;		/* Lower bits of K */
	double	CARRY_ADJUST1;	/* Adjustment constant #1 in wrapping carry */
	double	CARRY_ADJUST2;	/* Adjustment constant #2 in wrapping carry */
	double	CARRY_ADJUST3;	/* Adjustment constant #3 in wrapping carry */
	double	CARRY_ADJUST4;	/* Adjustment constant #4 in wrapping carry */
	double	CARRY_ADJUST5;	/* Adjustment constant #5 in wrapping carry */
	double	CARRY_ADJUST6;	/* Adjustment constant #6 in wrapping carry */

	uint32_t HIGH_WORD1_OFFSET;/* Offset of top FFT word */
	uint32_t HIGH_WORD2_OFFSET;/* Offset of second high FFT word */
	uint32_t HIGH_WORD3_OFFSET;/* Offset of third high FFT word */
	uint32_t HIGH_SCRATCH1_OFFSET;
				/* Offset of top FFT word in scratch area */
	uint32_t HIGH_SCRATCH2_OFFSET;
				/* Offset of second highest FFT word */
	uint32_t HIGH_SCRATCH3_OFFSET;
				/* Offset of third highest FFT word */

	void	*SAVED_RSP;	/* Saved stack pointer in 32-bit mode */
	void	*SRCARG;	/* Function argument */
	void	*SRC2ARG;	/* Function argument */
	void	*DEST2ARG;	/* Function argument */

	intptr_t normval2;	/* Add / sub temporaries */
	intptr_t normval3;
	intptr_t ZPAD_WORD5_OFFSET; /* Offset of the 5th word */
	intptr_t ZPAD_WORD5_RBP_OFFSET; /* Offset to 5th word's ttp data */

	int32_t	normcount1;
	int32_t	count2;
	int32_t	count3;
	int32_t	count4;

	int32_t	count5;
	float	HALF;		/* 0.5 */
	float	BIGVAL;		/* Rounding value */
	float	BIGBIGVAL;

	double	SQRTHALF;	/* SQRT(0.5) */
	double	MINUS_C;	/* -c */
	double	ttmp_ff_inv;	/* Inverse FFT adjust (2/FFTLEN) */
	double	P309;
	double	M809;
	double	M262;
	double	M382;
	double	P951;
	double	P588;
	double	M162;
	double	P618;
	double	P623;
	double	M358;
	double	P404;
	double	P975;
	double	P445;
	double	P180;
	double	M223;
	double	M901;
	double	M691;
	double	P866;
	double	P433;
	double	P577;
	float	P25;		/* 0.25 */
	float	P75;		/* 0.75 */
	float	P3;		/* 3.0 */

	int32_t	thread_num;	/* Thread num - so we can differentiate main */
				/* thread from auxillary threads */
	void	*data_addr;	/* FFT data to work on this pass */
	void	*data_prefetch;	/* FFT data to prefetch for next pass */
	void	*premult_addr;	/* Premult data to use this pass */
	void	*premult_prefetch;/* Premult data to prefetch for next pass */
	gwhandle *gwdata;	/* Allows callback routines to access gwdata */
	void	*pass1_wake_up_threads; /* Callback routine to wake up */
				/* auxillary threads */
	void	*pass1_wait_for_carries; /* Callback routine to ensure the */
				/* normalization routine accesses the carry */
				/* data in ascending block numbers */
	void	*pass1_done_with_carries; /* Callback routine to let the */
				/* next block access the carry data */
	void	*pass1_get_next_block; /* Callback routine to get next block */
				/* number for thread to process */
	void	*pass2_wake_up_threads; /* Callback routine to wake up */
				/* auxillary threads */
	void	*pass2_get_next_block; /* Callback routine to get next block */
				/* number for thread to process */
	void	(*thread_work_routine)(void*); /* Assembly routine to call */
				/* when auxillary thread wakes up */
	uint32_t this_block; /* Block currently being processed */
	uint32_t next_block; /* Next block to process */
	uint32_t last_pass1_block; /* Last block to process */
};

#endif
