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

/* gen1.c */
/* level generator functions */

#include "glob.h"
#include "scr.h"

#include <algorithm>
#include <ctime>

#ifdef SAVE_LEVELS
extern struct level TheLevel;
void                kill_levels(const std::string &str);
plv                 msdos_changelevel(plv oldlevel, int newenv, int newdepth);
#endif

/* Deallocate current dungeon */
void free_dungeon()
{
#ifdef SAVE_LEVELS
  if(Dungeon)
  {
    kill_levels(std::format("om{}.*.lev", Dungeon->environment));
  }
#else
  plv tlv;

  while(Dungeon)
  {
    tlv     = Dungeon;
    Dungeon = Dungeon->next;
    free_level(tlv);
  }
#endif
}

/* erase the level w/o deallocating it*/
void clear_level(struct level *dungeon_level)
{
  int i, j;
  if(dungeon_level)
  {
    dungeon_level->generated    = false;
    dungeon_level->numrooms     = 0;
    dungeon_level->tunnelled    = 0;
    dungeon_level->depth        = 0;
    dungeon_level->mlist        = nullptr;
    dungeon_level->next         = nullptr;
    dungeon_level->last_visited = time(nullptr);
    for(i = 0; i < MAXWIDTH; i++)
    {
      for(j = 0; j < MAXLENGTH; j++)
      {
        dungeon_level->site[i][j].locchar    = WALL;
        dungeon_level->site[i][j].showchar   = SPACE;
        dungeon_level->site[i][j].creature   = nullptr;
        dungeon_level->site[i][j].things     = nullptr;
        dungeon_level->site[i][j].aux        = difficulty() * 20;
        dungeon_level->site[i][j].buildaux   = 0;
        dungeon_level->site[i][j].p_locf     = L_NO_OP;
        dungeon_level->site[i][j].lstatus    = 0;
        dungeon_level->site[i][j].roomnumber = RS_WALLSPACE;
      }
    }
  }
}

/* Looks for level tolevel in current dungeon which is named by
Dungeon, which may be nullptr. If the level is found, and rewrite_level
is false, and the level has already been generated, nothing happens
beyond Level being set correctly. Otherwise the level is recreated
from scratch */

void change_level(char fromlevel, char tolevel, char rewrite_level)
{
  struct level *thislevel = nullptr;
  Player.sx               = -1;
  Player.sy               = -1; /* sanctuary effect dispelled */
#ifdef SAVE_LEVELS
  thislevel = msdos_changelevel(Level, Current_Environment, tolevel);
#else
  thislevel = findlevel(Dungeon, tolevel);
#endif
  deepest[Current_Environment] = std::max(deepest[Current_Environment], static_cast<int>(tolevel));
  if(!thislevel)
  {
#ifdef SAVE_LEVELS
    thislevel = &TheLevel;
#else
    thislevel = new level;
#endif
    clear_level(thislevel);
    Level       = thislevel;
    Level->next = Dungeon;
    Dungeon     = Level;
  }
  Level = thislevel;
  if((!Level->generated) || rewrite_level)
  {
    initrand(Current_Environment, tolevel);
    Level->environment = Current_Environment;
    Level->depth       = tolevel;
    Level->generated   = true;
    switch(Current_Environment)
    {
      case E_CAVES:
        if((random_range(4) == 0) && (tolevel < MaxDungeonLevels))
        {
          room_level();
        }
        else
        {
          cavern_level();
        }
        break;
      case E_SEWERS:
        if((random_range(4) == 0) && (tolevel < MaxDungeonLevels))
        {
          room_level();
        }
        else
        {
          sewer_level();
        }
        break;
      case E_CASTLE:
        room_level();
        break;
      case E_ASTRAL:
        maze_level();
        break;
      case E_VOLCANO:
        switch(random_range(3))
        {
          case 0:
            cavern_level();
            break;
          case 1:
            room_level();
            break;
          case 2:
            maze_level();
            break;
        }
        break;
      default:
        queue_message("This dungeon not implemented!");
        break;
    }
    install_traps();
    install_specials();
    make_stairs(fromlevel);
    make_stairs(fromlevel);
    initrand(E_RESTORE, 0);
    populate_level(Current_Environment);
    stock_level();
  }
  find_stairs(fromlevel, tolevel);
  calculate_offsets(Player.x, Player.y);
  show_screen();
  screencheck(Player.x, Player.y);
  drawvision(Player.x, Player.y);
  /* synchronize with player on level change */
  Player.click = (Tick + 1) % 60;
  roomcheck();
}

#ifndef SAVE_LEVELS
/* tries to find the level of depth levelnum in dungeon; if can't find
   it returns nullptr */
