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

// env.cpp
// some load_* routines for special environments

#include "glob.h"
#include "scr.h"

#include <format>

#ifdef SAVE_LEVELS
extern level TheLevel;
level *msdos_changelevel(level *oldlevel, int newenv, int newdepth);
#endif

// loads the arena level into Level
void load_arena()
{
  char site;
  FILE *fd;

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
  Level->environment = E_ARENA;
  fd                 = checkfopen(std::format("{}arena.dat", Omegalib), "rb");
  site               = cryptkey("arena.dat");
  for(int y = 0; y < LENGTH; ++y)
  {
    for(int x = 0; x < WIDTH; ++x)
    {
      Level->site[x][y].lstatus    = SEEN + LIT;
      Level->site[x][y].roomnumber = RS_ARENA;
      site                         = getc(fd) ^ site;
      Level->site[x][y].p_locf     = L_NO_OP;
      switch(site)
      {
        case 'P':
          Level->site[x][y].locchar = PORTCULLIS;
          Level->site[x][y].p_locf  = L_PORTCULLIS;
          break;
        case 'X':
          Level->site[x][y].locchar = FLOOR;
          Level->site[x][y].p_locf  = L_ARENA_EXIT;
          break;
        case '#':
          Level->site[x][y].locchar = WALL;
          break;
        case '.':
          Level->site[x][y].locchar = FLOOR;
          break;
      }
      Level->site[x][y].showchar = Level->site[x][y].locchar;
    }
    site = getc(fd) ^ site;
  }
  fclose(fd);
  Level->site[60][7].creature = Arena_Monster;
  Arena_Monster->x            = 60;
  Arena_Monster->y            = 7;
  Arena_Monster->sense        = 50;
  m_pickup(Arena_Monster, std::make_unique<object>(Objects[THINGID + 0]));
  // hehehehe cackled the dungeon master....
  queue_message("Your opponent holds the only way you can leave!");
  Arena_Monster->hp += Arena_Monster->level * 10;
  Arena_Monster->hit += Arena_Monster->hit;
  Arena_Monster->dmg += Arena_Monster->dmg / 2;
}

// make the prime sorceror
void make_prime(int x, int y)
{
  auto m = std::make_unique<monster>();
  make_hiscore_npc(m.get(), 10); // 10 is index for prime
  m->x                       = x;
  m->y                       = y;
  Level->site[x][y].creature = m.get();

  if(Objects[ARTIFACTID + 21].uniqueness != UNIQUE_TAKEN)
  {
    m->possessions.emplace_front(std::make_unique<object>(Objects[ARTIFACTID + 21]));
  }
  Level->mlist.emplace_front(std::move(m));
}

// loads the sorcereror's circle into Level
void load_circle(int populate)
{
  bool safe = (Player.rank[CIRCLE] > 0);

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
  Level->environment = E_CIRCLE;
  FILE *fd           = checkfopen(std::format("{}circle.dat", Omegalib), "rb");
  char site          = cryptkey("circle.dat");
  for(int y = 0; y < LENGTH; ++y)
  {
    for(int x = 0; x < WIDTH; ++x)
    {
      Level->site[x][y].lstatus    = 0;
      Level->site[x][y].roomnumber = RS_CIRCLE;
      Level->site[x][y].p_locf     = L_NO_OP;
      site                         = getc(fd) ^ site;
      switch(site)
      {
        case 'P':
          Level->site[x][y].locchar = FLOOR;
          if(populate)
          {
            make_prime(x, y); // prime sorceror
            Level->site[x][y].creature->specialf = M_SP_PRIME;
            if(!safe)
            {
              m_status_set(*Level->site[x][y].creature, HOSTILE);
            }
          }
          break;
        case 'D':
          Level->site[x][y].locchar = FLOOR;
          if(populate)
          {
            make_site_monster(x, y, DEMON_PRINCE); // prime circle demon
            if(safe)
            {
              m_status_reset(*Level->site[x][y].creature, HOSTILE);
            }
            Level->site[x][y].creature->specialf = M_SP_COURT;
          }
          break;
        case 's':
          Level->site[x][y].locchar = FLOOR;
          if(populate)
          {
            make_site_monster(x, y, SERV_CHAOS); // servant of chaos
            Level->site[x][y].creature->specialf = M_SP_COURT;
            if(safe)
            {
              m_status_reset(*Level->site[x][y].creature, HOSTILE);
            }
          }
          break;
        case 'e':
          Level->site[x][y].locchar = FLOOR;
          if(populate)
          {
            make_site_monster(x, y, ENCHANTOR); // enchanter
            Level->site[x][y].creature->specialf = M_SP_COURT;
            if(safe)
            {
              m_status_reset(*Level->site[x][y].creature, HOSTILE);
            }
          }
          break;
        case 'n':
          Level->site[x][y].locchar = FLOOR;
          if(populate)
          {
            make_site_monster(x, y, NECROMANCER); // necromancer
            Level->site[x][y].creature->specialf = M_SP_COURT;
            if(safe)
            {
              m_status_reset(*Level->site[x][y].creature, HOSTILE);
            }
          }
          break;
        case 'T':
          Level->site[x][y].locchar = FLOOR;
          if(populate)
          {
            make_site_monster(x, y, THAUMATURGIST); // High Thaumaturgist
            Level->site[x][y].creature->specialf = M_SP_COURT;
            if(safe)
            {
              m_status_reset(*Level->site[x][y].creature, HOSTILE);
            }
          }
          break;
        case '#':
          Level->site[x][y].locchar = WALL;
          Level->site[x][y].aux     = 1000;
          break;
        case 'L':
          Level->site[x][y].locchar = FLOOR;
          Level->site[x][y].p_locf  = L_CIRCLE_LIBRARY;
          break;
        case '?':
          Level->site[x][y].locchar = FLOOR;
          Level->site[x][y].p_locf  = L_TOME1;
          break;
        case '!':
          Level->site[x][y].locchar = FLOOR;
          Level->site[x][y].p_locf  = L_TOME2;
          break;
        case 'S':
          Level->site[x][y].locchar = FLOOR;
          lset(x, y, SECRET, *Level);
          break;
        case '.':
          Level->site[x][y].locchar = FLOOR;
          break;
        case '-':
          Level->site[x][y].locchar = CLOSED_DOOR;
          break;
      }
    }
    site = getc(fd) ^ site;
  }
  fclose(fd);
}

