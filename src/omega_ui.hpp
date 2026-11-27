/*
Game-facing hooks for the SDL/gamepad frontend. Compiles to no-ops in the
plain curses build so the game code can use them unconditionally.

This file is part of Omega (GPL-3.0-or-later).
*/
#ifndef OMEGA_UI_HPP_
#define OMEGA_UI_HPP_

// What kind of input the game is waiting for. The frontend uses this to decide
// what the buttons do, what the hint bar says and when to pop the keyboard.
enum ui_mode
{
  UI_PROMPT,    // generic single-key prompt (default)
  UI_COMMAND,   // main command loop
  UI_DIRECTION, // getdir()
  UI_TARGET,    // cursor targeting (ends with '.')
  UI_LOOK,      // examine cursor (ends with ESC/Enter)
  UI_YESNO,     // ynq()
  UI_MORE,      // --MORE--
  UI_TEXT,      // free text entry
  UI_NUMBER,    // number entry
  UI_COUNT,     // "how many?" with < and >
  UI_MENU,      // scrollable interactive menu
  UI_TITLE,     // title screen: arrow-highlight menu + name entry
};

#ifdef OMEGA_SDL
void ui_push_mode(ui_mode);
void ui_pop_mode();
#else
inline void ui_push_mode(ui_mode) {}
inline void ui_pop_mode() {}
#endif

struct ui_mode_scope
{
  explicit ui_mode_scope(ui_mode m) { ui_push_mode(m); }
  ~ui_mode_scope() { ui_pop_mode(); }
  ui_mode_scope(const ui_mode_scope &)            = delete;
  ui_mode_scope &operator=(const ui_mode_scope &) = delete;
};

#endif
