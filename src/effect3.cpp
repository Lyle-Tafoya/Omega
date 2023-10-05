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

/* effect3.c */

#include "glob.h"
#include "interactive_menu.hpp"
#include "scr.h"
#include "spell.h"

#include <algorithm>
#include <format>
#include <string>
#include <vector>

extern void item_equip(object *);
extern void item_unequip(object *);
extern interactive_menu *menu;

int list_monsters()
{
  int i, itemno;
  queue_message("Show ID list? ");
  if(ynq() == 'y')
  {
    do
    {
      queue_message("Summon monster: ");
      std::vector<std::string> lines;
      for(i = 0; i < NUMMONSTERS; i++)
      {
        lines.emplace_back(std::format("{}:{}", i+1, Monsters[i].monstring));
      }
      menu->load(lines);
      int player_input = menu->get_player_input();
      ungetch(player_input);

      itemno = (int)parsenum() - 1;
      if((itemno < 0) || (itemno > NUMMONSTERS - 1))
      {
        queue_message("How about trying a real monster?");
      }
    } while((itemno < 0) || (itemno > NUMMONSTERS - 1));
  }
  else
  {
    do
    {
      queue_message("Summon monster: ");
      itemno = (int)parsenum() - 1;
    } while((itemno < 0) || (itemno > NUMMONSTERS - 1));
  }
  return (itemno);
}

/* if know id, then summon that monster; else (if < 0) get one. */
void summon(int blessing, int id)
{
  int i, looking = true, x, y;
  pml tml;

  if(id < 0)
  {
    if(blessing > 0)
    {
      id = list_monsters();
      xredraw();
    }
    /* for (id ==0) case, see below -- get a "fair" monster */
    else if(blessing < 0)
    {
      id = random_range(NUMMONSTERS);
    }
  }
  for(i = 0; ((i < 8) && looking); i++)
  {
    x       = Player.x + Dirs[0][i];
    y       = Player.y + Dirs[1][i];
    looking = (!inbounds(x, y) || Level->site[x][y].locchar != FLOOR ||
               Level->site[x][y].creature);
  }

  if(!looking)
  {
    if((blessing == 0) && (id < 0))
    {
      Level->site[x][y].creature = m_create(x, y, WANDERING, difficulty());
    }
    else
    {
      Level->site[x][y].creature = make_creature(id);
    }
    Level->site[x][y].creature->x = x;
    Level->site[x][y].creature->y = y;
    tml                           = new monsterlist;
    tml->m                        = Level->site[x][y].creature;
    if(blessing > 0)
    {
      m_status_reset(*tml->m, HOSTILE);
    }
    else if(blessing < 0)
    {
      m_status_set(*tml->m, HOSTILE);
    }
    tml->next    = Level->mlist;
    Level->mlist = tml;
  }
}

int itemlist(int itemindex, int num)
{
  append_message("Show ID list? ", true);

  int player_input = ynq();
  append_message("Item ID? ", true);
  if(player_input == 'y')
  {
    std::vector<std::string> lines;
    for(int i = 0; i < num; ++i)
    {
      lines.emplace_back(std::format("{}:{}", i+1, Objects[i + itemindex].truename));
    }
    menu->load(lines);
    player_input = menu->get_player_input();
    ungetch(player_input);
  }
  int itemno = (int)parsenum() - 1;
  if(itemno >= num || itemno < 0)
  {
    itemno = ABORT;
  }
  return itemno;
}

/* uncurse all items, cure diseases, and neutralize poison */
void cleanse(int blessing)
{
  int i;

  if(blessing > -1)
  {
    if(blessing > 0)
    {
      for(i = 0; i < MAXITEMS; i++)
      {
        if(Player.possessions[i])
        {
          if((Player.possessions[i]->used) && (Player.possessions[i]->blessing < 0))
          {
            item_unequip(Player.possessions[i]);
            Player.possessions[i]->blessing = 0;
            item_equip(Player.possessions[i]);
          }
        }
      }
    }

    if(Player.status[POISONED] > 0)
    {
      Player.status[POISONED] = 0;
    }
    if(Player.status[DISEASED] > 0)
    {
      Player.status[DISEASED] = 0;
    }
    showflags();
    queue_message("You feel radiant!");
  }
  else
  {
    Player.status[POISONED] += 10;
    Player.status[DISEASED] += 10;
    queue_message("You feel besmirched!");
    showflags();
  }
}

void annihilate(int blessing)
{
  pml ml;
  int i;

  if(blessing == 0)
  {
    queue_message("Lightning strikes flash all around you!!!");
    for(i = 0; i < 9; i++)
    {
      if(Level->site[Player.x + Dirs[0][i]][Player.y + Dirs[1][i]].creature)
      {
        m_death(Level->site[Player.x + Dirs[0][i]][Player.y + Dirs[1][i]].creature);
      }
    }
  }
  if(blessing > 0)
  {
    if(Current_Environment == E_COUNTRYSIDE)
    {
      queue_message("Bolts of lightning flash down for as far as you can see!!!");
      queue_message("There is a rain of small birds and insects from the sky, and you");
      queue_message("notice that you can't hear any animal noises around here any "
             "more...");
      Player.alignment -= 3;
    }
    else
    {
      queue_message("Thousands of bolts of lightning flash throughout the level!!!");
      for(ml = Level->mlist; ml; ml = ml->next)
      {
        if(ml->m && ml->m->hp > 0)
        {
          m_death(ml->m);
        }
      }
    }
  }
  else
  {
    queue_message("You are hit by a bolt of mystic lightning!");
    p_death("self-annihilation");
  }
}

