/* omega copyright (c) 1987 by Laurence Raphael Brothers */
/* omon.c */
/* various functions to do with monsters */

#include <stdio.h>
#include <strings.h>
#include "oglob.h"

/* from oscr */
extern void mprint(),printm(),drawspot(),locprint(),filescanstring();
extern void erase_level(),menuprint();
extern void levelrefresh();
extern char ynq();

/* from ocity */
extern void load_city();

/* from omovef */
extern void l_jail();

/* from outil */
extern int random_range(),sign(),max(),distance(),bitp();
extern void do_los(),bitset(),bitreset();
extern int pow2();

/* from oaux */
extern void p_damage(),drop_at(),p_poison(),p_teleport();
extern void sleep_player();
extern void gain_experience(),calc_melee(),weapon_use(),p_miss();
extern char *itemid(),*actionlocstr();
extern int p_immune(),player_on_sanctuary();



/* from oeffect 1 or 2*/
extern void lball(),fbolt(),fball(),summon(),aggravate(),findspace();
extern void acid_cloud(),snowball(),nbolt(),level_drain(),disintegrate();
extern void disrupt();

/* from oitem */
extern pob create_object();

/* from omonf.c */
extern void m_normal_melee(),m_normal_move(),m_no_op(),m_sp_blackout();
extern void m_smart_move(),m_simple_move(),m_spirit_move(),m_vanish();
extern void m_fire_melee(),m_firebolt(),m_guard_talk(),m_random_move();
extern void m_abyss();
extern void m_flutter_move(),m_nbolt(),m_sp_surprise(),m_dragon_melee();
extern void m_stupid_talk(),m_hungry_talk(),m_silent_talk(),m_greedy_talk();
extern void m_mp_talk(),m_follow_move(),m_mp_melee(),m_sp_mp(),m_evil_talk();
extern void m_elec_melee(),m_lball(),m_fireball(),m_poison_melee(),m_titter();
extern void m_ng_melee(),m_sp_poison_cloud(),m_huge_sounds(),m_robot_talk();
extern void m_sleep_melee(),m_blind_strike(),m_im_talk();
extern void m_thief_f(),m_man_talk(),m_spectral_melee(),m_summon();
extern void m_teleport(),m_disease_melee(),m_illusion(),m_aggravate();
extern void m_sp_explode(),m_cold_melee(),m_sp_acid_cloud();
extern void m_sp_whirl(),m_sp_escape(),m_sp_ghost(),m_ghost_talk();
extern void m_mimsy_talk(),m_slithy_talk(),m_burble_talk(),m_sp_spell();
extern void m_hint_talk(),m_beg_talk(),m_scaredy_move();
extern void m_evil_fairy_talk(),m_good_fairy_talk(),m_sp_seductor();
extern void m_snowball(),m_sp_demon(),m_seductor_talk(),m_demonlover_talk();
extern void m_sp_demonlover(),m_sp_dragonlord(),m_sp_eater(),m_ninja_talk();
extern void m_assassin_talk();

/* omon functions */
int m_statusp(),m_immunityp(),tacticalp();
void m_pulse(),m_status_set(),m_damage();
void m_death(),m_hit(),m_miss(),m_pickup(),m_dropstuff();
void m_status_reset(),monster_strike(),tacmonster();
void monster_talk(),monster_melee(),monster_move(),monster_special();
void movemonster(),make_log_npc();
void m_movefunction();

void m_water(),m_lava(),m_altar(),m_whirlwind(),m_fire();
void m_trap_dart(),m_trap_pit(),m_trap_door(),m_trap_snare();
void m_trap_blade(),m_trap_fire(),m_trap_disintegrate(),m_trap_teleport();
void m_trap_manadrain(),m_trap_acid(),m_trap_sleepgas(),m_trap_abyss();
int m_open_door(),corpsevalue();
pmt make_hiscore_npc();

/* consider one monster's action */
void m_pulse(m)
struct monster *m;
{
  int i,range = distance(m->x, m->y, Player.x,Player.y);
  
  if (Time % 10 == 0) 
    if (m->hp < Monsters[m->id].hp)
      m->hp++;
  
  if ((! m_statusp(m,AWAKE)) && (range <= m->wakeup)) {
    m_status_set(m,AWAKE);
    Fastmove = FALSE;
  }
  
  if (m_statusp(m,AWAKE)) {
    if (m_statusp(m,WANDERING)) {
      if (m_statusp(m,MOBILE)) m_random_move(m);
      if (range <= m->sense) m_status_reset(m,WANDERING);
    }
    else /* not wandering */ {
      if (m_statusp(m,HOSTILE)) {
	if ((range > 2) && (range < m->sense) && (random_range(2) == 1))
	  if (los_p(m->x,m->y,Player.x,Player.y)) monster_strike(m);
      }
      if ((m_statusp(m,HOSTILE) || m_statusp(m,NEEDY))
	  && (range > 1)
	  && m_statusp(m,MOBILE)) {
	monster_move(m);
	/* if monster is greedy, picks up treasure it finds */
	if (m_statusp(m,GREEDY))
	  while (Dungeon[Dlevel][m->x][m->y].things != NULL) {
	    m_pickup(m,Dungeon[Dlevel][m->x][m->y].things->thing);
	    Dungeon[Dlevel][m->x][m->y].things =
	      Dungeon[Dlevel][m->x][m->y].things->next;
	  }
      }
      if (m_statusp(m,HOSTILE) && (range ==1)) {
	Fastmove = FALSE;
	monster_melee(m);
      }
    }
    /* prevents monsters from casting spells from other side of dungeon */
    if (range < max(4,m->level)) 
      monster_special(m);
  }
}
    


