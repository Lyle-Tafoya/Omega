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

/* aux1.c */
/* auxiliary functions for those in com.c, also see aux2.c and aux3.c */

#include "glob.h"
#include "scr.h"

#include <algorithm>
#include <format>
#include <string>

#ifdef SAVE_LEVELS
extern void kill_all_levels();
#endif

/* check to see if too much tunneling has been done in this level */
void tunnelcheck()
{
  if((Level->depth == 0 && Current_Environment != E_DLAIR) || Current_Environment == E_ASTRAL)
  {
    return;
  }
  Level->tunnelled++;
  if((Level->tunnelled) > LENGTH / 4)
  {
    queue_message("Dust and stone fragments fall on you from overhead.");
  }
  if((Level->tunnelled) > LENGTH / 2)
  {
    queue_message("You hear groaning and creaking noises.");
  }
  if((Level->tunnelled) > 3 * LENGTH / 4)
  {
    queue_message("The floor trembles and you hear a loud grinding screech.");
  }
  if((Level->tunnelled) > LENGTH)
  {
    queue_message("With a scream of tortured stone, the entire dungeon caves in!!!");
    gain_experience(5000);
    if(Player.status[SHADOWFORM])
    {
      change_environment(E_COUNTRYSIDE);
      switch(Country[Player.x][Player.y].base_terrain_type)
      {
        case CASTLE:
        case STARPEAK:
        case CAVES:
        case VOLCANO:
          Country[Player.x][Player.y].current_terrain_type = MOUNTAINS;
          break;
        case DRAGONLAIR:
          Country[Player.x][Player.y].current_terrain_type = DESERT;
          break;
        case MAGIC_ISLE:
          Country[Player.x][Player.y].current_terrain_type = CHAOS_SEA;
          break;
      }
      Country[Player.x][Player.y].base_terrain_type = Country[Player.x][Player.y].current_terrain_type;
      c_set(Player.x, Player.y, CHANGED, Country);
      queue_message("In your shadowy state, you float back up to the surface.");
      return;
    }
    queue_message("You are flattened into an unpleasant jellylike substance.");
    p_death("dungeon cave-in");
  }
}

/* displays a room's name */
void showroom(int i)
{
  std::string location;
  std::string room_name;
  switch(Current_Environment)
  {
    case E_MANSION:
      location = "A luxurious mansion: ";
      break;
    case E_HOUSE:
      location = "A house: ";
      break;
    case E_HOVEL:
      location = "A hovel: ";
      break;
    case E_CITY:
      location = "The City of Rampart";
      break;
    case E_VILLAGE:
      switch(Villagenum)
      {
        case 1:
          location = "The  Village of Star View";
          break;
        case 2:
          location = "The Village of Woodmere";
          break;
        case 3:
          location = "The Village of Stormwatch";
          break;
        case 4:
          location = "The Village of Thaumaris";
          break;
        case 5:
          location = "The Village of Skorch";
          break;
        case 6:
          location = "The Village of Whorfen";
          break;
      }
      break;
    case E_CAVES:
      location = "The Goblin Caves: ";
      break;
    case E_CASTLE:
      location = "The Archmage's Castle: ";
      break;
    case E_ASTRAL:
      location = "The Astral Plane: ";
      break;
    case E_VOLCANO:
      location = "The Volcano: ";
      break;
    case E_SEWERS:
      location = "The Sewers: ";
      break;
    case E_TACTICAL_MAP:
      location = "The Tactical Map ";
      break;
  }
  if(Current_Environment == Current_Dungeon)
  {
    location += "Level " + std::to_string(Level->depth);
    room_name = std::format("({})", roomname(i));
  }
  else if(location.empty() || Current_Environment == E_MANSION || Current_Environment == E_HOUSE ||
          Current_Environment == E_HOVEL)
  {
    location += roomname(i);
  }
  locprint(location);
  room_name_print(room_name);
}

int player_on_sanctuary()
{
  if((Player.x == Player.sx) && (Player.y == Player.sy))
  {
    return true;
  }
  else
  {
    if(Player.patron)
    {
      if((Level->site[Player.x][Player.y].locchar == ALTAR) &&
         (Level->site[Player.x][Player.y].aux == Player.patron))
      {
        return true;
      }
      else
      {
        return false;
      }
    }
    else
    {
      return false;
    }
  }
}

