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

// aux2.cpp
// some functions called by commmand[123].cpp, also see aux1.cpp and aux3.cpp
// This is a real grab bag file. It contains functions used by
// aux1.cpp and omega.cpp, as well as elsewhere. It is mainly here so aux1.cpp
// and aux3.cpp are not huge

#include "glob.h"
#include "interactive_menu.hpp"
#include "scr.h"

#include <algorithm>
#include <format>
#include <string>
#include <vector>

extern interactive_menu *menu;

#ifdef SAVE_LEVELS
level *msdos_changelevel(level *oldlevel, int newenv, int newdepth);
#endif

// Player stats like str, agi, etc give modifications to various abilities
// chances to do things, etc. Positive is good, negative bad.
int statmod(int stat)
{
  return (stat - 10) / 2;
}

// effects of hitting
void p_hit(monster *m, int dmg, damage_type dtype)
{
  int dmult;

  std::string hit_message;
  // chance for critical hit..., 3/10
  switch(random_range(10))
  {
    case 0:
      if(random_range(100) < Player.level + std::max(0, Player.rank[MONKS]))
      {
        hit_message = "You annihilate ";
        dmult       = 1000;
      }
      else
      {
        hit_message = "You blast ";
        dmult       = 5;
      }
      break;
    case 1:
    case 2:
      hit_message = "You smash ";
      dmult       = 2;
      break;

    default:
      dmult = 1;
      if(random_range(10))
      {
        hit_message = "You hit ";
      }
      else
      {
        switch(random_range(4))
        {
          case 0:
            hit_message = "You damage ";
            break;
          case 1:
            hit_message = "You inflict bodily harm on ";
            break;
          case 2:
            hit_message = "You injure ";
            break;
          case 3:
            hit_message = "You molest ";
            break;
        }
      }
      break;
  }
  if(Lunarity == 1)
  {
    dmult = dmult * 2;
  }
  else if(Lunarity == -1)
  {
    dmult = dmult / 2;
  }
  if(m->uniqueness == COMMON)
  {
    hit_message += "the ";
  }
  hit_message += std::format("{}.", m->monstring);
  if(Verbosity != TERSE)
  {
    queue_message(hit_message);
  }
  else
  {
    queue_message("You hit it.");
  }

  if(!Player.possessions[O_WEAPON_HAND] && Player.rank[MONKS] > MONK_MASTER_SIGHS)
  {
    dtype = UNSTOPPABLE;
  }

  m_damage(m, dmult * random_range(dmg), dtype);
  if((Verbosity != TERSE) && (random_range(10) == 3) && (m->hp > 0))
  {
    queue_message("It laughs at the injury and fights on!");
  }
}

// try to break a weapon (from fumbling)
void break_weapon()
{
  if(Player.possessions[O_WEAPON_HAND])
  {
    queue_message("Your " + itemid(Player.possessions[O_WEAPON_HAND].get()) + " vibrates in your hand....");
    damage_item(O_WEAPON_HAND);
  }
}

// try to drop a weapon (from fumbling)
void drop_weapon()
{
  if(Player.possessions[O_WEAPON_HAND])
  {
    queue_message(std::format("You dropped your {}.", Player.possessions[O_WEAPON_HAND]->objstr));
    p_drop_at(Player.x, Player.y, 1, Player.possessions[O_WEAPON_HAND].get());
    dispose_lost_objects(1, O_WEAPON_HAND);
  }
  else
  {
    queue_message("You feel fortunate.");
  }
}

// oh nooooo, a fumble....
void p_fumble(int dtype)
{
  queue_message("Ooops! You fumbled....");
  switch(random_range(10))
  {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
      drop_weapon();
      break;
    case 6:
    case 7:
    case 8:
      break_weapon();
      break;
    case 9:
      queue_message("Oh No! You hit yourself!");
      p_damage(Player.dmg, dtype, "stupidity");
      break;
  }
}

void player_miss(const monster *m, int dtype)
{
  if(!random_range(30))
  {
    p_fumble(dtype);
  }
  else
  {
    if(Verbosity != TERSE)
    {
      std::string monster_name;
      if(m->uniqueness == COMMON)
      {
        monster_name = "the ";
      }
      monster_name += m->monstring;
      if(random_range(10))
      {
        queue_message("You miss " + monster_name + ".");
      }
      else
      {
        switch(random_range(4))
        {
          case 0:
            queue_message("You flail lamely at " + monster_name + ".");
            break;
          case 1:
            queue_message("You only amuse " + monster_name + ".");
            break;
          case 2:
            queue_message("You fail to even come close to " + monster_name + ".");
            break;
          case 3:
            queue_message("You totally avoid contact with " + monster_name + ".");
            break;
        }
      }
    }
    else
    {
      queue_message("You missed it.");
    }
  }
}

// hooray
void p_win()
{
  queue_message("You won!");
  display_win();
  endgraf();
  exit(0);
}

// handle a h,j,k,l, etc., to change x and y by dx and dy for targeting in dungeon
void movecursor(int &x, int &y, int dx, int dy)
{
  if(inbounds(x + dx, y + dy))
  {
    x += dx;
    y += dy;
    screencheck(x, y);
  }
  omshowcursor(x, y);
}

