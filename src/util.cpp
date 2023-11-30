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

// utils.cpp

// Random utility functions called from all over

#include "glob.h"
#include "scr.h"

#include <algorithm>
#include <chrono>
#include <random>
#include <string>
#include <thread>

std::mt19937 generator;

// x and y on level?
bool inbounds(int x, int y)
{
  return x >= 0 && y >= 0 && x < WIDTH && y < LENGTH;
}

int random_range(int k)
{
  int small, large;
  if(k > 0)
  {
    small = 0;
    large = k - 1;
  }
  else if(k < 0)
  {
    small = k + 1;
    large = 0;
  }
  else
  {
    return 0;
  }
  std::uniform_int_distribution<> distribution(small, large);
  return distribution(generator);
}

// modify absolute y coord relative to which part of level we are on
int screenmod(int y)
{
  return y - ScreenOffset;
}

int screenmod_horizontal(int x)
{
  return x - HorizontalOffset;
}

bool offscreen(int x, int y)
{
  return (y < 0 || y < ScreenOffset || y > ScreenOffset + ScreenLength - 1 || y > LENGTH) ||
         (x < 0 || x < HorizontalOffset || x > HorizontalOffset + ScreenWidth - 1 || x > WIDTH);
}

// always hit on a natural 0; never hit on a natural 19
bool hitp(int hit, int ac)
{
  int roll = random_range(20);
  if(roll == 0)
  {
    return true;
  }
  else if(roll == 19)
  {
    return false;
  }
  else
  {
    return roll < (hit - ac);
  }
}

// number of moves from x1,y1 to x2,y2
int distance(int x1, int y1, int x2, int y2)
{
  return std::max(abs(x2 - x1), abs(y2 - y1));
}

// can you shoot, or move monsters through a spot?
bool unblocked(int x, int y)
{
  if(!inbounds(x, y) || Level->site[x][y].creature || Level->site[x][y].locchar == WALL ||
     (Level->site[x][y].locchar == PORTCULLIS) || (Level->site[x][y].locchar == STATUE) ||
     (Level->site[x][y].locchar == HEDGE) || (Level->site[x][y].locchar == CLOSED_DOOR) ||
     loc_statusp(x, y, SECRET, *Level) || ((x == Player.x) && (y == Player.y)))
  {
    return false;
  }
  else
  {
    return true;
  }
}

// do monsters want to move through a spot
bool m_unblocked(const monster *m, int x, int y)
{
  if((!inbounds(x, y)) || ((x == Player.x) && (y == Player.y)))
  {
    return false;
  }
  else if(Level->site[x][y].creature || Level->site[x][y].locchar == SPACE)
  {
    return false;
  }
  else if(m_statusp(*m, ONLYSWIM))
  {
    return Level->site[x][y].locchar == WATER;
  }
  else if(loc_statusp(x, y, SECRET, *Level))
  {
    if(m->movef == M_MOVE_SMART)
    {
      if(los_p(x, y, Player.x, Player.y))
      {
        queue_message("You see a secret door swing open!");
        lreset(x, y, SECRET, *Level);
        lset(x, y, CHANGED, *Level);
      }
      else
      {
        queue_message("You hear a door creak open, and then close again.");
      }
      // smart monsters would close secret doors behind them if the
      // player didn't see them using it
      return true;
    }
    else
    {
      return m_statusp(*m, INTANGIBLE);
    }
  }
  else if((Level->site[x][y].locchar == FLOOR) || (Level->site[x][y].locchar == OPEN_DOOR))
  {
    return true;
  }
  else if((Level->site[x][y].locchar == PORTCULLIS) || (Level->site[x][y].locchar == WALL) ||
          (Level->site[x][y].locchar == STATUE))
  {
    return m_statusp(*m, INTANGIBLE);
  }
  else if(Level->site[x][y].locchar == WATER)
  {
    return m_statusp(*m, SWIMMING) || m_statusp(*m, ONLYSWIM) ||
      m_statusp(*m, INTANGIBLE) || m_statusp(*m, FLYING);
  }
  else if(Level->site[x][y].locchar == CLOSED_DOOR)
  {
    if(m->movef == M_MOVE_SMART)
    {
      queue_message("You hear a door creak open.");
      Level->site[x][y].locchar = OPEN_DOOR;
      lset(x, y, CHANGED, *Level);
      return true;
    }
    else if(random_range(m->dmg) > random_range(100) && Level->site[x][y].p_locf != L_ORACLE)
    {
      queue_message("You hear a door shattering.");
      Level->site[x][y].locchar = RUBBLE;
      lset(x, y, CHANGED, *Level);
      return true;
    }
    else
    {
      return m_statusp(*m, INTANGIBLE);
    }
  }
  else if(Level->site[x][y].locchar == LAVA)
  {
    return (m_immunityp(*m, FLAME) && m_statusp(*m, SWIMMING)) ||
      m_statusp(*m, INTANGIBLE) || m_statusp(*m, FLYING);
  }
  else if(Level->site[x][y].locchar == FIRE)
  {
    return m_statusp(*m, INTANGIBLE) || m_immunityp(*m, FLAME);
  }
  else if((Level->site[x][y].locchar == TRAP) || (Level->site[x][y].locchar == HEDGE) ||
          (Level->site[x][y].locchar == ABYSS))
  {
    return m->movef == M_MOVE_CONFUSED || m_statusp(*m, INTANGIBLE) || m_statusp(*m, FLYING);
  }
  else
  {
    return true;
  }
}

