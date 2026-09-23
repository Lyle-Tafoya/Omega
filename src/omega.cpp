/*
Omega copyright (C) by Laurence Raphael Brothers, 1987,1988,1989
Modifications copyright (C) by Lyle Tafoya, 2019, 2021-2023

This file is part of Omega.

Omega is free software: you can redistribute it and/or modify it under the terms
of the GNU General Public License as published by the Free Software Foundation,
either version 3 of the License, or (at your option) any later version.

Omega is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
Omega. If not, see <https://www.gnu.org/licenses/>.
*/

// Standalone entry point: signal handlers and main().
// Global variable definitions live in globals.cpp.
// Game initialisation and loop live in game_session.cpp.

#include "game_session.h"
#include "glob.h"
#include "scr.h"

#include <csignal>
#include <cstdlib>

#ifdef PDC_GL_BUILD
extern "C"
{
#  include <pdcgl.h>
}
#endif

void signalexit(int)
{
  int reply;
  queue_message("Yikes!");
  queue_message("Sorry, caught a core-dump signal.");
  queue_message("Want to try and save the game?");
  reply = ynq();
  if(reply == 'y')
  {
    save(true);
  }
  else if(reply == EOF)
  {
    signalsave();
  }
  queue_message("Bye!");
  endgraf();
  exit(0);
}

int main(int, char *[])
{
#ifdef PDC_GL_BUILD
  putenv("PDC_COLS=106");
  putenv("PDC_LINES=34");
  pdc_resize_mode = PDC_GL_RESIZE_SCALE;
#endif

#ifdef SIGINT
  signal(SIGINT, quit);
#endif
#ifdef SIGHUP
  signal(SIGHUP, signalsave);
#endif

#ifndef NO_SIGNAL_CATCH
#  ifdef SIGQUIT
  signal(SIGQUIT, signalexit);
#  endif
  signal(SIGILL, signalexit);
#  ifdef SIGTRAP
  signal(SIGTRAP, signalexit);
#  endif
  signal(SIGFPE, signalexit);
  signal(SIGSEGV, signalexit);
#  ifdef SIGIOT
  signal(SIGIOT, signalexit);
#  endif
#  ifdef SIGABRT
  signal(SIGABRT, signalexit);
#  endif
#  ifdef SIGEMT
  signal(SIGEMT, signalexit);
#  endif
#  ifdef SIGBUS
  signal(SIGBUS, signalexit);
#  endif
#  ifdef SIGSYS
  signal(SIGSYS, signalexit);
#  endif
#endif

  const InitResult result = init_game_session();
  if(result == InitResult::Failed)
  {
    return 1;
  }

  run_game_loop(result == InitResult::NewGame);
  return 0;
}
