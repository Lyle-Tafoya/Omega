/****************************************************************/
/*			    CURSES.H				*/
/* Header file for definitions and declarations for the		*/
/* PCcurses package. This should be #include'd in all user	*/
/* programs.							*/
/****************************************************************/
/* This version of curses is based on ncurses, a curses version	*/
/* originally written by Pavel Curtis at Cornell University.	*/
/* I have made substantial changes to make it run on IBM PC's,	*/
/* and therefore consider myself free to make it public domain.	*/
/*				Bjorn Larsson (bl@infovox.se)	*/
/****************************************************************/
/* 1.4:  Window origin mod in overlay() and overwrite(),	*/
/*	 on public (and very reasonable) request. Swapped	*/
/*	 #define'd values of OK and ERR; OK now 1, and		*/
/*	 ERR is 0/NULL. Conforms better to UNIX versions.	*/
/*	 borderchars[] removed from WINDOW struct since		*/
/*	 the border() functions were re-defined. Use of		*/
/*	 short wherever possible. Portability improve-		*/
/*	 ments, mispelled name of [w]setscrreg():	900114	*/
/* 1.3:	 All modules lint-checked with MSC '-W3' and		*/
/*	 Turbo'C' '-w -w-pro' switches. Support for		*/
/*	 border(), wborder() functions:			881005	*/
/* 1.2:	 Rcsid[] string in all modules, for mainte-		*/
/*	 nance:						881002	*/
/* 1.1:	 'Raw' output routines, revision info in		*/
/*	 curses.h:					880306	*/
/* 1.0:	 Release:					870515	*/
/****************************************************************/

/* general definitions */

#ifndef	 bool
#define  bool		char		/* boolean type */
#endif

#ifndef	 TRUE
#define	 TRUE		1		/* booleans */
#endif

#ifndef	 FALSE
#define	 FALSE		0
#endif

#define	 ERR		0		/* general error flag */
#define	 OK		1		/* general OK flag */

/* functions defined as macros */

#define getch()	   wgetch(stdscr)	/* using macroes allows you to use */
#define	ungetch(c) wungetch(c)		/* #undef getch/ungetch in your */
					/* programs to use MSC/TRC getch() */
					/* and ungetch() routines */

#define getyx(win,y,x)   	(y = (win)->_cury, x = (win)->_curx)

#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif
#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

/* video attribute definitions. Colour definitions for */
/* CGA/EGA should also go here... */

#define	A_ALTCHARSET   0
#define	A_BLINK        0x100
#define	A_BLANK        0x200
#define	A_BOLD         0x400
#define	A_DIM          0
#define	A_PROTECT      0
#define	A_REVERSE      0x800
#define	A_STANDOUT     0x1000
#define	A_UNDERLINE    0x2000

/* function and keypad key definitions. Many are just for compatibility */

#define KEY_BREAK      0x101        /* Not on PC */
#define KEY_DOWN       0x102        /* The four arrow keys */
#define KEY_UP         0x103
#define KEY_LEFT       0x104
#define KEY_RIGHT      0x105
#define KEY_HOME       0x106        /* Home key */
#define KEY_BACKSPACE  0x107        /* Not on PC */
#define KEY_F0         0x108        /* Function keys. Space for */
#define KEY_F(n)       (KEY_F0+(n)) /*  64 keys is reserved. */
#define KEY_DL         0x148        /* Not on PC */
#define KEY_IL         0x149        /* Insert line */
#define KEY_DC         0x14a        /* Delete character */
#define KEY_IC         0x14b        /* Insert char or enter insert mode */
#define KEY_EIC        0x14c        /* Exit insert char mode */
#define KEY_CLEAR      0x14d        /* Clear screen */
#define KEY_EOS        0x14e        /* Clear to end of screen */
#define KEY_EOL        0x14f        /* Clear to end of line */
#define KEY_SF         0x150        /* Scroll 1 line forward */
#define KEY_SR         0x151        /* Scroll 1 line backwards (reverse) */
#define KEY_NPAGE      0x152        /* Next page */
#define KEY_PPAGE      0x153        /* Previous page */
#define KEY_STAB       0x154        /* Set tab */
#define KEY_CTAB       0x155        /* Clear tab */
#define KEY_CATAB      0x156        /* Clear all tabs */
#define KEY_ENTER      0x157        /* Enter or send (unreliable) */
#define KEY_SRESET     0x158        /* soft (partial) reset (unreliable) */
#define KEY_RESET      0x159        /* reset or hard reset (unreliable) */
#define KEY_PRINT      0x15a        /* print or copy */
#define KEY_LL         0x15b        /* home down or bottom (lower left) */
#define KEY_ABORT      0x15c        /*  Abort/Terminate key (any) */
#define KEY_SHELP      0x15d        /* Short help */
#define KEY_LHELP      0x15e        /* Long help */

/* type declarations */

