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

// country.cpp
// load_country(), and all load_ functions for places which are
// accessible from the country and don't have their own files

#include "glob.h"
#include "scr.h"

#include <format>

// loads the countryside level from the data file
void load_country()
{
  FILE *fd  = checkfopen(std::format("{}country.dat", Omegalib), "rb");
  char site = cryptkey("country.dat");

  for(int y = 0; y < LENGTH; ++y)
  {
    for(int x = 0; x < WIDTH; ++x)
    {
      site                 = getc(fd) ^ site;
      Country[x][y].aux    = 0;
      Country[x][y].status = 0;
      switch(site)
      {
        case(PASS & 0xff):
          Country[x][y].base_terrain_type    = PASS;
          Country[x][y].current_terrain_type = MOUNTAINS;
          break;
        case(CASTLE & 0xff):
          Country[x][y].base_terrain_type    = CASTLE;
          Country[x][y].current_terrain_type = MOUNTAINS;
          break;
        case(STARPEAK & 0xff):
          Country[x][y].base_terrain_type    = STARPEAK;
          Country[x][y].current_terrain_type = MOUNTAINS;
          break;
        case(CAVES & 0xff):
          Country[x][y].base_terrain_type    = CAVES;
          Country[x][y].current_terrain_type = MOUNTAINS;
          break;
        case(VOLCANO & 0xff):
          Country[x][y].base_terrain_type    = VOLCANO;
          Country[x][y].current_terrain_type = MOUNTAINS;
          break;
        case(DRAGONLAIR & 0xff):
          Country[x][y].base_terrain_type    = DRAGONLAIR;
          Country[x][y].current_terrain_type = DESERT;
          break;
        case(MAGIC_ISLE & 0xff):
          Country[x][y].base_terrain_type    = MAGIC_ISLE;
          Country[x][y].current_terrain_type = CHAOS_SEA;
          break;
        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
          Country[x][y].current_terrain_type = VILLAGE;
          Country[x][y].base_terrain_type    = VILLAGE;
          Country[x][y].aux                  = 1 + site - 'a';
          break;
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
          Country[x][y].current_terrain_type = TEMPLE;
          Country[x][y].base_terrain_type    = TEMPLE;
          Country[x][y].aux                  = site - '0';
          break;
        case(PLAINS & 0xff):
          Country[x][y].current_terrain_type = PLAINS;
          Country[x][y].base_terrain_type    = PLAINS;
          break;
        case(TUNDRA & 0xff):
          Country[x][y].current_terrain_type = TUNDRA;
          Country[x][y].base_terrain_type    = TUNDRA;
          break;
        case(ROAD & 0xff):
          Country[x][y].current_terrain_type = ROAD;
          Country[x][y].base_terrain_type    = ROAD;
          break;
        case(MOUNTAINS & 0xff):
          Country[x][y].current_terrain_type = MOUNTAINS;
          Country[x][y].base_terrain_type    = MOUNTAINS;
          break;
        case(RIVER & 0xff):
          Country[x][y].current_terrain_type = RIVER;
          Country[x][y].base_terrain_type    = RIVER;
          break;
        case(CITY & 0xff):
          Country[x][y].current_terrain_type = CITY;
          Country[x][y].base_terrain_type    = CITY;
          break;
        case(FOREST & 0xff):
          Country[x][y].current_terrain_type = FOREST;
          Country[x][y].base_terrain_type    = FOREST;
          break;
        case(JUNGLE & 0xff):
          Country[x][y].current_terrain_type = JUNGLE;
          Country[x][y].base_terrain_type    = JUNGLE;
          break;
        case(SWAMP & 0xff):
          Country[x][y].current_terrain_type = SWAMP;
          Country[x][y].base_terrain_type    = SWAMP;
          break;
        case(DESERT & 0xff):
          Country[x][y].current_terrain_type = DESERT;
          Country[x][y].base_terrain_type    = DESERT;
          break;
        case(CHAOS_SEA & 0xff):
          Country[x][y].current_terrain_type = CHAOS_SEA;
          Country[x][y].base_terrain_type    = CHAOS_SEA;
          break;
      }
    }
    site = getc(fd) ^ site;
  }
  fclose(fd);
}

