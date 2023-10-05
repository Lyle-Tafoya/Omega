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

/* move.c */
/* general functions for player moving */

#include "glob.h"
#include "scr.h"
#include "spell.h"

#include <algorithm>
#include <chrono>
#include <thread>

extern void item_unequip(object *);

/* various miscellaneous location functions */
void l_water()
{
  if(!gamestatusp(MOUNTED, GameStatus))
  {
    if(Player.possessions[O_ARMOR])
    {
      queue_message("Your heavy armor drags you under the water!");
      p_drown();
      queue_message("You reach the surface again.");
    }
    else if(Player.itemweight > ((int)(Player.maxweight / 2)))
    {
      queue_message("The weight of your burden drags you under water!");
      p_drown();
      queue_message("You reach the surface again.");
    }
    else
    {
      switch(random_range(32))
      {
        case 0:
          queue_message("Splish. Splash!");
          break;
        case 1:
          queue_message("I want my ducky!");
          break;
        case 2:
          queue_message("So finally you take a bath!");
          break;
        case 3:
          queue_message("You must be very thirsty!");
          break;
      }
    }
  }
  else
  {
    switch(random_range(32))
    {
      case 0:
        queue_message("Your horse frolics playfully in the water.");
        break;
      case 1:
        queue_message("Your horse quenches its thirst.");
        break;
      case 2:
        queue_message("Your steed begins to swim....");
        break;
      case 3:
        queue_message("Your mount thrashes about in the water.");
        break;
    }
  }
}

void l_chaos()
{
  if(gamestatusp(MOUNTED, GameStatus))
  {
    queue_message("Your steed tries to swim in the raw Chaos, but seems to");
    queue_message("be having some difficulties...");
    queue_message("probably because it's just turned into a chaffinch.");
    resetgamestatus(MOUNTED, GameStatus);
  }
  if(!onewithchaos)
  {
    queue_message("You are immersed in raw Chaos....");
  }
  if(Player.rank[ADEPT])
  {
    if(!onewithchaos)
    {
      onewithchaos = 1;
      queue_message("You achieve oneness of Chaos....");
    }
    Player.mana = std::max(Player.mana, calcmana());
    Player.hp   = std::max(Player.hp, Player.maxhp);
  }
  else if(Player.rank[PRIESTHOOD] && (!saved))
  {
    queue_message("A mysterious force protects you from the Chaos!");
    queue_message("Wow.... You feel a bit smug.");
    gain_experience(500);
    saved = true;
  }
  else
  {
    queue_message("Uh oh....");
    if(saved)
    {
      queue_message("Nothing mysterious happens this time....");
    }
    queue_message("Congratulations! You've achieved maximal entropy!");
    Player.alignment -= 50;
    gain_experience(1000);
    p_death("immersion in raw Chaos");
  }
}

void l_hedge()
{
  if(Player.patron == DRUID)
  {
    queue_message("You move through the hedges freely.");
  }
  else
  {
    queue_message("You struggle in the brambly hedge... ");
    switch(random_range(6))
    {
      case 0:
        queue_message("You are stabbed by thorns!");
        p_damage(random_range(6), NORMAL_DAMAGE, "a hedge");
        queue_message("The thorns were poisonous!");
        p_poison(random_range(12));
        break;
      case 1:
        queue_message("You are stabbed by thorns!");
        p_damage(random_range(12), NORMAL_DAMAGE, "a hedge");
        break;
      case 2:
        queue_message("You seem to have gotten stuck in the hedge.");
        Player.status[IMMOBILE] += random_range(5) + 1;
        break;
      case 3:
        if(Player.possessions[O_CLOAK])
        {
          queue_message("Your cloak was torn on the brambles!");
          dispose_lost_objects(1, Player.possessions[O_CLOAK]);
        }
        else
        {
          queue_message("Ouch! These thorns are scratchy!");
        }
        break;
      default:
        queue_message("You make your way through unscathed.");
        break;
    }
  }
}

void l_lava()
{
  queue_message("Very clever -- walking into a pool of lava...");
  if(gamestatusp(MOUNTED, GameStatus))
  {
    queue_message("Your horse is incinerated... You fall in too!");
    resetgamestatus(MOUNTED, GameStatus);
  }
  if(Player.name == "Saltheart Foamfollower")
  {
    queue_message("Strangely enough, you don't seem terribly affected.");
    p_damage(1, UNSTOPPABLE, "slow death in a pool of lava");
  }
  else
  {
    p_damage(random_range(75), FLAME, "incineration in a pool of lava");
    if(Player.hp > 0)
    {
      p_drown();
    }
    Player.status[IMMOBILE] += 2;
  }
}

void l_fire()
{
  queue_message("You boldly stride through the curtain of fire...");
  if(gamestatusp(MOUNTED, GameStatus))
  {
    queue_message("Your horse is fried and so are you...");
    resetgamestatus(MOUNTED, GameStatus);
  }
  p_damage(random_range(100), FLAME, "self-immolation");
}

