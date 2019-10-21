/* omega copyright (C) by Laurence Raphael Brothers, 1987 */

/* oaux2.c */

/* some functions called by ocom.c, also see oaux2.c */ 
/* This is a real grab bag file. It contains functions used by
   oaux1 and o.c, as well as elsewhere. It is mainly here so oaux1.c
   is not huge */

#include <strings.h>
#include "oglob.h"

/* from ospell */
extern char *spellid();

/* from oeffect 1 or 2 */
extern void manastorm();

/* from oitemf */
extern void weapon_normal_hit(),weapon_demonblade(),weapon_lightsabre();
extern void weapon_mace_disrupt(),weapon_bare_hands();
extern void weapon_vorpal(),weapon_desecrate(),weapon_firestar();
extern void weapon_defend(),weapon_victrix(),weapon_tangle();
extern void weapon_arrow(),weapon_bolt();

/* from oitem */
extern void item_use(),damage_item();

/* from oinv */
extern void pickup_at(),drop_at(),givemonster(),conform_lost_objects();
extern void conform_unused_object(),add_item_to_pack();
extern char *itemid();
extern int getitem(),badobject(),cursed();

/* from outil */
extern int random_range();
extern int inbounds(),confirmation();
extern char inversedir();
extern char *getarticle();
extern int unblocked(),bitp();
extern void bitset(),bitreset();

/* from oscr */
extern void showcursor(),refreshmsg(),drawspot(),describe_player();
extern void mprint(),endgraf(),showflags(),dataprint(),menuaddch();
extern void show_screen(),xredraw(),phaseprint();
extern void drawvision(),display_death(),morewait(),menumorewait();
extern void menuclear(),menuprint(),display_win(),mnumprint();
extern char mgetc(),lgetc(),ynq();
extern void screencheck();


/* from ocom */
void p_process(),wwp();

/* from omon */
extern void m_damage(),m_status_set(),m_status_reset(),m_death(),m_pickup();
extern void tacmonster();
extern int m_statusp();

/* from oaux1 */
extern void p_damage(),calc_melee(),p_death();

/* oaux2 functions */
char *actionlocstr();
void break_weapon(),drop_weapon(),moon_check(),toggle_item_use();
void p_fumble(),p_win(),p_hit(),p_miss(),addring(),torch_check();
void top_level(),printactions(),movecursor(),p_drown();
void tacexecute(),weapon_use(),move_status_check(),hour_status_check();
int expval(),player_hit(),statmod(),item_value(),true_item_value();




 /* Player stats like str, agi, etc give modifications to various abilities
    chances to do things, etc. Positive is good, negative bad. */
 int statmod(stat)
 int stat;
 {
   return((stat-10)/2);
 }


/* effects of hitting */
void p_hit (m,dmg,dtype)
struct monster *m;
int dmg;
int dtype;
{
  int dmult;

  /* chance for critical hit..., 3/10 */
  switch (random_range(10)) {  
    case 0:
    if (random_range(100) < Player.level) {
      strcpy(Str3,"You annihilate the ");
      dmult = 1000;
    }
    else {
      strcpy(Str3,"You blast "); 
      dmult=5; 
    }
    break;
    case 1:
    case 2: strcpy(Str3,"You smash "); dmult=2; break;
    default: strcpy(Str3,"You hit "); dmult=1; break;
  } 
  if (Lunarity == 1) dmult = dmult * 2;
  else if (Lunarity == -1) dmult = dmult / 2;
  if (m->uniqueness == COMMON) strcat(Str3,"the ");
  strcat(Str3,m->monstring);
  strcat(Str3,". ");
  if (Tacmode) {
    menuprint(Str3);
    menuprint("\n");
  }
  else mprint(Str3);
  m_damage(m,dmult * random_range(dmg),dtype);
}

