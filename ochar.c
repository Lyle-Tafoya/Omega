/* omega copyright (C) by Laurence Raphael Brothers, 1987 */
/* ochar.c */
/* Player generation */

#include <stdio.h>
#include <strings.h>
#include "oglob.h"

/* from oscr */
extern void printm(),mprint(),displaystats(),menuclear(),menuprint(),xredraw();
extern void menuaddch();
extern char mgetc(),*msgscanstring(),menugetc();

/* from oaux */
extern void calc_melee(),optionset();

/* from ocom */
extern void setoptions();

/* ochar functions */
void initplayer(),initstats(),changestat(),save_omegarc();
int calcmana(),fixnpc(),competence_check();
FILE *omegarc_check();

/* ofile functions */
void filescanstring();

/* olev functions */
extern void write_int();
extern int read_int();

/* set player to begin with */
void initplayer()
{
   int i;
   char option;
   int oldchar=FALSE;
   FILE *fd;

   Player.x = -1;
   Player.y = -1;
   Player.str = Player.maxstr = 13;
   Player.con = Player.maxcon = 13;
   Player.dex = Player.maxdex = 13;
   Player.agi = Player.maxagi = 13;
   Player.iq = Player.maxiq = 13;
   Player.pow = Player.maxpow = 13;
   Player.defense = 0;
   Player.absorption = 0;
   Player.vision = 1;
   Player.itemweight = 0;
   Player.armor = NULL;
   Player.primary = NULL;
   Player.secondary = NULL;
   Player.cloak = NULL;
   Player.boots = NULL;
   Player.numrings = 0; 
   Player.food = 19; 
   Player.options = 0;
   for (i=0;i<MAXITEMS;i++) Player.possessions[i] = NULL;
   for (i=0;i<NUMIMMUNITIES;i++) Player.immunity[i] = 0;
   for (i=0;i<NUMSTATI;i++) Player.status[i] = 0;
   for (i=0;i<NUMRANKS;i++) Player.rank[i] = 0;
   Player.alignment = 0;
   if ((fd=omegarc_check())!=NULL) {
     oldchar = TRUE;
     filescanstring(fd,Player.name);
     Player.options=read_int(fd);
     Searchnum=read_int(fd);
     Player.str=read_int(fd);
     Player.con=read_int(fd);
     Player.dex=read_int(fd);
     Player.agi=read_int(fd);
     Player.iq=read_int(fd);
     Player.pow=read_int(fd);
     Player.preference = getc(fd);
     fclose(fd);
     Player.maxstr = Player.str;
     Player.maxcon = Player.con;
     Player.maxdex = Player.dex;
     Player.maxagi = Player.agi;
     Player.maxiq = Player.iq;
     Player.maxpow = Player.pow;
     if (Searchnum > 9) Searchnum = 9;
     else if (Searchnum < 1) Searchnum = 1;
     if (Player.str+Player.con+Player.dex+
	 Player.agi+Player.iq+Player.pow > 78) {
       mprint("Stats too high! You can't fool me, un-huh, un-huh, un-huh...");
       oldchar = FALSE;
     }
   }
   if (! oldchar) {
     initstats();
     optionset(ASKME);
     optionset(RUNSTOP);
     optionset(CONFIRM);
   }
   Player.hp = Player.maxhp = Player.maxcon;
   Player.cash = 250;
   calc_melee();
   Player.mana = Player.maxmana = calcmana();
   Player.click = 1;
   dataprint();
 }


FILE *omegarc_check()
{
  FILE *fd;
  strcpy(Str3,".omegarc");
  if ((fd = fopen(Str3,"r")) != NULL) {
    mprint("Use .omegarc in wd? [yn]");
    if (ynq()!='y') fd = NULL;
  }
  return(fd);
}

void initstats()
{
  int done = FALSE;
  int statpoints = 0;

  mprint("*********Character Generation Module*********");

  menuclear();
  menuprint("\n\nExpend points on statistics\n\n");
  menuprint("s: lower strength     --- S: raise strength\n");
  menuprint("c: lower constitution --- C: raise constitution\n");
  menuprint("d: lower dexterity    --- D: raise dexterity\n");
  menuprint("a: lower agility      --- A: raise agility\n");
  menuprint("i: lower intelligence --- I: raise intelligence\n");
  menuprint("p: lower power        --- P: raise power\n");
  menuprint("\n\nESCAPE when you are satisfied with your statistics");
  displaystats(statpoints);
  while(! done) {
    switch(mgetc()) {
      case ESCAPE: done = TRUE; break;
      case 's': changestat(&statpoints,&Player.maxstr,-1); break;
      case 'S': changestat(&statpoints,&Player.maxstr,1); break;
      case 'c': changestat(&statpoints,&Player.maxcon,-1); break;
      case 'C': changestat(&statpoints,&Player.maxcon,1); break;
      case 'd': changestat(&statpoints,&Player.maxdex,-1); break;
      case 'D': changestat(&statpoints,&Player.maxdex,1); break;
      case 'a': changestat(&statpoints,&Player.maxagi,-1); break;
      case 'A': changestat(&statpoints,&Player.maxagi,1); break;
      case 'i': changestat(&statpoints,&Player.maxiq,-1); break;
      case 'I': changestat(&statpoints,&Player.maxiq,1); break;
      case 'p': changestat(&statpoints,&Player.maxpow,-1); break;
      case 'P': changestat(&statpoints,&Player.maxpow,1); break;
    }
    displaystats(statpoints);
  }
  Player.str = Player.maxstr;
  Player.hp = Player.maxhp = Player.con = Player.maxcon;
  Player.dex = Player.maxdex;
  Player.agi = Player.maxagi;
  Player.iq = Player.maxiq;
  Player.pow = Player.maxpow;
  printm("\nPlease enter your character's name: ");
  strcpy(Player.name,msgscanstring());
  printm("\nWhat is your character's sexual preference? [mf] ");
  do Player.preference = mgetc();
  while ((Player.preference != 'm') && (Player.preference != 'f'));
  printm("\nDo you want to save this set-up to .omegarc in this wd? [yn]");
  if (ynq()=='y')
    save_omegarc();
  xredraw();
}

