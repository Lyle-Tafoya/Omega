/* omega copyright (C) by Laurence Raphael Brothers, 1987 */
/* oeffect2.c */

#include <strings.h>
#include "oglob.h"


/* from oaux */
extern void setspot(),levelchange(),gain_experience();
extern void calc_melee(),p_death(),darkenroom(),lightroom();
extern void p_damage(),hour_status_check();
extern int p_immune();
extern char *spellid();

/* from oinv */
extern void drop_at(),pickup_at(),add_item_to_pack();
extern int getitem();
extern char *itemid();

/* from oitem */
extern int i_nothing();
extern void make_potion(), make_scroll(), make_ring(), make_stick();
extern void make_armor(), make_shield(), make_weapon(), make_boots();
extern void make_cloak(),make_food(),make_artifact(),make_thing();
extern void item_use();

/* from omovef */
extern void p_movefunction();


/* from omon */
extern void m_damage(),m_status_set(),m_status_reset(),m_death();
extern struct monster *make_creature(),*m_create();
extern int m_immunityp();

/* from outil */
extern int random_range(),inbounds(),hitp(),cursed();
extern void do_los(),findspace();


/* from ogen */
extern void clear_level(),create_level();

/* from oscr */
extern void mprint(),printm(),morewait(),drawvision();
extern void draw_explosion(),erase_level(),plotmon(),menuprintitem();
extern void levelrefresh(),menuclear(),menumorewait(),showflags();
extern void show_screen(),putspot(),menuprint();
extern void clearmsg(),xredraw();
extern char mgetc();

/* from oeffect1 */
extern void heal(),level_drain(),sleep_player();

/* oeffect2 functions */
void knowledge(),flux(),displace(),breathe(),invisible(),warp(),alert();
void haste(),regenerate(),recover_stat(),augment(),truesight();
void i_chaos(),i_law(),sanctify(),hero(),level_return(),cure(),dispel();
void polymorph(),hellfire(),illuminate();