/* try to open a door */
int m_open_door(m,x,y,canopen)
struct monster *m;
int x,y,canopen;
{
  if (canopen &&
      (Dungeon[Dlevel][x][y].locchar==CLOSED_DOOR) &&
      (! Dungeon[Dlevel][x][y].secret)) {
	if (Dungeon[Dlevel][x][y].aux != LOCKED) 
	  Dungeon[Dlevel][x][y].locchar==OPEN_DOOR;
	else {
	  if (((20*(random_range(10)+1)) < m->dmg)) {
	    mprint("You hear a door splintering!");
	    Dungeon[Dlevel][x][y].locchar==FLOOR;
	  }
	}
      }
  return(TRUE);
}





/* actually make a move */
void movemonster(m,newx,newy)
struct monster *m;
int newx,newy;
{
  Dungeon[Dlevel][m->x][m->y].creature = NULL;
  m->x = newx;
  m->y = newy;
  Dungeon[Dlevel][m->x][m->y].creature = m;
  m_movefunction(m,Dungeon[Dlevel][m->x][m->y].p_locf);
}


/* give object o to monster m */
void m_pickup(m,o)
struct monster *m;
struct object *o;
{
  pol tmp = ((pol) malloc(sizeof(oltype)));
  tmp->thing = o;
  tmp->next = m->possessions;
  m->possessions = tmp;
}

void m_dropstuff(m)
struct monster *m;
{
  pol tmp = m->possessions;
  if (tmp != NULL) {
    while (tmp->next != NULL)
      tmp = tmp->next;

    tmp->next = Dungeon[Dlevel][m->x][m->y].things;
    Dungeon[Dlevel][m->x][m->y].things = m->possessions;
    m->possessions = NULL;
  }
}



void m_status_set(m,s)
struct monster *m;
int s;
{
  bitset(&(m->status),s);
}

void m_status_reset(m,s)
struct monster *m;
int s;
{
    bitreset(&(m->status),s);
}



/* monster status predicate */
int m_statusp(m,s)
struct monster *m;
int s;
{
  return(bitp(m->status,s));
}

/* monster immunity predicate */
int m_immunityp(m,s)
struct monster *m;
int s;
{
  return(bitp(m->immunity,pow2(s)));
}



void m_damage(m,dmg,dtype)
struct monster *m;
int dmg,dtype;
{
  m_status_set(m,AWAKE);
  m_status_set(m,HOSTILE);
  if (m_immunityp(m,dtype)) {
    if (los_p(Player.x,Player.y,m->x,m->y)) { 
      if (m->uniqueness != COMMON) strcpy(Str1,m->monstring);
      else {
	strcpy(Str1,"The ");
	strcat(Str1,m->monstring);
      }
      strcat(Str1," ignores the attack!");
      mprint(Str1);
    }
  }
  else if ((m->hp -= dmg) < 1) m_death(m);
}


void m_death(m)
struct monster *m;
{
  int newlevel;
  pob corpse;
  Tacmode = FALSE;
  m->hp = -1;
  if (los_p(Player.x,Player.y,m->x,m->y)) {
    gain_experience(m->xpv);
    calc_melee();
    if (m->uniqueness != COMMON) strcpy(Str1,m->monstring);
    else {
      strcpy(Str1,"The ");
      strcat(Str1,m->monstring);
    }
    strcat(Str1," is dead! ");
    mprint(Str1);
  }
  m_dropstuff(m);
  Dungeon[Dlevel][m->x][m->y].creature = NULL;
  if ((! Arena)&&(! Final)) {
    corpse=((pob) malloc(sizeof(objtype)));
    *corpse = Objects[CORPSEID];
    corpse->aux = m->id;
    corpse->weight = m->corpseweight;
    corpse->basevalue = m->corpsevalue;
    corpse->known = 2;
    strcpy(corpse->objstr,m->corpsestr);
    strcpy(corpse->truename,m->corpsestr);
    strcpy(corpse->cursestr,m->corpsestr);
    if (strcmp(corpse->objstr,"a greasy spot") == 0) {
      corpse->basevalue = 0;
      corpse->weight = 0;
      corpse->aux = 0;
    }
    drop_at(m->x,m->y,corpse);
    plotspot(m->x,m->y,FALSE);
  }
}




void m_hit(m,dtype)
struct monster *m;
int dtype;
{
  if (m->uniqueness == COMMON) {
    strcpy(Str3,"a ");
    strcat(Str3,m->monstring);
  }
  else strcpy(Str3,m->monstring);
  if ((Player.status[DISPLACED] > 0) && (random_range(2) == 1))
    mprint("The attack was displaced!");
  else  p_damage(random_range(m->dmg),dtype,Str3);
}

    
    
				   
void monster_move(m)
struct monster *m;
{
  switch(m->movef) {
    case M_NO_OP:m_no_op(m); break;
    case M_MOVE_NORMAL:m_normal_move(m); break;
    case M_MOVE_FLUTTER:m_flutter_move(m); break;
    case M_MOVE_FOLLOW:m_follow_move(m); break;
    case M_MOVE_TELEPORT:m_teleport(m); break;
    case M_MOVE_RANDOM:m_random_move(m); break;
    case M_MOVE_SMART:m_smart_move(m); break;
    case M_MOVE_SPIRIT:m_spirit_move(m); break;
    case M_MOVE_CONFUSED:m_confused_move(m); break;
    case M_MOVE_SCAREDY:m_scaredy_move(m); break;
  }
}

