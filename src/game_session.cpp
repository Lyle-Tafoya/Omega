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

// Game-session initialisation and main loop, extracted from omega.cpp so that
// front-ends other than the curses main() can drive the game.

#include "game_session.h"

#include "glob.h"
#include "scr.h"

#include <cctype>
#include <filesystem>
#include <format>
#include <limits>
#include <random>
#include <string>

#ifdef PDC_GL_BUILD
extern "C"
{
#  include <pdcgl.h>
}
#endif

extern std::string get_username();
extern bool title_menu();
extern void init_game(bool play_yourself = false);
extern void omega_title();

// environment is the environment about to be generated, or -1 for the first
// time, or -2 if we want to restore the random number point
void initrand(int environment, int level)
{
  static int store;
  int seed;

  if(environment >= 0)
  {
    store = random_range((std::numeric_limits<int>::max)());
  }
  if(environment == E_RANDOM)
  {
    seed = static_cast<int>(time(nullptr));
  }
  else if(environment == E_RESTORE)
  {
    seed = store;
  }
  else
  {
    seed = level_seed[environment] + 1000 * level;
  }
  generator.seed(seed);
}

bool game_restore(const std::filesystem::path &save_file_path)
{
  if(std::filesystem::exists(save_file_path) && restore_game(save_file_path.string()))
  {
    std::filesystem::remove(save_file_path);
    return true;
  }
  return false;
}

void init_world()
{
  for(int env = 0; env <= E_MAX; ++env)
  {
    level_seed[env] = random_range((std::numeric_limits<int>::max)());
  }
  load_country();
  for(int i = 0; i < NUMCITYSITES; ++i)
  {
    CitySiteList[i][0] = false;
  }
  load_city(true);
  WIDTH               = 64;
  LENGTH              = 64;
  Player.x            = 62;
  Player.y            = 21;
  Current_Environment = E_CITY;
  queue_message("You pass through the massive gates of Rampart, the city.");
}

void inititem(int reset)
{
  if(reset)
  {
    shuffle(scroll_ids, 30);
    shuffle(potion_ids, 20);
    shuffle(stick_ids, 20);
    shuffle(boot_ids, 20);
    shuffle(cloak_ids, 20);
    shuffle(ring_ids, 20);
  }
  for(int i = 0; i < NUMSCROLLS; ++i)
  {
    Objects[SCROLLID + i].objstr = scrollname(i);
  }
  for(int i = 0; i < NUMPOTIONS; ++i)
  {
    Objects[POTIONID + i].objstr = potionname(i);
  }
  Objects[ARTIFACTID + 10].objstr = potionname(18);
  Objects[ARTIFACTID + 13].objstr = potionname(19);
  for(int i = 0; i < NUMSTICKS; ++i)
  {
    Objects[STICKID + i].objstr = stickname(i);
  }
  for(int i = 0; i < NUMBOOTS; ++i)
  {
    Objects[BOOTID + i].objstr = bootname(i);
  }
  for(int i = 0; i < NUMCLOAKS; ++i)
  {
    Objects[CLOAKID + i].objstr = cloakname(i);
  }
  for(int i = 0; i < NUMRINGS; ++i)
  {
    Objects[RINGID + i].objstr = ringname(i);
  }
}

InitResult init_game_session()
{
#ifdef USER_DEFINED_OMEGALIB
  if(!(Omegalib = getenv("OMEGALIB")))
#endif
  {
    Omegalib = OMEGALIB;
  }

  if(!filecheck())
  {
    return InitResult::Failed;
  }

  initgraf();
#ifdef PDC_GL_BUILD
  PDC_set_title("Omega Rebirth");
#endif
  initdirs();
  initrand(E_RANDOM, 0);

  omega_title();

  Player.name         = get_username();
  Player.name.front() = static_cast<char>(std::toupper(static_cast<unsigned char>(Player.name.front())));
  optionset(SHOW_COLOUR, Player);
  bool continuing = false;

#ifdef MULTI_USER_SYSTEM
  continuing = game_restore(std::format("{}saves/{}/{}.sav", Omegalib, get_username(), Player.name));
  showscores();
  if(!continuing)
  {
    init_game(false);
  }
#else
  continuing = title_menu();
#endif

  if(continuing)
  {
    queue_message("Your adventure continues....");
    if(Lunarity == 1)
    {
      queue_message("You feel vitalized by the moon!");
    }
    else if(Lunarity == -1)
    {
      queue_message("The feel enervated by the moon!");
    }
  }

  timeprint();
  calc_melee();
  if(Current_Environment != E_COUNTRYSIDE)
  {
    showroom(Level->site[Player.x][Player.y].roomnumber);
  }
  else
  {
    terrain_check(false);
  }

  if(optionp(SHOW_COLOUR, Player))
  {
    colour_on();
  }
  else
  {
    colour_off();
  }

  screencheck(Player.x, Player.y);
  xredraw();

  return continuing ? InitResult::Continued : InitResult::NewGame;
}

void run_game_loop(bool reset_clock)
{
  while(true)
  {
    if(Current_Environment == E_COUNTRYSIDE)
    {
      p_country_process();
    }
    else
    {
      time_clock(reset_clock);
    }
    reset_clock = false;
  }
}
