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

/* save.c */

#include "glob.h"
#include <filesystem>

#ifdef SAVE_LEVELS
plv msdos_changelevel(plv oldlevel, int newenv, int newdepth);
#endif

/*Various functions for doing game saves and restores */
/*The game remembers various player information, the city level,
the country level, and the last or current dungeon level */

/**************** SAVE FUNCTIONS */

/* Checks to see if save file already exists.
   Checks to see if save file can be opened for write.
   The player, the city level, and the current dungeon level are saved.
*/

bool save_game(const char *savestr)
{
  FILE *fd;
#ifdef SAVE_LEVELS
  int tmpdepth;
#endif
  int i;
  bool writeok = true;
  plv current, save;

  if(std::filesystem::exists(savestr))
  {
    if(std::filesystem::is_regular_file(savestr))
    {
      mprint(" Overwrite old file?");
      writeok = (ynq() == 'y');
    }
    else
    {
      writeok = false;
    }
  }
  if(writeok)
  {
    fd = fopen(savestr, "wb");
    if(!fd)
    {
      writeok = false;
      mprint(" Error opening file.");
    }
  }
  if(!writeok)
  {
    print2("Save aborted.");
  }
  else
  {
    print1("Saving Game....");

    /* write the version number */
    i = VERSION;
    fwrite((char *)&i, sizeof(int), 1, fd);
    /* write game id to save file */

    writeok &= save_player(fd);
    writeok &= save_country(fd);
#ifdef SAVE_LEVELS
    tmpdepth = Level->depth;
    City     = msdos_changelevel(Level, E_CITY, 0);
#endif
    writeok &= save_level(fd, City);

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
    for(i = 0, current = save; current; current = current->next, i++)
    {
      ;
    }
    if(!fwrite((char *)&i, sizeof(int), 1, fd))
    {
      writeok = false;
    }
#ifdef SAVE_LEVELS
    Level = msdos_changelevel(NULL, Current_Environment, tmpdepth);
#endif
    for(current = save; current; current = current->next)
    {
      if(current != Level)
      {
        writeok &= save_level(fd, current);
      }
    }
    if(save)
    {
      writeok &= save_level(fd, Level); /* put current level last */
    }
    fclose(fd);
    if(writeok)
    {
      print1("Game Saved.");
    }
    else
    {
      print1("Something didn't work... save aborted.");
    }
  }
  return (writeok);
}

/* saves game on SIGHUP */
void signalsave(int)
{
  save_game("Omega.Sav");
  print1("Signal - Saving file 'Omega.Sav'.");
  endgraf();
  exit(0);
}

/* also saves some globals like Level->depth... */