plv findlevel(struct level *dungeon, char levelnum)
{
  if(!dungeon)
  {
    return nullptr;
  }
  else
  {
    while(dungeon->next && (dungeon->depth != levelnum))
    {
      dungeon = dungeon->next;
    }
    if(dungeon->depth == levelnum)
    {
      dungeon->last_visited = time(nullptr);
      return (dungeon);
    }
    else
    {
      return nullptr;
    }
  }
}
#endif

/* keep going in one orthogonal direction or another until we hit our */
/* destination */

void straggle_corridor(int fx, int fy, int tx, int ty, Symbol loc, char rsi)
{
  int dx, dy;
  while((fx != tx) || (fy != ty))
  {
    dx = tx - fx;
    dy = ty - fy;
    if(random_range(abs(dx) + abs(dy)) < abs(dx))
    {
      corridor_crawl(&fx, &fy, sign(dx), 0, random_range(abs(dx)) + 1, loc, rsi);
    }
    else
    {
      corridor_crawl(&fx, &fy, 0, sign(dy), random_range(abs(dy)) + 1, loc, rsi);
    }
  }
}

void makedoor(int x, int y)
{
  if(random_range(20) <= Level->depth / 10)
  {
    Level->site[x][y].locchar = FLOOR;
    lset(x, y, SECRET, *Level);
  }
  else if(random_range(20) <= Level->depth / 2)
  {
    Level->site[x][y].locchar = CLOSED_DOOR;
    if(random_range(20) <= Level->depth / 10)
    {
      lset(x, y, SECRET, *Level);
    }
    if(random_range(40) <= Level->depth)
    {
      Level->site[x][y].aux = LOCKED;
    }
    else
    {
      Level->site[x][y].aux = UNLOCKED;
    }
  }
  else
  {
    Level->site[x][y].locchar = OPEN_DOOR;
    Level->site[x][y].aux     = UNLOCKED;
  }
  if(!loc_statusp(x, y, SECRET, *Level))
  {
    lset(x, y + 1, STOPS, *Level);
    lset(x + 1, y, STOPS, *Level);
    lset(x - 1, y, STOPS, *Level);
    lset(x, y - 1, STOPS, *Level);
    lset(x, y, STOPS, *Level);
  }
  Level->site[x][y].p_locf = L_NO_OP;
  /* prevents water corridors from being instant death in sewers */
}

void corridor_crawl(int *fx, int *fy, int sx, int sy, int n, Symbol loc, char rsi)
{
  int i;
  for(i = 0; i < n; i++)
  {
    *fx += sx;
    *fy += sy;
    if((*fx < WIDTH) && (*fx > -1) && (*fy > -1) && (*fy < LENGTH))
    {
      Level->site[*fx][*fy].locchar = loc;
      if(Level->site[*fx][*fy].roomnumber == RS_WALLSPACE)
      {
        Level->site[*fx][*fy].roomnumber = rsi;
      }
      if(loc == WATER)
      {
        Level->site[*fx][*fy].p_locf = L_WATER;
      }
      else if(loc == FLOOR)
      {
        Level->site[*fx][*fy].p_locf = L_NO_OP;
      }
      else if(loc == RUBBLE)
      {
        Level->site[*fx][*fy].p_locf = L_RUBBLE;
      }
    }
  }
}

