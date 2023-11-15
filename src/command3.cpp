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

/* command3.c */

/* This file contains some more top level command functions
   called from command1.c */

#include "date.h"
#include "glob.h"
#include "interactive_menu.hpp"
#include "scr.h"

#include <algorithm>
#include <cctype>
#include <format>
#include <string>
#include <vector>

#ifdef SAVE_LEVELS
extern void kill_all_levels();
#endif

extern std::string get_username();
extern void item_unequip(object *);

extern interactive_menu *menu;

// look at some spot
void examine()
{
  setgamestatus(SKIP_MONSTERS, GameStatus);
  if(Player.status[BLINDED] > 0)
  {
    queue_message("You're blind - you can't examine things.");
    return;
  }
  queue_message("Examine --");
  queue_message("Targeting....");
  append_message("", true);
  int x = Player.x, y = Player.y;
  int cursor_visibility = curs_set(1);
  omshowcursor(x, y);
  int player_input;
  do
  {
    if(inbounds(x, y))
    {
      if(Current_Environment == E_COUNTRYSIDE)
      {
        if(!c_statusp(x, y, SEEN, Country))
        {
          replace_last_message("How should I know what that is?");
        }
        else
        {
          replace_last_message("That terrain is:" + countryid(Country[x][y].current_terrain_type));
        }
      }
      else if(!view_los_p(Player.x, Player.y, x, y))
      {
        replace_last_message("I refuse to examine something I can't see.");
      }
      else
      {
        if(Level->site[x][y].creature)
        {
          replace_last_message(mstatus_string(Level->site[x][y].creature) + ".");
        }
        else if((Player.x == x) && (Player.y == y))
        {
          replace_last_message(describe_player());
        }
        else if(loc_statusp(x, y, SECRET, *Level))
        {
          replace_last_message("An age-worn stone wall.");
        }
        else
        {
          switch(Level->site[x][y].locchar)
          {
            case SPACE:
              replace_last_message("An infinite void.");
              break;
            case PORTCULLIS:
              replace_last_message("A heavy steel portcullis.");
              break;
            case ABYSS:
              replace_last_message("An entrance to the infinite abyss.");
              break;
            case FLOOR:
              if(Current_Dungeon == Current_Environment)
              {
                replace_last_message("A dirty stone floor.");
              }
              else
              {
                replace_last_message("The ground.");
              }
              break;
            case WALL:
              if(Level->site[x][y].aux == 0)
              {
                replace_last_message("A totally impervious wall.");
              }
              else if(Level->site[x][y].aux < 10)
              {
                replace_last_message("A pitted concrete wall.");
              }
              else if(Level->site[x][y].aux < 30)
              {
                replace_last_message("An age-worn sandstone wall.");
              }
              else if(Level->site[x][y].aux < 50)
              {
                replace_last_message("A smooth basalt wall.");
              }
              else if(Level->site[x][y].aux < 70)
              {
                replace_last_message("A solid granite wall.");
              }
              else if(Level->site[x][y].aux < 90)
              {
                replace_last_message("A wall of steel.");
              }
              else if(Level->site[x][y].aux < 210)
              {
                if(Current_Environment == E_CITY)
                {
                  replace_last_message("A thick wall of Rampart bluestone.");
                }
                else
                {
                  replace_last_message("A magically reinforced wall.");
                }
              }
              else
              {
                replace_last_message("An almost totally impervious wall.");
              }
              break;
            case RUBBLE:
              replace_last_message("A dangerous-looking pile of rubble.");
              break;
            case SAFE:
              replace_last_message("A steel safe inset into the floor.");
              break;
            case CLOSED_DOOR:
              replace_last_message("A solid oaken door, now closed.");
              break;
            case OPEN_DOOR:
              replace_last_message("A solid oaken door, now open.");
              break;
            case STATUE:
              replace_last_message("A strange-looking statue.");
              break;
            case STAIRS_UP:
              replace_last_message("A stairway leading up.");
              break;
            case STAIRS_DOWN:
              replace_last_message("A stairway leading down....");
              break;
            case TRAP:
              replace_last_message(trapid(Level->site[x][y].p_locf) + ".");
              break;
            case HEDGE:
              if(Level->site[x][y].p_locf == L_EARTH_STATION)
              {
                replace_last_message("A weird fibrillation of oozing tendrils.");
              }
              else
              {
                replace_last_message("A brambly, thorny hedge.");
              }
              break;
            case LAVA:
              replace_last_message("A bubbling pool of lava.");
              break;
            case LIFT:
              replace_last_message("A strange glowing disk.");
              break;
            case ALTAR:
              replace_last_message("An (un?)holy altar.");
              break;
            case CHAIR:
              replace_last_message("A chair.");
              break;
            case WHIRLWIND:
              replace_last_message("A strange cyclonic electrical storm.");
              break;
            case WATER:
              if(Level->site[x][y].p_locf == L_WATER)
              {
                replace_last_message("A deep pool of water.");
              }
              else if(Level->site[x][y].p_locf == L_CHAOS)
              {
                replace_last_message("A pool of primal chaos.");
              }
              else if(Level->site[x][y].p_locf == L_WATER_STATION)
              {
                replace_last_message("A bubbling pool of acid.");
              }
              else
              {
                replace_last_message("An eerie pool of water.");
              }
              break;
            case FIRE:
              replace_last_message("A curtain of fire.");
              break;
            default:
              replace_last_message("Wow, I haven't the faintest idea!");
              break;
          }
        }
        objectlist *item_list;
        if((item_list = Level->site[x][y].things) && !loc_statusp(x, y, SECRET, *Level))
        {
          if(!item_list->next)
          {
            replace_last_message(itemid(item_list->thing));
          }
          else
          {
            std::string items = "Things that are here : " + itemid(item_list->thing);
            std::string item_characters(1, static_cast<char>(item_list->thing->objchar & A_CHARTEXT));
            for(item_list = item_list->next; item_list; item_list = item_list->next)
            {
              items += ", " + itemid(item_list->thing);
              item_characters += static_cast<char>(item_list->thing->objchar & A_CHARTEXT);
            }
            if(items.length() <= static_cast<size_t>(COLS))
            {
              replace_last_message(items);
            }
            else
            {
              replace_last_message("Items here: " + item_characters);
            }
          }
        }
      }
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
          mouse_event event;
          switch(get_mouse_event(event))
          {
            case BUTTON1_CLICKED:
            case BUTTON1_RELEASED:
              x = event.x + HorizontalOffset;
              y = event.y + ScreenOffset;
              omshowcursor(x, y);
              break;
            default:
              break;
          }
        }
      }
    }
  } while(player_input != ESCAPE && player_input != KEY_ENTER && player_input != '\n');
  curs_set(cursor_visibility);
}

