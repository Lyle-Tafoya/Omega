
/* omega copyright (C) by Laurence Raphael Brothers, 1987,1988 */
/* oeffect1.c */

#include "oglob.h"


/* enchant */
void enchant(delta)
int delta;
{
  int i,used = FALSE;
  float mult;

  if (delta < 0) {
    i = random_item();
    if (i == ABORT) {
      print1("You feel fortunate.");
      morewait();
    }
    else {
      if (Player.possessions[i]->blessing < 0) {
	print1("The item glows, but the glow flickers out...");
	morewait();
      }
      else {
	used = (Player.possessions[i]->used);
	if (used) {
	  Player.possessions[i]->used = FALSE;
	  item_use(Player.possessions[i]);
	} 
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
  }
  else {
    i = getitem(NULL);
    if (i == ABORT) {
      print1("You feel unlucky.");
      morewait();
    }
    else if (i == CASHVALUE) {
      print1("You enchant your money.... What a concept!");
      mult = 1 + (random_range(7)-3)/6.0;
      if (mult > 1.0) print2("Seems to have been a good idea!");
      else print2("Maybe it wasn't such a good idea....");
      Player.cash = ((int) (mult*Player.cash));
      morewait();
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
      print1("A foul odor arises from your ");
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
    index = getitem(NULL);
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
  else if (Level->site[xx][yy].locchar == HEDGE)
    if (Level->site[xx][yy].p_locf == L_HEDGE) {
      if ((dtype == FLAME)||(dtype == ELECTRICITY)) {
	mprint("The hedge is blasted away!");
	Level->site[xx][yy].p_locf = L_NO_OP;
	Level->site[xx][yy].locchar = FLOOR;
      }
      else mprint("The hedge is unaffected.");
    }
    else mprint("The trifid absorbs the energy and laughs!");
  else if (Level->site[xx][yy].locchar == WATER)
    if (dtype == FLAME) {
      mprint("The water is vaporised!");
      Level->site[xx][yy].p_locf = L_NO_OP;
      Level->site[xx][yy].locchar = FLOOR;
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
    else if (NULL != (target = Level->site[ex][ey].creature)) {
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
    else if (Level->site[ex][ey].locchar == HEDGE)
      if (Level->site[ex][ey].p_locf == L_HEDGE) {
	if ((dtype == FLAME)||(dtype == ELECTRICITY)) {
	  mprint("The hedge is blasted away!");
	  Level->site[ex][ey].p_locf = L_NO_OP;
	  Level->site[ex][ey].locchar = FLOOR;
	}
	else mprint("The hedge is unaffected.");
      }
      else mprint("The trifid absorbs the energy and laughs!");
    else if (Level->site[xx][yy].locchar == WATER)
      if (dtype == FLAME) {
	mprint("The water is vaporised!");
	Level->site[xx][yy].p_locf = L_NO_OP;
	Level->site[xx][yy].locchar = FLOOR;
      }
  }
}




void mondet(blessing)
int blessing;    
{
  pml ml;
  for (ml=Level->mlist;ml!=NULL;ml=ml->next) 
    if (ml->m->hp > 0)
      plotmon(blessing > -1 ? ml->m : &(Monsters[random_range(NUMMONSTERS)]));
  levelrefresh();
  morewait();
  erase_level();
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
  erase_level();
  show_screen();
}

void identify(blessing)
{
  int index;

  clearmsg();

  if (blessing == 0) {
    print1("Identify:");
    index = getitem(NULL);
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
  int item,tries=0;
  int done = FALSE;

  for(tries=0;((tries<MAXITEMS)&&(!done));tries++) {
    item = random_range(MAXITEMS);
    done = (Player.possessions[item] != NULL);
  }
  return(done ? item : ABORT);
}

  
/* various kinds of wishes */
void wish(blessing)
int blessing;
{
  int i;
  char wishstr[80];
  clearmsg();
  print1("What do you wish for? ");
  if (blessing < 0) deathprint();
  strcpy(wishstr,msgscanstring());
  if (strcmp(wishstr,"Death")==0) {
    print2("As you wish, so shall it be.");
    p_death("a deathwish");
  }
  if (strcmp(wishstr,"Power")==0) {
    print2("You feel a sudden surge of energy");
    Player.mana=calcmana()*10;
  }
  else if (strcmp(wishstr,"Skill")==0) {
    print2("You feel more competent.");
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
    Player.hp = Player.maxhp;
    Player.status[DISEASED] = 0;
    Player.status[POISONED] = 0;
  }
  else if (strcmp(wishstr,"Destruction")==0)
    annihilate(gamestatusp(CHEATED));
  else if (strcmp(wishstr,"Acquisition")==0)
    acquire(gamestatusp(CHEATED));
  else if (strcmp(wishstr,"Summoning")==0)
    summon(gamestatusp(CHEATED),-1);
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
  else if (blessing == 0) {
    print1("You magically acquire an object!");
    morewait();
    newthing = ((pob) malloc(sizeof(objtype)));
    *newthing = Objects[random_range(TOTALITEMS)];
    newthing->used = FALSE;
    gain_item(newthing);
  }
  else {
    newthing = ((pob) malloc(sizeof(objtype)));
    newthing->id = -1;
    
    print1("Acquire which kind of item: !?][}{)/=%%\\ ");
    otype = mgetc();
    nprint1("Item ID? ");
    switch (otype) {
    case POTION: 
      id = itemlist(POTIONID,NUMPOTIONS);
      if (id < 0) print2("You feel stupid."); 
      else make_potion(newthing,id); 
      break;
    case SCROLL:
      id = itemlist(SCROLLID,NUMSCROLLS);
      if (id < 0) print2("You feel stupid."); 
      else make_scroll(newthing,id); 
      break;
    case RING: 
      id = itemlist(RINGID,NUMRINGS);
      if (id < 0) print2("You feel stupid."); 
      else make_ring(newthing,id); 
      break;
    case STICK:
      id = itemlist(STICKID,NUMSTICKS);
      if (id < 0) print2("You feel stupid."); 
      else make_stick(newthing,id); 
      break;
    case ARMOR: 
      id = itemlist(ARMORID,NUMARMOR);
      if (id < 0) print2("You feel stupid."); 
      else make_armor(newthing,id); 
      break;
    case SHIELD:
      id = itemlist(SHIELDID,NUMSHIELDS);
      if (id < 0) print2("You feel stupid."); 
      else make_shield(newthing,id); 
      break;
    case WEAPON:
      id = itemlist(WEAPONID,NUMWEAPONS);
      if (id < 0) print2("You feel stupid."); 
      else make_weapon(newthing,id); 
      break;
    case BOOTS:
      id = itemlist(BOOTID,NUMBOOTS);
      if (id < 0) print2("You feel stupid."); 
      else make_boots(newthing,id); 
      break;
    case CLOAK: 
      id = itemlist(CLOAKID,NUMCLOAKS);
      if (id < 0) print2("You feel stupid."); 
      else make_cloak(newthing,id); 
      break;
    case FOOD: 
      id = itemlist(FOODID,NUMFOODS);
      if (id < 0) print2("You feel stupid."); 
      else make_food(newthing,id); 
      break;
    case THING:
      id = itemlist(THINGID,10); /* crock to avoid grot*/
      if (id < 0) print2("You feel stupid."); 
      else make_thing(newthing,id); 
      break;
    }
    xredraw();
    if (id != ABORT) {
      newthing->known = 2;
      Objects[id].known = 1;
      gain_item(newthing);
    }
  }
}
