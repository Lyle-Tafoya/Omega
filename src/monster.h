#ifndef OMEGA_MONSTER_H_
#define OMEGA_MONSTER_H_

#include "object.h"

#include <curses.h>
#include <forward_list>
#include <memory>
#include <string>

class monster
{
public:
  std::forward_list<std::unique_ptr<object>> possessions;
  unsigned char attacked;
  int aux1, aux2, x, y, click;
  int id, hp, hit, ac, dmg, sense, wakeup, level, speed;
  unsigned char sleep, treasure;
  long xpv;
  int corpseweight, corpsevalue, transformid, startthing;
  unsigned char uniqueness;
  int talkf, movef, meleef, strikef, specialf;
  long status, immunity;
  chtype monchar;
  std::string monstring;
  std::string corpsestr;
  std::string meleestr;

  monster() = default;
  monster(const monster &m);
  monster(unsigned char attacked, int aux1, int aux2, int x, int y, int click, int id, int hp, int hit, int ac, int dmg, int sense, int wakeup, int level, int speed, unsigned char sleep, unsigned char treasure, long xpv, int corpseweight, int corpsevalue, int transformid, int startthing, unsigned char uniqueness, int talkf, int movef, int meleef, int strikef,int specialf, long status, long immunity, chtype monchar, const std::string &monstring, const std::string &corpsestr, const std::string &meleestr);
  monster &operator=(const monster &m);
};

#endif