void knowledge(blessing)
int blessing;
{
  if (blessing < 0)
    mprint("You feel ignorant.");
  else {
    mprint("You feel knowledgeable!");
    menuclear();
    menuprint("Alignment:\n");
    if (Player.alignment == 0)
      menuprint("Neutral: embodying the Cosmic Balance");
    else if (abs(Player.alignment) < 5)
      menuprint("Neutral, tending toward ");
    else if (abs(Player.alignment) < 20)
      menuprint("Neutral-");
    else if (abs(Player.alignment) > 100)
      menuprint("The essence of ");
    else if (abs(Player.alignment) > 200)
      menuprint("The Ultimate Avatar of ");
    if (Player.alignment < 0) menuprint("Chaos\n");
    else if (Player.alignment > 0) menuprint("Law\n");
    menumorewait();
    menuclear();
    menuprint("Current stati:\n");
    if (Player.status[BLINDED])
      menuprint("Blinded\n");
    if (Player.status[SLOWED])
      menuprint("Slowed\n");
    if (Player.status[HASTED])
      menuprint("Hasted\n");
    if (Player.status[DISPLACED])
      menuprint("Displaced\n");
    if (Player.status[SLEPT])
      menuprint("Slept\n");
    if (Player.status[DISEASED])
      menuprint("Diseased\n");
    if (Player.status[POISONED])
      menuprint("Poisoned\n");
    if (Player.status[BREATHING])
      menuprint("Breathing\n");
    if (Player.status[INVISIBLE])
      menuprint("Invisible\n");
    if (Player.status[REGENERATING])
      menuprint("Regenerating\n");
    if (Player.status[VULNERABLE])
      menuprint("Vulnerable\n");
    if (Player.status[BERSERK])
      menuprint("Berserk\n");
    if (Player.status[IMMOBILE])
      menuprint("Immobile\n");
    if (Player.status[ALERT])
      menuprint("Alert\n");
    if (Player.status[AFRAID])
      menuprint("Afraid\n");
    if (Player.status[ACCURATE])
      menuprint("Accurate\n");
    if (Player.status[HERO])
      menuprint("Heroic\n");
    if (Player.status[LEVITATING])
      menuprint("Levitating\n");
    menumorewait();
    menuclear();
    menuprint("Immunities:\n");
    if (p_immune(NORMAL_DAMAGE))
      menuprint("Normal Damage\n");
    if (p_immune(FLAME))
      menuprint("Flame\n");
    if (p_immune(ELECTRICITY))
      menuprint("Electricity\n");
    if (p_immune(COLD))
      menuprint("Cold\n");
    if (p_immune(POISON))
      menuprint("Poison\n");
    if (p_immune(ACID))
      menuprint("Acid\n");
    if (p_immune(FEAR))
      menuprint("Fear\n");
    if (p_immune(SLEEP))
      menuprint("Sleep\n");
    if (p_immune(NEGENERGY))
      menuprint("Negative Energies\n");
    if (p_immune(THEFT))
      menuprint("Theft\n");
    if (p_immune(GAZE))
      menuprint("Gaze\n");
    if (p_immune(INFECTION))
      menuprint("Infection\n");
    menumorewait();
    menuclear();
    menuprint("Ranks:\n");
    switch(Player.rank[LEGION]) {
    case COMMANDANT:
      menuprint("Commandant of the Legion.\n"); 
      break;
    case COLONEL:    
      menuprint("Colonel of the Legion.\n");
      break;
    case FORCE_LEADER:
      menuprint("Force Leader of the Legion.\n");
      break;
    case CENTURION:
      menuprint("Centurion of the Legion.\n");
      break;
    case LEGIONAIRE:
      menuprint("Legionaire.\n");
      break;
    }
    switch(Player.rank[ARENA]) {
    case -1:
      menuprint("Ex-gladiator\n");
    case CHAMPION:
      menuprint("Gladiator Champion.\n"); 
      break;
    case GLADIATOR:    
      menuprint("Gladiator of the Arena.\n");
      break;
    case RETIARIUS:
      menuprint("Retiarius of the Arena.\n");
      break;
    case BESTIARIUS:
      menuprint("Bestiarius of the Arena.\n");
      break;
    case TRAINEE:
      menuprint("Gladiator Trainee of the Arena.\n");
      break;
    }
    switch(Player.rank[COLLEGE]) {
    case ARCHMAGE:
      menuprint("Archmage of the Collegium Magii\n");
      break;
    case MAGE:
      menuprint("Collegium Magii: Mage\n");
      break;
    case PRECEPTOR:
      menuprint("Collegium Magii: Preceptor\n");
      break;
    case STUDENT:
      menuprint("Collegium Magii: Student\n");
      break;
    case NOVICE:
      menuprint("Collegium Magii: Novice\n");
      break;
    }
    switch(Player.rank[NOBILITY]) {
    case DUKE:
      menuprint("Duke of Rampart\n");
      break;
    case LORD:
      menuprint("Peer of the Realm\n");
      break;
    case KNIGHT:
      menuprint("Order of the Knights of Rampart\n");
      break;
    case ESQUIRE:
      menuprint("Squire of Rampart\n");
      break;
    case COMMONER:
      menuprint("Commoner\n");
      break;
    default:
      menuprint("Lowly Commoner\n");
      break;
    }
    switch(Player.rank[CIRCLE]) {
    case PRIME:
      menuprint("Prime Sorceror of the Inner Circle\n");
      break;
    case HIGHSORCEROR:
      menuprint("High Sorceror of the Inner Circle\n");
      break;
    case SORCEROR:
      menuprint("Member of the Circle of Sorcerors\n");
      break;
    case ENCHANTER:
      menuprint("Member of the Circle of Enchanters\n");
      break;
    case INITIATE:
      menuprint("Member of the Circle of Initiates\n");
      break;
    }
    switch(Player.rank[THIEVES]) {
    case SHADOWLORD:
      menuprint("Guild of Thieves: Shadowlord\n");
      break;
    case TMASTER:
      menuprint("Guild of Thieves: Master Thief\n");
      break;
    case THIEF:
      menuprint("Guild of Thieves: Thief\n");
      break;
    case ATHIEF:
      menuprint("Guild of Thieves: Apprentice Thief\n");
      break;
    case TMEMBER:
      menuprint("Guild of Thieves: Candidate Member\n");
      break;
    }
    switch(Player.rank[PRIESTHOOD]) {
      case LAY:
        menuprint("A lay devotee of ");
	break;
      case ACOLYTE:
        menuprint("An Acolyte of ");
	break;
      case PRIEST:
        menuprint("A Priest of ");
	break;
      case SPRIEST:
        menuprint("A Senior Priest of ");
	break;
      case HIGHPRIEST:
        menuprint("The High Priest of ");
	break;
      }
    switch(Player.patron) {
      case ODIN:
        menuprint("Odin.\n");
	break;
      case SET:
        menuprint("Set.\n");
	break;
      case ATHENA:
        menuprint("Athena.\n");
	break;
      case HECATE:
        menuprint("Hecate.\n");
	break;
      case DRUID:
        menuprint("Druidism.\n");
	break;
      case DESTINY:
        menuprint("the Lords of Destiny.\n");
	break;
    }
    if (Player.rank[ADEPT] > 0) 
      menuprint("**************\n*Omegan Adept*\n**************\n");
    menumorewait();
    xredraw();
  }
}