bool save_player(FILE *fd)
{
  int i;
  bool ok = true;

  /* Save random global state information */

  Player.click = (Tick + 1) % 60;
  ok &= (fwrite((char *)&Player, sizeof(Player), 1, fd) > 0);
  ok &= (fprintf(fd, "%s\n", Password) >= 0);
  ok &= (fprintf(fd, "%s\n", Player.name) >= 0);
  ok &= (fwrite((char *)CitySiteList, sizeof(CitySiteList), 1, fd) > 0);
  ok &= (fwrite((char *)&GameStatus, sizeof(long), 1, fd) > 0);
  ok &= (fwrite((char *)&Current_Environment, sizeof(int), 1, fd) > 0);
  ok &= (fwrite((char *)&Last_Environment, sizeof(int), 1, fd) > 0);
  ok &= (fwrite((char *)&Current_Dungeon, sizeof(int), 1, fd) > 0);
  ok &= (fwrite((char *)&Villagenum, sizeof(int), 1, fd) > 0);
  ok &= (fwrite((char *)&Verbosity, sizeof(char), 1, fd) > 0);
  ok &= (fwrite((char *)&Time, sizeof(long), 1, fd) > 0);
  ok &= (fwrite((char *)&Tick, sizeof(int), 1, fd) > 0);
  ok &= (fwrite((char *)&Searchnum, sizeof(int), 1, fd) > 0);
  ok &= (fwrite((char *)&Behavior, sizeof(int), 1, fd) > 0);
  ok &= (fwrite((char *)&Phase, sizeof(int), 1, fd) > 0);
  ok &= (fwrite((char *)&Date, sizeof(int), 1, fd) > 0);
  ok &= (fwrite((char *)&Spellsleft, sizeof(int), 1, fd) > 0);
  ok &= (fwrite((char *)&SymbolUseHour, sizeof(int), 1, fd) > 0);
  ok &= (fwrite((char *)&ViewHour, sizeof(int), 1, fd) > 0);
  ok &= (fwrite((char *)&HelmHour, sizeof(int), 1, fd) > 0);
  ok &= (fwrite((char *)&Constriction, sizeof(int), 1, fd) > 0);
  ok &= (fwrite((char *)&Blessing, sizeof(int), 1, fd) > 0);
  ok &= (fwrite((char *)&LastDay, sizeof(int), 1, fd) > 0);
  ok &= (fwrite((char *)&RitualHour, sizeof(int), 1, fd) > 0);
  ok &= (fwrite((char *)&Lawstone, sizeof(int), 1, fd) > 0);
  ok &= (fwrite((char *)&Chaostone, sizeof(int), 1, fd) > 0);
  ok &= (fwrite((char *)&Mindstone, sizeof(int), 1, fd) > 0);
  ok &= (fwrite((char *)&Arena_Opponent, sizeof(int), 1, fd) > 0);
  ok &= (fwrite((char *)&Imprisonment, sizeof(int), 1, fd) > 0);
  ok &= (fwrite((char *)&Gymcredit, sizeof(long), 1, fd) > 0);
  ok &= (fwrite((char *)&Balance, sizeof(long), 1, fd) > 0);
  ok &= (fwrite((char *)&StarGemUse, sizeof(int), 1, fd) > 0);
  ok &= (fwrite((char *)&HiMagicUse, sizeof(int), 1, fd) > 0);
  ok &= (fwrite((char *)&HiMagic, sizeof(int), 1, fd) > 0);
  ok &= (fwrite((char *)&FixedPoints, sizeof(long), 1, fd) > 0);
  ok &= (fwrite((char *)&LastCountryLocX, sizeof(int), 1, fd) > 0);
  ok &= (fwrite((char *)&LastCountryLocY, sizeof(int), 1, fd) > 0);
  ok &= (fwrite((char *)&LastTownLocX, sizeof(int), 1, fd) > 0);
  ok &= (fwrite((char *)&LastTownLocY, sizeof(int), 1, fd) > 0);
  ok &= (fwrite((char *)&Pawndate, sizeof(int), 1, fd) > 0);

  ok &= (fwrite((char *)Spells, sizeof(Spells), 1, fd) > 0);

  ok &= (fwrite((char *)&Command_Duration, sizeof(Command_Duration), 1, fd) > 0);
  ok &= (fwrite((char *)&Precipitation, sizeof(Precipitation), 1, fd) > 0);
  ok &= (fwrite((char *)&Lunarity, sizeof(Lunarity), 1, fd) > 0);
  ok &= (fwrite((char *)&ZapHour, sizeof(ZapHour), 1, fd) > 0);
  ok &= (fwrite((char *)&RitualRoom, sizeof(RitualRoom), 1, fd) > 0);

  /* stuff which used to be statics */
  ok &= (fwrite((char *)&twiddle, sizeof(twiddle), 1, fd) > 0);
  ok &= (fwrite((char *)&saved, sizeof(saved), 1, fd) > 0);
  ok &= (fwrite((char *)&onewithchaos, sizeof(onewithchaos), 1, fd) > 0);
  ok &= (fwrite((char *)&club_hinthour, sizeof(club_hinthour), 1, fd) > 0);
  ok &= (fwrite((char *)&winnings, sizeof(winnings), 1, fd) > 0);
  ok &= (fwrite((char *)&tavern_hinthour, sizeof(tavern_hinthour), 1, fd) > 0);
  ok &= (fwrite((char *)scroll_ids, sizeof(scroll_ids), 1, fd) > 0);
  ok &= (fwrite((char *)potion_ids, sizeof(potion_ids), 1, fd) > 0);
  ok &= (fwrite((char *)stick_ids, sizeof(stick_ids), 1, fd) > 0);
  ok &= (fwrite((char *)ring_ids, sizeof(ring_ids), 1, fd) > 0);
  ok &= (fwrite((char *)cloak_ids, sizeof(cloak_ids), 1, fd) > 0);
  ok &= (fwrite((char *)boot_ids, sizeof(boot_ids), 1, fd) > 0);
  ok &= (fwrite((char *)deepest, sizeof(int), E_MAX + 1, fd) > 0);
  ok &= (fwrite((char *)level_seed, sizeof(int), E_MAX + 1, fd) > 0);

  ok &= (fwrite((char *)&received_directions, sizeof(bool), 1, fd) > 0);

  /* Save player possessions */

  if(Player.possessions[O_READY_HAND] == Player.possessions[O_WEAPON_HAND])
  {
    Player.possessions[O_READY_HAND] = NULL;
  }
  for(i = 0; i < MAXITEMS; i++)
  {
    ok &= save_item(fd, Player.possessions[i]);
  }
  for(i = 0; i < MAXPACK; i++)
  {
    ok &= save_item(fd, Player.pack[i]);
  }
  for(i = 0; i < PAWNITEMS; i++)
  {
    ok &= save_item(fd, Pawnitems[i]);
  }

  /* Save items in condo vault */
  ok &= save_itemlist(fd, Condoitems);

  /* Save player item knowledge */
  for(i = 0; i < TOTALITEMS; i++)
  {
    ok &= (fwrite((char *)&(Objects[i].known), sizeof(Objects[i].known), 1, fd) > 0);
    ok &= (fwrite((char *)&(Objects[i].uniqueness), sizeof(Objects[i].uniqueness), 1, fd) > 0);
  }
  return ok;
}

