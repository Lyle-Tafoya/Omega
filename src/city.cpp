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

// city.cpp
// some functions to make the city level

#include "defs.h"
#include "glob.h"

#include <format>

#ifdef SAVE_LEVELS
extern level TheLevel;
level *msdos_changelevel(level *oldlevel, int newenv, int newdepth);
#endif

void assign_city_function(int x, int y)
{
  static int setup = 0;
  static int next  = 0;
  static int permutation[64]; // number of x's in city map
  int j, k, l;

  Level->site[x][y].aux = true;

  lset(x, y + 1, STOPS, *Level);
  lset(x + 1, y, STOPS, *Level);
  lset(x - 1, y, STOPS, *Level);
  lset(x, y - 1, STOPS, *Level);
  lset(x, y, STOPS, *Level);

  if(setup == 0)
  {
    setup = 1;
    for(int i = 0; i < 64; ++i)
    {
      permutation[i] = i;
    }
    for(int i = 0; i < 500; ++i)
    {
      j              = random_range(64);
      k              = random_range(64);
      l              = permutation[j];
      permutation[j] = permutation[k];
      permutation[k] = l;
    }
  }
  if(next > 63)
  { // in case someone changes the no. of x's
    Level->site[x][y].locchar = CLOSED_DOOR;
    Level->site[x][y].p_locf  = L_HOUSE;
    if(random_range(5))
    {
      Level->site[x][y].aux = LOCKED;
    }
  }
  else
  {
    switch(permutation[next])
    {
      case 0:
        Level->site[x][y].locchar                 = OPEN_DOOR;
        Level->site[x][y].p_locf                  = L_ARMORER;
        CitySiteList[L_ARMORER - CITYSITEBASE][1] = x;
        CitySiteList[L_ARMORER - CITYSITEBASE][2] = y;
        break;
      case 1:
        Level->site[x][y].locchar              = OPEN_DOOR;
        Level->site[x][y].p_locf               = L_CLUB;
        CitySiteList[L_CLUB - CITYSITEBASE][1] = x;
        CitySiteList[L_CLUB - CITYSITEBASE][2] = y;
        break;
      case 2:
        Level->site[x][y].locchar             = OPEN_DOOR;
        Level->site[x][y].p_locf              = L_GYM;
        CitySiteList[L_GYM - CITYSITEBASE][1] = x;
        CitySiteList[L_GYM - CITYSITEBASE][2] = y;
        break;
      case 3:
        Level->site[x][y].locchar                       = CLOSED_DOOR;
        Level->site[x][y].p_locf                        = L_THIEVES_GUILD;
        CitySiteList[L_THIEVES_GUILD - CITYSITEBASE][1] = x;
        CitySiteList[L_THIEVES_GUILD - CITYSITEBASE][2] = y;
        break;
      case 4:
        Level->site[x][y].locchar                = OPEN_DOOR;
        Level->site[x][y].p_locf                 = L_HEALER;
        CitySiteList[L_HEALER - CITYSITEBASE][1] = x;
        CitySiteList[L_HEALER - CITYSITEBASE][2] = y;
        break;
      case 5:
        Level->site[x][y].locchar                = OPEN_DOOR;
        Level->site[x][y].p_locf                 = L_CASINO;
        CitySiteList[L_CASINO - CITYSITEBASE][1] = x;
        CitySiteList[L_CASINO - CITYSITEBASE][2] = y;
        break;
      case 7:
        Level->site[x][y].locchar               = OPEN_DOOR;
        Level->site[x][y].p_locf                = L_DINER;
        CitySiteList[L_DINER - CITYSITEBASE][1] = x;
        CitySiteList[L_DINER - CITYSITEBASE][2] = y;
        break;
      case 8:
        Level->site[x][y].locchar              = OPEN_DOOR;
        Level->site[x][y].p_locf               = L_CRAP;
        CitySiteList[L_CRAP - CITYSITEBASE][1] = x;
        CitySiteList[L_CRAP - CITYSITEBASE][2] = y;
        break;
      case 6:
      case 9:
      case 20:
        Level->site[x][y].locchar                    = OPEN_DOOR;
        Level->site[x][y].p_locf                     = L_COMMANDANT;
        CitySiteList[L_COMMANDANT - CITYSITEBASE][1] = x;
        CitySiteList[L_COMMANDANT - CITYSITEBASE][2] = y;
        break;
      case 21:
        Level->site[x][y].locchar                = OPEN_DOOR;
        Level->site[x][y].p_locf                 = L_TAVERN;
        CitySiteList[L_TAVERN - CITYSITEBASE][1] = x;
        CitySiteList[L_TAVERN - CITYSITEBASE][2] = y;
        break;
      case 10:
        Level->site[x][y].locchar                   = OPEN_DOOR;
        Level->site[x][y].p_locf                    = L_ALCHEMIST;
        CitySiteList[L_ALCHEMIST - CITYSITEBASE][1] = x;
        CitySiteList[L_ALCHEMIST - CITYSITEBASE][2] = y;
        break;
      case 11:
        Level->site[x][y].locchar             = OPEN_DOOR;
        Level->site[x][y].p_locf              = L_DPW;
        CitySiteList[L_DPW - CITYSITEBASE][1] = x;
        CitySiteList[L_DPW - CITYSITEBASE][2] = y;
        break;
      case 12:
        Level->site[x][y].locchar                 = OPEN_DOOR;
        Level->site[x][y].p_locf                  = L_LIBRARY;
        CitySiteList[L_LIBRARY - CITYSITEBASE][1] = x;
        CitySiteList[L_LIBRARY - CITYSITEBASE][2] = y;
        break;
      case 13:
        Level->site[x][y].locchar                   = OPEN_DOOR;
        Level->site[x][y].p_locf                    = L_PAWN_SHOP;
        CitySiteList[L_PAWN_SHOP - CITYSITEBASE][1] = x;
        CitySiteList[L_PAWN_SHOP - CITYSITEBASE][2] = y;
        break;
      case 14:
        Level->site[x][y].locchar               = OPEN_DOOR;
        Level->site[x][y].p_locf                = L_CONDO;
        CitySiteList[L_CONDO - CITYSITEBASE][1] = x;
        CitySiteList[L_CONDO - CITYSITEBASE][2] = y;
        break;
      case 15:
        Level->site[x][y].locchar                 = CLOSED_DOOR;
        Level->site[x][y].p_locf                  = L_BROTHEL;
        CitySiteList[L_BROTHEL - CITYSITEBASE][1] = x;
        CitySiteList[L_BROTHEL - CITYSITEBASE][2] = y;
        break;
      default:
        Level->site[x][y].locchar = CLOSED_DOOR;
        switch(random_range(6))
        {
          case 0:
            Level->site[x][y].p_locf = L_HOVEL;
            break;
          case 1:
          case 2:
          case 3:
          case 4:
            Level->site[x][y].p_locf = L_HOUSE;
            break;
          case 5:
            Level->site[x][y].p_locf = L_MANSION;
            break;
        }
        if(random_range(5))
        {
          Level->site[x][y].aux = LOCKED;
        }
        break;
    }
  }
  next++;
}

