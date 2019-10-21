/* omega copyright (C) 1987 by Laurence Raphael Brothers */
/* oitemf.c */

/* only i_ and weapon_ functions should be in this file. Auxiliary functions */
/* in oitem */

#include <strings.h>
#include "oglob.h"

/* from ocom */
extern void levelchange();

/* from omon */
extern void m_damage(),m_death(),m_vanish(),m_status_reset();
extern int m_immunityp();


/* from outil */
extern int random_range(),pow2();

/* from oscr */
extern void mprint(),printm(),menumorewait(),showflags();
extern void erase_level(),menuprint(),menuclear(),xredraw(),morewait();
extern char mgetc();

/* ogen */
void clear_level(),create_level();




/* from oaux */
extern void setspot(),p_poison(),p_drown(),p_death(),p_damage();
extern void conform_lost_object(),calc_melee(),foodcheck();
extern void p_teleport(),p_hit();
extern void gain_experience();
extern int p_immune();

/* from oeffect1 */
extern void identify(),bless(),enchant(),heal(),cleanse(),acquire();
extern void fbolt(),lball(),mondet(),objdet(),snowball(),wish();
extern void lbolt(),fball(),disrupt(),disintegrate();
extern void sleep_player(),nbolt(),sleep_monster(),summon(),apport();
extern void hide(),clairvoyance(),learnspell(),amnesia(),level_drain();

/* from oeffect2 */
extern void knowledge(),flux(),displace(),breathe(),invisible(),recover_stat();
extern void augment(),haste(),warp(),alert(),regenerate(),accuracy(),hero();
extern void levitate(),dispel(),polymorph(),truesight(),illuminate();
extern void sanctify(),cure();

/* from oitem */
extern void item_use();

/* from omove */
extern void p_movefunction();

/* from oinv */
extern void conform_unused_object();
extern int inpack();

/* itemf functions */
void weapon_bare_hands(),weapon_normal_hit(),weapon_demonblade();
void weapon_lightsabre(),weapon_mace_disrupt(),weapon_tangle();
void weapon_vorpal(),weapon_desecrate(),weapon_firestar(),weapon_defend();
void weapon_victrix();

void i_no_op(),i_nothing();
void i_spells(),i_bless(),i_enchant(),i_acquire(),i_jane_t();
void i_flux(),i_breathing(),i_invisible(),i_perm_invisible();
void i_teleport(),i_wish(),i_clairvoyance(),i_displace(),i_id(),i_augment();
void i_heal(),i_objdet(),i_mondet(),i_sleep_self(),i_neutralize_poison();
void i_speed(),i_restore(),i_azoth(),i_regenerate(),i_demonblade();
void i_firebolt(),i_lbolt(),i_missile(),i_fireball(),i_lball(),i_snowball();
void i_sleep_other(),i_summon(),i_hide(),i_disrupt(),i_disintegrate();
void i_perm_vision(),i_perm_burden(),i_perm_fire_resist(),i_charge();
void i_perm_poison_resist(),i_perm_strength(),i_perm_gaze_immune();
void i_perm_energy_resist();void i_perm_speed(),i_perm_breathing();
void i_perm_displace(),i_perm_negimmune(),i_mace_disrupt(),i_warp();
void i_food(),i_poison_food(),i_stim(),i_immune(),i_lembas(),i_pow();
void i_orbfire(),i_orbwater(),i_orbair(),i_orbearth(),i_orbdead();
void i_orbmastery(),i_lightsabre(),i_alert(),i_knowledge(),i_pick();
void i_perm_knowledge(),i_fear_resist(),i_perm_fear_resist(),i_key();
void i_defend(),i_victrix(),i_apport(),i_corpse(),i_accuracy();
void i_perm_accuracy(),i_hero(),i_perm_hero(),i_levitate();
void i_perm_regenerate(),i_perm_levitate(),i_perm_protection();
void i_perm_agility(),i_truesight(),i_perm_truesight(),i_cure();
void i_dispel(),i_pepper_food(),i_enchantment(),polymorph();
void i_crystal(),i_kolwynia(),i_death(),i_life(),i_helm(),i_antioch();
void i_illuminate(),i_perm_illuminate(),i_desecrate();
void i_trap(),i_juggernaut();

/* general item functions */

void i_no_op(o)
pob o;
{
}

void i_nothing(o)
pob o;
{
}


/* ring functions */
void i_perm_knowledge(o)
pob o;
{
  o->known = 1;
  if (o->blessing > -1)
    Objects[o->id].known = 1;
  if (o->used) 
    knowledge(o->blessing);
}

void i_perm_strength(o)
pob o;
{
  o->known = 1;
  Objects[o->id].known = 1;
  if (o->used){
    if (o->blessing > -1)
      Player.str += abs(o->plus)+1;
    else
      Player.str -= abs(o->plus)+1;
  }
  else {
    if (o->blessing > -1)
      Player.str -= abs(o->plus)+1;
    else
      Player.str += abs(o->plus)+1;
  }
  calc_melee();
  calc_melee();
}


void i_perm_vision(o)
pob o;
{
  o->known = 2;
  Objects[o->id].known = 1;
  if (o->used) Player.vision += (o->plus+1);
  else Player.vision -= (o->plus+1);
}

void i_perm_burden(o)
pob o;
{
  int i;
  
  if (o->used) {
    o->weight = 1000;
    mprint("You feel heavier.");
  }
  else {
    o->weight = 1;
    mprint("Phew. What a relief.");
  }
  Player.itemweight = 0;
  for (i=0;i<MAXITEMS;i++) {
    if (Player.possessions[i] != NULL)
      Player.itemweight += 
	(Player.possessions[i]->weight*Player.possessions[i]->number);
  }
}

void i_perm_gaze_immune(o)
pob o;
{
  if (o->used) Player.immunity[GAZE]++;
  else Player.immunity[GAZE]--;
}

void i_perm_fire_resist(o)
pob o;
{
  if (o->used) Player.immunity[FLAME]++;
  else Player.immunity[FLAME]--;
}

void i_perm_poison_resist(o)
pob o;
{
  if (o->used) {
    if (o->blessing < 0) {
      Player.immunity[POISON] = 0;
      p_poison(100);
    }
    else {
      Player.immunity[POISON]++;
      if (Player.status[POISONED] > 0) {
	mprint("You feel much better now.");
	Player.status[POISONED] = 0;
      }
    }
  }
  else {
    Player.status[POISONED] = 0;
    Player.immunity[POISON]--;
  }
}


void i_perm_energy_resist(o)
pob o;
{
  if (o->used){
    Player.immunity[FLAME]++;
    Player.immunity[COLD]++;
    Player.immunity[ELECTRICITY]++;
  }
  else {
    Player.immunity[FLAME]--;
    Player.immunity[COLD]--;
    Player.immunity[ELECTRICITY]--;
  }
}



