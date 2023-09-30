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

// save.cpp

#include "file.h"
#include "glob.h"
#include "spell.h"

#include <filesystem>
#include <format>
#include <fstream>
#include <string>

#ifdef SAVE_LEVELS
plv msdos_changelevel(plv oldlevel, int newenv, int newdepth);
#endif

extern void queue_message(const std::string &message);

// Various functions for doing game saves and restores
// The game remembers various player information, the city level,
// the country level, and the last or current dungeon level

// SAVE FUNCTIONS

// saves game on SIGHUP
void signalsave(int)
{
  save_game("Omega.sav");
  queue_message("Signal - Saving file 'Omega.sav'.");
  endgraf();
  exit(0);
}

// Save o unless it's null, then save a special flag byte instead
// Use other values of flag byte to indicate what strings are saved
void save_item(std::ofstream &save_file, object *o)
{
  unsigned char type;
  if(!o)
  {
    type = 0xff;
    file_write(save_file, type);
  }
  else
  {
    type = 0;
    if(o->objstr != Objects[o->id].objstr)
    {
      type |= 1;
    }
    if(o->truename != Objects[o->id].truename)
    {
      type |= 2;
    }
    if(o->cursestr != Objects[o->id].cursestr)
    {
      type |= 4;
    }
    file_write(save_file, type);
    file_write(save_file, *o);
    if(type & 1)
    {
      file_write(save_file, o->objstr.size());
      save_file.write(reinterpret_cast<char *>(o->objstr.data()), o->objstr.size());
    }
    if(type & 2)
    {
      file_write(save_file, o->truename.size());
      save_file.write(reinterpret_cast<char *>(o->truename.data()), o->truename.size());
    }
    if(type & 4)
    {
      file_write(save_file, o->cursestr.size());
      save_file.write(reinterpret_cast<char *>(o->cursestr.data()), o->cursestr.size());
    }
  }
}

void save_itemlist(std::ofstream &save_file, objectlist *ol)
{
  int num_items = 0;
  for(objectlist *tol = ol; tol; tol = tol->next)
  {
    ++num_items;
  }
  file_write(save_file, num_items);
  for(objectlist *tol = ol; tol; tol = tol->next)
  {
    save_item(save_file, tol->thing);
  }
}

// also saves some globals like Level->depth...
void save_player(std::ofstream &save_file)
{
  // Save random global state information
  Player.click = (Tick + 1) % 60;
  file_write(save_file, Player);
  file_write(save_file, Password.size());
  save_file.write(reinterpret_cast<char *>(Password.data()), Password.size());
  file_write(save_file, Player.name.size());
  save_file.write(reinterpret_cast<char *>(Player.name.data()), Player.name.size());
  file_write(save_file, Player.meleestr.size());
  save_file.write(reinterpret_cast<char *>(Player.meleestr.data()), Player.meleestr.size());
  file_write(save_file, CitySiteList);
  file_write(save_file, GameStatus);
  file_write(save_file, Current_Environment);
  file_write(save_file, Last_Environment);
  file_write(save_file, Current_Dungeon);
  file_write(save_file, Villagenum);
  file_write(save_file, Verbosity);
  file_write(save_file, Time);
  file_write(save_file, Tick);
  file_write(save_file, Searchnum);
  file_write(save_file, Behavior);
  file_write(save_file, Phase);
  file_write(save_file, Date);
  file_write(save_file, Spellsleft);
  file_write(save_file, SymbolUseHour);
  file_write(save_file, ViewHour);
  file_write(save_file, HelmHour);
  file_write(save_file, Constriction);
  file_write(save_file, Blessing);
  file_write(save_file, LastDay);
  file_write(save_file, RitualHour);
  file_write(save_file, Lawstone);
  file_write(save_file, Chaostone);
  file_write(save_file, Mindstone);
  file_write(save_file, Arena_Opponent);
  file_write(save_file, Imprisonment);
  file_write(save_file, Gymcredit);
  file_write(save_file, Balance);
  file_write(save_file, StarGemUse);
  file_write(save_file, HiMagicUse);
  file_write(save_file, HiMagic);
  file_write(save_file, FixedPoints);
  file_write(save_file, LastCountryLocX);
  file_write(save_file, LastCountryLocY);
  file_write(save_file, LastTownLocX);
  file_write(save_file, LastTownLocY);
  file_write(save_file, Pawndate);
  for(const spell &s : spell::Spells)
  {
    file_write(save_file, s.powerdrain);
    file_write(save_file, s.known);
  }
  file_write(save_file, Command_Duration);
  file_write(save_file, Precipitation);
  file_write(save_file, Lunarity);
  file_write(save_file, ZapHour);
  file_write(save_file, RitualRoom);

  // stuff which used to be statics
  file_write(save_file, twiddle);
  file_write(save_file, saved);
  file_write(save_file, onewithchaos);
  file_write(save_file, club_hinthour);
  file_write(save_file, winnings);
  file_write(save_file, tavern_hinthour);
  file_write(save_file, scroll_ids);
  file_write(save_file, potion_ids);
  file_write(save_file, stick_ids);
  file_write(save_file, ring_ids);
  file_write(save_file, cloak_ids);
  file_write(save_file, boot_ids);
  file_write(save_file, deepest);
  file_write(save_file, level_seed);
  file_write(save_file, received_directions);

  // Save player possessions
  if(Player.possessions[O_READY_HAND] == Player.possessions[O_WEAPON_HAND])
  {
    Player.possessions[O_READY_HAND] = nullptr;
  }
  for(int i = 0; i < MAXITEMS; ++i)
  {
    save_item(save_file, Player.possessions[i]);
  }
  for(int i = 0; i < MAXPACK; ++i)
  {
    save_item(save_file, Player.pack[i]);
  }
  for(int i = 0; i < PAWNITEMS; ++i)
  {
    save_item(save_file, Pawnitems[i]);
  }

  // Save items in condo vault
  save_itemlist(save_file, Condoitems);

  // Save player item knowledge
  for(int i = 0; i < TOTALITEMS; ++i)
  {
    file_write(save_file, Objects[i].known);
    file_write(save_file, Objects[i].uniqueness);
  }
}

