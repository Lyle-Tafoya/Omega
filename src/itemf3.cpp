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

// itemf3.cpp

// mostly artifact and stick functions

#include "glob.h"
#include "scr.h"
#include "spell.h"

#include <algorithm>

// amulet of the planes
void i_planes(std::unique_ptr<object> &)
{
  if(Player.mana < 1)
  {
    queue_message("The amulet spits some multicolored sparks.");
  }
  else
  {
    queue_message("You focus mana into the amulet....");
    Player.mana = std::max(0l, Player.mana - 100);
    dataprint();
    strategic_teleport(1);
  }
}

// the sceptre of high magic
void i_sceptre(std::unique_ptr<object> &)
{
  if(HiMagicUse == Date)
  {
    queue_message("The Sceptre makes a sort of dull 'thut' noise.");
  }
  else if(Current_Environment == E_CIRCLE || Current_Environment == E_ASTRAL)
  {
    HiMagicUse = Date; // WDT: this looks like it's a good place to use the batteries
    queue_message("The Sceptre warps strangely for a second, and then subsides.");
    queue_message("You smell ozone."); // WDT: explain the battery use
  }
  else
  {
    HiMagicUse = Date;
    queue_message("With a shriek of tearing aether, a magic portal appears!");
    queue_message("Step through? [yn] ");
    if(ynq() == 'y')
    {
      change_environment(E_COURT);
    }
    queue_message("The sceptre seems to subside. You hear a high whine, as of");
    queue_message("capacitors beginning to recharge.");
  }
}

// the star gem
void i_stargem(std::unique_ptr<object> &o)
{
  if(StarGemUse == Date)
  {
    queue_message("The Star Gem glints weakly as if to say:");
    queue_message("'You have used me overmuch.'");
    queue_message("and it vanishes a puff of regret.");
    Objects[o->id].uniqueness = UNIQUE_UNMADE;
    // it's now out there, somewhere
    if(--o->number < 1)
    {
      o.reset();
    }
  }
  else
  {
    StarGemUse = Date;
    if(o->blessing < 1)
    {
      queue_message("The Star Gem shines brightly and emits a musical tone.");
      queue_message("You see a dark cloud roil away from it.");
      o->blessing = 10;
    }
    queue_message("The star gem flares with golden light!");
    if(Player.status[ILLUMINATION] < 1000)
    {
      queue_message("Interesting, you seem to be permanently accompanied");
      queue_message("by a friendly lambent glow....");
      Player.status[ILLUMINATION] = 1500;
    }
    queue_message("You suddenly find yourself whisked away by some unknown force!");
    setgamestatus(COMPLETED_ASTRAL, GameStatus);
    change_environment(E_COUNTRYSIDE);
    Player.x = 61;
    Player.y = 3;
    screencheck(Player.x, Player.y);
    drawvision(Player.x, Player.y);
    locprint("Star Peak");
    Country[Player.x][Player.y].current_terrain_type = Country[Player.x][Player.y].base_terrain_type;
    c_set(Player.x, Player.y, CHANGED, Country);
    queue_message("The Star Gem's brilliance seems to fade.");
  }
}

// wand of fear
void i_fear(std::unique_ptr<object> &o)
{
  int x = Player.x, y = Player.y;
  Objects[o->id].known = 1;
  o->known             = std::max(1, static_cast<int>(o->known));
  setspot(x, y);
  if(o->blessing < 0)
  {
    x = Player.x;
    y = Player.y;
  }
  inflict_fear(x, y);
}

