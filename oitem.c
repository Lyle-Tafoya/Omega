/* omega copyright (C) 1987 by Laurence Raphael Brothers */
/* oitem.c */

#include <strings.h>
#include "oglob.h"


/* from oetc */
extern void hint();

/* from outil */
extern int random_range(),pow2();

/* from oitemf */
extern void i_no_op(),i_nothing(),i_heal(),i_victrix(),i_desecrate();
extern void i_spells(),i_bless(),i_enchant(),i_acquire(),i_jane_t();
extern void i_flux(),i_breathing(),i_invisible(),i_perm_invisible();
extern void i_teleport(),i_wish(),i_clairvoyance(),i_displace(),i_id();
extern void i_objdet(),i_mondet(),i_sleep_self(),i_neutralize_poison();
extern void i_speed(),i_restore(),i_azoth(),i_regenerate(),i_demonblade();
extern void i_firebolt(),i_lbolt(),i_missile(),i_fireball(),i_lball();
extern void i_sleep_other(),i_summon(),i_hide(),i_disrupt(),i_disintegrate();
extern void i_perm_vision(),i_perm_burden(),i_perm_fire_resist(),i_charge();
extern void i_perm_poison_resist(),i_perm_strength(),i_perm_gaze_immune();
extern void i_perm_energy_resist();void i_perm_speed(),i_perm_breathing();
extern void i_perm_displace(),i_perm_negimmune(),i_mace_disrupt(),i_warp();
extern void i_food(),i_poison_food(),i_stim(),i_immune(),i_lembas();
extern void i_orbfire(),i_orbwater(),i_orbair(),i_orbearth(),i_orbdead();
extern void i_orbmastery(),i_lightsabre(),i_alert(),i_snowball(),i_pow();
extern void i_knowledge(),i_perm_knowledge(),i_fear_resist(),i_corpse();
extern void i_perm_fear_resist(),i_pick(),i_key(),i_defend(),i_apport();
extern void i_chaos(),i_law(),i_augment(),i_perm_regenerate(),i_symbol();
extern void i_accuracy(),i_perm_accuracy(),i_hero(),i_perm_hero();
extern void i_levitate(),i_perm_levitate(),i_perm_protection();
extern void i_perm_agility(),i_cure(),i_dispel(),i_pepper_food();
extern void i_enchantment(),i_polymorph(),i_illuminate(),i_perm_illuminate();
extern void i_crystal(),i_kolwynia(),i_death(),i_life(),i_helm(),i_antioch();
extern void i_juggernaut(),i_trap(),i_truesight(),i_perm_truesight();

/* oitem functions */
void item_use();
pob create_object();
void make_shield(),make_armor(),make_weapon();
void make_scroll(),make_potion(),make_food();
void make_thing(),make_ring(),make_artifact();
void make_boots(),make_cloak(),make_stick(),make_cash();

int itemplus(),itemcharge(),itemblessing(),twohanded_p();


void scrollname(),potionname(),stickname(),ringname(),cloakname(),grotname();
void bootname();

/* make a random new object, returning pointer */
pob create_object()
{
  pob new;
  int  r; 
  int ok = FALSE;

  while (! ok) {
    new = ((pob) malloc(sizeof(objtype)));
    r= random_range(135);
    if (r < 20) make_thing(new,-1);
    else if (r < 40) make_food(new,-1);
    else if (r < 50) make_scroll(new,-1);
    else if (r < 60) make_potion(new,-1);
    else if (r < 70) make_weapon(new,-1);
    else if (r < 80) make_armor(new,-1);
    else if (r < 90) make_shield(new,-1);
    else if (r < 100) make_stick(new,-1);
    else if (r < 110) make_boots(new,-1);
    else if (r < 120) make_cloak(new,-1);
    else if (r < 130) make_ring(new,-1);
    else make_artifact(new,-1);
    /* not ok if object is too good for level, or if unique and already made */
    /* 1/100 chance of finding object if too good for level */
    ok = ((new->uniqueness != UNIQUE_MADE) &&
	  ((new->level*3 < Dlevel+random_range(3)) 
	   || ((Dlevel == 0) && (new->level < 5)) /* better stuff in city */
	   || (random_range(100)==23)));
    if (! ok)    free((char *) new);
  }
  if (new->uniqueness == UNIQUE_UNMADE) 
    Objects[new->id].uniqueness=UNIQUE_MADE;
  return(new);
}

