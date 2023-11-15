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

/* abyss.c */
/* some functions to make the abyss level and run the final challenge */

#include "glob.h"

#include <filesystem>
#include <format>
#include <regex>

#ifdef SAVE_LEVELS
/* This stuff is in this file because the file was really small. */
extern level TheLevel;

void kill_levels(const std::string &str)
{
  /* Remove old level files laying around */
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
  /* Allocate the inner level of pointers for TheLevel */
  for(int i = 0; i < MAXWIDTH; i++)
    TheLevel.site[i] = new location[MAXLENGTH];

  /* Remove old level files */
  kill_all_levels();
}

static FILE *open_levfile(int env, int depth, int rw)
{
  return (fopen(std::format("{}om{:03}{:03}.lev", Omegalib, env, depth).c_str(), (rw) ? "wb" : "rb"));
}

/* Saves oldlevel (unless nullptr), and reads in the new level,
   unless depth is < 0. */
plv msdos_changelevel(plv oldlevel, int newenv, int newdepth)
{
  FILE *fp;

  if(oldlevel)
  {
    if(oldlevel->environment == newenv && oldlevel->depth == newdepth)
      return (oldlevel);
    if((fp = open_levfile(oldlevel->environment, oldlevel->depth, 1)))
    {
      save_level(fp, oldlevel);
      fclose(fp);
    }
    else
      queue_message("Cannot save level!!!");
    /* Free up monsters and items */
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

/* loads the abyss level into Level*/
void load_abyss()
{
  int i, j;
  char site;

  FILE *fd;

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
  fd   = checkfopen(filepath, "rb");
  site = cryptkey("abyss.dat");
  for(j = 0; j < LENGTH; j++)
  {
    for(i = 0; i < WIDTH; i++)
    {
      site                         = getc(fd) ^ site;
      Level->site[i][j].roomnumber = RS_ADEPT;
      switch(site)
      {
        case '0':
          Level->site[i][j].locchar = VOID_CHAR;
          Level->site[i][j].p_locf  = L_VOID;
          break;
        case 'V':
          Level->site[i][j].locchar = VOID_CHAR;
          Level->site[i][j].p_locf  = L_VOID_STATION;
          break;
        case '1':
          Level->site[i][j].locchar = FLOOR;
          Level->site[i][j].p_locf  = L_VOICE1;
          break;
        case '2':
          Level->site[i][j].locchar = FLOOR;
          Level->site[i][j].p_locf  = L_VOICE2;
          break;
        case '3':
          Level->site[i][j].locchar = FLOOR;
          Level->site[i][j].p_locf  = L_VOICE3;
          break;
        case '~':
          Level->site[i][j].locchar = WATER;
          Level->site[i][j].p_locf  = L_WATER_STATION;
          break;
        case ';':
          Level->site[i][j].locchar = FIRE;
          Level->site[i][j].p_locf  = L_FIRE_STATION;
          break;
        case '"':
          Level->site[i][j].locchar = HEDGE;
          Level->site[i][j].p_locf  = L_EARTH_STATION;
          break;
        case '6':
          Level->site[i][j].locchar = WHIRLWIND;
          Level->site[i][j].p_locf  = L_AIR_STATION;
          break;
        case '#':
          Level->site[i][j].locchar = WALL;
          break;
        case '.':
          Level->site[i][j].locchar = FLOOR;
          break;
      }
    }
    site = getc(fd) ^ site;
  }
  fclose(fd);
}
