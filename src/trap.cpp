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

/* trap.c */
/* trap movement functions */

#include "glob.h"
#include "scr.h"
#include "spell.h"

/* various traps */

/* note special function on different levels */
void l_trap_siren()
{
  monsterlist *ml;
  Level->site[Player.x][Player.y].locchar = TRAP;
  lset(Player.x, Player.y, CHANGED, *Level);
  queue_message("A klaxon goes off!");
  queue_message("'Intruder Alert -- Intruder Alert -- Intruder Alert'");
  queue_message("You have the feeling you have been discovered....");
  if((Current_Environment == E_HOUSE) || (Current_Environment == E_MANSION))
  {
    if(!gamestatusp(DESTROYED_ORDER, GameStatus))
    {
      queue_message("The city guard arrives!");
      queue_message("You are apprehended....");
      send_to_jail();
    }
    else
    {
      queue_message("Nobody answers the alarm.");
    }
  }
  else if(Current_Environment == E_HOVEL)
  {
    queue_message("Nobody answers the alarm.");
  }
  else
  {
    if(Current_Environment == E_CIRCLE)
    {
      summon(-1, DEMON_PRINCE); /* prime circle demon */
      summon(-1, DEMON_PRINCE);
      summon(-1, DEMON_PRINCE);
    }
    for(ml = Level->mlist; ml; ml = ml->next)
    {
      m_status_set(*ml->m, AWAKE);
      ml->m->sense *= 2;
      if((Current_Environment == E_CIRCLE) ||
         ((Current_Environment == E_VILLAGE) && (ml->m->id == GUARD)) ||
         ((Current_Environment == E_CITY) && (ml->m->id == GUARD)))
      {
        m_status_set(*ml->m, HOSTILE);
      }
    }
  }
}

void l_trap_dart()
{
  Level->site[Player.x][Player.y].locchar = TRAP;
  lset(Player.x, Player.y, CHANGED, *Level);
  if(gamestatusp(MOUNTED, GameStatus))
  {
    queue_message("A dart annoys your horse....");
  }
  else if(random_range(100) < Player.absorption)
  {
    queue_message("A dart plinks off your armor");
  }
  else
  {
    queue_message("You were hit by a dart!");
    p_damage(difficulty(), NORMAL_DAMAGE, "a dart trap");
    if(random_range(3) == 1)
    {
      queue_message("It was poisoned!");
      p_poison(difficulty());
    }
  }
}

void l_trap_pit()
{
  Level->site[Player.x][Player.y].locchar = TRAP;
  lset(Player.x, Player.y, CHANGED, *Level);
  if(gamestatusp(MOUNTED, GameStatus))
  {
    queue_message("Your horse stumbles into a pit and breaks a leg!");
    queue_message("You are forced to put it out of its misery.");
    resetgamestatus(MOUNTED, GameStatus);
    showflags();
  }
  else if(Player.itemweight < ((int)(Player.maxweight / 2)))
  {
    queue_message("You nimbly dodge a pit trap.");
  }
  else
  {
    queue_message("You fell into a pit!");
    if(random_range(3) == 1)
    {
      queue_message("And were impaled by spikes!");
      p_damage(difficulty() * 5, NORMAL_DAMAGE, "a spiked pit");
    }
    else
    {
      p_damage(difficulty() * 2, NORMAL_DAMAGE, "a pit");
    }
    Player.status[IMMOBILE]++;
  }
}

void l_trap_door()
{
  if(Current_Environment != Current_Dungeon)
  {
    queue_message("You feel oddly lucky.");
  }
  else
  {
    Level->site[Player.x][Player.y].locchar = TRAP;
    lset(Player.x, Player.y, CHANGED, *Level);
    if(gamestatusp(MOUNTED, GameStatus))
    {
      queue_message("You and your horse fall through a trap door!");
      queue_message("Your horse breaks its back. Snif.");
      if(Level->site[Player.x][Player.y].aux != spell::DISINTEGRATE)
      {
        queue_message("You're hit by a rockslide!");
        p_damage(Level->depth * difficulty(), UNSTOPPABLE, "a rockslide");
      }
      change_level(Level->depth, Level->depth + 1, false);
      resetgamestatus(MOUNTED, GameStatus);
      showflags();
      roomcheck();
    }
    else if(random_range(100) < Player.agi)
    {
      queue_message("You leap over a trap door.");
    }
    else
    {
      queue_message("You fell through a trap door!");
      p_damage(difficulty(), NORMAL_DAMAGE, "a trap door");
      if(Level->site[Player.x][Player.y].aux != spell::DISINTEGRATE)
      {
        queue_message("You're hit by a rockslide!");
        p_damage(Level->depth * difficulty(), UNSTOPPABLE, "a rockslide");
      }
      change_level(Level->depth, Level->depth + 1, false);
      roomcheck();
    }
  }
}

void l_trap_snare()
{
  Level->site[Player.x][Player.y].locchar = TRAP;
  lset(Player.x, Player.y, CHANGED, *Level);
  if(gamestatusp(MOUNTED, GameStatus))
  {
    queue_message("Your horse steps out of a snare trap.");
  }
  else if(random_range(100) < Player.agi)
  {
    queue_message("You agilely avoid a snare.");
  }
  else
  {
    queue_message("You were caught in a snare!");
    Player.status[IMMOBILE] += difficulty();
  }
}