/* Save whatever is pointed to by level */
bool save_level(FILE *fd, plv level)
{
  size_t            i, j, run;
  unsigned long int mask;
  bool              ok = true;

  ok &= (fwrite((char *)&level->depth, sizeof(char), 1, fd) > 0);
  ok &= (fwrite((char *)&level->numrooms, sizeof(char), 1, fd) > 0);
  ok &= (fwrite((char *)&level->tunnelled, sizeof(char), 1, fd) > 0);
  ok &= (fwrite((char *)&level->environment, sizeof(int), 1, fd) > 0);
  for(j = 0; j < MAXLENGTH; j++)
  {
    for(i = 0; i < MAXWIDTH; i++)
    {
      if(level->site[i][j].lstatus & CHANGED)
      {                                    /* this loc has been changed */
        for(run = i + 1; run < MAXWIDTH && /* find how many in a row */
                         level->site[run][j].lstatus & CHANGED;
            run++)
        {
          ;
        }
        ok &= (fwrite((char *)&i, sizeof(int), 1, fd) > 0);
        ok &= (fwrite((char *)&j, sizeof(int), 1, fd) > 0);
        ok &= (fwrite((char *)&run, sizeof(int), 1, fd) > 0);
        for(; i < run; i++)
        {
          ok &= (fwrite((char *)&level->site[i][j], sizeof(struct location), 1, fd) > 0);
        }
      }
    }
  }
  ok &= (fwrite((char *)&i, sizeof(int), 1, fd) > 0);
  ok &= (fwrite((char *)&j, sizeof(int), 1, fd) > 0); /* signify end */
  /* since we don't mark the 'seen' bits as CHANGED, need to save a bitmask */
  run  = 8 * sizeof(long int);
  mask = 0;
  for(j = 0; j < MAXLENGTH; j++)
  {
    for(i = 0; i < MAXWIDTH; i++)
    {
      if(run == 0)
      {
        run = 8 * sizeof(long int);
        ok &= (fwrite((char *)&mask, sizeof(long int), 1, fd) > 0);
        mask = 0;
      }
      mask >>= 1;
      if(level->site[i][j].lstatus & SEEN)
      {
        mask |= ((long int)1 << (sizeof(long int) * 8 - 1));
      }
      run--;
    }
  }
  if(run < 8 * sizeof(long int))
  {
    ok &= (fwrite((char *)&mask, sizeof(long int), 1, fd) > 0);
  }
  ok &= save_monsters(fd, level->mlist);
  for(i = 0; i < MAXWIDTH; i++)
  {
    for(j = 0; j < MAXLENGTH; j++)
    {
      if(level->site[i][j].things)
      {
        ok &= (fwrite((char *)&i, sizeof(int), 1, fd) > 0);
        ok &= (fwrite((char *)&j, sizeof(int), 1, fd) > 0);
        ok &= save_itemlist(fd, level->site[i][j].things);
      }
    }
  }
  ok &= (fwrite((char *)&i, sizeof(int), 1, fd) > 0);
  ok &= (fwrite((char *)&j, sizeof(int), 1, fd) > 0); /* signify end */
  return ok;
}