void i_juggernaut(std::unique_ptr<object> &o)
{
  int d, x = Player.x, y = Player.y;
  int seen = 1, not_seen = 0;
  int tunneled = 0;

  queue_message("You activate the Juggernaut of Karnak!");
  if(!o->known)
  {
    queue_message("Uh, oh, it's coming this way!");
    p_death("the Juggernaut of Karnak");
  }
  else
  {
    d = getdir();
    if(d == ABORT)
    {
      queue_message("You deactivate the Juggernaut before it escapes.");
    }
    else
    {
      queue_message("Vroom! ");
      int cursor_visibility = curs_set(1);
      while(inbounds(x + Dirs[0][d], y + Dirs[1][d]))
      {
        x += Dirs[0][d];
        y += Dirs[1][d];
        if(!view_unblocked(x, y) || offscreen(x, y))
        {
          seen = 0;
        }
        if(Level->site[x][y].locchar == WALL)
        {
          tunneled++;
        }
        if(Level->site[x][y].locchar != WATER && Level->site[x][y].locchar != VOID_CHAR &&
           Level->site[x][y].locchar != ABYSS && Level->site[x][y].locchar != SPACE &&
           Level->site[x][y].locchar != LAVA)
        {
          Level->site[x][y].locchar = FLOOR;
          Level->site[x][y].p_locf  = L_NO_OP;
        }
        lreset(x, y, SECRET, *Level);
        lset(x, y, CHANGED, *Level);
        if(Level->site[x][y].creature)
        {
          if(seen)
          {
            queue_message("Splat! ");
          }
          else
          {
            not_seen++;
          }
          setgamestatus(SUPPRESS_PRINTING, GameStatus);
          m_death(Level->site[x][y].creature);
          resetgamestatus(SUPPRESS_PRINTING, GameStatus);
        }
        plotspot(x, y, false);
        omshowcursor(x, y);
      }
      curs_set(cursor_visibility);
      if(not_seen > 6)
      {
        queue_message("You hear many distant screams...");
      }
      else if(not_seen > 3)
      {
        queue_message("You hear several distant screams...");
      }
      else if(not_seen > 1)
      {
        queue_message("You hear a couple of distant screams...");
      }
      else if(not_seen == 1)
      {
        queue_message("You hear a distant scream...");
      }
      gain_experience(1000);
      if(--o->number < 1)
      {
        o.reset();
      }
      Level->tunnelled += tunneled - 1;
      tunnelcheck();
    }
  }
}

void i_symbol(std::unique_ptr<object> &o)
{
  if(!o->known)
  {
    queue_message("Nothing seems to happen.");
    // if o->charge != 17, then symbol was stolen from own high priest!
  }
  else if((o->aux != Player.patron) || (o->charge != 17))
  {
    queue_message("You invoke the deity...");
    queue_message("...who for some reason seems rather annoyed at you...");
    queue_message("You are enveloped in Godsfire!");
    for(; Player.hp > 1; Player.hp--)
    {
      dataprint();
      for(int i = 0; i < MAXITEMS; ++i)
      {
        if(Player.possessions[i])
        {
          dispose_lost_objects(Player.possessions[i]->number, i);
        }
      }
      Player.mana = 0;
    }
  }
  else if(SymbolUseDay == day() && SymbolUseHour == hour())
  {
    queue_message("Your deity frowns upon this profligate use of power...");
    queue_message("Shazam! A bolt of Godsfire! Your symbol shatters!");
    o.reset();
    Player.hp = 1;
    dataprint();
  }
  else
  {
    queue_message("A mystic flow of theurgic energy courses through your body!");
    SymbolUseDay  = day();
    SymbolUseHour = hour();
    cleanse(1);
    heal(10);
    Player.mana = std::max(Player.mana, calcmana());
  }
}

void i_crystal(std::unique_ptr<object> &o)
{
  if(!o->known)
  {
    queue_message("You can't figure out how to activate this orb.");
  }
  else
  {
    queue_message("You gaze into your crystal ball.");
    if(ViewDay == day() && ViewHour == hour())
    {
      queue_message("All you get is Gilligan's Island reruns.");
    }
    else if((o->blessing < 0) || (Player.iq + Player.level < random_range(30)))
    {
      ViewDay  = day();
      ViewHour = hour();
      queue_message("Weird interference patterns from the crystal fog your mind....");
      amnesia();
    }
    else
    {
      ViewDay  = day();
      ViewHour = hour();
      queue_message("You sense the presence of life...");
      mondet(1);
      queue_message("You sense the presence of objects...");
      objdet(1);
      queue_message("You begin to see visions of things beyond your ken....");
      hint();
    }
  }
}

