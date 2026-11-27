/*
SDL2 curses shim for Omega.

Implements the subset of the curses API that Omega uses, backed by an SDL2
renderer (see sdlcurses.cpp / frontend.cpp). The game code keeps talking curses;
the frontend draws the level window with tiles and everything else as text, and
turns gamepad input into the keys the game expects.

This file is part of Omega (GPL-3.0-or-later).
*/
#ifndef OMEGA_SDL_CURSES_H_
#define OMEGA_SDL_CURSES_H_

#include <cstdint>
#include <vector>

#define OMEGA_SDL_CURSES 1

typedef uint32_t chtype;
typedef uint32_t attr_t;
typedef uint32_t mmask_t;

#define ERR (-1)
#define OK  0

#define A_NORMAL     0u
#define A_CHARTEXT   0x000000ffu
#define A_COLOR      0x0000ff00u
#define A_ATTRIBUTES 0xffffff00u
#define A_BOLD       0x00010000u
#define A_REVERSE    0x00020000u
#define A_STANDOUT   0x00040000u
#define A_UNDERLINE  0x00080000u
#define A_BLINK      0x00100000u
#define A_DIM        0x00200000u
#define WA_NORMAL    A_NORMAL
#define WA_BOLD      A_BOLD
#define WA_REVERSE   A_REVERSE
#define WA_STANDOUT  A_STANDOUT
#define WA_UNDERLINE A_UNDERLINE

#define COLOR_PAIR(n)  ((chtype)(((n) & 0xff) << 8))
#define PAIR_NUMBER(a) ((int)(((a) & A_COLOR) >> 8))

#define COLOR_BLACK   0
#define COLOR_RED     1
#define COLOR_GREEN   2
#define COLOR_YELLOW  3
#define COLOR_BLUE    4
#define COLOR_MAGENTA 5
#define COLOR_CYAN    6
#define COLOR_WHITE   7

// key codes (ncurses values)
#define KEY_DOWN      0402
#define KEY_UP        0403
#define KEY_LEFT      0404
#define KEY_RIGHT     0405
#define KEY_HOME      0406
#define KEY_BACKSPACE 0407
#define KEY_F0        0410
#define KEY_F(n)      (KEY_F0 + (n))
#define KEY_DC        0512
#define KEY_NPAGE     0522
#define KEY_PPAGE     0523
#define KEY_ENTER     0527
#define KEY_LL        0533
#define KEY_END       0550
#define KEY_MOUSE     0631
#define KEY_RESIZE    0632

// mouse
#define BUTTON1_RELEASED       0x00000001u
#define BUTTON1_PRESSED        0x00000002u
#define BUTTON1_CLICKED        0x00000004u
#define BUTTON1_DOUBLE_CLICKED 0x00000008u
#define BUTTON1_TRIPLE_CLICKED 0x00000010u
#define BUTTON2_RELEASED       0x00000040u
#define BUTTON2_PRESSED        0x00000080u
#define BUTTON2_CLICKED        0x00000100u
#define BUTTON2_DOUBLE_CLICKED 0x00000200u
#define BUTTON2_TRIPLE_CLICKED 0x00000400u
#define BUTTON3_RELEASED       0x00001000u
#define BUTTON3_PRESSED        0x00002000u
#define BUTTON3_CLICKED        0x00004000u
#define BUTTON3_DOUBLE_CLICKED 0x00008000u
#define BUTTON3_TRIPLE_CLICKED 0x00010000u
#define BUTTON4_RELEASED       0x00040000u
#define BUTTON4_PRESSED        0x00080000u
#define BUTTON4_CLICKED        0x00100000u
#define BUTTON4_DOUBLE_CLICKED 0x00200000u
#define BUTTON4_TRIPLE_CLICKED 0x00400000u
#define BUTTON5_RELEASED       0x01000000u
#define BUTTON5_PRESSED        0x02000000u
#define BUTTON5_CLICKED        0x04000000u
#define BUTTON5_DOUBLE_CLICKED 0x08000000u
#define BUTTON5_TRIPLE_CLICKED 0x10000000u
#define ALL_MOUSE_EVENTS       0x1fffffffu
#define REPORT_MOUSE_POSITION  0x20000000u

struct MEVENT
{
  short id;
  int x, y, z;
  mmask_t bstate;
};

struct WINDOW
{
  int h, w;        // size
  int by, bx;      // origin on screen
  int cy = 0, cx = 0;
  attr_t attrs = 0;
  int delay    = -1; // wtimeout
  bool keypad  = false;
  bool clear_on_refresh = false;
  std::vector<chtype> cells;
  std::vector<int> dirty_first, dirty_last; // per-line dirty span, -1 = clean
};