void sleep_monster(int blessing)
{
  int x = Player.x, y = Player.y;
  struct monster *target;

  if(blessing == 0)
  {
    setspot(&x, &y);
  }

  if(blessing < 0)
  {
    sleep_player(abs(blessing) + 2);
  }
  else if(blessing > 0)
  {
    queue_message("A silence pervades the area.");
    for(pml ml = Level->mlist; ml; ml = ml->next)
    {
      m_status_reset(*ml->m, AWAKE);
      ml->m->wakeup = 0;
    }
  }
  else
  {
    target = Level->site[x][y].creature;
    if(target)
    {
      std::string monster_name;
      if(target->uniqueness == COMMON)
      {
        monster_name = std::format("The {}", target->monstring);
      }
      else
      {
        monster_name = target->monstring;
      }
      if(!m_immunityp(*target, SLEEP))
      {
        queue_message(std::format("{} seems to have fallen asleep.", monster_name));
        m_status_reset(*target, AWAKE);
        target->wakeup = 0;
      }
      else
      {
        queue_message(std::format("{} is bright eyed, and bushy tailed!", monster_name));
      }
    }
    else
    {
      queue_message("Nothing to sleep there!");
    }
  }
}

void sleep_player(int amount)
{
  if(Player.status[SLEPT] == 0)
  { /* prevent player from sleeping forever */
    queue_message("You feel sleepy...");
    if(!p_immune(SLEEP))
    {
      Player.status[SLEPT] += random_range(amount * 2) + 2;
    }
    else
    {
      queue_message("but you shrug off the momentary lassitude.");
    }
  }
}

void hide(int x, int y)
{
  if(inbounds(x, y))
  {
    lset(x, y, SECRET, *Level);
    lset(x, y, CHANGED, *Level);
    putspot(x, y, WALL);
    queue_message("You feel sneaky.");
  }
}

void clairvoyance(int vision)
{
  int i, j;
  int x = Player.x, y = Player.y;
  queue_message("Clairvoyance... ");
  setspot(&x, &y);
  for(i = x - vision; i < x + vision + 1; i++)
  {
    for(j = y - vision; j < y + vision + 1; j++)
    {
      if(inbounds(i, j))
      {
        Level->site[i][j].showchar = SPACE;
        lreset(i, j, SECRET, *Level);
        lset(i, j, CHANGED, *Level);
        dodrawspot(i, j);
      }
    }
  }
  levelrefresh();
}

void aggravate()
{
  pml tm;

  for(tm = Level->mlist; tm; tm = tm->next)
  {
    m_status_set(*tm->m, AWAKE);
    m_status_set(*tm->m, HOSTILE);
  }
}

void learnspell(int blessing)
{
  int i, done = false;
  if(blessing < 0)
  {
    for(i = spell::NUM_SPELLS; ((i > -1) && (!done)); i--)
    {
      if(spell::Spells[i].known)
      {
        done                        = true;
        Objects[SCROLLID + 1].known = true;
        queue_message("You feel forgetful.");
        spell::Spells[i].known = false;
      }
    }
    if(i == ABORT)
    {
      queue_message("You feel fortunate.");
    }
  }
  else
  {
    Objects[SCROLLID + 1].known = true;
    spell::spell_id id = static_cast<spell::spell_id>(random_range(spell::NUM_SPELLS));
    queue_message("Spell Research");
    if((random_range(4 * spell::Spells[id].powerdrain) + spell::Spells[id].powerdrain) <
       (4 * Player.iq + 8 * Player.level))
    {
      queue_message(" -- Research successful: ");
      queue_message(spell::spell_names[id]);
      if(spell::Spells[id].known)
      {
        queue_message("...is now easier to cast.");
        spell::Spells[id].powerdrain = (spell::Spells[id].powerdrain + 1) / 2;
      }
      else
      {
        queue_message("...is added to your repertoire");
        spell::Spells[id].known = true;
        gain_experience(spell::Spells[id].powerdrain * 10);
      }
    }
    else
    {
      queue_message(" -- Research unsuccessful.");
    }
  }
}

void amnesia()
{
  int i, j;
  for(j = 0; j < LENGTH; j++)
  {
    for(i = 0; i < WIDTH; i++)
    {
      lreset(i, j, SEEN, *Level);
    }
  }

  erase_level();
  drawvision(Player.x, Player.y);
}

/*affects player only */
void level_drain(int levels, const std::string &source)
{
  int decrement = ((int)(Player.maxhp / (Player.level + 1)));

  Player.level -= levels;

  Player.maxhp -= (levels * decrement);
  Player.hp -= (levels * decrement);

  if((Player.hp < 1) || (Player.level < 0))
  {
    p_death(source);
  }
}

