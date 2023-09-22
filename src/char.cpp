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

/* char.c */
/* Player generation */

#include "glob.h"
#include "scrolling_buffer.hpp"

#include <algorithm>
#include <cstring>
#include <format>
#include <string>
#include <string_view>

extern scrolling_buffer message_buffer;
extern void enable_attr(WINDOW *, attr_t);
extern void queue_message(const std::string &message);
extern void expand_message_window();
extern void shrink_message_window();

std::string get_username()
{
  char *env = getenv("USERNAME");
  if(env && strlen(env) > 0)
  {
    return env;
  }
  env = getenv("USER");
  if(env && strlen(env) > 0)
  {
    return env;
  }
  return "pcuser";
}

std::string_view get_home_path()
{
  char *home_path = getenv("HOME");
  if(home_path)
  {
    return home_path;
  }
  else
  {
    return ".";
  }
}

/* set player to begin with */
void initplayer()
{
  int   i;
  int   oldchar = false;
  FILE *fd;
  std::string username = get_username();
  strcpy(Player.name, username.c_str());

  if(Player.name[0] >= 'a' && Player.name[0] <= 'z')
  {
    Player.name[0] += 'A' - 'a'; /* capitalise 1st letter */
  }
  Player.itemweight = 0;
  Player.food       = 36;
  Player.packptr    = 0;
  Behavior          = -1;
  Player.options    = 0;
  for(i = 0; i < MAXITEMS; i++)
  {
    Player.possessions[i] = NULL;
  }
  for(i = 0; i < MAXPACK; i++)
  {
    Player.pack[i] = NULL;
  }
  for(i = 0; i < NUMIMMUNITIES; i++)
  {
    Player.immunity[i] = 0;
  }
  for(i = 0; i < NUMSTATI; i++)
  {
    Player.status[i] = 0;
  }
  for(i = 0; i < NUMRANKS; i++)
  {
    Player.rank[i]    = 0;
    Player.guildxp[i] = 0;
  }
  Player.patron    = 0;
  Player.alignment = 0;
  Player.cash      = 250;
  expand_message_window();
  if((fd = omegarc_check()) != NULL)
  {
    fread((char *)&i, sizeof(int), 1, fd);
    if(i != VERSION)
    {
      print1("Out of date .omegarc! Make another!");
    }
    else
    {
      oldchar = true;
      fread((char *)&Player, sizeof(Player), 1, fd);
      fread((char *)&Searchnum, sizeof(int), 1, fd);
      fread((char *)&Verbosity, sizeof(char), 1, fd);
      strcpy(Player.name, username.c_str());
      if(Player.name[0] >= 'a' && Player.name[0] <= 'z')
      {
        Player.name[0] += 'A' - 'a'; /* capitalise 1st letter */
      }
    }
    fclose(fd);
  }
  if(!oldchar)
  {
    optionset(RUNSTOP, Player);
    optionset(CONFIRM, Player);
    optionset(SHOW_COLOUR, Player);
    initstats();
  }
  Searchnum = std::max(1, std::min(9, Searchnum));
  Player.hp = Player.maxhp = Player.maxcon;
  Player.mana = Player.maxmana = calcmana();
  Player.click                 = 1;
  strcpy(Player.meleestr, "ACBC");
  calc_melee();
  ScreenOffset = -1000; /* to force a redraw */
  shrink_message_window();
}

FILE *omegarc_check()
{
  FILE *fd = fopen(std::format("{}/.omegarc", get_home_path()).c_str(), "r");
  if(fd)
  {
    queue_message("Use .omegarc in home directory? [yn] ");
    if(ynq2() != 'y')
    {
      fclose(fd);
      fd = nullptr;
    }
  }
  return fd;
}

void initstats()
{
  char response;
  print1("Do you want to run a character [c] or play yourself [p]?");
  do
  {
    response = (char)mcigetc();
  } while((response != 'c') && (response != 'p'));
  if(response == 'c')
  {
    omegan_character_stats();
  }
  else
  {
    user_character_stats();
    user_intro();
    print1("Do you want to save this set-up to .omegarc in your home "
           "directory? [yn] ");
    if(ynq1() == 'y')
    {
      shrink_message_window();
      save_omegarc();
    }
  }
  xredraw();
}

