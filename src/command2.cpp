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

// command2.cpp

// This file contains toplevel commands called from command1.cpp

#include "glob.h"
#include "scr.h"
#include "spell.h"

#include <algorithm>
#include <array>
#include <chrono>
#include <filesystem>
#include <format>
#include <iostream>
#include <list>
#include <string>
#include <thread>
#include <utility>

extern std::string get_username();

// no op a turn....
void rest()
{
  if(random_range(20) == 1)
  {
    switch(random_range(10))
    {
      case 0:
        queue_message(" Time passes slowly.... ");
        break;
      case 1:
        queue_message(" Tick. Tock. Tick. Tock. ");
        break;
      case 2:
        queue_message(" Ho Hum. ");
        break;
      case 3:
        queue_message(" Beauty Sleep. Well, in your case, Ugly Sleep. ");
        break;
      case 4:
        queue_message(" And with Strange Aeons, even Death may die. ");
        break;
      case 5:
        queue_message(" La Di Da. ");
        break;
      case 6:
        queue_message(" Time keeps on tickin' tickin' -- into the future.... ");
        break;
      case 7:
        queue_message(" Boooring! ");
        break;
      case 8:
        queue_message(" You think I like watching you sleep? ");
        break;
      case 9:
        queue_message(" You sure have an early bedtime! ");
        break;
    }
  }
}

// read a scroll, book, tome, etc...
void peruse()
{
  if(Player.status[BLINDED] > 0)
  {
    queue_message("You're blind -- you can't read!!!");
  }
  else if(Player.status[AFRAID] > 0)
  {
    queue_message("You are too afraid to stop to read a scroll!");
  }
  else
  {
    queue_message("Read -- ");
    int slot = getitem(SCROLL);
    if(slot == ABORT)
    {
      setgamestatus(SKIP_MONSTERS, GameStatus);
    }
    else
    {
      std::unique_ptr<object> &o = Player.possessions[slot];
      if(o->objchar != SCROLL)
      {
        queue_message("There's nothing written on ");
        queue_message(itemid(o.get()));
      }
      else
      {
        queue_message("You carefully unfurl the scroll....");
        item_use(o);
        dispose_lost_objects(1, slot);
      }
    }
  }
}

void quaff()
{
  queue_message("Quaff --");
  int slot = getitem(POTION);
  if(slot == ABORT)
  {
    setgamestatus(SKIP_MONSTERS, GameStatus);
  }
  else
  {
    std::unique_ptr<object> &o = Player.possessions[slot];
    if(o->objchar != POTION)
    {
      queue_message("You can't drink ");
      queue_message(itemid(o.get()));
    }
    else
    {
      queue_message("You drink it down.... ");
      item_use(o);
      dispose_lost_objects(1, slot);
    }
  }
}

void activate()
{
  queue_message("Activate -- item [i] or artifact [a] or quit [ESCAPE]?");
  char response;
  do
  {
    response = (char)mcigetc();
  } while((response != 'i') && (response != 'a') && (response != ESCAPE));
  if(response != ESCAPE)
  {
    int slot;
    if(response == 'i')
    {
      slot = getitem(THING);
    }
    else
    {
      slot = getitem(ARTIFACT);
    }
    if(slot != ABORT)
    {
      queue_message("You activate it.... ");
      item_use(Player.possessions[slot]);
    }
    else
    {
      setgamestatus(SKIP_MONSTERS, GameStatus);
    }
  }
  else
  {
    setgamestatus(SKIP_MONSTERS, GameStatus);
  }
}

void eat()
{
  queue_message("Eat --");
  int slot = getitem(FOOD);
  if(slot == ABORT)
  {
    setgamestatus(SKIP_MONSTERS, GameStatus);
  }
  else
  {
    std::unique_ptr<object> &o = Player.possessions[slot];
    if(o->objchar != FOOD && o->objchar != CORPSE)
    {
      queue_message("You can't eat " + itemid(o.get()));
    }
    else
    {
      if(o->on_use == I_FOOD)
      {
        Player.food = std::max(0, Player.food + o->aux);
      }
      item_use(o);
      dispose_lost_objects(1, slot);
      calc_weight();
      if(Current_Dungeon == E_COUNTRYSIDE)
      {
        Time += 100;
        hourly_check();
      }
    }
  }
  foodcheck();
}

// search all adjacent spots for secrecy
void search(int *searchval)
{
  if(Player.status[AFRAID] > 0)
  {
    queue_message("You are too terror-stricken to stop to search for anything.");
  }
  else
  {
    if(!gamestatusp(FAST_MOVE, GameStatus))
    {
      setgamestatus(FAST_MOVE, GameStatus);
      *searchval = Searchnum;
    }
    for(int i = 0; i < 9; ++i)
    {
      searchat(Player.x + Dirs[0][i], Player.y + Dirs[1][i]);
    }
    drawvision(Player.x, Player.y);
  }
}

// pick up a thing where the player is
void pickup()
{
  if(Level->site[Player.x][Player.y].things.empty())
  {
    queue_message("There's nothing there!");
  }
  else if(Player.status[SHADOWFORM])
  {
    queue_message("You can't really interact with the real world in your shadowy state.");
  }
  else
  {
    pickup_at(Player.x, Player.y);
  }
}