void disrupt(int x, int y, int amount)
{
  struct monster *target;

  if((x == Player.x) && (y == Player.y))
  {
    queue_message("You feel disrupted!");
    p_damage(amount, NORMAL_DAMAGE, "magical disruption");
  }
  else
  {
    target = Level->site[x][y].creature;
    if(target)
    {
      std::string monster_name;
      if(target->uniqueness == COMMON)
      {
        monster_name = std::format("The {}", target->monstring);
      }
      else
      {
        monster_name = target->monstring;
      }
      if(!m_immunityp(*target, NORMAL_DAMAGE))
      {
        queue_message(std::format("{} was blasted!", monster_name));
        m_damage(target, amount, NORMAL_DAMAGE);
        target->wakeup = 0;
      }
      else
      {
        queue_message(std::format("{} does not seem affected.", monster_name));
      }
    }
  }
}

void disintegrate(int x, int y)
{
  struct monster *target;
  if(!inbounds(x, y))
  {
    queue_message("You feel a sense of wastage.");
  }
  else if((x == Player.x) && (y == Player.y))
  {
    if(Player.possessions[O_CLOAK])
    {
      queue_message("Your cloak disintegrates!");
      dispose_lost_objects(1, Player.possessions[O_CLOAK]);
    }
    else if(Player.possessions[O_ARMOR])
    {
      queue_message("Your armor disintegrates!");
      dispose_lost_objects(1, Player.possessions[O_ARMOR]);
    }
    else
    {
      queue_message("Uh, oh....");
      queue_message("Zzzap! You've been disintegrated!");
      p_damage(250, UNSTOPPABLE, "disintegration");
    }
  }
  else
  {
    if(!view_los_p(Player.x, Player.y, x, y))
    {
      setgamestatus(SUPPRESS_PRINTING, GameStatus);
    }
    if((target = Level->site[x][y].creature))
    {
      if(target->uniqueness == COMMON)
      {
        queue_message(std::format("The {} disintegrates!", target->monstring));
      }
      else
      {
        queue_message(std::format("{} disintegrates!", target->monstring));
      }
      m_damage(target, 100, UNSTOPPABLE);
      if(target->hp > 0)
      {
        queue_message("It was partially protected by its armor.");
      }
    }
    else if(Level->site[x][y].locchar == ALTAR)
    {
      queue_message("Zzzzap! the altar seems unaffected...");
      queue_message("But an angry deity retaliates....");
      disintegrate(Player.x, Player.y);
    }
    else if(Level->site[x][y].p_locf == L_TRAP_PIT)
    {
      if(Current_Environment == Current_Dungeon)
      {
        queue_message("A hole is blasted in the base of the pit!");
        Level->site[x][y].locchar = TRAP;
        Level->site[x][y].p_locf  = L_TRAP_DOOR;
        Level->site[x][y].aux     = spell::DISINTEGRATE;
        lset(x, y, CHANGED, *Level);
      }
      else
      {
        queue_message("The hole just gets deeper....");
      }
    }
    else if(Level->site[x][y].locchar == FLOOR)
    {
      queue_message("You zap a hole in the floor!");
      Level->site[x][y].locchar = TRAP;
      Level->site[x][y].p_locf  = L_TRAP_PIT;
      lset(x, y, CHANGED, *Level);
    }
    else if((Level->site[x][y].locchar == WALL) || (Level->site[x][y].locchar == OPEN_DOOR) ||
            (Level->site[x][y].locchar == CLOSED_DOOR) || (Level->site[x][y].locchar == PORTCULLIS) ||
            (Level->site[x][y].locchar == STATUE))
    {
      queue_message("The site is reduced to rubble!");
      if(Level->site[x][y].locchar == WALL)
      {
        tunnelcheck();
      }
      Level->site[x][y].p_locf  = L_RUBBLE;
      Level->site[x][y].locchar = RUBBLE;
      lreset(x, y, SECRET, *Level);
      lset(x, y, CHANGED, *Level);
    }
    else if((Level->site[x][y].locchar == RUBBLE) || (Level->site[x][y].locchar == TRAP))
    {
      queue_message("The site is blasted clear!");
      Level->site[x][y].p_locf  = L_NO_OP;
      Level->site[x][y].locchar = FLOOR;
      lreset(x, y, SECRET, *Level);
      lset(x, y, CHANGED, *Level);
    }
    else if(Level->site[x][y].locchar == HEDGE)
    {
      if(Level->site[x][y].p_locf == L_TRIFID)
      {
        queue_message("The trifid screams as it disintgrates!");
        gain_experience(50);
        Level->site[x][y].p_locf  = L_NO_OP;
        Level->site[x][y].locchar = FLOOR;
        lreset(x, y, SECRET, *Level);
        lset(x, y, CHANGED, *Level);
      }
      else
      {
        queue_message("The hedge is blasted away!");
        Level->site[x][y].p_locf  = L_NO_OP;
        Level->site[x][y].locchar = FLOOR;
        lreset(x, y, SECRET, *Level);
        lset(x, y, CHANGED, *Level);
      }
    }
    else
    {
      queue_message("The blast has no effect.");
    }
    if(!view_los_p(Player.x, Player.y, x, y))
    {
      resetgamestatus(SUPPRESS_PRINTING, GameStatus);
    }
    else
    {
      plotspot(x, y, true);
    }
  }
}

