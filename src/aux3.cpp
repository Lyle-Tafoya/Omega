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

// aux3.cpp
// some functions called by commmand[123].cpp, also see aux1.cpp, aux2.cpp
// This is a real grab bag file. It contains functions used by
// aux1.cpp and omega.cpp, as well as elsewhere. It is mainly here so aux1.cpp
// and aux2.cpp are not huge

#include "glob.h"
#include "interactive_menu.hpp"
#include "scr.h"
#include "scrolling_buffer.hpp"
#include "spell.h"

#include <algorithm>
#include <cassert>
#include <cctype>
#include <format>
#include <string>
#include <vector>

void indoors_random_event()
{
  switch(random_range(1000))
  {
    case 0:
      queue_message("You feel an unexplainable elation.");
      break;
    case 1:
      queue_message("You hear a distant rumbling.");
      break;
    case 2:
      queue_message("You realize your fly is open.");
      break;
    case 3:
      queue_message("You have a sudden craving for a pecan twirl.");
      break;
    case 4:
      queue_message("A mysterious healing flux settles over the level.");
      for(std::unique_ptr<monster> &m : Level->mlist)
      {
        if(m->hp > 0)
        {
          m->hp = Monsters[m->id].hp;
        }
      }
      Player.hp = std::max(Player.hp, Player.maxhp);
      break;
    case 5:
      queue_message("You discover an itch just where you can't scratch it.");
      break;
    case 6:
      queue_message("A cosmic ray strikes!");
      p_damage(10, UNSTOPPABLE, "a cosmic ray");
      break;
    case 7:
      queue_message("You catch your second wind....");
      Player.maxhp++;
      Player.hp   = std::max(Player.hp, Player.maxhp);
      Player.mana = std::max(Player.mana, calcmana());
      break;
    case 8:
      queue_message("You find some spare change in a hidden pocket.");
      Player.cash += Player.level * Player.level + 1;
      break;
    case 9:
      queue_message("You feel strangely lucky.");
      break;
    case 10:
      queue_message("You trip over something hidden in a shadow...");
      Level->site[Player.x][Player.y].things.emplace_back(create_object(difficulty()));
      pickup();
      break;
    case 11:
      queue_message("A mysterious voice echoes all around you....");
      hint();
      break;
    case 12:
      if(Balance > 0)
      {
        queue_message("You get word of the failure of your bank!");
        Balance = 0;
      }
      else
      {
        queue_message("You feel lucky.");
      }
      break;
    case 13:
      if(Balance > 0)
      {
        queue_message("You get word of a bank error in your favor!");
        Balance += 5000;
      }
      else
      {
        queue_message("You feel unlucky.");
      }
      break;
  }
  dataprint();
  showflags();
}

// check every ten minutes
void tenminute_check()
{
  if(Time % 60 == 0)
  {
    hourly_check();
  }
  else
  {
    if(Current_Environment == Current_Dungeon)
    {
      wandercheck();
    }
    minute_status_check();
    tenminute_status_check();
    if((Player.status[DISEASED] < 1) && (Player.hp < Player.maxhp))
    {
      Player.hp = std::min(Player.maxhp, Player.hp + Player.level + 1);
    }
    if(Current_Environment != E_COUNTRYSIDE && Current_Environment != E_ABYSS)
    {
      indoors_random_event();
    }
  }
}

// hourly check is same as ten_minutely check except food is also
// checked, and since time moves in hours out of doors, also
// outdoors_random_event is possible

void hourly_check()
{
  Player.food--;
  foodcheck();
  if(hour() == 0)
  { // midnight, a new day
    moon_check();
    Date++;
  }
  torch_check();
  if(Current_Environment == Current_Dungeon)
  {
    wandercheck();
  }
  minute_status_check();
  tenminute_status_check();
  if((Player.status[DISEASED] == 0) && (Player.hp < Player.maxhp))
  {
    Player.hp = std::min(Player.maxhp, Player.hp + Player.level + 1);
  }
  if(Current_Environment != E_COUNTRYSIDE && Current_Environment != E_ABYSS)
  {
    indoors_random_event();
  }
}