bool save_monsters(FILE *fd, pml ml)
{
  pml           tml;
  int           nummonsters = 0;
  bool          ok          = true;
  unsigned char type;

  /* First count monsters */
  for(tml = ml; tml != NULL; tml = tml->next)
  {
    if(tml->m->hp > 0)
    {
      nummonsters++;
    }
  }

  ok &= (fwrite((char *)&nummonsters, sizeof(int), 1, fd) > 0);

  /* Now save monsters */
  for(tml = ml; tml != NULL; tml = tml->next)
  {
    if(tml->m->hp > 0)
    {
      ok &= (fwrite((char *)tml->m, sizeof(montype), 1, fd) > 0);
      if(tml->m->id != HISCORE_NPC)
      {
        type = 0x0;
        if(strcmp(tml->m->monstring, Monsters[tml->m->id].monstring))
        {
          type |= 0x1;
        }
        if(strcmp(tml->m->corpsestr, Monsters[tml->m->id].corpsestr))
        {
          type |= 0x2;
        }
        ok &= (fwrite((char *)&type, sizeof(unsigned char), 1, fd) > 0);
        if(type & 1)
        {
          ok &= (fprintf(fd, "%s\n", tml->m->monstring) >= 0);
        }
        if(type & 2)
        {
          ok &= (fprintf(fd, "%s\n", tml->m->corpsestr) >= 0);
        }
        /* WDT: line moved from here... */
      } /* else it'll be reloaded from the hiscore file on restore */
      /* WDT: to here.  This bug fix is Sheldon Simm's suggestion
       * to fix the well-known 'Star Gem' bug; it should allow the
       * possessions of hiscore NPCs to be restored from the savefile.
       * See also the complementary change in restore_monsters. */
      ok &= save_itemlist(fd, tml->m->possessions);
    }
  }
  return ok;
}

/* Save o unless it's null, then save a special flag byte instead */
/* Use other values of flag byte to indicate what strings are saved */
bool save_item(FILE *fd, pob o)
{
  bool          ok = true;
  unsigned char type;

  if(o == NULL)
  {
    type = 0xff;
    ok &= (fwrite((char *)&type, sizeof(type), 1, fd) > 0);
  }
  else
  {
    type = 0;
    if(strcmp(o->objstr, Objects[o->id].objstr))
    {
      type |= 1;
    }
    if(strcmp(o->truename, Objects[o->id].truename))
    {
      type |= 2;
    }
    if(strcmp(o->cursestr, Objects[o->id].cursestr))
    {
      type |= 4;
    }
    ok &= (fwrite((char *)&type, sizeof(type), 1, fd) > 0);
    ok &= (fwrite((char *)o, sizeof(objtype), 1, fd) > 0);
    if(type & 1)
    {
      ok &= (fprintf(fd, "%s\n", o->objstr) >= 0);
    }
    if(type & 2)
    {
      ok &= (fprintf(fd, "%s\n", o->truename) >= 0);
    }
    if(type & 4)
    {
      ok &= (fprintf(fd, "%s\n", o->cursestr) >= 0);
    }
  }
  return ok;
}

bool save_itemlist(FILE *fd, pol ol)
{
  int numitems = 0;
  pol tol;
  bool ok = 1;

  for(tol = ol; tol != NULL; tol = tol->next)
  {
    numitems++;
  }
  ok &= (fwrite((char *)&numitems, sizeof(int), 1, fd) > 0);
  for(tol = ol; tol != NULL; tol = tol->next)
  {
    ok &= save_item(fd, tol->thing);
  }
  return ok;
}

bool save_country(FILE *fd)
{
  size_t            i, j, run;
  bool              ok = true;
  unsigned long int mask;

  for(i = 0; i < MAXWIDTH; i++)
  {
    for(j = 0; j < MAXLENGTH; j++)
    {
      if(c_statusp(i, j, CHANGED, Country))
      {
        ok &= (fwrite((char *)&i, sizeof(int), 1, fd) > 0);
        ok &= (fwrite((char *)&j, sizeof(int), 1, fd) > 0);
        ok &= (fwrite((char *)&Country[i][j], sizeof(struct terrain), 1, fd) > 0);
      }
    }
  }
  ok &= (fwrite((char *)&i, sizeof(int), 1, fd) > 0);
  ok &= (fwrite((char *)&j, sizeof(int), 1, fd) > 0);
  /* since we don't mark the 'seen' bits as CHANGED, need to save a bitmask */
  run  = 8 * sizeof(long int);
  mask = 0;
  for(i = 0; i < MAXWIDTH; i++)
  {
    for(j = 0; j < MAXLENGTH; j++)
    {
      if(run == 0)
      {
        run = 8 * sizeof(long int);
        ok &= (fwrite((char *)&mask, sizeof(long int), 1, fd) > 0);
        mask = 0;
      }
      mask >>= 1;
      if(c_statusp(i, j, SEEN, Country))
      {
        mask |= ((long int)1 << (sizeof(long int) * 8 - 1));
      }
      run--;
    }
  }
  if(run < 8 * sizeof(long int))
  {
    ok &= (fwrite((char *)&mask, sizeof(long int), 1, fd) > 0);
  }
  return ok;
}

