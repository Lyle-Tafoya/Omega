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

/* this file contains extern declarations of global variables used
   throughout the program */
/* it includes the other header files, so every program module
(except o.c) can just include this file. */

#ifndef OMEGA_GLOB_H_
#define OMEGA_GLOB_H_

/* glob.h */

#include "defs.h"
#include "extern.h"

/* This string holds the path to the library files */
extern const char *Omegalib;

/* one of each monster */
extern monster Monsters[NUMMONSTERS];

/* one of each item */
extern object Objects[TOTALITEMS];

/* locations of city sites [0] - found, [1] - x, [2] - y */
extern int CitySiteList[NUMCITYSITES][3];

extern player Player;
/* the player */

extern int LENGTH;
/* level y dimension */

extern int WIDTH;
/* level x dimension */

extern long GameStatus;
/* Game Status bit vector */

extern int ScreenLength;
/* How large is level window */

extern int ScreenWidth;

extern terrain Country[MAXWIDTH][MAXLENGTH];
/* The countryside */

extern level *City;
/* The city of Rampart */

extern level *TempLevel;
/* Place holder */

extern level *Dungeon;
/* Pointer to current Dungeon */

extern level *Level;
/* Pointer to current Level */

extern int Current_Dungeon;
/* What is Dungeon now (an E_ constant) */

extern int Villagenum;
/* Current Village number */

extern int ScreenOffset;
/* Offset of displayed screen to level */

extern int HorizontalOffset;

extern int MaxDungeonLevels;
/*Deepest level allowed in dungeon */

extern int Current_Environment;
/* Which environment are we in (an E_ constant) */

extern int Last_Environment;
/* Which environment were we in last (an E_ constant) */

extern int Dirs[2][9];
/* 9 xy directions */

extern int Cmd;
/* last player command */

extern int Command_Duration;
/* how long does current command take */

extern monster *Arena_Monster;
/* Opponent in arena */

extern int Arena_Opponent;
/* case label of opponent in l_arena()*/

extern int Arena_Victory;
/* did player win in arena? */

extern int Imprisonment;
/* amount of time spent in jail */

extern int Precipitation;
/* Hours of rain, snow, etc */

extern int Phase;
/* Phase of the moon */

extern int Lunarity;
/* How player is affected by moon */

extern int Date;
/* day of the year */

extern int Pawndate;
/* Pawn Shop item generation date */

extern pob Pawnitems[PAWNITEMS];
/* items in pawn shop */

extern int ViewDay;
extern int ViewHour;
/* crystal ball use marker */

extern int ZapDay;
extern int ZapHour;
/* staff of enchantment use marker */

extern int HelmDay;
extern int HelmHour;
/* helm of teleportation use marker*/

extern int SymbolUseDay;
extern int SymbolUseHour;
/* holy symbol use marker */

extern int Constriction;
/* Dragonlord Attack State */

extern int Blessing;
/* Altar Blessing State */

extern int LastDay;
/* DPW date of dole */

extern int RitualDay;
extern int RitualHour;
/* last use of ritual magic */

extern int RitualRoom;
/* last room use of ritual magic */

extern int Lawstone;
/* magic stone counter */

extern int Chaostone;
/* magic stone counter */

extern int Mindstone;
/* magic stone counter */

extern int Searchnum;
/* number of times to search on 's' */

extern int Verbosity;
/* verbosity level */

extern int Behavior;
/* NPC behavior, if entered */

extern char Seed;
/* random seed */

extern long Time;
/* turn number */

extern int Tick;
/* current second in minute; action coordinator */

extern std::array<std::string, STRING_BUFFER_SIZE> Stringbuffer;
/* the last printed strings */

extern long Gymcredit;
/* credit at rampart gym */

extern int Studiesleft;

extern int Spellsleft;
/* research allowance at college */

extern int StarGemUse;
/* last date of star gem use */

extern int HiMagicUse;
/* last date of high magic use */

extern int HiMagic;
/* current level for l_throne */

extern long Balance;
/* bank account */

extern long FixedPoints;
/* points are frozen after adepthood*/

extern int LastCountryLocX;
/* previous position in countryside */

extern int LastCountryLocY;
/* previous position in countryside */

extern int LastTownLocX;
/* previous position in village or city */

extern int LastTownLocY;
/* previous position in village or city */

extern std::string Password;
/* autoteller password */

extern int MazeNum;

extern objectlist *Condoitems;
/* items in condo */

/* high score names, levels, behavior */

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

/* New globals which used to be statics */
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