void outdoors_random_event()
{
  int num;

  switch(random_range(300))
  {
    case 0:
      switch(Country[Player.x][Player.y].current_terrain_type)
      {
        case TUNDRA:
          queue_message("It begins to snow. Heavily.");
          break;
        case DESERT:
          queue_message("A sandstorm swirls around you.");
          break;
        default:
          if((Date > 75) && (Date < 330))
          {
            queue_message("You are drenched by a sudden downpour!");
          }
          else
          {
            queue_message("It begins to snow. Heavily.");
          }
      }
      queue_message("Due to the inclement weather conditions, you have become lost.");
      Precipitation += random_range(12) + 1;
      setgamestatus(LOST, GameStatus);
      break;
    case 1:
      queue_message("You enter a field of brightly colored flowers...");
      queue_message("Wow, man! These are some pretty poppies...");
      queue_message("poppies...");
      queue_message("poppies...");
      queue_message("You become somewhat disoriented...");
      setgamestatus(LOST, GameStatus);
      break;
    case 2:
      queue_message("You discover a sprig of athelas growing lonely in the wild.");
      queue_message("Using your herbalist lore you cook a cake of lembas....");
      gain_item(std::make_unique<object>(Objects[FOODID + 1]));
      break;
    case 3:
      if(Precipitation > 0)
      {
        queue_message("You are struck by a bolt of lightning!");
        p_damage(random_range(25), ELECTRICITY, "a lightning strike");
      }
      else
      {
        queue_message("You feel static cling");
      }
      break;
    case 4:
      queue_message("You find a fast-food establishment.");
      l_commandant();
      break;
    case 5:
      queue_message("A weird howling tornado hits from out of the West!");
      queue_message("You've been caught in a chaos storm!");
      num = random_range(300);
      if(num < 10)
      {
        queue_message("Your cell-structure was disrupted!");
        p_damage(random_range(100), UNSTOPPABLE, "a chaos storm");
      }
      else if(num < 20)
      {
        queue_message("The chaos storm warps your frame!");
        queue_message("Your statistical entropy has been maximized.");
        queue_message("You feel average...");
        toggle_item_use(true);
        Player.str = Player.maxstr = Player.con = Player.maxcon = Player.dex = Player.maxdex =
          Player.agi = Player.maxagi = Player.iq = Player.maxiq = Player.pow = Player.maxpow =
            ((Player.maxstr + Player.maxcon + Player.maxdex + Player.maxagi + Player.maxiq +
              Player.maxpow + 12) /
             6);
        toggle_item_use(false);
      }
      else if(num < 30)
      {
        queue_message("Your entire body glows with an eerie flickering light.");
        toggle_item_use(true);
        for(int i = 1; i < MAXITEMS; ++i)
        {
          if(Player.possessions[i])
          {
            Player.possessions[i]->plus++;
            if(Player.possessions[i]->objchar == STICK)
            {
              Player.possessions[i]->charge += 10;
            }
            Player.possessions[i]->blessing += 10;
          }
        }
        toggle_item_use(false);
        cleanse(1);
        queue_message("You feel filled with energy!");
        Player.maxpow += 5;
        Player.pow += 5;
        Player.mana = Player.maxmana = calcmana() * 5;
        queue_message("You also feel weaker. Paradoxical, no?");
        Player.con -= 5;
        Player.maxcon -= 5;
        if(Player.con < 3)
        {
          p_death("congestive heart failure");
        }
      }
      else if(num < 40)
      {
        queue_message("Your entire body glows black.");
        dispel(-1);
        dispel(-1);
        Player.pow -= 10;
        Player.mana = 0;
      }
      else if(num < 60)
      {
        queue_message("The storm deposits you in a strange place....");
        do
        {
          Player.x = random_range(WIDTH);
          Player.y = random_range(LENGTH);
        } while(Country[Player.x][Player.y].current_terrain_type == CHAOS_SEA && !Player.rank[ADEPT]);
        screencheck(Player.x, Player.y);
      }
      else if(num < 70)
      {
        queue_message("A tendril of the storm condenses and falls into your hands.");
        auto o = std::make_unique<object>();
        make_artifact(o.get(), -1);
        gain_item(std::move(o));
      }
      else if(num < 80)
      {
        if(gamestatusp(MOUNTED, GameStatus))
        {
          queue_message("Your horse screams as he is transformed into an");
          queue_message("imaginary unseen dead tortoise.");
          queue_message("You are now on foot.");
          resetgamestatus(MOUNTED, GameStatus);
        }
        else
        {
          queue_message("You notice you are riding a horse. Odd. Very odd....");
          queue_message("Now that's a horse of a different color!");
          setgamestatus(MOUNTED, GameStatus);
        }
      }
      else if(num < 90)
      {
        queue_message("You feel imbued with godlike power....");
        wish(1);
      }
      else if(num < 100)
      {
        queue_message("The chaos storm has wiped your memory!");
        queue_message("You feel extraordinarily naive....");
#ifndef MULTI_USER_SYSTEM
        queue_message("You can't remember a thing! Not even your name.");
#endif
        Player.xp    = 0;
        Player.level = 0;
        for(int i = 0; i < NUMRANKS; ++i)
        {
          Player.rank[i] = 0;
        }
        for(int i = 0; i < spell::NUM_SPELLS; ++i)
        {
          spell::Spells[i].known = false;
        }
#ifndef MULTI_USER_SYSTEM
        rename_player();
#endif
      }
      else
      {
        queue_message("You survive the chaos storm relatively unscathed.");
        queue_message("It was a learning experience.");
        gain_experience(1000);
      }
      break;
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
      queue_message("An encounter!");
      change_environment(E_TACTICAL_MAP);
      break;
    case 11:
      queue_message("You find a Traveller's Aid station with maps of the local area.");
      if(gamestatusp(LOST, GameStatus))
      {
        resetgamestatus(LOST, GameStatus);
        queue_message("You know where you are now.");
      }
      for(int x = Player.x - 5; x < Player.x + 6; ++x)
      {
        for(int y = Player.y - 5; y < Player.y + 6; ++y)
        {
          if(inbounds(x, y))
          {
            c_set(x, y, SEEN, Country);
            if(Country[x][y].current_terrain_type != Country[x][y].base_terrain_type)
            {
              c_set(x, y, CHANGED, Country);
              Country[x][y].current_terrain_type = Country[x][y].base_terrain_type;
            }
          }
        }
      }
      show_screen();
      break;
    case 12:
      if(!gamestatusp(MOUNTED, GameStatus))
      {
        queue_message("You develop blisters....");
        p_damage(1, UNSTOPPABLE, "blisters");
      }
      break;
    case 13:
      queue_message("You discover an itch just where you can't scratch it.");
      break;
    case 14:
      queue_message("A cosmic ray strikes!");
      p_damage(10, UNSTOPPABLE, "a cosmic ray");
      break;
    case 15:
      queue_message("You feel strangely lucky.");
      break;
    case 16:
      queue_message("The west wind carries with it a weird echoing voice....");
      hint();
      break;
  }
  dataprint();
  showflags();
}