void make_cash(new,level)
pob new;
int level;
{
  *new = Objects[CASHID];
  new->basevalue = random_range(level*level+10)+1; /* aux is AU value */
  strcpy(new->objstr,cashstr());
  strcpy(new->truename,cashstr());
  strcpy(new->cursestr,cashstr());
}

void make_food(new,id)
pob new;
int id;
{
  if (id == -1) id = random_range(NUMFOODS);
  *new = Objects[FOODID+id];
}

void make_ring(new,id)
pob new;
int id;
{
  if (id == -1) id = random_range(NUMRINGS);
  *new = Objects[RINGID+id];
  if (new->blessing == 0) new->blessing = itemblessing();
  if (new->plus == 0) new->plus = itemplus()+1;
  if (new->blessing < 0) new->plus = -1 - abs(new->plus);
}

void make_thing(new,id)
pob new;
int id;
{
  if (id == -1) id = random_range(NUMTHINGS);
  *new = Objects[THINGID+id];
  if (strcmp(new->objstr,"grot") == 0) {
    grotname(new->objstr);
    strcpy(new->truename,new->objstr);
    strcpy(new->cursestr,new->objstr);
  }
}


void make_scroll(new,id)
pob new;
int id;
{
  if (id == -1) id = random_range(NUMSCROLLS);
  *new = Objects[SCROLLID+id];
  /* if a scroll of spells, aux is the spell id in Spells */
  if (new->id == SCROLLID+1) {
    new->aux = random_range(NUMSPELLS);
  }
}

void make_potion(new,id)
pob new;
int id;
{
  if (id == -1) id = random_range(NUMPOTIONS);
  *new = Objects[POTIONID+id];
  if (new->plus == 0) new->plus = itemplus();
}

void make_weapon(new,id)
pob new;
int id;
{
  if (id == -1) id = random_range(NUMWEAPONS);
  *new = Objects[WEAPONID+id];
  if ((id == 28) || (id == 29)) /* bolt or arrow */
    new->number = random_range(20)+1;
  if (new->blessing == 0) new->blessing = itemblessing();
  if (new->plus == 0) {
    new->plus = itemplus();
    if (new->blessing < 0)
      new->plus = -1 - abs(new->plus);
    else if (new->blessing > 0)
      new->plus = 1 + abs(new->plus);
  }
}

void make_shield(new,id)
pob new;
int id;
{
  if (id == -1) id = random_range(NUMSHIELDS);
  *new = Objects[SHIELDID+id];
  if (new->plus == 0)
    new->plus = itemplus();
  if (new->blessing == 0) new->blessing = itemblessing();
  if (new->blessing < 0)
    new->plus = -1 - abs(new->plus);
  else if (new->blessing > 0)
    new->plus = 1 + abs(new->plus);
}

void make_armor(new,id)
pob new;
int id;
{
  if (id == -1) id = random_range(NUMARMOR);
  *new = Objects[ARMORID+id];
  if (new->plus == 0) new->plus = itemplus();
  if (new->blessing == 0) new->blessing = itemblessing();
  if (new->blessing < 0)
    new->plus = -1 - abs(new->plus);
  else if (new->blessing > 0)
    new->plus = 1 + abs(new->plus);  
}

void make_cloak(new,id)
pob new;
int id;
{
  if (id == -1) id = random_range(NUMCLOAKS);
  Objects[CLOAKID+4].plus = 2;
  *new = Objects[CLOAKID+id];
  if (new->blessing == 0) new->blessing = itemblessing();
}

void make_boots(new,id)
pob new;
int id;
{
  if (id == -1) id = random_range(NUMBOOTS);
  *new = Objects[BOOTID+id];
  if (new->blessing == 0) new->blessing = itemblessing();
}