void save_omegarc()
{
  int   i = VERSION;
  FILE *fd = fopen(std::format("{}/.omegarc", get_home_path()).c_str(), "w");
  if(!fd)
  {
    print1("Sorry, couldn't save .omegarc for some reason.");
  }
  else
  {
    fwrite((char *)&i, sizeof(int), 1, fd);
    print1("First, set options.");
    setoptions();
    fwrite((char *)&Player, sizeof(Player), 1, fd);
    fwrite((char *)&Searchnum, sizeof(int), 1, fd);
    fwrite((char *)&Verbosity, sizeof(char), 1, fd);
    fclose(fd);
  }
}

long calcmana()
{
  return (Player.pow * (long)(Player.level + 1));
}

/*  npcbehavior digits 1234

4 : alignment (LAWFUL,CHAOTIC, or NEUTRAL)
3 : primary combat action (melee,missile,spell,thief,flight,1..5)
2 : competence at 4 (0..9, 0 = incompetent, 9 = masterful)
1 : conversation mode

status : 1 = dead, 2 = saved, 3 = retired, 4 = still playing
*/
int fixnpc(int status)
{
  int  npcbehavior = 0;
  char response;
  if(status == 1)
  { /* player is dead, all undead are chaotic */
    npcbehavior += CHAOTIC;
    npcbehavior += 10; /* melee */
    npcbehavior += 100 * std::min(9, static_cast<int>((Player.level / 3)));
    npcbehavior += 1000; /* threaten */
  }
  else if(Behavior >= 0)
  {
    npcbehavior = Behavior;
  }
  else
  {
    menuclear();
    menuprint("NPC Behavior Determination Module\n\n");
    menuprint("Your overall NPC behavior is:");
    if(Player.alignment < -10)
    {
      npcbehavior += CHAOTIC;
      menuprint("\n\n CHAOTIC");
    }
    else if(Player.alignment > 10)
    {
      npcbehavior += LAWFUL;
      menuprint("\n\n LAWFUL");
    }
    else
    {
      npcbehavior += NEUTRAL;
      menuprint("\n\n NEUTRAL");
    }
    menuprint("\n\n1: hand-to-hand combat");
    menuprint("\n2: missile combat");
    menuprint("\n3: spellcasting");
    menuprint("\n4: thieving");
    menuprint("\n5: escape");
    menuprint("\n\nEnter NPC response to combat: ");
    showmenu();
    response = '0';
    while((response != '1') && (response != '2') && (response != '3') && (response != '4') &&
          (response != '5'))
    {
      response = menugetc();
    }
    menuaddch(response);
    npcbehavior += 10 * (response - '0');
    npcbehavior += 100 * competence_check(response - '0');
    response = '0';
    menuclear();
    menuprint("1: threaten");
    menuprint("\n2: greet");
    menuprint("\n3: aid");
    menuprint("\n4: beg");
    menuprint("\n5: silence");
    menuprint("\n\nEnter NPC response to conversation: ");
    showmenu();
    while((response != '1') && (response != '2') && (response != '3') && (response != '4') &&
          (response != '5'))
    {
      response = menugetc();
    }
    menuaddch(response);
    npcbehavior += 1000 * (response - '0');
    xredraw();
  }
  Behavior = npcbehavior;
  return (npcbehavior);
}

/* estimates on a 0..9 scale how good a player is at something */
int competence_check(int attack)
{
  int ability = 0;
  switch(attack)
  {
    case 1: /* melee */
      ability += statmod(Player.str);
      [[fallthrough]];
    case 2: /* missle */
      ability += statmod(Player.dex);
      ability += Player.rank[LEGION];
      ability += static_cast<int>(Player.dmg/10 - 1);
      break;
    case 3: /* spellcasting */
      ability += statmod(Player.iq);
      ability += statmod(Player.pow);
      ability += Player.rank[CIRCLE];
      ability += Player.rank[COLLEGE];
      ability += Player.rank[PRIEST];
      break;
    case 4: /* thieving */
      ability += statmod(Player.dex);
      ability += statmod(Player.agi);
      ability += Player.rank[THIEVES];
      break;
    case 5: /* escape */
      ability += 2 * statmod(Player.agi);
      break;
  }
  ability += static_cast<int>((Player.level / 5));
  if(ability < 0)
  {
    ability = 0;
  }
  if(ability > 9)
  {
    ability = 9;
  }
  return (ability);
}