void monster_melee(m)
struct monster *m;
{
  /* It's lawful to wait to be attacked */
  if (player_on_sanctuary())
    mprint("The aegis of your deity protects you!");
  else {
    if (m->attacked==0) Player.alignment++;
    m->attacked++;
    switch(m->meleef) {
    case M_NO_OP:m_no_op(m); break;
    case M_MELEE_NORMAL:m_normal_melee(m); break;
    case M_MELEE_FIRE:m_fire_melee(m); break;
    case M_MELEE_DRAGON:m_dragon_melee(m); break;
    case M_MELEE_MP:m_mp_melee(m); break;
    case M_MELEE_ELEC:m_elec_melee(m); break;
    case M_MELEE_COLD:m_cold_melee(m); break;
    case M_MELEE_POISON:m_poison_melee(m); break;
    case M_MELEE_NG:m_ng_melee(m); break;
    case M_MELEE_SPIRIT:m_spectral_melee(m); break;
    case M_MELEE_DISEASE:m_disease_melee(m); break;
    case M_MELEE_SLEEP:m_sleep_melee(m); break;
    }
  }
}

void monster_strike(m)
struct monster *m;
{
  if (player_on_sanctuary())
    mprint("The aegis of your deity protects you!");
  else {
  /* It's lawful to wait to be attacked */
    if (m->attacked==0) Player.alignment++;
    m->attacked++;
    switch(m->strikef) {
    case M_NO_OP:m_no_op(m); break;
    case M_STRIKE_MISSILE:m_nbolt(m); break;
    case M_STRIKE_FBOLT:m_firebolt(m); break;
    case M_STRIKE_LBALL:m_lball(m); break;
    case M_STRIKE_FBALL:m_fireball(m); break;
    case M_STRIKE_SNOWBALL:m_snowball(m); break;
    case M_STRIKE_BLIND:m_blind_strike(m); break;
    }
  }
}

void monster_special(m)
struct monster *m;
{
  if (! player_on_sanctuary())
    switch(m->specialf) {
    case M_NO_OP:m_no_op(m); break;
    case M_SP_SURPRISE:m_sp_surprise(m); break;
    case M_SP_MP:m_sp_mp(m); break;
    case M_SP_THIEF:m_thief_f(m); break;
    case M_SP_DEMONLOVER:m_sp_demonlover(m); break;
    case M_SP_AGGRAVATE:m_aggravate(m); break;
    case M_SP_POISON_CLOUD:m_sp_poison_cloud(m); break;
    case M_SP_HUGE:m_huge_sounds(m); break;
    case M_SP_SUMMON:m_summon(m); break;
    case M_SP_ILLUSION:m_illusion(m); break;
    case M_SP_ESCAPE:m_sp_escape(m); break;
    case M_SP_FLUTTER:m_flutter_move(m); break;
    case M_SP_EXPLODE:m_sp_explode(m); break;
    case M_SP_DEMON:m_sp_demon(m); break;
    case M_SP_ACID_CLOUD:m_sp_acid_cloud(m); break;
    case M_SP_WHIRL:m_sp_whirl(m); break;
    case M_SP_GHOST:m_sp_ghost(m); break;
    case M_SP_SPELL:m_sp_spell(m); break;
    case M_SP_SEDUCTOR:m_sp_seductor(m); break;
    case M_SP_EATER:m_sp_eater(m); break;
    case M_SP_DRAGONLORD:m_sp_dragonlord(m); break;
    case M_SP_BLACKOUT:m_sp_blackout(m); break;
    case M_MOVE_TELEPORT:m_teleport(m); break;
    }
}  


void monster_talk(m)
struct monster *m;
{
  switch(m->talkf) {
    case M_NO_OP:m_no_op(m); break;
    case M_TALK_THIEF:m_thief_talk(m);break;
    case M_TALK_STUPID:m_stupid_talk(m); break;
    case M_TALK_SILENT:m_silent_talk(m); break;
    case M_TALK_HUNGRY:m_hungry_talk(m); break;
    case M_TALK_GREEDY:m_greedy_talk(m); break;
    case M_TALK_TITTER:m_titter(m); break;
    case M_TALK_MP:m_mp_talk(m); break;
    case M_TALK_IM:m_im_talk(m); break;
    case M_TALK_MAN:m_man_talk(m); break;
    case M_TALK_ROBOT:m_robot_talk(m); break;
    case M_TALK_EVIL:m_evil_talk(m); break;
    case M_TALK_GUARD:m_guard_talk(m); break;
    case M_TALK_MIMSY:m_mimsy_talk(m); break;
    case M_TALK_SLITHY:m_slithy_talk(m); break;
    case M_TALK_BURBLE:m_burble_talk(m); break;
    case M_TALK_GHOST:m_ghost_talk(m); break;
    case M_TALK_BEG:m_beg_talk(m); break;
    case M_TALK_HINT:m_hint_talk(m); break;
    case M_TALK_EF:m_evil_fairy_talk(m); break;
    case M_TALK_GF:m_good_fairy_talk(m); break;
    case M_TALK_SEDUCTOR:m_seductor_talk(m); break;
    case M_TALK_DEMONLOVER:m_demonlover_talk(m); break;
    case M_TALK_NINJA:m_ninja_talk(m); break;
    case M_TALK_ASSASSIN:m_assassin_talk(m); break;
  }
}  