void drop_pack_item()
{
  if(Player.pack.empty() && Player.cash <= 0)
  {
    queue_message("You have nothing to drop.");
    return;
  }
  std::list<std::pair<object *, char>> food;
  std::list<std::pair<object *, char>> weapons;
  std::list<std::pair<object *, char>> scrolls;
  std::list<std::pair<object *, char>> potions;
  std::list<std::pair<object *, char>> armor;
  std::list<std::pair<object *, char>> sticks;
  std::list<std::pair<object *, char>> jewelery;
  std::list<std::pair<object *, char>> artifacts;
  std::list<std::pair<object *, char>> other;
  const std::array object_categories{
    std::pair{"|YFood|w",          &food     },
    std::pair{"|YWeapons|w",       &weapons  },
    std::pair{"|YScrolls|w",       &scrolls  },
    std::pair{"|YPotions|w",       &potions  },
    std::pair{"|YArmor|w",         &armor    },
    std::pair{"|YSticks|w",        &sticks   },
    std::pair{"|YJewelery|w",      &jewelery },
    std::pair{"|YArtifacts|w",     &artifacts},
    std::pair{"|YMiscellaneous|w", &other    }
  };
  for(size_t i = 0; i < Player.pack.size(); ++i)
  {
    char pack_letter = 'a' + static_cast<char>(Player.pack.size() - 1 - i);
    object *pack_item = Player.pack[i].get();
    switch(pack_item->objchar)
    {
      case FOOD:
        food.emplace_back(std::make_pair(pack_item, pack_letter));
        break;
      case WEAPON:
      case MISSILEWEAPON:
        weapons.emplace_back(std::make_pair(pack_item, pack_letter));
        break;
      case SCROLL:
        scrolls.emplace_back(std::make_pair(pack_item, pack_letter));
        break;
      case POTION:
        potions.emplace_back(std::make_pair(pack_item, pack_letter));
        break;
      case ARMOR:
      case SHIELD:
      case CLOAK:
      case BOOTS:
        armor.emplace_back(std::make_pair(pack_item, pack_letter));
        break;
      case STICK:
        sticks.emplace_back(std::make_pair(pack_item, pack_letter));
        break;
      case RING:
        jewelery.emplace_back(std::make_pair(pack_item, pack_letter));
        break;
      case ARTIFACT:
        artifacts.emplace_back(std::make_pair(pack_item, pack_letter));
        break;
      case THING:
      default:
        other.emplace_back(std::make_pair(pack_item, pack_letter));
        break;
    }
  }
  std::vector<std::string> lines;
  if(Player.cash > 0)
  {
    lines.emplace_back("|YCash|w");
    lines.emplace_back(std::format("   $ - {} gold pieces", Player.cash));
  }
  for(auto &object_category : object_categories)
  {
    if(!object_category.second->empty())
    {
      lines.emplace_back(object_category.first);
      for(auto item : *object_category.second)
      {
        lines.emplace_back(std::format("   {} - {}", item.second, itemid(item.first)));
      }
    }
  }
  menu->load(lines, {"|WDrop Item:|w", ""});

  int player_input;
  size_t pack_index;
  do
  {
    player_input = menu->get_player_input();
    if(player_input == ESCAPE)
    {
      return;
    }
    else if(player_input == '$' && Player.cash > 0)
    {
      drop_money();
      calc_melee();
      return;
    }
    pack_index = Player.pack.size() - 1 - (player_input - 'a');
  } while(player_input < 'a' || player_input > 'z' || pack_index > Player.pack.size() - 1);
  object *item = Player.pack[pack_index].get();
  p_drop_at(Player.x, Player.y, item->number, item);
  Player.pack.erase(Player.pack.begin() + pack_index);
  calc_melee();
}

void drop_equipped_item()
{
  queue_message("Drop --");
  int slot = getitem(CASH);
  if(slot == ABORT)
  {
    setgamestatus(SKIP_MONSTERS, GameStatus);
  }
  else
  {
    if(slot == CASHVALUE)
    {
      drop_money();
    }
    else if(!Player.possessions[slot]->used || !cursed(Player.possessions[slot].get()))
    {
      if(Player.possessions[slot]->number == 1)
      {
        p_drop_at(Player.x, Player.y, 1, Player.possessions[slot].get());
        dispose_lost_objects(1, slot);
      }
      else
      {
        int n = getnumber(Player.possessions[slot]->number);
        p_drop_at(Player.x, Player.y, n, Player.possessions[slot].get());
        dispose_lost_objects(n, slot);
      }
    }
    else
    {
      queue_message("You can't seem to get rid of: ");
      queue_message(itemid(Player.possessions[slot].get()));
    }
  }
  calc_melee();
}