void acid_cloud()
{
  queue_message("You are caught in an acid cloud!  ");
  if(Player.possessions[O_CLOAK])
  {
    (void)damage_item(Player.possessions[O_CLOAK]);
    queue_message("You are burned by acid.");
    p_damage(3, ACID, "an acid cloud");
  }
  else if(Player.possessions[O_ARMOR])
  {
    queue_message("You are burned by acid.");
    p_damage(3, ACID, "an acid cloud");
    (void)damage_item(Player.possessions[O_ARMOR]);
  }
  else if(p_immune(ACID))
  {
    queue_message("You resist the effects!");
    return;
  }
  else
  {
    queue_message("The acid eats away at your bare skin!");
    p_damage(25, ACID, "an acid cloud");
  }
}

/* teleport player */
void p_teleport(int type)
{
  int x = Player.x, y = Player.y;
  drawspot(x, y);
  if(type < 0)
  {
    x = random_range(WIDTH);
    y = random_range(LENGTH);
    if((Level->site[x][y].locchar != FLOOR) && (Level->site[x][y].locchar != OPEN_DOOR))
    {
      queue_message("You teleported into a solid object....");
      queue_message("You are dead!");
      p_death("teleportation into a solid object");
    }
    else
    {
      Player.x = x;
      Player.y = y;
    }
  }
  else if(type == 0)
  {
    findspace(&(Player.x), &(Player.y), -1);
  }
  else
  {
    setspot(&Player.x, &Player.y);
    if(Level->site[Player.x][Player.y].locchar != FLOOR ||
       Level->site[Player.x][Player.y].creature)
    {
      queue_message("You feel deflected.");
      p_teleport(0);
    }
  }
  screencheck(Player.x, Player.y);
  roomcheck();
}

void p_poison(int toxicity)
{
  queue_message("You feel sick.");
  if(!p_immune(POISON))
  {
    Player.status[POISONED] += toxicity;
  }
  else
  {
    queue_message("The sickness fades!");
  }
  showflags();
}

void apport(int blessing)
{
  int i, index, x = Player.x, y = Player.y;
  if(blessing > -1)
  {
    queue_message("Apport from:");
    setspot(&x, &y);
    if(Level->site[x][y].things)
    {
      pickup_at(x, y);
      plotspot(x, y, true);
    }
    else
    {
      queue_message("There's nothing there to apport!");
    }
  }
  else
  {
    queue_message("You have a sense of loss.");
    for(i = 0; i < abs(blessing); i++)
    {
      index = random_item();
      if(index != ABORT)
      {
        drop_at(x, y, Player.possessions[index]);
        dispose_lost_objects(Player.possessions[index]->number, Player.possessions[index]);
      }
    }
  }
}

void strategic_teleport(int blessing)
{
  int new_env;

  /* WDT HACK: Game balance issue: the star gem is supposed to be the only
   * way out of the astral plane (including the Circle of Sorcerors).  However,
   * Hy Magic offers the Location wish, and some artifacts grant this
   * as well.  Seems to me that Hy Magic ought to allow it, and nothing
   * else (aside from the Star Gem, of course). */
  if((Current_Environment == E_CIRCLE || Current_Environment == E_ASTRAL) &&
     !gamestatusp(CHEATED, GameStatus))
  {
    queue_message("Some property of this eerie place interferes with the magic!\n");
    return;
  }
  queue_message("Magic portals open up all around you!");
  if(blessing < 0)
  {
    queue_message("You are dragged into one!");
    change_environment(E_COUNTRYSIDE);
    do
    {
      Player.x = random_range(WIDTH);
      Player.y = random_range(LENGTH);
    } while(Country[Player.x][Player.y].base_terrain_type == CHAOS_SEA);
  }
  else
  {
    queue_message("Below each portal is a caption. Enter which one:");
    menuclear();
    menuprint("a: Rampart\n");
    menuprint("b: Village of Star View\n");
    menuprint("c: Village of Woodmere\n");
    menuprint("d: Village of Stormwatch\n");
    menuprint("e: Village of Thaumaris\n");
    menuprint("f: Village of Skorch\n");
    menuprint("g: Village of Whorfen\n");
    menuprint("h: Temple of the Noose\n");
    menuprint("i: The Parthenon\n");
    menuprint("j: Temple of the Black Hand\n");
    menuprint("k: Temple of the Hidden Moon\n");
    menuprint("l: WoodHenge\n");
    menuprint("m: Temple of Destiny\n");
    menuprint("n: HellWell Volcano\n");
    if(gamestatusp(CHEATED, GameStatus))
    {
      menuprint("z: Anywhere\n");
    }
    menuprint("ANYTHING ELSE: Avoid entering a portal.");
    showmenu();
    switch((char)mcigetc())
    {
      case 'a':
        change_environment(E_COUNTRYSIDE);
        Player.x = 27;
        Player.y = 19;
        break;
      case 'b':
        change_environment(E_COUNTRYSIDE);
        Player.x = 56;
        Player.y = 5;
        break;
      case 'c':
        change_environment(E_COUNTRYSIDE);
        Player.x = 35;
        Player.y = 11;
        break;
      case 'd':
        change_environment(E_COUNTRYSIDE);
        Player.x = 10;
        Player.y = 40;
        break;
      case 'e':
        change_environment(E_COUNTRYSIDE);
        Player.x = 7;
        Player.y = 6;
        break;
      case 'f':
        change_environment(E_COUNTRYSIDE);
        Player.x = 41;
        Player.y = 43;
        break;
      case 'g':
        change_environment(E_COUNTRYSIDE);
        Player.x = 20;
        Player.y = 41;
        break;
      case 'h':
        change_environment(E_COUNTRYSIDE);
        Player.x = 22;
        Player.y = 30;
        break;
      case 'i':
        change_environment(E_COUNTRYSIDE);
        Player.x = 51;
        Player.y = 11;
        break;
      case 'j':
        change_environment(E_COUNTRYSIDE);
        Player.x = 45;
        Player.y = 45;
        break;
      case 'k':
        change_environment(E_COUNTRYSIDE);
        Player.x = 19;
        Player.y = 46;
        break;
      case 'l':
        change_environment(E_COUNTRYSIDE);
        Player.x = 32;
        Player.y = 5;
        break;
      case 'm':
        change_environment(E_COUNTRYSIDE);
        Player.x = 49;
        Player.y = 59;
        break;
      case 'n':
        change_environment(E_COUNTRYSIDE);
        Player.x = 30;
        Player.y = 58;
        break;
      default:
        if(gamestatusp(CHEATED, GameStatus))
        {
          queue_message("Enter environment number: ");
          new_env = (int)parsenum();
          change_environment(new_env);
        }
    }
    xredraw();
    if(gamestatusp(LOST, GameStatus))
    {
      queue_message("You know where you are now.");
      resetgamestatus(LOST, GameStatus);
      Precipitation = 0;
    }
  }
  setlastxy(Player.x, Player.y);
  screencheck(Player.x, Player.y);
  drawvision(Player.x, Player.y);
  if(Current_Environment == E_COUNTRYSIDE)
  {
    terrain_check(false);
  }
}