/* monster id arg */
int corpsevalue(mid)
int mid;
{
  switch(mid) {
    default:
      return(0);
      break;
    case ML1+2:
      return(250);
      break;
    case ML1+4:
      return(50);
      break;
    case ML1+5:
      return(200);
      break;
    case ML1+7:
      return(100);
      break;
    case ML2+3:
      return(300);
      break;
    case ML2+5:
      return(200);
      break;
    case ML3+0:
      return(100);
      break;
    case ML3+1:
      return(100);
      break;
    case ML3+4:
      return(200);
      break;
    case ML4+0:
      return(200);
      break;
    case ML4+3:
      return(100);
      break;
    case ML5+0:
      return(400);
      break;
    case ML5+1:
      return(300);
      break;
    case ML5+2:
      return(500);
      break;
    case ML5+4:
      return(750);
      break;
    case ML6+0:
      return(1000);
      break;
    case ML6+1:
      return(1000);
      break;
    case ML6+2:
      return(1000);
      break;
    case ML6+3:
      return(1000);
      break;
    case ML7+0:
      return(1000);
      break;
    case ML7+2:
      return(10000);
      break;
    }
}

    
    
pmt make_hiscore_npc(npcid)
int npcid;
{
  pmt npc = ((pmt) malloc(sizeof(montype)));
  pob ob;
  int i,treasures,behavior,level,combatype,competence,talktype;
  *npc = Monsters[ML0+4];

  /* each of the high score npc's can be created here */
  switch(npcid) {
  case 1:
    level = Priestlevel[1];
    behavior = Priestbehavior[1];
    break;
  case 2:
    level = Priestlevel[2];
    behavior = Priestbehavior[2];
    break;
  case 3:
    level = Priestlevel[3];
    behavior = Priestbehavior[3];
    break;
  case 4:
    level = Priestlevel[4];
    behavior = Priestbehavior[4];
    break;
  case 5:
    level = Priestlevel[5];
    behavior = Priestbehavior[5];
    break;
  case 6:
    level = Priestlevel[6];
    behavior = Priestbehavior[6];
    break;
  case 7:
    level = Shadowlordlevel;
    behavior = Shadowlordbehavior;
    break;
  case 8:
    level = Commandantlevel;
    behavior = Commandantbehavior;
    break;
  case 9:
    level = Archmagelevel;
    behavior = Archmagebehavior;
    break;
  case 10:
    level = Primelevel;
    behavior = Primebehavior;
    break;
  case 11:
    level = Championlevel;
    behavior = Championbehavior;
    break;
  case 12:
    level = Primelevel;
    behavior = Primebehavior;
    break;
  case 13:
    level = Dukelevel;
    behavior = Dukebehavior;
    break;
  }
  strcpy(npc->monstring,Str2);
  npc->hp = level*20;
  npc->status = AWAKE+MOBILE+WANDERING;
  combatype = ((int) ((behavior % 100) / 10));
  competence = ((int) ((behavior % 1000) / 100));
  talktype = ((int) (behavior / 1000));
  npc->level = competence;
  if (npc->level < 2*Dlevel) npc->status += HOSTILE;
  npc->xpv = npc->level*20;
  if (npcid == 0) npc->xpv *= 10; /* high scorer worth more xpv */
  switch (combatype) {
  case 1: /* melee */
    npc->meleef = M_MELEE_NORMAL;
    npc->actions = competence;
    if (competence < 3)
      npc->tactics = MM_POOR;
    else if (competence < 6)
      npc->tactics = MM_AVERAGE;
    else npc->tactics = MM_GOOD;
    npc->dmg = competence*5;
    npc->hit = competence*3;
    break;
  case 2: /*missile*/
    npc->meleef = M_MELEE_NORMAL;
    npc->actions = max(2,(2 * competence / 3));
    if (competence < 5)
      npc->tactics = MM_POOR;
    else if (competence < 8)
      npc->tactics = MM_AVERAGE;
    else npc->tactics = MM_GOOD;
    npc->strikef = M_STRIKE_MISSILE;
    npc->dmg = competence*3;
    npc->hit = competence*2;
    break;
  case 3: /* spellcasting */
    npc->meleef = M_MELEE_NORMAL;
    npc->actions = max(2,(competence / 3));
    npc->tactics = MM_POOR+MM_TIMID;
    npc->dmg = competence;
    npc->hit = competence;
    npc->specialf = M_SP_SPELL;
    break;
  case 4: /* thievery */
    npc->meleef = M_MELEE_NORMAL;
    npc->actions = max(2,(2 * competence / 3));
    if (competence < 5)
      npc->tactics = MM_POOR;
    else if (competence < 8)
      npc->tactics = MM_AVERAGE;
    else npc->tactics = MM_GOOD;
    npc->tactics += MM_TIMID;
    npc->dmg = competence;
    npc->hit = competence;
    npc->specialf=M_SP_THIEF;
    break;
  case 5: /* flee */
    npc->actions = max(2,(competence / 3));
    npc->tactics = MM_TIMID+MM_AVERAGE;
    npc->meleef = M_MELEE_NORMAL;
    npc->specialf = M_MOVE_SCAREDY;
    break;

  }
  switch (talktype) {
  case 1: npc->talkf = M_TALK_EVIL; break;
  case 2: npc->talkf = M_TALK_MAN; break;
  case 3: npc->talkf = M_TALK_HINT; break;
  case 4: npc->talkf = M_TALK_BEG; break;
  case 5: npc->talkf = M_TALK_SILENT; break;
  default: mprint("Say Whutt? (npc talk weirdness)"); break;
  }
  npc->uniqueness = UNIQUE_MADE;
  return(npc);
}


