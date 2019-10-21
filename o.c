/* omega copyright (c) 1987,1988 by Laurence Raphael Brothers */

/* this file includes main() and some top-level functions */
/* o.c */

#include <signal.h>

#include "odefs.h"

#include "oextern.h"

#include "ominit.h"

#include "oiinit.h"

/* most globals originate in o.c */

/* one of each spell */
struct spell Spells[NUMSPELLS+1];

/* locations of city sites [0] - found, [1] - x, [2] - y */
int CitySiteList[NUMCITYSITES][3];

/* Currently defined in caps since it is now a variable, was a constant */
int LENGTH=MAXLENGTH; 
int WIDTH=MAXWIDTH;

int GameStatus=0L;                    /* Game Status bit vector */
int ScreenLength;                     /* How large is level window */
struct player Player;                 /* the player */
struct terrain Country[MAXWIDTH][MAXLENGTH];/* The countryside */
struct level *City=NULL;              /* The city of Rampart */
struct level *TempLevel=NULL;         /* Place holder */
struct level *Level=NULL;             /* Pointer to current Level */
struct level *Dungeon=NULL;           /* Pointer to current Dungeon */
int Villagenum;                       /* Current Village number */ 
int ScreenOffset;                     /* Offset of displayed screen to level */
int MaxDungeonLevels;                 /* Deepest level allowed in dungeon */
int Current_Dungeon= -1;              /* What is Dungeon now */
int Current_Environment= E_CITY;      /* Which environment are we in */
int Last_Environment= E_COUNTRYSIDE;  /* Which environment were we in */
int Dirs[2][9];                       /* 9 xy directions */
char Cmd='s';                         /* last player command */
int Command_Duration = 0;             /* how long does current command take */
struct monster *Arena_Monster=NULL;   /* Opponent in arena */
int Arena_Opponent=0;                 /* case label of opponent in l_arena()*/
int Arena_Victory;                    /* did player win in arena? */
int Imprisonment=0;                   /* amount of time spent in jail */
int Precipitation=0;                  /* Hours of rain, snow, etc */
int Lunarity=0;                       /* Effect of the moon on character */
int Phase;                            /* Phase of the moon */
int Date;                             /* Starting date */
int Pawndate;                         /* Pawn Shop item generation date */
pob Pawnitems[PAWNITEMS] = {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
/* items in pawn shop */
int SymbolUseHour= -1;                /* holy symbol use marker */
int ViewHour= -1;                     /* crystal ball use marker */
int ZapHour= -1;                      /* staff of enchantment use marker */
int HelmHour= -1;                     /* helm of teleportation use marker*/
int Constriction=0;                   /* Dragonlord Attack State */
int Blessing=FALSE;                   /* Altar Blessing State */
int LastDay= -1;                      /* DPW date of dole */
int RitualHour= -1;                   /* last use of ritual magic */
int RitualRoom= -1;                   /* last room of ritual magic */
int Lawstone=0;                       /* magic stone counter */
int Chaostone=0;                      /* magic stone counter */
int Mindstone=0;                      /* magic stone counter */
int Searchnum = 1;                    /* number of times to search on 's' */
int Verbosity = VERBOSE;              /* verbosity level */
char Seed;                            /* random seed */
int Time = 0;                         /* turn number */
int Tick = 0;                         /* 10 a turn; action coordinator */
char Stringbuffer[10][80] = {
  "First String",  "First String",  "First String",  "First String",  
  "First String",  "First String",  "First String",  "First String",
  "First String",  "First String"}; /* last 10 strings */
int Gymcredit = 0;                    /* credit at rampart gym */
int Spellsleft = 0;                   /* research allowance at college */
int StarGemUse = 0;                   /* last date of star gem use */
int HiMagicUse = 0;                   /* last date of high magic use */
int HiMagic = 0;                      /* current level for l_throne */ 
int Balance = 0;                      /* bank account */
int FixedPoints = 0;                  /* points are frozen after adepthood*/
int LastTownLocX=0;            /* previous position in village or city */
int LastTownLocY=0;            /* previous position in village or city */
int LastCountryLocX=0;            /* previous position in countryside */
int LastCountryLocY=0;            /* previous position in countryside */
char Password[64];                    /* autoteller password */
char Str1[100],Str2[100],Str3[100],Str4[100];
   /* Some string space, random uses */

pol Condoitems=NULL;                        /* Items in condo */

/* high score names, levels, behavior */
int Shadowlordbehavior,Archmagebehavior,Primebehavior,Commandantbehavior;
int Championbehavior,Priestbehavior[7],Hibehavior,Dukebehavior;
int Chaoslordbehavior,Lawlordbehavior,Justiciarbehavior;
char Shadowlord[80],Archmage[80],Prime[80],Commandant[80],Duke[80];
char Champion[80],Priest[7][80],Hiscorer[80],Hidescrip[80];
char Chaoslord[80],Lawlord[80],Justiciar[80];
int Shadowlordlevel,Archmagelevel,Primelevel,Commandantlevel,Dukelevel;
int Championlevel,Priestlevel[7],Hiscore,Hilevel,Justiciarlevel;
int Chaoslordlevel,Lawlordlevel,Chaos,Law;

/* This may be implementation dependent */
/* SRANDFUNCTION is defined in odefs.h */
void initrand()
{
  SRANDFUNCTION;
}


int game_restore(argc,argv)
int argc;
char *argv[];
{
  char savestr[80];
  int ok;
  if (argc==2) {
    strcpy(savestr,argv[1]);
    ok = restore_game(savestr);
    unlink(savestr);
    if (! ok) {
      endgraf();
      printf("Try again with the right save file, luser!\n");
      exit(0);
    }
    return(TRUE);
  }
  else return(FALSE);
}


main(argc,argv)
int argc;
char *argv[];
{
  int continuing;


  /* always catch ^c and hang-up signals */

  signal(SIGINT,quit);
  signal(SIGHUP,signalsave);

  if (CATCH_SIGNALS) {
    signal(SIGQUIT,signalexit);
    signal(SIGILL,signalexit);
    signal(SIGTRAP,signalexit);
    signal(SIGIOT,signalexit);
    signal(SIGEMT,signalexit);
    signal(SIGFPE,signalexit);
    signal(SIGBUS,signalexit);
    signal(SIGSEGV,signalexit);
    signal(SIGSYS,signalexit);
    }



  /* if filecheck is 0, some necessary data files are missing */
  if (filecheck() == 0) exit(0);

  /* all kinds of initialization */
  initgraf();
  initdirs();
  initrand();
  inititem();
  initspells();

  strcpy(Stringbuffer[0],"First String");
  strcpy(Stringbuffer[1],"First String");
  strcpy(Stringbuffer[2],"First String");
  strcpy(Stringbuffer[3],"First String");
  strcpy(Stringbuffer[4],"First String");
  strcpy(Stringbuffer[5],"First String");
  strcpy(Stringbuffer[6],"First String");
  strcpy(Stringbuffer[7],"First String");
  strcpy(Stringbuffer[8],"First String");
  strcpy(Stringbuffer[9],"First String");


  /* game restore attempts to restore game if there is an argument */
  continuing = game_restore(argc,argv);


  /* monsters initialized in game_restore if game is being restored */  
  if (! continuing) {
    initplayer();
    
    Date = random_range(360);
    Phase = random_range(24);
    moon_check();
    strcpy(Password,"");

    init_world();
    
    mprint("'?' for help or commandlist, 'Q' to quit.");
  }
  else mprint("Your adventure continues....");

  dataprint();
  timeprint();
  showflags();

  screencheck(Player.y);

 /* game cycle */
  time_clock(TRUE);
  while (TRUE) {
    if (Current_Environment == E_COUNTRYSIDE)
      p_country_process();
    else time_clock(FALSE);
  }
}

int signalexit()
{
  mprint("Yikes!");
  morewait();
  mprint("Sorry, caught a core-dump signal.");
  mprint("Want to try and save the game?");
  if (ynq()=='y')
    save(FALSE); /* don't compress */
  mprint("Bye!");
  endgraf();
  exit(0);
}




/* Start up game with new dungeons; start with player in city */
void init_world()
{
  if (Level != NULL) free((char *) Level);
  if (City != NULL) free((char *) City);
  if (TempLevel != NULL) free((char *) TempLevel);
  if (Dungeon != NULL) free_dungeon();
  City = Level = TempLevel = Dungeon = NULL;
  load_country();
  load_city();
  change_environment(E_CITY);
  locprint("The City of Rampart.");
}

/* set variable item names */
void inititem()
{
  int i;
  scrollname(TRUE,0);
  for(i=0;i<NUMSCROLLS;i++) {
    Objects[SCROLLID+i].objstr = salloc(scrollname(FALSE,i));
  }
  potionname(TRUE,0);
  for(i=0;i<NUMPOTIONS;i++) {
    Objects[POTIONID+i].objstr = salloc(potionname(FALSE,i));
  }
  stickname(TRUE,0);
  for(i=0;i<NUMSTICKS;i++) {
    Objects[STICKID+i].objstr = salloc(stickname(FALSE,i));
  }
  bootname(TRUE,0);
  for(i=0;i<NUMBOOTS;i++) {
    Objects[BOOTID+i].objstr = salloc(bootname(FALSE,i));
  }
  cloakname(TRUE,0);
  for(i=0;i<NUMCLOAKS;i++) {
    Objects[CLOAKID+i].objstr = salloc(cloakname(FALSE,i));
  }
  ringname(TRUE,0);
  for(i=0;i<NUMRINGS;i++) {
    Objects[RINGID+i].objstr = salloc(ringname(FALSE,i));
  }
}

