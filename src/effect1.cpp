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

/* effect1.c */

#include "glob.h"
#include "scr.h"
#include "spell.h"

#include <algorithm>
#include <cassert>
#include <format>

extern void item_equip(object *);
extern void item_unequip(object *);

/* enchant */
void enchant(int delta)
{
  if(delta < 0)
  {
    int i = random_item();
    if(i == ABORT ||
       (Player.possessions[i]->on_use == I_NOTHING && Player.possessions[i]->on_equip == I_NOTHING) ||
       Player.possessions[i]->on_use == I_NO_OP || Player.possessions[i]->on_use == I_NORMAL_ARMOR ||
       Player.possessions[i]->on_use == I_NORMAL_WEAPON ||
       Player.possessions[i]->on_use == I_NORMAL_SHIELD || Player.possessions[i]->objchar == FOOD ||
       Player.possessions[i]->objchar == MISSILEWEAPON)
    {
      queue_message("You feel fortunate.");
    }
    else if(Player.possessions[i]->blessing < 0 || (Player.possessions[i]->objchar == ARTIFACT && random_range(3)))
    {
      if(Player.possessions[i]->uniqueness == COMMON)
      {
        queue_message("Your ");
      }
      queue_message(itemid(Player.possessions[i]));
      queue_message(" glows, but the glow flickers out...");
    }
    else
    {
      bool used = Player.possessions[i]->used;
      if(used)
      {
        item_unequip(Player.possessions[i]);
      }
      if(Player.possessions[i]->uniqueness == COMMON)
      {
        queue_message("Your ");
      }
      queue_message(itemid(Player.possessions[i]));
      queue_message(" radiates an aura of mundanity!");
      Player.possessions[i]->plus       = 0;
      Player.possessions[i]->charge     = -1;
      Player.possessions[i]->on_use     = I_NOTHING;
      Player.possessions[i]->on_equip   = I_NOTHING;
      Player.possessions[i]->on_unequip = I_NOTHING;
      if(used)
      {
        item_equip(Player.possessions[i]);
      }
    }
  }
  else
  {
    int i = getitem(CASH);
    if(i == ABORT)
    {
      queue_message("You feel unlucky.");
    }
    else if(i == CASHVALUE)
    {
      queue_message("You enchant your money.... What a concept!");
      long change_cash = Player.cash * (random_range(7) - 3) / 6;
      if(change_cash > 0)
      {
        queue_message("Seems to have been a good idea!");
      }
      else
      {
        queue_message("Maybe it wasn't such a good idea....");
      }
      Player.cash += change_cash;
    }
    else if(Player.possessions[i]->objchar == ARTIFACT)
    {
      if(Player.possessions[i]->on_use != Objects[Player.possessions[i]->id].on_use)
      {
        queue_message("It re-acquires its magical aura!");
        Player.possessions[i]->on_use = Objects[Player.possessions[i]->id].on_use;
      }
      else
      {
        queue_message("The enchantment spell enfolds the ");
        queue_message(itemid(Player.possessions[i]));
        queue_message("and the potent enchantment of the Artifact causes a backlash!");
        manastorm(Player.x, Player.y, Player.possessions[i]->level * 5);
      }
    }
    else
    {
      if(Player.possessions[i]->plus > random_range(20) + 1)
      {
        queue_message("Uh-oh, the force of the enchantment was too much!");
        queue_message("There is a loud explosion!");
        manastorm(Player.x, Player.y, Player.possessions[i]->plus * 5);
        dispose_lost_objects(1, Player.possessions[i]);
      }
      else
      {
        bool used = (Player.possessions[i]->used);
        if(used)
        {
          setgamestatus(SUPPRESS_PRINTING, GameStatus);
          item_unequip(Player.possessions[i]);
          resetgamestatus(SUPPRESS_PRINTING, GameStatus);
        }
        queue_message("The item shines!");
        Player.possessions[i]->plus += delta + 1;
        if(Player.possessions[i]->charge > -1 && Player.possessions[i]->objchar == STICK)
        {
          Player.possessions[i]->charge += (delta + 1) * (random_range(10) + 1);
        }
        if(used)
        {
          setgamestatus(SUPPRESS_PRINTING, GameStatus);
          item_equip(Player.possessions[i]);
          resetgamestatus(SUPPRESS_PRINTING, GameStatus);
        }
      }
    }
    calc_melee();
  }
}