void i_perm_breathing(o)
pob o;
{
 
  o->known = 1;
  if (o->blessing > -1)
    Objects[o->id].known = 1;

  if (o->blessing > -1) {
    if (o->used) {
      mprint("Your breath is energized!");
      Player.status[BREATHING] += 1500;
    }
    else  {
      Player.status[BREATHING] -= 1500;
      if (Player.status[BREATHING] < 1) {
	mprint("Your breathing is back to normal.");
	Player.status[BREATHING] = 0;
      }
    }
  }
  else if (o->used) {
    Player.status[BREATHING] = 0;
    p_drown();
  }
}




void i_perm_regenerate(o)
pob o;
{
  o->known = 1;
  if (o->blessing > -1)
    Objects[o->id].known = 1;
  if (o->used) {
    mprint("You seem abnormally healthy.");
    Player.status[REGENERATING] += 1500;
  }
  else {
    Player.status[REGENERATING] -= 1500;
    if (Player.status[REGENERATING] < 1) {
      mprint("Your vitality is back to normal");
      Player.status[REGENERATING] = 0;
    }
  }
}


/*  scroll functions */

void i_knowledge(o)
pob o;
{
  if (o->blessing > -1)
    Objects[o->id].known = 1;
  knowledge(o->blessing);
}

void i_jane_t(o)
pob o;
{
  int volume = random_range(6);
  int i,j,k;

  printm("\nJane's Guide to the World's Treasures: ");
  
  switch(volume) {
    case 0:mprint("SCROLLS");j = SCROLLID; k = POTIONID; break;
    case 1:mprint("POTIONS");j = POTIONID; k = WEAPONID; break;
    case 2:mprint("CLOAKS");j = CLOAKID; k = BOOTID; break;
    case 3:mprint("BOOTS");j = BOOTID; k = RINGID; break;
    case 4:mprint("RINGS");j = RINGID; k = STICKID; break;
    case 5:mprint("STICKS");j = STICKID; k = ARTIFACTID; break;
  }
  for(i=j;i<k;i++)
    Objects[i].known = 1;
}

   
void i_flux(o)
pob o;
{
  if (o->blessing > -1)
    Objects[o->id].known = 1;
  flux(o->blessing);
}


/* enchantment */
void i_enchant(o)
pob o;
{
  if (o->blessing > -1)
    Objects[o->id].known = 1;
  enchant(o->blessing < 0 ? -1-o->plus : o->plus+1);
}

/* scroll of clairvoyance */
void i_clairvoyance(o)
struct object *o;
{
  if (o->blessing > -1)
    Objects[o->id].known = 1;
  if (o->blessing < 0)
    amnesia();
  else if (o->blessing > 0)
      clairvoyance(Player.vision*10);
  else clairvoyance(Player.vision*2);
}



void i_acquire(o)
pob o;
{
  if (o->blessing > -1)
    Objects[o->id].known = 1;
  acquire(o->blessing);
}

void i_teleport(o)
pob o;
{
  if (o->blessing > -1)
    Objects[o->id].known = 1;
  p_teleport(o->blessing);
}


void i_spells(o)
pob o;
{
  if (o->blessing > -1)
    Objects[o->id].known = 1;
  mprint("A scroll of spells.");
  learnspell(o->blessing);
}



/* scroll of blessing */
void i_bless(o)
pob o;
{
  Objects[o->id].known = 1;
  bless(o->blessing);
}

/* scroll of wishing */
void i_wish(o)
pob o;
{
  if (o->blessing > -1)
    Objects[o->id].known = 1;
  wish(o->blessing);
}

/* scroll of displacement */
void i_displace(o)
pob o;
{
  if (o->blessing > -1) 
      Objects[o->id].known = 1;
  displace(o->blessing);
}


/* scroll of identification */
void i_id(o)
pob o;
{
  if (o->blessing > -1)
    Objects[o->id].known = 1;
  identify(o->blessing);
}

/* potion functions */

/* potion of healing */
void i_heal(o)
pob o;
{
  if (o->blessing > -1) {
    Objects[o->id].known = 1;
    heal(1+o->plus);
  }
  else heal(-1-abs(o->plus));
}

/* potion of monster detection */
void i_mondet(o)
pob o;
{
  if (o->blessing > -1)
    Objects[o->id].known = 1;
  mondet(o->blessing);
}


/* potion of object detection */
void i_objdet(o)
pob o;
{

  if (o->blessing > -1)
    Objects[o->id].known = 1;
  objdet(o->blessing);
}

/* potion of neutralize poison */
void i_neutralize_poison(o)
pob o;
{
  if (o->blessing > -1) {
    Objects[o->id].known = 1;
    mprint("You feel vital!");
    Player.status[POISONED] = 0;
  }
  else p_poison(random_range(20)+5);
}


/* potion of sleep */
void i_sleep_self(o)
pob o;
{
  sleep_player(6);
  Objects[o->id].known = 1;
}




/* potion of speed */
void i_speed(o)
pob o;
{
  if (o->blessing > -1) 
    Objects[o->id].known = 1;
  haste(o->blessing);
}


/* potion of restoration */
void i_restore(o)
pob o;
{
  if (o->blessing > -1) 
    Objects[o->id].known = 1;
  recover_stat(o->blessing);
}

void i_augment(o)
pob o;
{
  if (o->blessing > -1) 
    Objects[o->id].known = 1;
  augment(o->blessing);
}  

void i_azoth(o)
pob o;
{
  if (o->plus < 0) {
    mprint("The mercury was poisonous!");
    p_poison(25);
  }
  else if (o->plus == 0) {
    mprint("The partially enchanted azoth makes you sick!");
    Player.con = ((int) (Player.con / 2));
    calc_melee();
  }
  else if (o->blessing < 1) {
    mprint("The unblessed azoth warps your soul!");
    Player.pow = Player.maxpow = ((int) (Player.maxpow / 2));
    level_drain(random_range(10),"cursed azoth");
  }
  else {
    mprint("The azoth fills you with cosmic power!");
    if (Player.str > Player.maxstr*2) {
      mprint("The power rages out of control!");
      p_death("overdose of azoth");
    }
    else {
      heal(10);
      cleanse(1);
      Player.mana = calcmana()*3;
      Player.str = (Player.maxstr++)*3;
    }
  }
}

void i_regenerate(o)
pob o;
{
  regenerate(o->blessing);
}


/* orb functions */
void i_orbfire(o)
pob o;
{
  if (o->known < 1) {
    mprint("Fumbling with the orb, you set it off accidentally!");
    mprint("The Orb of Fire blasts you!");
    fball(Player.x,Player.y,Player.x,Player.y,100);
    o->known = 1;
  }
  else {
    mprint("The Orb of Fire is activated!");
    Spells[S_FIREBOLT].known = TRUE;
    gain_experience(10000);
    Player.immunity[FLAME]+=100;
    mprint("You feel fiery!");
    o->plus = 100;
    o->blessing = 100;
    i_firebolt(o);
    *o = Objects[ARTIFACTID+5];
  }
}