void i_antioch(std::unique_ptr<object> &o)
{
  int x = Player.x, y = Player.y;
  int count;
  if(!o->known)
  {
    queue_message("Ka-Boom!");
    queue_message("You seem to have annihilated yourself.");
    p_death("the Holy Hand-Grenade of Antioch");
  }
  else
  {
    queue_message("Bring out the Holy Hand-Grenade of Antioch!");
    setspot(x, y);
    queue_message("Ok, you pull the pin.....");
    queue_message("What do you count up to? ");
    count = (int)parsenum();
    if(count < 3 && Level->site[x][y].creature)
    {
      queue_message("`Three shall be the number of thy counting....");
      queue_message("And the number of thy counting shall be three.'");
      queue_message("Your target picks up the grenade and throws it back!");
      queue_message("Ka-Boom!");
      p_death("the Holy Hand-Grenade of Antioch");
    }
    else if(count > 3)
    {
      queue_message("`Three shall be the number of thy counting.");
      queue_message("And the number of thy counting shall be three.'");
      queue_message("Ka-Boom!");
      p_death("the Holy Hand-Grenade of Antioch");
    }
    else
    {
      queue_message("Ka-Boom!");
      gain_experience(1000);
      Level->site[x][y].locchar = TRAP;
      Level->site[x][y].p_locf  = L_TRAP_DOOR;
      lset(x, y, CHANGED, *Level);
      if(Level->site[x][y].creature)
      {
        m_death(Level->site[x][y].creature);
        queue_message("You are covered with gore.");
      }
      Level->site[x][y].things.clear();
    }
  }
  if(--o->number < 1)
  {
    o.reset();
  }
}

void i_kolwynia(std::unique_ptr<object> &o)
{
  if(!o->known)
  {
    queue_message("You destroy youself with a mana storm. How sad.");
    p_death("Kolwynia, The Key That Was Lost");
  }
  else
  {
    gain_experience(5000);
    queue_message("You seem to have gained complete mastery of magic.");
    Player.pow = Player.maxpow = 2 * Player.maxpow;
    for(int i = 0; i < spell::NUM_SPELLS; ++i)
    {
      spell::Spells[i].known = true;
    }
  }
  if(--o->number < 1)
  {
    o.reset();
  }
}

void i_enchantment(std::unique_ptr<object> &o)
{
  char response;
  if(ZapDay == day() && ZapHour == hour())
  {
    queue_message("The staff doesn't seem to have recharged yet.");
  }
  else if(!o->known)
  {
    ZapDay  = day();
    ZapHour = hour();
    queue_message("You blast the staff backwards....");
    dispel(-1);
  }
  else
  {
    ZapDay  = day();
    ZapHour = hour();
    queue_message("Zap with white or black end [wb] ");
    do
    {
      response = (char)mcigetc();
    } while((response != 'w') && (response != 'b'));
    queue_message("The staff discharges!");
    if(response == 'w')
    {
      enchant(o->blessing * 2 + 1);
    }
    else
    {
      dispel(o->blessing);
    }
  }
}

void i_helm(std::unique_ptr<object> &o)
{
  if(HelmDay == day() && HelmHour == hour())
  {
    queue_message("The helm doesn't seem to have recharged yet.");
  }
  else if(!o->known)
  {
    HelmDay  = day();
    HelmHour = hour();
    queue_message("You put the helm on backwards....");
    p_teleport(-1);
  }
  else
  {
    HelmDay  = day();
    HelmHour = hour();
    queue_message("Your environment fades.... and rematerializes.");
    p_teleport(o->blessing);
  }
}