/* bless */
void bless(int blessing)
{
  if(blessing < 0)
  {
    int index = random_item();
    if(index == ABORT)
    {
      queue_message("You feel fortunate.");
    }
    else
    {
      queue_message("A foul odor arises from ");
      if(Player.possessions[index]->uniqueness == COMMON)
      {
        queue_message("your ");
      }
      queue_message(itemid(Player.possessions[index]));
      if(Player.possessions[index]->used)
      {
        setgamestatus(SUPPRESS_PRINTING, GameStatus);
        item_unequip(Player.possessions[index]);
        resetgamestatus(SUPPRESS_PRINTING, GameStatus);
      }
      Player.possessions[index]->blessing -= 2;
      if(Player.possessions[index]->blessing < 0)
      {
        Player.possessions[index]->plus = abs(Player.possessions[index]->plus) - 1;
      }
      if(Player.possessions[index]->used)
      {
        setgamestatus(SUPPRESS_PRINTING, GameStatus);
        item_equip(Player.possessions[index]);
        resetgamestatus(SUPPRESS_PRINTING, GameStatus);
      }
    }
  }
  else
  {
    int index = getitem(NULL_ITEM);
    if(index == CASHVALUE)
    {
      queue_message("Blessing your money has no effect.");
    }
    else if(index != ABORT)
    {
      bool used = Player.possessions[index]->used;
      if(used)
      {
        setgamestatus(SUPPRESS_PRINTING, GameStatus);
        item_unequip(Player.possessions[index]);
        resetgamestatus(SUPPRESS_PRINTING, GameStatus);
      }
      queue_message("A pure white light surrounds the item... ");
      if(Player.possessions[index]->blessing < 0 - (blessing + 1))
      {
        queue_message("which is evil enough to resist the effect of the blessing!");
      }
      else if(Player.possessions[index]->blessing < -1)
      {
        queue_message("which disintegrates under the influence of the holy aura!");
        Player.itemweight -= Player.possessions[index]->weight;
        dispose_lost_objects(1, Player.possessions[index]);
      }
      else if(Player.possessions[index]->blessing < blessing + 1)
      {
        queue_message("which now seems affected by afflatus!");
        Player.possessions[index]->blessing++;
        Player.possessions[index]->plus = abs(Player.possessions[index]->plus) + 1;
      }
      else
      {
        queue_message("The hierolux fades without any appreciable effect....");
      }
      if(used && Player.possessions[index])
      {
        setgamestatus(SUPPRESS_PRINTING, GameStatus);
        item_equip(Player.possessions[index]);
        resetgamestatus(SUPPRESS_PRINTING, GameStatus);
      }
    }
  }
  calc_melee();
}

void heal(int amount)
{
  if(amount > -1)
  {
    queue_message("You feel better.");
    if(Player.hp < Player.maxhp + amount)
    {
      Player.hp += random_range(10 * amount) + 1;
      if(Player.hp > Player.maxhp)
      {
        Player.hp = Player.maxhp + amount;
      }
    }
    Player.status[BLINDED] = 0;
  }
  else
  {
    queue_message("You feel unwell.");
    Player.hp -= random_range(10 * abs(amount) + 1);
    if(Player.hp < 0)
    {
      p_death("magical disruption");
    }
  }
  dataprint();
}