void help()
{
  std::vector<std::string> lines =
  {
    {"a: Overview"},
    {"b: Characters"},
    {"c: Inventories"},
    {"d: Movement"},
    {"e: Combat"},
    {"f: Bugs"},
    {"g: Magic"},
    {"h: The Countryside"},
    {"i: The Screen Display"},
    {"j: Saving and Restoring"},
    {"k: Options Settings"},
    {"l: Dungeon/City/Other Command List"},
    {"m: Countryside Command List"},
    {"ESCAPE: Forget the whole thing."},
    {""},
    {"Please enter the letter indcating what topic you want help on."}
  };
  menu->load(lines);
  menu->print();
  char  c;
  do
  {
    c = static_cast<char>(mcigetc());
  } while((c < 'a' || c > 'm') && c != ESCAPE);
  if(c != ESCAPE)
  {
    displayfile(std::format("{}help{}.txt", Omegalib, c+1-'a'));
  }
  xredraw();
}

void version()
{
  setgamestatus(SKIP_MONSTERS, GameStatus);
  queue_message(VERSIONSTRING);
  queue_message(":");
  queue_message(" Last Edited: ");
  queue_message(LAST_OMEGA_EDIT_DATE);
}

void fire()
{
  int             index, x1, y1, x2, y2;
  pob             obj;
  monster *m;

  queue_message("Fire/Throw --");
  index = getitem(NULL_ITEM);
  if(index == ABORT)
  {
    setgamestatus(SKIP_MONSTERS, GameStatus);
  }
  else if(index == CASHVALUE)
  {
    queue_message("Can't fire money at something!");
  }
  else if(cursed(Player.possessions[index]) && Player.possessions[index]->used)
  {
    queue_message("You can't seem to get rid of it!");
    /* load a crossbow */
  }
  else if(Player.possessions[O_WEAPON_HAND] &&
          (Player.possessions[O_WEAPON_HAND]->id == WEAPONID + 27) &&
          (Player.possessions[O_WEAPON_HAND]->aux != LOADED) &&
          (Player.possessions[index]->id == WEAPONID + 29))
  {
    queue_message("You crank back the crossbow and load a bolt.");
    Player.possessions[O_WEAPON_HAND]->aux = LOADED;
  }
  else
  {
    obj = Player.possessions[index];
    x1 = x2 = Player.x;
    y1 = y2 = Player.y;
    setspot(x2, y2);
    if(x2 == ABORT || y2 == ABORT)
    {
      setgamestatus(SKIP_MONSTERS, GameStatus);
      queue_message("Aborted.");
      return;
    }
    if((x2 == Player.x) && (y2 == Player.y))
    {
      queue_message("You practice juggling for a moment or two.");
    }
    else
    {
      do_object_los(obj->objchar, &x1, &y1, x2, y2);
      if((m = Level->site[x1][y1].creature))
      {
        int hitroll;
        if(obj->used)
        {
          hitroll = Player.hit;
        }
        else
        {
          pob main_hand_object = Player.possessions[O_WEAPON_HAND];
          if(main_hand_object && main_hand_object->used)
          {
            hitroll = Player.hit - main_hand_object->hit - main_hand_object->plus + obj->hit + obj->plus;
          }
          else
          {
            hitroll = Player.hit + obj->hit + obj->plus;
          }
        }
        if(obj->dmg == 0)
        {
          if(m->treasure > 0)
          { /* the monster can have treasure/objects */
            queue_message("Your gift is caught!");
            givemonster(m, split_item(1, obj));
            conform_lost_objects(1, obj);
          }
          else
          {
            queue_message("Your thrown offering is ignored.");
            setgamestatus(SUPPRESS_PRINTING, GameStatus);
            p_drop_at(x1, y1, 1, obj);
            resetgamestatus(SUPPRESS_PRINTING, GameStatus);
            conform_lost_objects(1, obj);
          }
        }
        else if(obj->aux == I_SCYTHE)
        {
          queue_message("It isn't very aerodynamic... you miss.");
          setgamestatus(SUPPRESS_PRINTING, GameStatus);
          p_drop_at(x1, y1, 1, obj);
          resetgamestatus(SUPPRESS_PRINTING, GameStatus);
          conform_lost_objects(1, obj);
        }
        else if(hitp(hitroll, m->ac))
        { /* ok already, hit the damn thing */
          weapon_use(2 * statmod(Player.str), obj, m);
          if((obj->id == WEAPONID + 28 || obj->id == WEAPONID + 29) && !random_range(4))
          {
            dispose_lost_objects(1, obj);
          }
          else
          {
            setgamestatus(SUPPRESS_PRINTING, GameStatus);
            p_drop_at(x1, y1, 1, obj);
            resetgamestatus(SUPPRESS_PRINTING, GameStatus);
            conform_lost_objects(1, obj);
          }
        }
        else
        {
          queue_message("You miss it.");
          setgamestatus(SUPPRESS_PRINTING, GameStatus);
          p_drop_at(x1, y1, 1, obj);
          resetgamestatus(SUPPRESS_PRINTING, GameStatus);
          conform_lost_objects(1, obj);
        }
      }
      else
      {
        setgamestatus(SUPPRESS_PRINTING, GameStatus);
        p_drop_at(x1, y1, 1, obj);
        resetgamestatus(SUPPRESS_PRINTING, GameStatus);
        conform_lost_objects(1, obj);
        plotspot(x1, y1, true);
      }
    }
  }
}