// makes a hiscore npc for mansions
void make_justiciar(int x, int y)
{
  auto m = std::make_unique<monster>(Monsters[NPC]);
  make_hiscore_npc(m.get(), 15);
  m->x                       = x;
  m->y                       = y;
  m->click                   = (Tick + 1) % 60;
  Level->site[x][y].creature = m.get();
  m_status_reset(*m, AWAKE);
  Level->mlist.emplace_front(std::move(m));
}

// undead are not hostile unless disturbed....
void make_major_undead(int x, int y)
{
  int mid;
  if(random_range(2))
  {
    mid = LICHE; // lich
  }
  else
  {
    mid = VAMP_LORD; // vampire lord
  }
  make_site_monster(x, y, mid);
  m_status_reset(*Level->site[x][y].creature, AWAKE);
  m_status_reset(*Level->site[x][y].creature, HOSTILE);
}

// undead are not hostile unless disturbed....
void make_minor_undead(int x, int y)
{
  int mid;
  if(random_range(2))
  {
    mid = GHOST;
  }
  else
  {
    mid = HAUNT;
  }
  make_site_monster(x, y, mid);
  m_status_reset(*Level->site[x][y].creature, AWAKE);
  m_status_reset(*Level->site[x][y].creature, HOSTILE);
}

void randommazesite(int x, int y, int populate)
{
  switch(random_range(7))
  {
    case 0:
    case 1:
      Level->site[x][y].locchar = FLOOR;
      Level->site[x][y].p_locf  = TRAP_BASE + random_range(NUMTRAPS);
      break;
    case 2:
    case 3:
      Level->site[x][y].locchar = FLOOR;
      if(populate)
      {
        make_site_monster(x, y, -1);
      }
      break;
    case 4:
    case 5:
      Level->site[x][y].locchar = FLOOR;
      if(populate)
      {
        make_site_treasure(x, y, 5);
      }
      break;
    default:
      Level->site[x][y].locchar = FLOOR;
  }
}