void i_orbwater(o)
pob o;
{
  if (o->known < 1) {
    mprint("Fumbling with the orb, you set it off accidentally!");
    mprint("The Orb of Water blasts you!");
    heal(-100);
    o->known = 1;
  }
  else {
    mprint("The Orb of Water is activated!");
    Spells[S_DISRUPT].known = TRUE;
    Player.immunity[POISON]+=100;
    gain_experience(10000);
    mprint("You feel wet!");
    o->plus = 100;
    o->blessing = 100;
    i_disrupt(o);
    *o = Objects[ARTIFACTID+5];
  }
}


void i_orbearth(o)
pob o;
{
  int i;
  if (o->known < 1) {
    mprint("Fumbling with the orb, you set it off accidentally!");
    mprint("The Orb of Earth blasts you!");
    for (i=0;i<MAXITEMS;i++) {
      if (Player.possessions[i] != NULL) {
	conform_lost_objects(Player.possessions[i]);
      }
    }
    mprint("Your pack disintegrates!");
    o->known = 1;
  }
  else {
    mprint("The Orb of Earth is activated!");
    Spells[S_DISINTEGRATE].known = TRUE;
    gain_experience(10000);
    Player.immunity[NEGENERGY]+=100;
    mprint("You feel earthy!");
    o->plus = 100;
    o->blessing = 100;
    i_disintegrate(o);
    *o = Objects[ARTIFACTID+5];
  }
}


void i_orbair(o)
pob o;
{
  if (o->known < 1) {
    mprint("Fumbling with the orb, you set it off accidentally!");
    mprint("The Orb of Air blasts you!");
    lball(Player.x,Player.y,Player.x,Player.y,100);
    o->known = 1;
  }
  else {
    mprint("The Orb of Air is activated!");
    Spells[S_LBALL].known = TRUE; /* lball */
    gain_experience(10000);
    mprint("You feel airy!");
    Player.immunity[ELECTRICITY]+=100;
    o->plus = 100;
    o->blessing = 100;
    i_invisible(o);
    i_lbolt(o);
    *o = Objects[ARTIFACTID+5];
  }
}


void i_orbmastery(o)
pob o;
{
  int a,b,c,d,i;
  if (o->known < 1) {
    mprint("Fumbling with the orb, you set it off accidentally!");
    mprint("The Orb of Mastery blasts you to cinders!");
    p_death("playing with the Orb of Mastery");
    o->known = 1;
  }
  else if (((a=inpack(ARTIFACTID+1,-1)) != ABORT) &&
	   ((b=inpack(ARTIFACTID+2,-1)) != ABORT) &&
	   ((c=inpack(ARTIFACTID+3,-1)) != ABORT) &&
	   ((d=inpack(ARTIFACTID+4,-1)) != ABORT)) {
    item_use(Player.possessions[a]);
    item_use(Player.possessions[b]);
    item_use(Player.possessions[c]);
    item_use(Player.possessions[d]);
    mprint("The Orb of Mastery is activated!");
    mprint("You feel godlike.");
    Player.iq = Player.maxiq = 2 * Player.maxiq;
    Player.pow = Player.maxpow = 2 * Player.maxpow;
    Player.str = Player.maxstr = 2 * Player.maxstr;
    Player.dex = Player.maxdex = 2 * Player.maxdex;
    Player.con = Player.maxcon = 2 * Player.maxcon;
    Player.agi = Player.maxagi = 2 * Player.maxagi;
    dataprint();
    mprint("You have been imbued with a cosmic power....");
    wish(1);
    mprint("You feel much more experienced.");
    gain_experience(20000);
    *o = Objects[ARTIFACTID+5];
  }
  else {
    mprint("The Orb of Mastery's power is unbalanced!");
    mprint("The Orb of Mastery blasts you to cinders!");
    p_death("playing with the Orb of Mastery");
  }  
}


void i_orbdead(o)
pob o;
{
  int i;
  mprint("The burnt-out orb drains all your energy!");
  for(i=0;i<NUMSPELLS;i++)
    Spells[i].known = FALSE;
  mprint("You feel not at all like a mage.");
  for(i=0;i<MAXITEMS;i++) {
    if (Player.possessions[i] != NULL) {
      Player.possessions[i]->plus = 0;
      if (Player.possessions[i]->usef > 100)
	Player.possessions[i]->usef = I_NOTHING;
    }
  }
  mprint("A storm of mundanity surounds you!");
  level_drain(Player.level-1,"a Burnt-out Orb");
}



/* stick functions * /


/* wand of apportation */
void i_apport(o)
pob o;
{
  o->known = max(1,o->known);
  Objects[o->id].known = 1;
  apport(o->blessing);
}

/* staff of firebolts */
void i_firebolt(o)
pob o;
{
  int x=Player.x,y=Player.y;
  o->known = max(1,o->known);
  Objects[o->id].known = 1;
  setspot(&x,&y);
  if (o->blessing < 0) {
    x = Player.x;
    y = Player.y;
  }
  fbolt(Player.x,Player.y,x,y,Player.dex*2+Player.level,75);
}


void i_disintegrate(o)
pob o;
{
  int x=Player.x,y=Player.y;
  o->known = max(1,o->known);
  Objects[o->id].known = 1;
  setspot(&x,&y);
  if (o->blessing < 0) {
    x = Player.x;
    y = Player.y;
  }
  disintegrate(x,y);
}

void i_disrupt(o)
pob o;
{
  int x=Player.x,y=Player.y;
  o->known = max(1,o->known);
  Objects[o->id].known = 1;
  setspot(&x,&y);
  if (o->blessing < 0) {
    x = Player.x;
    y = Player.y;
  }
  disrupt(x,y,100);
}


/* staff of lightning bolts */
void i_lbolt(o)
pob o;
{
  int x=Player.x,y=Player.y;
  o->known = max(1,o->known);
  Objects[o->id].known = 1;
  setspot(&x,&y);
  if (o->blessing < 0) {
    x = Player.x;
    y = Player.y;
  }
  lbolt(Player.x,Player.y,x,y,Player.dex*2+Player.level,75);
}

/* wand of magic missiles */
void i_missile(o)
pob o;
{
  int x=Player.x,y=Player.y;
  o->known = max(1,o->known);
  Objects[o->id].known = 1;
  setspot(&x,&y);
  if (o->blessing < 0) {
    x = Player.x;
    y = Player.y;
  }
  nbolt(Player.x,Player.y,x,y,Player.dex*2+Player.level,20);
}

/* wand of fire balls */
void i_fireball(o)
pob o;
{
  int x=Player.x,y=Player.y;
  Objects[o->id].known = 1;
  o->known = max(1,o->known);
  setspot(&x,&y);
  if (o->blessing < 0) {
    x = Player.x;
    y = Player.y;
  }
  fball(Player.x,Player.y,x,y,35);
}


