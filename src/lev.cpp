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

/* lev.c */

#include "glob.h"

#include <cassert>
#include <format>

/* Functions dealing with dungeon and country levels aside from actual
level structure generation */

/* monsters for tactical encounters */
void make_country_monsters(chtype terrain)
{
  static int plains[10] = {BUNNY, BUNNY, HORNET, QUAIL, HAWK, DEER, WOLF, LION, BRIGAND, RANDOM};
  /*    {BUNNY,BUNNY,BLACKSNAKE,HAWK,IMPALA,WOLF,LION,BRIGAND,RANDOM};*/
  /* DG changed (WDT: I'd like to see a blacksnake). */
  static int forest[10]   = {BUNNY, QUAIL, HAWK, BADGER, DEER, DEER, WOLF, BEAR, BRIGAND, RANDOM};
  static int jungle[10]   = {ANTEATER, PARROT, MAMBA, ANT, ANT, HYENA, HYENA, ELEPHANT, LION, RANDOM};
  static int river[10]    = {QUAIL, TROUT, TROUT, MANOWAR, BASS, BASS, CROC, CROC, BRIGAND, RANDOM};
  static int swamp[10]    = {BASS, BASS, CROC, CROC, BOGTHING, ANT, ANT, RANDOM, RANDOM, RANDOM};
  static int desert[10]   = {HAWK, HAWK, CAMEL, CAMEL, HYENA, HYENA, LION, LION, RANDOM, RANDOM};
  static int tundra[10]   = {WOLF, WOLF, BEAR, BEAR, DEER, DEER, RANDOM, RANDOM, RANDOM, RANDOM};
  static int mountain[10] = {BUNNY, SHEEP, WOLF, WOLF, HAWK, HAWK, HAWK, RANDOM, RANDOM, RANDOM};
  int *monsters, nummonsters;

  nummonsters = (random_range(5) + 1) * (random_range(3) + 1);

  switch(terrain)
  {
    case PLAINS:
      monsters = plains;
      break;
    case FOREST:
      monsters = forest;
      break;
    case JUNGLE:
      monsters = jungle;
      break;
    case RIVER:
      monsters = river;
      break;
    case SWAMP:
      monsters = swamp;
      break;
    case MOUNTAINS:
    case PASS:
    case VOLCANO:
      monsters = mountain;
      break;
    case DESERT:
      monsters = desert;
      break;
    case TUNDRA:
      monsters = tundra;
      break;
    default:
      monsters = nullptr;
  }
  for(int i = 0; i < nummonsters; ++i)
  {
    std::unique_ptr<monster> m;
    if(!monsters)
    {
      m = m_create(random_range(WIDTH), random_range(LENGTH), true, difficulty());
    }
    else
    {
      m    = make_creature(*(monsters + random_range(10)));
      m->x = random_range(WIDTH);
      m->y = random_range(LENGTH);
    }
    Level->site[m->x][m->y].creature = m.get();
    m->sense                         = WIDTH;
    if(m_statusp(*m, ONLYSWIM))
    {
      Level->site[m->x][m->y].locchar = WATER;
      Level->site[m->x][m->y].p_locf  = L_WATER;
      lset(m->x, m->y, CHANGED, *Level);
    }

    Level->mlist.push_front(std::move(m));
  }
}

