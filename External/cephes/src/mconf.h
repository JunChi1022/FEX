/*							mconf.h
 *
 *	Common include file for math routines
 *
 *
 *
 * SYNOPSIS:
 *
 * #include "mconf.h"
 *
 *
 *
 * DESCRIPTION:
 *
 * This file contains definitions for error codes that are
 * passed to the common error handling routine mtherr()
 * (which see).
 *
 * The file also includes a conditional assembly definition
 * for the type of computer arithmetic (IEEE, DEC, Motorola
 * IEEE, or UNKnown).
 * 
 * For Digital Equipment PDP-11 and VAX computers, certain
 * IBM systems, and others that use numbers with a 56-bit
 * significand, the symbol DEC should be defined.  In this
 * mode, most floating point constants are given as arrays
 * of octal integers to eliminate decimal to binary conversion
 * errors that might be introduced by the compiler.
 *
 * For little-endian computers, such as IBM PC, that follow the
 * IEEE Standard for Binary Floating Point Arithmetic (ANSI/IEEE
 * Std 754-1985), the symbol IBMPC should be defined.  These
 * numbers have 53-bit significands.  In this mode, constants
 * are provided as arrays of hexadecimal 16 bit integers.
 *
 * Big-endian IEEE format is denoted MIEEE.  On some RISC
 * systems such as Sun SPARC, double precision constants
 * must be stored on 8-byte address boundaries.  Since integer
 * arrays may be aligned differently, the MIEEE configuration
 * may fail on such machines.
 *
 * To accommodate other types of computer arithmetic, all
 * constants are also provided in a normal decimal radix
 * which one can hope are correctly converted to a suitable
 * format by the available C language compiler.  To invoke
 * this mode, define the symbol UNK.
 *
 * An important difference among these modes is a predefined
 * set of machine arithmetic constants for each.  The numbers
 * MACHEP (the machine roundoff error), MAXNUM (largest number
 * represented), and several other parameters are preset by
 * the configuration symbol.  Check the file const.c to
 * ensure that these values are correct for your computer.
 *
 * Configurations NANS, INFINITIES, MINUSZERO, and DENORMAL
 * may fail on many systems.  Verify that they are supposed
 * to work on your computer.
 */

/*
Cephes Math Library Release 2.3:  June, 1995
Copyright 1984, 1987, 1989, 1995 by Stephen L. Moshier
*/


/* Constant definitions for math error conditions
 */

#include "SoftFloat-3e/platform.h"
#include "SoftFloat-3e/softfloat.h"

#define DOMAIN		1	/* argument domain error */
#define SING		2	/* argument singularity */
#define OVERFLOW	3	/* overflow range error */
#define UNDERFLOW	4	/* underflow range error */
#define TLOSS		5	/* total loss of precision */
#define PLOSS		6	/* partial loss of precision */

#define EDOM		33
#define ERANGE		34

/* Complex numeral.  */
typedef struct
	{
	double r;
	double i;
	} cmplx;

typedef struct
	{
	float r;
	float i;
	} cmplxf;

/* Long double complex numeral.  */

typedef struct
	{
	float128_t r;
	float128_t i;
	} cmplxl;


/* Type of computer arithmetic */

/* PDP-11, Pro350, VAX:
 */
/* #define DEC 1 */

/* Intel IEEE, low order words come first:
 */
#define IBMPC 1

/* Motorola IEEE, high order words come first
 * (Sun 680x0 workstation):
 */
/* #define MIEEE 1 */

/* UNKnown arithmetic, invokes coefficients given in
 * normal decimal format.  Beware of range boundary
 * problems (MACHEP, MAXLOG, etc. in const.c) and
 * roundoff problems in pow.c:
 * (Sun SPARCstation)
 */
/* #define UNK 1 */

/* If you define UNK, then be sure to set BIGENDIAN properly. */
/* #define BIGENDIAN 1 */

/* Define this `volatile' if your compiler thinks
 * that floating point arithmetic obeys the associative
 * and distributive laws.  It will defeat some optimizations
 * (but probably not enough of them).
 *
 * #define VOLATILE volatile
 */
#define VOLATILE

/* For 12-byte long doubles on an i386, pad a 16-bit short 0
 * to the end of real constants initialized by integer arrays.
 *
 * #define XPD 0,
 *
 * Otherwise, the type is 10 bytes long and XPD should be
 * defined blank (e.g., Microsoft C).
 *
 * #define XPD
 */
#define XPD 0,

/* Define to support tiny denormal numbers, else undefine. */
#define DENORMAL 1

/* Define to ask for infinity support, else undefine. */
#define INFINITIES 1

/* Define to ask for support of numbers that are Not-a-Number,
   else undefine.  This may automatically define INFINITIES in some files. */
#define NANS 1

/* Define to distinguish between -0.0 and +0.0.  */
#define MINUSZERO 1

/* Define 1 for ANSI C atan2() function
   and ANSI prototypes for float arguments.
   See atan.c and clog.c. */
#define ANSIC 1

/* Variable for error reporting.  See mtherr.c.  */
extern int merror;

/* Forward declarations */
extern float128_t MINLOGL;

extern float128_t MAXNUML;
extern float128_t PIL;
extern float128_t PIO2L, PIO4L;

float128_t cephes_f128_atanl(float128_t x);
#if ANSIC
float128_t cephes_f128_atan2l( float128_t y, float128_t x );
#else
float128_t cephes_f128_atan2l( float128_t x, float128_t y );
#endif
float128_t cephes_f128_ceill(float128_t x);
float128_t cephes_f128_cosl(float128_t x);
float128_t cephes_f128_fabsl(float128_t x);
float128_t cephes_f128_floorl(float128_t x);
float128_t cephes_f128_frexpl( float128_t x, int *pw2 );
int cephes_f128_isfinitel(float128_t x);
int cephes_f128_isnanl(float128_t x);
float128_t cephes_f128_ldexpl( float128_t x, int pw2 );
float128_t cephes_f128_polevll( float128_t x, void *PP, int n );
float128_t cephes_f128_p1evll( float128_t x, void *PP, int n );
int cephes_f128_signbitl(float128_t x);
float128_t cephes_f128_sinl(float128_t x);
int mtherr( char *name, int code );

/* Public symbol declarations */
float128_t cephes_f128_log2l(float128_t x);