void make_stick(new,id)
pob new;
int id;
{
  if (id == -1) id = random_range(NUMSTICKS);
  *new = Objects[STICKID+id];
  new->charge = itemcharge();
  if (new->blessing == 0) new->blessing = itemblessing();
}

void make_artifact(new,id)
pob new;
{
  if (id == -1) id = random_range(NUMARTIFACTS);
  *new = Objects[ARTIFACTID+id];
}




/* item name functions */

void scrollname(reset,sstr)
int reset;
char sstr[64];
{
  static int used[30];
  int i;

  if (reset)
    for(i=0;i<30;i++)
      used[i]=FALSE;
  else {
    do 
      i = random_range(30);
    while(used[i]);
    
    used[i] = TRUE;
    
    switch(i) {
      case 0: strcpy(sstr,"scroll-GRISTOGRUE"); break;
      case 1: strcpy(sstr,"scroll-Kho Reck Tighp"); break;
      case 2: strcpy(sstr,"scroll-E Z"); break;
      case 3: strcpy(sstr,"scroll-Kevitz"); break;
      case 4: strcpy(sstr,"scroll-Arcanum Prime"); break;
      case 5: strcpy(sstr,"scroll-NYARLATHOTEP"); break;
      case 6: strcpy(sstr,"scroll-Gilthoniel"); break;
      case 7: strcpy(sstr,"scroll-Zarathustra"); break;
      case 8: strcpy(sstr,"scroll-Ancient Lore"); break;
      case 9: strcpy(sstr,"scroll-Eyes Only"); break;
      case 10: strcpy(sstr,"scroll-Ambogar Empheltz"); break;
      case 11: strcpy(sstr,"scroll-Isengard"); break;
      case 12: strcpy(sstr,"scroll-Deosil Widdershins"); break;
      case 13: strcpy(sstr,"scroll-Magister Paracelsus"); break;
      case 14: strcpy(sstr,"scroll-Qlipphotic Summons"); break;
      case 15: strcpy(sstr,"scroll-Aratron Samael"); break;
      case 16: strcpy(sstr,"scroll-De Wormiis Mysterius"); break;
      case 17: strcpy(sstr,"scroll-Necronomicon"); break;
      case 18: strcpy(sstr,"scroll-Pnakotic Manuscript"); break;
      case 19: strcpy(sstr,"scroll-Codex of Xalimar"); break;
      case 20: strcpy(sstr,"scroll-The Mabinogion"); break;
      case 21: strcpy(sstr,"scroll-Ginseng Shiatsu"); break;
      case 22: strcpy(sstr,"scroll-Tome of Tromax"); break;
      case 23: strcpy(sstr,"scroll-Book of the Dead "); break;
      case 24: strcpy(sstr,"scroll-The Flame Tongue"); break;
      case 25: strcpy(sstr,"scroll-Karst Khogar"); break;
      case 26: strcpy(sstr,"scroll-The Yellow Sign"); break;
      case 27: strcpy(sstr,"scroll-The Kevillist Manifesto"); break;
      case 28: strcpy(sstr,"scroll-Goshtar Script"); break;
      case 29: strcpy(sstr,"scroll-Pendragon Encryption"); break;
    }
  }
}

/* actually copies into gstr for some reason */
void grotname(gstr)
char gstr[64];
{
  switch(random_range(20)) {
    case 0: strcpy(gstr,"pot lid"); break;
    case 1: strcpy(gstr,"mound of offal"); break;
    case 2: strcpy(gstr,"sword that was broken"); break;
    case 3: strcpy(gstr,"salted snail"); break;
    case 4: strcpy(gstr,"key"); break;
    case 5: strcpy(gstr,"toadstool"); break;
    case 6: strcpy(gstr,"greenish spindle"); break;
    case 7: strcpy(gstr,"tin soldier"); break;
    case 8: strcpy(gstr,"broken yo-yo"); break;
    case 9: strcpy(gstr,"NYC subway map"); break;
    case 10: strcpy(gstr,"Nixon's the One! button"); break;
    case 11: strcpy(gstr,"beer can (empty)"); break;
    case 12: strcpy(gstr,"golden bejewelled falcon"); break;
    case 13: strcpy(gstr,"hamster cage"); break;
    case 14: strcpy(gstr,"wooden nickel"); break;
    case 15: strcpy(gstr,"three-dollar bill"); break;
    case 16: strcpy(gstr,"rosebud"); break;
    case 17: strcpy(gstr,"water pistol"); break;
    case 18: strcpy(gstr,"shattered skull"); break;
    case 19: strcpy(gstr,"jawbone of an ass"); break;
  }
}