/* Recreates the current level */
void flux(blessing)
int blessing;
{
  mprint("The universe warps around you!");
  if (Dlevel == 0) {
    mprint("Sensing dangerous high order magic, the Collegium Magii");
    mprint("and the Circle of Sorcerors join forces to negate the spell.");
    mprint("You are zapped by an antimagic ray!!!");
    dispel(-1);
    mprint("The universe unwarps itself....");
  }
  else {
    mprint("You stagger as the very nature of reality warps!");
    erase_level();
    clear_level(Dlevel);
    mprint("The fabric of spacetime reknits....");
    create_level(Dlevel-1,Dlevel,'<');
  }
}

/*Turns on displacement status for the player */
void displace(blessing)
int blessing;
{
  if (blessing > -1) {
      mprint("You feel a sense of dislocation.");
      Player.status[DISPLACED] = blessing + random_range(6);
    }
  else {
    mprint("You feel vulnerable");
    Player.status[VULNERABLE] += random_range(6) - blessing;
  }
}

  
	
void invisible(blessing)
int blessing;
{  
  if (blessing > -1) {
    mprint("You feel transparent!");
    Player.status[INVISIBLE]+= 2+5*blessing;
  }
  else {
    mprint("You feel dangerous!");
    Player.status[VULNERABLE] +=
      random_range(10)+1;
  }
}


void warp(blessing)
int blessing;
{
  int newlevel,ok=FALSE;
  if (blessing > -1) {
    mprint("Warp to which level? ");
    newlevel = parsenum();
    if ((newlevel > NUMLEVELS-10) && 
	(newlevel < NUMLEVELS) &&
	(Player.patron != DESTINY)) 
      mprint("Some mysterious force alters your destination.");
    else ok = TRUE;
  }
  if (! ok) {
    do newlevel=random_range(NUMLEVELS+20)-10;
    while ((newlevel>NUMLEVELS-10) && (newlevel < NUMLEVELS));
  }
  mprint("You dematerialize...");
  levelchange(newlevel,Dungeon[Dlevel][Player.x][Player.y].locchar);
  if (newlevel > 0)
    mprint("You feel homesick.");
}
  
void alert(blessing)
 int blessing;
{
  if (blessing > -1) {
    mprint("You feel a sense of insomnia.");
    Player.status[ALERT]+= 4+(5*blessing);
  }
  else sleep_player(abs(blessing)+3);
}

void regenerate(blessing)
int blessing;
{
  if (blessing < 0)
    heal(blessing * 10);
  else {
    mprint("You feel abnormally healthy.");
    Player.status[REGENERATING] += (blessing+1)*50;
  }
}