// from f to t
void bolt(int fx, int fy, int tx, int ty, int hit, int dmg, damage_type dtype)
{
  int xx, yy;
  monster *target;
  chtype boltchar;
  xx = fx;
  yy = fy;

  switch(dtype)
  {
    case FLAME:
      boltchar = ('*' | CLR(LIGHT_RED));
      break;
    case ELECTRICITY:
      boltchar = ('^' | CLR(LIGHT_BLUE));
      break;
    case NORMAL_DAMAGE:
      boltchar = ('!' | CLR(BROWN));
      break;
    case COLD:
      boltchar = ('o' | CLR(WHITE));
      break;
    default:
      assert(false); /* this should never happen, right? WDT */
  }

  do_los(boltchar, &xx, &yy, tx, ty);

  if((xx == Player.x) && (yy == Player.y))
  {
    if(Player.status[DEFLECTION] > 0)
    {
      queue_message("The bolt just missed you!");
    }
    else
    {
      switch(dtype)
      {
        case FLAME:
          queue_message("You were blasted by a firebolt!");
          p_damage(random_range(dmg), dtype, "a firebolt");
          break;
        case ELECTRICITY:
          queue_message("You were zapped by lightning!");
          p_damage(random_range(dmg), dtype, "a bolt of lightning");
          break;
        case NORMAL_DAMAGE:
          queue_message("You were hit by a missile!");
          p_damage(random_range(dmg), dtype, "a missile");
          break;
        case COLD:
          queue_message("You were hit by an icicle!");
          p_damage(random_range(dmg), dtype, "an icicle");
          break;
        default:
          break;
      }
    }
  }
  else if((target = Level->site[xx][yy].creature))
  {
    if(hitp(hit, target->ac))
    {
      std::string damage_message;
      if(target->uniqueness == COMMON)
      {
        damage_message = std::format("The {}", target->monstring);
      }
      else
      {
        damage_message = target->monstring;
      }
      switch(dtype)
      {
          /* WDT: these sentances really ought to be livened up.  Especially
         * in full verbose mode. */
        case FLAME:
          damage_message += " was blasted by a firebolt!";
          break;
        case ELECTRICITY:
          damage_message += " was zapped by lightning!";
          break;
        case NORMAL_DAMAGE:
          damage_message += " was hit by a missile!";
          break;
        case COLD:
          damage_message += " was hit by an icicle!";
          break;
        default:
          break;
      }
      queue_message(damage_message);
      m_status_set(*target, HOSTILE);
      m_damage(target, random_range(dmg), dtype);
    }
    else
    {
      std::string miss_message;
      if(target->uniqueness == COMMON)
      {
        miss_message = std::format("The {}", target->monstring);
      }
      else
      {
        miss_message = target->monstring;
      }
      switch(dtype)
      {
        case FLAME:
          miss_message += " was missed by a firebolt!";
          break;
        case ELECTRICITY:
          miss_message += " was missed by lightning!";
          break;
        case NORMAL_DAMAGE:
          miss_message += " was missed by a missile!";
          break;
        case COLD:
          miss_message += " was missed by a flying icicle!";
          break;
        default:
          break;
      }
      queue_message(miss_message);
    }
  }
  else if(Level->site[xx][yy].locchar == HEDGE)
  {
    if(Level->site[xx][yy].p_locf != L_TRIFFID)
    {
      if((dtype == FLAME) || (dtype == ELECTRICITY))
      {
        queue_message("The hedge is blasted away!");
        Level->site[xx][yy].p_locf  = L_NO_OP;
        Level->site[xx][yy].locchar = FLOOR;
        plotspot(xx, yy, true);
        lset(xx, yy, CHANGED, *Level);
      }
      else
      {
        queue_message("The hedge is unaffected.");
      }
    }
    else
    {
      queue_message("The triffid absorbs the energy and laughs!");
    }
  }
  else if(Level->site[xx][yy].locchar == WATER)
  {
    if(dtype == FLAME)
    {
      queue_message("The water is vaporised!");
      Level->site[xx][yy].p_locf  = L_NO_OP;
      Level->site[xx][yy].locchar = FLOOR;
      lset(xx, yy, CHANGED, *Level);
    }
  }
}

void fbolt(int fx, int fy, int tx, int ty, int hit, int dmg)
{
  bolt(fx, fy, tx, ty, hit, dmg, FLAME);
}