void quit(int)
{
  queue_message("Quit: Are you sure? [yn] ");
  if(ynq() == 'y')
  {
    if(Player.rank[ADEPT] == 0)
    {
      display_quit();
    }
    else
    {
      display_bigwin();
    }
#ifdef SAVE_LEVELS
    kill_all_levels();
#endif
    endgraf();
    exit(0);
  }
  else
  {
    resetgamestatus(SKIP_MONSTERS, GameStatus);
  }
}

/* rest in 10 second segments so if woken up by monster won't
die automatically.... */
void nap()
{
  static int naptime;
  if(gamestatusp(FAST_MOVE, GameStatus))
  {
    if(naptime-- < 1)
    {
      queue_message("Yawn. You wake up.");
      resetgamestatus(FAST_MOVE, GameStatus);
      drawvision(Player.x, Player.y);
    }
  }
  else
  {
    queue_message("Rest for how long? (in minutes) ");
    naptime = (int)parsenum();
    if(naptime > 600)
    {
      queue_message("You can only sleep up to 10 hours (600 minutes).");
      naptime = 3600;
    }
    else
    {
      naptime *= 6;
    }
    if(naptime > 1)
    {
      setgamestatus(FAST_MOVE, GameStatus);
      queue_message("Resting.... ");
    }
  }
}