/* monstertype is more or less Current_Dungeon */
/* The caves and sewers get harder as you penetrate them; the castle
is completely random, but also gets harder as it is explored;
the astral and the volcano just stay hard... */
void populate_level(int monstertype)
{
  int nummonsters = (random_range(difficulty() / 3) + 1) * 3 + 8;

  if(monstertype == E_CASTLE)
  {
    nummonsters += 10;
  }
  else if(monstertype == E_ASTRAL)
  {
    nummonsters += 10;
  }
  else if(monstertype == E_VOLCANO)
  {
    nummonsters += 20;
  }

  int x, y, monsterid;
  for(int k = 0; k < nummonsters; ++k)
  {
    findspace(&x, &y, -1);

    switch(monstertype)
    {
      case E_CAVES:
        if(Level->depth * 10 + random_range(100) > 150)
        {
          monsterid = GOBLIN_SHAMAN;
        }
        else if(Level->depth * 10 + random_range(100) > 100)
        {
          monsterid = GOBLIN_CHIEF; // Goblin Chieftain
        }
        else if(random_range(100) > 50)
        {
          monsterid = GOBLIN;
        }
        else
        {
          monsterid = RANDOM; // IE, random monster
        }
        break;
      case E_SEWERS:
        if(!random_range(3))
        {
          monsterid = -1;
        }
        else
        {
          switch(random_range(Level->depth + 3))
          {
            case 0:
              monsterid = SEWER_RAT;
              break;
            case 1:
              monsterid = AGGRAVATOR;
              break; // aggravator fungus
            case 2:
              monsterid = BLIPPER;
              break; // blipper rat
            case 3:
              monsterid = NIGHT_GAUNT;
              break;
            case 4:
              monsterid = NASTY;
              break; // transparent nasty
            case 5:
              monsterid = MURK;
              break; // murk fungus
            case 6:
              monsterid = CATOBLEPAS;
              break;
            case 7:
              monsterid = ACID_CLOUD;
              break;
            case 8:
              monsterid = DENEBIAN;
              break; // denebian slime devil
            case 9:
              monsterid = CROC;
              break; // giant crocodile
            case 10:
              monsterid = TESLA;
              break; // tesla monster
            case 11:
              monsterid = SHADOW;
              break; // shadow spirit
            case 12:
              monsterid = BOGTHING;
              break; // bogthing
            case 13:
              monsterid = WATER_ELEM;
              break; // water elemental
            case 14:
              monsterid = TRITON;
              break;
            case 15:
              monsterid = ROUS;
              break;
            default:
              monsterid = RANDOM;
              break; // whatever seems good
          }
        }
        break;
      case E_ASTRAL:
        if(random_range(2))
        { // random astral creatures
          switch(random_range(12))
          {
            case 0:
              monsterid = THOUGHTFORM;
              break;
            case 1:
              monsterid = FUZZY;
              break; // astral fuzzy
            case 2:
              monsterid = BAN_SIDHE;
              break;
            case 3:
              monsterid = GRUE;
              break; // astral grue
            case 4:
              monsterid = SHADOW;
              break; // shadow spirit
            case 5:
              monsterid = ASTRAL_VAMP;
              break; // astral vampire
            case 6:
              monsterid = MANABURST;
              break;
            case 7:
              monsterid = RAKSHASA;
              break;
            case 8:
              monsterid = ILL_FIEND;
              break; // illusory fiend
            case 9:
              monsterid = MIRRORMAST;
              break; // mirror master
            case 10:
              monsterid = ELDER_GRUE;
              break; // elder etheric grue
            default:
              monsterid = SHADOW_SLAY; // shadow slayer
          }
        }
        else if(random_range(2) && Level->depth == 1)
        {                         // plane of earth
          monsterid = EARTH_ELEM; // earth elemental
        }
        else if(random_range(2) && Level->depth == 2)
        {                       // plane of air
          monsterid = AIR_ELEM; // air elemental
        }
        else if(random_range(2) && Level->depth == 3)
        {                         // plane of water
          monsterid = WATER_ELEM; // water elemental
        }
        else if(random_range(2) && Level->depth == 4)
        {                        // plane of fire
          monsterid = FIRE_ELEM; // fire elemental
        }
        else if(random_range(2) && (Level->depth == 5))
        { // deep astral
          switch(random_range(12))
          {
            case 0:
              monsterid = NIGHT_GAUNT;
              break;
            case 1:
              monsterid = SERV_LAW;
              break; // servant of law
            case 2:
              monsterid = SERV_CHAOS;
              break; // servant of chaos
            case 3:
              monsterid = FROST_DEMON;
              break; // lesser frost demon
            case 4:
              monsterid = OUTER_DEMON;
              break; // outer circle demon
            case 5:
              monsterid = DEMON_SERP;
              break; // demon serpent
            case 6:
              monsterid = ANGEL;
              break;
            case 7:
              monsterid = INNER_DEMON;
              break; // inner circle demon
            case 8:
              monsterid = FDEMON_L;
              break; // frost demon lord
            case 9:
              monsterid = HIGH_ANGEL;
              break;
            case 10:
              monsterid = DEMON_PRINCE;
              break; // prime circle demon
            default:
              monsterid = ARCHANGEL;
          }
        }
        else
        {
          monsterid = RANDOM;
        }
        break;
      case E_VOLCANO:
        if(random_range(2))
        {
          do
          {
            monsterid = random_range(ML10 - ML4) + ML4;
          } while(Monsters[monsterid].uniqueness != COMMON);
        }
        else
        {
          switch(random_range(Level->depth / 2 + 2))
          { // evil & fire creatures
            case 0:
              monsterid = HAUNT;
              break;
            case 1:
              monsterid = INCUBUS;
              break;
            case 2:
              monsterid = DRAGONETTE;
              break;
            case 3:
              monsterid = FROST_DEMON;
              break;
            case 4:
              monsterid = SPECTRE;
              break;
            case 5:
              monsterid = LAVA_WORM;
              break;
            case 6:
              monsterid = FIRE_ELEM;
              break;
            case 7:
              monsterid = LICHE;
              break;
            case 8:
              monsterid = RAKSHASA;
              break;
            case 9:
              monsterid = DEMON_SERP;
              break;
            case 10:
              monsterid = NAZGUL;
              break;
            case 11:
              monsterid = FLAME_DEV;
              break;
            case 12:
              monsterid = LOATHLY;
              break;
            case 13:
              monsterid = ZOMBIE;
              break;
            case 14:
              monsterid = INNER_DEMON;
              break;
            case 15:
              monsterid = BAD_FAIRY;
              break;
            case 16:
              monsterid = DRAGON;
              break;
            case 17:
              monsterid = FDEMON_L;
              break;
            case 18:
              monsterid = SHADOW_SLAY;
              break;
            case 19:
              monsterid = DEATHSTAR;
              break;
            case 20:
              monsterid = VAMP_LORD;
              break;
            case 21:
              monsterid = DEMON_PRINCE;
              break;
            default:
              monsterid = RANDOM;
              break;
          }
        }
        break;
      case E_CASTLE:
        if(random_range(4) == 1)
        {
          if(difficulty() < 5)
          {
            monsterid = ENCHANTOR;
          }
          else if(difficulty() < 6)
          {
            monsterid = NECROMANCER;
          }
          else if(difficulty() < 8)
          {
            monsterid = FIRE_ELEM;
          }
          else
          {
            monsterid = THAUMATURGIST;
          }
        }
        else
        {
          monsterid = RANDOM;
        }
        break;

      default:
        monsterid = RANDOM;
        break;
    }

    assert(RANDOM == -1); // WDT: the following test slightly assumes this
    std::unique_ptr<monster> m;
    if(monsterid > RANDOM)
    {
      m = make_creature(monsterid);
      Level->site[x][y].creature = m.get();
    }
    else
    {
      m = m_create(x, y, true, difficulty());
      Level->site[x][y].creature = m.get();
    }

    Level->site[x][y].creature->x = x;
    Level->site[x][y].creature->y = y;

    if(m_statusp(*Level->site[x][y].creature, ONLYSWIM))
    {
      Level->site[x][y].locchar = WATER;
      Level->site[x][y].p_locf  = L_WATER;
      lset(x, y, CHANGED, *Level);
    }

    Level->mlist.push_front(std::move(m));
  }
}