/* returns true if the given version can be restored by this version */
bool ok_outdated(int version)
{
  switch(version)
  {
    case 80:
      print1("Converting version 0.80 savefile to current.");
      return true;
      break;
    case 81:
      print1("Loading version 0.81 savefile.");
      return true;
      break;
    default:
      return false;
      break;
  }
}

/* read player data, city level, dungeon level,
   check on validity of save file, etc.
   return true if game restored, false otherwise */

bool restore_game(char *savestr)
{
  int   i, version;
  FILE *fd;

  if(!std::filesystem::exists(savestr) || !std::filesystem::is_regular_file(savestr))
  {
    print1("Unable to access save file: ");
    nprint1(savestr);
    return false;
  }

  fd = fopen(savestr, "rb");

  if(!fd)
  {
    print1("Error restoring game -- aborted.");
    print2("File name was: ");
    nprint2(savestr);
    return false;
  }
  else
  {
    print1("Restoring...");

    fread((char *)&version, sizeof(int), 1, fd);

    if(VERSION != version && !ok_outdated(version))
    {
      fclose(fd);
      mprint(" Sorry, I can't restore an outdated save file!");
      mprint(" savefile is version " + std::to_string(version / 100) + "." +
             std::to_string(version % 100));
      return false;
    }
    restore_player(fd, version);
    restore_country(fd, version);
    restore_level(fd, version); /* the city level */
    fread((char *)&i, sizeof(int), 1, fd);
    for(; i > 0; i--)
    {
#ifdef SAVE_LEVELS
      msdos_changelevel(Level, 0, -1);
#endif
      restore_level(fd, version);
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
    /* this disgusting kludge because LENGTH and WIDTH are globals... */
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
    fclose(fd);
    print3("Restoration complete.");
    ScreenOffset = -1000; /* to force a redraw */
    setgamestatus(SKIP_MONSTERS, GameStatus);
    return (true);
  }
}

void restore_player(FILE *fd, int version)
{
  int i;
  fread((char *)&Player, sizeof(Player), 1, fd);
  filescanstring(fd, Password);
  filescanstring(fd, Player.name);
  fread((char *)CitySiteList, sizeof(CitySiteList), 1, fd);
  fread((char *)&GameStatus, sizeof(long), 1, fd);
  fread((char *)&Current_Environment, sizeof(int), 1, fd);
  fread((char *)&Last_Environment, sizeof(int), 1, fd);
  fread((char *)&Current_Dungeon, sizeof(int), 1, fd);
  fread((char *)&Villagenum, sizeof(int), 1, fd);
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
  fread((char *)&Verbosity, sizeof(char), 1, fd);
  fread((char *)&Time, sizeof(long), 1, fd);
  fread((char *)&Tick, sizeof(int), 1, fd);
  fread((char *)&Searchnum, sizeof(int), 1, fd);
  fread((char *)&Behavior, sizeof(int), 1, fd);
  fread((char *)&Phase, sizeof(int), 1, fd);
  fread((char *)&Date, sizeof(int), 1, fd);
  fread((char *)&Spellsleft, sizeof(int), 1, fd);
  fread((char *)&SymbolUseHour, sizeof(int), 1, fd);
  fread((char *)&ViewHour, sizeof(int), 1, fd);
  fread((char *)&HelmHour, sizeof(int), 1, fd);
  fread((char *)&Constriction, sizeof(int), 1, fd);
  fread((char *)&Blessing, sizeof(int), 1, fd);
  fread((char *)&LastDay, sizeof(int), 1, fd);
  fread((char *)&RitualHour, sizeof(int), 1, fd);
  fread((char *)&Lawstone, sizeof(int), 1, fd);
  fread((char *)&Chaostone, sizeof(int), 1, fd);
  fread((char *)&Mindstone, sizeof(int), 1, fd);
  fread((char *)&Arena_Opponent, sizeof(int), 1, fd);
  fread((char *)&Imprisonment, sizeof(int), 1, fd);
  fread((char *)&Gymcredit, sizeof(long), 1, fd);
  fread((char *)&Balance, sizeof(long), 1, fd);
  fread((char *)&StarGemUse, sizeof(int), 1, fd);
  fread((char *)&HiMagicUse, sizeof(int), 1, fd);
  fread((char *)&HiMagic, sizeof(int), 1, fd);
  fread((char *)&FixedPoints, sizeof(long), 1, fd);
  fread((char *)&LastCountryLocX, sizeof(int), 1, fd);
  fread((char *)&LastCountryLocY, sizeof(int), 1, fd);
  fread((char *)&LastTownLocX, sizeof(int), 1, fd);
  fread((char *)&LastTownLocY, sizeof(int), 1, fd);
  fread((char *)&Pawndate, sizeof(int), 1, fd);

  fread((char *)Spells, sizeof(Spells), 1, fd);

  fread((char *)&Command_Duration, sizeof(Command_Duration), 1, fd);
  fread((char *)&Precipitation, sizeof(Precipitation), 1, fd);
  fread((char *)&Lunarity, sizeof(Lunarity), 1, fd);
  fread((char *)&ZapHour, sizeof(ZapHour), 1, fd);
  fread((char *)&RitualRoom, sizeof(RitualRoom), 1, fd);

  /* stuff which used to be statics */
  fread((char *)&twiddle, sizeof(twiddle), 1, fd);
  fread((char *)&saved, sizeof(saved), 1, fd);
  fread((char *)&onewithchaos, sizeof(onewithchaos), 1, fd);
  fread((char *)&club_hinthour, sizeof(club_hinthour), 1, fd);
  fread((char *)&winnings, sizeof(winnings), 1, fd);
  fread((char *)&tavern_hinthour, sizeof(tavern_hinthour), 1, fd);
  fread((char *)scroll_ids, sizeof(scroll_ids), 1, fd);
  fread((char *)potion_ids, sizeof(potion_ids), 1, fd);
  fread((char *)stick_ids, sizeof(stick_ids), 1, fd);
  fread((char *)ring_ids, sizeof(ring_ids), 1, fd);
  fread((char *)cloak_ids, sizeof(cloak_ids), 1, fd);
  fread((char *)boot_ids, sizeof(boot_ids), 1, fd);
  fread((char *)deepest, sizeof(int), E_MAX + 1, fd);
  fread((char *)level_seed, sizeof(int), E_MAX + 1, fd);

  fread((char *)&received_directions, sizeof(bool), 1, fd);

  /* Set up the strings for the id's */
  inititem(false);

  for(i = 0; i < MAXITEMS; i++)
  {
    Player.possessions[i] = restore_item(fd, version);
  }

  if(!Player.possessions[O_READY_HAND] && Player.possessions[O_WEAPON_HAND] &&
     twohandedp(Player.possessions[O_WEAPON_HAND]->id))
  {
    Player.possessions[O_READY_HAND] = Player.possessions[O_WEAPON_HAND];
  }

  for(i = 0; i < MAXPACK; i++)
  {
    Player.pack[i] = restore_item(fd, version);
  }
  for(i = 0; i < PAWNITEMS; i++)
  {
    Pawnitems[i] = restore_item(fd, version);
  }
  Condoitems = restore_itemlist(fd, version);
  for(i = 0; i < TOTALITEMS; i++)
  {
    fread((char *)&(Objects[i].known), sizeof(Objects[i].known), 1, fd);
    if(version != 80)
    {
      fread((char *)&(Objects[i].uniqueness), sizeof(Objects[i].uniqueness), 1, fd);
    }
  }
}

/* Restore an item, the first byte tells us if it's NULL, and what strings */
/* have been saved as different from the typical */
pob restore_item(FILE *fd, int)
{
  char          tempstr[80];
  unsigned char type;
  pob           obj = NULL;

  fread((char *)&type, sizeof(type), 1, fd);
  if(type != 0xff)
  {
    obj = ((pob)checkmalloc(sizeof(objtype)));
    fread((char *)obj, sizeof(objtype), 1, fd);
    if(type & 1)
    {
      filescanstring(fd, tempstr);
      obj->objstr = salloc(tempstr);
    }
    else
    {
      obj->objstr = Objects[obj->id].objstr;
    }
    if(type & 2)
    {
      filescanstring(fd, tempstr);
      obj->truename = salloc(tempstr);
    }
    else
    {
      obj->truename = Objects[obj->id].truename;
    }
    if(type & 4)
    {
      filescanstring(fd, tempstr);
      obj->cursestr = salloc(tempstr);
    }
    else
    {
      obj->cursestr = Objects[obj->id].cursestr;
    }
  }
  return obj;
}

pol restore_itemlist(FILE *fd, int version)
{
  pol ol = NULL, cur = NULL, new_pol = NULL;
  int i, numitems;
  bool firsttime = true;
  fread((char *)&numitems, sizeof(int), 1, fd);
  for(i = 0; i < numitems; i++)
  {
    new_pol        = ((pol)checkmalloc(sizeof(oltype)));
    new_pol->thing = restore_item(fd, version);
    new_pol->next  = NULL;
    if(firsttime == true)
    {
      ol = cur  = new_pol;
      firsttime = false;
    }
    else
    {
      cur->next = new_pol;
      cur       = new_pol;
    }
  }
  return (ol);
}

void restore_level(FILE *fd, int version)
{
  int               i, j, run;
  unsigned long int mask;
  int               temp_env;

  Level = (plv)checkmalloc(sizeof(levtype));
  clear_level(Level);
  fread((char *)&Level->depth, sizeof(char), 1, fd);
  fread((char *)&Level->numrooms, sizeof(char), 1, fd);
  fread((char *)&Level->tunnelled, sizeof(char), 1, fd);
  fread((char *)&Level->environment, sizeof(int), 1, fd);
  Level->generated    = true;
  temp_env            = Current_Environment;
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
      print3("This dungeon not implemented!");
      break;
  }
  if(Level->depth > 0)
  { /* dungeon... */
    install_traps();
    install_specials();
    make_stairs(-1);
    make_stairs(-1);
    initrand(E_RESTORE, 0);
  }
  Current_Environment = temp_env;
  fread((char *)&i, sizeof(int), 1, fd);
  fread((char *)&j, sizeof(int), 1, fd);
  while(j < MAXLENGTH && i < MAXWIDTH)
  {
    fread((char *)&run, sizeof(int), 1, fd);
    for(; i < run; i++)
    {
      fread((char *)&Level->site[i][j], sizeof(struct location), 1, fd);
      Level->site[i][j].creature = NULL;
      Level->site[i][j].things   = NULL;
    }
    fread((char *)&i, sizeof(int), 1, fd);
    fread((char *)&j, sizeof(int), 1, fd);
  }
  run = 0;
  for(j = 0; j < MAXLENGTH; j++)
  {
    for(i = 0; i < MAXWIDTH; i++)
    {
      if(run == 0)
      {
        run = 8 * sizeof(long int);
        fread((char *)&mask, sizeof(long int), 1, fd);
      }
      if(mask & 1)
      {
        lset(i, j, SEEN, *Level);
      }
      mask >>= 1;
      run--;
    }
  }
  restore_monsters(fd, Level, version);
  fread((char *)&i, sizeof(int), 1, fd);
  fread((char *)&j, sizeof(int), 1, fd);
  while(j < MAXLENGTH && i < MAXWIDTH)
  {
    Level->site[i][j].things = restore_itemlist(fd, version);
    fread((char *)&i, sizeof(int), 1, fd);
    fread((char *)&j, sizeof(int), 1, fd);
  }
}