void l_abyss()
{
  int i;
  if(Current_Environment != Current_Dungeon)
  {
    queue_message("You fall through a dimensional portal!");
    strategic_teleport(-1);
  }
  else
  {
    queue_message("You enter the infinite abyss!");
    if(random_range(100) == 13)
    {
      queue_message("As you fall you see before you what seems like");
      queue_message("an infinite congerie of iridescent bubbles.");
      queue_message("You have met Yog Sothoth!!!");
      if(Player.alignment > -10)
      {
        p_death("the Eater of Souls");
      }
      else
      {
        queue_message("The All-In-One must have taken pity on you.");
        queue_message("A transdimensional portal appears...");
        change_level(Level->depth, Level->depth + 1, false);
        gain_experience(2000);
        Player.alignment -= 50;
      }
    }
    else
    {
      i = 0;
      queue_message("You fall...");
      while(random_range(3) != 2)
      {
        if(i % 6 == 0)
        {
          queue_message("and fall... ");
        }
        else
        {
          queue_message("and fall... ");
        }
        i++;
      }
      i++;
      queue_message("Finally,you emerge through an interdimensional interstice...");
      if(Level->depth + i > MaxDungeonLevels)
      {
        queue_message("You emerge high above the ground!!!!");
        queue_message("Yaaaaaaaah........");
        change_environment(E_COUNTRYSIDE);
        do
        {
          Player.x = random_range(WIDTH);
          Player.y = random_range(LENGTH);
        } while(Country[Player.x][Player.y].base_terrain_type == CHAOS_SEA);
        p_damage(i * 50, NORMAL_DAMAGE, "a fall from a great height");
      }
      else
      {
        queue_message("You built up some velocity during your fall, though....");
        p_damage(i * 5, NORMAL_DAMAGE, "a fall through the abyss");
        change_level(Level->depth, Level->depth + i, false);
        gain_experience(i * i * 50);
      }
    }
  }
}

void l_lift()
{
  char response;
  int  levelnum;
  int  distance;
  int  too_far = 0;

  Level->site[Player.x][Player.y].locchar = FLOOR;
  Level->site[Player.x][Player.y].p_locf  = L_NO_OP;
  lset(Player.x, Player.y, CHANGED, *Level);
  queue_message("You walk onto a shimmering disk....");
  queue_message("The disk vanishes, and a glow surrounds you.");
  queue_message("You feel weightless.... You feel ghostly....");
  queue_message("Go up, down, or neither [u,d,ESCAPE] ");
  do
  {
    response = (char)mcigetc();
  } while((response != 'u') && (response != 'd') && (response != ESCAPE));
  if(response != ESCAPE)
  {
    queue_message("How many levels?");
    levelnum = (int)parsenum();
    if(levelnum > 6)
    {
      too_far  = 1;
      levelnum = 6;
    }
    if(response == 'u' && Level->depth - levelnum < 1)
    {
      distance = levelnum - Level->depth;
      change_environment(E_COUNTRYSIDE); /* "you return to the countryside." */
      if(distance > 0)
      {
        queue_message("..");
        queue_message("...and keep going up!  You hang in mid air...");
        queue_message("\"What goes up...\"");
        queue_message("Yaaaaaaaah........");
        p_damage(distance * 10, NORMAL_DAMAGE, "a fall from a great height");
      }
      return;
    }
    else if(response == 'd' && Level->depth + levelnum > MaxDungeonLevels)
    {
      too_far  = 1;
      levelnum = MaxDungeonLevels - Level->depth;
    }
    if(levelnum == 0)
    {
      queue_message("Nothing happens.");
      return;
    }
    if(too_far)
    {
      queue_message("The lift gives out partway...");
      queue_message("You rematerialize.....");
    }
    else
    {
      queue_message("You rematerialize.....");
    }
    change_level(Level->depth, (response == 'd' ? Level->depth + levelnum : Level->depth - levelnum),
                 false);
    roomcheck();
  }
}