// is Player immune to damage type dtype
bool p_immune(int dtype)
{
  return Player.immunity[dtype] > 0;
}

// deal with each possible stati -- values are per move
// this function is executed every move
// A value over 1000 indicates a permanent effect
void minute_status_check()
{
  if(Player.status[HASTED] > 0)
  {
    if(Player.status[HASTED] < 1000)
    {
      Player.status[HASTED]--;
      if(Player.status[HASTED] == 0)
      {
        queue_message("The world speeds up.");
        calc_melee();
      }
    }
  }

  if(Player.status[POISONED] > 0)
  {
    Player.status[POISONED]--;
    p_damage(3, POISON, "poison");
    if(Player.status[POISONED] == 0)
    {
      showflags();
      queue_message("You feel better now.");
    }
  }

  if(Player.immunity[UNSTOPPABLE] > 0)
  {
    for(int i = 0; i < NUMIMMUNITIES; ++i)
    {
      Player.immunity[i]--;
    }
    if(Player.immunity[UNSTOPPABLE] == 1)
    {
      queue_message("You feel vincible again.");
    }
  }

  if(Player.status[IMMOBILE] > 0)
  {
    Player.status[IMMOBILE]--;
    if(Player.status[IMMOBILE] == 0)
    {
      queue_message("You can move again.");
    }
  }

  if(Player.status[SLEPT] > 0)
  {
    Player.status[SLEPT]--;
    if(Player.status[SLEPT] == 0)
    {
      queue_message("You woke up.");
    }
  }

  if(Player.status[REGENERATING] > 0)
  {
    if((Player.hp < Player.maxhp) && (Player.mana > 0))
    {
      Player.hp++;
      Player.mana--;
      dataprint();
    }
    if(Player.status[REGENERATING] < 1000)
    {
      Player.status[REGENERATING]--;
      if(Player.status[REGENERATING] == 0)
      {
        queue_message("You feel less homeostatic.");
      }
    }
  }

  if(Player.status[SLOWED] > 0)
  {
    if(Player.status[SLOWED] < 1000)
    {
      Player.status[SLOWED]--;
      if(Player.status[SLOWED] == 0)
      {
        queue_message("You feel quicker now.");
        calc_melee();
      }
    }
  }

  if(Player.status[RETURNING] > 0)
  {
    Player.status[RETURNING]--;
    if(Player.status[RETURNING] == 10)
    {
      queue_message("Your return spell slowly hums towards activation...");
    }
    else if(Player.status[RETURNING] == 8)
    {
      queue_message("There is an electric tension in the air!");
    }
    else if(Player.status[RETURNING] == 5)
    {
      queue_message("A vortex of mana begins to form around you!");
    }
    else if(Player.status[RETURNING] == 1)
    {
      queue_message("Your surroundings start to warp and fade!");
    }
    if(Player.status[RETURNING] == 0)
    {
      level_return();
    }
  }

  if(Player.status[AFRAID] > 0)
  {
    if(Player.status[AFRAID] < 1000)
    {
      Player.status[AFRAID]--;
      if(Player.status[AFRAID] == 0)
      {
        queue_message("You feel bolder now.");
      }
    }
  }
}

// effect of gamma ray radiation...
void moon_check()
{
  // 24 day lunar cycle
  Phase    = (Phase + 1) % 24;
  Lunarity = 0;
  if(((Player.patron == DRUID) && ((Phase / 2 == 3) || (Phase / 2 == 9))) ||
     ((Player.alignment > 10) && (Phase / 2 == 6)) || ((Player.alignment < -10) && (Phase / 2 == 0)))
  {
    queue_message("As the moon rises you feel unusually vital!");
    Lunarity = 1;
  }
  else if(((Player.patron == DRUID) && ((Phase / 2 == 0) || (Phase / 2 == 6))) ||
          ((Player.alignment > 10) && (Phase / 2 == 0)) ||
          ((Player.alignment < -10) && (Phase / 2 == 6)))
  {
    queue_message("The rise of the moon tokens a strange enervation!");
    Lunarity = -1;
  }
}

// check 1/hour for torch to burn out if used
void torch_check()
{
  for(int i = O_READY_HAND; i <= O_WEAPON_HAND; ++i)
  {
    if(Player.possessions[i])
    {
      if((Player.possessions[i]->id == THINGID + 8) && Player.possessions[i]->aux)
      {
        --Player.possessions[i]->charge;
        if(!Player.possessions[i]->charge)
        {
          queue_message("Your torch goes out!!!");
          conform_unused_object(Player.possessions[i]);
          Player.possessions[i]->on_use     = I_NO_OP;
          Player.possessions[i]->on_unequip = I_NO_OP;
          Player.possessions[i]->cursestr   = Player.possessions[i]->truename =
            Player.possessions[i]->objstr   = "burnt-out torch";
        }
      }
    }
  }
}