// can you see through a spot?
bool view_unblocked(int x, int y)
{
  if(!inbounds(x, y))
  {
    return false;
  }
  else if((Level->site[x][y].locchar == WALL) || (Level->site[x][y].locchar == STATUE) ||
          (Level->site[x][y].locchar == HEDGE) || (Level->site[x][y].locchar == FIRE) ||
          (Level->site[x][y].locchar == CLOSED_DOOR) || loc_statusp(x, y, SECRET, *Level))
  {
    return false;
  }
  else
  {
    return true;
  }
}

// 8 moves in Dirs
void initdirs()
{
  Dirs[0][0] = 1;
  Dirs[0][1] = 1;
  Dirs[0][2] = -1;
  Dirs[0][3] = -1;
  Dirs[0][4] = 1;
  Dirs[0][5] = -1;
  Dirs[0][6] = 0;
  Dirs[0][7] = 0;
  Dirs[0][8] = 0;
  Dirs[1][0] = 1;
  Dirs[1][1] = -1;
  Dirs[1][2] = 1;
  Dirs[1][3] = -1;
  Dirs[1][4] = 0;
  Dirs[1][5] = 0;
  Dirs[1][6] = 1;
  Dirs[1][7] = -1;
  Dirs[1][8] = 0;
}

// do_los moves pyx along a lineofsight from x1 to x2
// x1 and x2 are pointers because as a side effect they are changed
// to the final location of the pyx
void do_los(chtype pyx, int *x1, int *y1, int x2, int y2)
{
  int dx, dy, ox, oy;
  int major, minor;
  int error, delta, step;
  int blocked;

  if(x2 - *x1 < 0)
  {
    dx = 5;
  }
  else if(x2 - *x1 > 0)
  {
    dx = 4;
  }
  else
  {
    dx = -1;
  }
  if(y2 - *y1 < 0)
  {
    dy = 7;
  }
  else if(y2 - *y1 > 0)
  {
    dy = 6;
  }
  else
  {
    dy = -1;
  }
  if(abs(x2 - *x1) > abs(y2 - *y1))
  {
    major = dx;
    minor = dy;
    step  = abs(x2 - *x1);
    delta = 2 * abs(y2 - *y1);
  }
  else
  {
    major = dy;
    minor = dx;
    step  = abs(y2 - *y1);
    delta = 2 * abs(x2 - *x1);
  }
  if(major == -1)
  { // x1,y2 already == x2,y2
    return;
  }
  error = 0;
  do
  {
    ox = *x1;
    oy = *y1;
    *x1 += Dirs[0][major];
    *y1 += Dirs[1][major];
    error += delta;
    if(error > step)
    { // don't need to check that minor >= 0
      *x1 += Dirs[0][minor];
      *y1 += Dirs[1][minor];
      error -= 2 * step;
    }
    blocked = !unblocked(*x1, *y1);
    if(error < 0 && (*x1 != x2 || *y1 != y2) && blocked)
    {
      *x1 -= Dirs[0][minor];
      *y1 -= Dirs[1][minor];
      error += 2 * step;
      blocked = !unblocked(*x1, *y1);
    }
    Level->site[*x1][*y1].showchar = pyx;
    plotchar(pyx, *x1, *y1);
    plotspot(ox, oy, true);
    doupdate();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
  } while((*x1 != x2 || *y1 != y2) && !blocked);
  plotspot(*x1, *y1, true);
  levelrefresh();
}

