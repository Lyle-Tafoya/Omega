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

#include <algorithm>
#include <string>
#include <vector>

#ifdef SAVE_LEVELS
extern void kill_all_levels();
#endif

extern std::string get_username();
extern void item_unequip(object *);

extern interactive_menu *menu;

/* look at some spot */
void examine()
{
  pol ol;
  int x = Player.x, y = Player.y, drewmenu = false;

  /* WDT HACK: I'm not sure I buy that one shouldn't be able to examine
   * when one is blind.  However, the 'right' way to do it is certainly
   * too difficult (I would expect to be able to examine only the items
   * I actually recall).  So, for now I'll use David Given's compromise.
   * 12/30/98
   */
  if(Player.status[BLINDED] > 0)
  {
    mprint("You're blind - you can't examine things.");
    return;
  }
  setgamestatus(SKIP_MONSTERS, GameStatus);
  mprint("Examine --");
  setspot(&x, &y);
  if(inbounds(x, y))
  {
    if(Current_Environment == E_COUNTRYSIDE)
    {
      if(!c_statusp(x, y, SEEN, Country))
      {
        print3("How should I know what that is?");
      }
      else
      {
        mprint("That terrain is:");
        mprint(countryid(Country[x][y].current_terrain_type));
      }
    }
    else if(!view_los_p(Player.x, Player.y, x, y))
    {
      print3("I refuse to examine something I can't see.");
    }
    else
    {
      if(Level->site[x][y].creature != NULL)
      {
        mprint(mstatus_string(Level->site[x][y].creature));
      }
      else if((Player.x == x) && (Player.y == y))
      {
        describe_player();
      }
      if(loc_statusp(x, y, SECRET, *Level))
      {
        print2("An age-worn stone wall.");
      }
      else
      {
        switch(Level->site[x][y].locchar)
        {
          case SPACE:
            print2("An infinite void.");
            break;
          case PORTCULLIS:
            print2("A heavy steel portcullis");
            break;
          case ABYSS:
            print2("An entrance to the infinite abyss");
            break;
          case FLOOR:
            if(Current_Dungeon == Current_Environment)
            {
              print2("A dirty stone floor.");
            }
            else
            {
              print2("The ground.");
            }
            break;
          case WALL:
            if(Level->site[x][y].aux == 0)
            {
              print2("A totally impervious wall.");
            }
            else if(Level->site[x][y].aux < 10)
            {
              print2("A pitted concrete wall.");
            }
            else if(Level->site[x][y].aux < 30)
            {
              print2("An age-worn sandstone wall.");
            }
            else if(Level->site[x][y].aux < 50)
            {
              print2("A smooth basalt wall.");
            }
            else if(Level->site[x][y].aux < 70)
            {
              print2("A solid granite wall.");
            }
            else if(Level->site[x][y].aux < 90)
            {
              print2("A wall of steel.");
            }
            else if(Level->site[x][y].aux < 210)
            {
              if(Current_Environment == E_CITY)
              {
                print2("A thick wall of Rampart bluestone");
              }
              else
              {
                print2("A magically reinforced wall.");
              }
            }
            else
            {
              print2("An almost totally impervious wall.");
            }
            break;
          case RUBBLE:
            print2("A dangerous-looking pile of rubble.");
            break;
          case SAFE:
            print2("A steel safe inset into the floor.");
            break;
          case CLOSED_DOOR:
            print2("A solid oaken door, now closed.");
            break;
          case OPEN_DOOR:
            print2("A solid oaken door, now open.");
            break;
          case STATUE:
            print2("A strange-looking statue.");
            break;
          case STAIRS_UP:
            print2("A stairway leading up.");
            break;
          case STAIRS_DOWN:
            print2("A stairway leading down....");
            break;
          case TRAP:
            print2(trapid(Level->site[x][y].p_locf));
            break;
          case HEDGE:
            if(Level->site[x][y].p_locf == L_EARTH_STATION)
            {
              print2("A weird fibrillation of oozing tendrils.");
            }
            else
            {
              print2("A brambly, thorny hedge.");
            }
            break;
          case LAVA:
            print2("A bubbling pool of lava.");
            break;
          case LIFT:
            print2("A strange glowing disk.");
            break;
          case ALTAR:
            print2("An (un?)holy altar.");
            break;
          case CHAIR:
            print2("A chair.");
            break;
          case WHIRLWIND:
            print2("A strange cyclonic electrical storm.");
            break;
          case WATER:
            if(Level->site[x][y].p_locf == L_WATER)
            {
              print2("A deep pool of water.");
            }
            else if(Level->site[x][y].p_locf == L_CHAOS)
            {
              print2("A pool of primal chaos.");
            }
            else if(Level->site[x][y].p_locf == L_WATER_STATION)
            {
              print2("A bubbling pool of acid.");
            }
            else
            {
              print2("An eerie pool of water.");
            }
            break;
          case FIRE:
            print2("A curtain of fire.");
            break;
          default:
            print2("Wow, I haven't the faintest idea!");
            break;
        }
      }
      if((ol = Level->site[x][y].things) != NULL && !loc_statusp(x, y, SECRET, *Level))
      {
        if(ol->next == NULL)
        {
          print3(itemid(ol->thing));
        }
        else
        {
          drewmenu = true;
          std::vector<std::string> lines;
          while(ol != NULL)
          {
            lines.emplace_back(std::string("  ") + itemid(ol->thing));
            ol = ol->next;
          }
          menu->load(lines, {{"Things on floor:"}});
          menu->get_player_input();
        }
      }
      sign_print(x, y, true);
    }
  }
  if(drewmenu)
  {
    xredraw();
  }
}

