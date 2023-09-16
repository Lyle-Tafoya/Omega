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

/* aux3.c */
/* some functions called by com.c, also see aux1.c, aux2.c */
/* This is a real grab bag file. It contains functions used by
   aux1.c and omega.c, as well as elsewhere. It is mainly here so aux1.c
   and aux2.c are not huge */

#include "glob.h"
#include "interactive_menu.hpp"
#include "scrolling_buffer.hpp"

#include <algorithm>
#include <cassert>
#include <string>
#include <vector>

extern scrolling_buffer message_buffer;
extern void           append_message(const std::string &message, bool force_break = false);
extern interactive_menu *menu;

/* check every ten minutes */
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

/* hourly check is same as ten_minutely check except food is also
   checked, and since time moves in hours out of doors, also
   outdoors_random_event is possible */

void hourly_check()
{
  Player.food--;
  foodcheck();
  if(hour() == 0)
  { /* midnight, a new day */
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

void indoors_random_event()
{
  pml ml;
  pol ol;
  switch(random_range(1000))
  {
    case 0:
      print3("You feel an unexplainable elation.");
      break;
    case 1:
      print3("You hear a distant rumbling.");
      break;
    case 2:
      print3("You realize your fly is open.");
      break;
    case 3:
      print3("You have a sudden craving for a pecan twirl.");
      break;
    case 4:
      print3("A mysterious healing flux settles over the level.");
      for(ml = Level->mlist; ml != NULL; ml = ml->next)
      {
        if(ml->m->hp > 0)
        {
          ml->m->hp = Monsters[ml->m->id].hp;
        }
      }
      Player.hp = std::max(Player.hp, Player.maxhp);
      break;
    case 5:
      print3("You discover an itch just where you can't scratch it.");
      break;
    case 6:
      print3("A cosmic ray strikes!");
      p_damage(10, UNSTOPPABLE, "a cosmic ray");
      break;
    case 7:
      print3("You catch your second wind....");
      Player.maxhp++;
      Player.hp   = std::max(Player.hp, Player.maxhp);
      Player.mana = std::max(Player.mana, calcmana());
      break;
    case 8:
      print3("You find some spare change in a hidden pocket.");
      Player.cash += Player.level * Player.level + 1;
      break;
    case 9:
      print3("You feel strangely lucky.");
      break;
    case 10:
      print3("You trip over something hidden in a shadow...");
      ol        = ((pol)checkmalloc(sizeof(oltype)));
      ol->thing = create_object(difficulty()); /* FIXED!  12/30/98 */
      assert(ol->thing);                       /* WDT I want to make sure... */
      ol->next                               = Level->site[Player.x][Player.y].things;
      Level->site[Player.x][Player.y].things = ol;
      pickup();
      break;
    case 11:
      print3("A mysterious voice echoes all around you....");
      hint();
      break;
    case 12:
      if(Balance > 0)
      {
        print3("You get word of the failure of your bank!");
        Balance = 0;
      }
      else
      {
        print3("You feel lucky.");
      }
      break;
    case 13:
      if(Balance > 0)
      {
        print3("You get word of a bank error in your favor!");
        Balance += 5000;
      }
      else
      {
        print3("You feel unlucky.");
      }
      break;
  }
  dataprint();
  showflags();
}

void outdoors_random_event()
{
  int num, i, j;
  pob ob;

  switch(random_range(300))
  {
    case 0:
      switch(Country[Player.x][Player.y].current_terrain_type)
      {
        case TUNDRA:
          mprint("It begins to snow. Heavily.");
          break;
        case DESERT:
          mprint("A sandstorm swirls around you.");
          break;
        default:
          if((Date > 75) && (Date < 330))
          {
            mprint("You are drenched by a sudden downpour!");
          }
          else
          {
            mprint("It begins to snow. Heavily.");
          }
      }
      mprint("Due to the inclement weather conditions, you have become lost.");
      Precipitation += random_range(12) + 1;
      setgamestatus(LOST, GameStatus);
      break;
    case 1:
      mprint("You enter a field of brightly colored flowers...");
      mprint("Wow, man! These are some pretty poppies...");
      mprint("poppies...");
      mprint("poppies...");
      print3("You become somewhat disoriented...");
      setgamestatus(LOST, GameStatus);
      break;
    case 2:
      mprint("You discover a sprig of athelas growing lonely in the wild.");
      mprint("Using your herbalist lore you cook a cake of lembas....");
      ob  = ((pob)checkmalloc(sizeof(objtype)));
      *ob = Objects[FOODID + 1];
      gain_item(ob);
      break;
    case 3:
      if(Precipitation > 0)
      {
        mprint("You are struck by a bolt of lightning!");
        p_damage(random_range(25), ELECTRICITY, "a lightning strike");
      }
      else
      {
        mprint("You feel static cling");
      }
      break;
    case 4:
      mprint("You find a fast-food establishment.");
      l_commandant();
      break;
    case 5:
      mprint("A weird howling tornado hits from out of the West!");
      mprint("You've been caught in a chaos storm!");
      num = random_range(300);
      if(num < 10)
      {
        mprint("Your cell-structure was disrupted!");
        p_damage(random_range(100), UNSTOPPABLE, "a chaos storm");
      }
      else if(num < 20)
      {
        mprint("The chaos storm warps your frame!");
        mprint("Your statistical entropy has been maximized.");
        mprint("You feel average...");
        toggle_item_use(true); /* FIXED! 12/30/98 */
        Player.str = Player.maxstr = Player.con = Player.maxcon = Player.dex = Player.maxdex =
          Player.agi = Player.maxagi = Player.iq = Player.maxiq = Player.pow = Player.maxpow =
            ((Player.maxstr + Player.maxcon + Player.maxdex + Player.maxagi + Player.maxiq +
              Player.maxpow + 12) /
             6);
        toggle_item_use(false); /* FIXED! 12/30/98 */
      }
      else if(num < 30)
      {
        mprint("Your entire body glows with an eerie flickering light.");
        toggle_item_use(true); /* FIXED! 12/30/98 */
        for(i = 1; i < MAXITEMS; i++)
        {
          if(Player.possessions[i] != NULL)
          {
            Player.possessions[i]->plus++;
            if(Player.possessions[i]->objchar == STICK)
            {
              Player.possessions[i]->charge += 10;
            }
            Player.possessions[i]->blessing += 10;
          }
        }
        toggle_item_use(false); /* FIXED! 12/30/98 */
        cleanse(1);
        mprint("You feel filled with energy!");
        Player.maxpow += 5;
        Player.pow += 5;
        Player.mana = Player.maxmana = calcmana() * 5;
        mprint("You also feel weaker. Paradoxical, no?");
        Player.con -= 5;
        Player.maxcon -= 5;
        if(Player.con < 3)
        {
          p_death("congestive heart failure");
        }
      }
      else if(num < 40)
      {
        mprint("Your entire body glows black.");
        dispel(-1);
        dispel(-1);
        Player.pow -= 10;
        Player.mana = 0;
      }
      else if(num < 60)
      {
        mprint("The storm deposits you in a strange place....");
        do
        {
          Player.x = random_range(WIDTH);
          Player.y = random_range(LENGTH);
        } while(Country[Player.x][Player.y].current_terrain_type == CHAOS_SEA && !Player.rank[ADEPT]);
        screencheck(Player.x, Player.y);
      }
      else if(num < 70)
      {
        mprint("A tendril of the storm condenses and falls into your hands.");
        ob = ((pob)checkmalloc(sizeof(objtype)));
        make_artifact(ob, -1);
        gain_item(ob);
      }
      else if(num < 80)
      {
        if(gamestatusp(MOUNTED, GameStatus))
        {
          mprint("Your horse screams as he is transformed into an");
          mprint("imaginary unseen dead tortoise.");
          mprint("You are now on foot.");
          resetgamestatus(MOUNTED, GameStatus);
        }
        else
        {
          mprint("You notice you are riding a horse. Odd. Very odd....");
          mprint("Now that's a horse of a different color!");
          setgamestatus(MOUNTED, GameStatus);
        }
      }
      else if(num < 90)
      {
        mprint("You feel imbued with godlike power....");
        wish(1);
      }
      else if(num < 100)
      {
        mprint("The chaos storm has wiped your memory!");
        mprint("You feel extraordinarily naive....");
        mprint("You can't remember a thing! Not even your name.");
        Player.xp    = 0;
        Player.level = 0;
        for(i = 0; i < NUMRANKS; i++)
        {
          Player.rank[i] = 0;
        }
        for(i = 0; i < NUMSPELLS; i++)
        {
          Spells[i].known = false;
        }
        rename_player();
      }
      else
      {
        mprint("You survive the chaos storm relatively unscathed.");
        mprint("It was a learning experience.");
        gain_experience(1000);
      }
      break;
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
      mprint("An encounter!");
      change_environment(E_TACTICAL_MAP);
      break;
    case 11:
      mprint("You find a Traveller's Aid station with maps of the local area.");
      if(gamestatusp(LOST, GameStatus))
      {
        resetgamestatus(LOST, GameStatus);
        mprint("You know where you are now.");
      }
      for(i = Player.x - 5; i < Player.x + 6; i++)
      {
        for(j = Player.y - 5; j < Player.y + 6; j++)
        {
          if(inbounds(i, j))
          {
            c_set(i, j, SEEN, Country);
            if(Country[i][j].current_terrain_type != Country[i][j].base_terrain_type)
            {
              c_set(i, j, CHANGED, Country);
              Country[i][j].current_terrain_type = Country[i][j].base_terrain_type;
            }
          }
        }
      }
      show_screen();
      break;
    case 12:
      if(!gamestatusp(MOUNTED, GameStatus))
      {
        mprint("You develop blisters....");
        p_damage(1, UNSTOPPABLE, "blisters");
      }
      break;
    case 13:
      mprint("You discover an itch just where you can't scratch it.");
      break;
    case 14:
      mprint("A cosmic ray strikes!");
      p_damage(10, UNSTOPPABLE, "a cosmic ray");
      break;
    case 15:
      mprint("You feel strangely lucky.");
      break;
    case 16:
      mprint("The west wind carries with it a weird echoing voice....");
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
  return (c - 'a' + 'A');
}

/* chance for player to resist magic somehow */
/* hostile_magic ranges in power from 0 (weak) to 10 (strong) */
int magic_resist(int hostile_magic)
{
  if((Player.rank[COLLEGE] + Player.rank[CIRCLE] > 0) &&
     (Player.level / 2 + random_range(20) > hostile_magic + random_range(20)))
  {
    if(Player.mana > hostile_magic * hostile_magic)
    {
      mprint("Thinking fast, you defend youself with a counterspell!");
      Player.mana -= hostile_magic * hostile_magic;
      dataprint();
      return (true);
    }
  }
  if(Player.level / 4 + Player.status[PROTECTION] + random_range(20) > hostile_magic + random_range(30))
  {
    mprint("You resist the spell!");
    return (true);
  }
  else
  {
    return (false);
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
        print2("Along the many paths of nature...");
        break;
      case 1:
        print2("You move swiftly through the wilderness.");
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
        print2("Clippity Clop.");
        break;
      case 2:
        print2("....my spurs go jingle jangle jingle....");
        break;
      case 3:
        print2("....as I go riding merrily along....");
        break;
    }
  }
  else if(Player.possessions[O_BOOTS] && Player.possessions[O_BOOTS]->on_equip == I_BOOTS_7LEAGUE)
  {
    takestime = 0;
    switch(random_range(32))
    {
      case 0:
        print2("Boingg!");
        break;
      case 1:
        print2("Whooosh!");
        break;
      case 2:
        print2("Over hill, over dale....");
        break;
      case 3:
        print2("...able to leap over 7 leagues in a single bound....");
        break;
    }
  }
  else if(Player.status[SHADOWFORM])
  {
    faster = 1;
    switch(random_range(32))
    {
      case 0:
        print2("As swift as a shadow.");
        break;
      case 1:
        print2("\"I walk through the trees...\"");
        break;
    }
  }
  else
  {
    switch(random_range(32))
    {
      case 0:
        print2("Trudge. Trudge.");
        break;
      case 1:
        print2("The road goes ever onward....");
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
      mprint("You have entered the sea of chaos...");
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
        mprint("Well, I guess you know where you are now....");
      }
      locprint("Outside Rampart, the city.");
      break;
    case VILLAGE:
      if(gamestatusp(LOST, GameStatus))
      {
        resetgamestatus(LOST, GameStatus);
        mprint("The village guards let you know where you are....");
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
      mprint("You notice a concealed entrance into the hill.");
      break;
    case CASTLE:
      locprint("Near a fortified castle.");
      if(takestime)
      {
        Time += 60;
        hourly_check();
      }
      mprint("The castle is hewn from solid granite. The drawbridge is down.");
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
      mprint("You notice an entrance conveniently at hand.");
      break;
    case MAGIC_ISLE:
      locprint("A strange island in the midst of the Sea of Chaos.");
      if(takestime)
      {
        Time += 60;
        hourly_check();
      }
      mprint("There is a narrow causeway to the island from here.");
      break;
    case STARPEAK:
      locprint("Star Peak.");
      if(takestime)
      {
        Time += 60;
        hourly_check();
      }
      mprint("The top of the mountain seems to glow with a allochroous aura.");
      break;
    case DRAGONLAIR:
      locprint("A rocky chasm.");
      if(takestime)
      {
        Time += 60;
        hourly_check();
      }
      mprint("You are at a cave entrance from which you see the glint of gold.");
      break;
    case VOLCANO:
      locprint("HellWell Volcano.");
      if(takestime)
      {
        Time += 60;
        hourly_check();
      }
      mprint("A shimmer of heat lightning plays about the crater rim.");
      break;
    default:
      locprint("I haven't any idea where you are!!!");
      break;
  }
  outdoors_random_event();
}

void countrysearch()
{
  int x, y;
  Time += 60;
  hourly_check();
  for(x = Player.x - 1; x < Player.x + 2; x++)
  {
    for(y = Player.y - 1; y < Player.y + 2; y++)
    {
      if(inbounds(x, y))
      {
        if(Country[x][y].current_terrain_type != Country[x][y].base_terrain_type)
        {
          mprint("Your search was fruitful!");
          Country[x][y].current_terrain_type = Country[x][y].base_terrain_type;
          c_set(x, y, CHANGED, Country);
          mprint("You discovered:");
          mprint(countryid(Country[x][y].base_terrain_type));
        }
      }
    }
  }
}

char *countryid(Symbol terrain)
{
  switch(terrain & 0xff)
  {
    case MOUNTAINS & 0xff:
      strcpy(Str1, "Almost impassable mountains");
      break;
    case PLAINS & 0xff:
      strcpy(Str1, "Seemingly endless plains");
      break;
    case TUNDRA & 0xff:
      strcpy(Str1, "A frosty stretch of tundra");
      break;
    case ROAD & 0xff:
      strcpy(Str1, "A paved highway");
      break;
    case PASS & 0xff:
      strcpy(Str1, "A secret mountain pass");
      break;
    case RIVER & 0xff:
      strcpy(Str1, "A rolling river");
      break;
    case CITY & 0xff:
      strcpy(Str1, "The city of Rampart");
      break;
    case VILLAGE & 0xff:
      strcpy(Str1, "A rural village");
      break;
    case FOREST & 0xff:
      strcpy(Str1, "A verdant forest");
      break;
    case JUNGLE & 0xff:
      strcpy(Str1, "A densely overgrown jungle");
      break;
    case SWAMP & 0xff:
      strcpy(Str1, "A swampy fen");
      break;
    case VOLCANO & 0xff:
      strcpy(Str1, "A huge active volcano");
      break;
    case CASTLE & 0xff:
      strcpy(Str1, "An imposing castle");
      break;
    case STARPEAK & 0xff:
      strcpy(Str1, "A mysterious mountain.");
      break;
    case DRAGONLAIR & 0xff:
      strcpy(Str1, "A cavern filled with treasure.");
      break;
    case MAGIC_ISLE & 0xff:
      strcpy(Str1, "An island emanating magic.");
      break;
    case CAVES & 0xff:
      strcpy(Str1, "A hidden cave entrance");
      break;
    case TEMPLE & 0xff:
      strcpy(Str1, "A neoclassical temple");
      break;
    case DESERT & 0xff:
      strcpy(Str1, "A sere desert");
      break;
    case CHAOS_SEA & 0xff:
      strcpy(Str1, "The Sea of Chaos");
      break;
    default:
      strcpy(Str1, "I have no idea.");
      break;
  }
  return (Str1);
}

static const char *sitenames[] = {/* alphabetical listing */
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
                                  "oracle",
                                  "order of paladins",
                                  "pawn shop",
                                  "sorcerors' guild ",
                                  "tavern",
                                  "temple",
                                  "thieves' guild"};

static int sitenums[] = {/* the order matches sitenames[] */
                         L_ALCHEMIST, L_ARENA,       L_ARMORER,      L_BANK,   L_BROTHEL,   L_CASINO,
                         L_CASTLE,    L_COUNTRYSIDE, L_COLLEGE,      L_CONDO,  L_DPW,       L_DINER,
                         L_CLUB,      L_COMMANDANT,  L_GYM,          L_HEALER, L_CHARITY,   L_CRAP,
                         L_LIBRARY,   L_MERC_GUILD,  L_ORACLE,       L_ORDER,  L_PAWN_SHOP, L_SORCERORS,
                         L_TAVERN,    L_TEMPLE,      L_THIEVES_GUILD};

std::vector<std::string> known_sites(int first, int last)
{
  std::vector<std::string> lines;
  int printed = false;
  for(int i = first; i <= last; ++i)
  {
    if(CitySiteList[sitenums[i] - CITYSITEBASE][0])
    {
      printed = true;
      lines.emplace_back(std::string("  ") + sitenames[i]);
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
  char   prefix[80];
  int    found = 0;
  int    f, l;
  int    first          = 0;
  int    last           = NUMCITYSITES - 1;
  size_t pos            = 0;
  bool   menu_displayed = false;
  int    player_input;
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
    if(player_input == KEY_BACKSPACE || player_input == '\b' || player_input == DELETE || player_input == KEY_DC)
    {
      if(pos > 0)
      {
        prefix[--pos] = '\0';
        player_input  = prefix[pos - 1];
        f             = first;
        while(f >= 0 && !strncmp(prefix, sitenames[f], pos))
        {
          if(CitySiteList[sitenums[f] - CITYSITEBASE][0])
          {
            first = f;
          }
          --f;
        }
        l = last;
        while(l < NUMCITYSITES && !strncmp(prefix, sitenames[l], pos))
        {
          if(CitySiteList[sitenums[l] - CITYSITEBASE][0])
          {
            last = l;
          }
          ++l;
        }
        if(found)
        {
          found = 0;
        }
        message_buffer.replace_last(prefix);
      }
      if(pos == 0)
      {
        first = 0;
        last  = NUMCITYSITES - 1;
        found = 0;
      }
    }
    else if(player_input == ESCAPE)
    {
      message_buffer.replace_last("_ Move cancelled.");
      xredraw();
      return ABORT;
    }
    else if(player_input == '?')
    {
      menu_displayed = true;
    }
    else if(player_input != '\n')
    {
      if(player_input >= 'A' && player_input <= 'Z')
      {
        player_input += 'a' - 'A';
      }
      if(found)
      {
        continue;
      }
      f = first;
      l = last;
      while(f < NUMCITYSITES && (!CitySiteList[sitenums[f] - CITYSITEBASE][0] ||
                                 strlen(sitenames[f]) < pos || sitenames[f][pos] < player_input))
      {
        ++f;
      }
      while(l >= 0 && (!CitySiteList[sitenums[l] - CITYSITEBASE][0] || strlen(sitenames[l]) < pos ||
                       sitenames[l][pos] > player_input))
      {
        --l;
      }
      if(l < f)
      {
        continue;
      }
      prefix[pos++] = player_input;
      prefix[pos]   = '\0';
      message_buffer.replace_last(prefix);
      first = f;
      last  = l;
      if(first == last && !found)
      { /* unique name */
        found = 1;
        message_buffer.replace_last(sitenames[first]);
      }
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

/* are there hostile monsters within 2 moves? */
int hostilemonstersnear()
{
  int i, j, hostile = false;

  for(i = Player.x - 2; ((i < Player.x + 3) && (!hostile)); i++)
  {
    for(j = Player.y - 2; ((j < Player.y + 3) && (!hostile)); j++)
    {
      if(inbounds(i, j))
      {
        if(Level->site[i][j].creature != NULL)
        {
          hostile = m_statusp(*Level->site[i][j].creature, HOSTILE);
        }
      }
    }
  }

  return (hostile);
}

/* random effects from some of stones in villages */
/* if alignment of stone is alignment of player, gets done sooner */
int stonecheck(int alignment)
{
  int *stone, match = false, cycle = false, i;

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
      print1("The stone glows grey.");
      print2("Not much seems to happen this time.");
      (*stone)--;
      break;
    case 1:
      print1("The stone glows black");
      print2("A burden has been removed from your shoulders.....");
      print3("Your pack has disintegrated!");
      for(i = 0; i < MAXPACK; i++)
      {
        if(Player.pack[i] != NULL)
        {
          free((char *)Player.pack[i]);
          Player.pack[i] = NULL;
        }
      }
      Player.packptr = 0;
      break;
    case 3:
      print1("The stone glows microwave");
      print2("A vortex of antimana spins about you!");
      dispel(-1);
      break;
    case 5:
      print1("The stone glows infrared");
      print2("A portal opens nearby and an obviously confused monster appears!");
      summon(-1, -1);
      break;
    case 7:
      print1("The stone glows brick red");
      print2("A gold piece falls from the heavens into your money pouch!");
      Player.cash++;
      break;
    case 9:
      print1("The stone glows cherry red");
      print2("A flush of warmth spreads through your body.");
      augment(1);
      break;
    case 11:
      print1("The stone glows orange");
      print2("A flux of energy blasts you!");
      manastorm(Player.x, Player.y, random_range(Player.maxhp) + 1);
      break;
    case 13:
      print1("The stone glows lemon yellow");
      print2("You're surrounded by enemies! You begin to foam at the mouth.");
      Player.status[BERSERK] += 10;
      break;
    case 15:
      print1("The stone glows yellow");
      print2("Oh no! The DREADED AQUAE MORTIS!");
      print2("No, wait, it's just your imagination.");
      break;
    case 17:
      print1("The stone glows chartreuse");
      print2("Your joints stiffen up.");
      Player.agi -= 3;
      break;
    case 19:
      print1("The stone glows green");
      print2("You come down with an acute case of Advanced Leprosy.");
      Player.status[DISEASED] = 1100;
      Player.hp               = 1;
      Player.dex -= 5;
      break;
    case 21:
      print1("The stone glows forest green");
      print2("You feel wonderful!");
      Player.status[HERO] += 10;
      break;
    case 23:
      print1("The stone glows cyan");
      print2("You feel a strange twisting sensation....");
      strategic_teleport(-1);
      break;
    case 25:
      print1("The stone glows blue");
      print1("You feel a tingle of an unearthly intuition:");
      hint();
      break;
    case 27:
      print1("The stone glows navy blue");
      print2("A sudden shock of knowledge overcomes you.");
      identify(1);
      knowledge(1);
      break;
    case 29:
      print1("The stone glows blue-violet");
      print2("You feel forgetful.");
      for(i = 0; i < NUMSPELLS; i++)
      {
        if(Spells[i].known)
        {
          Spells[i].known = false;
          break;
        }
      }
      break;
    case 31:
      print1("The stone glows violet");
      acquire(0);
      break;
    case 33:
      print1("The stone glows deep purple");
      print2("You vanish.");
      Player.status[INVISIBLE] += 10;
      break;
    case 35:
      print1("The stone glows ultraviolet");
      print2("All your hair rises up on end.... A bolt of lightning hits you!");
      p_damage(random_range(Player.maxhp), ELECTRICITY, "mystic lightning");
      break;
    case 37:
      print1("The stone glows roentgen");
      print2("You feel more experienced.");
      gain_experience((Player.level + 1) * 250);
      break;
    case 39:
      print1("The stone glows gamma");
      print2("Your left hand shines silvery, and your right emits a golden aura.");
      enchant(1);
      bless(1);
      print3("Your hands stop glowing.");
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
      print1("The stone glows cosmic!");
      print2("The stone's energy field quiets for a moment...");
      *stone = 50;
      cycle  = true;
      break;
    default:
      print1("The stone glows polka-dot (?!?!?!?)");
      print2("You feel a strange twisting sensation....");
      *stone = 0;
      strategic_teleport(-1);
      break;
  }
  calc_melee();
  return (cycle);
}

void alert_guards()
{
  int foundguard = false;
  pml ml;
  int suppress = 0;
  for(ml = Level->mlist; ml != NULL; ml = ml->next)
  {
    if(((ml->m->id == GUARD) || ((ml->m->id == HISCORE_NPC) && (ml->m->aux2 == 15))) && /*justiciar*/
       (ml->m->hp > 0))
    {
      foundguard = true;
      m_status_set(*ml->m, AWAKE);
      m_status_set(*ml->m, HOSTILE);
    }
  }
  if(foundguard)
  {
    mprint("You hear a whistle and the sound of running feet!");
    if(Current_Environment == E_CITY)
    {
      Level->site[40][60].p_locf = L_NO_OP; /* pacify_guards restores this */
    }
  }
  if((!foundguard) && (Current_Environment == E_CITY) && !gamestatusp(DESTROYED_ORDER, GameStatus))
  {
    suppress = gamestatusp(SUPPRESS_PRINTING, GameStatus);
    resetgamestatus(SUPPRESS_PRINTING, GameStatus);
    print2("The last member of the Order of Paladins dies....");
    gain_experience(1000);
    Player.alignment -= 250;
    if(!gamestatusp(KILLED_LAWBRINGER, GameStatus))
    {
      print1("A chime sounds from far away.... The sound grows stronger....");
      print2("Suddenly the great shadowy form of the LawBringer appears over");
      print3("the city. He points his finger at you....");
      print1("\"Cursed art thou, minion of chaos! May thy strength fail thee");
      print2("in thy hour of need!\" You feel an unearthly shiver as the");
      print3("LawBringer waves his palm across the city skies....");
      Player.str /= 2;
      dataprint();
      print1("You hear a bell tolling, and eerie moans all around you....");
      print2("Suddenly, the image of the LawBringer is gone.");
      print3("You hear a guardsman's whistle in the distance!");
      resurrect_guards();
    }
    else
    {
      print1("The Order's magical defenses have dropped, and the");
      print2("Legions of Chaos strike....");
      print1("The city shakes! An earthquake has struck!");
      print2("Cracks open in the street, and a chasm engulfs the Order HQ!");
      print3("Flames lick across the sky and you hear wild laughter....");
      gain_experience(5000);
      destroy_order();
    }
  }
  if(suppress)
  {
    resetgamestatus(SUPPRESS_PRINTING, GameStatus);
  }
}

/* can only occur when player is in city, so OK to use Level */
void destroy_order()
{
  int i, j;
  setgamestatus(DESTROYED_ORDER, GameStatus);
  if(Level != City)
  {
    print1("Zounds! A Serious Mistake!");
  }
  else
  {
    for(i = 35; i < 46; i++)
    {
      for(j = 60; j < 63; j++)
      {
        if(i == 40 && (j == 60 || j == 61))
        {
          lreset(i, j, SECRET, *Level);
          Level->site[i][j].locchar = FLOOR;
          Level->site[i][j].p_locf  = L_NO_OP;
          lset(i, j, CHANGED, *Level);
        }
        else
        {
          Level->site[i][j].locchar = RUBBLE;
          Level->site[i][j].p_locf  = L_RUBBLE;
          lset(i, j, CHANGED, *Level);
        }
        if(Level->site[i][j].creature != NULL)
        {
          Level->site[i][j].creature->hp = -1;
          Level->site[i][j].creature     = NULL;
        }
        make_site_monster(i, j, GHOST);
        Level->site[i][j].creature->monstring = "ghost of a Paladin";
        m_status_set(*Level->site[i][j].creature, HOSTILE);
      }
    }
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