void lbolt(int fx, int fy, int tx, int ty, int hit, int dmg)
{
  bolt(fx, fy, tx, ty, hit, dmg, ELECTRICITY);
}

void icebolt(int fx, int fy, int tx, int ty, int hit, int dmg)
{
  bolt(fx, fy, tx, ty, hit, dmg, COLD);
}

void nbolt(int fx, int fy, int tx, int ty, int hit, int dmg)
{
  bolt(fx, fy, tx, ty, hit, dmg, NORMAL_DAMAGE);
}

// from f to t
void ball(int fx, int fy, int tx, int ty, int dmg, damage_type dtype)
{
  int xx, yy, ex, ey, i;
  monster *target;
  chtype expchar = ('@' | CLR(LIGHT_PURPLE));

  xx = fx;
  yy = fy;

  switch(dtype)
  {
    case FLAME:
      expchar = ('*' | CLR(LIGHT_RED));
      break;
    case COLD:
      expchar = ('o' | CLR(WHITE));
      break;
    case ELECTRICITY:
      expchar = ('^' | CLR(LIGHT_BLUE));
      break;
    default:
      break;
  }

  do_los(expchar, &xx, &yy, tx, ty);
  draw_explosion(expchar, xx, yy);
  for(i = 0; i < 9; i++)
  {
    ex = xx + Dirs[0][i];
    ey = yy + Dirs[1][i];

    if((ex == Player.x) && (ey == Player.y))
    {
      switch(dtype)
      {
        case FLAME:
          queue_message("You were blasted by a fireball!");
          p_damage(random_range(dmg), FLAME, "a fireball");
          break;
        case COLD:
          queue_message("You were blasted by a snowball!");
          p_damage(random_range(dmg), COLD, "a snowball");
          break;
        case ELECTRICITY:
          queue_message("You were blasted by ball lightning!");
          p_damage(random_range(dmg), ELECTRICITY, "ball lightning");
          break;
        case UNSTOPPABLE:
          queue_message("Oh No! Manastorm!");
          p_damage(random_range(dmg), UNSTOPPABLE, "a manastorm!");
          break;
        default:
          break;
      }
    }
    if((target = Level->site[ex][ey].creature))
    {
      if(los_p(Player.x, Player.y, target->x, target->y))
      {
        std::string hit_message;
        if(target->uniqueness == COMMON)
        {
          hit_message = std::format("The {}", target->monstring);
        }
        else
        {
          hit_message = target->monstring;
        }
        switch(dtype)
        {
          case FLAME:
            hit_message += " was zorched by a fireball!";
            break;
          case COLD:
            hit_message += " was blasted by a snowball!";
            break;
          case ELECTRICITY:
            hit_message += " was zapped by ball lightning!";
            break;
          case UNSTOPPABLE:
            hit_message += " was nuked by a manastorm!";
            break;
          default:
            break;
        }
        queue_message(hit_message);
      }
      m_status_set(*target, HOSTILE);
      m_damage(target, random_range(dmg), dtype);
    }
    if(Level->site[ex][ey].locchar == HEDGE)
    {
      if(Level->site[ex][ey].p_locf != L_TRIFFID)
      {
        if((dtype == FLAME) || (dtype == ELECTRICITY))
        {
          queue_message("The hedge is blasted away!");
          Level->site[ex][ey].p_locf  = L_NO_OP;
          Level->site[ex][ey].locchar = FLOOR;
          plotspot(ex, ey, true);
          lset(ex, ey, CHANGED, *Level);
        }
        else
        {
          queue_message("The hedge is unaffected.");
        }
      }
      else
      {
        queue_message("The triffid absorbs the energy and laughs!");
      }
    }
    else if(Level->site[ex][ey].locchar == WATER)
    {
      if(dtype == FLAME)
      {
        queue_message("The water is vaporised!");
        Level->site[ex][ey].p_locf  = L_NO_OP;
        Level->site[ex][ey].locchar = FLOOR;
        plotspot(ex, ey, true);
        lset(ex, ey, CHANGED, *Level);
      }
    }
  }
}