void potionname(reset,pstr)
int reset;
char pstr[64];
{
  static int used[20];
  int i;
  if (reset)
    for(i=0;i<20;i++)
      used[i]=FALSE;
  else {
    do 
      i = random_range(20);
    while(used[i]);
    
    used[i] = TRUE;

    switch (i) {
      case 0: strcpy(pstr,"vial of dewy liquid"); break;
      case 1: strcpy(pstr,"jug of tarry black substance"); break;
      case 2: strcpy(pstr,"flask of cold smoking froth"); break;
      case 3: strcpy(pstr,"phial of glowing fluid"); break;
      case 4: strcpy(pstr,"bottle of sickening slime"); break;
      case 5: strcpy(pstr,"sac of greenish gel"); break;
      case 6: strcpy(pstr,"wineskin of odorous goo"); break;
      case 7: strcpy(pstr,"canteen of sweet sap"); break;
      case 8: strcpy(pstr,"urn of clear fluid"); break;
      case 9: strcpy(pstr,"clotted grey ooze"); break;
      case 10: strcpy(pstr,"keg of bubbly golden fluid"); break;
      case 11: strcpy(pstr,"tube of minty paste"); break;
      case 12: strcpy(pstr,"pitcher of aromatic liquid"); break;
      case 13: strcpy(pstr,"pot of rancid grease"); break;
      case 14: strcpy(pstr,"thermos of hot black liquid"); break;
      case 15: strcpy(pstr,"magnum of deep red liquid"); break;
      case 16: strcpy(pstr,"vase full of ichor"); break;
      case 17: strcpy(pstr,"container of white cream"); break;
      case 18: strcpy(pstr,"syringe of clear fluid"); break;
      case 19: strcpy(pstr,"can of volatile essence"); break;
    }      
  }
}


void stickname(reset,sstr)
int reset;
char sstr[64];
{
  static int used[20];
  int i;
  if (reset)
    for(i=0;i<20;i++)
      used[i]=FALSE;
  else {
    do 
      i = random_range(20);
    while(used[i]);
    
    used[i] = TRUE;

    switch (i) {
      case 0: strcpy(sstr,"oaken staff"); break;
      case 1: strcpy(sstr,"heavy metal rod"); break;
      case 2: strcpy(sstr,"shaft of congealed light"); break;
      case 3: strcpy(sstr,"slender ceramic wand"); break;
      case 4: strcpy(sstr,"rune-inscribed bone wand"); break;
      case 5: strcpy(sstr,"knurly staff"); break;
      case 6: strcpy(sstr,"steel knobbed rod");  break;
      case 7: strcpy(sstr,"lucite wand"); break;
      case 8: strcpy(sstr,"sturdy alpenstock"); break;
      case 9: strcpy(sstr,"gem-studded ebony staff"); break;
      case 10: strcpy(sstr,"chromed sequinned staff"); break;
      case 11: strcpy(sstr,"old peeling stick"); break;
      case 12: strcpy(sstr,"jointed metal rod"); break;
      case 13: strcpy(sstr,"wand with lead ferrules"); break;
      case 14: strcpy(sstr,"forked wooden stick"); break;
      case 15: strcpy(sstr,"cane with gold eagle handle"); break;
      case 16: strcpy(sstr,"crystalline wand");  break;
      case 17: strcpy(sstr,"metal stick with trigger"); break;
      case 18: strcpy(sstr,"leather-handled stone rod"); break;
      case 19: strcpy(sstr,"tiny mithril wand"); break;
    }      
  }
}