void hero(int blessing)
{
  if(blessing > -1)
  {
    queue_message("You feel super!");
    Player.status[HERO] += random_range(5) + 1 + blessing;
    calc_melee();
  }
  else
  {
    Player.status[HERO] = 0;
    calc_melee();
    queue_message("You feel cowardly.");
    level_drain(abs(blessing), "a potion of cowardice");
  }
}

void levitate(int blessing)
{
  if(blessing > -1)
  {
    if(gamestatusp(MOUNTED, GameStatus))
    {
      queue_message("You have a strange feeling of lightness in your saddle.");
    }
    else
    {
      queue_message("You start to float a few inches above the floor.");
      queue_message("You discover you can easily control your altitude...");
      queue_message("(Note use of '@' command may be useful while levitating)");
      Player.status[LEVITATING] += random_range(5) + 1 + blessing;
    }
  }
  else
  {
    queue_message("Nothing much happens.");
  }
}

/* has effect of switching between 1st level and deepest level attained */
void level_return()
{
  if(Current_Environment == Current_Dungeon)
  {
    queue_message("The vortex of mana carries you off!");
    if(Level->depth > 1)
    {
      change_level(Level->depth, 1, false);
    }
    else
    {
      change_level(Level->depth, deepest[Current_Environment], false);
    }
  }
  else if(Current_Environment == E_COUNTRYSIDE)
  {
    queue_message("A mysterious force wafts you back home!");
    Player.x = 27;
    Player.y = 19;
    screencheck(Player.x, Player.y);
    drawvision(Player.x, Player.y);
    locprint("Back Outside Rampart.");
  }
  else
  {
    queue_message("A feeble vortex of magic swirls by and has no further effect.");
  }
}

void cure(int blessing)
{
  int happened = false;
  if(blessing > -1)
  {
    if(Player.status[DISEASED])
    {
      Player.status[DISEASED] = 0;
      queue_message("You feel hygienic!");
      happened = true;
    }
    if(Player.status[POISONED])
    {
      Player.status[POISONED] -= 5 + blessing * 10;
      if(Player.status[POISONED] > 0)
      {
        queue_message("The effect of the poison has been reduced.");
      }
      else
      {
        Player.status[POISONED] = 0;
        queue_message("The poison has been purged from your system.");
      }
      happened = true;
    }
    if(Player.status[BLINDED])
    {
      Player.status[BLINDED] = 0;
      happened               = true;
      queue_message("Cobwebs clear from before your eyes.");
    }
    if(!happened)
    {
      queue_message("Nothing much happens.");
    }
  }
  else
  {
    disease(12);
  }
  showflags();
}

void disease(int amount)
{
  queue_message("You feel ill.");
  if(!Player.immunity[INFECTION])
  {
    queue_message("You begin to shiver with ague.");
    Player.status[DISEASED] += random_range(amount * 2) + 1;
  }
  else
  {
    queue_message("The illness fades.");
  }
}

void truesight(int blessing)
{
  if(blessing > -1)
  {
    Player.status[TRUESIGHT] += random_range(10) + 1;
    queue_message("You feel sharp.");
  }
  else
  {
    Player.status[BLINDED] += random_range(10) + 1;
    queue_message("You've been blinded!");
  }
}