void l_trap_blade()
{
  Level->site[Player.x][Player.y].locchar = TRAP;
  lset(Player.x, Player.y, CHANGED, *Level);
  if(random_range(30) < Player.agi + Player.level)
  {
    queue_message("You duck under a scything blade!");
  }
  else
  {
    if(gamestatusp(MOUNTED, GameStatus))
    {
      queue_message("Your horse is struck by a scything blade and killed instantly.");
      resetgamestatus(MOUNTED, GameStatus);
      showflags();
    }
    else
    {
      queue_message("A heavy blade scythes across the room and hits you!");
      p_damage(
        random_range(difficulty() * 3) + difficulty() - Player.absorption, NORMAL_DAMAGE, "a blade trap"
      );
    }
  }
}

void l_trap_fire()
{
  if(gamestatusp(MOUNTED, GameStatus))
  {
    queue_message("Your horse is struck by a blast of fire and is charbroiled!");
    resetgamestatus(MOUNTED, GameStatus);
    showflags();
  }
  else if(random_range(50) < Player.agi + Player.level)
  {
    queue_message("You dodge a pillar of fire!");
  }
  else
  {
    queue_message("You were blasted by a fire trap!");
    p_damage(random_range((difficulty() + 1) * 5), FLAME, "a fire trap");
  }
  Level->site[Player.x][Player.y].locchar = FIRE;
  Level->site[Player.x][Player.y].p_locf  = L_FIRE;
  lset(Player.x, Player.y, CHANGED, *Level);
}

void l_trap_teleport()
{
  Level->site[Player.x][Player.y].locchar = TRAP;
  lset(Player.x, Player.y, CHANGED, *Level);
  queue_message("You experience a momentary disorientation....");
  if(random_range(10000) > difficulty() * difficulty())
  {
    p_teleport(0);
  }
  else
  {
    p_teleport(-1);
  }
}

void l_trap_disintegrate()
{
  Level->site[Player.x][Player.y].locchar = TRAP;
  lset(Player.x, Player.y, CHANGED, *Level);
  queue_message("Oh, no! A disintegration trap!");
  if(gamestatusp(MOUNTED, GameStatus))
  {
    queue_message("Your horse falls apart into its component atoms...");
    resetgamestatus(MOUNTED, GameStatus);
    showflags();
  }
  disintegrate(Player.x, Player.y);
}

void l_trap_sleepgas()
{
  Level->site[Player.x][Player.y].locchar = TRAP;
  lset(Player.x, Player.y, CHANGED, *Level);
  queue_message("Hsssssssss....");
  queue_message("You detect a strange odor....");
  sleep_player((difficulty() / 5) + 1);
}

void l_trap_acid()
{
  int i, k, itemdamage;
  Level->site[Player.x][Player.y].locchar = TRAP;
  lset(Player.x, Player.y, CHANGED, *Level);
  if(Player.agi + Player.level < random_range(100))
  {
    queue_message("You are drenched by a spray of acid!");
    if(gamestatusp(MOUNTED, GameStatus))
    {
      queue_message("Your horse dies unpleasantly.");
      resetgamestatus(MOUNTED, GameStatus);
      showflags();
    }
    p_damage(random_range(difficulty() * 5), ACID, "an acid trap");
    if(!p_immune(ACID))
    {
      queue_message("The acid seeps over your possessions...");
      itemdamage = random_range(5);
      for(i = k = 0; ((i < MAXITEMS) && (k < itemdamage)); i++)
      {
        if(Player.possessions[i])
        {
          k++;
          damage_item(Player.possessions[i]);
        }
      }
    }
  }
  else
  {
    queue_message("You somehow dodge a shower of hydroflouric acid!");
  }
}

void l_trap_abyss()
{
  Level->site[Player.x][Player.y].locchar = ABYSS;
  lset(Player.x, Player.y, CHANGED, *Level);
  if(gamestatusp(MOUNTED, GameStatus))
  {
    queue_message("You and your horse fall into the infinite abyss!");
    l_abyss();
  }
  if(Player.dex + Player.level < random_range(100))
  {
    queue_message("You stumble over a concealed entrance to the abyss!");
    l_abyss();
  }
  else
  {
    queue_message("You gingerly step around a concealed entrance to the abyss!");
  }
}

void l_trap_manadrain()
{
  Level->site[Player.x][Player.y].locchar = TRAP;
  lset(Player.x, Player.y, CHANGED, *Level);
  if(Player.mana == 0)
  {
    if(random_range(10) == 3)
    {
      queue_message("The manadrain trap overloads -- positive feedback....");
      queue_message("That's strange.... You feel repowered!");
      Level->site[Player.x][Player.y].locchar = FLOOR;
      Level->site[Player.x][Player.y].p_locf  = L_NO_OP;
      lset(Player.x, Player.y, CHANGED, *Level);
      Player.mana = calcmana();
    }
    else
    {
      queue_message("You feel strangely unaffected by the manadrain trap.");
    }
  }
  else
  {
    queue_message("A weird rainbow light seems to play over you....");
    queue_message("You feel drained.");
    while(Player.mana > 1)
    {
      Player.mana /= 2;
      dataprint();
    }
    Player.mana = 0;
    dispel(-1);
  }
}