extern WINDOW *stdscr;
extern WINDOW *curscr;
extern int LINES, COLS, COLORS, COLOR_PAIRS, ESCDELAY;

WINDOW *initscr();
int endwin();
bool isendwin();
int noecho();
int echo();
int cbreak();
int crmode();
int raw();
int noraw();
int nonl();
int start_color();
bool has_colors();
bool can_change_color();
int init_pair(short pair, short fg, short bg);
int init_color(short color, short r, short g, short b);
int pair_content(short pair, short *fg, short *bg);
int curs_set(int visibility);
int set_escdelay(int);
int beep();
int flash();
int napms(int ms);

WINDOW *newwin(int nlines, int ncols, int begin_y, int begin_x);
int delwin(WINDOW *);
int mvwin(WINDOW *, int y, int x);
int wresize(WINDOW *, int lines, int cols);
int touchwin(WINDOW *);
int wnoutrefresh(WINDOW *);
int wrefresh(WINDOW *);
int doupdate();
int werase(WINDOW *);
int wclear(WINDOW *);
int wclrtoeol(WINDOW *);
int wclrtobot(WINDOW *);
int wmove(WINDOW *, int y, int x);
int waddch(WINDOW *, chtype);
int waddstr(WINDOW *, const char *);
int waddnstr(WINDOW *, const char *, int n);
int wprintw(WINDOW *, const char *fmt, ...);
int mvwprintw(WINDOW *, int y, int x, const char *fmt, ...);
int printw(const char *fmt, ...);
int mvprintw(int y, int x, const char *fmt, ...);
int wattrset(WINDOW *, attr_t);
int wattron(WINDOW *, attr_t);
int wattroff(WINDOW *, attr_t);
int wattr_on(WINDOW *, attr_t, void *);
int wattr_off(WINDOW *, attr_t, void *);
int wcolor_set(WINDOW *, short pair, void *);
int wstandout(WINDOW *);
int wstandend(WINDOW *);
int box(WINDOW *, chtype, chtype);
chtype winch(WINDOW *);
int wgetch(WINDOW *);
int ungetch(int);
int wtimeout(WINDOW *, int delay);
int keypad(WINDOW *, bool);
int nodelay(WINDOW *, bool);
int flushinp();

int getmouse(MEVENT *);
int ungetmouse(MEVENT *);
mmask_t mousemask(mmask_t newmask, mmask_t *oldmask);
int mouseinterval(int);

inline int getcury(const WINDOW *w) { return w->cy; }
inline int getcurx(const WINDOW *w) { return w->cx; }
inline int getmaxy(const WINDOW *w) { return w->h; }
inline int getmaxx(const WINDOW *w) { return w->w; }
inline int getbegy(const WINDOW *w) { return w->by; }
inline int getbegx(const WINDOW *w) { return w->bx; }
#define getyx(win, y, x)    ((y) = getcury(win), (x) = getcurx(win))
#define getmaxyx(win, y, x) ((y) = getmaxy(win), (x) = getmaxx(win))
#define getbegyx(win, y, x) ((y) = getbegy(win), (x) = getbegx(win))

inline int mvwaddch(WINDOW *w, int y, int x, chtype c) { return wmove(w, y, x) == ERR ? ERR : waddch(w, c); }
inline int mvwaddstr(WINDOW *w, int y, int x, const char *s) { return wmove(w, y, x) == ERR ? ERR : waddstr(w, s); }
inline int mvwgetch(WINDOW *w, int y, int x) { return wmove(w, y, x) == ERR ? ERR : wgetch(w); }
inline int addch(chtype c) { return waddch(stdscr, c); }
inline int addstr(const char *s) { return waddstr(stdscr, s); }
inline int mvaddch(int y, int x, chtype c) { return mvwaddch(stdscr, y, x, c); }
inline int mvaddstr(int y, int x, const char *s) { return mvwaddstr(stdscr, y, x, s); }
inline int move(int y, int x) { return wmove(stdscr, y, x); }
inline int erase() { return werase(stdscr); }
inline int clear() { return wclear(stdscr); }
inline int clrtoeol() { return wclrtoeol(stdscr); }
inline int clrtobot() { return wclrtobot(stdscr); }
inline int refresh() { return wrefresh(stdscr); }
inline int getch() { return wgetch(stdscr); }
inline int attrset(attr_t a) { return wattrset(stdscr, a); }
inline int attron(attr_t a) { return wattron(stdscr, a); }
inline int attroff(attr_t a) { return wattroff(stdscr, a); }
inline int standout() { return wstandout(stdscr); }
inline int standend() { return wstandend(stdscr); }
inline int timeout(int d) { return wtimeout(stdscr, d); }

// ---- SDL frontend extensions (not curses) ----
void sdlc_set_map_window(WINDOW *);

#endif