void make_log_npc(npc)
struct monster *npc;
{
  int i,n = random_range(Logsize)+1;
  int behavior,status,level,combatype,competence,talktype;
  FILE *fd;
  strcpy(Str1,OMEGALIB);
  strcat(Str1,"omega.log");
  fd = fopen(Str1,"r");
  for (i=0;i<n;i++) {
    filescanstring(fd,Str2); /* eats the username */
    filescanstring(fd,Str2);
    fscanf(fd,"%d %d %d\n",&status,&level,&behavior);
  }
  fclose(fd);
  npc->hp = level*20;
  if (status==1) {
    if (level < 7) {
      *npc = Monsters[ML2+6];
      strcpy(npc->monstring,"ghost named ");
    }
    else if (level < 13) {
      *npc = Monsters[ML4+5];
      strcpy(npc->monstring,"haunt named ");
    }
    else if (level < 23) {
      *npc = Monsters[ML5+5];
      strcpy(npc->monstring,"spectre named ");
    }
    else {
      *npc = Monsters[ML6+5];
      strcpy(npc->monstring,"lich named ");
    }
    strcat(npc->monstring,Str2);
  }
  else {
    strcpy(npc->monstring,Str2);
    npc->status = AWAKE+MOBILE+WANDERING;
    combatype = ((int) ((behavior % 100) / 10));
    competence = ((int) ((behavior % 1000) / 100));
    talktype = ((int) (behavior / 1000));
    npc->level = competence;
    if (npc->level < 2*Dlevel) npc->status += HOSTILE;
    npc->xpv = npc->level*20;
    switch (combatype) {
    case 1: /* melee */
      npc->meleef = M_MELEE_NORMAL;
      npc->actions = competence;
      if (competence < 3)
	npc->tactics = MM_POOR;
      else if (competence < 6)
	npc->tactics = MM_AVERAGE;
      else npc->tactics = MM_GOOD;
      npc->dmg = competence*5;
      npc->hit = competence*3;
      break;
    case 2: /*missile*/
      npc->meleef = M_MELEE_NORMAL;
      npc->actions = max(2,(2 * competence / 3));
      if (competence < 5)
	npc->tactics = MM_POOR;
      else if (competence < 8)
	npc->tactics = MM_AVERAGE;
      else npc->tactics = MM_GOOD;
      npc->strikef = M_STRIKE_MISSILE;
      npc->dmg = competence*3;
      npc->hit = competence*2;
      break;
    case 3: /* spellcasting */
      npc->meleef = M_MELEE_NORMAL;
      npc->actions = max(2,(competence / 3));
      npc->tactics = MM_POOR+MM_TIMID;
      npc->dmg = competence;
      npc->hit = competence;
      npc->specialf = M_SP_SPELL;
      break;
    case 4: /* thievery */
      npc->meleef = M_MELEE_NORMAL;
      npc->actions = max(2,(2 * competence / 3));
      if (competence < 5)
	npc->tactics = MM_POOR;
      else if (competence < 8)
	npc->tactics = MM_AVERAGE;
      else npc->tactics = MM_GOOD;
      npc->tactics += MM_TIMID;
      npc->dmg = competence;
      npc->hit = competence;
      npc->specialf=M_SP_THIEF;
      break;
    case 5: /* flee */
      npc->actions = max(2,(competence / 3));
      npc->tactics = MM_TIMID+MM_AVERAGE;
      npc->meleef = M_MELEE_NORMAL;
      npc->specialf = M_MOVE_SCAREDY;
      break;
    }
    switch (talktype) {
    case 1: npc->talkf = M_TALK_EVIL; break;
    case 2: npc->talkf = M_TALK_MAN; break;
    case 3: npc->talkf = M_TALK_HINT; break;
    case 4: npc->talkf = M_TALK_BEG; break;
    case 5: npc->talkf = M_TALK_SILENT; break;
    default: mprint("Say Whutt? (npc talk weirdness)"); break;
    }
  }
  npc->uniqueness = UNIQUE_MADE;
}


/* decide monster actions in tactical combat mode */

void tacmonster(p_actions,pnumactions,m_actions,mnumactions,m)
int p_actions[10],pnumactions;
int m_actions[10],*mnumactions;
struct monster *m;
{
  int i,attacks,blocks;
  int blocknot,blockcounter,loc1,loc2;
  int p_attacks[4];
  int p_blocks[4];

  *mnumactions = min(10,m->actions);

  /* First decide whether or not to disengage */
  if ((tacticalp(m,MM_TIMID) && (m->hp < Monsters[m->id].hp / 2)) ||
      (! tacticalp(m,MM_BERSERK) && (m->hp < Monsters[m->id].hp / 4))) {
    *mnumactions = 1;
    m_actions[0] = DISENGAGE;
  }