void help()
{
  char  c;
  char  filestr[80];
  FILE *in, *out;
  int   n;

  print1("Please enter the letter indicating what topic you want help on.");
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
    {"n: Everything"},
    {"ESCAPE: Forget the whole thing."}
  };
  menu->load(lines);
  menu->print();
  do
  {
    c = (char)mcigetc();
  } while((c < 'a' || c > 'n') && c != ESCAPE);
  if(c == 'n')
  {
    print1("Trying to copy all help files to ./omega.doc ");
    nprint1("Confirm [yn]");
    if(ynq1() == 'y')
    {
      out = checkfopen("omega.doc", "w");
      print2("Copying");
      for(n = 1; n <= 13; n++)
      {
        nprint2(".");
        sprintf(Str1, "%shelp%d.txt", Omegalib, n);
        in = checkfopen(Str1, "r");
        while(fgets(Str1, STRING_LEN, in))
        {
          fputs(Str1, out);
        }
        fclose(in);
      }
      fclose(out);
      nprint2(" Done.");
    }
  }
  else if(c != ESCAPE)
  {
    sprintf(filestr, "%shelp%d.txt", Omegalib, c + 1 - 'a');
    displayfile(filestr);
  }
  xredraw();
}

void version()
{
  setgamestatus(SKIP_MONSTERS, GameStatus);
  print3(VERSIONSTRING);
  nprint3(":");
  nprint3(" Last Edited: ");
  nprint3(LAST_OMEGA_EDIT_DATE);
}

