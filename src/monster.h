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

#ifndef OMEGA_MONSTER_H_
#define OMEGA_MONSTER_H_

#include "object.h"

#include <curses.h>
#include <forward_list>
#include <memory>
#include <string>

class monster
{
public:
  std::forward_list<std::unique_ptr<object>> possessions;
  unsigned char attacked;
  int aux1, aux2, x, y, click;
  int id, hp, hit, ac, dmg, sense, wakeup, level, speed;
  unsigned char sleep, treasure;
  long xpv;
  int corpseweight, corpsevalue, transformid, startthing;
  unsigned char uniqueness;
  int talkf, movef, meleef, strikef, specialf;
  long status, immunity;
  chtype monchar;
  std::string monstring;
  std::string corpsestr;
  std::string meleestr;

  monster() = default;
  monster(const monster &m);
  monster(int id, int hp, int hit, int ac, int dmg, int sense, int wakeup, int level, int speed, unsigned char sleep, unsigned char treasure, long xpv, int corpseweight, int corpsevalue, int transformid, int startthing, unsigned char uniqueness, int talkf, int movef, int meleef, int strikef,int specialf, long status, long immunity, chtype monchar, const std::string &monstring, const std::string &corpsestr, const std::string &meleestr);
  monster &operator=(const monster &m);
};

#endif
