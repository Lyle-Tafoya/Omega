/* omega copyright (c) 1987,1988 by Laurence Raphael Brothers */
/* omon.c */
/* various functions to do with monsters */

#include "oglob.h"



/* consider one monster's action */
void m_pulse(m)
struct monster *m;
{
  int range = distance(m->x, m->y, Player.x,Player.y);

  if (Time % 10 == 0) 
    if (m->hp < Monsters[m->id].hp)
      m->hp++;
  
  if ((! m_statusp(m,AWAKE)) && (range <= m->wakeup)) {
    m_status_set(m,AWAKE);
    resetgamestatus(FAST_MOVE);
  }
  
  if (m_statusp(m,AWAKE)) {
    if (m_statusp(m,WANDERING)) {
      if (m_statusp(m,MOBILE)) m_random_move(m);
      if (range <= m->sense) m_status_reset(m,WANDERING);
    }
    else /* not wandering */ {
      if (m_statusp(m,HOSTILE)) {
	if ((range > 2) && (range < m->sense) && (random_range(2) == 1))
	  if (los_p(m->x,m->y,Player.x,Player.y) &&
	      (Player.status[INVISIBLE] == 0)) monster_strike(m);
      }
      if ((m_statusp(m,HOSTILE) || m_statusp(m,NEEDY))
	  && (range > 1)
	  && m_statusp(m,MOBILE)) {
	monster_move(m);
	/* if monster is greedy, picks up treasure it finds */
	if (m_statusp(m,GREEDY))
	  while (Level->site[m->x][m->y].things != NULL) {
	    m_pickup(m,Level->site[m->x][m->y].things->thing);
	    Level->site[m->x][m->y].things =
	      Level->site[m->x][m->y].things->next;
	  }
      }
      if (m_statusp(m,HOSTILE) && (range ==1)) {
	resetgamestatus(FAST_MOVE);
	tacmonster(m);
      }
    }
    /* prevents monsters from casting spells from other side of dungeon */
    if (range < max(5,m->level)) 
      monster_special(m);
  }
}
    






/* actually make a move */
void movemonster(m,newx,newy)
struct monster *m;
int newx,newy;
{
  Level->site[m->x][m->y].creature = NULL;
  m->x = newx;
  m->y = newy;
  Level->site[m->x][m->y].creature = m;
  m_movefunction(m,Level->site[m->x][m->y].p_locf);
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

    tmp->next = Level->site[m->x][m->y].things;
    Level->site[m->x][m->y].things = m->possessions;
    m->possessions = NULL;
  }
}



void m_status_set(m,s)
struct monster *m;
int s;
{
  m->status |= s;
}

void m_status_reset(m,s)
struct monster *m;
int s;
{
  m->status &= ~s;
}



/* monster status predicate */
int m_statusp(m,s)
struct monster *m;
int s;
{
  return(m->status & s);
}