void dispel(int blessing)
{
  int i, x = Player.x, y = Player.y;
  pob o;
  if(blessing > -1)
  {
    setspot(&x, &y);
    if((x == Player.x) && (y == Player.y))
    {
      for(i = 0; i < MAXITEMS; i++)
      {
        o = Player.possessions[i];
        if(o)
        {
          if((o->used) && (o->blessing < 0))
          {
            if(blessing + 1 + o->blessing >= 0)
            {
              setgamestatus(SUPPRESS_PRINTING, GameStatus);
              item_unequip(o);
              resetgamestatus(SUPPRESS_PRINTING, GameStatus);
              queue_message("You hear a sighing sound from");
              queue_message(itemid(o));
              o->blessing = 0;
              setgamestatus(SUPPRESS_PRINTING, GameStatus);
              item_equip(o);
              resetgamestatus(SUPPRESS_PRINTING, GameStatus);
            }
            else
            {
              queue_message("You hear dark laughter from");
              queue_message(itemid(o));
            }
          }
        }
      }
    }
    else if(Level->site[x][y].creature)
    {
      if(Level->site[x][y].creature->level < blessing * 3)
      {
        Level->site[x][y].creature->specialf = M_NO_OP;
        if(Level->site[x][y].creature->meleef != M_NO_OP)
        {
          Level->site[x][y].creature->meleef = M_MELEE_NORMAL;
        }
        Level->site[x][y].creature->strikef  = M_NO_OP;
        Level->site[x][y].creature->immunity = 0;
        m_status_reset(*Level->site[x][y].creature, M_INVISIBLE);
        m_status_reset(*Level->site[x][y].creature, INTANGIBLE);
      }
      else
      {
        queue_message("The monster ignores the effect!");
      }
    }
    else if((Level->site[x][y].p_locf == L_TRAP_FIRE) || (Level->site[x][y].p_locf == L_STATUE_WAKE) ||
            (Level->site[x][y].p_locf == L_TRAP_TELEPORT) ||
            (Level->site[x][y].p_locf == L_TRAP_DISINTEGRATE))
    {
      Level->site[x][y].p_locf = L_NO_OP;
      if(Level->site[x][y].locchar == TRAP)
      {
        Level->site[x][y].locchar = FLOOR;
      }
      lset(x, y, CHANGED, *Level);
    }
    else if(Level->site[x][y].p_locf == L_MAGIC_POOL)
    {
      Level->site[x][y].p_locf = L_WATER;
    }
    else
    {
      queue_message("Nothing much seems to happen.");
    }
  }
  else
  {
    queue_message("A smell of ozone and positive ions fills the air..");
    if(Player.status[ACCURACY] && (Player.status[ACCURACY] < 1000))
    {
      Player.status[ACCURACY] = 1;
    }
    if(Player.status[DISPLACED] && (Player.status[DISPLACED] < 1000))
    {
      Player.status[DISPLACED] = 1;
    }
    if(Player.status[HASTED] && (Player.status[HASTED] < 1000))
    {
      Player.status[HASTED] = 1;
    }
    if(Player.status[BREATHING] && (Player.status[BREATHING] < 1000))
    {
      Player.status[BREATHING] = 1;
    }
    if(Player.status[INVISIBLE] && (Player.status[INVISIBLE] < 1000))
    {
      Player.status[INVISIBLE] = 1;
    }
    if(Player.status[REGENERATING] && (Player.status[REGENERATING] < 1000))
    {
      Player.status[REGENERATING] = 1;
    }
    if(Player.status[ALERT] && (Player.status[ALERT] < 1000))
    {
      Player.status[ALERT] = 1;
    }
    if(Player.status[HERO] && (Player.status[HERO] < 1000))
    {
      Player.status[HERO] = 1;
    }
    if(Player.status[LEVITATING] && (Player.status[LEVITATING] < 1000))
    {
      Player.status[LEVITATING] = 1;
    }
    if(Player.status[ACCURATE] && (Player.status[ACCURATE] < 1000))
    {
      Player.status[ACCURATE] = 1;
    }
    if(Player.status[TRUESIGHT] && (Player.status[TRUESIGHT] < 1000))
    {
      Player.status[TRUESIGHT] = 1;
    }
    tenminute_status_check();
  }
}

