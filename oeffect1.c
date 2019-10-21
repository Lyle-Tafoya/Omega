/* omega copyright (C) by Laurence Raphael Brothers, 1987 */
/* oeffect1.c */

#include <strings.h>
#include "oglob.h"

/* from omove.c */
extern void l_trap_disintegrate();

/* from oaux */
extern void setspot(),roomcheck(),tunnelcheck();
extern void calc_melee(),p_death();
extern void p_damage(),damage_item();
extern int p_immune();
extern char *spellid();

/* from oinv */
extern void drop_at(),pickup_at(),add_item_to_pack(),conform_lost_objects();
extern void conform_unused_object();
extern int getitem();


/* from oitem */
extern int i_nothing();
extern void make_potion(), make_scroll(), make_ring(), make_stick();
extern void make_armor(), make_shield(), make_weapon(), make_boots();
extern void make_cloak(),make_food(),make_artifact(),make_thing();
extern void item_use();


/* from omon */
extern void m_damage(),m_status_set(),m_status_reset(),m_death();
extern struct monster *make_creature(),*m_create();
extern int m_immunityp();

/* from outil */
extern int random_range(),inbounds(),hitp(),cursed();
extern void do_los(),findspace();


/* from oscr */
extern void mprint(),printm(),morewait(),drawvision();
extern void draw_explosion(),erase_level(),plotmon(),menuprintitem();
extern void levelrefresh(),menuclear(),screencheck();
extern void show_screen(),putspot(),menuprint(),plotspot();
extern void clearmsg(),xredraw(),drawspot(),showflags();
extern char mgetc(),*msgscanstring();



/* oeffect functions */
void bless(),identify(),bolt(),ball(),megawish(),amnesia();
void heal(),mondet(),objdet(),enchant(),wish(),manastorm();
void acquire(),cleanse(),annihilate(),sleep_monster(),apport();
void sleep_player(),summon(),hide(),clairvoyance(),aggravate(),p_poison();
void fbolt(),lbolt(),lball(),fball(),nbolt(),learnspell(),level_drain();
void disintegrate(),disrupt(),snowball(),p_teleport();

int random_item(),itemlist(),monsterlist();

/* enchant */
void enchant(delta)
int delta;
{
  int i,used = FALSE;
  float mult;

  if (delta < 0) {
    i = random_item();
    if (i == ABORT) mprint("You feel fortunate.");
    else {
      if (Player.possessions[i]->blessing < 0)
	mprint("The item glows, but the glow flickers out...");
      else {
	used = (Player.possessions[i]->used);
	if (used) {
	  Player.possessions[i]->used = FALSE;
	  item_use(Player.possessions[i]);
	} 
	mprint("An aura of mundanity surrounds you!");
	Player.possessions[i]->plus = 0;
	Player.possessions[i]->charge = -1;
	Player.possessions[i]->usef = I_NOTHING;
	if (used) {
	  Player.possessions[i]->used = TRUE;
	  item_use(Player.possessions[i]);      
	}
      }
    }
  }
  else {
    i = getitem(NULL);
    if (i == ABORT) mprint("You feel unlucky.");
    else if (i == CASHVALUE) {
      mprint("You enchant your money.... What a concept!");
      mult = 1 + (random_range(7)-3)/6.0;
      if (mult > 1.0) mprint("Seems to have been a good idea!");
      else mprint("Maybe it wasn't such a good idea....");
      Player.cash = ((int) (mult*Player.cash));
    }
    else {
      i -= 'a';
      if (Player.possessions[i]->plus > random_range(20)+1) {
	mprint("Uh-oh, the force of the enchantment was too much!");
	mprint("There is a loud explosion!");
	manastorm(Player.x,Player.y,Player.possessions[i]->plus*5);
	conform_lost_objects(1,Player.possessions[i]);
      }
      else {
	used = (Player.possessions[i]->used);
	if (used) {
	  SuppressPrinting = TRUE;
	  Player.possessions[i]->used = FALSE;
	  item_use(Player.possessions[i]);
	  SuppressPrinting = FALSE;
	}
	mprint("The item shines!");
	Player.possessions[i]->plus += delta+1;
	if (Player.possessions[i]->charge > -1)
	  Player.possessions[i]->charge += 
	    ((delta+1) * (random_range(10) + 1));
	if (used) {
	  SuppressPrinting = TRUE;
	  Player.possessions[i]->used = TRUE;
	  item_use(Player.possessions[i]);      
	  SuppressPrinting = FALSE;
	}
      }
    }
    calc_melee();
  }
}

