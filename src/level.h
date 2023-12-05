#ifndef OMEGA_LEVEL_H_
#define OMEGA_LEVEL_H_

#include "defs.h"
#include "monster.h"

#include <ctime>
#include <curses.h>
#include <forward_list>
#include <vector>

// dungeon locations
struct location
{
  uint8_t p_locf;            // function executed when moved on
  uint8_t lstatus;  // seen,stopsrun,lit,secret,
  uint8_t roomnumber;     // so room can be named
  chtype locchar;         // terrain type
  chtype showchar;        // char instantaneously drawn for site
  int aux;                // signifies various things
  uint8_t buildaux; // used in constructing level
  std::vector<std::unique_ptr<object>> things;
  monster *creature;
};

struct level
{
  uint8_t depth;      // which level is this
  bool generated;     // has the level been made (visited) yet?
  uint8_t numrooms;   // number of rooms on level
  uint8_t tunnelled;  // amount of tunnelling done on this level
  int environment;    // where kind of level is this?
  std::forward_list<std::unique_ptr<monster>> mlist; // List of monsters on level
  std::array<std::array<location, MAXLENGTH>, MAXWIDTH> site; // dungeon data
};

inline bool loc_statusp(int x, int y, lstatus_bit status, const level &lvl)
{
  return lvl.site[x][y].lstatus & status;
}

inline void lset(int x, int y, lstatus_bit status, level &lvl)
{
  lvl.site[x][y].lstatus |= status;
}

inline void lreset(int x, int y, lstatus_bit status, level &lvl)
{
  lvl.site[x][y].lstatus &= ~status;
}

#endif