void user_character_stats()
{
  int iqpts = 0, numints = 0, ok, agipts = 0, dexpts = 0, powpts = 0, conpts = 0;
  message_buffer.receive("OK, now try to answer the following questions honestly:");
  message_buffer.receive("How many pounds can you bench press? ");
  int num = static_cast<int>(parsenum());
  if(num < 30)
  {
    Player.str = Player.maxstr = 3;
  }
  else if(num < 90)
  {
    Player.str = Player.maxstr = num / 10;
  }
  else
  {
    Player.str = Player.maxstr = 9 + ((num - 120) / 30);
  }
  if(Player.str > 18)
  {
    message_buffer.receive("Even if it's true, I don't believe it.");
    Player.str = Player.maxstr = 18;
  }

  message_buffer.receive("Took an official IQ test? [yn] ", true);
  if(ynq1() == 'y')
  {
    message_buffer.receive("So, whadja get? ", true);
    num = static_cast<int>(parsenum() / 10);
    if(num > 18)
    {
      message_buffer.receive("Even if it's true, I don't believe it.", true);
      num = 18;
    }
    iqpts += num;
    numints++;
  }

  message_buffer.receive("Took Undergraduate entrance exams? [yn] ", true);
  if(ynq1() == 'y')
  {
    do
    {
      message_buffer.receive("So, what percentile? ", true);
      num = static_cast<int>(parsenum());
      ok  = (num < 100);
      if(!ok)
      {
        message_buffer.receive("That's impossible!", true);
      }
    } while(!ok);
    iqpts += (num - 49) * 9 / 50 + 9;
    numints++;
  }
  message_buffer.receive("Took Graduate entrance exams? [yn] ", true);
  if(ynq1() == 'y')
  {
    do
    {
      message_buffer.receive("So, what percentile? ", true);
      num = static_cast<int>(parsenum());
      ok  = (num < 100);
      if(!ok)
      {
        message_buffer.receive("That's impossible!", true);
      }
    } while(!ok);
    iqpts += (num - 49) * 9 / 50 + 9;
    numints++;
  }

  if(numints == 0)
  {
    message_buffer.receive("Pretty dumb, aren't you? [yn] ", true);
    if(ynq1() == 'y')
    {
      Player.iq = random_range(3) + 3;
      message_buffer.receive("I thought so....", true);
    }
    else
    {
      Player.iq = random_range(6) + 8;
      message_buffer.receive("Well, not *that* dumb.", true);
    }
  }
  else
  {
    Player.iq = iqpts / numints;
  }
  Player.maxiq = Player.iq;
  agipts       = 0;
  message_buffer.receive("Can you dance? [yn] ", true);
  if(ynq1() == 'y')
  {
    agipts++;
    message_buffer.receive("Well? [yn] ", true);
    if(ynq1() == 'y')
    {
      agipts += 2;
    }
  }
  message_buffer.receive("Do you have training in a martial art or gymnastics? [yn] ", true);
  if(ynq1() == 'y')
  {
    agipts += 2;
    message_buffer.receive("Do you have dan rank or equivalent? [yn] ", true);
    if(ynq2() == 'y')
    {
      agipts += 4;
    }
  }
  message_buffer.receive("Do you play some field sport? [yn] ", true);
  if(ynq1() == 'y')
  {
    agipts++;
    message_buffer.receive("Are you good? [yn] ", true);
    if(ynq1() == 'y')
    {
      agipts++;
    }
  }
  message_buffer.receive("Do you cave, mountaineer, etc.? [yn] ", true);
  if(ynq1() == 'y')
  {
    agipts += 3;
  }
  message_buffer.receive("Do you skate or ski? [yn] ", true);
  if(ynq1() == 'y')
  {
    agipts += 2;
    message_buffer.receive("Well? [yn] ", true);
    if(ynq1() == 'y')
    {
      agipts += 2;
    }
  }
  message_buffer.receive("Are you physically handicapped? [yn] ", true);
  if(ynq1() == 'y')
  {
    agipts -= 4;
  }
  message_buffer.receive("Are you accident prone? [yn] ", true);
  if(ynq1() == 'y')
  {
    agipts -= 4;
  }
  message_buffer.receive("Can you use a bicycle? [yn] ", true);
  if(ynq1() != 'y')
  {
    agipts -= 4;
  }
  Player.agi = Player.maxagi = 9 + agipts / 2;
  message_buffer.receive("Do you play video games? [yn] ", true);
  if(ynq1() == 'y')
  {
    dexpts += 2;
    message_buffer.receive("Do you get high scores? [yn] ", true);
    if(ynq2() == 'y')
    {
      dexpts += 4;
    }
  }
  message_buffer.receive("Are you an archer, fencer, or marksman? [yn] ", true);
  if(ynq1() == 'y')
  {
    dexpts += 2;
    message_buffer.receive("A good one? [yn] ", true);
    if(ynq2() == 'y')
    {
      dexpts += 4;
    }
  }
  message_buffer.receive("Have you ever picked a lock? [yn] ", true);
  if(ynq1() == 'y')
  {
    dexpts += 2;
    message_buffer.receive("Really. Well, the police are being notified.", true);
  }
  message_buffer.receive("What's your typing speed (words per minute) ", true);
  num = static_cast<int>(parsenum());
  if(num > 125)
  {
    message_buffer.receive("Tell me another one....", true);
    num = 125;
  }
  dexpts += num / 25;
  message_buffer.receive("Hold your arm out. Tense your fist. Hand shaking? [yn] ", true);
  if(ynq1() == 'y')
  {
    dexpts -= 3;
  }
  message_buffer.receive("Ambidextrous, are you? [yn] ", true);
  if(ynq1() == 'y')
  {
    dexpts += 4;
  }
  message_buffer.receive("Can you cut a deck of cards with one hand? [yn] ", true);
  if(ynq1() == 'y')
  {
    dexpts += 2;
  }
  message_buffer.receive("Can you tie your shoes blindfolded? [yn] ", true);
  if(ynq1() != 'y')
  {
    dexpts -= 3;
  }
  Player.dex = Player.maxdex = 6 + dexpts / 2;
  message_buffer.receive("Do you ever get colds? [yn] ", true);
  if(ynq1() != 'y')
  {
    conpts += 4;
  }
  else
  {
    message_buffer.receive("Frequently? [yn] ", true);
    if(ynq1() == 'y')
    {
      conpts -= 4;
    }
  }
  message_buffer.receive("Had any serious accident or illness this year? [yn] ", true);
  if(ynq1() == 'y')
  {
    conpts -= 4;
  }
  else
  {
    conpts += 4;
  }
  message_buffer.receive("Have a chronic disease? [yn] ", true);
  if(ynq1() == 'y')
  {
    conpts -= 4;
  }
  message_buffer.receive("Overweight or underweight by more than 20 percent? [yn] ", true);
  if(ynq1() == 'y')
  {
    conpts -= 2;
  }
  message_buffer.receive("High Blood Pressure? [yn] ", true);
  if(ynq1() == 'y')
  {
    conpts -= 2;
  }
  message_buffer.receive("Smoke? [yn] ", true);
  if(ynq1() == 'y')
  {
    conpts -= 3;
  }
  message_buffer.receive("Take aerobics classes? [yn] ", true);
  if(ynq1() == 'y')
  {
    conpts += 2;
  }
  message_buffer.receive("How many miles can you run? ", true);
  num = static_cast<int>(parsenum());
  if(num > 25)
  {
    message_buffer.receive("Right. Sure. Give me a break.", true);
    conpts += 8;
  }
  else if(num < 1)
  {
    conpts -= 3;
  }
  else if(num < 5)
  {
    conpts += 2;
  }
  else if(num < 10)
  {
    conpts += 4;
  }
  else
  {
    conpts += 8;
  }
  Player.con = Player.maxcon = 12 + conpts / 3;
  message_buffer.receive("Do animals react oddly to your presence? [yn] ", true);
  if(ynq1() == 'y')
  {
    message_buffer.receive("How curious that must be.", true);
    powpts += 2;
  }
  message_buffer.receive("Can you see auras? [yn] ", true);
  if(ynq1() == 'y')
  {
    message_buffer.receive("How strange.", true);
    powpts += 3;
  }
  message_buffer.receive("Ever have an out-of-body experience? [yn] ", true);
  if(ynq1() == 'y')
  {
    message_buffer.receive("Wow, man. Fly the friendly skies....", true);
    powpts += 3;
  }
  message_buffer.receive("Did you ever cast a spell? [yn] ", true);
  if(ynq1() == 'y')
  {
    powpts += 3;
    message_buffer.receive("Did it work? [yn] ", true);
    if(ynq1() == 'y')
    {
      powpts += 7;
      message_buffer.receive("Sure it did.", true);
    }
  }
  message_buffer.receive("Do you have ESP? [yn] ", true);
  if(ynq1() == 'y')
  {
    powpts += 3;
    message_buffer.receive("Somehow, I knew you were going to say that.", true);
  }
  message_buffer.receive("Do you have PK? [yn] ", true);
  if(ynq1() == 'y')
  {
    powpts += 6;
    message_buffer.receive("I can't tell you how much that moves me.", true);
  }
  message_buffer.receive("Do you believe in ghosts? [yn] ", true);
  if(ynq1() == 'y')
  {
    powpts += 2;
    message_buffer.receive("I do! I do! I do believe in ghosts!", true);
  }
  message_buffer.receive("Are you Irish? [yn] ", true);
  if(ynq1() == 'y')
  {
    powpts += 2;
    message_buffer.receive("Is that blarney or what?", true);
  }
  Player.pow = Player.maxpow = 3 + powpts / 2;
}