void l_magic_pool()
{
  int possibilities = random_range(100);
  queue_message("This pool seems to be enchanted....");
  if(gamestatusp(MOUNTED, GameStatus))
  {
    if(random_range(2))
    {
      queue_message("Your horse is polymorphed into a fig newton.");
      resetgamestatus(MOUNTED, GameStatus);
    }
    else
    {
      queue_message("Whatever it was, your horse enjoyed it....");
    }
  }
  else if(possibilities == 0)
  {
    queue_message("Oh no! You encounter the DREADED AQUAE MORTIS...");
    if(random_range(1000) < Player.level * Player.level * Player.level)
    {
      queue_message("The DREADED AQUAE MORTIS throttles you within inches....");
      queue_message("but for some reason chooses to let you escape.");
      gain_experience(500);
      Player.hp = 1;
    }
    else
    {
      p_death("the DREADED AQUAE MORTIS!");
    }
  }
  else if(possibilities < 25)
  {
    augment(0);
  }
  else if(possibilities < 30)
  {
    augment(1);
  }
  else if(possibilities < 60)
  {
    augment(-1);
  }
  else if(possibilities < 65)
  {
    cleanse(1);
  }
  else if(possibilities < 80)
  {
    if(Player.possessions[O_WEAPON_HAND])
    {
      queue_message("You drop your weapon in the pool! It's gone forever!");
      dispose_lost_objects(1, Player.possessions[O_WEAPON_HAND]);
    }
    else
    {
      queue_message("You feel fortunate.");
    }
  }
  else if(possibilities < 90)
  {
    if(Player.possessions[O_WEAPON_HAND])
    {
      queue_message("Your weapon leaves the pool with a new edge....");
      Player.possessions[O_WEAPON_HAND]->plus += random_range(10) + 1;
      calc_melee();
    }
    else
    {
      queue_message("You feel unfortunate.");
    }
  }
  else if(possibilities < 95)
  {
    Player.hp += 10;
    queue_message("You feel healthier after the dip...");
  }
  else if(possibilities < 99)
  {
    queue_message("Oooh, a tainted pool...");
    p_poison(10);
  }
  else if(possibilities == 99)
  {
    queue_message("Wow! A pool of azoth!");
    heal(10);
    cleanse(1);
    Player.mana = calcmana() * 3;
    Player.str  = (Player.maxstr++) * 3;
  }
  queue_message("The pool seems to have dried up.");
  Level->site[Player.x][Player.y].locchar = TRAP;
  Level->site[Player.x][Player.y].p_locf  = L_TRAP_PIT;
  lset(Player.x, Player.y, CHANGED, *Level);
}

void l_no_op() {}

void l_tactical_exit()
{
  if(optionp(CONFIRM, Player))
  {
    queue_message("Do you really want to leave this place? ");
    if(ynq() != 'y')
    {
      return;
    }
  }
  /* Free up monsters and items, and the level, if not SAVE_LEVELS */
  free_level(Level);
  Level = nullptr;
  if((Current_Environment == E_TEMPLE) || (Current_Environment == E_TACTICAL_MAP))
  {
    change_environment(E_COUNTRYSIDE);
  }
  else
  {
    change_environment(Last_Environment);
  }
}

void l_rubble()
{
  int screwup = random_range(100) - (Player.agi + Player.level);
  queue_message("You climb over the unstable pile of rubble....");
  if(screwup < 0)
  {
    queue_message("No problem!");
  }
  else
  {
    queue_message("You tumble and fall in a small avalanche of debris!");
    queue_message("You're trapped in the pile!");
    Player.status[IMMOBILE] += 2;
    p_damage(screwup / 5, UNSTOPPABLE, "rubble and debris");
  }
}

/* Drops all portcullises in 5 moves */
void l_portcullis_trap()
{
  int i, j, slam = false;

  queue_message("Click.");
  for(i = std::max(Player.x - 5, 0); i < std::min(Player.x + 6, WIDTH); i++)
  {
    for(j = std::max(Player.y - 5, 0); j < std::min(Player.y + 6, LENGTH); j++)
    {
      if((Level->site[i][j].p_locf == L_PORTCULLIS) && (Level->site[i][j].locchar != PORTCULLIS))
      {
        Level->site[i][j].locchar = PORTCULLIS;
        lset(i, j, CHANGED, *Level);
        putspot(i, j, PORTCULLIS);
        if((i == Player.x) && (j == Player.y))
        {
          queue_message("Smash! You've been hit by a falling portcullis!");
          p_damage(random_range(1000), NORMAL_DAMAGE, "a portcullis");
        }
        slam = true;
      }
    }
  }
  if(slam)
  {
    queue_message("You hear heavy walls slamming down!");
  }
}

/* drops every portcullis on level, then kills itself and all similar traps. */
void l_drop_every_portcullis()
{
  int i, j, slam = false;

  queue_message("Click.");
  for(i = 0; i < WIDTH; i++)
  {
    for(j = 0; j < LENGTH; j++)
    {
      if(Level->site[i][j].p_locf == L_DROP_EVERY_PORTCULLIS)
      {
        Level->site[i][j].p_locf = L_NO_OP;
        lset(i, j, CHANGED, *Level);
      }
      else if((Level->site[i][j].p_locf == L_PORTCULLIS) && (Level->site[i][j].locchar != PORTCULLIS))
      {
        Level->site[i][j].locchar = PORTCULLIS;
        lset(i, j, CHANGED, *Level);
        putspot(i, j, PORTCULLIS);
        if((i == Player.x) && (j == Player.y))
        {
          queue_message("Smash! You've been hit by a falling portcullis!");
          p_damage(random_range(1000), NORMAL_DAMAGE, "a portcullis");
        }
        slam = true;
      }
    }
  }
  if(slam)
  {
    queue_message("You hear heavy walls slamming down!");
  }
}

