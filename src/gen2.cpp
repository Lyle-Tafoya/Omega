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

// gen2.cpp
// level generator functions

#include "glob.h"
#include "scr.h"

#ifdef SAVE_LEVELS
extern level TheLevel;
level *msdos_changelevel(level *oldlevel, int newenv, int newdepth);
#endif

// For each level, there should be one stairway going up and one down.
// fromlevel determines whether the player is placed on the up or the down
// staircase. The aux value is currently unused elsewhere, but is set
// to the destination level.

void make_stairs(int fromlevel)
{
  int x, y;
  // no stairway out of astral
  if(Current_Environment != E_ASTRAL)
  {
    do
    {
      findspace(&x, &y, -1);
    } while(Level->site[x][y].p_locf != L_NO_OP);
    Level->site[x][y].locchar = STAIRS_UP;
    Level->site[x][y].aux     = Level->depth - 1;
    lset(x, y, STOPS, *Level);
    if(fromlevel >= 0 && fromlevel < Level->depth)
    {
      Player.x = x;
      Player.y = y;
    }
  }
  if(Level->depth < MaxDungeonLevels)
  {
    do
    {
      findspace(&x, &y, -1);
    } while(Level->site[x][y].p_locf != L_NO_OP);
    Level->site[x][y].locchar = STAIRS_DOWN;
    Level->site[x][y].aux     = Level->depth + 1;
    lset(x, y, STOPS, *Level);
    if(fromlevel > Level->depth)
    {
      Player.x = x;
      Player.y = y;
    }
  }
}

void make_general_map(const std::string &terrain)
{
  char curr;

  for(int x = 0; x < WIDTH; ++x)
  {
    for(int y = 0; y < LENGTH; ++y)
    {
      if((x == 0 && y == 0) || !random_range(5))
      {
        curr = terrain[random_range(static_cast<int>(terrain.length()))];
      }
      else if(y == 0 || (random_range(2) && x > 0))
      {
        curr = Level->site[x - 1][y].locchar & 0xff;
      }
      else
      {
        curr = Level->site[x][y - 1].locchar & 0xff;
      }
      switch(curr)
      {
        case(FLOOR & 0xff):
          Level->site[x][y].locchar = Level->site[x][y].showchar = FLOOR;
          Level->site[x][y].p_locf                               = L_NO_OP;
          break;
        case(HEDGE & 0xff):
          Level->site[x][y].locchar = Level->site[x][y].showchar = HEDGE;
          Level->site[x][y].p_locf                               = L_HEDGE;
          break;
        case(WATER & 0xff):
          Level->site[x][y].locchar = Level->site[x][y].showchar = WATER;
          Level->site[x][y].p_locf                               = L_WATER;
          break;
        case(RUBBLE & 0xff):
          Level->site[x][y].locchar = Level->site[x][y].showchar = RUBBLE;
          Level->site[x][y].p_locf                               = L_RUBBLE;
          break;
      }
      Level->site[x][y].lstatus    = SEEN + LIT;
      Level->site[x][y].roomnumber = RS_COUNTRYSIDE;
      if(x == 0 || y == 0 || x == WIDTH - 1 || y == LENGTH - 1)
      {
        Level->site[x][y].p_locf = L_TACTICAL_EXIT;
      }
    }
  }
}

void make_forest()
{
  make_general_map("\".");
  straggle_corridor(0, random_range(LENGTH), WIDTH, random_range(LENGTH), WATER, RS_COUNTRYSIDE);
}

void make_plains()
{
  make_general_map(".");
}

void make_road()
{
  make_general_map("\"\"~4....");
  for(int x = WIDTH / 2 - 3; x <= WIDTH / 2 + 3; ++x)
  {
    for(int y = 0; y < LENGTH; ++y)
    {
      Level->site[x][y].locchar = Level->site[x][y].showchar = FLOOR;
      if(y != 0 && y != LENGTH - 1)
      {
        Level->site[x][y].p_locf = L_NO_OP;
      }
    }
  }
}

