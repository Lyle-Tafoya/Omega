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

// itemf1.cpp

// various item functions: potions,scrolls,boots,cloaks,things,food

#include "glob.h"
#include "scr.h"

#include <algorithm>
#include <string>
#include <vector>

// general item functions

void i_no_op(std::unique_ptr<object> &) {}

void i_nothing(std::unique_ptr<object> &) {}

//  scroll functions

void i_knowledge(std::unique_ptr<object> &o)
{
  if(o->blessing > -1)
  {
    Objects[o->id].known = 1;
  }
  knowledge(o->blessing);
}

void i_jane_t(std::unique_ptr<object> &o)
{
  int volume = random_range(6);
  int j, k;
  char v;

  Objects[o->id].known = 1;
  queue_message("Jane's Guide to the World's Treasures: ");
  switch(volume)
  {
    case 0:
      queue_message("SCROLLS");
      j = SCROLLID;
      k = POTIONID;
      break;
    case 1:
      queue_message("POTIONS");
      j = POTIONID;
      k = WEAPONID;
      break;
    case 2:
      queue_message("CLOAKS");
      j = CLOAKID;
      k = BOOTID;
      break;
    case 3:
      queue_message("BOOTS");
      j = BOOTID;
      k = RINGID;
      break;
    case 4:
      queue_message("RINGS");
      j = RINGID;
      k = STICKID;
      break;
    default:
      queue_message("STICKS");
      j = STICKID;
      k = ARTIFACTID;
  }
  std::vector<std::string> lines;
  lines.emplace_back("You could probably now recognize:");
  for(int i = j; i < k; ++i)
  {
    Objects[i].known = 1;
    v                = Objects[i].truename[0];
    if((v >= 'A' && v <= 'Z') || volume == 3)
    {
      lines.emplace_back("   " + Objects[i].truename);
    }
    else if(v == 'a' || v == 'e' || v == 'i' || v == 'o' || v == 'u')
    {
      lines.emplace_back("   an " + Objects[i].truename);
    }
    else
    {
      lines.emplace_back("   a " + Objects[i].truename);
    }
  }
  menu->load(lines);
  menu->get_player_input();
  xredraw();
}

void i_flux(std::unique_ptr<object> &o)
{
  if(o->blessing > -1)
  {
    Objects[o->id].known = 1;
  }
  flux(o->blessing);
}

// enchantment
void i_enchant(std::unique_ptr<object> &o)
{
  if(o->blessing > -1)
  {
    Objects[o->id].known = 1;
  }
  enchant(o->blessing < 0 ? -1 - o->plus : o->plus + 1);
}

// scroll of clairvoyance
void i_clairvoyance(std::unique_ptr<object> &o)
{
  if(o->blessing > -1)
  {
    Objects[o->id].known = 1;
  }
  if(o->blessing < 0)
  {
    amnesia();
  }
  else
  {
    clairvoyance(5 + o->blessing * 5);
  }
}

void i_acquire(std::unique_ptr<object> &o)
{
  int blessing;

  if(o->blessing > -1)
  {
    Objects[o->id].known = 1;
  }
  blessing = o->blessing;
  *o       = Objects[SCROLLID + 0]; // blank out the scroll
  acquire(blessing);
}

void i_teleport(std::unique_ptr<object> &o)
{
  if(o->blessing > -1)
  {
    Objects[o->id].known = 1;
  }
  p_teleport(o->blessing);
}

void i_spells(std::unique_ptr<object> &o)
{
  if(o->blessing > -1)
  {
    Objects[o->id].known = 1;
  }
  queue_message("A scroll of spells.");
  learnspell(o->blessing);
}

// scroll of blessing
void i_bless(std::unique_ptr<object> &o)
{
  Objects[o->id].known = 1;
  bless(o->blessing);
}

// scroll of wishing
void i_wish(std::unique_ptr<object> &o)
{
  if(o->blessing > -1)
  {
    Objects[o->id].known = 1;
  }
  wish(o->blessing);
  *o = Objects[SCROLLID + 0]; // blank out the scroll
}