void l_raise_portcullis()
{
  int i, j, open = false;
  for(i = 0; i < WIDTH; i++)
  {
    for(j = 0; j < LENGTH; j++)
    {
      if(Level->site[i][j].locchar == PORTCULLIS)
      {
        Level->site[i][j].locchar = FLOOR;
        lset(i, j, CHANGED, *Level);
        putspot(i, j, FLOOR);
        open = true;
      }
    }
  }
  if(open)
  {
    queue_message("You hear the sound of steel on stone!");
  }
}

void l_arena_exit()
{
  resetgamestatus(ARENA_MODE, GameStatus);
  free_level(Level);
  Level = nullptr;
  change_environment(E_CITY);
}

void l_house_exit()
{
  if(optionp(CONFIRM, Player))
  {
    queue_message("Do you really want to leave this abode? ");
    if(ynq() != 'y')
    {
      return;
    }
  }
  free_level(Level);
  Level = nullptr;
  change_environment(Last_Environment);
}

void l_void()
{
  queue_message("Geronimo!");
  queue_message("You leap into the void.");
  if(Level->mlist)
  {
    queue_message("Death peers over the edge and gazes quizzically at you....");
    queue_message("'Bye-bye,' he says... 'We'll meet again.'");
  }
  while(Player.hp > 0)
  {
    Time += 60;
    hourly_check();
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
  }
}

void l_fire_station()
{
  queue_message("The flames leap up, and the heat is incredible.");
  if(Player.immunity[FLAME])
  {
    queue_message("You feel the terrible heat despite your immunity to fire!");
  }
  queue_message("Enter the flames? [yn] ");
  if(ynq() == 'y')
  {
    if(Player.hp == 1)
    {
      p_death("total incineration");
    }
    else
    {
      Player.hp = 1;
    }
    dataprint();
    queue_message("You feel like you are being incinerated! Jump back? [yn] ");
    if(ynq() == 'y')
    {
      queue_message("Phew! That was close!");
    }
    else
    {
      Player.pow -= (15 + random_range(15));
      if(Player.pow > 0)
      {
        queue_message("That's odd, the flame seems to have cooled down now....");
        queue_message("A flicker of fire seems to dance above the void nearby.");
        Level->site[Player.x][Player.y].locchar = FLOOR;
        Level->site[Player.x][Player.y].p_locf  = L_NO_OP;
        stationcheck();
      }
      else
      {
        queue_message("The flames seem to have leached away all your mana!");
        p_death("the Essence of Fire");
      }
    }
  }
  else
  {
    queue_message("You flinch away from the all-consuming fire.");
  }
}

void l_water_station()
{
  queue_message("The fluid seems murky and unknowably deep.");
  queue_message("It bubbles and hisses threateningly.");
  if(Player.status[BREATHING])
  {
    queue_message("You don't feel sanguine about trying to breathe that stuff!");
  }
  if(Player.immunity[ACID])
  {
    queue_message("The vapor burns despite your immunity to acid!");
  }
  queue_message("Enter the fluid? [yn] ");
  if(ynq() == 'y')
  {
    if(Player.hp == 1)
    {
      p_death("drowning in acid (ick, what a way to go)");
    }
    else
    {
      Player.hp = 1;
    }
    dataprint();
    queue_message("You choke....");
    queue_message("Your lungs burn....");
    queue_message("Your body begins to disintegrate.... Leave the pool? [yn] ");
    if(ynq() == 'y')
    {
      queue_message("Phew! That was close!");
    }
    else
    {
      Player.con -= (15 + random_range(15));
      if(Player.con > 0)
      {
        queue_message("That's odd, the fluid seems to have been neutralized....");
        queue_message("A moist miasma wafts above the void nearby.");
        Level->site[Player.x][Player.y].locchar = FLOOR;
        Level->site[Player.x][Player.y].p_locf  = L_NO_OP;
        stationcheck();
      }
      else
      {
        queue_message("The bubbling fluid has destroyed your constitution!");
        p_death("the Essence of Water");
      }
    }
  }
  else
  {
    queue_message("You step back from the pool of acid.");
  }
}