/* check a move attempt, maybe attack something, return true if ok to move. */
/* x y is the proposed place to move to */
int p_moveable(int x, int y)
{
  setgamestatus(SKIP_MONSTERS, GameStatus);
  if(!inbounds(x, y))
  {
    return false;
  }
  else if(Player.status[SHADOWFORM])
  {
    switch(Level->site[x][y].p_locf)
    {
      case L_CHAOS:
      case L_ABYSS:
      case L_VOID:
        return confirmation();
      default:
        resetgamestatus(SKIP_MONSTERS, GameStatus);
        return true;
    }
  }
  else if(loc_statusp(x, y, SECRET, *Level))
  {
    if(!gamestatusp(FAST_MOVE, GameStatus))
    {
      queue_message("Ouch!");
    }
    return false;
  }
  else if(Level->site[x][y].creature)
  {
    if(!gamestatusp(FAST_MOVE, GameStatus))
    {
      fight_monster(Level->site[x][y].creature);
      resetgamestatus(SKIP_MONSTERS, GameStatus);
      return false;
    }
    else
    {
      return false;
    }
  }
  else if((Level->site[x][y].locchar == WALL) || (Level->site[x][y].locchar == STATUE) ||
          (Level->site[x][y].locchar == PORTCULLIS) || (Level->site[x][y].locchar == CLOSED_DOOR) ||
          (gamestatusp(FAST_MOVE, GameStatus) &&
           ((Level->site[x][y].locchar == HEDGE) || (Level->site[x][y].locchar == LAVA) ||
            (Level->site[x][y].locchar == ABYSS) || (Level->site[x][y].locchar == VOID_CHAR) ||
            (Level->site[x][y].locchar == FIRE) || (Level->site[x][y].locchar == WHIRLWIND) ||
            (Level->site[x][y].locchar == WATER) || (Level->site[x][y].locchar == LIFT) ||
            (Level->site[x][y].locchar == TRAP) || Level->site[x][y].locchar == RUBBLE)))
  {
    if(!gamestatusp(FAST_MOVE, GameStatus))
    {
      queue_message("Ouch!");
    }
    return false;
  }
  else if(optionp(CONFIRM, Player) &&
          !(Player.status[LEVITATING] && Level->site[x][y].p_locf > LEVITATION_AVOIDANCE && Level->site[x][y].locchar != HEDGE))
  {
    if((Level->site[x][y].locchar == HEDGE) || (Level->site[x][y].locchar == LAVA) ||
       (Level->site[x][y].locchar == FIRE) || (Level->site[x][y].locchar == WHIRLWIND) ||
       (Level->site[x][y].locchar == ABYSS) || (Level->site[x][y].locchar == VOID_CHAR) ||
       (Level->site[x][y].locchar == WATER) || (Level->site[x][y].locchar == RUBBLE) ||
       (Level->site[x][y].locchar == LIFT) || (Level->site[x][y].locchar == TRAP))
    {
      /* horses WILL go into water... */
      if(gamestatusp(MOUNTED, GameStatus))
      {
        if(Level->site[x][y].locchar != WATER || Level->site[x][y].p_locf != L_WATER)
        {
          queue_message("You can't convince your steed to continue.");
          setgamestatus(SKIP_MONSTERS, GameStatus);
          return false;
        }
        else
        {
          return true;
        }
      }
      else if(confirmation())
      {
        resetgamestatus(SKIP_MONSTERS, GameStatus);
      }
      else
      {
        setgamestatus(SKIP_MONSTERS, GameStatus);
      }
      return (!gamestatusp(SKIP_MONSTERS, GameStatus));
    }
    else
    {
      resetgamestatus(SKIP_MONSTERS, GameStatus);
      return true;
    }
  }
  else
  {
    resetgamestatus(SKIP_MONSTERS, GameStatus);
    return true;
  }
}

/* check a move attempt in the countryside */
int p_country_moveable(int x, int y)
{
  if(!inbounds(x, y))
  {
    return false;
  }
  else if(optionp(CONFIRM, Player))
  {
    if((Country[x][y].current_terrain_type == CHAOS_SEA) ||
       (Country[x][y].current_terrain_type == MOUNTAINS))
    {
      return (confirmation());
    }
    else
    {
      return true;
    }
  }
  else
  {
    return true;
  }
}

