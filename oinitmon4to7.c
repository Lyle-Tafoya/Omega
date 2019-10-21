/* omega copyright (c) 1987 by Laurence Raphael Brothers */
/* oinitmon4to7.c */
/* monster levels 4 to 7 */

#include <strings.h>
#include "oglob.h"

void initmon4to7()
{
  /* the manticore */
  Monsters[ML4+0].id = ML4+0;
  Monsters[ML4+0].hp = 100;
  Monsters[ML4+0].speed = 6;
  Monsters[ML4+0].hit = 15;
  Monsters[ML4+0].actions = 4;
  Monsters[ML4+0].tactics = MM_BERSERK+MM_POOR;
  Monsters[ML4+0].ac = 12;
  Monsters[ML4+0].dmg = 25;
  Monsters[ML4+0].sense = 6;
  Monsters[ML4+0].wakeup = 2;
  Monsters[ML4+0].level = 4;
  Monsters[ML4+0].status = HOSTILE+HUNGRY+MOBILE+FLYING;
  Monsters[ML4+0].immunity = 0;
  Monsters[ML4+0].sleep = 25;
  Monsters[ML4+0].treasure = 2;
  Monsters[ML4+0].possessions = NULL;
  Monsters[ML4+0].xpv = 100;
  Monsters[ML4+0].x = -1;
  Monsters[ML4+0].y = -1;
  Monsters[ML4+0].transformid = STICKID+11;
  Monsters[ML4+0].corpsevalue = 20;
  Monsters[ML4+0].monchar = 'M';
  strcpy(Monsters[ML4+0].monstring,"manticore");
  strcpy(Monsters[ML4+0].corpsestr,"manticore spikes");
  Monsters[ML4+0].corpseweight = 100;
  Monsters[ML4+0].talkf = M_TALK_HUNGRY;
  Monsters[ML4+0].movef = M_MOVE_NORMAL;
  Monsters[ML4+0].meleef = M_MELEE_NORMAL;
  Monsters[ML4+0].strikef = M_STRIKE_MISSILE;
  Monsters[ML4+0].specialf = M_NO_OP;

  /* the tasmanian devil */
  Monsters[ML4+1].id = ML4+1;
  Monsters[ML4+1].hp = 50;
  Monsters[ML4+1].speed = 2;
  Monsters[ML4+1].hit = 12;
  Monsters[ML4+1].actions =  8;
  Monsters[ML4+1].tactics = MM_BERSERK+MM_POOR;
  Monsters[ML4+1].hitloc = LOW;
  Monsters[ML4+1].ac = 10;
  Monsters[ML4+1].dmg = 10;
  Monsters[ML4+1].sense = 10;
  Monsters[ML4+1].wakeup = 10;
  Monsters[ML4+1].level = 4;
  Monsters[ML4+1].status = HOSTILE+HUNGRY+MOBILE;
  Monsters[ML4+1].immunity = 0;
  Monsters[ML4+1].xpv = 75;
  Monsters[ML4+1].treasure = 0;
  Monsters[ML4+1].possessions = NULL;
  Monsters[ML4+1].sleep = 50;
  Monsters[ML4+1].x = -1;
  Monsters[ML4+1].y = -1;
  Monsters[ML4+1].transformid = POTIONID+6;
  Monsters[ML4+1].corpsevalue = 10;
  Monsters[ML4+1].monchar = 'T';
  strcpy(Monsters[ML4+1].monstring,"tasmanian devil");
  strcpy(Monsters[ML4+1].corpsestr,"dead tasmanian devil");
  Monsters[ML4+1].corpseweight = 300;
  Monsters[ML4+1].talkf = M_TALK_HUNGRY;
  Monsters[ML4+1].movef = M_MOVE_FLUTTER;
  Monsters[ML4+1].meleef = M_MELEE_NORMAL;
  Monsters[ML4+1].strikef = M_NO_OP;
  Monsters[ML4+1].specialf = M_NO_OP;

  /* Automaton Minor */
  Monsters[ML4+2].id = ML4+2;
  Monsters[ML4+2].hp = 100;
  Monsters[ML4+2].speed = 7;
  Monsters[ML4+2].hit = 15;
  Monsters[ML4+2].actions = 4;
  Monsters[ML4+2].tactics = MM_FORCED+MM_AVERAGE;
  Monsters[ML4+2].hitloc = CENTER;
  Monsters[ML4+2].ac = 15;
  Monsters[ML4+2].dmg = 50;
  Monsters[ML4+2].sense = 5;
  Monsters[ML4+2].wakeup = 2;
  Monsters[ML4+2].level = 4;
  Monsters[ML4+2].status = HOSTILE+MOBILE;
  Monsters[ML4+2].immunity = pow2(SLEEP)+pow2(POISON)+pow2(FEAR)+
                             pow2(ELECTRICITY);
  Monsters[ML4+2].xpv = 100;
  Monsters[ML4+2].treasure = 0;
  Monsters[ML4+2].possessions = NULL;
  Monsters[ML4+2].sleep = 100;
  Monsters[ML4+2].x = -1;
  Monsters[ML4+2].y = -1;
  Monsters[ML4+2].transformid = -1;
  Monsters[ML4+2].corpsevalue = 0;
  Monsters[ML4+2].monchar = 'a';
  strcpy(Monsters[ML4+2].monstring,"automatum minor");
  strcpy(Monsters[ML4+2].corpsestr,"automatum minor battery");
  Monsters[ML4+2].corpseweight = 100;
  Monsters[ML4+2].talkf = M_TALK_ROBOT;
  Monsters[ML4+2].movef = M_MOVE_SMART;
  Monsters[ML4+2].meleef = M_MELEE_NORMAL;
  Monsters[ML4+2].strikef = M_NO_OP;
  Monsters[ML4+2].specialf = M_SP_POISON_CLOUD;

  /* denebian slime devil */
  Monsters[ML4+3].id = ML4+3;
  Monsters[ML4+3].hp = 75;
  Monsters[ML4+3].speed = 12;
  Monsters[ML4+3].hit = 30;
  Monsters[ML4+3].actions = 3;
  Monsters[ML4+3].tactics = MM_FORCED+MM_BERSERK;
  Monsters[ML4+3].hitloc = LOW;
  Monsters[ML4+3].ac = 10;
  Monsters[ML4+3].dmg = 15;
  Monsters[ML4+3].sense = 5;
  Monsters[ML4+3].wakeup = 1;
  Monsters[ML4+3].level = 4;
  Monsters[ML4+3].status = HOSTILE+HUNGRY+MOBILE+SWIMMING+M_INVISIBLE;
  Monsters[ML4+3].immunity = pow2(POISON)+pow2(FLAME)+pow2(ELECTRICITY);
  Monsters[ML4+3].xpv = 125;
  Monsters[ML4+3].treasure = 0;
  Monsters[ML4+3].possessions = NULL;
  Monsters[ML4+3].sleep = 75;
  Monsters[ML4+3].x = -1;
  Monsters[ML4+3].y = -1;
  Monsters[ML4+3].transformid = POTIONID+17;
  Monsters[ML4+3].corpsevalue = 20;
  Monsters[ML4+3].monchar = 's';
  strcpy(Monsters[ML4+3].monstring,"denebian slime devil");
  strcpy(Monsters[ML4+3].corpsestr,"denebian slime");
  Monsters[ML4+3].corpseweight = 50;
  Monsters[ML4+3].talkf = M_TALK_SILENT;
  Monsters[ML4+3].movef = M_MOVE_NORMAL;
  Monsters[ML4+3].meleef = M_MELEE_DISEASE;
  Monsters[ML4+3].strikef = M_NO_OP;
  Monsters[ML4+3].specialf = M_SP_SURPRISE;


  /* jub-jub bird */
  Monsters[ML4+4].id = ML4+4;
  Monsters[ML4+4].hp = 50;
  Monsters[ML4+4].speed = 3;
  Monsters[ML4+4].hit = 18;
  Monsters[ML4+4].actions =  3;
  Monsters[ML4+4].tactics = MM_BERSERK+MM_AVERAGE;
  Monsters[ML4+4].ac = 12;
  Monsters[ML4+4].dmg = 25;
  Monsters[ML4+4].sense = 5;
  Monsters[ML4+4].wakeup = 1;
  Monsters[ML4+4].level = 4;
  Monsters[ML4+4].status = HOSTILE+HUNGRY+MOBILE+FLYING;
  Monsters[ML4+4].immunity = pow2(POISON);
  Monsters[ML4+4].xpv = 125;
  Monsters[ML4+4].treasure = 0;
  Monsters[ML4+4].possessions = NULL;
  Monsters[ML4+4].sleep = 75;
  Monsters[ML4+4].x = -1;
  Monsters[ML4+4].y = -1;
  Monsters[ML4+4].transformid = -1;
  Monsters[ML4+4].corpsevalue = 0;
  Monsters[ML4+4].monchar = 'j';
  strcpy(Monsters[ML4+4].monstring,"jub-jub bird");
  strcpy(Monsters[ML4+4].corpsestr,"dead jub-jub bird");
  Monsters[ML4+4].corpseweight = 500;
  Monsters[ML4+4].talkf = M_TALK_BURBLE;
  Monsters[ML4+4].movef = M_MOVE_FLUTTER;
  Monsters[ML4+4].meleef = M_MELEE_NORMAL;
  Monsters[ML4+4].strikef = M_NO_OP;
  Monsters[ML4+4].specialf = M_NO_OP;

  /* haunt */
  Monsters[ML4+5].id = ML4+5;
  Monsters[ML4+5].hp = 50;
  Monsters[ML4+5].speed = 7;
  Monsters[ML4+5].hit = 15;
  Monsters[ML4+5].actions = 3;
  Monsters[ML4+5].tactics = MM_GOOD;
  Monsters[ML4+5].ac = 5;
  Monsters[ML4+5].dmg = 20;
  Monsters[ML4+5].sense = 4;
  Monsters[ML4+5].wakeup = 1;
  Monsters[ML4+5].level = 4;
  Monsters[ML4+5].status = HOSTILE+HUNGRY+MOBILE+M_INVISIBLE;
  Monsters[ML4+5].immunity = pow2(POISON)+pow2(SLEEP)+pow2(FEAR);
  Monsters[ML4+5].xpv = 150;
  Monsters[ML4+5].treasure = 0;
  Monsters[ML4+5].possessions = NULL;
  Monsters[ML4+5].sleep = 0;
  Monsters[ML4+5].x = -1;
  Monsters[ML4+5].y = -1;
  Monsters[ML4+5].transformid = -1;
  Monsters[ML4+5].corpsevalue = 0;
  Monsters[ML4+5].monchar = 'h';
  strcpy(Monsters[ML4+5].monstring,"haunt");
  strcpy(Monsters[ML4+5].corpsestr,"dessicated corpse");
  Monsters[ML4+5].corpseweight = 500;
  Monsters[ML4+5].talkf = M_TALK_EVIL;
  Monsters[ML4+5].movef = M_MOVE_NORMAL;
  Monsters[ML4+5].meleef = M_MELEE_POISON;
  Monsters[ML4+5].strikef = M_NO_OP;
  Monsters[ML4+5].specialf = M_SP_SURPRISE;

  /* incubus/succubus */
  Monsters[ML4+6].id = ML4+6;
  Monsters[ML4+6].hp = 50;
  Monsters[ML4+6].speed = 5;
  Monsters[ML4+6].hit = 20;
  Monsters[ML4+6].actions = 4;
  Monsters[ML4+6].tactics = MM_GOOD;
  Monsters[ML4+6].ac = 10;
  Monsters[ML4+6].dmg = 10;
  Monsters[ML4+6].sense = 4;
  Monsters[ML4+6].wakeup = 4;
  Monsters[ML4+6].level = 4;
  Monsters[ML4+6].status = MOBILE+HOSTILE;
  Monsters[ML4+6].immunity = pow2(POISON)+pow2(SLEEP)+pow2(FEAR);
  Monsters[ML4+6].xpv = 150;
  Monsters[ML4+6].treasure = 1;
  Monsters[ML4+6].possessions = NULL;
  Monsters[ML4+6].sleep = 0;
  Monsters[ML4+6].x = -1;
  Monsters[ML4+6].y = -1;
  Monsters[ML4+6].transformid = -1;
  Monsters[ML4+6].corpsevalue = 200;
  if (Player.preference == 'm') {
    Monsters[ML4+6].monchar = 's';
    strcpy(Monsters[ML4+6].monstring,"satyr");
    strcpy(Monsters[ML4+6].corpsestr,"incubus' horns");
  }
  else {
    Monsters[ML4+6].monchar = 'n';
    strcpy(Monsters[ML4+6].monstring,"nymph");
    strcpy(Monsters[ML4+6].corpsestr,"succubus' horns");
  }
  Monsters[ML4+6].corpseweight = 1;
  Monsters[ML4+6].talkf = M_TALK_DEMONLOVER;
  Monsters[ML4+6].movef = M_MOVE_SMART;
  Monsters[ML4+6].meleef = M_NO_OP;
  Monsters[ML4+6].strikef = M_NO_OP;
  Monsters[ML4+6].specialf = M_SP_DEMONLOVER;

  /* satyr/nymph */
  Monsters[ML4+7].id = ML4+7;
  Monsters[ML4+7].hp = 50;
  Monsters[ML4+7].speed = 6;
  Monsters[ML4+7].hit = 0;
  Monsters[ML4+7].actions = 1;
  Monsters[ML4+7].tactics = MM_POOR+MM_TIMID;
  Monsters[ML4+7].ac = 10;
  Monsters[ML4+7].dmg = 0;
  Monsters[ML4+7].sense = 4;
  Monsters[ML4+7].wakeup = 4;
  Monsters[ML4+7].level = 4;
  Monsters[ML4+7].status = MOBILE+NEEDY;
  Monsters[ML4+7].immunity = 0;
  Monsters[ML4+7].xpv = 50;
  Monsters[ML4+7].treasure = 0;
  Monsters[ML4+7].possessions = NULL;
  Monsters[ML4+7].sleep = 0;
  Monsters[ML4+7].x = -1;
  Monsters[ML4+7].y = -1;
  Monsters[ML4+7].transformid = -1;
  Monsters[ML4+7].corpsevalue = 0;
  if (Player.preference == 'm') {
    Monsters[ML4+7].monchar = 's';
    strcpy(Monsters[ML4+7].monstring,"satyr");
    strcpy(Monsters[ML4+7].corpsestr,"satyr's horns");
  }
  else {
    Monsters[ML4+7].monchar = 'n';
    strcpy(Monsters[ML4+7].monstring,"nymph");
    strcpy(Monsters[ML4+7].corpsestr,"nymph's lingerie");
  }
  Monsters[ML4+7].corpseweight = 1;
  Monsters[ML4+7].talkf = M_TALK_SEDUCTOR;
  Monsters[ML4+7].movef = M_MOVE_SMART;
  Monsters[ML4+7].meleef = M_NO_OP;
  Monsters[ML4+7].strikef = M_NO_OP;
  Monsters[ML4+7].specialf = M_SP_SEDUCTOR;



  /* Croc */
  Monsters[ML4+8].id = ML4+8;
  Monsters[ML4+8].hp = 80;
  Monsters[ML4+8].speed = 8;
  Monsters[ML4+8].hit = 15;
  Monsters[ML4+8].actions = 3;
  Monsters[ML4+8].tactics = MM_BERSERK+MM_FORCED;
  Monsters[ML4+8].hitloc = LOW;
  Monsters[ML4+8].ac = 20;
  Monsters[ML4+8].dmg = 30;
  Monsters[ML4+8].sense = 4;
  Monsters[ML4+8].wakeup = 1;
  Monsters[ML4+8].level = 4;
  Monsters[ML4+8].status = MOBILE+HOSTILE+HUNGRY;
  Monsters[ML4+8].immunity = 0;
  Monsters[ML4+8].xpv = 100;
  Monsters[ML4+8].treasure = 0;
  Monsters[ML4+8].possessions = NULL;
  Monsters[ML4+8].sleep = 50;
  Monsters[ML4+8].x = -1;
  Monsters[ML4+8].y = -1;
  Monsters[ML4+8].transformid = -1;
  Monsters[ML4+8].corpsevalue = 100;
  strcpy(Monsters[ML4+8].monstring,"giant crocodile");
  strcpy(Monsters[ML4+8].corpsestr,"crocodile hide");
  Monsters[ML4+8].monchar = 'c';
  Monsters[ML4+8].corpseweight = 300;
  Monsters[ML4+8].talkf = M_NO_OP;
  Monsters[ML4+8].movef = M_MOVE_NORMAL;
  Monsters[ML4+8].meleef = M_MELEE_NORMAL;
  Monsters[ML4+8].strikef = M_NO_OP;
  Monsters[ML4+8].specialf = M_NO_OP;



  /* torpor beast */
  Monsters[ML4+9].id = ML4+9;
  Monsters[ML4+9].hp = 40;
  Monsters[ML4+9].speed = 12;
  Monsters[ML4+9].hit = 20;
  Monsters[ML4+9].actions = 2;
  Monsters[ML4+9].tactics = MM_FORCED+MM_POOR;
  Monsters[ML4+9].hitloc = CENTER;
  Monsters[ML4+9].ac = 10;
  Monsters[ML4+9].dmg = 10;
  Monsters[ML4+9].sense = 4;
  Monsters[ML4+9].wakeup = 4;
  Monsters[ML4+9].level = 4;
  Monsters[ML4+9].status = MOBILE+HOSTILE+HUNGRY;
  Monsters[ML4+9].immunity = pow2(SLEEP)+pow2(FEAR);
  Monsters[ML4+9].xpv = 120;
  Monsters[ML4+9].treasure = 0;
  Monsters[ML4+9].possessions = NULL;
  Monsters[ML4+9].sleep = 0;
  Monsters[ML4+9].x = -1;
  Monsters[ML4+9].y = -1;
  Monsters[ML4+9].transformid = POTIONID+5;
  Monsters[ML4+9].corpsevalue = 50;
  strcpy(Monsters[ML4+9].monstring,"torpor beast");
  strcpy(Monsters[ML4+9].corpsestr,"a valium gland");
  Monsters[ML4+9].monchar = 't';
  Monsters[ML4+9].corpseweight = 30;
  Monsters[ML4+9].talkf = M_TALK_STUPID;
  Monsters[ML4+9].movef = M_MOVE_NORMAL;
  Monsters[ML4+9].meleef = M_MELEE_SLEEP;
  Monsters[ML4+9].strikef = M_NO_OP;
  Monsters[ML4+9].specialf = M_NO_OP;



  /* the dragonette */
  Monsters[ML5+0].id = ML5+0;
  Monsters[ML5+0].hp = 50;
  Monsters[ML5+0].speed = 3;
  Monsters[ML5+0].hit = 20;
  Monsters[ML5+0].actions = 6;
  Monsters[ML5+0].tactics = MM_AVERAGE;
  Monsters[ML5+0].ac = 15;
  Monsters[ML5+0].dmg = 40;
  Monsters[ML5+0].sense = 10;
  Monsters[ML5+0].wakeup = 2;
  Monsters[ML5+0].level = 5;
  Monsters[ML5+0].status = HOSTILE+HUNGRY+GREEDY+MOBILE+FLYING;
  Monsters[ML5+0].immunity = pow2(FLAME);
  Monsters[ML5+0].xpv = 180;
  Monsters[ML5+0].treasure = 4;
  Monsters[ML5+0].possessions = NULL;
  Monsters[ML5+0].sleep = 90;
  Monsters[ML5+0].x = -1;
  Monsters[ML5+0].y = -1;
  Monsters[ML5+0].transformid = RINGID+4;
  Monsters[ML5+0].corpsevalue = 50;
  Monsters[ML5+0].monchar = 'd';
  strcpy(Monsters[ML5+0].monstring,"dragonette");
  strcpy(Monsters[ML5+0].corpsestr,"dead dragonette");
  Monsters[ML5+0].corpseweight = 500;
  Monsters[ML5+0].talkf = M_TALK_GREEDY;
  Monsters[ML5+0].movef = M_MOVE_NORMAL;
  Monsters[ML5+0].meleef = M_MELEE_DRAGON;
  Monsters[ML5+0].strikef = M_STRIKE_FBOLT;
  Monsters[ML5+0].specialf = M_NO_OP;


  /* the tesla monster */
  Monsters[ML5+1].id = ML5+1;
  Monsters[ML5+1].hp = 50;
  Monsters[ML5+1].speed = 12;
  Monsters[ML5+1].hit = 15;
  Monsters[ML5+1].ac = 10;
  Monsters[ML5+1].actions = 4;
  Monsters[ML5+1].tactics = MM_FORCED+MM_BERSERK;
  Monsters[ML5+1].hitloc = LOW;
  Monsters[ML5+1].dmg = 40;
  Monsters[ML5+1].sense = 10;
  Monsters[ML5+1].wakeup = 2;
  Monsters[ML5+1].level = 5;
  Monsters[ML5+1].status = HOSTILE+MOBILE;
  Monsters[ML5+1].immunity = pow2(ELECTRICITY);
  Monsters[ML5+1].xpv = 150;
  Monsters[ML5+1].treasure = 0;
  Monsters[ML5+1].possessions = NULL;
  Monsters[ML5+1].sleep = 40;
  Monsters[ML5+1].x = -1;
  Monsters[ML5+1].y = -1;
  Monsters[ML5+1].transformid = STICKID+3;
  Monsters[ML5+1].corpsevalue = 200;
  Monsters[ML5+1].monchar = 't';
  strcpy(Monsters[ML5+1].monstring,"tesla monster");
  strcpy(Monsters[ML5+1].corpsestr,"tesla monster whip");
  Monsters[ML5+1].corpseweight = 100;
  Monsters[ML5+1].talkf = M_TALK_STUPID;
  Monsters[ML5+1].movef = M_MOVE_FLUTTER;
  Monsters[ML5+1].meleef = M_MELEE_ELEC;
  Monsters[ML5+1].strikef = M_STRIKE_LBALL;
  Monsters[ML5+1].specialf = M_NO_OP;



  /* the wyvern */
  Monsters[ML5+2].id = ML5+2;
  Monsters[ML5+2].hp = 100;
  Monsters[ML5+2].speed = 4;
  Monsters[ML5+2].hit = 20;
  Monsters[ML5+2].actions = 4;
  Monsters[ML5+2].tactics = MM_AVERAGE;
  Monsters[ML5+2].ac = 12;
  Monsters[ML5+2].dmg = 20;
  Monsters[ML5+2].sense = 4;
  Monsters[ML5+2].wakeup = 2;
  Monsters[ML5+2].level = 5;
  Monsters[ML5+2].status = GREEDY+HOSTILE+MOBILE+FLYING;
  Monsters[ML5+2].immunity = pow2(FLAME);
  Monsters[ML5+2].xpv = 150;
  Monsters[ML5+2].treasure = 2;
  Monsters[ML5+2].possessions = NULL;
  Monsters[ML5+2].sleep = 80;
  Monsters[ML5+2].x = -1;
  Monsters[ML5+2].y = -1;
  Monsters[ML5+2].transformid = RINGID+6;
  Monsters[ML5+2].corpsevalue = 150;
  Monsters[ML5+2].monchar = 'w';
  strcpy(Monsters[ML5+2].monstring,"wyvern");
  strcpy(Monsters[ML5+2].corpsestr,"wyvern's sting");
  Monsters[ML5+2].corpseweight = 100;
  Monsters[ML5+2].talkf = M_TALK_GREEDY;
  Monsters[ML5+2].movef = M_MOVE_NORMAL;
  Monsters[ML5+2].meleef = M_MELEE_POISON;
  Monsters[ML5+2].strikef = M_NO_OP;
  Monsters[ML5+2].specialf = M_NO_OP;



  /* the radeligian cateagle */
  Monsters[ML5+3].id = ML5+3;
  Monsters[ML5+3].hp = 50;
  Monsters[ML5+3].speed = 2;
  Monsters[ML5+3].hit = 15;
  Monsters[ML5+3].actions = 5;
  Monsters[ML5+3].tactics = MM_FORCED+MM_BERSERK;
  Monsters[ML5+3].hitloc = HIGH;
  Monsters[ML5+3].ac = 10;
  Monsters[ML5+3].dmg = 20;
  Monsters[ML5+3].sense = 10;
  Monsters[ML5+3].wakeup = 2;
  Monsters[ML5+3].level = 5;
  Monsters[ML5+3].status = HUNGRY+HOSTILE+MOBILE+FLYING;
  Monsters[ML5+3].immunity = 0;
  Monsters[ML5+3].xpv = 150;
  Monsters[ML5+3].treasure = 0;
  Monsters[ML5+3].possessions = NULL;
  Monsters[ML5+3].sleep = 50;
  Monsters[ML5+3].x = -1;
  Monsters[ML5+3].y = -1;
  Monsters[ML5+3].transformid = -1;
  Monsters[ML5+3].corpsevalue = 0;
  Monsters[ML5+3].monchar = 'c';
  strcpy(Monsters[ML5+3].monstring,"radeligian cateagle");
  strcpy(Monsters[ML5+3].corpsestr,"dead radeligian cateagle");
  Monsters[ML5+3].corpseweight = 400;
  Monsters[ML5+3].talkf = M_TALK_HUNGRY;
  Monsters[ML5+3].movef = M_MOVE_FLUTTER;
  Monsters[ML5+3].meleef = M_MELEE_NORMAL;
  Monsters[ML5+3].strikef = M_NO_OP;
  Monsters[ML5+3].specialf = M_SP_FLUTTER;


  /* frost demon */
  Monsters[ML5+4].id = ML5+4;
  Monsters[ML5+4].hp = 75;
  Monsters[ML5+4].speed = 3;
  Monsters[ML5+4].hit = 20;
  Monsters[ML5+4].ac = 20;
  Monsters[ML5+4].actions = 4;
  Monsters[ML5+4].tactics = MM_GOOD;
  Monsters[ML5+4].dmg = 20;
  Monsters[ML5+4].sense = 5;
  Monsters[ML5+4].wakeup = 5;
  Monsters[ML5+4].level = 5;
  Monsters[ML5+4].status = HOSTILE+MOBILE;
  Monsters[ML5+4].immunity = pow2(SLEEP)+pow2(POISON)+pow2(FEAR)+pow2(COLD);
  Monsters[ML5+4].xpv = 175;
  Monsters[ML5+4].treasure = 3;
  Monsters[ML5+4].possessions = NULL;
  Monsters[ML5+4].sleep = 50;
  Monsters[ML5+4].x = -1;
  Monsters[ML5+4].y = -1;
  Monsters[ML5+4].transformid = STICKID+4;
  Monsters[ML5+4].corpsevalue = 200;
  Monsters[ML5+4].monchar = 'f';
  strcpy(Monsters[ML5+4].monstring,"frost demon");
  strcpy(Monsters[ML5+4].corpsestr,"frost demon's heart");
  Monsters[ML5+4].corpseweight = 50;
  Monsters[ML5+4].talkf = M_TALK_EVIL;
  Monsters[ML5+4].movef = M_MOVE_SMART;
  Monsters[ML5+4].meleef = M_MELEE_COLD;
  Monsters[ML5+4].strikef = M_STRIKE_SNOWBALL;
  Monsters[ML5+4].specialf = M_SP_DEMON;



  /* spectre */
  Monsters[ML5+5].id = ML5+5;
  Monsters[ML5+5].hp = 75;
  Monsters[ML5+5].speed = 6;
  Monsters[ML5+5].hit = 20;
  Monsters[ML5+5].actions = 5;
  Monsters[ML5+5].tactics = MM_GOOD;
  Monsters[ML5+5].ac = 20;
  Monsters[ML5+5].dmg = 20;
  Monsters[ML5+5].sense = 5;
  Monsters[ML5+5].wakeup = 2;
  Monsters[ML5+5].level = 5;
  Monsters[ML5+5].status = HOSTILE+MOBILE+FLYING;
  Monsters[ML5+5].immunity = pow2(SLEEP)+pow2(POISON)+pow2(FEAR);
  Monsters[ML5+5].xpv = 200;
  Monsters[ML5+5].treasure = 0;
  Monsters[ML5+5].possessions = NULL;
  Monsters[ML5+5].sleep = 50;
  Monsters[ML5+5].x = -1;
  Monsters[ML5+5].y = -1;
  Monsters[ML5+5].transformid = CLOAKID+1;
  Monsters[ML5+5].corpsevalue = 200;
  Monsters[ML5+5].monchar = 'S';
  strcpy(Monsters[ML5+5].monstring,"spectre");
  strcpy(Monsters[ML5+5].corpsestr,"spectre's veil");
  Monsters[ML5+5].corpseweight = 10;
  Monsters[ML5+5].talkf = M_TALK_EVIL;
  Monsters[ML5+5].movef = M_MOVE_SPIRIT;
  Monsters[ML5+5].meleef = M_MELEE_SPIRIT;
  Monsters[ML5+5].strikef = M_NO_OP;
  Monsters[ML5+5].specialf = M_NO_OP;




  /* Necromancer */
  Monsters[ML5+6].id = ML5+6;
  Monsters[ML5+6].hp = 50;
  Monsters[ML5+6].speed = 5;
  Monsters[ML5+6].hit = 12;
  Monsters[ML5+6].actions = 4;
  Monsters[ML5+6].tactics = MM_TIMID+MM_GOOD;
  Monsters[ML5+6].ac = 13;
  Monsters[ML5+6].dmg = 10;
  Monsters[ML5+6].sense = 5;
  Monsters[ML5+6].wakeup = 5;
  Monsters[ML5+6].level = 5;
  Monsters[ML5+6].status = HOSTILE+MOBILE+COWARDLY;
  Monsters[ML5+6].immunity = pow2(FLAME)+pow2(COLD)+pow2(ELECTRICITY);
  Monsters[ML5+6].xpv = 205;
  Monsters[ML5+6].treasure = 4;
  Monsters[ML5+6].possessions = NULL;
  Monsters[ML5+6].sleep = 50;
  Monsters[ML5+6].x = -1;
  Monsters[ML5+6].y = -1;
  Monsters[ML5+6].transformid = -1;
  Monsters[ML5+6].corpsevalue = 500;
  Monsters[ML5+6].monchar = 'n';
  strcpy(Monsters[ML5+6].monstring,"necromancer");
  strcpy(Monsters[ML5+6].corpsestr,"necromancer's sigil");
  Monsters[ML5+6].corpseweight = 10;
  Monsters[ML5+6].talkf = M_TALK_EVIL;
  Monsters[ML5+6].movef = M_MOVE_SMART;
  Monsters[ML5+6].meleef = M_MELEE_NORMAL;
  Monsters[ML5+6].strikef = M_NO_OP;
  Monsters[ML5+6].specialf = M_SP_SPELL;



  /* Shadow Spirit */
  Monsters[ML5+7].id = ML5+7;
  Monsters[ML5+7].hp = 40;
  Monsters[ML5+7].speed = 7;
  Monsters[ML5+7].hit = 18;
  Monsters[ML5+7].actions = 3;
  Monsters[ML5+7].tactics = MM_AVERAGE;
  Monsters[ML5+7].ac = 18;
  Monsters[ML5+7].dmg = 20;
  Monsters[ML5+7].sense = 5;
  Monsters[ML5+7].wakeup = 5;
  Monsters[ML5+7].level = 5;
  Monsters[ML5+7].status = HOSTILE+MOBILE+INTANGIBLE;
  Monsters[ML5+7].immunity = pow2(NORMAL_DAMAGE);
  Monsters[ML5+7].xpv = 175;
  Monsters[ML5+7].treasure = 2;
  Monsters[ML5+7].possessions = NULL;
  Monsters[ML5+7].sleep = 50;
  Monsters[ML5+7].x = -1;
  Monsters[ML5+7].y = -1;
  Monsters[ML5+7].transformid = SCROLLID+22;
  Monsters[ML5+7].corpsevalue = 10;
  Monsters[ML5+7].monchar = 'S';
  strcpy(Monsters[ML5+7].monstring,"shadow spirit");
  strcpy(Monsters[ML5+7].corpsestr,"shadowstuff");
  Monsters[ML5+7].corpseweight = 2;
  Monsters[ML5+7].talkf = M_TALK_EVIL;
  Monsters[ML5+7].movef = M_MOVE_SPIRIT;
  Monsters[ML5+7].meleef = M_MELEE_SPIRIT;
  Monsters[ML5+7].strikef = M_NO_OP;
  Monsters[ML5+7].specialf = M_SP_BLACKOUT;






  /* the fire elemental */
  Monsters[ML6+0].id = ML6+0;
  Monsters[ML6+0].hp = 125;
  Monsters[ML6+0].speed = 4;
  Monsters[ML6+0].hit = 20;
  Monsters[ML6+0].ac = 0;
  Monsters[ML6+0].actions = 4;
  Monsters[ML6+0].tactics = MM_AVERAGE;
  Monsters[ML6+0].dmg = 50;
  Monsters[ML6+0].sense = 10;
  Monsters[ML6+0].wakeup = 2;
  Monsters[ML6+0].level = 6;
  Monsters[ML6+0].status = HOSTILE+MOBILE+FLYING;
  Monsters[ML6+0].immunity = pow2(NORMAL_DAMAGE)+pow2(FLAME)+
                             pow2(SLEEP)+pow2(POISON);
  Monsters[ML6+0].xpv = 250;
  Monsters[ML6+0].treasure = 0;
  Monsters[ML6+0].possessions = NULL;
  Monsters[ML6+0].sleep = 100;
  Monsters[ML6+0].x = -1;
  Monsters[ML6+0].y = -1;
  Monsters[ML6+0].transformid = STICKID+7;
  Monsters[ML6+0].corpsevalue = 200;
  Monsters[ML6+0].monchar = 'F';
  strcpy(Monsters[ML6+0].monstring,"fire elemental");
  strcpy(Monsters[ML6+0].corpsestr,"essence of fire elemental");
  Monsters[ML6+0].corpseweight = 1;
  Monsters[ML6+0].talkf = M_TALK_SILENT;
  Monsters[ML6+0].movef = M_MOVE_NORMAL;
  Monsters[ML6+0].meleef = M_MELEE_FIRE;
  Monsters[ML6+0].strikef = M_STRIKE_FBALL;
  Monsters[ML6+0].specialf = M_NO_OP;



  /* the air elemental */
  Monsters[ML6+1].id = ML6+1;
  Monsters[ML6+1].hp = 125;
  Monsters[ML6+1].speed = 2;
  Monsters[ML6+1].hit = 25;
  Monsters[ML6+1].actions = 5;
  Monsters[ML6+1].tactics = MM_AVERAGE;
  Monsters[ML6+1].ac = 0;
  Monsters[ML6+1].dmg = 20;
  Monsters[ML6+1].sense = 10;
  Monsters[ML6+1].wakeup = 2;
  Monsters[ML6+1].level = 6;
  Monsters[ML6+1].status = HOSTILE+MOBILE+FLYING+INTANGIBLE;
  Monsters[ML6+1].immunity = pow2(NORMAL_DAMAGE)+pow2(SLEEP)+pow2(POISON);
  Monsters[ML6+1].xpv = 250;
  Monsters[ML6+1].treasure = 0;
  Monsters[ML6+1].possessions = NULL;
  Monsters[ML6+1].sleep = 100;
  Monsters[ML6+1].x = -1;
  Monsters[ML6+1].y = -1;
  Monsters[ML6+1].transformid = CLOAKID+2;
  Monsters[ML6+1].corpsevalue = 200;
  Monsters[ML6+1].monchar = 'A';
  strcpy(Monsters[ML6+1].monstring,"air elemental");
  strcpy(Monsters[ML6+1].corpsestr,"essence of air elemental");
  Monsters[ML6+1].corpseweight = 1;
  Monsters[ML6+1].talkf = M_TALK_SILENT;
  Monsters[ML6+1].movef = M_MOVE_FLUTTER;
  Monsters[ML6+1].meleef = M_MELEE_NORMAL;
  Monsters[ML6+1].strikef = M_NO_OP;
  Monsters[ML6+1].specialf = M_SP_WHIRL;

  /* the water elemental */
  Monsters[ML6+2].id = ML6+2;
  Monsters[ML6+2].hp = 100;
  Monsters[ML6+2].speed = 6;
  Monsters[ML6+2].hit = 15;
  Monsters[ML6+2].ac = 10;
  Monsters[ML6+2].actions = 4;
  Monsters[ML6+2].tactics = MM_AVERAGE;
  Monsters[ML6+2].dmg = 30;
  Monsters[ML6+2].sense = 5;
  Monsters[ML6+2].wakeup = 1;
  Monsters[ML6+2].level = 6;
  Monsters[ML6+2].status = HOSTILE+MOBILE+SWIMMING;
  Monsters[ML6+2].immunity = pow2(NORMAL_DAMAGE)+pow2(SLEEP);
  Monsters[ML6+2].xpv = 250;
  Monsters[ML6+2].treasure = 0;
  Monsters[ML6+2].possessions = NULL;
  Monsters[ML6+2].sleep = 100;
  Monsters[ML6+2].x = -1;
  Monsters[ML6+2].y = -1;
  Monsters[ML6+2].transformid = STICKID+14;
  Monsters[ML6+2].corpsevalue = 200;
  Monsters[ML6+2].monchar = 'W';
  strcpy(Monsters[ML6+2].monstring,"water elemental");
  strcpy(Monsters[ML6+2].corpsestr,"essence of water elemental");
  Monsters[ML6+2].corpseweight = 1;
  Monsters[ML6+2].talkf = M_TALK_SILENT;
  Monsters[ML6+2].movef = M_MOVE_NORMAL;
  Monsters[ML6+2].meleef = M_MELEE_NORMAL;
  Monsters[ML6+2].strikef = M_NO_OP;
  Monsters[ML6+2].specialf = M_NO_OP;

  /* the earth elemental */
  Monsters[ML6+3].id = ML6+3;
  Monsters[ML6+3].hp = 200;
  Monsters[ML6+3].speed = 15;
  Monsters[ML6+3].hit = 20;
  Monsters[ML6+3].actions = 2;
  Monsters[ML6+3].tactics = MM_FORCED+MM_AVERAGE;
  Monsters[ML6+3].hitloc = LOW;
  Monsters[ML6+3].ac = 25;
  Monsters[ML6+3].dmg = 100;
  Monsters[ML6+3].sense = 5;
  Monsters[ML6+3].wakeup = 1;
  Monsters[ML6+3].level = 6;
  Monsters[ML6+3].status = HOSTILE+MOBILE;
  Monsters[ML6+3].immunity = pow2(POISON)+pow2(SLEEP);
  Monsters[ML6+3].xpv = 250;
  Monsters[ML6+3].treasure = 0;
  Monsters[ML6+3].possessions = NULL;
  Monsters[ML6+3].sleep = 100;
  Monsters[ML6+3].x = -1;
  Monsters[ML6+3].y = -1;
  Monsters[ML6+3].transformid = CLOAKID+4;
  Monsters[ML6+3].corpsevalue = 200;
  Monsters[ML6+3].monchar = 'E';
  strcpy(Monsters[ML6+3].monstring,"earth elemental");
  strcpy(Monsters[ML6+3].corpsestr,"essence of earth elemental");
  Monsters[ML6+3].corpseweight = 1;
  Monsters[ML6+3].talkf = M_TALK_SILENT;
  Monsters[ML6+3].movef = M_MOVE_SPIRIT;
  Monsters[ML6+3].meleef = M_MELEE_NORMAL;
  Monsters[ML6+3].strikef = M_NO_OP;
  Monsters[ML6+3].specialf = M_NO_OP;

  /* the bandersnatch */
  Monsters[ML6+4].id = ML6+4;
  Monsters[ML6+4].hp = 100;
  Monsters[ML6+4].speed = 2;
  Monsters[ML6+4].hit = 20;
  Monsters[ML6+4].actions = 8;
  Monsters[ML6+4].tactics = MM_BERSERK+MM_POOR;
  Monsters[ML6+4].ac = 20;
  Monsters[ML6+4].dmg = 50;
  Monsters[ML6+4].sense = 3;
  Monsters[ML6+4].wakeup = 1;
  Monsters[ML6+4].level = 6;
  Monsters[ML6+4].status = HOSTILE+MOBILE;
  Monsters[ML6+4].immunity = pow2(SLEEP);
  Monsters[ML6+4].xpv = 250;
  Monsters[ML6+4].treasure = 1;
  Monsters[ML6+4].possessions = NULL;
  Monsters[ML6+4].sleep = 50;
  Monsters[ML6+4].x = -1;
  Monsters[ML6+4].y = -1;
  Monsters[ML6+4].transformid = -1;
  Monsters[ML6+4].corpsevalue = 100;
  Monsters[ML6+4].monchar = 'b';
  strcpy(Monsters[ML6+4].monstring,"bandersnatch");
  strcpy(Monsters[ML6+4].corpsestr,"bandersnatch hide");
  Monsters[ML6+4].corpseweight = 1000;
  Monsters[ML6+4].talkf = M_TALK_SILENT;
  Monsters[ML6+4].movef = M_MOVE_FLUTTER;
  Monsters[ML6+4].meleef = M_MELEE_NORMAL;
  Monsters[ML6+4].strikef = M_NO_OP;
  Monsters[ML6+4].specialf = M_NO_OP;




  /* the liche */
  Monsters[ML6+5].id = ML6+5;
  Monsters[ML6+5].hp = 100;
  Monsters[ML6+5].speed = 3;
  Monsters[ML6+5].hit = 30;
  Monsters[ML6+5].ac = 20;
  Monsters[ML6+5].actions = 6;
  Monsters[ML6+5].tactics = MM_GOOD;
  Monsters[ML6+5].dmg = 50;
  Monsters[ML6+5].sense = 5;
  Monsters[ML6+5].wakeup = 1;
  Monsters[ML6+5].level = 6;
  Monsters[ML6+5].status = HOSTILE+MOBILE;
  Monsters[ML6+5].immunity = pow2(SLEEP)+pow2(FEAR)+pow2(POISON)+pow2(FLAME);
  Monsters[ML6+5].xpv = 300;
  Monsters[ML6+5].treasure = 5;
  Monsters[ML6+5].possessions = NULL;
  Monsters[ML6+5].sleep = 50;
  Monsters[ML6+5].x = -1;
  Monsters[ML6+5].y = -1;
  Monsters[ML6+5].transformid = -1;
  Monsters[ML6+5].corpsevalue = 1000;
  Monsters[ML6+5].monchar = 'L';
  strcpy(Monsters[ML6+5].monstring,"lich");
  strcpy(Monsters[ML6+5].corpsestr,"lich's sceptre");
  Monsters[ML6+5].corpseweight = 100;
  Monsters[ML6+5].talkf = M_TALK_EVIL;
  Monsters[ML6+5].movef = M_MOVE_SMART;
  Monsters[ML6+5].meleef = M_MELEE_SPIRIT;
  Monsters[ML6+5].strikef = M_NO_OP;
  Monsters[ML6+5].specialf = M_SP_SPELL;




  /* the Triton */
  Monsters[ML6+6].id = ML6+6;
  Monsters[ML6+6].hp = 100;
  Monsters[ML6+6].speed = 5;
  Monsters[ML6+6].hit = 20;
  Monsters[ML6+6].ac = 20;
  Monsters[ML6+6].actions = 6;
  Monsters[ML6+6].tactics = MM_GOOD;
  Monsters[ML6+6].dmg = 30;
  Monsters[ML6+6].sense = 5;
  Monsters[ML6+6].wakeup = 3;
  Monsters[ML6+6].level = 6;
  Monsters[ML6+6].status = HOSTILE+MOBILE+SWIMMING;
  Monsters[ML6+6].immunity = pow2(SLEEP);
  Monsters[ML6+6].xpv = 300;
  Monsters[ML6+6].treasure = 3;
  Monsters[ML6+6].possessions = NULL;
  Monsters[ML6+6].sleep = 30;
  Monsters[ML6+6].x = -1;
  Monsters[ML6+6].y = -1;
  Monsters[ML6+6].transformid = -1;
  Monsters[ML6+6].corpsevalue = 500;
  Monsters[ML6+6].monchar = 'T';
  strcpy(Monsters[ML6+6].monstring,"triton");
  strcpy(Monsters[ML6+6].corpsestr,"triton's pearl");
  Monsters[ML6+6].corpseweight = 10;
  Monsters[ML6+6].talkf = M_TALK_EVIL;
  Monsters[ML6+6].movef = M_MOVE_SMART;
  Monsters[ML6+6].meleef = M_MELEE_NORMAL;
  Monsters[ML6+6].strikef = M_NO_OP;
  Monsters[ML6+6].specialf = M_SP_SPELL;



  /* the master thief */
  Monsters[ML6+7].id = ML6+7;
  Monsters[ML6+7].hp = 100;
  Monsters[ML6+7].speed = 3;
  Monsters[ML6+7].hit = 0;
  Monsters[ML6+7].ac = 20;
  Monsters[ML6+7].actions = 3;
  Monsters[ML6+7].tactics = MM_TIMID+MM_GOOD;
  Monsters[ML6+7].dmg = 0;
  Monsters[ML6+7].sense = 10;
  Monsters[ML6+7].wakeup = 3;
  Monsters[ML6+7].level = 6;
  Monsters[ML6+7].status = HOSTILE+GREEDY+SWIMMING;
  Monsters[ML6+7].immunity = 0;
  Monsters[ML6+7].xpv = 200;
  Monsters[ML6+7].treasure = 5;
  Monsters[ML6+7].possessions = NULL;
  Monsters[ML6+7].sleep = 0;
  Monsters[ML6+7].x = -1;
  Monsters[ML6+7].y = -1;
  Monsters[ML6+7].transformid = -1;
  Monsters[ML6+7].corpsevalue = 0;
  if (random_range(2)) {
    Monsters[ML6+7].monchar = 'm';
    strcpy(Monsters[ML6+7].monstring,"man");
  }
  else {
    Monsters[ML6+7].monchar = 'w';
    strcpy(Monsters[ML6+7].monstring,"woman");
  }
  strcpy(Monsters[ML6+7].corpsestr,"dead master thief");
  Monsters[ML6+7].corpseweight = 10;
  Monsters[ML6+7].talkf = M_TALK_THIEF;
  Monsters[ML6+7].movef = M_MOVE_SMART;
  Monsters[ML6+7].meleef = M_SP_THIEF;
  Monsters[ML6+7].strikef = M_NO_OP;
  Monsters[ML6+7].specialf = M_SP_THIEF;



  /* the behemoth */
  Monsters[ML7+0].id = ML7+0;
  Monsters[ML7+0].hp = 300;
  Monsters[ML7+0].speed = 15;
  Monsters[ML7+0].hit = 30;
  Monsters[ML7+0].actions = 1;
  Monsters[ML7+0].tactics = MM_BERSERK+MM_POOR;
  Monsters[ML7+0].hitloc = HIGH;
  Monsters[ML7+0].ac = 5;
  Monsters[ML7+0].dmg = 120;
  Monsters[ML7+0].sense = 3;
  Monsters[ML7+0].wakeup = 3;
  Monsters[ML7+0].level = 7;
  Monsters[ML7+0].status = HOSTILE+MOBILE+SWIMMING;
  Monsters[ML7+0].immunity = pow2(ELECTRICITY);
  Monsters[ML7+0].xpv = 400;
  Monsters[ML7+0].treasure = 0;
  Monsters[ML7+0].possessions = NULL;
  Monsters[ML7+0].sleep = 100;
  Monsters[ML7+0].x = -1;
  Monsters[ML7+0].y = -1;
  Monsters[ML7+0].transformid = RINGID+2;
  Monsters[ML7+0].corpsevalue = 50;
  Monsters[ML7+0].monchar = 'B';
  strcpy(Monsters[ML7+0].monstring,"behemoth");
  strcpy(Monsters[ML7+0].corpsestr,"behemoth's gonads");
  Monsters[ML7+0].corpseweight = 500;
  Monsters[ML7+0].talkf = M_TALK_STUPID;
  Monsters[ML7+0].movef = M_MOVE_NORMAL;
  Monsters[ML7+0].meleef = M_MELEE_NORMAL;
  Monsters[ML7+0].strikef = M_NO_OP;
  Monsters[ML7+0].specialf = M_SP_HUGE;


  /* Nazgul */
  Monsters[ML7+1].id = ML7+1;
  Monsters[ML7+1].hp = 250;
  Monsters[ML7+1].speed = 5;
  Monsters[ML7+1].hit = 30;
  Monsters[ML7+1].actions = 9;
  Monsters[ML7+1].tactics = MM_GOOD;
  Monsters[ML7+1].ac = 20;
  Monsters[ML7+1].dmg = 20;
  Monsters[ML7+1].sense = 10;
  Monsters[ML7+1].wakeup = 5;
  Monsters[ML7+1].level = 7;
  Monsters[ML7+1].status = HOSTILE+MOBILE+FLYING;
  Monsters[ML7+1].immunity = pow2(SLEEP)+pow2(POISON)+pow2(FEAR)+pow2(COLD);
  Monsters[ML7+1].xpv = 500;
  Monsters[ML7+1].treasure = 0;
  Monsters[ML7+1].possessions = NULL;
  Monsters[ML7+1].sleep = 0;
  Monsters[ML7+1].x = -1;
  Monsters[ML7+1].y = -1;
  Monsters[ML7+1].transformid = -1;
  Monsters[ML7+1].corpsevalue = 2000;
  Monsters[ML7+1].monchar = 'N';
  strcpy(Monsters[ML7+1].monstring,"nazgul");
  strcpy(Monsters[ML7+1].corpsestr,"nazgul's crown");
  Monsters[ML7+1].corpseweight = 100;
  Monsters[ML7+1].talkf = M_TALK_EVIL;
  Monsters[ML7+1].movef = M_MOVE_SPIRIT;
  Monsters[ML7+1].meleef = M_MELEE_SPIRIT;
  Monsters[ML7+1].strikef = M_NO_OP;
  Monsters[ML7+1].specialf = M_SP_SPELL;


  /* Unicorn */
  Monsters[ML7+2].id = ML7+2;
  Monsters[ML7+2].hp = 100;
  Monsters[ML7+2].speed = 2;
  Monsters[ML7+2].hit = 100;
  Monsters[ML7+2].actions = 10;
  Monsters[ML7+2].tactics = MM_GOOD+MM_FORCED;
  Monsters[ML7+2].hitloc = CENTER;
  Monsters[ML7+2].ac = 20;
  Monsters[ML7+2].dmg = 50;
  Monsters[ML7+2].sense = 10;
  Monsters[ML7+2].wakeup = 3;
  Monsters[ML7+2].level = 7;
  Monsters[ML7+2].status = 0+MOBILE+SWIMMING;
  Monsters[ML7+2].immunity = pow2(POISON)+pow2(FEAR);
  Monsters[ML7+2].xpv = 400;
  Monsters[ML7+2].treasure = 0;
  Monsters[ML7+2].possessions = NULL;
  Monsters[ML7+2].sleep = 50;
  Monsters[ML7+2].x = -1;
  Monsters[ML7+2].y = -1;
  Monsters[ML7+2].transformid = RINGID+6;
  Monsters[ML7+2].corpsevalue = 50;
  Monsters[ML7+2].monchar = 'U';
  strcpy(Monsters[ML7+2].monstring,"unicorn");
  strcpy(Monsters[ML7+2].corpsestr,"unicorn's horn");
  Monsters[ML7+2].corpseweight = 100;
  Monsters[ML7+2].talkf = M_TALK_SILENT;
  Monsters[ML7+2].movef = M_MOVE_NORMAL;
  Monsters[ML7+2].meleef = M_MELEE_NORMAL;
  Monsters[ML7+2].strikef = M_NO_OP;
  Monsters[ML7+2].specialf = M_MOVE_SCAREDY;


  /* Rodent of Unusual Size */
  Monsters[ML7+3].id = ML7+3;
  Monsters[ML7+3].hp = 150;
  Monsters[ML7+3].speed = 3;
  Monsters[ML7+3].hit = 20;
  Monsters[ML7+3].actions = 3;
  Monsters[ML7+3].tactics = MM_BERSERK+MM_FORCED;
  Monsters[ML7+3].hitloc = LOW;
  Monsters[ML7+3].ac = 20;
  Monsters[ML7+3].dmg = 50;
  Monsters[ML7+3].sense = 10;
  Monsters[ML7+3].wakeup = 3;
  Monsters[ML7+3].level = 7;
  Monsters[ML7+3].status = MOBILE+HOSTILE+SWIMMING+HUNGRY+M_INVISIBLE;
  Monsters[ML7+3].immunity = pow2(FEAR);
  Monsters[ML7+3].xpv = 300;
  Monsters[ML7+3].treasure = 0;
  Monsters[ML7+3].possessions = NULL;
  Monsters[ML7+3].sleep = 50;
  Monsters[ML7+3].x = -1;
  Monsters[ML7+3].y = -1;
  Monsters[ML7+3].transformid = -1;
  Monsters[ML7+3].corpsevalue = 0;
  Monsters[ML7+3].monchar = 'r';
  strcpy(Monsters[ML7+3].monstring,"rodent of unusual size");
  strcpy(Monsters[ML7+3].corpsestr,"dead R.O.U.S.");
  Monsters[ML7+3].corpseweight = 100;
  Monsters[ML7+3].talkf = M_NO_OP;
  Monsters[ML7+3].movef = M_MOVE_NORMAL;
  Monsters[ML7+3].meleef = M_MELEE_DISEASE;
  Monsters[ML7+3].strikef = M_NO_OP;
  Monsters[ML7+3].specialf = M_SP_SURPRISE;


  /* Illusory Fiend */
  Monsters[ML7+4].id = ML7+4;
  Monsters[ML7+4].hp = 100;
  Monsters[ML7+4].speed = 5;
  Monsters[ML7+4].hit = 20;
  Monsters[ML7+4].actions = 4;
  Monsters[ML7+4].tactics = MM_FORCED+MM_BERSERK;
  Monsters[ML7+4].hitloc = CENTER;
  Monsters[ML7+4].ac = 25;
  Monsters[ML7+4].dmg = 30;
  Monsters[ML7+4].sense = 10;
  Monsters[ML7+4].wakeup = 2;
  Monsters[ML7+4].level = 7;
  Monsters[ML7+4].status = MOBILE+HOSTILE+GREEDY;
  Monsters[ML7+4].immunity = pow2(OTHER_MAGIC);
  Monsters[ML7+4].xpv = 300;
  Monsters[ML7+4].treasure = 3;
  Monsters[ML7+4].possessions = NULL;
  Monsters[ML7+4].sleep = 50;
  Monsters[ML7+4].x = -1;
  Monsters[ML7+4].y = -1;
  Monsters[ML7+4].transformid = -1;
  Monsters[ML7+4].corpsevalue = 0;
  Monsters[ML7+4].monchar = 'F';
  strcpy(Monsters[ML7+4].monstring,"illusory fiend");
  strcpy(Monsters[ML7+4].corpsestr,"not much of anything");
  Monsters[ML7+4].corpseweight = 0;
  Monsters[ML7+4].talkf = M_NO_OP;
  Monsters[ML7+4].movef = M_MOVE_NORMAL;
  Monsters[ML7+4].meleef = M_MELEE_NORMAL;
  Monsters[ML7+4].strikef = M_NO_OP;
  Monsters[ML7+4].specialf = M_SP_ILLUSION;


  /* The Mutant */
  /* A random new kind of creature each game */
  Monsters[ML7+5].id = ML7+5;
  Monsters[ML7+5].hp = random_range(200);
  Monsters[ML7+5].speed = (random_range(3) * random_range(7))+1;
  Monsters[ML7+5].hit = 10+random_range(40);
  Monsters[ML7+5].actions =  random_range(8);
  switch(random_range(3)) {
  case 0: Monsters[ML7+5].tactics = MM_POOR; break;
  case 1: Monsters[ML7+5].tactics = MM_AVERAGE; break;
  case 2: Monsters[ML7+5].tactics = MM_GOOD; break;
  }
  if (random_range(3) == 2) Monsters[ML7+5].tactics += MM_FORCED;
  if (random_range(2) == 1) Monsters[ML7+5].tactics += MM_BERSERK;
  else if (random_range(2) == 1) Monsters[ML7+5].tactics += MM_TIMID;
  Monsters[ML7+5].hitloc = random_range(4);
  Monsters[ML7+5].ac = random_range(30)+5;
  Monsters[ML7+5].dmg = random_range(30)+5;
  Monsters[ML7+5].sense = random_range(5)+3;
  Monsters[ML7+5].wakeup = random_range(5);
  Monsters[ML7+5].level = 7;
  Monsters[ML7+5].status = MOBILE+HOSTILE;
  Monsters[ML7+5].immunity = 
    pow2(random_range(NUMIMMUNITIES-1)+1) +
      pow2(random_range(NUMIMMUNITIES-1)+1);
  Monsters[ML7+5].xpv = 350;
  Monsters[ML7+5].treasure = random_range(5);
  Monsters[ML7+5].possessions = NULL;
  Monsters[ML7+5].sleep = random_range(101);
  Monsters[ML7+5].x = -1;
  Monsters[ML7+5].y = -1;
  Monsters[ML7+5].transformid = -1;
  Monsters[ML7+5].corpsevalue = 0;
  Monsters[ML7+5].monchar = 'M';
  strcpy(Monsters[ML7+5].monstring,"mutant");
  strcpy(Monsters[ML7+5].corpsestr,"irradiated germ plasm");
  Monsters[ML7+5].corpseweight = 0;

  /* hardwired constants, yikes */
  Monsters[ML7+5].talkf = 101+random_range(12); 
  if (random_range(3)==1)
    Monsters[ML7+5].talkf = M_NO_OP;
  Monsters[ML7+5].movef = 501+random_range(9);
  if (random_range(3)==1)
    Monsters[ML7+5].movef = M_NO_OP;
  Monsters[ML7+5].meleef = 401+random_range(12);
  if (random_range(3)==1)
    Monsters[ML7+5].meleef = M_NO_OP;
  Monsters[ML7+5].strikef = 301+random_range(6);
  if (random_range(3)==1)
    Monsters[ML7+5].strikef = M_NO_OP;
  Monsters[ML7+5].specialf = 201+random_range(16);
  if (random_range(3)==1)
    Monsters[ML7+5].specialf = M_NO_OP;

  
  /* ... */
}
