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

/* mmove.c */
/* monster move functions */

#include "glob.h"
#include "scr.h"

#include <algorithm>
#include <format>

/* like m_normal_move, but can open doors */
void m_smart_move(monster *m)
{
  m_simple_move(m);
}

/* not very smart, but not altogether stupid movement */
void m_normal_move(monster *m)
{
  m_simple_move(m);
}

/* used by both m_normal_move and m_smart_move */
void m_simple_move(monster *m)
{
  int dx = sign(Player.x - m->x);
  int dy = sign(Player.y - m->y);

  erase_monster(m);
  if(m->hp < Monsters[m->id].hp / 4)
  {
    dx       = -dx;
    dy       = -dy;
    m->movef = M_MOVE_SCAREDY;
    std::string monster_name;
    if(m->uniqueness == COMMON)
    {
      monster_name = std::format("The {}", m->monstring);
    }
    else
    {
      monster_name = m->monstring;
    }
    if(!m->possessions.empty())
    {
      queue_message(std::format("{} drops it's treasure and flees!", monster_name));
      m_dropstuff(m);
    }
    else
    {
      queue_message(std::format("{} flees!", monster_name));
    }
    m->speed = std::min(2, m->speed - 1);
  }
  if((!m_statusp(*m, HOSTILE) && !m_statusp(*m, NEEDY)) || (Player.status[INVISIBLE] > 0))
  {
    m_random_move(m);
  }
  else
  {
    if(m_unblocked(m, m->x + dx, m->y + dy))
    {
      movemonster(m, m->x + dx, m->y + dy);
    }
    else if(dx == 0)
    {
      if(m_unblocked(m, m->x + 1, m->y + dy))
      {
        movemonster(m, m->x + 1, m->y + dy);
      }
      else if(m_unblocked(m, m->x - 1, m->y + dy))
      {
        movemonster(m, m->x - 1, m->y + dy);
      }
    }

    else if(dy == 0)
    {
      if(m_unblocked(m, m->x + dx, m->y + 1))
      {
        movemonster(m, m->x + dx, m->y + 1);
      }
      else if(m_unblocked(m, m->x + dx, m->y - 1))
      {
        movemonster(m, m->x + dx, m->y - 1);
      }
    }

    else
    {
      if(m_unblocked(m, m->x + dx, m->y))
      {
        movemonster(m, m->x + dx, m->y);
      }
      else if(m_unblocked(m, m->x, m->y + dy))
      {
        movemonster(m, m->x, m->y + dy);
      }
    }
  }
}

void m_move_animal(monster *m)
{
  if(m_statusp(*m, HOSTILE))
  {
    m_normal_move(m);
  }
  else
  {
    m_scaredy_move(m);
  }
}

/* same as simple move except run in opposite direction */
void m_scaredy_move(monster *m)
{
  int dx = -sign(Player.x - m->x);
  int dy = -sign(Player.y - m->y);
  erase_monster(m);
  if(Player.status[INVISIBLE])
  {
    m_random_move(m);
  }
  else
  {
    if(m_unblocked(m, m->x + dx, m->y + dy))
    {
      movemonster(m, m->x + dx, m->y + dy);
    }
    else if(dx == 0)
    {
      if(m_unblocked(m, m->x + 1, m->y + dy))
      {
        movemonster(m, m->x + 1, m->y + dy);
      }
      else if(m_unblocked(m, m->x - 1, m->y + dy))
      {
        movemonster(m, m->x - 1, m->y + dy);
      }
    }

    else if(dy == 0)
    {
      if(m_unblocked(m, m->x + dx, m->y + 1))
      {
        movemonster(m, m->x + dx, m->y + 1);
      }
      else if(m_unblocked(m, m->x + dx, m->y - 1))
      {
        movemonster(m, m->x + dx, m->y - 1);
      }
    }

    else
    {
      if(m_unblocked(m, m->x + dx, m->y))
      {
        movemonster(m, m->x + dx, m->y);
      }
      else if(m_unblocked(m, m->x, m->y + dy))
      {
        movemonster(m, m->x, m->y + dy);
      }
    }
  }
}

/* for spirits (and earth creatures) who can ignore blockages because
   either they are noncorporeal or they can move through stone */
void m_spirit_move(monster *m)
{
  int dx = sign(Player.x - m->x);
  int dy = sign(Player.y - m->y);
  erase_monster(m);
  if(m->hp < Monsters[m->id].hp / 6)
  {
    dx = -dx;
    dy = -dy;
  }

  if(Player.status[INVISIBLE] > 0 || !m_unblocked(m, m->x + dx, m->y + dy))
  {
    m_random_move(m);
  }
  else
  {
    movemonster(m, m->x + dx, m->y + dy);
  }
}