void make_jungle()
{
  make_general_map("\"\".");
}

void make_river()
{
  make_general_map("\".......");
  int y  = random_range(LENGTH);
  int y1 = random_range(LENGTH);
  straggle_corridor(0, y, WIDTH, y1, WATER, RS_COUNTRYSIDE);
  for(int i = 0; i < 7; ++i)
  {
    if(y > LENGTH / 2)
    {
      y--;
    }
    else
    {
      y++;
    }
    if(y1 > LENGTH / 2)
    {
      y1--;
    }
    else
    {
      y1++;
    }
    straggle_corridor(0, y, WIDTH, y1, WATER, RS_COUNTRYSIDE);
  }
}

void make_mountains()
{
  make_general_map("4...");
  int x  = 0;
  int y  = random_range(LENGTH);
  int x1 = WIDTH;
  int y1 = random_range(LENGTH);
  straggle_corridor(x, y, x1, y1, WATER, RS_COUNTRYSIDE);
  for(int i = 0; i < 7; ++i)
  {
    x  = random_range(WIDTH);
    x1 = random_range(WIDTH);
    y  = 0;
    y1 = LENGTH;
    straggle_corridor(x, y, x1, y1, WATER, RS_COUNTRYSIDE);
  }
}

void make_swamp()
{
  make_general_map("~~\".");
}

// tactical map generating functions
void make_country_screen(chtype terrain)
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
  Level->environment = E_TACTICAL_MAP;
  Level->generated   = true;
  switch(terrain)
  {
    case FOREST:
      make_forest();
      break;
    case JUNGLE:
      make_jungle();
      break;
    case SWAMP:
      make_swamp();
      break;
    case RIVER:
      make_river();
      break;
    case MOUNTAINS:
    case PASS:
      make_mountains();
      break;
    case ROAD:
      make_road();
      break;
    default:
      make_plains();
      break;
  }
  if(nighttime())
  {
    queue_message("Night's gloom shrouds your sight.");
    for(int x = 0; x < WIDTH; ++x)
    {
      for(int y = 0; y < LENGTH; ++y)
      {
        Level->site[x][y].showchar = SPACE;
        Level->site[x][y].lstatus  = 0;
      }
    }
  }
}

// goes from f to t unless it hits a site which is not a wall and doesn't have buildaux field == baux
void room_corridor(int fx, int fy, int tx, int ty, int baux)
{
  int dx = sign(tx - fx);
  int dy = sign(ty - fy);

  makedoor(fx, fy);

  fx += dx;
  fy += dy;

  bool continuing = true;
  while(continuing)
  {
    Level->site[fx][fy].locchar    = FLOOR;
    Level->site[fx][fy].roomnumber = RS_CORRIDOR;
    Level->site[fx][fy].buildaux   = baux;
    dx                             = sign(tx - fx);
    dy                             = sign(ty - fy);
    if((dx != 0) && (dy != 0))
    {
      if(random_range(2))
      {
        dx = 0;
      }
      else if(random_range(2))
      {
        dy = 0;
      }
    }
    fx += dx;
    fy += dy;
    continuing =
      (((fx != tx) || (fy != ty)) &&
       ((Level->site[fx][fy].buildaux == 0) || (Level->site[fx][fy].buildaux == baux)));
  }
  makedoor(fx, fy);
}

// builds a room. Then, for each successive room, sends off at least one
// corridor which is guaranteed to connect up to another room, thus guaranteeing
// fully connected level.