// scroll of displacement
void i_displace(std::unique_ptr<object> &o)
{
  if(o->blessing > -1)
  {
    Objects[o->id].known = 1;
  }
  displace(o->blessing);
}

// scroll of deflection
void i_deflect(std::unique_ptr<object> &o)
{
  if(o->blessing > -1)
  {
    Objects[o->id].known = 1;
  }
  deflection(o->blessing);
}

// scroll of identification
void i_id(std::unique_ptr<object> &o)
{
  if(o->blessing > -1)
  {
    Objects[o->id].known = 1;
  }
  identify(o->blessing);
}

// potion functions

// potion of healing
void i_heal(std::unique_ptr<object> &o)
{
  if(o->blessing > -1)
  {
    Objects[o->id].known = 1;
    heal(1 + o->plus);
  }
  else
  {
    heal(-1 - abs(o->plus));
  }
}

// potion of monster detection
void i_mondet(std::unique_ptr<object> &o)
{
  if(o->blessing > -1)
  {
    Objects[o->id].known = 1;
  }
  mondet(o->blessing);
}

// potion of object detection
void i_objdet(std::unique_ptr<object> &o)
{
  if(o->blessing > -1)
  {
    Objects[o->id].known = 1;
  }
  objdet(o->blessing);
}

// potion of neutralize poison
void i_neutralize_poison(std::unique_ptr<object> &o)
{
  if(o->blessing > -1)
  {
    Objects[o->id].known = 1;
    queue_message("You feel vital!");
    Player.status[POISONED] = 0;
  }
  else
  {
    p_poison(random_range(20) + 5);
  }
}

// potion of sleep
void i_sleep_self(std::unique_ptr<object> &o)
{
  sleep_player(6);
  Objects[o->id].known = 1;
}

// potion of speed
void i_speed(std::unique_ptr<object> &o)
{
  if(o->blessing > -1)
  {
    Objects[o->id].known = 1;
  }
  haste(o->blessing);
}

// potion of restoration
void i_restore(std::unique_ptr<object> &o)
{
  if(o->blessing > -1)
  {
    Objects[o->id].known = 1;
  }
  recover_stat(o->blessing);
}

void i_augment(std::unique_ptr<object> &o)
{
  if(o->blessing > -1)
  {
    Objects[o->id].known = 1;
  }
  augment(o->blessing);
}

void i_azoth(std::unique_ptr<object> &o)
{
  if(o->plus < 0)
  {
    queue_message("The mercury was poisonous!");
    p_poison(25);
  }
  else if(o->plus == 0)
  {
    queue_message("The partially enchanted azoth makes you sick!");
    Player.con = ((int)(Player.con / 2));
    calc_melee();
  }
  else if(o->blessing < 1)
  {
    queue_message("The unblessed azoth warps your soul!");
    Player.pow = Player.maxpow = ((int)(Player.maxpow / 2));
    level_drain(random_range(10), "cursed azoth");
  }
  else
  {
    queue_message("The azoth fills you with cosmic power!");
    if(Player.str > Player.maxstr * 2)
    {
      queue_message("The power rages out of control!");
      p_death("overdose of azoth");
    }
    else
    {
      heal(10);
      cleanse(1);
      Player.mana = calcmana() * 3;
      toggle_item_use(true);
      Player.str = (Player.maxstr++) * 3;
      toggle_item_use(false);
    }
  }
}

void i_regenerate(std::unique_ptr<object> &o)
{
  regenerate(o->blessing);
}

// boots functions
void i_perm_speed(std::unique_ptr<object> &o)
{
  if(o->blessing > -1)
  {
    if(o->used)
    {
      o->known             = 2;
      Objects[o->id].known = 1;
      if(Player.status[SLOWED] > 0)
      {
        Player.status[SLOWED] = 0;
      }
      queue_message("The world slows down!");
      Player.status[HASTED] += 1500;
    }
    else
    {
      Player.status[HASTED] -= 1500;
      if(Player.status[HASTED] < 1)
      {
        queue_message("The world speeds up again.");
      }
    }
  }
  else
  {
    if(o->used)
    {
      if(Player.status[HASTED] > 0)
      {
        Player.status[HASTED] = 0;
      }
      queue_message("You feel slower.");
      Player.status[SLOWED] += 1500;
    }
    else
    {
      Player.status[SLOWED] -= 1500;
      if(Player.status[SLOWED] < 1)
      {
        queue_message("You feel quicker again.");
      }
    }
  }
}