/* and effects of missing */
void p_miss(m,dtype)
struct monster *m;
int dtype;
{
  if (random_range(30)==1) /* fumble 1 in 30 */
    p_fumble(dtype);
  else {
    strcpy(Str3,"You miss ");
    if (m->uniqueness == COMMON) strcat(Str3,"the ");
    strcat(Str3,m->monstring);
    strcat(Str3,". ");
    if (Tacmode) {
      menuprint(Str3);
      menuprint("\n");
    }
    else mprint(Str3);
  }
}  

/* oh nooooo, a fumble.... */
void p_fumble(dtype)
int dtype;
{
  mprint("Ooops! You fumbled....");
  switch(random_range(10)) {
    case 0:
    case 1:
    case 2:
    case 3: 
    case 4: 
    case 5: drop_weapon(); break;
    case 6:
    case 7:
    case 8: break_weapon(); break;
    case 9: mprint("Oh No! You hit yourself!");
	    p_damage(Player.dmg,dtype,"stupidity");
	    break;
  }
}	    

/* try to drop a weapon (from fumbling) */
void drop_weapon()
{
  int i;
  if (Player.primary != NULL) {
    strcpy(Str1,"You dropped your ");
    strcat(Str1,Player.primary->objstr);
    mprint(Str1);
    morewait();
    p_drop_at(Player.x,Player.y,1,Player.primary);
    conform_lost_objects(1,Player.primary);
  }
  else mprint("You feel fortunate.");
}


/* try to break a weapon (from fumbling) */
void break_weapon()
{
  if (Player.primary != NULL) {
    strcpy(Str1,"Your ");
    strcat(Str1,itemid(Player.primary));
    strcat(Str1,"vibrates in your hand....");
    mprint(Str1);
    damage_item(Player.primary);
    morewait();
  }
}


/* hooray */
void p_win()
{
  mprint("You won!");
  morewait();
  display_win();
  endgraf();
  exit(0);
}


/* handle a h,j,k,l, etc., to change x and y by dx and dy */
void movecursor(x,y,dx,dy)
int *x,*y;
int dx,dy;
{
  if (inbounds(*x+dx,*y+dy)) {
    *x += dx;
    *y += dy;
    screencheck(*y);
  }
  showcursor(*x,*y);
}


/* is Player immune to damage type dtype */
int p_immune(dtype)
int dtype;
{
  return(Player.immunity[dtype]);
}






/* deal with each possible stati -- values are per move */
/* this function is executed every move */
/* A value over 1000 indicates a permanent effect */
void move_status_check()
{
  int i;

  if (Player.status[HASTED]>0) {
    if (Player.status[HASTED] < 1000) {
      Player.status[HASTED]--;
      if (Player.status[HASTED]==0) {
	mprint("The world speeds up.");
	calc_melee();
      }
    }
  }


  if (Player.status[POISONED]>0) {
    Player.status[POISONED]--;
    p_damage(3,POISON,"poison");
    if (Player.status[POISONED] == 0) {
      showflags();
      mprint("You feel better now.");
    }
  }


  if (Player.immunity[UNSTOPPABLE]>0) {
    for(i=0;i<NUMIMMUNITIES;i++)
    Player.immunity[i]--;
    if (Player.immunity[UNSTOPPABLE]==1)
      mprint("You feel vincible again.");
  }


  if (Player.status[IMMOBILE]>0) {
    Player.status[IMMOBILE]--;
    if (Player.status[IMMOBILE] == 0) 
      mprint("You can move again.");
  }


  if (Player.status[SLEPT]>0) {
    Player.status[SLEPT]--;
    Skipplayer = TRUE;
    if (Player.status[SLEPT] == 0) {
      mprint("You woke up.");
      Skipplayer = FALSE;
    }
  }

  if (Player.status[REGENERATING]>0) {
    if (Player.hp < Player.maxhp) Player.hp++;
    if (Player.status[REGENERATING] < 1000) {
      Player.status[REGENERATING]--;
      if (Player.status[REGENERATING] == 0) {
	mprint("You feel less homeostatic.");
      }
    }
  }

  if (Player.status[SLOWED]>0) {
    if (Player.status[SLOWED] < 1000) {
      Player.status[SLOWED]--;
      if (Player.status[SLOWED] == 0) {
	mprint("You feel quicker now.");
	calc_melee();
      }
    }
  }

  if (Player.status[RETURNING]>0) {
    Player.status[RETURNING]--;
    if (Player.status[RETURNING] == 150)
      mprint("Your spell slowly hums towards activation...");
    else if (Player.status[RETURNING] == 100)
      mprint("There is an electric tension in the air!");
    else if (Player.status[RETURNING] == 50)
      mprint("A vortex of mana begins to form around you!");
    else if (Player.status[RETURNING] == 10)
      mprint("Your surroundings start to warp and fade!");
    if (Player.status[RETURNING] == 0) {
      mprint("The vortex of mana carries you off!");
      level_return();
    }
  }
  
  if (Player.status[AFRAID]>0) {
    if (Player.status[AFRAID] < 1000) {
      Player.status[AFRAID]--;
      if (Player.status[AFRAID] == 0) {
	mprint("You feel bolder now.");
      }
    }
  }
  
}



