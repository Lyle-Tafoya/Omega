#ifndef OMEGA_OBJECT_H_
#define OMEGA_OBJECT_H_

#include "curses.h"

#include <string>

struct object
{
  int id;
  int weight;
  int plus;
  int charge;
  int dmg;
  int hit;
  int aux;
  int number;
  int fragility;
  long basevalue;
  unsigned char known;
  unsigned char used;
  int blessing;
  unsigned char type;
  unsigned char uniqueness;
  int on_use;
  int on_equip;
  int on_unequip;
  unsigned char level;
  chtype objchar;
  std::string objstr;
  std::string truename;
  std::string cursestr;
};

#endif
