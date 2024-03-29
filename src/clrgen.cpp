/*
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

#include "clrgen.h"

#include <cstdio>
#include <cstdlib>
#include <curses.h>

void clrgen_init()
{
  if(32 > COLOR_PAIRS - 1)
  {
    endwin();
    fputs("Too many color pairs!\n", stderr);
    exit(1);
  }
  init_pair(0, COLOR_BLACK, COLOR_BLACK);
  init_pair(1, COLOR_RED, COLOR_BLACK);
  init_pair(2, COLOR_GREEN, COLOR_BLACK);
  init_pair(3, COLOR_YELLOW, COLOR_BLACK);
  init_pair(4, COLOR_BLUE, COLOR_BLACK);
  init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(6, COLOR_CYAN, COLOR_BLACK);
  init_pair(7, COLOR_WHITE, COLOR_BLACK);
  init_pair(8, COLOR_BLACK + 8, COLOR_BLACK);
  init_pair(9, COLOR_RED + 8, COLOR_BLACK);
  init_pair(10, COLOR_GREEN + 8, COLOR_BLACK);
  init_pair(11, COLOR_YELLOW + 8, COLOR_BLACK);
  init_pair(12, COLOR_BLUE + 8, COLOR_BLACK);
  init_pair(13, COLOR_MAGENTA + 8, COLOR_BLACK);
  init_pair(14, COLOR_CYAN + 8, COLOR_BLACK);
  init_pair(15, COLOR_WHITE + 8, COLOR_BLACK);

  init_pair(16, COLOR_BLACK, COLOR_GREEN);
  init_pair(17, COLOR_BLACK, COLOR_RED);
  init_pair(18, COLOR_BLACK, COLOR_WHITE);
  init_pair(19, COLOR_BLACK, COLOR_YELLOW);
  init_pair(20, COLOR_BLUE, COLOR_WHITE);
  init_pair(21, COLOR_GREEN, COLOR_BLUE);
  init_pair(22, COLOR_GREEN, COLOR_RED);
  init_pair(23, COLOR_GREEN, COLOR_YELLOW);
  init_pair(24, COLOR_MAGENTA, COLOR_WHITE);
  init_pair(25, COLOR_RED, COLOR_WHITE);
  init_pair(26, COLOR_WHITE, COLOR_BLUE);
  init_pair(27, COLOR_WHITE, COLOR_RED);
  init_pair(28, COLOR_WHITE, COLOR_YELLOW);
  init_pair(29, COLOR_YELLOW, COLOR_BLUE);
  init_pair(30, COLOR_YELLOW, COLOR_RED);
  init_pair(31, COLOR_YELLOW, COLOR_WHITE);
  init_pair(32, COLOR_YELLOW, COLOR_YELLOW);
}