// Add a wandering monster possibly
void wandercheck()
{
  if(random_range(MaxDungeonLevels) < difficulty())
  {
    int x, y;
    findspace(&x, &y, -1);
    std::unique_ptr<monster> m = m_create(x, y, WANDERING, difficulty());
    Level->site[x][y].creature = m.get();
    Level->mlist.push_front(std::move(m));
  }
}

// call make_creature and place created monster on Level->mlist and Level
void make_site_monster(int i, int j, int mid)
{
  std::unique_ptr<monster> m;
  if(mid > -1)
  {
    m = make_creature(mid);
  }
  else
  {
    m = m_create(i, j, WANDERING, difficulty());
  }
  m->x                       = i;
  m->y                       = j;
  Level->site[i][j].creature = m.get();
  Level->mlist.push_front(std::move(m));
}

/* make and return an appropriate monster for the level and depth*/
/* called by populate_level, doesn't actually add to mlist for some reason*/
/* eventually to be more intelligent */
std::unique_ptr<monster> m_create(int x, int y, int kind, int level)
{
  int monster_range;

  switch(level)
  {
    case 0:
      monster_range = ML1;
      break;
    case 1:
      monster_range = ML2;
      break;
    case 2:
      monster_range = ML3;
      break;
    case 3:
      monster_range = ML4;
      break;
    case 4:
      monster_range = ML5;
      break;
    case 5:
      monster_range = ML6;
      break;
    case 6:
      monster_range = ML7;
      break;
    case 7:
      monster_range = ML8;
      break;
    case 8:
      monster_range = ML9;
      break;
    case 9:
      monster_range = ML10;
      break;
    default:
      monster_range = NUMMONSTERS;
      break;
  }

  int mid;
  do
  {
    mid = random_range(monster_range);
  } while(Monsters[mid].uniqueness != COMMON);
  std::unique_ptr<monster> newmonster = make_creature(mid);

  // no duplicates of unique monsters
  if(kind == WANDERING)
  {
    m_status_set(*newmonster, WANDERING);
  }
  newmonster->x = x;
  newmonster->y = y;
  return newmonster;
}