void mazesite(int x, int y, int populate)
{
  static FILE *fd = nullptr;
  static int k    = 0;
  static char site;
  if(!fd)
  {
    fd   = checkfopen(std::format("{}maze{}.dat", Omegalib, MazeNum), "rb");
    site = cryptkey("mazes");
  }
  site = getc(fd) ^ site;
  k++;
  if(k == 286)
  {
    fclose(fd);
  }
  switch(site)
  {
    case '"':
      Level->site[x][y].locchar = HEDGE;
      if(random_range(10))
      {
        Level->site[x][y].p_locf = L_HEDGE;
      }
      else
      {
        Level->site[x][y].p_locf = L_TRIFFID;
      }
      break;
    case '-':
      Level->site[x][y].locchar = CLOSED_DOOR;
      break;
    case '.':
      Level->site[x][y].locchar = FLOOR;
      break;
    case '>':
      Level->site[x][y].locchar = STAIRS_DOWN;
      Level->site[x][y].p_locf  = L_SEWER;
      break;
    case 'z':
      Level->site[x][y].locchar = FLOOR;
      Level->site[x][y].p_locf  = L_MAZE;
      break;
    case 'O':
      Level->site[x][y].locchar                = OPEN_DOOR;
      Level->site[x][y].p_locf                 = L_ORACLE;
      CitySiteList[L_ORACLE - CITYSITEBASE][1] = x;
      CitySiteList[L_ORACLE - CITYSITEBASE][2] = y;
      break;
    case '?':
      randommazesite(x, y, populate);
      break;
  }
  lreset(x, y, SEEN, *Level);
}