void save_country(std::ofstream &save_file)
{
  int x, y;
  for(x = 0; x < MAXWIDTH; ++x)
  {
    for(y = 0; y < MAXLENGTH; ++y)
    {
      if(c_statusp(x, y, CHANGED, Country))
      {
        file_write(save_file, x);
        file_write(save_file, y);
        file_write(save_file, Country[x][y]);
      }
    }
  }
  file_write(save_file, x);
  file_write(save_file, y);

  // since we don't mark the 'seen' bits as CHANGED, need to save a bitmask
  unsigned long int mask = 0;
  size_t run = 8 * sizeof(mask);
  for(x = 0; x < MAXWIDTH; x++)
  {
    for(y = 0; y < MAXLENGTH; y++)
    {
      if(run == 0)
      {
        run = 8 * sizeof(mask);
        file_write(save_file, mask);
        mask = 0;
      }
      mask >>= 1;
      if(c_statusp(x, y, SEEN, Country))
      {
        mask |= (1L << (sizeof(mask) * 8 - 1));
      }
      --run;
    }
  }
  if(run < 8 * sizeof(mask))
  {
    file_write(save_file, mask);
  }
}

void save_monsters(std::ofstream &save_file, monsterlist *ml)
{
  // First count monsters
  int num_monsters = 0;
  for(monsterlist *tml = ml; tml; tml = tml->next)
  {
    if(tml->m->hp > 0)
    {
      ++num_monsters;
    }
  }
  file_write(save_file, num_monsters);

  // Now save monsters
  for(monsterlist *tml = ml; tml; tml = tml->next)
  {
    if(tml->m->hp > 0)
    {
      file_write(save_file, *tml->m);
      if(tml->m->id != HISCORE_NPC)
      {
        unsigned char type = 0x0;
        if(tml->m->monstring != Monsters[tml->m->id].monstring)
        {
          type |= 0x1;
        }
        if(tml->m->corpsestr != Monsters[tml->m->id].corpsestr)
        {
          type |= 0x2;
        }
        file_write(save_file, type);
        if(type & 1)
        {
          file_write(save_file, tml->m->monstring.size());
          save_file.write(reinterpret_cast<char *>(tml->m->monstring.data()), tml->m->monstring.size());
        }
        if(type & 2)
        {
          file_write(save_file, tml->m->corpsestr.size());
          save_file.write(reinterpret_cast<char *>(tml->m->corpsestr.data()), tml->m->corpsestr.size());
        }
      }
      save_itemlist(save_file, tml->m->possessions);
    }
  }
}