char getlocation()
{
  char c = '\0';

  menuprint(" (enter location [HCL]) ");
  showmenu();
  while(c == '\0')
  {
    switch(c = ((char)mcigetc()))
    {
      case 'h':
        menuprint(" High.");
        break;
      case 'c':
        menuprint(" Center.");
        break;
      case 'l':
        menuprint(" Low.");
        break;
      default:
        c = '\0';
        break;
    }
  }
  showmenu();
  return c - 'a' + 'A';
}

// chance for player to resist magic somehow
// hostile_magic ranges in power from 0 (weak) to 10 (strong)
bool magic_resist(int hostile_magic)
{
  if((Player.rank[COLLEGE] + Player.rank[CIRCLE] > 0) && (Player.level / 2 + random_range(20) > hostile_magic + random_range(20)))
  {
    if(Player.mana > hostile_magic * hostile_magic)
    {
      queue_message("Thinking fast, you defend youself with a counterspell!");
      Player.mana -= hostile_magic * hostile_magic;
      dataprint();
      return true;
    }
  }
  if(Player.level / 4 + Player.status[PROTECTION] + random_range(20) > hostile_magic + random_range(30))
  {
    queue_message("You resist the spell!");
    return true;
  }
  else
  {
    return false;
  }
}

void terrain_check(int takestime)
{
  int faster = 0;

  if(Player.patron == DRUID)
  {
    faster = 1;
    switch(random_range(32))
    {
      case 0:
        queue_message("Along the many paths of nature...");
        break;
      case 1:
        queue_message("You move swiftly through the wilderness.");
        break;
    }
  }
  else if(gamestatusp(MOUNTED, GameStatus))
  {
    faster = 1;
    switch(random_range(32))
    {
      case 0:
      case 1:
        queue_message("Clippity Clop.");
        break;
      case 2:
        queue_message("....my spurs go jingle jangle jingle....");
        break;
      case 3:
        queue_message("....as I go riding merrily along....");
        break;
    }
  }
  else if(Player.possessions[O_BOOTS] && Player.possessions[O_BOOTS]->on_equip == I_BOOTS_7LEAGUE)
  {
    takestime = 0;
    switch(random_range(32))
    {
      case 0:
        queue_message("Boingg!");
        break;
      case 1:
        queue_message("Whooosh!");
        break;
      case 2:
        queue_message("Over hill, over dale....");
        break;
      case 3:
        queue_message("...able to leap over 7 leagues in a single bound....");
        break;
    }
  }
  else if(Player.status[SHADOWFORM])
  {
    faster = 1;
    switch(random_range(32))
    {
      case 0:
        queue_message("As swift as a shadow.");
        break;
      case 1:
        queue_message("\"I walk through the trees...\"");
        break;
    }
  }
  else
  {
    switch(random_range(32))
    {
      case 0:
        queue_message("Trudge. Trudge.");
        break;
      case 1:
        queue_message("The road goes ever onward....");
        break;
    }
  }
  switch(Country[Player.x][Player.y].current_terrain_type)
  {
    case RIVER:
      if((Player.y < 6) && (Player.x > 20))
      {
        locprint("Star Lake.");
      }
      else if(Player.y < 41)
      {
        if(Player.x < 10)
        {
          locprint("Aerie River.");
        }
        else
        {
          locprint("The Great Flood.");
        }
      }
      else if(Player.x < 42)
      {
        locprint("The Swamp Runs.");
      }
      else
      {
        locprint("River Greenshriek.");
      }
      if(takestime)
      {
        Time += 60;
        hourly_check();
        Time += 60;
        hourly_check();
        Time += 60;
        hourly_check();
      }
      break;
    case ROAD:
      locprint("A well-maintained road.");
      if(takestime)
      {
        Time += 60;
        hourly_check();
      }
      break;
    case PLAINS:
      locprint("A rippling sea of grass.");
      if(takestime)
      {
        Time += 60;
        hourly_check();
        if(!faster)
        {
          Time += 60;
          hourly_check();
        }
      }
      break;
    case TUNDRA:
      locprint("The Great Northern Wastes.");
      if(takestime)
      {
        Time += 60;
        hourly_check();
        if(!faster)
        {
          Time += 60;
          hourly_check();
        }
      }
      break;
    case FOREST:
      if(Player.y < 10)
      {
        locprint("The Deepwood.");
      }
      else if(Player.y < 18)
      {
        locprint("The Forest of Erelon.");
      }
      else if(Player.y < 46)
      {
        locprint("The Great Forest.");
      }
      if(takestime)
      {
        Time += 60;
        hourly_check();
        if(Player.rank[PRIESTHOOD] == 0 || Player.patron != DRUID)
        {
          Time += 60;
          hourly_check();
          if(!faster)
          {
            Time += 60;
            hourly_check();
          }
        }
      }
      break;
    case JUNGLE:
      locprint("Greenshriek Jungle.");
      if(takestime)
      {
        Time += 60;
        hourly_check();
        Time += 60;
        hourly_check();
        if(!faster)
        {
          Time += 60;
          hourly_check();
          Time += 60;
          hourly_check();
        }
      }
      break;
    case DESERT:
      locprint("The Waste of Time.");
      if(takestime)
      {
        Time += 60;
        hourly_check();
        Time += 60;
        hourly_check();
        if(!faster)
        {
          Time += 60;
          hourly_check();
          Time += 60;
          hourly_check();
        }
      }
      break;
    case MOUNTAINS:
      if((Player.y < 9) && (Player.x < 12))
      {
        locprint("The Magic Mountains");
      }
      else if((Player.y < 9) && (Player.y > 2) && (Player.x < 40))
      {
        locprint("The Peaks of the Fist.");
      }
      else if(Player.x < 52)
      {
        locprint("The Rift Mountains.");
      }
      else
      {
        locprint("Borderland Mountains.");
      }
      if(takestime)
      {
        Time += 60;
        hourly_check();
        Time += 60;
        hourly_check();
        Time += 60;
        hourly_check();
        Time += 60;
        hourly_check();
        Time += 60;
        hourly_check();
        if(!faster)
        {
          Time += 60;
          hourly_check();
          Time += 60;
          hourly_check();
          Time += 60;
          hourly_check();
        }
      }
      break;
    case PASS:
      locprint("A hidden pass.");
      if(takestime)
      {
        Time += 60;
        hourly_check();
      }
      break;
    case CHAOS_SEA:
      locprint("The Sea of Chaos.");
      if(takestime)
      {
        Time += 60;
        hourly_check();
      }
      queue_message("You have entered the sea of chaos...");
      l_chaos();
      break;
    case SWAMP:
      locprint("The Loathly Swamp.");
      if(takestime)
      {
        Time += 60;
        hourly_check();
        Time += 60;
        hourly_check();
        Time += 60;
        hourly_check();
        Time += 60;
        hourly_check();
        Time += 60;
        hourly_check();
        Time += 60;
        hourly_check();
        if(!faster)
        {
          Time += 60;
          hourly_check();
          Time += 60;
          hourly_check();
        }
      }
      break;
    case CITY:
      if(gamestatusp(LOST, GameStatus))
      {
        resetgamestatus(LOST, GameStatus);
        queue_message("Well, I guess you know where you are now....");
      }
      locprint("Outside Rampart, the city.");
      break;
    case VILLAGE:
      if(gamestatusp(LOST, GameStatus))
      {
        resetgamestatus(LOST, GameStatus);
        queue_message("The village guards let you know where you are....");
      }
      locprint("Outside a small village.");
      break;
    case CAVES:
      locprint("A deserted hillside.");
      if(takestime)
      {
        Time += 60;
        hourly_check();
      }
      queue_message("You notice a concealed entrance into the hill.");
      break;
    case CASTLE:
      locprint("Near a fortified castle.");
      if(takestime)
      {
        Time += 60;
        hourly_check();
      }
      queue_message("The castle is hewn from solid granite. The drawbridge is down.");
      break;
    case TEMPLE:
      switch(Country[Player.x][Player.y].aux)
      {
        case ODIN:
          locprint("A rough-hewn granite temple.");
          break;
        case SET:
          locprint("A black pyramidal temple made of sandstone.");
          break;
        case ATHENA:
          locprint("A classical marble-columned temple.");
          break;
        case HECATE:
          locprint("A temple of ebony adorned with ivory.");
          break;
        case DRUID:
          locprint("A temple formed of living trees.");
          break;
        case DESTINY:
          locprint("A temple of some mysterious blue crystal.");
          break;
      }
      if(takestime)
      {
        Time += 60;
        hourly_check();
      }
      queue_message("You notice an entrance conveniently at hand.");
      break;
    case MAGIC_ISLE:
      locprint("A strange island in the midst of the Sea of Chaos.");
      if(takestime)
      {
        Time += 60;
        hourly_check();
      }
      queue_message("There is a narrow causeway to the island from here.");
      break;
    case STARPEAK:
      locprint("Star Peak.");
      if(takestime)
      {
        Time += 60;
        hourly_check();
      }
      queue_message("The top of the mountain seems to glow with a allochroous aura.");
      break;
    case DRAGONLAIR:
      locprint("A rocky chasm.");
      if(takestime)
      {
        Time += 60;
        hourly_check();
      }
      queue_message("You are at a cave entrance from which you see the glint of gold.");
      break;
    case VOLCANO:
      locprint("HellWell Volcano.");
      if(takestime)
      {
        Time += 60;
        hourly_check();
      }
      queue_message("A shimmer of heat lightning plays about the crater rim.");
      break;
    default:
      locprint("I haven't any idea where you are!!!");
      break;
  }
  outdoors_random_event();
}