// cloak functions
void i_perm_displace(std::unique_ptr<object> &o)
{
  if(o->blessing > -1)
  {
    if(o->used)
    {
      queue_message("You feel dislocated.");
      Player.status[DISPLACED] += 1500;
    }
    else
    {
      Player.status[DISPLACED] -= 1500;
      if(Player.status[DISPLACED] < 1)
      {
        queue_message("You feel a sense of position.");
        Player.status[DISPLACED] = 0;
      }
    }
  }
  else
  {
    if(o->used)
    {
      queue_message("You have a forboding of bodily harm!");
      Player.status[VULNERABLE] += 1500;
    }
    else
    {
      Player.status[VULNERABLE] -= 1500;
      if(Player.status[VULNERABLE] < 1)
      {
        queue_message("You feel less endangered.");
        Player.status[VULNERABLE] = 0;
      }
    }
  }
}

void i_perm_negimmune(std::unique_ptr<object> &o)
{
  if(o->blessing > -1)
  {
    if(o->used)
    {
      Player.immunity[NEGENERGY]++;
    }
    else
    {
      Player.immunity[NEGENERGY]--;
    }
  }
  else if(o->used)
  {
    level_drain(abs(o->blessing), "cursed cloak of level drain");
  }
}

// food functions

void i_food(std::unique_ptr<object> &)
{
  switch(random_range(5))
  {
    case 0:
      queue_message("That tasted horrible!");
      break;
    case 1:
      queue_message("Yum!");
      break;
    case 2:
      queue_message("How nauseous!");
      break;
    case 3:
      queue_message("Can I have some more? Please?");
      break;
    case 4:
      queue_message("Your mouth feels like it is growing hair!");
      break;
  }
}

void i_stim(std::unique_ptr<object> &o)
{
  queue_message("You feel Hyper!");
  i_speed(o);
  Player.str += 3;
  Player.con -= 1;
  calc_melee();
}

void i_pow(std::unique_ptr<object> &)
{
  queue_message("You feel a surge of mystic power!");
  Player.mana = 2 * calcmana();
}

void i_poison_food(std::unique_ptr<object> &)
{
  queue_message("This food was contaminated with cyanide!");
  p_poison(random_range(20) + 5);
}

void i_pepper_food(std::unique_ptr<object> &)
{
  queue_message("You innocently start to chew the szechuan pepper.....");
  queue_message("hot.");
  queue_message("Hot.");
  queue_message("Hot!");
  queue_message("HOT!!!!!!");
  p_damage(1, UNSTOPPABLE, "a szechuan pepper");
  queue_message("Your sinuses melt and run out your ears.");
  queue_message("Your mouth and throat seem to be permanently on fire.");
  queue_message("You feel much more awake now....");
  Player.immunity[SLEEP]++;
}

void i_lembas(std::unique_ptr<object> &)
{
  heal(10);
  cleanse(0);
  Player.food = 40;
}

void i_cure(std::unique_ptr<object> &o)
{
  cure(o->blessing);
}

void i_immune(std::unique_ptr<object> &o)
{
  if(o->blessing > 0)
  {
    queue_message("You feel a sense of innoculation");
    Player.immunity[INFECTION]++;
    cure(o->blessing);
  }
}

void i_breathing(std::unique_ptr<object> &o)
{
  if(o->blessing > -1)
  {
    Objects[o->id].known = 1;
  }
  breathe(o->blessing);
}

void i_invisible(std::unique_ptr<object> &o)
{
  if(o->blessing > -1)
  {
    Objects[o->id].known = 1;
  }
  invisible(o->blessing);
}