void charid()
{
  int  countryside = false;

  queue_message("Character to identify: ");
  char id = mgetc();
  if(Current_Environment == E_COUNTRYSIDE)
  {
    countryside = true;
    std::string country_description{countryid(id)};
    if(country_description == "I have no idea.")
    {
      countryside = false;
    }
    else
    {
      append_message(country_description);
    }
  }
  if(!countryside)
  {
    if(std::isalpha(id) || id == '@')
    {
      append_message("A monster or NPC -- examine (x) to find out exactly.");
    }
    else
    {
      switch(id)
      {
        case(SPACE & 0xff):
          append_message("An airless void (if seen) or unknown region (if unseen).");
          break;
        case(WALL & 0xff):
          append_message("An (impenetrable?) wall.");
          break;
        case(OPEN_DOOR & 0xff):
          append_message("An open door.");
          break;
        case(CLOSED_DOOR & 0xff):
          append_message("A closed (possibly locked) door.");
          break;
        case(LAVA & 0xff):
          append_message("A pool of lava.");
          break;
        case(HEDGE & 0xff):
          append_message("A dense hedge.");
          break;
        case(WATER & 0xff):
          append_message("A deep body of water.");
          break;
        case(FIRE & 0xff):
          append_message("A curtain of fire.");
          break;
        case(TRAP & 0xff):
          append_message("An uncovered trap.");
          break;
        case(STAIRS_UP & 0xff):
          append_message("A stairway leading up.");
          break;
        case(STAIRS_DOWN & 0xff):
          append_message("A stairway leading down.");
          break;
        case(FLOOR & 0xff):
          append_message("The dungeon floor.");
          break;
        case(PORTCULLIS & 0xff):
          append_message("A heavy steel portcullis.");
          break;
        case(ABYSS & 0xff):
          append_message("An entrance to the infinite abyss.");
          break;
        case(PLAYER & 0xff):
          append_message("You, the player.");
          break;
        case(CORPSE & 0xff):
          append_message("The remains of some creature.");
          break;
        case(THING & 0xff):
          append_message("Some random miscellaneous object.");
          break;
        case(SAFE & 0xff):
          append_message("A steel safe inset into the floor.");
          break;
        case(RUBBLE & 0xff):
          append_message("A dangerous-looking pile of rubble.");
          break;
        case(STATUE & 0xff):
          append_message("A statue.");
          break;
        case(ALTAR & 0xff):
          append_message("A (un?)holy altar.");
          break;
        case(CASH & 0xff):
          append_message("Bills, specie, gems: cash.");
          break;
        case(PILE & 0xff):
          append_message("A pile of objects.");
          break;
        case(FOOD & 0xff):
          append_message("Something edible.");
          break;
        case(WEAPON & 0xff):
          append_message("Some kind of weapon.");
          break;
        case(MISSILEWEAPON & 0xff):
          append_message("Some kind of missile weapon.");
          break;
        case(SCROLL & 0xff):
          append_message("Something readable.");
          break;
        case(POTION & 0xff):
          append_message("Something drinkable.");
          break;
        case(ARMOR & 0xff):
          append_message("A suit of armor.");
          break;
        case(SHIELD & 0xff):
          append_message("A shield.");
          break;
        case(CLOAK & 0xff):
          append_message("A cloak.");
          break;
        case(BOOTS & 0xff):
          append_message("A pair of boots.");
          break;
        case(STICK & 0xff):
          append_message("A stick.");
          break;
        case(RING & 0xff):
          append_message("A ring.");
          break;
        case(ARTIFACT & 0xff):
          append_message("An artifact.");
          break;
        case(CHAIR & 0xff):
          append_message("A chair.");
          break;
        case(WHIRLWIND & 0xff):
          append_message("A whirlwind.");
          break;
        default:
          append_message("That character is unused.");
          break;
      }
    }
  }
}

void wizard()
{
  setgamestatus(SKIP_MONSTERS, GameStatus);
  if(gamestatusp(CHEATED, GameStatus))
  {
    queue_message("You're already in wizard mode!");
  }
  else
  {
    queue_message("Really try to enter wizard mode? [yn] ");
    if(ynq() == 'y')
    {
      if(get_username() == WIZARD)
      {
        setgamestatus(CHEATED, GameStatus);
        queue_message("Wizard mode set.");
      }
      else
      {
        queue_message("There is a shrieking sound, as of reality being distorted.");
        std::string message{std::format("{}, the Wizard of Omega appears before you....", WIZARD)};
        message[0] = std::toupper(message[0]);
        queue_message(message);
        queue_message("'Do not meddle in the affairs of Wizards --");
        if(random_range(2))
        {
          append_message("it makes them soggy and hard to light.'");
        }
        else
        {
          append_message("for they are subtle, and swift to anger!'");
        }
      }
    }
  }
}

