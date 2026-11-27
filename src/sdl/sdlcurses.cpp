/*
SDL2 curses shim: windows, compositing and the input entry points.
This file is part of Omega (GPL-3.0-or-later).
*/
#include "curses.h"
#include "frontend.hpp"

#include <algorithm>
#include <array>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <vector>

WINDOW *stdscr = nullptr;
WINDOW *curscr = nullptr;
int LINES = 0, COLS = 0, COLORS = 16, COLOR_PAIRS = 256, ESCDELAY = 0;

namespace
{
sdl_screen screen;
bool ended = false;
std::vector<WINDOW *> windows;
std::array<sdl_rgb, 16> palette = {{
  {0, 0, 0}, {178, 24, 24}, {24, 178, 24}, {178, 104, 24},
  {24, 24, 178}, {178, 24, 178}, {24, 178, 178}, {178, 178, 178},
  {104, 104, 104}, {255, 84, 84}, {84, 255, 84}, {255, 255, 84},
  {84, 84, 255}, {255, 84, 255}, {84, 255, 255}, {255, 255, 255},
}};
std::array<std::array<short, 2>, 256> pairs{}; // fg, bg

void mark(WINDOW *w, int y, int x0, int x1)
{
  if(y < 0 || y >= w->h)
  {
    return;
  }
  int &f = w->dirty_first[y];
  int &l = w->dirty_last[y];
  if(f < 0)
  {
    f = x0;
    l = x1;
  }
  else
  {
    f = std::min(f, x0);
    l = std::max(l, x1);
  }
}

void touch_all(WINDOW *w)
{
  for(int y = 0; y < w->h; ++y)
  {
    w->dirty_first[y] = 0;
    w->dirty_last[y]  = w->w - 1;
  }
}

void alloc(WINDOW *w, int h, int wd)
{
  h  = std::max(1, h);
  wd = std::max(1, wd);
  std::vector<chtype> cells(static_cast<size_t>(h) * wd, ' ');
  for(int y = 0; y < std::min(h, w->h); ++y)
  {
    for(int x = 0; x < std::min(wd, w->w); ++x)
    {
      cells[y * wd + x] = w->cells[y * w->w + x];
    }
  }
  w->h = h;
  w->w = wd;
  w->cells.swap(cells);
  w->dirty_first.assign(h, -1);
  w->dirty_last.assign(h, -1);
  w->cy = std::min(w->cy, h - 1);
  w->cx = std::min(w->cx, wd - 1);
  touch_all(w);
}

int put(WINDOW *w, chtype c)
{
  unsigned char ch = c & A_CHARTEXT;
  if(ch == '\n')
  {
    wclrtoeol(w);
    w->cx = 0;
    if(w->cy < w->h - 1)
    {
      ++w->cy;
    }
    return OK;
  }
  if(ch == '\r')
  {
    w->cx = 0;
    return OK;
  }
  if(ch == '\t')
  {
    int n = 8 - (w->cx % 8);
    while(n-- > 0)
    {
      put(w, ' ' | (c & A_ATTRIBUTES));
    }
    return OK;
  }
  if(ch == '\b')
  {
    if(w->cx > 0)
    {
      --w->cx;
    }
    return OK;
  }
  if(w->cy >= w->h)
  {
    return ERR;
  }
  chtype attrs = c & A_ATTRIBUTES;
  chtype wattr = w->attrs;
  if((attrs & A_COLOR) == 0)
  {
    attrs |= wattr & A_COLOR;
  }
  attrs |= wattr & ~A_COLOR;
  w->cells[w->cy * w->w + w->cx] = ch | attrs;
  mark(w, w->cy, w->cx, w->cx);
  if(++w->cx >= w->w)
  {
    w->cx = 0;
    if(w->cy < w->h - 1)
    {
      ++w->cy;
    }
    else
    {
      w->cx = w->w - 1;
      return ERR;
    }
  }
  return OK;
}

int vw_printw(WINDOW *w, const char *fmt, va_list ap)
{
  char buf[4096];
  vsnprintf(buf, sizeof buf, fmt, ap);
  return waddstr(w, buf);
}
} // namespace

const sdl_screen &sdlc_screen()
{
  return screen;
}

sdl_rgb sdlc_color(int index)
{
  return palette[index & 15];
}

void sdlc_pair(int pair, int &fg, int &bg)
{
  if(pair == 0)
  {
    fg = 7;
    bg = 0;
    return;
  }
  fg = pairs[pair & 0xff][0];
  bg = pairs[pair & 0xff][1];
}

void sdlc_set_map_window(WINDOW *w)
{
  screen.map_win = w;
}

void sdlc_resized(int lines, int cols)
{
  LINES        = lines;
  COLS         = cols;
  screen.lines = lines;
  screen.cols  = cols;
  screen.cells.assign(static_cast<size_t>(lines) * cols, {' ', nullptr});
  if(stdscr)
  {
    alloc(stdscr, lines, cols);
  }
}