// loads the city level
void load_city(int populate)
{
  initrand(E_CITY, 0);

  FILE *fd  = checkfopen(std::format("{}city.dat", Omegalib), "rb");
  char site = cryptkey("city.dat");

  TempLevel = Level;
  if(ok_to_free(TempLevel))
  {
#ifndef SAVE_LEVELS
    delete TempLevel;
#endif
    TempLevel = nullptr;
  }
#ifdef SAVE_LEVELS
  msdos_changelevel(TempLevel, 0, -1);
  Level = &TheLevel;
#else
  Level = new level;
#endif
  clear_level(Level);
  Level->depth       = 0;
  Level->environment = E_CITY;
  for(int y = 0; y < LENGTH; ++y)
  {
    for(int x = 0; x < WIDTH; ++x)
    {
      lset(x, y, SEEN, *Level);
      site = getc(fd) ^ site;
      switch(site)
      {
        case 'g':
          Level->site[x][y].locchar = FLOOR;
          Level->site[x][y].p_locf  = L_GARDEN;
          break;
        case 'y':
          Level->site[x][y].locchar = FLOOR;
          Level->site[x][y].p_locf  = L_CEMETARY;
          break;
        case 'x':
          assign_city_function(x, y);
          break;
        case 't':
          Level->site[x][y].locchar                = FLOOR;
          Level->site[x][y].p_locf                 = L_TEMPLE;
          CitySiteList[L_TEMPLE - CITYSITEBASE][0] = true;
          CitySiteList[L_TEMPLE - CITYSITEBASE][1] = x;
          CitySiteList[L_TEMPLE - CITYSITEBASE][2] = y;
          break;
        case 'T':
          Level->site[x][y].locchar = FLOOR;
          Level->site[x][y].p_locf  = L_PORTCULLIS_TRAP;
          Level->site[x][y].aux     = NOCITYMOVE;
          break;
        case 'R':
          Level->site[x][y].locchar = FLOOR;
          Level->site[x][y].p_locf  = L_RAISE_PORTCULLIS;
          Level->site[x][y].aux     = NOCITYMOVE;
          break;
        case '7':
          Level->site[x][y].locchar = FLOOR;
          Level->site[x][y].p_locf  = L_PORTCULLIS;
          Level->site[x][y].aux     = NOCITYMOVE;
          break;
        case 'C':
          Level->site[x][y].locchar                 = OPEN_DOOR;
          Level->site[x][y].p_locf                  = L_COLLEGE;
          CitySiteList[L_COLLEGE - CITYSITEBASE][0] = true;
          CitySiteList[L_COLLEGE - CITYSITEBASE][1] = x;
          CitySiteList[L_COLLEGE - CITYSITEBASE][2] = y;
          break;
        case 's':
          Level->site[x][y].locchar                   = OPEN_DOOR;
          Level->site[x][y].p_locf                    = L_SORCERORS;
          CitySiteList[L_SORCERORS - CITYSITEBASE][0] = true;
          CitySiteList[L_SORCERORS - CITYSITEBASE][1] = x;
          CitySiteList[L_SORCERORS - CITYSITEBASE][2] = y;
          break;
        case 'M':
          Level->site[x][y].locchar                    = OPEN_DOOR;
          Level->site[x][y].p_locf                     = L_MERC_GUILD;
          CitySiteList[L_MERC_GUILD - CITYSITEBASE][0] = true;
          CitySiteList[L_MERC_GUILD - CITYSITEBASE][1] = x;
          CitySiteList[L_MERC_GUILD - CITYSITEBASE][2] = y;
          break;
        case 'K':
          Level->site[x][y].locchar                   = OPEN_DOOR;
          Level->site[x][y].p_locf                    = L_MONASTERY;
          CitySiteList[L_MONASTERY - CITYSITEBASE][0] = true;
          CitySiteList[L_MONASTERY - CITYSITEBASE][1] = x;
          CitySiteList[L_MONASTERY - CITYSITEBASE][2] = y;
          break;
        case 'c':
          Level->site[x][y].locchar                = OPEN_DOOR;
          Level->site[x][y].p_locf                 = L_CASTLE;
          CitySiteList[L_CASTLE - CITYSITEBASE][0] = true;
          CitySiteList[L_CASTLE - CITYSITEBASE][1] = x;
          CitySiteList[L_CASTLE - CITYSITEBASE][2] = y;
          break;
        case '?':
          mazesite(x, y, populate);
          break;
        case 'P':
          Level->site[x][y].locchar               = OPEN_DOOR;
          Level->site[x][y].p_locf                = L_ORDER;
          CitySiteList[L_ORDER - CITYSITEBASE][0] = true;
          CitySiteList[L_ORDER - CITYSITEBASE][1] = x;
          CitySiteList[L_ORDER - CITYSITEBASE][2] = y;
          break;
        case 'H':
          Level->site[x][y].locchar                 = OPEN_DOOR;
          Level->site[x][y].p_locf                  = L_CHARITY;
          CitySiteList[L_CHARITY - CITYSITEBASE][0] = true;
          CitySiteList[L_CHARITY - CITYSITEBASE][1] = x;
          CitySiteList[L_CHARITY - CITYSITEBASE][2] = y;
          break;
        case 'j':
          Level->site[x][y].locchar = FLOOR;
          if(populate)
          {
            make_justiciar(x, y);
          }
          break;
        case 'J':
          Level->site[x][y].locchar = CLOSED_DOOR;
          Level->site[x][y].p_locf  = L_JAIL;
          break;
        case 'A':
          Level->site[x][y].locchar               = OPEN_DOOR;
          Level->site[x][y].p_locf                = L_ARENA;
          CitySiteList[L_ARENA - CITYSITEBASE][0] = true;
          CitySiteList[L_ARENA - CITYSITEBASE][1] = x;
          CitySiteList[L_ARENA - CITYSITEBASE][2] = y;
          break;
        case 'B':
          Level->site[x][y].locchar              = OPEN_DOOR;
          Level->site[x][y].p_locf               = L_BANK;
          CitySiteList[L_BANK - CITYSITEBASE][0] = true;
          CitySiteList[L_BANK - CITYSITEBASE][1] = x;
          CitySiteList[L_BANK - CITYSITEBASE][2] = y;
          lset(x, y + 1, STOPS, *Level);
          lset(x + 1, y, STOPS, *Level);
          lset(x - 1, y, STOPS, *Level);
          lset(x, y - 1, STOPS, *Level);
          break;
        case 'X':
          Level->site[x][y].locchar                     = FLOOR;
          Level->site[x][y].p_locf                      = L_COUNTRYSIDE;
          CitySiteList[L_COUNTRYSIDE - CITYSITEBASE][0] = true;
          CitySiteList[L_COUNTRYSIDE - CITYSITEBASE][1] = x;
          CitySiteList[L_COUNTRYSIDE - CITYSITEBASE][2] = y;
          break;
        case 'v':
          Level->site[x][y].locchar = FLOOR;
          Level->site[x][y].p_locf  = L_VAULT;
          Level->site[x][y].aux     = NOCITYMOVE;
          lset(x, y, SECRET, *Level);
          break;
        case 'S':
          Level->site[x][y].locchar = FLOOR;
          Level->site[x][y].aux     = NOCITYMOVE;
          lset(x, y, SECRET, *Level);
          break;
        case 'G':
          Level->site[x][y].locchar = FLOOR;
          if(populate)
          {
            make_site_monster(x, y, GUARD);
            Level->site[x][y].creature->aux1 = x;
            Level->site[x][y].creature->aux2 = y;
          }
          break;
        case 'u':
          Level->site[x][y].locchar = FLOOR;
          if(populate)
          {
            make_minor_undead(x, y);
          }
          break;
        case 'U':
          Level->site[x][y].locchar = FLOOR;
          if(populate)
          {
            make_major_undead(x, y);
          }
          break;
        case 'V':
          Level->site[x][y].showchar = WALL;
          Level->site[x][y].locchar  = FLOOR;
          Level->site[x][y].p_locf   = L_VAULT;
          if(populate)
          {
            make_site_treasure(x, y, 5);
          }
          Level->site[x][y].aux = NOCITYMOVE;
          lset(x, y, SECRET, *Level);
          break;
        case '%':
          Level->site[x][y].showchar = WALL;
          Level->site[x][y].locchar  = FLOOR;
          Level->site[x][y].p_locf   = L_TRAP_SIREN;
          if(populate)
          {
            make_site_treasure(x, y, 5);
          }
          Level->site[x][y].aux = NOCITYMOVE;
          lset(x, y, SECRET, *Level);
          break;
        case '$':
          Level->site[x][y].locchar = FLOOR;
          if(populate)
          {
            make_site_treasure(x, y, 5);
          }
          break;
        case '2':
          Level->site[x][y].locchar = ALTAR;
          Level->site[x][y].p_locf  = L_ALTAR;
          Level->site[x][y].aux     = ODIN;
          break;
        case '3':
          Level->site[x][y].locchar = ALTAR;
          Level->site[x][y].p_locf  = L_ALTAR;
          Level->site[x][y].aux     = SET;
          break;
        case '4':
          Level->site[x][y].locchar = ALTAR;
          Level->site[x][y].p_locf  = L_ALTAR;
          Level->site[x][y].aux     = ATHENA;
          break;
        case '5':
          Level->site[x][y].locchar = ALTAR;
          Level->site[x][y].p_locf  = L_ALTAR;
          Level->site[x][y].aux     = HECATE;
          break;
        case '6':
          Level->site[x][y].locchar = ALTAR;
          Level->site[x][y].p_locf  = L_ALTAR;
          Level->site[x][y].aux     = DESTINY;
          break;
        case '^':
          Level->site[x][y].showchar = WALL;
          Level->site[x][y].locchar  = FLOOR;
          Level->site[x][y].p_locf   = static_cast<char>(TRAP_BASE + random_range(NUMTRAPS));
          lset(x, y, SECRET, *Level);
          break;
        case '"':
          Level->site[x][y].locchar = HEDGE;
          break;
        case '~':
          Level->site[x][y].locchar = WATER;
          Level->site[x][y].p_locf  = L_WATER;
          break;
        case '=':
          Level->site[x][y].locchar = WATER;
          Level->site[x][y].p_locf  = L_MAGIC_POOL;
          break;
        case '*':
          Level->site[x][y].locchar = WALL;
          Level->site[x][y].aux     = 10;
          break;
        case '#':
          Level->site[x][y].locchar = WALL;
          Level->site[x][y].aux     = 500;
          break;
        case '.':
          Level->site[x][y].locchar = FLOOR;
          break;
        case ',':
          Level->site[x][y].showchar = WALL;
          Level->site[x][y].locchar  = FLOOR;
          Level->site[x][y].aux      = NOCITYMOVE;
          lset(x, y, SECRET, *Level);
          break;
        case '-':
          Level->site[x][y].locchar = CLOSED_DOOR;
          break;
        case '1':
          Level->site[x][y].locchar = STATUE;
          break;
        default:
          printf("\nOops... missed a case: '%c'   \n", site);
      }

      if(loc_statusp(x, y, SEEN, *Level))
      {
        if(loc_statusp(x, y, SECRET, *Level))
        {
          Level->site[x][y].showchar = WALL;
        }
        else
        {
          Level->site[x][y].showchar = Level->site[x][y].locchar;
        }
      }
    }
    site = getc(fd) ^ site;
  }
  City = Level;

  // make all city monsters asleep, and shorten their wakeup range to 2
  // to prevent players from being molested by vicious monsters on the streets
  for(std::unique_ptr<monster> &m : Level->mlist)
  {
    m_status_reset(*m, AWAKE);
    m->wakeup = 2;
  }
  fclose(fd);
  initrand(E_RESTORE, 0);
}

