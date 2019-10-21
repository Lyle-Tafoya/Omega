/*
 * Generate C code to initialize and support Omega colors on UNIX.
 *
 * Usage:
 *   cpp -DOMEGA_CLRGEN *.[ch] | genclr <c-file> <h-file>
 * where <c-file> should be compiled and linked with the Omega binary and
 * <h-file> should be #included in all Omega sources that reference colors.
 *
 * Curses color micro-tutorial:
 *   The curses library requires color attributes to be specified as
 *   COLOR_PAIR(<n>) macro calls, where init_pair(<n>, <fore-color>,
 *   <back-color>) has previously been called.  <n> must fall in the range
 *   1..COLOR_PAIRS, where COLOR_PAIRS is a run-time read-only curses variable
 *   typically set to something like 64.
 *
 * <c-file> defines function clrgen_init(), which contains the minimal number
 * of curses init_pair() calls to support the color usages detected by this
 * program on standard input.
 *
 * <h-file> defines preprocessor variables that cause each detected color
 * usage to expand to an appropriate curses COLOR_PAIR() call.
 *
 * This approach to UNIX color support is perhaps overkill, but does have
 * advantages over these alternative approaches:
 *
 *   - hard-coded init_pair() calls and color definitions, which would
 *     require manual checking and possibly editing for color reference added
 *     or removed anywhere in the sources;
 *
 *   - replacement of color references with function calls that lazily call
 *     init_pair() as necessary, which would consume more run-time resources
 *     and behave unpredictably if the number of pairs exceeded COLOR_PAIRS;
 *
 *   - run-time analysis of color pairs required by e.g. monster list, which
 *     would be a bit more complex to code, consume more run-time resources,
 *     and require significant code changes to move all color references into
 *     static storage.
 */

#ifndef OMEGA_CLRGEN		/* this file confuses its own scanner */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Special tag cpp prepends to color symbols
 */
#define PREFIX "OMEGA_CLRGEN"

/*
 * Return whether a character could be part of a C identifier
 */
#define ISCID(c)	((c) >= 'A' && (c) <= 'Z' || \
			 (c) >= 'a' && (c) <= 'z' || \
			 (c) >= '0' && (c) <= '9' || \
			 (c) == '_')

/*
 * Colors specified in cpp output on standard input
 */
typedef struct {
    char *ofg, *obg;			/* Omega fore/background color */
    char *cfg, *cbg;			/* curses fore/background color */
    unsigned int boldfg, boldbg;	/* fore/background bold flag */
    unsigned int idx;			/* COLOR_PAIR() argument */
} ClrPair;

/*
 * Omega versus curses color names
 */
typedef struct {
    char *omega;
    char *curses;
    unsigned int bold;
} ClrEquiv;

static ClrEquiv clr_equiv[17] = {
    { "BLACK",		"BLACK",	0 },
    { "BLUE",		"BLUE",		0 },
    { "GREEN",		"GREEN",	0 },
    { "CYAN",		"CYAN",		0 },
    { "RED",		"RED",		0 },
    { "PURPLE",		"MAGENTA",	0 },
    { "BROWN",		"YELLOW",	0 },
    { "WHITE",		"WHITE",	0 },
    { "GREY",		"BLACK",	1 },
    { "LIGHT_BLUE",	"BLUE",		1 },
    { "LIGHT_GREEN",	"GREEN",	1 },
    { "LIGHT_CYAN",	"CYAN",		1 },
    { "LIGHT_RED",	"RED",		1 },
    { "LIGHT_PURPLE",	"MAGENTA",	1 },
    { "YELLOW",		"YELLOW",	1 },
    { "BRIGHT_WHITE",	"WHITE",	1 },
    { NULL, NULL, 0 } };

static char *clr_lookup (char *omega, char **curses, unsigned int *bold)
{
    /*
     * Point CURSES to the curses color corresponding to Omega color OMEGA,
     * set *BOLD to whether the bold attribute should accompany that curses
     * color, and return a copy of OMEGA.  If OMEGA is unrecognized, return
     * null.
     */
    ClrEquiv *e = clr_equiv;
    for (; e->omega; e++)
	if (!strcmp (e->omega, omega)) {
	    *curses = e->curses;
	    *bold = e->bold;
	    return e->omega;
	}
    return NULL;
}

static char *clr_scan (char *p, char **curses, unsigned int *bold, char **end)
{
    /*
     * Return a copy of the Omega color nearest the start of writable buffer
     * P, point CURSES to the corresponding curses color, and point END just
     * past the color's location in P.
     *
     * If the Omega color is unrecognized, issue an error and exit.
     */
    char c, *start, *omega;
    for (; c = *p; p++) {
	if (!ISCID (c))
	    continue;
	for (start = p++; c = *p; p++) {
	    if (ISCID (c))
		continue;
	    *p = '\0';
	    if (!(omega = clr_lookup (start, curses, bold))) {
		fprintf (stderr, "unrecognized Omega color \"%s\"\n", start);
		exit (1);
	    }
	    *p = c;
	    *end = p;
	    return omega;
	}
    }
    return NULL;
}

static int opaircmp (const void *pair1, const void *pair2)
{
    /*
     * qsort comparison function: return less than, equal to, or greater than
     * 0 according to whether PAIR1 precedes, coincides with, or follows PAIR2
     * in a sorted list of Omega color pairs.
     */
    ClrPair *p1 = (ClrPair *)pair1, *p2 = (ClrPair *)pair2;
    int diff = strcmp (p1->ofg, p2->ofg);
    if (diff)
	return diff;
    return strcmp (p1->obg, p2->obg);
}