void save_omegarc()
{
  FILE *fd = fopen(".omegarc","w");
  if (fd == NULL)
    mprint("Sorry, couldn't save .omegarc for some reason.");
  else {
    mprint("First, set options.");
    setoptions();
    fprintf(fd,"%s\n",Player.name);
    write_int(fd,Player.options);
    write_int(fd,Searchnum);
    write_int(fd,Player.str);
    write_int(fd,Player.con);
    write_int(fd,Player.dex);
    write_int(fd,Player.agi);
    write_int(fd,Player.iq);
    write_int(fd,Player.pow);
    putc(Player.preference,fd);
    fclose(fd);
  }
}


void changestat(statpoints,stat,delta)
int *statpoints,*stat,delta;
{
  if ((*statpoints > 0) || (delta == -1)) 
    if ((*stat+delta < 19) && (*stat+delta > 2)) {
      *statpoints -= delta;
      *stat += delta;
    }
}


int calcmana()
{
  return(Player.pow * (Player.level+1));
}


/*  npcbehavior digits 1234

4 : alignment (LAWFUL,CHAOTIC, or NEUTRAL)
3 : primary combat action (melee,missile,spell,thief,flight,1..5)
2 : competence at 4 (0..9, 0 = incompetent, 9 = masterful)
1 : conversation mode

status : 1 = dead, 2 = saved, 3 = retired
*/
int fixnpc(status)
int status;
{
  int npcbehavior=0;
  char response;
  if (status == 1) { /* player is dead, all undead are chaotic */
    npcbehavior+=CHAOTIC;
    npcbehavior+=10; /* melee */
    npcbehavior+=100*min(9,((int) (Player.level/3)));
    npcbehavior+=3000; /* evil */
  }
  else {
    printm("NPC Behavior Determination Module");
    menuclear();
    menuprint("Your overall NPC behavior is:");
    if (Player.alignment < -10) {
      npcbehavior += CHAOTIC;
      menuprint("\n\n CHAOTIC");
    }
    else if (Player.alignment > 10) {
      npcbehavior += LAWFUL;
      menuprint("\n\n LAWFUL");
    }
    else {
      npcbehavior += NEUTRAL;
      menuprint("\n\n NEUTRAL");
    }
    menuprint("\n\n1: hand-to-hand combat");
    menuprint("\n2: missile combat");
    menuprint("\n3: spellcasting");
    menuprint("\n4: thieving");
    menuprint("\n5: escape");
    menuprint("\n\nEnter NPC response to combat: ");
    response = '0';
    while ((response != '1') && 
	   (response != '2') &&
	   (response != '3') &&
	   (response != '4') &&
	   (response != '5'))
      response = menugetc();
    menuaddch(response);
    npcbehavior+=10*(response - '0');
    npcbehavior+=100*competence_check(response-'0');
    response = '0';
    menuprint("\n\n1: threaten");
    menuprint("\n2: greet");
    menuprint("\n3: aid");
    menuprint("\n4: beg");
    menuprint("\n5: silence");
    menuprint("\n\nEnter NPC response to conversation: ");
    while ((response != '1') && 
	   (response != '2') &&
	   (response != '3') &&
	   (response != '4') &&
	   (response != '5'))
      response = menugetc();
    menuaddch(response);
    npcbehavior+=1000*(response - '0');
    xredraw();
  }
    return(npcbehavior);
}


/* estimates on a 0..9 scale how good a player is at something */
int competence_check(attack)
int attack;  
{
  int ability = 0;
  switch(attack) {
  case 1: /* melee */
    ability += statmod(Player.str);
  case 2: /* missle */
    ability += statmod(Player.dex);
    ability += Player.rank[LEGION];
    ability += ((int) (Player.dmg / 10) - 1);
    break;
  case 3: /* spellcasting */
    ability += statmod(Player.iq);
    ability += statmod(Player.pow);
    ability += Player.rank[CIRCLE];
    ability += Player.rank[COLLEGE];
    ability += Player.rank[PRIEST];
    break;
  case 4: /* thieving */
    ability += statmod(Player.dex);
    ability += statmod(Player.agi);
    ability += Player.rank[THIEVES];
    break;
  case 5: /* escape */
    ability += 2 * statmod(Player.agi);
    break;
  }
  ability += ((int) (Player.level / 5));
  if (ability < 0) ability = 0;
  if (ability > 9) ability = 9;
  return(ability);
}