// loads the city level
void resurrect_guards()
{
  FILE *fd  = checkfopen(std::format("{}city.dat", Omegalib), "rb");
  char site = cryptkey("city.dat");
  for(int y = 0; y < LENGTH; ++y)
  {
    for(int x = 0; x < WIDTH; ++x)
    {
      site = getc(fd) ^ site;
      if(site == 'G')
      {
        make_site_monster(x, y, GUARD);
        Level->site[x][y].creature->monstring = "undead guardsman";
        Level->site[x][y].creature->meleef    = M_MELEE_SPIRIT;
        Level->site[x][y].creature->movef     = M_MOVE_SPIRIT;
        Level->site[x][y].creature->strikef   = M_STRIKE_MISSILE;
        Level->site[x][y].creature->immunity  = EVERYTHING - pow2(NORMAL_DAMAGE);
        Level->site[x][y].creature->hp *= 2;
        Level->site[x][y].creature->hit *= 2;
        Level->site[x][y].creature->dmg *= 2;
        Level->site[x][y].creature->ac *= 2;
        m_status_set(*Level->site[x][y].creature, HOSTILE);
        m_status_set(*Level->site[x][y].creature, AWAKE);
      }
    }
    site = getc(fd) ^ site;
  }
  fclose(fd);
}

static char jail[5][11] = {
  {'#', '#', '*', '#', '#', '*', '#', '#', '*', '#', '#'},
  {'#', '#', '*', '#', '#', '*', '#', '*', '#', '#', '#'},
  {'#', '#', 'T', '#', 'T', '#', 'T', '#', 'T', '#', '#'},
  {'#', '#', '7', '#', '7', '#', '7', '#', '7', '#', '#'},
  {'#', '#', 'R', '#', 'R', '#', 'R', '#', 'R', '#', '#'}
};