static int cpaircmp (const void *pair1, const void *pair2)
{
    /*
     * qsort comparison function: return less than, equal to, or greater than
     * 0 according to whether PAIR1 precedes, coincides with, or follows PAIR2
     * in a sorted list of curses color pairs.
     */
    ClrPair *p1 = *(ClrPair **)pair1, *p2 = *(ClrPair **)pair2;
    int diff = strcmp (p1->cfg, p2->cfg);
    if (diff)
	return diff;
    return strcmp (p1->cbg, p2->cbg);
}

static FILE *emitopen (char *file, char **argv)
{
    /*
     * Write to the top of FILE a suitable header based on ARGV, and return a
     * writable file pointer on FILE.  Exit on error.
     */
    FILE *fp = fopen (file, "w");
    if (!fp) {
	fprintf (stderr, "error opening %s", file);
	perror ("");
	exit (1);
    }
    fprintf (fp, "\
/*\n\
 * Do not edit this file.  It was automatically generated by running:\n\
 *   %s %s %s\n\
 */\n\
\n\
",
	     argv[0], argv[1], argv[2]);
    return fp;
}

static void emitclose (FILE *fp, char *file)
{
    /*
     * Close FP attached to FILE, exiting on error.
     */
    if (fclose (fp) == 0)
	return;
    fprintf (stderr, "error closing %s", file);
    perror ("");
    exit (1);    
}

int main (int argc, char **argv)
{
    char line[1024], *p;
    unsigned int i, j, nopairs = 0, ncpairs, opairslen = 80, one;
    ClrPair *pair;
    ClrPair *opairs;			/* Omega color pairs */
    ClrPair **cpairs;			/* curses color pairs */
    char *cfile, *hfile;
    FILE *fp;

    if (argc != 3) {
	fprintf (stderr, "usage: %s <c-file> <h-file>\n", argv[0]);
	exit (1);
    }
    cfile = argv[1];
    hfile = argv[2];

    /*
     * Accumulate Omega color pairs from standard input into pairs.
     */
    opairs = (ClrPair *)malloc (opairslen * sizeof (ClrPair));
    while (fgets (line, 1024, stdin)) {
	for (p = line; p = strstr (p, PREFIX);) {
	    p += sizeof (PREFIX) - 1;
	    if (nopairs == opairslen) {
		opairslen *= 2;
		opairs = (ClrPair *)realloc (opairs, opairslen *
					     sizeof (ClrPair));
	    }
	    pair = opairs + nopairs++;
	    one = *p++ == '1';
	    pair->ofg = clr_scan (p, &pair->cfg, &pair->boldfg, &p);
	    pair->obg = one ?
		clr_lookup ("BLACK", &pair->cbg, &pair->boldbg) :
		    clr_scan (p, &pair->cbg, &pair->boldbg, &p);
	    if (pair->boldbg)
		fprintf (stderr, "warning: \"%s\": bg bold unimplemented\n",
			 pair->obg);
	}
    }
    if (!nopairs) {
	fputs ("no colors detected in standard input\n", stderr);
	exit (1);
    }

    /*
     * Remove duplicate Omega color pairs.
     */
    qsort (opairs, nopairs, sizeof (ClrPair), opaircmp);
    for (i = 0, j = 1; j < nopairs; j++) {
	if (opaircmp (opairs + i, opairs + j))
	    opairs[++i] = opairs[j];
    }
    nopairs = i + 1;

    /*
     * Construct a list of unique curses color pairs, and instantiate all
     * ClrPair.idx fields.
     */
    cpairs = (ClrPair **)malloc (nopairs * sizeof (ClrPair *));
    for (i = 0; i < nopairs; i++)
	cpairs[i] = opairs + i;
    qsort (cpairs, nopairs, sizeof (ClrPair *), cpaircmp);
    cpairs[0]->idx = 1;

    for (i = 0, j = 1; j < nopairs; j++) {
	if (cpaircmp (cpairs + i, cpairs + j))
	    cpairs[++i] = cpairs[j];
	cpairs[j]->idx = i + 1;
    }
    ncpairs = i + 1;

    /*
     * Emit .c file.
     */
    fp = emitopen (cfile, argv);
    fprintf (fp, "\
#include <curses.h>\n\
#include <stdio.h>\n\
#include <stdlib.h>\n\

#include \"%s\"\n\

void clrgen_init (void)\n\
""{\n\
    if (%d > COLOR_PAIRS - 1) {\n\
	endwin();
	fputs (\"Too many color pairs!\\n\", stderr);\n\
	exit (1);\n\
    }\n\
",
	     hfile, ncpairs);
    for (i = 0; i < ncpairs; i++)
	fprintf (fp, "\
    init_pair (%d, COLOR_%s, COLOR_%s);\n\
",
		 cpairs[i]->idx, cpairs[i]->cfg, cpairs[i]->cbg);
    fputs ("\
""}\n\
",
	   fp);
    emitclose (fp, cfile);

    /*
     * Emit .h file.
     */
    fp = emitopen (hfile, argv);
    for (i = 0; i < nopairs; i++) {
	pair = opairs + i;
	fprintf (fp, "#define CLR_%s_%s\t%sCOLOR_PAIR(%d)%s\n",
		 pair->ofg, pair->obg,
		 strlen (pair->ofg) + strlen (pair->obg) > 10 ? "" : "\t",
		 pair->idx, pair->boldfg ? "|A_BOLD" : "");
    }
    fputs ("\
\n\
extern void clrgen_init (void);\n\
",
	    fp);
    emitclose (fp, hfile);

    return 0;
}

#endif   /* !OMEGA_CLRGEN */