WINDOW *initscr()
{
  int lines = 40, cols = 128;
  if(!fe_init(lines, cols))
  {
    fprintf(stderr, "omega: could not start the SDL frontend\n");
    exit(1);
  }
  stdscr = new WINDOW{};
  alloc(stdscr, lines, cols);
  windows.push_back(stdscr);
  curscr = stdscr;
  sdlc_resized(lines, cols);
  for(int i = 0; i < 16; ++i)
  {
    pairs[i] = {static_cast<short>(i), 0};
  }
  return stdscr;
}

int endwin()
{
  if(!ended)
  {
    ended = true;
    fe_shutdown();
  }
  return OK;
}

bool isendwin() { return ended; }
int noecho() { return OK; }
int echo() { return OK; }
int cbreak() { return OK; }
int crmode() { return OK; }
int raw() { return OK; }
int noraw() { return OK; }
int nonl() { return OK; }
int start_color() { return OK; }
bool has_colors() { return true; }
bool can_change_color() { return true; }
int set_escdelay(int) { return OK; }
int beep() { fe_flash(); return OK; }
int flash() { fe_flash(); return OK; }

int napms(int ms)
{
  fe_delay(ms);
  return OK;
}

int init_pair(short pair, short fg, short bg)
{
  if(pair < 0 || pair > 255)
  {
    return ERR;
  }
  pairs[pair] = {fg, bg};
  return OK;
}

int init_color(short color, short r, short g, short b)
{
  if(color < 0 || color > 15)
  {
    return ERR;
  }
  palette[color] = {static_cast<uint8_t>(r * 255 / 1000), static_cast<uint8_t>(g * 255 / 1000),
                    static_cast<uint8_t>(b * 255 / 1000)};
  fe_mark_dirty();
  return OK;
}

int pair_content(short pair, short *fg, short *bg)
{
  *fg = pairs[pair & 0xff][0];
  *bg = pairs[pair & 0xff][1];
  return OK;
}

int curs_set(int visibility)
{
  int old                  = screen.cursor_visibility;
  screen.cursor_visibility = visibility;
  fe_mark_dirty();
  return old;
}

WINDOW *newwin(int nlines, int ncols, int begin_y, int begin_x)
{
  if(nlines <= 0)
  {
    nlines = LINES - begin_y;
  }
  if(ncols <= 0)
  {
    ncols = COLS - begin_x;
  }
  auto *w = new WINDOW{};
  w->by   = begin_y;
  w->bx   = begin_x;
  alloc(w, nlines, ncols);
  windows.push_back(w);
  return w;
}

int delwin(WINDOW *w)
{
  if(!w)
  {
    return ERR;
  }
  std::erase(windows, w);
  for(auto &c : screen.cells)
  {
    if(c.src == w)
    {
      c.src = nullptr;
    }
  }
  if(screen.cursor_win == w)
  {
    screen.cursor_win = nullptr;
  }
  if(screen.map_win == w)
  {
    screen.map_win = nullptr;
  }
  delete w;
  return OK;
}

int mvwin(WINDOW *w, int y, int x)
{
  w->by = y;
  w->bx = x;
  touch_all(w);
  return OK;
}

int wresize(WINDOW *w, int lines, int cols)
{
  alloc(w, lines, cols);
  return OK;
}

int touchwin(WINDOW *w)
{
  touch_all(w);
  return OK;
}

int wnoutrefresh(WINDOW *w)
{
  if(!w)
  {
    return ERR;
  }
  if(w->clear_on_refresh)
  {
    w->clear_on_refresh = false;
    for(auto &c : screen.cells)
    {
      c = {' ', nullptr};
    }
  }
  for(int y = 0; y < w->h; ++y)
  {
    int f = w->dirty_first[y];
    if(f < 0)
    {
      continue;
    }
    int l  = w->dirty_last[y];
    int sy = w->by + y;
    if(sy >= 0 && sy < screen.lines)
    {
      for(int x = f; x <= l && x < w->w; ++x)
      {
        int sx = w->bx + x;
        if(sx >= 0 && sx < screen.cols)
        {
          screen.cells[sy * screen.cols + sx] = {w->cells[y * w->w + x], w};
        }
      }
    }
    w->dirty_first[y] = w->dirty_last[y] = -1;
  }
  screen.cursor_y = w->by + w->cy;
  screen.cursor_x = w->bx + w->cx;
  fe_mark_dirty();
  return OK;
}

int doupdate()
{
  fe_present_if_due();
  return OK;
}

int wrefresh(WINDOW *w)
{
  wnoutrefresh(w);
  return doupdate();
}

int werase(WINDOW *w)
{
  std::fill(w->cells.begin(), w->cells.end(), static_cast<chtype>(' '));
  w->cy = w->cx = 0;
  touch_all(w);
  return OK;
}

int wclear(WINDOW *w)
{
  werase(w);
  w->clear_on_refresh = true;
  return OK;
}