void room_level()
{
  int fx, fy, tx, ty, t, l, e;
  char rsi;

  Level->numrooms = random_range(8) + 9;

  do
  {
    t = random_range(LENGTH - 10) + 1;
    l = random_range(WIDTH - 10) + 1;
    e = 4 + random_range(5);
  } while((Level->site[l][t].roomnumber != RS_WALLSPACE) ||
          (Level->site[l + e][t].roomnumber != RS_WALLSPACE) ||
          (Level->site[l][t + e].roomnumber != RS_WALLSPACE) ||
          (Level->site[l + e][t + e].roomnumber != RS_WALLSPACE));
  if(Current_Dungeon == E_SEWERS)
  {
    if(random_range(2))
    {
      rsi = ROOMBASE + 25;
    }
    else
    {
      rsi = ROOMBASE + random_range(NUMROOMNAMES);
    }
  }
  else
  {
    rsi = ROOMBASE + random_range(NUMROOMNAMES);
  }
  build_room(l, t, e, rsi, 1);

  for(int i = 2; i <= Level->numrooms; ++i)
  {
    do
    {
      t = random_range(LENGTH - 10) + 1;
      l = random_range(WIDTH - 10) + 1;
      e = 4 + random_range(5);
    } while((Level->site[l][t].roomnumber != RS_WALLSPACE) ||
            (Level->site[l + e][t].roomnumber != RS_WALLSPACE) ||
            (Level->site[l][t + e].roomnumber != RS_WALLSPACE) ||
            (Level->site[l + e][t + e].roomnumber != RS_WALLSPACE));
    if(Current_Dungeon == E_SEWERS)
    {
      if(random_range(2))
      {
        rsi = ROOMBASE + 25;
      }
      else
      {
        rsi = ROOMBASE + random_range(NUMROOMNAMES);
      }
    }
    else
    {
      rsi = ROOMBASE + random_range(NUMROOMNAMES);
    }
    build_room(l, t, e, rsi, i);

    // corridor which is guaranteed to connect
    if(!findspace(&tx, &ty, i))
    {
      continue;
    }

    // figure out where to start corridor from
    if((ty <= t) && (tx <= l + e))
    {
      fx = l + 1 + random_range(e - 1);
      fy = t;
    }
    else if((tx >= l + e) && (ty <= t + e))
    {
      fx = l + e;
      fy = t + 1 + random_range(e - 1);
    }
    else if((ty >= t + e) && (tx >= l))
    {
      fx = l + 1 + random_range(e - 1);
      fy = t + e;
    }
    else
    {
      fx = l;
      fy = t + 1 + random_range(e - 1);
    }

    room_corridor(fx, fy, tx, ty, i);

    // corridor which may not go anywhere
    if(random_range(2))
    {
      if(!findspace(&tx, &ty, i))
      {
        continue;
      }
      if((ty <= t) && (tx <= l + e))
      {
        fx = l + 1 + random_range(e - 1);
        fy = t;
      }
      else if((tx >= l + e) && (ty <= t + e))
      {
        fx = l + e;
        fy = t + 1 + random_range(e - 1);
      }
      else if((ty >= t + e) && (tx >= l))
      {
        fx = l + 1 + random_range(e - 1);
        fy = t + e;
      }
      else
      {
        fx = l;
        fy = t + 1 + random_range(e - 1);
      }
      room_corridor(fx, fy, tx, ty, i);
    }
  }

  if(Current_Dungeon == E_SEWERS)
  {
    if(Level->depth == SEWERLEVELS)
    {
      findspace(&tx, &ty, -1);
      std::unique_ptr<monster> m   = make_creature(GREAT_WYRM);
      Level->site[tx][ty].creature = m.get();
      m->x                         = tx;
      m->y                         = ty;
      Level->mlist.emplace_front(std::move(m));
    }
  }
  else if(Current_Environment == E_CASTLE)
  {
    if(Level->depth == CASTLELEVELS)
    {
      findspace(&tx, &ty, -1);
      Level->site[tx][ty].locchar = STAIRS_DOWN;
      Level->site[tx][ty].p_locf  = L_ENTER_COURT;
    }
  }
  else if(Current_Environment == E_VOLCANO)
  {
    if(Level->depth == VOLCANOLEVELS && !gamestatusp(COMPLETED_VOLCANO, GameStatus))
    {
      findspace(&tx, &ty, -1);
      std::unique_ptr<monster> m   = make_creature(DEMON_EMP);
      Level->site[tx][ty].creature = m.get();
      m->x                         = tx;
      m->y                         = ty;
      Level->mlist.emplace_front(std::move(m));
    }
  }
}