/* bless */
void bless(blessing)
int blessing;
{
  int index,used;

  if (blessing < 0) {
    index = random_item();
    if (index == ABORT) mprint("You feel fortunate.");
    else {
      mprint("A foul odor arises from your pack... ");
      used = (Player.possessions[index]->used);
      if (used) {
	SuppressPrinting = TRUE;
	Player.possessions[index]->used = FALSE;
	item_use(Player.possessions[index]);
	SuppressPrinting = FALSE;
      }
      Player.possessions[index]->blessing -= 2;
      if (Player.possessions[index]->blessing < 0) 
	Player.possessions[index]->plus =
	  abs(Player.possessions[index]->plus) - 1;
      if (used) {
	SuppressPrinting = TRUE;
	Player.possessions[index]->used = TRUE;
	item_use(Player.possessions[index]);      
	SuppressPrinting = FALSE;
      }
    }
  }
  else {
    index = getitem(NULL);
    if (index == CASHVALUE) mprint("Blessing your money has no effect.");
    else if (index != ABORT) {
      index -= 'a';
      used = (Player.possessions[index]->used == TRUE);
      if (used) {
	SuppressPrinting = TRUE;
	Player.possessions[index]->used = TRUE;
	item_use(Player.possessions[index]);      
	SuppressPrinting = FALSE;
      }
      mprint("A pure white light surrounds the item... ");
      if (Player.possessions[index]->blessing < 0-blessing)
	mprint("which is evil enough to resist the effect of the blessing!");
      else if (Player.possessions[index]->blessing < -1) {
	mprint("which disintegrates under the influence of the holy aura!");
	Player.itemweight -=  Player.possessions[index]->weight;
	conform_lost_object(Player.possessions[index]);
      }
      else {
	mprint("which now seems affected by afflatus!");
	Player.possessions[index]->blessing++;
	Player.possessions[index]->plus =
	  abs(Player.possessions[index]->plus)+1;
      }
      if (used && (Player.possessions[index] != NULL)) {
	SuppressPrinting = TRUE;
	Player.possessions[index]->used = TRUE;
	item_use(Player.possessions[index]);     
	SuppressPrinting = FALSE;
      } 
    }
  }
  calc_melee();
}



void heal(amount)
int amount;
{
  if (amount > -1) {
    mprint("You feel better.");
    Player.hp += random_range(10*amount)+1;
    Player.status[BLINDED] = 0;
    if (Player.hp > Player.maxhp)
      Player.hp = Player.maxhp + amount;
  }
  else {
    mprint("You feel unwell.");
    Player.hp -= random_range(10*abs(amount)+1);
    if (Player.hp < 0)
      p_death("magical disruption");
  }
}


void fbolt(fx,fy,tx,ty,hit,dmg)
int fx,fy,tx,ty,hit,dmg;
{
  bolt(fx,fy,tx,ty,hit,dmg,FLAME);
}

void lbolt(fx,fy,tx,ty,hit,dmg)
int fx,fy,tx,ty,hit,dmg;
{
  bolt(fx,fy,tx,ty,hit,dmg,ELECTRICITY);
}

void nbolt(fx,fy,tx,ty,hit,dmg)
int fx,fy,tx,ty,hit,dmg;
{
  bolt(fx,fy,tx,ty,hit,dmg,NORMAL_DAMAGE);
}