/* monster immunity predicate */
int m_immunityp(m,s)
struct monster *m;
int s;
{
  return(m->immunity & pow2(s));
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
  pob corpse;
  pml ml;
  int x,y,found=FALSE;
  m->hp = -1;
  if (los_p(Player.x,Player.y,m->x,m->y)) {
    putspot(m->x,m->y,getspot(m->x,m->y,FALSE));
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
  if (m->id == ML10+10) { /* death */
    mprint("Death lies sprawled out on the ground......");
    mprint("Death laughs ironically and gets back to his feet.");
    mprint("He gestures and another scythe appears in his hands.");
    switch(random_range(10)) {
    case 0:
      mprint("Death performs a little bow and goes back on guard."); 
      break;
    case 1:
      mprint("'A hit! A palpable hit!' Death goes back on the attack."); 
      break;
    case 2:
      mprint("'Ah, if only it could be so simple!' snickers Death.");
      break;
    case 3:
      mprint("'This fool thinks he can slay death! What a jest!' says Death.");
      break;
    case 4:
      mprint("'Your point is well taken.' says Death, attacking again.");
      break;
    case 5:
      mprint("'Oh, come now, stop delaying the inevitable.' says Death.");
      break;
    case 6:
      mprint("'Your destiny ends here with me.' says Death, scythe raised.");
      break;
    case 7:
      mprint("'I almost felt that.' says Death, smiling.");
      break;
    case 8:
      mprint("'Timeo Mortis?' asks Death quizzically, 'Not me!'");
      break;
    case 9:
      mprint("Death sighs theatrically. 'They never learn.'");
      break;
    }
    strengthen_death(m);
  }
  else {
    Level->site[m->x][m->y].creature = NULL;
    if (random_range(2) || (m->uniqueness != COMMON)) {
      corpse=((pob) malloc(sizeof(objtype)));
      make_corpse(corpse,m);
      drop_at(m->x,m->y,corpse);
    }
    plotspot(m->x,m->y,FALSE);
    switch(m->id) {
    case ML0+8: /* hiscore npc */
      switch(m->aux2) {
      case 0:
	mprint("You hear a faroff dirge. You feel a sense of triumph.");
	break;
      case 1:case 2: case 3:case 4:case 5:case 6:
	mprint("You hear a faroff sound like angels crying....");
	strcpy(Priest[1],nameprint());
	Priestbehavior[1] = 2933;
	break;
      case 7:
      mprint("A furtive figure dashes out of the shadows, takes a look at");
	mprint("the corpse, and runs away!");
	strcpy(Shadowlord,nameprint());
	Shadowlordbehavior = 2912;
	break;
      case 8:
	mprint("An aide-de-camp approaches, removes the corpse's insignia,");
	mprint("and departs.");
	strcpy(Commandant,nameprint());
	Commandantbehavior = 2912;
	break;
      case 9:
	mprint("An odd glow surrounds the corpse, and slowly fades.");
	strcpy(Archmage,nameprint());
	Archmagebehavior = 2933;
	break;
      case 10:
	mprint("A demon materializes, takes a quick look at the corpse,");
	mprint("and teleports away with a faint popping noise.");
	strcpy(Prime,nameprint());
	Primebehavior = 2932;
	break;
      case 11:
	mprint("A sports columnist rushes forward and takes a quick photo");
	mprint("of the corpse and rushes off muttering about a deadline.");
	strcpy(Champion,nameprint());
	Championbehavior = 2913;
	break;
      case 12:
	mprint("You hear a fanfare in the distance, and feel dismayed.");
	strcpy(Duke,nameprint());
	Dukebehavior = 2911;
	break;
      case 13:
	if (Player.alignment > 10) mprint("You feel smug.");
	else if (Player.alignment < 10) mprint("You feel ashamed.");
	strcpy(Chaoslord,nameprint());
	Chaoslordbehavior = 2912;
	break;
      case 14:
	if (Player.alignment < 10) mprint("You feel smug.");
	else if (Player.alignment > 10) mprint("You feel ashamed.");
	strcpy(Lawlord,nameprint());
	Lawlordbehavior = 2911;
	break;
      case 15: 
	/* just a tad complicated. Promote a new justiciar if any
	   guards are left in the city, otherwise Destroy the Order! */
	Player.alignment -= 100;
	if (! gamestatusp(DESTROYED_ORDER)) {
	  mprint("In the distance you hear a trumpet. A Servant of Law");
	  mprint("materializes, sheds a tear, and leaves.");
	  strcpy(Justiciar,nameprint());
	  Justiciarbehavior = 2911;
	  /* promote one of the city guards to be justiciar */
	  ml = City->mlist;
	  while ((! found) && (ml != NULL)) {
	    found = ((ml->m->id == ML0+3) && (ml->m->hp > 0));
	    if (! found) ml=ml->next;
	  }
	  if (ml != NULL) {
	    mprint("A new justiciar has been promoted!");
	    x = ml->m->x; y = ml->m->y;
	    make_hiscore_npc(ml->m,15);
	    ml->m->x = x;
	    ml->m->y = y;
	    ml->m->click = (Tick + 1) % 60;
	    m_status_reset(ml->m,AWAKE);
	    m_status_reset(ml->m,HOSTILE);
	  }
	  alert_guards(); 
	  /* will cause order to be destroyed if no guards or justiciar*/
	}
	else {
	  mprint("A Servant of Chaos materializes, grabs the corpse,");
	  mprint("snickers a bit, and vanishes.");
	}
	break;
      }
      break;
    case ML0+3: /* guard */
      Player.alignment -= 10;
      if ((Current_Environment == E_CITY) || 
	  (Current_Environment == E_VILLAGE))
	alert_guards();
      break;
    case ML3+5: 
      if (! gamestatusp(ATTACKED_ORACLE)) {
	mprint("You seem to hear a woman's voice from far off:");
	mprint("'Well done! Come to me now....'");
      }
      setgamestatus(COMPLETED_CAVES); 
      break; /* gob king */
    case ML7+5: 
      if (! gamestatusp(ATTACKED_ORACLE)) {
	mprint("A female voice sounds from just behind your ear:");
	mprint("'Well fought! I have some new advice for you....'");
      }
      setgamestatus(COMPLETED_SEWERS); 
      break; /*grt worm */
    case ML10+1:
      setgamestatus(KILLED_EATER);
      break;
    case ML10+2:
      setgamestatus(KILLED_LAWBRINGER);
      break;
    case ML10+3:
      setgamestatus(KILLED_DRAGONLORD);
      break;
    case ML10+4:
      setgamestatus(COMPLETED_VOLCANO);
      if (! gamestatusp(ATTACKED_ORACLE)) {
	mprint("You feel a soft touch on your shoulder...");
	mprint("You turn around but there is no one there!");
	mprint("You turn back and see a note: 'See me soon.'");
	mprint("The note vanishes in a burst of blue fire!");
      }
      break;
    case ML10+9: 
      if (! gamestatusp(ATTACKED_ORACLE)) {
	mprint("Words appear before you, traced in blue flame!");
	mprint("'Return to the Prime Plane via the Circle of Sorcerors....'");
      }
      break; /* elem mast */
    }
  }
  dodrawspot(m->x,m->y);
}




    
    
				   
void monster_move(m)
struct monster *m;
{
  monster_action(m,m->movef);
}









void monster_strike(m)
struct monster *m;
{
  if (player_on_sanctuary())
    print1("The aegis of your deity protects you!");
  else {
  /* It's lawful to wait to be attacked */
    if (m->attacked==0) Player.alignment++;
    m->attacked++;
    monster_action(m,m->strikef);
  }
}

void monster_special(m)
struct monster *m;
{
  /* since many special functions are really attacks, cancel them
     all if on sanctuary */
  if (! player_on_sanctuary())
    monster_action(m,m->specialf);
}  


void monster_talk(m)
struct monster *m;
{
  monster_action(m,m->talkf);
}

void monster_action(m,action)
struct monster *m;
int action;
{
  int meleef;
  if ((action >= M_MELEE_NORMAL) && (action < M_MOVE_NORMAL)) {
    /* kluge allows multiple attack forms */
    if (distance(m->x,m->y,Player.x,Player.y)<2) {
      meleef = m->meleef;
      m->meleef = action;
      tacmonster(m);
      m->meleef = meleef;
    }
  }
  else switch(action) {

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
  case M_MOVE_ANIMAL:m_move_animal(m); break;

  case M_STRIKE_MISSILE:m_nbolt(m); break;
  case M_STRIKE_FBOLT:m_firebolt(m); break;
  case M_STRIKE_LBALL:m_lball(m); break;
  case M_STRIKE_FBALL:m_fireball(m); break;
  case M_STRIKE_SNOWBALL:m_snowball(m); break;
  case M_STRIKE_BLIND:m_blind_strike(m); break;
  case M_STRIKE_SONIC:m_strike_sonic(m); break;
    
  case M_TALK_HORSE:m_talk_horse(m); break;
  case M_TALK_THIEF:m_talk_thief(m);break;
  case M_TALK_STUPID:m_talk_stupid(m); break;
  case M_TALK_SILENT:m_talk_silent(m); break;
  case M_TALK_HUNGRY:m_talk_hungry(m); break;
  case M_TALK_GREEDY:m_talk_greedy(m); break;
  case M_TALK_TITTER:m_talk_titter(m); break;
  case M_TALK_MP:m_talk_mp(m); break;
  case M_TALK_IM:m_talk_im(m); break;
  case M_TALK_MAN:m_talk_man(m); break;
  case M_TALK_ROBOT:m_talk_robot(m); break;
  case M_TALK_EVIL:m_talk_evil(m); break;
  case M_TALK_GUARD:m_talk_guard(m); break;
  case M_TALK_MIMSY:m_talk_mimsy(m); break;
  case M_TALK_SLITHY:m_talk_slithy(m); break;
  case M_TALK_BURBLE:m_talk_burble(m); break;
  case M_TALK_BEG:m_talk_beg(m); break;
  case M_TALK_HINT:m_talk_hint(m); break;
  case M_TALK_EF:m_talk_ef(m); break;
  case M_TALK_GF:m_talk_gf(m); break;
  case M_TALK_SEDUCTOR:m_talk_seductor(m); break;
  case M_TALK_DEMONLOVER:m_talk_demonlover(m); break;
  case M_TALK_NINJA:m_talk_ninja(m); break;
  case M_TALK_ASSASSIN:m_talk_assassin(m); break;
  case M_TALK_SERVANT: m_talk_servant(m); break;
  case M_TALK_ANIMAL: m_talk_animal(m); break;
  case M_TALK_SCREAM: m_talk_scream(m); break;
  case M_TALK_PARROT: m_talk_parrot(m); break;
  case M_TALK_HYENA: m_talk_hyena(m); break;
  case M_TALK_DRUID: m_talk_druid(m); break;
  case M_TALK_ARCHMAGE: m_talk_archmage(m); break;
  case M_TALK_MERCHANT: m_talk_merchant(m); break;
  case M_TALK_PRIME: m_talk_prime(m); break;

  case M_SP_BOG:m_sp_bogthing(m); break;
  case M_SP_LEASH:m_sp_leash(m); break;
  case M_SP_WERE:m_sp_were(m); break;
  case M_SP_WHISTLEBLOWER:m_sp_whistleblower(m); break;
  case M_SP_MERCHANT:m_sp_merchant(m); break;
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
  case M_SP_GHOST:m_sp_ghost(m); break;
  case M_SP_SPELL:m_sp_spell(m); break;
  case M_SP_SEDUCTOR:m_sp_seductor(m); break;
  case M_SP_EATER:m_sp_eater(m); break;
  case M_SP_DRAGONLORD:m_sp_dragonlord(m); break;
  case M_SP_BLACKOUT:m_sp_blackout(m); break;
  case M_SP_SWARM: m_sp_swarm(m); break;
  case M_SP_ANGEL: m_sp_angel(m); break;
  case M_SP_SERVANT: m_sp_servant(m); break;
  case M_SP_AV: m_sp_av(m); break;
  case M_SP_LW: m_sp_lw(m); break;
  case M_SP_MB: m_sp_mb(m); break;
  case M_SP_RAISE: m_sp_raise(m); break;
  case M_SP_MIRROR: m_sp_mirror(m); break;
  case M_SP_COURT: m_sp_court(m); break;
  case M_SP_LAIR: m_sp_lair(m); break;
  case M_SP_PRIME: m_sp_prime(m); break;
  }
}
  
/* makes one of the highscore npc's */    
void make_hiscore_npc(npc,npcid)
pmt npc;
int npcid;
{
  int behavior,level;
  int st = -1;
  pob ob;
  *npc = Monsters[HISCORE_NPC];
  npc->aux2 = npcid;
  /* each of the high score npc's can be created here */
  switch(npcid) {
  case 0:
    level = Hilevel;
    behavior = Hibehavior;
    strcpy(Str2,Hiscorer);
    break;
  case 1:
    level = Priestlevel[1];
    behavior = Priestbehavior[1];
    strcpy(Str2,Priest[1]);
    st = ARTIFACTID+14; /* hs of odin */
    break;
  case 2:
    level = Priestlevel[2];
    behavior = Priestbehavior[2];
    strcpy(Str2,Priest[2]);
    st = ARTIFACTID+17; /* hs of set */
    break;
  case 3:
    level = Priestlevel[3];
    behavior = Priestbehavior[3];
    strcpy(Str2,Priest[3]);
    st = ARTIFACTID+16; /* hs of athena */
    break;
  case 4:
    level = Priestlevel[4];
    behavior = Priestbehavior[4];
    strcpy(Str2,Priest[4]);
    st = ARTIFACTID+15; /* hs of hecate */
    break;
  case 5:
    level = Priestlevel[5];
    behavior = Priestbehavior[5];
    strcpy(Str2,Priest[5]);
    st = ARTIFACTID+18; /* hs of druid */
    npc->talkf = M_TALK_DRUID;
    break;
  case 6:
    level = Priestlevel[6];
    behavior = Priestbehavior[6];
    strcpy(Str2,Priest[6]);
    st = ARTIFACTID+19; /* hs of destiny */
    break;
  case 7:
    level = Shadowlordlevel;
    behavior = Shadowlordbehavior;
    strcpy(Str2,Shadowlord);
    break;
  case 8:
    level = Commandantlevel;
    behavior = Commandantbehavior;
    strcpy(Str2,Commandant);
    break;
  case 9:
    level = Archmagelevel;
    behavior = Archmagebehavior;
    strcpy(Str2,Archmage);
    st = ARTIFACTID+9; /* kolwynia */
    npc->talkf = M_TALK_ARCHMAGE;
    break;
  case 10:
    level = Primelevel;
    behavior = Primebehavior;
    strcpy(Str2,Prime);
    npc->talkf = M_TALK_PRIME;
    npc->specialf = M_SP_PRIME;
    break;
  case 11:
    level = Championlevel;
    behavior = Championbehavior;
    strcpy(Str2,Champion);
    break;
  case 12:
    level = Dukelevel;
    behavior = Dukebehavior;
    strcpy(Str2,Duke);
    break;
  case 13:
    level = Chaoslordlevel;
    behavior = Chaoslordbehavior;
    strcpy(Str2,Chaoslord);
    break;
  case 14:
    level = Lawlordlevel;
    behavior = Lawlordbehavior;
    strcpy(Str2,Lawlord);
    break;
  case 15:
    st = THINGID+16;
    level = Justiciarlevel;
    behavior = Justiciarbehavior;
    strcpy(Str2,Justiciar);
    npc->talkf = M_TALK_GUARD;
    npc->specialf = M_SP_WHISTLEBLOWER;
    break;
  }
  if (st > -1) {
    ob = ((pob) malloc(sizeof(objtype)));
    *ob = Objects[st];
    m_pickup(npc,ob);
  }
  npc->monstring = salloc(Str2);
  strcpy(Str1,"The body of ");
  strcat(Str1,Str2);
  npc->corpsestr = salloc(Str1);
  determine_npc_behavior(npc,level,behavior);
}




/* sets npc behavior given level and behavior code */
void determine_npc_behavior(npc,level,behavior)
pmt npc;
int level,behavior;
{
  int combatype,competence,talktype;
  npc->hp = (level+1)*20;
  npc->status = AWAKE+MOBILE+WANDERING;
  combatype = (behavior % 100) / 10;
  competence = (behavior % 1000) / 100;
  talktype = behavior / 1000;
  npc->level = competence;
  if (npc->level < 2*difficulty()) npc->status += HOSTILE;
  npc->xpv = npc->level*20;
  switch (combatype) {
  case 1: /* melee */
    npc->meleef = M_MELEE_NORMAL;
    npc->dmg = competence*5;
    npc->hit = competence*3;
    npc->speed = 3;
    break;
  case 2: /*missile*/
    npc->meleef = M_MELEE_NORMAL;
    npc->strikef = M_STRIKE_MISSILE;
    npc->dmg = competence*3;
    npc->hit = competence*2;
    npc->speed = 4;
    break;
  case 3: /* spellcasting */
    npc->meleef = M_MELEE_NORMAL;
    npc->dmg = competence;
    npc->hit = competence;
    npc->specialf = M_SP_SPELL;
    npc->speed = 6;
    break;
  case 4: /* thievery */
    npc->meleef = M_MELEE_NORMAL;
    npc->dmg = competence;
    npc->hit = competence;
    npc->specialf=M_SP_THIEF;
    npc->speed = 3;
    break;
  case 5: /* flee */
    npc->dmg = competence;
    npc->hit = competence;
    npc->meleef = M_MELEE_NORMAL;
    npc->specialf = M_MOVE_SCAREDY;
    npc->speed = 3;
    break;
  }
  if (npc->talkf == M_TALK_MAN)
    switch (talktype) {
    case 1: npc->talkf = M_TALK_EVIL; break;
    case 2: npc->talkf = M_TALK_MAN; break;
    case 3: npc->talkf = M_TALK_HINT; break;
    case 4: npc->talkf = M_TALK_BEG; break;
    case 5: npc->talkf = M_TALK_SILENT; break;
    default: mprint("Say Whutt? (npc talk weirdness)"); break;
    }
  npc->uniqueness = UNIQUE_MADE;
}


/* makes an ordinary npc (maybe undead) */
void make_log_npc(npc)
struct monster *npc;
{
  int b,s,l,i,j,n = -1;
  int behavior,status,level;
  FILE *fd;

  /* in case the log file is null */
  behavior = 2718;
  level = 1;
  status = 2;
  strcpy(Str2,"Malaprop the Misnamed");
  
  strcpy(Str1,OMEGALIB);
  strcat(Str1,"omega.log");
  fd = fopen(Str1,"r");
  i = getc(fd);
  while(i!=EOF) {
    fscanf(fd,"%d %d %d ",&s,&l,&b);
    filescanstring(fd,Str2);
    if ((j=random_range(10000))>n) {
      status = s; 
      level = l; 
      behavior = b;
      n = j;
    }
    i = fgetc(fd);
  }
  fclose(fd);
  npc->hp = level*20;
  if (status==1) {
    if (level < 3) {
      *npc = Monsters[GHOST];
      strcpy(Str1,"ghost named ");
    }
    else if (level < 7) {
      *npc = Monsters[HAUNT];
      strcpy(Str1,"haunt named ");
    }
    else if (level < 12) {
      *npc = Monsters[SPECTRE];
      strcpy(Str1,"spectre named ");
    }
    else {
      *npc = Monsters[LICHE];
      strcpy(Str1,"lich named ");
    }
    strcat(Str1,Str2);
    npc->monstring = salloc(Str1);
    strcpy(Str3,"the mortal remains of ");
    strcat(Str3,Str2);
    npc->corpsestr = salloc(Str3);
  }
  else {
    npc->monstring=salloc(Str2);
    strcpy(Str3,"the corpse of ");
    strcat(Str3,Str2);
    npc->corpsestr = salloc(Str3);
    determine_npc_behavior(npc,level,behavior);
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
  Level->site[m->x][m->y].locchar = TRAP;
  }
  m_damage(m,difficulty()*2,NORMAL_DAMAGE);
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
  Level->site[m->x][m->y].locchar = TRAP;
  }
  if (! m_statusp(m,INTANGIBLE))
    m_status_reset(m,MOBILE);
  m_damage(m,difficulty()*5,NORMAL_DAMAGE);

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
  Level->site[m->x][m->y].locchar = TRAP;
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
  Level->site[m->x][m->y].locchar = ABYSS;
  Level->site[m->x][m->y].p_locf = L_ABYSS;
  }
  m_vanish(m);
}