typedef struct
  {
  short	   _cury;			/* Current pseudo-cursor */
  short	   _curx;
  short	   _maxy;			/* Max coordinates */
  short	   _maxx;
  short	   _begy;			/* Origin on screen */
  short	   _begx;
  short	   _flags;			/* Window properties */
  short	   _attrs;			/* Attributes of written characters */
  short	   _tabsize;			/* Tab character size */
  bool	   _clear;			/* Causes clear at next refresh */
  bool	   _leave;			/* Leaves cursor as it happens */
  bool	   _scroll;			/* Allows window scrolling */
  bool	   _nodelay;			/* Input character wait flag */
  bool	   _keypad;			/* Flags keypad key mode active */
  short   **_line;			/* Pointer to line pointer array */
  short	  *_minchng;			/* First changed character in line */
  short	  *_maxchng;			/* Last changed character in line */
  short	   _regtop;			/* Top of scrolling region */
  short	   _regbottom;			/* Bottom of scrolling region */
  }	WINDOW;

/* External variables */

extern	int	LINES;			/* terminal height */
extern	int	COLS;			/* terminal width */
extern	WINDOW *curscr;			/* the current screen image */
extern	WINDOW *stdscr;			/* the default screen window */

/* PCcurses function declarations */

extern	int	 addch();		/* put char in stdscr */
extern	int	 addrawch();		/* put char in stdscr, raw */
extern	int	 addstr();		/* put string in stdscr */
extern	void	 attrset();		/* set stdscr char attributes */
extern	void	 attroff();		/* clear attribute(a) stdscr */
extern	void	 attron();		/* add attribute(s) stdscr */
extern	int	 baudrate();		/* compatibility dummy */
extern	void	 beep();		/* sound bell */
extern	void	 border();		/* Set non-std box characters */
extern	void	 box();			/* draw a box around a window */
extern	void	 cbreak();		/* set terminal cbreak mode */
extern	void	 clear();		/* clear stdscr */
extern	void	 clearok();		/* marks a window for screen clear */
extern	int	 clrtobot();		/* clear end of stdscr */
extern	int	 clrtoeol();		/* clear end of line in stdscr */
extern	void	 crmode();		/* set terminal cbreak mode */
extern	void	 cursoff();		/* turns off hardware cursor */
extern	void	 curson();		/* turns on hardware cursor */
extern	void	 def_prog_mode();	/* save TTY modes */
extern	void	 def_shell_mode();	/* compatibility dummy */
extern	int	 delch();		/* delete a char in stdscr */
extern	int	 deleteln();		/* delete a line in stdscr */
extern	void	 delwin();		/* delete a window or a subwindow */
extern  void	 doupdate();		/* update physical screen */
extern	void	 echo();		/* set terminal echo mode */
extern	int	 endwin();		/* cleanup and finitialization */
extern	void	 erase();		/* erase stdscr */
extern	int	 erasechar();		/* return char kill character */
extern	int	 fixterm();		/* compatibility dummy */
extern	void	 flash();		/* flash terminal screen */
extern	void	 flushinp();		/* kill pending keyboard input */
extern  int	 getstr();		/* get string to stdscr and buffer */
extern	int	 gettmode();		/* compatibility dummy */
extern	void	 idlok();		/* use ins/del line (dummy) */
extern	int	 initscr();		/* curses initialization */
extern  int	 inch();		/* get char at stdscr cursor */
extern	int	 insch();		/* insert character in stdscr */
extern	int	 insrawch();		/* insert character in stdscr, raw */
extern	int	 insertln();		/* insert new line in stdscr */
extern	void	 keypad();		/* marks a window for keypad usage */
extern	int	 killchar();		/* return line kill character */
extern	char	*longname();		/* terminal description */
extern	void	 leaveok();		/* marks window for cursor 'leave' */
extern	void	 meta();		/* marks window for meta (dummy) */
extern	int	 move();		/* move cursor in stdscr */
extern	int	 mvaddch();		/* move & put char in stdscr */
extern	int	 mvaddrawch();		/* move & put char in stdscr, raw */
extern	int	 mvaddstr();		/* move & put string in stdscr */
extern	int	 mvclrtobot();		/* move & clear end of stdscr */
extern	int	 mvclrtoeol();		/* move & clear lineend in stdscr */
extern	int	 mvcur();		/* move terminal cursor */
extern	int	 mvdelch();		/* move & delete a char in stdscr */
extern	int	 mvdeleteln();		/* move & delete a line in stdscr */
extern	int	 mvgetch();		/* move & get char to stdscr */
extern	int	 mvgetstr();		/* move & get string to stdscr */
extern	int	 mvinch();		/* move & get char at stdscr cursor */
extern	int	 mvinsch();		/* move & insert char in stdscr */
extern	int	 mvinsrawch();		/* move & insert raw char in stdscr */
extern	int	 mvinsertln();		/* move & insert new line in stdscr */
extern	int	 mvprintw();		/* move & print string in stdscr */
extern	int	 mvscanw();		/* move & get values via stdscr */
extern	int	 mvwaddch();		/* move & put char in a window */
extern	int	 mvwaddrawch();		/* move & put char in a window, raw */
extern	int	 mvwaddstr();		/* move & put string in a window */
extern	int	 mvwclrtobot();		/* move & clear end of a window */
extern	int	 mvwclrtoeol();		/* move & clear lineend in a window */
extern	int	 mvwdelch();		/* move & delete a char in a window */
extern	int	 mvwdeleteln();		/* move & delete a line in a window */
extern	int	 mvwgetch();		/* move & get char to a window */
extern	int	 mvwgetstr();		/* move & get string to a window */
extern	int	 mvwinch();		/* move & get char at window cursor */
extern	int	 mvwinsch();		/* move & insert char in a window */
extern	int	 mvwinsrawch();		/* move & insert raw char in window */
extern	int	 mvwinsertln();		/* move & insert new line in window */
extern	int	 mvwin();		/* move window */
extern	int	 mvwprintw();		/* move & print string in a window */
extern	int	 mvwscanw();		/* move & get values via a window */
extern	WINDOW	*newwin();		/* create a window */
extern	void	 nl();			/* set terminal cr-crlf map mode */
extern	void	 nocbreak();		/* unset terminal cbreak mode */
extern	void	 nocrmode();		/* unset terminal cbreak mode */
extern	void	 nodelay();		/* marks window for no input wait */
extern	void	 noecho();		/* unset terminal echo mode */
extern	void	 nonl();		/* unset terminal cr-crlf map mode */
extern	void	 noraw();		/* unset raw terminal mode */
extern	void	 overlay();		/* overlay one window on another */
extern	void	 overwrite();		/* overwrite one window on another */
extern	int	 printw();		/* print string in stdscr */
extern	void	 raw();			/* set raw terminal mode */
extern	void	 refrbrk();		/* set screen refresh break mode */
extern	void	 refresh();		/* refresh stdscr */
extern	int	 resetterm();		/* compatibility dummy */
extern	int	 resetty();		/* restore terminal I/O modes */
extern	void	 reset_prog_mode();	/* restore terminal I/O modes */
extern	void	 reset_shell_mode();	/* set terminal to default modes */
extern	int	 saveoldterm();		/* compatibility dummy */
extern	int	 saveterm();		/* compatibility dummy */
extern	int	 savetty();		/* save terminal I/O modes */
extern	int	 scanw();		/* get values via stdscr */
extern	void	 scroll();		/* scroll region in a window */
extern	void	 scrollok();		/* marks a window to allow scroll */
extern	int	 setscrreg();		/* define stdscr's scroll region */
extern	int	 setterm();		/* compatibility dummy */
extern	int	 setupterm();		/* set up terminal (no-op) */
extern	void	 standend();		/* start normal chars in stdscr */
extern	void	 standout();		/* start standout chars in stdscr */
extern	WINDOW	*subwin();		/* create a sub-window */
extern	int	 tabsize();		/* set/get tabsize of stdscr */
extern	void	 touchwin();		/* mark a window as modified */
extern	char	*unctrl();		/* char-to-string converter */
extern	int	 waddch();		/* put char in a window */
extern	int	 waddrawch();		/* put char in a window, raw */
extern	int	 waddstr();		/* put string in a window */
extern	void	 wattroff();		/* clear attribute(a) in window */
extern	void	 wattron();		/* add attribute(s) in window */
extern	void	 wattrset();		/* set window char attributes */
extern	void	 wborder();		/* Set non-std box characters */
extern	int	 wbox();		/* draw a box inside a window */
extern	void	 wclear();		/* clear a window */
extern	int	 wclrtobot();		/* clear end of a window */
extern	int	 wclrtoeol();		/* clear end of line in a window */
extern	int	 wdelch();		/* delete a char in a window */
extern	int	 wdeleteln();		/* delete a line in a window */
extern	void	 werase();		/* erase a window */
extern	int	 wgetch();		/* get char to a window */
extern  int	 wgetstr();		/* get string to window and buffer */
extern	int	 winch();		/* get char at window cursor */
extern	int	 winsch();		/* insert character in a window */
extern	int	 winsrawch();		/* insert raw character in a window */
extern	int	 winsertln();		/* insert new line in a window */
extern	int	 wmove();		/* move cursor in a window */
extern	void	 wnoutrefresh();	/* create screen image, w/o display */
extern	int	 wprintw();		/* print string in a window */
extern	void	 wrefresh();		/* refresh screen */
extern	int	 wscanw();		/* get values via a window */
extern	int	 wsetscrreg();		/* define a window's scroll region */
extern	void	 wstandend();		/* start normal chars in window */
extern	void	 wstandout();		/* start standout chars in window */
extern	int	 wtabsize();		/* set/get tabsize of a window */
extern	int	 wungetch();		/* character push-back */
