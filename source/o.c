/* omega copyright (c) 1987,1988,1989 by Laurence Raphael Brothers */

/* this file includes main() and some top-level functions */
/* o.c */

#include <alloc.h> //
#include <signal.h>
#ifdef MSDOS
#include <fcntl.h>
/* Note: in order to avoid a memory bug I've been told about, I'm
   explicitly initializing every global to something. */
#endif

#include "oglob.h"


/* most globals originate in o.c */

/* Objects and Monsters are allocated and initialized in oinit.c */

/* one of each spell */
#ifndef MSDOS
struct spell Spells[NUMSPELLS+1];
#else
struct spell Spells[NUMSPELLS+1] = {0};
#endif

/* locations of city sites [0] - found, [1] - x, [2] - y */
#ifndef MSDOS
int CitySiteList[NUMCITYSITES][3];
#else
int CitySiteList[NUMCITYSITES][3] = {0};
#endif

/* Currently defined in caps since it is now a variable, was a constant */
int LENGTH=MAXLENGTH; 
int WIDTH=MAXWIDTH;

#ifndef MSDOS
int GameStatus=0L;                    /* Game Status bit vector */
#else
long GameStatus=0L;                   /* Game Status bit vector */
#endif
#ifndef MSDOS
int ScreenLength;                     /* How large is level window */
#else
int ScreenLength = 0;                 /* How large is level window */
#endif
#ifndef MSDOS
struct player Player;                 /* the player */
#else
struct player Player = {0};           /* the player */
#endif
#ifndef MSDOS
struct terrain Country[MAXWIDTH][MAXLENGTH];/* The countryside */
#else
struct terrain Country[MAXWIDTH][MAXLENGTH] = {0};/* The countryside */
#endif
#ifdef MSDOS
struct level TheLevel;
#endif
struct level *City=NULL;              /* The city of Rampart */
struct level *TempLevel=NULL;         /* Place holder */
struct level *Level=NULL;             /* Pointer to current Level */
struct level *Dungeon=NULL;           /* Pointer to current Dungeon */
#ifndef MSDOS
int Villagenum;                       /* Current Village number */ 
#else
int Villagenum = 0;                   /* Current Village number */ 
#endif
#ifndef MSDOS
int ScreenOffset;                     /* Offset of displayed screen to level */
#else
int ScreenOffset = 0;                 /* Offset of displayed screen to level */
#endif
#ifndef MSDOS
int MaxDungeonLevels;                 /* Deepest level allowed in dungeon */
#else
int MaxDungeonLevels = 0;             /* Deepest level allowed in dungeon */
#endif
int Current_Dungeon= -1;              /* What is Dungeon now */
int Current_Environment= E_CITY;      /* Which environment are we in */
int Last_Environment= E_COUNTRYSIDE;  /* Which environment were we in */
#ifndef MSDOS
int Dirs[2][9];                       /* 9 xy directions */
#else
int Dirs[2][9]=                       /* 9 xy directions */
  {1,1,-1,-1,1,-1,0,0,0,1,-1,1,-1,0,0,1,-1,0};
#endif
char Cmd='s';                         /* last player command */
int Command_Duration = 0;             /* how long does current command take */
struct monster *Arena_Monster=NULL;   /* Opponent in arena */
int Arena_Opponent=0;                 /* case label of opponent in l_arena()*/
#ifndef MSDOS
int Arena_Victory;                    /* did player win in arena? */
#else
int Arena_Victory = 0;                /* did player win in arena? */
#endif
int Imprisonment=0;                   /* amount of time spent in jail */
int Precipitation=0;                  /* Hours of rain, snow, etc */
int Lunarity=0;                       /* Effect of the moon on character */
#ifndef MSDOS
int Phase;                            /* Phase of the moon */
int Date;                             /* Starting date */
int Pawndate;                         /* Pawn Shop item generation date */
#else
int Phase = 0;                        /* Phase of the moon */
int Date = 0;                         /* Starting date */
int Pawndate = 0;                     /* Pawn Shop item generation date */
#endif
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
#ifndef MSDOS
char Seed;                            /* random seed */
#else
char Seed = 0;                            /* random seed */
#endif
#ifndef MSDOS
int Time = 0;                         /* turn number */
#else
long Time = 0;                        /* turn number */
#endif
int Tick = 0;                         /* 10 a turn; action coordinator */
char Stringbuffer[10][80] = {
  "First String",  "First String",  "First String",  "First String",  
  "First String",  "First String",  "First String",  "First String",
  "First String",  "First String"}; /* last 10 strings */
#ifndef MSDOS
int Gymcredit = 0;                    /* credit at rampart gym */
#else
long Gymcredit = 0;                   /* credit at rampart gym */
#endif
int Spellsleft = 0;                   /* research allowance at college */
int StarGemUse = 0;                   /* last date of star gem use */
int HiMagicUse = 0;                   /* last date of high magic use */
int HiMagic = 0;                      /* current level for l_throne */ 
#ifndef MSDOS
int Balance = 0;                      /* bank account */
#else
long Balance = 0;                     /* bank account */
#endif
#ifndef MSDOS
int FixedPoints = 0;                  /* points are frozen after adepthood*/
#else
long FixedPoints = 0;                 /* points are frozen after adepthood*/
#endif
int LastTownLocX=0;            /* previous position in village or city */
int LastTownLocY=0;            /* previous position in village or city */
int LastCountryLocX=0;            /* previous position in countryside */
int LastCountryLocY=0;            /* previous position in countryside */
#ifndef MSDOS
char Password[64];                    /* autoteller password */
#else
char Password[64] = {0};              /* autoteller password */
#endif
#ifndef MSDOS
char Str1[100],Str2[100],Str3[100],Str4[100];
#else
char Str1[100] = {0},Str2[100] = {0},Str3[100] = {0},Str4[100] = {0};
#endif
   /* Some string space, random uses */

