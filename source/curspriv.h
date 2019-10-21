/****************************************************************/
/*			   CURSPRIV.H				*/
/* Header file for definitions and declarations for the		*/
/* PCcurses package. These definitions should not be gene-	*/
/* rally accessible to programmers.				*/
/****************************************************************/
/* This version of curses is based on ncurses, a curses version	*/
/* originally written by Pavel Curtis at Cornell University.	*/
/* I have made substantial changes to make it run on IBM PC's,	*/
/* and therefore consider myself free to make it public domain.	*/
/*				Bjorn Larsson (bl@infovox.se)	*/
/****************************************************************/
/* 1.4:  ERR/ OK redefinied in curses.h. Use of short		*/
/*	 wherever possible Portability improvements:	900114	*/
/* 1.3:	 All modules lint-checked with MSC '-W3' and		*/
/*	 Turbo'C' '-w -w-pro' switches:			881005	*/
/* 1.2:	 Support (by #ifdef UCMASM) for uppercase-only		*/
/*	 assembly routine names. If UCMASM if defined,		*/
/*	 all assembler names are #defined as upper case.	*/
/*	 Not needed if you do "MASM /MX. Also missing		*/
/* 	 declaration of cursesscroll(). Fixes thanks to		*/
/*	 N.D. Pentcheff:				881002	*/ 
/* 1.1:	 Add _chadd() for raw output routines:		880306	*/
/* 1.0:	 Release:					870515	*/
/****************************************************************/

#define	CURSES_RCS_ID	"@(#)PCcurses     v.1.4  - 900114"

/* window properties */

#define	_SUBWIN		1		/* window is a subwindow */
#define	_ENDLINE	2		/* last winline is last screen line */
#define	_FULLWIN	4		/* window fills screen */
#define	_SCROLLWIN	8		/* window lwr rgt is screen lwr rgt */

/* Miscellaneous */

#define	_INBUFSIZ	200		/* size of terminal input buffer */
#define	_NO_CHANGE	-1		/* flags line edge unchanged */

#define	_BREAKCHAR	0x03		/* ^C character */
#define _DCCHAR		0x08		/* Delete Char char (BS) */
#define _DLCHAR		0x1b		/* Delete Line char (ESC) */
#define	_GOCHAR		0x11		/* ^Q character */
#define	_PRINTCHAR	0x10		/* ^P character */
#define	_STOPCHAR	0x13		/* ^S character */
#define	 NUNGETCH	10		/* max # chars to ungetch() */

/* character mask definitions */

#define CHR_MSK	((int) 0x00ff)		/* ASCIIZ character mask */
#define	ATR_MSK	((int) 0xff00)		/* attribute mask */
#define ATR_NRM	((int) 0x0000)		/* no special attributes */

/* type declarations */

typedef	struct
  {
  WINDOW  *tmpwin;			/* window used for updates */
  short	   cursrow;			/* position of physical cursor */
  short	   curscol;
  bool	   autocr;			/* if lf -> crlf */
  bool	   cbreak;			/* if terminal unbuffered */
  bool	   echo;			/* if terminal echo */
  bool	   raw;				/* if terminal raw mode */
  bool	   refrbrk;			/* if premature refresh brk allowed */
  bool     orgcbr;			/* original MSDOS ^-BREAK setting */
  }	cursv;

/* External variables */

extern	cursv   _cursvar;		/* curses variables */

/* 'C' standard library function declarations */

#ifndef __STDIO_H
extern	char	*calloc();
extern	char	*malloc();
extern	void	 free();
extern	int	 sprintf();
extern	int	 sscanf();
#endif

/* Curses internal functions, not to be used by programmers */

/* #Define UCMASM if your version of MASM does not support */
/*  the '/MX' switch, or if you use another assembler */

#ifdef	UCMASM
#define	_cursescattr	_CURSESCATTR
#define	_cursescmode	_CURSESCMODE
#define	_cursescursor	_CURSESCURSOR
#define	_cursesgcb	_CURSESGCB
#define	_cursesgcmode	_CURSESGCMODE
#define	_cursesgcols	_CURSESGCOLS
#define	_curseskey	_CURSESKEY
#define	_cursesscroll	_CURSESSCROLL
#define	_curseskeytst	_CURSESKEYTST
#define	_cursesputc	_CURSESPUTC
#define	_cursesscb	_CURSESSCB
#endif
 
extern	int	_chadd();
extern	void	_cursescattr();
extern	void	_cursescmode();
extern	void	_cursescursor();
extern	int	_cursesgcb();
extern	int	_cursesgcmode();
extern	int	_cursesgcols();
extern	int	_curseskey();
extern	bool	_curseskeytst();
extern	void	_cursesscroll();
extern	bool	_cursespendch();
extern	void	_cursesputc();
extern	void	_cursesscb();
