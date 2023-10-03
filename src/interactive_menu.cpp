/*
Copyright (C) Lyle Tafoya, 2019, 2021-2023

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

#include "defs.h"
#include "scr.h"
#include "interactive_menu.hpp"

extern bool IsMenu;

interactive_menu::interactive_menu(WINDOW *menu_window, uint16_t width, uint16_t height)
    : menu_window(menu_window), width(width), height(height) {};

void interactive_menu::load(const std::vector<std::string> &lines)
{
  this->lines = lines;
  header.clear();
  position = 0;
}

void interactive_menu::load(const std::vector<std::string> &lines, const std::vector<std::string> &header)
{
  this->lines = lines;
  this->header = header;
  position = 0;
}

void interactive_menu::resize(uint16_t width, uint16_t height)
{
  wresize(menu_window, height, width);
  this->width = width;
  this->height = height;
  uint16_t body_size = height-header.size();
  uint16_t max_increment = lines.size() > body_size ? lines.size()-body_size : 0;
  if(position > max_increment)
  {
    position = max_increment;
  }
}

void interactive_menu::move(uint16_t x, uint16_t y)
{
  mvwin(menu_window, y, x);
}

void interactive_menu::reset_position()
{
  position = 0;
}

void interactive_menu::print()
{
  werase(menu_window);
  for(size_t i = 0; i < header.size(); ++i)
  {
    mvwaddstr(menu_window, i, 0, header[i].c_str());
  }
  for(size_t i = 0; i + position < lines.size() && i + header.size() < height; ++i)
  {
    if(header.size() + i == static_cast<size_t>(height)-1 && i + position + 1 < lines.size())
    {
      mvwaddstr(menu_window, i+header.size(), 0, "** MORE **");
    }
    else
    {
      mvwaddstr(menu_window, i+header.size(), 0, lines[i + position].c_str());
    }
  }
  wrefresh(menu_window);
}

int interactive_menu::get_player_input()
{
  IsMenu = true;
  while(true)
  {
    print();
    int player_input = get_message_input();
    if(player_input == KEY_UP)
    {
      position = position == 0 ? 0 : position - 1;
    }
    else if(player_input == KEY_DOWN)
    {
      uint16_t body_size = height-header.size();
      uint16_t max_increment = lines.size() > body_size ? lines.size()-body_size : 0;
      position = position + 1 >= max_increment ? max_increment : position + 1;
    }
    else if(player_input == ' ')
    {
      uint16_t body_size = height-header.size();
      uint16_t max_increment = lines.size() > body_size ? lines.size()-body_size : 0;
      position = position + body_size >= max_increment ? max_increment : position + body_size;
    }
    else
    {
      IsMenu = false;
      return player_input;
    }
  }
}