/* from f to t */
void bolt(fx,fy,tx,ty,hit,dmg,dtype)
int fx,fy,tx,ty,hit,dmg,dtype;
{
  int xx,yy;
  struct monster *target;
  char boltchar;
  xx = fx; yy = fy;
  
  switch(dtype) {
    case FLAME:boltchar='*';break;
    case ELECTRICITY:boltchar = '^';break;
    case NORMAL_DAMAGE:boltchar = '!';break;
  }

  do_los(boltchar,&xx,&yy,tx,ty);

  if ((xx == Player.x) && (yy == Player.y)) {
    switch (dtype) {
      case FLAME:mprint("You were blasted by a firebolt!");break;
      case ELECTRICITY:mprint("You were zapped by lightning!");break;
      case NORMAL_DAMAGE:mprint("You were hit by a missile!"); break;
    }
    p_damage(random_range(dmg),dtype,"magic");
  }
  else if (NULL != (target = Dungeon[Dlevel][xx][yy].creature)) {
    if (hitp(hit,target->ac)) {
      if (target->uniqueness == COMMON) {
	strcpy(Str1,"The ");
	strcat(Str1,target->monstring);
      }
      else strcpy(Str1,target->monstring);
      switch (dtype) {
      case FLAME:strcat(Str1," was blasted by a firebolt!");break;
      case ELECTRICITY:strcat(Str1," was zapped by lightning!");break;
      case NORMAL_DAMAGE:strcat(Str1," was hit by a missile!"); break;
      }
      mprint(Str1);
      m_status_set(target,HOSTILE);
      m_damage(target,random_range(dmg),dtype);
    }
    else {
      if (target->uniqueness == COMMON) {
	strcpy(Str1,"The ");
	strcat(Str1,target->monstring);
      }
      else strcpy(Str1,target->monstring);
      switch (dtype) {
      case FLAME:strcat(Str1," was missed by a firebolt!");break;
      case ELECTRICITY:strcat(Str1," was missed by lightning!");break;
      case NORMAL_DAMAGE:strcat(Str1," was missed by a missile!"); break;
      }
      mprint(Str1);
    }
  }
  else if (Dungeon[Dlevel][xx][yy].locchar == HEDGE)
    if (Dungeon[Dlevel][xx][yy].p_locf == L_HEDGE) {
      if ((dtype == FLAME)||(dtype == ELECTRICITY)) {
	mprint("The hedge is blasted away!");
	Dungeon[Dlevel][xx][yy].p_locf = L_NO_OP;
	Dungeon[Dlevel][xx][yy].locchar = FLOOR;
      }
      else mprint("The hedge is unaffected.");
    }
    else mprint("The trifid absorbs the energy and laughs!");
  else if (Dungeon[Dlevel][xx][yy].locchar == WATER)
    if (dtype == FLAME) {
      mprint("The water is vaporised!");
      Dungeon[Dlevel][xx][yy].p_locf = L_NO_OP;
      Dungeon[Dlevel][xx][yy].locchar = FLOOR;
    }
}


void lball(fx,fy,tx,ty,dmg)
int fx,fy,tx,ty,dmg;
{
  ball(fx,fy,tx,ty,dmg,ELECTRICITY);
}

void manastorm(x,y,dmg)
int x,y,dmg;
{
  ball(x,y,x,y,dmg,UNSTOPPABLE);
}

void snowball(fx,fy,tx,ty,dmg)
int fx,fy,tx,ty,dmg;
{
  ball(fx,fy,tx,ty,dmg,COLD);
}

void fball(fx,fy,tx,ty,dmg)
int fx,fy,tx,ty,dmg;
{
  ball(fx,fy,tx,ty,dmg,FLAME);
}


/* from f to t */
void ball(fx,fy,tx,ty,dmg,dtype)
int fx,fy,tx,ty,dmg,dtype;
{
  int xx,yy,ex,ey,i;
  struct monster *target;
  char expchar='@';

  xx = fx; yy = fy;

  switch(dtype){
    case FLAME:expchar='*';break;
    case COLD:expchar='o';break;
    case ELECTRICITY:expchar='^';break;
  }

  do_los(expchar,&xx,&yy,tx,ty);
  draw_explosion(expchar,xx,yy);
  for(i=0;i<9;i++) {
    ex = xx + Dirs[0][i];
    ey = yy + Dirs[1][i];
    
    if ((ex == Player.x) && (ey == Player.y)) {
      switch(dtype) {
	case FLAME:mprint("You were blasted by a fireball!");
	  p_damage(random_range(dmg),FLAME,"a fireball");
	  break;
	case COLD:mprint("You were blasted by a snowball!");
	  p_damage(random_range(dmg),COLD,"a snowball");
	  break;
	case ELECTRICITY:mprint("You were blasted by ball lightning!");
	  p_damage(random_range(dmg),ELECTRICITY,"ball lightning");
	  break;
	case UNSTOPPABLE:mprint("Oh No! Manastorm!");
	  p_damage(random_range(dmg),UNSTOPPABLE,"a manastorm!");
	  break;
      }
    }
    else if (NULL != (target = Dungeon[Dlevel][ex][ey].creature)) {
      if (los_p(Player.x,Player.y,target->x,target->y)) {
      if (target->uniqueness == COMMON) {
	strcpy(Str1,"The ");
	strcat(Str1,target->monstring);
      }
      else strcpy(Str1,target->monstring);
	switch(dtype) {
	  case FLAME:strcat(Str1," was zorched by a fireball!"); break;
	  case COLD:strcat(Str1," was blasted by a snowball!"); break;
	  case ELECTRICITY:strcat(Str1," was zapped by ball lightning!");break;
	  case UNSTOPPABLE:strcat(Str1," was nuked by a manastorm!");break;
	}
	mprint(Str1);
      }
      m_status_set(target,HOSTILE);
      m_damage(target,random_range(dmg),dtype);
    }
    else if (Dungeon[Dlevel][ex][ey].locchar == HEDGE)
      if (Dungeon[Dlevel][ex][ey].p_locf == L_HEDGE) {
	if ((dtype == FLAME)||(dtype == ELECTRICITY)) {
	  mprint("The hedge is blasted away!");
	  Dungeon[Dlevel][ex][ey].p_locf = L_NO_OP;
	  Dungeon[Dlevel][ex][ey].locchar = FLOOR;
	}
	else mprint("The hedge is unaffected.");
      }
      else mprint("The trifid absorbs the energy and laughs!");
    else if (Dungeon[Dlevel][xx][yy].locchar == WATER)
      if (dtype == FLAME) {
	mprint("The water is vaporised!");
	Dungeon[Dlevel][xx][yy].p_locf = L_NO_OP;
	Dungeon[Dlevel][xx][yy].locchar = FLOOR;
      }
  }
}