void maze_level()
{
  int tx, ty, mid;
  char rsi;
  if(Current_Environment == E_ASTRAL)
  {
    switch(Level->depth)
    {
      case 1:
        rsi = RS_EARTHPLANE;
        break;
      case 2:
        rsi = RS_AIRPLANE;
        break;
      case 3:
        rsi = RS_WATERPLANE;
        break;
      case 4:
        rsi = RS_FIREPLANE;
        break;
      default:
        rsi = RS_HIGHASTRAL;
    }
  }
  else
  {
    rsi = RS_VOLCANO;
  }
  maze_corridor(
    random_range(WIDTH - 1) + 1, random_range(LENGTH - 1) + 1, random_range(WIDTH - 1) + 1,
    random_range(LENGTH - 1) + 1, rsi, 0
  );
  if(Current_Dungeon == E_ASTRAL)
  {
    for(int x = 0; x < WIDTH; ++x)
    {
      for(int y = 0; y < LENGTH; ++y)
      {
        if(Level->site[x][y].locchar == WALL)
        {
          switch(Level->depth)
          {
            case 1:
              Level->site[x][y].aux = 500;
              break;
            case 2:
              Level->site[x][y].locchar = WHIRLWIND;
              Level->site[x][y].p_locf  = L_WHIRLWIND;
              break;
            case 3:
              Level->site[x][y].locchar = WATER;
              Level->site[x][y].p_locf  = L_WATER;
              break;
            case 4:
              Level->site[x][y].locchar = FIRE;
              Level->site[x][y].p_locf  = L_FIRE;
              break;
            case 5:
              Level->site[x][y].locchar = ABYSS;
              Level->site[x][y].p_locf  = L_ABYSS;
              break;
          }
        }
      }
    }
    switch(Level->depth)
    {
      case 1:
        mid = LORD_EARTH;
        break; // Elemental Lord of Earth
      case 2:
        mid = LORD_AIR;
        break; // Elemental Lord of Air
      case 3:
        mid = LORD_WATER;
        break; // Elemental Lord of Water
      case 4:
        mid = LORD_FIRE;
        break; // Elemental Lord of Fire
      default:
        mid = ELEM_MASTER; // Elemental Master
    }
    if(Level->depth == 5)
    {
      findspace(&tx, &ty, -1);
      Level->site[tx][ty].p_locf  = L_ENTER_CIRCLE;
      Level->site[tx][ty].locchar = STAIRS_DOWN;
    }
    if(!gamestatusp(COMPLETED_ASTRAL, GameStatus))
    {
      findspace(&tx, &ty, -1);
      std::unique_ptr<monster> m   = make_creature(mid);
      Level->site[tx][ty].creature = m.get();
      m->x                         = tx;
      m->y                         = ty;
      Level->mlist.emplace_front(std::move(m));
    }
  }
  else if(Current_Environment == E_VOLCANO)
  {
    if(Level->depth == VOLCANOLEVELS && !gamestatusp(COMPLETED_VOLCANO, GameStatus))
    {
      findspace(&tx, &ty, -1);
      std::unique_ptr<monster> m   = make_creature(DEMON_EMP);
      Level->site[tx][ty].creature = m.get();
      m->x                         = tx;
      m->y                         = ty;
      Level->mlist.emplace_front(std::move(m));
    }
  }
}

// keep drawing corridors recursively for 2^5 endpoints
void maze_corridor(int fx, int fy, int tx, int ty, char rsi, char num)
{
  if(num < 6)
  {
    straggle_corridor(fx, fy, tx, ty, FLOOR, rsi);
    maze_corridor(tx, ty, random_range(WIDTH - 1) + 1, random_range(LENGTH - 1) + 1, rsi, num + 1);
  }
}
