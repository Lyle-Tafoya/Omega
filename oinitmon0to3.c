
/* oinitmon0to3.c */


#include <stdio.h>
#include <strings.h>
#include "oglob.h"


/* from oitem */
extern void make_weapon();

/* from omon */
extern void m_pickup();

void initmon0to3();

void initmon0to3()
{
  int i;
  char filestring[80];
  FILE *fd;
  pob tob;

  for(i=0;i<NUMLEVELS;i++)
    Mlist[i] = NULL;
  for(i=0;i<NUMMONSTERS;i++) {
    Monsters[i].attacked = FALSE;
    Monsters[i].click = 0;
    Monsters[i].hitloc = 0;
    Monsters[i].uniqueness = 0;
  }

  /* Find out how many players have played, for npc generator */
  strcpy(filestring,OMEGALIB);
  strcat(filestring,"omega.lognum");
  fd = fopen(filestring,"r");
  fscanf(fd,"%d",&Logsize);
  fclose(fd);




  /* the hornet */
  Monsters[ML0+0].id = 0; /* monster id slot == index in Monsters */
  Monsters[ML0+0].hp = 1;
  Monsters[ML0+0].speed = 1;
  Monsters[ML0+0].actions = 3;
  Monsters[ML0+0].tactics = MM_POOR+MM_BERSERK;
  Monsters[ML0+0].hit = 1;
  Monsters[ML0+0].ac = 10;
  Monsters[ML0+0].dmg = 2;
  Monsters[ML0+0].sense = 4;
  Monsters[ML0+0].wakeup = 1;
  Monsters[ML0+0].level = 0;
  Monsters[ML0+0].status = HOSTILE+MOBILE+FLYING;
  Monsters[ML0+0].immunity = 0;
  Monsters[ML0+0].sleep = 50;
  Monsters[ML0+0].treasure = 0;
  Monsters[ML0+0].possessions = NULL;
  Monsters[ML0+0].xpv = 1;
  Monsters[ML0+0].x = -1;
  Monsters[ML0+0].y = -1;
  Monsters[ML0+0].transformid = -1;
  Monsters[ML0+0].corpsevalue = 1;
  Monsters[ML0+0].monchar = 'h';
  strcpy(Monsters[ML0+0].monstring,"hornet");
  strcpy(Monsters[ML0+0].corpsestr,"dead hornet");
  Monsters[ML0+0].corpseweight = 1;
  Monsters[ML0+0].talkf = M_TALK_STUPID;
  Monsters[ML0+0].movef = M_MOVE_FLUTTER;
  Monsters[ML0+0].meleef = M_MELEE_NORMAL;
  Monsters[ML0+0].strikef = M_NO_OP;
  Monsters[ML0+0].specialf = M_NO_OP;




  /* the mendicant priest */
  Monsters[ML0+1].id = ML0+1;
  Monsters[ML0+1].hp = 30;
  Monsters[ML0+1].speed = 5;
  Monsters[ML0+1].hit = 0;
  Monsters[ML0+1].actions = 2;
  Monsters[ML0+1].tactics = MM_POOR+MM_TIMID;
  Monsters[ML0+1].ac = 0;
  Monsters[ML0+1].dmg = 0;
  Monsters[ML0+1].sense = 10;
  Monsters[ML0+1].wakeup = 10;
  Monsters[ML0+1].level = 0;
  Monsters[ML0+1].status = NEEDY+MOBILE;
  Monsters[ML0+1].immunity = 0;
  Monsters[ML0+1].xpv = 15;
  Monsters[ML0+1].treasure = 2;
  Monsters[ML0+1].possessions = NULL;
  Monsters[ML0+1].sleep = 100;
  Monsters[ML0+1].x = -1;
  Monsters[ML0+1].y = -1;
  Monsters[ML0+1].transformid = -1;
  Monsters[ML0+1].corpsevalue = 0;
  if (random_range(2)) {
    Monsters[ML0+1].monchar = 'm';
    strcpy(Monsters[ML0+1].monstring,"man");
  }
  else {
    Monsters[ML0+1].monchar = 'w';
    strcpy(Monsters[ML0+1].monstring,"woman");
  }
  strcpy(Monsters[ML0+1].corpsestr,"dead mendicant priest");
  Monsters[ML0+1].corpseweight = 1000;
  Monsters[ML0+1].talkf = M_TALK_MP;
  Monsters[ML0+1].movef = M_MOVE_FOLLOW;
  Monsters[ML0+1].meleef = M_MELEE_MP;
  Monsters[ML0+1].strikef = M_NO_OP;
  Monsters[ML0+1].specialf = M_SP_MP;

  /* Itinerant Merchant */
  Monsters[ML0+2].id = ML0+2;
  Monsters[ML0+2].hp = 15;
  Monsters[ML0+2].speed = 7;
  Monsters[ML0+2].hit = 0;
  Monsters[ML0+2].actions = 1;
  Monsters[ML0+2].tactics = MM_POOR+MM_TIMID;
  Monsters[ML0+2].ac = 0;
  Monsters[ML0+2].dmg = 0;
  Monsters[ML0+2].sense = 10;
  Monsters[ML0+2].wakeup = 10;
  Monsters[ML0+2].level = 0;
  Monsters[ML0+2].status = 0+MOBILE;
  Monsters[ML0+2].immunity = 0;
  Monsters[ML0+2].xpv = 5;
  Monsters[ML0+2].treasure = 3;
  Monsters[ML0+2].possessions = NULL;
  Monsters[ML0+2].sleep = 0;
  Monsters[ML0+2].x = -1;
  Monsters[ML0+2].y = -1;
  Monsters[ML0+2].transformid = -1;
  Monsters[ML0+2].corpsevalue = 0;
  if (random_range(2)) {
    Monsters[ML0+2].monchar = 'm';
    strcpy(Monsters[ML0+2].monstring,"man");
  }
  else {
    Monsters[ML0+2].monchar = 'w';
    strcpy(Monsters[ML0+2].monstring,"woman");
  }
  strcpy(Monsters[ML0+2].corpsestr,"dead itinerant merchant");
  Monsters[ML0+2].corpseweight = 1000;
  Monsters[ML0+2].talkf = M_TALK_IM;
  Monsters[ML0+2].movef = M_MOVE_FOLLOW;
  Monsters[ML0+2].meleef = M_NO_OP;
  Monsters[ML0+2].strikef = M_NO_OP;
  Monsters[ML0+2].specialf = M_SP_ESCAPE;


  /* Guardsman */
  Monsters[ML0+3].id = ML0+3;
  Monsters[ML0+3].hp = 150;
  Monsters[ML0+3].speed = 3;
  Monsters[ML0+3].hit = 20;
  Monsters[ML0+3].actions = 6;
  Monsters[ML0+3].tactics = MM_GOOD;
  Monsters[ML0+3].ac = 20;
  Monsters[ML0+3].dmg = 25;
  Monsters[ML0+3].sense = 10;
  Monsters[ML0+3].wakeup = 1;
  Monsters[ML0+3].level = 0;
  Monsters[ML0+3].status = 0+MOBILE;
  Monsters[ML0+3].immunity = 0;
  Monsters[ML0+3].xpv = 150;
  Monsters[ML0+3].treasure = 0;
  Monsters[ML0+3].possessions = NULL;
  Monsters[ML0+3].sleep = 50;
  Monsters[ML0+3].x = -1;
  Monsters[ML0+3].y = -1;
  Monsters[ML0+3].transformid = -1;
  Monsters[ML0+3].corpsevalue = 0;
  Monsters[ML0+3].monchar = 'G';
  strcpy(Monsters[ML0+3].monstring,"guardsman");
  strcpy(Monsters[ML0+3].corpsestr,"dead guardsman");
  Monsters[ML0+3].corpseweight = 1000;
  Monsters[ML0+3].talkf = M_TALK_GUARD;
  Monsters[ML0+3].movef = M_MOVE_SMART;
  Monsters[ML0+3].meleef = M_MELEE_NORMAL;
  Monsters[ML0+3].strikef = M_NO_OP;
  Monsters[ML0+3].specialf = M_NO_OP;


  /* Random NPC from log */
  Monsters[ML0+4].id = ML0+4;
  Monsters[ML0+4].hp = 5; /* variable */
  Monsters[ML0+4].speed = 5;
  Monsters[ML0+4].hit = 5; /* variable */
  Monsters[ML0+4].actions = 5;/* variable */
  Monsters[ML0+4].tactics = MM_AVERAGE; /* variable */
  Monsters[ML0+4].ac = 5; /* variable */
  Monsters[ML0+4].dmg = 5; /* variable */
  Monsters[ML0+4].sense = 5; /* variable */
  Monsters[ML0+4].wakeup = 5; /* variable */
  Monsters[ML0+4].level = 0;/* variable */
  Monsters[ML0+4].status = 5; /* variable */
  Monsters[ML0+4].immunity = 5; /* variable */
  Monsters[ML0+4].xpv = 5; /* variable */
  Monsters[ML0+4].treasure = 0; /* variable */
  Monsters[ML0+4].possessions = NULL; /* variable */
  Monsters[ML0+4].sleep = 5; /* variable */
  Monsters[ML0+4].x = 5;
  Monsters[ML0+4].y = 5;
  Monsters[ML0+4].transformid = -1;
  Monsters[ML0+4].corpsevalue = 0;
  Monsters[ML0+4].monchar = 'm';
  strcpy(Monsters[ML0+4].monstring,"an NPC");
  strcpy(Monsters[ML0+4].corpsestr,"a human corpse");
  Monsters[ML0+4].corpseweight = 1000;
  Monsters[ML0+4].talkf = M_TALK_MAN;
  Monsters[ML0+4].movef = M_MOVE_SMART;
  Monsters[ML0+4].meleef = M_MELEE_NORMAL;
  Monsters[ML0+4].strikef = M_NO_OP;
  Monsters[ML0+4].specialf = M_NO_OP;


  /* the grunt */
  Monsters[ML1+0].id = ML1+0;
  Monsters[ML1+0].hp = 25;
  Monsters[ML1+0].speed = 8;
  Monsters[ML1+0].hit = 8;
  Monsters[ML1+0].actions = 2;
  Monsters[ML1+0].tactics = MM_FORCED+MM_BERSERK;
  Monsters[ML1+0].hitloc = CENTER;
  Monsters[ML1+0].ac = 5;
  Monsters[ML1+0].dmg = 15;
  Monsters[ML1+0].sense = 3;
  Monsters[ML1+0].wakeup = 3;
  Monsters[ML1+0].level = 1;
  Monsters[ML1+0].status = HOSTILE+HUNGRY+MOBILE;
  Monsters[ML1+0].immunity = pow2(FLAME)+pow2(ELECTRICITY)+pow2(COLD);
  Monsters[ML1+0].xpv = 10;
  Monsters[ML1+0].treasure = 0;
  Monsters[ML1+0].possessions = NULL;
  Monsters[ML1+0].sleep = 100;
  Monsters[ML1+0].x = -1;
  Monsters[ML1+0].y = -1;
  Monsters[ML1+0].transformid = -1;
  Monsters[ML1+0].corpsevalue = 0;
  Monsters[ML1+0].monchar = 'g';
  strcpy(Monsters[ML1+0].monstring,"grunt");
  strcpy(Monsters[ML1+0].corpsestr,"grunt's hide");
  Monsters[ML1+0].corpseweight = 100;
  Monsters[ML1+0].talkf = M_TALK_STUPID;
  Monsters[ML1+0].movef = M_MOVE_NORMAL;
  Monsters[ML1+0].meleef = M_MELEE_NORMAL;
  Monsters[ML1+0].strikef = M_NO_OP;
  Monsters[ML1+0].specialf = M_NO_OP;


  /* the tse-tse fly */
  Monsters[ML1+1].id = ML1+1;
  Monsters[ML1+1].hp = 1;
  Monsters[ML1+1].speed = 4;
  Monsters[ML1+1].hit = 10;
  Monsters[ML1+1].actions = 1;
  Monsters[ML1+1].tactics = MM_BERSERK+MM_POOR;
  Monsters[ML1+1].ac = 5;
  Monsters[ML1+1].dmg = 2;
  Monsters[ML1+1].sense = 10;
  Monsters[ML1+1].wakeup = 10;
  Monsters[ML1+1].level = 1;
  Monsters[ML1+1].status = HOSTILE+MOBILE+FLYING;
  Monsters[ML1+1].immunity = 0;
  Monsters[ML1+1].xpv = 3;
  Monsters[ML1+1].treasure = 0;
  Monsters[ML1+1].possessions = NULL;
  Monsters[ML1+1].sleep = 0;
  Monsters[ML1+1].x = -1;
  Monsters[ML1+1].y = -1;
  Monsters[ML1+1].transformid = POTIONID+5;
  Monsters[ML1+1].corpsevalue = 5;
  Monsters[ML1+1].monchar = 't';
  strcpy(Monsters[ML1+1].monstring,"tse-tse fly");
  strcpy(Monsters[ML1+1].corpsestr,"dead tse-tse fly");
  Monsters[ML1+1].corpseweight = 1;
  Monsters[ML1+1].talkf = M_TALK_STUPID;
  Monsters[ML1+1].movef = M_MOVE_FLUTTER;
  Monsters[ML1+1].meleef = M_MELEE_SLEEP;
  Monsters[ML1+1].strikef = M_NO_OP;
  Monsters[ML1+1].specialf = M_NO_OP;



  /* fnord */
  Monsters[ML1+2].id = ML1+2;
  Monsters[ML1+2].hp = 1;
  Monsters[ML1+2].speed = 10;
  Monsters[ML1+2].hit = 0;
  Monsters[ML1+2].actions = 0;
  Monsters[ML1+2].tactics = MM_POOR;
  Monsters[ML1+2].ac = 0;
  Monsters[ML1+2].dmg = 0;
  Monsters[ML1+2].sense = 10;
  Monsters[ML1+2].wakeup = 10;
  Monsters[ML1+2].level = 1;
  Monsters[ML1+2].status = COWARDLY;
  Monsters[ML1+2].immunity = 0;
  Monsters[ML1+2].xpv = 5;
  Monsters[ML1+2].treasure = 1;
  Monsters[ML1+2].possessions = NULL;
  Monsters[ML1+2].sleep = 0;
  Monsters[ML1+2].x = -1;
  Monsters[ML1+2].y = -1;
  Monsters[ML1+2].transformid = SCROLLID+21;
  Monsters[ML1+2].corpsevalue = 50;
  Monsters[ML1+2].monchar = 'f';
  strcpy(Monsters[ML1+2].monstring,"fnord");
  strcpy(Monsters[ML1+2].corpsestr,"fnord's antennae");
  Monsters[ML1+2].corpseweight = 2;
  Monsters[ML1+2].talkf = M_TALK_STUPID;
  Monsters[ML1+2].movef = M_MOVE_FLUTTER;
  Monsters[ML1+2].meleef = M_NO_OP;
  Monsters[ML1+2].strikef = M_NO_OP;
  Monsters[ML1+2].specialf = M_SP_SUMMON;


  /* sewer rat */
  Monsters[ML1+3].id = ML1+3;
  Monsters[ML1+3].hp = 5;
  Monsters[ML1+3].speed = 3;
  Monsters[ML1+3].hit = 5;
  Monsters[ML1+3].actions = 2;
  Monsters[ML1+3].tactics = MM_BERSERK+MM_FORCED;
  Monsters[ML1+3].hitloc = LOW;
  Monsters[ML1+3].ac = 3;
  Monsters[ML1+3].dmg = 5;
  Monsters[ML1+3].sense = 5;
  Monsters[ML1+3].wakeup = 2;
  Monsters[ML1+3].level = 1;
  Monsters[ML1+3].status = HOSTILE+HUNGRY+MOBILE+SWIMMING;
  Monsters[ML1+3].immunity = 0;
  Monsters[ML1+3].xpv = 5;
  Monsters[ML1+3].treasure = 0;
  Monsters[ML1+3].possessions = NULL;
  Monsters[ML1+3].sleep = 30;
  Monsters[ML1+3].x = -1;
  Monsters[ML1+3].y = -1;
  Monsters[ML1+3].transformid = -1;
  Monsters[ML1+3].corpsevalue = 0;
  Monsters[ML1+3].monchar = 'r';
  strcpy(Monsters[ML1+3].monstring,"sewer rat");
  strcpy(Monsters[ML1+3].corpsestr,"dead sewer rat");
  Monsters[ML1+3].corpseweight = 10;
  Monsters[ML1+3].talkf = M_TALK_STUPID;
  Monsters[ML1+3].movef = M_MOVE_FLUTTER;
  Monsters[ML1+3].meleef = M_MELEE_DISEASE;
  Monsters[ML1+3].strikef = M_NO_OP;
  Monsters[ML1+3].specialf = M_NO_OP;


  /* aggravator fungus */
  Monsters[ML1+4].id = ML1+4;
  Monsters[ML1+4].hp = 12;
  Monsters[ML1+4].speed = 10;
  Monsters[ML1+4].hit = 0;
  Monsters[ML1+4].actions = 0;
  Monsters[ML1+4].tactics = MM_POOR;
  Monsters[ML1+4].ac = 0;
  Monsters[ML1+4].dmg = 0;
  Monsters[ML1+4].sense = 10;
  Monsters[ML1+4].wakeup = 10;
  Monsters[ML1+4].level = 1;
  Monsters[ML1+4].status = 0;
  Monsters[ML1+4].immunity = pow2(FLAME);
  Monsters[ML1+4].xpv = 1;
  Monsters[ML1+4].treasure = 0;
  Monsters[ML1+4].possessions = NULL;
  Monsters[ML1+4].sleep = 0;
  Monsters[ML1+4].x = -1;
  Monsters[ML1+4].y = -1;
  Monsters[ML1+4].transformid = -1;
  Monsters[ML1+4].corpsevalue = 0;
  Monsters[ML1+4].monchar = 'f';
  strcpy(Monsters[ML1+4].monstring,"grey fungus");
  strcpy(Monsters[ML1+4].corpsestr,"grey fungus spores");
  Monsters[ML1+4].corpseweight = 1;
  Monsters[ML1+4].talkf = M_TALK_STUPID;
  Monsters[ML1+4].movef = M_NO_OP;
  Monsters[ML1+4].meleef = M_NO_OP;
  Monsters[ML1+4].strikef = M_NO_OP;
  Monsters[ML1+4].specialf = M_SP_AGGRAVATE;


  /* blipper */
  Monsters[ML1+5].id = ML1+5;
  Monsters[ML1+5].hp = 50;
  Monsters[ML1+5].speed = 24;
  Monsters[ML1+5].hit = 0;
  Monsters[ML1+5].actions = 0;
  Monsters[ML1+5].tactics = MM_POOR;
  Monsters[ML1+5].ac = 5;
  Monsters[ML1+5].dmg = 0;
  Monsters[ML1+5].sense = 3;
  Monsters[ML1+5].wakeup = 3;
  Monsters[ML1+5].level = 1;
  Monsters[ML1+5].status = MOBILE;
  Monsters[ML1+5].immunity = 0;
  Monsters[ML1+5].xpv = 20;
  Monsters[ML1+5].treasure = 2;
  Monsters[ML1+5].possessions = NULL;
  Monsters[ML1+5].sleep = 100;
  Monsters[ML1+5].x = -1;
  Monsters[ML1+5].y = -1;
  Monsters[ML1+5].transformid = SCROLLID+5;
  Monsters[ML1+5].corpsevalue = 25;
  Monsters[ML1+5].monchar = 'b';
  strcpy(Monsters[ML1+5].monstring,"blipper");
  strcpy(Monsters[ML1+5].corpsestr,"blipper organ");
  Monsters[ML1+5].corpseweight = 20;
  Monsters[ML1+5].talkf = M_TALK_STUPID;
  Monsters[ML1+5].movef = M_NO_OP;
  Monsters[ML1+5].meleef = M_NO_OP;
  Monsters[ML1+5].strikef = M_NO_OP;
  Monsters[ML1+5].specialf = M_MOVE_TELEPORT;


  /* goblin */
  Monsters[ML1+6].id = ML1+6;
  Monsters[ML1+6].hp = 8;
  Monsters[ML1+6].speed = 5;
  Monsters[ML1+6].hit = 6;
  Monsters[ML1+6].actions = 2;
  Monsters[ML1+6].tactics = MM_AVERAGE;
  Monsters[ML1+6].ac = 8;
  Monsters[ML1+6].dmg = 8;
  Monsters[ML1+6].sense = 10;
  Monsters[ML1+6].wakeup = 2;
  Monsters[ML1+6].level = 1;
  Monsters[ML1+6].status = HOSTILE+GREEDY+MOBILE;
  Monsters[ML1+6].immunity = 0;
  Monsters[ML1+6].xpv = 10;
  Monsters[ML1+6].treasure = 1;
  Monsters[ML1+6].possessions = NULL;
  Monsters[ML1+6].sleep = 0;
  Monsters[ML1+6].x = -1;
  Monsters[ML1+6].y = -1;
  Monsters[ML1+6].transformid = -1;
  Monsters[ML1+6].corpsevalue = 0;
  Monsters[ML1+6].monchar = 'g';
  strcpy(Monsters[ML1+6].monstring,"goblin");
  strcpy(Monsters[ML1+6].corpsestr,"dead goblin");
  Monsters[ML1+6].corpseweight = 500;
  Monsters[ML1+6].talkf = M_TALK_GREEDY;
  Monsters[ML1+6].movef = M_MOVE_SMART;
  Monsters[ML1+6].meleef = M_MELEE_NORMAL;
  Monsters[ML1+6].strikef = M_NO_OP;
  Monsters[ML1+6].specialf = M_NO_OP;


  /* phantasticon */
  Monsters[ML1+7].id = ML1+7;
  Monsters[ML1+7].hp = 10;
  Monsters[ML1+7].speed = 9;
  Monsters[ML1+7].hit = 10;
  Monsters[ML1+7].actions = 2;
  Monsters[ML1+7].tactics = MM_AVERAGE;
  Monsters[ML1+7].ac = 10;
  Monsters[ML1+7].dmg = 10;
  Monsters[ML1+7].sense = 4;
  Monsters[ML1+7].wakeup = 4;
  Monsters[ML1+7].level = 1;
  Monsters[ML1+7].status = HOSTILE+MOBILE;
  Monsters[ML1+7].immunity = pow2(SLEEP);
  Monsters[ML1+7].xpv = 25;
  Monsters[ML1+7].treasure = 1;
  Monsters[ML1+7].possessions = NULL;
  Monsters[ML1+7].sleep = 50;
  Monsters[ML1+7].x = -1;
  Monsters[ML1+7].y = -1;
  Monsters[ML1+7].transformid = SCROLLID+19;
  Monsters[ML1+7].corpsevalue = 10;
  Monsters[ML1+7].monchar = 'p';
  strcpy(Monsters[ML1+7].monstring,"phantasticon");
  strcpy(Monsters[ML1+7].corpsestr,"phantasticon's eyes");
  Monsters[ML1+7].corpseweight = 5;
  Monsters[ML1+7].talkf = M_TALK_SILENT;
  Monsters[ML1+7].movef = M_MOVE_SMART;
  Monsters[ML1+7].meleef = M_MELEE_NORMAL;
  Monsters[ML1+7].strikef = M_NO_OP;
  Monsters[ML1+7].specialf = M_SP_ILLUSION;


  /* household robot */
  Monsters[ML1+8].id = ML1+8;
  Monsters[ML1+8].hp = 5;
  Monsters[ML1+8].speed = 12;
  Monsters[ML1+8].hit = 10;
  Monsters[ML1+8].actions = 4;
  Monsters[ML1+8].tactics = MM_POOR+MM_FORCED;
  Monsters[ML1+8].hitloc = CENTER;
  Monsters[ML1+8].ac = 10;
  Monsters[ML1+8].dmg = 15;
  Monsters[ML1+8].sense = 10;
  Monsters[ML1+8].wakeup = 3;
  Monsters[ML1+8].level = 1;
  Monsters[ML1+8].status = 0+MOBILE;
  Monsters[ML1+8].immunity = pow2(SLEEP)+pow2(POISON)+pow2(FEAR)+
                             pow2(ELECTRICITY);
  Monsters[ML1+8].xpv = 15;
  Monsters[ML1+8].treasure = 0;
  Monsters[ML1+8].possessions = NULL;
  Monsters[ML1+8].sleep = 100;
  Monsters[ML1+8].x = -1;
  Monsters[ML1+8].y = -1;
  Monsters[ML1+8].transformid = -1;
  Monsters[ML1+8].corpsevalue = 0;
  Monsters[ML1+8].monchar = 'R';
  strcpy(Monsters[ML1+8].monstring,"household robot");
  strcpy(Monsters[ML1+8].corpsestr,"household robot battery");
  Monsters[ML1+8].corpseweight = 25;
  Monsters[ML1+8].talkf = M_TALK_ROBOT;
  Monsters[ML1+8].movef = M_MOVE_SMART;
  Monsters[ML1+8].meleef = M_MELEE_NORMAL;
  Monsters[ML1+8].strikef = M_NO_OP;
  Monsters[ML1+8].specialf = M_NO_OP;


  /* pencil-necked geek */
  Monsters[ML1+9].id = ML1+9;
  Monsters[ML1+9].hp = 1;
  Monsters[ML1+9].speed = 8;
  Monsters[ML1+9].hit = 5;
  Monsters[ML1+9].actions = 1;
  Monsters[ML1+9].tactics = MM_POOR;
  Monsters[ML1+9].ac = 5;
  Monsters[ML1+9].dmg = 5;
  Monsters[ML1+9].sense = 10;
  Monsters[ML1+9].wakeup = 10;
  Monsters[ML1+9].level = 1;
  Monsters[ML1+9].status = HOSTILE+MOBILE+COWARDLY;
  Monsters[ML1+9].immunity = 0;
  Monsters[ML1+9].xpv = 1;
  Monsters[ML1+9].treasure = 0;
  Monsters[ML1+9].possessions = NULL;
  Monsters[ML1+9].sleep = 50;
  Monsters[ML1+9].x = -1;
  Monsters[ML1+9].y = -1;
  Monsters[ML1+9].transformid = -1;
  Monsters[ML1+9].corpsevalue = 0;
  Monsters[ML1+9].monchar = 'g';
  strcpy(Monsters[ML1+9].monstring,"pencil-necked geek");
  strcpy(Monsters[ML1+9].corpsestr,"dead pencil-necked geek");
  Monsters[ML1+9].corpseweight = 250;
  Monsters[ML1+9].talkf = M_TALK_STUPID;
  Monsters[ML1+9].movef = M_MOVE_FLUTTER;
  Monsters[ML1+9].meleef = M_MELEE_NORMAL;
  Monsters[ML1+9].strikef = M_NO_OP;
  Monsters[ML1+9].specialf = M_NO_OP;

  /* borogrove */
  Monsters[ML1+10].id = ML1+10;
  Monsters[ML1+10].hp = 6;
  Monsters[ML1+10].speed = 4;
  Monsters[ML1+10].hit = 8;
  Monsters[ML1+10].actions = 1;
  Monsters[ML1+10].tactics = MM_AVERAGE+MM_FORCED;
  Monsters[ML1+10].hitloc = LOW;
  Monsters[ML1+10].ac = 5;
  Monsters[ML1+10].dmg = 6;
  Monsters[ML1+10].sense = 4;
  Monsters[ML1+10].wakeup = 3;
  Monsters[ML1+10].level = 1;
  Monsters[ML1+10].status = 0+MOBILE;
  Monsters[ML1+10].immunity = 0;
  Monsters[ML1+10].xpv = 6;
  Monsters[ML1+10].treasure = 0;
  Monsters[ML1+10].possessions = NULL;
  Monsters[ML1+10].sleep = 50;
  Monsters[ML1+10].x = -1;
  Monsters[ML1+10].y = -1;
  Monsters[ML1+10].transformid = -1;
  Monsters[ML1+10].corpsevalue = 0;
  Monsters[ML1+10].monchar = 'b';
  strcpy(Monsters[ML1+10].monstring,"borogrove");
  strcpy(Monsters[ML1+10].corpsestr,"borogrove wings");
  Monsters[ML1+10].corpseweight = 30;
  Monsters[ML1+10].talkf = M_TALK_MIMSY;
  Monsters[ML1+10].movef = M_MOVE_FLUTTER;
  Monsters[ML1+10].meleef = M_MELEE_NORMAL;
  Monsters[ML1+10].strikef = M_NO_OP;
  Monsters[ML1+10].specialf = M_NO_OP;



  /* the ninja */
  Monsters[ML2+0].id = ML2+0;
  Monsters[ML2+0].hp = 20;
  Monsters[ML2+0].speed = 3;
  Monsters[ML2+0].hit = 12;
  Monsters[ML2+0].actions = 4;
  Monsters[ML2+0].tactics = MM_GOOD;
  Monsters[ML2+0].ac = 10;
  Monsters[ML2+0].dmg = 15;
  Monsters[ML2+0].sense = 10;
  Monsters[ML2+0].wakeup = 3;
  Monsters[ML2+0].level = 2;
  Monsters[ML2+0].status = HOSTILE+MOBILE+M_INVISIBLE;
  Monsters[ML2+0].immunity = 0;
  Monsters[ML2+0].xpv = 30;
  Monsters[ML2+0].treasure = 1;
  Monsters[ML2+0].possessions = NULL;
  Monsters[ML2+0].sleep = 50;
  Monsters[ML2+0].x = -1;
  Monsters[ML2+0].y = -1;
  Monsters[ML2+0].transformid = -1;
  Monsters[ML2+0].corpsevalue = 0;
  if (random_range(2)) {
    Monsters[ML2+0].monchar = 'm';
    strcpy(Monsters[ML2+0].monstring,"man");
  }
  else {
    Monsters[ML2+0].monchar = 'w';
    strcpy(Monsters[ML2+0].monstring,"woman");
  }
  strcpy(Monsters[ML2+0].corpsestr,"dead ninja");
  Monsters[ML2+0].corpseweight = 1000;
  Monsters[ML2+0].talkf = M_TALK_NINJA;
  Monsters[ML2+0].movef = M_MOVE_SMART;
  Monsters[ML2+0].meleef = M_MELEE_NORMAL;
  Monsters[ML2+0].strikef = M_NO_OP;
  Monsters[ML2+0].specialf = M_SP_SURPRISE;

  /* the night gaunt */
  Monsters[ML2+1].id = ML2+1;
  Monsters[ML2+1].hp = 30;
  Monsters[ML2+1].speed = 6;
  Monsters[ML2+1].hit = 12;
  Monsters[ML2+1].actions = 2;
  Monsters[ML2+1].tactics = MM_FORCED+MM_POOR;
  Monsters[ML2+1].hitloc = HIGH;
  Monsters[ML2+1].ac = 10;
  Monsters[ML2+1].dmg = 5;
  Monsters[ML2+1].sense = 10;
  Monsters[ML2+1].wakeup = 10;
  Monsters[ML2+1].level = 2;
  Monsters[ML2+1].status = HOSTILE+MOBILE+FLYING;
  Monsters[ML2+1].immunity = 0;
  Monsters[ML2+1].xpv = 25;
  Monsters[ML2+1].treasure = 0;
  Monsters[ML2+1].possessions = NULL;
  Monsters[ML2+1].sleep = 0;
  Monsters[ML2+1].x = -1;
  Monsters[ML2+1].y = -1;
  Monsters[ML2+1].transformid = -1;
  Monsters[ML2+1].corpsevalue = 100;
  Monsters[ML2+1].monchar = 'n';
  strcpy(Monsters[ML2+1].monstring,"night gaunt");
  strcpy(Monsters[ML2+1].corpsestr,"night gaunt's hide");
  Monsters[ML2+1].corpseweight = 50;
  Monsters[ML2+1].talkf = M_TALK_TITTER;
  Monsters[ML2+1].movef = M_MOVE_FLUTTER;
  Monsters[ML2+1].meleef = M_MELEE_NG;
  Monsters[ML2+1].strikef = M_NO_OP;
  Monsters[ML2+1].specialf = M_NO_OP;

  /* thief */
  Monsters[ML2+2].id = ML2+2;
  Monsters[ML2+2].hp = 32;
  Monsters[ML2+2].speed = 2;
  Monsters[ML2+2].hit = 0;
  Monsters[ML2+2].actions = 2;
  Monsters[ML2+2].tactics = MM_TIMID+MM_AVERAGE;
  Monsters[ML2+2].ac = 10;
  Monsters[ML2+2].dmg = 0;
  Monsters[ML2+2].sense = 10;
  Monsters[ML2+2].wakeup = 10;
  Monsters[ML2+2].level = 2;
  Monsters[ML2+2].status = GREEDY + HOSTILE+MOBILE;
  Monsters[ML2+2].immunity = 0;
  Monsters[ML2+2].xpv = 20;
  Monsters[ML2+2].treasure = 3;
  Monsters[ML2+2].possessions = NULL;
  Monsters[ML2+2].sleep = 0;
  Monsters[ML2+2].x = -1;
  Monsters[ML2+2].y = -1;
  Monsters[ML2+2].transformid = -1;
  Monsters[ML2+2].corpsevalue = 0;
  if (random_range(2)) {
    Monsters[ML2+2].monchar = 'm';
    strcpy(Monsters[ML2+2].monstring,"man");
  }
  else {
    Monsters[ML0+1].monchar = 'w';
    strcpy(Monsters[ML2+2].monstring,"woman");
  }
  strcpy(Monsters[ML2+2].corpsestr,"dead sneak-thief");
  Monsters[ML2+2].corpseweight = 1000;
  Monsters[ML2+2].talkf = M_TALK_THIEF;
  Monsters[ML2+2].movef = M_MOVE_SMART;
  Monsters[ML2+2].meleef = M_NO_OP;
  Monsters[ML2+2].strikef = M_NO_OP;
  Monsters[ML2+2].specialf = M_SP_THIEF;



  /* floating eye */
  Monsters[ML2+3].id = ML2+3;
  Monsters[ML2+3].hp = 25;
  Monsters[ML2+3].speed = 5;
  Monsters[ML2+3].hit = 0;
  Monsters[ML2+3].actions = 0;
  Monsters[ML2+3].tactics = MM_POOR;
  Monsters[ML2+3].ac = 0;
  Monsters[ML2+3].dmg = 0;
  Monsters[ML2+3].sense = 10;
  Monsters[ML2+3].wakeup = 10;
  Monsters[ML2+3].level = 2;
  Monsters[ML2+3].status = MOBILE+FLYING;
  Monsters[ML2+3].immunity = pow2(SLEEP);
  Monsters[ML2+3].xpv = 10;
  Monsters[ML2+3].treasure = 0;
  Monsters[ML2+3].possessions = NULL;
  Monsters[ML2+3].sleep = 0;
  Monsters[ML2+3].x = -1;
  Monsters[ML2+3].y = -1;
  Monsters[ML2+3].transformid = POTIONID+16;
  Monsters[ML2+3].corpsevalue = 20;
  Monsters[ML2+3].monchar = 'e';
  strcpy(Monsters[ML2+3].monstring,"floating eye");
  strcpy(Monsters[ML2+3].corpsestr,"dead floating eye");
  Monsters[ML2+3].corpseweight = 1;
  Monsters[ML2+3].talkf = M_NO_OP;
  Monsters[ML2+3].movef = M_MOVE_RANDOM;
  Monsters[ML2+3].meleef = M_NO_OP;
  Monsters[ML2+3].strikef = M_NO_OP;
  Monsters[ML2+3].specialf = M_SP_EXPLODE;



  /* tove */
  Monsters[ML2+4].id = ML2+4;
  Monsters[ML2+4].hp = 30;
  Monsters[ML2+4].speed = 4;
  Monsters[ML2+4].hit = 10;
  Monsters[ML2+4].actions = 2;
  Monsters[ML2+4].tactics = MM_FORCED+MM_AVERAGE;
  Monsters[ML2+4].hitloc = LOW;
  Monsters[ML2+4].ac = 10;
  Monsters[ML2+4].dmg = 10;
  Monsters[ML2+4].sense = 6;
  Monsters[ML2+4].wakeup = 3;
  Monsters[ML2+4].level = 2;
  Monsters[ML2+4].status = 0+MOBILE;
  Monsters[ML2+4].immunity = 0;
  Monsters[ML2+4].xpv = 15;
  Monsters[ML2+4].treasure = 0;
  Monsters[ML2+4].possessions = NULL;
  Monsters[ML2+4].sleep = 0;
  Monsters[ML2+4].x = -1;
  Monsters[ML2+4].y = -1;
  Monsters[ML2+4].transformid = -1;
  Monsters[ML2+4].corpsevalue = 0;
  Monsters[ML2+4].monchar = 't';
  strcpy(Monsters[ML2+4].monstring,"tove");
  strcpy(Monsters[ML2+4].corpsestr,"dead tove");
  Monsters[ML2+4].corpseweight = 10;
  Monsters[ML2+4].talkf = M_TALK_SLITHY;
  Monsters[ML2+4].movef = M_MOVE_FLUTTER;
  Monsters[ML2+4].meleef = M_MELEE_NORMAL;
  Monsters[ML2+4].strikef = M_NO_OP;
  Monsters[ML2+4].specialf = M_NO_OP;



  /* transparent nasty  */
  Monsters[ML2+5].id = ML2+5;
  Monsters[ML2+5].hp = 32;
  Monsters[ML2+5].speed = 8;
  Monsters[ML2+5].hit = 15;
  Monsters[ML2+5].actions = 2;
  Monsters[ML2+5].tactics = MM_FORCED+MM_AVERAGE;
  Monsters[ML2+5].hitloc = 2;
  Monsters[ML2+5].ac = 10;
  Monsters[ML2+5].dmg = 5;
  Monsters[ML2+5].sense = 5;
  Monsters[ML2+5].wakeup = 3;
  Monsters[ML2+5].level = 2;
  Monsters[ML2+5].status =  HOSTILE+MOBILE+M_INVISIBLE;
  Monsters[ML2+5].immunity = 0;
  Monsters[ML2+5].xpv = 20;
  Monsters[ML2+5].treasure = 1;
  Monsters[ML2+5].possessions = NULL;
  Monsters[ML2+5].sleep = 0;
  Monsters[ML2+5].x = -1;
  Monsters[ML2+5].y = -1;
  Monsters[ML2+5].transformid = POTIONID+10;
  Monsters[ML2+5].corpsevalue = 30;
  Monsters[ML2+5].monchar = 'n';
  strcpy(Monsters[ML2+5].monstring,"transparent nasty");
  strcpy(Monsters[ML2+5].corpsestr,"dead transparent nasty");
  Monsters[ML2+5].corpseweight = 50;
  Monsters[ML2+5].talkf = M_NO_OP;
  Monsters[ML2+5].movef = M_MOVE_FLUTTER;
  Monsters[ML2+5].meleef = M_MELEE_NORMAL;
  Monsters[ML2+5].strikef = M_STRIKE_MISSILE;
  Monsters[ML2+5].specialf = M_NO_OP;


  /* ghost  */
  Monsters[ML2+6].id = ML2+6;
  Monsters[ML2+6].hp = 32;
  Monsters[ML2+6].speed = 20;
  Monsters[ML2+6].hit = 0;
  Monsters[ML2+6].actions = 0;
  Monsters[ML2+6].tactics = MM_POOR;
  Monsters[ML2+6].ac = 10;
  Monsters[ML2+6].dmg = 0;
  Monsters[ML2+6].sense = 5;
  Monsters[ML2+6].wakeup = 10;
  Monsters[ML2+6].level = 2;
  Monsters[ML2+6].status =  AWAKE+WANDERING+MOBILE+INTANGIBLE;
  Monsters[ML2+6].immunity = pow2(SLEEP)+pow2(FEAR)+pow2(NORMAL_DAMAGE);
  Monsters[ML2+6].xpv = 30;
  Monsters[ML2+6].treasure = 0;
  Monsters[ML2+6].possessions = NULL;
  Monsters[ML2+6].sleep = 0;
  Monsters[ML2+6].x = -1;
  Monsters[ML2+6].y = -1;
  Monsters[ML2+6].transformid = -1;
  Monsters[ML2+6].corpsevalue = 0;
  Monsters[ML2+6].monchar = 'g';
  strcpy(Monsters[ML2+6].monstring,"ghost");
  strcpy(Monsters[ML2+6].corpsestr,"ghost ectoplasm");
  Monsters[ML2+6].corpseweight = 1;
  Monsters[ML2+6].talkf = M_TALK_GHOST;
  Monsters[ML2+6].movef = M_MOVE_SPIRIT;
  Monsters[ML2+6].meleef = M_NO_OP;
  Monsters[ML2+6].strikef = M_NO_OP;
  Monsters[ML2+6].specialf = M_SP_GHOST;


  /* enchanter  */
  Monsters[ML2+7].id = ML2+7;
  Monsters[ML2+7].hp = 20;
  Monsters[ML2+7].speed = 8;
  Monsters[ML2+7].hit = 5;
  Monsters[ML2+7].actions = 2;
  Monsters[ML2+7].tactics = MM_TIMID+MM_POOR;
  Monsters[ML2+7].ac = 10;
  Monsters[ML2+7].dmg = 5;
  Monsters[ML2+7].sense = 5;
  Monsters[ML2+7].wakeup = 3;
  Monsters[ML2+7].level = 2;
  Monsters[ML2+7].status =  AWAKE+HOSTILE+MOBILE+COWARDLY;
  Monsters[ML2+7].immunity = pow2(COLD);
  Monsters[ML2+7].xpv = 40;
  Monsters[ML2+7].treasure = 2;
  Monsters[ML2+7].possessions = NULL;
  Monsters[ML2+7].sleep = 0;
  Monsters[ML2+7].x = -1;
  Monsters[ML2+7].y = -1;
  Monsters[ML2+7].transformid = -1;
  Monsters[ML2+7].corpsevalue = 100;
  Monsters[ML2+7].monchar = 'e';
  strcpy(Monsters[ML2+7].monstring,"enchanter");
  strcpy(Monsters[ML2+7].corpsestr,"enchanter's wand");
  Monsters[ML2+7].corpseweight = 1;
  Monsters[ML2+7].talkf = M_TALK_EVIL;
  Monsters[ML2+7].movef = M_MOVE_SMART;
  Monsters[ML2+7].meleef = M_MELEE_NORMAL;
  Monsters[ML2+7].strikef = M_NO_OP;
  Monsters[ML2+7].specialf = M_SP_SPELL;



  /* murk fungus  */
  Monsters[ML2+8].id = ML2+7;
  Monsters[ML2+8].hp = 7;
  Monsters[ML2+8].speed = 12;
  Monsters[ML2+8].hit = 0;
  Monsters[ML2+8].actions = 0;
  Monsters[ML2+8].tactics = MM_POOR;
  Monsters[ML2+8].ac = 0;
  Monsters[ML2+8].dmg = 0;
  Monsters[ML2+8].sense = 10;
  Monsters[ML2+8].wakeup = 3;
  Monsters[ML2+8].level = 2;
  Monsters[ML2+8].status =  AWAKE+HOSTILE;
  Monsters[ML2+8].immunity = 0;
  Monsters[ML2+8].xpv = 1;
  Monsters[ML2+8].treasure = 0;
  Monsters[ML2+8].possessions = NULL;
  Monsters[ML2+8].sleep = 0;
  Monsters[ML2+8].x = -1;
  Monsters[ML2+8].y = -1;
  Monsters[ML2+8].transformid = SCROLLID+22;
  Monsters[ML2+8].corpsevalue = 10;
  Monsters[ML2+8].monchar = 'f';
  strcpy(Monsters[ML2+8].monstring,"murk fungus");
  strcpy(Monsters[ML2+8].corpsestr,"murk cloud particles");
  Monsters[ML2+8].corpseweight = 1;
  Monsters[ML2+8].talkf = M_NO_OP;
  Monsters[ML2+8].movef = M_NO_OP;
  Monsters[ML2+8].meleef = M_NO_OP;
  Monsters[ML2+8].strikef = M_NO_OP;
  Monsters[ML2+8].specialf = M_SP_BLACKOUT;





  /* the salamander */
  Monsters[ML3+0].id = ML3+0;
  Monsters[ML3+0].hp = 35;
  Monsters[ML3+0].speed = 4;
  Monsters[ML3+0].hit = 10;
  Monsters[ML3+0].actions = 4;
  Monsters[ML3+0].tactics = MM_AVERAGE;
  Monsters[ML3+0].ac = 5;
  Monsters[ML3+0].dmg = 30;
  Monsters[ML3+0].sense = 4;
  Monsters[ML3+0].wakeup = 1;
  Monsters[ML3+0].level = 3;
  Monsters[ML3+0].status = HOSTILE+GREEDY+MOBILE;
  Monsters[ML3+0].immunity = pow2(FLAME);
  Monsters[ML3+0].sleep = 50;
  Monsters[ML3+0].treasure = 0;
  Monsters[ML3+0].possessions = NULL;
  Monsters[ML3+0].xpv = 75;
  Monsters[ML3+0].x = -1;
  Monsters[ML3+0].y = -1;
  Monsters[ML3+0].transformid = RINGID+4;
  Monsters[ML3+0].corpsevalue = 50;
  Monsters[ML3+0].monchar = 's';
  strcpy(Monsters[ML3+0].monstring,"salamander");
  strcpy(Monsters[ML3+0].corpsestr,"salamander scales");
  Monsters[ML3+0].corpseweight = 50;
  Monsters[ML3+0].talkf = M_TALK_STUPID;
  Monsters[ML3+0].movef = M_MOVE_FLUTTER;
  Monsters[ML3+0].meleef = M_MELEE_FIRE;
  Monsters[ML3+0].strikef = M_NO_OP;
  Monsters[ML3+0].specialf = M_NO_OP;



  /* the catoblepas */
  Monsters[ML3+1].id = ML3+1;
  Monsters[ML3+1].hp = 100;
  Monsters[ML3+1].speed = 8;
  Monsters[ML3+1].hit = 10;
  Monsters[ML3+1].actions = 2;
  Monsters[ML3+1].tactics = MM_FORCED+MM_POOR;
  Monsters[ML3+1].hitloc = HIGH;
  Monsters[ML3+1].ac = 10;
  Monsters[ML3+1].dmg = 20;
  Monsters[ML3+1].sense = 5;
  Monsters[ML3+1].wakeup = 2;
  Monsters[ML3+1].level = 3;
  Monsters[ML3+1].status = HOSTILE+HUNGRY+GREEDY+MOBILE;
  Monsters[ML3+1].immunity = 0;
  Monsters[ML3+1].xpv = 50;
  Monsters[ML3+1].treasure = 0;
  Monsters[ML3+1].possessions = NULL;
  Monsters[ML3+1].sleep = 50;
  Monsters[ML3+1].x = -1;
  Monsters[ML3+1].y = -1;
  Monsters[ML3+1].transformid = RINGID+3;
  Monsters[ML3+1].corpsevalue = 50;
  Monsters[ML3+1].monchar = 'C';
  strcpy(Monsters[ML3+1].monstring,"catoblepas");
  strcpy(Monsters[ML3+1].corpsestr,"catoblepas' eyes");
  Monsters[ML3+1].corpseweight = 10;
  Monsters[ML3+1].talkf = M_TALK_STUPID;
  Monsters[ML3+1].movef = M_MOVE_NORMAL;
  Monsters[ML3+1].meleef = M_MELEE_NORMAL;
  Monsters[ML3+1].strikef = M_STRIKE_BLIND;
  Monsters[ML3+1].specialf = M_NO_OP;



  /* Lesser Frost Demon */
  Monsters[ML3+2].id = ML3+2;
  Monsters[ML3+2].hp = 15;
  Monsters[ML3+2].speed = 4;
  Monsters[ML3+2].hit = 15;
  Monsters[ML3+2].actions = 3;
  Monsters[ML3+2].tactics = MM_GOOD;
  Monsters[ML3+2].ac = 15;
  Monsters[ML3+2].dmg = 15;
  Monsters[ML3+2].sense = 3;
  Monsters[ML3+2].wakeup = 3;
  Monsters[ML3+2].level = 3;
  Monsters[ML3+2].status = HOSTILE+MOBILE;
  Monsters[ML3+2].immunity = pow2(SLEEP)+pow2(POISON)+pow2(FEAR)+pow2(COLD);
  Monsters[ML3+2].xpv = 50;
  Monsters[ML3+2].treasure = 2;
  Monsters[ML3+2].possessions = NULL;
  Monsters[ML3+2].sleep = 0;
  Monsters[ML3+2].x = -1;
  Monsters[ML3+2].y = -1;
  Monsters[ML3+2].transformid = STICKID+10;
  Monsters[ML3+2].corpsevalue = 50;
  Monsters[ML3+2].monchar = 'f';
  strcpy(Monsters[ML3+2].monstring,"lesser frost demon");
  strcpy(Monsters[ML3+2].corpsestr,"lesser frost demon's heart");
  Monsters[ML3+2].corpseweight = 25;
  Monsters[ML3+2].talkf = M_TALK_EVIL;
  Monsters[ML3+2].movef = M_MOVE_SMART;
  Monsters[ML3+2].meleef = M_MELEE_COLD;
  Monsters[ML3+2].strikef = M_NO_OP;
  Monsters[ML3+2].specialf = M_SP_DEMON;



  /* acid cloud */
  Monsters[ML3+3].id = ML3+3;
  Monsters[ML3+3].hp = 10;
  Monsters[ML3+3].speed = 20;
  Monsters[ML3+3].hit = 20;
  Monsters[ML3+3].actions = 1;
  Monsters[ML3+3].tactics = MM_POOR;
  Monsters[ML3+3].ac = 0;
  Monsters[ML3+3].dmg = 50;
  Monsters[ML3+3].sense = 5;
  Monsters[ML3+3].wakeup = 1;
  Monsters[ML3+3].level = 3;
  Monsters[ML3+3].status = 0+MOBILE+FLYING;
  Monsters[ML3+3].immunity = pow2(SLEEP)+pow2(POISON);
  Monsters[ML3+3].xpv = 30;
  Monsters[ML3+3].treasure = 0;
  Monsters[ML3+3].possessions = NULL;
  Monsters[ML3+3].sleep = 0;
  Monsters[ML3+3].x = -1;
  Monsters[ML3+3].y = -1;
  Monsters[ML3+3].transformid = -1;
  Monsters[ML3+3].corpsevalue = 0;
  Monsters[ML3+3].monchar = 'a';
  strcpy(Monsters[ML3+3].monstring,"acid cloud");
  strcpy(Monsters[ML3+3].corpsestr,"acid pool");
  Monsters[ML3+3].corpseweight = 10;
  Monsters[ML3+3].talkf = M_NO_OP;
  Monsters[ML3+3].movef = M_MOVE_FOLLOW;
  Monsters[ML3+3].meleef = M_MELEE_NORMAL;
  Monsters[ML3+3].strikef = M_NO_OP;
  Monsters[ML3+3].specialf = M_SP_ACID_CLOUD;



  /* phantom */
  Monsters[ML3+4].id = ML3+4;
  Monsters[ML3+4].hp = 1;
  Monsters[ML3+4].speed = 10;
  Monsters[ML3+4].hit = 20;
  Monsters[ML3+4].actions = 3;
  Monsters[ML3+4].tactics = MM_GOOD+MM_BERSERK;
  Monsters[ML3+4].ac = 0;
  Monsters[ML3+4].dmg = 0;
  Monsters[ML3+4].sense = 5;
  Monsters[ML3+4].wakeup = 5;
  Monsters[ML3+4].level = 2;
  Monsters[ML3+4].status = HOSTILE+MOBILE+INTANGIBLE+FLYING+M_INVISIBLE;
  Monsters[ML3+4].immunity = pow2(SLEEP)+pow2(POISON)+pow2(FEAR)+
                             pow2(NORMAL_DAMAGE);
  Monsters[ML3+4].xpv = 50;
  Monsters[ML3+4].treasure = 0;
  Monsters[ML3+4].possessions = NULL;
  Monsters[ML3+4].sleep = 100;
  Monsters[ML3+4].x = -1;
  Monsters[ML3+4].y = -1;
  Monsters[ML3+4].transformid = -1;
  Monsters[ML3+4].corpsevalue = 0;
  Monsters[ML3+4].monchar = 'p';
  strcpy(Monsters[ML3+4].monstring,"phantom");
  strcpy(Monsters[ML3+4].corpsestr,"phantom's cowl");
  Monsters[ML3+4].corpseweight = 10;
  Monsters[ML3+4].talkf = M_TALK_SILENT;
  Monsters[ML3+4].movef = M_MOVE_SPIRIT;
  Monsters[ML3+4].meleef = M_MELEE_SPIRIT;
  Monsters[ML3+4].strikef = M_NO_OP;
  Monsters[ML3+4].specialf = M_SP_SURPRISE;


  /* Goblin King */
  Monsters[ML3+5].id = ML3+5;
  Monsters[ML3+5].uniqueness = 2;
  Monsters[ML3+5].speed = 5;
  Monsters[ML3+5].hp = 32;
  Monsters[ML3+5].hit = 14;
  Monsters[ML3+5].actions = 5;
  Monsters[ML3+5].tactics = MM_GOOD;
  Monsters[ML3+5].ac = 10;
  Monsters[ML3+5].dmg = 32;
  Monsters[ML3+5].sense = 5;
  Monsters[ML3+5].wakeup = 20;
  Monsters[ML3+5].level = 3;
  Monsters[ML3+5].status = HOSTILE+GREEDY+MOBILE;
  Monsters[ML3+5].immunity = 0;
  Monsters[ML3+5].xpv = 75;
  Monsters[ML3+5].treasure = 3;
  Monsters[ML3+5].possessions = NULL;
  /******* this is OK, since both creature and object are unique */
  tob = ((pob) (calloc(1,sizeof(objtype))));
  make_weapon(tob,36); /* Goblin's axe */
  m_pickup(&(Monsters[ML3+5]),tob);
  /*******/
  Monsters[ML3+5].sleep = 100;
  Monsters[ML3+5].x = -1;
  Monsters[ML3+5].y = -1;
  Monsters[ML3+5].transformid = -1;
  Monsters[ML3+5].corpsevalue = 0;
  Monsters[ML3+5].monchar = 'G';
  strcpy(Monsters[ML3+5].monstring,"The Goblin King");
  strcpy(Monsters[ML3+5].corpsestr,"The Head of the Goblin King");
  Monsters[ML3+5].corpseweight = 100;
  Monsters[ML3+5].talkf = M_TALK_GREEDY;
  Monsters[ML3+5].movef = M_MOVE_SMART;
  Monsters[ML3+5].meleef = M_MELEE_NORMAL;
  Monsters[ML3+5].strikef = M_STRIKE_MISSILE;
  Monsters[ML3+5].specialf = M_NO_OP;

  /* Living Whirlwind */
  Monsters[ML3+6].id = ML3+6;
  Monsters[ML3+6].hp = 16;
  Monsters[ML3+6].hit = 0;
  Monsters[ML3+6].speed = 10;
  Monsters[ML3+6].actions = 0;
  Monsters[ML3+6].tactics = MM_POOR;
  Monsters[ML3+6].ac = 1000;
  Monsters[ML3+6].dmg = 0;
  Monsters[ML3+6].sense = 5;
  Monsters[ML3+6].wakeup = 0;
  Monsters[ML3+6].level = 3;
  Monsters[ML3+6].status = HOSTILE+MOBILE;
  Monsters[ML3+6].immunity = EVERYTHING-FLAME;
  Monsters[ML3+6].xpv = 75;
  Monsters[ML3+6].treasure = 3;
  Monsters[ML3+6].possessions = NULL;
  Monsters[ML3+6].sleep = 0;
  Monsters[ML3+6].x = -1;
  Monsters[ML3+6].y = -1;
  Monsters[ML3+6].transformid = -1;
  Monsters[ML3+6].corpsevalue = 200;
  Monsters[ML3+6].monchar = '6';
  strcpy(Monsters[ML3+6].monstring,"living whirlwind ");
  strcpy(Monsters[ML3+6].corpsestr,"whirlwind's eye");
  Monsters[ML3+6].corpseweight = 1;
  Monsters[ML3+6].talkf = M_NO_OP;
  Monsters[ML3+6].movef = M_MOVE_RANDOM;
  Monsters[ML3+6].meleef = M_NO_OP;
  Monsters[ML3+6].strikef = M_NO_OP;
  Monsters[ML3+6].specialf = M_SP_WHIRL;
}
