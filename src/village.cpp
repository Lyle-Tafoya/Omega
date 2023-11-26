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

// village.cpp
// some functions to make the village levels

#include "glob.h"

#include <format>

#ifdef SAVE_LEVELS
extern level TheLevel;
level *msdos_changelevel(level *oldlevel, int newenv, int newdepth);
#endif

void assign_village_function(int x, int y, int setup)
{
  static int next = 0;
  static int permutation[24]; // number of x's in village map

  if(setup)
  {
    next = 0;
    for(int i = 0; i < 24; ++i)
    {
      permutation[i] = i;
    }
    for(int i = 0; i < 24; ++i)
    {
      int j          = permutation[i];
      int k          = random_range(24);
      permutation[i] = permutation[k];
      permutation[k] = j;
    }
  }
  else
  {
    lset(x, y + 1, STOPS, *Level);
    lset(x + 1, y, STOPS, *Level);
    lset(x - 1, y, STOPS, *Level);
    lset(x, y - 1, STOPS, *Level);
    lset(x, y, STOPS, *Level);

    switch(permutation[next++])
    {
      case 0:
        Level->site[x][y].locchar = OPEN_DOOR;
        Level->site[x][y].p_locf  = L_ARMORER;
        break;
      case 1:
        Level->site[x][y].locchar = OPEN_DOOR;
        Level->site[x][y].p_locf  = L_HEALER;
        break;
      case 2:
        Level->site[x][y].locchar = OPEN_DOOR;
        Level->site[x][y].p_locf  = L_TAVERN;
        break;
      case 3:
        Level->site[x][y].locchar = OPEN_DOOR;
        Level->site[x][y].p_locf  = L_COMMANDANT;
        break;
      case 4:
        Level->site[x][y].locchar = OPEN_DOOR;
        Level->site[x][y].p_locf  = L_CARTOGRAPHER;
        break;
      default:
        Level->site[x][y].locchar = CLOSED_DOOR;
        if(random_range(2))
        {
          Level->site[x][y].aux = LOCKED;
        }
        if(random_range(2))
        {
          Level->site[x][y].p_locf = L_HOVEL;
        }
        else
        {
          Level->site[x][y].p_locf = L_HOUSE;
        }
        break;
    }
  }
}

void make_food_bin(int x, int y)
{
  for(int k = 0; k < 10; ++k)
  {
    auto o = std::make_unique<object>();
    make_food(o.get(), 15); // grain
    Level->site[x][y].things.emplace_back(std::move(o));
  }
}

void make_guard(int x, int y)
{
  std::unique_ptr<monster> m = make_creature(GUARD);
  Level->site[x][y].creature = m.get();
  m->x                       = x;
  m->y                       = y;
  Level->mlist.push_front(std::move(m));
}

void make_sheep(int x, int y)
{
  std::unique_ptr<monster> m = make_creature(SHEEP);
  Level->site[x][y].creature = m.get();
  m->x                       = x;
  m->y                       = y;
  Level->mlist.push_front(std::move(m));
}

void make_horse(int x, int y)
{
  std::unique_ptr<monster> m = make_creature(HORSE);
  Level->site[x][y].creature = m.get();
  m->x                       = x;
  m->y                       = y;
  Level->mlist.push_front(std::move(m));
}

void make_merchant(int x, int y)
{
  std::unique_ptr<monster> m = make_creature(MERCHANT);
  Level->site[x][y].creature = m.get();
  m->x                       = x;
  m->y                       = y;
  Level->mlist.push_front(std::move(m));
}

void special_village_site(int x, int y, int villagenum)
{
  if(villagenum == 1)
  {
    Level->site[x][y].locchar = ALTAR;
    Level->site[x][y].p_locf  = L_LAWSTONE;
  }
  if(villagenum == 2)
  {
    Level->site[x][y].locchar = ALTAR;
    Level->site[x][y].p_locf  = L_BALANCESTONE;
  }
  else if(villagenum == 3)
  {
    Level->site[x][y].locchar = ALTAR;
    Level->site[x][y].p_locf  = L_CHAOSTONE;
  }
  else if(villagenum == 4)
  {
    Level->site[x][y].locchar = ALTAR;
    Level->site[x][y].p_locf  = L_MINDSTONE;
  }
  else if(villagenum == 5)
  {
    Level->site[x][y].locchar = ALTAR;
    Level->site[x][y].p_locf  = L_SACRIFICESTONE;
  }
  else if(villagenum == 6)
  {
    Level->site[x][y].locchar = ALTAR;
    Level->site[x][y].p_locf  = L_VOIDSTONE;
  }
}