/* Jump, that is */
void vault()
{
  int x = Player.x, y = Player.y, jumper = 0;

  if(Player.possessions[O_BOOTS])
  {
    if(Player.possessions[O_BOOTS]->on_equip == I_BOOTS_JUMPING)
    {
      jumper = 2;
    }
  }
  if(Player.status[IMMOBILE] > 0)
  {
    resetgamestatus(FAST_MOVE, GameStatus);
    queue_message("You are unable to move.");
  }
  else
  {
    setgamestatus(SKIP_MONSTERS, GameStatus);
    queue_message("Jump where?");
    setspot(x, y);
    if(!los_p(Player.x, Player.y, x, y))
    {
      queue_message("The way is obstructed.");
    }
    else if(Player.itemweight > Player.maxweight)
    {
      queue_message("You are too burdened to jump anywhere.");
    }
    else if(distance(x, y, Player.x, Player.y) > std::max(2, statmod(Player.agi) + 2) + jumper)
    {
      queue_message("The jump is too far for you.");
    }
    else if(Level->site[x][y].creature)
    {
      queue_message("You can't jump on another creature.");
    }
    else if(!p_moveable(x, y))
    {
      queue_message("You can't jump there.");
    }
    else
    {
      resetgamestatus(SKIP_MONSTERS, GameStatus);
      Player.x = x;
      Player.y = y;
      if((!jumper) && (random_range(30) > Player.agi))
      {
        queue_message("Oops -- took a tumble.");
        setgamestatus(SKIP_PLAYER, GameStatus);
        p_damage((Player.itemweight / 250), UNSTOPPABLE, "clumsiness");
      }
      p_movefunction(Level->site[Player.x][Player.y].p_locf);
      if(Current_Environment != E_COUNTRYSIDE)
      {
        if((Level->site[Player.x][Player.y].things) && (optionp(PICKUP, Player)))
        {
          pickup();
        }
      }
    }
  }
}

std::string attack_string()
{
  if(!Player.possessions[O_WEAPON_HAND])
  {
    return "Punch";
  }
  else if(Player.possessions[O_WEAPON_HAND]->type == THRUSTING)
  {
    return "Thrust";
  }
  else if(Player.possessions[O_WEAPON_HAND]->type == STRIKING)
  {
    return "Strike";
  }
  else
  {
    return "Cut";
  }
}

std::string defense_string()
{
  if(!Player.possessions[O_WEAPON_HAND])
  {
    return "Dodge";
  }
  else if(Player.possessions[O_WEAPON_HAND]->type == THRUSTING)
  {
    return "Parry";
  }
  else
  {
    return "Block";
  }
}