void restore_hiscore_npc(pmt npc, int npcid)
{
  int  level, behavior;
  long status;

  switch(npcid)
  {
    case 0:
      strcpy(Str2, Hiscorer);
      level    = Hilevel;
      behavior = Hibehavior;
      break;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
      strcpy(Str2, Priest[npcid]);
      level    = Priestlevel[npcid];
      behavior = Priestbehavior[npcid];
      break;
    case 7:
      strcpy(Str2, Shadowlord);
      level    = Shadowlordlevel;
      behavior = Shadowlordbehavior;
      break;
    case 8:
      strcpy(Str2, Commandant);
      level    = Commandantlevel;
      behavior = Commandantbehavior;
      break;
    case 9:
      strcpy(Str2, Archmage);
      level    = Archmagelevel;
      behavior = Archmagebehavior;
      break;
    case 10:
      strcpy(Str2, Prime);
      level    = Primelevel;
      behavior = Primebehavior;
      break;
    case 11:
      strcpy(Str2, Champion);
      level    = Championlevel;
      behavior = Championbehavior;
      break;
    case 12:
      strcpy(Str2, Duke);
      level    = Dukelevel;
      behavior = Dukebehavior;
      break;
    case 13:
      strcpy(Str2, Chaoslord);
      level    = Chaoslordlevel;
      behavior = Chaoslordbehavior;
      break;
    case 14:
      strcpy(Str2, Lawlord);
      level    = Lawlordlevel;
      behavior = Lawlordbehavior;
      break;
    default:
      strcpy(Str2, Justiciar);
      level    = Justiciarlevel;
      behavior = Justiciarbehavior;
  }
  npc->monstring = salloc(Str2);
  strcpy(Str1, "The body of ");
  strcat(Str1, Str2);
  npc->corpsestr = salloc(Str1);
  if(!m_statusp(*npc, HOSTILE))
  {
    status = npc->status;
    determine_npc_behavior(npc, level, behavior);
    npc->status = status;
  }
}