void i_death(std::unique_ptr<object> &)
{
  queue_message("Bad move...");
  p_death("the Potion of Death");
}

void i_life(std::unique_ptr<object> &o)
{
  queue_message("Good move.");
  Player.hp = Player.maxhp = 2 * Player.maxhp;
  if(--o->number < 1)
  {
    o.reset();
  }
}

// f = fire, w = water, e = earth, a = air, m = mastery
int orbcheck(char element)
{
  char response;
  queue_message("The orb begins to glow with increasing intensity!");
  queue_message("You have the feeling you need to do something more....");
  queue_message("Burn it in fire [f] ");
  queue_message("Douse it with water [w] ");
  queue_message("Smash it against the earth [e] ");
  queue_message("Toss is through the air [a] ");
  queue_message("Mix the above actions, doing them in sequence [m] ");
  do
  {
    queue_message("Which one [f,w,e,a,m] ");
    response = (char)mcigetc();
  } while((response != 'f') && (response != 'w') && (response != 'e') && (response != 'a') &&
          (response != 'm'));
  return response == element;
}

// orb functions
void i_orbfire(std::unique_ptr<object> &o)
{
  if(!orbcheck('f'))
  {
    queue_message("Bad choice!");
    queue_message("The Orb of Fire blasts you!");
    fball(Player.x, Player.y, Player.x, Player.y, 250);
    if(!o->known)
    {
      o->known = 1;
    }
  }
  else
  {
    queue_message("The Orb of Fire flares a brilliant red!");
    spell::Spells[spell::FIREBOLT].known = true;
    gain_experience(10000);
    Player.immunity[FLAME] += 100;
    queue_message("You feel fiery!");
    o->plus     = 100;
    o->blessing = 100;
    i_firebolt(o);
  }
  *o = Objects[ARTIFACTID + 5];
}

void i_orbwater(std::unique_ptr<object> &o)
{
  if(!orbcheck('w'))
  {
    queue_message("A serious mistake!");
    queue_message("The Orb of Water blasts you!");
    heal(-250);
    if(!o->known)
    {
      o->known = 1;
    }
  }
  else
  {
    queue_message("The Orb of Water pulses a deep green!");
    spell::Spells[spell::DISRUPT].known = true;
    Player.immunity[POISON] += 100;
    gain_experience(10000);
    queue_message("You feel wet!");
    o->plus     = 100;
    o->blessing = 100;
    i_disrupt(o);
  }
  *o = Objects[ARTIFACTID + 5];
}

void i_orbearth(std::unique_ptr<object> &o)
{
  if(!orbcheck('e'))
  {
    queue_message("What a maroon!");
    queue_message("The Orb of Earth blasts you!");
    Player.con -= 10;
    if(Player.con < 3)
    {
      p_death("congestive heart failure");
    }
    else
    {
      queue_message("Your possessions disintegrate!");
      for(int i = 0; i < MAXITEMS; ++i)
      {
        if(Player.possessions[i])
        {
          dispose_lost_objects(Player.possessions[i]->number, i);
        }
      }
      Player.pack.clear();
      if(!o->known)
      {
        o->known = 1;
      }
    }
  }
  else
  {
    queue_message("The Orb of Earth emanates a brownish aura!");
    spell::Spells[spell::DISINTEGRATE].known = true;
    gain_experience(10000);
    Player.immunity[NEGENERGY] += 100;
    queue_message("You feel earthy!");
    o->plus     = 100;
    o->blessing = 100;
    i_disintegrate(o);
  }
  *o = Objects[ARTIFACTID + 5];
}

