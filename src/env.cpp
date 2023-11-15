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

/* env.c */
/* some load_* routines for special environments */

#include "glob.h"
#include "scr.h"

#include <format>

#ifdef SAVE_LEVELS
extern level TheLevel;
plv msdos_changelevel(plv oldlevel, int newenv, int newdepth);
#endif

/* loads the arena level into Level*/
void load_arena()
{
  int i, j;
  char site;
  pob box = new object;
  FILE *fd;

  *box = Objects[THINGID + 0];

  TempLevel = Level;
  if(ok_to_free(TempLevel))
  {
#ifndef SAVE_LEVELS
    free_level(TempLevel);
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
  for(j = 0; j < LENGTH; j++)
  {
    for(i = 0; i < WIDTH; i++)
    {
      Level->site[i][j].lstatus    = SEEN + LIT;
      Level->site[i][j].roomnumber = RS_ARENA;
      site                         = getc(fd) ^ site;
      Level->site[i][j].p_locf     = L_NO_OP;
      switch(site)
      {
        case 'P':
          Level->site[i][j].locchar = PORTCULLIS;
          Level->site[i][j].p_locf  = L_PORTCULLIS;
          break;
        case 'X':
          Level->site[i][j].locchar = FLOOR;
          Level->site[i][j].p_locf  = L_ARENA_EXIT;
          break;
        case '#':
          Level->site[i][j].locchar = WALL;
          break;
        case '.':
          Level->site[i][j].locchar = FLOOR;
          break;
      }
      Level->site[i][j].showchar = Level->site[i][j].locchar;
    }
    site = getc(fd) ^ site;
  }
  fclose(fd);
  Level->site[60][7].creature = Arena_Monster;
  Arena_Monster->x            = 60;
  Arena_Monster->y            = 7;
  Arena_Monster->sense        = 50;
  m_pickup(Arena_Monster, box);
  m_status_set(*Arena_Monster, AWAKE);
  Level->mlist       = new monsterlist;
  Level->mlist->m    = Arena_Monster;
  Level->mlist->next = nullptr;
  /* hehehehe cackled the dungeon master.... */
  queue_message("Your opponent holds the only way you can leave!");
  Arena_Monster->hp += Arena_Monster->level * 10;
  Arena_Monster->hit += Arena_Monster->hit;
  Arena_Monster->dmg += Arena_Monster->dmg / 2;
}

// make the prime sorceror
void make_prime(int i, int j)
{
  monsterlist *ml = new monsterlist;
  pmt m  = new monster;
  pol ol;
  pob o;
  make_hiscore_npc(m, 10); /* 10 is index for prime */
  m->x                       = i;
  m->y                       = j;
  Level->site[i][j].creature = m;
  ml->m                      = m;
  ml->next                   = Level->mlist;
  Level->mlist               = ml;

  if(Objects[ARTIFACTID + 21].uniqueness != UNIQUE_TAKEN)
  {
    ol             = new objectlist;
    o              = new object;
    *o             = Objects[ARTIFACTID + 21];
    ol->thing      = o;
    ol->next       = nullptr;
    m->possessions = ol;
  }
}

/* loads the sorcereror's circle into Level*/
void load_circle(int populate)
{
  int i, j;
  int safe = (Player.rank[CIRCLE] > 0);
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
#ifdef SAVE_LEVELS
  msdos_changelevel(TempLevel, 0, -1);
  Level = &TheLevel;
#else
  Level = new level;
#endif
  clear_level(Level);
  Level->environment = E_CIRCLE;
  fd                 = checkfopen(std::format("{}circle.dat", Omegalib), "rb");
  site               = cryptkey("circle.dat");
  for(j = 0; j < LENGTH; j++)
  {
    for(i = 0; i < WIDTH; i++)
    {
      Level->site[i][j].lstatus    = 0;
      Level->site[i][j].roomnumber = RS_CIRCLE;
      Level->site[i][j].p_locf     = L_NO_OP;
      site                         = getc(fd) ^ site;
      switch(site)
      {
        case 'P':
          Level->site[i][j].locchar = FLOOR;
          if(populate)
          {
            make_prime(i, j); /* prime sorceror */
            Level->site[i][j].creature->specialf = M_SP_PRIME;
            if(!safe)
            {
              m_status_set(*Level->site[i][j].creature, HOSTILE);
            }
          }
          break;
        case 'D':
          Level->site[i][j].locchar = FLOOR;
          if(populate)
          {
            make_site_monster(i, j, DEMON_PRINCE); /* prime circle demon */
            if(safe)
            {
              m_status_reset(*Level->site[i][j].creature, HOSTILE);
            }
            Level->site[i][j].creature->specialf = M_SP_COURT;
          }
          break;
        case 's':
          Level->site[i][j].locchar = FLOOR;
          if(populate)
          {
            make_site_monster(i, j, SERV_CHAOS); /* servant of chaos */
            Level->site[i][j].creature->specialf = M_SP_COURT;
            if(safe)
            {
              m_status_reset(*Level->site[i][j].creature, HOSTILE);
            }
          }
          break;
        case 'e':
          Level->site[i][j].locchar = FLOOR;
          if(populate)
          {
            make_site_monster(i, j, ENCHANTOR); /* enchanter */
            Level->site[i][j].creature->specialf = M_SP_COURT;
            if(safe)
            {
              m_status_reset(*Level->site[i][j].creature, HOSTILE);
            }
          }
          break;
        case 'n':
          Level->site[i][j].locchar = FLOOR;
          if(populate)
          {
            make_site_monster(i, j, NECROMANCER); /* necromancer */
            Level->site[i][j].creature->specialf = M_SP_COURT;
            if(safe)
            {
              m_status_reset(*Level->site[i][j].creature, HOSTILE);
            }
          }
          break;
        case 'T':
          Level->site[i][j].locchar = FLOOR;
          if(populate)
          {
            make_site_monster(i, j, THAUMATURGIST); /* High Thaumaturgist */
            Level->site[i][j].creature->specialf = M_SP_COURT;
            if(safe)
            {
              m_status_reset(*Level->site[i][j].creature, HOSTILE);
            }
          }
          break;
        case '#':
          Level->site[i][j].locchar = WALL;
          Level->site[i][j].aux     = 1000;
          break;
        case 'L':
          Level->site[i][j].locchar = FLOOR;
          Level->site[i][j].p_locf  = L_CIRCLE_LIBRARY;
          break;
        case '?':
          Level->site[i][j].locchar = FLOOR;
          Level->site[i][j].p_locf  = L_TOME1;
          break;
        case '!':
          Level->site[i][j].locchar = FLOOR;
          Level->site[i][j].p_locf  = L_TOME2;
          break;
        case 'S':
          Level->site[i][j].locchar = FLOOR;
          lset(i, j, SECRET, *Level);
          break;
        case '.':
          Level->site[i][j].locchar = FLOOR;
          break;
        case '-':
          Level->site[i][j].locchar = CLOSED_DOOR;
          break;
      }
    }
    site = getc(fd) ^ site;
  }
  fclose(fd);
}

// make the archmage
void make_archmage(int i, int j)
{
  monsterlist *ml = new monsterlist;
  pmt m  = new monster;
  make_hiscore_npc(m, 9); /* 9 is index for archmage */
  m->x                       = i;
  m->y                       = j;
  Level->site[i][j].creature = m;
  ml->m                      = m;
  ml->next                   = Level->mlist;
  Level->mlist               = ml;
  m->specialf                = M_SP_COURT;
}

/* loads the court of the archmage into Level*/
void load_court(int populate)
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
#ifdef SAVE_LEVELS
  msdos_changelevel(TempLevel, 0, -1);
  Level = &TheLevel;
#else
  Level = new level;
#endif
  clear_level(Level);
  Level->environment = E_COURT;
  fd                 = checkfopen(std::format("{}court.dat", Omegalib), "rb");
  site               = cryptkey("court.dat");
  for(j = 0; j < LENGTH; j++)
  {
    for(i = 0; i < WIDTH; i++)
    {
      Level->site[i][j].lstatus    = 0;
      Level->site[i][j].roomnumber = RS_COURT;
      Level->site[i][j].p_locf     = L_NO_OP;
      site                         = getc(fd) ^ site;
      switch(site)
      {
        case '5':
          Level->site[i][j].locchar = CHAIR;
          Level->site[i][j].p_locf  = L_THRONE;
          if(populate)
          {
            make_specific_treasure(i, j, ARTIFACTID + 22);
            make_archmage(i, j);
            m_status_reset(*Level->site[i][j].creature, HOSTILE);
            m_status_reset(*Level->site[i][j].creature, MOBILE);
          }
          break;
        case 'e':
          Level->site[i][j].locchar = FLOOR;
          if(populate)
          {
            make_site_monster(i, j, ENCHANTOR); /* enchanter */
            m_status_reset(*Level->site[i][j].creature, HOSTILE);
            Level->site[i][j].creature->specialf = M_SP_COURT;
          }
          break;
        case 'n':
          Level->site[i][j].locchar = FLOOR;
          if(populate)
          {
            make_site_monster(i, j, NECROMANCER); /* necromancer */
            m_status_reset(*Level->site[i][j].creature, HOSTILE);
            Level->site[i][j].creature->specialf = M_SP_COURT;
          }
          break;
        case 'T':
          Level->site[i][j].locchar = FLOOR;
          if(populate)
          {
            make_site_monster(i, j, THAUMATURGIST); /* High Thaumaturgist */
            m_status_reset(*Level->site[i][j].creature, HOSTILE);
            Level->site[i][j].creature->specialf = M_SP_COURT;
          }
          break;
        case '#':
          Level->site[i][j].locchar = WALL;
          Level->site[i][j].aux     = 1000;
          break;
        case 'G':
          Level->site[i][j].locchar = FLOOR;
          Level->site[i][j].locchar = FLOOR;
          if(populate)
          {
            make_site_monster(i, j, GUARD); /* guard */
            m_status_reset(*Level->site[i][j].creature, HOSTILE);
          }
          break;
        case '<':
          Level->site[i][j].locchar = STAIRS_UP;
          Level->site[i][j].p_locf  = L_ESCALATOR;
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