void polymorph(int blessing)
{
  int             x = Player.x, y = Player.y, newmonster;
  struct monster *m;
  setspot(&x, &y);
  if((x == Player.x) && (y == Player.y))
  {
    /* WDT HACK: shouldn't this use one of the 'getarticle' functions
     * to prevent things like "a elder grue" (should be "an elder grue")?
     */
    queue_message("You enjoy your new life as a");
    queue_message(Monsters[random_range(NUMMONSTERS)].monstring);
    queue_message("But your game is over....");
    p_death("polymorphing oneself");
  }
  else if(!(m = Level->site[x][y].creature))
  {
    queue_message("Nothing happens.");
  }
  else
  {
    if(m_immunityp(*m, OTHER_MAGIC) || (m->level > random_range(12)))
    {
      queue_message(std::format("The {} resists the change!", m->monstring));
      m_status_set(*m, HOSTILE);
    }
    else
    {
      if(blessing < 0)
      {
        do
        {
          newmonster = random_range(NUMMONSTERS);
        } while((newmonster == NPC) || (newmonster == MAST_THIEF) ||
                (Monsters[newmonster].level <= m->level) || (Monsters[newmonster].uniqueness != COMMON));
      }
      else
      {
        do
        {
          newmonster = random_range(NUMMONSTERS);
        } while((newmonster == NPC) || (newmonster == MAST_THIEF) ||
                (Monsters[newmonster].uniqueness != COMMON));
      }
      /* WDT HACK: most of this could (and should) be implemented by
       * the following line: "*m = Monsters[newmonster];".  The exception,
       * of course, are the parts where the new monster inherits the old
       * one's abilities.  This would be better because it would be robust
       * even in the face of additions to the monster structure. */
      m->id           = Monsters[newmonster].id;
      m->hp           = std::max(m->hp, Monsters[newmonster].id);
      m->speed        = Monsters[newmonster].speed;
      m->hit          = Monsters[newmonster].hit;
      m->ac           = Monsters[newmonster].ac;
      m->dmg          = Monsters[newmonster].dmg;
      m->sense        = Monsters[newmonster].sense;
      m->wakeup       = Monsters[newmonster].wakeup;
      m->level        = std::max(m->level, Monsters[newmonster].level);
      m->status       = Monsters[newmonster].status;
      m->immunity     = (m->immunity | Monsters[newmonster].immunity);
      m->xpv          = std::max(m->xpv, static_cast<long>(Monsters[newmonster].wakeup));
      m->transformid  = Monsters[newmonster].transformid;
      m->corpsevalue  = Monsters[newmonster].corpsevalue;
      m->corpseweight = Monsters[newmonster].corpseweight;
      m->monchar      = Monsters[newmonster].monchar;
      m->meleestr     = Monsters[newmonster].meleestr;
      m->monstring    = Monsters[newmonster].monstring;
      m->corpsestr    = Monsters[newmonster].corpsestr;
      m->talkf        = Monsters[newmonster].talkf;
      m->movef        = Monsters[newmonster].movef;
      m->meleef       = Monsters[newmonster].meleef;
      m->strikef      = Monsters[newmonster].strikef;
      m->specialf     = Monsters[newmonster].specialf;
      m_status_set(*m, HOSTILE);
    }
  }
}

void hellfire(int x, int y, int blessing)
{
  struct monster *m;
  if((x == Player.x) && (y == Player.y))
  {
    queue_message("You have been completely annihilated. Congratulations.");
    p_death("hellfire");
  }
  else if(!(m = Level->site[x][y].creature))
  {
    queue_message("The gods are angry over your waste of power...");
    level_drain(5, "indiscriminate use of hellfire");
  }
  else
  {
    queue_message("The monster writhes in the flames...");
    if(blessing < 0)
    {
      queue_message("...and appears stronger.");
      queue_message("Much stronger.");
      m->hp += 1000;
      m->hit += 20;
      m->dmg += 100;
      m_status_set(*m, HOSTILE);
    }
    else
    {
      if(m->uniqueness == COMMON)
      {
        queue_message("and is utterly annihilated. Only a greasy spot remains...");
        m->corpsestr = "a greasy spot";
        m->id        = 0;
        free_objlist(m->possessions);
        m->possessions = nullptr;
      }
      else
      {
        queue_message("and dies, cursing your name and the uncaring gods....");
      }
      m_death(m);
    }
  }
}

void drain(int blessing)
{
  int             x = Player.x, y = Player.y;
  struct monster *m;
  setspot(&x, &y);
  queue_message("You begin to drain energy...");
  if((x == Player.x) && (y == Player.y))
  {
    queue_message("You drain your own energy....");
    queue_message("Uh, oh, positive feedback....");
    level_drain(Player.level, "self-vampirism");
  }
  else if((m = Level->site[x][y].creature))
  {
    if((blessing > -1) && (!m_immunityp(*m, NEGENERGY)))
    {
      queue_message("The monster seems weaker...");
      m_damage(m, m->level * m->level, NEGENERGY);
      m->hit   = std::max(m->hit - m->level, 1);
      m->dmg   = std::max(m->dmg - m->level * m->level, 1);
      m->ac    = std::max(m->ac - m->level, 1);
      m->level = std::max(1, m->level - 1);
      queue_message("You feel stronger...");
      gain_experience(m->level * 5);
      Player.hp += (m->level * m->level / 2);
    }
    else
    {
      queue_message("The effect reverses itself!");
      queue_message("The monster seems stronger...");
      m->hp += Player.level * Player.level;
      m->hit += Player.level;
      m->dmg += Player.level * Player.level;
      m->ac += Player.level;
      m->level++;
      queue_message("You feel weaker...");
      Player.mana = std::min(0, Player.level * Player.level);
      level_drain(m->level, "negative energy conflict");
    }
  }
  else if(blessing < 0)
  {
    queue_message("You seem to lose energy, instead of gaining it!");
    level_drain(3, "reversed energy drain");
  }
  else if(Level->site[x][y].locchar == ALTAR)
  {
    queue_message("The altar collapses in on itself....");
    Level->site[x][y].locchar = ABYSS;
    Level->site[x][y].p_locf  = L_ABYSS;
    lset(x, y, CHANGED, *Level);
    if(!Player.patron)
    {
      queue_message("You drain some theurgic energy from the altar....");
      gain_experience(40);
      Player.hp += 20;
      Player.pow += 2;
    }
    if(Level->site[x][y].aux == Player.patron)
    {
      queue_message("Your deity is enraged.");
      queue_message("You are struck by godsfire.");
      p_damage(Player.hp - 1, UNSTOPPABLE, "godsfire");
      queue_message("You feel atheistic.");
      Player.patron           = -1;
      Player.rank[PRIESTHOOD] = 0;
    }
    else
    {
      queue_message("You feel the wrath of a god....");
      p_damage(random_range(Player.level * 10), UNSTOPPABLE, "divine wrath");
      if(Player.patron != 0)
      {
        queue_message("Your deity doesn't seem to mind your action, though.");
        gain_experience(100);
      }
    }
  }
  else
  {
    queue_message("You drain some energy from the ambient megaflow.");
    Player.hp++;
  }
}