void mondet(blessing)
int blessing;    
{
  pml ml;
  if (Dlevel == NUMLEVELS-1)
    mprint("That's funny, no effect!");
  else {
    for (ml=Mlist[Dlevel];ml!=NULL;ml=ml->next) 
      plotmon(blessing > -1 ? ml->m : &(Monsters[random_range(NUMMONSTERS)]));
    levelrefresh();
    if (blessing < 1) {
      morewait();
      erase_level();
      show_screen(WhichScreen);
    }
  }
} 

void objdet(blessing)
int blessing;     
{
  int i,j;
 if (Dlevel == NUMLEVELS-1)
    mprint("That's funny, no effect!");
  else {
    for (i=0;i<WIDTH;i++)
      for (j=0;j<LENGTH;j++) 
	if (Dungeon[Dlevel][i][j].things != NULL) {
	  if (blessing < 0)
	    putspot(random_range(WIDTH),
		    random_range(LENGTH),
		    Dungeon[Dlevel][i][j].things->thing->objchar);
	  else putspot(i,j,Dungeon[Dlevel][i][j].things->thing->objchar);
	}
    levelrefresh();
    if (blessing < 1) {
      morewait();
      erase_level();
      show_screen(WhichScreen);
    }
  } 
}

void identify(blessing)
{
  int index;
  int ok = FALSE;;

  if (blessing == 0) {
    printm("\nIdentify:");
    index = getitem(NULL);
    if (index == CASHVALUE) mprint("Your money is really money.");
    if (index == ABORT)
      Skipmonsters = TRUE;
    else {
      index -= 'a';
      if (Player.possessions[index]->objchar == FOOD)
	Player.possessions[index]->known = 1;
      else {
	Player.possessions[index]->known = 2;
	Objects[Player.possessions[index]->id].known = 1;
      }
      printm("\nIdentified: ");
      mprint(itemid(Player.possessions[index]));
    }
  }
  else if (blessing < 0) {
    mprint("You feel forgetful.");
    for (index=0;index<MAXITEMS;index++) 
      if (Player.possessions[index] != NULL) {
	Player.possessions[index]->known = 0;
	Objects[Player.possessions[index]->id].known = 0;
      }
  }
  else {
    mprint("You feel encyclopaedic.");
    for (index=0;index<MAXITEMS;index++)
      if (Player.possessions[index] != NULL) {
	if (Player.possessions[index]->objchar == FOOD)
	  Player.possessions[index]->known = 1;
	else {
	  Player.possessions[index]->known = 2;
	  Objects[Player.possessions[index]->id].known = 1;
	}
      }
  }
  calc_melee();
}




/* returns index of random item, ABORT if player carrying none */
int random_item()
{
  int item,tries=0;
  int done = FALSE;

  while (! done) {
    item = random_range(MAXITEMS);
    done = ((tries == MAXITEMS) || (Player.possessions[item] != NULL));
  }
  return(done ? item : ABORT);
}


/* various kinds of wishes */
void wish(blessing)
int blessing;
{
  char wishtype;
  
  if (blessing == 0) {
    wishtype = 'a'+random_range(5);
    switch(wishtype) {
      case 'a': mprint("A wish for acquiring."); break;
      case 'b': mprint("A wish for cleansing."); break;
      case 'c': mprint("A wish for healing."); break;
      case 'd': mprint("A wish for annihilation."); break;
      case 'e': mprint("A wish for summoning."); break;
    }
  }
  else {
    mprint("Wish for: ");
    menuclear();
    menuprint("a: acquiring\n");
    menuprint("b: cleansing\n");
    menuprint("c: healing\n");
    menuprint("d: annihilation\n");
    menuprint("e: summoning\n");
    wishtype = mgetc();
    clearmsg();
  }
  
  if (blessing == 0) blessing = 1;
  switch(wishtype) {
    case 'a': acquire(blessing); break;
    case 'b': cleanse(blessing); break;
    case 'c': heal(10*blessing); break;
    case 'd': annihilate(blessing); break;
    case 'e': summon(blessing,-1); break;
    case 'f': megawish(); break;
    default:mprint("You feel stupid."); break;
  }
  xredraw();
}

