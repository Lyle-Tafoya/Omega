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

void m_firebolt(struct monster *m)
{
  fbolt(m->x, m->y, Player.x, Player.y, m->hit, m->dmg);
}

void m_nbolt(struct monster *m)
{
  nbolt(m->x, m->y, Player.x, Player.y, m->hit, m->dmg);
}

void m_lball(struct monster *m)
{
  lball(m->x, m->y, Player.x, Player.y, m->dmg);
}

void m_fireball(struct monster *m)
{
  fball(m->x, m->y, Player.x, Player.y, m->dmg);
}

void m_snowball(struct monster *m)
{
  snowball(m->x, m->y, Player.x, Player.y, m->dmg);
}

void m_blind_strike(struct monster *m)
{
  pml ml;
  if((Player.status[BLINDED] == 0) && los_p(m->x, m->y, Player.x, Player.y) &&
     (distance(m->x, m->y, Player.x, Player.y) < 5))
  {
    if(m->uniqueness == COMMON)
    {
      strcpy(Str2, "The ");
      strcat(Str2, m->monstring);
    }
    else
    {
      strcpy(Str2, m->monstring);
    }
    strcat(Str2, " gazes at you menacingly");
    mprint(Str2);
    if(!p_immune(GAZE))
    {
      mprint("You've been blinded!");
      Player.status[BLINDED] = random_range(4) + 1;
      for(ml = Level->mlist; ml != NULL; ml = ml->next)
      {
        plotspot(ml->m->x, ml->m->y, false);
      }
    }
    else
    {
      mprint("You gaze steadily back....");
    }
  }
}

void m_strike_sonic(struct monster *m)
{
  if(m->uniqueness == COMMON)
  {
    strcpy(Str2, "The ");
    strcat(Str2, m->monstring);
  }
  else
  {
    strcpy(Str2, m->monstring);
  }
  strcat(Str2, " screams at you!");
  mprint(Str2);
  p_damage(m->dmg, OTHER_MAGIC, "a sonic blast");
}