// Sets sequence of combat maneuvers.
void tacoptions()
{
  setgamestatus(SKIP_MONSTERS, GameStatus);

  size_t actionsleft = maneuvers();
  size_t place       = 0;
  bool   done        = false;
  bool   draw_again  = true;
  do
  {
    if(draw_again)
    {
      menuclear();
      menuprint("Enter a combat maneuvers sequence.\n");
      menuprint("? for help, ! for default, backspace to start again,\n");
      menuprint(" RETURN to save sequence\n");
      showmenu();
      draw_again = false;
    }
    queue_message("Maneuvers Left: " + std::to_string(actionsleft));
    switch(mgetc())
    {
      case '?':
        combat_help();
        draw_again = true;
        break;
      case 'a':
      case 'A':
        if(actionsleft < 1)
        {
          queue_message("No more maneuvers!");
        }
        else
        {
          menuprint("\n" + attack_string() + ":");
          Player.meleestr[place++] = 'A';
          Player.meleestr[place++] = getlocation();
          --actionsleft;
        }
        break;
      case 'b':
      case 'B':
        if(actionsleft < 1)
        {
          queue_message("No more maneuvers!");
        }
        else
        {
          menuprint("\n" + defense_string() + ":");
          Player.meleestr[place++] = 'B';
          Player.meleestr[place++] = getlocation();
          --actionsleft;
        }
        break;
      case 'l':
      case 'L':
        if(actionsleft < 2)
        {
          queue_message("Not enough maneuvers to lunge!");
        }
        else
        {
          if(Player.possessions[O_WEAPON_HAND])
          {
            if(Player.possessions[O_WEAPON_HAND]->type != MISSILE)
            {
              menuprint("\nLunge:");
              Player.meleestr[place++] = 'L';
              Player.meleestr[place++] = getlocation();
              actionsleft -= 2;
            }
            else
            {
              queue_message("Can't lunge with a missile weapon!");
            }
          }
          else
          {
            queue_message("Can't lunge without a weapon!");
          }
        }
        break;
      case 'r':
      case 'R':
        if(actionsleft < 2)
        {
          queue_message("Not enough maneuvers to riposte!");
        }
        else
        {
          if(Player.possessions[O_WEAPON_HAND])
          {
            if(Player.possessions[O_WEAPON_HAND]->type == THRUSTING)
            {
              Player.meleestr[place++] = 'R';
              menuprint("\nRiposte:");
              Player.meleestr[place++] = getlocation();
              actionsleft -= 2;
            }
            else
            {
              queue_message("Can't riposte without a thrusting weapon!");
            }
          }
          else
          {
            queue_message("Can't riposte without a thrusting weapon!");
          }
        }
        break;
      case KEY_BACKSPACE:
      case KEY_DC:
      case DELETE:
      case '\b':
        place       = 0;
        actionsleft = maneuvers();
        draw_again  = true;
        break;
      case '!':
        menuclear();
        menuprint("Enter a combat maneuvers sequence.\n");
        menuprint("? for help, ! for default, backspace to start again,\n");
        menuprint(" RETURN to save sequence\n\n");
        for(place = 0; place < maneuvers(); ++place)
        {
          if(place & 1)
          { // every 2nd time around
            Player.meleestr[place * 2]       = 'B';
            Player.meleestr[(place * 2) + 1] = 'C';
            menuprint(defense_string());
            menuprint(" Center.\n");
          }
          else
          {
            Player.meleestr[place * 2]       = 'A';
            Player.meleestr[(place * 2) + 1] = 'C';
            menuprint(attack_string());
            menuprint(" Center.\n");
          }
        }
        actionsleft = 0;
        showmenu();
        Player.meleestr[place * 2] = '\0';
        break;
      case '\n':
      case '\r':
      case ESCAPE:
        done = true;
        break;
    }
  } while(!done);
  xredraw();
  Player.meleestr[place] = 0;
}

/* Do the Artful Dodger trick */
void pickpocket()
{
  int             dx, dy;
  monster *m;

  queue_message("Pickpocketing --");

  int index = getdir();

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
      queue_message("There's nothing there to steal from!!!");
      setgamestatus(SKIP_MONSTERS, GameStatus);
    }
    else
    {
      m = Level->site[Player.x + dx][Player.y + dy].creature;
      if(m->id == GUARD)
      {
        queue_message("Trying to steal from a guardsman, eh?");
        queue_message("Not a clever idea.");
        if(Player.cash > 0)
        {
          queue_message("As a punitive fine, the guard takes all your money.");
          Player.cash = 0;
          dataprint();
        }
        else
        {
          queue_message("The guardsman places you under arrest.");
          send_to_jail();
        }
      }
      else
      {
        bool success = (Player.dex*5 + Player.rank[THIEVES]*20 - m->level*20) > random_range(100);
        if(!m->possessions)
        {
          queue_message("You couldn't find anything worth taking!");
        }
        else if(success)
        {
          queue_message(std::format("You stole: {}.", itemid(m->possessions->thing)));
          Player.alignment--;
          gain_experience(m->level * m->level);
          gain_item(m->possessions->thing);
          m->possessions = m->possessions->next;
        }
        else
        {
          queue_message("You fail to obtain anything.");
        }
        bool caught = (Player.dex*4 + Player.rank[THIEVES]*20 - m->level*10) < random_range(100);
        if(caught && (!Player.status[INVISIBLE] || random_range(2)))
        {
          queue_message("You manage to annoy it...");
          m_status_set(*m, HOSTILE);
        }
      }
    }
  }
}

void rename_player()
{
  setgamestatus(SKIP_MONSTERS, GameStatus);
  queue_message("Rename Character: ");
  std::string name = msgscanstring();
  if(!name.empty())
  {
    name.front() = std::toupper(name.front());
    Player.name = name;
  }
  queue_message(std::format("Henceforth, you shall be known as {}.", Player.name));
  dataprint();
}