/* gain for an item */
void acquire(blessing)
int blessing;
{
  char otype;
  int index,id = ABORT;
  pob newthing;

  if (blessing < 0) {
    index = random_item();
    if (index == ABORT)
      mprint("You feel fortunate.");
    else {
      mprint("Smoke drifts out of your pack.... ");
      mprint("Destroyed... ");
      mprint(itemid(Player.possessions[index]));
      conform_lost_object(Player.possessions[index]);
    }
  }
  else if (blessing == 0) {
    newthing = ((pob) calloc(1,sizeof(objtype)));
    *newthing = Objects[random_range(TOTALITEMS)];
    newthing->used = FALSE;
    mprint("An item appears in your pack.");
    add_item_to_pack(newthing);
  }
  else {
    newthing = ((pob) calloc(1,sizeof(objtype)));
    newthing->id = -1;
    
    mprint("Acquire which kind of item: !?][}{)/=\%%\\");
    otype = mgetc();
    mprint("Item ID?");
    switch (otype) {
    case POTION: 
      id = itemlist(POTIONID,NUMPOTIONS);
      if (id < 0) mprint("You feel stupid."); 
      else make_potion(newthing,id); 
      break;
    case SCROLL:
      id = itemlist(SCROLLID,NUMSCROLLS);
      if (id < 0) mprint("You feel stupid."); 
      else make_scroll(newthing,id); 
      break;
    case RING: 
      id = itemlist(RINGID,NUMRINGS);
      if (id < 0) mprint("You feel stupid."); 
      else make_ring(newthing,id); 
      break;
    case STICK:
      id = itemlist(STICKID,NUMSTICKS);
      if (id < 0) mprint("You feel stupid."); 
      else make_stick(newthing,id); 
      break;
    case ARMOR: 
      id = itemlist(ARMORID,NUMARMOR);
      if (id < 0) mprint("You feel stupid."); 
      else make_armor(newthing,id); 
      break;
    case SHIELD:
      id = itemlist(SHIELDID,NUMSHIELDS);
      if (id < 0) mprint("You feel stupid."); 
      else make_shield(newthing,id); 
      break;
    case WEAPON:
      id = itemlist(WEAPONID,NUMWEAPONS);
      if (id < 0) mprint("You feel stupid."); 
      else make_weapon(newthing,id); 
      break;
    case BOOTS:
      id = itemlist(BOOTID,NUMBOOTS);
      if (id < 0) mprint("You feel stupid."); 
      else make_boots(newthing,id); 
      break;
    case CLOAK: 
      id = itemlist(CLOAKID,NUMCLOAKS);
      if (id < 0) mprint("You feel stupid."); 
      else make_cloak(newthing,id); 
      break;
    case FOOD: 
      id = itemlist(FOODID,NUMFOODS);
      if (id < 0) mprint("You feel stupid."); 
      else make_food(newthing,id); 
      break;
    case THING:
      id = itemlist(THINGID,10); /* crock to avoid grot*/
      if (id < 0) mprint("You feel stupid."); 
      else make_thing(newthing,id); 
      break;
    }
    if (id != ABORT) {
      mprint("An item appears in your pack.");
      newthing->known = 2;
      Objects[id].known = 1;
      add_item_to_pack(newthing);
    }
  }
}

/* if know id, then summon that monster; else (if < 0) get one. */
void summon(blessing,id)
int blessing,id;
{
  int i,looking=TRUE,x,y;
  pml tml;

  if (id < 0) {
    if (blessing > 0) {
      id = monsterlist();
      xredraw();
    }
    /* for (id ==0) case, see below -- get a "fair" monster */
    else if (blessing < 0) id = random_range(NUMMONSTERS);
  }
  for(i=0;((i<8) && looking);i++) {
    x = Player.x+Dirs[0][i];
    y = Player.y+Dirs[1][i];
    looking = ((! inbounds(x,y)) ||
	       (Dungeon[Dlevel][x][y].locchar != FLOOR) ||
	       (Dungeon[Dlevel][x][y].creature != NULL));
  }

  if (! looking) {
    if ((blessing == 0) && (id < 0))
      Dungeon[Dlevel][x][y].creature = m_create(x,y,WANDERING,Dlevel);
    else Dungeon[Dlevel][x][y].creature = make_creature(id);
    Dungeon[Dlevel][x][y].creature->x = x;
    Dungeon[Dlevel][x][y].creature->y = y;
    tml = ((pml) (calloc(1,sizeof(mltype))));
    tml->m = Dungeon[Dlevel][x][y].creature;
    if (blessing > 0)
      m_status_reset(tml->m,HOSTILE);
    else if (blessing < 0)
      m_status_set(tml->m,HOSTILE);
    tml->next = Mlist[Dlevel];
    Mlist[Dlevel] = tml;
  }
}