/* wand of snowballs */
void i_snowball(o)
pob o;
{
  int x=Player.x,y=Player.y;
  Objects[o->id].known = 1;
  o->known = max(1,o->known);
  setspot(&x,&y);
  if (o->blessing < 0) {
    x = Player.x;
    y = Player.y;
  }
  snowball(Player.x,Player.y,x,y,20);
}

/* wand of lightning balls */
void i_lball(o)
pob o;
{
  int x=Player.x,y=Player.y;
  Objects[o->id].known = 1;
  o->known = max(1,o->known);
  setspot(&x,&y);
  if (o->blessing < 0) {
    x = Player.x;
    y = Player.y;
  }
  lball(Player.x,Player.y,x,y,50);
}

/* staff of sleep */
void i_sleep_other(o)
pob o;
{
  Objects[o->id].known = 1;
  o->known = max(1,o->known);
  sleep_monster(o->blessing);
}

/* rod of summoning */
void i_summon(o)
pob o;
{
  Objects[o->id].known = 1;
  o->known = max(1,o->known);
  summon(o->blessing,-1);
}

void i_hide(o)
pob o;
{
  int x=Player.x,y=Player.y;
  Objects[o->id].known = 1;
  o->known = max(1,o->known);
  setspot(&x,&y);
  hide(x,y);
}

void i_polymorph(o)
pob o;
{
  Objects[o->id].known = 1;
  o->known = max(1,o->known);
  polymorph(o->blessing);
}



/* boots functions */
void i_perm_speed(o)
pob o;
{
  if (o->blessing > -1) {
    if (o->used) {
      o->known = 2;
      Objects[o->id].known = 1;
      if (Player.status[SLOWED] > 0) {
	Player.status[SLOWED] = 0;
      }
      mprint("The world slows down!"); 
      Player.status[HASTED] += 1500;
    }
    else {
      Player.status[HASTED] -= 1500;
      if (Player.status[HASTED] < 1) 
	mprint("The world speeds up again.");
    }
  }
  else {
    if (o->used) {
      if (Player.status[HASTED] > 0) {
	Player.status[HASTED] = 0;
      }
      mprint("You feel slower.");
      Player.status[SLOWED] += 1500;
    }
    else  {
      Player.status[SLOWED] -= 1500;
      if (Player.status[SLOWED] < 1) 
	mprint("You feel quicker again.");
    }  
  }
}

/* cloak functions */
void i_perm_displace(o)
pob o;
{
  if (o->blessing > -1) {
    if (o->used) {
      mprint("You feel dislocated.");
      Player.status[DISPLACED] += 1500;
    }
    else {
      Player.status[DISPLACED] -= 1500;
      if (Player.status[DISPLACED] < 1) {
	mprint("You feel a sense of position.");
	Player.status[DISPLACED] = 0;
      }
    }
  }
  else {
    if (o->used) {
      mprint("You have a forboding of bodily harm!");
      Player.status[VULNERABLE] += 1500;
    }
    else {
      Player.status[VULNERABLE] -= 1500;
      if (Player.status[VULNERABLE] < 1) {
	mprint("You feel less endangered.");
	Player.status[VULNERABLE] = 0;
      }
    }
  }
}

void i_perm_negimmune(o)
pob o;
{
  if (o->blessing > -1) {
    if (o->used) {
      Player.immunity[NEGENERGY]++;
    }
    else Player.immunity[NEGENERGY]--;
  }
  else if (o->used)
    level_drain(abs(o->blessing),"cursed cloak of level drain");
}

/* food functions */


void i_food(o)
pob o;
{
  switch(random_range(5)) {
    case 0: mprint("That tasted horrible!"); break;
    case 1: mprint("Yum!"); break;
    case 2: mprint("How nauseous!"); break;
    case 3: mprint("Can I have some more? Please?"); break;
    case 4: mprint("Your mouth feels like it is growing hair!"); break;
  }
}

void i_stim(o)
pob o;
{
  mprint("You feel Hyper!");
  i_speed(o);
  Player.str +=3;
  Player.con -=1;
  calc_melee();
}

void i_pow(o)
pob o;
{
  mprint("You feel a surge of mystic power!");
  Player.mana = 2 * calcmana();
}

void i_poison_food(o)
pob o;
{
  mprint("This food was contaminated with cyanide!");
  p_poison(random_range(20)+5);
}

void i_pepper_food(o)
pob o;
{
  mprint("You innocently start to chew the szechuan pepper.....");
  morewait();
  mprint("hot.");
  morewait();
  mprint("Hot.");
  morewait();
  mprint("Hot!");
  morewait();
  mprint("HOT!!!!!!");
  morewait();
  p_damage(1,UNSTOPPABLE,"a szechuan pepper");
  mprint("Your sinuses melt and run out your ears.");
  mprint("Your mouth and throat seem to be permanently on fire.");
  mprint("You feel much more awake now....");
  Player.immunity[SLEEP]++;
}

void i_lembas(o)
pob o;
{
  heal(10);
  cleanse(0);
  if (Player.food > 24) Player.food = 24;
}


void i_cure(o)
pob o;
{
  cure(o->blessing);
}

void i_immune(o)
pob o;
{
  if (o->blessing > 0) {
    mprint("You feel a sense of innoculation");
    Player.immunity[INFECTION]++;
    cure(o->blessing);
  }
}



void i_breathing(o)
pob o;
{
  
  if (o->blessing > -1)
    Objects[o->id].known = 1;
  breathe(o->blessing);
}

      
void i_invisible(o)
pob o;
{
  if (o->blessing > -1)
    Objects[o->id].known = 1;
  invisible(o->blessing);
}


void i_perm_invisible(o)
pob o;
{
  if (o->blessing > -1)
    Objects[o->id].known = 1;
  if (o->used) {
    if (o->blessing > -1) {
      mprint("You feel transparent!");
      Player.status[INVISIBLE] += 1500;
    }
    else {
      mprint("You feel a forboding of bodily harm!");
      Player.status[VULNERABLE] += 1500;
    }
  }
  else {
    if (o->blessing > -1) {
      Player.status[INVISIBLE]-=1500;
      if (Player.status[INVISIBLE] < 1) {
	mprint("You feel opaque again.");
	Player.status[INVISIBLE] = 0;
      }
    }
    else {
      Player.status[VULNERABLE] -= 1500;
      if (Player.status[VULNERABLE] < 1) {
	mprint("You feel less endangered now.");
	Player.status[VULNERABLE] = 0;
      }
    }
  }
}