pol Condoitems=NULL;                        /* Items in condo */

/* high score names, levels, behavior */
#ifndef MSDOS
int Shadowlordbehavior,Archmagebehavior,Primebehavior,Commandantbehavior;
int Championbehavior,Priestbehavior[7],Hibehavior,Dukebehavior;
int Chaoslordbehavior,Lawlordbehavior,Justiciarbehavior;
char Shadowlord[80],Archmage[80],Prime[80],Commandant[80],Duke[80];
char Champion[80],Priest[7][80],Hiscorer[80],Hidescrip[80];
char Chaoslord[80],Lawlord[80],Justiciar[80];
int Shadowlordlevel,Archmagelevel,Primelevel,Commandantlevel,Dukelevel;
#else
int Shadowlordbehavior = 0,Archmagebehavior = 0,Primebehavior = 0,Commandantbehavior = 0;
int Championbehavior = 0,Priestbehavior[7] = {0},Hibehavior = 0,Dukebehavior = 0;
int Chaoslordbehavior = 0,Lawlordbehavior = 0,Justiciarbehavior = 0;
char Shadowlord[80] = {0},Archmage[80] = {0},Prime[80] = {0},Commandant[80] = {0},Duke[80] = {0};
char Champion[80] = {0},Priest[7][80] = {0},Hiscorer[80] = {0},Hidescrip[80] = {0};
char Chaoslord[80] = {0},Lawlord[80] = {0},Justiciar[80] = {0};
int Shadowlordlevel = 0,Archmagelevel = 0,Primelevel = 0,Commandantlevel = 0,Dukelevel = 0;
#endif
#ifndef MSDOS
int Championlevel,Priestlevel[7],Hiscore,Hilevel,Justiciarlevel;
#else
int Championlevel = 0,Priestlevel[7] = {0},Hilevel = 0,Justiciarlevel = 0;
long Hiscore = 0L;
#endif
#ifndef MSDOS
int Chaoslordlevel,Lawlordlevel,Chaos,Law;
#else
int Chaoslordlevel = 0,Lawlordlevel = 0,Chaos = 0,Law = 0;
#endif

/* New globals which used to be statics */
int twiddle = FALSE;
int saved=FALSE;
int onewithchaos=FALSE;
int club_hinthour = 0;
int winnings = 0;
int tavern_hinthour;
int scroll_ids[30];
int potion_ids[30];
int stick_ids[30];
int ring_ids[30];
int cloak_ids[30];
int boot_ids[30];

int deepest[E_MAX + 1];


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

  printf("%lu\n", coreleft()); getchar(); //

  /* always catch ^c and hang-up signals */

//signal(SIGINT,quit);
#ifndef MSDOS
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
#endif



  /* if filecheck is 0, some necessary data files are missing */
  if (filecheck() == 0) exit(0);

  /* all kinds of initialization */
  initgraf();
#ifndef MSDOS
  initdirs();
#endif
  initrand();
  initspells();

#ifndef MSDOS
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
#endif

#ifdef MSDOS
  msdos_init();
#endif

  /* game restore attempts to restore game if there is an argument */
  continuing = game_restore(argc,argv);


  /* monsters initialized in game_restore if game is being restored */  
  /* items initialized in game_restore if game is being restored */
  if (! continuing) {
	inititem(1);
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

#ifndef MSDOS
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
#endif




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
void inititem(reset)
int reset;
{
  int i;
  if (reset)
    scrollname(TRUE,0);
  for(i=0;i<NUMSCROLLS;i++) {
    Objects[SCROLLID+i].objstr = salloc(scrollname(FALSE,i));
  }
  if (reset)
    potionname(TRUE,0);
  for(i=0;i<NUMPOTIONS;i++) {
    Objects[POTIONID+i].objstr = salloc(potionname(FALSE,i));
  }
  if (reset)
    stickname(TRUE,0);
  for(i=0;i<NUMSTICKS;i++) {
    Objects[STICKID+i].objstr = salloc(stickname(FALSE,i));
  }
  if (reset)
    bootname(TRUE,0);
  for(i=0;i<NUMBOOTS;i++) {
    Objects[BOOTID+i].objstr = salloc(bootname(FALSE,i));
  }
  if (reset)
    cloakname(TRUE,0);
  for(i=0;i<NUMCLOAKS;i++) {
    Objects[CLOAKID+i].objstr = salloc(cloakname(FALSE,i));
  }
  if (reset)
    ringname(TRUE,0);
  for(i=0;i<NUMRINGS;i++) {
    Objects[RINGID+i].objstr = salloc(ringname(FALSE,i));
  }
}