const std::string roomname(int index)
{
  switch(index)
  {
    case RS_ZORCH:
      return "A place zorched by powerful magic";
    case RS_COURT:
      return "The Court of the ArchMage";
    case RS_CIRCLE:
      return "Astral Demesne of the Circle of Sorcerors";
    case RS_MAGIC_ISLE:
      return "An island positively reeking of magic";
    case RS_STARPEAK:
      return "Near the oddly glowing peak of a mountain";
    case RS_VOLCANO:
      return "Deep within the bowels of the earth";
    case RS_HIGHASTRAL:
      return "The High Astral Plane";
    case RS_EARTHPLANE:
      return "The Plane of Earth";
    case RS_WATERPLANE:
      return "The Plane of Water";
    case RS_FIREPLANE:
      return "The Plane of Fire";
    case RS_AIRPLANE:
      return "The Plane of Air";
    case RS_KITCHEN:
      return "A kitchen";
    case RS_BATHROOM:
      return "A bathroom";
    case RS_BEDROOM:
      return "A bedroom";
    case RS_DININGROOM:
      return "A dining room";
    case RS_SECRETPASSAGE:
      return "A secret passage";
    case RS_CLOSET:
      return "A stuffy closet";
    case RS_ARENA:
      return "The Rampart Arena";
    case RS_DROWNED_SEWER:
      return "A water-filled sewer node";
    case RS_DRAINED_SEWER:
      return "An unused sewer node";
    case RS_SEWER_DUCT:
      return "A winding sewer duct";
    case RS_DESTINY:
      return "The Halls of Fate";
    case RS_DRUID:
      return "The Great Henge";
    case RS_HECATE:
      return "The Church of the Far Side";
    case RS_SET:
      return "The Temple of the Black Hand";
    case RS_ATHENA:
      return "The Parthenon";
    case RS_ODIN:
      return "The Shrine of the Noose";
    case RS_ADEPT:
      return "The Adept's Challenge";
    case RS_WYRM:
      return "The Sunken Cavern of the Great Wyrm";
    case RS_OCEAN:
      return "The Underground Ocean";
    case RS_PONDS:
      return "A series of subterranean pools and streams";
    case RS_DRAGONLORD:
      return "The Lair of the DragonLord";
    case RS_GOBLINKING:
      return "The Caves of the Goblins";
    case RS_CAVERN:
      return "A vast natural cavern";
    case RS_CORRIDOR:
      return "A dimly lit corridor";
    case RS_WALLSPACE:
      return "A niche hollowed out of the wall";
    case ROOMBASE + 0:
      return "An abandoned garderobe";
    case ROOMBASE + 1:
      return "A dungeon cell";
    case ROOMBASE + 2:
      return "A tiled chamber";
    case ROOMBASE + 3:
      return "A crystal cavern";
    case ROOMBASE + 4:
      return "Someone's bedroom";
    case ROOMBASE + 5:
      return "An old storeroom";
    case ROOMBASE + 6:
      return "A room with charred walls";
    case ROOMBASE + 7:
      return "A marble hall";
    case ROOMBASE + 8:
      return "An eerie cave";
    case ROOMBASE + 9:
      return "A ransacked treasure-chamber";
    case ROOMBASE + 10:
      return "A smoke-filled room";
    case ROOMBASE + 11:
      return "A well-appointed apartment";
    case ROOMBASE + 12:
      return "An antechamber";
    case ROOMBASE + 13:
      return "An unoccupied harem";
    case ROOMBASE + 14:
      return "A multi-purpose room";
    case ROOMBASE + 15:
      return "A room filled with stalactites";
    case ROOMBASE + 16:
      return "An underground greenhouse";
    case ROOMBASE + 17:
      return "A water closet";
    case ROOMBASE + 18:
      return "A study";
    case ROOMBASE + 19:
      return "A living room";
    case ROOMBASE + 20:
      return "A comfortable den";
    case ROOMBASE + 21:
      return "An abatoir";
    case ROOMBASE + 22:
      return "A boudoir";
    case ROOMBASE + 23:
      return "A star chamber";
    case ROOMBASE + 24:
      return "A manmade cavern";
    case ROOMBASE + 25:
      return "A sewer control room";
    case ROOMBASE + 26:
      return "A shrine to High Magic";
    case ROOMBASE + 27:
      return "A magic laboratory";
    case ROOMBASE + 28:
      return "A room with inscribed pentagram";
    case ROOMBASE + 29:
      return "A chamber with a blue crystal omega dais";
    default:
      return "A room of mystery and allure";
  }
}

/* puts the player on the first set of stairs from the apt level */
/* if can't find them, just drops player anywhere.... */
void find_stairs(char fromlevel, char tolevel)
{
  int    i, j, found = false;
  Symbol sitechar;
  if(fromlevel > tolevel)
  {
    sitechar = STAIRS_DOWN;
  }
  else
  {
    sitechar = STAIRS_UP;
  }
  for(i = 0; i < WIDTH; i++)
  {
    for(j = 0; j < LENGTH; j++)
    {
      if((Level->site[i][j].locchar == sitechar) && (!found))
      {
        found    = true;
        Player.x = i;
        Player.y = j;
        break;
      }
    }
  }
  if(!found)
  {
    findspace(&Player.x, &Player.y, -1);
    if(Level->environment != E_ASTRAL)
    {
      Level->site[Player.x][Player.y].locchar = sitechar;
      lset(Player.x, Player.y, CHANGED, *Level);
    }
  }
}

void install_traps()
{
  int i, j;
  for(i = 0; i < WIDTH; i++)
  {
    for(j = 0; j < LENGTH; j++)
    {
      if((Level->site[i][j].locchar == FLOOR) && (Level->site[i][j].p_locf == L_NO_OP) &&
         random_range(500) <= ((int)(Level->depth / 6)))
      {
        Level->site[i][j].p_locf = TRAP_BASE + random_range(NUMTRAPS);
      }
    }
  }
}