// fluttery dumb movement
void m_flutter_move(monster *m)
{
  int trange, range = distance(m->x, m->y, Player.x, Player.y);
  int tx, ty, nx = m->x, ny = m->y;
  erase_monster(m);
  if(Player.status[INVISIBLE] > 0)
  {
    m_random_move(m);
  }
  else
  {
    for(int i = 0; i < 8; ++i)
    {
      tx     = m->x + Dirs[0][i];
      ty     = m->y + Dirs[1][i];
      trange = distance(tx, ty, Player.x, Player.y);
      if(m->hp < Monsters[m->id].hp / 6)
      {
        if((trange > range) && m_unblocked(m, tx, ty))
        {
          range = trange;
          nx    = tx;
          ny    = ty;
        }
      }
      else if((trange <= range) && m_unblocked(m, tx, ty))
      {
        range = trange;
        nx    = tx;
        ny    = ty;
      }
    }
    movemonster(m, nx, ny);
  }
}

void m_follow_move(monster *m)
{
  if(!m_statusp(*m, HOSTILE))
  {
    m_normal_move(m);
  }
  else
  {
    m_scaredy_move(m);
  }
}

// allows monsters to fall into pools, revealed traps, etc
void m_confused_move(monster *m)
{
  erase_monster(m);
  bool done = false;
  for(int i = 0; i < 8 && !done; ++i)
  {
    int nx = m->x + random_range(3) - 1;
    int ny = m->y + random_range(3) - 1;
    if(unblocked(nx, ny) && ((nx != Player.x) || (ny != Player.y)))
    {
      done = true;
      movemonster(m, nx, ny);
    }
  }
}

void m_random_move(monster *m)
{
  erase_monster(m);
  bool done = false;
  for(int i = 0; i < 8 && !done; ++i)
  {
    int nx = m->x + random_range(3) - 1;
    int ny = m->y + random_range(3) - 1;
    if(m_unblocked(m, nx, ny) && ((nx != Player.x) || (ny != Player.y)))
    {
      done = true;
      movemonster(m, nx, ny);
    }
  }
}

// monster removed from play
void m_vanish(monster *m)
{
  if(m->uniqueness == COMMON)
  {
    queue_message(std::format("The {} vanishes in the twinkling of an eye!", m->monstring));
  }
  else
  {
    queue_message(std::format("{} vanishes in the twinkling of an eye!", m->monstring));
  }
  Level->site[m->x][m->y].creature = nullptr;
  erase_monster(m);
  m->hp = -1; // signals "death" -- no credit to player, though
}

// monster still in play
void m_teleport(monster *m)
{
  erase_monster(m);
  if(m_statusp(*m, AWAKE))
  {
    Level->site[m->x][m->y].creature = nullptr;
    putspot(m->x, m->y, getspot(m->x, m->y, false));
    findspace(&(m->x), &(m->y), -1);
    Level->site[m->x][m->y].creature = m;
  }
}

void m_move_leash(monster *m)
{
  m_simple_move(m);
  if(m->aux1 == 0)
  {
    m->aux1 = m->x;
    m->aux2 = m->y;
  }
  else if(distance(m->x, m->y, m->aux1, m->aux2) > 5)
  {
    monster *creature = Level->site[m->aux1][m->aux2].creature;
    if(creature)
    {
      if(los_p(Player.x, Player.y, m->aux1, m->aux2))
      {
        /* some other monster is where the chain starts */
        if(creature->uniqueness == COMMON)
        {
          queue_message(std::format("The {} releases the dog's chain!", creature->monstring));
        }
        else
        {
          queue_message(std::format("{} releases the dog's chain!", creature->monstring));
        }
      }
      m->movef = M_MOVE_NORMAL;
      /* otherwise, we'd lose either the dog or the other monster. */
    }
    else if(los_p(Player.x, Player.y, m->x, m->y))
    {
      queue_message("You see the dog jerked back by its chain!");
      plotspot(m->x, m->y, false);
    }
    else
    {
      queue_message("You hear a strangled sort of yelp!");
    }
    Level->site[m->x][m->y].creature = nullptr;
    m->x                             = m->aux1;
    m->y                             = m->aux2;
    Level->site[m->x][m->y].creature = m;
  }
}