void sanctuary()
{
  if(Level->environment == E_TEMPLE)
  {
    queue_message("Odd, the spell has no effect. I wonder why.");
  }
  else
  {
    queue_message("You're standing on sacred ground!");
    Player.sx = Player.x;
    Player.sy = Player.y;
  }
}

void shadowform()
{
  /* WDT HACK: this fix might work, but it seems like the immunity
   * will be FAR too short.  It's obviously better than the old
   * situation, though... */
  if(!Player.status[SHADOWFORM])
  {
    queue_message("You feel like a shadow.");
    Player.immunity[NORMAL_DAMAGE]++;
    Player.immunity[ACID]++;
    Player.immunity[THEFT]++;
    Player.immunity[INFECTION]++;
    Player.status[SHADOWFORM] += Player.level;
  }
  else
  {
    queue_message("You feel even more shadowy.");
    Player.status[SHADOWFORM] += Player.level;
  }
}

void illuminate(int blessing)
{
  int r = Level->site[Player.x][Player.y].roomnumber;
  if(blessing > -1)
  {
    if(r > ROOMBASE)
    {
      if(loc_statusp(Player.x, Player.y, LIT, *Level))
      {
        queue_message("A glow surrounds you.");
      }
      else
      {
        queue_message("The room lights up!");
        Player.status[ILLUMINATION] += blessing + 3;
        spreadroomlight(Player.x, Player.y, Level->site[Player.x][Player.y].roomnumber);
      }
    }
    else
    {
      queue_message("You see a faint glimmer of light which quickly fades.");
    }
  }
  else
  {
    if(r > ROOMBASE)
    {
      if(!loc_statusp(Player.x, Player.y, LIT, *Level))
      {
        queue_message("Nothing much happens.");
      }
      else
      {
        queue_message("The room darkens!");
        spreadroomdark(Player.x, Player.y, Level->site[Player.x][Player.y].roomnumber);
      }
    }
    else
    {
      queue_message("The gloom thickens for a moment.");
    }
  }
}

void drain_life(int amount)
{
  amount = abs(amount);
  queue_message("You feel cold!");
  if(p_immune(NEGENERGY))
  {
    queue_message("... but the feeling quickly fades.");
  }
  else
  {
    if(random_range(2))
    {
      queue_message("The coldness spreads throughout your body...");
      Player.str -= amount;
      Player.con -= amount;
      if((Player.str < 3) || (Player.con < 3))
      {
        queue_message("You suffer a fatal heart attack!!!");
        Player.hp = 0;
        p_death("a coronary");
      }
    }
    else
    {
      queue_message("The coldness saps your very soul...");
      level_drain(amount, "soul destruction");
    }
  }
}

void inflict_fear(int x, int y)
{
  struct monster *m;
  if((Player.x == x) && (Player.y == y))
  {
    queue_message("You shudder with otherworldly dread.");
    if(Player.immunity[FEAR] > 0)
    {
      queue_message("You brace up and face your fear like a hero!");
    }
    else
    {
      queue_message("You panic!");
      Player.status[AFRAID] += 10;
    }
  }
  else if((m = Level->site[x][y].creature))
  {
    std::string monster_name;
    if(m->uniqueness == COMMON)
    {
      monster_name = std::format("The {}", m->monstring);
    }
    else
    {
      monster_name = m->monstring;
    }
    m->speed = std::max(2, m->speed - 1);
    if(m_immunityp(*m, FEAR))
    {
      queue_message(std::format("{} seems enraged!", monster_name));
    }
    else
    {
      queue_message(std::format("{} is terrorized!", monster_name));
      m_dropstuff(m);
      if(m_statusp(*m, MOBILE))
      {
        m->movef = M_MOVE_SCAREDY;
      }
    }
  }
  else
  {
    queue_message("A thrill of fear tickles your spine ... and passes.");
  }
}

/*Turns on deflection status for the player */
void deflection(int blessing)
{
  if(blessing > -1)
  {
    queue_message("You feel buffered.");
    Player.status[DEFLECTION] = blessing + random_range(6);
  }
  else
  {
    queue_message("You feel vulnerable");
    Player.status[VULNERABLE] += random_range(6) - blessing;
  }
}
