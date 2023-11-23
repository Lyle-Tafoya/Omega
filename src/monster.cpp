#include "monster.h"

monster::monster(unsigned char attacked, int aux1, int aux2, int x, int y, int click, int id, int hp, int hit, int ac, int dmg, int sense, int wakeup, int level, int speed, unsigned char sleep, unsigned char treasure, long xpv, int corpseweight, int corpsevalue, int transformid, int startthing, unsigned char uniqueness, int talkf, int movef, int meleef, int strikef,int specialf, long status, long immunity, chtype monchar, const std::string &monstring, const std::string &corpsestr, const std::string &meleestr)
  : attacked(attacked), aux1(aux1), aux2(aux2), x(x), y(y), click(click), id(id), hp(hp), hit(hit), ac(ac), dmg(dmg), sense(sense), wakeup(wakeup), level(level), speed(speed), sleep(sleep), treasure(treasure), xpv(xpv), corpseweight(corpseweight), corpsevalue(corpsevalue), transformid(transformid), startthing(startthing), uniqueness(uniqueness), talkf(talkf), movef(movef), meleef(meleef), strikef(strikef), specialf(specialf), status(status), immunity(immunity), monchar(monchar), monstring(monstring), corpsestr(corpsestr), meleestr(meleestr)
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