/* search once particular spot */
void searchat(int x, int y)
{
  int i;
  if(inbounds(x, y) && (random_range(3) || Player.status[ALERT]))
  {
    if(loc_statusp(x, y, SECRET, *Level))
    {
      lreset(x, y, SECRET, *Level);
      lset(x, y, CHANGED, *Level);
      if((Level->site[x][y].locchar == OPEN_DOOR) || (Level->site[x][y].locchar == CLOSED_DOOR))
      {
        queue_message("You find a secret door!");
        for(i = 0; i <= 8; i++)
        { /* FIXED! 12/25/98 */
          lset(x + Dirs[0][i], y + Dirs[1][i], STOPS, *Level);
          lset(x + Dirs[0][i], y + Dirs[1][i], CHANGED, *Level);
        }
      }
      else
      {
        queue_message("You find a secret passage!");
      }
      drawvision(Player.x, Player.y);
    }
    if((Level->site[x][y].p_locf >= TRAP_BASE) && (Level->site[x][y].locchar != TRAP) &&
       (Level->site[x][y].p_locf <= TRAP_BASE + NUMTRAPS))
    {
      Level->site[x][y].locchar = TRAP;
      lset(x, y, CHANGED, *Level);
      queue_message("You find a trap!");
      drawvision(Player.x, Player.y);
      resetgamestatus(FAST_MOVE, GameStatus);
    }
  }
}

/* This is to be called whenever anything might change player performance in
   melee, such as changing weapon, statistics, etc. */
void calc_melee()
{
  calc_weight();

  Player.maxweight  = (Player.str * Player.agi * 10);
  Player.absorption = Player.status[PROTECTION];
  Player.defense    = 2 * statmod(Player.agi) + (Player.level / 2);
  Player.hit        = Player.level + statmod(Player.dex) + 1;
  Player.dmg        = statmod(Player.str) + 3;
  Player.speed      = 5 - std::min(4, (statmod(Player.agi) / 2));
  if(Player.status[HASTED] > 0)
  {
    Player.speed = Player.speed / 2;
  }
  if(Player.status[SLOWED] > 0)
  {
    Player.speed = Player.speed * 2;
  }
  if(Player.itemweight > 0)
  {
    switch(Player.maxweight / Player.itemweight)
    {
      case 0:
        Player.speed += 6;
        break;
      case 1:
        Player.speed += 3;
        break;
      case 2:
        Player.speed += 2;
        break;
      case 3:
        Player.speed += 1;
        break;
    }
  }

  if(Player.status[ACCURATE])
  {
    Player.hit += 20;
  }
  if(Player.status[HERO])
  {
    Player.hit += Player.dex;
  }
  if(Player.status[HERO])
  {
    Player.dmg += Player.str;
  }
  if(Player.status[HERO])
  {
    Player.defense += Player.agi;
  }
  if(Player.status[HERO])
  {
    Player.speed = Player.speed / 2;
  }

  Player.speed = std::max(1, std::min(25, Player.speed));

  if(gamestatusp(MOUNTED, GameStatus))
  {
    Player.speed = 3;
    Player.hit += 10;
    Player.dmg += 10;
  }
  else if(Player.rank[MONKS] > 0)
  {
    // monks are faster when not in armor or on horseback
    if(!Player.possessions[O_ARMOR])
    {
      Player.speed += std::min(0, Player.rank[MONKS]-1);
    }
  }

  /* weapon */
  /* have to check for used since it could be a 2h weapon just carried
     in one hand */
  if(Player.possessions[O_WEAPON_HAND])
  {
    if(Player.possessions[O_WEAPON_HAND]->used &&
       ((Player.possessions[O_WEAPON_HAND]->objchar == WEAPON) ||
        (Player.possessions[O_WEAPON_HAND]->objchar == MISSILEWEAPON)))
    {
      Player.hit += Player.possessions[O_WEAPON_HAND]->hit + Player.possessions[O_WEAPON_HAND]->plus;
      Player.dmg += Player.possessions[O_WEAPON_HAND]->dmg + Player.possessions[O_WEAPON_HAND]->plus;
    }
  }

  if(Player.rank[MONKS] > 0)
  {
    if(!Player.possessions[O_WEAPON_HAND])
    {
      Player.hit += Player.rank[MONKS] * Player.level;
      Player.dmg += Player.rank[MONKS] * Player.level;
      Player.defense += Player.rank[MONKS] * Player.level;

      if(Player.rank[MONKS] == MONK_GRANDMASTER)
      {
        Player.dmg *= 3;
      }
    }
  }

  /* shield or defensive weapon */
  if(Player.possessions[O_SHIELD])
  {
    Player.defense += Player.possessions[O_SHIELD]->aux + Player.possessions[O_SHIELD]->plus;
  }

  /* armor */
  if(Player.possessions[O_ARMOR])
  {
    Player.absorption += Player.possessions[O_ARMOR]->dmg;
    Player.defense += Player.possessions[O_ARMOR]->plus - Player.possessions[O_ARMOR]->aux;
  }

  print_combat_stats();
  showflags();
  dataprint();
}