/* x, y, is top left corner, l is length of side, rsi is room string index */
/* baux is so all rooms will have a key field. */
void build_square_room(int x, int y, int l, char rsi, int baux)
{
  int i, j;

  for(i = x; i <= x + l; i++)
  {
    for(j = y; j <= y + l; j++)
    {
      Level->site[i][j].roomnumber = rsi;
      Level->site[i][j].buildaux   = baux;
    }
  }
  for(i = x + 1; i < x + l; i++)
  {
    for(j = y + 1; j < y + l; j++)
    {
      Level->site[i][j].locchar = FLOOR;
      Level->site[i][j].p_locf  = L_NO_OP;
    }
  }
}

void build_room(int x, int y, int l, char rsi, int baux)
{
  build_square_room(x, y, l, rsi, baux);
}

void cavern_level()
{
  int  i, fx, fy, tx, ty, t, l, e;
  char rsi;

  Level->numrooms = 1;

  if((Current_Dungeon == E_CAVES) && (Level->depth == CAVELEVELS))
  {
    rsi = RS_GOBLINKING;
  }
  else
  {
    rsi = RS_CAVERN;
  }
  t = random_range(LENGTH / 2);
  l = random_range(WIDTH / 2);
  e = random_range(WIDTH / 8) + WIDTH / 8;
  build_square_room(t, l, e, rsi, 0);

  for(i = 0; i < 16; i++)
  {
    findspace(&tx, &ty, -1);
    fx = random_range(WIDTH - 2) + 1;
    fy = random_range(LENGTH - 2) + 1;
    straggle_corridor(fx, fy, tx, ty, FLOOR, RS_CORRIDOR);
  }
  while(random_range(3) == 1)
  {
    findspace(&tx, &ty, -1);
    fx = random_range(WIDTH - 2) + 1;
    fy = random_range(LENGTH - 2) + 1;
    straggle_corridor(fx, fy, tx, ty, WATER, RS_PONDS);
  }
  if(Current_Dungeon == E_CAVES)
  {
    if((Level->depth == CAVELEVELS) && (!gamestatusp(COMPLETED_CAVES, GameStatus)))
    {
      findspace(&tx, &ty, -1);
      Level->mlist       = new monsterlist;
      Level->mlist->next = nullptr;
      Level->mlist->m    = Level->site[tx][ty].creature =
        ((pmt)make_creature(GOBLIN_KING)); /* goblin king */
      Level->mlist->m->x = tx;
      Level->mlist->m->y = ty;
    }
  }
  else if(Current_Environment == E_VOLCANO)
  {
    if(Level->depth == VOLCANOLEVELS)
    {
      findspace(&tx, &ty, -1);
      Level->mlist       = new monsterlist;
      Level->mlist->next = nullptr;
      Level->mlist->m    = Level->site[tx][ty].creature =
        ((pmt)make_creature(DEMON_EMP)); /* The dark emp */
      Level->mlist->m->x = tx;
      Level->mlist->m->y = ty;
    }
  }
}

void sewer_level()
{
  int    i, tx, ty, t, l, e;
  char   rsi;
  Symbol lchar;

  Level->numrooms = random_range(3) + 3;
  rsi             = RS_DRAINED_SEWER;
  for(i = 0; i < Level->numrooms; i++)
  {
    do
    {
      t = random_range(LENGTH - 10) + 1;
      l = random_range(WIDTH - 10) + 1;
      e = 4;
    } while((Level->site[l][t].roomnumber == rsi) || (Level->site[l + e][t].roomnumber == rsi) ||
            (Level->site[l][t + e].roomnumber == rsi) || (Level->site[l + e][t + e].roomnumber == rsi));
    if(random_range(5))
    {
      lchar = FLOOR;
      rsi   = RS_DRAINED_SEWER;
    }
    else
    {
      lchar = WATER;
      rsi   = RS_DROWNED_SEWER;
    }
    build_room(l, t, e, rsi, i);
    sewer_corridor(l, t, -1, -1, lchar);
    sewer_corridor(l + e, t, 1, -1, lchar);
    sewer_corridor(l, t + e, -1, 1, lchar);
    sewer_corridor(l + e, t + e, 1, 1, lchar);
  }
  if(Current_Dungeon == E_SEWERS)
  {
    if((Level->depth == SEWERLEVELS) && (!gamestatusp(COMPLETED_SEWERS, GameStatus)))
    {
      findspace(&tx, &ty, -1);
      Level->mlist       = new monsterlist;
      Level->mlist->next = nullptr;
      Level->mlist->m    = Level->site[tx][ty].creature =
        ((pmt)make_creature(GREAT_WYRM)); /* The Great Wyrm */
      Level->mlist->m->x = tx;
      Level->mlist->m->y = ty;
    }
  }
}