// values are in multiples of ten minutes
// values over 1000 indicate a permanent effect
void tenminute_status_check()
{
  if((Player.status[SHADOWFORM] > 0) && (Player.status[SHADOWFORM] < 1000))
  {
    Player.status[SHADOWFORM]--;
    if(Player.status[SHADOWFORM] == 0)
    {
      Player.immunity[NORMAL_DAMAGE]--;
      Player.immunity[ACID]--;
      Player.immunity[THEFT]--;
      Player.immunity[INFECTION]--;
      queue_message("You feel less shadowy now.");
    }
  }

  if((Player.status[ILLUMINATION] > 0) && (Player.status[ILLUMINATION] < 1000))
  {
    Player.status[ILLUMINATION]--;
    if(Player.status[ILLUMINATION] == 0)
    {
      queue_message("Your light goes out!");
    }
  }

  if((Player.status[VULNERABLE] > 0) && (Player.status[VULNERABLE] < 1000))
  {
    Player.status[VULNERABLE]--;
    if(Player.status[VULNERABLE] == 0)
    {
      queue_message("You feel less endangered.");
    }
  }

  if((Player.status[DEFLECTION] > 0) && (Player.status[DEFLECTION] < 1000))
  {
    Player.status[DEFLECTION]--;
    if(Player.status[DEFLECTION] == 0)
    {
      queue_message("You feel less well defended.");
    }
  }

  if((Player.status[ACCURATE] > 0) && (Player.status[ACCURACY] < 1000))
  {
    Player.status[ACCURATE]--;
    if(Player.status[ACCURATE] == 0)
    {
      calc_melee();
      queue_message("The bulls' eyes go away.");
    }
  }
  if((Player.status[HERO] > 0) && (Player.status[HERO] < 1000))
  {
    Player.status[HERO]--;
    if(Player.status[HERO] == 0)
    {
      calc_melee();
      queue_message("You feel less than super.");
    }
  }

  if((Player.status[LEVITATING] > 0) && (Player.status[LEVITATING] < 1000))
  {
    Player.status[LEVITATING]--;
    if(Player.status[LEVITATING] == 0)
    {
      queue_message("You're no longer walking on air.");
    }
  }

  if(Player.status[DISEASED] > 0)
  {
    Player.status[DISEASED]--;
    if(Player.status[DISEASED] == 0)
    {
      showflags();
      queue_message("You feel better now.");
    }
  }

  if((Player.status[INVISIBLE] > 0) && (Player.status[INVISIBLE] < 1000))
  {
    Player.status[INVISIBLE]--;
    if(Player.status[INVISIBLE] == 0)
    {
      queue_message("You feel more opaque now.");
    }
  }

  if((Player.status[BLINDED] > 0) && (Player.status[BLINDED] < 1000))
  {
    Player.status[BLINDED]--;
    if(Player.status[BLINDED] == 0)
    {
      queue_message("You can see again.");
    }
  }

  if((Player.status[TRUESIGHT] > 0) && (Player.status[TRUESIGHT] < 1000))
  {
    Player.status[TRUESIGHT]--;
    if(Player.status[TRUESIGHT] == 0)
    {
      queue_message("You feel less keen now.");
    }
  }

  if((Player.status[BERSERK] > 0) && (Player.status[BERSERK] < 1000))
  {
    Player.status[BERSERK]--;
    if(Player.status[BERSERK] == 0)
    {
      queue_message("You stop foaming at the mouth.");
    }
  }

  if((Player.status[ALERT] > 0) && (Player.status[ALERT] < 1000))
  {
    Player.status[ALERT]--;
    if(Player.status[ALERT] == 0)
    {
      queue_message("You feel less alert now.");
    }
  }

  if((Player.status[BREATHING] > 0) && (Player.status[BREATHING] < 1000))
  {
    Player.status[BREATHING]--;
    if(Player.status[BREATHING] == 0)
    {
      queue_message("You feel somewhat congested.");
    }
  }

  if((Player.status[DISPLACED] > 0) && (Player.status[DISPLACED] < 1000))
  {
    Player.status[DISPLACED]--;
    if(Player.status[DISPLACED] == 0)
    {
      queue_message("You feel a sense of position.");
    }
  }
  timeprint();
  dataprint();
}

// experience requirements
long expval(int plevel)
{
  switch(plevel)
  {
    case 0:
      return 0L;
    case 1:
      return 20L;
    case 2:
      return 50L;
    case 3:
      return 200L;
    case 4:
      return 500L;
    case 5:
      return 1000L;
    case 6:
      return 2000L;
    case 7:
      return 3000L;
    case 8:
      return 5000L;
    case 9:
      return 7000L;
    case 10:
      return 10000L;
    default:
      return (plevel - 9) * 10000L;
  }
}