void weapon_demonblade(dmgmod,o,m)
int dmgmod;
pob o;
struct monster *m;
{ 
  if (o->blessing > -1) {
    mprint("Demonblade disintegrates with a soft sigh.");
    mprint("You stop foaming at the mouth.");
    Player.status[BERSERK] = 0;
    conform_lost_object(o);
  }
  else if (m->specialf == M_SP_DEMON) {
    mprint("The demon flees in terror before your weapon!");
    m_vanish(m);
  }
  else if (m->meleef != M_MELEE_SPIRIT) {
    if (m->level > random_range(10)) {
      Player.hp = min(Player.maxhp,Player.hp+m->hp);
      Player.str++;
      Player.pow = min(Player.maxpow,Player.pow+m->level);
      m_death(m);
      mprint("You feel a surge of raw power from Demonblade!");
    }
    else p_hit(m,Player.dmg+dmgmod,NORMAL_DAMAGE);

  }
  else {
    mprint("Demonblade howls as it strikes the spirit!");
    if (random_range(10) == 1) {
      mprint("... and shatters into a thousand lost fragments!");
      p_damage(50,UNSTOPPABLE,"Demonblade exploding");
      conform_lost_object(o);
    }
    else {
      mprint("You feel your lifeforce draining....");
      p_damage(25,UNSTOPPABLE,"a backlash of negative energies");
      Player.str -= 3;
      Player.con -= 3;
    }
  }
}

void weapon_lightsabre(dmgmod,o,m)
int dmgmod;
pob o;
struct monster *m;
{
  if (! o->known) {
    mprint("Fumbling with the cylinder, you press the wrong stud....");
    p_death("fumbling with a lightsabre");
  }
  else {
    mprint("Vzzzzzmm... Zap!");
    m_damage(m,20,NORMAL_DAMAGE);
    if (m->hp>0) m_damage(m,20,FLAME);
  }
}

void weapon_tangle(dmgmod,o,m)
int dmgmod;
pob o;
struct monster *m;
{
  p_hit(m,Player.dmg+dmgmod,NORMAL_DAMAGE);
  if ((random_range(2) == 1) && (! m_immunityp(m,NORMAL_DAMAGE))) {
    mprint("You entangle the monster!");
    m_status_reset(m,MOBILE);
  }
}

/* if wielding a bow, add bow damage to arrow damage */
void weapon_arrow(dmgmod,o,m)
int dmgmod;
pob o;
struct monster *m;
{
  if ((Player.primary != NULL) &&
      (Player.primary->id == WEAPONID+26)) /* ie, using a bow */
    p_hit(m,Player.dmg+o->plus+o->dmg+dmgmod,NORMAL_DAMAGE);
  else p_hit(m,o->plus+o->dmg+dmgmod,NORMAL_DAMAGE);
}

/* if wielding a crossbow, add bow damage to arrow damage */
void weapon_bolt(dmgmod,o,m)
pob o;
struct monster *m;
{
  if ((Player.primary != NULL) &&
      (Player.primary->id == WEAPONID+27) && /*ie using a crossbow */
      (Player.primary->aux==LOADED)) {
	p_hit(m,Player.dmg+o->plus+o->dmg+dmgmod,NORMAL_DAMAGE);
	Player.primary->aux = UNLOADED;
      }
  else p_hit(m,o->plus+o->dmg,NORMAL_DAMAGE);
}


void weapon_mace_disrupt(dmgmod,o,m)
int dmgmod;
pob o;
struct monster *m;
{
  if (m->meleef == M_MELEE_SPIRIT) {
    mprint("The monster crumbles away to dust!");
    m_death(m);
  }
  else p_hit(m,Player.dmg+dmgmod,UNSTOPPABLE);
}

void weapon_normal_hit(dmgmod,o,m)
int dmgmod;
pob o;
struct monster *m;
{
  p_hit(m,Player.dmg+dmgmod,NORMAL_DAMAGE);
}


/* will be updated eventually */
void weapon_bare_hands(dmgmod,m)
int dmgmod;
struct monster *m;
{
  p_hit(m,Player.dmg+dmgmod,NORMAL_DAMAGE);
}


void i_demonblade(o)
pob o;
{
  if (o->used) {
    o->known = 2;
    mprint("Demonblade's fangs open and bury themselves in your wrist!");
    mprint("You hear evil laughter in the distance....");
    mprint("You begin to foam at the mouth!");
    Player.status[BERSERK] = 1500;
  }
  else {
    mprint("You hear a sound like a demon cursing.");
    mprint("You feel less like biting your shield.");
    Player.status[BERSERK] = 0;
  }
}


void i_lightsabre(o)
pob o;
{
  if (o->used) mprint("You feel one with the Force.");
  else mprint("You feel out of touch with the Force.");
}

void i_mace_disrupt(o)
pob o;
{
  mprint("That's a damned heavy mace!");
}

void i_warp(o)
pob o;
{
  if (o->blessing > -1)
    Objects[o->id].known = 1;
  warp(o->blessing);
}


void i_alert(o)
pob o;
{
  if (o->blessing > -1) {
    Objects[o->id].known = 1;
    alert(o->blessing);
  }
}

void i_charge(o)
pob o;
{
  int i;
  if (o->blessing > -1)
    Objects[o->id].known = 1;
  mprint("A scroll of charging.");
  mprint("Charge: ");
  i = getitem(STICK);
  if (i != ABORT) {
    i-='a';
    if (o->blessing < 0) {
      mprint("The stick glows black!");
      Player.possessions[i]->charge == 0;
    }
    else {
      mprint("The stick glows blue!");
      Player.possessions[i]->charge += (random_range(10)+1)*(o->blessing+1);
      if (Player.possessions[i]->charge > 99)
	Player.possessions[i]->charge = 99;
    }
  }
}


void i_fear_resist(o)
pob o;
{
  if (o->blessing > -1) {
    Objects[o->id].known = 1;
    if (Player.status[AFRAID] > 0) {
      mprint("You feel stauncher now.");
      Player.status[AFRAID] = 0;
    }
  }
  else if (! p_immune(FEAR)) {
    mprint("You panic!");
    Player.status[AFRAID]+=random_range(100);
  }
}

void i_perm_fear_resist(o)
pob o;
{
  if (o->used){
    Player.immunity[FEAR]++;
    if (o->blessing < 0) {
      Player.status[BERSERK] += 1500;
      mprint("You feel blood-simple!");
    }
  }
  else {
    Player.immunity[FEAR]--;
    if (o->blessing < 0) {
      Player.status[BERSERK] -= 1500;
      if (Player.status[BERSERK] < 1) {
	mprint("You feel less rabid now.");
      Player.status[BERSERK] = 0;
      }
    }
  }
}    