// talk to the animals -- learn their languages....
void talk()
{
  int dx, dy, index = 0;
  char response;
  monster *m;

  queue_message("Talk --");
  index = getdir();

  if(index == ABORT)
  {
    setgamestatus(SKIP_MONSTERS, GameStatus);
  }
  else
  {
    dx = Dirs[0][index];
    dy = Dirs[1][index];

    if(!inbounds(Player.x + dx, Player.y + dy) || !Level->site[Player.x + dx][Player.y + dy].creature)
    {
      queue_message("There's nothing there to talk to!!!");
      setgamestatus(SKIP_MONSTERS, GameStatus);
    }
    else
    {
      m = Level->site[Player.x + dx][Player.y + dy].creature;
      std::vector<std::string> lines;
      lines.emplace_back(std::format("     Talk to {}:", m->monstring));
      lines.emplace_back("a: Greet.");
      lines.emplace_back("b: Threaten.");
      lines.emplace_back("c: Surrender.");
      lines.emplace_back("ESCAPE: Clam up.");
      menu->load(lines);
      do
      {
        response = menu->get_player_input();
      } while((response != 'a') && (response != 'b') && (response != 'c') && (response != ESCAPE));
      switch(response)
      {
        case 'a':
          monster_talk(m);
          break;
        case 'b':
          threaten(m);
          break;
        case 'c':
          surrender(m);
          break;
        default:
          setgamestatus(SKIP_MONSTERS, GameStatus);
          break;
      }
    }
  }
  xredraw();
}

// try to deactivate a trap
void disarm()
{
  int x, y, index = 0;

  queue_message("Disarm -- ");

  index = getdir();

  if(index == ABORT)
  {
    setgamestatus(SKIP_MONSTERS, GameStatus);
  }
  else
  {
    x = Dirs[0][index] + Player.x;
    y = Dirs[1][index] + Player.y;

    if(!inbounds(x, y))
    {
      queue_message("Whoa, off the map...");
    }
    else if(Level->site[x][y].locchar != TRAP)
    {
      queue_message("You can't see a trap there!");
    }
    else
    {
      if(random_range(50 + difficulty() * 5) < Player.dex * 2 + Player.level * 3 + Player.rank[THIEVES] * 10)
      {
        queue_message("You disarmed the trap!");
        if(random_range(100) < Player.dex + Player.rank[THIEVES] * 10)
        {
          auto o = std::make_unique<object>();
          switch(Level->site[x][y].p_locf)
          {
            case L_TRAP_DART:
              *o = Objects[THINGID + 17];
              break;
            case L_TRAP_DISINTEGRATE:
              *o = Objects[THINGID + 23];
              break;
            case L_TRAP_SLEEP_GAS:
              *o = Objects[THINGID + 22];
              break;
            case L_TRAP_TELEPORT:
              *o = Objects[THINGID + 21];
              break;
            case L_TRAP_ABYSS:
              *o = Objects[THINGID + 24];
              break;
            case L_TRAP_FIRE:
              *o = Objects[THINGID + 20];
              break;
            case L_TRAP_SNARE:
              *o = Objects[THINGID + 19];
              break;
            case L_TRAP_ACID:
              *o = Objects[THINGID + 18];
              break;
            case L_TRAP_MANADRAIN:
              *o = Objects[THINGID + 25];
              break;
            default:
              o.reset();
              break;
          }
          if(o)
          {
            queue_message("You manage to retrieve the trap components!");
            Objects[o->id].known = 1;
            o->known             = 1;
            gain_item(std::move(o));
            gain_experience(25);
          }
        }
        Level->site[x][y].p_locf  = L_NO_OP;
        Level->site[x][y].locchar = FLOOR;
        lset(x, y, CHANGED, *Level);
        gain_experience(5);
      }
      else if(random_range(10 + difficulty() * 2) > Player.dex)
      {
        queue_message("You accidentally set off the trap!");
        Player.x = x;
        Player.y = y;
        p_movefunction(Level->site[x][y].p_locf);
      }
      else
      {
        queue_message("You failed to disarm the trap.");
      }
    }
  }
}

// is it more blessed to give, or receive?
void give()
{
  int dx, dy, dindex = 0;
  queue_message("Give to monster --");
  dindex = getdir();
  if(dindex == ABORT)
  {
    setgamestatus(SKIP_MONSTERS, GameStatus);
  }
  else
  {
    dx = Dirs[0][dindex];
    dy = Dirs[1][dindex];
    if(!inbounds(Player.x + dx, Player.y + dy))
    {
      queue_message("Whoa, off the map...");
    }
    else if(!Level->site[Player.x + dx][Player.y + dy].creature)
    {
      queue_message("There's nothing there to give something to!!!");
      setgamestatus(SKIP_MONSTERS, GameStatus);
    }
    else
    {
      monster *m = Level->site[Player.x + dx][Player.y + dy].creature;
      queue_message("Give what? ");
      int slot = getitem(CASH);
      if(slot == ABORT)
      {
        setgamestatus(SKIP_MONSTERS, GameStatus);
      }
      else if(slot == CASHVALUE)
      {
        give_money(m);
      }
      else if(!cursed(Player.possessions[slot].get()))
      {
        auto obj = std::make_unique<object>(*Player.possessions[slot]);
        obj->used = false;
        obj->number = 1;
        dispose_lost_objects(1, slot);
        queue_message(std::format("Given: {}.", itemid(obj.get())));
        givemonster(m, std::move(obj));
        calc_melee();
      }
      else
      {
        queue_message("You can't even give away: ");
        queue_message(itemid(Player.possessions[slot].get()));
      }
    }
  }
}

