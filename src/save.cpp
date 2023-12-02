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
#include "scr.h"
#include "spell.h"

#include <filesystem>
#include <format>
#include <fstream>
#include <string>

extern std::string version_string(int version);

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

void save_itemlist(std::ofstream &save_file, std::forward_list<std::unique_ptr<object>> &ol)
{
  size_t num_items = std::distance(ol.begin(), ol.end());
  file_write(save_file, num_items);
  for(std::unique_ptr<object> &o : ol)
  {
    save_item(save_file, o.get());
  }
}
void save_items(std::ofstream &save_file, std::vector<std::unique_ptr<object>> &items)
{
  file_write(save_file, items.size());
  for(std::unique_ptr<object> &o : items)
  {
    save_item(save_file, o.get());
  }
}

// also saves some globals like Level->depth...
void save_player(std::ofstream &save_file)
{
  // Save random global state information
  Player.click = (Tick + 1) % 60;

  file_write(save_file, Player.name.size());
  save_file.write(reinterpret_cast<char *>(Player.name.data()), Player.name.size());

  file_write(save_file, Player.level);
  file_write(save_file, Player.xp);
  file_write(save_file, Player.hp);
  file_write(save_file, Player.maxhp);
  file_write(save_file, Player.mana);
  file_write(save_file, Player.maxmana);

  file_write(save_file, Player.str);
  file_write(save_file, Player.con);
  file_write(save_file, Player.dex);
  file_write(save_file, Player.agi);
  file_write(save_file, Player.iq);
  file_write(save_file, Player.pow);
  file_write(save_file, Player.maxstr);
  file_write(save_file, Player.maxcon);
  file_write(save_file, Player.maxdex);
  file_write(save_file, Player.maxagi);
  file_write(save_file, Player.maxiq);
  file_write(save_file, Player.maxpow);
  file_write(save_file, Player.hit);
  file_write(save_file, Player.dmg);
  file_write(save_file, Player.absorption);
  file_write(save_file, Player.speed);
  file_write(save_file, Player.click);
  file_write(save_file, Player.defense);
  file_write(save_file, Player.food);
  file_write(save_file, Player.alignment);
  file_write(save_file, Player.cash);
  file_write(save_file, Player.patron);
  file_write(save_file, Player.birthday);
  file_write(save_file, Player.sx);
  file_write(save_file, Player.sy);
  file_write(save_file, Player.x);
  file_write(save_file, Player.y);
  file_write(save_file, Player.itemweight);
  file_write(save_file, Player.maxweight);
  file_write(save_file, Player.options);

  file_write(save_file, Player.immunity);
  file_write(save_file, Player.status);
  file_write(save_file, Player.rank);
  file_write(save_file, Player.guildxp);

  file_write(save_file, Player.meleestr.size());
  save_file.write(reinterpret_cast<char *>(Player.meleestr.data()), Player.meleestr.size());

  file_write(save_file, Password.size());
  save_file.write(reinterpret_cast<char *>(Password.data()), Password.size());
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
  file_write(save_file, Studiesleft);
  file_write(save_file, SymbolUseDay);
  file_write(save_file, SymbolUseHour);
  file_write(save_file, ViewDay);
  file_write(save_file, ViewHour);
  file_write(save_file, HelmDay);
  file_write(save_file, HelmHour);
  file_write(save_file, Constriction);
  file_write(save_file, Blessing);
  file_write(save_file, LastDay);
  file_write(save_file, RitualDay);
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
  file_write(save_file, ZapDay);
  file_write(save_file, ZapHour);
  file_write(save_file, RitualRoom);
  file_write(save_file, MazeNum);

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
  for(std::unique_ptr<object> &o : Player.possessions)
  {
    save_item(save_file, o.get());
  }
  save_items(save_file, Player.pack);

  for(int i = 0; i < PAWNITEMS; ++i)
  {
    save_item(save_file, Pawnitems[i].get());
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
  int run                = 8 * sizeof(mask);
  for(x = 0; x < MAXWIDTH; ++x)
  {
    for(y = 0; y < MAXLENGTH; ++y)
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
  if(run < static_cast<int>(8 * sizeof(mask)))
  {
    file_write(save_file, mask);
  }
}

void save_monsters(std::ofstream &save_file, std::forward_list<std::unique_ptr<monster>> &ml)
{
  // First count monsters
  int num_monsters = 0;
  for(std::unique_ptr<monster> &m : ml)
  {
    if(m->hp > 0)
    {
      ++num_monsters;
    }
  }
  file_write(save_file, num_monsters);

  // Now save monsters
  for(std::unique_ptr<monster> &m : ml)
  {
    if(m->hp > 0)
    {
      file_write(save_file, *m);
      if(m->id != HISCORE_NPC)
      {
        unsigned char type = 0x0;
        if(m->monstring != Monsters[m->id].monstring)
        {
          type |= 0x1;
        }
        if(m->corpsestr != Monsters[m->id].corpsestr)
        {
          type |= 0x2;
        }
        file_write(save_file, type);
        if(type & 1)
        {
          file_write(save_file, m->monstring.size());
          save_file.write(reinterpret_cast<char *>(m->monstring.data()), m->monstring.size());
        }
        if(type & 2)
        {
          file_write(save_file, m->corpsestr.size());
          save_file.write(reinterpret_cast<char *>(m->corpsestr.data()), m->corpsestr.size());
        }
      }
      save_itemlist(save_file, m->possessions);
    }
  }
}

void save_level(std::ofstream &save_file, level *level)
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
        for(run = x + 1; run < MAXWIDTH && level->site[run][y].lstatus & CHANGED; ++run)
        {
        }
        file_write(save_file, x);
        file_write(save_file, y);
        file_write(save_file, run);
        for(; x < run; ++x)
        {
          file_write(save_file, level->site[x][y]);
        }
      }
    }
  }
  file_write(save_file, x);
  file_write(save_file, y);
  // since we don't mark the 'seen' bits as CHANGED, need to save a bitmask
  unsigned long int mask = 0;
  int run                = 8 * sizeof(mask);
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
  if(run < static_cast<int>(8 * sizeof(mask)))
  {
    file_write(save_file, mask);
  }
  save_monsters(save_file, level->mlist);
  for(x = 0; x < MAXWIDTH; ++x)
  {
    for(y = 0; y < MAXLENGTH; ++y)
    {
      if(!level->site[x][y].things.empty())
      {
        file_write(save_file, x);
        file_write(save_file, y);
        save_items(save_file, level->site[x][y].things);
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
  bool writeok = true;
  if(std::filesystem::exists(save_file_name))
  {
    queue_message("Save file already exists. Overwrite it? [yn] ");
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

  file_write(save_file, SAVE_FILE_VERSION);
  file_write(save_file, GAME_VERSION);
  save_player(save_file);
  save_country(save_file);

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
  for(i = 0, current = save; current; current = current->next, i++)
  {
  }
  file_write(save_file, i);

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

// Restore an item, the first byte tells us if it's nullptr, and what strings
// have been saved as different from the typical
std::unique_ptr<object> restore_item(std::ifstream &save_file)
{
  std::unique_ptr<object> item;

  unsigned char type;
  file_read(save_file, type);
  if(type != 0xff)
  {
    item = std::make_unique<object>();
    item->objstr.~basic_string();
    item->truename.~basic_string();
    item->cursestr.~basic_string();

    file_read(save_file, *item);
    new(&item->objstr) std::string;
    new(&item->truename) std::string;
    new(&item->cursestr) std::string;
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

std::forward_list<std::unique_ptr<object>> restore_itemlist(std::ifstream &save_file)
{
  std::forward_list<std::unique_ptr<object>> item_list;
  size_t num_items;
  file_read(save_file, num_items);
  auto it = item_list.before_begin();
  for(size_t i = 0; i < num_items; ++i)
  {
    it = item_list.insert_after(it, restore_item(save_file));
  }
  return item_list;
}
std::vector<std::unique_ptr<object>> restore_items(std::ifstream &save_file)
{
  std::vector<std::unique_ptr<object>> items;
  std::vector<std::unique_ptr<object>>::size_type num_items;
  file_read(save_file, num_items);
  for(std::vector<std::unique_ptr<object>>::size_type i = 0; i < num_items; ++i)
  {
    items.emplace_back(restore_item(save_file));
  }
  return items;
}

void restore_player(std::ifstream &save_file, player &p)
{
  std::string::size_type size;
  file_read(save_file, size);
  p.name.resize(size);
  save_file.read(&p.name[0], size);

  file_read(save_file, p.level);
  file_read(save_file, p.xp);
  file_read(save_file, p.hp);
  file_read(save_file, p.maxhp);
  file_read(save_file, p.mana);
  file_read(save_file, p.maxmana);

  file_read(save_file, p.str);
  file_read(save_file, p.con);
  file_read(save_file, p.dex);
  file_read(save_file, p.agi);
  file_read(save_file, p.iq);
  file_read(save_file, p.pow);
  file_read(save_file, p.maxstr);
  file_read(save_file, p.maxcon);
  file_read(save_file, p.maxdex);
  file_read(save_file, p.maxagi);
  file_read(save_file, p.maxiq);
  file_read(save_file, p.maxpow);
  file_read(save_file, p.hit);
  file_read(save_file, p.dmg);
  file_read(save_file, p.absorption);
  file_read(save_file, p.speed);
  file_read(save_file, p.click);
  file_read(save_file, p.defense);
  file_read(save_file, p.food);
  file_read(save_file, p.alignment);
  file_read(save_file, p.cash);
  file_read(save_file, p.patron);
  file_read(save_file, p.birthday);
  file_read(save_file, p.sx);
  file_read(save_file, p.sy);
  file_read(save_file, p.x);
  file_read(save_file, p.y);
  file_read(save_file, p.itemweight);
  file_read(save_file, p.maxweight);
  file_read(save_file, p.options);

  file_read(save_file, p.immunity);
  file_read(save_file, p.status);
  file_read(save_file, p.rank);
  file_read(save_file, p.guildxp);

  file_read(save_file, size);
  p.meleestr.resize(size);
  save_file.read(&p.meleestr[0], size);

  file_read(save_file, size);
  Password.resize(size);
  save_file.read(&Password[0], size);

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
  file_read(save_file, Studiesleft);
  file_read(save_file, SymbolUseDay);
  file_read(save_file, SymbolUseHour);
  file_read(save_file, ViewDay);
  file_read(save_file, ViewHour);
  file_read(save_file, HelmDay);
  file_read(save_file, HelmHour);
  file_read(save_file, Constriction);
  file_read(save_file, Blessing);
  file_read(save_file, LastDay);
  file_read(save_file, RitualDay);
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
  file_read(save_file, ZapDay);
  file_read(save_file, ZapHour);
  file_read(save_file, RitualRoom);
  file_read(save_file, MazeNum);

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

  inititem(false); // Set up the strings for the id's
  for(int i = 0; i < MAXITEMS; ++i)
  {
    p.possessions[i] = restore_item(save_file);
  }
  p.pack = restore_items(save_file);

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
  int level, behavior;
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
    case 15:
      npc_name = Justiciar;
      level    = Justiciarlevel;
      behavior = Justiciarbehavior;
      break;
    case 16:
      npc_name = Grandmaster;
      level    = Grandmasterlevel;
      behavior = Grandmasterbehavior;
      break;
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
  int num_monsters;
  file_read(save_file, num_monsters);

  auto it = lvl->mlist.before_begin();
  for(int i = 0; i < num_monsters; ++i)
  {
    auto m = std::make_unique<monster>();
    m->monstring.~basic_string();
    m->corpsestr.~basic_string();
    m->meleestr.~basic_string();
    m->possessions.~forward_list();
    file_read(save_file, *m);
    new(&m->monstring) std::string;
    new(&m->corpsestr) std::string;
    new(&m->meleestr) std::string;
    new(&m->possessions) std::forward_list<std::unique_ptr<object>>;
    if(m->id == HISCORE_NPC)
    {
      restore_hiscore_npc(m.get(), m->aux2);
    }
    else
    {
      unsigned char type;
      file_read(save_file, type);
      if(type & (1 << 0))
      {
        size_t size;
        file_read(save_file, size);
        m->monstring.resize(size);
        save_file.read(&m->monstring[0], size);
      }
      else
      {
        m->monstring = Monsters[m->id].monstring;
      }
      if(type & (1 << 1))
      {
        size_t size;
        file_read(save_file, size);
        m->corpsestr.resize(size);
        save_file.read(&m->corpsestr[0], size);
      }
      else
      {
        m->corpsestr = Monsters[m->id].corpsestr;
      }
      m->meleestr = Monsters[m->id].meleestr;
    }
    m->possessions                 = restore_itemlist(save_file);
    lvl->site[m->x][m->y].creature = m.get();
    it = lvl->mlist.insert_after(it, std::move(m));
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
  int run;
  while(y < MAXLENGTH && x < MAXWIDTH)
  {
    file_read(save_file, run);
    for(; x < run; ++x)
    {
      Level->site[x][y].things.~vector();
      file_read(save_file, Level->site[x][y]);
      new(&Level->site[x][y].things) std::vector<std::unique_ptr<object >>;
      Level->site[x][y].creature = nullptr;
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
    Level->site[x][y].things = restore_items(save_file);
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
  int run = 0;
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

    int save_file_version;
    file_read(save_file, save_file_version);
    int game_version;
    file_read(save_file, game_version);
    if(save_file_version != SAVE_FILE_VERSION)
    {
      save_file.close();
      mvaddstr(0, 0, "Sorry, I can't restore an outdated save file!");
      mvaddstr(1, 0, std::format("Save file is version {}", version_string(game_version)).c_str());
      getch();
      return false;
    }
    restore_player(save_file, Player);
    restore_country(save_file);
    restore_level(save_file); // the city level
    int i;
    file_read(save_file, i);
    for(; i > 0; --i)
    {
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
