/* omega copyright (c) 1987,1988 by Laurence Raphael Brothers */

/* this file contains extern declarations of global variables used
   throughout the program */
/* it includes the other header files, so every program module 
(except o.c) can just include this file. */

/* oglob.h */

#include "odefs.h"

#include "oextern.h"

/* one of each monster */
extern struct monster Monsters[NUMMONSTERS];

/* one of each spell */
extern struct spell Spells[NUMSPELLS+1];

/* one of each item */
extern struct object Objects[TOTALITEMS];

/* locations of city sites [0] - found, [1] - x, [2] - y */
extern int CitySiteList[NUMCITYSITES][3];

extern struct player Player;                 
/* the player */

extern int LENGTH;
/* level y dimension */

extern int WIDTH;
/* level x dimension */

extern int GameStatus;
/* Game Status bit vector */

extern int ScreenLength;
/* How large is level window */

extern struct terrain Country[MAXWIDTH][MAXLENGTH];
/* The countryside */

extern struct level *City;
/* The city of Rampart */

extern struct level *TempLevel;
/* Place holder */

extern struct level *Dungeon;
/* Pointer to current Dungeon */

extern struct level *Level;
/* Pointer to current Level */

extern int Current_Dungeon;
/* What is Dungeon now (an E_ constant) */

extern int Villagenum;
/* Current Village number */ 

extern int ScreenOffset;
/* Offset of displayed screen to level */

extern int MaxDungeonLevels;
/*Deepest level allowed in dungeon */

extern int Current_Environment;
/* Which environment are we in (an E_ constant) */

extern int Last_Environment;
/* Which environment were we in last (an E_ constant) */

extern int Dirs[2][9];                       
/* 9 xy directions */

extern char Cmd;                         
/* last player command */

extern int Command_Duration;
/* how long does current command take */

extern struct monster *Arena_Monster;
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

extern int ViewHour;
/* crystal ball use marker */

extern int ZapHour;
/* staff of enchantment use marker */

int HelmHour;
/* helm of teleportation use marker*/

extern int SymbolUseHour;                
/* holy symbol use marker */

extern int Constriction;
/* Dragonlord Attack State */

extern int Blessing;
/* Altar Blessing State */

extern int LastDay;
/* DPW date of dole */

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

extern char Seed;                            
/* random seed */

extern int Time;
/* turn number */

extern int Tick;
/* current second in minute; action coordinator */

extern char Stringbuffer[10][80];
/* the last printed strings */

extern int Gymcredit;
/* credit at rampart gym */

extern int Spellsleft;
/* research allowance at college */

extern int StarGemUse;
/* last date of star gem use */

extern int HiMagicUse;
/* last date of high magic use */

extern int HiMagic;
/* current level for l_throne */ 

extern int Balance;
/* bank account */

extern int FixedPoints;
/* points are frozen after adepthood*/

extern int LastCountryLocX;
/* previous position in countryside */

extern int LastCountryLocY;
/* previous position in countryside */

extern int LastTownLocX;
/* previous position in village or city */

extern int LastTownLocY;
/* previous position in village or city */

extern char Password[64];
/* autoteller password */

pol Condoitems;
/* items in condo */

extern char Str1[100],Str2[100],Str3[100],Str4[100];
/* Some string space, random uses */



/* high score names, levels, behavior */

extern int Shadowlordbehavior,Archmagebehavior,Primebehavior,Justiciarbehavior;
extern int Commandantbehavior,Chaoslordbehavior,Lawlordbehavior;
extern int Championbehavior,Priestbehavior[7],Hibehavior,Dukebehavior;
extern char Shadowlord[80],Archmage[80],Prime[80],Commandant[80],Duke[80];
extern char Champion[80],Priest[7][80],Hiscorer[80],Hidescrip[80];
extern char Chaoslord[80],Lawlord[80],Justiciar[80];
extern int Shadowlordlevel,Archmagelevel,Primelevel,Commandantlevel,Dukelevel;
extern int Championlevel,Priestlevel[7],Hiscore,Hilevel,Justiciarlevel;
extern int Chaoslordlevel,Lawlordlevel,Chaos,Law;