void omegan_character_stats()
{
  int stat_num = 0;
  int stat_pool = 10;
  int stats[6] = { 10, 10, 10, 10, 10, 10 };
  std::string stat_names[6] = {"Str", "Dex", "Con", "Agi", "Int", "Pow"};
  for(bool character_creation_done = false; !character_creation_done;)
  {

    erase();
    enable_attr(stdscr, A_NORMAL);
    mvaddstr(0, 0, "Add or remove points from your stats using the point pool. Press ENTER to confirm.");
    mvaddstr(1, 0, "Point Pool: ");
    if(stat_pool > 0)
    {
      enable_attr(stdscr, CLR(GREEN));
    }
    printw("%d", stat_pool);
    for(int i = 0; i < 6; ++i)
    {
      if(i == stat_num)
      {
        enable_attr(stdscr, CLR(GREY));
        mvaddstr(3+i, 0, ">>");
        enable_attr(stdscr, CLR(WHITE));
      }
      else
      {
        enable_attr(stdscr, CLR(GREY));
      }
      mvprintw(3+i, 3, "%s: ", stat_names[i].c_str());
      if(stats[i] < 10)
      {
        enable_attr(stdscr, CLR(RED));
      }
      else if(stats[i] > 10)
      {
        enable_attr(stdscr, CLR(GREEN));
      }
      else
      {
        enable_attr(stdscr, A_BOLD);
      }
      printw("%d", stats[i]);
    }
    enable_attr(stdscr, A_NORMAL);

    switch(getch())
    {
      case KEY_ENTER:
      case '\r':
      case '\n':
        character_creation_done = true;
        break;
      case KEY_DOWN:
      case 'j':
        if(stat_num < 5)
        {
          ++stat_num;
        }
        break;
      case KEY_UP:
      case 'k':
        if(stat_num > 0)
        {
          --stat_num;
        }
        break;
      case KEY_LEFT:
      case '<':
      case 'h':
        if(stats[stat_num] > 4)
        {
          --stats[stat_num];
          ++stat_pool;
        }
        break;
      case KEY_RIGHT:
      case '>':
      case 'l':
        if(stats[stat_num] < 18 && stat_pool > 0)
        {
          ++stats[stat_num];
          --stat_pool;
        }
        break;
    }
  }
  erase();
  Player.str = Player.maxstr = stats[0];
  Player.dex = Player.maxdex = stats[1];
  Player.con = Player.maxcon = stats[2];
  Player.agi = Player.maxagi = stats[3];
  Player.iq = Player.maxiq = stats[4];
  Player.pow = Player.maxpow = stats[5];
  Player.cash = 250;

  message_buffer.receive("Please enter your character's name: ");
  strcpy(Player.name, msgscanstring().c_str());
  if(Player.name[0] >= 'a' && Player.name[0] <= 'z')
  {
    Player.name[0] += 'A' - 'a'; // capitalise 1st letter
  }
}