void m_trap_snare(m)
struct monster *m;
{
  char Str1[80];
  Level->site[m->x][m->y].locchar = TRAP;
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
  Level->site[m->x][m->y].locchar = TRAP;
  if (los_p(m->x,m->y,Player.x,Player.y)) { 
    if (m->uniqueness != COMMON) strcpy(Str1,m->monstring);
    else {
      strcpy(Str1,"The ");
      strcat(Str1,m->monstring);
    }
    strcat(Str1," was hit by a blade trap!");
    mprint(Str1); } 
  m_damage(m,(difficulty()+1)*7-Player.defense,NORMAL_DAMAGE);
}

void m_trap_fire(m)
struct monster *m;
{
  char Str1[80];
  Level->site[m->x][m->y].locchar = TRAP;
  if (los_p(m->x,m->y,Player.x,Player.y)) { 
    if (m->uniqueness != COMMON) strcpy(Str1,m->monstring);
    else {
      strcpy(Str1,"The ");
      strcat(Str1,m->monstring);
    }
    strcat(Str1," was hit by a fire trap!");
    mprint(Str1); 
  } 
  m_damage(m,(difficulty()+1)*5,FLAME);
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
  Level->site[m->x][m->y].locchar = TRAP;
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
    Level->site[m->x][m->y].locchar = TRAP;
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
    Level->site[m->x][m->y].locchar = TRAP;
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
    Level->site[m->x][m->y].locchar = TRAP;
  } 
  m_damage(m,random_range(difficulty()*difficulty()),ACID);
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
    Level->site[m->x][m->y].locchar = TRAP;
  } 
  if (m->specialf == M_SP_SPELL) m->specialf = M_NO_OP;
}