// fixes up the jail in case it has been munged by player action
void repair_jail()
{
  for(int x = 0; x < 11; ++x)
  {
    for(int y = 0; y < 5; ++y)
    {
      switch(jail[y][x])
      {
        case '#':
          City->site[x + 35][y + 52].locchar = WALL;
          City->site[x + 35][y + 52].p_locf  = L_NO_OP;
          City->site[x + 35][y + 52].aux     = NOCITYMOVE;
          break;
        case '*':
          City->site[x + 35][y + 52].locchar = WALL;
          City->site[x + 35][y + 52].p_locf  = L_NO_OP;
          City->site[x + 35][y + 52].aux     = 10;
          break;
        case 'T':
          City->site[x + 35][y + 52].locchar = FLOOR;
          City->site[x + 35][y + 52].p_locf  = L_PORTCULLIS_TRAP;
          City->site[x + 35][y + 52].aux     = NOCITYMOVE;
          break;
        case '7':
          City->site[x + 35][y + 52].locchar = FLOOR;
          City->site[x + 35][y + 52].p_locf  = L_PORTCULLIS;
          City->site[x + 35][y + 52].aux     = NOCITYMOVE;
          break;
        case 'R':
          City->site[x + 35][y + 52].locchar = FLOOR;
          City->site[x + 35][y + 52].p_locf  = L_RAISE_PORTCULLIS;
          City->site[x + 35][y + 52].aux     = NOCITYMOVE;
          break;
      }
      lreset(x + 35, y + 52, CHANGED, *Level);
    }
  }
}
