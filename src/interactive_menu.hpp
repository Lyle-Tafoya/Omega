/*
Copyright (C) by Lyle Tafoya, 2019, 2021-2023

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

#ifndef OMEGA_INTERACTIVE_MENU_HPP_
#define OMEGA_INTERACTIVE_MENU_HPP_

#include <curses.h>
#include <string>
#include <vector>

class interactive_menu
{
public:
  interactive_menu(WINDOW *menu_window, uint16_t width, uint16_t height);
  int get_player_input();
  void print();
  void load(const std::vector<std::string> &lines, const std::vector<std::string> &header);
  void load(const std::vector<std::string> &lines);
  void resize(uint16_t width, uint16_t height);
  void move(uint16_t x, uint16_t y);
  void reset_position();

private:
  WINDOW *menu_window;
  std::vector<std::string> lines;
  std::vector<std::string> header;
  uint16_t position = 0;
  uint16_t width;
  uint16_t height;
};

#endif
