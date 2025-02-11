/* Copyright 1995-2002 Just For Fun Software, Inc. */
/* Author:  George Woltman */
/* Email: woltman@alum.mit.edu */
/* Adapted for LLR program by Jean Penn� */
/* Email : jpenne@free.fr */

/* Include files */

#ifdef __FreeBSD__
/* FreeBSD needs to process sys/types.h before it can understand either
/* sys/time.h or sys/resource.h */
#include <sys/types.h>
#endif
#include <ctype.h>
#include <fcntl.h>
#include <math.h>
#include <memory.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#if defined (__linux__) || defined (__FreeBSD__) || defined (__APPLE__)
#include <sys/wait.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
//#define B64b "%qi"
#define __int64 long long
#else
#include <direct.h>
#include <dos.h>
#include <io.h>
#include <time.h>
#include <process.h>
#include <windows.h>
//#define B64b "%I64u"
#endif
#include <sys/timeb.h>
#include "lprime.h"

/* Globals */

#define OPEN_MAX 20 
 
#ifdef MPRIME_LOADAVG
#define LINUX_LDAV_FILE "/proc/loadavg"
int volatile SLEEP_STOP = 0;
long LOAD_CHECK_TIME = 0;
double HI_LOAD = 0.0;
double LO_LOAD = 0.0;
#endif

int volatile THREAD_STOP = 0;
int volatile THREAD_KILL = 0;
int NO_GUI = 1;
int VERBOSE = 0;
int MENUING = 0;
int PROCESSFILE = 0;
int SINGLETEST = 0;

/* Common code */

#ifdef __linux__
#define PORT	2
#endif
#ifdef __APPLE__
#define PORT	2
#endif
#ifdef __FreeBSD__
#define PORT	6
#endif
#ifdef __EMX__
#define PORT	7
#endif

#include "Jacobi.c"
#include "kronecker.c"
#include "Qfields.c"
#include "factor.c"
//#if defined (__linux__) || defined (__FreeBSD__) || defined (__APPLE__)
#include "mpz_aprcl.c"
//#endif
#include "md5.c"
#include "Llr.c"

/* Signal handlers */

void sigterm_handler(int signo)
{
	THREAD_STOP = TRUE;
	if (signo != SIGINT) THREAD_KILL = TRUE;
	printf("\n Caught signal.  Terminating.\n"); 
	(void)signal(signo, sigterm_handler);
}

#ifdef MPRIME_LOADAVG

/* Routine to get the current load average */

double get_load_average ()
{
#if defined (__linux__) || defined (__APPLE__)
	char	ldavgbuf[40];
	double	load_avg;
	int	fd, count;

	fd = open (LINUX_LDAV_FILE, O_RDONLY);
	if (fd == -1) return (-1.0);
	count = read (fd, ldavgbuf, 40);
	(void) close (fd);
	if (count <= 0) return (-1.0);
	count = sscanf (ldavgbuf, "%lf", &load_avg);
	if (count < 1) return (-1.0);
	return (load_avg);
#endif
#ifdef __FreeBSD__
	double load[3];

	if (getloadavg (load, sizeof(load)/sizeof(load[0])) < 0) return (-1.0);
	return (load[0]);
#endif
}

/* load_handler: call by signal routine,
   sets SLEEP_STOP to TRUE if load is too high */
void load_handler (
	int	sig)
{
	double  load_avg;

	load_avg = get_load_average ();
	if (load_avg < 0.0) return;
  
	if (SLEEP_STOP) {
		if (load_avg < LO_LOAD)
			SLEEP_STOP = FALSE;
	} else {
		if (load_avg > HI_LOAD)
			SLEEP_STOP = TRUE;
	}
}

/* init_load_check: initialises timer that calls load_handler
   every LOAD_CHECK_TIME seconds */
void init_load_check ()
{
	struct itimerval timer, otimer;
	struct sigaction sigact;
	int	ret;

	timer.it_interval.tv_sec  =  LOAD_CHECK_TIME;
	timer.it_interval.tv_usec =  0;
	timer.it_value.tv_sec     =  LOAD_CHECK_TIME;
	timer.it_value.tv_usec    =  0;

	ret = setitimer (ITIMER_REAL, &timer, &otimer);
	if (ret < 0) return;
  
	sigact.sa_handler = &load_handler;
	sigemptyset(&sigact.sa_mask);
	sigact.sa_flags =  SA_RESTART;
	ret = sigaction(SIGALRM, &sigact, NULL);
	if (ret < 0) { /* clean up after ourselves */
		setitimer (ITIMER_REAL, &otimer, NULL);
	}
}