void countrysearch()
{
  Time += 60;
  hourly_check();
  for(int x = Player.x - 1; x < Player.x + 2; ++x)
  {
    for(int y = Player.y - 1; y < Player.y + 2; ++y)
    {
      if(inbounds(x, y))
      {
        if(Country[x][y].current_terrain_type != Country[x][y].base_terrain_type)
        {
          queue_message("Your search was fruitful!");
          Country[x][y].current_terrain_type = Country[x][y].base_terrain_type;
          c_set(x, y, CHANGED, Country);
          queue_message("You discovered:");
          queue_message(countryid(Country[x][y].base_terrain_type));
        }
      }
    }
  }
}

std::string countryid(chtype terrain)
{
  switch(terrain & 0xff)
  {
    case MOUNTAINS & 0xff:
      return "Almost impassable mountains";
    case PLAINS & 0xff:
      return "Seemingly endless plains";
    case TUNDRA & 0xff:
      return "A frosty stretch of tundra";
    case ROAD & 0xff:
      return "A paved highway";
    case PASS & 0xff:
      return "A secret mountain pass";
    case RIVER & 0xff:
      return "A rolling river";
    case CITY & 0xff:
      return "The city of Rampart";
    case VILLAGE & 0xff:
      return "A rural village";
    case FOREST & 0xff:
      return "A verdant forest";
    case JUNGLE & 0xff:
      return "A densely overgrown jungle";
    case SWAMP & 0xff:
      return "A swampy fen";
    case VOLCANO & 0xff:
      return "A huge active volcano";
    case CASTLE & 0xff:
      return "An imposing castle";
    case STARPEAK & 0xff:
      return "A mysterious mountain.";
    case DRAGONLAIR & 0xff:
      return "A cavern filled with treasure.";
    case MAGIC_ISLE & 0xff:
      return "An island emanating magic.";
    case CAVES & 0xff:
      return "A hidden cave entrance";
    case TEMPLE & 0xff:
      return "A neoclassical temple";
    case DESERT & 0xff:
      return "A sere desert";
    case CHAOS_SEA & 0xff:
      return "The Sea of Chaos";
    default:
      return "I have no idea.";
  }
}