void sewer_corridor(int x, int y, int dx, int dy, Symbol locchar)
{
  int continuing = true;
  makedoor(x, y);
  x += dx;
  y += dy;
  while(continuing)
  {
    Level->site[x][y].locchar = locchar;
    if(locchar == WATER)
    {
      Level->site[x][y].p_locf = L_WATER;
    }
    else
    {
      Level->site[x][y].p_locf = L_NO_OP;
    }
    Level->site[x][y].roomnumber = RS_SEWER_DUCT;
    x += dx;
    y += dy;
    if(locchar == WATER)
    {
      continuing = (inbounds(x, y) &&
                    ((Level->site[x][y].locchar == WALL) || (Level->site[x][y].locchar == WATER)));
    }
    else
    {
      continuing = (inbounds(x, y) && ((Level->site[x][y].roomnumber == RS_WALLSPACE) ||
                                       (Level->site[x][y].roomnumber == RS_SEWER_DUCT)));
    }
  }
  if(inbounds(x, y))
  {
    makedoor(x, y);
  }
}

void install_specials()
{
  int i, j, x, y;

  for(x = 0; x < WIDTH; x++)
  {
    for(y = 0; y < LENGTH; y++)
    {
      if((Level->site[x][y].locchar == FLOOR) && (Level->site[x][y].p_locf == L_NO_OP) &&
         (random_range(300) < difficulty()))
      {
        i = random_range(100);
        if(i < 10)
        {
          Level->site[x][y].locchar = ALTAR;
          Level->site[x][y].p_locf  = L_ALTAR;
          Level->site[x][y].aux     = random_range(10);
        }
        else if(i < 20)
        {
          Level->site[x][y].locchar = WATER;
          Level->site[x][y].p_locf  = L_MAGIC_POOL;
        }
        else if(i < 35)
        {
          Level->site[x][y].locchar = RUBBLE;
          Level->site[x][y].p_locf  = L_RUBBLE;
        }
        else if(i < 40)
        {
          Level->site[x][y].locchar = LAVA;
          Level->site[x][y].p_locf  = L_LAVA;
        }
        else if(i < 45)
        {
          Level->site[x][y].locchar = FIRE;
          Level->site[x][y].p_locf  = L_FIRE;
        }
        else if((i < 50) && (Current_Environment != E_ASTRAL))
        {
          Level->site[x][y].locchar = LIFT;
          Level->site[x][y].p_locf  = L_LIFT;
        }
        else if((i < 55) && (Current_Environment != E_VOLCANO))
        {
          Level->site[x][y].locchar = HEDGE;
          Level->site[x][y].p_locf  = L_HEDGE;
        }
        else if(i < 57)
        {
          Level->site[x][y].locchar = HEDGE;
          Level->site[x][y].p_locf  = L_TRIFID;
        }
        else if(i < 70)
        {
          Level->site[x][y].locchar = STATUE;
          if(random_range(100) < difficulty())
          {
            for(j = 0; j < 8; j++)
            {
              if(Level->site[x + Dirs[0][j]][y + Dirs[1][j]].p_locf != L_NO_OP)
              {
                Level->site[x + Dirs[0][j]][y + Dirs[1][j]].locchar = FLOOR;
              }
              Level->site[x + Dirs[0][j]][y + Dirs[1][j]].p_locf = L_STATUE_WAKE;
            }
          }
        }
        else
        {
          if(Current_Environment == E_VOLCANO)
          {
            Level->site[x][y].locchar = LAVA;
            Level->site[x][y].p_locf  = L_LAVA;
          }
          else if(Current_Environment == E_ASTRAL)
          {
            if(Level->depth == 1)
            {
              Level->site[x][y].locchar = RUBBLE;
              Level->site[x][y].p_locf  = L_RUBBLE;
            }
            else if(Level->depth == 2)
            {
              Level->site[x][y].locchar = FIRE;
              Level->site[x][y].p_locf  = L_FIRE;
            }
            else if(Level->depth == 3)
            {
              Level->site[x][y].locchar = WATER;
              Level->site[x][y].p_locf  = L_WATER;
            }
            else if(Level->depth == 4)
            {
              Level->site[x][y].locchar = ABYSS;
              Level->site[x][y].p_locf  = L_ABYSS;
            }
          }
          else
          {
            Level->site[x][y].locchar = WATER;
            Level->site[x][y].p_locf  = L_WATER;
          }
        }
      }
    }
  }
}