/* test_sleep: tests if SLEEP_STOP is set and sleeps until load is normal
   again or THREAD_STOP is set
*/
void test_sleep (void) 
{
	sigset_t newmask;

	while (SLEEP_STOP && !THREAD_STOP) {
		sigemptyset (&newmask);
		sigsuspend (&newmask);
	}
}
#endif

/* Main entry point! */

int main (
	int	argc,
	char	*argv[])
{
	char	buf[IBSIZE];
	int	named_ini_files = -1;
	int	background = 0;
	int	contact_server = 0;
	int	i, lchd, opcnt = 0, leftbracket = 0, errnb = 0;
	char	*p;
	char	*p2;
	char	dnflag = ' ';
	FILE	*in;

/* No buffering of output */

	setvbuf (stdout, NULL, _IONBF, 0);

/* catch termination signals */

	(void)signal(SIGTERM, sigterm_handler);
	(void)signal(SIGINT, sigterm_handler);

/* Change to the executable's directory*/

	strcpy (buf, argv[0]);
	p = strrchr (buf, '/');
	if (p != NULL) {
		*p = 0;
		lchd = _chdir (buf);
	}

/* Process command line switches */

	for (i = 1; i < argc; i++) {
		p = argv[i];
		if (*p++ != '-') {		// Process filename in command line
			if (PROCESSFILE) break;
			p--;
			strcpy (m_pgen_input, p);
			strcpy (m_pgen_output, p);
			p2 = strrchr (m_pgen_output, '.');
			if (p2 != NULL)
				strcpy (p2, ".res");
			else
				strcpy (m_pgen_output+strlen (m_pgen_output), ".res");
			PROCESSFILE = 1;
			continue;
		}
		switch (*p++) {

/* Accept a -A switch indicating an alternate set of INI files */
/* are to be used. */

		case 'A':
		case 'a':
			named_ini_files = 0;
			while (isspace (*p)) p++;
			while (isdigit (*p)) {
				named_ini_files = named_ini_files * 10 + (*p - '0');
				p++;
			}
			break;

#if defined (__linux__) || defined (__FreeBSD__) || defined (__APPLE__)

/* -B - put in the background.  Accepts an optional CPU count. */

		case 'B':
		case 'b':
			while (isspace (*p)) p++;
			if (isdigit (*p)) {
				background = 0;
				while (isdigit (*p)) {
					background = background * 10 + (*p - '0');
					p++;
				}
			} else
				background = 1;
			break;

#endif

/* -D - debug */

		case 'D':
		case 'd':
			VERBOSE = TRUE;
			NO_GUI = FALSE;
			break;

/* -O - Option */

		case 'O':
		case 'o':
			p2 = strchr (p, '=');
			if (p2 == NULL)					// Ignore an invalid option...
				break;
			if (opcnt >= 10)				// Maximum 10 options...
				break;
			strcpy (values[opcnt], p2+1);
			p2 = keywords[opcnt];
			while (*p != '=')
				*p2++ = *p++;
			*p2 = '\0';
			opcnt++;
			break;

/* -T - ThreadsPerTest */					// Set a ThreadsPerTest=<number> option

		case 'T':
		case 't':
			if (opcnt >= 10)				// Maximum 10 options...
				break;
			strcpy (keywords[opcnt], "ThreadsPerTest");
			while (isspace (*p))
				p++;
			p2 = values[opcnt];
			while (isdigit (*p))
				*p2++ = *p++;
			*p2 = '\0';
			opcnt++;
			break;


/* -Q - Test a single k*b^n+c or b^n-b^m+c or (k*b^n+c)/d or (k*b^n+c)^2-2 number */

		case 'Q':
		case 'q':
			if (*p == '(') {				// must begin with digits or (
//				quotient = TRUE;
			    leftbracket = 1;
				p++;
			}
			if (!isdigit(*p))				// must be digits,now...
				goto errexpr;
			p2 = multiplier;				// get the expected multiplier
			while (isdigit(*p))				// copy the digits
				*p2++ = *p++;
			*p2 = '\0';
			if (*p == '\0') {				// Multiplier alone
				strcpy (base, "2");			// No matter...
				strcpy (exponent, "0");		// Exponent = zero
				strcpy (addin, "+0");		// c = +zero
				goto DIGITSONLY;
			}
			if (*p == '^')	{				// the multiplier was ommitted
				strcpy (base, multiplier);		// get the base in place
				strcpy (multiplier, "1");		// default multiplier = 1
				p++;
				goto NOMULTIPLIER;
			}
			else if (*p++ != '*') {				// now, get the base
			    errnb = 1;
				goto errexpr;
			}
			if (!isdigit(*p)) {				// it must be a digit string
			    errnb = 2;
				goto errexpr;
			}
			p2 = base;
			while (isdigit(*p))				// copy the digits
				*p2++ = *p++;
			*p2 = '\0';
			if (*p++ != '^') {				// must be power
			    errnb = 3;
				goto errexpr;
			}
NOMULTIPLIER:
			if (!isdigit(*p)) {				// must be digits...
			    errnb = 4;
				goto errexpr;
			}
			p2 = exponent;					// get the exponent
			while (isdigit(*p))				// copy the digits
				*p2++ = *p++;
			*p2 = '\0';
			if (*p != '+' && *p != '-') {			// must be plus or minus
			    errnb = 5;
				goto errexpr;
			}
			p2 = addin;
			*p2++ = *p++;					// copy the sign
			if (!isdigit(*p)) {
			    errnb = 6;
				goto errexpr;
			}
			while (isdigit(*p))				// copy the c value
				*p2++ = *p++; 
			*p2 = '\0';
			dnflag = ' ';					// restore the dnflag
			if (*p == ')') {
			    if (*(p+1) == '/') {			// get the divisor(s)
				quotient = TRUE;
				p += 2;
				strcpy (divisors, p);			// and that is all...
			    }
			}
			else {
				if ((*p != '\0')) { 
					if ((addin[0] != '-') || (*p != '^')) { // must be end of string or diffnum...
					    errnb = 7;
					    goto errexpr;
					}
					if (strcmp (base, addin+1)) {	// The second base must be the same...
					    errnb = 8;
						goto errexpr;
					}
					p++;
					if (!isdigit(*p)) {		// must be digits...
					    errnb = 9;
						goto errexpr;
					}
					p2 = exponent2;			// get the exponent
					while (isdigit(*p))		// copy the digits
						*p2++ = *p++;
					*p2 = '\0';
					if (*p != '+' && *p != '-') {	// must be plus or minus
					    errnb = 10;
						goto errexpr;
					}
					dnflag = *p;			// dnflag = sign
					p2 = addin;
					*p2++ = *p++;			// copy the sign
					if (!isdigit(*p)) {
					    errnb = 11;
						goto errexpr;
					}
					while (isdigit(*p))		// copy the c value
						*p2++ = *p++; 
				}
			}
			*p2 = '\0';
DIGITSONLY:
			in = fopen ("$temp.npg", "w");	// open the temporary input file
			if ((dnflag == '+') || (dnflag == '-')) {
				fprintf (in, "ABC $a^$b-$a^$c$d\n");// write ABC header and data
				fprintf (in, "%s %s %s %s\n", base, exponent, exponent2, addin);
			}
			else {
				if (quotient) {
					fprintf (in, "ABC ($a*$b^$c$d)/$e\n");// write ABC header and data
					fprintf (in, "%s %s %s %s %s\n", multiplier, base, exponent, addin, divisors);
				}
				else if (leftbracket){
					fprintf (in, "ABC ($a^$b$c)^2-2\n");// write ABC header and data
					fprintf (in, "%s %s %s\n", base, exponent, addin);
				}
				else	{
					fprintf (in, "ABC $a*$b^$c$d\n");// write ABC header and data
					fprintf (in, "%s %s %s %s\n", multiplier, base, exponent, addin);
				}
			}
			fclose (in);
			strcpy (m_pgen_input, "$temp.npg");
			strcpy (m_pgen_output, "$temp.res");
 			PROCESSFILE = 1;
			SINGLETEST = 1;
			break;

/* -H - help */

		case 'H':
		case 'h':
		case '?':
			goto usage;

/* -M - Menu */

		case 'M':
		case 'm':
			MENUING = TRUE;
			NO_GUI = FALSE;
			break;

/* -V - version number */

		case 'V':
		case 'v':
			printf ("LLR Program - Version "LLR_VERSION", using Gwnum Library Version "GWNUM_VERSION"\n");
			return (0); 

/* -W - use a different working directory */

		case 'W':
		case 'w':
			lchd = _chdir (p);
			break; 

/* Otherwise unknown switch */

		default:
			printf ("Invalid switch\n");
			goto usage;
		}
	}

#if defined (__linux__) || defined (__FreeBSD__) || defined (__APPLE__)

/* Run in background if requested.  Code courtesy of Francois Gouget. */

	if (background) {
		int	i;

/* To enter daemon mode, close all the filedescs and detach from the tty */

		for (i = 0; i < background; i++) {
			int	fd;
			fd = fork ();
			if (fd == -1) {
				perror ("Could not fork to the background");
				exit (1);
			}
			if (fd != 0) {
				exit (0);
			}
			for (fd = 0; fd < OPEN_MAX; fd++) {
				close (fd);
			}
			open ("/dev/null", O_APPEND);
			dup2 (0,1);
			dup2 (0,2);
			setsid ();

			if (i) named_ini_files = i;
		}
	}

#endif


/* Determine the names of the INI files */
/* Read the INI files */
//	trace(0);
	nameIniFiles (named_ini_files);
//	trace(1);

        readIniFiles ();
//		trace(2);

// Copy the options in the init. file

	for (i=0; i< opcnt; i++) {
			IniWriteString (INI_FILE, keywords[i] , NULL);		// Delete the line
			IniWriteString (INI_FILE, keywords[i] , values[i]);	// Make a new line
	}

#ifdef MPRIME_LOADAVG

/* Read load averaging settings from INI files */

	IniGetString (INI_FILE, "MaxLoad", buf, sizeof (buf), "0");
	HI_LOAD = atof (buf);
	IniGetString (INI_FILE, "MinLoad", buf, sizeof (buf), "0");
	LO_LOAD = atof (buf);
	IniGetString (INI_FILE, "PauseTime", buf, sizeof (buf), "0");
	LOAD_CHECK_TIME = atol (buf);

/* Initialise load checking */

	if (HI_LOAD > 0.0 && LOAD_CHECK_TIME > 0)
		init_load_check ();
#endif

// Process the file name

	if (PROCESSFILE) {
			IniWriteInt (INI_FILE, "Work", 0);
			IniGetString (INI_FILE, "PgenInputFile", oldm_pgen_input, 80, "blablabla.bla");
			IniWriteString (INI_FILE, "PgenInputFile", m_pgen_input);
			if (strcmp (m_pgen_input, oldm_pgen_input) || IniGetInt (INI_FILE, "WorkDone", 0)) {
				IniWriteString (INI_FILE, "PgenOutputFile", m_pgen_output);
				if (!IniGetInt (INI_FILE, "PgenLine", 0))
					IniWriteInt (INI_FILE, "PgenLine", 1);
				IniWriteInt (INI_FILE, "WorkDone", 0);
			}
	}

	readIniFiles ();	// Read again the ini files that may have been updated...

/* Bring up the main menu */

	if (MENUING)
            main_menu ();
        
/* Continue testing the range */

	else
            linuxContinue ("Another llr is already running!\n");

/* All done */

	return (0);

/* Invalid args message */

#if defined (__linux__) || defined (__FreeBSD__) || defined (__APPLE__)
usage:	printf ("Usage: llr [-aN] [-bdhmoqv] [-wDIR] [input file name]\n");
#else
usage:	printf ("Usage: cllr [-aN] [-dhmoqv] [-wDIR] [input file name]\n");
#endif
	printf ("-aN\tUse an alternate set of INI and output files.\n");
#if defined (__linux__) || defined (__FreeBSD__) || defined (__APPLE__)
	printf ("-bN\tRun in the background.\n");
#endif
	printf ("-d\tPrint detailed information to stdout.\n");
	printf ("-h\tPrint this.\n");
	printf ("-m\tMenu to configure llr.\n");
	printf ("-okeyword=value\tSet an option in .ini file.\n");
	printf ("-q\"expression\"\tTest a single k*b^n+c or b^n-b^m+c number.\n");
	printf ("-v\tPrint the version number.\n");
	printf ("-wDIR\tRun from a different working directory.\n");
	printf ("\n");
	return (1);

/* Invalid expression message */

errexpr:	printf ("Invalid expression in command line : %s, errnb = %d\n", argv[i]+2, errnb);
			return (2);
}