void fire()
{
  int             index, x1, y1, x2, y2;
  pob             obj;
  struct monster *m;

  print1("Fire/Throw --");
  index = getitem(NULL_ITEM);
  if(index == ABORT)
  {
    setgamestatus(SKIP_MONSTERS, GameStatus);
  }
  else if(index == CASHVALUE)
  {
    print3("Can't fire money at something!");
  }
  else if(cursed(Player.possessions[index]) && Player.possessions[index]->used)
  {
    print3("You can't seem to get rid of it!");
    /* load a crossbow */
  }
  else if((Player.possessions[O_WEAPON_HAND] != NULL) &&
          (Player.possessions[O_WEAPON_HAND]->id == WEAPONID + 27) &&
          (Player.possessions[O_WEAPON_HAND]->aux != LOADED) &&
          (Player.possessions[index]->id == WEAPONID + 29))
  {
    mprint("You crank back the crossbow and load a bolt.");
    Player.possessions[O_WEAPON_HAND]->aux = LOADED;
  }
  else
  {
    if(Player.possessions[index]->used)
    {
      item_unequip(Player.possessions[index]);
    }
    obj = Player.possessions[index];
    x1 = x2 = Player.x;
    y1 = y2 = Player.y;
    setspot(&x2, &y2);
    if(x2 == ABORT || y2 == ABORT)
    {
      setgamestatus(SKIP_MONSTERS, GameStatus);
      print2("Aborted.");
      return;
    }
    if((x2 == Player.x) && (y2 == Player.y))
    {
      mprint("You practice juggling for a moment or two.");
    }
    else
    {
      do_object_los(obj->objchar, &x1, &y1, x2, y2);
      if((m = Level->site[x1][y1].creature) != NULL)
      {
        if(obj->dmg == 0)
        {
          if(m->treasure > 0)
          { /* the monster can have treasure/objects */
            mprint("Your gift is caught!");
            givemonster(m, split_item(1, obj));
            conform_lost_objects(1, obj);
          }
          else
          {
            mprint("Your thrown offering is ignored.");
            setgamestatus(SUPPRESS_PRINTING, GameStatus);
            p_drop_at(x1, y1, 1, obj);
            resetgamestatus(SUPPRESS_PRINTING, GameStatus);
            conform_lost_objects(1, obj);
          }
        }
        else if(obj->aux == I_SCYTHE)
        {
          mprint("It isn't very aerodynamic... you miss.");
          setgamestatus(SUPPRESS_PRINTING, GameStatus);
          p_drop_at(x1, y1, 1, obj);
          resetgamestatus(SUPPRESS_PRINTING, GameStatus);
          conform_lost_objects(1, obj);
        }
        else if(hitp(Player.hit, m->ac))
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
          mprint("You miss it.");
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
  mprint("Quit: Are you sure? [yn] ");
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
      mprint("Yawn. You wake up.");
      resetgamestatus(FAST_MOVE, GameStatus);
      drawvision(Player.x, Player.y);
    }
  }
  else
  {
    mprint("Rest for how long? (in minutes) ");
    naptime = (int)parsenum();
    if(naptime > 600)
    {
      print3("You can only sleep up to 10 hours (600 minutes)");
      naptime = 3600;
    }
    else
    {
      naptime *= 6;
    }
    if(naptime > 1)
    {
      setgamestatus(FAST_MOVE, GameStatus);
      mprint("Resting.... ");
    }
  }
}

void charid()
{
  char id;
  int  countryside = false;
  char cstr[80];

  mprint("Character to identify: ");
  id = mgetc();
  if(Current_Environment == E_COUNTRYSIDE)
  {
    countryside = true;
    strcpy(cstr, countryid(id));
    if(strcmp(cstr, "I have no idea.") == 0)
    {
      countryside = false;
    }
    else
    {
      mprint(cstr);
    }
  }
  if(!countryside)
  {
    if((id >= 'a' && id <= 'z') || (id >= 'A' && id <= 'Z') || id == '@')
    {
      mprint("A monster or NPC -- examine (x) to find out exactly.");
    }
    else
    {
      switch(id)
      {
        case(SPACE & 0xff):
          mprint(" : An airless void (if seen) or unknown region (if unseen)");
          break;
        case(WALL & 0xff):
          mprint(" : An (impenetrable?) wall");
          break;
        case(OPEN_DOOR & 0xff):
          mprint(" : An open door");
          break;
        case(CLOSED_DOOR & 0xff):
          mprint(" : A closed (possibly locked) door");
          break;
        case(LAVA & 0xff):
          mprint(" : A pool of lava");
          break;
        case(HEDGE & 0xff):
          mprint(" : A dense hedge");
          break;
        case(WATER & 0xff):
          mprint(" : A deep body of water");
          break;
        case(FIRE & 0xff):
          mprint(" : A curtain of fire");
          break;
        case(TRAP & 0xff):
          mprint(" : An uncovered trap");
          break;
        case(STAIRS_UP & 0xff):
          mprint(" : A stairway leading up");
          break;
        case(STAIRS_DOWN & 0xff):
          mprint(" : A stairway leading down");
          break;
        case(FLOOR & 0xff):
          mprint(" : The dungeon floor");
          break;
        case(PORTCULLIS & 0xff):
          mprint(" : A heavy steel portcullis");
          break;
        case(ABYSS & 0xff):
          mprint(" : An entrance to the infinite abyss");
          break;
        case(PLAYER & 0xff):
          mprint(" : You, the player");
          break;
        case(CORPSE & 0xff):
          mprint(" : The remains of some creature");
          break;
        case(THING & 0xff):
          mprint(" : Some random miscellaneous object");
          break;
        case(SAFE & 0xff):
          mprint(" : A steel safe inset into the floor");
          break;
        case(RUBBLE & 0xff):
          mprint(" : A dangerous-looking pile of rubble");
          break;
        case(STATUE & 0xff):
          mprint(" : A statue");
          break;
        case(ALTAR & 0xff):
          mprint(" : A (un?)holy altar");
          break;
        case(CASH & 0xff):
          mprint(" : Bills, specie, gems: cash");
          break;
        case(PILE & 0xff):
          mprint(" : A pile of objects");
          break;
        case(FOOD & 0xff):
          mprint(" : Something edible");
          break;
        case(WEAPON & 0xff):
          mprint(" : Some kind of weapon");
          break;
        case(MISSILEWEAPON & 0xff):
          mprint(" : Some kind of missile weapon");
          break;
        case(SCROLL & 0xff):
          mprint(" : Something readable");
          break;
        case(POTION & 0xff):
          mprint(" : Something drinkable");
          break;
        case(ARMOR & 0xff):
          mprint(" : A suit of armor");
          break;
        case(SHIELD & 0xff):
          mprint(" : A shield");
          break;
        case(CLOAK & 0xff):
          mprint(" : A cloak");
          break;
        case(BOOTS & 0xff):
          mprint(" : A pair of boots");
          break;
        case(STICK & 0xff):
          mprint(" : A stick");
          break;
        case(RING & 0xff):
          mprint(" : A ring");
          break;
        case(ARTIFACT & 0xff):
          mprint(" : An artifact");
          break;
        case(CHAIR & 0xff):
          mprint(" : A chair");
          break;
        case(WHIRLWIND & 0xff):
          mprint(" : A whirlwind");
          break;
        default:
          mprint("That character is unused.");
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
    mprint("You're already in wizard mode!");
  }
  else
  {
    mprint("Really try to enter wizard mode? [yn] ");
    if(ynq() == 'y')
    {
      if(get_username() == WIZARD)
      {
        setgamestatus(CHEATED, GameStatus);
        mprint("Wizard mode set.");
      }
      else
      {
        mprint("There is a shrieking sound, as of reality being distorted.");
        strcpy(Str1, WIZARD);
        strcat(Str1, ", the Wizard of Omega appears before you....");
        if(Str1[0] >= 'a' && Str1[0] <= 'z')
        {
          Str1[0] += 'A' - 'a'; /* capitalise 1st letter */
        }
        mprint(Str1);
        mprint("'Do not meddle in the affairs of Wizards --");
        if(random_range(2))
        {
          mprint("it makes them soggy and hard to light.'");
        }
        else
        {
          mprint("for they are subtle, and swift to anger!'");
        }
      }
    }
  }
}

/* Jump, that is */
void vault()
{
  int x = Player.x, y = Player.y, jumper = 0;

  if(Player.possessions[O_BOOTS] != NULL)
  {
    if(Player.possessions[O_BOOTS]->on_equip == I_BOOTS_JUMPING)
    {
      jumper = 2;
    }
  }
  if(Player.status[IMMOBILE] > 0)
  {
    resetgamestatus(FAST_MOVE, GameStatus);
    print3("You are unable to move");
  }
  else
  {
    setgamestatus(SKIP_MONSTERS, GameStatus);
    mprint("Jump where?");
    setspot(&x, &y);
    if(!los_p(Player.x, Player.y, x, y))
    {
      print3("The way is obstructed.");
    }
    else if(Player.itemweight > Player.maxweight)
    {
      print3("You are too burdened to jump anywhere.");
    }
    else if(distance(x, y, Player.x, Player.y) > std::max(2, statmod(Player.agi) + 2) + jumper)
    {
      print3("The jump is too far for you.");
    }
    else if(Level->site[x][y].creature != NULL)
    {
      print3("You can't jump on another creature.");
    }
    else if(!p_moveable(x, y))
    {
      print3("You can't jump there.");
    }
    else
    {
      resetgamestatus(SKIP_MONSTERS, GameStatus);
      Player.x = x;
      Player.y = y;
      if((!jumper) && (random_range(30) > Player.agi))
      {
        mprint("Oops -- took a tumble.");
        setgamestatus(SKIP_PLAYER, GameStatus);
        p_damage((Player.itemweight / 250), UNSTOPPABLE, "clumsiness");
      }
      p_movefunction(Level->site[Player.x][Player.y].p_locf);
      if(Current_Environment != E_COUNTRYSIDE)
      {
        if((Level->site[Player.x][Player.y].things != NULL) && (optionp(PICKUP, Player)))
        {
          pickup();
        }
      }
    }
  }
}

std::string attack_string()
{
  if(Player.possessions[O_WEAPON_HAND] == NULL)
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
    mprint("Maneuvers Left: " + std::to_string(actionsleft));
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
          print3("No more maneuvers!");
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
          print3("No more maneuvers!");
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
          print3("Not enough maneuvers to lunge!");
        }
        else
        {
          if(Player.possessions[O_WEAPON_HAND] != NULL)
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
              print3("Can't lunge with a missile weapon!");
            }
          }
          else
          {
            print3("Can't lunge without a weapon!");
          }
        }
        break;
      case 'r':
      case 'R':
        if(actionsleft < 2)
        {
          print3("Not enough maneuvers to riposte!");
        }
        else
        {
          if(Player.possessions[O_WEAPON_HAND] != NULL)
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
              print3("Can't riposte without a thrusting weapon!");
            }
          }
          else
          {
            print3("Can't riposte without a thrusting weapon!");
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
  int             dx, dy, index = 0;
  struct monster *m;

  mprint("Pickpocketing --");

  index = getdir();

  if(index == ABORT)
  {
    setgamestatus(SKIP_MONSTERS, GameStatus);
  }
  else
  {
    dx = Dirs[0][index];
    dy = Dirs[1][index];

    if((!inbounds(Player.x + dx, Player.y + dy)) ||
       (Level->site[Player.x + dx][Player.y + dy].creature == NULL))
    {
      print3("There's nothing there to steal from!!!");
      setgamestatus(SKIP_MONSTERS, GameStatus);
    }
    else
    {
      m = Level->site[Player.x + dx][Player.y + dy].creature;
      if(m->id == GUARD)
      {
        mprint("Trying to steal from a guardsman, eh?");
        mprint("Not a clever idea.");
        if(Player.cash > 0)
        {
          mprint("As a punitive fine, the guard takes all your money.");
          Player.cash = 0;
          dataprint();
        }
        else
        {
          mprint("The guardsman places you under arrest.");
          send_to_jail();
        }
      }
      else if(m->possessions == NULL)
      {
        mprint("You couldn't find anything worth taking!");
        mprint("But you managed to annoy it...");
        m_status_set(*m, HOSTILE);
      }
      else if(Player.dex * 5 + Player.rank[THIEVES] * 20 + random_range(100) >
              random_range(100) + m->level * 20)
      {
        mprint("You successfully complete your crime!");
        mprint("You stole:");
        mprint(itemid(m->possessions->thing));
        Player.alignment--;
        gain_experience(m->level * m->level);
        gain_item(m->possessions->thing);
        m->possessions = m->possessions->next;
      }
    }
  }
}

