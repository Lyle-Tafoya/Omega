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

// house.cpp
// some functions to make the house levels

#include "glob.h"
#include "scr.h"

#include <format>

// makes a log npc for houses and hovels
void make_house_npc(int x, int y)
{
  auto m = std::make_unique<monster>(Monsters[NPC]);
  make_log_npc(m.get());
  if(m->id == NPC)
  {
    queue_message("You detect signs of life in this house.");
  }
  else
  {
    queue_message("An eerie shiver runs down your spine as you enter....");
  }
  // if not == NPC, then we got a ghost off the npc list
  m->x                       = x;
  m->y                       = y;
  Level->site[x][y].creature = m.get();
  m->click                   = (Tick + 1) % 50;
  m_status_set(*m, HOSTILE);
  if(nighttime())
  {
    m_status_reset(*m, AWAKE);
  }
  else
  {
    m_status_set(*m, AWAKE);
  }
  if(m->startthing > -1)
  {
    m_pickup(m.get(), std::make_unique<object>(Objects[m->startthing]));
  }
  Level->mlist.emplace_front(std::move(m));
}
// makes a hiscore npc for mansions
void make_mansion_npc(int x, int y)
{
  auto m = std::make_unique<monster>(Monsters[NPC]);
  make_hiscore_npc(m.get(), random_range(14) + 1);
  queue_message("You detect signs of life in this house.");
  m->x                       = x;
  m->y                       = y;
  Level->site[x][y].creature = m.get();
  m->click                   = (Tick + 1) % 50;
  m_status_set(*m, HOSTILE);
  if(nighttime())
  {
    m_status_reset(*m, AWAKE);
  }
  else
  {
    m_status_set(*m, AWAKE);
  }
  Level->mlist.emplace_front(std::move(m));
}

// loads the house level into Level
void load_house(int kind, int populate, std::unique_ptr<level> lvl)
{
  if(lvl)
  {
    TempLevel = std::move(lvl);
  }
  else
  {
    TempLevel = std::make_unique<level>();
  }
  initrand(Current_Environment, Player.x + Player.y + hour() * 10);
  Level = TempLevel.get();
  clear_level(Level);
  std::string home_filepath;
  char site;
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
  FILE *fd = checkfopen(home_filepath, "rb");
  int stops = 0;
  for(int y = 0; y < LENGTH; ++y)
  {
    for(int x = 0; x < WIDTH; ++x)
    {
      if(kind == E_HOVEL)
      {
        Level->site[x][y].lstatus = SEEN;
      }
      else
      {
        Level->site[x][y].lstatus = 0;
      }
      Level->site[x][y].roomnumber = RS_CORRIDOR;
      Level->site[x][y].p_locf     = L_NO_OP;
      site                         = getc(fd) ^ site;
      switch(site)
      {
        case 'N':
          Level->site[x][y].locchar    = FLOOR;
          Level->site[x][y].roomnumber = RS_BEDROOM;
          if(random_range(2) && populate)
          {
            make_house_npc(x, y);
          }
          break;
        case 'H':
          Level->site[x][y].locchar    = FLOOR;
          Level->site[x][y].roomnumber = RS_BEDROOM;
          if(random_range(2) && populate)
          {
            make_mansion_npc(x, y);
          }
          break;
        case 'D':
          Level->site[x][y].locchar    = FLOOR;
          Level->site[x][y].roomnumber = RS_DININGROOM;
          break;
        case '.':
          Level->site[x][y].locchar = FLOOR;
          if(stops)
          {
            lset(x, y, STOPS, *Level);
            stops = 0;
          }
          break;
        case 'c':
          Level->site[x][y].locchar    = FLOOR;
          Level->site[x][y].roomnumber = RS_CLOSET;
          break;
        case 'G':
          Level->site[x][y].locchar    = FLOOR;
          Level->site[x][y].roomnumber = RS_BATHROOM;
          break;
        case 'B':
          Level->site[x][y].locchar    = FLOOR;
          Level->site[x][y].roomnumber = RS_BEDROOM;
          break;
        case 'K':
          Level->site[x][y].locchar    = FLOOR;
          Level->site[x][y].roomnumber = RS_KITCHEN;
          break;
        case 'S':
          Level->site[x][y].locchar  = FLOOR;
          Level->site[x][y].showchar = WALL;
          lset(x, y, SECRET, *Level);
          Level->site[x][y].roomnumber = RS_SECRETPASSAGE;
          break;
        case '3':
          Level->site[x][y].locchar  = SAFE;
          Level->site[x][y].showchar = WALL;
          lset(x, y, SECRET, *Level);
          Level->site[x][y].p_locf = L_SAFE;
          break;
        case '^':
          Level->site[x][y].locchar = FLOOR;
          Level->site[x][y].p_locf  = TRAP_BASE + random_range(NUMTRAPS);
          break;
        case 'P':
          Level->site[x][y].locchar = PORTCULLIS;
          Level->site[x][y].p_locf  = L_PORTCULLIS;
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
          Level->site[x][y].p_locf  = L_PORTCULLIS_TRAP;
          break;
        case 'X':
          Level->site[x][y].locchar = FLOOR;
          Level->site[x][y].p_locf  = L_HOUSE_EXIT;
          stops                     = 1;
          break;
        case '#':
          Level->site[x][y].locchar = WALL;
          switch(kind)
          {
            case E_HOVEL:
              Level->site[x][y].aux = 10;
              break;
            case E_HOUSE:
              Level->site[x][y].aux = 50;
              break;
            case E_MANSION:
              Level->site[x][y].aux = 150;
              break;
          }
          break;
        case '|':
          Level->site[x][y].locchar    = OPEN_DOOR;
          Level->site[x][y].roomnumber = RS_CORRIDOR;
          lset(x, y, STOPS, *Level);
          break;
        case '+':
          Level->site[x][y].locchar    = CLOSED_DOOR;
          Level->site[x][y].roomnumber = RS_CORRIDOR;
          Level->site[x][y].aux        = LOCKED;
          lset(x, y, STOPS, *Level);
          break;
        case 'd':
          Level->site[x][y].locchar    = FLOOR;
          Level->site[x][y].roomnumber = RS_CORRIDOR;
          if(populate)
          {
            make_site_monster(x, y, DOBERMAN);
          }
          break;
        case 'a':
          Level->site[x][y].locchar    = FLOOR;
          Level->site[x][y].roomnumber = RS_CORRIDOR;
          Level->site[x][y].p_locf     = L_TRAP_SIREN;
          break;
        case 'A':
          Level->site[x][y].locchar    = FLOOR;
          Level->site[x][y].roomnumber = RS_CORRIDOR;
          if(populate)
          {
            make_site_monster(x, y, AUTO_MINOR); // automaton
          }
          break;
      }
      Level->site[x][y].showchar = ' ';
    }
    site = getc(fd) ^ site;
  }
  fclose(fd);
  initrand(E_RESTORE, 0);
}