/* effect of gamma ray radiation... */
void moon_check()
{
  /* 24 day lunar cycle */
  Phase = (Phase+1)%24;
  phaseprint();
  Lunarity = 0;
  if (((Player.patron == DRUID) && ((Phase/2 == 3) || (Phase/2 == 9))) ||
      ((Player.alignment > 10) && (Phase/2 == 6)) ||
      ((Player.alignment < -10) && (Phase/2 == 0))) {
    mprint("As the moon rises you feel unusually vital!");
    Lunarity = 1;
  }
  else
  if (((Player.patron == DRUID) && ((Phase/2 == 0) || (Phase/2 == 6))) ||
      ((Player.alignment > 10) && (Phase/2 == 0)) ||
      ((Player.alignment < -10) && (Phase/2 == 6))) {
    mprint("The rise of the moon tokens a strange enervation!");
    Lunarity = -1;
  }

}


void torch_check()
{
  int i;
  for(i=0;i<MAXITEMS;i++) 
    if (Player.possessions[i]!=NULL)
      if ((Player.possessions[i]->id == THINGID+8) && /*torch */
	  (Player.possessions[i]->used) &&
	  (Player.possessions[i]->aux > 0)) {
	Player.possessions[i]->aux--;
	if (Player.possessions[i]->aux==0) {
	  mprint("Your torch goes out!!!");
	  conform_unused_object(Player.possessions[i]);
	  if (Player.possessions[i]->number > 1) {
	    Player.possessions[i]->number--;
	    Player.possessions[i]->aux = 6;
	  }
	  else {
	    Player.possessions[i]->usef = I_NO_OP;
	    strcpy(Player.possessions[i]->objstr,"burnt-out torch");
	    strcpy(Player.possessions[i]->truename,"burnt-out torch");
	    strcpy(Player.possessions[i]->cursestr,"burnt-out torch");
	  }
	}
      }
}