void l_air_station()
{
  queue_message("The whirlwind spins wildly and crackles with lightning.");
  if(Player.immunity[ELECTRICITY])
  {
    queue_message("You feel static cling despite your immunity to electricity!");
  }
  queue_message("Enter the storm? [yn] ");
  if(ynq() == 'y')
  {
    if(Player.hp == 1)
    {
      p_death("being torn apart and then electrocuted");
    }
    else
    {
      Player.hp = 1;
    }
    dataprint();
    queue_message("You are buffeted and burnt by the storm....");
    queue_message("You begin to lose consciousness.... Leave the storm? [yn] ");
    if(ynq() == 'y')
    {
      queue_message("Phew! That was close!");
    }
    else
    {
      Player.iq -= (random_range(15) + 15);
      if(Player.iq > 0)
      {
        queue_message("That's odd, the storm subsides....");
        queue_message("A gust of wind brushes past the void nearby.");
        Level->site[Player.x][Player.y].locchar = FLOOR;
        Level->site[Player.x][Player.y].p_locf  = L_NO_OP;
        stationcheck();
      }
      else
      {
        queue_message("The swirling storm has destroyed your intelligence!");
        p_death("the Essence of Air");
      }
    }
  }
  else
  {
    queue_message("You step back from the ominous whirlwind.");
  }
}

void l_earth_station()
{
  pob o;
  queue_message("The tendrilled mass reaches out for you from the muddy ooze.");
  if(find_item(&o, THINGID + 6, -1))
  {
    queue_message("A splash of salt water does nothing to dissuade the vines.");
  }
  queue_message("Enter the overgrown mire? [yn] ");
  if(ynq() == 'y')
  {
    if(Player.hp == 1)
    {
      p_death("being eaten alive");
    }
    else
    {
      Player.hp = 1;
    }
    dataprint();
    queue_message("You are being dragged into the muck. Suckers bite you....");
    queue_message("You're about to be entangled.... Leave the mud? [yn] ");
    if(ynq() == 'y')
    {
      queue_message("Phew! That was close!");
    }
    else
    {
      Player.str -= (15 + random_range(15));
      if(Player.str > 0)
      {
        queue_message("That's odd, the vine withdraws....");
        queue_message("A spatter of dirt sprays into the void nearby.");
        Level->site[Player.x][Player.y].locchar = FLOOR;
        Level->site[Player.x][Player.y].p_locf  = L_NO_OP;
        stationcheck();
      }
      else
      {
        queue_message("The tendril has destroyed your strength!");
        p_death("the Essence of Earth");
      }
    }
  }
  else
  {
    queue_message("You step back from the ominous vegetation.");
  }
}

void stationcheck()
{
  int stationsleft = false;
  int i, j;
  queue_message("You feel regenerated.");
  Player.hp = Player.maxhp;
  dataprint();
  for(i = 0; i < WIDTH; i++)
  {
    for(j = 0; j < LENGTH; j++)
    {
      if((Level->site[i][j].locchar == WATER) || (Level->site[i][j].locchar == HEDGE) ||
         (Level->site[i][j].locchar == WHIRLWIND) || (Level->site[i][j].locchar == FIRE))
      {
        stationsleft = true;
      }
    }
  }
  if(!stationsleft)
  {
    queue_message("There is a noise like a wild horse's neigh.");
    queue_message("You spin around, and don't see anyone around at all");
    queue_message("except for a spurred black cloaked figure carrying a scythe.");
    queue_message("Death coughs apologetically. He seems a little embarrassed.");
    queue_message("A voice peals out:");
    queue_message("'An Adept must be able to conquer Death himself....");
    make_site_monster(32, 4, DEATH);
  }
}

/* To survive the void, the other four stations must be visited first,
   to activate the void, then something (Death's scythe, possibly)
   must be thrown in to satiate the void, then all other items must
   be dropped, then the void must be entered. */

void l_void_station()
{
  int i, something = false;
  queue_message("You are at the brink of an endless void. Enter it? [yn] ");
  if(ynq() == 'y')
  {
    if(!Level->mlist)
    {
      queue_message("You fall forever. Eventually you die of starvation.");
      while(Player.hp > 0)
      {
        Time += 60;
        hourly_check();
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
      }
    }
    else
    {
      queue_message("You enter the void.");
      queue_message("You feel a sudden surge of power from five directions.");
      something = (Player.packptr > 0);
      if(!something)
      {
        for(i = 0; ((i < MAXITEMS) && (!something)); i++)
        {
          if(Player.possessions[i])
          {
            something = true;
          }
        }
      }
      if(something)
      {
        queue_message("The flow of power is disrupted by something!");
        queue_message("The power is unbalanced! You lose control!");
        queue_message("Each of your cells explodes with a little scream of pain.");
        queue_message("Your disrupted essence merges with the megaflow.");
        p_death("the Power of the Void");
      }
      else if(!gamestatusp(PREPARED_VOID, GameStatus))
      {
        queue_message("The hungry void swallows you whole!");
        queue_message("Your being dissipates with a pathetic little sigh....");
        p_death("the Emptyness of the Void");
      }
      else
      {
        queue_message("The flow of power rages through your body,");
        queue_message("but you manage to master the surge!");
        queue_message("You feel adept....");
        queue_message("With a thought, you soar up through the void to the");
        queue_message("place from whence you came.");
        queue_message("As the platform of the Challenge dwindles beneath you");
        queue_message("You see Death raise his scythe to you in a salute.");
        Player.rank[ADEPT] = 1;
        setgamestatus(COMPLETED_CHALLENGE, GameStatus);
        FixedPoints = calc_points();
        /* set so change_environment puts player in correct temple! */
        Player.x = 49;
        Player.y = 59;
        queue_message("You find yourself back in the Temple of Destiny.");
        change_environment(E_TEMPLE);
      }
    }
  }
  else
  {
    queue_message("You back away from the edge....");
  }
}

