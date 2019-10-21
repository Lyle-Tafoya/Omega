/* omega copyright (c) 1987 by Laurence Raphael Brothers */
/* oinitmon8to10.c */
/* monster levels 8 to 10 */

#include <strings.h>
#include "oglob.h"

/* from oitem */
extern void make_weapon(), make_artifact(), make_thing();

/* from omon */
extern void m_pickup();

void initmon8to10()
{
  pob tob;

  /* the good fairy */
  Monsters[ML8+0].id = ML8+0;
  Monsters[ML8+0].hp = 1;
  Monsters[ML8+0].speed = 3;
  Monsters[ML8+0].hit = 0;
  Monsters[ML8+0].actions = 0;
  Monsters[ML8+0].tactics = MM_GOOD+MM_TIMID;
  Monsters[ML8+0].ac = 100;
  Monsters[ML8+0].dmg = 0;
  Monsters[ML8+0].sense = 6;
  Monsters[ML8+0].wakeup = 2;
  Monsters[ML8+0].level = 8;
  Monsters[ML8+0].status = MOBILE+FLYING;
  Monsters[ML8+0].immunity = pow2(FLAME)+pow2(COLD)+pow2(SLEEP);
  Monsters[ML8+0].sleep = 0;
  Monsters[ML8+0].treasure = 0;
  Monsters[ML8+0].possessions = NULL;
  Monsters[ML8+0].xpv = 50;
  Monsters[ML8+0].x = -1;
  Monsters[ML8+0].y = -1;
  Monsters[ML8+0].transformid = -1;
  Monsters[ML8+0].corpsevalue = 0;
  Monsters[ML8+0].monchar = 'f';
  strcpy(Monsters[ML8+0].monstring,"fairy");
  strcpy(Monsters[ML8+0].corpsestr,"good fairy dust");
  Monsters[ML8+0].corpseweight = 100;
  Monsters[ML8+0].talkf = M_TALK_GF;
  Monsters[ML8+0].movef = M_MOVE_RANDOM;
  Monsters[ML8+0].meleef = M_NO_OP;
  Monsters[ML8+0].strikef = M_NO_OP;
  Monsters[ML8+0].specialf = M_NO_OP;

  /* the evil fairy */
  Monsters[ML8+1].id = ML8+1;
  Monsters[ML8+1].hp = 100;
  Monsters[ML8+1].speed = 3;
  Monsters[ML8+1].hit = 50;
  Monsters[ML8+1].actions = 6;
  Monsters[ML8+1].tactics = MM_GOOD+MM_BERSERK;
  Monsters[ML8+1].hitloc = LOW;
  Monsters[ML8+1].ac = 50;
  Monsters[ML8+1].dmg = 20;
  Monsters[ML8+1].sense = 10;
  Monsters[ML8+1].wakeup = 10;
  Monsters[ML8+1].level = 8;
  Monsters[ML8+1].status = HOSTILE+MOBILE+FLYING;
  Monsters[ML8+1].immunity = pow2(FLAME)+pow2(COLD)+pow2(SLEEP);
  Monsters[ML8+1].xpv = 500;
  Monsters[ML8+1].treasure = 0;
  Monsters[ML8+1].possessions = NULL;
  Monsters[ML8+1].sleep = 0;
  Monsters[ML8+1].x = -1;
  Monsters[ML8+1].y = -1;
  Monsters[ML8+1].transformid = -1;
  Monsters[ML8+1].corpsevalue = 0;
  Monsters[ML8+1].monchar = 'f';
  strcpy(Monsters[ML8+1].monstring,"fairy");
  strcpy(Monsters[ML8+1].corpsestr,"evil fairy dust");
  Monsters[ML8+1].corpseweight = 3;
  Monsters[ML8+1].talkf = M_TALK_EF;
  Monsters[ML8+1].movef = M_MOVE_RANDOM;
  Monsters[ML8+1].meleef = M_NO_OP;
  Monsters[ML8+1].strikef = M_NO_OP;
  Monsters[ML8+1].specialf = M_NO_OP;

  /* Automaton Major */
  Monsters[ML8+2].id = ML8+2;
  Monsters[ML8+2].hp = 500;
  Monsters[ML8+2].speed = 3;
  Monsters[ML8+2].hit = 30;
  Monsters[ML8+2].actions = 6;
  Monsters[ML8+2].tactics = MM_AVERAGE;
  Monsters[ML8+2].ac = 50;
  Monsters[ML8+2].dmg = 100;
  Monsters[ML8+2].sense = 5;
  Monsters[ML8+2].wakeup = 2;
  Monsters[ML8+2].level = 8;
  Monsters[ML8+2].status = HOSTILE+MOBILE;
  Monsters[ML8+2].immunity = pow2(SLEEP)+pow2(POISON)+pow2(FEAR)+
                             pow2(ELECTRICITY);
  Monsters[ML8+2].xpv = 500;
  Monsters[ML8+2].treasure = 0;
  Monsters[ML8+2].possessions = NULL;
  Monsters[ML8+2].sleep = 100;
  Monsters[ML8+2].x = -1;
  Monsters[ML8+2].y = -1;
  Monsters[ML8+2].transformid = WEAPONID+23;
  Monsters[ML8+2].corpsevalue = 500;
  Monsters[ML8+2].monchar = 'A';
  strcpy(Monsters[ML8+2].monstring,"automatum major");
  strcpy(Monsters[ML8+2].corpsestr,"automatum major components");
  Monsters[ML8+2].corpseweight = 1000;
  Monsters[ML8+2].talkf = M_TALK_ROBOT;
  Monsters[ML8+2].movef = M_MOVE_SMART;
  Monsters[ML8+2].meleef = M_MELEE_NORMAL;
  Monsters[ML8+2].strikef = M_STRIKE_LBALL;
  Monsters[ML8+2].specialf = M_SP_POISON_CLOUD;

  /* dragon */
  Monsters[ML8+3].id = ML8+3;
  Monsters[ML8+3].hp = 500;
  Monsters[ML8+3].speed = 5;
  Monsters[ML8+3].hit = 50;
  Monsters[ML8+3].actions = 7;
  Monsters[ML8+3].tactics = MM_FORCED+MM_GOOD;
  Monsters[ML8+3].hitloc = HIGH;
  Monsters[ML8+3].ac = 30;
  Monsters[ML8+3].dmg = 75;
  Monsters[ML8+3].sense = 20;
  Monsters[ML8+3].wakeup = 1;
  Monsters[ML8+3].level = 8;
  Monsters[ML8+3].status = HOSTILE+HUNGRY+GREEDY+MOBILE+FLYING;
  Monsters[ML8+3].immunity = pow2(FLAME)+pow2(SLEEP)+pow2(POISON);
  Monsters[ML8+3].xpv = 750;
  Monsters[ML8+3].treasure = 7;
  Monsters[ML8+3].possessions = NULL;
  Monsters[ML8+3].sleep = 100;
  Monsters[ML8+3].x = -1;
  Monsters[ML8+3].y = -1;
  Monsters[ML8+3].transformid = ARMORID+12;
  Monsters[ML8+3].corpsevalue = 1000;
  Monsters[ML8+3].monchar = 'D';
  strcpy(Monsters[ML8+3].monstring,"dragon");
  strcpy(Monsters[ML8+3].corpsestr,"dragon scales");
  Monsters[ML8+3].corpseweight = 500;
  Monsters[ML8+3].talkf = M_TALK_GREEDY;
  Monsters[ML8+3].movef = M_MOVE_NORMAL;
  Monsters[ML8+3].meleef = M_MELEE_DRAGON;
  Monsters[ML8+3].strikef = M_STRIKE_FBOLT;
  Monsters[ML8+3].specialf = M_NO_OP;


  /* jabberwock */
  Monsters[ML8+4].id = ML8+4;
  Monsters[ML8+4].hp = 500;
  Monsters[ML8+4].speed = 2;
  Monsters[ML8+4].hit = 40;
  Monsters[ML8+4].actions = 6;
  Monsters[ML8+4].tactics = MM_GOOD;
  Monsters[ML8+4].ac = 25;
  Monsters[ML8+4].dmg = 100;
  Monsters[ML8+4].sense = 5;
  Monsters[ML8+4].wakeup = 1;
  Monsters[ML8+4].level = 8;
  Monsters[ML8+4].status = HOSTILE+HUNGRY+MOBILE;
  Monsters[ML8+4].immunity = pow2(SLEEP)+pow2(POISON);
  Monsters[ML8+4].xpv = 600;
  Monsters[ML8+4].treasure = 2;
  Monsters[ML8+4].possessions = NULL;
  Monsters[ML8+4].sleep = 75;
  Monsters[ML8+4].x = -1;
  Monsters[ML8+4].y = -1;
  Monsters[ML8+4].transformid = -1;
  Monsters[ML8+4].corpsevalue = 1000;
  Monsters[ML8+4].monchar = 'J';
  strcpy(Monsters[ML8+4].monstring,"jabberwock");
  strcpy(Monsters[ML8+4].corpsestr,"jabberwock's head");
  Monsters[ML8+4].corpseweight = 500;
  Monsters[ML8+4].talkf = M_TALK_BURBLE;
  Monsters[ML8+4].movef = M_MOVE_FLUTTER;
  Monsters[ML8+4].meleef = M_MELEE_NORMAL;
  Monsters[ML8+4].strikef = M_NO_OP;
  Monsters[ML8+4].specialf = M_NO_OP;

  /* frost demon lord */
  Monsters[ML8+5].id = ML8+5;
  Monsters[ML8+5].hp = 350;
  Monsters[ML8+5].speed = 2;
  Monsters[ML8+5].hit = 40;
  Monsters[ML8+5].actions = 8;
  Monsters[ML8+5].tactics = MM_GOOD;
  Monsters[ML8+5].ac = 40;
  Monsters[ML8+5].dmg = 40;
  Monsters[ML8+5].sense = 10;
  Monsters[ML8+5].wakeup = 10;
  Monsters[ML8+5].level = 8;
  Monsters[ML8+5].status = HOSTILE+HUNGRY+MOBILE;
  Monsters[ML8+5].immunity = pow2(POISON)+pow2(SLEEP)+pow2(FEAR)+pow2(COLD);
  Monsters[ML8+5].xpv = 700;
  Monsters[ML8+5].treasure = 5;
  Monsters[ML8+5].possessions = NULL;
  Monsters[ML8+5].sleep = 25;
  Monsters[ML8+5].x = -1;
  Monsters[ML8+5].y = -1;
  Monsters[ML8+5].transformid = WEAPONID+24;
  Monsters[ML8+5].corpsevalue = 500;
  Monsters[ML8+5].monchar = 'F';
  strcpy(Monsters[ML8+5].monstring,"frost demon lord");
  strcpy(Monsters[ML8+5].corpsestr,"frost demon lord's sigil");
  Monsters[ML8+5].corpseweight = 100;
  Monsters[ML8+5].talkf = M_TALK_EVIL;
  Monsters[ML8+5].movef = M_MOVE_SMART;
  Monsters[ML8+5].meleef = M_MELEE_COLD;
  Monsters[ML8+5].strikef = M_STRIKE_SNOWBALL;
  Monsters[ML8+5].specialf = M_SP_DEMON;


  /* tigershark */
  Monsters[ML8+6].id = ML8+6;
  Monsters[ML8+6].hp = 350;
  Monsters[ML8+6].speed = 2;
  Monsters[ML8+6].hit = 30;
  Monsters[ML8+6].actions = 4;
  Monsters[ML8+6].tactics = MM_FORCED+MM_BERSERK;
  Monsters[ML8+6].hitloc = LOW;
  Monsters[ML8+6].ac = 30;
  Monsters[ML8+6].dmg = 40;
  Monsters[ML8+6].sense = 10;
  Monsters[ML8+6].wakeup = 4;
  Monsters[ML8+6].level = 8;
  Monsters[ML8+6].status = HOSTILE+HUNGRY+MOBILE+SWIMMING;
  Monsters[ML8+6].immunity = pow2(POISON)+pow2(SLEEP)+pow2(FEAR)+pow2(COLD);
  Monsters[ML8+6].xpv = 700;
  Monsters[ML8+6].treasure = 0;
  Monsters[ML8+6].possessions = NULL;
  Monsters[ML8+6].sleep = 25;
  Monsters[ML8+6].x = -1;
  Monsters[ML8+6].y = -1;
  Monsters[ML8+6].transformid = -1;
  Monsters[ML8+6].corpsevalue = 100;
  Monsters[ML8+6].monchar = 'F';
  strcpy(Monsters[ML8+6].monstring,"tigershark");
  strcpy(Monsters[ML8+6].corpsestr,"tigershark pelt");
  Monsters[ML8+6].corpseweight = 100;
  Monsters[ML8+6].talkf = M_NO_OP;
  Monsters[ML8+6].movef = M_MOVE_NORMAL;
  Monsters[ML8+6].meleef = M_MELEE_NORMAL;
  Monsters[ML8+6].strikef = M_NO_OP;
  Monsters[ML8+6].specialf = M_NO_OP;


  /* Master Assassin */
  Monsters[ML8+7].id = ML8+7;
  Monsters[ML8+7].hp = 250;
  Monsters[ML8+7].speed = 3;
  Monsters[ML8+7].hit = 30;
  Monsters[ML8+7].actions = 6;
  Monsters[ML8+7].tactics = MM_GOOD;
  Monsters[ML8+7].ac = 20;
  Monsters[ML8+7].dmg = 50;
  Monsters[ML8+7].sense = 10;
  Monsters[ML8+7].wakeup = 4;
  Monsters[ML8+7].level = 8;
  Monsters[ML8+7].status = HOSTILE+MOBILE+M_INVISIBLE;
  Monsters[ML8+7].immunity = pow2(POISON)+pow2(SLEEP);
  Monsters[ML8+7].xpv = 800;
  Monsters[ML8+7].treasure = 2;
  Monsters[ML8+7].possessions = NULL;
  Monsters[ML8+7].sleep = 0;
  Monsters[ML8+7].x = -1;
  Monsters[ML8+7].y = -1;
  Monsters[ML8+7].transformid = -1;
  Monsters[ML8+7].corpsevalue = 100;
  if (random_range(2)) {
    Monsters[ML8+7].monchar = 'm';
    strcpy(Monsters[ML8+7].monstring,"man");
  }
  else {
    Monsters[ML8+7].monchar = 'w';
    strcpy(Monsters[ML8+7].monstring,"woman");
  }
  strcpy(Monsters[ML8+7].corpsestr,"Master Assassin's corpse");
  Monsters[ML8+7].corpseweight = 100;
  Monsters[ML8+7].talkf = M_TALK_ASSASSIN;
  Monsters[ML8+7].movef = M_MOVE_NORMAL;
  Monsters[ML8+7].meleef = M_MELEE_POISON;
  Monsters[ML8+7].strikef = M_STRIKE_MISSILE;
  Monsters[ML8+7].specialf = M_SP_SURPRISE;


  /* Shadow Slayer */
  Monsters[ML8+8].id = ML8+8;
  Monsters[ML8+8].hp = 250;
  Monsters[ML8+8].speed = 3;
  Monsters[ML8+8].hit = 30;
  Monsters[ML8+8].actions = 4;
  Monsters[ML8+8].tactics = MM_AVERAGE;
  Monsters[ML8+8].ac = 40;
  Monsters[ML8+8].dmg = 30;
  Monsters[ML8+8].sense = 10;
  Monsters[ML8+8].wakeup = 4;
  Monsters[ML8+8].level = 8;
  Monsters[ML8+8].status = HOSTILE+MOBILE+INTANGIBLE+M_INVISIBLE;
  Monsters[ML8+8].immunity = pow2(NORMAL_DAMAGE)+pow2(POISON)+pow2(SLEEP);
  Monsters[ML8+8].xpv = 500;
  Monsters[ML8+8].treasure = 3;
  Monsters[ML8+8].possessions = NULL;
  Monsters[ML8+8].sleep = 0;
  Monsters[ML8+8].x = -1;
  Monsters[ML8+8].y = -1;
  Monsters[ML8+8].transformid = RINGID+0;
  Monsters[ML8+8].corpsevalue = 75;
  Monsters[ML8+7].monchar = 'S';
  strcpy(Monsters[ML8+8].monstring,"shadow slayer");
  strcpy(Monsters[ML8+8].corpsestr,"shadow matrix");
  Monsters[ML8+8].corpseweight = 10;
  Monsters[ML8+8].talkf = M_TALK_EVIL;
  Monsters[ML8+8].movef = M_MOVE_SPIRIT;
  Monsters[ML8+8].meleef = M_MELEE_POISON;
  Monsters[ML8+8].strikef = M_SP_SPELL;
  Monsters[ML8+8].specialf = M_SP_BLACKOUT;

  /* Militant Priest */
  Monsters[ML8+9].id = ML8+9;
  Monsters[ML8+9].hp = 250;
  Monsters[ML8+9].speed = 6;
  Monsters[ML8+9].hit = 25;
  Monsters[ML8+9].actions = 8;
  Monsters[ML8+9].tactics = MM_GOOD;
  Monsters[ML8+9].ac = 20;
  Monsters[ML8+9].dmg = 30;
  Monsters[ML8+9].sense = 10;
  Monsters[ML8+9].wakeup = 4;
  Monsters[ML8+9].level = 8;
  Monsters[ML8+9].status = HOSTILE+MOBILE;
  Monsters[ML8+9].immunity = pow2(SLEEP);
  Monsters[ML8+9].xpv = 700;
  Monsters[ML8+9].treasure = 3;
  Monsters[ML8+9].possessions = NULL;
  Monsters[ML8+9].sleep = 0;
  Monsters[ML8+9].x = -1;
  Monsters[ML8+9].y = -1;
  Monsters[ML8+9].transformid = -1;
  Monsters[ML8+9].monchar = 'M';
  Monsters[ML8+9].corpsevalue = 75;
  strcpy(Monsters[ML8+9].monstring,"militant priest");
  strcpy(Monsters[ML8+9].corpsestr,"dead militant priest");
  Monsters[ML8+9].corpseweight = 10;
  Monsters[ML8+9].talkf = M_TALK_SILENT;
  Monsters[ML8+9].movef = M_MOVE_SMART;
  Monsters[ML8+9].meleef = M_MELEE_NORMAL;
  Monsters[ML8+9].strikef = M_STRIKE_MISSILE;
  Monsters[ML8+9].specialf = M_SP_SPELL;


  /* Coma Beast */
  Monsters[ML8+10].id = ML8+10;
  Monsters[ML8+10].hp = 150;
  Monsters[ML8+10].speed = 5;
  Monsters[ML8+10].hit = 25;
  Monsters[ML8+10].actions = 2;
  Monsters[ML8+10].tactics = MM_POOR;
  Monsters[ML8+10].ac = 40;
  Monsters[ML8+10].dmg = 30;
  Monsters[ML8+10].sense = 10;
  Monsters[ML8+10].wakeup = 4;
  Monsters[ML8+10].level = 8;
  Monsters[ML8+10].status = HOSTILE+MOBILE+M_INVISIBLE;
  Monsters[ML8+10].immunity = pow2(SLEEP);
  Monsters[ML8+10].xpv = 500;
  Monsters[ML8+10].treasure = 3;
  Monsters[ML8+10].possessions = NULL;
  Monsters[ML8+10].sleep = 0;
  Monsters[ML8+10].x = -1;
  Monsters[ML8+10].y = -1;
  Monsters[ML8+10].transformid = -1;
  Monsters[ML8+10].corpsevalue = 75;
  Monsters[ML8+10].monchar = 'C';
  strcpy(Monsters[ML8+10].monstring,"coma beast");
  strcpy(Monsters[ML8+10].corpsestr,"dead coma beast");
  Monsters[ML8+10].corpseweight = 10;
  Monsters[ML8+10].talkf = M_TALK_SILENT;
  Monsters[ML8+10].movef = M_MOVE_SMART;
  Monsters[ML8+10].meleef = M_MELEE_SLEEP;
  Monsters[ML8+10].strikef = M_NO_OP;
  Monsters[ML8+10].specialf = M_NO_OP;



  /* the jotun */
  Monsters[ML9+0].id = ML9+0;
  Monsters[ML9+0].hp = 750;
  Monsters[ML9+0].speed = 8;
  Monsters[ML9+0].hit = 80;
  Monsters[ML9+0].actions = 3;
  Monsters[ML9+0].tactics = MM_GOOD+MM_FORCED;
  Monsters[ML9+0].hitloc = HIGH;
  Monsters[ML9+0].ac = 50;
  Monsters[ML9+0].dmg = 200;
  Monsters[ML9+0].sense = 10;
  Monsters[ML9+0].wakeup = 2;
  Monsters[ML9+0].level = 9;
  Monsters[ML9+0].status = HOSTILE+HUNGRY+GREEDY+MOBILE;
  Monsters[ML9+0].immunity = pow2(COLD)+pow2(FEAR);
  Monsters[ML9+0].xpv = 1000;
  Monsters[ML9+0].treasure = 5;
  Monsters[ML9+0].possessions = NULL;
  Monsters[ML9+0].sleep = 90;
  Monsters[ML9+0].x = -1;
  Monsters[ML9+0].y = -1;
  Monsters[ML9+0].transformid = -1;
  Monsters[ML9+0].corpsevalue = 2000;
  Monsters[ML9+0].monchar = 'J';
  strcpy(Monsters[ML9+0].monstring,"jotun");
  strcpy(Monsters[ML9+0].corpsestr,"jotun's head");
  Monsters[ML9+0].corpseweight = 500;
  Monsters[ML9+0].talkf = M_TALK_GREEDY;
  Monsters[ML9+0].movef = M_MOVE_NORMAL;
  Monsters[ML9+0].meleef = M_MELEE_NORMAL;
  Monsters[ML9+0].strikef = M_STRIKE_MISSILE;
  Monsters[ML9+0].specialf = M_SP_HUGE;

  /* Invisible Slayer */
  Monsters[ML9+1].id = ML9+1;
  Monsters[ML9+1].hp = 1000;
  Monsters[ML9+1].speed = 4;
  Monsters[ML9+1].hit = 50;
  Monsters[ML9+1].ac = 30;
  Monsters[ML9+1].actions = 6;
  Monsters[ML9+1].tactics = MM_GOOD;
  Monsters[ML9+1].dmg = 40;
  Monsters[ML9+1].sense = 20;
  Monsters[ML9+1].wakeup = 2;
  Monsters[ML9+1].level = 9;
  Monsters[ML9+1].status = HOSTILE+MOBILE+FLYING+M_INVISIBLE;
  Monsters[ML9+1].immunity = pow2(ELECTRICITY);
  Monsters[ML9+1].xpv = 750;
  Monsters[ML9+1].treasure = 0;
  Monsters[ML9+1].possessions = NULL;
  Monsters[ML9+1].sleep = 40;
  Monsters[ML9+1].x = -1;
  Monsters[ML9+1].y = -1;
  Monsters[ML9+1].transformid = CLOAKID+2;
  Monsters[ML9+1].corpsevalue = 200;
  Monsters[ML9+1].monchar = 'i';
  strcpy(Monsters[ML9+1].monstring,"invisible slayer");
  strcpy(Monsters[ML9+1].corpsestr,"dead slayer");
  Monsters[ML9+1].corpseweight = 100;
  Monsters[ML9+1].talkf = M_TALK_SILENT;
  Monsters[ML9+1].movef = M_MOVE_FLUTTER;
  Monsters[ML9+1].meleef = M_MELEE_NORMAL;
  Monsters[ML9+1].strikef = M_NO_OP;
  Monsters[ML9+1].specialf = M_NO_OP;



  /* the king wyvern */
  Monsters[ML9+2].id = ML9+2;
  Monsters[ML9+2].hp = 700;
  Monsters[ML9+2].speed = 3;
  Monsters[ML9+2].hit = 50;
  Monsters[ML9+2].actions = 6;
  Monsters[ML9+2].tactics = MM_BERSERK+MM_AVERAGE;
  Monsters[ML9+2].ac = 32;
  Monsters[ML9+2].dmg = 50;
  Monsters[ML9+2].sense = 5;
  Monsters[ML9+2].wakeup = 2;
  Monsters[ML9+2].level = 9;
  Monsters[ML9+2].status = GREEDY+HOSTILE+MOBILE+FLYING;
  Monsters[ML9+2].immunity = pow2(POISON)+pow2(FLAME);
  Monsters[ML9+2].xpv = 1000;
  Monsters[ML9+2].treasure = 4;
  Monsters[ML9+2].possessions = NULL;
  Monsters[ML9+2].sleep = 80;
  Monsters[ML9+2].x = -1;
  Monsters[ML9+2].y = -1;
  Monsters[ML9+2].transformid = RINGID+6;
  Monsters[ML9+2].corpsevalue = 50;
  Monsters[ML9+2].monchar = 'W';
  strcpy(Monsters[ML9+2].monstring,"king wyvern");
  strcpy(Monsters[ML9+2].corpsestr,"king wyvern's sting");
  Monsters[ML9+2].corpseweight = 100;
  Monsters[ML9+2].talkf = M_TALK_GREEDY;
  Monsters[ML9+2].movef = M_MOVE_NORMAL;
  Monsters[ML9+2].meleef = M_MELEE_POISON;
  Monsters[ML9+2].strikef = M_NO_OP;
  Monsters[ML9+2].specialf = M_NO_OP;



  /* the deathstar */
  Monsters[ML9+3].id = ML9+3;
  Monsters[ML9+3].hp = 500;
  Monsters[ML9+3].speed = 4;
  Monsters[ML9+3].hit = 30;
  Monsters[ML9+3].actions = 3;
  Monsters[ML9+3].tactics = MM_FORCED+MM_BERSERK;
  Monsters[ML9+3].hitloc = CENTER;
  Monsters[ML9+3].ac = 50;
  Monsters[ML9+3].dmg = 30;
  Monsters[ML9+3].sense = 10;
  Monsters[ML9+3].wakeup = 2;
  Monsters[ML9+3].level = 9;
  Monsters[ML9+3].status = HUNGRY+HOSTILE+MOBILE+FLYING;
  Monsters[ML9+3].immunity = pow2(FLAME);
  Monsters[ML9+3].xpv = 500;
  Monsters[ML9+3].treasure = 0;
  Monsters[ML9+3].possessions = NULL;
  Monsters[ML9+3].sleep = 50;
  Monsters[ML9+3].x = -1;
  Monsters[ML9+3].y = -1;
  Monsters[ML9+3].transformid = WEAPONID+33;
  Monsters[ML9+3].corpsevalue = 500;
  Monsters[ML9+3].monchar = 'd';
  strcpy(Monsters[ML9+3].monstring,"deathstar");
  strcpy(Monsters[ML9+3].corpsestr,"deathstar's eye");
  Monsters[ML9+3].corpseweight = 40;
  Monsters[ML9+3].talkf = M_NO_OP;
  Monsters[ML9+3].movef = M_MOVE_RANDOM;
  Monsters[ML9+3].meleef = M_MELEE_NORMAL;
  Monsters[ML9+3].strikef = M_NO_OP;
  Monsters[ML9+3].specialf = M_SP_EXPLODE;


  /* Archmage */
  Monsters[ML9+4].id = ML9+4;
  Monsters[ML9+4].hp = 250;
  Monsters[ML9+4].speed = 4;
  Monsters[ML9+4].hit = 20;
  Monsters[ML9+4].actions = 4;
  Monsters[ML9+4].tactics = MM_TIMID+MM_AVERAGE;
  Monsters[ML9+4].ac = 25;
  Monsters[ML9+4].dmg = 25;
  Monsters[ML9+4].sense = 5;
  Monsters[ML9+4].wakeup = 5;
  Monsters[ML9+4].level = 9;
  Monsters[ML9+4].status = MOBILE+COWARDLY;
  Monsters[ML9+4].immunity = pow2(FLAME)+pow2(COLD)+pow2(ELECTRICITY);
  Monsters[ML9+4].xpv = 1500;
  Monsters[ML9+4].treasure = 4;
  Monsters[ML9+4].possessions = NULL;
  Monsters[ML9+4].sleep = 50;
  Monsters[ML9+4].x = -1;
  Monsters[ML9+4].y = -1;
  Monsters[ML9+4].transformid = -1;
  Monsters[ML9+4].corpsevalue = 3000;
  Monsters[ML9+4].monchar = 'A';
  strcpy(Monsters[ML9+4].monstring,"archmage");
  strcpy(Monsters[ML9+4].corpsestr,"archmage's staff");
  Monsters[ML9+4].corpseweight = 50;
  Monsters[ML9+4].talkf = M_TALK_EVIL;
  Monsters[ML9+4].movef = M_MOVE_SMART;
  Monsters[ML9+4].meleef = M_MELEE_NORMAL;
  Monsters[ML9+4].strikef = M_NO_OP;
  Monsters[ML9+4].specialf = M_SP_SPELL;


  /* vampire lord */
  Monsters[ML9+5].id = ML9+5;
  Monsters[ML9+5].hp = 750;
  Monsters[ML9+5].speed = 2;
  Monsters[ML9+5].hit = 70;
  Monsters[ML9+5].actions = 9;
  Monsters[ML9+5].tactics = MM_GOOD;
  Monsters[ML9+5].ac = 50;
  Monsters[ML9+5].dmg = 50;
  Monsters[ML9+5].sense = 10;
  Monsters[ML9+5].wakeup = 1;
  Monsters[ML9+5].level = 9;
  Monsters[ML9+5].status = HOSTILE+MOBILE+COWARDLY+FLYING+M_INVISIBLE;
  Monsters[ML9+5].immunity = pow2(SLEEP)+pow2(POISON)+pow2(FEAR);
  Monsters[ML9+5].xpv = 1000;
  Monsters[ML9+5].treasure = 0;
  Monsters[ML9+5].possessions = NULL;
  Monsters[ML9+5].sleep = 50;
  Monsters[ML9+5].x = -1;
  Monsters[ML9+5].y = -1;
  Monsters[ML9+5].transformid = CLOAKID+1;
  Monsters[ML9+5].corpsevalue = 200;
  Monsters[ML9+5].monchar = 'V';
  strcpy(Monsters[ML9+5].monstring,"vampire lord");
  strcpy(Monsters[ML9+5].corpsestr,"vampire dust");
  Monsters[ML9+5].corpseweight = 1;
  Monsters[ML9+5].talkf = M_TALK_EVIL;
  Monsters[ML9+5].movef = M_MOVE_SPIRIT;
  Monsters[ML9+5].meleef = M_MELEE_SPIRIT;
  Monsters[ML9+5].strikef = M_MELEE_SPIRIT;
  Monsters[ML9+5].specialf = M_SP_SURPRISE;



  /* great wyrm */
  Monsters[ML9+6].id = ML9+6;
  Monsters[ML9+6].hp = 1000;
  Monsters[ML9+6].speed = 5;
  Monsters[ML9+6].hit = 50;
  Monsters[ML9+6].actions = 5;
  Monsters[ML9+6].tactics = MM_BERSERK+MM_POOR;
  Monsters[ML9+6].ac = 50;
  Monsters[ML9+6].dmg = 50;
  Monsters[ML9+6].sense = 10;
  Monsters[ML9+6].wakeup = 1;
  Monsters[ML9+6].level = 9;
  Monsters[ML9+6].status = HOSTILE+MOBILE+SWIMMING;
  Monsters[ML9+6].immunity = pow2(SLEEP)+pow2(POISON)+pow2(FEAR)+pow2(COLD);
  Monsters[ML9+6].xpv = 1500;
  Monsters[ML9+6].treasure = 8;
  Monsters[ML9+6].uniqueness = 2;
  Monsters[ML9+6].possessions = NULL;
  tob = ((pob) (calloc(1,sizeof(objtype))));
  make_weapon(tob,34); /* Defender */
  m_pickup(&(Monsters[ML9+6]),tob);
  Monsters[ML9+6].sleep = 100;
  Monsters[ML9+6].x = -1;
  Monsters[ML9+6].y = -1;
  Monsters[ML9+6].transformid = -1;
  Monsters[ML9+6].corpsevalue = 2000;
  Monsters[ML9+6].monchar = 'W';
  strcpy(Monsters[ML9+6].monstring,"Great Wyrm");
  strcpy(Monsters[ML9+6].corpsestr,"The head of the Great Wyrm");
  Monsters[ML9+6].corpseweight = 1500;
  Monsters[ML9+6].talkf = M_TALK_EVIL;
  Monsters[ML9+6].movef = M_MOVE_NORMAL;
  Monsters[ML9+6].meleef = M_MELEE_NORMAL;
  Monsters[ML9+6].strikef = M_MELEE_COLD;
  Monsters[ML9+6].specialf = M_SP_WYRM;






/* level 10 mostly unique */

  /* Morgon the Justiciar */
  Monsters[ML10+0].id = ML10+0;
  Monsters[ML10+0].uniqueness = 1;
  Monsters[ML10+0].hp = 500;
  Monsters[ML10+0].speed = 2;
  Monsters[ML10+0].hit = 100;
  Monsters[ML10+0].actions = 10;
  Monsters[ML10+0].tactics = MM_GOOD;
  Monsters[ML10+0].ac = 50;
  Monsters[ML10+0].dmg = 50;
  Monsters[ML10+0].sense = 5;
  Monsters[ML10+0].wakeup = 1;
  Monsters[ML10+0].level = 10;
  Monsters[ML10+0].status = MOBILE;
  Monsters[ML10+0].immunity = pow2(POISON)+pow2(FEAR)+pow2(SLEEP);
  Monsters[ML10+0].xpv = 2000;
  Monsters[ML10+0].treasure = 0;
  Monsters[ML10+0].possessions = NULL;
  /******* this is OK, since both creature and object are unique */
  tob = ((pob) (malloc(sizeof(objtype))));
  make_thing(tob,16); /* Morgon's Badge */
  m_pickup(&(Monsters[ML10+0]),tob);
  /*******/
  Monsters[ML10+0].sleep = 50;
  Monsters[ML10+0].x = -1;
  Monsters[ML10+0].y = -1;
  Monsters[ML10+0].transformid = -1;
  Monsters[ML10+0].corpsevalue = 0;
  Monsters[ML10+0].monchar = 'M';
  strcpy(Monsters[ML10+0].monstring,"Morgon, the Justiciar");
  strcpy(Monsters[ML10+0].corpsestr,"Morgon's body");
  Monsters[ML10+0].corpseweight = 1000;
  Monsters[ML10+0].talkf = M_TALK_MORGON;
  Monsters[ML10+0].movef = M_MOVE_SMART;
  Monsters[ML10+0].meleef = M_MELEE_NORMAL;
  Monsters[ML10+0].strikef = M_NO_OP;
  Monsters[ML10+0].specialf = M_SP_MORGON;

  /* The Eater of Magic */
  Monsters[ML10+1].id = ML10+1;
  Monsters[ML10+1].uniqueness = 1;
  Monsters[ML10+1].hp = 1000;
  Monsters[ML10+1].speed = 5;
  Monsters[ML10+1].hit = 0;
  Monsters[ML10+1].actions = 0;
  Monsters[ML10+1].tactics = MM_TIMID+MM_POOR;
  Monsters[ML10+1].ac = 20;
  Monsters[ML10+1].dmg = 0;
  Monsters[ML10+1].sense = 3;
  Monsters[ML10+1].wakeup = 3;
  Monsters[ML10+1].level = 10;
  Monsters[ML10+1].status = MOBILE;
  Monsters[ML10+1].immunity = EVERYTHING - pow2(NORMAL_DAMAGE);
  Monsters[ML10+1].xpv = 2000;
  Monsters[ML10+1].treasure = 0;
  Monsters[ML10+1].possessions = NULL;
  Monsters[ML10+1].sleep = 50;
  Monsters[ML10+1].x = -1;
  Monsters[ML10+1].y = -1;
  Monsters[ML10+1].transformid = STICKID+13;
  Monsters[ML10+1].corpsevalue = 250;
  Monsters[ML10+1].monchar = 'E';
  strcpy(Monsters[ML10+1].monstring,"The Eater of Magic");
  strcpy(Monsters[ML10+1].corpsestr,"The Heart of the Eater of Magic");
  Monsters[ML10+1].corpseweight = 1000;
  Monsters[ML10+1].talkf = M_TALK_SILENT;
  Monsters[ML10+1].movef = M_MOVE_RANDOM;
  Monsters[ML10+1].meleef = M_NO_OP;
  Monsters[ML10+1].strikef = M_NO_OP;
  Monsters[ML10+1].specialf = M_SP_EATER;


  /* The LawBringer */
  Monsters[ML10+2].id = ML10+2;
  Monsters[ML10+2].uniqueness = 1;
  Monsters[ML10+2].hp = 500;
  Monsters[ML10+2].speed = 5;
  Monsters[ML10+2].hit = 50;
  Monsters[ML10+2].actions = 9;
  Monsters[ML10+2].tactics = MM_BERSERK+MM_GOOD;
  Monsters[ML10+2].ac = 50;
  Monsters[ML10+2].dmg = 50;
  Monsters[ML10+2].sense = 5;
  Monsters[ML10+2].wakeup = 3;
  Monsters[ML10+2].level = 10;
  Monsters[ML10+2].status = MOBILE;
  Monsters[ML10+2].immunity = pow2(UNSTOPPABLE)+pow2(NORMAL_DAMAGE);
  Monsters[ML10+2].xpv = 2000;
  Monsters[ML10+2].treasure = 0;
  Monsters[ML10+2].possessions = NULL;
  Monsters[ML10+2].sleep = 50;
  Monsters[ML10+2].x = -1;
  Monsters[ML10+2].y = -1;
  Monsters[ML10+2].transformid = -1;
  Monsters[ML10+2].corpsevalue = 2000;
  Monsters[ML10+2].monchar = 'L';
  strcpy(Monsters[ML10+2].monstring,"The LawBringer");
  strcpy(Monsters[ML10+2].corpsestr,"The LawBringer's Crown");
  Monsters[ML10+2].corpseweight = 1000;
  Monsters[ML10+2].talkf = M_TALK_LB;
  Monsters[ML10+2].movef = M_MOVE_SMART;
  Monsters[ML10+2].meleef = M_MELEE_NORMAL;
  Monsters[ML10+2].strikef = M_NO_OP;
  Monsters[ML10+2].specialf = M_SP_LAWBRINGER;

  /* The Dragon Lord */
  Monsters[ML10+3].id = ML10+3;
  Monsters[ML10+3].uniqueness = 2;
  Monsters[ML10+3].hp = 2000;
  Monsters[ML10+3].speed = 1;
  Monsters[ML10+3].hit = 100;
  Monsters[ML10+3].actions = 10;
  Monsters[ML10+3].tactics = MM_GOOD;
  Monsters[ML10+3].ac = 70;
  Monsters[ML10+3].dmg = 100;
  Monsters[ML10+3].sense = 10;
  Monsters[ML10+3].wakeup = 1;
  Monsters[ML10+3].level = 10;
  Monsters[ML10+3].status = HOSTILE+MOBILE+FLYING;
  Monsters[ML10+3].immunity = 
    pow2(FLAME)+pow2(ELECTRICITY)+pow2(COLD)+ pow2(ACID)+pow2(NORMAL_DAMAGE);
  Monsters[ML10+3].xpv = 5000;
  Monsters[ML10+3].treasure = 10;
  Monsters[ML10+3].possessions = NULL;
  Monsters[ML10+3].sleep = 100;
  Monsters[ML10+3].x = -1;
  Monsters[ML10+3].y = -1;
  Monsters[ML10+3].transformid = ARMORID+12;
  Monsters[ML10+3].corpsevalue = 2000;
  Monsters[ML10+3].monchar = 'D';
  strcpy(Monsters[ML10+3].monstring,"The Dragon Lord");
  strcpy(Monsters[ML10+3].corpsestr,"The Dragon Lord's Armor");
  Monsters[ML10+3].corpseweight = 1000;
  Monsters[ML10+3].talkf = M_TALK_EVIL;
  Monsters[ML10+3].movef = M_MOVE_NORMAL;
  Monsters[ML10+3].meleef = M_MELEE_DRAGON;
  Monsters[ML10+3].strikef = M_STRIKE_FBOLT;
  Monsters[ML10+3].specialf = M_SP_DRAGONLORD;

  /* The Dark Emperor */
  Monsters[ML10+4].id = ML10+4;
  Monsters[ML10+4].hp = 3000;
  Monsters[ML10+4].speed = 2;
  Monsters[ML10+4].hit = 200;
  Monsters[ML10+4].actions = 10;
  Monsters[ML10+4].tactics = MM_GOOD;
  Monsters[ML10+4].ac = 200;
  Monsters[ML10+4].dmg = 200;
  Monsters[ML10+4].sense = 20;
  Monsters[ML10+4].wakeup = 20;
  Monsters[ML10+4].level = 10;
  Monsters[ML10+4].status = MOBILE+INTANGIBLE+M_INVISIBLE;
  Monsters[ML10+4].immunity = 
    EVERYTHING - pow2(NORMAL_DAMAGE) - pow2(UNSTOPPABLE)-1;
  Monsters[ML10+4].xpv = 10000;
  Monsters[ML10+4].treasure = 7;
  Monsters[ML10+4].possessions = NULL;
  /******* this is OK, since both creature and object are unique */
  tob = ((pob) (calloc(1,sizeof(objtype))));
  make_weapon(tob,37); /* Dark Emperor's Sword */
  m_pickup(&(Monsters[ML10+4]),tob);
  /*******/
  Monsters[ML10+4].sleep = 0;
  Monsters[ML10+4].x = -1;
  Monsters[ML10+4].y = -1;
  Monsters[ML10+4].transformid = -1;
  Monsters[ML10+4].corpsevalue = 3000;
  Monsters[ML10+4].monchar = 'D';
  strcpy(Monsters[ML10+4].monstring,"The Dark Emperor");
  strcpy(Monsters[ML10+4].corpsestr,"The Dark Emperor's Regalia");
  Monsters[ML10+4].corpseweight = 1000;
  Monsters[ML10+4].talkf = M_TALK_EVIL;
  Monsters[ML10+4].movef = M_MOVE_SPIRIT;
  Monsters[ML10+4].meleef = M_MELEE_SPIRIT;
  Monsters[ML10+4].strikef = M_MELEE_SPIRIT;
  Monsters[ML10+4].specialf = M_SP_SURPRISE;



  /* The Elemental Lord of Earth */
  Monsters[ML10+5].id = ML10+5;
  Monsters[ML10+5].hp = 2000;
  Monsters[ML10+5].speed = 12;
  Monsters[ML10+5].hit = 200;
  Monsters[ML10+5].actions = 4;
  Monsters[ML10+5].tactics = MM_POOR;
  Monsters[ML10+5].ac = 100;
  Monsters[ML10+5].dmg = 200;
  Monsters[ML10+5].sense = 5;
  Monsters[ML10+5].wakeup = 1;
  Monsters[ML10+5].level = 10;
  Monsters[ML10+5].status = MOBILE+INTANGIBLE;
  Monsters[ML10+5].immunity = 
    EVERYTHING - pow2(ELECTRICITY) - pow2(NORMAL_DAMAGE) - pow2(UNSTOPPABLE);
  Monsters[ML10+5].xpv = 5000;
  Monsters[ML10+5].treasure = 0;
  Monsters[ML10+5].possessions = NULL;
  /******* this is OK, since both creature and object are unique */
  tob = ((pob) (calloc(1,sizeof(objtype))));
  make_artifact(tob,4); /* Orb of Air */
  m_pickup(&(Monsters[ML10+5]),tob);
  /*******/
  Monsters[ML10+5].sleep = 0;
  Monsters[ML10+5].x = -1;
  Monsters[ML10+5].y = -1;
  Monsters[ML10+5].transformid = -1;
  Monsters[ML10+5].corpsevalue = 1;
  Monsters[ML10+5].monchar = 'E';
  strcpy(Monsters[ML10+5].monstring,"The Elemental Lord of Earth");
  strcpy(Monsters[ML10+5].corpsestr,"some dirt");
  Monsters[ML10+5].corpseweight = 10000;
  Monsters[ML10+5].talkf = M_TALK_SILENT;
  Monsters[ML10+5].movef = M_MOVE_SPIRIT;
  Monsters[ML10+5].meleef = M_MELEE_NORMAL;
  Monsters[ML10+5].strikef = M_MELEE_NORMAL;
  Monsters[ML10+5].specialf = M_SP_DE;

  /* The Elemental Lord of Air */
  Monsters[ML10+6].id = ML10+6;
  Monsters[ML10+6].hp = 1000;
  Monsters[ML10+6].speed = 1;
  Monsters[ML10+6].hit = 100;
  Monsters[ML10+6].actions = 6;
  Monsters[ML10+6].tactics = MM_GOOD;
  Monsters[ML10+6].ac = 50;
  Monsters[ML10+6].dmg = 50;
  Monsters[ML10+6].sense = 20;
  Monsters[ML10+6].wakeup = 1;
  Monsters[ML10+6].level = 10;
  Monsters[ML10+6].status = MOBILE+INTANGIBLE+FLYING+M_INVISIBLE;
  Monsters[ML10+6].immunity =   
    EVERYTHING - pow2(OTHER_MAGIC) - pow2(NORMAL_DAMAGE) - pow2(UNSTOPPABLE);
  Monsters[ML10+6].xpv = 5000;
  Monsters[ML10+6].treasure = 0;
  Monsters[ML10+6].possessions = NULL;
  /******* this is OK, since both creature and object are unique */
  tob = ((pob) (calloc(1,sizeof(objtype))));
  make_artifact(tob,3); /* orb of earth */
  m_pickup(&(Monsters[ML10+6]),tob);
  /*******/
  Monsters[ML10+6].sleep = 100;
  Monsters[ML10+6].x = -1;
  Monsters[ML10+6].y = -1;
  Monsters[ML10+6].transformid = -1;
  Monsters[ML10+6].corpsevalue = 1;
  Monsters[ML10+6].monchar = 'A';
  strcpy(Monsters[ML10+6].monstring,"The Elemental Lord of Air");
  strcpy(Monsters[ML10+6].corpsestr,"some air");
  Monsters[ML10+6].corpseweight = 0;
  Monsters[ML10+6].talkf = M_TALK_SILENT;
  Monsters[ML10+6].movef = M_MOVE_SPIRIT;
  Monsters[ML10+6].meleef = M_MELEE_NORMAL;
  Monsters[ML10+6].strikef = M_STRIKE_LBALL;
  Monsters[ML10+6].specialf = M_SP_WHIRL;

  /* The Elemental Lord of Water */
  Monsters[ML10+7].id = ML10+7;
  Monsters[ML10+7].hp = 1500;
  Monsters[ML10+7].speed = 5;
  Monsters[ML10+7].hit = 100;
  Monsters[ML10+7].actions = 5;
  Monsters[ML10+7].tactics = MM_AVERAGE;
  Monsters[ML10+7].ac = 100;
  Monsters[ML10+7].dmg = 100;
  Monsters[ML10+7].sense = 5;
  Monsters[ML10+7].wakeup = 1;
  Monsters[ML10+7].level = 10;
  Monsters[ML10+7].status = MOBILE+SWIMMING;
  Monsters[ML10+7].immunity = 
    EVERYTHING - pow2(NORMAL_DAMAGE) - pow2(FLAME) - pow2(UNSTOPPABLE);
  Monsters[ML10+7].xpv = 5000;
  Monsters[ML10+7].treasure = 0;
  Monsters[ML10+7].possessions = NULL;
  /******* this is OK, since both creature and object are unique */
  tob = ((pob) (calloc(1,sizeof(objtype))));
  make_artifact(tob,1); /* Orb of Fire */
  m_pickup(&(Monsters[ML10+7]),tob);
  /*******/
  Monsters[ML10+7].sleep = 100;
  Monsters[ML10+7].x = -1;
  Monsters[ML10+7].y = -1;
  Monsters[ML10+7].transformid = -1;
  Monsters[ML10+7].corpsevalue = 1;
  Monsters[ML10+7].monchar = 'W';
  strcpy(Monsters[ML10+7].monstring,"The Elemental Lord of Water");
  strcpy(Monsters[ML10+7].corpsestr,"some water");
  Monsters[ML10+7].corpseweight = 10;
  Monsters[ML10+7].talkf = M_TALK_SILENT;
  Monsters[ML10+7].movef = M_MOVE_NORMAL;
  Monsters[ML10+7].meleef = M_MELEE_NORMAL;
  Monsters[ML10+7].strikef = M_STRIKE_SNOWBALL;
  Monsters[ML10+7].specialf = M_NO_OP;

  /* The Elemental Lord of Fire */
  Monsters[ML10+8].id = ML10+8;
  Monsters[ML10+8].hp = 1500;
  Monsters[ML10+8].speed = 3;
  Monsters[ML10+8].hit = 200;
  Monsters[ML10+8].actions = 7;
  Monsters[ML10+8].tactics = MM_AVERAGE;
  Monsters[ML10+8].ac = 100;
  Monsters[ML10+8].dmg = 300;
  Monsters[ML10+8].sense = 10;
  Monsters[ML10+8].wakeup = 1;
  Monsters[ML10+8].level = 10;
  Monsters[ML10+8].status = MOBILE;
  Monsters[ML10+8].immunity = 
    EVERYTHING -pow2(NORMAL_DAMAGE)-pow2(COLD)-pow2(UNSTOPPABLE);
  Monsters[ML10+8].xpv = 5000;
  Monsters[ML10+8].treasure = 0;
  Monsters[ML10+8].possessions = NULL;
  /******* this is OK, since both creature and object are unique */
  tob = ((pob) (calloc(1,sizeof(objtype))));
  make_artifact(tob,2); /* orb of water */
  m_pickup(&(Monsters[ML10+8]),tob);
  /*******/
  Monsters[ML10+8].sleep = 100;
  Monsters[ML10+8].x = -1;
  Monsters[ML10+8].y = -1;
  Monsters[ML10+8].transformid = -1;
  Monsters[ML10+8].corpsevalue = 1;
  Monsters[ML10+8].monchar = 'F';
  strcpy(Monsters[ML10+8].monstring,"The Elemental Lord of Fire");
  strcpy(Monsters[ML10+8].corpsestr,"a spark");
  Monsters[ML10+8].corpseweight = 1000;
  Monsters[ML10+8].talkf = M_TALK_SILENT;
  Monsters[ML10+8].movef = M_MOVE_NORMAL;
  Monsters[ML10+8].meleef = M_MELEE_FIRE;
  Monsters[ML10+8].strikef = M_STRIKE_FBALL;
  Monsters[ML10+8].specialf = M_NO_OP;



  /* The Elemental Master */
  Monsters[ML10+9].id = ML10+9;
  Monsters[ML10+9].hp = 1000;
  Monsters[ML10+9].speed = 2;
  Monsters[ML10+9].hit = 200;
  Monsters[ML10+9].actions = 8;
  Monsters[ML10+9].tactics = MM_GOOD;
  Monsters[ML10+9].ac = 200;
  Monsters[ML10+9].dmg = 200;
  Monsters[ML10+9].sense = 20;
  Monsters[ML10+9].wakeup = 20;
  Monsters[ML10+9].level = 10;
  Monsters[ML10+9].status = MOBILE+FLYING;
  Monsters[ML10+9].immunity = 
   EVERYTHING -pow2(NORMAL_DAMAGE)-pow2(UNSTOPPABLE)-1;
  Monsters[ML10+9].xpv = 10000;
  Monsters[ML10+9].treasure = 10;
  Monsters[ML10+9].possessions = NULL;
  /******* this is OK, since both creature and object are unique */
  tob = ((pob) (calloc(1,sizeof(objtype))));
  make_artifact(tob,0); /* Orb of Mastery */
  m_pickup(&(Monsters[ML10+9]),tob);
  /*******/
  Monsters[ML10+9].sleep = 0;
  Monsters[ML10+9].x = -1;
  Monsters[ML10+9].y = -1;
  Monsters[ML10+9].transformid = -1;
  Monsters[ML10+9].corpsevalue = 1;
  Monsters[ML10+9].monchar = 'M';
  strcpy(Monsters[ML10+9].monstring,"The Elemental Master");
  strcpy(Monsters[ML10+9].corpsestr,"A burning mound of bubbling mud");
  Monsters[ML10+9].corpseweight = 10000;
  Monsters[ML10+9].talkf = M_TALK_EVIL;
  Monsters[ML10+9].movef = M_MOVE_SMART;
  Monsters[ML10+9].meleef = M_MELEE_MASTER;
  Monsters[ML10+9].strikef = M_STRIKE_MASTER;
  Monsters[ML10+9].specialf = M_SP_MASTER;


}