void rename_player()
{
  setgamestatus(SKIP_MONSTERS, GameStatus);
  mprint("Rename Character: ");
  std::string name = msgscanstring();
  if(!name.empty())
  {
    if(name[0] >= 'a' && name[0] <= 'z')
    {
      name[0] += 'A' - 'a';
    }
    strcpy(Player.name, name.c_str());
  }
  print2("Henceforth, you shall be known as " + std::string(Player.name));
  dataprint();
}

void abortshadowform()
{
  setgamestatus(SKIP_MONSTERS, GameStatus);
  if(Player.status[SHADOWFORM] && (Player.status[SHADOWFORM] < 1000))
  {
    mprint("You abort your spell of Shadow Form.");
    Player.immunity[NORMAL_DAMAGE]--;
    Player.immunity[ACID]--;
    Player.immunity[THEFT]--;
    Player.immunity[INFECTION]--;
    mprint("You feel less shadowy now.");
    Player.status[SHADOWFORM] = 0;
  }
}

void tunnel()
{
  int dir, ox, oy, aux;

  mprint("Tunnel -- ");
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
      mprint("You have no success as yet.");
    }
    else if(Level->site[ox][oy].locchar != WALL)
    {
      print3("You can't tunnel through that!");
      setgamestatus(SKIP_MONSTERS, GameStatus);
    }
    else
    {
      aux = Level->site[ox][oy].aux;
      if(random_range(20) == 1)
      {
        if(Player.possessions[O_WEAPON_HAND] == NULL)
        {
          mprint("Ouch! broke a fingernail...");
          p_damage(Player.str / 6, UNSTOPPABLE, "a broken fingernail");
        }
        else if((Player.possessions[O_WEAPON_HAND]->type == THRUSTING) ||
                ((Player.possessions[O_WEAPON_HAND]->type != STRIKING) &&
                 (Player.possessions[O_WEAPON_HAND]->fragility < random_range(20))))
        {
          mprint("Clang! Uh oh...");
          (void)damage_item(Player.possessions[O_WEAPON_HAND]);
        }
        else
        {
          mprint("Your digging implement shows no sign of breaking.");
        }
      }
      if(Player.possessions[O_WEAPON_HAND] == NULL)
      {
        if((aux > 0) && ((Player.str / 3) + random_range(100) > aux))
        {
          mprint("You carve a tunnel through the stone!");
          tunnelcheck();
          Level->site[ox][oy].locchar = RUBBLE;
          Level->site[ox][oy].p_locf  = L_RUBBLE;
          lset(ox, oy, CHANGED, *Level);
        }
        else
        {
          mprint("No joy.");
        }
      }
      else if(Player.possessions[O_WEAPON_HAND]->type == THRUSTING)
      {
        if((aux > 0) && (Player.possessions[O_WEAPON_HAND]->dmg * 2 + random_range(100) > aux))
        {
          mprint("You carve a tunnel through the stone!");
          tunnelcheck();
          Level->site[ox][oy].locchar = RUBBLE;
          Level->site[ox][oy].p_locf  = L_RUBBLE;
          lset(ox, oy, CHANGED, *Level);
        }
        else
        {
          mprint("No luck.");
        }
      }
      else if((aux > 0) && (Player.possessions[O_WEAPON_HAND]->dmg + random_range(100) > aux))
      {
        mprint("You carve a tunnel through the stone!");
        tunnelcheck();
        Level->site[ox][oy].locchar = RUBBLE;
        Level->site[ox][oy].p_locf  = L_RUBBLE;
        lset(ox, oy, CHANGED, *Level);
      }
      else
      {
        mprint("You have no success as yet.");
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
      mprint("You hesitate to hunt for food in the marshy wasteland.");
      break;
    case VOLCANO:
    case CASTLE:
    case TEMPLE:
    case CAVES:
    case STARPEAK:
    case MAGIC_ISLE:
    case DRAGONLAIR:
      mprint("There is nothing alive here (or so it seems)");
      break;
    case VILLAGE:
    case CITY:
      mprint("You can find no food here; perhaps if you went inside....");
      break;
    case ROAD:
      mprint("You feel it would be a better idea to hunt off the road.");
      break;
    case CHAOS_SEA:
      mprint("Food in the Sea of Chaos? Go on!");
      break;
    case DESERT:
      mprint("You wander off into the trackless desert in search of food...");
      Time += 100;
      hourly_check();
      fertility = 10;
      break;
    case JUNGLE:
      mprint("You search the lush and verdant jungle for game....");
      Time += 100;
      hourly_check();
      fertility = 80;
      break;
    case PLAINS:
      mprint("You set off through the tall grass; the game is afoot.");
      Time += 100;
      hourly_check();
      fertility = 50;
      break;
    case TUNDRA:
      mprint("You blaze a trail through the frozen wasteland....");
      Time += 100;
      hourly_check();
      fertility = 30;
      break;
    case FOREST:
      mprint("You try to follow the many tracks through the forest loam....");
      Time += 100;
      hourly_check();
      fertility = 70;
      break;
    case MOUNTAINS:
    case PASS:
      mprint("You search the cliff walls looking for something to eat....");
      Time += 100;
      hourly_check();
      fertility = 30;
      break;
    case RIVER:
      mprint("The halcyon river is your hopeful food source...");
      Time += 100;
      hourly_check();
      fertility = 80;
      break;
  }
  if(((Date % 360 < 60) || (Date % 360 > 300)) && (terrain != DESERT) && (terrain != JUNGLE))
  {
    mprint("The cold weather impedes your hunt....");
    fertility = fertility / 2;
  }
  if(fertility > random_range(100))
  {
    mprint("You have an encounter...");
    change_environment(E_TACTICAL_MAP);
  }
  else
  {
    mprint("Your hunt is fruitless.");
  }
}

