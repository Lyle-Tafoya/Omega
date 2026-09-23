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

// Definitions of all global game-state variables declared extern in glob.h.
// Moved here from omega.cpp so front-ends other than main() can link them.

#include "glob.h"

#include <array>
#include <string>

const char *Omegalib;

int CitySiteList[NUMCITYSITES][3];

int  LENGTH               = MAXLENGTH;
int  WIDTH                = MAXWIDTH;
bool terminal_size_too_small;
bool received_directions  = false;
bool IsMenu               = false;
long GameStatus           = 0L;
int  ScreenLength         = 0;
int  ScreenWidth          = 0;

player                               Player;
terrain                              Country[MAXWIDTH][MAXLENGTH];
std::unique_ptr<level>               TempLevel;
std::unique_ptr<level>               City;
std::vector<std::unique_ptr<level>>  Dungeon;
level                               *Level              = nullptr;

int Villagenum          = 0;
int ScreenOffset        = 0;
int HorizontalOffset    = 0;
int MaxDungeonLevels    = 0;
int Current_Dungeon     = -1;
int Current_Environment = E_CITY;
int Last_Environment    = E_COUNTRYSIDE;
int Dirs[2][9];
int Cmd                 = 's';
int Command_Duration    = 0;

monster *Arena_Monster  = nullptr;
int      Arena_Opponent = 0;
int      Arena_Victory  = 0;

int Imprisonment  = 0;
int Precipitation = 0;
int Lunarity      = 0;
int Phase         = 0;
int Date          = 0;
int Pawndate      = -1;

std::array<std::unique_ptr<object>, PAWNITEMS> Pawnitems;

int SymbolUseDay  = -1;
int SymbolUseHour = -1;
int ViewDay       = -1;
int ViewHour      = -1;
int ZapDay        = -1;
int ZapHour       = -1;
int HelmDay       = -1;
int HelmHour      = -1;

int Constriction = 0;
int Blessing     = false;
int LastDay      = -1;
int RitualDay    = -1;
int RitualHour   = -1;
int RitualRoom   = -1;

int Lawstone  = 0;
int Chaostone = 0;
int Mindstone = 0;
int Searchnum = 1;
int Behavior;
int Verbosity = VERBOSE;

long Time      = 0;
int  Tick      = 0;
long Gymcredit = 0;
int  Spellsleft  = 0;
int  Studiesleft = 0;

int StarGemUse = 0;
int HiMagicUse = 0;
int HiMagic    = 0;

long Balance     = 0;
long FixedPoints = 0;

int LastTownLocX    = 0;
int LastTownLocY    = 0;
int LastCountryLocX = 0;
int LastCountryLocY = 0;

std::string Password;
int         MazeNum = 0;

std::forward_list<std::unique_ptr<object>> Condoitems;

int Shadowlordbehavior, Archmagebehavior, Primebehavior, Commandantbehavior;
int Championbehavior, Priestbehavior[7], Hibehavior, Dukebehavior;
int Chaoslordbehavior, Lawlordbehavior, Justiciarbehavior, Grandmasterbehavior;

std::string Shadowlord, Archmage, Prime, Commandant, Duke;
std::string Champion, Priest[7], Hiscorer, Hidescrip;
std::string Chaoslord, Lawlord, Justiciar, Grandmaster;

int Shadowlordlevel, Archmagelevel, Primelevel, Commandantlevel, Dukelevel;
int Championlevel, Priestlevel[7], Hilevel, Justiciarlevel, Grandmasterlevel;
long Hiscore       = 0L;
int  Chaoslordlevel = 0, Lawlordlevel = 0, Chaos = 0, Law = 0;

int twiddle        = false;
int saved          = false;
int onewithchaos   = false;
int club_hinthour  = 0;
int winnings       = 0;
int tavern_hinthour;
int scroll_ids[30];
int potion_ids[30];
int stick_ids[30];
int ring_ids[30];
int cloak_ids[30];
int boot_ids[30];

int deepest[E_MAX + 1];
int level_seed[E_MAX + 1];
