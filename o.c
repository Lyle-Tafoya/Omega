/* omega copyright (c) 1987 by Laurence Raphael Brothers */

/* this file includes main() and some top-level functions */
/* o.c */

#include <stdio.h>
#include <signal.h>
#include <strings.h>
#include "odefs.h"

/* from ofile */
extern int filecheck();

/* from outil */
extern void initdirs();
extern int getdir();

/* from oscr */
extern void initgraf(),drawvision(),endgraf(),mprint(),dataprint();
extern void printm(),morewait(),showflags(),show_screen(),screencheck();
extern char ynq();

/* from ocom */
extern void p_process(),pickup(),save(),quit();

/* from somewhere */
extern int calcmana();

/* from oaux */
extern void initplayer(),initspells(),p_damage(),p_teleport(),hourly_check();
extern void foodcheck(),move_status_check(),hour_status_check();
extern void damage_item();

/* from otime */
extern void time_clock();

/* from oetc */
extern void hint();

/* from oinitmon0to3 */
extern void initmon0to3();

/* from oinitmon4to7 */
extern void initmon4to7();

/* from oinitmon8to10 */
extern void initmon8to10();

/* from olev */
extern void wandercheck();
extern int restore_game();

/* from ogen */
extern void init_dungeon();

/* from oitem */
extern pob create_object();

/* from oinititem1 */
extern void inititem1();

/* from oinititem2 */
extern void inititem2();

              /* globals all originate in o.c */
              /* extern defs in oglob.h */
              /* There do seem to be an awful lot of them.... */

/* one of each monster */
struct monster Monsters[NUMMONSTERS];

/* one of each spell */
struct spell Spells[NUMSPELLS+1];

/* one of each item */
struct object Objects[TOTALITEMS];

pml Mlist[NUMLEVELS];                 /* monsters on each level */
struct player Player;                 /* the player */
struct location Dungeon[NUMLEVELS][WIDTH][LENGTH];  /* the whole dungeon */
struct level_data Leveldata[NUMLEVELS];  /* info about various levels */
int Dirs[2][9];                       /* 9 xy directions */
char Cmd='s';                         /* last player command */
int Lunarity=0;                       /* Effect of the moon on character */
int Phase;                            /* Phase of the moon */
int Date;                             /* Starting date */
int Searchnum = 1;                    /* number of times to search on 's' */
int Wizard = FALSE;                   /* Wizard mode ? */
int Cheated = FALSE;                  /* If ever used wizard mode */
int Fastmove = TRUE;                  /* are we in the middle of a fast move */
int Skipmonsters = FALSE;             /* don't deal with them this move */
int Skipplayer = FALSE;               /* don't deal with him this move */
int Gameover = FALSE;                 /* yow! are we having fun yet? */
int Dlevel = 0;                       /* current dungeon level */
int WhichScreen=0;                    /* which screen are we on? (0..3) */
char Seed;                            /* random seed */
int Time = 0;                         /* turn number */
int Tick = 0;                         /* 1/0 a turn; action coordinator */
char Laststring[80];                  /* the last mprint string */
int Logsize;                          /* number of players in log */
int Tacmode=FALSE;                    /* In tactical combat mode? */
int Arena = FALSE;                    /* Arena combat leaves no corpses */
int Final = FALSE;                    /* Final duel flag */
int Gymcredit = 0;                    /* credit at rampart gym */
int Balance = 0;                      /* bank account */
int Bank_Broken = FALSE;              /* Has bank been broken into */
char Password[64];                    /* autoteller password */
int Deepest = 0;                      /* The deepest level attained */
int SuppressPrinting=FALSE;           /* turn off mprint, printm, when TRUE */
char Str1[100],Str2[100],Str3[100],Str4[100];
   /* Some string space, random uses */

int Thieflevel,Clericlevel,Sorcerorlevel,Collegelevel,Merclevel;
/*record entry levels for various guilds, etc */

int Clubmember=FALSE;                       /* Explorers' club member? */ 
int Sawdruid=FALSE;                         /* met the druid? */
int Soldcondo=FALSE;                        /* Bought the condo? */
pol Condoitems=NULL;                        /* Items in condo */

/* high score names, levels, behavior */
int Shadowlordbehavior,Archmagebehavior,Primebehavior,Commandantbehavior;
int Championbehavior,Priestbehavior[7],Hibehavior,Dukebehavior;
char Shadowlord[80],Archmage[80],Prime[80],Commandant[80],Duke[80];
char Champion[80],Priest[7][80],Hiscorer[80],Hidescrip[80];
int Shadowlordlevel,Archmagelevel,Primelevel,Commandantlevel,Dukelevel;
int Championlevel,Priestlevel[7],Hiscore,Hilevel;


/* o.c functions */
void intercycle(),initrand();
int game_restore(),signalexit(),main();

/* bookkeeping between monster and player moves. Is executed every move */



/* This may be implementation dependent */
void initrand()
{
  srandom(time()+Seed);
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
      mprint("Try again with the right save file, luser!");
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


  if (CATCH_SIGNALS) {
    signal(SIGINT,quit);
    signal(3,signalexit);
    signal(4,signalexit);
    signal(5,signalexit);
    signal(6,signalexit);
    signal(7,signalexit);
    signal(8,signalexit);
    signal(10,signalexit);
    signal(11,signalexit);
    signal(12,signalexit);
    signal(29,signalexit);
  }

  /* if filecheck is 0, some necessary data files are missing */
  if (filecheck() == 0) exit(0);

  /* all kinds of initialization */
  initdirs();
  initrand();
  inititem1();
  inititem2();
  initspells();
  initgraf();

  /* if filecheck is 0, some necessary data files are missing */
  if (filecheck == 0) {
    endgraf();
    exit(0);
  }

  /* game restore attempts to restore game if there is an argument */
  continuing = game_restore(argc,argv);


  /* monsters initialized in game_restore if game is being restored */  
  if (! continuing) {
    initplayer();
    initmon0to3();
    initmon4to7();
    initmon8to10();
    init_dungeon();
    Leveldata[0].generated=TRUE;  
    Dlevel = 0;
    Player.x = 32;
    Player.y = 2;
    Date = random_range(360);
    Phase = random_range(24);
    strcpy(Password,"");
    mprint("You find yourself on Park Avenue, the City of Rampart.");
    mprint("'?' for help or commandlist, 'Q' to quit.");
    show_screen(0);
  }
  else mprint("Your adventure continues....");

  
  timeprint();
  showflags();
  drawvision(Player.x,Player.y,Player.vision);

  /* game cycle */
  while (! Gameover) time_clock();
  
  /* clean up curses */
  endgraf();
  
}

int signalexit()
{
  mprint("Yikes!");
  morewait();
  mprint("Sorry, caught a core-dump signal.");
  if (Dlevel==0) {
    mprint("Want to try and save the game?");
    if (ynq()=='y')
      save();
  }
  mprint("Bye!");
  endgraf();
  exit(0);
}