int itemlist(itemindex,num)
int itemindex,num;
{
  int i,itemno;

  menuclear();
  for(i=0;i<num;i++)
    menuprintitem(i+1,Objects[i+itemindex].truename);
  itemno = parsenum()-1;
  if ((itemno > num)||(itemno<0)) itemno = ABORT;
  return(itemno);
}

int monsterlist()
{
  int i,itemno;
  do {
    printm("\nSummon monster: ");
    menuclear();
    for(i=0;i<NUMMONSTERS;i++)
      menuprintitem(i+1,Monsters[i].monstring);
    itemno = parsenum()-1;
    if ((itemno < 0) || (itemno > NUMMONSTERS-1)) {
      printm("\nHow about trying a real monster?");
      morewait();
    }
  } while ((itemno < 0) || (itemno > NUMMONSTERS-1));
  return(itemno);
}
      


/* uncurse all items, cure diseases, and neutralize poison */
void cleanse(blessing)
{
  int i;

  if (blessing > -1) {
    for(i=0;i<MAXITEMS;i++) 
      if (Player.possessions[i] != NULL) {
	if ((Player.possessions[i]->used) &&
	    (Player.possessions[i]->blessing < 0)) {
	  Player.possessions[i]->used = FALSE;
	  item_use(Player.possessions[i]);
	  Player.possessions[i]->blessing = 0;
	  Player.possessions[i]->used = TRUE;
	  item_use(Player.possessions[i]);
	}
      }
    
    if (Player.status[POISONED] > 0) {
      Player.status[POISONED] = 0;
    }
    if (Player.status[DISEASED] > 0) {
      Player.status[DISEASED] = 0;
    }
    showflags();
    mprint("You feel radiant!");
  }
  else {
    Player.status[POISONED] += 10;
    Player.status[DISEASED] += 10;
    mprint("You feel besmirched!");
    showflags();
  }
}

void annihilate(blessing)
int blessing;
{
  pml ml;
  mprint("Lightning strikes flash all around you!!!");
  if (blessing > -1) {
    for(ml=Mlist[Dlevel];ml!=NULL;ml=ml->next)
      if (ml->m != NULL)
      m_death(ml->m);
    Mlist[Dlevel] = NULL;
  }
  else {
    mprint("You are hit by a bolt of mystic lightning!");
    p_death("self-annihilation");
  }
}

  
void megawish()
{
  char wishstr[256];
  mprint("What do you wish for? ");
  strcpy(wishstr,msgscanstring());
  if (strcmp(wishstr,"Power")==0) {
    mprint("You feel a sudden surge of energy");
    Player.mana+=1000;
  }
  else if (strcmp(wishstr,"Wealth")==0) {
    mprint("You are submerged in shower of gold pieces!");
    Player.cash += 10000;
  }
  else if (strcmp(wishstr,"Skill")==0) {
    mprint("You feel enlightened!");
    gain_experience(10000);
  }
  else if (strcmp(wishstr,"Balance")==0) {
    mprint("You feel neutral.");
    Player.alignment = 0;
  }
  else mprint("You feel stupid.");
}





void sleep_monster(blessing)
int blessing;
{
  pml ml;
  int x=Player.x,y=Player.y;
  struct monster *target;

  if (blessing == 0) setspot(&x,&y);

  if (blessing < 0)
    sleep_player(abs(blessing)+2);
  else if (blessing > 0) {
    mprint("A silence pervades the area.");
    for (ml=Mlist[Dlevel];ml!=NULL;ml=ml->next) {
      m_status_reset(ml->m,AWAKE);
      ml->m->wakeup = 0;
    }
  }
  else {
    target = Dungeon[Dlevel][x][y].creature;
    if (target != NULL) {
      if (target->uniqueness == COMMON) {
	strcpy(Str1,"The ");
	strcat(Str1,target->monstring);
      }
      else strcpy(Str1,target->monstring);
      if (! m_immunityp(target,SLEEP)) {
	strcat(Str1," seems to have fallen asleep.");
	m_status_reset(target,AWAKE);
	target->wakeup = 0;
      }
      else strcat(Str1," is bright eyed, and bushy tailed!");
      mprint(Str1);
    }
    else mprint("Nothing to sleep there!");
  }
}
  