const std::string sitenames[]{
  "alchemist",
  "arena",
  "armorer",
  "bank",
  "brothel",
  "casino",
  "castle",
  "city gates",
  "collegium magii",
  "condo",
  "department of public works",
  "diner",
  "explorers' club",
  "fast food",
  "gymnasium",
  "healer",
  "hospice",
  "les crapuleux",
  "library",
  "mercenary guild",
  "monastery",
  "oracle",
  "order of paladins",
  "pawn shop",
  "sorcerors' guild ",
  "tavern",
  "temple",
  "thieves' guild"};

static int sitenums[] // the order matches sitenames[]
  {L_ALCHEMIST,   L_ARENA,   L_ARMORER,   L_BANK,      L_BROTHEL, L_CASINO,     L_CASTLE,
   L_COUNTRYSIDE, L_COLLEGE, L_CONDO,     L_DPW,       L_DINER,   L_CLUB,       L_COMMANDANT,
   L_GYM,         L_HEALER,  L_CHARITY,   L_CRAP,      L_LIBRARY, L_MERC_GUILD, L_MONASTERY,
   L_ORACLE,      L_ORDER,   L_PAWN_SHOP, L_SORCERORS, L_TAVERN,  L_TEMPLE,     L_THIEVES_GUILD};

std::vector<std::string> known_sites(int first, int last)
{
  std::vector<std::string> lines;
  bool printed = false;
  for(int i = first; i <= last; ++i)
  {
    if(CitySiteList[sitenums[i] - CITYSITEBASE][0])
    {
      printed = true;
      lines.emplace_back(std::format("  {}", sitenames[i]));
    }
  }
  if(!printed)
  {
    lines.emplace_back("No known sites match that prefix!");
  }
  return lines;
}