void dismount_steed()
{
  pml ml;
  if(!gamestatusp(MOUNTED, GameStatus))
  {
    print3("You're on foot already!");
  }
  else if(Current_Environment == E_COUNTRYSIDE)
  {
    mprint("If you leave your steed here he will wander away!");
    mprint("Do it anyway? [yn] ");
    if(ynq() == 'y')
    {
      resetgamestatus(MOUNTED, GameStatus);
    }
  }
  else
  {
    resetgamestatus(MOUNTED, GameStatus);
    ;
    ml                                       = ((pml)checkmalloc(sizeof(mltype)));
    ml->m                                    = ((pmt)checkmalloc(sizeof(montype)));
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
    print3("This command only works in the city!");
    setgamestatus(SKIP_MONSTERS, GameStatus);
  }
  else if(Player.status[IMMOBILE] > 0)
  {
    print3("You can't even move!");
  }
  else if(hostilemonstersnear())
  {
    setgamestatus(SKIP_MONSTERS, GameStatus);
    print3("You can't move this way with hostile monsters around!");
  }
  else if(Level->site[Player.x][Player.y].aux == NOCITYMOVE)
  {
    print3("You can't use the 'M' command from this location.");
  }
  else
  {
    print1("Move to which establishment [? for help, ESCAPE to quit]");
    site = parsecitysite();
    if(site != ABORT)
    {
      mprint("You're on your way...");
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
      mprint("Made it!");
      drawvision(Player.x, Player.y);
      p_movefunction(Level->site[x][y].p_locf);
    }
  }
}

void frobgamestatus()
{
  char response;
  long num;
  mprint("Set or Reset or Forget it [s,r,ESCAPE]:");
  do
  {
    response = (char)mcigetc();
  } while((response != 'r') && (response != 's') && (response != ESCAPE));
  if(response != ESCAPE)
  {
    mprint("Enter log2 of flag:");
    num = (int)parsenum();
    if(num > -1)
    {
      num = pow2(num);
      if(num == CHEATED)
      {
        mprint("Can't alter Wizard flag.");
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
      mprint("Done....");
    }
  }
}