void lball(int fx, int fy, int tx, int ty, int dmg)
{
  ball(fx, fy, tx, ty, dmg, ELECTRICITY);
}

void manastorm(int x, int y, int dmg)
{
  ball(x, y, x, y, dmg, UNSTOPPABLE);
}

void snowball(int fx, int fy, int tx, int ty, int dmg)
{
  ball(fx, fy, tx, ty, dmg, COLD);
}

void fball(int fx, int fy, int tx, int ty, int dmg)
{
  ball(fx, fy, tx, ty, dmg, FLAME);
}

void mondet(int blessing)
{
  pml ml;
  for(ml = Level->mlist; ml; ml = ml->next)
  {
    if(ml->m->hp > 0) /* FIXED 12/30/98 DG */
    {
      if(blessing > -1)
      {
        plotmon(ml->m);
      }
      else
      {
        putspot(random_range(WIDTH), random_range(LENGTH), Monsters[random_range(NUMMONSTERS)].monchar);
      }
    }
  }
  levelrefresh();
  get_message_input();
  show_screen();
}

void objdet(int blessing)
{
  for(int i = 0; i < WIDTH; ++i)
  {
    for(int j = 0; j < LENGTH; ++j)
    {
      if(Level->site[i][j].things)
      {
        if(blessing < 0)
        {
          putspot(random_range(WIDTH), random_range(LENGTH), Level->site[i][j].things->thing->objchar);
        }
        else
        {
          putspot(i, j, Level->site[i][j].things->thing->objchar);
        }
      }
    }
  }
  levelrefresh();
  get_message_input();
  show_screen();
}

void identify(int blessing)
{
  int index;

  if(blessing == 0)
  {
    queue_message("Identify:");
    index = getitem(NULL_ITEM);
    if(index == CASHVALUE)
    {
      queue_message("Your money is really money.");
    }
    else if(index == ABORT)
    {
      setgamestatus(SKIP_MONSTERS, GameStatus);
    }
    else
    {
      if(Player.possessions[index]->objchar == FOOD)
      {
        Player.possessions[index]->known = 1;
      }
      else
      {
        Player.possessions[index]->known             = 2;
        Objects[Player.possessions[index]->id].known = 1;
      }
      queue_message("Identified: ");
      queue_message(itemid(Player.possessions[index]));
    }
  }
  else if(blessing < 0)
  {
    queue_message("You feel forgetful.");
    for(index = 0; index < MAXITEMS; index++)
    {
      if(Player.possessions[index])
      {
        Player.possessions[index]->known             = 0;
        Objects[Player.possessions[index]->id].known = 0;
      }
    }
  }
  else
  {
    queue_message("You feel encyclopaedic.");
    for(index = 0; index < MAXITEMS; index++)
    {
      if(Player.possessions[index])
      {
        if(Player.possessions[index]->objchar == FOOD)
        {
          Player.possessions[index]->known = 1;
        }
        else
        {
          Player.possessions[index]->known             = 2;
          Objects[Player.possessions[index]->id].known = 1;
        }
      }
    }
    for(index = 0; index < Player.packptr; index++)
    {
      if(Player.pack[index])
      {
        if(Player.pack[index]->objchar == FOOD)
        {
          Player.pack[index]->known = 1;
        }
        else
        {
          Player.pack[index]->known             = 2;
          Objects[Player.pack[index]->id].known = 1;
        }
      }
    }
  }
  calc_melee();
}

/* returns index of random item, ABORT if player carrying none */
int random_item()
{
  int item = ABORT, tries = 0;
  int number = 0;

  for(tries = 0; tries < MAXITEMS; tries++)
  {
    if(Player.possessions[tries])
    {
      number++;
      if(random_range(number) == 0)
      {
        item = tries;
      }
    }
  }
  return (item);
}