// zap a wand, of course
void zapwand()
{
  if(Player.status[AFRAID] > 0)
  {
    queue_message("You are so terror-stricken you can't hold a wand straight!");
  }
  else
  {
    queue_message("Zap --");
    int index = getitem(STICK);
    if(index == ABORT)
    {
      setgamestatus(SKIP_MONSTERS, GameStatus);
    }
    else
    {
      std::unique_ptr<object> &obj = Player.possessions[index];
      if(obj->objchar != STICK)
      {
        queue_message("You can't zap: ");
        queue_message(itemid(obj.get()));
      }
      else if(obj->charge < 1)
      {
        queue_message("Fizz.... Pflpt. Out of charges. ");
      }
      else
      {
        obj->charge--;
        item_use(obj);
      }
    }
  }
}

// cast a spell
void magic()
{
  int drain;
  if(Player.status[AFRAID] > 0)
  {
    queue_message("You are too afraid to concentrate on a spell!");
  }
  else
  {
    std::optional<spell::spell_id> id = getspell();
    xredraw();
    if(!id.has_value())
    {
      setgamestatus(SKIP_MONSTERS, GameStatus);
    }
    else
    {
      drain = spell::Spells[id.value()].powerdrain;
      if(Lunarity == 1)
      {
        drain /= 2;
      }
      else if(Lunarity == -1)
      {
        drain *= 2;
      }
      if(drain > Player.mana)
      {
        if(Lunarity == -1 && Player.mana >= drain / 2)
        {
          queue_message("The contrary moon has made that spell too draining! ");
        }
        else
        {
          queue_message("You lack the power for that spell! ");
        }
      }
      else
      {
        Player.mana -= drain;
        cast_spell(id.value());
      }
    }
  }
  dataprint();
}

// go upstairs ('<' command)
void upstairs()
{
  if(Level->site[Player.x][Player.y].locchar != STAIRS_UP)
  {
    queue_message("Not here!");
  }
  else if(Level->site[Player.x][Player.y].p_locf == L_ESCALATOR)
  {
    p_movefunction(Level->site[Player.x][Player.y].p_locf);
  }
  else
  {
    if(gamestatusp(MOUNTED, GameStatus))
    {
      queue_message("You manage to get your horse upstairs.");
    }
    queue_message("You ascend a level.");
    if(Level->depth <= 1)
    {
      if(Level->environment == E_SEWERS)
      {
        change_environment(E_CITY);
      }
      else
      {
        change_environment(E_COUNTRYSIDE);
      }
    }
    else
    {
      change_level(Level->depth, Level->depth - 1, false);
    }
    roomcheck();
  }
  setgamestatus(SKIP_MONSTERS, GameStatus);
}

// go downstairs ('>' command)
void downstairs()
{
  if(Level->site[Player.x][Player.y].locchar != STAIRS_DOWN)
  {
    queue_message("Not here!");
  }
  else if(Level->site[Player.x][Player.y].p_locf == L_ENTER_CIRCLE || Level->site[Player.x][Player.y].p_locf == L_ENTER_COURT)
  {
    p_movefunction(Level->site[Player.x][Player.y].p_locf);
  }
  else
  {
    if(gamestatusp(MOUNTED, GameStatus))
    {
      queue_message("You manage to get your horse downstairs.");
    }
    if(Current_Environment == Current_Dungeon)
    {
      queue_message("You descend a level.");
      change_level(Level->depth, Level->depth + 1, false);
      roomcheck();
    }
    else if((Current_Environment == E_CITY) || (Last_Environment == E_CITY))
    {
      change_environment(E_SEWERS);
    }
    else if(Current_Environment != Current_Dungeon)
    {
      queue_message("This stairway is deviant. You can't use it.");
    }
  }
  setgamestatus(SKIP_MONSTERS, GameStatus);
}