  else { /* some combination of blocks and cuts to some locations */
    
    /* decide based on monster behavior what combo of cuts and blocks */
    if (tacticalp(m,MM_BERSERK)) 
      for(i=0;i<*mnumactions;i++) {
	m_actions[i] = CUT;
	attacks = *mnumactions;
      }
    
    else if (tacticalp(m,MM_TIMID)) 
      for(i=0;i<*mnumactions;i++) {
	m_actions[i] = BLOCK;
	blocks = *mnumactions;
      }
    
    else {
      attacks = *mnumactions / 2;
      blocks = *mnumactions / 2;
      if (attacks + blocks < *mnumactions) {
	if (m->hp > Player.hp) attacks++;
	else blocks++;
      }
      for(i=0;i<attacks;i++)
	m_actions[i] = CUT;
      for(i=0;i<blocks;i++)
	m_actions[i+attacks] = BLOCK;
    }
    
    /* decide where to cuts and blocks should go */
    
    /* forced location */
    if (tacticalp(m,MM_FORCED))
      for(i=0;i<*mnumactions;i++) {
	if (m_actions[i] == CUT)
	  m_actions[i]+=m->hitloc;
	else m_actions[i]+=(random_range(3)+1);
      }

    /* random locations for attacks and blocks */
    else if (tacticalp(m,MM_POOR)) {
      loc1 = random_range(3)+1;
      loc2 = random_range(3)+1;
      for(i=0;i<*mnumactions;i++) {
	if (m_actions[i] == BLOCK) m_actions[i] += loc1;
	else  m_actions[i] += loc2;
      }
    }
    
    /* distributed location */
    else if (tacticalp(m,MM_AVERAGE)) {
      blocknot = random_range(3)+1;
      blockcounter = random_range(3)+1;
      for(i=0;i<*mnumactions;i++) {
	if (m_actions[i] == BLOCK) {
	  if (blockcounter != blocknot)
	    m_actions[i] += ((blockcounter % 3)+1);
	  else m_actions[i] += (((++blockcounter) % 3) + 1);
	  blockcounter++;
	}
	else if (m_actions[i] == CUT)
	  m_actions[i] += blocknot;
      }
    }
    
    else { /* If monster is competent enough, will see player's actions */
      if (m->level+random_range(30) > Player.level+random_range(20)) {
	
	/* Find which area player blocks and attacks least in */
	
	for (i=0;i<pnumactions;i++)
	  if ((p_actions[i] - (p_actions[i] % 10) == BLOCK) ||
	      (p_actions[i] - (p_actions[i] % 10) == RIPOSTE))
	    p_blocks[p_actions[i] % 10]++;
	
	if ((p_blocks[LOW] <= p_blocks[CENTER]) &&
	    (p_blocks[LOW] <= p_blocks[HIGH]))
	  loc1 = LOW;
	else if ((p_blocks[CENTER] <= p_blocks[LOW]) &&
		 (p_blocks[CENTER] <= p_blocks[HIGH]))
	  loc1 = CENTER;
	else loc1 = HIGH;
	
	
	for (i=0;i<pnumactions;i++)
	  if ((p_actions[i] - (p_actions[i] % 10) == CUT) ||
	      (p_actions[i] - (p_actions[i] % 10) == THRUST) ||
	      (p_actions[i] - (p_actions[i] % 10) == LUNGE))
	    p_attacks[p_actions[i] % 10]++;
	
	if ((p_attacks[LOW] <= p_attacks[CENTER]) &&
	    (p_attacks[LOW] <= p_attacks[HIGH]))
	  loc2 = LOW;
	else if ((p_attacks[CENTER] <= p_attacks[LOW]) &&
		 (p_attacks[CENTER] <= p_attacks[HIGH]))
	  loc2 = CENTER;
	else loc2 = HIGH;
	
	for(i=0;i<*mnumactions;i++) {
	  if (m_actions[i] == CUT)
	    m_actions[i] += loc1;
	  else if (m_actions[i] == BLOCK)
	    m_actions[i] += loc2;
	}
      }
      else { /* monster was not clever enough  so use average result */
	blocknot = random_range(3)+1;
	blockcounter = random_range(3)+1;
	for(i=0;i<*mnumactions;i++) {
	  if (m_actions[i] == BLOCK) {
	    if (blockcounter != blocknot)
	      m_actions[i] += ((blockcounter % 3)+1);
	    else m_actions[i] += (((++blockcounter) % 3) + 1);
	    blockcounter++;
	  }
	  else if (m_actions[i] == CUT)
	    m_actions[i] += blocknot;
	}
      }
    }
  }
}

int tacticalp(m,tac)
struct monster *m;
int tac;
{
  return(bitp(m->tactics,tac));
}


/* execute some move function for a monster */
void m_movefunction(m,movef)
struct monster *m;
int movef;
{
  /* loc functs above traps should be activated whether levitating or not */
  if (! m_statusp(m,FLYING))
    switch(movef) {

    /* miscellaneous */
    case L_NO_OP:m_no_op(m); break;
    case L_WATER:m_water(m); break;
    case L_WHIRLWIND:m_whirlwind(m); break;
    case L_LAVA:m_lava(m); break;
    case L_FIRE:m_fire(m); break;
    case L_MAGIC_POOL:m_water(m); break;
    case L_ABYSS: m_abyss(m); break;

    case L_TRAP_DART:m_trap_dart(m); break;
    case L_TRAP_PIT:m_trap_pit(m); break;
    case L_TRAP_DOOR:m_trap_door(m); break;
    case L_TRAP_SNARE:m_trap_snare(m); break;
    case L_TRAP_BLADE:m_trap_blade(m); break;
    case L_TRAP_FIRE:m_trap_fire(m); break;
    case L_TRAP_TELEPORT:m_trap_teleport(m); break;
    case L_TRAP_DISINTEGRATE:m_trap_disintegrate(m); break;
    case L_TRAP_MANADRAIN:m_trap_manadrain(m); break;
    case L_TRAP_SLEEP_GAS:m_trap_sleepgas(m); break;
    case L_TRAP_ACID:m_trap_acid(m); break;
    case L_TRAP_ABYSS:m_trap_abyss(m);break;

    case L_ALTAR:m_altar(m); break;
    }
}