void title (char *msg)
#if defined (__linux__) || defined (__FreeBSD__) || defined (__APPLE__)
{
}
#else
{
	char buf[80];
	int i;
	sprintf (buf, "title %s", msg);
	for (i=0;i<80;i++)
		if (buf[i]=='^')
			buf[i] = (char)0x88;	// For Windows extended ASCII...
	system (buf);
}
#endif

void flashWindowAndBeep (unsigned long n)
{
	while (n--)
		printf ("\007\r");
		Sleep (500);
}

/* Return TRUE if we should stop calculating */

int escapeCheck ()
{
	if (THREAD_STOP) {
		THREAD_STOP = 0;
		return (TRUE);
	}
	return (FALSE);
}

void doMiscTasks ()
{
#ifdef MPRIME_LOADAVG
	test_sleep ();
#endif
}

void OutputStr (char *buf)
{
	if (VERBOSE || MENUING) printf ("%s", buf);
}

#if defined (__linux__) || defined (__FreeBSD__) || defined (__APPLE__)

void Sleep (
	long	ms) 
{
	sleep (ms/1000);
}

/* Set priority.  Map one (prime95's lowest priority) to 20 */
/* (linux's lowest priority).  Map eight (prime95's normal priority) to */
/* 0 (linux's normal priority). */

