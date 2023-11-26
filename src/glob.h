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

// this file contains extern declarations of global variables used
// throughout the program
// it includes the other header files, so every program module
// (except o.c) can just include this file.

#ifndef OMEGA_GLOB_H_
#define OMEGA_GLOB_H_

// glob.h

#include "defs.h"
#include "extern.h"
#include "player.h"

#include <memory>

// This string holds the path to the library files
extern const char *Omegalib;

// one of each monster
extern monster Monsters[NUMMONSTERS];

// one of each item
extern object Objects[TOTALITEMS];

// locations of city sites [0] - found, [1] - x, [2] - y
extern int CitySiteList[NUMCITYSITES][3];

// the player
extern player Player;

// level y dimension
extern int LENGTH;

// level x dimension
extern int WIDTH;

// Game Status bit vector
extern long GameStatus;

// How large is level window
extern int ScreenLength;
extern int ScreenWidth;

// The countryside
extern terrain Country[MAXWIDTH][MAXLENGTH];

// The city of Rampart
extern level *City;

// Place holder
extern level *TempLevel;

// Pointer to current Dungeon
extern level *Dungeon;

// Pointer to current Level
extern level *Level;

// What is Dungeon now (an E_ constant)
extern int Current_Dungeon;

// Current Village number
extern int Villagenum;

// Offset of displayed screen to level
extern int ScreenOffset;

extern int HorizontalOffset;

//Deepest level allowed in dungeon
extern int MaxDungeonLevels;

// Which environment are we in (an E_ constant)
extern int Current_Environment;

// Which environment were we in last (an E_ constant)
extern int Last_Environment;

// 9 xy directions
extern int Dirs[2][9];

// last player command
extern int Cmd;

// how long does current command take
extern int Command_Duration;

// Opponent in arena
extern monster *Arena_Monster;

// case label of opponent in l_arena(
extern int Arena_Opponent;

// did player win in arena?
extern int Arena_Victory;

// amount of time spent in jail
extern int Imprisonment;

// Hours of rain, snow, etc
extern int Precipitation;

// Phase of the moon
extern int Phase;

// How player is affected by moon
extern int Lunarity;

// day of the year
extern int Date;

// Pawn Shop item generation date
extern int Pawndate;

// items in pawn shop
extern std::array<std::unique_ptr<object>, PAWNITEMS> Pawnitems;

// crystal ball use marker
extern int ViewDay;
extern int ViewHour;

// staff of enchantment use marker
extern int ZapDay;
extern int ZapHour;

// helm of teleportation use marke
extern int HelmDay;
extern int HelmHour;

// holy symbol use marker
extern int SymbolUseDay;
extern int SymbolUseHour;

// Dragonlord Attack State
extern int Constriction;

// Altar Blessing State
extern int Blessing;

// DPW date of dole
extern int LastDay;

extern int RitualDay;
// last use of ritual magic
extern int RitualHour;

// last room use of ritual magic
extern int RitualRoom;

// magic stone counter
extern int Lawstone;

// magic stone counter
extern int Chaostone;

// magic stone counter
extern int Mindstone;

// number of times to search on 's'
extern int Searchnum;

// verbosity level
extern int Verbosity;

// NPC behavior, if entered
extern int Behavior;

// random seed
extern char Seed;

// turn number
extern long Time;

// current second in minute; action coordinator
extern int Tick;

// the last printed strings
extern std::array<std::string, STRING_BUFFER_SIZE> Stringbuffer;

// credit at rampart gym
extern long Gymcredit;

extern int Studiesleft;

// research allowance at college
extern int Spellsleft;

// last date of star gem use
extern int StarGemUse;

// last date of high magic use
extern int HiMagicUse;

// current level for l_throne
extern int HiMagic;

// bank account
extern long Balance;

// points are frozen after adepthood
extern long FixedPoints;

// previous position in countryside
extern int LastCountryLocX;

// previous position in countryside
extern int LastCountryLocY;

// previous position in village or city
extern int LastTownLocX;

// previous position in village or city
extern int LastTownLocY;

// autoteller password
extern std::string Password;

extern int MazeNum;

// items in condo
extern std::forward_list<std::unique_ptr<object>> Condoitems;

// high score names, levels, behavior
extern int Shadowlordbehavior, Archmagebehavior, Primebehavior, Justiciarbehavior;
extern int Commandantbehavior, Chaoslordbehavior, Lawlordbehavior, Grandmasterbehavior;
extern int Championbehavior, Priestbehavior[7], Hibehavior, Dukebehavior;
extern std::string Shadowlord, Archmage, Prime, Commandant, Duke;
extern std::string Champion, Priest[7], Hiscorer, Hidescrip;
extern std::string Chaoslord, Lawlord, Justiciar, Grandmaster;
extern int Shadowlordlevel, Archmagelevel, Primelevel, Commandantlevel, Dukelevel;
extern int Championlevel, Priestlevel[7], Hilevel, Justiciarlevel, Grandmasterlevel;
extern long Hiscore;
extern int Chaoslordlevel, Lawlordlevel, Chaos, Law;

// New globals which used to be statics
extern int twiddle;
extern int saved;
extern int onewithchaos;
extern int club_hinthour;
extern int winnings;
extern int tavern_hinthour;
extern int scroll_ids[30];
extern int potion_ids[30];
extern int stick_ids[30];
extern int ring_ids[30];
extern int cloak_ids[30];
extern int boot_ids[30];

extern int deepest[E_MAX + 1];
extern int level_seed[E_MAX + 1];

extern bool received_directions;

#endif