// make the archmage
void make_archmage(int x, int y)
{
  auto m = std::make_unique<monster>();
  make_hiscore_npc(m.get(), 9); // 9 is index for archmage
  m->x                       = x;
  m->y                       = y;
  Level->site[x][y].creature = m.get();
  m->specialf                = M_SP_COURT;
  Level->mlist.emplace_front(std::move(m));
}

// loads the court of the archmage into Level
void load_court(int populate)
{
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
  Level->environment = E_COURT;
  FILE *fd           = checkfopen(std::format("{}court.dat", Omegalib), "rb");
  char site          = cryptkey("court.dat");
  for(int y = 0; y < LENGTH; ++y)
  {
    for(int x = 0; x < WIDTH; ++x)
    {
      Level->site[x][y].lstatus    = 0;
      Level->site[x][y].roomnumber = RS_COURT;
      Level->site[x][y].p_locf     = L_NO_OP;
      site                         = getc(fd) ^ site;
      switch(site)
      {
        case '5':
          Level->site[x][y].locchar = CHAIR;
          Level->site[x][y].p_locf  = L_THRONE;
          if(populate)
          {
            make_specific_treasure(x, y, ARTIFACTID + 22);
            make_archmage(x, y);
            m_status_reset(*Level->site[x][y].creature, HOSTILE);
            m_status_reset(*Level->site[x][y].creature, MOBILE);
          }
          break;
        case 'e':
          Level->site[x][y].locchar = FLOOR;
          if(populate)
          {
            make_site_monster(x, y, ENCHANTOR); // enchanter
            m_status_reset(*Level->site[x][y].creature, HOSTILE);
            Level->site[x][y].creature->specialf = M_SP_COURT;
          }
          break;
        case 'n':
          Level->site[x][y].locchar = FLOOR;
          if(populate)
          {
            make_site_monster(x, y, NECROMANCER); // necromancer
            m_status_reset(*Level->site[x][y].creature, HOSTILE);
            Level->site[x][y].creature->specialf = M_SP_COURT;
          }
          break;
        case 'T':
          Level->site[x][y].locchar = FLOOR;
          if(populate)
          {
            make_site_monster(x, y, THAUMATURGIST); // High Thaumaturgist
            m_status_reset(*Level->site[x][y].creature, HOSTILE);
            Level->site[x][y].creature->specialf = M_SP_COURT;
          }
          break;
        case '#':
          Level->site[x][y].locchar = WALL;
          Level->site[x][y].aux     = 1000;
          break;
        case 'G':
          Level->site[x][y].locchar = FLOOR;
          Level->site[x][y].locchar = FLOOR;
          if(populate)
          {
            make_site_monster(x, y, GUARD); // guard
            m_status_reset(*Level->site[x][y].creature, HOSTILE);
          }
          break;
        case '<':
          Level->site[x][y].locchar = STAIRS_UP;
          Level->site[x][y].p_locf  = L_ESCALATOR;
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