// set various player options
// have to redefine in odefs for next full recompile
void setoptions()
{
  int slot = 1, to;
  bool done = false;
  int response;

  menuclear();

  display_options();

  move_slot(1, 1, NUMOPTIONS);
  queue_message("Currently selected option is preceded by highlit >>");
  queue_message("Move selected option with '>' and '<', ESCAPE to quit.");
  do
  {
    response = mcigetc();
    switch(response)
    {
      case 'j':
      case '>':
      case KEY_DOWN:
        to   = slot + 1;
        slot = move_slot(slot, to, NUMOPTIONS + 1);
        break;
      case 'k':
      case '<':
      case KEY_UP:
        to = slot - 1;
        if(to > 0)
        {
          slot = move_slot(slot, to, NUMOPTIONS + 1);
        }
        break;
      case KEY_HOME:
        slot = move_slot(slot, 1, NUMOPTIONS + 1);
        break;
      case KEY_LL:
        slot = move_slot(slot, NUMOPTIONS, NUMOPTIONS + 1);
        break;
      case 't':
        if(slot <= NUMTFOPTIONS)
        {
          optionset(pow2(slot - 1), Player);
        }
        else if(slot == VERBOSITY_LEVEL)
        {
          Verbosity = TERSE;
        }
        else
        {
          queue_message("'T' is meaningless for this option.");
        }
        break;
      case 'f':
        if(slot <= NUMTFOPTIONS)
        {
          optionreset(pow2(slot - 1), Player);
        }
        else
        {
          queue_message("'F' is meaningless for this option.");
        }
        break;
      case 'm':
        if(slot == VERBOSITY_LEVEL)
        {
          Verbosity = MEDIUM;
        }
        else
        {
          queue_message("'M' is meaningless for this option.");
        }
        break;
      case 'v':
        if(slot == VERBOSITY_LEVEL)
        {
          Verbosity = VERBOSE;
        }
        else
        {
          queue_message("'V' is meaningless for this option.");
        }
        break;
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
        if(slot == SEARCH_DURATION)
        {
          Searchnum = response - '0';
        }
        else
        {
          queue_message("A number is meaningless for this option.");
        }
        break;
      case ESCAPE:
        done = true;
        break;
      default:
        queue_message("That response is meaningless for this option.");
        break;
    }
    display_option_slot(slot);
    move_slot(slot, slot, NUMOPTIONS + 1);
  } while(!done);
  if(optionp(SHOW_COLOUR, Player))
  {
    colour_on();
  }
  else
  {
    colour_off();
  }
  if(optionp(MOUSE_ENABLED, Player))
  {
    mouse_enable();
  }
  else
  {
    mouse_disable();
  }
  xredraw();
}

// name an item
void callitem()
{
  setgamestatus(SKIP_MONSTERS, GameStatus);
  queue_message("Call --");
  int index = getitem(NULL_ITEM);
  if(index == CASHVALUE)
  {
    queue_message("Can't rename cash!");
  }
  else if(index != ABORT)
  {
    object *obj = Player.possessions[index].get();
    if(obj->known)
    {
      queue_message("That item is already identified!");
    }
    else
    {
      queue_message("Call it:");
      obj->objstr = msgscanstring();
      queue_message("Also call all similar items by that name? [yn] ");
      if(ynq() == 'y')
      {
        Objects[obj->id].objstr = obj->objstr;
      }
    }
  }
}

// open a door
void opendoor()
{
  int dir;
  int ox, oy;

  queue_message("Open --");
  dir = getdir();
  if(dir == ABORT)
  {
    setgamestatus(SKIP_MONSTERS, GameStatus);
  }
  else
  {
    ox = Player.x + Dirs[0][dir];
    oy = Player.y + Dirs[1][dir];
    if(Level->site[ox][oy].locchar == OPEN_DOOR)
    {
      queue_message("That door is already open!");
      setgamestatus(SKIP_MONSTERS, GameStatus);
    }
    else if(Level->site[ox][oy].locchar == PORTCULLIS)
    {
      queue_message("You try to lift the massive steel portcullis....");
      if(random_range(100) < Player.str)
      {
        queue_message("Incredible. You bust a gut and lift the portcullis.");
        Level->site[ox][oy].locchar = FLOOR;
        lset(ox, oy, CHANGED, *Level);
      }
      else
      {
        queue_message("Argh. You ruptured yourself.");
        p_damage(Player.str, UNSTOPPABLE, "a portcullis");
      }
    }
    else if((Level->site[ox][oy].locchar != CLOSED_DOOR) || loc_statusp(ox, oy, SECRET, *Level))
    {
      queue_message("You can't open that!");
      setgamestatus(SKIP_MONSTERS, GameStatus);
    }
    else if(Level->site[ox][oy].aux == LOCKED)
    {
      queue_message("That door seems to be locked.");
    }
    else
    {
      Level->site[ox][oy].locchar = OPEN_DOOR;
      lset(ox, oy, CHANGED, *Level);
    }
  }
}