/* player attacks monster m */
void fight_monster(struct monster *m)
{
  int hitmod      = 0;
  int reallyfight = true;

  if(Player.status[AFRAID])
  {
    queue_message("You are much too afraid to fight!");
    reallyfight = false;
  }
  else if(player_on_sanctuary())
  {
    queue_message("You restrain yourself from desecrating this holy place.");
    reallyfight = false;
  }
  else if(Player.status[SHADOWFORM])
  {
    queue_message("Your attack has no effect in your shadowy state.");
    reallyfight = false;
  }
  else if((Player.status[BERSERK] < 1) && (!m_statusp(*m, HOSTILE)))
  {
    if(optionp(BELLICOSE, Player))
    {
      reallyfight = true;
    }
    else
    {
      reallyfight = confirmation();
    }
  }
  else
  {
    reallyfight = true;
  }

  if(reallyfight)
  {
    if(Lunarity == 1)
    {
      hitmod += Player.level;
    }
    else if(Lunarity == -1)
    {
      hitmod -= (Player.level / 2);
    }

    if(!m->attacked)
    {
      Player.alignment -= 2; /* chaotic action */
    }
    m_status_set(*m, AWAKE);
    m_status_set(*m, HOSTILE);
    m->attacked = true;
    Player.hit += hitmod;
    tacplayer(m);
    Player.hit -= hitmod;
  }
}

/* Attempt to break an object o */
int damage_item(pob o)
{
  /* special case -- break star gem */
  if(o->id == ARTIFACTID + 21)
  {
    queue_message("The Star Gem shatters into a million glistening shards....");
    if(Current_Environment == E_STARPEAK)
    {
      if(!gamestatusp(KILLED_LAWBRINGER, GameStatus))
      {
        queue_message("You hear an agonizing scream of anguish and despair.");
      }
      queue_message("A raging torrent of energy escapes in an explosion of magic!");
      queue_message("The energy flows to the apex of Star Peak where there is");
      queue_message("an enormous explosion!");
      annihilate(1);
      queue_message("You seem to gain strength in the chaotic glare of magic!");
      Player.str = std::max(Player.str, Player.maxstr + 5); /* FIXED! 12/25/98 */
      Player.pow = std::max(Player.pow, Player.maxpow + 5); /* ditto */
      Player.alignment -= 200;
      dispose_lost_objects(1, o);
    }
    else
    {
      queue_message("The shards coalesce back together again, and vanish");
      queue_message("with a muted giggle.");
      dispose_lost_objects(1, o);
      Objects[o->id].uniqueness = UNIQUE_UNMADE; /* FIXED! 12/30/98 */
      /* WDT HACK: the above is correct only if UNIQUE_UNMADE means that
       * the artifact hasn't been generated yet.  (Clearly, Omega is a
       * little buggy in that regard with respect to artifacts in general
       * -- it's almost trivial to force two identical artefacts to be
       * generated right now.) */
    }
    return 1;
  }
  else
  {
    if(o->fragility < random_range(30))
    {
      if(o->objchar == STICK)
      {
        queue_message(std::format("Your {} explodes!", (o->blessing >= 0 ? o->truename : o->cursestr)));
        if(o->charge < 1)
        {
          queue_message(" Fzzz... Out of Power... Oh well...");
        }
        else
        {
          queue_message(" Ka-Blamm!!!");
          /* general case. Some sticks will eventually do special things */
          manastorm(Player.x, Player.y, o->charge * o->level * 10);
          dispose_lost_objects(1, o);
        }
        return 1;
      }
      else if((o->blessing > 0) && (o->level > random_range(10)))
      {
        queue_message("Your " + itemid(o) + " glows strongly.");
        return 0;
      }
      else if((o->blessing < -1) && (o->level > random_range(10)))
      {
        queue_message(std::format("You hear an evil giggle from your {}.", itemid(o)));
        return 0;
      }
      else if(o->plus > 0)
      {
        queue_message(std::format("Your {} glos and fades.", itemid(o)));
        o->plus--;
        return 0;
      }
      else
      {
        if(o->blessing > 0)
        {
          queue_message("You hear a faint despairing cry!");
        }
        else if(o->blessing < 0)
        {
          queue_message("You hear an agonized scream!");
        }
        queue_message(std::format("Your {} shatters in a thousand lost fragments!", itemid(o)));
        dispose_lost_objects(1, o);
        return 1;
      }
    }
    return 0;
  }
}