void l_voice1()
{
  queue_message("A mysterious voice says: The Hunger of the Void must be satiated.");
  Level->site[Player.x][Player.y].p_locf = L_NO_OP;
}

void l_voice2()
{
  queue_message("A strange voice recites: Enter the Void as you entered the World.");
  Level->site[Player.x][Player.y].p_locf = L_NO_OP;
}

void l_voice3()
{
  queue_message("An eerie voice resounds: The Void is the fifth Elemental Station.");
  Level->site[Player.x][Player.y].p_locf = L_NO_OP;
}

void l_whirlwind()
{
  queue_message("Buffeting winds swirl you up!");
  p_damage(random_range(difficulty() * 10), NORMAL_DAMAGE, "a magic whirlwind");
  if(random_range(2))
  {
    queue_message("You are jolted by lightning!");
    p_damage(random_range(difficulty() * 10), ELECTRICITY, "a magic whirlwind");
  }
  if(random_range(2))
  {
    queue_message("The whirlwind carries you off....");
    if(random_range(20) == 17)
    {
      queue_message("'I don't think we're in Kansas anymore, toto.'");
    }
    p_teleport(0);
  }
}

void l_enter_circle()
{
  queue_message("You see a translucent stairway before you, leading down.");
  queue_message("Take it? [yn] ");
  if(ynq() == 'y')
  {
    change_environment(E_CIRCLE);
  }
}

void l_circle_library()
{
  queue_message("You see before you the arcane library of the Circle of Sorcerors.");
}

void l_tome1()
{
  menuclear();
  menuprint("\nYou discover in a dusty tome some interesting information....");
  menuprint("\nThe Star Gem holds a vast amount of mana, usable");
  menuprint("\nfor either Law or Chaos. It is magically linked to Star Peak");
  menuprint("\nand can either be activated or destroyed there. If destroyed,");
  menuprint("\nits power will be used for Chaos, if activated, for Law.");
  menuprint("\n\nIt is said the LawBringer has waited for an eternity");
  menuprint("\nat Star Peak for someone to bring him the gem.");
  menuprint("\nIt is also rumored that while anyone might destroy the gem,");
  menuprint("\nreleasing chaotic energy, only the LawBringer can release");
  menuprint("\nthe lawful potential of the gem.");
  showmenu();
  xredraw();
}

void l_tome2()
{
  menuclear();
  menuprint("\nYou discover in some ancient notes that the Star Gem can be");
  menuprint("\nused for transportation, but also read a caution that it must");
  menuprint("\nbe allowed to recharge a long time between uses.");
  menuprint("\nA marginal note says 'if only it could be reset to go somewhere");
  menuprint("\nbesides Star Peak, the gem might be useful....'");
  showmenu();
  xredraw();
}

void l_temple_warning()
{
  queue_message("A stern voice thunders in the air around you:");
  queue_message("'No unbelievers may enter these sacred precincts;");
  queue_message("those who defile this shrine will be destroyed!");
}