void haste(blessing)
int blessing;
{
  if (blessing > -1) {
    if (! Player.status[HASTED]) 
      mprint("The world slows down!"); 
    else mprint("Nothing much happens.");
    if (Player.status[SLOWED]) 
      Player.status[SLOWED] = 0;
    Player.status[HASTED] += (blessing*100)+random_range(250);
  }
  else {
    mprint("You feel slower.");
    if (Player.status[HASTED] > 0) mprint("...but the feeling quickly fades.");
    else 
      Player.status[SLOWED] += random_range(250)+250;
  }
}


void recover_stat(blessing)
int blessing;
{
  if (blessing < 0) {
    mprint("You feel a cold surge!");
    switch(random_range(6)) {
      case 0: Player.str = min(Player.str-1,Player.maxstr-1); break;
      case 1: Player.con = min(Player.con-1,Player.maxcon-1); break;
      case 2: Player.dex = min(Player.dex-1,Player.maxdex-1); break;
      case 3: Player.agi = min(Player.agi-1,Player.maxagi-1); break;
      case 4: Player.iq = min(Player.iq-1,Player.maxiq-1); break;
      case 5: Player.pow = min(Player.pow-1,Player.maxpow-1); break;
    }
  }
  else {
    mprint("You feel a warm tingle!");
    Player.str = max(Player.str,Player.maxstr);
    Player.con = max(Player.con,Player.maxcon);
    Player.dex = max(Player.dex,Player.maxdex);
    Player.agi = max(Player.agi,Player.maxagi);
    Player.iq = max(Player.iq,Player.maxiq);
    Player.pow = max(Player.pow,Player.maxpow);
    }
  calc_melee();
}

void augment(blessing)
int blessing;
{
  if (blessing < 0) {
    mprint("You feel a cold surge!");
    switch(random_range(6)) {
      case 0: Player.str = min(Player.str-1,Player.maxstr-1); break;
      case 1: Player.con = min(Player.con-1,Player.maxcon-1); break;
      case 2: Player.dex = min(Player.dex-1,Player.maxdex-1); break;
      case 3: Player.agi = min(Player.agi-1,Player.maxagi-1); break;
      case 4: Player.iq = min(Player.iq-1,Player.maxiq-1); break;
      case 5: Player.pow = min(Player.pow-1,Player.maxpow-1); break;
    }
  }
  else if (blessing == 0) {
    mprint("You feel a warm tingle!");
    switch(random_range(6)) {
      case 0: Player.str = max(Player.str+1,Player.maxstr+1); break;
      case 1: Player.con = max(Player.con+1,Player.maxcon+1); break;
      case 2: Player.dex = max(Player.dex+1,Player.maxdex+1); break;
      case 3: Player.agi = max(Player.agi+1,Player.maxagi+1); break;
      case 4: Player.iq = max(Player.iq+1,Player.maxiq+1); break;
      case 5: Player.pow = max(Player.pow+1,Player.maxpow+1); break;
    }
  }
  else {
    mprint("You feel a hot flash!");
    Player.str = max(Player.str+1,Player.maxstr+1);
    Player.con = max(Player.con+1,Player.maxcon+1);
    Player.dex = max(Player.dex+1,Player.maxdex+1);
    Player.agi = max(Player.agi+1,Player.maxagi+1);
    Player.iq = max(Player.iq+1,Player.maxiq+1);
    Player.pow = max(Player.pow+1,Player.maxpow+1);
  }
  calc_melee();
}

void breathe(blessing)
int blessing;
{
  if (blessing > -1) {
      mprint("Your breath is energized!");
      Player.status[BREATHING] += 6+blessing;
    }
  else {
    mprint("You choke as your lungs fill with water!");
    p_damage(50,UNSTOPPABLE,"drowning");
  }
}


void i_chaos(o)
pob o;
{
  if (Player.alignment < 0) {
    Player.alignment -= random_range(20);
    mprint("You feel deliciously chaotic!");
    gain_experience(abs(Player.alignment)*10);
  }
  else {
    mprint("You feel a sense of inner turmoil!");
    Player.alignment -= random_range(20);
  }
}

void i_law(o)
pob o;
{
  if (Player.alignment > 0) {
    Player.alignment += random_range(20);
    mprint("You feel wonderfully lawful!");
    gain_experience(Player.alignment*10);
  }
  else {
    mprint("You feel a sense of inner constraint!");
    Player.alignment += random_range(20);
  }
}