void save_level(std::ofstream &save_file, plv level)
{
  file_write(save_file, level->depth);
  file_write(save_file, level->numrooms);
  file_write(save_file, level->tunnelled);
  file_write(save_file, level->environment);
  int x, y;
  for(y = 0; y < MAXLENGTH; ++y)
  {
    for(x = 0; x < MAXWIDTH; ++x)
    {
      // this loc has been changed
      if(level->site[x][y].lstatus & CHANGED)
      {
        // find how many in a row
        int run;
        for(run = x + 1; run < MAXWIDTH && level->site[run][y].lstatus & CHANGED; ++run) {}
        file_write(save_file, x);
        file_write(save_file, y);
        file_write(save_file, run);
        for(; x < run; ++x)
        {
          file_write(save_file, Level->site[x][y]);
        }
      }
    }
  }
  file_write(save_file, x);
  file_write(save_file, y);
  // since we don't mark the 'seen' bits as CHANGED, need to save a bitmask
  unsigned long int mask = 0;
  size_t run  = 8 * sizeof(mask);
  for(y = 0; y < MAXLENGTH; ++y)
  {
    for(x = 0; x < MAXWIDTH; ++x)
    {
      if(run == 0)
      {
        run = 8 * sizeof(mask);
        file_write(save_file, mask);
        mask = 0;
      }
      mask >>= 1;
      if(level->site[x][y].lstatus & SEEN)
      {
        mask |= (1L << (sizeof(mask) * 8 - 1));
      }
      --run;
    }
  }
  if(run < 8 * sizeof(mask))
  {
    file_write(save_file, mask);
  }
  save_monsters(save_file, level->mlist);
  for(x = 0; x < MAXWIDTH; ++x)
  {
    for(y = 0; y < MAXLENGTH; ++y)
    {
      if(level->site[x][y].things)
      {
        file_write(save_file, x);
        file_write(save_file, y);
        save_itemlist(save_file, level->site[x][y].things);
      }
    }
  }
  file_write(save_file, x);
  file_write(save_file, y);
}

// Checks to see if save file already exists.
// Checks to see if save file can be opened for write.
// The player, the city level, and the current dungeon level are saved.
bool save_game(const std::string &save_file_name)
{
#ifdef SAVE_LEVELS
  int tmpdepth;
#endif

  bool writeok = true;
  if(std::filesystem::exists(save_file_name))
  {
    queue_message( "Save file already exists. Overwrite it? [yn] ");
    writeok = (ynq() == 'y');

  }
  std::ofstream save_file{save_file_name, std::ios::binary};
  if(!save_file)
  {
    writeok = false;
    queue_message(" Error opening file.");
  }

  if(!writeok)
  {
    queue_message("Save aborted.");
    return false;
  }

  queue_message("Saving Game....");

  file_write(save_file, VERSION);
  save_player(save_file);
  save_country(save_file);

#ifdef SAVE_LEVELS
  tmpdepth = Level->depth;
  City     = msdos_changelevel(Level, E_CITY, 0);
#endif

  save_level(save_file, City);

  level *save;
  if(Current_Environment == E_CITY || Current_Environment == E_COUNTRYSIDE)
  {
    save = Dungeon;
  }
  else if(Current_Environment == Current_Dungeon)
  {
    save = Dungeon;
  }
  else
  {
    save = Level;
  }
  int i;
  level *current;
  for(i = 0, current = save; current; current = current->next, i++) {}
  file_write(save_file, i);

#ifdef SAVE_LEVELS
  Level = msdos_changelevel(nullptr, Current_Environment, tmpdepth);
#endif

  for(current = save; current; current = current->next)
  {
    if(current != Level)
    {
      save_level(save_file, current);
    }
  }
  if(save)
  {
    save_level(save_file, Level); // put current level last
  }
  save_file.close();
  if(!save_file.fail())
  {
    queue_message("Game Saved.");
  }
  else
  {
    queue_message("Something didn't work... save aborted.");
    return false;
  }
  return true;
}

// returns true if the given version can be restored by this version
bool ok_outdated(int savefile_version)
{
  int savefile_major_version = savefile_version / 10000;
  int savefile_minor_version = (savefile_version - savefile_major_version) / 100;
  int game_major_version = VERSION / 10000;
  int game_minor_version = (VERSION - game_major_version) / 100;
  if(game_major_version == savefile_major_version && game_minor_version == savefile_minor_version)
  {
    return true;
  }
  else
  {
    return false;
  }
}