/* these values are in hours */
/* this function is checked hourly */
/* values over 1000 indicate a permanent effect */
void hour_status_check()
{
  if (hour()==0) { /* midnight, a new day */
    moon_check();
    Date++;
  }
  torch_check();
  if ((Player.status[SHADOWFORM]>0) && (Player.status[SHADOWFORM]<1000)) {
    Player.status[SHADOWFORM]--;
    if (Player.status[SHADOWFORM] == 0) {
      Player.immunity[NORMAL_DAMAGE]--;
      Player.immunity[ACID]--;
      Player.immunity[THEFT]--;
      Player.immunity[INFECTION]--;
      mprint("You feel less shadowy now.");
    }
  }

  if ((Player.status[VULNERABLE]>0) && (Player.status[VULNERABLE]<1000)){
    Player.status[VULNERABLE]--;
    if (Player.status[VULNERABLE] == 0)
      mprint("You feel less endangered.");
  }

  if ((Player.status[ACCURATE]>0) && (Player.status[ACCURACY]<1000)){
    Player.status[ACCURATE]--;
    if (Player.status[ACCURATE] == 0) {
      calc_melee();
      mprint("The bulls' eyes go away.");
    }
  }
  if ((Player.status[HERO]>0) && (Player.status[HERO]<1000)){
    Player.status[HERO]--;
    if (Player.status[HERO] == 0) {
      calc_melee();
      mprint("You feel less than super.");
    }
  }

  if ((Player.status[LEVITATING]>0) && (Player.status[LEVITATING]<1000)){
    Player.status[LEVITATING]--;
    if (Player.status[LEVITATING] == 0)
      mprint("You're no longer walking on air.");
  }

  if (Player.status[DISEASED]>0) {
    Player.status[DISEASED]--;
    if (Player.status[DISEASED] == 0) {
      showflags();
      mprint("You feel better now.");
    }
  }


  if ((Player.status[INVISIBLE] > 0) && (Player.status[INVISIBLE]<1000)){
    Player.status[INVISIBLE]--;
    if (Player.status[INVISIBLE] == 0)
      mprint("You feel more opaque now.");
  }

  if ((Player.status[BLINDED]>0) && (Player.status[BLINDED]<1000)) {
    Player.status[BLINDED]--;
    if (Player.status[BLINDED] == 0) 
      mprint("You can see again.");
  }

  if ((Player.status[TRUESIGHT]>0) && (Player.status[TRUESIGHT]<1000)) {
    Player.status[TRUESIGHT]--;
    if (Player.status[TRUESIGHT] == 0) 
      mprint("You feel less keen now.");
  }

  if ((Player.status[BERSERK]>0) && (Player.status[BERSERK]<1000)) {
    Player.status[BERSERK]--;
    if (Player.status[BERSERK] == 0) 
      mprint("You stop foaming at the mouth.");
  }

  if ((Player.status[ALERT]>0) && (Player.status[ALERT] < 1000)) {
    Player.status[ALERT]--;
    if (Player.status[ALERT] == 0) 
      mprint("You feel less alert now.");
  }

  if ((Player.status[BREATHING]>0) && (Player.status[BREATHING] < 1000)) {
    Player.status[BREATHING]--;
    if (Player.status[BREATHING] == 0) 
      mprint("You feel somewhat congested."); 
  }

  if ((Player.status[DISPLACED]>0) && (Player.status[DISPLACED] < 1000)) {
    Player.status[DISPLACED]--;
    if (Player.status[DISPLACED]==0) 
      mprint("You feel a sense of position.");
  }
  timeprint();
}



/* Increase in level at appropriate experience gain */
void gain_level()
{
  if (expval(Player.level+1) <= Player.xp) {
    Player.level++;
    mprint("You have attained a new level of experience!");
    Player.maxhp += random_range(Player.con)+1;
    Player.mana = Player.maxmana = calcmana();
    gain_level();
  }
  calc_melee();
}

/* experience requirements */
int expval(plevel)
int plevel;
{
  switch(plevel) {
    case 0:return(0);break;
    case 1:return(20);break;
    case 2:return(40);break;
    case 3:return(80);break;
    case 4:return(160);break;
    case 5:return(320);break;
    case 6:return(640);break;
    case 7:return(1280);break;
    case 8:return(2560);break;
    case 9:return(5120);break;
    case 10:return(10240);break;
    default:return((plevel-9) * 10240); break;
  }
}

/* If an item is unidentified, it isn't worth much to those who would buy it */
int item_value(item)
pob item;
{
  if (item->known == 0) {
    if (item->objchar == THING) return(1);
    else return(true_item_value(item) / 10);
  }
  else if (item->known == 1) {
    if (item->objchar == THING) return(item->basevalue);
    else return(item->basevalue / 2);
  }
  else return(true_item_value(item));
}


