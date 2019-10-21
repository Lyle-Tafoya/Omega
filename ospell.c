/* omega copyright (C) by Laurence Raphael Brothers, 1987 */
/* ospell.c */
/* functions having to do with spellcasting */

#include <strings.h>
#include "oglob.h"

/* from outil */
extern int strprefix();


/* from oscr */
extern void menuspellprint(),mprint(),printm(),menuclear(),dobackspace();
extern void maddch(),xredraw(),menuprint();
extern char mgetc();

/* from oaux */
extern void findspace(),setspot();

/* from oeffect 1 or 2*/
extern void identify(),mondet(),objdet(),fbolt(),lball(),nbolt();
extern void sleep_monster(),disintegrate(),heal(),enchant(),cure();
extern void disrupt(),knowledge(),regenerate(),displace(),truesight();
extern void augment(),invisible(),warp(),alert(),flux(),haste();
extern void recover_stat(),breathe(),apport(),clairvoyance(),sanctuary();
extern void bless(),summon(),p_teleport(),hero(),levitate(),accuracy();
extern void sanctify(),dispel(),polymorph(),hellfire();
extern void invulnerable(),shadowform();

/* ospell functions */
void s_firebolt(),s_lball(),s_sleep(),s_objdet(),s_mondet(),s_identify();
void s_teleport(),s_disrupt(),s_disintegrate(),s_missile(),drain();
void s_heal(),s_dispel(),s_breathe(),s_invisible(),s_warp(),s_enchant();
void s_bless(),s_truesight(),s_hellfire(),s_knowledge(),s_hero();
void s_restore(),s_cure(),s_return(),s_desecrate(),s_summon();
void s_accuracy(),s_ritual(),s_apport(),s_shadowform(),s_alert();
void s_regenerate(),s_sanctify(),s_clairvoyance(),s_drain(),s_polymorph();
void s_invulnerable();


void initspells(),expandspellabbrevs();
int getspell(),expandspell();
char *spellid();

void s_firebolt()
{
  int x=Player.x,y=Player.y;
  setspot(&x,&y);
  fbolt(Player.x,Player.y,x,y,Player.dex*2+Player.level,Player.level*10+10);
}

void s_missile()
{
  int x=Player.x,y=Player.y;
  setspot(&x,&y);
  nbolt(Player.x,Player.y,x,y,Player.dex*2+Player.level,Player.level*3+3);
}

void s_teleport()
{
  p_teleport(0);
}
/* disrupt */
void s_disrupt()
{
  int x=Player.x,y=Player.y;
  setspot(&x,&y);
  disrupt(x,y,Player.level*10+25);
}

/* disintegrate */
void s_disintegrate()
{
  int x=Player.x,y=Player.y;
  setspot(&x,&y);
  disintegrate(x,y);
}

/* sleep spell */
void s_sleep()
{
  sleep_monster(0);
}

void s_heal()
{
  heal(3);
}

void s_dispel()
{
  dispel((Player.level+Player.maxpow)/10); 
}

void s_breathe()
{
  breathe(0);
}

void s_invisible()
{
  invisible(0);
}


void s_warp()
{
  warp(1);
}

void s_enchant()
{
  enchant(1);
}


void s_bless()
{
  bless(0);
}


void s_restore()
{
  recover_stat(0);
}


void s_cure()
{
  cure(0);
}


void s_truesight()
{
  truesight(0);
}


void s_hellfire()
{
  int x=Player.x,y=Player.y;
  setspot(&x,&y);
  hellfire(x,y,0);
}


void s_knowledge()
{
  knowledge(0);
}


void s_hero()
{
  hero(0);
}

/* spell works immediately only from city level */
void s_return()
{
  if (Dlevel == NUMLEVELS-1) 
    mprint("Your spell doesn't seem to work.");
  else {
    mprint("You hear a whine as your spell begins to charge up.");
    Player.status[RETURNING] = 
      (Dlevel == 0 ? 3 : 200-(Player.level+Player.pow));
  }
}