// loads the village level into Level
void load_village(int villagenum, int populate)
{
  TempLevel = Level;
  if(ok_to_free(TempLevel))
  {
#ifndef SAVE_LEVELS
    free_level(TempLevel);
#endif
    TempLevel = nullptr;
  }

  initrand(Current_Environment, villagenum);

  assign_village_function(0, 0, true);

#ifdef SAVE_LEVELS
  msdos_changelevel(TempLevel, 0, -1);
  Level = &TheLevel;
#else
  Level = new level;
#endif
  clear_level(Level);
  Level->environment = E_VILLAGE;
  char site          = cryptkey("village.dat");
  FILE *fd           = checkfopen(std::format("{}village{}.dat", Omegalib, villagenum), "rb");
  for(int y = 0; y < LENGTH; ++y)
  {
    for(int x = 0; x < WIDTH; ++x)
    {
      lset(x, y, SEEN, *Level);
      site                     = getc(fd) ^ site;
      Level->site[x][y].p_locf = L_NO_OP;
      switch(site)
      {
        case 'f':
          Level->site[x][y].locchar = FLOOR;
          if(populate)
          {
            make_food_bin(x, y);
          }
          break;
        case 'g':
          Level->site[x][y].locchar = FLOOR;
          Level->site[x][y].p_locf  = L_GRANARY;
          break;
        case 'h':
          Level->site[x][y].locchar = FLOOR;
          if(populate)
          {
            make_horse(x, y);
          }
          break;
        case 'S':
          Level->site[x][y].locchar = FLOOR;
          Level->site[x][y].p_locf  = L_STABLES;
          break;
        case 'H':
          Level->site[x][y].locchar = FLOOR;
          if(populate)
          {
            make_merchant(x, y);
          }
          break;
        case 'C':
          Level->site[x][y].locchar = FLOOR;
          Level->site[x][y].p_locf  = L_COMMONS;
          break;
        case 's':
          Level->site[x][y].locchar = FLOOR;
          if(populate)
          {
            make_sheep(x, y);
          }
          break;
        case 'x':
          assign_village_function(x, y, false);
          break;
        case 'X':
          Level->site[x][y].locchar = FLOOR;
          Level->site[x][y].p_locf  = L_COUNTRYSIDE;
          break;
        case 'G':
          Level->site[x][y].locchar = FLOOR;
          if(populate)
          {
            make_guard(x, y);
            Level->site[x][y].creature->aux1 = x;
            Level->site[x][y].creature->aux2 = y;
          }
          break;
        case '^':
          Level->site[x][y].locchar = FLOOR;
          Level->site[x][y].p_locf  = L_TRAP_SIREN;
          break;
        case '"':
          Level->site[x][y].locchar = HEDGE;
          Level->site[x][y].p_locf  = L_HEDGE;
          break;
        case '~':
          Level->site[x][y].locchar = WATER;
          Level->site[x][y].p_locf  = L_WATER;
          break;
        case '+':
          Level->site[x][y].locchar = WATER;
          Level->site[x][y].p_locf  = L_CHAOS;
          break;
        case '\'':
          Level->site[x][y].locchar = HEDGE;
          Level->site[x][y].p_locf  = L_TRIFFID;
          break;
        case '!':
          special_village_site(x, y, villagenum);
          break;
        case '#':
          Level->site[x][y].locchar = WALL;
          Level->site[x][y].aux     = 100;
          break;
        case '.':
          Level->site[x][y].locchar = FLOOR;
          break;
        case '-':
          Level->site[x][y].locchar = CLOSED_DOOR;
          break;
        case '1':
          Level->site[x][y].locchar = STATUE;
          break;
      }
      if(loc_statusp(x, y, SECRET, *Level))
      {
        Level->site[x][y].showchar = WALL;
      }
      else
      {
        Level->site[x][y].showchar = Level->site[x][y].locchar;
      }
    }
    site = getc(fd) ^ site;
  }
  fclose(fd);
  initrand(E_RESTORE, 0);
}
