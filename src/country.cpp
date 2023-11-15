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

/* country.c */
/* load_country(), and all load_ functions for places which are */
/* accessible from the country and don't have their own files */

#include "glob.h"
#include "scr.h"

#include <format>

#ifdef SAVE_LEVELS
extern level TheLevel;
plv msdos_changelevel(plv oldlevel, int newenv, int newdepth);
#endif

/* loads the countryside level from the data file */
void load_country()
{
  FILE *fd  = checkfopen(std::format("{}country.dat", Omegalib), "rb");
  char site = cryptkey("country.dat");

  for(int j = 0; j < LENGTH; j++)
  {
    for(int i = 0; i < WIDTH; i++)
    {
      site                 = getc(fd) ^ site;
      Country[i][j].aux    = 0;
      Country[i][j].status = 0;
      switch(site)
      {
        case(PASS & 0xff):
          Country[i][j].base_terrain_type    = PASS;
          Country[i][j].current_terrain_type = MOUNTAINS;
          break;
        case(CASTLE & 0xff):
          Country[i][j].base_terrain_type    = CASTLE;
          Country[i][j].current_terrain_type = MOUNTAINS;
          break;
        case(STARPEAK & 0xff):
          Country[i][j].base_terrain_type    = STARPEAK;
          Country[i][j].current_terrain_type = MOUNTAINS;
          break;
        case(CAVES & 0xff):
          Country[i][j].base_terrain_type    = CAVES;
          Country[i][j].current_terrain_type = MOUNTAINS;
          break;
        case(VOLCANO & 0xff):
          Country[i][j].base_terrain_type    = VOLCANO;
          Country[i][j].current_terrain_type = MOUNTAINS;
          break;
        case(DRAGONLAIR & 0xff):
          Country[i][j].base_terrain_type    = DRAGONLAIR;
          Country[i][j].current_terrain_type = DESERT;
          break;
        case(MAGIC_ISLE & 0xff):
          Country[i][j].base_terrain_type    = MAGIC_ISLE;
          Country[i][j].current_terrain_type = CHAOS_SEA;
          break;
        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
          Country[i][j].current_terrain_type = Country[i][j].base_terrain_type = VILLAGE;
          Country[i][j].aux                                                    = 1 + site - 'a';
          break;
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
          Country[i][j].current_terrain_type = Country[i][j].base_terrain_type = TEMPLE;
          Country[i][j].aux                                                    = site - '0';
          break;
        case(PLAINS & 0xff):
          Country[i][j].current_terrain_type = Country[i][j].base_terrain_type = PLAINS;
          break;
        case(TUNDRA & 0xff):
          Country[i][j].current_terrain_type = Country[i][j].base_terrain_type = TUNDRA;
          break;
        case(ROAD & 0xff):
          Country[i][j].current_terrain_type = Country[i][j].base_terrain_type = ROAD;
          break;
        case(MOUNTAINS & 0xff):
          Country[i][j].current_terrain_type = Country[i][j].base_terrain_type = MOUNTAINS;
          break;
        case(RIVER & 0xff):
          Country[i][j].current_terrain_type = Country[i][j].base_terrain_type = RIVER;
          break;
        case(CITY & 0xff):
          Country[i][j].current_terrain_type = Country[i][j].base_terrain_type = CITY;
          break;
        case(FOREST & 0xff):
          Country[i][j].current_terrain_type = Country[i][j].base_terrain_type = FOREST;
          break;
        case(JUNGLE & 0xff):
          Country[i][j].current_terrain_type = Country[i][j].base_terrain_type = JUNGLE;
          break;
        case(SWAMP & 0xff):
          Country[i][j].current_terrain_type = Country[i][j].base_terrain_type = SWAMP;
          break;
        case(DESERT & 0xff):
          Country[i][j].current_terrain_type = Country[i][j].base_terrain_type = DESERT;
          break;
        case(CHAOS_SEA & 0xff):
          Country[i][j].current_terrain_type = Country[i][j].base_terrain_type = CHAOS_SEA;
          break;
      }
    }
    site = getc(fd) ^ site;
  }
  fclose(fd);
}