void sleep_player(amount)
int amount;
{
  if (Player.status[SLEPT] == 0) {
    mprint("You feel sleepy...");
    if (! p_immune(SLEEP)) {
      Player.status[SLEPT] += amount*(random_range(10)+3);
    }
    else mprint("but you shrug off the momentary lassitude.");
  }
}


void hide(x,y)
int x,y;
{
  if (inbounds(x,y)) {
    Dungeon[Dlevel][x][y].secret = TRUE;
    mprint("You feel knowledgeable.");
  }
}

void clairvoyance(vision)
int vision;
{
  int i,j;
  int x = Player.x, y = Player.y;
  if (Dlevel == NUMLEVELS-1)
    mprint("That's funny, no effect!");
  else {
    mprint("Clairvoyance... ");
    setspot(&x,&y);
    for(i=x-vision;i<x+vision+1;i++)
      for(j=y-vision;j<y+vision+1;j++) {
	if (inbounds(i,j)) {
	  Dungeon[Dlevel][i][j].secret = FALSE;
	  Dungeon[Dlevel][i][j].seen = TRUE;
	  plotspot(i,j,FALSE);
	}
      }
    levelrefresh();
  }
}

void aggravate()
{
  pml tm;

  for (tm=Mlist[Dlevel];tm!=NULL;tm=tm->next){
    m_status_set(tm->m,AWAKE);
    m_status_set(tm->m,HOSTILE);
  }
}




void learnspell(blessing)
int blessing;
{
  int i,spell,done=FALSE;
  char c;
  if (blessing < 0) {
    for(i=NUMSPELLS;((i>-1) && (! done));i--)
      if (Spells[i].known) {
	done = TRUE;
	Objects[SCROLLID+1].known = TRUE;
	mprint("You feel forgetful.");
	Spells[i].known = FALSE;
      }
    if (i == ABORT)
      mprint("You feel fortunate.");
  }
  else {
    Objects[SCROLLID+1].known = TRUE;
    spell = random_range(NUMSPELLS);
    printm("\nSpell Research");
    if ((random_range(50)+Spells[spell].powerdrain) <
	(4*(Player.iq+Player.level))) {
      mprint(" -- Research successful!");
      mprint(spellid(spell));
      if (Spells[spell].known) {
	mprint("is now easier to cast.");
	Spells[spell].powerdrain = ((int) ((Spells[spell].powerdrain+1)/2));
      }
      else {
	mprint("You discovered the spell:");
	mprint(spellid(spell));
	Spells[spell].known = TRUE;
	gain_experience(Spells[spell].powerdrain*Spells[spell].powerdrain);
      }
    }
    else mprint(" -- Research unsuccessful.");
  }
}


void amnesia()
{
  int i,j;
  for (j=0;j<LENGTH;j++)
    for (i=0;i<WIDTH;i++)
      Dungeon[Dlevel][i][j].seen=FALSE;

  erase_level();
  drawvision(Player.x,Player.y,Player.vision);
}


void level_drain(levels,source)
int levels;
char *source;
{
  int decrement = ((int) (Player.maxhp / (Player.level+1)));

  Player.level -= levels;

  Player.maxhp -= (levels * decrement);
  Player.hp -= (levels * decrement);

  if ((Player.hp < 1) || (Player.level < 0))
    p_death(source);
}




void disrupt(x,y,amount)
int x,y,amount;
{
  pml ml;
  struct monster *target;

  if ((x ==Player.x) && (y==Player.y)) {
    mprint("You feel disrupted!");
    p_damage(amount,NORMAL_DAMAGE,"magical disruption");
  }
  else {
    target = Dungeon[Dlevel][x][y].creature;
    if (target != NULL) {
      if (target->uniqueness == COMMON) {
	strcpy(Str1,"The ");
	strcat(Str1,target->monstring);
      }
      else strcpy(Str1,target->monstring);
      if (! m_immunityp(target,NORMAL_DAMAGE)) {
	strcat(Str1," was blasted!");
	mprint(Str1);
	m_damage(target,amount,NORMAL_DAMAGE);
	target->wakeup = 0;
      }
      else {
	strcat(Str1," does not seem affected.");
	mprint(Str1);
      }
    }
  }
}




