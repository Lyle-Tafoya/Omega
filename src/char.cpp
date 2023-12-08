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

// char.cpp
// Player generation

#include "file.h"
#include "glob.h"
#include "scr.h"
#include "scrolling_buffer.hpp"

#include <algorithm>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <string>

extern bool read_omegarc();
extern void save_omegarc();
extern void restore_player(std::ifstream &save_file, player &p);
bool game_restore(const std::filesystem::path &save_file_path);
extern WINDOW *menu_window;

std::string get_username()
{
  char *env = getenv("USERNAME");
  if(env && env[0] != '\0')
  {
    return env;
  }
  env = getenv("USER");
  if(env && env[0] != '\0')
  {
    return env;
  }
  return "pcuser";
}

std::string get_home_path()
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

std::ifstream omegarc_check()
{
  std::string omegarc_filename = std::format("{}/.omegarc", get_home_path());
  std::ifstream omegarc_file(omegarc_filename, std::ios::binary);
  if(omegarc_file.is_open())
  {
    queue_message("Use .omegarc in home directory? [yn] ");
    if(ynq() != 'y')
    {
      omegarc_file.close();
    }
  }
  return omegarc_file;
}

std::string version_string(int version)
{
  int bugfix_version = version % 100;
  int minor_version  = version / 100;
  int major_version  = version / 10000;
  return std::format("{}.{}.{}", major_version, minor_version, bugfix_version);
}