int parsecitysite()
{
  std::string prefix;
  bool found          = false;
  int first           = 0;
  int last            = NUMCITYSITES - 1;
  bool menu_displayed = false;
  int player_input;
  append_message("", true);
  do
  {
    if(menu_displayed)
    {
      menu->load(known_sites(first, last), {"Possible Sites -"});
      player_input = menu->get_player_input();
    }
    else
    {
      player_input = mgetc();
    }
    switch(player_input)
    {
      case KEY_BACKSPACE:
      case DELETE:
      case KEY_DC:
      case '\b':
        if(!prefix.empty())
        {
          prefix.pop_back();
          for(int f = first; f >= 0 && sitenames[f].starts_with(prefix); --f)
          {
            if(CitySiteList[sitenums[f] - CITYSITEBASE][0])
            {
              first = f;
            }
          }
          for(int l = last; l < NUMCITYSITES && sitenames[l].starts_with(prefix); ++l)
          {
            if(CitySiteList[sitenums[l] - CITYSITEBASE][0])
            {
              last = l;
            }
          }
          if(found)
          {
            found = false;
          }
          replace_last_message(prefix);
        }
        if(prefix.empty())
        {
          first = 0;
          last  = NUMCITYSITES - 1;
          found = false;
        }
        break;
      case ESCAPE:
        replace_last_message("_ Move cancelled.");
        xredraw();
        return ABORT;
      case '?':
        menu_displayed = true;
        break;
      case '\n':
        break;
      default:
        if(std::isupper(player_input))
        {
          player_input = std::tolower(player_input);
        }
        if(found)
        {
          continue;
        }
        int f = first;
        while(f < NUMCITYSITES &&
              (!CitySiteList[sitenums[f] - CITYSITEBASE][0] || sitenames[f].length() < prefix.length() ||
               sitenames[f][prefix.length()] < player_input))
        {
          ++f;
        }
        int l = last;
        while(l >= 0 &&
              (!CitySiteList[sitenums[l] - CITYSITEBASE][0] || sitenames[l].length() < prefix.length() ||
               sitenames[l][prefix.length()] > player_input))
        {
          --l;
        }
        if(l < f)
        {
          continue;
        }
        prefix.push_back(player_input);
        replace_last_message(prefix);
        first = f;
        last  = l;
        if(first == last && !found)
        { // unique name
          found = true;
          replace_last_message(sitenames[first]);
        }
        break;
    }
  } while(player_input != '\n');
  xredraw();
  if(found)
  {
    return sitenums[first] - CITYSITEBASE;
  }
  else
  {
    append_message("That is an ambiguous abbreviation!", true);
    return ABORT;
  }
}