int wclrtoeol(WINDOW *w)
{
  for(int x = w->cx; x < w->w; ++x)
  {
    w->cells[w->cy * w->w + x] = ' ';
  }
  mark(w, w->cy, w->cx, w->w - 1);
  return OK;
}

int wclrtobot(WINDOW *w)
{
  wclrtoeol(w);
  for(int y = w->cy + 1; y < w->h; ++y)
  {
    for(int x = 0; x < w->w; ++x)
    {
      w->cells[y * w->w + x] = ' ';
    }
    mark(w, y, 0, w->w - 1);
  }
  return OK;
}

int wmove(WINDOW *w, int y, int x)
{
  if(!w || y < 0 || x < 0 || y >= w->h || x >= w->w)
  {
    return ERR;
  }
  w->cy = y;
  w->cx = x;
  return OK;
}

int waddch(WINDOW *w, chtype c)
{
  return put(w, c);
}

int waddnstr(WINDOW *w, const char *s, int n)
{
  int r = OK;
  for(int i = 0; s[i] && (n < 0 || i < n); ++i)
  {
    r = put(w, static_cast<unsigned char>(s[i]));
  }
  return r;
}

int waddstr(WINDOW *w, const char *s)
{
  return waddnstr(w, s, -1);
}

int wprintw(WINDOW *w, const char *fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);
  int r = vw_printw(w, fmt, ap);
  va_end(ap);
  return r;
}

int mvwprintw(WINDOW *w, int y, int x, const char *fmt, ...)
{
  if(wmove(w, y, x) == ERR)
  {
    return ERR;
  }
  va_list ap;
  va_start(ap, fmt);
  int r = vw_printw(w, fmt, ap);
  va_end(ap);
  return r;
}

int printw(const char *fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);
  int r = vw_printw(stdscr, fmt, ap);
  va_end(ap);
  return r;
}

int mvprintw(int y, int x, const char *fmt, ...)
{
  if(wmove(stdscr, y, x) == ERR)
  {
    return ERR;
  }
  va_list ap;
  va_start(ap, fmt);
  int r = vw_printw(stdscr, fmt, ap);
  va_end(ap);
  return r;
}

int wattrset(WINDOW *w, attr_t a) { w->attrs = a; return OK; }
int wattron(WINDOW *w, attr_t a)
{
  if(a & A_COLOR)
  {
    w->attrs &= ~A_COLOR;
  }
  w->attrs |= a;
  return OK;
}
int wattroff(WINDOW *w, attr_t a)
{
  w->attrs &= ~a;
  return OK;
}
int wattr_on(WINDOW *w, attr_t a, void *) { return wattron(w, a); }
int wattr_off(WINDOW *w, attr_t a, void *) { return wattroff(w, a); }
int wcolor_set(WINDOW *w, short pair, void *)
{
  w->attrs = (w->attrs & ~A_COLOR) | COLOR_PAIR(pair);
  return OK;
}
int wstandout(WINDOW *w) { return wattron(w, A_STANDOUT); }
int wstandend(WINDOW *w) { w->attrs = 0; return OK; }

int box(WINDOW *w, chtype v, chtype h)
{
  v = v ? v : '|';
  h = h ? h : '-';
  for(int x = 0; x < w->w; ++x)
  {
    w->cells[x]                     = h;
    w->cells[(w->h - 1) * w->w + x] = h;
  }
  for(int y = 0; y < w->h; ++y)
  {
    w->cells[y * w->w]            = v;
    w->cells[y * w->w + w->w - 1] = v;
  }
  touch_all(w);
  return OK;
}

chtype winch(WINDOW *w)
{
  return w->cells[w->cy * w->w + w->cx];
}

int wgetch(WINDOW *w)
{
  wnoutrefresh(w); // like curses: getch refreshes the window
  screen.cursor_win = w;
  screen.cursor_y   = w->by + w->cy;
  screen.cursor_x   = w->bx + w->cx;
  return fe_get_key(w->delay);
}

int ungetch(int c)
{
  fe_unget_key(c);
  return OK;
}

int wtimeout(WINDOW *w, int delay)
{
  w->delay = delay;
  return OK;
}

int keypad(WINDOW *w, bool on)
{
  w->keypad = on;
  return OK;
}

int nodelay(WINDOW *w, bool on)
{
  w->delay = on ? 0 : -1;
  return OK;
}

int flushinp()
{
  while(fe_get_key(0) != ERR)
  {
  }
  return OK;
}

int getmouse(MEVENT *e)
{
  return fe_pop_mouse(*e) ? OK : ERR;
}

int ungetmouse(MEVENT *e)
{
  fe_push_mouse_front(*e);
  fe_unget_key(KEY_MOUSE);
  return OK;
}

mmask_t mousemask(mmask_t newmask, mmask_t *oldmask)
{
  if(oldmask)
  {
    *oldmask = ALL_MOUSE_EVENTS;
  }
  return newmask;
}

int mouseinterval(int)
{
  return 0;
}