void s_desecrate()
{
  sanctify(-1);
}


void s_haste()
{
  haste(0);
}


void s_summon()
{
  summon(1,-1);
}


void s_sanctuary()
{
  sanctuary();
}

void s_sanctify()
{
  sanctify(1);
}


void s_accuracy()
{
  accuracy(0);
}

/* Has all kinds of effects in different circumstances.
   Eventually will be more interesting */
void s_ritual()
{
  static int ritualhour = -1; /* only 1 ritual per hour */
  static int lastroom = -100;
  pob symbol;
  int i,j,roomno;
  mprint("You begin your ritual....");
  mprint("You enter a deep trance. Time Passes...");
  Skipplayer = TRUE;
  time_clock();
  Skipplayer = TRUE;
  time_clock();
  Skipplayer = TRUE;
  time_clock();
  Skipplayer = TRUE;
  time_clock();
  Skipplayer = TRUE;
  time_clock();
  if (ritualhour == hour())
    mprint("Your mental fatigue prevents from completing the ritual!");
  else if (random_range(100) > Player.iq+Player.pow+Player.level)
    mprint("Your concentration was broken -- the ritual fails!");
  else {
    mprint("You charge the ritual with magical energy and focus your will.");
    mprint("Time Passes...");
    Skipplayer = TRUE;
    time_clock();
    Skipplayer = TRUE;
    time_clock();
    Skipplayer = TRUE;
    time_clock();
    Skipplayer = TRUE;
    time_clock();
    Skipplayer = TRUE;
    time_clock();
    ritualhour = hour();
    /* set of random conditions for different ritual effects */
    if (Dlevel == 0) {
      mprint("Flowing waves of mystical light congeal all around you.");
      mprint("'Like wow, man! Colors!'");
      mprint("Appreciative citizens throw you spare change.");
      Player.cash +=random_range(50);
    }
    else if ((roomno=Dungeon[Dlevel][Player.x][Player.y].roomnumber) >= 0) {
      if (lastroom == Leveldata[Dlevel].rooms[roomno].rsi)
	mprint("For some reason the ritual doesn't work this time...");
      else {
	lastroom = Leveldata[Dlevel].rooms[roomno].rsi;
	switch (lastroom) {
	case 6: /* charred wall room */
	  mprint("Blasts of fire fill the room!!!");
	  for(i=Leveldata[Dlevel].rooms[roomno].left+1;
	      i<Leveldata[Dlevel].rooms[roomno].right;
	      i++)
	    for(j=Leveldata[Dlevel].rooms[roomno].top+1;
		j<Leveldata[Dlevel].rooms[roomno].bottom;
		j++)
	      fbolt(Player.x,Player.y,i,j,100,100);
	  break;
	case 16: /* ransacked treasure chamber */
	  mprint("Your spell sets off frenetic growth all around you!");
	  for(i=0;i<8;i++){
	    Dungeon[Dlevel][Player.x+Dirs[0][i]][Player.y+Dirs[1][i]].locchar =
	      HEDGE;
	    Dungeon[Dlevel][Player.x+Dirs[0][i]][Player.y+Dirs[1][i]].p_locf =
	      L_TRIFID;
	  }
	  break;
	case 22: /* boudoir */
	  mprint("A secret chamber opens next to the bed....");
	  if (random_range(2))
	    summon(0,ML4+6); /* succubus/incubus */
	  else summon(0,ML4+7); /* satyr/nymph */
	  break;
	case 25: /* sewer control room */
	  mprint ("Louvers open and the room fills with water!");
	  for(i=Leveldata[Dlevel].rooms[roomno].left+1;
	      i<Leveldata[Dlevel].rooms[roomno].right;
	      i++)
	  for(j=Leveldata[Dlevel].rooms[roomno].top+1;
	      j<Leveldata[Dlevel].rooms[roomno].bottom;
	      j++) {
	    Dungeon[Dlevel][i][j].locchar = WATER;
	    Dungeon[Dlevel][i][j].p_locf = L_WATER;
	    if (Dungeon[Dlevel][i][j].creature != NULL)
	      m_water(Dungeon[Dlevel][i][j].creature);
	  }
	  break;
	case 26: /* magician's lab */
	  mprint("Your magical activity sets off a latent spell in the lab!");
	  (*(Spells[random_range(NUMSPELLS)].castf))();
	  break;
	case 27: /* pentagram room */
	  mprint("A smoky form begins to coalesce....");
	  summon(-1,-1);
	  mprint("Fortunately, it seems confined to the pentagram.");
	  m_status_reset(Mlist[Dlevel]->m,MOBILE);
	  break;
	case 28: /* blue omega room */
	  mprint("The Lords of Destiny look upon you....");
	  if (Player.patron == DESTINY) {
	    mprint("Your patrons take pity on you.");
	    if ((Player.rank[PRIESTHOOD]<SPRIEST) &&
		(! ((inpack(ARTIFACTID+19,-1))+1))) {
	      symbol = ((pob) calloc(1,sizeof(objtype)));
	      *symbol = Objects[ARTIFACTID+19];
	      symbol->charge = 17;
	      add_item_to_pack(symbol);
	      mprint("You feel uplifted.");
	    }
	    else gain_experience(min(1000,Player.xp));
	  }
	  else if (random_range(3)==1) {
	    mprint("You feel Fated.");
	    gain_experience(Player.level*Player.level*10);
	    Player.hp = Player.maxhp;
	  }
	  else if (random_range(2)) {
	    mprint("You feel Doomed.");
	    Player.hp = 1;
	    Player.mana = 0;
	    Player.xp = 0;
	  }
	  else mprint("The Lords of Destiny laugh at you!");
	  break;
	default:
	  mprint("Well, not much effect. Chalk it up to experience.");
	  gain_experience(Player.level*5);
	  break;
	}
      }
    }
    else {
      if (lastroom == Dungeon[Dlevel][Player.x][Player.y].roomnumber)
	mprint("The ritual fails for some unexplainable reason.");
      else {
	lastroom = Dungeon[Dlevel][Player.x][Player.y].roomnumber;
	switch (lastroom) {
	case RS_WALLSPACE:
	  shadowform();
	  break;
	case RS_CORRIDOR:
	  haste(0);
	  break;
	case RS_PONDS:
	  breathe(0);
	  break;
	case RS_ADEPT:
	  hero(1);
	  break;
	default:
	  mprint("The ritual doesn't seem to produce any tangible results...");
	  gain_experience(Player.level*6);
	}
      }
    }
  }
}