void i_orbair(std::unique_ptr<object> &o)
{
  if(!orbcheck('a'))
  {
    queue_message("You lose!");
    queue_message("The Orb of Air blasts you!");
    lball(Player.x, Player.y, Player.x, Player.y, 100);
    if(!o->known)
    {
      o->known = 1;
    }
  }
  else
  {
    queue_message("The Orb of Air flashes blue!");
    spell::Spells[spell::BALL_LIGHTNING].known = true; // lball
    gain_experience(10000);
    queue_message("You feel airy!");
    Player.immunity[ELECTRICITY] += 100;
    o->plus     = 100;
    o->blessing = 100;
    i_invisible(o);
    i_lbolt(o);
  }
  *o = Objects[ARTIFACTID + 5];
}

void i_orbmastery(std::unique_ptr<object> &o)
{
  if(!orbcheck('m'))
  {
    queue_message("A fatal error!");
    queue_message("The Orb of Mastery blasts you to cinders!");
    p_death("playing with the Orb of Mastery");
    if(!o->known)
    {
      o->known = 1;
    }
  }
  else if((find_and_remove_item(ARTIFACTID + 1, -1)) && (find_and_remove_item(ARTIFACTID + 2, -1)) && (find_and_remove_item(ARTIFACTID + 3, -1)) && (find_and_remove_item(ARTIFACTID + 4, -1)))
  {
    queue_message("The Orb of Mastery radiates rainbow colors!");
    queue_message("You feel godlike.");
    Player.iq = Player.maxiq = 2 * Player.maxiq;
    Player.pow = Player.maxpow = 2 * Player.maxpow;
    Player.str = Player.maxstr = 2 * Player.maxstr;
    Player.dex = Player.maxdex = 2 * Player.maxdex;
    Player.con = Player.maxcon = 2 * Player.maxcon;
    Player.agi = Player.maxagi = 2 * Player.maxagi;
    dataprint();
    queue_message("You have been imbued with a cosmic power....");
    wish(1);
    queue_message("You feel much more experienced.");
    gain_experience(20000);
    *o = Objects[ARTIFACTID + 5];
  }
  else
  {
    queue_message("The Orb of Mastery's power is unbalanced!");
    queue_message("The Orb of Mastery blasts you to cinders!");
    p_death("playing with the Orb of Mastery");
  }
}

void i_orbdead(std::unique_ptr<object> &)
{
  queue_message("The burnt-out orb drains all your energy!");
  for(int i = 0; i < spell::NUM_SPELLS; ++i)
  {
    spell::Spells[i].known = false;
  }
  queue_message("You feel not at all like a mage.");
  for(int i = 0; i < MAXITEMS; ++i)
  {
    if(Player.possessions[i])
    {
      bool used = Player.possessions[i]->used;
      if(used)
      {
        item_unequip(Player.possessions[i]);
      }
      Player.possessions[i]->plus = 0;
      if(Player.possessions[i]->on_use > 100 || Player.possessions[i]->on_equip > 100 ||
         Player.possessions[i]->on_unequip > 100)
      {
        Player.possessions[i]->on_use     = I_NOTHING;
        Player.possessions[i]->on_equip   = I_NOTHING;
        Player.possessions[i]->on_unequip = I_NOTHING;
      }
      if(used)
      {
        item_equip(Player.possessions[i]);
      }
    }
  }
  queue_message("A storm of mundanity surounds you!");
  level_drain(Player.level - 1, "a Burnt-out Orb");
  Player.mana = 0;
  Player.pow -= 10;
}

void i_dispel(std::unique_ptr<object> &o)
{
  dispel((o->blessing > -1) ? o->blessing + random_range(3) : o->blessing);
}

// stick functions

// wand of apportation
void i_apport(std::unique_ptr<object> &o)
{
  o->known             = std::max(1, static_cast<int>(o->known));
  Objects[o->id].known = 1;
  apport(o->blessing);
}

// staff of firebolts
void i_firebolt(std::unique_ptr<object> &o)
{
  int x = Player.x, y = Player.y;
  o->known             = std::max(1, static_cast<int>(o->known));
  Objects[o->id].known = 1;
  setspot(x, y);
  if(o->blessing < 0)
  {
    x = Player.x;
    y = Player.y;
  }
  fbolt(Player.x, Player.y, x, y, Player.dex * 2 + Player.level, 75);
}

