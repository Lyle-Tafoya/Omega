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

/* time.c */

/* this file deals with the passage of time in omega */

#include "glob.h"
#include "scr.h"

// remedies occasional defective monsters
void fix_phantom(monster *m)
{
  if(!Level->site[m->x][m->y].creature)
  {
    queue_message("You hear a sound like a sigh of relief....");
    Level->site[m->x][m->y].creature = m;
  }
  else
  {
    queue_message("You hear a puff of displaced air....");
    findspace(&(m->x), &(m->y), -1);
    Level->site[m->x][m->y].creature = m;
    m_death(m);
  }
}

// This function coordinates monsters and player actions, as well as
// random events. Each tick is a second. There are therefore 60 ticks to
// the minute and 60 minutes to the hour.
void time_clock(int reset)
{
  int env;

  if(++Tick > 60)
  {
    Tick = 0;
    minute_status_check(); /* see about some player statuses each minute */
    if(++Time % 10 == 0)
    {
      tenminute_check();
    }
  }

  if(reset)
  {
    Tick = (Player.click = 0);
  }

  env = Current_Environment;
  while((Tick == Player.click) && (Current_Environment != E_COUNTRYSIDE) && Current_Environment == env)
  {
    if(!gamestatusp(SKIP_PLAYER, GameStatus))
    {
      do
      {
        resetgamestatus(SKIP_MONSTERS, GameStatus);
        if((!Player.status[SLEPT]) && (Current_Environment != E_COUNTRYSIDE))
        {
          p_process();
        }
      } while(gamestatusp(SKIP_MONSTERS, GameStatus) && (Current_Environment != E_COUNTRYSIDE));
    }
    else
    {
      resetgamestatus(SKIP_PLAYER, GameStatus);
    }
    Player.click = (Player.click + Command_Duration) % 60;
  }

  /* klugy but what the heck. w/o this line, if the player caused
  a change-environment to the country, the monsters on the old Level
  will still act, causing all kinds of anomalies and core dumps,
  intermittently. However, any other environment change will reset
  Level appropriately, so only have to check for countryside */

  if(Current_Environment != E_COUNTRYSIDE)
  {
    for(auto prev = Level->mlist.before_begin(); std::next(prev) != Level->mlist.end();)
    {
      auto it = std::next(prev);
      if((*it)->hp > 0)
      {
        // following is a hack until I discover source of phantom monsters
        if(Level->site[(*it)->x][(*it)->y].creature != *it)
        {
          fix_phantom(*it);
        }
        if(Tick == (*it)->click)
        {
          (*it)->click += (*it)->speed;
          while((*it)->click > 60)
          {
            (*it)->click -= 60;
          }
          m_pulse(*it);
        }
        ++prev;
      }
      else if(*it != Arena_Monster)
      {
        delete(*it);
        Level->mlist.erase_after(prev);
      }
      else
      {
        ++prev;
      }
    }
  }
}