void SetPriority ()
{
	int	p;
	p = (8 - (int) PRIORITY) * 20 / 7;
	setpriority (PRIO_PROCESS, getpid (), p);
#if !defined (__APPLE__)
	if (CPU_AFFINITY != 99) {
		char affstring [80];
		sprintf (affstring, "taskset -p %d %d\n", CPU_MASK, getpid());
		if (system (affstring) != 0);
	}
#endif
}

#else

void SetPriority (void)
{
	SetPriorityClass (GetCurrentProcess (),
		(PRIORITY > 6) ? NORMAL_PRIORITY_CLASS : IDLE_PRIORITY_CLASS);
	SetThreadPriority (GetCurrentThread (),
		(PRIORITY == 1) ? THREAD_PRIORITY_IDLE :
		(PRIORITY == 2 || PRIORITY == 7) ? THREAD_PRIORITY_LOWEST :
		(PRIORITY == 3 || PRIORITY == 8) ? THREAD_PRIORITY_BELOW_NORMAL :
		(PRIORITY == 4 || PRIORITY == 9) ? THREAD_PRIORITY_NORMAL :
		(PRIORITY == 5 || PRIORITY == 10) ? THREAD_PRIORITY_ABOVE_NORMAL :
		THREAD_PRIORITY_HIGHEST);
	if (CPU_AFFINITY != 99)
		SetProcessAffinityMask (GetCurrentProcess (), CPU_MASK);
}

