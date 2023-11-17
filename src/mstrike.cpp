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

/* mstrike.c */
/* monster strike functions */

#include "glob.h"
#include "scr.h"

#include <format>

void m_firebolt(monster *m)
{
  fbolt(m->x, m->y, Player.x, Player.y, m->hit, m->dmg);
}

void m_nbolt(monster *m)
{
  nbolt(m->x, m->y, Player.x, Player.y, m->hit, m->dmg);
}

void m_lball(monster *m)
{
  lball(m->x, m->y, Player.x, Player.y, m->dmg);
}

void m_fireball(monster *m)
{
  fball(m->x, m->y, Player.x, Player.y, m->dmg);
}

void m_snowball(monster *m)
{
  snowball(m->x, m->y, Player.x, Player.y, m->dmg);
}

void m_blind_strike(monster *m)
{
  if((Player.status[BLINDED] == 0) && los_p(m->x, m->y, Player.x, Player.y) &&
     (distance(m->x, m->y, Player.x, Player.y) < 5))
  {
    if(m->uniqueness == COMMON)
    {
      queue_message(std::format("The {} gazes at you menacingly.", m->monstring));
    }
    else
    {
      queue_message(std::format("{} gazes at you menacingly.", m->monstring));
    }
    if(!p_immune(GAZE))
    {
      queue_message("You've been blinded!");
      Player.status[BLINDED] = random_range(4) + 1;
      for(monster *level_monster : Level->mlist)
      {
        plotspot(level_monster->x, level_monster->y, false);
      }
    }
    else
    {
      queue_message("You gaze steadily back....");
    }
  }
}

void m_strike_sonic(monster *m)
{
  if(m->uniqueness == COMMON)
  {
    queue_message(std::format("The {} screams at you!", m->monstring));
  }
  else
  {
    queue_message(std::format("{} screams at you!", m->monstring));
  }
  p_damage(m->dmg, OTHER_MAGIC, "a sonic blast");
}