/* loads the dragon's lair into Level*/
void load_dlair(int empty, int populate)
{
  if(empty)
  {
    queue_message("The Lair is now devoid of inhabitants and treasure.");
  }

  if(!populate)
  {
    empty = true;
  }
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
  Level->environment = E_DLAIR;
  FILE *fd           = checkfopen(std::format("{}dlair.dat", Omegalib), "rb");
  char site          = cryptkey("dlair.dat");
  for(int j = 0; j < LENGTH; j++)
  {
    for(int i = 0; i < WIDTH; i++)
    {
      Level->site[i][j].lstatus = 0;
      if(i < 48)
      {
        Level->site[i][j].roomnumber = RS_CAVERN;
      }
      else
      {
        Level->site[i][j].roomnumber = RS_DRAGONLORD;
      }
      Level->site[i][j].p_locf = L_NO_OP;
      site                     = getc(fd) ^ site;
      switch(site)
      {
        case 'D':
          Level->site[i][j].locchar = FLOOR;
          if(!empty)
          {
            make_site_monster(i, j, DRAGON_LORD);
            Level->site[i][j].creature->specialf = M_SP_LAIR;
          }
          break;
        case 'd':
          Level->site[i][j].locchar = FLOOR;
          if(!empty)
          {
            make_site_monster(i, j, DRAGON); /* elite dragons, actually */
            Level->site[i][j].creature->specialf = M_SP_LAIR;
            Level->site[i][j].creature->hit *= 2;
            Level->site[i][j].creature->dmg *= 2;
          }
          break;
        case 'W':
          Level->site[i][j].locchar = FLOOR;
          if(!empty)
          {
            make_site_monster(i, j, KING_WYV);
          }
          break;
        case 'M':
          Level->site[i][j].locchar = FLOOR;
          if(!empty)
          {
            make_site_monster(i, j, RANDOM);
          }
          break;
        case 'S':
          Level->site[i][j].locchar  = FLOOR;
          Level->site[i][j].showchar = WALL;
          if(!empty)
          {
            lset(i, j, SECRET, *Level);
          }
          Level->site[i][j].roomnumber = RS_SECRETPASSAGE;
          break;
        case '$':
          Level->site[i][j].locchar = FLOOR;
          if(!empty)
          {
            make_site_treasure(i, j, 10);
          }
          break;
        case 's':
          Level->site[i][j].locchar = FLOOR;
          Level->site[i][j].p_locf  = L_TRAP_SIREN;
          break;
        case '7':
          if(!empty)
          {
            Level->site[i][j].locchar = PORTCULLIS;
          }
          else
          {
            Level->site[i][j].locchar = FLOOR;
          }
          Level->site[i][j].p_locf = L_PORTCULLIS;
          break;
        case 'R':
          Level->site[i][j].locchar = FLOOR;
          Level->site[i][j].p_locf  = L_RAISE_PORTCULLIS;
          break;
        case 'p':
          Level->site[i][j].locchar = FLOOR;
          Level->site[i][j].p_locf  = L_PORTCULLIS;
          break;
        case 'T':
          Level->site[i][j].locchar = FLOOR;
          if(!empty)
          {
            Level->site[i][j].p_locf = L_PORTCULLIS_TRAP;
          }
          break;
        case 'X':
          Level->site[i][j].locchar = FLOOR;
          Level->site[i][j].p_locf  = L_TACTICAL_EXIT;
          break;
        case '#':
          Level->site[i][j].locchar = WALL;
          Level->site[i][j].aux     = 150;
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

/* loads the star peak into Level*/
void load_speak(int empty, int populate)
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
  Level->environment = E_STARPEAK;
  FILE *fd           = checkfopen(std::format("{}speak.dat", Omegalib), "rb");
  char site          = cryptkey("speak.dat");
  for(int j = 0; j < LENGTH; j++)
  {
    for(int i = 0; i < WIDTH; i++)
    {
      Level->site[i][j].lstatus    = 0;
      Level->site[i][j].roomnumber = RS_STARPEAK;
      Level->site[i][j].p_locf     = L_NO_OP;
      site                         = getc(fd) ^ site;
      switch(site)
      {
        case 'S':
          Level->site[i][j].locchar  = FLOOR;
          Level->site[i][j].showchar = WALL;
          lset(i, j, SECRET, *Level);
          Level->site[i][j].roomnumber = RS_SECRETPASSAGE;
          break;
        case 'L':
          Level->site[i][j].locchar = FLOOR;
          if(!empty)
          {
            make_site_monster(i, j, LAWBRINGER);
            if(safe)
            {
              m_status_reset(*Level->site[i][j].creature, HOSTILE);
            }
          }
          break;
        case 's':
          Level->site[i][j].locchar = FLOOR;
          if(!empty)
          {
            make_site_monster(i, j, SERV_LAW); /* servant of law */
            if(safe)
            {
              m_status_reset(*Level->site[i][j].creature, HOSTILE);
            }
          }
          break;
        case 'M':
          Level->site[i][j].locchar = FLOOR;
          if(!empty)
          {
            make_site_monster(i, j, -1);
            if(safe)
            {
              m_status_reset(*Level->site[i][j].creature, HOSTILE);
            }
          }
          break;
        case '$':
          Level->site[i][j].locchar = FLOOR;
          if(!empty)
          {
            make_site_treasure(i, j, 10);
          }
          break;
        case '7':
          if(!empty)
          {
            Level->site[i][j].locchar = PORTCULLIS;
          }
          else
          {
            Level->site[i][j].locchar = FLOOR;
          }
          Level->site[i][j].p_locf = L_PORTCULLIS;
          break;
        case 'R':
          Level->site[i][j].locchar = FLOOR;
          Level->site[i][j].p_locf  = L_RAISE_PORTCULLIS;
          break;
        case '-':
          Level->site[i][j].locchar = CLOSED_DOOR;
          break;
        case '|':
          Level->site[i][j].locchar = OPEN_DOOR;
          break;
        case 'p':
          Level->site[i][j].locchar = FLOOR;
          Level->site[i][j].p_locf  = L_PORTCULLIS;
          break;
        case 'T':
          Level->site[i][j].locchar = FLOOR;
          if(!empty)
          {
            Level->site[i][j].p_locf = L_PORTCULLIS_TRAP;
          }
          break;
        case 'X':
          Level->site[i][j].locchar = FLOOR;
          Level->site[i][j].p_locf  = L_TACTICAL_EXIT;
          break;
        case '#':
          Level->site[i][j].locchar = WALL;
          Level->site[i][j].aux     = 150;
          break;
        case '4':
          Level->site[i][j].locchar = RUBBLE;
          Level->site[i][j].p_locf  = L_RUBBLE;
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

/* loads the magic isle into Level*/
void load_misle(int empty, int populate)
{
  if(empty)
  {
    queue_message("The isle is now devoid of inhabitants and treasure.");
  }

  if(!populate)
  {
    empty = true;
  }

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
  Level->environment = E_MAGIC_ISLE;
  FILE *fd           = checkfopen(std::format("{}misle.dat", Omegalib), "rb");
  char site          = cryptkey("misle.dat");
  for(int j = 0; j < LENGTH; j++)
  {
    for(int i = 0; i < WIDTH; i++)
    {
      Level->site[i][j].lstatus    = 0;
      Level->site[i][j].roomnumber = RS_MAGIC_ISLE;
      Level->site[i][j].p_locf     = L_NO_OP;
      site                         = getc(fd) ^ site;
      switch(site)
      {
        case 'E':
          Level->site[i][j].locchar = FLOOR;
          if(!empty)
          {
            make_site_monster(i, j, EATER); /* eater of magic */
          }
          break;
        case 'm':
          Level->site[i][j].locchar = FLOOR;
          if(!empty)
          {
            make_site_monster(i, j, MIL_PRIEST); /* militant priest */
          }
          break;
        case 'n':
          Level->site[i][j].locchar = FLOOR;
          if(!empty)
          {
            make_site_monster(i, j, NAZGUL);
          }
          break;
        case 'X':
          Level->site[i][j].locchar = FLOOR;
          Level->site[i][j].p_locf  = L_TACTICAL_EXIT;
          break;
        case '#':
          Level->site[i][j].locchar = WALL;
          Level->site[i][j].aux     = 150;
          break;
        case '4':
          Level->site[i][j].locchar = RUBBLE;
          Level->site[i][j].p_locf  = L_RUBBLE;
          break;
        case '~':
          Level->site[i][j].locchar = WATER;
          Level->site[i][j].p_locf  = L_CHAOS;
          break;
        case '=':
          Level->site[i][j].locchar = WATER;
          Level->site[i][j].p_locf  = L_MAGIC_POOL;
          break;
        case '-':
          Level->site[i][j].locchar = CLOSED_DOOR;
          break;
        case '|':
          Level->site[i][j].locchar = OPEN_DOOR;
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

void make_high_priest(int i, int j, int deity)
{
  monsterlist *ml = new monsterlist;
  pmt m  = new monster;
  make_hiscore_npc(m, deity);
  m->x                       = i;
  m->y                       = j;
  Level->site[i][j].creature = m;
  ml->m                      = m;
  ml->next                   = Level->mlist;
  Level->mlist               = ml;
}

void random_temple_site(int i, int j, int, int populate)
{
  switch(random_range(12))
  {
    case 0:
      if(populate)
      {
        make_site_monster(i, j, MEND_PRIEST);
      }
      break; /* mendicant priest */
    case 1:
      Level->site[i][j].locchar = WATER;
      Level->site[i][j].p_locf  = L_MAGIC_POOL;
      [[fallthrough]];
    case 2:
      if(populate)
      {
        make_site_monster(i, j, INNER_DEMON);
      }
      break; /* inner circle demon */
    case 3:
      if(populate)
      {
        make_site_monster(i, j, ANGEL);
      }
      break; /* angel of apropriate sect */
    case 4:
      if(populate)
      {
        make_site_monster(i, j, HIGH_ANGEL);
      }
      break; /* high angel of apropriate sect */
    case 5:
      if(populate)
      {
        make_site_monster(i, j, ARCHANGEL);
      }
      break; /* archangel of apropriate sect */
  }
}

/* loads a temple into Level*/
void load_temple(int deity, int populate)
{
  /* WDT HACK: I don't know why this is wrong.  Shrug.  David Givens
   * suggested removing it, and he has more experience with Omega
   * than I, so... */
  /*  initrand(Current_Environment, deity); */ /* FIXED! 12/30/98 */
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
  Level->environment = E_TEMPLE;
  FILE *fd           = checkfopen(std::format("{}temple.dat", Omegalib), "rb");
  char site          = cryptkey("temple.dat");
  for(int j = 0; j < LENGTH; j++)
  {
    for(int i = 0; i < WIDTH; i++)
    {
      switch(deity)
      {
        case ODIN:
          Level->site[i][j].roomnumber = RS_ODIN;
          break;
        case SET:
          Level->site[i][j].roomnumber = RS_SET;
          break;
        case HECATE:
          Level->site[i][j].roomnumber = RS_HECATE;
          break;
        case ATHENA:
          Level->site[i][j].roomnumber = RS_ATHENA;
          break;
        case DRUID:
          Level->site[i][j].roomnumber = RS_DRUID;
          break;
        case DESTINY:
          Level->site[i][j].roomnumber = RS_DESTINY;
          break;
      }
      site = getc(fd) ^ site;
      switch(site)
      {
        case '8':
          Level->site[i][j].locchar = ALTAR;
          Level->site[i][j].p_locf  = L_ALTAR;
          Level->site[i][j].aux     = deity;
          break;
        case 'H':
          Level->site[i][j].locchar = FLOOR;
          if(populate && (!Player.patron || Player.name != Priest[Player.patron] || Player.rank[PRIESTHOOD] != HIGHPRIEST))
          {
            make_high_priest(i, j, deity);
          }
          break;
        case 'S':
          Level->site[i][j].locchar = FLOOR;
          if(!Player.patron || Player.name != Priest[Player.patron] || Player.rank[PRIESTHOOD] != HIGHPRIEST)
          {
            lset(i, j, SECRET, *Level);
          }
          break;
        case 'W':
          Level->site[i][j].locchar = FLOOR;
          if(deity != Player.patron && deity != DRUID)
          {
            Level->site[i][j].p_locf = L_TEMPLE_WARNING;
          }
          break;
        case 'm':
          Level->site[i][j].locchar = FLOOR;
          if(populate)
          {
            make_site_monster(i, j, MIL_PRIEST); /* militant priest */
          }
          break;
        case 'd':
          Level->site[i][j].locchar = FLOOR;
          if(populate)
          {
            make_site_monster(i, j, DOBERMAN); /* doberman death hound */
          }
          break;
        case 'X':
          Level->site[i][j].locchar = FLOOR;
          Level->site[i][j].p_locf  = L_TACTICAL_EXIT;
          break;
        case '#':
          if(deity != DRUID)
          {
            Level->site[i][j].locchar = WALL;
            Level->site[i][j].aux     = 150;
          }
          else
          {
            Level->site[i][j].locchar = HEDGE;
            Level->site[i][j].p_locf  = L_HEDGE;
          }
          break;
        case '.':
          Level->site[i][j].locchar = FLOOR;
          break;
        case 'x':
          Level->site[i][j].locchar = FLOOR;
          random_temple_site(i, j, deity, populate);
          break;
        case '?':
          if(deity != DESTINY)
          {
            Level->site[i][j].locchar = FLOOR;
          }
          else
          {
            Level->site[i][j].locchar = ABYSS;
            Level->site[i][j].p_locf  = L_ADEPT;
          }
          break;
        case '-':
          Level->site[i][j].locchar = CLOSED_DOOR;
          break;
        case '|':
          Level->site[i][j].locchar = OPEN_DOOR;
          break;
      }
    }
    site = getc(fd) ^ site;
  }
  /* Main Temple is peaceful for player of same sect,druids always peaceful. */
  if((Player.patron == deity) || (deity == DRUID))
  {
    for(monsterlist *ml = Level->mlist; ml; ml = ml->next)
    {
      m_status_reset(*ml->m, HOSTILE);
    }
  }
  fclose(fd);
  /*  initrand(-2, 0); */ /* FIXED! 12/30/98 */
}