void user_character_stats()
{
  std::filesystem::path omegarc_path{std::format("{}/.omegarc", get_home_path())};
  if(std::filesystem::exists(omegarc_path))
  {
    queue_message("Would you like to load saved responses from your .omegarc? ");
    if(ynq() == 'y')
    {
      if(read_omegarc())
      {
        return;
      }
    }
  }
  int iqpts = 0, numints = 0, ok, agipts = 0, dexpts = 0, powpts = 0, conpts = 0;
  queue_message("OK, now try to answer the following questions honestly:");
  queue_message("How many pounds can you bench press? ");
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
    queue_message("Even if it's true, I don't believe it.");
    Player.str = Player.maxstr = 18;
  }

  queue_message("Took an official IQ test? [yn] ", true);
  if(ynq() == 'y')
  {
    queue_message("So, whadja get? ", true);
    num = static_cast<int>(parsenum() / 10);
    if(num > 18)
    {
      queue_message("Even if it's true, I don't believe it.", true);
      num = 18;
    }
    iqpts += num;
    numints++;
  }

  queue_message("Took Undergraduate entrance exams? [yn] ", true);
  if(ynq() == 'y')
  {
    do
    {
      queue_message("So, what percentile? ", true);
      num = static_cast<int>(parsenum());
      ok  = (num < 100);
      if(!ok)
      {
        queue_message("That's impossible!", true);
      }
    } while(!ok);
    iqpts += (num - 49) * 9 / 50 + 9;
    numints++;
  }
  queue_message("Took Graduate entrance exams? [yn] ", true);
  if(ynq() == 'y')
  {
    do
    {
      queue_message("So, what percentile? ", true);
      num = static_cast<int>(parsenum());
      ok  = (num < 100);
      if(!ok)
      {
        queue_message("That's impossible!", true);
      }
    } while(!ok);
    iqpts += (num - 49) * 9 / 50 + 9;
    numints++;
  }

  if(numints == 0)
  {
    queue_message("Pretty dumb, aren't you? [yn] ", true);
    if(ynq() == 'y')
    {
      Player.iq = random_range(3) + 3;
      queue_message("I thought so....", true);
    }
    else
    {
      Player.iq = random_range(6) + 8;
      queue_message("Well, not *that* dumb.", true);
    }
  }
  else
  {
    Player.iq = iqpts / numints;
  }
  Player.maxiq = Player.iq;
  agipts       = 0;
  queue_message("Can you dance? [yn] ", true);
  if(ynq() == 'y')
  {
    agipts++;
    queue_message("Well? [yn] ", true);
    if(ynq() == 'y')
    {
      agipts += 2;
    }
  }
  queue_message("Do you have training in a martial art or gymnastics? [yn] ", true);
  if(ynq() == 'y')
  {
    agipts += 2;
    queue_message("Do you have dan rank or equivalent? [yn] ", true);
    if(ynq() == 'y')
    {
      agipts += 4;
    }
  }
  queue_message("Do you play some field sport? [yn] ", true);
  if(ynq() == 'y')
  {
    agipts++;
    queue_message("Are you good? [yn] ", true);
    if(ynq() == 'y')
    {
      agipts++;
    }
  }
  queue_message("Do you cave, mountaineer, etc.? [yn] ", true);
  if(ynq() == 'y')
  {
    agipts += 3;
  }
  queue_message("Do you skate or ski? [yn] ", true);
  if(ynq() == 'y')
  {
    agipts += 2;
    queue_message("Well? [yn] ", true);
    if(ynq() == 'y')
    {
      agipts += 2;
    }
  }
  queue_message("Are you physically handicapped? [yn] ", true);
  if(ynq() == 'y')
  {
    agipts -= 4;
  }
  queue_message("Are you accident prone? [yn] ", true);
  if(ynq() == 'y')
  {
    agipts -= 4;
  }
  queue_message("Can you use a bicycle? [yn] ", true);
  if(ynq() != 'y')
  {
    agipts -= 4;
  }
  Player.agi = Player.maxagi = 9 + agipts / 2;
  queue_message("Do you play video games? [yn] ", true);
  if(ynq() == 'y')
  {
    dexpts += 2;
    queue_message("Do you get high scores? [yn] ", true);
    if(ynq() == 'y')
    {
      dexpts += 4;
    }
  }
  queue_message("Are you an archer, fencer, or marksman? [yn] ", true);
  if(ynq() == 'y')
  {
    dexpts += 2;
    queue_message("A good one? [yn] ", true);
    if(ynq() == 'y')
    {
      dexpts += 4;
    }
  }
  queue_message("Have you ever picked a lock? [yn] ", true);
  if(ynq() == 'y')
  {
    dexpts += 2;
    queue_message("Really. Well, the police are being notified.", true);
  }
  queue_message("What's your typing speed (words per minute) ", true);
  num = static_cast<int>(parsenum());
  if(num > 125)
  {
    queue_message("Tell me another one....", true);
    num = 125;
  }
  dexpts += num / 25;
  queue_message("Hold your arm out. Tense your fist. Hand shaking? [yn] ", true);
  if(ynq() == 'y')
  {
    dexpts -= 3;
  }
  queue_message("Ambidextrous, are you? [yn] ", true);
  if(ynq() == 'y')
  {
    dexpts += 4;
  }
  queue_message("Can you cut a deck of cards with one hand? [yn] ", true);
  if(ynq() == 'y')
  {
    dexpts += 2;
  }
  queue_message("Can you tie your shoes blindfolded? [yn] ", true);
  if(ynq() != 'y')
  {
    dexpts -= 3;
  }
  Player.dex = Player.maxdex = 6 + dexpts / 2;
  queue_message("Do you ever get colds? [yn] ", true);
  if(ynq() != 'y')
  {
    conpts += 4;
  }
  else
  {
    queue_message("Frequently? [yn] ", true);
    if(ynq() == 'y')
    {
      conpts -= 4;
    }
  }
  queue_message("Had any serious accident or illness this year? [yn] ", true);
  if(ynq() == 'y')
  {
    conpts -= 4;
  }
  else
  {
    conpts += 4;
  }
  queue_message("Have a chronic disease? [yn] ", true);
  if(ynq() == 'y')
  {
    conpts -= 4;
  }
  queue_message("Overweight or underweight by more than 20 percent? [yn] ", true);
  if(ynq() == 'y')
  {
    conpts -= 2;
  }
  queue_message("High Blood Pressure? [yn] ", true);
  if(ynq() == 'y')
  {
    conpts -= 2;
  }
  queue_message("Smoke? [yn] ", true);
  if(ynq() == 'y')
  {
    conpts -= 3;
  }
  queue_message("Take aerobics classes? [yn] ", true);
  if(ynq() == 'y')
  {
    conpts += 2;
  }
  queue_message("How many miles can you run? ", true);
  num = static_cast<int>(parsenum());
  if(num > 25)
  {
    queue_message("Right. Sure. Give me a break.", true);
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
  queue_message("Do animals react oddly to your presence? [yn] ", true);
  if(ynq() == 'y')
  {
    queue_message("How curious that must be.", true);
    powpts += 2;
  }
  queue_message("Can you see auras? [yn] ", true);
  if(ynq() == 'y')
  {
    queue_message("How strange.", true);
    powpts += 3;
  }
  queue_message("Ever have an out-of-body experience? [yn] ", true);
  if(ynq() == 'y')
  {
    queue_message("Wow, man. Fly the friendly skies....", true);
    powpts += 3;
  }
  queue_message("Did you ever cast a spell? [yn] ", true);
  if(ynq() == 'y')
  {
    powpts += 3;
    queue_message("Did it work? [yn] ", true);
    if(ynq() == 'y')
    {
      powpts += 7;
      queue_message("Sure it did.", true);
    }
  }
  queue_message("Do you have ESP? [yn] ", true);
  if(ynq() == 'y')
  {
    powpts += 3;
    queue_message("Somehow, I knew you were going to say that.", true);
  }
  queue_message("Do you have PK? [yn] ", true);
  if(ynq() == 'y')
  {
    powpts += 6;
    queue_message("I can't tell you how much that moves me.", true);
  }
  queue_message("Do you believe in ghosts? [yn] ", true);
  if(ynq() == 'y')
  {
    powpts += 2;
    queue_message("I do! I do! I do believe in ghosts!", true);
  }
  queue_message("Are you Irish? [yn] ", true);
  if(ynq() == 'y')
  {
    powpts += 2;
    queue_message("Is that blarney or what?", true);
  }
  Player.pow = Player.maxpow = 3 + powpts / 2;

  queue_message("Do you want to save this set-up to .omegarc in your home directory? [yn] ");
  if(ynq() == 'y')
  {
    save_omegarc();
  }
}