void sanctify(blessing)
int blessing;
{
  if (blessing > -1) {
    if (Dungeon[Dlevel][Player.x][Player.y].locchar == ALTAR) 
      mprint("This site can't get any holier!");
    else if (Player.patron == 0) {
      mprint("The gods are angered!");
      Dungeon[Dlevel][Player.x][Player.y].locchar = LAVA;
      Dungeon[Dlevel][Player.x][Player.y].p_locf = L_LAVA;
      p_movefunction(L_LAVA);
    }
    else {
      Dungeon[Dlevel][Player.x][Player.y].locchar = ALTAR;
      Dungeon[Dlevel][Player.x][Player.y].aux = Player.patron;
      Dungeon[Dlevel][Player.x][Player.y].p_locf = L_ALTAR;
      mprint("You are standing on sacred ground!");
    }
  }
  else {
    if (Dungeon[Dlevel][Player.x][Player.y].locchar == ALTAR) {
      mprint("The altar crumbles before your unholy blast....");
      Dungeon[Dlevel][Player.x][Player.y].locchar = FLOOR;
      Dungeon[Dlevel][Player.x][Player.y].p_locf = L_NO_OP;
      if (Dungeon[Dlevel][Player.x][Player.y].aux == Player.patron) {
	mprint("Your deity is not amused....");
	p_damage(Player.hp-1,UNSTOPPABLE,"Divine Wrath");
      }
      else if ((Player.patron == ATHENA) || (Player.patron == ODIN)) {
	if ((Dungeon[Dlevel][Player.x][Player.y].aux == SET) ||
	    (Dungeon[Dlevel][Player.x][Player.y].aux == HECATE)) {
	  mprint("Your deity applauds the eradication of Chaos' taint");
	  gain_experience(1000);
	}
	else {
	  mprint("Your deity approves of your action.");
	  gain_experience(100);
	}
      }
      if ((Player.patron == SET) || (Player.patron == HECATE)) {
	if ((Dungeon[Dlevel][Player.x][Player.y].aux == ODIN) ||
	    (Dungeon[Dlevel][Player.x][Player.y].aux == ATHENA)) {
	  mprint("Your deity applauds the obliteration of Law");
	  gain_experience(1000);
	}
	else {
	  mprint("Your deity approves of your action.");
	  gain_experience(100);
	}
      }
      else if (Player.patron == DRUID) {
	mprint("Your attempt to maintain the Balance is applauded....");
	gain_experience(250);
      }
      else mprint("Nothing much happens");
    }
    else mprint("You feel an aura of unholiness arising from this spot....");
  }
}

void accuracy(blessing)
int blessing;
{
  if (blessing > -1) {
      mprint("Everything seems covered in bullseyes!");
      Player.status[ACCURACY] += random_range(5)+1+blessing*5;
      calc_melee();
    }
  else {
    Player.status[ACCURACY]=0;
    calc_melee();
    mprint("Your vision blurs...");
  }
}

void hero(blessing)
int blessing;
{
  if (blessing > -1) {
      mprint("You feel super!");
      Player.status[HERO] += random_range(5)+1+blessing;
      calc_melee();
    }
  else {
    Player.status[HERO]=0;
    calc_melee();
    mprint("You feel cowardly.");
    level_drain(abs(blessing),"a potion of cowardice");
  }
}


void levitate(blessing)
int blessing;
{
  if (blessing > -1) {
      mprint("You start to float a few inches above the floor.");
      mprint("You discover you can easily control your altitude...");
      Player.status[LEVITATING] += random_range(5)+1+blessing;
    }
    else mprint("Nothing much happens.");
}


/* has effect of switching between city and lowest level attained */
void level_return()
{
  if (Dlevel > 0)
    levelchange(0,Dungeon[Dlevel][Player.x][Player.y].locchar);
  else levelchange(Deepest,Dungeon[Dlevel][Player.x][Player.y].locchar);
}


