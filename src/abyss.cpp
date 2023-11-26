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

#ifdef SAVE_LEVELS
// This stuff is in this file because the file was really small
extern level TheLevel;

void kill_levels(const std::string &str)
{
  // Remove old level files laying around
  for(auto &p : std::filesystem::directory_iterator(Omegalib))
  {
    if(std::filesystem::is_regular_file(p) && std::regex_match(p.path().filename().string(), std::regex(str)))
    {
      std::filesystem::remove(p.path());
    }
  }
}

void kill_all_levels()
{
  kill_levels("om.*.lev");
}

void msdos_init()
{
  // Allocate the inner level of pointers for TheLevel
  for(int x = 0; x < MAXWIDTH; ++x)
    TheLevel.site[x] = new location[MAXLENGTH];

  // Remove old level files
  kill_all_levels();
}

static FILE *open_levfile(int env, int depth, int rw)
{
  return fopen(std::format("{}om{:03}{:03}.lev", Omegalib, env, depth).c_str(), (rw) ? "wb" : "rb");
}

// Saves oldlevel (unless nullptr), and reads in the new level, unless depth is < 0
level *msdos_changelevel(level *oldlevel, int newenv, int newdepth)
{
  FILE *fp;

  if(oldlevel)
  {
    if(oldlevel->environment == newenv && oldlevel->depth == newdepth)
      return oldlevel;
    if((fp = open_levfile(oldlevel->environment, oldlevel->depth, 1)))
    {
      save_level(fp, oldlevel);
      fclose(fp);
    }
    else
      queue_message("Cannot save level!!!");
    // Free up monsters and items
    free_level(oldlevel);
  }
  if(newdepth >= 0)
  {
    if(!(fp = open_levfile(newenv, newdepth, 0)))
      return nullptr;
    restore_level(fp, VERSION);
    fclose(fp);
    return Level;
  }
  return nullptr;
}

#endif

// loads the abyss level into Level
void load_abyss()
{
  TempLevel = Level;
  if(ok_to_free(TempLevel))
  {
#ifndef SAVE_LEVELS
    free_level(TempLevel);
#endif
    TempLevel = nullptr;
  }
#ifndef SAVE_LEVELS
  Level = new level;
#else
  msdos_changelevel(TempLevel, 0, -1);
  Level = &TheLevel;
#endif

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
