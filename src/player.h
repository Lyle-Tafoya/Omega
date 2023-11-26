#ifndef OMEGA_PLAYER_H_
#define OMEGA_PLAYER_H_

#include "defs.h"
#include "object.h"

#include <array>
#include <memory>
#include <string>

// PLAYER STATUS INDICES
enum status_id
{
  ACCURACY,
  BLINDED,
  SLOWED,
  DISPLACED,
  SLEPT,
  DISEASED,
  POISONED,
  HASTED,
  BREATHING,
  INVISIBLE,
  REGENERATING,
  VULNERABLE,
  BERSERK,
  IMMOBILE,
  ALERT,
  AFRAID,
  HERO,
  LEVITATING,
  ACCURATE,
  TRUESIGHT,
  SHADOWFORM,
  ILLUMINATION,
  DEFLECTION,
  PROTECTION, // PROTECTION is deviant -- indicates protective value, not duration
  RETURNING   // RETURNING is somewhat deviant--how many turns 'til RETURN spell goes off
};
constexpr int NUMSTATI = 25;

constexpr int NUMIMMUNITIES = 14;

struct player
{
  std::string name;
  int level;
  long xp;
  int hp, maxhp;
  long mana, maxmana;
  int str, con, dex, agi, iq, pow, maxstr, maxcon, maxdex, maxagi, maxiq, maxpow;
  int hit, dmg, absorption, speed, click;
  int defense, food, alignment;
  long cash;
  int patron, birthday;
  int sx, sy; // sanctuary coordinates
  int x, y;   // current player coordinates
  int itemweight, maxweight;
  long options;
  int packptr;
  std::string meleestr;
  std::array<int, NUMIMMUNITIES> immunity;
  std::array<int, NUMSTATI> status;
  std::array<int, NUMRANKS> rank;
  std::array<long, NUMRANKS> guildxp;
  std::array<std::unique_ptr<object>, MAXITEMS> possessions;
  std::array<std::unique_ptr<object>, MAXPACK> pack;
};

inline bool optionp(int option, const player &p)
{
  return p.options & option;
}

inline void optionset(int option, player &p)
{
  p.options |= option;
}

inline void optionreset(int option, player &p)
{
  p.options &= ~option;
}

#endif