void disintegrate(x,y)
int x,y;
{
  struct monster *target;
  if (! inbounds(x,y)) mprint("You feel a sense of wastage.");
  else if ((x==Player.x)&&(y==Player.y)) {
    if (Player.cloak != NULL) {
      mprint("Your cloak disintegrates!");
      conform_lost_objects(1,Player.cloak);
    }
    else if (Player.armor != NULL) {
      mprint("Your armor disintegrates!");
      conform_lost_objects(1,Player.armor);
    }
    else {
      mprint("Uh, oh....");
      mprint("Zzzap! You've been disintegrated!");
      p_damage(250,UNSTOPPABLE,"disintegration");
    }
  }
  else if ((target = Dungeon[Dlevel][x][y].creature) != NULL) {
    if (target->uniqueness == COMMON) {
      strcpy(Str1,"The ");
      strcat(Str1,target->monstring);
    }
    else strcpy(Str1,target->monstring);
    strcat(Str1," disintegrates!");
    mprint(Str1);
    m_damage(target,100,UNSTOPPABLE);
    if (target->hp > 0) mprint("It was partially protected by its armor.");
  }
  else if (Dlevel == NUMLEVELS-1)
    mprint("Your blast seems to vanish just before it strikes!");
  else if (Dungeon[Dlevel][x][y].locchar == ALTAR) {
    mprint("Zzzzap! the altar seems unaffected...");
    mprint("But an angry deity retaliates....");
    disintegrate(Player.x,Player.y);
  }
  else if (Dungeon[Dlevel][x][y].p_locf == L_TRAP_PIT) {
    mprint("A hole is blasted in the base of the pit!");
    Dungeon[Dlevel][x][y].locchar = '>';
    Dungeon[Dlevel][x][y].aux = Dlevel+1;
    Dungeon[Dlevel][x][y].stopsrun = TRUE;
    Dungeon[Dlevel][x][y].p_locf = L_NO_OP;
  }
  else if (Dungeon[Dlevel][x][y].locchar == FLOOR) {
    mprint("You zap a hole in the floor!");
    Dungeon[Dlevel][x][y].locchar = '^';
    Dungeon[Dlevel][x][y].p_locf = L_TRAP_PIT;
  }
  else {
    mprint("A new floorspace is blasted!");
    if (Dungeon[Dlevel][x][y].locchar == WALL)
      tunnelcheck();
    Dungeon[Dlevel][x][y].p_locf = L_NO_OP;
    Dungeon[Dlevel][x][y].locchar = FLOOR;
  }
  if (Dungeon[Dlevel][x][y].things != NULL) 
    Dungeon[Dlevel][x][y].things = NULL;
  /* blow up all the items there too */
}


void acid_cloud()
{
  mprint("You are caught in an acid cloud!");
  if (Player.cloak != NULL) {
    damage_item(Player.cloak);
    mprint("You are burned by acid.");
    p_damage(3,ACID,"an acid cloud");
  }
  else if (Player.armor != NULL) {
    mprint("You are burned by acid.");
    p_damage(3,ACID,"an acid cloud");
    damage_item(Player.armor);
  }
  else {
    mprint("The acid eats away at your bare skin!");
    p_damage(25,ACID,"an acid cloud");
  }
}



/* teleport player */
void p_teleport(type)
int type;
{
  int x=Player.x,y=Player.y;
  drawspot(x,y);
  if (Dlevel == NUMLEVELS-1) 
    mprint("Some force seems to block the operation of the effect!");
  else if (type < 0) {
    x = random_range(WIDTH);
    y = random_range(LENGTH);
    if (Dungeon[Dlevel][x][y].locchar != FLOOR) {
      mprint("You teleported into a stone wall.");
      mprint("You are dead!");
      p_death("teleportation into a solid object");
    }
    else {
      Player.x = x;
      Player.y = y;
    }
  }
  else if (type == 0)
    findspace(&(Player.x),&(Player.y));
  else {
    setspot(&Player.x,&Player.y);
    if ((Dungeon[Dlevel][Player.x][Player.y].locchar != FLOOR) ||
	(Dungeon[Dlevel][Player.x][Player.y].creature != NULL)) {
      mprint("You feel deflected.");
      p_teleport(0);
    }
  }
  screencheck(Player.y);
  roomcheck();
}


void p_poison(toxicity)
int toxicity;
{
  mprint("You feel sick.");
  if (! p_immune(POISON))
    Player.status[POISONED]+=toxicity;
  else mprint("The sickness fades!");
  showflags();
}

void apport(blessing)
int blessing;
{
  int i,index,x=Player.x,y=Player.y;
  if (blessing > -1) {
    mprint("Apport from:");
    setspot(&x,&y);
    if (Dungeon[Dlevel][x][y].things != NULL)
      pickup_at(x,y);
    else mprint("There's nothing there to apport!");
  }
  else {
    mprint("You have a sense of loss.");
    for(i=0;i<abs(blessing);i++) {
      index = random_item();
      if (index != ABORT) {
	conform_unused_object(Player.possessions[index]);
	drop_at(x,y,Player.possessions[index]);
      }
    }
  }
}