// make creature # mid, totally random if mid == -1
// make creature allocates space for the creature
std::unique_ptr<monster> make_creature(int mid)
{
  auto newmonster = std::make_unique<monster>();

  if(mid == -1)
  {
    mid = random_range(ML9);
  }
  *newmonster = Monsters[mid];
  if((mid == ANGEL) || (mid == HIGH_ANGEL) || (mid == ARCHANGEL))
  {
    /* aux1 field of an angel is its deity */
    if(Current_Environment == E_TEMPLE)
    {
      newmonster->aux1 = Country[LastCountryLocX][LastCountryLocY].aux;
    }
    else
    {
      newmonster->aux1 = random_range(6) + 1;
    }
    std::string angel_name;
    switch(newmonster->aux1)
    {
      case ODIN:
        angel_name = std::format("{} of Odin", Monsters[mid].monstring);
        break;
      case SET:
        angel_name = std::format("{} of Set", Monsters[mid].monstring);
        break;
      case HECATE:
        angel_name = std::format("{} of Hecate", Monsters[mid].monstring);
        break;
      case ATHENA:
        angel_name = std::format("{} of Athena", Monsters[mid].monstring);
        break;
      case DESTINY:
        angel_name = std::format("{} of Destiny", Monsters[mid].monstring);
        break;
      case DRUID:
        angel_name = std::format("{} of Balance", Monsters[mid].monstring);
        break;
    }
    newmonster->monstring = angel_name;
  }
  else if(mid == ZERO_NPC || mid == WEREHUMAN)
  {
    /* generic 0th level human, or a were-human */
    newmonster->monstring = mantype();
    newmonster->corpsestr = std::format("dead {}", newmonster->monstring);
  }
  else if((newmonster->monchar & 0xff) == '!')
  {
    /* the nymph/satyr and incubus/succubus */
    if(random_range(2))
    {
      newmonster->monchar   = 'n' | CLR(RED);
      newmonster->monstring = "nymph";
      newmonster->corpsestr = "dead nymph";
    }
    else
    {
      newmonster->monchar   = 's' | CLR(RED);
      newmonster->monstring = "satyr";
      newmonster->corpsestr = "dead satyr";
    }
    if(newmonster->id == INCUBUS)
    {
      if((newmonster->monchar & 0xff) == 'n')
      {
        newmonster->corpsestr = "dead succubus";
      }
      else
      {
        newmonster->corpsestr = "dead incubus";
      }
    }
  }
  if(mid == NPC)
  {
    make_log_npc(newmonster.get());
  }
  else if(mid == HISCORE_NPC)
  {
    make_hiscore_npc(newmonster.get(), random_range(15));
  }
  else
  {
    if(newmonster->sleep < random_range(100))
    {
      m_status_set(*newmonster, AWAKE);
    }
    if(newmonster->startthing > -1 && Objects[newmonster->startthing].uniqueness <= UNIQUE_MADE)
    {
      m_pickup(newmonster.get(), std::make_unique<object>(Objects[newmonster->startthing]));
    }
    int treasures = random_range(newmonster->treasure);
    for(int i = 0; i < treasures; ++i)
    {
      std::unique_ptr<object> o;
      do
      {
        o = create_object(newmonster->level);
        if(o->uniqueness != COMMON)
        {
          Objects[o->id].uniqueness = UNIQUE_UNMADE;
          o.reset();
        }
      } while(!o);
      m_pickup(newmonster.get(), std::move(o));
    }
  }
  newmonster->click = (Tick + 1) % 50;
  return newmonster;
}

