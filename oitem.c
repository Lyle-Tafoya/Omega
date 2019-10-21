/* omega copyright (C) 1987,1988 by Laurence Raphael Brothers */
/* oitem.c */

#include "oglob.h"

/* make a random new object, returning pointer */
/* may return NULL. */
pob create_object(itemlevel)
int itemlevel;
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
	  ((new->level < itemlevel+random_range(3))
	   || (random_range(100)==23)));
    if (!ok) free((char *) new);
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
  new->objstr = salloc(cashstr());
  new->cursestr = new->truename = new->objstr;
}

void make_food(new,id)
pob new;
int id;
{
  if (id == -1) id = random_range(NUMFOODS);
  *new = Objects[FOODID+id];
}


void make_corpse(new,m)
pob new;
struct monster *m;
{
  *new = Objects[CORPSEID];
  new->charge = m->id;
  new->weight = m->corpseweight;
  new->basevalue = m->corpsevalue;
  new->known = 2;
  new->objstr = salloc(m->corpsestr);
  new->truename = new->cursestr = new->objstr;
  if (m->monchar == '@')
    new->usef = I_CANNIBAL;
  else if (m_statusp(m,EDIBLE)) {
    new->usef = I_FOOD;
    new->aux = 6;
  }
  else if (m_statusp(m,POISONOUS))
    new->usef = I_POISON_FOOD;
  /* Special corpse-eating effects */
  else switch(m->id) {
  case ML1+1: /*tse tse fly */
  case ML4+9: /*torpor beast */
    new->usef = I_SLEEP_SELF;
    break;
  case ML2+5:
    new->usef = I_INVISIBLE;
    break;
  case ML1+5: /* blipper */
    new->usef = I_TELEPORT;
    break;
  case ML2+3: /* floating eye -- it's traditional.... */
    new->usef = I_CLAIRVOYANCE;
    break;
  case ML4+11: /*astral fuzzy */
    new->usef = I_DISPLACE;
    break;
  case ML4+12: /*s o law */
    new->usef = I_CHAOS;
    break;
  case ML4+13: /*s o chaos */
    new->usef = I_LAW;
    break;
  case ML5+9: /* astral vampire */
    new->usef = I_ENCHANT;
    break;
  case ML5+11: /* manaburst */
    new->usef = I_SPELLS;
    break;
  case ML6+9: /* rakshasa */
    new->usef = I_TRUESIGHT;
    break;
  case ML7+0: /* behemoth */
    new->usef = I_HEAL;
    break;
  case ML7+2: /* unicorn */
    new->usef = I_NEUTRALIZE_POISON;
    break;
  case ML8+10: /*coma beast */
    new->usef = I_ALERT;
    break;
  default:
    new->usef = I_INEDIBLE; 
    break;
  }
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
    new->objstr = salloc(grotname());
    new->truename = new->cursestr = new->objstr;
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

char *scrollname(reset,id)
int reset,id;

{
  static int ids[30];
  int i,j,k;

  if (reset) {
    for(i=0;i<30;i++)
      ids[i]=i;
    for(i=0;i<500;i++) {
      j = random_range(30);
      k = ids[i % 30];
      ids[i % 30]=ids[j];
      ids[j]=k;
    }
  }
  else {
    switch(ids[id]) {
      case 0: strcpy(Str4,"scroll-GRISTOGRUE"); break;
      case 1: strcpy(Str4,"scroll-Kho Reck Tighp"); break;
      case 2: strcpy(Str4,"scroll-E Z"); break;
      case 3: strcpy(Str4,"scroll-Kevitz"); break;
      case 4: strcpy(Str4,"scroll-Arcanum Prime"); break;
      case 5: strcpy(Str4,"scroll-NYARLATHOTEP"); break;
      case 6: strcpy(Str4,"scroll-Gilthoniel"); break;
      case 7: strcpy(Str4,"scroll-Zarathustra"); break;
      case 8: strcpy(Str4,"scroll-Ancient Lore"); break;
      case 9: strcpy(Str4,"scroll-Eyes Only"); break;
      case 10: strcpy(Str4,"scroll-Ambogar Empheltz"); break;
      case 11: strcpy(Str4,"scroll-Isengard"); break;
      case 12: strcpy(Str4,"scroll-Deosil Widdershins"); break;
      case 13: strcpy(Str4,"scroll-Magister Paracelsus"); break;
      case 14: strcpy(Str4,"scroll-Qlipphotic Summons"); break;
      case 15: strcpy(Str4,"scroll-Aratron Samael"); break;
      case 16: strcpy(Str4,"scroll-De Wormiis Mysterius"); break;
      case 17: strcpy(Str4,"scroll-Necronomicon"); break;
      case 18: strcpy(Str4,"scroll-Pnakotic Manuscript"); break;
      case 19: strcpy(Str4,"scroll-Codex of Xalimar"); break;
      case 20: strcpy(Str4,"scroll-The Mabinogion"); break;
      case 21: strcpy(Str4,"scroll-Ginseng Shiatsu"); break;
      case 22: strcpy(Str4,"scroll-Tome of Tromax"); break;
      case 23: strcpy(Str4,"scroll-Book of the Dead "); break;
      case 24: strcpy(Str4,"scroll-The Flame Tongue"); break;
      case 25: strcpy(Str4,"scroll-Karst Khogar"); break;
      case 26: strcpy(Str4,"scroll-The Yellow Sign"); break;
      case 27: strcpy(Str4,"scroll-The Kevillist Manifesto"); break;
      case 28: strcpy(Str4,"scroll-Goshtar Script"); break;
      case 29: strcpy(Str4,"scroll-Pendragon Encryption"); break;
      }
  }
  return(Str4);
}


char *grotname()
{
  switch(random_range(20)) {
    case 0: strcpy(Str4,"pot lid"); break;
    case 1: strcpy(Str4,"mound of offal"); break;
    case 2: strcpy(Str4,"sword that was broken"); break;
    case 3: strcpy(Str4,"salted snail"); break;
    case 4: strcpy(Str4,"key"); break;
    case 5: strcpy(Str4,"toadstool"); break;
    case 6: strcpy(Str4,"greenish spindle"); break;
    case 7: strcpy(Str4,"tin soldier"); break;
    case 8: strcpy(Str4,"broken yo-yo"); break;
    case 9: strcpy(Str4,"NYC subway map"); break;
    case 10: strcpy(Str4,"Nixon's the One! button"); break;
    case 11: strcpy(Str4,"beer can (empty)"); break;
    case 12: strcpy(Str4,"golden bejewelled falcon"); break;
    case 13: strcpy(Str4,"hamster cage"); break;
    case 14: strcpy(Str4,"wooden nickel"); break;
    case 15: strcpy(Str4,"three-dollar bill"); break;
    case 16: strcpy(Str4,"rosebud"); break;
    case 17: strcpy(Str4,"water pistol"); break;
    case 18: strcpy(Str4,"shattered skull"); break;
    case 19: strcpy(Str4,"jawbone of an ass"); break;
  }
  return(Str4);
}




char *potionname(reset,id)
int reset,id;
{
  static int ids[20];
  int i,j,k;

  if (reset) {
    for(i=0;i<20;i++)
      ids[i]=i;
    for(i=0;i<500;i++) {
      j = random_range(20);
      k = ids[i % 20];
      ids[i % 20]=ids[j];
      ids[j]=k;
    }
  }
  else {
    switch (ids[id]) {
      case 0: strcpy(Str4,"vial of dewy liquid"); break;
      case 1: strcpy(Str4,"jug of tarry black substance"); break;
      case 2: strcpy(Str4,"flask of cold smoking froth"); break;
      case 3: strcpy(Str4,"phial of glowing fluid"); break;
      case 4: strcpy(Str4,"bottle of sickening slime"); break;
      case 5: strcpy(Str4,"sac of greenish gel"); break;
      case 6: strcpy(Str4,"wineskin of odorous goo"); break;
      case 7: strcpy(Str4,"canteen of sweet sap"); break;
      case 8: strcpy(Str4,"urn of clear fluid"); break;
      case 9: strcpy(Str4,"clotted grey ooze"); break;
      case 10: strcpy(Str4,"keg of bubbly golden fluid"); break;
      case 11: strcpy(Str4,"tube of minty paste"); break;
      case 12: strcpy(Str4,"pitcher of aromatic liquid"); break;
      case 13: strcpy(Str4,"pot of rancid grease"); break;
      case 14: strcpy(Str4,"thermos of hot black liquid"); break;
      case 15: strcpy(Str4,"magnum of deep red liquid"); break;
      case 16: strcpy(Str4,"vase full of ichor"); break;
      case 17: strcpy(Str4,"container of white cream"); break;
      case 18: strcpy(Str4,"syringe of clear fluid"); break;
      case 19: strcpy(Str4,"can of volatile essence"); break;
      }      
    return(Str4);
  }
}


char *stickname(reset,id)
int reset,id;
{
  static int ids[20];
  int i,j,k;

  if (reset) {
    for(i=0;i<20;i++)
      ids[i]=i;
    for(i=0;i<500;i++) {
      j = random_range(20);
      k = ids[i % 20];
      ids[i % 20]=ids[j];
      ids[j]=k;
    }
  }
  else {
    switch (ids[id]) {
      case 0: strcpy(Str4,"oaken staff"); break;
      case 1: strcpy(Str4,"heavy metal rod"); break;
      case 2: strcpy(Str4,"shaft of congealed light"); break;
      case 3: strcpy(Str4,"slender ceramic wand"); break;
      case 4: strcpy(Str4,"rune-inscribed bone wand"); break;
      case 5: strcpy(Str4,"knurly staff"); break;
      case 6: strcpy(Str4,"steel knobbed rod");  break;
      case 7: strcpy(Str4,"lucite wand"); break;
      case 8: strcpy(Str4,"sturdy alpenstock"); break;
      case 9: strcpy(Str4,"gem-studded ebony staff"); break;
      case 10: strcpy(Str4,"chromed sequinned staff"); break;
      case 11: strcpy(Str4,"old peeling stick"); break;
      case 12: strcpy(Str4,"jointed metal rod"); break;
      case 13: strcpy(Str4,"wand with lead ferrules"); break;
      case 14: strcpy(Str4,"forked wooden stick"); break;
      case 15: strcpy(Str4,"cane with gold eagle handle"); break;
      case 16: strcpy(Str4,"crystalline wand");  break;
      case 17: strcpy(Str4,"metal stick with trigger"); break;
      case 18: strcpy(Str4,"leather-handled stone rod"); break;
      case 19: strcpy(Str4,"tiny mithril wand"); break;
      }      
    return(Str4);
  }
}

char *ringname(reset,id)
int reset,id;
{
  static int ids[20];
  int i,j,k;

  if (reset) {
    for(i=0;i<20;i++)
      ids[i]=i;
    for(i=0;i<500;i++) {
      j = random_range(20);
      k = ids[i % 20];
      ids[i % 20]=ids[j];
      ids[j]=k;
    }
  }
  else {
    switch (ids[id]) {
      case 0: strcpy(Str4,"gold ring with a blue gem"); break;
      case 1: strcpy(Str4,"brass ring"); break;
      case 2: strcpy(Str4,"mithril ring with a red gem"); break;
      case 3: strcpy(Str4,"platinum ring");  break;
      case 4: strcpy(Str4,"gold dragon's head ring"); break;
      case 5: strcpy(Str4,"bronze ring"); break;
      case 6: strcpy(Str4,"aardvark seal ring"); break;
      case 7: strcpy(Str4,"grey metal ring"); break;
      case 8: strcpy(Str4,"silver skull ring"); break;
      case 9: strcpy(Str4,"onyx ring"); break;
      case 10: strcpy(Str4,"Collegium Magii class ring"); break;
      case 11: strcpy(Str4,"worn stone ring"); break;
      case 12: strcpy(Str4,"diorite ring"); break;
      case 13: strcpy(Str4,"ancient scarab ring");  break;
      case 14: strcpy(Str4,"plastic charm ring"); break;
      case 15: strcpy(Str4,"soapy gypsum ring"); break;
      case 16: strcpy(Str4,"glass ring"); break;
      case 17: strcpy(Str4,"glowing bluestone ring"); break;
      case 18: strcpy(Str4,"ring with eye sigil"); break;
      case 19: strcpy(Str4,"zirconium ring"); break;
      }      
    return(Str4);
  }
}


char *cloakname(reset,id)
int reset,id;
{
  static int ids[20];
  int i,j,k;

  if (reset) {
    for(i=0;i<20;i++)
      ids[i]=i;
    for(i=0;i<500;i++) {
      j = random_range(20);
      k = ids[i % 20];
      ids[i % 20]=ids[j];
      ids[j]=k;
    }
  }
  else {
    switch (ids[id]) {
      case 0: strcpy(Str4,"tattered piece of cloth"); break;
      case 1: strcpy(Str4,"fuligin cloak"); break;
      case 2: strcpy(Str4,"chintz cloak"); break;
      case 3: strcpy(Str4,"diaphanous cape");  break;
      case 4: strcpy(Str4,"red half-cloak"); break;
      case 5: strcpy(Str4,"mouse-hide cloak"); break;
      case 6: strcpy(Str4,"kelly green cloak"); break;
      case 7: strcpy(Str4,"cloth-of-gold cloak"); break;
      case 8: strcpy(Str4,"dirty old cloak"); break;
      case 9: strcpy(Str4,"weightless cloak"); break;
      case 10: strcpy(Str4,"boat cloak"); break;
      case 11: strcpy(Str4,"greasy tarpaulin"); break;
      case 12: strcpy(Str4,"sable cloak"); break;
      case 13: strcpy(Str4,"soft velvet cloak");  break;
      case 14: strcpy(Str4,"opera cape"); break;
      case 15: strcpy(Str4,"elegant brocade cloak"); break;
      case 16: strcpy(Str4,"cloak of many colors"); break;
      case 17: strcpy(Str4,"grey-green rag"); break;
      case 18: strcpy(Str4,"puce and chartreuse cloak"); break;
      case 19: strcpy(Str4,"smoky cloak"); break;
      }      
    return(Str4);
  }
}

char *bootname(reset,id)
int reset,id;
{
  static int ids[20];
  int i,j,k;

  if (reset) {
    for(i=0;i<20;i++)
      ids[i]=i;
    for(i=0;i<500;i++) {
      j = random_range(20);
      k = ids[i % 20];
      ids[i % 20]=ids[j];
      ids[j]=k;
    }
  }
  else {
    switch (ids[id]) {
      case 0: strcpy(Str4,"sturdy leather boots"); break;
      case 1: strcpy(Str4,"calf-length moccasins"); break;
      case 2: strcpy(Str4,"dark-colored tabi"); break;
      case 3: strcpy(Str4,"patent-leather shoes");  break;
      case 4: strcpy(Str4,"beaten-up gumshoes"); break;
      case 5: strcpy(Str4,"alligator-hide boots"); break;
      case 6: strcpy(Str4,"comfortable sandals"); break;
      case 7: strcpy(Str4,"roller skates"); break;
      case 8: strcpy(Str4,"purple suede gaiters"); break;
      case 9: strcpy(Str4,"mirror-plated wingtips"); break;
      case 10: strcpy(Str4,"heavy workboots"); break;
      case 11: strcpy(Str4,"polyurethane-soled sneakers"); break;
      case 12: strcpy(Str4,"clodhoppers"); break;
      case 13: strcpy(Str4,"wooden shoes");  break;
      case 14: strcpy(Str4,"ski boots"); break;
      case 15: strcpy(Str4,"hob-nailed boots"); break;
      case 16: strcpy(Str4,"elven boots"); break;
      case 17: strcpy(Str4,"cowboy boots"); break;
      case 18: strcpy(Str4,"flipflop slippers"); break;
      case 19: strcpy(Str4,"riding boots"); break;
    }      
    return(Str4);
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

    
int twohandedp(id)
int id;
{
  switch(id) {
  case WEAPONID+5:
  case WEAPONID+12:
  case WEAPONID+18:
  case WEAPONID+20:
  case WEAPONID+26:
  case WEAPONID+27:
  case WEAPONID+32:
  case WEAPONID+36:
  case WEAPONID+38:
  case WEAPONID+39:
    return(TRUE); break;
  default: return(FALSE); break;
  }
}


void item_use(o)
struct object *o;
{
  clearmsg();
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
    case I_DEFLECT: i_deflect(o); break;

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
    case I_FEAR:i_fear(o); break;

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
    case I_SCEPTRE:i_sceptre(o);break;
    case I_PLANES:i_planes(o);break;
    case I_STARGEM:i_stargem(o);break;
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
    case I_PERM_BURDEN: i_perm_burden(o); break;
    case I_PERM_STRENGTH: i_perm_strength(o); break;
    case I_PERM_GAZE_IMMUNE: i_perm_gaze_immune(o); break;
    case I_PERM_FIRE_RESIST: i_perm_fire_resist(o); break;
    case I_PERM_POISON_RESIST: i_perm_poison_resist(o); break;
    case I_PERM_REGENERATE: i_perm_regenerate(o); break;
    case I_PERM_KNOWLEDGE: i_perm_knowledge(o); break;

    /* armor functions */
    case I_NORMAL_ARMOR: i_normal_armor(o); break;
    case I_PERM_FEAR_RESIST: i_perm_fear_resist(o); break;
    case I_PERM_ENERGY_RESIST: i_perm_energy_resist(o); break;
    case I_PERM_BREATHING: i_perm_breathing(o); break;

    /* weapons functions */
    case I_NORMAL_WEAPON: i_normal_weapon(o); break;
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
    case I_RAISE_PORTCULLIS:i_raise_portcullis(o); break;

    /* shield functions */
    case I_NORMAL_SHIELD: i_normal_shield(o); break;
    case I_PERM_DEFLECT: i_perm_deflect(o); break;
  }
}