// Increase in level at appropriate experience gain
void gain_level()
{
  int hp_gain;

  if(gamestatusp(SUPPRESS_PRINTING, GameStatus))
  {
    return;
  }
  while(expval(Player.level + 1) <= Player.xp)
  {
    Player.level++;
    queue_message("You have attained a new experience level!");
    queue_message(
      std::format("You are now {}{}", getarticle(levelname(Player.level)), levelname(Player.level))
    );
    hp_gain = random_range(Player.con) + 1;
    if(Player.hp < Player.maxhp)
    {
      Player.hp += hp_gain * Player.hp / Player.maxhp;
    }
    else if(Player.hp < Player.maxhp + hp_gain)
    {
      Player.hp = Player.maxhp + hp_gain;
    }
    // else leave current hp alone
    Player.maxhp += hp_gain;
    Player.maxmana = calcmana();
    // If the character was given a bonus, let him keep it.  Otherwise recharge him
    Player.mana = std::max(Player.mana, Player.maxmana);
  }
  calc_melee();
}

// If an item is unidentified, it isn't worth much to those who would buy it
long item_value(const object *item)
{
  if(item->known == 0)
  {
    if(item->objchar == THING)
    {
      return 1;
    }
    else
    {
      return true_item_value(item) / 10;
    }
  }
  else if(item->known == 1)
  {
    if(item->objchar == THING)
    {
      return item->basevalue;
    }
    else
    {
      return item->basevalue / 2;
    }
  }
  else
  {
    return true_item_value(item);
  }
}

// figures value based on item base-value, charge, plus, and blessing
long true_item_value(const object *item)
{
  long value = item->basevalue;

  if(item->objchar == THING)
  {
    return item->basevalue;
  }
  else
  {
    if(item->objchar == STICK)
    {
      value += value * item->charge / 20;
    }
    if(item->plus > -1)
    {
      value += value * item->plus / 4;
    }
    else
    {
      value /= -item->plus;
    }
    if(item->blessing > 0)
    {
      value *= 2;
    }
    return (long)value;
  }
}

// kill off player if he isn't got the "breathing" status
void p_drown()
{
  int attempts = 3;

  if(Player.status[BREATHING] > 0)
  {
    queue_message("Your breathing is unaffected!");
  }
  else
  {
    while(Player.possessions[O_ARMOR] || Player.itemweight > ((int)(Player.maxweight / 2)))
    {
      switch(attempts--)
      {
        case 3:
          queue_message("You try to hold your breath...");
          break;
        case 2:
          queue_message("You try to hold your breath... You choke...");
          break;
        case 1:
          queue_message("You try to hold your breath... You choke... Your lungs fill...");
          break;
        case 0:
          p_death("drowning");
      }
      std::vector<std::string> lines = {
        {"a: Drop an item."},
        {"b: Bash an item."},
        {"c: Drop your whole pack."}};
      menu->load(lines);
      menu->print();
      switch(menugetc())
      {
        case 'a':
          drop_equipped_item();
          if(Level->site[Player.x][Player.y].p_locf == L_WATER && !Level->site[Player.x][Player.y].things.empty())
          {
            queue_message("It sinks without a trace.");
            Level->site[Player.x][Player.y].things.clear();
          }
          break;
        case 'b':
          bash_item();
          break;
        case 'c':
          setgamestatus(SUPPRESS_PRINTING, GameStatus);
          for(std::unique_ptr<object> &item : Player.pack)
          {
            if(Level->site[Player.x][Player.y].p_locf != L_WATER)
            {
              p_drop_at(Player.x, Player.y, item->number, item.get());
            }
          }
          Player.pack.clear();
          if(Level->site[Player.x][Player.y].p_locf == L_WATER)
          {
            queue_message("It sinks without a trace.");
          }
          resetgamestatus(SUPPRESS_PRINTING, GameStatus);
          calc_melee();
          break;
      }
    }
  }
  show_screen();
  return;
}

// the effect of some weapon on monster m, with dmgmod a bonus to damage
void weapon_use(int dmgmod, std::unique_ptr<object> &weapon, monster *m)
{
  int aux = (!weapon ? -2 : weapon->aux); // bare hands
  switch(aux)
  {
    case -2:
      weapon_bare_hands(dmgmod, m);
      break;
    default:
    case I_NO_OP:
      weapon_normal_hit(dmgmod, weapon, m);
      break;
    case I_ACIDWHIP:
      weapon_acidwhip(dmgmod, weapon, m);
      break;
    case I_TANGLE:
      weapon_tangle(dmgmod, weapon, m);
      break;
    case I_ARROW:
      weapon_arrow(dmgmod, weapon, m);
      break;
    case I_BOLT:
      weapon_bolt(dmgmod, weapon, m);
      break;
    case I_DEMONBLADE:
      weapon_demonblade(dmgmod, weapon, m);
      break;
    case I_LIGHTSABRE:
      weapon_lightsabre(dmgmod, weapon, m);
      break;
    case I_MACE_DISRUPT:
      weapon_mace_disrupt(dmgmod, weapon, m);
      break;
    case I_VORPAL:
      weapon_vorpal(dmgmod, weapon, m);
      break;
    case I_DESECRATE:
      weapon_desecrate(dmgmod, weapon, m);
      break;
    case I_FIRESTAR:
      weapon_firestar(dmgmod, weapon, m);
      break;
    case I_DEFEND:
      weapon_defend(dmgmod, weapon, m);
      break;
    case I_VICTRIX:
      weapon_victrix(dmgmod, weapon, m);
      break;
    case I_SCYTHE:
      weapon_scythe(dmgmod, weapon, m);
      break;
  }
}

