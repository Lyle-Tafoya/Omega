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

/* house.c */
/* some functions to make the house levels */

#include "glob.h"
#include "scr.h"

#include <format>

#ifdef SAVE_LEVELS
extern level TheLevel;
plv msdos_changelevel(plv oldlevel, int newenv, int newdepth);
#endif

// makes a log npc for houses and hovels
void make_house_npc(int i, int j)
{
  pml ml = new monsterlist;
  pob ob;
  ml->m    = new monster;
  *(ml->m) = Monsters[NPC];
  make_log_npc(ml->m);
  if(ml->m->id == NPC)
  {
    queue_message("You detect signs of life in this house.");
  }
  else
  {
    queue_message("An eerie shiver runs down your spine as you enter....");
  }
  // if not == NPC, then we got a ghost off the npc list
  ml->m->x                   = i;
  ml->m->y                   = j;
  Level->site[i][j].creature = ml->m;
  ml->m->click               = (Tick + 1) % 50;
  ml->next                   = Level->mlist;
  Level->mlist               = ml;
  m_status_set(*ml->m, HOSTILE);
  if(nighttime())
  {
    m_status_reset(*ml->m, AWAKE);
  }
  else
  {
    m_status_set(*ml->m, AWAKE);
  }
  if(ml->m->startthing > -1)
  {
    ob  = new object;
    *ob = Objects[ml->m->startthing];
    m_pickup(ml->m, ob);
  }
}

// makes a hiscore npc for mansions
void make_mansion_npc(int i, int j)
{
  pml ml   = new monsterlist;
  ml->m    = new monster;
  *(ml->m) = Monsters[NPC];
  make_hiscore_npc(ml->m, random_range(14) + 1);
  queue_message("You detect signs of life in this house.");
  ml->m->x                   = i;
  ml->m->y                   = j;
  Level->site[i][j].creature = ml->m;
  ml->m->click               = (Tick + 1) % 50;
  ml->next                   = Level->mlist;
  Level->mlist               = ml;
  m_status_set(*ml->m, HOSTILE);
  if(nighttime())
  {
    m_status_reset(*ml->m, AWAKE);
  }
  else
  {
    m_status_set(*ml->m, AWAKE);
  }
}