void cure(blessing)
int blessing;
{
  if (blessing > -1) {
    if (Player.status[DISEASED]) {
      Player.status[DISEASED]=0;
      mprint("You feel hygienic!");
    }
    else mprint("Nothing much happens.");
  }
  else if (! Player.immunity[INFECTION]) {
    mprint("You feel tainted.");
    Player.status[DISEASED]+=random_range(10)+1;
  }
  else mprint("Nothing much happens.");
  showflags();
}


void truesight(blessing)
int blessing;
{
  if (blessing > -1) {
    Player.status[TRUESIGHT]+=random_range(10)+1;
    mprint("You feel sharp.");
  }
  else {
    Player.status[BLINDED]+=random_range(10)+1;
    mprint("You've been blinded!");
  }
}



void dispel(blessing)
int blessing;     
{
  int i,x=Player.x,y=Player.y;
  if (blessing > -1) {
    setspot(&x,&y);
    if ((x==Player.x)&&(y==Player.y)) {
      for(i=0;i<MAXITEMS;i++) {
	if (Player.possessions[i]!=NULL)
	  if ((Player.possessions[i]->used) &&
	      (Player.possessions[i]->blessing < 0)) {
	    if (blessing+1 + Player.possessions[i]->blessing >=0) {
	      mprint("You hear a sighing sound from");
	      mprint(itemid(Player.possessions[i]));
	      Player.possessions[i]->blessing = 0;
	    }
	    else {
	      mprint("You hear dark laughter from");
	      mprint(itemid(Player.possessions[i]));
	    }
	  }
      }
    }
    else if (Dungeon[Dlevel][x][y].creature != NULL) {
      if (Dungeon[Dlevel][x][y].creature->level < blessing * 3) {
	Dungeon[Dlevel][x][y].creature->specialf = M_NO_OP;
	if (Dungeon[Dlevel][x][y].creature->meleef != M_NO_OP)
	  Dungeon[Dlevel][x][y].creature->meleef = M_MELEE_NORMAL;
	Dungeon[Dlevel][x][y].creature->strikef = M_NO_OP;
	Dungeon[Dlevel][x][y].creature->immunity=0;
	m_status_reset(Dungeon[Dlevel][x][y].creature,M_INVISIBLE);	
	m_status_reset(Dungeon[Dlevel][x][y].creature,INTANGIBLE);
      }
      else mprint("The monster ignores the effect!");
    }
    else if ((Dungeon[Dlevel][x][y].p_locf == L_TRAP_FIRE) ||
	     (Dungeon[Dlevel][x][y].p_locf == L_STATUE_WAKE) ||
	     (Dungeon[Dlevel][x][y].p_locf == L_TRAP_TELEPORT) ||
	     (Dungeon[Dlevel][x][y].p_locf == L_TRAP_DISINTEGRATE)) {
      Dungeon[Dlevel][x][y].p_locf == L_NO_OP;
      if (Dungeon[Dlevel][x][y].locchar == TRAP)
	Dungeon[Dlevel][x][y].locchar = FLOOR;
    }
    else if (Dungeon[Dlevel][x][y].p_locf == L_MAGIC_POOL)
      Dungeon[Dlevel][x][y].p_locf = L_WATER;
    else mprint("Nothing much seems to happen.");
  }
  else {
    mprint("A smell of ozone and positive ions fills the air..");
    if (Player.status[ACCURACY] && (Player.status[ACCURACY] < 1000))
      Player.status[ACCURACY]=1;
    if (Player.status[DISPLACED]&&(Player.status[DISPLACED] < 1000))
	Player.status[DISPLACED]=1;
    if (Player.status[HASTED]&&(Player.status[HASTED] < 1000))
      Player.status[HASTED]=1;
    if (Player.status[BREATHING]&&(Player.status[BREATHING] < 1000))
      Player.status[BREATHING]=1;
    if (Player.status[INVISIBLE]&&(Player.status[INVISIBLE] < 1000))
	Player.status[INVISIBLE]=1;
    if (Player.status[REGENERATING]&&(Player.status[REGENERATING] < 1000))
      Player.status[REGENERATING]=1;
    if (Player.status[ALERT]&&(Player.status[ALERT] < 1000))
	Player.status[ALERT]=1;
    if (Player.status[HERO]&&(Player.status[HERO] < 1000))
	Player.status[HERO]=1;
    if (Player.status[LEVITATING]&&(Player.status[LEVITATING] < 1000))
	Player.status[LEVITATING]=1;
    if (Player.status[ACCURATE]&&(Player.status[ACCURATE] < 1000))
	Player.status[ACCURATE]=1;
    if (Player.status[TRUESIGHT]&&(Player.status[TRUESIGHT] < 1000))
	Player.status[TRUESIGHT]=1;
    hour_status_check();
  }
}


