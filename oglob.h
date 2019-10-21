/* omega copyright (c) 1987 by Laurence Raphael Brothers */

/* this file contains extern declarations of global variables used
   throughout the program */

/* oglob.h */

#include "odefs.h"

/* one of each monster */
extern struct monster Monsters[NUMMONSTERS];

/* one of each spell */
extern struct spell Spells[NUMSPELLS+1];

/* one of each item */
extern struct object Objects[TOTALITEMS];

extern pml Mlist[NUMLEVELS];                 
/* monsters on each level */

extern struct player Player;                 
/* the player */

extern struct location Dungeon[NUMLEVELS][WIDTH][LENGTH];  
/* the whole dungeon */

extern struct level_data Leveldata[NUMLEVELS];  
/* info about various levels */

extern int Dirs[2][9];                       
/* 9 xy directions */

extern char Cmd;                         
/* last player command */

extern int Lunarity;
/* Player affected by phae of moon? */

extern int Phase;
/* Phase of the moon */

extern int Date;
/* day of the year */

extern int Searchnum;                    
/* number of times to search on 's' */

extern int Wizard;                   
/* Wizard mode flag */

extern int Cheated;
/* If ever used wizard mode */

extern int Fastmove;
/* are we in the middle of a fast move */

extern int Skipmonsters;
/* don't deal with them this move */

extern int Skipplayer;
/* don't deal with him this move */

extern int Gameover;
/* yow! are we having fun yet? */

extern int Dlevel;
/* current dungeon level */

extern int WhichScreen;
/* which screen are we on? (0..3) */

extern char Seed;                            
/* random seed */

extern int Time;
/* turn number */

extern int Tick;
/* 1/10 a turn; action coordinator */

extern char Laststring[80];                  
/* the last mprint string */

extern int Logsize;                          
/* number of players in log */

extern int Tacmode;
/* In tactical combat mode? */

extern int Arena;
/* Arena combat leaves no corpses */

extern int Final;
/* Final duel flag */

extern int Gymcredit;
/* credit at rampart gym */

extern int Balance;
/* bank account */

extern int Bank_Broken;
/* has bank been robbed */

extern char Password[64];
/* autoteller password */

extern int Deepest;
/* The deepest level attained */

extern int SuppressPrinting;
/* turn off mprint, printm, when TRUE */

extern char Str1[100],Str2[100],Str3[100],Str4[100];
/* Some string space, random uses */



/* high score names, levels, behavior */

extern int Shadowlordbehavior,Archmagebehavior,Primebehavior;
extern int Commandantbehavior;
extern int Championbehavior,Priestbehavior[7],Hibehavior,Dukebehavior;
extern char Shadowlord[80],Archmage[80],Prime[80],Commandant[80],Duke[80];
extern char Champion[80],Priest[7][80],Hiscorer[80],Hidescrip[80];
extern int Shadowlordlevel,Archmagelevel,Primelevel,Commandantlevel,Dukelevel;
extern int Championlevel,Priestlevel[7],Hiscore,Hilevel;

extern int Clubmember;
extern int Sawdruid;
extern int Soldcondo;
extern pol Condoitems;

extern int Thieflevel,Clericlevel,Sorcerorlevel,Collegelevel,Merclevel;