// Restore an item, the first byte tells us if it's nullptr, and what strings
// have been saved as different from the typical
object *restore_item(std::ifstream &save_file)
{
  object *item = nullptr;

  unsigned char type;
  file_read(save_file, type);
  if(type != 0xff)
  {
    item = new object;
    item->objstr.~basic_string();
    item->truename.~basic_string();
    item->cursestr.~basic_string();

    file_read(save_file, *item);
    new (&item->objstr) std::string;
    new (&item->truename) std::string;
    new (&item->cursestr) std::string;
    size_t size;
    if(type & (1 << 0))
    {
      file_read(save_file, size);
      item->objstr.resize(size);
      save_file.read(&item->objstr[0], size);
    }
    else
    {
      item->objstr = Objects[item->id].objstr;
    }
    if(type & (1 << 1))
    {
      file_read(save_file, size);
      item->truename.resize(size);
      save_file.read(&item->truename[0], size);
    }
    else
    {
      item->truename = Objects[item->id].truename;
    }
    if(type & (1 << 2))
    {
      file_read(save_file, size);
      item->cursestr.resize(size);
      save_file.read(&item->cursestr[0], size);
    }
    else
    {
      item->cursestr = Objects[item->id].cursestr;
    }
  }
  return item;
}

objectlist *restore_itemlist(std::ifstream &save_file)
{
  objectlist *ol = nullptr, *cur = nullptr, *new_pol = nullptr;
  int num_items;
  file_read(save_file, num_items);
  bool first_time = true;
  for(int i = 0; i < num_items; ++i)
  {
    new_pol        = new objectlist;
    new_pol->thing = restore_item(save_file);
    new_pol->next  = nullptr;
    if(first_time)
    {
      ol = cur  = new_pol;
      first_time = false;
    }
    else
    {
      cur->next = new_pol;
      cur       = new_pol;
    }
  }
  return ol;
}