void l_throne()
{
  pob o;
  queue_message("You have come upon a huge ornately appointed throne!");
  queue_message("Sit in it? [yn] ");
  if(ynq() == 'y')
  {
    if(!find_item(&o, ARTIFACTID + 22, -1))
    {
      queue_message("The throne emits an eerie violet-black radiance.");
      queue_message("You find, to your horror, that you cannot get up!");
      queue_message("You feel an abstract sucking sensation...");
      for(int i = 0; i < spell::NUM_SPELLS; ++i)
      {
        spell::Spells[i].known = false;
      }
      Player.pow  = 3;
      Player.mana = 0;
      Player.hp   = 1;
      dispel(-1);
      queue_message("The radiance finally ceases. You can get up now.");
    }
    else
    {
      if(HiMagicUse == Date)
      {
        queue_message("You hear the sound of a magic kazoo played by an asthmatic.");
      }
      else
      {
        HiMagicUse = Date;
        queue_message("Following some strange impulse, you raise the Sceptre....");
        queue_message("You hear a magical fanfare, repeated three times.");
        switch(HiMagic++)
        {
          case 0:
            queue_message("Strength.");
            Player.str += 5;
            Player.maxstr += 5;
            break;
          case 1:
            queue_message("Constitution.");
            Player.con += 5;
            Player.maxcon += 5;
            break;
          case 2:
            queue_message("Dexterity.");
            Player.dex += 5;
            Player.maxdex += 5;
            break;
          case 3:
            queue_message("Agility.");
            Player.agi += 5;
            Player.maxagi += 5;
            break;
          case 4:
            queue_message("Intelligence.");
            Player.iq += 5;
            Player.maxiq += 5;
            break;
          case 5:
            queue_message("Power.");
            Player.pow += 5;
            Player.maxpow += 5;
            break;
          default:
            if(spell::Spells[spell::WISH].known)
            {
              queue_message("A mysterious voice mutters peevishly....");
              queue_message("So what do you want now? A medal?");
            }
            else
            {
              queue_message("Mystic runes appear in the air before you:");
              queue_message("They appear to describe some high-powered spell.");
              queue_message("You hear a distant voice....");
              queue_message("'You may now tread the path of High Magic.'");
              spell::Spells[spell::WISH].known = true;
            }
            break;
          case 17:
            queue_message("Weird flickering lights play over the throne.");
            queue_message("You hear a strange droning sound, as of a magical");
            queue_message("artifact stressed by excessive use....");
            queue_message("With an odd tinkling sound the throne shatters!");
            Level->site[Player.x][Player.y].locchar = RUBBLE;
            Level->site[Player.x][Player.y].p_locf  = L_RUBBLE;
            lset(Player.x, Player.y, CHANGED, *Level);
            if(find_and_remove_item(ARTIFACTID + 22, -1))
            {
              queue_message("Your sceptre reverberates with the noise, and");
              queue_message("it too explodes in a spray of shards.");
            }
            break;
        }
        calc_melee();
        dataprint();
      }
    }
  }
}

void l_escalator()
{
  queue_message("You have found an extremely long stairway going straight up.");
  queue_message("The stairs are grilled steel and the bannister is rubber.");
  queue_message("Take the stairway? [yn] ");
  if(ynq() == 'y')
  {
    queue_message("The stairs suddenly start moving with a grind of gears!");
    queue_message("You are wafted to the surface....");
    change_environment(E_COUNTRYSIDE);
  }
}

void l_enter_court()
{
  queue_message("You have found a magical portal! Enter it? [yn] ");
  if(ynq() == 'y')
  {
    if(!gamestatusp(COMPLETED_CASTLE, GameStatus))
    {
      if(!gamestatusp(ATTACKED_ORACLE, GameStatus))
      {
        queue_message("A dulcet voice says: 'Jolly good show!'");
      }
      setgamestatus(COMPLETED_CASTLE, GameStatus);
    }
    change_environment(E_COURT);
  }
}

void l_chaostone()
{
  queue_message("This is a menhir carved of black marble with veins of gold.");
  queue_message("It emanates an aura of raw chaos, which is not terribly");
  queue_message("surprising, considering its location.");
  if(Player.alignment < 0)
  {
    queue_message("You feel an almost unbearable attraction to the stone.");
  }
  else
  {
    queue_message("You find it extremely difficult to approach the stone.");
  }
  queue_message("Touch it? [yn] ");
  if(ynq() == 'y')
  {
    queue_message("A sudden flux of energy surrounds you!");
    if(stonecheck(-1))
    {
      queue_message("You feel stronger!");
      Player.maxstr = std::min(Player.maxstr + 10, std::max(30, Player.maxstr));
      dataprint();
    }
  }
  else
  {
    queue_message("You step back from the ominous dolmech.");
  }
}

void l_balancestone()
{
  queue_message("This is a massive granite slab teetering dangerously on a corner.");
  queue_message("You feel a sense of balance as you regard it.");
  queue_message("Touch it? [yn] ");
  if(ynq() == 'y')
  {
    queue_message("A vortex of mana spins about you!");
    if(abs(Player.alignment) > random_range(50))
    {
      queue_message("The cyclone whirls you off to a strange place!");
      change_environment(E_COUNTRYSIDE);
      do
      {
        Player.x = random_range(WIDTH);
        Player.y = random_range(LENGTH);
      } while(Country[Player.x][Player.y].current_terrain_type == CHAOS_SEA);
      screencheck(Player.x, Player.y);
      drawvision(Player.x, Player.y);
    }
    else
    {
      queue_message("You are being drained of experience! Step back? [yn] ");
      if(ynq() == 'y')
      {
        queue_message("The vortex calms down, dimishes, and then disappears.");
      }
      else
      {
        Player.xp -= Player.xp / 4;
        dataprint();
        queue_message("The vortex vanishes. Suddenly, there is a clap of thunder!");
        Player.alignment = 0;
        strategic_teleport(1);
      }
    }
  }
  else
  {
    queue_message("You step back from the unlikely boulder.");
  }
}