void restore_monsters(FILE *fd, plv level, int version)
{
  pml           ml = NULL;
  int           i, nummonsters;
  char          tempstr[80];
  int           temp_x, temp_y;
  unsigned char type;

  level->mlist = NULL;

  fread((char *)&nummonsters, sizeof(int), 1, fd);

  for(i = 0; i < nummonsters; i++)
  {
    ml       = ((pml)checkmalloc(sizeof(mltype)));
    ml->m    = ((pmt)checkmalloc(sizeof(montype)));
    ml->next = NULL;
    fread((char *)ml->m, sizeof(montype), 1, fd);
    if(ml->m->id == HISCORE_NPC)
    {
      if(version == 80)
      {
        temp_x = ml->m->x;
        temp_y = ml->m->y;
        make_hiscore_npc(ml->m, ml->m->aux2);
        ml->m->x = temp_x;
        ml->m->y = temp_y;
      }
      else
      {
        restore_hiscore_npc(ml->m, ml->m->aux2);
      }
    }
    else
    {
      fread((char *)&type, sizeof(unsigned char), 1, fd);
      if(type & 1)
      {
        filescanstring(fd, tempstr);
        ml->m->monstring = salloc(tempstr);
      }
      else
      {
        ml->m->monstring = Monsters[ml->m->id].monstring;
      }
      if(type & 2)
      {
        filescanstring(fd, tempstr);
        ml->m->corpsestr = salloc(tempstr);
      }
      else
      {
        ml->m->corpsestr = Monsters[ml->m->id].corpsestr;
      }
      /* WDT: As suggested by Sheldon Simms, I'm moving this line... */
      if(version <= 80)
      {
        ml->m->possessions = restore_itemlist(fd, version);
      }
      ml->m->meleestr = Monsters[ml->m->id].meleestr;
    }
    /* WDT: ...to here, so that all creatures will have their stuff
     * restored to them.  Savefile versioning added by David Given. */
    if(version > 80)
    {
      ml->m->possessions = restore_itemlist(fd, version);
    }
    level->site[ml->m->x][ml->m->y].creature = ml->m;
    ml->next                                 = level->mlist;
    level->mlist                             = ml;
  }
}

void restore_country(FILE *fd, int)
{
  int               i, j;
  int               run;
  unsigned long int mask;

  load_country();
  fread((char *)&i, sizeof(int), 1, fd);
  fread((char *)&j, sizeof(int), 1, fd);
  while(i < MAXWIDTH && j < MAXLENGTH)
  {
    fread((char *)&Country[i][j], sizeof(struct terrain), 1, fd);
    fread((char *)&i, sizeof(int), 1, fd);
    fread((char *)&j, sizeof(int), 1, fd);
  }
  run = 0;
  for(i = 0; i < MAXWIDTH; i++)
  {
    for(j = 0; j < MAXLENGTH; j++)
    {
      if(run == 0)
      {
        run = 8 * sizeof(long int);
        fread((char *)&mask, sizeof(long int), 1, fd);
      }
      if(mask & 1)
      {
        c_set(i, j, SEEN, Country);
      }
      mask >>= 1;
      run--;
    }
  }
}