// for printing actions in printactions above
std::string actionlocstr(char dir)
{
  switch(dir)
  {
    case 'L':
      return "low.";
    case 'C':
      return "center.";
    case 'H':
      return "high.";
    default:
      return "wildly.";
  }
}

// checks to see if player hits with hitmod vs. monster m at location hitloc
int player_hit(int hitmod, char hitloc, monster *m)
{
  if(m->hp < 1)
  {
    queue_message("Unfortunately, your opponent is already dead!");
    return false;
  }
  else
  {
    if(hitloc == 'X')
    {
      hitloc = random_loc();
    }

    transcribe_monster_actions(m);

    bool blocks    = false;
    int goodblocks = 0;

    for(size_t i = 0; i < m->meleestr.length(); i += 2)
    {
      if((m->meleestr[i] == 'B') || (m->meleestr[i] == 'R'))
      {
        blocks = true;
        if(hitloc == m->meleestr[i + 1])
        {
          ++goodblocks;
        }
      }
    }

    if(!blocks)
    {
      goodblocks = -1;
    }
    int hit = hitp(Player.hit + hitmod, m->ac + goodblocks * 10);
    if(!hit && goodblocks > 0)
    {
      std::string block_message;
      if(m->uniqueness == COMMON)
      {
        block_message = std::format("The {}", m->monstring);
      }
      else
      {
        block_message = m->monstring;
      }
      block_message += " blocks it!";
      if(Verbosity == VERBOSE)
      {
        queue_message(block_message);
      }
    }
    return hit;
  }
}

// execute player combat actions versus monster m
void tacplayer(monster *m)
{
  size_t meleestr_length = std::min(Player.meleestr.length(), maneuvers() * 2);
  for(size_t i = 0; i < meleestr_length; i += 2)
  {
    if(m->hp <= 0)
    {
      break;
    }
    switch(Player.meleestr[i])
    {
      case 'L':
        if(Player.possessions[O_WEAPON_HAND] || Player.status[BERSERK])
        {
          if(Verbosity == VERBOSE)
          {
            queue_message("You lunge " + actionlocstr(Player.meleestr[i + 1]));
          }
          if(player_hit(Player.level + Player.dex, Player.meleestr[i + 1], m))
          {
            weapon_use(Player.level, Player.possessions[O_WEAPON_HAND], m);
          }
          else
          {
            player_miss(m, NORMAL_DAMAGE);
          }
          break;
        }
        [[fallthrough]];
      case 'A':
        if(!Player.possessions[O_WEAPON_HAND] || Player.possessions[O_WEAPON_HAND]->type != THRUSTING)
        {
          if(Verbosity == VERBOSE)
          {
            std::string combat_message;
            if(!Player.possessions[O_WEAPON_HAND])
            {
              queue_message("You punch " + actionlocstr(Player.meleestr[i + 1]));
            }
            else if(Player.possessions[O_WEAPON_HAND]->type == CUTTING)
            {
              queue_message("You cut " + actionlocstr(Player.meleestr[i + 1]));
            }
            else if(Player.possessions[O_WEAPON_HAND]->type == STRIKING)
            {
              queue_message("You strike " + actionlocstr(Player.meleestr[i + 1]));
            }
            else
            {
              queue_message("You attack " + actionlocstr(Player.meleestr[i + 1]));
            }
          }
          if(player_hit(0, Player.meleestr[i + 1], m))
          {
            weapon_use(2 * statmod(Player.str), Player.possessions[O_WEAPON_HAND], m);
          }
          else
          {
            player_miss(m, NORMAL_DAMAGE);
          }
        }
        else
        {
          if(Verbosity == VERBOSE)
          {
            queue_message("You thrust " + actionlocstr(Player.meleestr[i + 1]));
          }
          if(player_hit(2 * statmod(Player.dex), Player.meleestr[i + 1], m))
          {
            weapon_use(0, Player.possessions[O_WEAPON_HAND], m);
          }
          else
          {
            player_miss(m, NORMAL_DAMAGE);
          }
        }
        break;
    }
  }
}

// This function is used to undo all items temporarily, should
// always be used in pairs with on being true and false, and may cause
// anomalous stats and item-usage if used indiscriminately

void toggle_item_use(bool on)
{
  static int used[MAXITEMS];
  setgamestatus(SUPPRESS_PRINTING, GameStatus);
  if(on)
  {
    for(int i = 0; i < MAXITEMS; ++i)
    {
      used[i] = false;
      if(Player.possessions[i])
      {
        if(Player.possessions[i]->on_equip != I_NO_OP && (used[i] = Player.possessions[i]->used))
        {
          item_unequip(Player.possessions[i]);
        }
      }
    }
  }
  else
  {
    for(int i = 1; i < MAXITEMS; ++i)
    {
      if(used[i])
      {
        item_equip(Player.possessions[i]);
      }
    }
    calc_melee();
    showflags();
    dataprint();
    timeprint();
  }
  resetgamestatus(SUPPRESS_PRINTING, GameStatus);
}

