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

#ifndef OMEGA_OBJECT_H_
#define OMEGA_OBJECT_H_

#include "curses.h"

#include <string>

struct object
{
  int id;
  int weight;
  int plus;
  int charge;
  int dmg;
  int hit;
  int aux;
  int number;
  int fragility;
  long basevalue;
  unsigned char known;
  unsigned char used;
  int blessing;
  unsigned char type;
  unsigned char uniqueness;
  int on_use;
  int on_equip;
  int on_unequip;
  unsigned char level;
  chtype objchar;
  std::string objstr;
  std::string truename;
  std::string cursestr;
};

#endif