// are there hostile monsters within 2 moves?
bool hostilemonstersnear()
{
  bool hostile = false;

  for(int x = Player.x - 2; (x < Player.x + 3 && !hostile); ++x)
  {
    for(int y = Player.y - 2; (y < Player.y + 3 && !hostile); ++y)
    {
      if(inbounds(x, y))
      {
        if(Level->site[x][y].creature)
        {
          hostile = m_statusp(*Level->site[x][y].creature, HOSTILE);
        }
      }
    }
  }

  return hostile;
}

// random effects from some of stones in villages
// if alignment of stone is alignment of player, gets done sooner
bool stonecheck(int alignment)
{
  int *stone;
  bool match = false, cycle = false;

  if(alignment == 1)
  {
    stone = &Lawstone;
    match = Player.alignment > 0;
  }
  else if(alignment == -1)
  {
    stone = &Chaostone;
    match = Player.alignment < 0;
  }
  else
  {
    stone = &Mindstone;
    match = false;
  }
  *stone += random_range(4) + (match ? random_range(4) : 0);
  switch((*stone)++)
  {
    case 0:
    case 2:
    case 4:
    case 6:
    case 8:
    case 10:
    case 12:
    case 14:
    case 16:
    case 18:
    case 20:
    case 22:
    case 24:
    case 26:
    case 28:
    case 30:
    case 32:
    case 34:
    case 36:
    case 38:
    case 40:
      queue_message("The stone glows grey.");
      queue_message("Not much seems to happen this time.");
      (*stone)--;
      break;
    case 1:
      queue_message("The stone glows black");
      queue_message("A burden has been removed from your shoulders.....");
      queue_message("Your pack has disintegrated!");
      Player.pack.clear();
      break;
    case 3:
      queue_message("The stone glows microwave");
      queue_message("A vortex of antimana spins about you!");
      dispel(-1);
      break;
    case 5:
      queue_message("The stone glows infrared");
      queue_message("A portal opens nearby and an obviously confused monster appears!");
      summon(-1, -1);
      break;
    case 7:
      queue_message("The stone glows brick red");
      queue_message("A gold piece falls from the heavens into your money pouch!");
      Player.cash++;
      break;
    case 9:
      queue_message("The stone glows cherry red");
      queue_message("A flush of warmth spreads through your body.");
      augment(1);
      break;
    case 11:
      queue_message("The stone glows orange");
      queue_message("A flux of energy blasts you!");
      manastorm(Player.x, Player.y, random_range(Player.maxhp) + 1);
      break;
    case 13:
      queue_message("The stone glows lemon yellow");
      queue_message("You're surrounded by enemies! You begin to foam at the mouth.");
      Player.status[BERSERK] += 10;
      break;
    case 15:
      queue_message("The stone glows yellow");
      queue_message("Oh no! The DREADED AQUAE MORTIS!");
      queue_message("No, wait, it's just your imagination.");
      break;
    case 17:
      queue_message("The stone glows chartreuse");
      queue_message("Your joints stiffen up.");
      Player.agi -= 3;
      break;
    case 19:
      queue_message("The stone glows green");
      queue_message("You come down with an acute case of Advanced Leprosy.");
      Player.status[DISEASED] = 1100;
      Player.hp               = 1;
      Player.dex -= 5;
      break;
    case 21:
      queue_message("The stone glows forest green");
      queue_message("You feel wonderful!");
      Player.status[HERO] += 10;
      break;
    case 23:
      queue_message("The stone glows cyan");
      queue_message("You feel a strange twisting sensation....");
      strategic_teleport(-1);
      break;
    case 25:
      queue_message("The stone glows blue");
      queue_message("You feel a tingle of an unearthly intuition:");
      hint();
      break;
    case 27:
      queue_message("The stone glows navy blue");
      queue_message("A sudden shock of knowledge overcomes you.");
      identify(1);
      knowledge(1);
      break;
    case 29:
      queue_message("The stone glows blue-violet");
      queue_message("You feel forgetful.");
      for(int i = 0; i < spell::NUM_SPELLS; ++i)
      {
        if(spell::Spells[i].known)
        {
          spell::Spells[i].known = false;
          break;
        }
      }
      break;
    case 31:
      queue_message("The stone glows violet");
      acquire(0);
      break;
    case 33:
      queue_message("The stone glows deep purple");
      queue_message("You vanish.");
      Player.status[INVISIBLE] += 10;
      break;
    case 35:
      queue_message("The stone glows ultraviolet");
      queue_message("All your hair rises up on end.... A bolt of lightning hits you!");
      p_damage(random_range(Player.maxhp), ELECTRICITY, "mystic lightning");
      break;
    case 37:
      queue_message("The stone glows roentgen");
      queue_message("You feel more experienced.");
      gain_experience((Player.level + 1) * 250);
      break;
    case 39:
      queue_message("The stone glows gamma");
      queue_message("Your left hand shines silvery, and your right emits a golden aura.");
      enchant(1);
      bless(1);
      queue_message("Your hands stop glowing.");
      break;
    case 41:
    case 42:
    case 43:
    case 44:
    case 45:
    case 46:
    case 47:
    case 48:
    case 49:
      queue_message("The stone glows cosmic!");
      queue_message("The stone's energy field quiets for a moment...");
      *stone = 50;
      cycle  = true;
      break;
    default:
      queue_message("The stone glows polka-dot (?!?!?!?)");
      queue_message("You feel a strange twisting sensation....");
      *stone = 0;
      strategic_teleport(-1);
      break;
  }
  calc_melee();
  return cycle;
}