/* do dmg points of damage of type dtype, from source fromstring */
void p_damage(int dmg, int dtype, const std::string &fromstring)
{
  if(!p_immune(dtype))
  {
    if(gamestatusp(FAST_MOVE, GameStatus))
    {
      drawvision(Player.x, Player.y);
      resetgamestatus(FAST_MOVE, GameStatus);
    }
    if(dtype == NORMAL_DAMAGE)
    {
      Player.hp -= std::max(1, (dmg - Player.absorption));
    }
    else
    {
      Player.hp -= dmg;
    }
    if(Player.hp < 1)
    {
      p_death(fromstring);
    }
  }
  else
  {
    queue_message("You resist the effects!");
  }
  dataprint();
}

/* game over, you lose! */
void p_death(const std::string &fromstring)
{
  Player.hp = 0;
  dataprint();
  queue_message("You died!", true);
  more_wait();
  display_death(fromstring);
#ifdef SAVE_LEVELS
  kill_all_levels();
#endif
  endgraf();
  exit(0);
}

/* move the cursor around, like for firing a wand, sets x and y to target */
void setspot(int &x, int &y)
{
  queue_message("Targeting.... '?' for help.");
  int cursor_visibility = curs_set(1);
  omshowcursor(x, y);
  int player_input;
  do
  {
    player_input = get_level_input();
    switch(player_input)
    {
      case 'h':
      case '4':
      case KEY_LEFT:
        movecursor(x, y, -1, 0);
        break;
      case 'j':
      case '2':
      case KEY_DOWN:
        movecursor(x, y, 0, 1);
        break;
      case 'k':
      case '8':
      case KEY_UP:
        movecursor(x, y, 0, -1);
        break;
      case 'l':
      case '6':
      case KEY_RIGHT:
        movecursor(x, y, 1, 0);
        break;
      case 'b':
      case '1':
        movecursor(x, y, -1, 1);
        break;
      case 'n':
      case '3':
        movecursor(x, y, 1, 1);
        break;
      case 'y':
      case '7':
        movecursor(x, y, -1, -1);
        break;
      case 'u':
      case '9':
        movecursor(x, y, 1, -1);
        break;
      case KEY_MOUSE:
      {
        MEVENT event;
        while(getmouse(&event) == OK)
        {
          if(event.bstate & BUTTON1_DOUBLE_CLICKED)
          {
            x = event.x + HorizontalOffset;
            y = event.y + ScreenOffset;
            omshowcursor(x, y);
            player_input = '.';
          }
          else if(event.bstate & BUTTON1_PRESSED)
          {
            x = event.x + HorizontalOffset;
            y = event.y + ScreenOffset;
            omshowcursor(x, y);
          }
        }
      }
        break;
      case '?':
        queue_message("Use vi keys or numeric keypad to move cursor to target.");
        queue_message("Hit the '.' key when done, or ESCAPE to abort.");
        break;
    }
  } while(player_input != '.' && player_input != ESCAPE);
  curs_set(cursor_visibility);
  if(player_input == ESCAPE)
  {
    x = y = ABORT;
  }
  screencheck(Player.x, Player.y);
}

/* get a direction: return index into Dirs array corresponding to direction */
int getdir()
{
  while(1)
  {
    queue_message("Select direction [hjklyubn, ESCAPE to quit]: ");
    switch(mgetc())
    {
      case '4':
      case 'h':
      case 'H':
        return (5);
      case '2':
      case 'j':
      case 'J':
        return (6);
      case '8':
      case 'k':
      case 'K':
        return (7);
      case '6':
      case 'l':
      case 'L':
        return (4);
      case '7':
      case 'y':
      case 'Y':
        return (3);
      case '9':
      case 'u':
      case 'U':
        return (1);
      case '1':
      case 'b':
      case 'B':
        return (2);
      case '3':
      case 'n':
      case 'N':
        return (0);
      case ESCAPE:
        return (ABORT);
      default:
        queue_message("That's not a direction!");
    }
  }
}