void restore_player(std::ifstream &save_file)
{
  Player.name.~basic_string();
  Player.meleestr.~basic_string();
  file_read(save_file, Player);
  new (&Player.name) std::string;
  new (&Player.meleestr) std::string;

  size_t size;
  file_read(save_file, size);
  Password.resize(size);
  save_file.read(&Password[0], size);
  file_read(save_file, size);
  Player.name.resize(size);
  save_file.read(&Player.name[0], size);
  file_read(save_file, size);
  Player.meleestr.resize(size);
  save_file.read(&Player.meleestr[0], size);

  file_read(save_file, CitySiteList);
  file_read(save_file, GameStatus);
  file_read(save_file, Current_Environment);
  file_read(save_file, Last_Environment);
  file_read(save_file, Current_Dungeon);
  file_read(save_file, Villagenum);
  switch(Current_Dungeon)
  {
    case E_ASTRAL:
      MaxDungeonLevels = ASTRALLEVELS;
      break;
    case E_SEWERS:
      MaxDungeonLevels = SEWERLEVELS;
      break;
    case E_CASTLE:
      MaxDungeonLevels = CASTLELEVELS;
      break;
    case E_CAVES:
      MaxDungeonLevels = CAVELEVELS;
      break;
    case E_VOLCANO:
      MaxDungeonLevels = VOLCANOLEVELS;
      break;
  }
  file_read(save_file, Verbosity);
  file_read(save_file, Time);
  file_read(save_file, Tick);
  file_read(save_file, Searchnum);
  file_read(save_file, Behavior);
  file_read(save_file, Phase);
  file_read(save_file, Date);
  file_read(save_file, Spellsleft);
  file_read(save_file, SymbolUseHour);
  file_read(save_file, ViewHour);
  file_read(save_file, HelmHour);
  file_read(save_file, Constriction);
  file_read(save_file, Blessing);
  file_read(save_file, LastDay);
  file_read(save_file, RitualHour);
  file_read(save_file, Lawstone);
  file_read(save_file, Chaostone);
  file_read(save_file, Mindstone);
  file_read(save_file, Arena_Opponent);
  file_read(save_file, Imprisonment);
  file_read(save_file, Gymcredit);
  file_read(save_file, Balance);
  file_read(save_file, StarGemUse);
  file_read(save_file, HiMagicUse);
  file_read(save_file, HiMagic);
  file_read(save_file, FixedPoints);
  file_read(save_file, LastCountryLocX);
  file_read(save_file, LastCountryLocY);
  file_read(save_file, LastTownLocX);
  file_read(save_file, LastTownLocY);
  file_read(save_file, Pawndate);

  for(spell &s : spell::Spells)
  {
    file_read(save_file, s.powerdrain);
    file_read(save_file, s.known);
  }

  file_read(save_file, Command_Duration);
  file_read(save_file, Precipitation);
  file_read(save_file, Lunarity);
  file_read(save_file, ZapHour);
  file_read(save_file, RitualRoom);

  // stuff which used to be statics
  file_read(save_file, twiddle);
  file_read(save_file, saved);
  file_read(save_file, onewithchaos);
  file_read(save_file, club_hinthour);
  file_read(save_file, winnings);
  file_read(save_file, tavern_hinthour);
  file_read(save_file, scroll_ids);
  file_read(save_file, potion_ids);
  file_read(save_file, stick_ids);
  file_read(save_file, ring_ids);
  file_read(save_file, cloak_ids);
  file_read(save_file, boot_ids);
  file_read(save_file, deepest);
  file_read(save_file, level_seed);

  file_read(save_file, received_directions);

  // Set up the strings for the id's
  inititem(false);

  for(int i = 0; i < MAXITEMS; ++i)
  {
    Player.possessions[i] = restore_item(save_file);
  }

  if(!Player.possessions[O_READY_HAND] && Player.possessions[O_WEAPON_HAND] &&
     twohandedp(Player.possessions[O_WEAPON_HAND]->id))
  {
    Player.possessions[O_READY_HAND] = Player.possessions[O_WEAPON_HAND];
  }

  for(int i = 0; i < MAXPACK; ++i)
  {
    Player.pack[i] = restore_item(save_file);
  }
  for(int i = 0; i < PAWNITEMS; ++i)
  {
    Pawnitems[i] = restore_item(save_file);
  }
  Condoitems = restore_itemlist(save_file);
  for(int i = 0; i < TOTALITEMS; ++i)
  {
    file_read(save_file, Objects[i].known);
    file_read(save_file, Objects[i].uniqueness);
  }
}

void restore_hiscore_npc(monster *npc, int npcid)
{
  int  level, behavior;
  long status;

  std::string npc_name;
  switch(npcid)
  {
    case 0:
      npc_name = Hiscorer;
      level    = Hilevel;
      behavior = Hibehavior;
      break;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
      npc_name = Priest[npcid];
      level    = Priestlevel[npcid];
      behavior = Priestbehavior[npcid];
      break;
    case 7:
      npc_name = Shadowlord;
      level    = Shadowlordlevel;
      behavior = Shadowlordbehavior;
      break;
    case 8:
      npc_name = Commandant;
      level    = Commandantlevel;
      behavior = Commandantbehavior;
      break;
    case 9:
      npc_name = Archmage;
      level    = Archmagelevel;
      behavior = Archmagebehavior;
      break;
    case 10:
      npc_name = Prime;
      level    = Primelevel;
      behavior = Primebehavior;
      break;
    case 11:
      npc_name = Champion;
      level    = Championlevel;
      behavior = Championbehavior;
      break;
    case 12:
      npc_name = Duke;
      level    = Dukelevel;
      behavior = Dukebehavior;
      break;
    case 13:
      npc_name = Chaoslord;
      level    = Chaoslordlevel;
      behavior = Chaoslordbehavior;
      break;
    case 14:
      npc_name = Lawlord;
      level    = Lawlordlevel;
      behavior = Lawlordbehavior;
      break;
    default:
      npc_name = Justiciar;
      level    = Justiciarlevel;
      behavior = Justiciarbehavior;
  }
  npc->monstring = npc_name;
  npc->corpsestr = std::format("The body of {}", npc_name);
  if(!m_statusp(*npc, HOSTILE))
  {
    status = npc->status;
    determine_npc_behavior(npc, level, behavior);
    npc->status = status;
  }
}