/* figures value based on item base-value, charge, plus, and blessing */
int true_item_value(item)
pob item;
{
  float value = item->basevalue;
  
  if (item->objchar == THING) return(item->basevalue);
  else {
    if (item->objchar == STICK) value *= (1.0 + (item->charge/20.0));
    if (item->plus > -1.0) value *= (1.0 + (item->plus/4.0));
    else value *= (1.0/abs(item->plus));
    if (item->blessing > 0) value *= 2.0;
    return((int) value);
  }
}

/* kill of player if he isn't got the "breathing" status */
void p_drown()
{
  if (Player.status[BREATHING] > 0)
    mprint("Your breathing is unaffected!");
  else {
    mprint("You try to hold your breath....");
    mprint("You choke....");
    mprint("Your lungs fill...");
    p_death("drowning");
  }
}


/* the effect of some weapon on monster m, with dmgmod a bonus to damage */
void weapon_use(dmgmod,weapon,m)
int dmgmod;
pob weapon;
struct monster *m;
{
  int aux = (weapon==NULL ? -2 : weapon->aux); /* bare hands */
  switch(aux) {
    case -2: weapon_bare_hands(dmgmod,m); break;
    case I_NO_OP: weapon_normal_hit(dmgmod,weapon,m); break;
    case I_TANGLE: weapon_tangle(dmgmod,weapon,m); break;
    case I_ARROW: weapon_arrow(dmgmod,weapon,m); break;
    case I_BOLT: weapon_bolt(dmgmod,weapon,m); break;
    case I_DEMONBLADE: weapon_demonblade(dmgmod,weapon,m); break;
    case I_LIGHTSABRE: weapon_lightsabre(dmgmod,weapon,m); break;
    case I_MACE_DISRUPT: weapon_mace_disrupt(dmgmod,weapon,m); break;
    case I_VORPAL: weapon_vorpal(dmgmod,weapon,m); break;
    case I_DESECRATE: weapon_desecrate(dmgmod,weapon,m); break;
    case I_FIRESTAR: weapon_firestar(dmgmod,weapon,m); break;
    case I_DEFEND: weapon_defend(dmgmod,weapon,m); break;
    case I_VICTRIX: weapon_victrix(dmgmod,weapon,m); break;
  }
}

/* auxiliary for tacmode: block, attack, lunge, riposte actions */
int getlocation()
{
  char response;
  menuprint(" low, center, or high [lch]: ");
  do
    response = mgetc();
  while((response!='l')&&(response!='c')&&(response!='h'));
  if (response == 'l') return(LOW);
  else return(response=='c' ? CENTER : HIGH);
}

void printactions(pnumactions,p_actions)
int pnumactions,p_actions[10];
{
  int i;
  menuprint("\nActions:\n");
  for(i=0;i<pnumactions;i++) {
    switch(p_actions[i] - (p_actions[i] % 10)) {
    case PICK_UP: 
      strcpy(Str1,"Pick up item.");
      i = pnumactions;
      break;
    case WIELD: 
      strcpy(Str1,"Wield item.");
      i = pnumactions;
      break;
    case BLOCK: 
      strcpy(Str1,"Block");
      strcat(Str1,actionlocstr(p_actions[i] % 10));
      break;
    case CUT: 
      strcpy(Str1,"Cut");
      strcat(Str1,actionlocstr(p_actions[i] % 10));
      break;
    case THRUST: 
      strcpy(Str1,"Thrust");
      strcat(Str1,actionlocstr(p_actions[i] % 10));
      break;
    case RIPOSTE: 
      strcpy(Str1,"Riposte");
      strcat(Str1,actionlocstr(p_actions[i] % 10));
      break;
    case MAGIC:
      strcpy(Str1,"Cast Spell --");
      strcat(Str1,spellid(p_actions[2]));
      i = pnumactions;
      break;
    case LUNGE:
      strcpy(Str1,"Lunge");
      strcat(Str1,actionlocstr(p_actions[i] % 10));
      break;
    case DISENGAGE:
      strcpy(Str1,"Disengage.");
      i = pnumactions;
      break;
    default:
      strcpy(Str1,"Nothing.");
      break;
    }
    menuprint(Str1);
    menuprint("\n");
  }
}