/* functions describes monster m's state for examine function */
std::string mstatus_string(struct monster *m)
{
  std::string monster_status;
  if(m_statusp(*m, M_INVISIBLE) && !Player.status[TRUESIGHT])
  {
    monster_status = "Some invisible creature";
  }
  else if(m->uniqueness == COMMON)
  {
    if(m->hp < Monsters[m->id].hp / 3)
    {
      monster_status = "a grievously injured ";
    }
    else if(m->hp < Monsters[m->id].hp / 2)
    {
      monster_status = "a severely injured ";
    }
    else if(m->hp < Monsters[m->id].hp)
    {
      monster_status = "an injured ";
    }
    else
    {
      monster_status = getarticle(m->monstring);
    }
    if(m->level > Monsters[m->id].level)
    {
      monster_status += std::format(" (level {}) ", wordnum(m->level+1-Monsters[m->id].level));
    }
    monster_status += m->monstring;
  }
  else
  {
    monster_status = m->monstring;
    if(m->hp < Monsters[m->id].hp / 3)
    {
      monster_status += " who is grievously injured ";
    }
    else if(m->hp < Monsters[m->id].hp / 2)
    {
      monster_status += " who is severely injured ";
    }
    else if(m->hp < Monsters[m->id].hp)
    {
      monster_status += " who is injured ";
    }
  }
  return monster_status;
}

/* for the examine function */
std::string describe_player()
{
  std::string description;
  if(Player.hp < (Player.maxhp / 5))
  {
    description = "A grievously injured ";
  }
  else if(Player.hp < (Player.maxhp / 2))
  {
    description = "A seriously wounded ";
  }
  else if(Player.hp < Player.maxhp)
  {
    description = "A somewhat bruised ";
  }
  else
  {
    description = "A fit ";
  }

  if(Player.status[SHADOWFORM])
  {
    description += "shadow";
  }
  else
  {
    description += levelname(Player.level);
  }
  description += " named " + Player.name;
  if(gamestatusp(MOUNTED, GameStatus))
  {
    description += " (riding a horse).";
  }
  else
  {
    description += ".";
  }
  return description;
}

/* access to player experience... */
/* share out experience among guild memberships */
void gain_experience(int amount)
{
  int i, count = 0, share;
  Player.xp += static_cast<long>(amount);
  gain_level(); /* actually, check to see if should gain level */
  for(i = 0; i < NUMRANKS; i++)
  {
    if(Player.guildxp[i] > 0)
    {
      count++;
    }
  }
  share = amount / (std::max(count, 1));
  for(i = 0; i < NUMRANKS; i++)
  {
    if(Player.guildxp[i] > 0)
    {
      Player.guildxp[i] += share;
    }
  }
}

/* try to hit a monster in an adjacent space. If there are none
   return false. Note if you're berserk you get to attack ALL
   adjacent monsters! */
bool goberserk()
{
  bool wentberserk = false;
  std::string meleestr;
  meleestr = Player.meleestr;
  Player.meleestr = "LLLCLH";
  for(uint8_t i = 0; i < 8; ++i)
  {
    if(Level->site[Player.x + Dirs[0][i]][Player.y + Dirs[1][i]].creature)
    {
      wentberserk = true;
      fight_monster(Level->site[Player.x + Dirs[0][i]][Player.y + Dirs[1][i]].creature);
    }
  }
  Player.meleestr = meleestr;
  return wentberserk;
}

/* identifies a trap for examine() by its aux value */
std::string trapid(int trapno)
{
  switch(trapno)
  {
    case L_TRAP_SIREN:
      return "A siren trap";
    case L_TRAP_DART:
      return "A dart trap";
    case L_TRAP_PIT:
      return "A pit";
    case L_TRAP_SNARE:
      return "A snare";
    case L_TRAP_BLADE:
      return "A blade trap";
    case L_TRAP_FIRE:
      return "A fire trap";
    case L_TRAP_TELEPORT:
      return "A teleport trap";
    case L_TRAP_DISINTEGRATE:
      return "A disintegration trap";
    case L_TRAP_DOOR:
      return "A trap door";
    case L_TRAP_MANADRAIN:
      return "A manadrain trap";
    case L_TRAP_ACID:
      return "An acid shower trap";
    case L_TRAP_SLEEP_GAS:
      return "A sleep gas trap";
    case L_TRAP_ABYSS:
      return "A concealed entrance to the abyss";
    default:
      return "A completely inoperative trap";
  }
}