void s_invulnerable()
{
  invulnerable();
}

void s_apport()
{
  apport(0);
}

void s_shadowform()
{
  shadowform();
}

void s_alert()
{
  alert(0);
}

void s_regenerate()
{
  regenerate(0);
}

void s_clairvoyance()
{
  clairvoyance(10);
}

void s_drain()
{
  drain(0);
}

void s_levitate()
{
  levitate(0);
}

void s_polymorph()
{
  polymorph(0);
}


/* lball spell */
void s_lball()
{
  int x=Player.x,y=Player.y;
  setspot(&x,&y);
  lball(Player.x,Player.y,x,y,Player.level*10+10);
}

void s_identify()
{
  identify(0);
}

void s_objdet()
{
  objdet(0);
}

void s_mondet()
{
  mondet(0);
}


/* select a spell to cast */
int getspell()
{
  int spell= -2;
  do {
    mprint("\nCast Spell: [type spell abbrev, ?, or ESCAPE]: ");
    spell = spellparse();
  } while (spell < ABORT);
  return(spell);
}


char *spellid(id)
int id;
{
  switch(id) {
    case S_MON_DET:return("monster detection");break;
    case S_OBJ_DET:return("object detection");break;
    case S_IDENTIFY:return("identification");break;
    case S_FIREBOLT:return("firebolt");break;
    case S_LBALL:return("ball lightning");break;
    case S_SLEEP:return("sleep");break;
    case S_DISRUPT:return("disrupt");break;
    case S_DISINTEGRATE:return("disintegrate");break;
    case S_TELEPORT:return("teleport");break;
    case S_MISSILE:return("magic missile"); break;
    case S_HEAL:return("healing"); break;
    case S_DISPEL:return("dispelling"); break;
    case S_BREATHE:return("breathing"); break;
    case S_INVISIBLE:return("invisibility"); break;
    case S_WARP:return("the warp"); break;
    case S_ENCHANT:return("enchantment"); break;
    case S_BLESS:return("blessing"); break;
    case S_RESTORE:return("restoration"); break;
    case S_CURE:return("curing"); break;
    case S_TRUESIGHT:return("true sight"); break;
    case S_HELLFIRE:return("hellfire"); break;
    case S_KNOWLEDGE:return("self knowledge"); break;
    case S_HERO:return("heroism"); break;
    case S_RETURN:return("return"); break;
    case S_DESECRATE:return("desecration"); break;
    case S_HASTE:return("haste"); break;
    case S_SUMMON:return("summoning"); break;
    case S_SANCTUARY:return("sanctuary"); break;
    case S_ACCURACY:return("accuracy"); break;
    case S_RITUAL:return("ritual magic"); break;
    case S_INVULNERABLE:return("invulnerability"); break;
    case S_APPORT:return("apportation"); break;
    case S_SHADOWFORM:return("shadow form"); break;
    case S_ALERT:return("alertness"); break;
    case S_REGENERATE:return("regeneration"); break;
    case S_SANCTIFY:return("sanctification"); break;
    case S_CLAIRVOYANCE:return("clairvoyance"); break;
    case S_DRAIN:return("energy drain"); break;
    case S_LEVITATE:return("levitate"); break;
    case S_POLYMORPH:return("polymorph"); break;
    default:return("???"); break;
  }      
}