/* various kinds of wishes */
void wish(int blessing)
{
  std::string wishstr;
  queue_message("What do you wish for? ");
  if(blessing < 0)
  {
    deathprint();
  }
  else
  {
    wishstr = msgscanstring();
  }
  if(blessing < 0 || wishstr == "Death")
  {
    queue_message("As you wish, so shall it be.");
    p_death("a deathwish");
  }
  if(wishstr == "Power")
  {
    queue_message("You feel a sudden surge of energy");
    Player.mana = calcmana() * 10;
  }
  else if(wishstr == "Skill")
  {
    queue_message("You feel more competent.");
    if(gamestatusp(CHEATED, GameStatus))
    {
      gain_experience(10000);
    }
    else
    {
      gain_experience(std::min(10000l, Player.xp));
    }
  }
  else if(wishstr == "Wealth")
  {
    queue_message("You are submerged in shower of gold pieces!");
    Player.cash += 10000;
  }
  else if(wishstr == "Balance")
  {
    queue_message("You feel neutral.");
    Player.alignment = 0;
  }
  else if(wishstr == "Chaos")
  {
    queue_message("You feel chaotic.");
    Player.alignment -= 25;
  }
  else if(wishstr == "Law")
  {
    queue_message("You feel lawful.");
    Player.alignment += 25;
  }
  else if(wishstr == "Location")
  {
    strategic_teleport(1);
  }
  else if(wishstr == "Knowledge")
  {
    queue_message("You feel more knowledgeable.");
    int i = random_range(spell::NUM_SPELLS);
    if(spell::Spells[i].known)
    {
      spell::Spells[i].powerdrain = (std::max(1, spell::Spells[i].powerdrain / 2));
    }
    else
    {
      spell::Spells[i].known = true;
    }
  }
  else if(wishstr == "Health")
  {
    queue_message("You feel vigorous");
    Player.hp               = std::max(Player.hp, Player.maxhp);
    Player.status[DISEASED] = 0;
    Player.status[POISONED] = 0;
  }
  else if(wishstr == "Destruction")
  {
    annihilate(gamestatusp(CHEATED, GameStatus));
  }
  else if(wishstr == "Acquisition")
  {
    acquire(gamestatusp(CHEATED, GameStatus));
  }
  else if(wishstr == "Summoning")
  {
    summon(gamestatusp(CHEATED, GameStatus), -1);
  }
  else if(wishstr == "Stats" && gamestatusp(CHEATED, GameStatus))
  {
    Player.str = Player.maxstr = Player.con = Player.maxcon = Player.agi = Player.maxagi = Player.dex =
      Player.maxdex = Player.iq = Player.maxiq = Player.pow = Player.maxpow = 200;
    calc_melee();
  }
  else
  {
    queue_message("You feel stupid.");
  }
  dataprint();
  showflags();
}