void l_lawstone()
{
  queue_message("This is a stele carved of blueish-green feldspar.");
  queue_message("You feel an aura of serenity rising from it, and your gaze");
  queue_message("is attracted to the bulk of Star Peak to the North-East.");
  if(Player.alignment > 0)
  {
    queue_message("You feel a subtle attraction to the stone.");
  }
  else
  {
    queue_message("You find the stone extremely distasteful to contemplate.");
  }
  queue_message("Touch it? [yn] ");
  if(ynq() == 'y')
  {
    queue_message("A matrix of power flows about you!");
    if(stonecheck(1))
    {
      queue_message("You feel more vigorous!");
      Player.maxcon = std::min(Player.maxcon + 10, std::max(Player.maxcon, 30));
      dataprint();
    }
  }
  else
  {
    queue_message("You step back from the strange obelisk.");
  }
}

void l_voidstone()
{
  queue_message("This is a grey and uninteresting stone.");
  queue_message("A feeling of nihility emanates from it.");
  queue_message("Touch it? [yn] ");
  if(ynq() == 'y')
  {
    queue_message("You feel negated.");
    Player.mana = 0;
    toggle_item_use(true);
    for(int i = 0; i < NUMSTATI; ++i)
    {
      Player.status[i] = 0;
    }
    for(int i = 0; i < MAXITEMS; ++i)
    {
      if(Player.possessions[i])
      {
        Player.possessions[i]->blessing   = 0;
        Player.possessions[i]->plus       = 0;
        Player.possessions[i]->on_use     = I_NOTHING;
        Player.possessions[i]->on_equip   = I_NOTHING;
        Player.possessions[i]->on_unequip = I_NOTHING;
      }
    }
    toggle_item_use(false);
    calc_melee();
  }
  else
  {
    queue_message("You back away from the strange rock.");
  }
}

void l_sacrificestone()
{
  int sacrifice = 1;
  int oldmaxhp  = Player.maxhp;
  queue_message("You have come on a weathered basaltic block.");
  queue_message("On the top surface is an indentation in human shape.");
  queue_message("You see old rust colored stains in the grain of the stone.");
  queue_message("You sense something awakening. Touch the block? [yn] ");
  if(ynq() == 'y')
  {
    queue_message("You sense great pain emanating from the ancient altar.");
    queue_message("Climb on to the block? [yn] ");
    if(ynq() == 'y')
    {
      queue_message("You are stuck fast to the block!");
      queue_message("You feel your life-force being sucked away!");
      queue_message("Hit ESCAPE to try and get up at any moment, SPACE to remain.");
      do
      {
        switch(random_range(4))
        {
          case 0:
            queue_message("You feel weaker.");
            break;
          case 1:
            queue_message("You feel your life fading.");
            break;
          case 2:
            queue_message("Your energy is vanishing.");
            break;
          case 3:
            queue_message("You are being drained.");
            break;
        }
        Player.hp -= sacrifice;
        Player.maxhp -= sacrifice / 2;
        sacrifice *= 2;
        dataprint();
        if((Player.hp < 1) || (Player.maxhp < 1))
        {
          p_death("self-sacrifice");
        }
      } while(stillonblock());
      queue_message("You manage to wrench yourself off the ancient altar!");
      queue_message("You leave some skin behind, though....");
      if((oldmaxhp > 10) && (Player.maxhp < 3 * oldmaxhp / 4))
      {
        queue_message("A strange red glow arises from the altar.");
        queue_message("The glow surrounds you.... You sense gratitude.");
        Player.pow += sacrifice;
        Player.maxpow += sacrifice / 10;
        dataprint();
      }
      else
      {
        queue_message("You a have a sense of rejection.");
        queue_message("A roil of fetid vapor smokes up from the altar.");
        gain_experience(sacrifice);
      }
    }
    else
    {
      queue_message("You sense an emotion of pique all around you.");
      queue_message("You retreat from the strange stone.");
    }
  }
  else
  {
    queue_message("You decide discretion to be the better part of valour.");
    queue_message("The stone seems to subside sleepily.");
  }
}

void l_mindstone()
{
  queue_message("You approach a giant crystal of some opaline material.");
  queue_message("Flashes of irridescent light glint from the object.");
  queue_message("You feel your attention being drawn by the intricate crystal.");
  queue_message("Look away from the interesting phenomenon? [yn] ");
  if(ynq() == 'n')
  {
    queue_message("Your gaze focuses deeply on the gem....");
    queue_message("The crystal seems to open up and surround you!");
    if(stonecheck(0))
    {
      queue_message("Your mind has been enhanced by the experience!");
      Player.maxiq = std::min(Player.maxiq + 10, std::max(Player.maxiq, 30));
      dataprint();
    }
  }
  else
  {
    queue_message("You manage to wrench your gaze from the odd jewel.");
    queue_message("The light flashes from the crystal diminish in frequency.");
  }
}