void m_trap_dart(m)
struct monster *m;
{
  if (los_p(m->x,m->y,Player.x,Player.y)) {
    if (m->uniqueness != COMMON) strcpy(Str1,m->monstring);
    else {
      strcpy(Str1,"The ");
      strcat(Str1,m->monstring);
    }
    strcat(Str1," was hit by a dart!");
    mprint(Str1);
  Dungeon[Dlevel][m->x][m->y].locchar = TRAP;
  }
  m_damage(m,Dlevel*2,NORMAL_DAMAGE);
}

void m_trap_pit(m)
struct monster *m;
{
  if (los_p(m->x,m->y,Player.x,Player.y)) {
    if (m->uniqueness != COMMON) strcpy(Str1,m->monstring);
    else {
      strcpy(Str1,"The ");
      strcat(Str1,m->monstring);
    }
    strcat(Str1," fell into a pit!");
    mprint(Str1);
  Dungeon[Dlevel][m->x][m->y].locchar = TRAP;
  }
  if (! m_statusp(m,INTANGIBLE))
    m_status_reset(m,MOBILE);
  m_damage(m,Dlevel*5,NORMAL_DAMAGE);

}

void m_trap_door(m)
struct monster *m;
{
  if (los_p(m->x,m->y,Player.x,Player.y)) {
    if (m->uniqueness != COMMON) strcpy(Str1,m->monstring);
    else {
      strcpy(Str1,"The ");
      strcat(Str1,m->monstring);
    }
    strcat(Str1," fell into a trap door!");
    mprint(Str1);
  Dungeon[Dlevel][m->x][m->y].locchar = TRAP;
  }
  m_vanish(m);
}

void m_trap_abyss(m)
struct monster *m;
{
  char Str1[80];
  if (los_p(m->x,m->y,Player.x,Player.y)) {
    if (m->uniqueness != COMMON) strcpy(Str1,m->monstring);
    else {
      strcpy(Str1,"The ");
      strcat(Str1,m->monstring);
    }
    strcat(Str1," fell into the infinite abyss!");
    mprint(Str1);
  Dungeon[Dlevel][m->x][m->y].locchar = ABYSS;
  Dungeon[Dlevel][m->x][m->y].p_locf = L_ABYSS;
  }
  m_vanish(m);
}

void m_trap_snare(m)
struct monster *m;
{
  char Str1[80];
  Dungeon[Dlevel][m->x][m->y].locchar = TRAP;
  if (los_p(m->x,m->y,Player.x,Player.y)) {
    if (m->uniqueness != COMMON) strcpy(Str1,m->monstring);
    else {
      strcpy(Str1,"The ");
      strcat(Str1,m->monstring);
    }
    strcat(Str1," was caught in a snare!");
    mprint(Str1);
  }
  if (! m_statusp(m,INTANGIBLE)) m_status_reset(m,MOBILE);
}

void m_trap_blade(m)
struct monster *m;
{
  char Str1[80];
  Dungeon[Dlevel][m->x][m->y].locchar = TRAP;
  if (los_p(m->x,m->y,Player.x,Player.y)) { 
    if (m->uniqueness != COMMON) strcpy(Str1,m->monstring);
    else {
      strcpy(Str1,"The ");
      strcat(Str1,m->monstring);
    }
    strcat(Str1," was hit by a blade trap!");
    mprint(Str1); } 
  m_damage(m,(Dlevel+1)*7-Player.defense,NORMAL_DAMAGE);
}

void m_trap_fire(m)
struct monster *m;
{
  char Str1[80];
  Dungeon[Dlevel][m->x][m->y].locchar = TRAP;
  if (los_p(m->x,m->y,Player.x,Player.y)) { 
    if (m->uniqueness != COMMON) strcpy(Str1,m->monstring);
    else {
      strcpy(Str1,"The ");
      strcat(Str1,m->monstring);
    }
    strcat(Str1," was hit by a fire trap!");
    mprint(Str1); 
  } 
  m_damage(m,(Dlevel+1)*5,FLAME);
}


void m_fire(m)
struct monster *m;
{
  char Str1[80];
  if (los_p(m->x,m->y,Player.x,Player.y)) { 
    if (m->uniqueness != COMMON) strcpy(Str1,m->monstring);
    else {
      strcpy(Str1,"The ");
      strcat(Str1,m->monstring);
    }
    strcat(Str1," was blasted by fire!");
    mprint(Str1); 
  } 
  m_damage(m,random_range(100),FLAME);
}

void m_trap_teleport(m)
struct monster *m;
{
  char Str1[80];
  Dungeon[Dlevel][m->x][m->y].locchar = TRAP;
  if (los_p(m->x,m->y,Player.x,Player.y)) { 
    if (m->uniqueness != COMMON) strcpy(Str1,m->monstring);
    else {
      strcpy(Str1,"The ");
      strcat(Str1,m->monstring);
    }
    strcat(Str1," walked into a teleport trap!");
    mprint(Str1); 
  } 
  m_teleport(m);
}

void m_trap_disintegrate(m)
struct monster *m;
{
  char Str1[80];
  if (los_p(m->x,m->y,Player.x,Player.y)) { 
    if (m->uniqueness != COMMON) strcpy(Str1,m->monstring);
    else {
      strcpy(Str1,"The ");
      strcat(Str1,m->monstring);
    }
    strcat(Str1," walked into a disintegration trap!");
    mprint(Str1); 
    Dungeon[Dlevel][m->x][m->y].locchar = TRAP;
  } 
  disintegrate(m->x,m->y);
}