// Try to destroy some location
void bash_location()
{
  int dir;
  int ox, oy;

  queue_message("Bashing --");
  dir = getdir();
  if(dir == ABORT)
  {
    setgamestatus(SKIP_MONSTERS, GameStatus);
  }
  else
  {
    ox = Player.x + Dirs[0][dir];
    oy = Player.y + Dirs[1][dir];
    if((Current_Environment == E_CITY) && (ox == 0) && (oy == 0))
    {
      queue_message("Back Door WIZARD Mode!");
      queue_message("You will invalidate your score if you proceed.");
      queue_message("Enable WIZARD Mode? [yn] ");
      if(ynq() == 'y')
      {
        queue_message("You feel like a cheater.");
        setgamestatus(CHEATED, GameStatus);
      }
      else
      {
        queue_message("A sudden tension goes out of the air....");
      }
    }
    else
    {
      if(Level->site[ox][oy].locchar == WALL)
      {
        queue_message("You hurl yourself at the wall!");
        p_damage(Player.str, NORMAL_DAMAGE, "a suicidal urge");
      }
      else if(Level->site[ox][oy].locchar == OPEN_DOOR)
      {
        queue_message("You hurl yourself through the open door!");
        queue_message("Yaaaaah! ... thud.");
        Player.x = ox;
        Player.y = oy;
        p_damage(3, UNSTOPPABLE, "silliness");
        p_movefunction(Level->site[Player.x][Player.y].p_locf);
        setgamestatus(SKIP_MONSTERS, GameStatus); // monsters are surprised...
      }
      else if(Level->site[ox][oy].locchar == CLOSED_DOOR)
      {
        if(loc_statusp(ox, oy, SECRET, *Level))
        {
          queue_message("You found a secret door!");
          lreset(ox, oy, SECRET, *Level);
          lset(ox, oy, CHANGED, *Level);
        }
        if(Level->site[ox][oy].aux == LOCKED)
        {
          if(random_range(50 + difficulty() * 10) < Player.str)
          {
            Player.x = ox;
            Player.y = oy;
            queue_message("You blast the door off its hinges!");
            Level->site[ox][oy].locchar = FLOOR;
            lset(ox, oy, CHANGED, *Level);
            p_movefunction(Level->site[Player.x][Player.y].p_locf);
            setgamestatus(SKIP_MONSTERS, GameStatus); // monsters are surprised...
          }
          else
          {
            queue_message("Crash! The door holds.");
            if(random_range(30) > Player.str)
            {
              p_damage(std::max(1, statmod(Player.str)), UNSTOPPABLE, "a door");
            }
          }
        }
        else
        {
          Player.x = ox;
          Player.y = oy;
          queue_message("You bash open the door!");
          if(random_range(30) > Player.str)
          {
            p_damage(1, UNSTOPPABLE, "a door");
          }
          Level->site[ox][oy].locchar = OPEN_DOOR;
          lset(ox, oy, CHANGED, *Level);
          p_movefunction(Level->site[Player.x][Player.y].p_locf);
          setgamestatus(SKIP_MONSTERS, GameStatus); // monsters are surprised...
        }
      }
      else if(Level->site[ox][oy].locchar == STATUE)
      {
        statue_random(ox, oy);
      }
      else if(Level->site[ox][oy].locchar == PORTCULLIS)
      {
        queue_message("Really, you don't have a prayer.");
        if(random_range(1000) < Player.str)
        {
          queue_message("The portcullis flies backwards into a thousand fragments.");
          queue_message("Wow. What a stud.");
          gain_experience(100);
          Level->site[ox][oy].locchar = FLOOR;
          Level->site[ox][oy].p_locf  = L_NO_OP;
          lset(ox, oy, CHANGED, *Level);
        }
        else
        {
          queue_message("You only hurt yourself on the 3'' thick steel bars.");
          p_damage(Player.str, UNSTOPPABLE, "a portcullis");
        }
      }
      else if(Level->site[ox][oy].locchar == ALTAR)
      {
        if((Player.patron > 0) && (Level->site[ox][oy].aux == Player.patron))
        {
          queue_message("You have a vision! An awesome angel hovers over the altar.");
          queue_message("The angel says: 'You twit, don't bash your own altar!'");
          queue_message("The angel slaps you upside the head for your presumption.");
          p_damage(Player.hp - 1, UNSTOPPABLE, "an annoyed angel");
        }
        else if(Level->site[ox][oy].aux == 0)
        {
          queue_message("The feeble powers of the minor godling are not enough to");
          queue_message("protect his altar! The altar crumbles away to dust.");
          queue_message("You feel almost unbearably smug.");
          Level->site[ox][oy].locchar = RUBBLE;
          Level->site[ox][oy].p_locf  = L_RUBBLE;
          lset(ox, oy, CHANGED, *Level);
          gain_experience(5);
        }
        else
        {
          queue_message("You have successfully annoyed a major deity. Good job.");
          queue_message("Zzzzap! A bolt of godsfire strikes!");
          if(Player.rank[PRIESTHOOD] > 0)
          {
            queue_message("Your own deity's aegis defends you from the bolt!");
          }
          p_damage(
            std::max(0, random_range(100) - Player.rank[PRIESTHOOD] * 20), UNSTOPPABLE,
            "a bolt of godsfire"
          );
          if(Player.rank[PRIESTHOOD] * 20 + Player.pow + Player.level > random_range(200))
          {
            queue_message("The altar crumbles...");
            Level->site[ox][oy].locchar = RUBBLE;
            Level->site[ox][oy].p_locf  = L_RUBBLE;
            lset(ox, oy, CHANGED, *Level);
            if(Player.rank[PRIESTHOOD])
            {
              queue_message("You sense your deity's pleasure with you.");
              queue_message("You are surrounded by a golden glow.");
              cleanse(1);
              heal(10);
            }
            gain_experience(500);
          }
        }
      }
      else
      {
        queue_message("You restrain yourself from total silliness.");
        setgamestatus(SKIP_MONSTERS, GameStatus);
      }
    }
  }
}