void i_perm_invisible(std::unique_ptr<object> &o)
{
  if(o->blessing > -1)
  {
    Objects[o->id].known = 1;
  }
  if(o->used)
  {
    if(o->blessing > -1)
    {
      queue_message("You feel transparent!");
      Player.status[INVISIBLE] += 1500;
    }
    else
    {
      queue_message("You feel a forboding of bodily harm!");
      Player.status[VULNERABLE] += 1500;
    }
  }
  else
  {
    if(o->blessing > -1)
    {
      Player.status[INVISIBLE] -= 1500;
      if(Player.status[INVISIBLE] < 1)
      {
        queue_message("You feel opaque again.");
        Player.status[INVISIBLE] = 0;
      }
    }
    else
    {
      Player.status[VULNERABLE] -= 1500;
      if(Player.status[VULNERABLE] < 1)
      {
        queue_message("You feel less endangered now.");
        Player.status[VULNERABLE] = 0;
      }
    }
  }
}

void i_warp(std::unique_ptr<object> &o)
{
  if(o->blessing > -1)
  {
    Objects[o->id].known = 1;
  }
  warp(o->blessing);
}

void i_alert(std::unique_ptr<object> &o)
{
  if(o->blessing > -1)
  {
    Objects[o->id].known = 1;
    alert(o->blessing);
  }
}

void i_charge(std::unique_ptr<object> &o)
{
  int i;
  if(o->blessing > -1)
  {
    Objects[o->id].known = 1;
  }
  queue_message("A scroll of charging.");
  queue_message("Charge: ");
  i = getitem(STICK);
  if(i != ABORT)
  {
    if(o->blessing < 0)
    {
      queue_message("The stick glows black!");
      Player.possessions[i]->charge = 0;
    }
    else
    {
      queue_message("The stick glows blue!");
      Player.possessions[i]->charge += (random_range(10) + 1) * (o->blessing + 1);
      if(Player.possessions[i]->charge > 99)
      {
        Player.possessions[i]->charge = 99;
      }
    }
  }
}

void i_fear_resist(std::unique_ptr<object> &o)
{
  if(o->blessing > -1)
  {
    Objects[o->id].known = 1;
    if(Player.status[AFRAID] > 0)
    {
      queue_message("You feel stauncher now.");
      Player.status[AFRAID] = 0;
    }
  }
  else if(!p_immune(FEAR))
  {
    queue_message("You panic!");
    Player.status[AFRAID] += random_range(100);
  }
}

// use a thieves pick
void i_pick(std::unique_ptr<object> &o)
{
  int dir;
  int ox, oy;
  o->used = false;
  if((!o->known) && (!Player.rank[THIEVES]))
  {
    queue_message("You have no idea what do with a piece of twisted metal.");
  }
  else
  {
    if(!o->known)
    {
      o->known = 1;
    }
    Objects[o->id].known = 1;
    queue_message("Pick lock:");
    dir = getdir();
    if(dir == ABORT)
    {
      resetgamestatus(SKIP_MONSTERS, GameStatus);
    }
    else
    {
      ox = Player.x + Dirs[0][dir];
      oy = Player.y + Dirs[1][dir];
      if((Level->site[ox][oy].locchar != CLOSED_DOOR) || loc_statusp(ox, oy, SECRET, *Level))
      {
        queue_message("You can't unlock that!");
        resetgamestatus(SKIP_MONSTERS, GameStatus);
      }
      else if(Level->site[ox][oy].aux == LOCKED)
      {
        if(Level->depth == MaxDungeonLevels - 1)
        {
          queue_message("The lock is too complicated for you!!!");
        }
        else if(Level->depth * 2 + random_range(50) < Player.dex + Player.level + Player.rank[THIEVES] * 10)
        {
          queue_message("You picked the lock!");
          Level->site[ox][oy].aux = UNLOCKED;
          lset(ox, oy, CHANGED, *Level);
          gain_experience(std::max(3, static_cast<int>(Level->depth)));
        }
        else
        {
          queue_message("You failed to pick the lock.");
        }
      }
      else
      {
        queue_message("That door is already unlocked!");
      }
    }
  }
}

