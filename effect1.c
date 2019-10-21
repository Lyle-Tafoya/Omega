/* omega copyright (C) by Laurence Raphael Brothers, 1987,1988,1989 */
/* effect1.c */

#include "glob.h"


/* enchant */
void enchant(delta)
int delta;
{
  int i,used = FALSE;
  long change_cash;

  if (delta < 0) {
    i = random_item();
    if (i == ABORT || Player.possessions[i]->usef == I_NOTHING ||
	Player.possessions[i]->usef == I_NO_OP ||
	Player.possessions[i]->usef == I_NORMAL_ARMOR ||
	Player.possessions[i]->usef == I_NORMAL_WEAPON ||
	Player.possessions[i]->usef == I_NORMAL_SHIELD ||
	Player.possessions[i]->objchar == FOOD ||
	Player.possessions[i]->objchar == MISSILEWEAPON) {
      print1("You feel fortunate.");
      morewait();
    }
    else if (Player.possessions[i]->blessing < 0 ||
      (Player.possessions[i]->objchar == ARTIFACT && random_range(3))) {
	if (Player.possessions[i]->uniqueness == COMMON)
	  print1("Your ");
	nprint1(itemid(Player.possessions[i]));
	nprint1(" glows, but the glow flickers out...");
	morewait();
    }
    else {
      used = (Player.possessions[i]->used);
      if (used) {
	Player.possessions[i]->used = FALSE;
	item_use(Player.possessions[i]);
      } 
      if (Player.possessions[i]->uniqueness == COMMON)
	print1("Your ");
      nprint1(itemid(Player.possessions[i]));
      nprint1(" radiates an aura of mundanity!");
      morewait();
      Player.possessions[i]->plus = 0;
      Player.possessions[i]->charge = -1;
      Player.possessions[i]->usef = I_NOTHING;
      if (used) {
	Player.possessions[i]->used = TRUE;
	item_use(Player.possessions[i]);      
      }
    }
  }
  else {
    i = getitem(CASH);
    if (i == ABORT) {
      print1("You feel unlucky.");
      morewait();
    }
    else if (i == CASHVALUE) {
      print1("You enchant your money.... What a concept!");
      change_cash = Player.cash*(random_range(7) - 3)/6;
      if (change_cash > 0) print2("Seems to have been a good idea!");
      else print2("Maybe it wasn't such a good idea....");
      Player.cash += change_cash;
      morewait();
    }
    else if (Player.possessions[i]->objchar == ARTIFACT) {
      if (Player.possessions[i]->usef !=
	Objects[Player.possessions[i]->id].usef) {
	print1("It re-acquires its magical aura!");
	Player.possessions[i]->usef = Objects[Player.possessions[i]->id].usef;
      }
      else {
	print1("The enchantment spell enfolds the ");
	nprint1(itemid(Player.possessions[i]));
	print2("and the potent enchantment of the Artifact causes a backlash!");
	morewait();
	clearmsg();
	manastorm(Player.x,Player.y,Player.possessions[i]->level*5);
      }
    }
    else {
      if (Player.possessions[i]->plus > random_range(20)+1) {
	print1("Uh-oh, the force of the enchantment was too much!");
	print2("There is a loud explosion!");
	morewait();
	manastorm(Player.x,Player.y,Player.possessions[i]->plus*5);
	dispose_lost_objects(1,Player.possessions[i]);
      }
      else {
	used = (Player.possessions[i]->used);
	if (used) {
	  setgamestatus(SUPPRESS_PRINTING);
	  Player.possessions[i]->used = FALSE;
	  item_use(Player.possessions[i]);
	  resetgamestatus(SUPPRESS_PRINTING);
	}
	print1("The item shines!");
	morewait();
	Player.possessions[i]->plus += delta+1;
	if (Player.possessions[i]->charge > -1)
	  Player.possessions[i]->charge += 
	    ((delta+1) * (random_range(10) + 1));
	if (used) {
	  setgamestatus(SUPPRESS_PRINTING);
	  Player.possessions[i]->used = TRUE;
	  item_use(Player.possessions[i]);      
	  resetgamestatus(SUPPRESS_PRINTING);
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
    if (index == ABORT) {
      print1("You feel fortunate.");
      morewait();
    }
    else {
      print1("A foul odor arises from ");
      if (Player.possessions[index]->uniqueness == COMMON)
	nprint1("your ");
      nprint1(itemid(Player.possessions[index]));
      morewait();
      used = (Player.possessions[index]->used);
      if (used) {
	setgamestatus(SUPPRESS_PRINTING);
	Player.possessions[index]->used = FALSE;
	item_use(Player.possessions[index]);
	resetgamestatus(SUPPRESS_PRINTING);
      }
      Player.possessions[index]->blessing -= 2;
      if (Player.possessions[index]->blessing < 0) 
	Player.possessions[index]->plus =
	  abs(Player.possessions[index]->plus) - 1;
      if (used) {
	setgamestatus(SUPPRESS_PRINTING);
	Player.possessions[index]->used = TRUE;
	item_use(Player.possessions[index]);      
	resetgamestatus(SUPPRESS_PRINTING);
      }
    }
  }
  else {
    index = getitem(NULL_ITEM);
    if (index == CASHVALUE) {
      print1("Blessing your money has no effect.");
      morewait();
    }
    else if (index != ABORT) {
      used = (Player.possessions[index]->used == TRUE);
      if (used) {
	setgamestatus(SUPPRESS_PRINTING);
	Player.possessions[index]->used = FALSE;
	item_use(Player.possessions[index]);      
	resetgamestatus(SUPPRESS_PRINTING);
      }
      print1("A pure white light surrounds the item... ");
      if (Player.possessions[index]->blessing < 0-(blessing+1)) {
	print2("which is evil enough to resist the effect of the blessing!");
        morewait();
      }
      else if (Player.possessions[index]->blessing < -1) {
	print2("which disintegrates under the influence of the holy aura!");
        morewait();
	Player.itemweight -=  Player.possessions[index]->weight;
	dispose_lost_objects(1,Player.possessions[index]);
      }
      else if (Player.possessions[index]->blessing < blessing+1) {
	print2("which now seems affected by afflatus!");
	morewait();
	Player.possessions[index]->blessing++;
	Player.possessions[index]->plus =
	  abs(Player.possessions[index]->plus)+1;
      }
      else {
        print2("The hierolux fades without any appreciable effect....");
        morewait();
      }
      if (used && (Player.possessions[index] != NULL)) {
	setgamestatus(SUPPRESS_PRINTING);
	Player.possessions[index]->used = TRUE;
	item_use(Player.possessions[index]);     
	resetgamestatus(SUPPRESS_PRINTING);
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
    if (Player.hp < Player.maxhp + amount)
    {
      Player.hp += random_range(10*amount)+1;
      if (Player.hp > Player.maxhp)
        Player.hp = Player.maxhp + amount;
    }
    Player.status[BLINDED] = 0;
  }
  else {
    mprint("You feel unwell.");
    Player.hp -= random_range(10*abs(amount)+1);
    if (Player.hp < 0)
      p_death("magical disruption");
  }
  dataprint();
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

/* Added 12/30/98 DG */
void icebolt(fx,fy,tx,ty,hit,dmg)
int fx,fy,tx,ty,hit,dmg;
{
  bolt(fx,fy,tx,ty,hit,dmg,COLD);
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
  Symbol boltchar;
  xx = fx; yy = fy;
  
  switch(dtype) {
    case FLAME:boltchar=('*' | CLR(LIGHT_RED));break;
    case ELECTRICITY:boltchar = ('^' | CLR(LIGHT_BLUE));break;
    case NORMAL_DAMAGE:boltchar = ('!' | CLR(BROWN));break;
    case COLD:boltchar=('o' | CLR(WHITE));break;
    default:assert(FALSE); /* this should never happen, right? WDT */
  }
  clearmsg();

  do_los(boltchar,&xx,&yy,tx,ty);

  if ((xx == Player.x) && (yy == Player.y)) {
    if (Player.status[DEFLECTION] > 0) 
      mprint("The bolt just missed you!");
    else {
      switch (dtype) {
      case FLAME:
	mprint("You were blasted by a firebolt!");
	p_damage(random_range(dmg),dtype,"a firebolt");
	break;
      case ELECTRICITY:
	mprint("You were zapped by lightning!");
	p_damage(random_range(dmg),dtype,"a bolt of lightning");
	break;
      case NORMAL_DAMAGE:
	mprint("You were hit by a missile!");
	p_damage(random_range(dmg),dtype,"a missile");
	break;
      case COLD:
	mprint("You were hit by an icicle!");
	p_damage(random_range(dmg),dtype,"an icicle");
	break;
      }
    }
  }
  else if (NULL != (target = Level->site[xx][yy].creature)) {
    if (hitp(hit,target->ac)) {
      if (target->uniqueness == COMMON) {
	strcpy(Str1,"The ");
	strcat(Str1,target->monstring);
      }
      else strcpy(Str1,target->monstring);
      switch (dtype) {
        /* WDT: these sentances really ought to be livened up.  Especially
         * in full verbose mode. */
      case FLAME:strcat(Str1," was blasted by a firebolt!");break;
      case ELECTRICITY:strcat(Str1," was zapped by lightning!");break;
      case NORMAL_DAMAGE:strcat(Str1," was hit by a missile!"); break;
      case COLD:strcat(Str1," was hit by an icicle!"); break;
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
      case COLD:strcat(Str1," was missed by a flying icicle!"); break;
      }
      mprint(Str1);
    }
  }
  else if (Level->site[xx][yy].locchar == HEDGE)
    if (Level->site[xx][yy].p_locf != L_TRIFID) {
      if ((dtype == FLAME)||(dtype == ELECTRICITY)) {
	mprint("The hedge is blasted away!");
	Level->site[xx][yy].p_locf = L_NO_OP;
	Level->site[xx][yy].locchar = FLOOR;
	plotspot(xx, yy, TRUE);
	lset(xx, yy, CHANGED);
      }
      else mprint("The hedge is unaffected.");
    }
    else mprint("The trifid absorbs the energy and laughs!");
  else if (Level->site[xx][yy].locchar == WATER)
    if (dtype == FLAME) {
      mprint("The water is vaporised!");
      Level->site[xx][yy].p_locf = L_NO_OP;
      Level->site[xx][yy].locchar = FLOOR;
      lset(xx, yy, CHANGED);
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
  Symbol expchar=('@' | CLR(LIGHT_PURPLE));

  xx = fx; yy = fy;

  switch(dtype){
    case FLAME:expchar=('*' | CLR(LIGHT_RED));break;
    case COLD:expchar=('o' | CLR(WHITE));break;
    case ELECTRICITY:expchar=('^' | CLR(LIGHT_BLUE));break;
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
    if (NULL != (target = Level->site[ex][ey].creature)) {
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
    if (Level->site[ex][ey].locchar == HEDGE)
      if (Level->site[ex][ey].p_locf != L_TRIFID) {
	if ((dtype == FLAME)||(dtype == ELECTRICITY)) {
	  mprint("The hedge is blasted away!");
	  Level->site[ex][ey].p_locf = L_NO_OP;
	  Level->site[ex][ey].locchar = FLOOR;
	  plotspot(ex,ey,TRUE);
	  lset(ex, ey, CHANGED);
	}
	else mprint("The hedge is unaffected.");
      }
      else mprint("The trifid absorbs the energy and laughs!");
    else if (Level->site[ex][ey].locchar == WATER)
      if (dtype == FLAME) {
	mprint("The water is vaporised!");
	Level->site[ex][ey].p_locf = L_NO_OP;
	Level->site[ex][ey].locchar = FLOOR;
	plotspot(ex,ey,TRUE);
	lset(ex, ey, CHANGED);
      }
  }
}




void mondet(blessing)
int blessing;    
{
  pml ml;
  for (ml=Level->mlist;ml!=NULL;ml=ml->next) 
    if (ml->m->hp > 0) /* FIXED 12/30/98 DG */
      if (blessing > -1)
	plotmon(ml->m);
      else
	putspot(random_range(WIDTH), random_range(LENGTH),
	    Monsters[random_range(NUMMONSTERS)].monchar);
  levelrefresh();
  morewait();
  show_screen();
}


void objdet(blessing)
int blessing;     
{
  int i,j;
  for (i=0;i<WIDTH;i++)
    for (j=0;j<LENGTH;j++) 
      if (Level->site[i][j].things != NULL) {
	if (blessing < 0)
	  putspot(random_range(WIDTH),
		  random_range(LENGTH),
		  Level->site[i][j].things->thing->objchar);
	else putspot(i,j,Level->site[i][j].things->thing->objchar);
      }
  levelrefresh();
  morewait();
  show_screen();
}

void identify(blessing)
int blessing;
{
  int index;

  clearmsg();

  if (blessing == 0) {
    print1("Identify:");
    index = getitem(NULL_ITEM);
    if (index == CASHVALUE) print3("Your money is really money.");
    else if (index == ABORT)
      setgamestatus(SKIP_MONSTERS);
    else {
      if (Player.possessions[index]->objchar == FOOD)
	Player.possessions[index]->known = 1;
      else {
	Player.possessions[index]->known = 2;
	Objects[Player.possessions[index]->id].known = 1;
      }
      print1("Identified: ");
      mprint(itemid(Player.possessions[index]));
    }
  }
  else if (blessing < 0) {
    print2("You feel forgetful.");
    for (index=0;index<MAXITEMS;index++) 
      if (Player.possessions[index] != NULL) {
	Player.possessions[index]->known = 0;
	Objects[Player.possessions[index]->id].known = 0;
      }
  }
  else {
    print2("You feel encyclopaedic.");
    for (index=0;index<MAXITEMS;index++)
      if (Player.possessions[index] != NULL) {
	if (Player.possessions[index]->objchar == FOOD)
	  Player.possessions[index]->known = 1;
	else {
	  Player.possessions[index]->known = 2;
	  Objects[Player.possessions[index]->id].known = 1;
	}
      }
    for (index=0;index<Player.packptr;index++)
      if (Player.pack[index] != NULL) {
	if (Player.pack[index]->objchar == FOOD)
	  Player.pack[index]->known = 1;
	else {
	  Player.pack[index]->known = 2;
	  Objects[Player.pack[index]->id].known = 1;
	}
      }
  }
  calc_melee();
}




/* returns index of random item, ABORT if player carrying none */
int random_item()
{
  int item = ABORT,tries=0;
  int number = 0;

  for(tries=0; tries<MAXITEMS; tries++)
    if (Player.possessions[tries] != NULL) {
      number++;
      if (random_range(number) == 0)
	item = tries;
    }
  return(item);
}

  
/* various kinds of wishes */
void wish(blessing)
int blessing;
{
  int i;
  char wishstr[80];
  clearmsg();
  print1("What do you wish for? ");
  if (blessing < 0)
    deathprint();
  else
    strcpy(wishstr,msgscanstring());
  if (blessing < 0 || strcmp(wishstr,"Death")==0) {
    print2("As you wish, so shall it be.");
    p_death("a deathwish");
  }
  if (strcmp(wishstr,"Power")==0) {
    print2("You feel a sudden surge of energy");
    Player.mana=calcmana()*10;
  }
  else if (strcmp(wishstr,"Skill")==0) {
    print2("You feel more competent.");
    if (gamestatusp(CHEATED))
      gain_experience(10000);
    else
      gain_experience(min(10000,Player.xp));
  }
  else if (strcmp(wishstr,"Wealth")==0) {
    print2("You are submerged in shower of gold pieces!");
    Player.cash += 10000;
  }
  else if (strcmp(wishstr,"Balance")==0) {
    print2("You feel neutral.");
    Player.alignment = 0;
  }
  else if (strcmp(wishstr,"Chaos")==0) {
    print2("You feel chaotic.");
    Player.alignment -= 25;
  }
  else if (strcmp(wishstr,"Law")==0) {
    print2("You feel lawful.");
    Player.alignment += 25;
  }
  else if (strcmp(wishstr,"Location")==0)
    strategic_teleport(1);
  else if (strcmp(wishstr,"Knowledge")==0) {
    print2("You feel more knowledgeable.");
    i = random_range(NUMSPELLS);
    if (Spells[i].known) 
      Spells[i].powerdrain =
	(max(1,Spells[i].powerdrain/2));
    else Spells[i].known = TRUE;
  }
  else if (strcmp(wishstr,"Health")==0) {
    print2("You feel vigorous");
    Player.hp = max( Player.hp, Player.maxhp);
    Player.status[DISEASED] = 0;
    Player.status[POISONED] = 0;
  }
  else if (strcmp(wishstr,"Destruction")==0)
    annihilate(gamestatusp(CHEATED));
  else if (strcmp(wishstr,"Acquisition")==0)
    acquire(gamestatusp(CHEATED));
  else if (strcmp(wishstr,"Summoning")==0)
    summon(gamestatusp(CHEATED),-1);
  else if (strcmp(wishstr,"Stats") == 0 && gamestatusp(CHEATED))
  {
    Player.str = Player.maxstr = Player.con = Player.maxcon = 
      Player.agi = Player.maxagi = Player.dex = Player.maxdex = 
      Player.iq = Player.maxiq = Player.pow = Player.maxpow = 200;
    calc_melee();
  }
  else print2("You feel stupid.");
  dataprint();
  showflags();
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
      print1("Smoke drifts out of your pack.... ");
      print2("Destroyed: ");
      nprint2(itemid(Player.possessions[index]));
      morewait();
      dispose_lost_objects(1,Player.possessions[index]);
    }
  }
  else {
    newthing = ((pob) checkmalloc(sizeof(objtype)));
    newthing->id = -1;
    if (gamestatusp(CHEATED))
      print1("Acquire which kind of item: !?][}{)/=%%\\& ");
    else
      print1("Acquire which kind of item: !?][}{)/=%%\\ ");
    otype = mgetc();
    switch (otype) {
    case (POTION&0xff): 
      if (blessing > 0)
	id = itemlist(POTIONID,NUMPOTIONS);
      else
	id = random_range(NUMPOTIONS);
      if (id < 0) print2("You feel stupid."); 
      else make_potion(newthing,id); 
      break;
    case (SCROLL&0xff):
      if (blessing > 0)
	id = itemlist(SCROLLID,NUMSCROLLS);
      else
	id = random_range(NUMSCROLLS);
      if (id < 0) print2("You feel stupid."); 
      else make_scroll(newthing,id); 
      break;
    case (RING&0xff): 
      if (blessing > 0)
	id = itemlist(RINGID,NUMRINGS);
      else
	id = random_range(NUMRINGS);
      if (id < 0) print2("You feel stupid."); 
      else make_ring(newthing,id); 
      break;
    case (STICK&0xff):
      if (blessing > 0)
	id = itemlist(STICKID,NUMSTICKS);
      else
	id = random_range(NUMSTICKS);
      if (id < 0) print2("You feel stupid."); 
      else make_stick(newthing,id); 
      break;
    case (ARMOR&0xff): 
      if (blessing > 0)
	id = itemlist(ARMORID,NUMARMOR);
      else
	id = random_range(NUMARMOR);
      if (id < 0) print2("You feel stupid."); 
      else make_armor(newthing,id); 
      break;
    case (SHIELD&0xff):
      if (blessing > 0)
	id = itemlist(SHIELDID,NUMSHIELDS);
      else
	id = random_range(NUMSHIELDS);
      if (id < 0) print2("You feel stupid."); 
      else make_shield(newthing,id); 
      break;
    case (WEAPON&0xff):
      if (blessing > 0)
	id = itemlist(WEAPONID,NUMWEAPONS);
      else
	id = random_range(NUMWEAPONS);
      if (id < 0) print2("You feel stupid."); 
      else make_weapon(newthing,id); 
      break;
    case (BOOTS&0xff):
      if (blessing > 0)
	id = itemlist(BOOTID,NUMBOOTS);
      else
	id = random_range(NUMBOOTS);
      if (id < 0) print2("You feel stupid."); 
      else make_boots(newthing,id); 
      break;
    case (CLOAK&0xff): 
      if (blessing > 0)
	id = itemlist(CLOAKID,NUMCLOAKS);
      else
	id = random_range(NUMCLOAKS);
      if (id < 0) print2("You feel stupid."); 
      else make_cloak(newthing,id); 
      break;
    case (FOOD&0xff): 
      if (blessing > 0)
	id = itemlist(FOODID,NUMFOODS);
      else
	id = random_range(NUMFOODS);
      if (id < 0) print2("You feel stupid."); 
      else make_food(newthing,id); 
      break;
    case (THING&0xff):
      if (blessing > 0)
	id = itemlist(THINGID,NUMTHINGS);
      else
	id = random_range(NUMTHINGS);
      if (id < 0) print2("You feel stupid."); 
      else make_thing(newthing,id); 
      break;
    case (ARTIFACT&0xff):
      if (gamestatusp(CHEATED))
	id = itemlist(ARTIFACTID,NUMARTIFACTS);
      else
	id = -1;
      if (id < 0) print2("You feel stupid."); 
      else make_artifact(newthing,id); 
      break;
    default:
      print2("You feel stupid."); 
    }
    xredraw();
    if (id != ABORT) {
      if (blessing > 0) {
	newthing->known = 2;
	Objects[id].known = 1;
      }
      newthing->used = FALSE;
      gain_item(newthing);
    }
  }
}