// loads the dragon's lair into Level
void load_dlair(int empty, int populate, std::unique_ptr<level> lvl)
{
  if(empty)
  {
    queue_message("The Lair is now devoid of inhabitants and treasure.");
  }

  if(!populate)
  {
    empty = true;
  }
  if(lvl)
  {
    TempLevel = std::move(lvl);
  }
  else
  {
    TempLevel = std::make_unique<level>();
  }
  Level = TempLevel.get();
  clear_level(Level);
  Level->environment = E_DLAIR;
  FILE *fd           = checkfopen(std::format("{}dlair.dat", Omegalib), "rb");
  char site          = cryptkey("dlair.dat");
  for(int y = 0; y < LENGTH; ++y)
  {
    for(int x = 0; x < WIDTH; ++x)
    {
      Level->site[x][y].lstatus = 0;
      if(x < 48)
      {
        Level->site[x][y].roomnumber = RS_CAVERN;
      }
      else
      {
        Level->site[x][y].roomnumber = RS_DRAGONLORD;
      }
      Level->site[x][y].p_locf = L_NO_OP;
      site                     = getc(fd) ^ site;
      switch(site)
      {
        case 'D':
          Level->site[x][y].locchar = FLOOR;
          if(!empty)
          {
            make_site_monster(x, y, DRAGON_LORD);
            Level->site[x][y].creature->specialf = M_SP_LAIR;
          }
          break;
        case 'd':
          Level->site[x][y].locchar = FLOOR;
          if(!empty)
          {
            make_site_monster(x, y, DRAGON); // elite dragons, actually
            Level->site[x][y].creature->specialf = M_SP_LAIR;
            Level->site[x][y].creature->hit *= 2;
            Level->site[x][y].creature->dmg *= 2;
          }
          break;
        case 'W':
          Level->site[x][y].locchar = FLOOR;
          if(!empty)
          {
            make_site_monster(x, y, KING_WYV);
          }
          break;
        case 'M':
          Level->site[x][y].locchar = FLOOR;
          if(!empty)
          {
            make_site_monster(x, y, RANDOM);
          }
          break;
        case 'S':
          Level->site[x][y].locchar  = FLOOR;
          Level->site[x][y].showchar = WALL;
          if(!empty)
          {
            lset(x, y, SECRET, *Level);
          }
          Level->site[x][y].roomnumber = RS_SECRETPASSAGE;
          break;
        case '$':
          Level->site[x][y].locchar = FLOOR;
          if(!empty)
          {
            make_site_treasure(x, y, 10);
          }
          break;
        case 's':
          Level->site[x][y].locchar = FLOOR;
          Level->site[x][y].p_locf  = L_TRAP_SIREN;
          break;
        case '7':
          if(!empty)
          {
            Level->site[x][y].locchar = PORTCULLIS;
          }
          else
          {
            Level->site[x][y].locchar = FLOOR;
          }
          Level->site[x][y].p_locf = L_PORTCULLIS;
          break;
        case 'R':
          Level->site[x][y].locchar = FLOOR;
          Level->site[x][y].p_locf  = L_RAISE_PORTCULLIS;
          break;
        case 'p':
          Level->site[x][y].locchar = FLOOR;
          Level->site[x][y].p_locf  = L_PORTCULLIS;
          break;
        case 'T':
          Level->site[x][y].locchar = FLOOR;
          if(!empty)
          {
            Level->site[x][y].p_locf = L_PORTCULLIS_TRAP;
          }
          break;
        case 'X':
          Level->site[x][y].locchar = FLOOR;
          Level->site[x][y].p_locf  = L_TACTICAL_EXIT;
          break;
        case '#':
          Level->site[x][y].locchar = WALL;
          Level->site[x][y].aux     = 150;
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

// loads the star peak into Level
void load_speak(int empty, int populate, std::unique_ptr<level> lvl)
{
  int safe = Player.alignment > 0;

  if(empty)
  {
    queue_message("The peak is now devoid of inhabitants and treasure.");
  }

  if(!populate)
  {
    empty = true;
  }

  if(lvl)
  {
    TempLevel = std::move(lvl);
  }
  else
  {
    TempLevel = std::make_unique<level>();
  }
  Level = TempLevel.get();
  clear_level(Level);
  Level->environment = E_STARPEAK;
  FILE *fd           = checkfopen(std::format("{}speak.dat", Omegalib), "rb");
  char site          = cryptkey("speak.dat");
  for(int y = 0; y < LENGTH; ++y)
  {
    for(int x = 0; x < WIDTH; ++x)
    {
      Level->site[x][y].lstatus    = 0;
      Level->site[x][y].roomnumber = RS_STARPEAK;
      Level->site[x][y].p_locf     = L_NO_OP;
      site                         = getc(fd) ^ site;
      switch(site)
      {
        case 'S':
          Level->site[x][y].locchar  = FLOOR;
          Level->site[x][y].showchar = WALL;
          lset(x, y, SECRET, *Level);
          Level->site[x][y].roomnumber = RS_SECRETPASSAGE;
          break;
        case 'L':
          Level->site[x][y].locchar = FLOOR;
          if(!empty)
          {
            make_site_monster(x, y, LAWBRINGER);
            if(safe)
            {
              m_status_reset(*Level->site[x][y].creature, HOSTILE);
            }
          }
          break;
        case 's':
          Level->site[x][y].locchar = FLOOR;
          if(!empty)
          {
            make_site_monster(x, y, SERV_LAW); // servant of law
            if(safe)
            {
              m_status_reset(*Level->site[x][y].creature, HOSTILE);
            }
          }
          break;
        case 'M':
          Level->site[x][y].locchar = FLOOR;
          if(!empty)
          {
            make_site_monster(x, y, -1);
            if(safe)
            {
              m_status_reset(*Level->site[x][y].creature, HOSTILE);
            }
          }
          break;
        case '$':
          Level->site[x][y].locchar = FLOOR;
          if(!empty)
          {
            make_site_treasure(x, y, 10);
          }
          break;
        case '7':
          if(!empty)
          {
            Level->site[x][y].locchar = PORTCULLIS;
          }
          else
          {
            Level->site[x][y].locchar = FLOOR;
          }
          Level->site[x][y].p_locf = L_PORTCULLIS;
          break;
        case 'R':
          Level->site[x][y].locchar = FLOOR;
          Level->site[x][y].p_locf  = L_RAISE_PORTCULLIS;
          break;
        case '-':
          Level->site[x][y].locchar = CLOSED_DOOR;
          break;
        case '|':
          Level->site[x][y].locchar = OPEN_DOOR;
          break;
        case 'p':
          Level->site[x][y].locchar = FLOOR;
          Level->site[x][y].p_locf  = L_PORTCULLIS;
          break;
        case 'T':
          Level->site[x][y].locchar = FLOOR;
          if(!empty)
          {
            Level->site[x][y].p_locf = L_PORTCULLIS_TRAP;
          }
          break;
        case 'X':
          Level->site[x][y].locchar = FLOOR;
          Level->site[x][y].p_locf  = L_TACTICAL_EXIT;
          break;
        case '#':
          Level->site[x][y].locchar = WALL;
          Level->site[x][y].aux     = 150;
          break;
        case '4':
          Level->site[x][y].locchar = RUBBLE;
          Level->site[x][y].p_locf  = L_RUBBLE;
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

// loads the magic isle into Level
void load_misle(int empty, int populate, std::unique_ptr<level> lvl)
{
  if(empty)
  {
    queue_message("The isle is now devoid of inhabitants and treasure.");
  }

  if(!populate)
  {
    empty = true;
  }

  if(lvl)
  {
    TempLevel = std::move(lvl);
  }
  else
  {
    TempLevel = std::make_unique<level>();
  }
  Level = TempLevel.get();
  clear_level(Level);
  Level->environment = E_MAGIC_ISLE;
  FILE *fd           = checkfopen(std::format("{}misle.dat", Omegalib), "rb");
  char site          = cryptkey("misle.dat");
  for(int y = 0; y < LENGTH; ++y)
  {
    for(int x = 0; x < WIDTH; ++x)
    {
      Level->site[x][y].lstatus    = 0;
      Level->site[x][y].roomnumber = RS_MAGIC_ISLE;
      Level->site[x][y].p_locf     = L_NO_OP;
      site                         = getc(fd) ^ site;
      switch(site)
      {
        case 'E':
          Level->site[x][y].locchar = FLOOR;
          if(!empty)
          {
            make_site_monster(x, y, EATER); // eater of magic
          }
          break;
        case 'm':
          Level->site[x][y].locchar = FLOOR;
          if(!empty)
          {
            make_site_monster(x, y, MIL_PRIEST); // militant priest
          }
          break;
        case 'n':
          Level->site[x][y].locchar = FLOOR;
          if(!empty)
          {
            make_site_monster(x, y, NAZGUL);
          }
          break;
        case 'X':
          Level->site[x][y].locchar = FLOOR;
          Level->site[x][y].p_locf  = L_TACTICAL_EXIT;
          break;
        case '#':
          Level->site[x][y].locchar = WALL;
          Level->site[x][y].aux     = 150;
          break;
        case '4':
          Level->site[x][y].locchar = RUBBLE;
          Level->site[x][y].p_locf  = L_RUBBLE;
          break;
        case '~':
          Level->site[x][y].locchar = WATER;
          Level->site[x][y].p_locf  = L_CHAOS;
          break;
        case '=':
          Level->site[x][y].locchar = WATER;
          Level->site[x][y].p_locf  = L_MAGIC_POOL;
          break;
        case '-':
          Level->site[x][y].locchar = CLOSED_DOOR;
          break;
        case '|':
          Level->site[x][y].locchar = OPEN_DOOR;
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

void make_high_priest(int x, int y, int deity)
{
  auto m = std::make_unique<monster>();
  make_hiscore_npc(m.get(), deity);
  m->x                       = x;
  m->y                       = y;
  Level->site[x][y].creature = m.get();
  Level->mlist.emplace_front(std::move(m));
}

void random_temple_site(int x, int y, int, int populate)
{
  switch(random_range(12))
  {
    case 0:
      if(populate)
      {
        make_site_monster(x, y, MEND_PRIEST);
      }
      break; // mendicant priest
    case 1:
      Level->site[x][y].locchar = WATER;
      Level->site[x][y].p_locf  = L_MAGIC_POOL;
      [[fallthrough]];
    case 2:
      if(populate)
      {
        make_site_monster(x, y, INNER_DEMON);
      }
      break; // inner circle demon
    case 3:
      if(populate)
      {
        make_site_monster(x, y, ANGEL);
      }
      break; // angel of apropriate sect
    case 4:
      if(populate)
      {
        make_site_monster(x, y, HIGH_ANGEL);
      }
      break; // high angel of apropriate sect
    case 5:
      if(populate)
      {
        make_site_monster(x, y, ARCHANGEL);
      }
      break; // archangel of apropriate sect
  }
}

// loads a temple into Level
void load_temple(int deity, int populate, std::unique_ptr<level> lvl)
{
  // WDT HACK: I don't know why this is wrong.  Shrug.  David Givens
  // suggested removing it, and he has more experience with Omega
  // than I, so...
  //  initrand(Current_Environment, deity); // FIXED! 12/30/98
  if(lvl)
  {
    TempLevel = std::move(lvl);
  }
  else
  {
    TempLevel = std::make_unique<level>();
  }
  Level = TempLevel.get();
  clear_level(Level);
  Level->environment = E_TEMPLE;
  FILE *fd           = checkfopen(std::format("{}temple.dat", Omegalib), "rb");
  char site          = cryptkey("temple.dat");
  for(int y = 0; y < LENGTH; ++y)
  {
    for(int x = 0; x < WIDTH; ++x)
    {
      switch(deity)
      {
        case ODIN:
          Level->site[x][y].roomnumber = RS_ODIN;
          break;
        case SET:
          Level->site[x][y].roomnumber = RS_SET;
          break;
        case HECATE:
          Level->site[x][y].roomnumber = RS_HECATE;
          break;
        case ATHENA:
          Level->site[x][y].roomnumber = RS_ATHENA;
          break;
        case DRUID:
          Level->site[x][y].roomnumber = RS_DRUID;
          break;
        case DESTINY:
          Level->site[x][y].roomnumber = RS_DESTINY;
          break;
      }
      site = getc(fd) ^ site;
      switch(site)
      {
        case '8':
          Level->site[x][y].locchar = ALTAR;
          Level->site[x][y].p_locf  = L_ALTAR;
          Level->site[x][y].aux     = deity;
          break;
        case 'H':
          Level->site[x][y].locchar = FLOOR;
          if(populate && (!Player.patron || Player.name != Priest[Player.patron] || Player.rank[PRIESTHOOD] != HIGHPRIEST))
          {
            make_high_priest(x, y, deity);
          }
          break;
        case 'S':
          Level->site[x][y].locchar = FLOOR;
          if(!Player.patron || Player.name != Priest[Player.patron] || Player.rank[PRIESTHOOD] != HIGHPRIEST)
          {
            lset(x, y, SECRET, *Level);
          }
          break;
        case 'W':
          Level->site[x][y].locchar = FLOOR;
          if(deity != Player.patron && deity != DRUID)
          {
            Level->site[x][y].p_locf = L_TEMPLE_WARNING;
          }
          break;
        case 'm':
          Level->site[x][y].locchar = FLOOR;
          if(populate)
          {
            make_site_monster(x, y, MIL_PRIEST); // militant priest
          }
          break;
        case 'd':
          Level->site[x][y].locchar = FLOOR;
          if(populate)
          {
            make_site_monster(x, y, DOBERMAN); // doberman death hound
          }
          break;
        case 'X':
          Level->site[x][y].locchar = FLOOR;
          Level->site[x][y].p_locf  = L_TACTICAL_EXIT;
          break;
        case '#':
          if(deity != DRUID)
          {
            Level->site[x][y].locchar = WALL;
            Level->site[x][y].aux     = 150;
          }
          else
          {
            Level->site[x][y].locchar = HEDGE;
            Level->site[x][y].p_locf  = L_HEDGE;
          }
          break;
        case '.':
          Level->site[x][y].locchar = FLOOR;
          break;
        case 'x':
          Level->site[x][y].locchar = FLOOR;
          random_temple_site(x, y, deity, populate);
          break;
        case '?':
          if(deity != DESTINY)
          {
            Level->site[x][y].locchar = FLOOR;
          }
          else
          {
            Level->site[x][y].locchar = ABYSS;
            Level->site[x][y].p_locf  = L_ADEPT;
          }
          break;
        case '-':
          Level->site[x][y].locchar = CLOSED_DOOR;
          break;
        case '|':
          Level->site[x][y].locchar = OPEN_DOOR;
          break;
      }
    }
    site = getc(fd) ^ site;
  }
  // Main Temple is peaceful for player of same sect,druids always peaceful.
  if((Player.patron == deity) || (deity == DRUID))
  {
    for(std::unique_ptr<monster> &m : Level->mlist)
    {
      m_status_reset(*m, HOSTILE);
    }
  }
  fclose(fd);
}