void enter_site(chtype site)
{
  switch(site)
  {
    case CITY:
      change_environment(E_CITY);
      break;
    case VILLAGE:
      change_environment(E_VILLAGE);
      break;
    case CAVES:
      change_environment(E_CAVES);
      break;
    case CASTLE:
      change_environment(E_CASTLE);
      break;
    case VOLCANO:
      change_environment(E_VOLCANO);
      break;
    case TEMPLE:
      change_environment(E_TEMPLE);
      break;
    case DRAGONLAIR:
      change_environment(E_DLAIR);
      break;
    case STARPEAK:
      change_environment(E_STARPEAK);
      break;
    case MAGIC_ISLE:
      change_environment(E_MAGIC_ISLE);
      break;
    default:
      queue_message("There's nothing to enter here!");
      break;
  }
}

// Switches context dungeon/countryside/city, etc
void change_environment(char new_environment)
{
  bool emerging = false;

  Player.sx = -1;
  Player.sy = -1;                    // reset sanctuary if there was one
  resetgamestatus(LOST, GameStatus); // in case the player gets lost _on_ a site

  resetgamestatus(FAST_MOVE, GameStatus);

  Last_Environment = Current_Environment;
  if(Last_Environment == E_COUNTRYSIDE)
  {
    LastCountryLocX = Player.x;
    LastCountryLocY = Player.y;
  }
  if(((Last_Environment == E_CITY) || (Last_Environment == E_VILLAGE)) && ((new_environment == E_MANSION) || (new_environment == E_HOUSE) || (new_environment == E_HOVEL) || (new_environment == E_SEWERS) || (new_environment == E_ARENA)))
  {
    LastTownLocX = Player.x;
    LastTownLocY = Player.y;
  }
  else if(((Last_Environment == E_MANSION) || (Last_Environment == E_HOUSE) || (Last_Environment == E_HOVEL) || (Last_Environment == E_SEWERS) || (Last_Environment == E_ARENA)) && ((new_environment == E_CITY) || (new_environment == E_VILLAGE)))
  {
    Player.x = LastTownLocX;
    Player.y = LastTownLocY;
    emerging = true;
  }

  Current_Environment = new_environment;
  switch(new_environment)
  {
    case E_ARENA:
      LENGTH   = 16;
      WIDTH    = 64;
      Player.x = 5;
      Player.y = 7;
      setgamestatus(ARENA_MODE, GameStatus);
      load_arena();
      ScreenOffset     = 0;
      HorizontalOffset = 0;
      show_screen();
      break;
    case E_ABYSS:
      LENGTH   = 16;
      WIDTH    = 64;
      Player.x = 32;
      Player.y = 15;
      load_abyss();
      abyss_file();
      lose_all_items();
      ScreenOffset     = 0;
      HorizontalOffset = 0;
      show_screen();
      break;
    case E_CIRCLE:
      LENGTH   = 16;
      WIDTH    = 64;
      Player.x = 32;
      Player.y = 14;
      load_circle(true);
      if(Objects[ARTIFACTID + 21].uniqueness == UNIQUE_TAKEN)
      {
        queue_message("A bemused voice says:");
        queue_message("'Why are you here? You already have the Star Gem!'");
      }
      else if(Player.rank[CIRCLE] > 0)
      {
        queue_message("You hear the voice of the Prime Sorceror:");
        queue_message("'Congratulations on your attainment of the Circle's Demesne.'");
        queue_message("For the honor of the Circle, you may take the Star Gem");
        queue_message("and destroy it on the acme of Star Peak.");
        queue_message("Beware the foul LawBringer who resides there...");
        queue_message("By the way, some of the members of the Circle seem to");
        queue_message("have become a bit jealous of your success --");
        queue_message("I'd watch out for them too if I were you.");
      }
      else if(Player.alignment > 0)
      {
        queue_message("A mysterious ghostly image materializes in front of you.");
        queue_message("It speaks: 'Greetings, fellow abider in Law. I am called");
        queue_message("The LawBringer. If you wish to advance our cause, obtain");
        queue_message("the mystic Star Gem and return it to me on Star Peak.");
        queue_message("Beware the power of the evil Circle of Sorcerors and the");
        queue_message("forces of Chaos which guard the gem.'");
        queue_message("The strange form fades slowly.");
      }
      ScreenOffset     = 0;
      HorizontalOffset = 0;
      show_screen();
      break;
    case E_COURT:
      WIDTH           = 64;
      LENGTH          = 24;
      Player.x        = 32;
      Player.y        = 2;
      LastCountryLocX = 6;
      LastCountryLocY = 1;
      load_court(true);
      ScreenOffset     = 0;
      HorizontalOffset = 0;
      show_screen();
      break;
    case E_MANSION:
      WIDTH  = 64;
      LENGTH = 16;
      load_house(E_MANSION, true);
      Player.y         = 8;
      Player.x         = 2;
      ScreenOffset     = 0;
      HorizontalOffset = 0;
      show_screen();
      break;
    case E_HOUSE:
      WIDTH  = 64;
      LENGTH = 16;
      load_house(E_HOUSE, true);
      Player.y         = 13;
      Player.x         = 2;
      ScreenOffset     = 0;
      HorizontalOffset = 0;
      show_screen();
      break;
    case E_HOVEL:
      WIDTH  = 64;
      LENGTH = 16;
      load_house(E_HOVEL, true);
      Player.y         = 9;
      Player.x         = 2;
      ScreenOffset     = 0;
      HorizontalOffset = 0;
      show_screen();
      break;
    case E_DLAIR:
      WIDTH    = 64;
      LENGTH   = 16;
      Player.y = 9;
      Player.x = 2;
      load_dlair(gamestatusp(KILLED_DRAGONLORD, GameStatus), true);
      ScreenOffset     = 0;
      HorizontalOffset = 0;
      show_screen();
      break;
    case E_STARPEAK:
      WIDTH    = 64;
      LENGTH   = 16;
      Player.y = 9;
      Player.x = 2;
      load_speak(gamestatusp(KILLED_LAWBRINGER, GameStatus), true);
      ScreenOffset     = 0;
      HorizontalOffset = 0;
      show_screen();
      break;
    case E_MAGIC_ISLE:
      WIDTH    = 64;
      LENGTH   = 16;
      Player.y = 14;
      Player.x = 62;
      load_misle(gamestatusp(KILLED_EATER, GameStatus), true);
      ScreenOffset     = 0;
      HorizontalOffset = 0;
      show_screen();
      break;
    case E_TEMPLE:
      WIDTH  = 64;
      LENGTH = 16;
      load_temple(Country[Player.x][Player.y].aux, true);
      Player.y         = 15;
      Player.x         = 32;
      ScreenOffset     = 0;
      HorizontalOffset = 0;
      show_screen();
      break;
    case E_CITY:
      WIDTH  = 64;
      LENGTH = 64;
      if(emerging)
      {
        queue_message("You emerge onto the street.");
        emerging = false;
      }
      else
      {
        queue_message("You pass through the massive gates of Rampart, the city.");
        Player.x = 62;
        Player.y = 21;
      }
      if(!City)
      {
        load_city(true);
      }
#ifdef SAVE_LEVELS
      else
        msdos_changelevel(Level, new_environment, 0);
#endif
      Level = City;
      calculate_offsets(Player.x, Player.y);
      show_screen();
      break;
    case E_VILLAGE:
      WIDTH  = 64;
      LENGTH = 16;
      if(!emerging)
      {
        // different villages per different locations
        switch(Country[Player.x][Player.y].aux)
        {
          case 1:
            Player.x   = 0;
            Player.y   = 6;
            Villagenum = 1;
            break;
          default:
            queue_message("Very strange, a nonexistent village.");
            [[fallthrough]];
          case 2:
            Player.x   = 39;
            Player.y   = 15;
            Villagenum = 2;
            break;
          case 3:
            Player.x   = 63;
            Player.y   = 8;
            Villagenum = 3;
            break;
          case 4:
            Player.x   = 32;
            Player.y   = 15;
            Villagenum = 4;
            break;
          case 5:
            Player.x   = 2;
            Player.y   = 8;
            Villagenum = 5;
            break;
          case 6:
            Player.x   = 2;
            Player.y   = 2;
            Villagenum = 6;
            break;
        }
      }
      if(!emerging || !TempLevel)
      {
        load_village(Villagenum, true);
      }
      else if(TempLevel->environment != E_VILLAGE)
      {
        load_village(Villagenum, true);
      }
      else
      {
#ifdef SAVE_LEVELS
        msdos_changelevel(Level, new_environment, 0);
#endif
        Level = TempLevel;
      }
      if(emerging)
      {
        queue_message("You emerge onto the street.");
        emerging = false;
      }
      else
      {
        queue_message("You enter a small rural village.");
      }
      ScreenOffset     = 0;
      HorizontalOffset = 0;
      show_screen();
      break;
    case E_CAVES:
      WIDTH  = 64;
      LENGTH = 64;
      queue_message("You enter a dark cleft in a hillside;");
      queue_message("You note signs of recent passage in the dirt nearby.");
      if(gamestatusp(MOUNTED, GameStatus))
      {
        queue_message("Seeing as you might not be coming back, you feel compelled");
        queue_message("to let your horse go, rather than keep him hobbled outside.");
        resetgamestatus(MOUNTED, GameStatus);
        calc_melee();
      }
      MaxDungeonLevels = CAVELEVELS;
      if(Current_Dungeon != E_CAVES)
      {
#ifdef SAVE_LEVELS
        msdos_changelevel(Level, 0, -1);
#endif
        free_dungeon();
        Dungeon         = nullptr;
        Level           = nullptr;
        Current_Dungeon = E_CAVES;
      }
      change_level(0, 1, false);
      break;
    case E_VOLCANO:
      WIDTH  = 64;
      LENGTH = 64;
      queue_message("You pass down through the glowing crater.");
      if(gamestatusp(MOUNTED, GameStatus))
      {
        queue_message("Seeing as you might not be coming back, you feel compelled");
        queue_message("to let your horse go, rather than keep him hobbled outside.");
        resetgamestatus(MOUNTED, GameStatus);
        calc_melee();
      }
      MaxDungeonLevels = VOLCANOLEVELS;
      if(Current_Dungeon != E_VOLCANO)
      {
#ifdef SAVE_LEVELS
        msdos_changelevel(Level, 0, -1);
#endif
        free_dungeon();
        Dungeon         = nullptr;
        Level           = nullptr;
        Current_Dungeon = E_VOLCANO;
      }
      change_level(0, 1, false);
      break;
    case E_ASTRAL:
      WIDTH  = 64;
      LENGTH = 64;
      queue_message("You are in a weird flickery maze.");
      if(gamestatusp(MOUNTED, GameStatus))
      {
        queue_message("Your horse doesn't seem to have made it....");
        resetgamestatus(MOUNTED, GameStatus);
        calc_melee();
      }
      MaxDungeonLevels = ASTRALLEVELS;
      if(Current_Dungeon != E_ASTRAL)
      {
#ifdef SAVE_LEVELS
        msdos_changelevel(Level, 0, -1);
#endif
        free_dungeon();
        Dungeon         = nullptr;
        Level           = nullptr;
        Current_Dungeon = E_ASTRAL;
      }
      change_level(0, 1, false);
      break;
    case E_CASTLE:
      WIDTH  = 64;
      LENGTH = 64;
      queue_message("You cross the drawbridge. Strange forms move beneath the water.");
      if(gamestatusp(MOUNTED, GameStatus))
      {
        queue_message("Seeing as you might not be coming back, you feel compelled");
        queue_message("to let your horse go, rather than keep him hobbled outside.");
        resetgamestatus(MOUNTED, GameStatus);
      }
      MaxDungeonLevels = CASTLELEVELS;
      if(Current_Dungeon != E_CASTLE)
      {
#ifdef SAVE_LEVELS
        msdos_changelevel(Level, 0, -1);
#endif
        free_dungeon();
        Dungeon         = nullptr;
        Level           = nullptr;
        Current_Dungeon = E_CASTLE;
      }
      change_level(0, 1, false);
      break;
    case E_SEWERS:
      WIDTH  = 64;
      LENGTH = 64;
      queue_message("You pry open a manhole and descend into the sewers below.");
      if(gamestatusp(MOUNTED, GameStatus))
      {
        queue_message("You horse waits patiently outside the sewer entrance....");
        dismount_steed();
      }
      MaxDungeonLevels = SEWERLEVELS;
      if(Current_Dungeon != E_SEWERS)
      {
#ifdef SAVE_LEVELS
        msdos_changelevel(Level, 0, -1);
#endif
        free_dungeon();
        Dungeon         = nullptr;
        Level           = nullptr;
        Current_Dungeon = E_SEWERS;
      }
      change_level(0, 1, false);
      break;
    case E_COUNTRYSIDE:
      WIDTH  = 64;
      LENGTH = 64;
      queue_message("You return to the fresh air of the open countryside.");
      if(Last_Environment == E_CITY)
      {
        Player.x = 27;
        Player.y = 19;
      }
      else
      {
        Player.x = LastCountryLocX;
        Player.y = LastCountryLocY;
      }
      for(int i = 0; i < 9; ++i)
      {
        c_set(Player.x + Dirs[0][i], Player.y + Dirs[1][i], SEEN, Country);
      }
      calculate_offsets(Player.x, Player.y);
      show_screen();
      break;
    case E_TACTICAL_MAP:
      WIDTH  = 64;
      LENGTH = 16;
      queue_message("You are now on the tactical screen; exit off any side to leave");
      make_country_screen(Country[Player.x][Player.y].current_terrain_type);
      make_country_monsters(Country[Player.x][Player.y].current_terrain_type);
      Player.x = WIDTH / 2;
      Player.y = LENGTH / 2;
      while(Level->site[Player.x][Player.y].locchar == WATER)
      {
        if(Player.y < LENGTH / 2 + 5)
        {
          Player.y++;
        }
        else if(Player.x > WIDTH / 2 - 10)
        {
          Player.x--;
          Player.y = LENGTH / 2 - 5;
        }
        else
        {
          Level->site[Player.x][Player.y].locchar = Level->site[Player.x][Player.y].showchar = FLOOR;
          Level->site[Player.x][Player.y].p_locf                                             = L_NO_OP;
        }
      }
      ScreenOffset     = 0;
      HorizontalOffset = 0;
      show_screen();
      break;
    case E_NEVER_NEVER_LAND:
    default:
      queue_message("There must be some mistake. You don't look like Peter Pan.");
      queue_message("(But here you are in Never-Never Land)");
      calculate_offsets(Player.x, Player.y);
      show_screen();
      break;
  }
  setlastxy(Player.x, Player.y);
  if(Current_Environment != E_COUNTRYSIDE)
  {
    showroom(Level->site[Player.x][Player.y].roomnumber);
  }
  else
  {
    terrain_check(false);
  }
}