void i_disintegrate(std::unique_ptr<object> &o)
{
  int x = Player.x, y = Player.y;
  o->known             = std::max(1, static_cast<int>(o->known));
  Objects[o->id].known = 1;
  setspot(x, y);
  if(o->blessing < 0)
  {
    x = Player.x;
    y = Player.y;
  }
  disintegrate(x, y);
}

void i_disrupt(std::unique_ptr<object> &o)
{
  int x = Player.x, y = Player.y;
  o->known             = std::max(1, static_cast<int>(o->known));
  Objects[o->id].known = 1;
  setspot(x, y);
  if(o->blessing < 0)
  {
    x = Player.x;
    y = Player.y;
  }
  disrupt(x, y, 100);
}

// staff of lightning bolts
void i_lbolt(std::unique_ptr<object> &o)
{
  int x = Player.x, y = Player.y;
  o->known             = std::max(1, static_cast<int>(o->known));
  Objects[o->id].known = 1;
  setspot(x, y);
  if(o->blessing < 0)
  {
    x = Player.x;
    y = Player.y;
  }
  lbolt(Player.x, Player.y, x, y, Player.dex * 2 + Player.level, 75);
}

// wand of magic missiles
void i_missile(std::unique_ptr<object> &o)
{
  int x = Player.x, y = Player.y;
  o->known             = std::max(1, static_cast<int>(o->known));
  Objects[o->id].known = 1;
  setspot(x, y);
  if(o->blessing < 0)
  {
    x = Player.x;
    y = Player.y;
  }
  nbolt(Player.x, Player.y, x, y, Player.dex * 2 + Player.level, 20);
}

// wand of fire balls
void i_fireball(std::unique_ptr<object> &o)
{
  int x = Player.x, y = Player.y;
  Objects[o->id].known = 1;
  o->known             = std::max(1, static_cast<int>(o->known));
  setspot(x, y);
  if(o->blessing < 0)
  {
    x = Player.x;
    y = Player.y;
  }
  fball(Player.x, Player.y, x, y, 35);
}

// wand of snowballs
void i_snowball(std::unique_ptr<object> &o)
{
  int x = Player.x, y = Player.y;
  Objects[o->id].known = 1;
  o->known             = std::max(1, static_cast<int>(o->known));
  setspot(x, y);
  if(o->blessing < 0)
  {
    x = Player.x;
    y = Player.y;
  }
  snowball(Player.x, Player.y, x, y, 20);
}

// wand of lightning balls
void i_lball(std::unique_ptr<object> &o)
{
  int x = Player.x, y = Player.y;
  Objects[o->id].known = 1;
  o->known             = std::max(1, static_cast<int>(o->known));
  setspot(x, y);
  if(o->blessing < 0)
  {
    x = Player.x;
    y = Player.y;
  }
  lball(Player.x, Player.y, x, y, 50);
}

// staff of sleep
void i_sleep_other(std::unique_ptr<object> &o)
{
  Objects[o->id].known = 1;
  o->known             = std::max(1, static_cast<int>(o->known));
  sleep_monster(o->blessing);
}

// rod of summoning
// rod of summoning now always summons as if cursed
void i_summon(std::unique_ptr<object> &o)
{
  Objects[o->id].known = 1;
  o->known             = std::max(1, static_cast<int>(o->known));
  summon(-1, -1);
}

void i_hide(std::unique_ptr<object> &o)
{
  int x = Player.x, y = Player.y;
  Objects[o->id].known = 1;
  o->known             = std::max(1, static_cast<int>(o->known));
  setspot(x, y);
  hide(x, y);
}

void i_polymorph(std::unique_ptr<object> &o)
{
  Objects[o->id].known = 1;
  o->known             = std::max(1, static_cast<int>(o->known));
  polymorph(o->blessing);
}