// attempt destroy an item
void bash_item()
{
  queue_message("Destroy an item --");
  int slot = getitem(NULL_ITEM);
  if(slot == CASHVALUE)
  {
    queue_message("Can't destroy cash!");
  }
  else if(slot != ABORT)
  {
    std::unique_ptr<object> &obj = Player.possessions[slot];
    if(Player.str + random_range(20) > obj->fragility + random_range(20))
    {
      int item_level = obj->level;
      if(!damage_item(slot))
      {
        queue_message("Damn thing refuses to break...");
      }
      else if(Player.alignment < 0)
      {
        queue_message("That was fun....");
        gain_experience(item_level * item_level * 5);
      }
    }
    else
    {
      if(obj->objchar == WEAPON)
      {
        queue_message("The weapon turned in your hand -- you hit yourself!");
        p_damage(random_range(obj->dmg + abs(obj->plus)), NORMAL_DAMAGE, "a failure at vandalism");
      }
      else if(obj->objchar == ARTIFACT)
      {
        queue_message("Uh Oh -- Now you've gotten it angry....");
        p_damage(obj->level * 10, UNSTOPPABLE, "an enraged artifact");
      }
      else
      {
        queue_message("Ouch! Damn thing refuses to break...");
        p_damage(1, UNSTOPPABLE, "a failure at vandalism");
      }
    }
  }
}

// guess what this does
// if force is true, exiting due to some problem - don't bomb out
void save(int force)
{
  bool ok = true;

  if(gamestatusp(ARENA_MODE, GameStatus))
  {
    if(force)
    {
      resetgamestatus(ARENA_MODE, GameStatus);
      change_environment(E_CITY);
    }
    else
    {
      queue_message("Can't save the game in the arena!");
      setgamestatus(SKIP_MONSTERS, GameStatus);
      ok = false;
    }
  }
  else if(Current_Environment == E_ABYSS)
  {
    if(force)
    {
      change_environment(E_COUNTRYSIDE);
    }
    else
    {
      queue_message("Can't save the game in the Adept's Challenge!");
      setgamestatus(SKIP_MONSTERS, GameStatus);
      ok = false;
    }
  }
  else if(Current_Environment == E_TACTICAL_MAP)
  {
    if(force)
    {
      change_environment(E_COUNTRYSIDE);
    }
    else
    {
      queue_message("Can't save the game in the tactical map!");
      setgamestatus(SKIP_MONSTERS, GameStatus);
      ok = false;
    }
  }
  if(!force && ok)
  {
    queue_message("Confirm Save? [yn] ");
    ok = (ynq() == 'y');
  }
  std::string save_file_directory{std::format("{}saves/{}", Omegalib, get_username())};
  if(ok)
  {
    if(!std::filesystem::exists(save_file_directory))
    {
      if(!std::filesystem::create_directories(save_file_directory))
      {
        queue_message(std::format("Cannot create save file directory: {}", save_file_directory));
        ok = false;
      }
    }
  }
  if(force || ok)
  {
    std::string file_name = std::format("{}/{}.sav", save_file_directory, Player.name);
    if(save_game(file_name))
    {
      endgraf();
      std::cout << "Bye!" << std::endl;
      exit(0);
    }
    else
    {
      queue_message("Save Aborted.");
    }
  }
  if(force)
  {
    queue_message("The game is quitting - you will lose your character.");
    queue_message("Try to save again? ");
    if(ynq() == 'y')
    {
      save(force);
    }
  }
  setgamestatus(SKIP_MONSTERS, GameStatus); // if we get here, we failed to save
}

// close a door
void closedoor()
{
  int dir;
  int ox, oy;

  queue_message("Close --");
  dir = getdir();
  if(dir == ABORT)
  {
    setgamestatus(SKIP_MONSTERS, GameStatus);
  }
  else
  {
    ox = Player.x + Dirs[0][dir];
    oy = Player.y + Dirs[1][dir];
    if(Level->site[ox][oy].locchar == CLOSED_DOOR)
    {
      queue_message("That door is already closed!");
      setgamestatus(SKIP_MONSTERS, GameStatus);
    }
    else if(Level->site[ox][oy].locchar != OPEN_DOOR)
    {
      queue_message("You can't close that!");
      setgamestatus(SKIP_MONSTERS, GameStatus);
    }
    else
    {
      Level->site[ox][oy].locchar = CLOSED_DOOR;
    }
    lset(ox, oy, CHANGED, *Level);
  }
}