/* checks current food status of player, every hour, and when food is eaten */
void foodcheck()
{
  if(Player.food > 48)
  {
    queue_message("You vomit up your huge meal.");
    Player.food = 12;
  }
  else if(Player.food == 30)
  {
    queue_message("Time for a smackerel of something.");
  }
  else if(Player.food == 20)
  {
    queue_message("You feel hungry.");
  }
  else if(Player.food == 12)
  {
    queue_message("You are ravenously hungry.");
  }
  else if(Player.food == 3)
  {
    queue_message("You feel weak.");
    if(gamestatusp(FAST_MOVE, GameStatus))
    {
      drawvision(Player.x, Player.y);
      resetgamestatus(FAST_MOVE, GameStatus);
    }
  }
  else if(Player.food < 0)
  {
    if(gamestatusp(FAST_MOVE, GameStatus))
    {
      drawvision(Player.x, Player.y);
      resetgamestatus(FAST_MOVE, GameStatus);
    }
    queue_message("You're starving!");
    p_damage(-5 * Player.food, UNSTOPPABLE, "starvation");
  }
  showflags();
}

/* see whether room should be illuminated */
void roomcheck()
{
  static int oldroomno = -1;
  static plv oldlevel  = nullptr;
  int        roomno    = Level->site[Player.x][Player.y].roomnumber;

  if((roomno == RS_CAVERN) || (roomno == RS_SEWER_DUCT) || (roomno == RS_KITCHEN) ||
     (roomno == RS_BATHROOM) || (roomno == RS_BEDROOM) || (roomno == RS_DININGROOM) ||
     (roomno == RS_CLOSET) || (roomno > ROOMBASE))
  {
    if((!loc_statusp(Player.x, Player.y, LIT, *Level)) && (!Player.status[BLINDED]) &&
       (Player.status[ILLUMINATION] || (difficulty() < 6)))
    {
      showroom(Level->site[Player.x][Player.y].roomnumber);
      spreadroomlight(Player.x, Player.y, roomno);
      levelrefresh();
    }
  }
  if((oldroomno != roomno) || (oldlevel != Level))
  {
    showroom(roomno);
    oldroomno = roomno;
    oldlevel  = Level;
  }
}

/* ask for mercy */
void surrender(struct monster *m)
{
  int  i;
  long bestitem, bestvalue;

  switch(random_range(4))
  {
    case 0:
      queue_message("You grovel at the monster's feet...");
      break;
    case 1:
      queue_message("You cry 'uncle'!");
      break;
    case 2:
      queue_message("You beg for mercy.");
      break;
    case 3:
      queue_message("You yield to the monster.");
      break;
  }
  if(m->id == GUARD)
  {
    if(m_statusp(*m, HOSTILE))
    {
      monster_talk(m);
    }
    else
    {
      queue_message("The guard (bored): Have you broken a law? [yn] ");
      if(ynq() == 'y')
      {
        queue_message("The guard grabs you, and drags you to court.");
        send_to_jail();
      }
      else
      {
        queue_message("Then don't bother me. Scat!");
      }
    }
  }
  else if((m->talkf == M_NO_OP) || (m->talkf == M_TALK_STUPID))
  {
    queue_message("Your plea is ignored.");
  }
  else
  {
    queue_message("Your surrender is accepted.");
    if(Player.cash > 0)
    {
      queue_message(" All your gold is taken....");
    }
    Player.cash = 0;
    bestvalue   = 0;
    bestitem    = ABORT;
    for(i = 1; i < MAXITEMS; i++)
    {
      if(Player.possessions[i])
      {
        if(bestvalue < true_item_value(Player.possessions[i]))
        {
          bestitem  = i;
          bestvalue = true_item_value(Player.possessions[i]);
        }
      }
    }
    if(bestitem != ABORT)
    {
      queue_message("You also give away your best item... ");
      queue_message(itemid(Player.possessions[bestitem]));
      queue_message(".");
      givemonster(m, Player.possessions[bestitem]);
      conform_unused_object(Player.possessions[bestitem]);
      Player.possessions[bestitem] = nullptr;
    }
    queue_message("You feel less experienced... ");
    Player.xp = std::max(0l, Player.xp - m->xpv);
    queue_message("The monster seems more experienced!");
    m->level = (std::min(10, m->level + 1));
    m->hp += m->level * 20;
    m->hit += m->level;
    m->dmg += m->level;
    m->ac += m->level;
    m->xpv += m->level * 10;
    if((m->talkf == M_TALK_EVIL) && random_range(10))
    {
      queue_message("It continues to attack you, laughing evilly!");
      m_status_set(*m, HOSTILE);
      m_status_reset(*m, GREEDY);
    }
    else if(m->id == HORNET || m->id == GUARD)
    {
      queue_message("It continues to attack you. ");
    }
    else
    {
      queue_message("The monster leaves, chuckling to itself....");
      m_teleport(m);
    }
  }
  dataprint();
}