void ringname(reset,rstr)
int reset;
char rstr[64];
{
  static int used[20];
  int i;
  if (reset)
    for(i=0;i<20;i++)
      used[i]=FALSE;
  else {
    do 
      i = random_range(20);
    while(used[i]);
    
    used[i] = TRUE;

    switch (i) {
      case 0: strcpy(rstr,"gold ring with a blue gem"); break;
      case 1: strcpy(rstr,"brass ring"); break;
      case 2: strcpy(rstr,"mithril ring with a red gem"); break;
      case 3: strcpy(rstr,"platinum ring");  break;
      case 4: strcpy(rstr,"gold dragon's head ring"); break;
      case 5: strcpy(rstr,"bronze ring"); break;
      case 6: strcpy(rstr,"aardvark seal ring"); break;
      case 7: strcpy(rstr,"grey metal ring"); break;
      case 8: strcpy(rstr,"silver skull ring"); break;
      case 9: strcpy(rstr,"onyx ring"); break;
      case 10: strcpy(rstr,"Collegium Magii class ring"); break;
      case 11: strcpy(rstr,"worn stone ring"); break;
      case 12: strcpy(rstr,"diorite ring"); break;
      case 13: strcpy(rstr,"ancient scarab ring");  break;
      case 14: strcpy(rstr,"plastic charm ring"); break;
      case 15: strcpy(rstr,"soapy gypsum ring"); break;
      case 16: strcpy(rstr,"glass ring"); break;
      case 17: strcpy(rstr,"glowing bluestone ring"); break;
      case 18: strcpy(rstr,"ring with eye sigil"); break;
      case 19: strcpy(rstr,"zirconium ring"); break;
    }      
  }
}


void cloakname(reset,cstr)
int reset;
char cstr[64];
{
  static int used[20];
  int i;
  if (reset)
    for(i=0;i<20;i++)
      used[i]=FALSE;
  else {
    do 
      i = random_range(20);
    while(used[i]);
    
    used[i] = TRUE;

    switch (i) {
      case 0: strcpy(cstr,"tattered piece of cloth"); break;
      case 1: strcpy(cstr,"fuligin cloak"); break;
      case 2: strcpy(cstr,"chintz cloak"); break;
      case 3: strcpy(cstr,"diaphanous cape");  break;
      case 4: strcpy(cstr,"red half-cloak"); break;
      case 5: strcpy(cstr,"mouse-hide cloak"); break;
      case 6: strcpy(cstr,"kelly green cloak"); break;
      case 7: strcpy(cstr,"cloth-of-gold cloak"); break;
      case 8: strcpy(cstr,"dirty old cloak"); break;
      case 9: strcpy(cstr,"weightless cloak"); break;
      case 10: strcpy(cstr,"boat cloak"); break;
      case 11: strcpy(cstr,"greasy tarpaulin"); break;
      case 12: strcpy(cstr,"sable cloak"); break;
      case 13: strcpy(cstr,"soft velvet cloak");  break;
      case 14: strcpy(cstr,"opera cape"); break;
      case 15: strcpy(cstr,"elegant brocade cloak"); break;
      case 16: strcpy(cstr,"cloak of many colors"); break;
      case 17: strcpy(cstr,"grey-green rag"); break;
      case 18: strcpy(cstr,"puce and chartreuse cloak"); break;
      case 19: strcpy(cstr,"smoky cloak"); break;
    }      
  }
}

void bootname(reset,cstr)
int reset;
char cstr[64];
{
  static int used[20];
  int i;
  if (reset)
    for(i=0;i<20;i++)
      used[i]=FALSE;
  else {
    do 
      i = random_range(20);
    while(used[i]);
    
    used[i] = TRUE;

    switch (i) {
      case 0: strcpy(cstr,"sturdy leather boots"); break;
      case 1: strcpy(cstr,"dayglo spandex socks"); break;
      case 2: strcpy(cstr,"dark-colored tabi"); break;
      case 3: strcpy(cstr,"patent-leather shoes");  break;
      case 4: strcpy(cstr,"beaten-up gumshoes"); break;
      case 5: strcpy(cstr,"alligator-hide boots"); break;
      case 6: strcpy(cstr,"comfortable sandals"); break;
      case 7: strcpy(cstr,"roller skates"); break;
      case 8: strcpy(cstr,"purple suede gaiters"); break;
      case 9: strcpy(cstr,"mirror-plated wingtips"); break;
      case 10: strcpy(cstr,"heavy workboots"); break;
      case 11: strcpy(cstr,"polyurethane-soled sneakers"); break;
      case 12: strcpy(cstr,"clodhoppers"); break;
      case 13: strcpy(cstr,"wooden shoes");  break;
      case 14: strcpy(cstr,"ski boots"); break;
      case 15: strcpy(cstr,"hob-nailed boots"); break;
      case 16: strcpy(cstr,"elven boots"); break;
      case 17: strcpy(cstr,"cowboy boots"); break;
      case 18: strcpy(cstr,"flipflop slippers"); break;
      case 19: strcpy(cstr,"riding boots"); break;
    }      
  }
}