/* use a thieves pick */
void i_pick(o)
pob o;
{
  int dir;
  int ox,oy;
  o->used = FALSE;
  if ((! o->known) && (! Player.rank[THIEVES]))
    mprint("You have no idea what do with a piece of twisted metal.");
  else {
    o->known = 1;
    Objects[o->id].known = 1;
    printm("\nPick lock:");
    dir = getdir();
    if (dir == ABORT)
      Skipmonsters = TRUE;
    else {
      ox = Player.x + Dirs[0][dir];
      oy = Player.y + Dirs[1][dir];
      if ((Dungeon[Dlevel][ox][oy].locchar != CLOSED_DOOR) || 
	  Dungeon[Dlevel][ox][oy].secret) {
	mprint("You can't unlock that!");
	Skipmonsters = TRUE;
      }
      else if (Dungeon[Dlevel][ox][oy].aux == LOCKED) {
	if (Dlevel == NUMLEVELS-1) 
	  mprint("The lock is too complicated for you!!!");
	else if (Dlevel*2 + random_range(50) <
	    Player.dex+Player.level+Player.rank[THIEVES]*10) {
	  mprint("You picked the lock!");
	  Dungeon[Dlevel][ox][oy].aux = UNLOCKED;
	  gain_experience(max(3,Dlevel));
	}
	else mprint("You failed to pick the lock.");
      }
      else mprint("That door is already unlocked!");
    }
  }
}

/* use a magic key*/
void i_key(o)
pob o;
{
  int dir;
  int ox,oy;
  o->used = FALSE;
  printm("\nUnlock door: ");
  dir = getdir();
  if (dir == ABORT)
    Skipmonsters = TRUE;
  else {
    ox = Player.x + Dirs[0][dir];
    oy = Player.y + Dirs[1][dir];
    if ((Dungeon[Dlevel][ox][oy].locchar != CLOSED_DOOR) || 
	Dungeon[Dlevel][ox][oy].secret) {
	  mprint("You can't unlock that!");
	  Skipmonsters = TRUE;
	}
    else if (Dungeon[Dlevel][ox][oy].aux == LOCKED) {
	mprint("The lock clicks open!");
	Dungeon[Dlevel][ox][oy].aux = UNLOCKED;
	o->blessing--;
	if ((o->blessing<0)||(Dlevel == NUMLEVELS-1)) {
	  mprint("The key disintegrates!");
	  conform_lost_objects(1,o);
	}
	else {
	  mprint("Your key glows faintly.");
	  o->blessing--;
	}
      }
    else mprint("That door is already unlocked!");
  }
}

void weapon_vorpal(dmgmod,o,m)
int dmgmod;
pob o;
struct monster *m;
{
  if ((random_range(10) < 3) && (! m_immunityp(m,NORMAL_DAMAGE))) {
    o->known = 2;
    if (random_range(2) == 1)
      mprint("One Two! One Two! And through and through!");
    else mprint("Your vorpal blade goes snicker-snack!");
    m_death(m);
  }
  else weapon_normal_hit(dmgmod,o,m);
}

void weapon_desecrate(dmgmod,o,m)
int dmgmod;
pob o;
struct monster *m;
{
  o->known = 2;
  if (Player.alignment < 0) {
    mprint("Your opponent screams in agony!");
    p_hit(m,Player.dmg+dmgmod,UNSTOPPABLE);
    Player.alignment--;
    if (Player.hp < Player.maxhp) {
      mprint("You feel a thrill of power surging up your blade!");
      Player.hp = min(Player.maxhp,Player.hp+Player.dmg+dmgmod);
    }
  }
  else {
    mprint("Your blade turns in your hands and hits you!");
    mprint("You hear evil laughter....");
    level_drain(Player.dmg,"the sword Desecrator");
    Player.alignment-=10;
    mprint("A strange force spreads from the wound throughout your body...");
    mprint("You feel much more chaotic now.");
  }
}



void weapon_firestar(dmgmod,o,m)
int dmgmod;
pob o;
struct monster *m;
{
  if (random_range(3) == 1) {
    o->known = 2;
    fball(Player.x,Player.y,Player.x,Player.y,max(Player.dmg,25));
  }
  if (m->hp > 0) weapon_normal_hit(dmgmod,o,m);
}

void weapon_defend(dmgmod,o,m)
pob o;
struct monster *m;
{
  if ((Player.alignment < 0) && (o->blessing > 0)) {
      mprint("The Holy Defender screams in your hands....");
      mprint("You stagger before the sound of its rage....");
      p_damage(50,UNSTOPPABLE,"a pissed-off Holy Defender");
      mprint("The weapon finally quiets. It seems less holy now.");
      strcpy(o->truename,o->cursestr);
      Player.status[PROTECTION] -= (o->hit);
      o->plus = 0-abs(o->plus);
      o->blessing = -1;
    }
  if ((m->specialf == M_SP_DEMON) ||
      (m->meleef == M_MELEE_SPIRIT)) {
    mprint("Your opponent shies back before your holy weapon!");
    m->hit = 0;
  }
  weapon_normal_hit(dmgmod,o,m);
}

void weapon_victrix(dmgmod,o,m)
int dmgmod;
pob o;
struct monster *m;
{
  if (Player.alignment < -20) {
    mprint("The Spear Victrix dissolves in your hands with a faint sigh.");
    conform_lost_object(o);
    mprint("You hear congratulatory laughter all around you.");
    gain_experience(1000);
  }
  else {  
    if ((m->specialf == M_SP_DEMON) ||
	(m->meleef == M_MELEE_SPIRIT)) {
      mprint("Your opponent dissipates in a harmless cloud of vapors...");
      Player.alignment++;
      m_death(m);
    }
    else weapon_normal_hit(dmgmod,o,m);
  }
}

void i_defend(o)
pob o;
{
  o->known = 2;
  if (o->used) {
    mprint("You feel under an aegis!");
    Player.status[PROTECTION] += o->hit;
  }
  else Player.status[PROTECTION] -= o->hit;
}


void i_victrix(o)
pob o;
{
  o->known = 2;
  o->blessing = abs(o->blessing);
  if (o->used) {
    Player.immunity[POISON]++;
    Player.immunity[FEAR]++;
    Player.immunity[INFECTION]++;
  }
  else {
    Player.immunity[POISON]--;
    Player.immunity[FEAR]--;
    Player.immunity[INFECTION]--;
  }
}    