/* for printing actions in printactions above */
char *actionlocstr(dir)
int dir;
{
  switch(dir) {
  case LOW: return(" low."); break;
  case CENTER: return(" center."); break;
  case HIGH: return(" high."); break;
  default: return("."); break;
  }
}


/* execute actions chosen in tacplayer and tacmonster() */
void tacexecute(p_actions,pnumactions,m_actions,mnumactions,m)
int pnumactions,p_actions[10],mnumactions,m_actions[10];
struct monster *m;
{
  int i,j,hitmod;

  /* player actions first */
  for (i=0;i<pnumactions;i++) {
    if (m->hp > 0) {
      menuprint("\n");
      switch(p_actions[i]-(p_actions[i]%10)){
      case DISENGAGE:
	menuprint("\nYou try to disengage...");
	if (Player.agi+Player.level + random_range(20) >
	    m->level*2+m->speed*5+random_range(20)) {
	  menuprint("\nYou get away!");
	  Tacmode = FALSE;
	  Skipmonsters = TRUE;
	}
	else menuprint("\nYou don't get away!");
	break;
      case THRUST:
	if (Player.primary == NULL) strcpy(Str1,"You jab");
	else if (Player.primary->type == MISSILE) strcpy(Str1,"You shoot");
	else strcpy(Str1,"You thrust");
	strcat(Str1,actionlocstr(p_actions[i] % 10));
	menuprint(Str1);
	hitmod = 0;
	for (j=0;j<pnumactions;j++)
	  if (((p_actions[j] - (p_actions[j] % 10) == BLOCK) ||
	       (p_actions[j] - (p_actions[j] % 10) == RIPOSTE)) &&
	      (p_actions[j] % 10 == p_actions[i] % 10))
	    hitmod -= 10;
	if (player_hit(2*statmod(Player.dex)+hitmod,p_actions[i] % 10,
		       mnumactions,
		       m_actions,
		       m))
	  weapon_use(0,Player.primary,m);
	else p_miss(m,NORMAL_DAMAGE);
	break;
      case CUT:
	if (Player.primary == NULL) strcpy(Str1,"You punch");
	else if (Player.primary->type == CUTTING) strcpy(Str1,"You cut");
	else if (Player.primary->type == STRIKING) strcpy(Str1,"You strike");
	strcat(Str1,actionlocstr(p_actions[i] % 10));
	menuprint(Str1);
	hitmod = 0;
	for (j=0;j<pnumactions;j++)
	  if (((p_actions[j] - (p_actions[j] % 10) == BLOCK) ||
	       (p_actions[j] - (p_actions[j] % 10) == RIPOSTE)) &&
	      (p_actions[j] % 10 == p_actions[i] % 10))
	    hitmod -= 10;
	if (player_hit(hitmod,p_actions[i] % 10,
		       mnumactions,
		       m_actions,
		       m))
	  weapon_use(2*statmod(Player.str),Player.primary,m);
	else p_miss(m,NORMAL_DAMAGE);
	break;
      case MAGIC:
	Player.mana -= Spells[p_actions[1]].powerdrain;
	(*(Spells[p_actions[1]].castf))();
	i = pnumactions;
	break;
      case PICK_UP:
	pickup();
	i = pnumactions;
	break;
      case WIELD:
	wwp();
	i = pnumactions;
	break;
      case LUNGE:
	strcpy(Str1,"You lunge");
	strcat(Str1,actionlocstr(p_actions[i] % 10));
	menuprint(Str1);
	hitmod = 0;
	for (j=0;j<pnumactions;j++)
	  if (((p_actions[j] - (p_actions[j] % 10) == BLOCK) ||
	       (p_actions[j] - (p_actions[j] % 10) == RIPOSTE)) &&
	      (p_actions[j] % 10 == p_actions[i] % 10))
	    hitmod -= 10;
	if (player_hit(Player.level+Player.dex+hitmod,
		       p_actions[i] % 10,
		       mnumactions,
		       m_actions,
		       m))
	  weapon_use(Player.level,Player.primary,m);
	else p_miss(m,NORMAL_DAMAGE);
	break;
      }
    }
  }


