/*
Interface between the curses shim, the SDL frontend and the game.
This file is part of Omega (GPL-3.0-or-later).
*/
#ifndef OMEGA_SDL_FRONTEND_HPP_
#define OMEGA_SDL_FRONTEND_HPP_

#include "curses.h"

#include <string>
#include <vector>

// ---- composited virtual screen, owned by sdlcurses.cpp ----
struct sdl_cell
{
  chtype ch;
  const WINDOW *src;
};

struct sdl_screen
{
  int lines = 0, cols = 0;
  std::vector<sdl_cell> cells;
  int cursor_y = 0, cursor_x = 0;
  int cursor_visibility = 0;
  const WINDOW *cursor_win = nullptr; // window that last asked for input
  const WINDOW *map_win    = nullptr; // the level window (drawn with tiles)
};
const sdl_screen &sdlc_screen();
void sdlc_resized(int lines, int cols); // frontend -> shim

struct sdl_rgb
{
  uint8_t r, g, b;
};
sdl_rgb sdlc_color(int index);                // 0..15
void sdlc_pair(int pair, int &fg, int &bg);   // pair -> colour indices

// ---- frontend, implemented in frontend.cpp ----
bool fe_init(int &lines, int &cols);
void fe_shutdown();
void fe_mark_dirty();             // screen changed, render soon
void fe_present_if_due();         // render now if enough time passed
int fe_get_key(int timeout_ms);   // ERR on timeout
void fe_unget_key(int key);
void fe_flash();
void fe_delay(int ms);            // pump events + render while waiting
bool fe_pop_mouse(MEVENT &);
void fe_push_mouse_front(const MEVENT &);

// ---- game side, implemented in sdl_hooks.cpp (has access to game state) ----
struct fe_map_state
{
  bool valid      = false;
  bool country    = false;
  std::string env;           // "city", "caves", ... used for tile variants
  int hoff = 0, voff = 0;    // world coords of level window cell (0,0)
  int px = -1, py = -1;      // player world position
  int world_w = 0, world_h = 0;
};
void omega_map_state(fe_map_state &);
// Layer keys for the world cell (x,y) currently showing glyph `shown`, bottom first.
// A key "glyph" means: draw the shown character itself.
void omega_cell_keys(int x, int y, chtype shown, std::vector<std::string> &out);
int omega_context_key();          // what A does in the command loop
std::string omega_context_label();
void omega_window_closed();      // save and exit
void omega_debug_change_env(int); // test scripts only

#endif