void m_water(m)
struct monster *m;
{
  char Str1[80];
  if ((! m_statusp(m,INTANGIBLE)) && 
      (! m_statusp(m,SWIMMING)) &&
      (! m_statusp(m,ONLYSWIM))) {
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
  if ((! m_immunityp(m,FLAME)) ||
      ((! m_statusp(m,SWIMMING))&& (! m_statusp(m,ONLYSWIM)))) {
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
  int visible = view_los_p(Player.x,Player.y,m->x,m->y);
  if (m->uniqueness != COMMON) strcpy(Str1,m->monstring);
  else {
    strcpy(Str1,"The ");
    strcat(Str1,m->monstring);
  }
  strcat(Str1," walks next to an altar...");
  if (visible) mprint(Str1);
  if (Level->site[m->x][m->y].aux==Player.patron) {
    if (visible) {
      mprint("Your deity is angry!");
      mprint("A bolt of godsfire strikes the monster....");
    }
    disrupt(m->x,m->y,Player.rank[PRIESTHOOD]*50);
  }
  else if ((Player.patron == ODIN) || (Player.patron == ATHENA)) {
    if ((Level->site[m->x][m->y].aux == SET) ||
	(Level->site[m->x][m->y].aux == HECATE)) {
      if (visible) {
	mprint("The deity of the altar smiles on the monster....");
	mprint("A shaft of light zaps the altar...");
      }
      m->hp = Monsters[m->id].hp*2;
    }
    else if (visible) mprint("but nothing much seems to happen");
  }
  else if ((Player.patron == SET) || (Player.patron == HECATE)) {
    if ((Level->site[m->x][m->y].aux == ODIN) ||
	(Level->site[m->x][m->y].aux == ATHENA)) {
      if (visible) {
	mprint("The deity of the altar smiles on the monster....");
	mprint("A shaft of light zaps the altar...");
      }
      m->hp = Monsters[m->id].hp*2;
    }
    else if (visible) mprint("but nothing much seems to happen");
  }
  else if (visible) mprint("but nothing much seems to happen");
}


/* who knows what ml will point to when this is done... */
void free_mlist(ml)
pml ml;
{
  pml tml;
   while (ml != NULL) {
    tml = ml;
    ml= ml->next;
    free((char *) tml->m);
    free((char *) tml);
  }
}


char *mantype()
{
  switch(random_range(20)) {
  case 0: strcpy(Str3,"janitor"); break;
  case 1: strcpy(Str3,"beggar"); break;
  case 2: strcpy(Str3,"barbarian"); break;
  case 3: strcpy(Str3,"hairdresser"); break;
  case 4: strcpy(Str3,"accountant"); break;
  case 5: strcpy(Str3,"lawyer"); break;
  case 6: strcpy(Str3,"indian chief"); break;
  case 7: strcpy(Str3,"tinker"); break;
  case 8: strcpy(Str3,"tailor"); break;
  case 9: strcpy(Str3,"soldier"); break;
  case 10: strcpy(Str3,"spy"); break;
  case 11: strcpy(Str3,"doctor"); break;
  case 12: strcpy(Str3,"miner"); break;
  case 13: strcpy(Str3,"noble"); break;
  case 14: strcpy(Str3,"serf"); break;
  case 15: strcpy(Str3,"neer-do-well"); break;
  case 16: strcpy(Str3,"vendor"); break;
  case 17: strcpy(Str3,"dilettante"); break;
  case 18: strcpy(Str3,"surveyor"); break;
  case 19: strcpy(Str3,"jongleur"); break;
  }
  return(Str3);
}


void strengthen_death(m)
struct monster *m;
{
  pol ol = ((pol)malloc(sizeof(oltype)));
  pob scythe = ((pob)malloc(sizeof(objtype)));
  m->xpv += min(10000,m->xpv+1000);
  m->hit += min(1000,m->hit+10);
  m->dmg = min(10000,m->dmg*2);
  m->ac += min(1000,m->ac+10);
  m->speed = max(m->speed-1,1);
  m->hp = min(100000,100+m->dmg*10);
  *scythe = Objects[WEAPONID+39];
  ol->thing = scythe;
  ol->next = NULL;
  m->possessions = ol;
}




void m_no_op(m)
struct monster *m;
{
}