void initspells()
{
  int i;

  for (i=0; i<NUMSPELLS; i++)
    Spells[i].known = FALSE;

  Spells[S_MON_DET].powerdrain = 3;
  Spells[S_MON_DET].castf = s_mondet;
  Spells[S_MON_DET].id = S_MON_DET;

  Spells[S_OBJ_DET].powerdrain = 3;
  Spells[S_OBJ_DET].castf = s_objdet;
  Spells[S_OBJ_DET].id = S_OBJ_DET;

  Spells[S_IDENTIFY].powerdrain = 10;
  Spells[S_IDENTIFY].castf = s_identify;
  Spells[S_IDENTIFY].id = S_IDENTIFY;

  Spells[S_FIREBOLT].powerdrain = 20;
  Spells[S_FIREBOLT].castf = s_firebolt;
  Spells[S_FIREBOLT].id = S_FIREBOLT;

  Spells[S_SLEEP].powerdrain = 15;
  Spells[S_SLEEP].castf = s_sleep;
  Spells[S_SLEEP].id = S_SLEEP;

  Spells[S_LBALL].powerdrain = 25;
  Spells[S_LBALL].castf = s_lball;
  Spells[S_LBALL].id = S_LBALL;

  Spells[S_TELEPORT].powerdrain = 20;
  Spells[S_TELEPORT].castf = s_teleport;
  Spells[S_TELEPORT].id = S_TELEPORT;

  Spells[S_DISRUPT].powerdrain = 30;
  Spells[S_DISRUPT].castf = s_disrupt;
  Spells[S_DISRUPT].id = S_DISRUPT;

  Spells[S_DISINTEGRATE].powerdrain = 40;
  Spells[S_DISINTEGRATE].castf = s_disintegrate;
  Spells[S_DISINTEGRATE].id = S_DISINTEGRATE;

  Spells[S_MISSILE].powerdrain = 10;
  Spells[S_MISSILE].castf = s_missile;
  Spells[S_MISSILE].id = S_MISSILE;

  Spells[S_HEAL].powerdrain = 15;
  Spells[S_HEAL].castf = s_heal;
  Spells[S_HEAL].id = S_HEAL;

  Spells[S_DISPEL].powerdrain = 40;
  Spells[S_DISPEL].castf = s_dispel;
  Spells[S_DISPEL].id = S_DISPEL;

  Spells[S_BREATHE].powerdrain = 20;
  Spells[S_BREATHE].castf = s_breathe;
  Spells[S_BREATHE].id = S_BREATHE;

  Spells[S_INVISIBLE].powerdrain = 15;
  Spells[S_INVISIBLE].castf = s_invisible;
  Spells[S_INVISIBLE].id = S_INVISIBLE;

  Spells[S_WARP].powerdrain = 50;
  Spells[S_WARP].castf = s_warp;
  Spells[S_WARP].id = S_WARP;

  Spells[S_ENCHANT].powerdrain = 30;
  Spells[S_ENCHANT].castf = s_enchant;
  Spells[S_ENCHANT].id = S_ENCHANT;

  Spells[S_BLESS].powerdrain = 30;
  Spells[S_BLESS].castf = s_bless;
  Spells[S_BLESS].id = S_BLESS;

  Spells[S_RESTORE].powerdrain = 20;
  Spells[S_RESTORE].castf = s_restore;
  Spells[S_RESTORE].id = S_RESTORE;

  Spells[S_CURE].powerdrain = 20;
  Spells[S_CURE].castf = s_cure;
  Spells[S_CURE].id = S_CURE;

  Spells[S_TRUESIGHT].powerdrain = 20;
  Spells[S_TRUESIGHT].castf = s_truesight;
  Spells[S_TRUESIGHT].id = S_TRUESIGHT;

  Spells[S_HELLFIRE].powerdrain = 100;
  Spells[S_HELLFIRE].castf = s_hellfire;
  Spells[S_HELLFIRE].id = S_HELLFIRE;

  Spells[S_KNOWLEDGE].powerdrain = 10;
  Spells[S_KNOWLEDGE].castf = s_knowledge;
  Spells[S_KNOWLEDGE].id = S_KNOWLEDGE;

  Spells[S_HERO].powerdrain = 20;
  Spells[S_HERO].castf = s_hero;
  Spells[S_HERO].id = S_HERO;

  Spells[S_RETURN].powerdrain = 10;
  Spells[S_RETURN].castf = s_return;
  Spells[S_RETURN].id = S_RETURN;

  Spells[S_DESECRATE].powerdrain = 50;
  Spells[S_DESECRATE].castf = s_desecrate;
  Spells[S_DESECRATE].id = S_DESECRATE;

  Spells[S_HASTE].powerdrain = 15;
  Spells[S_HASTE].castf = s_haste;
  Spells[S_HASTE].id = S_HASTE;

  Spells[S_SUMMON].powerdrain = 20;
  Spells[S_SUMMON].castf = s_summon;
  Spells[S_SUMMON].id = S_SUMMON;

  Spells[S_SANCTUARY].powerdrain = 75;
  Spells[S_SANCTUARY].castf = s_sanctuary;
  Spells[S_SANCTUARY].id = S_SANCTUARY;

  Spells[S_ACCURACY].powerdrain = 20;
  Spells[S_ACCURACY].castf = s_accuracy;
  Spells[S_ACCURACY].id = S_ACCURACY;

  Spells[S_RITUAL].powerdrain = 50;
  Spells[S_RITUAL].castf = s_ritual;
  Spells[S_RITUAL].id = S_RITUAL;

  Spells[S_INVULNERABLE].powerdrain = 50;
  Spells[S_INVULNERABLE].castf = s_invulnerable;
  Spells[S_INVULNERABLE].id = S_INVULNERABLE;

  Spells[S_APPORT].powerdrain = 15;
  Spells[S_APPORT].castf = s_apport;
  Spells[S_APPORT].id = S_APPORT;

  Spells[S_SHADOWFORM].powerdrain = 50;
  Spells[S_SHADOWFORM].castf = s_shadowform;
  Spells[S_SHADOWFORM].id = S_SHADOWFORM;

  Spells[S_ALERT].powerdrain = 15;
  Spells[S_ALERT].castf = s_alert;
  Spells[S_ALERT].id = S_ALERT;

  Spells[S_REGENERATE].powerdrain = 20;
  Spells[S_REGENERATE].castf = s_regenerate;
  Spells[S_REGENERATE].id = S_REGENERATE;

  Spells[S_SANCTIFY].powerdrain = 75;
  Spells[S_SANCTIFY].castf = s_sanctify;
  Spells[S_SANCTIFY].id = S_SANCTIFY;

  Spells[S_CLAIRVOYANCE].powerdrain = 10;
  Spells[S_CLAIRVOYANCE].castf = s_clairvoyance;
  Spells[S_CLAIRVOYANCE].id = S_CLAIRVOYANCE;

  Spells[S_DRAIN].powerdrain = 40;
  Spells[S_DRAIN].castf = s_drain;
  Spells[S_DRAIN].id = S_DRAIN;

  Spells[S_LEVITATE].powerdrain = 25;
  Spells[S_LEVITATE].castf = s_levitate;
  Spells[S_LEVITATE].id = S_LEVITATE;

  Spells[S_POLYMORPH].powerdrain = 30;
  Spells[S_POLYMORPH].castf = s_polymorph;
  Spells[S_POLYMORPH].id = S_POLYMORPH;

}