/* loads the house level into Level*/
void load_house(int kind, int populate)
{
  int i, j;
  char site;
  int stops;

  FILE *fd;

  TempLevel = Level;
  initrand(Current_Environment, Player.x + Player.y + hour() * 10);
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
  std::string home_filepath;
  switch(kind)
  {
    case E_HOUSE:
      home_filepath      = std::format("{}home1.dat", Omegalib);
      Level->environment = E_HOUSE;
      site               = cryptkey("home1.dat");
      break;
    case E_MANSION:
      home_filepath      = std::format("{}home2.dat", Omegalib);
      Level->environment = E_MANSION;
      site               = cryptkey("home2.dat");
      break;
    default:
    case E_HOVEL:
      home_filepath      = std::format("{}home3.dat", Omegalib);
      Level->environment = E_HOVEL;
      site               = cryptkey("home3.dat");
      break;
  }
  fd    = checkfopen(home_filepath, "rb");
  stops = 0;
  for(j = 0; j < LENGTH; j++)
  {
    for(i = 0; i < WIDTH; i++)
    {
      if(kind == E_HOVEL)
      {
        Level->site[i][j].lstatus = SEEN;
      }
      else
      {
        Level->site[i][j].lstatus = 0;
      }
      Level->site[i][j].roomnumber = RS_CORRIDOR;
      Level->site[i][j].p_locf     = L_NO_OP;
      site                         = getc(fd) ^ site;
      switch(site)
      {
        case 'N':
          Level->site[i][j].locchar    = FLOOR;
          Level->site[i][j].roomnumber = RS_BEDROOM;
          if(random_range(2) && populate)
          {
            make_house_npc(i, j);
          }
          break;
        case 'H':
          Level->site[i][j].locchar    = FLOOR;
          Level->site[i][j].roomnumber = RS_BEDROOM;
          if(random_range(2) && populate)
          {
            make_mansion_npc(i, j);
          }
          break;
        case 'D':
          Level->site[i][j].locchar    = FLOOR;
          Level->site[i][j].roomnumber = RS_DININGROOM;
          break;
        case '.':
          Level->site[i][j].locchar = FLOOR;
          if(stops)
          {
            lset(i, j, STOPS, *Level);
            stops = 0;
          }
          break;
        case 'c':
          Level->site[i][j].locchar    = FLOOR;
          Level->site[i][j].roomnumber = RS_CLOSET;
          break;
        case 'G':
          Level->site[i][j].locchar    = FLOOR;
          Level->site[i][j].roomnumber = RS_BATHROOM;
          break;
        case 'B':
          Level->site[i][j].locchar    = FLOOR;
          Level->site[i][j].roomnumber = RS_BEDROOM;
          break;
        case 'K':
          Level->site[i][j].locchar    = FLOOR;
          Level->site[i][j].roomnumber = RS_KITCHEN;
          break;
        case 'S':
          Level->site[i][j].locchar  = FLOOR;
          Level->site[i][j].showchar = WALL;
          lset(i, j, SECRET, *Level);
          Level->site[i][j].roomnumber = RS_SECRETPASSAGE;
          break;
        case '3':
          Level->site[i][j].locchar  = SAFE;
          Level->site[i][j].showchar = WALL;
          lset(i, j, SECRET, *Level);
          Level->site[i][j].p_locf = L_SAFE;
          break;
        case '^':
          Level->site[i][j].locchar = FLOOR;
          Level->site[i][j].p_locf  = TRAP_BASE + random_range(NUMTRAPS);
          break;
        case 'P':
          Level->site[i][j].locchar = PORTCULLIS;
          Level->site[i][j].p_locf  = L_PORTCULLIS;
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
          Level->site[i][j].p_locf  = L_PORTCULLIS_TRAP;
          break;
        case 'X':
          Level->site[i][j].locchar = FLOOR;
          Level->site[i][j].p_locf  = L_HOUSE_EXIT;
          stops                     = 1;
          break;
        case '#':
          Level->site[i][j].locchar = WALL;
          switch(kind)
          {
            case E_HOVEL:
              Level->site[i][j].aux = 10;
              break;
            case E_HOUSE:
              Level->site[i][j].aux = 50;
              break;
            case E_MANSION:
              Level->site[i][j].aux = 150;
              break;
          }
          break;
        case '|':
          Level->site[i][j].locchar    = OPEN_DOOR;
          Level->site[i][j].roomnumber = RS_CORRIDOR;
          lset(i, j, STOPS, *Level);
          break;
        case '+':
          Level->site[i][j].locchar    = CLOSED_DOOR;
          Level->site[i][j].roomnumber = RS_CORRIDOR;
          Level->site[i][j].aux        = LOCKED;
          lset(i, j, STOPS, *Level);
          break;
        case 'd':
          Level->site[i][j].locchar    = FLOOR;
          Level->site[i][j].roomnumber = RS_CORRIDOR;
          if(populate)
          {
            make_site_monster(i, j, DOBERMAN);
          }
          break;
        case 'a':
          Level->site[i][j].locchar    = FLOOR;
          Level->site[i][j].roomnumber = RS_CORRIDOR;
          Level->site[i][j].p_locf     = L_TRAP_SIREN;
          break;
        case 'A':
          Level->site[i][j].locchar    = FLOOR;
          Level->site[i][j].roomnumber = RS_CORRIDOR;
          if(populate)
          {
            make_site_monster(i, j, AUTO_MINOR); /* automaton */
          }
          break;
      }
      Level->site[i][j].showchar = ' ';
    }
    site = getc(fd) ^ site;
  }
  fclose(fd);
  initrand(E_RESTORE, 0);
}