// This is the same as do_los, except we stop before hitting nonliving obstructions
void do_object_los(chtype pyx, int *x1, int *y1, int x2, int y2)
{
  int dx, dy, ox, oy;
  int major, minor;
  int error, delta, step;
  int blocked;

  if(x2 - *x1 < 0)
  {
    dx = 5;
  }
  else if(x2 - *x1 > 0)
  {
    dx = 4;
  }
  else
  {
    dx = -1;
  }
  if(y2 - *y1 < 0)
  {
    dy = 7;
  }
  else if(y2 - *y1 > 0)
  {
    dy = 6;
  }
  else
  {
    dy = -1;
  }
  if(abs(x2 - *x1) > abs(y2 - *y1))
  {
    major = dx;
    minor = dy;
    step  = abs(x2 - *x1);
    delta = 2 * abs(y2 - *y1);
  }
  else
  {
    major = dy;
    minor = dx;
    step  = abs(y2 - *y1);
    delta = 2 * abs(x2 - *x1);
  }
  if(major == -1)
  { // x1,y2 already == x2,y2
    return;
  }
  error = 0;
  do
  {
    ox = *x1;
    oy = *y1;
    *x1 += Dirs[0][major];
    *y1 += Dirs[1][major];
    error += delta;
    if(error > step)
    { // don't need to check that minor >= 0
      *x1 += Dirs[0][minor];
      *y1 += Dirs[1][minor];
      error -= 2 * step;
    }
    blocked = !unblocked(*x1, *y1);
    if(error < 0 && (*x1 != x2 || *y1 != y2) && blocked)
    {
      *x1 -= Dirs[0][minor];
      *y1 -= Dirs[1][minor];
      error += 2 * step;
      blocked = !unblocked(*x1, *y1);
    }
    plotspot(ox, oy, true);
    if(unblocked(*x1, *y1))
    {
      plotchar(pyx, *x1, *y1);
      Level->site[*x1][*y1].showchar = pyx;
      doupdate();
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
  } while((*x1 != x2 || *y1 != y2) && !blocked);
  if(!Level->site[*x1][*y1].creature && blocked)
  {
    *x1 = ox;
    *y1 = oy;
  }
  plotspot(*x1, *y1, true);
  levelrefresh();
}

// los_p checks to see whether there is an unblocked los from x1,y1 to x2,y2
bool los_p(int x1, int y1, int x2, int y2)
{
  int dx, dy;
  int major, minor;
  int error, delta, step;
  int blocked;

  if(x2 - x1 < 0)
  {
    dx = 5;
  }
  else if(x2 - x1 > 0)
  {
    dx = 4;
  }
  else
  {
    dx = -1;
  }
  if(y2 - y1 < 0)
  {
    dy = 7;
  }
  else if(y2 - y1 > 0)
  {
    dy = 6;
  }
  else
  {
    dy = -1;
  }
  if(abs(x2 - x1) > abs(y2 - y1))
  {
    major = dx;
    minor = dy;
    step  = abs(x2 - x1);
    delta = 2 * abs(y2 - y1);
  }
  else
  {
    major = dy;
    minor = dx;
    step  = abs(y2 - y1);
    delta = 2 * abs(x2 - x1);
  }
  if(major == -1)
  { // x1,y2 already == x2,y2
    return true;
  }
  error = 0;
  do
  {
    x1 += Dirs[0][major];
    y1 += Dirs[1][major];
    error += delta;
    if(error > step)
    { // don't need to check that minor >= 0
      x1 += Dirs[0][minor];
      y1 += Dirs[1][minor];
      error -= 2 * step;
    }
    blocked = !unblocked(x1, y1);
    if(error < 0 && (x1 != x2 || y1 != y2) && blocked)
    {
      x1 -= Dirs[0][minor];
      y1 -= Dirs[1][minor];
      error += 2 * step;
      blocked = !unblocked(x1, y1);
    }
  } while((x1 != x2 || y1 != y2) && !blocked);
  return x1 == x2 && y1 == y2;
}

// view_los_p sees through monsters
bool view_los_p(int x1, int y1, int x2, int y2)
{
  int dx, dy;
  int major, minor;
  int error, delta, step;
  bool blocked;

  if(x2 - x1 < 0)
  {
    dx = 5;
  }
  else if(x2 - x1 > 0)
  {
    dx = 4;
  }
  else
  {
    dx = -1;
  }
  if(y2 - y1 < 0)
  {
    dy = 7;
  }
  else if(y2 - y1 > 0)
  {
    dy = 6;
  }
  else
  {
    dy = -1;
  }
  if(abs(x2 - x1) > abs(y2 - y1))
  {
    major = dx;
    minor = dy;
    step  = abs(x2 - x1);
    delta = 2 * abs(y2 - y1);
  }
  else
  {
    major = dy;
    minor = dx;
    step  = abs(y2 - y1);
    delta = 2 * abs(x2 - x1);
  }
  if(major == -1)
  { // x1,y2 already == x2,y2
    return true;
  }
  error = 0;
  do
  {
    x1 += Dirs[0][major];
    y1 += Dirs[1][major];
    error += delta;
    if(error > step)
    {
      x1 += Dirs[0][minor];
      y1 += Dirs[1][minor];
      error -= 2 * step;
    }
    blocked = !view_unblocked(x1, y1);
    if(error < 0 && (x1 != x2 || y1 != y2) && blocked)
    {
      x1 -= Dirs[0][minor];
      y1 -= Dirs[1][minor];
      error += 2 * step;
      blocked = !view_unblocked(x1, y1);
    }
  } while((x1 != x2 || y1 != y2) && !blocked);
  return x1 == x2 && y1 == y2;
}

long calc_points()
{
  long points = 0;

  if(gamestatusp(SPOKE_TO_DRUID, GameStatus))
  {
    points += 50;
  }
  if(gamestatusp(COMPLETED_CAVES, GameStatus))
  {
    points += 100;
  }
  if(gamestatusp(COMPLETED_SEWERS, GameStatus))
  {
    points += 200;
  }
  if(gamestatusp(COMPLETED_CASTLE, GameStatus))
  {
    points += 300;
  }
  if(gamestatusp(COMPLETED_ASTRAL, GameStatus))
  {
    points += 400;
  }
  if(gamestatusp(COMPLETED_VOLCANO, GameStatus))
  {
    points += 500;
  }
  if(gamestatusp(KILLED_DRAGONLORD, GameStatus))
  {
    points += 100;
  }
  if(gamestatusp(KILLED_EATER, GameStatus))
  {
    points += 100;
  }
  if(gamestatusp(KILLED_LAWBRINGER, GameStatus))
  {
    points += 100;
  }

  points += Player.xp / 50;

  points += Player.cash / 500;

  for(int i = 0; i < MAXITEMS; ++i)
  {
    if(Player.possessions[i])
    {
      points += Player.possessions[i]->level * (Player.possessions[i]->known + 1);
    }
  }
  for(std::unique_ptr<object> &item : Player.pack)
  {
    points += item->level * (item->known + 1);
  }

  for(int i = 0; i < NUMRANKS; ++i)
  {
    if(Player.rank[i] == 5)
    {
      points += 500;
    }
    else
    {
      points += 20 * Player.rank[i];
    }
  }

  if(Player.hp < 1)
  {
    points = (points / 2);
  }
  else if(Player.rank[ADEPT])
  {
    points *= 10;
  }

  return points;
}

// returns the 24 hour clock hour
int hour()
{
  return static_cast<int>(((Time + 720) / 60) % 24);
}

// returns 0, 10, 20, 30, 40, or 50
int showminute()
{
  return static_cast<int>(((Time % 60) / 10) * 10);
}

// returns the 12 hour clock hour
int showhour()
{
  int showtime;
  if((hour() == 0) || (hour() == 12))
  {
    showtime = 12;
  }
  else
  {
    showtime = (hour() % 12);
  }
  return showtime;
}

// nighttime is defined from 9 PM to 6AM
bool nighttime()
{
  return hour() > 20 || hour() < 7;
}

const std::string getarticle(const std::string &str)
{
  if((str[0] == 'a') || (str[0] == 'A') || (str[0] == 'e') || (str[0] == 'E') || (str[0] == 'i') || (str[0] == 'I') || (str[0] == 'o') || (str[0] == 'O') || (str[0] == 'u') || (str[0] == 'U') || (((str[0] == 'h') || (str[0] == 'H')) && ((str[1] == 'i') || (str[1] == 'e'))))
  {
    return "an ";
  }
  else
  {
    return "a ";
  }
}

int day()
{
  return (Date % 30) + 1;
}

const std::string ordinal(int number)
{
  if((number == 11) || (number == 12) || (number == 13))
  {
    return "th";
  }
  else
  {
    switch(number % 10)
    {
      case 1:
        return "st";
      case 2:
        return "nd";
      case 3:
        return "rd";
      default:
        return "th";
    }
  }
}

const std::string month()
{
  switch((Date % 360) / 30)
  {
    case 0:
      return "Freeze";
    case 1:
      return "Ice";
    case 2:
      return "Mud";
    case 3:
      return "Storm";
    case 4:
      return "Breeze";
    case 5:
      return "Light";
    case 6:
      return "Flame";
    case 7:
      return "Broil";
    case 8:
      return "Cool";
    case 9:
      return "Haunt";
    case 10:
      return "Chill";
    case 11:
      return "Dark";
    case 12:
      return "Twixt";
    default:
      return "***Error***";
  }
}

// WDT: code for the following two functions contributed by Sheldon Simms
// finds floor space on level with buildaux not equal to baux,
// sets x,y there. There must *be* floor space somewhere on level...
int spaceok(int x, int y, int baux)
{
  return Level->site[x][y].locchar == FLOOR && !Level->site[x][y].creature &&
    !loc_statusp(x, y, SECRET, *Level) && Level->site[x][y].buildaux != baux;
}

bool findspace(int *x, int *y, int baux)
{
  int i, j;
  bool tog = true, done = false;

  int attempts = 0;
  do
  {
    i = random_range(WIDTH);
    j = random_range(LENGTH);
    if(spaceok(i, j, baux))
    {
      done = true;
    }
    else
    {
      if(tog)
      {
        tog = !tog;
        while(1)
        {
          i++;
          if(i >= WIDTH)
          {
            break;
          }
          else if(spaceok(i, j, baux))
          {
            done = true;
            break;
          }
        }
      }
      else
      {
        tog = !tog;
        while(1)
        {
          j++;
          if(j >= LENGTH)
          {
            break;
          }
          else if(spaceok(i, j, baux))
          {
            done = true;
            break;
          }
        }
      }
    }
  } while(!done && ++attempts < 10000);
  *x = i;
  *y = j;

  return attempts < 10000;
}

bool confirmation()
{
  switch(random_range(4))
  {
    case 0:
      queue_message("Are you sure?");
      break;
    case 1:
      queue_message("Certain about that?");
      break;
    case 2:
      queue_message("Do you really mean it?");
      break;
    case 3:
      queue_message("Confirm that, would you?");
      break;
  }
  if(optionp(PARANOID_CONFIRM, Player))
  {
    queue_message("[yes] [no]");
    return msgscanstring() == "yes";
  }
  else
  {
    queue_message("[yn] (n)");
    return ynq() == 'y';
  }
}

void calc_weight()
{
  int weight = 0;

  for(int i = 1; i < MAXITEMS; ++i)
  {
    if(Player.possessions[i])
    {
      weight += Player.possessions[i]->weight * Player.possessions[i]->number;
    }
  }
  for(std::unique_ptr<object> &item : Player.pack)
  {
    weight += item->weight * item->number;
  }
  Player.itemweight = weight;
  dataprint();
}

// returns true if its ok to get rid of a level
bool ok_to_free(level *level)
{
  if(!level)
  {
    return false;
  }
  else
  {
    return level->environment != E_CITY && level->environment != E_VILLAGE &&
      level->environment != Current_Dungeon;
  }
}

// Free up monsters and items on a level
void free_level(level *level)
{
#ifndef SAVE_LEVELS
  delete level;
#endif
}

char cryptkey(const std::string &fname)
{
  int key = 0;
  for(char ch : fname)
  {
    key += 3 * (ch - ' ');
  }
  return key & 0xff;
}