/* threaten a monster */
void threaten(struct monster *m)
{
  char response;
  switch(random_range(4))
  {
    case 0:
      queue_message("You demand that your opponent surrender!");
      break;
    case 1:
      queue_message("You threaten to do bodily harm to it.");
      break;
    case 2:
      queue_message("You attempt to bluster it into submission.");
      break;
    case 3:
      queue_message("You try to cow it with your awesome presence.");
      break;
  }
  if(!m_statusp(*m, HOSTILE))
  {
    queue_message("You only annoy it with your futile demand.");
    m_status_set(*m, HOSTILE);
  }
  else if(((m->level * 2 > Player.level) && (m->hp > Player.dmg)) || (m->uniqueness != COMMON))
  {
    queue_message("It sneers contemptuously at you.");
  }
  else if((m->talkf != M_TALK_GREEDY) && (m->talkf != M_TALK_HUNGRY) && (m->talkf != M_TALK_EVIL) &&
          (m->talkf != M_TALK_MAN) && (m->talkf != M_TALK_BEG) && (m->talkf != M_TALK_THIEF) &&
          (m->talkf != M_TALK_MERCHANT) && (m->talkf != M_TALK_IM))
  {
    queue_message("Your demand is ignored");
  }
  else
  {
    queue_message("It yields to your mercy.");
    Player.alignment += 3;
    queue_message("Kill it, rob it, or free it? [krf] ");
    do
    {
      response = static_cast<char>(mcigetc());
    } while((response != 'k') && (response != 'r') && (response != 'f'));
    if(response == 'k')
    {
      m_death(m);
      queue_message("You treacherous rogue!");
      Player.alignment -= 13;
    }
    else if(response == 'r')
    {
      Player.alignment -= 2;
      queue_message("It drops its treasure and flees.");
      m_dropstuff(m);
      m->hp                            = -1;
      Level->site[m->x][m->y].creature = nullptr;
      putspot(m->x, m->y, getspot(m->x, m->y, false));
    }
    else
    {
      Player.alignment += 2;
      queue_message("'If you love something set it free ... '");
      if(random_range(100) == 13)
      {
        queue_message("'...If it doesn't come back, hunt it down and kill it.'");
      }
      queue_message("It departs with a renewed sense of its own mortality.");
      m->hp                            = -1;
      Level->site[m->x][m->y].creature = nullptr;
      putspot(m->x, m->y, getspot(m->x, m->y, false));
    }
  }
}

/* name of the player's experience level */
std::string levelname(int level)
{
  switch(level)
  {
    case 0:
      return "Neophyte";
    case 1:
      return "Beginner";
    case 2:
      return "Tourist";
    case 3:
      return "Traveller";
    case 4:
      return "Wayfarer";
    case 5:
      return "Peregrinator";
    case 6:
      return "Wanderer";
    case 7:
      return "Hunter";
    case 8:
      return "Scout";
    case 9:
      return "Trailblazer";
    case 10:
      return "Discoverer";
    case 11:
      return "Explorer";
    case 12:
      return "Senior Explorer";
    case 13:
      return "Ranger";
    case 14:
      return "Ranger Captain";
    case 15:
      return "Ranger Knight";
    case 16:
      return "Adventurer";
    case 17:
      return "Experienced Adventurer";
    case 18:
      return "Skilled Adventurer";
    case 19:
      return "Master Adventurer";
    case 20:
      return "Hero";
    case 21:
      return "Superhero";
    case 22:
      return "Demigod";
    default:
      if(level < 100)
      {
        int order = level / 10 - 2;
        return std::format("Order {} Master of Omega", order);
      }
      else
      {
        return "Ultimate Master of Omega";
      }
  }
}