  /* now monster actions */
  for (i=0;i<mnumactions;i++) {
    if (m->hp > 0) {
      menuprint("\n");
      switch(m_actions[i]-(m_actions[i]%10)){
      case DISENGAGE:
	menuprint("\nYour opponent tries to run away...");
	if (Player.agi+Player.level + random_range(20) <
	    m->level*2+m->speed*5+random_range(20)) {
	  menuprint("\n...and succeeds!");
	  m->movef = M_MOVE_SCAREDY;
	  movemonster(m);
	  movemonster(m);
	  Tacmode = FALSE;
	}
	else menuprint("\n...and fails!");
	break;
      case CUT:
	hitmod = 0;
	for (j=0;j<mnumactions;j++)
	  if (((m_actions[j] - (m_actions[j] % 10) == BLOCK) ||
	       (m_actions[j] - (m_actions[j] % 10) == RIPOSTE)) &&
	      (m_actions[j] % 10 == m_actions[i] % 10))
	    hitmod -= 10;
	m->hit += hitmod;
	monster_melee(m);
	m->hit -= hitmod;
	break;
      }
    }
  }
  morewait();
  monster_special(m);
}




/* checks to see if player hits with hitmod vs.monster m at location hitloc */
int player_hit(hitmod,hitloc,mnumactions,m_actions,m)
int hitmod,hitloc,mnumactions,m_actions[10];
struct monster *m;
{
  int i,b,blocks=FALSE,goodblocks=0,hit;
  if (m->hp < 1) {
    mprint("Unfortunately, your opponent is already dead!");
    return(FALSE);
  }
  else {
    for(i=0;i<mnumactions;i++) 
      if (m_actions[i] - (m_actions[i] % 10) == BLOCK) {
	blocks = TRUE;
	if (hitloc == m_actions[i] % 10)
	  goodblocks++;
      }
    if (! blocks) goodblocks = -1;
    hit = hitp(Player.hit+hitmod,m->ac+goodblocks*10);
    if ((! hit) && (goodblocks > 0)) {
      if (m->uniqueness == COMMON) {
	strcpy(Str1,"The ");
	strcat(Str1,m->monstring);
      }
      else strcpy(Str1,m->monstring);
      strcat(Str1," blocks it!");
      menuprint("\n");
      menuprint(Str1);
    }
    return(hit);
  }
}



/* This function is used to undo all items temporarily, should
always be used in pairs with on being TRUE and FALSE, and may cause
anomalous stats and item-usage if used indiscriminately */


void toggle_item_use(on)
int on;
{
  static int used[MAXITEMS];
  int i;
  SuppressPrinting = TRUE;
  if (on) 
    for(i=0;i<MAXITEMS;i++) {
      used[i] = FALSE;
      if (Player.possessions[i] != NULL) {
	if (used[i] = Player.possessions[i]->used) {
	  Player.possessions[i]->used = FALSE;
	  item_use(Player.possessions[i]);
	}
      }
    }
  else {
    for(i=0;i<MAXITEMS;i++) 
      if (used[i]) {
	Player.possessions[i]->used = TRUE;
	item_use(Player.possessions[i]);
      }
    calc_melee();
    showflags();
    dataprint();
    timeprint();
  }
  SuppressPrinting = FALSE;
}