/* gain for an item */
void acquire(int blessing)
{
  if(blessing < 0)
  {
    int index = random_item();
    if(index == ABORT)
    {
      queue_message("You feel fortunate.");
    }
    else
    {
      queue_message("Smoke drifts out of your pack.... ");
      queue_message("Destroyed: ");
      queue_message(itemid(Player.possessions[index]));
      dispose_lost_objects(1, Player.possessions[index]);
    }
  }
  else
  {
    object *newthing = new object;
    newthing->id     = -1;
    if(gamestatusp(CHEATED, GameStatus))
    {
      queue_message("Acquire which kind of item: !?][}{)/=%%\\& ");
    }
    else
    {
      queue_message("Acquire which kind of item: !?][}{)/=%%\\ ");
    }
    int id     = ABORT;
    char otype = mgetc();
    switch(otype)
    {
      case(POTION & 0xff):
        if(blessing > 0)
        {
          id = itemlist(POTIONID, NUMPOTIONS);
        }
        else
        {
          id = random_range(NUMPOTIONS);
        }
        if(id < 0)
        {
          queue_message("You feel stupid.");
        }
        else
        {
          make_potion(newthing, id);
        }
        break;
      case(SCROLL & 0xff):
        if(blessing > 0)
        {
          id = itemlist(SCROLLID, NUMSCROLLS);
        }
        else
        {
          id = random_range(NUMSCROLLS);
        }
        if(id < 0)
        {
          queue_message("You feel stupid.");
        }
        else
        {
          make_scroll(newthing, id);
        }
        break;
      case(RING & 0xff):
        if(blessing > 0)
        {
          id = itemlist(RINGID, NUMRINGS);
        }
        else
        {
          id = random_range(NUMRINGS);
        }
        if(id < 0)
        {
          queue_message("You feel stupid.");
        }
        else
        {
          make_ring(newthing, id);
        }
        break;
      case(STICK & 0xff):
        if(blessing > 0)
        {
          id = itemlist(STICKID, NUMSTICKS);
        }
        else
        {
          id = random_range(NUMSTICKS);
        }
        if(id < 0)
        {
          queue_message("You feel stupid.");
        }
        else
        {
          make_stick(newthing, id);
        }
        break;
      case(ARMOR & 0xff):
        if(blessing > 0)
        {
          id = itemlist(ARMORID, NUMARMOR);
        }
        else
        {
          id = random_range(NUMARMOR);
        }
        if(id < 0)
        {
          queue_message("You feel stupid.");
        }
        else
        {
          make_armor(newthing, id);
        }
        break;
      case(SHIELD & 0xff):
        if(blessing > 0)
        {
          id = itemlist(SHIELDID, NUMSHIELDS);
        }
        else
        {
          id = random_range(NUMSHIELDS);
        }
        if(id < 0)
        {
          queue_message("You feel stupid.");
        }
        else
        {
          make_shield(newthing, id);
        }
        break;
      case(WEAPON & 0xff):
        if(blessing > 0)
        {
          id = itemlist(WEAPONID, NUMWEAPONS);
        }
        else
        {
          id = random_range(NUMWEAPONS);
        }
        if(id < 0)
        {
          queue_message("You feel stupid.");
        }
        else
        {
          make_weapon(newthing, id);
        }
        break;
      case(BOOTS & 0xff):
        if(blessing > 0)
        {
          id = itemlist(BOOTID, NUMBOOTS);
        }
        else
        {
          id = random_range(NUMBOOTS);
        }
        if(id < 0)
        {
          queue_message("You feel stupid.");
        }
        else
        {
          make_boots(newthing, id);
        }
        break;
      case(CLOAK & 0xff):
        if(blessing > 0)
        {
          id = itemlist(CLOAKID, NUMCLOAKS);
        }
        else
        {
          id = random_range(NUMCLOAKS);
        }
        if(id < 0)
        {
          queue_message("You feel stupid.");
        }
        else
        {
          make_cloak(newthing, id);
        }
        break;
      case(FOOD & 0xff):
        if(blessing > 0)
        {
          id = itemlist(FOODID, NUMFOODS);
        }
        else
        {
          id = random_range(NUMFOODS);
        }
        if(id < 0)
        {
          queue_message("You feel stupid.");
        }
        else
        {
          make_food(newthing, id);
        }
        break;
      case(THING & 0xff):
        if(blessing > 0)
        {
          id = itemlist(THINGID, NUMTHINGS);
        }
        else
        {
          id = random_range(NUMTHINGS);
        }
        if(id < 0)
        {
          queue_message("You feel stupid.");
        }
        else
        {
          make_thing(newthing, id);
        }
        break;
      case(ARTIFACT & 0xff):
        if(gamestatusp(CHEATED, GameStatus))
        {
          id = itemlist(ARTIFACTID, NUMARTIFACTS);
        }
        else
        {
          id = -1;
        }
        if(id < 0)
        {
          queue_message("You feel stupid.");
        }
        else
        {
          make_artifact(newthing, id);
        }
        break;
      default:
        queue_message("You feel stupid.");
    }
    xredraw();
    if(id != ABORT)
    {
      if(blessing > 0)
      {
        newthing->known   = 2;
        Objects[id].known = 1;
      }
      newthing->used = false;
      gain_item(newthing);
    }
    else
    {
      delete newthing;
    }
  }
}