void abortshadowform()
{
  setgamestatus(SKIP_MONSTERS, GameStatus);
  if(Player.status[SHADOWFORM] && (Player.status[SHADOWFORM] < 1000))
  {
    queue_message("You abort your spell of Shadow Form.");
    Player.immunity[NORMAL_DAMAGE]--;
    Player.immunity[ACID]--;
    Player.immunity[THEFT]--;
    Player.immunity[INFECTION]--;
    queue_message("You feel less shadowy now.");
    Player.status[SHADOWFORM] = 0;
  }
}

void tunnel()
{
  int dir, ox, oy, aux;

  queue_message("Tunnel -- ");
  dir = getdir();
  if(dir == ABORT)
  {
    setgamestatus(SKIP_MONSTERS, GameStatus);
  }
  else
  {
    ox = Player.x + Dirs[0][dir];
    oy = Player.y + Dirs[1][dir];
    if(loc_statusp(ox, oy, SECRET, *Level))
    {
      queue_message("You have no success as yet.");
    }
    else if(Level->site[ox][oy].locchar != WALL)
    {
      queue_message("You can't tunnel through that!");
      setgamestatus(SKIP_MONSTERS, GameStatus);
    }
    else
    {
      aux = Level->site[ox][oy].aux;
      if(random_range(20) == 1)
      {
        if(!Player.possessions[O_WEAPON_HAND])
        {
          queue_message("Ouch! broke a fingernail...");
          p_damage(Player.str / 6, UNSTOPPABLE, "a broken fingernail");
        }
        else if((Player.possessions[O_WEAPON_HAND]->type == THRUSTING) ||
                ((Player.possessions[O_WEAPON_HAND]->type != STRIKING) &&
                 (Player.possessions[O_WEAPON_HAND]->fragility < random_range(20))))
        {
          queue_message("Clang! Uh oh...");
          (void)damage_item(Player.possessions[O_WEAPON_HAND]);
        }
        else
        {
          queue_message("Your digging implement shows no sign of breaking.");
        }
      }
      if(!Player.possessions[O_WEAPON_HAND])
      {
        if((aux > 0) && ((Player.str / 3) + random_range(100) > aux))
        {
          queue_message("You carve a tunnel through the stone!");
          tunnelcheck();
          Level->site[ox][oy].locchar = RUBBLE;
          Level->site[ox][oy].p_locf  = L_RUBBLE;
          lset(ox, oy, CHANGED, *Level);
        }
        else
        {
          queue_message("No joy.");
        }
      }
      else if(Player.possessions[O_WEAPON_HAND]->type == THRUSTING)
      {
        if((aux > 0) && (Player.possessions[O_WEAPON_HAND]->dmg * 2 + random_range(100) > aux))
        {
          queue_message("You carve a tunnel through the stone!");
          tunnelcheck();
          Level->site[ox][oy].locchar = RUBBLE;
          Level->site[ox][oy].p_locf  = L_RUBBLE;
          lset(ox, oy, CHANGED, *Level);
        }
        else
        {
          queue_message("No luck.");
        }
      }
      else if((aux > 0) && (Player.possessions[O_WEAPON_HAND]->dmg + random_range(100) > aux))
      {
        queue_message("You carve a tunnel through the stone!");
        tunnelcheck();
        Level->site[ox][oy].locchar = RUBBLE;
        Level->site[ox][oy].p_locf  = L_RUBBLE;
        lset(ox, oy, CHANGED, *Level);
      }
      else
      {
        queue_message("You have no success as yet.");
      }
    }
  }
}

