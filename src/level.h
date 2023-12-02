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
  char p_locf;            // function executed when moved on
  unsigned char lstatus;  // seen,stopsrun,lit,secret,
  char roomnumber;        // so room can be named
  chtype locchar;         // terrain type
  chtype showchar;        // char instantaneously drawn for site
  int aux;                // signifies various things
  unsigned char buildaux; // used in constructing level
  std::vector<std::unique_ptr<object>> things;
  monster *creature;
};

struct level
{
  char depth;  // which level is this
  level *next; // pointer to next level in dungeon
  std::array<std::array<location, MAXLENGTH>, MAXWIDTH> site; // dungeon data
  char generated;     // has the level been made (visited) yet?
  char numrooms;      // number of rooms on level
  char tunnelled;     // amount of tunnelling done on this level
  std::forward_list<std::unique_ptr<monster>> mlist; // List of monsters on level
  int environment;    // where kind of level is this?
  time_t last_visited;   // time player was last on this level
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