void polymorph(blessing)
int blessing;     
{
  int i,x=Player.x,y=Player.y,newmonster;
  struct monster *m;
  setspot(&x,&y);
  if ((x==Player.x)&&(y==Player.y)) {
    mprint("You enjoy your new life as a");
    mprint(Monsters[random_range(NUMMONSTERS)].monstring);
    mprint("But your game is over....");
    p_death("polymorphing oneself");
  }
  else if ((m=Dungeon[Dlevel][x][y].creature) == NULL)
    mprint("Nothing happens.");
  else {
    if (m_immunityp(m,OTHER_MAGIC) || (m->level > random_range(12))) {
      strcpy(Str1,"The ");
      strcat(Str1,m->monstring);
      strcat(Str1," resists the change!");
      m_status_set(m,HOSTILE);
    }
    else {
      if (blessing < 0) {
	do newmonster = random_range(NUMMONSTERS);
	while ((newmonster == ML0+4) ||
	       (newmonster == ML7+3) ||
	       (Monsters[newmonster].level < m->level) ||
	       (Monsters[newmonster].uniqueness != COMMON));
      }
      else {
	do newmonster = random_range(NUMMONSTERS);
	while ((newmonster == ML0+4) ||
	       (newmonster == ML7+3) ||
	       (Monsters[newmonster].uniqueness != COMMON));
      }
      *m = Monsters[newmonster];
      m_status_set(m,HOSTILE);
    }
  }
}




void hellfire(x,y,blessing)
int x,y,blessing;
{
  int i;
  struct monster *m;
  if ((x==Player.x)&&(y==Player.y)) {
    mprint("You have been completely annihilated. Congratulations.");
    p_death("hellfire");
  }
  else if ((m=Dungeon[Dlevel][x][y].creature) == NULL) {
    mprint("The gods are angry over your waste of power...");
    level_drain(5,"indiscriminate use of hellfire");
  }
  else {
    mprint("The monster writhes in the flames...");
    if (blessing < 0) {
      mprint("...and appears stronger.");
      morewait();
      mprint("Much stronger.");
      m->hp += 1000;
      m->hit +=20;
      m->dmg += 100;
      m->actions += 4;
      m_status_set(m,HOSTILE);
    }
    else {
      mprint("and is utterly annihilated. Only a greasy spot remains...");
      strcpy(m->corpsestr,"a greasy spot");
      m->corpsevalue = 0;
      m->possessions = NULL; /* it happens that that transformid is -1 */
      m_death(m);
    }
  }
}