// can only occur when player is in city, so OK to use Level
void destroy_order()
{
  setgamestatus(DESTROYED_ORDER, GameStatus);
  if(Level != City.get())
  {
    queue_message("Zounds! A Serious Mistake!");
  }
  else
  {
    for(int x = 35; x < 46; ++x)
    {
      for(int y = 60; y < 63; ++y)
      {
        if(x == 40 && (y == 60 || y == 61))
        {
          lreset(x, y, SECRET, *Level);
          Level->site[x][y].locchar = FLOOR;
          Level->site[x][y].p_locf  = L_NO_OP;
          lset(x, y, CHANGED, *Level);
        }
        else
        {
          Level->site[x][y].locchar = RUBBLE;
          Level->site[x][y].p_locf  = L_RUBBLE;
          lset(x, y, CHANGED, *Level);
        }
        if(Level->site[x][y].creature)
        {
          Level->site[x][y].creature->hp = -1;
          Level->site[x][y].creature     = nullptr;
        }
        make_site_monster(x, y, GHOST);
        Level->site[x][y].creature->monstring = "ghost of a Paladin";
        m_status_set(*Level->site[x][y].creature, HOSTILE);
      }
    }
  }
}

void alert_guards()
{
  bool foundguard = false;
  int suppress = 0;
  for(std::unique_ptr<monster> &m : Level->mlist)
  {
    if((m->id == GUARD || (m->id == HISCORE_NPC && m->aux2 == 15)) && m->hp > 0)
    { // justiciar
      foundguard = true;
      m_status_set(*m, AWAKE);
      m_status_set(*m, HOSTILE);
    }
  }
  if(foundguard)
  {
    queue_message("You hear a whistle and the sound of running feet!");
    if(Current_Environment == E_CITY)
    {
      Level->site[40][60].p_locf = L_NO_OP; // pacify_guards restores this
    }
  }
  if((!foundguard) && (Current_Environment == E_CITY) && !gamestatusp(DESTROYED_ORDER, GameStatus))
  {
    suppress = gamestatusp(SUPPRESS_PRINTING, GameStatus);
    resetgamestatus(SUPPRESS_PRINTING, GameStatus);
    queue_message("The last member of the Order of Paladins dies....");
    gain_experience(1000);
    Player.alignment -= 250;
    if(!gamestatusp(KILLED_LAWBRINGER, GameStatus))
    {
      queue_message("A chime sounds from far away.... The sound grows stronger....");
      queue_message("Suddenly the great shadowy form of the LawBringer appears over");
      queue_message("the city. He points his finger at you....");
      queue_message("\"Cursed art thou, minion of chaos! May thy strength fail thee");
      queue_message("in thy hour of need!\" You feel an unearthly shiver as the");
      queue_message("LawBringer waves his palm across the city skies....");
      Player.str /= 2;
      dataprint();
      queue_message("You hear a bell tolling, and eerie moans all around you....");
      queue_message("Suddenly, the image of the LawBringer is gone.");
      queue_message("You hear a guardsman's whistle in the distance!");
      resurrect_guards();
    }
    else
    {
      queue_message("The Order's magical defenses have dropped, and the");
      queue_message("Legions of Chaos strike....");
      queue_message("The city shakes! An earthquake has struck!");
      queue_message("Cracks open in the street, and a chasm engulfs the Order HQ!");
      queue_message("Flames lick across the sky and you hear wild laughter....");
      gain_experience(5000);
      destroy_order();
    }
  }
  if(suppress)
  {
    resetgamestatus(SUPPRESS_PRINTING, GameStatus);
  }
}

size_t maneuvers()
{
  int m = 2 + Player.level / 7;
  if(Player.rank[ARENA])
  {
    ++m;
  }
  if(Player.status[HASTED])
  {
    m *= 2;
  }
  if(Player.status[SLOWED])
  {
    m /= 2;
  }
  return std::min(8, std::max(1, m));
}