// use a magic key
void i_key(std::unique_ptr<object> &o)
{
  int dir;
  int ox, oy;
  o->used = false;
  queue_message("Unlock door: ");
  dir = getdir();
  if(dir == ABORT)
  {
    resetgamestatus(SKIP_MONSTERS, GameStatus);
  }
  else
  {
    ox = Player.x + Dirs[0][dir];
    oy = Player.y + Dirs[1][dir];
    if((Level->site[ox][oy].locchar != CLOSED_DOOR) || loc_statusp(ox, oy, SECRET, *Level))
    {
      queue_message("You can't unlock that!");
      resetgamestatus(SKIP_MONSTERS, GameStatus);
    }
    else if(Level->site[ox][oy].aux == LOCKED)
    {
      queue_message("The lock clicks open!");
      Level->site[ox][oy].aux = UNLOCKED;
      lset(ox, oy, CHANGED, *Level);
      o->blessing--;
      if((o->blessing < 0) || (Level->depth == MaxDungeonLevels - 1))
      {
        queue_message("The key disintegrates!");
        if(--o->number < 1)
        {
          o.reset();
        }
      }
      else
      {
        queue_message("Your key glows faintly.");
      }
    }
    else
    {
      queue_message("That door is already unlocked!");
    }
  }
}

void i_corpse(std::unique_ptr<object> &o)
{
  // WDT HACK: there are some comments in this function which need
  // to be backed up with assert().
  // object's charge holds the former monster id
  switch(o->charge)
  {
    case MEND_PRIEST:
    case ITIN_MERCH:
    case GUARD:
    case NPC:
    case MERCHANT:
    case ZERO_NPC:
    case HISCORE_NPC:
    case APPR_NINJA:
    case SNEAK_THIEF:
    case BRIGAND:
    case GENIN:
    case MAST_THIEF:
    case CHUNIN:
    case JONIN: // cannibalism
      queue_message("Yechh! How could you! You didn't even cook him, first!");
      if(Player.alignment > 0)
      {
        Player.food = 25;
      }
      Player.food += 8;
      Player.alignment -= 10;
      foodcheck();
      break;
    case FNORD: // fnord
      queue_message("You feel illuminated!");
      Player.iq++;
      break;
    case DENEBIAN: // denebian slime devil
      queue_message("I don't believe this. You ate Denebian Slime?");
      queue_message("You deserve a horrible wasting death, uncurable by any means!");
      break;
    case DRAGONETTE: // can't get here... i_usef changed to I_FOOD
      queue_message("Those dragon-steaks were fantastic!");
      Player.food = 24;
      foodcheck();
      break;
    case BEHEMOTH:
      queue_message("You feel infinitely more virile now.");
      Player.str  = std::max(Player.str, Player.maxstr + 10);
      Player.food = 24;
      foodcheck();
      break;
    case INVIS_SLAY:
      queue_message("Guess what? You're invisible.");
      if(Player.status[INVISIBLE] < 1000)
      {
        Player.status[INVISIBLE] = 666;
      }
      Player.food += 6;
      foodcheck();
      break;
    case UNICORN:
      queue_message("You ATE a unicorn's horn?!?!?");
      Player.immunity[POISON] = 1000;
      break;
    case HORNET: // can't get here... i_usef changed to I_POISON_FOOD
    case GRUNT:
    case TSETSE: // can't get here... i_usef changed to I_SLEEP_SELF
    case AGGRAVATOR:
    case BLIPPER: // can't get here... i_usef changed to I_TELEPORT
    case GOBLIN:
    case GEEK:
    case NIGHT_GAUNT: // can't get here... i_usef changed to I_POISON_FOOD
    case TOVE:
    case TASMANIAN:
    case JUBJUB: // can't get here... i_usef changed to I_FOOD
    case CATEAGLE:
      queue_message("Well, you forced it down. Not much nutrition, though.");
      Player.food++;
      foodcheck();
      break;
    case SEWER_RAT:
    case PHANTASTICON: // can't get here... i_usef changed to I_POISON_FOOD
    case EYE:          // can't get here... i_usef changed to I_CLAIRVOYANCE
    case NASTY:        // can't get here... i_usef changed to I_INVISIBLE
    case CATOBLEPAS:
    case HAUNT: // can't get here... i_usef changed to I_POISON_FOOD
    case ROUS:
    case DEATHSTAR: // can't get here... i_usef changed to I_POISON_FOOD
    case EATER:
      queue_message("Oh, yuck. The 'food' seems to be tainted.");
      queue_message("You feel very sick. You throw up.");
      Player.food = std::min(Player.food, 4);
      [[fallthrough]];
    default:
      queue_message("It proved completely inedible, but you tried anyhow.");
  }
}