void m_trap_sleepgas(m)
struct monster *m;
{
  char Str1[80];
  if (los_p(m->x,m->y,Player.x,Player.y)) { 
    if (m->uniqueness != COMMON) strcpy(Str1,m->monstring);
    else {
      strcpy(Str1,"The ");
      strcat(Str1,m->monstring);
    }
    strcat(Str1," walked into a sleepgas trap!");
    mprint(Str1); 
    Dungeon[Dlevel][m->x][m->y].locchar = TRAP;
  } 
  if (! m_immunityp(m,SLEEP)) m_status_reset(m,AWAKE);
}

void m_trap_acid(m)
struct monster *m;
{
  char Str1[80];
  if (los_p(m->x,m->y,Player.x,Player.y)) { 
    if (m->uniqueness != COMMON) strcpy(Str1,m->monstring);
    else {
      strcpy(Str1,"The ");
      strcat(Str1,m->monstring);
    }
    strcat(Str1," walked into an acid bath trap!");
    mprint(Str1); 
    Dungeon[Dlevel][m->x][m->y].locchar = TRAP;
  } 
  m_damage(m,random_range(Dlevel*Dlevel),ACID);
}

void m_trap_manadrain(m)
struct monster *m;
{
  char Str1[80];
  if (los_p(m->x,m->y,Player.x,Player.y)) { 
    if (m->uniqueness != COMMON) strcpy(Str1,m->monstring);
    else {
      strcpy(Str1,"The ");
      strcat(Str1,m->monstring);
    }
    strcat(Str1," walked into a manadrain trap!");
    mprint(Str1); 
    Dungeon[Dlevel][m->x][m->y].locchar = TRAP;
  } 
  if (m->specialf == M_SP_SPELL) m->specialf = M_NO_OP;
}


void m_water(m)
struct monster *m;
{
  char Str1[80];
  if ((! m_statusp(m,INTANGIBLE)) && (! m_statusp(m,SWIMMING))) {
    if (los_p(m->x,m->y,Player.x,Player.y)) { 
    if (m->uniqueness != COMMON) strcpy(Str1,m->monstring);
    else {
      strcpy(Str1,"The ");
      strcat(Str1,m->monstring);
    }
      strcat(Str1," drowned!");
      mprint(Str1); 
    }
    m_death(m);
  }
}

void m_whirlwind(m)
struct monster *m;
{
  char Str1[80];
  if (! m_statusp(m,INTANGIBLE)) {
    if (los_p(m->x,m->y,Player.x,Player.y)) { 
    if (m->uniqueness != COMMON) strcpy(Str1,m->monstring);
    else {
      strcpy(Str1,"The ");
      strcat(Str1,m->monstring);
    }
      strcat(Str1," was caught in a whirlwind!");
      mprint(Str1); 
    }
    m_damage(m,25,ELECTRICITY);
    m_teleport(m);
  }
}

void m_abyss(m)
struct monster *m;
{
  char Str1[80];
  if (los_p(m->x,m->y,Player.x,Player.y)) {
    if (m->uniqueness != COMMON) strcpy(Str1,m->monstring);
    else {
      strcpy(Str1,"The ");
      strcat(Str1,m->monstring);
    }
    strcat(Str1," fell into the infinite abyss!");
    mprint(Str1);
  }
  m_vanish(m);
}



void m_lava(m)
struct monster *m;
{
  char Str1[80];
  if ((! m_immunityp(m,FLAME)) && (! m_statusp(m,SWIMMING))) {
    if (los_p(m->x,m->y,Player.x,Player.y)) { 
    if (m->uniqueness != COMMON) strcpy(Str1,m->monstring);
    else {
      strcpy(Str1,"The ");
      strcat(Str1,m->monstring);
    }
      strcat(Str1," died in a pool of lava!");
      mprint(Str1); 
    }
    m_death(m);
  }
}

void m_altar(m)
struct monster *m;
{
  if (m->uniqueness != COMMON) strcpy(Str1,m->monstring);
  else {
    strcpy(Str1,"The ");
    strcat(Str1,m->monstring);
  }
  strcat(Str1," walks next to an altar...");
  mprint(Str1);
  if (Dungeon[Dlevel][m->x][m->y].aux==Player.patron) {
    mprint("Your deity is angry!");
    mprint("A bolt of godsfire strikes the monster....");
    disrupt(m->x,m->y,Player.rank[PRIESTHOOD]*50);
  }
  else if ((Player.patron == ODIN) || (Player.patron == ATHENA)) {
    if ((Dungeon[Dlevel][m->x][m->y].aux == SET) ||
	(Dungeon[Dlevel][m->x][m->y].aux == HECATE)) {
      mprint("The deity of the altar smiles on the monster....");
      mprint("A shaft of light zaps the altar...");
      m->hp = Monsters[m->id].hp*2;
    }
    else mprint("but nothing much seems to happen");
  }
  else if ((Player.patron == SET) || (Player.patron == HECATE)) {
    if ((Dungeon[Dlevel][m->x][m->y].aux == ODIN) ||
	(Dungeon[Dlevel][m->x][m->y].aux == ATHENA)) {
      mprint("The deity of the altar smiles on the monster....");
      mprint("A shaft of light zaps the altar...");
      m->hp = Monsters[m->id].hp*2;
    }
    else mprint("but nothing much seems to happen");
  }
  else mprint("but nothing much seems to happen");
}