// drop treasures randomly onto level
void stock_level()
{
  int x, y, numtreasures = 2 * random_range(difficulty() / 4) + 4;

  // put cash anywhere, including walls, put other treasures only on floor
  for(int k = 0; k < numtreasures + 10; ++k)
  {
    do
    {
      x = random_range(WIDTH);
      y = random_range(LENGTH);
    } while(Level->site[x][y].locchar != FLOOR);
    make_site_treasure(x, y, difficulty());
    x         = random_range(WIDTH);
    y         = random_range(LENGTH);
    auto o = std::make_unique<object>();
    make_cash(o.get(), difficulty());
    Level->site[x][y].things.push_front(std::move(o));
    // caves have more random cash strewn around
    if(Current_Dungeon == E_CAVES)
    {
      x = random_range(WIDTH);
      y = random_range(LENGTH);
      o = std::make_unique<object>();
      make_cash(o.get(), difficulty());
      Level->site[x][y].things.push_front(std::move(o));
      x = random_range(WIDTH);
      y = random_range(LENGTH);
      o = std::make_unique<object>();
      make_cash(o.get(), difficulty());
      Level->site[x][y].things.push_front(std::move(o));
    }
  }
}

// make a new object (of at most level itemlevel) at site i,j on level
void make_site_treasure(int i, int j, int itemlevel)
{
  Level->site[i][j].things.push_front(create_object(itemlevel));
}

/* make a specific new object at site i,j on level*/
void make_specific_treasure(int i, int j, int itemid)
{
  if(Objects[itemid].uniqueness == UNIQUE_TAKEN)
  {
    return;
  }
  Level->site[i][j].things.push_front(std::make_unique<object>(Objects[itemid]));
}

/* returns a "level of difficulty" based on current environment
   and depth in dungeon. Is somewhat arbitrary. value between 1 and 10.
   May not actually represent real difficulty, but instead level
   of items, monsters encountered.    */
int difficulty()
{
  int depth = 1;
  if(Level)
  {
    depth = Level->depth;
  }
  switch(Current_Environment)
  {
    case E_COUNTRYSIDE:
      return (7);
    case E_CITY:
      return (3);
    case E_VILLAGE:
      return (1);
    case E_TACTICAL_MAP:
      return (7);
    case E_SEWERS:
      return (depth / 6) + 3;
    case E_CASTLE:
      return (depth / 4) + 4;
    case E_CAVES:
      return (depth / 3) + 1;
    case E_VOLCANO:
      return (depth / 4) + 5;
    case E_ASTRAL:
      return (8);
    case E_ARENA:
      return (5);
    case E_HOVEL:
      return (3);
    case E_MANSION:
      return (7);
    case E_HOUSE:
      return (5);
    case E_DLAIR:
      return (9);
    case E_ABYSS:
      return (10);
    case E_STARPEAK:
      return (9);
    case E_CIRCLE:
      return (8);
    case E_MAGIC_ISLE:
      return (8);
    case E_TEMPLE:
      return (8);
    default:
      return (3);
  }
}