#endif

void BlinkIcon (int x)
{
}

void ChangeIcon (int x)
{
}

void ReplaceableLine (int x)
{
}

/* This routine calls primeContinue unless there is another copy of mprime */
/* already running.  In that case, it outputs an optional error message. */

void linuxContinue (
	char	*error_message)
{
	int completed = FALSE;

#ifdef __APPLE__
#define PROCNAME	"/proc/%d/exe"
#endif
#ifdef __linux__
#define PROCNAME	"/proc/%d/exe"
#endif
#ifdef __FreeBSD__
#define PROCNAME	"/proc/%d/file"
#endif

#if defined (__linux__) || defined (__FreeBSD__) || defined (__APPLE__)

	pid_t	my_pid, running_pid;
	char	filename[30];
	int	fd;
	struct stat filedata;
	ino_t	inode1, inode2;

/* Compare this process' ID and the pid from the INI file */

	my_pid = getpid ();
	openIniFile (INI_FILE, 1);
	running_pid = IniGetInt (INI_FILE, "Pid", 0);
	if (running_pid == 0 || my_pid == running_pid) goto ok;

/* See if the two pids are running the same executable */

	sprintf (filename, PROCNAME, my_pid);
	fd = _open (filename, _O_RDONLY);
	if (fd < 0) goto ok;
	fstat (fd, &filedata);
	inode1 = filedata.st_ino;
	_close (fd);
	sprintf (filename, PROCNAME, running_pid);
	fd = _open (filename, _O_RDONLY);
	if (fd < 0) goto ok;
	fstat (fd, &filedata);
	inode2 = filedata.st_ino;
	_close (fd);
	if (inode1 != inode2) goto ok;

/* The two pids are running the same executable, raise an error and return */

	if (error_message != NULL) printf ("%s", error_message);
	return;

/* All is OK.  Save our pid, run, then delete our pid */

ok:	IniWriteInt (INI_FILE, "Pid", my_pid);

#endif

/* Allocate a work unit structure */

	w = (struct work_unit *) malloc (sizeof (struct work_unit));
//	if (w == NULL) goto nomem;
	memset (w, 0, sizeof (struct work_unit));
//	trace(1);
	completed = primeContinue ();
	IniWriteInt (INI_FILE, "Pid", 0);
	_unlink ("$temp.npg");
	_unlink ("$temp.res");
	if (PROCESSFILE && IniGetInt (INI_FILE, "WorkDone", 0))
			IniWriteString (INI_FILE, "PgenLine", NULL);
	if (SINGLETEST && completed)
		_unlink (INI_FILE);
}