void restore_monsters(std::ifstream &save_file, level *lvl)
{
  monsterlist  *ml = nullptr;
  lvl->mlist = nullptr;

  int num_monsters;
  file_read(save_file, num_monsters);

  for(int i = 0; i < num_monsters; ++i)
  {
    ml       = new monsterlist;
    ml->m    = new monster;
    ml->m->monstring.~basic_string();
    ml->m->corpsestr.~basic_string();
    ml->m->meleestr.~basic_string();
    ml->next = nullptr;
    file_read(save_file, *ml->m);
    new (&ml->m->monstring) std::string;
    new (&ml->m->corpsestr) std::string;
    new (&ml->m->meleestr) std::string;
    if(ml->m->id == HISCORE_NPC)
    {
      restore_hiscore_npc(ml->m, ml->m->aux2);
    }
    else
    {
      unsigned char type;
      file_read(save_file, type);
      if(type & (1 << 0))
      {
        size_t size;
        file_read(save_file, size);
        ml->m->monstring.resize(size);
        save_file.read(&ml->m->monstring[0], size);
      }
      else
      {
        ml->m->monstring = Monsters[ml->m->id].monstring;
      }
      if(type & (1 << 1))
      {
        size_t size;
        file_read(save_file, size);
        ml->m->corpsestr.resize(size);
        save_file.read(&ml->m->corpsestr[0], size);
      }
      else
      {
        ml->m->corpsestr = Monsters[ml->m->id].corpsestr;
      }
      ml->m->meleestr = Monsters[ml->m->id].meleestr;
    }
    ml->m->possessions = restore_itemlist(save_file);
    lvl->site[ml->m->x][ml->m->y].creature = ml->m;
    ml->next                               = lvl->mlist;
    lvl->mlist                             = ml;
  }
}

void restore_level(std::ifstream &save_file)
{
  Level = new level;
  clear_level(Level);
  file_read(save_file, Level->depth);
  file_read(save_file, Level->numrooms);
  file_read(save_file, Level->tunnelled);
  file_read(save_file, Level->environment);
  Level->generated    = true;
  int temp_env        = Current_Environment;
  Current_Environment = Level->environment;
  switch(Level->environment)
  {
    case E_COUNTRYSIDE:
      load_country();
      break;
    case E_CITY:
      load_city(false);
      break;
    case E_VILLAGE:
      load_village(Country[LastCountryLocX][LastCountryLocY].aux, false);
      break;
    case E_CAVES:
      initrand(Current_Environment, Level->depth);
      if((random_range(4) == 0) && (Level->depth < MaxDungeonLevels))
      {
        room_level();
      }
      else
      {
        cavern_level();
      }
      break;
    case E_SEWERS:
      initrand(Current_Environment, Level->depth);
      if((random_range(4) == 0) && (Level->depth < MaxDungeonLevels))
      {
        room_level();
      }
      else
      {
        sewer_level();
      }
      break;
    case E_CASTLE:
      initrand(Current_Environment, Level->depth);
      room_level();
      break;
    case E_ASTRAL:
      initrand(Current_Environment, Level->depth);
      maze_level();
      break;
    case E_VOLCANO:
      initrand(Current_Environment, Level->depth);
      switch(random_range(3))
      {
        case 0:
          cavern_level();
          break;
        case 1:
          room_level();
          break;
        case 2:
          maze_level();
          break;
      }
      break;
    case E_HOVEL:
    case E_MANSION:
    case E_HOUSE:
      load_house(Level->environment, false);
      break;
    case E_DLAIR:
      load_dlair(gamestatusp(KILLED_DRAGONLORD, GameStatus), false);
      break;
    case E_STARPEAK:
      load_speak(gamestatusp(KILLED_LAWBRINGER, GameStatus), false);
      break;
    case E_MAGIC_ISLE:
      load_misle(gamestatusp(KILLED_EATER, GameStatus), false);
      break;
    case E_TEMPLE:
      load_temple(Country[LastCountryLocX][LastCountryLocY].aux, false);
      break;
    case E_CIRCLE:
      load_circle(false);
      break;
    case E_COURT:
      load_court(false);
      break;
    default:
      queue_message("This dungeon not implemented!");
      break;
  }
  if(Level->depth > 0)
  { // dungeon...
    install_traps();
    install_specials();
    make_stairs(-1);
    make_stairs(-1);
    initrand(E_RESTORE, 0);
  }
  Current_Environment = temp_env;
  int x, y;
  file_read(save_file, x);
  file_read(save_file, y);
  size_t run;
  while(y < MAXLENGTH && x < MAXWIDTH)
  {
    file_read(save_file, run);
    for(; x < static_cast<int>(run); ++x)
    {
      file_read(save_file, Level->site[x][y]);
      Level->site[x][y].creature = nullptr;
      Level->site[x][y].things   = nullptr;
    }
    file_read(save_file, x);
    file_read(save_file, y);
  }
  run = 0;
  unsigned long int mask;
  for(y = 0; y < MAXLENGTH; ++y)
  {
    for(x = 0; x < MAXWIDTH; ++x)
    {
      if(run == 0)
      {
        run = 8 * sizeof(mask);
        file_read(save_file, mask);
      }
      if(mask & 1)
      {
        lset(x, y, SEEN, *Level);
      }
      mask >>= 1;
      --run;
    }
  }
  restore_monsters(save_file, Level);
  file_read(save_file, x);
  file_read(save_file, y);
  while(y < MAXLENGTH && x < MAXWIDTH)
  {
    Level->site[x][y].things = restore_itemlist(save_file);
    file_read(save_file, x);
    file_read(save_file, y);
  }
}