// handle a h,j,k,l, etc...
void moveplayer(int dx, int dy)
{
  if(p_moveable(Player.x + dx, Player.y + dy))
  {
    if(Player.status[IMMOBILE] > 0)
    {
      resetgamestatus(FAST_MOVE, GameStatus);
      queue_message("You are unable to move");
    }
    else if((Player.maxweight < Player.itemweight) && random_range(2) && (!Player.status[LEVITATING]))
    {
      if(gamestatusp(MOUNTED, GameStatus))
      {
        queue_message("Your horse refuses to carry you and your pack another step!");
        queue_message("Your steed bucks wildly and throws you off!");
        p_damage(10, UNSTOPPABLE, "a cruelly abused horse");
        resetgamestatus(MOUNTED, GameStatus);
        summon(-1, HORSE);
      }
      else
      {
        p_damage(1, UNSTOPPABLE, "a rupture");
        queue_message("The weight of your pack drags you down. You can't move.");
      }
    }
    else
    {
      Player.x += dx;
      Player.y += dy;
      screencheck(Player.x, Player.y);
      p_movefunction(Level->site[Player.x][Player.y].p_locf);

      // causes moves to take effectively 30 seconds in town without
      // monsters being sped up compared to player
      if((Current_Environment == E_CITY) || (Current_Environment == E_VILLAGE))
      {
        twiddle = !twiddle;
        if(twiddle)
        {
          Time++;
          if(Time % 10 == 0)
          {
            tenminute_check();
          }
          else
          {
            minute_status_check();
          }
        }
      }

      // this test protects against player entering countryside and still
      // having effects from being on the Level, a kluge, but hey,...

      if(Current_Environment != E_COUNTRYSIDE)
      {
        if(!Level->site[Player.x][Player.y].things.empty())
        {
          std::vector<std::unique_ptr<object>> &item_list = Level->site[Player.x][Player.y].things;
          if(item_list.size() == 1)
          {
            queue_message("You see here a " + itemid(item_list.front().get()) + ".");
          }
          else
          {
            std::string items;
            std::string item_characters;
            for(auto it = item_list.rbegin(); it != item_list.rend();)
            {
              item_characters += static_cast<char>((*it)->objchar & A_CHARTEXT);
              items += itemid((*it).get());
              if(++it != item_list.rend())
              {
                items += ", ";
              }
            }
            if(items.length() > static_cast<unsigned int>(COLS))
            {
              queue_message("Items here: " + item_characters);
            }
            else
            {
              queue_message("Things that are here: ");
              append_message(items, true);
            }
          }
        }

        if(gamestatusp(FAST_MOVE, GameStatus))
        {
          if(!Level->site[Player.x][Player.y].things.empty() ||
             (optionp(RUNSTOP, Player) && loc_statusp(Player.x, Player.y, STOPS, *Level)))
          {
            resetgamestatus(FAST_MOVE, GameStatus);
          }
        }
        if(!Level->site[Player.x][Player.y].things.empty() && optionp(PICKUP, Player))
        {
          pickup();
        }
      }
    }
  }
  else if(gamestatusp(FAST_MOVE, GameStatus))
  {
    drawvision(Player.x, Player.y);
    resetgamestatus(FAST_MOVE, GameStatus);
  }
}

// handle a h,j,k,l, etc...
void movepincountry(int dx, int dy)
{
  bool takestime = true;
  if((Player.maxweight < Player.itemweight) && random_range(2) && (!Player.status[LEVITATING]))
  {
    if(gamestatusp(MOUNTED, GameStatus))
    {
      queue_message("Your horse refuses to carry you and your pack another step!");
      queue_message("Your steed bucks wildly and throws you off!");
      p_damage(10, UNSTOPPABLE, "a cruelly abused horse");
      resetgamestatus(MOUNTED, GameStatus);
      queue_message("With a shrill neigh of defiance, your former steed gallops");
      queue_message("off into the middle distance....");
      if(!Player.pack.empty())
      {
        queue_message("You remember (too late) that the contents of your pack");
        queue_message("were kept in your steed's saddlebags!");
        Player.pack.clear();
        calc_melee();
      }
    }
    else
    {
      p_damage(1, UNSTOPPABLE, "a rupture");
      queue_message("The weight of your pack drags you down. You can't move.");
    }
  }
  else
  {
    if(gamestatusp(LOST, GameStatus))
    {
      queue_message("Being lost, you strike out randomly....");
      dx = random_range(3) - 1;
      dy = random_range(3) - 1;
    }
    if(p_country_moveable(Player.x + dx, Player.y + dy))
    {
      if(Player.status[IMMOBILE] > 0)
      {
        queue_message("You are unable to move");
      }
      else
      {
        Player.x += dx;
        Player.y += dy;
        if(!gamestatusp(MOUNTED, GameStatus) && Player.possessions[O_BOOTS])
        {
          if(Player.possessions[O_BOOTS]->on_equip == I_BOOTS_7LEAGUE)
          {
            takestime = false;
            if(Player.possessions[O_BOOTS]->blessing < 0)
            {
              queue_message("Whooah! -- Your boots launch you into the sky....");
              queue_message("You come down in a strange location....");
              Player.x = random_range(WIDTH);
              Player.y = random_range(LENGTH);
              queue_message("Your boots disintegrate with a malicious giggle...");
              dispose_lost_objects(1, O_BOOTS);
            }
            else if(Player.possessions[O_BOOTS]->known != 2)
            {
              queue_message("Wow! Your boots take you 7 leagues in a single stride!");
              Player.possessions[O_BOOTS]->known = 2;
            }
          }
        }
        if(gamestatusp(LOST, GameStatus) && (Precipitation < 1) && c_statusp(Player.x, Player.y, SEEN, Country))
        {
          queue_message("Ah! Now you know where you are!");
          resetgamestatus(LOST, GameStatus);
        }
        else if(gamestatusp(LOST, GameStatus))
        {
          queue_message("You're still lost.");
        }
        if(Precipitation > 0)
        {
          Precipitation--;
        }
        c_set(Player.x, Player.y, SEEN, Country);
        terrain_check(takestime);
      }
    }
  }
}