void omegan_character_stats()
{
  int stat_num              = 0;
  int stat_pool             = 10;
  int stats[6]              = {10, 10, 10, 10, 10, 10};
  std::string stat_names[6] = {"Str", "Dex", "Con", "Agi", "Int", "Pow"};
  for(bool character_creation_done = false; !character_creation_done;)
  {
    werase(menu_window);
    Player.str = Player.maxstr = stats[0];
    Player.dex = Player.maxdex = stats[1];
    Player.con = Player.maxcon = stats[2];
    Player.agi = Player.maxagi = stats[3];
    Player.iq = Player.maxiq = stats[4];
    Player.pow = Player.maxpow = stats[5];
    Player.cash                = 250;
    Player.hp = Player.maxhp = Player.maxcon;
    Player.mana = Player.maxmana = calcmana();
    calc_melee();

    enable_attr(menu_window, A_NORMAL);
    mvwaddstr(
      menu_window, 0, 0,
      "Add or remove points from your stats using the point pool.\n"
      "Point Pool: "
    );
    if(stat_pool > 0)
    {
      enable_attr(menu_window, CLR(GREEN));
    }
    wprintw(menu_window, "%d", stat_pool);
    for(int i = 0; i < 6; ++i)
    {
      if(i == stat_num)
      {
        enable_attr(menu_window, CLR(GREY));
        mvwaddstr(menu_window, 3 + i, 0, ">>");
        enable_attr(menu_window, CLR(WHITE));
      }
      else
      {
        enable_attr(menu_window, CLR(GREY));
      }
      mvwprintw(menu_window, 3 + i, 3, "%s: ", stat_names[i].c_str());
      if(stats[i] < 10)
      {
        enable_attr(menu_window, CLR(RED));
      }
      else if(stats[i] > 10)
      {
        enable_attr(menu_window, CLR(GREEN));
      }
      else
      {
        enable_attr(menu_window, A_BOLD);
      }
      wprintw(menu_window, "%d", stats[i]);
    }
    enable_attr(menu_window, A_NORMAL);
    mvwaddstr(menu_window, 10, 0, "Press ENTER to confirm");

    switch(wgetch(menu_window))
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
}

// set player to begin with
void initplayer(bool play_yourself = false)
{
  Player.itemweight = 0;
  Player.food       = 36;
  Behavior          = -1;
  Player.options    = 0;
  for(int i = 0; i < NUMIMMUNITIES; ++i)
  {
    Player.immunity[i] = 0;
  }
  for(int i = 0; i < NUMSTATI; ++i)
  {
    Player.status[i] = 0;
  }
  for(int i = 0; i < NUMRANKS; ++i)
  {
    Player.rank[i]    = 0;
    Player.guildxp[i] = 0;
  }
  Player.patron    = 0;
  Player.alignment = 0;
  Player.cash      = 250;

  optionset(RUNSTOP, Player);
  optionset(CONFIRM, Player);
  optionset(SHOW_COLOUR, Player);
  optionset(MOUSE_ENABLED, Player);

  if(play_yourself)
  {
    expand_message_window();
    user_character_stats();
    shrink_message_window();
  }
  else
  {
    omegan_character_stats();
  }
  Searchnum = std::max(1, std::min(9, Searchnum));
  Player.hp = Player.maxhp = Player.maxcon;
  Player.mana = Player.maxmana = calcmana();
  Player.click                 = 1;
  Player.meleestr              = "ACBC";
  calc_melee();
  ScreenOffset = -1000; /* to force a redraw */
}

// monsters initialized in game_restore if game is being restored
// items initialized in game_restore if game is being restored
void init_game(bool play_yourself = false)
{
  read_scores();
  inititem(true);
  Date    = random_range(360);
  Phase   = random_range(24);
  MazeNum = random_range(4) + 1;
  initplayer(play_yourself);
  init_world();
  xredraw();
  queue_message("'?' for help or commandlist, 'Q' to quit.");
}

bool title_menu()
{
  const std::string default_name       = Player.name;
  std::filesystem::path save_directory = std::format("{}saves/{}", Omegalib, get_username());

  size_t longest_line_length = 16;
  std::vector<std::filesystem::path> save_file_paths;
  std::vector<std::string> save_file_lines;
  if(std::filesystem::is_directory(save_directory))
  {
    for(const auto &entry : std::filesystem::directory_iterator{save_directory})
    {
      const std::string &file_path = entry.path().string();
      if(entry.is_regular_file() && file_path.ends_with(".sav"))
      {
        std::ifstream save_file{file_path, std::ios::binary};
        if(save_file)
        {
          int save_file_version;
          file_read(save_file, save_file_version);
          int game_version;
          file_read(save_file, game_version);

          std::string::size_type size;
          file_read(save_file, size);
          std::string name;
          name.resize(size);
          save_file.read(&name[0], size);
          int level;
          file_read(save_file, level);

          if(!save_file.fail())
          {
            std::string line{
              std::format("{}, Level {}, v{}", name, level, version_string(game_version))};
            if(line.length() > longest_line_length)
            {
              longest_line_length = line.length();
            }
            save_file_lines.emplace_back(line);
            save_file_paths.emplace_back(file_path);
          }
          save_file.close();
        }
      }
    }
  }

  enum menu_option : uint8_t
  {
    PLAY_GAME,
    PLAY_YOURSELF,
    SHOW_SCORES,
    DISPLAY_HELP
  };

  const int top_option    = PLAY_GAME;
  const int bottom_option = DISPLAY_HELP;

  std::array menu_lines   = {"Omega Rebirth", "Play As Yourself", "High Scores", "Help Menu"};
  uint8_t selected_option = save_file_lines.empty() ? PLAY_GAME : bottom_option + 1;
  std::string name;
  int player_input;
  while(true)
  {
    erase();
    attron(CLR(BLUE) | A_BOLD);
    mvaddstr(0, 0, std::format("Welcome to Omega Rebirth v{}!", version_string(GAME_VERSION)).c_str());
    attroff(CLR(BLUE) | A_BOLD);
    attron(CLR(GREY) | A_BOLD);
    mvaddstr(1, 0, "(c) Copyright 1987-1989 Lawrence R. Brothers, 2019,2021-2023 Lyle Tafoya");
    attroff(CLR(GREY) | A_BOLD);
    mvaddstr(3, 0, "Enter your name: ");
    attron(CLR(BRIGHT_WHITE));
    if(name.empty())
    {
      mvaddstr(3, 17, default_name.c_str());
    }
    else
    {
      mvaddstr(3, 17, name.c_str());
    }
    attroff(CLR(BRIGHT_WHITE));
    mvaddstr(5, 0, "Choices:");
    attron(CLR(BRIGHT_WHITE));
    for(uint16_t i = 0; i < menu_lines.size(); ++i)
    {
      if(i == selected_option)
      {
        attroff(CLR(BRIGHT_WHITE));
        attron(A_REVERSE);
        mvaddstr(5 + i, 17, std::format("{:<{}}", menu_lines[i], longest_line_length).c_str());
        attroff(A_REVERSE);
        attron(CLR(BRIGHT_WHITE));
      }
      else
      {
        mvaddstr(5 + i, 17, menu_lines[i]);
      }
    }
    if(!save_file_paths.empty())
    {
      attroff(CLR(BRIGHT_WHITE));
      mvaddstr(10, 0, "Saved games:");
      attron(CLR(BRIGHT_WHITE));
      for(uint16_t i = 0; i < save_file_paths.size(); ++i)
      {
        if(i + menu_lines.size() == selected_option)
        {
          attroff(CLR(BRIGHT_WHITE));
          attron(A_REVERSE);
          mvaddstr(10 + i, 17, std::format("{:<{}}", save_file_lines[i], longest_line_length).c_str());
          attroff(A_REVERSE);
          attron(CLR(BRIGHT_WHITE));
        }
        else
        {
          mvaddstr(10 + i, 17, std::format("{:<{}}", save_file_lines[i], longest_line_length).c_str());
        }
      }
    }
    attroff(CLR(BRIGHT_WHITE));

    switch(player_input = getch())
    {
      case ESCAPE:
        endgraf();
        std::cout << "Bye!" << std::endl;
        exit(0);
        break;
      case KEY_ENTER:
      case '\n':
        switch(selected_option)
        {
          case PLAY_GAME:
            Player.name = name.empty() ? default_name : name;
            erase();
            refresh();
            init_game(false);
            return false;
          case PLAY_YOURSELF:
            Player.name = name.empty() ? default_name : name;
            init_game(true);
            user_intro();
            return false;
          case SHOW_SCORES:
            showscores();
            break;
          case DISPLAY_HELP:
            erase();
            refresh();
            help();
            break;
          default:
            erase();
            if(game_restore(save_file_paths[selected_option - menu_lines.size()]))
            {
              return true;
            }
        }
        break;
      case KEY_UP:
        selected_option = static_cast<menu_option>(std::max(top_option, selected_option - 1));
        break;
      case KEY_DOWN:
        selected_option = static_cast<menu_option>(std::min(
          static_cast<uint16_t>(bottom_option + save_file_paths.size()),
          static_cast<uint16_t>(selected_option + 1u)
        ));
        break;
      case KEY_BACKSPACE:
      case '\b':
        if(!name.empty())
        {
          name.pop_back();
        }
        break;
      default:
        if(std::isalpha(player_input) || (player_input == ' ' && !name.empty()))
        {
          if(name.empty())
          {
            name += std::toupper(player_input);
          }
          else
          {
            name += player_input;
          }
        }
        break;
    }
  }
}

long calcmana()
{
  return Player.pow * (long)(Player.level + 1);
}

// estimates on a 0..9 scale how good a player is at something
int competence_check(int attack)
{
  int ability = 0;
  switch(attack)
  {
    case 1: // melee
      ability += statmod(Player.str);
      [[fallthrough]];
    case 2: // missle
      ability += statmod(Player.dex);
      ability += Player.rank[LEGION];
      ability += static_cast<int>(Player.dmg / 10 - 1);
      break;
    case 3: // spellcasting
      ability += statmod(Player.iq);
      ability += statmod(Player.pow);
      ability += Player.rank[CIRCLE];
      ability += Player.rank[COLLEGE];
      ability += Player.rank[PRIEST];
      break;
    case 4: // thieving
      ability += statmod(Player.dex);
      ability += statmod(Player.agi);
      ability += Player.rank[THIEVES];
      break;
    case 5: // escape
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
  return ability;
}

// npcbehavior digits 1234
// 4 : alignment (LAWFUL,CHAOTIC, or NEUTRAL)
// 3 : primary combat action (melee,missile,spell,thief,flight,1..5)
// 2 : competence at 4 (0..9, 0 = incompetent, 9 = masterful)
// 1 : conversation mode
// status : 1 = dead, 2 = saved, 3 = retired, 4 = still playing

int fixnpc(int status)
{
  int npcbehavior = 0;
  char response;
  if(status == 1)
  { // player is dead, all undead are chaotic
    npcbehavior += CHAOTIC;
    npcbehavior += 10; // melee
    npcbehavior += 100 * std::min(9, static_cast<int>((Player.level / 3)));
    npcbehavior += 1000; // threaten
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
  return npcbehavior;
}
