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

#include "monster.h"

monster::monster(int id, int hp, int hit, int ac, int dmg, int sense, int wakeup, int level, int speed, unsigned char sleep, unsigned char treasure, long xpv, int corpseweight, int corpsevalue, int transformid, int startthing, unsigned char uniqueness, int talkf, int movef, int meleef, int strikef,int specialf, long status, long immunity, chtype monchar, const std::string &monstring, const std::string &corpsestr, const std::string &meleestr)
  : attacked(0), aux1(0), aux2(0), x(0), y(0), click(0), id(id), hp(hp), hit(hit), ac(ac), dmg(dmg), sense(sense), wakeup(wakeup), level(level), speed(speed), sleep(sleep), treasure(treasure), xpv(xpv), corpseweight(corpseweight), corpsevalue(corpsevalue), transformid(transformid), startthing(startthing), uniqueness(uniqueness), talkf(talkf), movef(movef), meleef(meleef), strikef(strikef), specialf(specialf), status(status), immunity(immunity), monchar(monchar), monstring(monstring), corpsestr(corpsestr), meleestr(meleestr)
{
}


monster::monster(const monster &m)
{
  attacked = m.attacked;
  aux1 = m.aux1;
  aux2 = m.aux2;
  x = m.x;
  y = m.y;
  click = m.click;
  id = m.id;
  hp = m.hp;
  hit = m.hit;
  ac = m.ac;
  dmg = m.dmg;
  sense = m.sense;
  wakeup = m.wakeup;
  level = m.level;
  speed = m.speed;
  sleep = m.sleep;
  treasure = m.treasure;
  xpv = m.xpv;
  corpseweight = m.corpseweight;
  corpsevalue = m.corpsevalue;
  transformid = m.transformid;
  startthing = m.startthing;
  uniqueness = m.uniqueness;
  talkf = m.talkf;
  movef = m.movef;
  meleef = m.meleef;
  strikef = m.strikef;
  specialf = m.specialf;
  status = m.status;
  immunity = m.immunity;
  monchar = m.monchar;
  monstring = m.monstring;
  corpsestr = m.corpsestr;
  meleestr = m.meleestr;
}

monster &monster::operator=(const monster &m)
{
  attacked = m.attacked;
  aux1 = m.aux1;
  aux2 = m.aux2;
  x = m.x;
  y = m.y;
  click = m.click;
  id = m.id;
  hp = m.hp;
  hit = m.hit;
  ac = m.ac;
  dmg = m.dmg;
  sense = m.sense;
  wakeup = m.wakeup;
  level = m.level;
  speed = m.speed;
  sleep = m.sleep;
  treasure = m.treasure;
  xpv = m.xpv;
  corpseweight = m.corpseweight;
  corpsevalue = m.corpsevalue;
  transformid = m.transformid;
  startthing = m.startthing;
  uniqueness = m.uniqueness;
  talkf = m.talkf;
  movef = m.movef;
  meleef = m.meleef;
  strikef = m.strikef;
  specialf = m.specialf;
  status = m.status;
  immunity = m.immunity;
  monchar = m.monchar;
  monstring = m.monstring;
  corpsestr = m.corpsestr;
  meleestr = m.meleestr;
  return *this;
}