void hunt(Symbol terrain)
{
  int fertility = 0;
  switch(terrain)
  {
    case SWAMP:
      queue_message("You hesitate to hunt for food in the marshy wasteland.");
      break;
    case VOLCANO:
    case CASTLE:
    case TEMPLE:
    case CAVES:
    case STARPEAK:
    case MAGIC_ISLE:
    case DRAGONLAIR:
      queue_message("There is nothing alive here (or so it seems).");
      break;
    case VILLAGE:
    case CITY:
      queue_message("You can find no food here; perhaps if you went inside....");
      break;
    case ROAD:
      queue_message("You feel it would be a better idea to hunt off the road.");
      break;
    case CHAOS_SEA:
      queue_message("Food in the Sea of Chaos? Go on!");
      break;
    case DESERT:
      queue_message("You wander off into the trackless desert in search of food...");
      Time += 100;
      hourly_check();
      fertility = 10;
      break;
    case JUNGLE:
      queue_message("You search the lush and verdant jungle for game....");
      Time += 100;
      hourly_check();
      fertility = 80;
      break;
    case PLAINS:
      queue_message("You set off through the tall grass; the game is afoot.");
      Time += 100;
      hourly_check();
      fertility = 50;
      break;
    case TUNDRA:
      queue_message("You blaze a trail through the frozen wasteland....");
      Time += 100;
      hourly_check();
      fertility = 30;
      break;
    case FOREST:
      queue_message("You try to follow the many tracks through the forest loam....");
      Time += 100;
      hourly_check();
      fertility = 70;
      break;
    case MOUNTAINS:
    case PASS:
      queue_message("You search the cliff walls looking for something to eat....");
      Time += 100;
      hourly_check();
      fertility = 30;
      break;
    case RIVER:
      queue_message("The halcyon river is your hopeful food source...");
      Time += 100;
      hourly_check();
      fertility = 80;
      break;
  }
  if(((Date % 360 < 60) || (Date % 360 > 300)) && (terrain != DESERT) && (terrain != JUNGLE))
  {
    queue_message("The cold weather impedes your hunt....");
    fertility = fertility / 2;
  }
  if(fertility > random_range(100))
  {
    queue_message("You have an encounter...");
    change_environment(E_TACTICAL_MAP);
  }
  else
  {
    queue_message("Your hunt is fruitless.");
  }
}

void dismount_steed()
{
  pml ml;
  if(!gamestatusp(MOUNTED, GameStatus))
  {
    queue_message("You're on foot already!");
  }
  else if(Current_Environment == E_COUNTRYSIDE)
  {
    queue_message("If you leave your steed here he will wander away!");
    queue_message("Do it anyway? [yn] ");
    if(ynq() == 'y')
    {
      resetgamestatus(MOUNTED, GameStatus);
    }
  }
  else
  {
    resetgamestatus(MOUNTED, GameStatus);
    ;
    ml                                       = new monsterlist;
    ml->m                                    = new monster;
    *(ml->m)                                 = Monsters[HORSE];
    ml->m->x                                 = Player.x;
    ml->m->y                                 = Player.y;
    ml->m->status                            = MOBILE + SWIMMING;
    ml->next                                 = Level->mlist;
    Level->site[Player.x][Player.y].creature = ml->m;
    Level->mlist                             = ml;
  }
  calc_melee();
}

void city_move()
{
  int site, x = Player.x, y = Player.y, toggle = false;
  if(Current_Environment != E_CITY)
  {
    queue_message("This command only works in the city!");
    setgamestatus(SKIP_MONSTERS, GameStatus);
  }
  else if(Player.status[IMMOBILE] > 0)
  {
    queue_message("You can't even move!");
  }
  else if(hostilemonstersnear())
  {
    setgamestatus(SKIP_MONSTERS, GameStatus);
    queue_message("You can't move this way with hostile monsters around!");
  }
  else if(Level->site[Player.x][Player.y].aux == NOCITYMOVE)
  {
    queue_message("You can't use the 'M' command from this location.");
  }
  else
  {
    queue_message("Move to which establishment [? for help, ESCAPE to quit]");
    site = parsecitysite();
    if(site != ABORT)
    {
      queue_message("You're on your way...");
      while((x != CitySiteList[site][1]) || (y != CitySiteList[site][2]))
      {
        toggle = !toggle;
        if(toggle)
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
        x += sign(CitySiteList[site][1] - x);
        y += sign(CitySiteList[site][2] - y);
        screencheck(x, y);
        levelrefresh();
      }
      Player.x = x;
      Player.y = y;
      screencheck(Player.x, Player.y);
      queue_message("Made it!");
      drawvision(Player.x, Player.y);
      p_movefunction(Level->site[x][y].p_locf);
    }
  }
}

void frobgamestatus()
{
  char response;
  long num;
  queue_message("Set or Reset or Forget it [s,r,ESCAPE]:");
  do
  {
    response = (char)mcigetc();
  } while((response != 'r') && (response != 's') && (response != ESCAPE));
  if(response != ESCAPE)
  {
    queue_message("Enter log2 of flag:");
    num = (int)parsenum();
    if(num > -1)
    {
      num = pow2(num);
      if(num == CHEATED)
      {
        queue_message("Can't alter Wizard flag.");
        return;
      }
      if(response == 's')
      {
        setgamestatus(num, GameStatus);
      }
      else
      {
        resetgamestatus(num, GameStatus);
      }
      queue_message("Done....");
    }
  }
}