void i_accuracy(std::unique_ptr<object> &o)
{
  if(!o->known)
  {
    o->known = 1;
  }
  Objects[o->id].known = 1;
  accuracy(o->blessing);
}

void i_perm_accuracy(std::unique_ptr<object> &o)
{
  if(!o->known)
  {
    o->known = 1;
  }
  Objects[o->id].known = 1;
  if((o->used) && (o->blessing > -1))
  {
    Player.status[ACCURATE] += 1500;
    queue_message("You feel skillful and see bulls' eyes everywhere.");
  }
  else
  {
    Player.status[ACCURATE] -= 1500;
    if(Player.status[ACCURATE] < 1)
    {
      Player.status[ACCURATE] = 0;
      calc_melee();
      queue_message("Your vision blurs....");
    }
  }
}

void i_hero(std::unique_ptr<object> &o)
{
  if(!o->known)
  {
    o->known = 1;
  }
  Objects[o->id].known = 1;
  hero(o->blessing);
}

void i_perm_hero(std::unique_ptr<object> &o)
{
  if(!o->known)
  {
    o->known = 1;
  }
  Objects[o->id].known = 1;
  if(o->used)
  {
    if(o->blessing > -1)
    {
      Player.status[HERO] += 1500;
      calc_melee();
      queue_message("You feel super!");
    }
    else
    {
      Player.status[HERO] = 0;
      calc_melee();
      if(!Player.immunity[FEAR])
      {
        Player.status[AFRAID] += 1500;
        queue_message("You feel cowardly....");
      }
    }
  }
  else
  {
    if(o->blessing > -1)
    {
      Player.status[HERO] -= 1500;
      if(Player.status[HERO] < 1)
      {
        calc_melee();
        queue_message("You feel less super now.");
        Player.status[HERO] = 0;
      }
    }
    else
    {
      Player.status[AFRAID] -= 1500;
      if(Player.status[AFRAID] < 1)
      {
        queue_message("You finally conquer your fear.");
        Player.status[AFRAID] = 0;
      }
    }
  }
}

void i_levitate(std::unique_ptr<object> &o)
{
  if(!o->known)
  {
    o->known = 1;
  }
  Objects[o->id].known = 1;
  levitate(o->blessing);
}

void i_perm_levitate(std::unique_ptr<object> &o)
{
  if(!o->known)
  {
    o->known = 1;
  }
  Objects[o->id].known = 1;
  if(o->blessing > -1)
  {
    if(o->used)
    {
      Player.status[LEVITATING] += 1400;
      queue_message("You start to float a few inches above the floor");
      queue_message("You find you can easily control your altitude");
    }
    else
    {
      Player.status[LEVITATING] -= 1500;
      if(Player.status[LEVITATING] < 1)
      {
        Player.status[LEVITATING] = 0;
        queue_message("You sink to the floor.");
      }
    }
  }
  else
  {
    i_perm_burden(o);
  }
}

void i_perm_protection(std::unique_ptr<object> &o)
{
  if(o->used)
  {
    if(o->blessing > -1)
    {
      Player.status[PROTECTION] += abs(o->plus) + 1;
    }
    else
    {
      Player.status[PROTECTION] -= abs(o->plus) + 1;
    }
  }
  else
  {
    if(o->blessing > -1)
    {
      Player.status[PROTECTION] -= abs(o->plus) + 1;
    }
    else
    {
      Player.status[PROTECTION] += abs(o->plus) + 1;
    }
  }
  calc_melee();
}