void drain(blessing)
int blessing;
{
  int i,x=Player.x,y=Player.y;
  struct monster *m;
  setspot(&x,&y);
  mprint("You begin to drain energy...");
  if ((x==Player.x)&&(y==Player.y)) {
    mprint("You drain your own energy....");
    mprint("Uh, oh, positive feedback....");
    level_drain(Player.level,"self-vampirism");
  }
  else if ((m=Dungeon[Dlevel][x][y].creature) != NULL) {
    if ((blessing > -1) && (! m_immunityp(m,NEGENERGY))) {
      mprint("The monster seems weaker...");
      m_damage(m,NEGENERGY,m->level*m->level);
      m->hit -= m->level;
      m->dmg -= m->level*m->level;
      m->ac -= m->level;
      m->level--;
      mprint("You feel stronger...");
      gain_experience(m->level*5);
      Player.hp+=(m->level*m->level / 2);
    }
    else {
      mprint("The effect reverses itself!");
      mprint("The monster seems stronger...");
      m->hp+=Player.level*Player.level;
      m->hit += Player.level;
      m->dmg += Player.level*Player.level;
      m->ac += Player.level;
      m->level++;
      mprint("You feel weaker...");
      Player.mana = min(0,Player.level*Player.level);
      level_drain(m->level,"negative energy conflict.");
    }
  }
  else if (blessing < 1) {
    mprint("You seem to lose energy, instead of gaining it!");
    level_drain(3,"reversed energy drain");
  }
  else if (Dungeon[Dlevel][x][y].locchar == WHIRLWIND) {
    Dungeon[Dlevel][x][y].locchar = FLOOR;
    Dungeon[Dlevel][x][y].p_locf = L_NO_OP;
    if (p_immune(ELECTRICITY)) {
      mprint("You transform some electrical energy.");
      gain_experience(20);
      Player.hp += 20;
      Player.str++;
      Player.con++;
    }
    else {
      mprint("The storm's lightning zaps you as you try to drain it!");
      p_damage(40,UNSTOPPABLE,"an attempt to drain energy from a whirlwind");
    }
  }
  else if (Dungeon[Dlevel][x][y].locchar == ALTAR) {
    mprint("The altar collapses in on itself....");
    Dungeon[Dlevel][x][y].locchar = ABYSS;
    Dungeon[Dlevel][x][y].p_locf = L_ABYSS;
    if (! Player.patron) {
      mprint("You drain some theurgic energy from the altar....");
      gain_experience(40);
      Player.hp += 20;
      Player.pow+=2;
    }
    if (Dungeon[Dlevel][x][y].aux == Player.patron) {
      mprint("Your deity is enraged.");
      mprint("You are struck by godsfire.");
      p_damage(Player.hp-1,UNSTOPPABLE,"godsfire");
      mprint("You feel atheistic.");
      Player.patron = -1;
      Player.rank[PRIESTHOOD] = 0;
    }
    else {
      mprint("You feel the wrath of a god....");
      p_damage(random_range(Player.level*10),UNSTOPPABLE,"divine wrath");
      mprint("Your deity doesn't seem to mind your action, though.");
      gain_experience(100);
    }
  }
  else {
    mprint("You drain some energy from the ambient megaflow.");
    Player.hp++;
  }
}

void sanctuary()
{
  mprint("You're standing on sacred ground!");
  Player.sx = Player.x;
  Player.sy = Player.y;
  Player.slevel = Dlevel;
}

void invulnerable()
{
  int i;
  mprint("You feel invincible.");
  for(i=0;i<NUMIMMUNITIES;i++)
    Player.immunity[i]+=20;
}

void shadowform()
{
  int i;
  if (Dlevel == NUMLEVELS-1) 
    mprint("Some force seems to block the operation of the effect!");
  mprint("You feel like a shadow.");
  Player.immunity[NORMAL_DAMAGE]++;
  Player.immunity[ACID]++;
  Player.immunity[THEFT]++;
  Player.immunity[INFECTION]++;
  Player.status[SHADOWFORM]+=Player.level;
}

void illuminate(blessing)
int blessing;
{
  int r;
  if (blessing > -1) {
    if (r =Dungeon[Dlevel][Player.x][Player.y].roomnumber > 0) {
      if (Leveldata[Dlevel].rooms[r].lighted)
	mprint("Nothing much happens.");
      else {
	mprint("The room lights up!");
	Leveldata[Dlevel].rooms[r].lighted = TRUE;
	lightroom(r);
      }
    }
    else mprint("You see a faint glimmer of light which quickly fades.");
  }
  else {
    if (r = Dungeon[Dlevel][Player.x][Player.y].roomnumber > 0) {
      if (! Leveldata[Dlevel].rooms[r].lighted)
	mprint("Nothing much happens.");
      else {
	mprint("The room darkens!");
	Leveldata[Dlevel].rooms[r].lighted = FALSE;
	darkenroom(r);
      }
    }
    else mprint("The gloom thickens for a moment.");
  }
}