void restore_country(std::ifstream &save_file)
{

  load_country();
  int x, y;
  file_read(save_file, x);
  file_read(save_file, y);
  while(x < MAXWIDTH && y < MAXLENGTH)
  {
    file_read(save_file, Country[x][y]);
    file_read(save_file, x);
    file_read(save_file, y);
  }
  size_t run = 0;
  unsigned long int mask;
  for(x = 0; x < MAXWIDTH; ++x)
  {
    for(y = 0; y < MAXLENGTH; ++y)
    {
      if(run == 0)
      {
        run = 8 * sizeof(mask);
        file_read(save_file, mask);
      }
      if(mask & 1)
      {
        c_set(x, y, SEEN, Country);
      }
      mask >>= 1;
      --run;
    }
  }
}

// read player data, city level, dungeon level,
// check on validity of save file, etc.
// return true if game restored, false otherwise
bool restore_game(const std::string &save_file_name)
{
  if(!std::filesystem::exists(save_file_name) || !std::filesystem::is_regular_file(save_file_name))
  {
    queue_message("Unable to access save file: " + save_file_name);
    return false;
  }

  std::ifstream save_file{save_file_name, std::ios::binary};
  if(!save_file)
  {
    queue_message("Error restoring game -- aborted.");
    queue_message("File name was: " + save_file_name);
    return false;
  }
  else
  {
    queue_message("Restoring...");

    int version;
    file_read(save_file, version);
    if(VERSION != version && !ok_outdated(version))
    {
      save_file.close();
      int major_version = version / 10000;
      int minor_version = (version - major_version) / 100;
      int bugfix_version = version % 100;
      mvaddstr(0, 0, "Sorry, I can't restore an outdated save file!");
      mvaddstr(1, 0, std::format("Save file is version {}.{}.{}", major_version, minor_version, bugfix_version).c_str());
      getch();
      return false;
    }
    restore_player(save_file);
    restore_country(save_file);
    restore_level(save_file); // the city level
    int i;
    file_read(save_file, i);
    for(; i > 0; --i)
    {

#ifdef SAVE_LEVELS
      msdos_changelevel(Level, 0, -1);
#endif

      restore_level(save_file);
      if(Level->environment == Current_Dungeon)
      {
        Level->next = Dungeon;
        Dungeon     = Level;
      }
      if(Current_Environment == E_CITY)
      {
        Level = City;
      }
    }
    // this disgusting kludge because LENGTH and WIDTH are globals...
    WIDTH = 64;
    switch(Current_Environment)
    {
      case E_COURT:
        LENGTH = 24;
        break;
      case E_ARENA:
      case E_ABYSS:
      case E_CIRCLE:
      case E_MANSION:
      case E_HOUSE:
      case E_HOVEL:
      case E_DLAIR:
      case E_STARPEAK:
      case E_MAGIC_ISLE:
      case E_TEMPLE:
      case E_VILLAGE:
        LENGTH = 16;
        break;
      default:
        LENGTH = 64;
        break;
    }
    save_file.close();
    queue_message("Restoration complete.");
    ScreenOffset = -1000; // to force a redraw
    setgamestatus(SKIP_MONSTERS, GameStatus);
    return true;
  }
}
