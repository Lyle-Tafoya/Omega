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

// abyss.cpp
// some functions to make the abyss level and run the final challenge

#include "glob.h"

#include <filesystem>
#include <format>
#include <regex>

// loads the abyss level into Level
void load_abyss()
{
  TempLevel = std::make_unique<level>();
  Level = TempLevel.get();
  clear_level(Level);

  std::string filepath{std::format("{}abyss.dat", Omegalib)};
  FILE *fd   = checkfopen(filepath, "rb");
  char site = cryptkey("abyss.dat");
  for(int y = 0; y < LENGTH; ++y)
  {
    for(int x = 0; x < WIDTH; ++x)
    {
      site                         = getc(fd) ^ site;
      Level->site[x][y].roomnumber = RS_ADEPT;
      switch(site)
      {
        case '0':
          Level->site[x][y].locchar = VOID_CHAR;
          Level->site[x][y].p_locf  = L_VOID;
          break;
        case 'V':
          Level->site[x][y].locchar = VOID_CHAR;
          Level->site[x][y].p_locf  = L_VOID_STATION;
          break;
        case '1':
          Level->site[x][y].locchar = FLOOR;
          Level->site[x][y].p_locf  = L_VOICE1;
          break;
        case '2':
          Level->site[x][y].locchar = FLOOR;
          Level->site[x][y].p_locf  = L_VOICE2;
          break;
        case '3':
          Level->site[x][y].locchar = FLOOR;
          Level->site[x][y].p_locf  = L_VOICE3;
          break;
        case '~':
          Level->site[x][y].locchar = WATER;
          Level->site[x][y].p_locf  = L_WATER_STATION;
          break;
        case ';':
          Level->site[x][y].locchar = FIRE;
          Level->site[x][y].p_locf  = L_FIRE_STATION;
          break;
        case '"':
          Level->site[x][y].locchar = HEDGE;
          Level->site[x][y].p_locf  = L_EARTH_STATION;
          break;
        case '6':
          Level->site[x][y].locchar = WHIRLWIND;
          Level->site[x][y].p_locf  = L_AIR_STATION;
          break;
        case '#':
          Level->site[x][y].locchar = WALL;
          break;
        case '.':
          Level->site[x][y].locchar = FLOOR;
          break;
      }
    }
    site = getc(fd) ^ site;
  }
  fclose(fd);
}