void i_corpse(o)
pob o;
{
  switch (o->aux) {
  case ML0+1: 
  case ML0+2:
  case ML0+3:
  case ML0+4:
  case ML2+0:
  case ML2+2:
  case ML7+3:
  case ML10+0: /* cannibalism */
    mprint("Yechh! How could you! You didn't even cook him, first!");
    if (Player.alignment > 0) Player.food = 25;
    Player.food += 8;
    Player.alignment -=10;
    foodcheck();
    break;
  case ML1+2: /* fnord */
    mprint("You feel illuminated!");
    Player.iq++;
    break;
  case ML4+3: /* denebian slime devil */
    mprint("I don't believe this. You ate Denebian Slime?");
    mprint("You deserve a horrible wasting death, uncurable by any means!");
    break;
  case ML5+0:
    mprint("Those dragon-steaks were fantastic!");
    Player.food=24;
    foodcheck();
    break;
  case ML7+0:
    mprint("You feel infinitely more virile now.");
    Player.str = max(Player.str,Player.maxstr+10);
    Player.food = 24;
    foodcheck();
    break;
  case ML9+1:
    mprint("Guess what? You're invisible.");
    if (Player.status[INVISIBLE] < 1000) Player.status[INVISIBLE] = 666;
    Player.food+=6;
    foodcheck();
    break;
  case ML7+2:  
    mprint("You ATE a unicorn's horn?!?!?");
    Player.immunity[POISON]=1000;
    break;
  case ML0+0:
  case ML1+0:
  case ML1+1:
  case ML1+4:
  case ML1+5:
  case ML1+6:
  case ML1+10:
  case ML2+1:
  case ML2+4:
  case ML4+1:
  case ML4+4:
  case ML5+3:
    mprint("Well, you forced it down. Not much nutrition, though.");
    Player.food++;
    foodcheck();
    break;
  case ML1+3:
  case ML1+7:
  case ML2+3:
  case ML2+5:
  case ML3+1:
  case ML4+5:
  case ML9+3:
  case ML10+1:
    mprint("Oh, yuck. The 'food' seems to be tainted.");
    mprint("You feel very sick. You throw up.");
    Player.food = min(Player.food, 4);
    if (! Player.immunity[INFECTION])
      Player.status[DISEASED]+=24;
    p_poison(10);
    break;
  default: mprint("It proved completely inedible, but you tried anyhow.");
  }
}

void i_accuracy(o)
pob o;
{
  o->known = 1;
  Objects[o->id].known = 1;
  accuracy(o->blessing);
}

void i_perm_accuracy(o)
pob o;
{
  o->known = 1;
  Objects[o->id].known = 1;
  if ((o->used) && (o->blessing > -1)) {
    Player.status[ACCURATE] += 1500;
    mprint("You feel skillful and see bulls' eyes everywhere.");
  }
  else {
    Player.status[ACCURATE] -= 1500;
    if (Player.status[ACCURATE] < 1) {
      Player.status[ACCURATE] = 0;
      calc_melee();
      mprint("Your vision blurs....");
    }
  }
}

void i_hero(o)
pob o;
{
  o->known = 1;
  Objects[o->id].known = 1;
  hero(o->blessing);
}

void i_perm_hero(o)
pob o;
{
  o->known = 1;
  Objects[o->id].known = 1;
  if (o->used){
    if (o->blessing > -1) {
      Player.status[HERO] += 1500;
      calc_melee();
      mprint("You feel super!");
    }
    else {
      Player.status[HERO] = 0;
      calc_melee();
      if (! Player.immunity[FEAR]) {
	Player.status[AFRAID]+=1500;
	mprint("You feel cowardly....");
      }
    }
  }
  else {
    if (o->blessing > -1) {
      Player.status[HERO] -= 1500;
      if (Player.status[HERO] < 1) {
	calc_melee();
	mprint("You feel less super now.");
	Player.status[HERO] = 0;
      }
    }
    else {
      Player.status[AFRAID] -= 1500;
      if (Player.status[AFRAID] < 1) {
	mprint("You finally conquer your fear.");
	Player.status[AFRAID] = 0;
      }
    }
  } 
}

void i_levitate(o)
pob o;
{
  o->known = 1;
  Objects[o->id].known = 1;
  levitate(o->blessing);
}

void i_perm_levitate(o)
pob o;
{
  o->known = 1;
  Objects[o->id].known = 1;
  if (o->blessing > -1) {
    if (o->used) {
      Player.status[LEVITATING] += 1500;
      mprint("You start to float a few inches above the floor");
      mprint("You fid you can easily control your altitude");
    }
    else {
      Player.status[LEVITATING] -= 1500;
      if (Player.status[LEVITATING] < 1) {
	Player.status[LEVITATING] = 0;
	mprint("You sink to the floor.");
      }
    }
  }
  else i_perm_burden(o);
}

void i_perm_protection(o)
pob o;
{
  if (o->used){
    if (o->blessing > -1)
      Player.status[PROTECTION] += abs(o->plus)+1;
    else
      Player.status[PROTECTION] -= abs(o->plus)+1;
  }
  else {
    if (o->blessing > -1)
      Player.status[PROTECTION] -= abs(o->plus)+1;
    else
      Player.status[PROTECTION] += abs(o->plus)+1;
  }
  calc_melee();
}

void i_perm_agility(o)
pob o;
{
  o->known = 1;
  Objects[o->id].known = 1;
  if (o->used){
    if (o->blessing > -1)
      Player.agi += abs(o->plus)+1;
    else
      Player.agi -= abs(o->plus)+1;
  }
  else {
    if (o->blessing > -1)
      Player.agi -= abs(o->plus)+1;
    else
      Player.agi += abs(o->plus)+1;
  }
  calc_melee();
}

void i_truesight(o)
pob o;
{
  o->known = 1;
  Objects[o->id].known = 1;
  truesight(o->blessing);
}

void i_perm_truesight(o)
pob o;
{
  o->known = 1;
  Objects[o->id].known = 1;
  if (o->used){
    if (o->blessing > -1) {
      Player.status[TRUESIGHT] += 1500;
      mprint("You feel sharp!");
    }
    else {
      Player.status[BLINDED] += 1500;
      mprint("You've been blinded!");
    }
  }
  else {
    if (o->blessing > -1) {
      Player.status[TRUESIGHT] -= 1500;
      if (Player.status[TRUESIGHT] < 1) {
	mprint("You feel less keen now.");
	Player.status[TRUESIGHT] = 0;
      }
    }
    else {
      Player.status[BLINDED] -= 1500;
      if (Player.status[BLINDED] < 1) {
	mprint("You can see again!"); 
	Player.status[BLINDED] = 0;
      }
    }
  } 
}

void i_dispel(o)
pob o;
{
  dispel((o->blessing > -1) ? o->blessing+random_range(3): o->blessing);
}

void i_crystal(o)
pob o;
{
  static int viewhour = 0;
  if (!o->known) mprint("You can't figure out how to activate this orb.");
  else {
    mprint("You gaze into your crystal ball.");
    if (viewhour == hour()) mprint("All you get is Gilligan's Island reruns.");
    else if ((o->blessing<0) || (Player.iq+Player.level < random_range(30))) {
      viewhour = hour();
      mprint("Weird interference patterns from the crystal fog your mind....");
      amnesia();
    }
    else {
      viewhour = hour();
      mprint("You sense the presence of life...");
      mondet(1);
      morewait();
      mprint("You sense the presence of objects...");
      objdet(1);
      morewait();
      mprint("You begin to see visions of things beyond your ken....");
      hint();
    }
  }
}
    