int itemplus()
{
  int p = 0;

  while (random_range(2) == 0)
    p++;
  return(p);
}



int itemcharge()
{
  return(random_range(20)+1);
}



int itemblessing()
{
  switch(random_range(10)) {
    case 0:
    case 1:return(-1-random_range(10)); break;
    case 8:
    case 9:return(1+random_range(10)); break;
    default: return(0);	   
  }
}

    
int twohanded_p(id)
int id;
{
  switch(id) {
  case WEAPONID+5:
  case WEAPONID+12:
  case WEAPONID+18:
  case WEAPONID+20:
  case WEAPONID+24:
  case WEAPONID+26:
  case WEAPONID+27:
  case WEAPONID+32:
  case WEAPONID+36:
    return(TRUE); break;
  default: return(FALSE); break;
  }
}


void item_use(o)
struct object *o;
{
  switch(o->usef) {
    case -1:i_no_op(o); break;
    case 0:i_nothing(o); break;

    /* scrolls */	    
    case I_SPELLS: i_spells(o); break;
    case I_BLESS: i_bless(o); break;
    case I_ACQUIRE: i_acquire(o); break;
    case I_ENCHANT: i_enchant(o); break;
    case I_TELEPORT: i_teleport(o); break;
    case I_WISH: i_wish(o); break;
    case I_CLAIRVOYANCE: i_clairvoyance(o); break;
    case I_DISPLACE: i_displace(o); break;
    case I_ID: i_id(o); break;
    case I_JANE_T: i_jane_t(o); break;
    case I_FLUX: i_flux(o); break;
    case I_WARP: i_warp(o); break;
    case I_ALERT: i_alert(o); break;
    case I_CHARGE: i_charge(o); break;
    case I_KNOWLEDGE: i_knowledge(o); break;
    case I_LAW: i_law(o); break;
    case I_HINT: hint(); break;
    case I_HERO: i_hero(o); break;
    case I_TRUESIGHT: i_truesight(o); break;
    case I_ILLUMINATE: i_illuminate(o); break;

    /* potion functions */
    case I_HEAL: i_heal(o); break;
    case I_OBJDET: i_objdet(o); break;
    case I_MONDET: i_mondet(o); break;
    case I_SLEEP_SELF: i_sleep_self(o); break;
    case I_NEUTRALIZE_POISON: i_neutralize_poison(o); break;
    case I_RESTORE: i_restore(o); break;
    case I_AZOTH: i_azoth(o); break;
    case I_AUGMENT: i_augment(o); break;
    case I_REGENERATE: i_regenerate(o); break;
    case I_INVISIBLE: i_invisible(o); break;
    case I_BREATHING: i_breathing(o); break;
    case I_FEAR_RESIST: i_fear_resist(o); break;
    case I_CHAOS: i_chaos(o); break;
    case I_ACCURACY: i_accuracy(o); break;
    case I_LEVITATION: i_levitate(o); break;
    case I_CURE: i_cure(o); break;
	    
    /* stick functions */
    case I_FIREBOLT: i_firebolt(o); break;
    case I_LBOLT: i_lbolt(o); break;
    case I_MISSILE: i_missile(o); break;
    case I_SLEEP_OTHER: i_sleep_other(o); break;
    case I_FIREBALL: i_fireball(o); break;
    case I_LBALL: i_lball(o); break;    
    case I_SNOWBALL: i_snowball(o); break;
    case I_SUMMON: i_summon(o); break;
    case I_HIDE: i_hide(o); break;
    case I_DISRUPT: i_disrupt(o); break;
    case I_DISINTEGRATE: i_disintegrate(o); break;
    case I_APPORT: i_apport(o); break;
    case I_DISPEL: i_dispel(o); break;
    case I_POLYMORPH: i_polymorph(o); break;

    /* food functions */
    case I_FOOD: i_food(o); break;
    case I_LEMBAS: i_lembas(o); break;
    case I_STIM: i_stim(o); break;
    case I_POW: i_pow(o); break;
    case I_IMMUNE: i_immune(o); break;
    case I_POISON_FOOD: i_poison_food(o); break;
    case I_PEPPER_FOOD: i_pepper_food(o); break;
    case I_CORPSE: i_corpse(o); break;

    /* boots functions */
    case I_PERM_SPEED: i_perm_speed(o); break;
    case I_PERM_HERO: i_perm_hero(o); break;
    case I_PERM_LEVITATE: i_perm_levitate(o); break;
    case I_PERM_AGILITY: i_perm_agility(o); break;
      
    /* artifact functions */
    case I_SYMBOL:i_symbol(o); break;
    case I_ORBMASTERY: i_orbmastery(o); break;
    case I_ORBFIRE: i_orbfire(o); break;
    case I_ORBWATER: i_orbwater(o); break;
    case I_ORBEARTH: i_orbearth(o); break;
    case I_ORBAIR: i_orbair(o); break;
    case I_ORBDEAD: i_orbdead(o); break;
    case I_CRYSTAL: i_crystal(o); break;
    case I_LIFE: i_life(o); break;
    case I_DEATH: i_death(o); break;
    case I_ANTIOCH: i_antioch(o); break;
    case I_HELM: i_helm(o); break;
    case I_KOLWYNIA: i_kolwynia(o); break;
    case I_ENCHANTMENT: i_enchantment(o); break;
    case I_JUGGERNAUT: i_juggernaut(o); break;

    /* cloak functions */
    case I_PERM_DISPLACE: i_perm_displace(o); break;
    case I_PERM_NEGIMMUNE: i_perm_negimmune(o); break;
    case I_PERM_INVISIBLE: i_perm_invisible(o); break;
    case I_PERM_PROTECTION: i_perm_protection(o);break;
    case I_PERM_ACCURACY: i_perm_accuracy(o);break;
    case I_PERM_TRUESIGHT: i_perm_truesight(o); break;

    /* ring functions */
    case I_PERM_VISION: i_perm_vision(o); break;
    case I_PERM_BURDEN: i_perm_burden(o); break;
    case I_PERM_STRENGTH: i_perm_strength(o); break;
    case I_PERM_GAZE_IMMUNE: i_perm_gaze_immune(o); break;
    case I_PERM_FIRE_RESIST: i_perm_fire_resist(o); break;
    case I_PERM_POISON_RESIST: i_perm_poison_resist(o); break;
    case I_PERM_REGENERATE: i_perm_regenerate(o); break;
    case I_PERM_KNOWLEDGE: i_perm_knowledge(o); break;

    /* armor functions */
    case I_PERM_FEAR_RESIST: i_perm_fear_resist(o); break;
    case I_PERM_ENERGY_RESIST: i_perm_energy_resist(o); break;
    case I_PERM_BREATHING: i_perm_breathing(o); break;

    /* weapons functions */
    case I_LIGHTSABRE: i_lightsabre(o); break;
    case I_DEMONBLADE: i_demonblade(o); break;
    case I_DESECRATE: i_desecrate(o); break;
    case I_MACE_DISRUPT: i_mace_disrupt(o); break;
    case I_DEFEND: i_defend(o); break;
    case I_VICTRIX: i_victrix(o); break;

    /* thing functions */	    
    case I_PICK: i_pick(o); break;
    case I_KEY: i_key(o); break;
    case I_PERM_ILLUMINATE: i_perm_illuminate(o); break;
    case I_TRAP: i_trap(o); break;
  }
}