void i_perm_agility(std::unique_ptr<object> &o)
{
  o->known             = 2;
  Objects[o->id].known = 1;
  if(o->used)
  {
    if(o->blessing > -1)
    {
      Player.agi += abs(o->plus) + 1;
    }
    else
    {
      Player.agi -= abs(o->plus) + 1;
    }
  }
  else
  {
    if(o->blessing > -1)
    {
      Player.agi -= abs(o->plus) + 1;
    }
    else
    {
      Player.agi += abs(o->plus) + 1;
    }
  }
  calc_melee();
}

void i_truesight(std::unique_ptr<object> &o)
{
  if(!o->known)
  {
    o->known = 1;
  }
  Objects[o->id].known = 1;
  truesight(o->blessing);
}

void i_perm_truesight(std::unique_ptr<object> &o)
{
  if(!o->known)
  {
    o->known = 1;
  }
  Objects[o->id].known = 1;
  if(o->used)
  {
    if(o->blessing > -1)
    {
      Player.status[TRUESIGHT] += 1500;
      queue_message("You feel sharp!");
    }
    else
    {
      Player.status[BLINDED] += 1500;
      queue_message("You've been blinded!");
    }
  }
  else
  {
    if(o->blessing > -1)
    {
      Player.status[TRUESIGHT] -= 1500;
      if(Player.status[TRUESIGHT] < 1)
      {
        queue_message("You feel less keen now.");
        Player.status[TRUESIGHT] = 0;
      }
    }
    else
    {
      Player.status[BLINDED] -= 1500;
      if(Player.status[BLINDED] < 1)
      {
        queue_message("You can see again!");
        Player.status[BLINDED] = 0;
      }
    }
  }
}

void i_illuminate(std::unique_ptr<object> &o)
{
  if(!o->known)
  {
    o->known = 1;
  }
  Objects[o->id].known = 1;
  illuminate(o->blessing);
}

void i_torch_use(std::unique_ptr<object> &o)
{
  if(!o->used || !o->charge)
  {
    return;
  }
  if(!o->known)
  {
    o->known = 1;
  }
  Objects[o->id].known = 1;
  o->aux               = !o->aux;
  if(o->aux)
  {
    Player.status[ILLUMINATION] += 1500;
  }
  else
  {
    Player.status[ILLUMINATION] = std::max(0, Player.status[ILLUMINATION] - 1500);
  }
}

void i_torch_unequip(std::unique_ptr<object> &o)
{
  if(o->aux)
  {
    o->aux                      = false;
    Player.status[ILLUMINATION] = std::max(0, Player.status[ILLUMINATION] - 1500);
  }
}

void i_perm_illuminate(std::unique_ptr<object> &o)
{
  if(!o->known)
  {
    o->known = 1;
  }
  Objects[o->id].known = 1;
  if(o->used)
  {
    Player.status[ILLUMINATION] += 1500;
  }
  else
  {
    Player.status[ILLUMINATION] = std::max(0, Player.status[ILLUMINATION] - 1500);
  }
}

void i_trap(std::unique_ptr<object> &o)
{
  Objects[o->id].known = 1;

  if((Level->site[Player.x][Player.y].locchar != FLOOR) || (Level->site[Player.x][Player.y].p_locf != L_NO_OP))
  {
    queue_message("Your attempt fails.");
  }
  else if(!o->known)
  {
    queue_message("Fiddling with the thing, you have a small accident....");
    p_movefunction(o->aux);
  }
  else
  {
    queue_message("You successfully set a trap at your location.");
    Level->site[Player.x][Player.y].p_locf = o->aux;
    lset(Player.x, Player.y, CHANGED, *Level);
  }
  if(--o->number < 1)
  {
    o.reset();
  }
}

void i_raise_portcullis(std::unique_ptr<object> &o)
{
  l_raise_portcullis();
  queue_message("The box beeps once and explodes in your hands!");
  if(--o->number < 1)
  {
    o.reset();
  }
}