int spellparse()
{
  int spell= -3,place=0;
  char byte,prefix[80];

  prefix[0]=0;

  do {
    byte = mgetc();
    if ((byte >= 'A') && (byte <= 'Z')) {
      maddch(byte+'a'-'A');
      prefix[place] = byte+'a'-'A';
      prefix[place+1] = 0;
      place++;
    }
    else if ((byte == ' ') || ((byte >= 'a') && (byte <= 'z'))) {
      maddch(byte);
      prefix[place] = byte;
      prefix[place+1] = 0;
      place++;
    }
    else if ((byte == 8) || (byte == 127)) { /* ^h or delete */ 
      prefix[place]=0;
      if (place > 0) {
	place--;
	dobackspace();
      }
    }
    else if (byte == '?') {
      maddch(byte);
      expandspellabbrevs(prefix);
      dobackspace();
    }
    else if (byte == '\n')
      spell = expandspell(prefix);
    else if (byte == ESCAPE)
      spell = ABORT;
  } while (spell == -3);
  xredraw();
  return(spell);
}

void expandspellabbrevs(prefix)
char prefix[80];
{
  int i,printed=FALSE;
  
  menuclear();
  menuprint("\nPossible Spells:\n");
  for (i=0;i<NUMSPELLS;i++)
    if (Spells[i].known && strprefix(prefix,spellid(i))) {
      menuspellprint(i);
      printed = TRUE;
    }
  if (! printed)
    menuprint("\nNo spells match that prefix!");
}


int expandspell(prefix)
char prefix[80];
{
  int i,spell,matched=0;
  
  for (i=0;i<NUMSPELLS;i++)
    if (Spells[i].known && strprefix(prefix,spellid(i))) {
      spell = i;
      matched++;
    }
  if (matched==0) {
    mprint("No spells match that prefix!");
    return(-2);
  }
  else if (matched > 1) {
    mprint("That is an ambiguous abbreviation!");
    return(-2);
  }
  else return(spell);
}