void i_antioch(o)
pob o;
{
  int x=Player.x,y=Player.y;
  int count;
  if (!o->known){
    mprint("Ka-Boom!");
    mprint("You seem to have annihilated yourself.");
    p_death("The Holy Hand-Grenade of Antioch");
  }
  else {
    mprint("Bring out the Holy Hand-Grenade of Antioch!");
    setspot(&x,&y);
    mprint("Ok, you pull the pin.....");
    mprint("What do you count up to? ");
    count = parsenum();
    if ((count < 3)&&(Dungeon[Dlevel][x][y].creature!=NULL)) {
      mprint("Three shall be the number of thy counting.");
      mprint("And the number of thy counting shall be three.");
      mprint("Your target picks up the grenade and throws it back!");
      mprint("Ka-Boom!");
      p_death("The Holy Hand-Grenade of Antioch");
    }
    else if (count>3) {
      mprint("Three shall be the number of thy counting.");
      mprint("And the number of thy counting shall be three.");
      mprint("Ka-Boom!");
      p_death("The Holy Hand-Grenade of Antioch");
    }
    else {
      mprint("Ka-Boom!");
      gain_experience(1000);
      Dungeon[Dlevel][x][y].locchar = TRAP;
      Dungeon[Dlevel][x][y].p_locf = L_TRAP_DOOR;
      if (Dungeon[Dlevel][x][y].creature != NULL) {
	m_death(Dungeon[Dlevel][x][y].creature);
	mprint("You are covered with gore.");
      }
      Dungeon[Dlevel][x][y].things = NULL;
    }
  }
  conform_lost_object(o);
}

void i_kolwynia(o)
pob o;
{
  int i;
  if (! o->known) {
    mprint("You destroy youself with a mana storm. How sad.");
    p_death("Kolwynia, The Key That Was Lost");
  }
  else {
    gain_experience(5000);
    mprint("You seem to have gained complete mastery of magic.");
    Player.pow = Player.maxpow = 2*Player.maxpow;
    for(i=0;i<NUMSPELLS;i++)
      Spells[i].known = TRUE;
  }
  conform_lost_object(o);
}

void i_enchantment(o)
pob o;
{
  static int zaphour = 0;
  char response;
  if (zaphour == hour()) 
    mprint("The staff doesn't seem to have recharged yet.");
  else if (! o->known) {
    zaphour = hour();
    mprint("You blast the staff backwards....");
    dispel(-1);
  }
  else {
    zaphour = hour();
    mprint("Zap with white or black end [wb]");
    do response = mgetc(); while ((response != 'w') && (response != 'b'));
    mprint("The staff discharges!");
    if (response = 'w') enchant(o->blessing*2+1);
    else dispel(o->blessing);
  }
}

void i_helm(o)
pob o;
{
  static int helmhour = 0;
  if (helmhour == hour()) 
    mprint("The helm doesn't seem to have recharged yet.");
  else if (! o->known) {
    helmhour = hour();
    mprint("You put the helm on backwards....");
    p_teleport(-1);
  }
  else {
    helmhour = hour();
    mprint("Your environment fades.... and rematerializes.");
    p_teleport(o->blessing);
  }
}
 

void i_death(o)
pob o;
{
  mprint("Bad move...");
  p_death("The Potion of Death");
}

void i_life(o)
pob o;
{
  mprint("Good move.");
  Player.hp = Player.maxhp = 2 * Player.maxhp;
  conform_lost_object(o);
}

void i_illuminate(o)
pob o;
{
  o->known = 1;
  Objects[o->id].known = 1;
  illuminate(o->blessing);
}

void i_perm_illuminate(o)
pob o;
{
  o->known = 1;
  Objects[o->id].known = 1;
  if (o->used) 
    Player.status[ILLUMINATION]+=1500;
  else
    Player.status[ILLUMINATION]=max(0,Player.status[ILLUMINATION]-1500);
}



void i_desecrate(o)
pob o;
{
  o->known = 1;
  if (o->blessing > 0) {
    mprint("How weird, a blessed desecrator... ");
    mprint("The structure of reality cannot permit such a thing....");
    *o = Objects[WEAPONID+0];
  }
  else if (Dungeon[Dlevel][Player.x][Player.y].locchar == ALTAR)
    sanctify(-1);
}


void i_trap(o)
pob o;
{
  Objects[o->id].known = 1;
  
  if ((Dungeon[Dlevel][Player.x][Player.y].locchar != FLOOR) ||
      (Dungeon[Dlevel][Player.x][Player.y].p_locf != L_NO_OP))
    mprint("Your attempt fails.");
  else  if (! o->known) {
    mprint("Fiddling with the thing, you have a small accident....");
    p_movefunction(o->aux);
  }
  else {
    mprint("You successfully set a trap at your location.");
    Dungeon[Dlevel][Player.x][Player.y].p_locf = o->aux;
  }
  conform_lost_objects(1,o);
}

void i_juggernaut(o)
pob o;
{
  int d,x=Player.x,y=Player.y;
  mprint("You activate the Juggernaut of Karnak!");
  if (! o->known) {
    mprint("Uh, oh, it's coming this way!");
    p_death("The Juggernaut of Karnak");
  }
  else {
    d = getdir();
    if (d == ABORT)
      mprint("You deactivate the Juggernaut before it escapes.");
    else {
      mprint("Vroom!");
      while (inbounds(x+Dirs[0][d],y+Dirs[1][d])) {
	x+=Dirs[0][d];
	y+=Dirs[1][d];
	Dungeon[Dlevel][x][y].locchar = FLOOR;
	Dungeon[Dlevel][x][y].p_locf = L_NO_OP;
	if (Dungeon[Dlevel][x][y].creature != NULL) {
	  mprint("You hear a distant scream....");
	  m_death(Dungeon[Dlevel][x][y].creature);
	}
      }
      gain_experience(1000);
    }
  }
  conform_lost_object(o);
}


void i_symbol(o)
pob o;
{
  static int usehour = -1;
  int i;
  if (! o->known)
    mprint("Nothing seems to happen.");
  /* if o->charge != 17, then symbol was stolen from own high priest! */
  else if ((o->aux != Player.patron) || (o->charge != 17)){
    mprint("You invoke the deity...");
    mprint("...who for some reason seems rather annoyed at you...");
    mprint("You are enveloped in Godsfire!");
    morewait();
    for(;Player.hp>1;Player.hp--) {
      dataprint();
      morewait();
      for(i=0;i<MAXITEMS;i++)
	if (Player.possessions[i] != NULL)
	  conform_lost_object(Player.possessions[i]);
      Player.mana = 0;
    }
  }
  else if (usehour == hour()) {
    mprint("Your deity frowns upon this profligate use of power...");
    mprint("Shazam! A bolt of Godsfire! Your symbol shatters!");
    conform_lost_object(o);
    Player.hp = 1;
    dataprint();
  }
  else {
    usehour = hour();
    cleanse(1);
    heal(10);
    Player.mana = max(Player.mana,calcmana());
  }
}
