/* omega copyright (C) by Laurence Raphael Brothers, 1987,1988 */

/* oaux2.c */

/* some functions called by ocom.c, also see oaux1.c and oaux3.c*/ 
/* This is a real grab bag file. It contains functions used by
   oaux1 and o.c, as well as elsewhere. It is mainly here so oaux1.c
   and oaux3.c are not huge */

#include "oglob.h"


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
      strcpy(Str3,"You annihilate ");
      dmult = 1000;
    }
    else {
      strcpy(Str3,"You blast "); 
      dmult=5; 
    }
    break;
    case 1:
    case 2: 
    strcpy(Str3,"You smash "); 
    dmult=2; break;

    default: 
    dmult=1;
    if (random_range(10)) strcpy(Str3,"You hit ");
    else switch(random_range(4)) {
    case 0: strcpy(Str3,"You damage "); break;
    case 1: strcpy(Str3,"You inflict bodily harm on "); break;
    case 2: strcpy(Str3,"You injure "); break;
    case 3: strcpy(Str3,"You molest "); break;
    }
    break;
  } 
  if (Lunarity == 1) dmult = dmult * 2;
  else if (Lunarity == -1) dmult = dmult / 2;
  if (m->uniqueness == COMMON) strcat(Str3,"the ");
  strcat(Str3,m->monstring);
  strcat(Str3,". ");
  if (Verbosity != TERSE) mprint(Str3);
  else mprint("You hit it.");
  m_damage(m,dmult * random_range(dmg),dtype);
  if ((Verbosity != TERSE) && (random_range(10)==3) && (m->hp > 0))
    mprint("It laughs at the injury and fights on!");
}

/* and effects of missing */
void player_miss(m,dtype)
struct monster *m;
int dtype;
{
  if (random_range(30)==1) /* fumble 1 in 30 */
    p_fumble(dtype);
  else {
    if (Verbosity != TERSE) {
      if (random_range(10))
	strcpy(Str3,"You miss ");
      else switch(random_range(4)) {
      case 0: strcpy(Str3,"You flail lamely at "); break;
      case 1: strcpy(Str3,"You only amuse "); break;
      case 2: strcpy(Str3,"You fail to even come close to "); break;
      case 3: strcpy(Str3,"You totally avoid contact with "); break;
      }	
      if (m->uniqueness == COMMON) strcat(Str3,"the ");
      strcat(Str3,m->monstring);
      strcat(Str3,". ");
      mprint(Str3);
    }
    else mprint("You missed it.");
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
  if (Player.possessions[O_WEAPON_HAND] != NULL) {
    strcpy(Str1,"You dropped your ");
    strcat(Str1,Player.possessions[O_WEAPON_HAND]->objstr);
    mprint(Str1);
    morewait();
    p_drop_at(Player.x,Player.y,1,Player.possessions[O_WEAPON_HAND]);
    conform_lost_objects(1,Player.possessions[O_WEAPON_HAND]);
  }
  else mprint("You feel fortunate.");
}


/* try to break a weapon (from fumbling) */
void break_weapon()
{
  if (Player.possessions[O_WEAPON_HAND] != NULL) {
    strcpy(Str1,"Your ");
    strcat(Str1,itemid(Player.possessions[O_WEAPON_HAND]));
    strcat(Str1," vibrates in your hand....");
    mprint(Str1);
    damage_item(Player.possessions[O_WEAPON_HAND]);
    morewait();
  }
}


/* hooray */
void p_win()
{
  morewait();
  clearmsg();
  print1("You won!");
  morewait();
  display_win();
  endgraf();
  exit(0);
}


/* handle a h,j,k,l, etc., to change x and y by dx and dy */
/* for targeting in dungeon */
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
  return(Player.immunity[dtype]>0);
}






/* deal with each possible stati -- values are per move */
/* this function is executed every move */
/* A value over 1000 indicates a permanent effect */
void minute_status_check()
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
    if (Player.status[SLEPT] == 0) {
      mprint("You woke up.");
    }
  }

  if (Player.status[REGENERATING]>0) {
    if ((Player.hp < Player.maxhp) && (Player.mana > 0)){
      Player.hp++;
      Player.mana--;
      dataprint();
    }
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



/* check 1/hour for torch to burn out if used */
void torch_check()
{
  int i;
  for(i=O_READY_HAND;i<=O_WEAPON_HAND;i++) {
    if (Player.possessions[i]!=NULL)
      if ((Player.possessions[i]->id == THINGID+8) && /*torch */
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
	    Player.possessions[i]->cursestr =
	      Player.possessions[i]->truename =
		Player.possessions[i]->objstr = 
		  salloc("burnt-out torch");
	  }
	}
      }
  }
}



/* values are in multiples of ten minutes */
/* values over 1000 indicate a permanent effect */
void tenminute_status_check()
{
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

  if ((Player.status[ILLUMINATION]>0) && (Player.status[ILLUMINATION]<1000)) {
    Player.status[ILLUMINATION]--;
    if (Player.status[ILLUMINATION] == 0) {
      mprint("Your light goes out!");
    }
  }


  if ((Player.status[VULNERABLE]>0) && (Player.status[VULNERABLE]<1000)){
    Player.status[VULNERABLE]--;
    if (Player.status[VULNERABLE] == 0)
      mprint("You feel less endangered.");
  }


  if ((Player.status[DEFLECTION]>0) && (Player.status[DEFLECTION]<1000)){
    Player.status[DEFLECTION]--;
    if (Player.status[DEFLECTION] == 0)
      mprint("You feel less well defended.");
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
  dataprint();
}



/* Increase in level at appropriate experience gain */
void gain_level()
{
  int gained=FALSE;
  morewait();
  while (expval(Player.level+1) <= Player.xp) {
    gained = TRUE;
    Player.level++;
    print1("You have attained a new experience level!");
    print2("You are now ");
    nprint2(getarticle(levelname(Player.level)));
    nprint2(levelname(Player.level));
    Player.maxhp += random_range(Player.con)+1;
    Player.mana = Player.maxmana = calcmana();
    morewait();
  }
  if (gained) clearmsg();
  calc_melee();
}

/* experience requirements */
int expval(plevel)
int plevel;
{
  switch(plevel) {
    case 0:return(0);break;
    case 1:return(20);break;
    case 2:return(50);break;
    case 3:return(200);break;
    case 4:return(500);break;
    case 5:return(1000);break;
    case 6:return(2000);break;
    case 7:return(3000);break;
    case 8:return(5000);break;
    case 9:return(7000);break;
    case 10:return(10000);break;
    default:return((plevel-9) * 10000); break;
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

/* kill off player if he isn't got the "breathing" status */
void p_drown()
{
  if (Player.status[BREATHING] > 0)
    mprint("Your breathing is unaffected!");
  else {
    print1("You try to hold your breath....");
    morewait();
    print2("You choke....");
    morewait();
    print3("Your lungs fill...");
    morewait();
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
    default:
    case I_NO_OP: weapon_normal_hit(dmgmod,weapon,m); break;
    case I_ACIDWHIP: weapon_acidwhip(dmgmod,weapon,m); break;
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
    case I_SCYTHE: weapon_scythe(dmgmod,weapon,m); break;
  }
}


/* for printing actions in printactions above */
char *actionlocstr(dir)
char dir;
{
  switch(dir) {
  case 'L': strcpy(Str3,"low."); break;
  case 'C': strcpy(Str3,"center."); break;
  case 'H': strcpy(Str3,"high."); break;
  default: strcpy(Str3,"wildly."); break;
  }
  return(Str3);
}


/* execute player combat actions versus monster m */
void tacplayer(m)
struct monster *m;
{
  int i=0;

  while (i < strlen(Player.meleestr)) {
    if (m->hp > 0) {
      switch(Player.meleestr[i]) {
      case 't': case 'T':
	if (Player.possessions[O_WEAPON_HAND] == NULL) 
	  strcpy(Str1,"You punch ");
	else strcpy(Str1,"You thrust ");
	strcat(Str1,actionlocstr(Player.meleestr[i+1]));
	if (Verbosity == VERBOSE) mprint(Str1);
	if (player_hit(2*statmod(Player.dex),Player.meleestr[i+1],m))
	  weapon_use(0,Player.possessions[O_WEAPON_HAND],m);
	else player_miss(m,NORMAL_DAMAGE);
	break;
      case 'c': case 'C':
	if (Player.possessions[O_WEAPON_HAND] == NULL) 
	  strcpy(Str1,"You punch ");
	else if (Player.possessions[O_WEAPON_HAND]->type == CUTTING) 
	  strcpy(Str1,"You cut ");
	else if (Player.possessions[O_WEAPON_HAND]->type == STRIKING) 
	  strcpy(Str1,"You strike ");
	else strcpy(Str1,"You attack ");
	strcat(Str1,actionlocstr(Player.meleestr[i+1]));
	if (Verbosity == VERBOSE) mprint(Str1);
	if (player_hit(0,Player.meleestr[i+1],m))
	  weapon_use(2*statmod(Player.str),
		     Player.possessions[O_WEAPON_HAND],
		     m);
	else player_miss(m,NORMAL_DAMAGE);
	break;
      case 'l': case 'L':
	strcpy(Str1,"You lunge ");
	strcat(Str1,actionlocstr(Player.meleestr[i+1]));
	if (Verbosity == VERBOSE) mprint(Str1);
	if (player_hit(Player.level+Player.dex,Player.meleestr[i+1],m))
	  weapon_use(Player.level,Player.possessions[O_WEAPON_HAND],m);
	else player_miss(m,NORMAL_DAMAGE);
	break;
      }
    }    
    i+=2;
  }
}




/* checks to see if player hits with hitmod vs. monster m at location hitloc */
int player_hit(hitmod,hitloc,m)
int hitmod;
char hitloc;
struct monster *m;
{
  int i=0,blocks=FALSE,goodblocks=0,hit;
  if (m->hp < 1) {
    mprint("Unfortunately, your opponent is already dead!");
    return(FALSE);
  }
  else {
    if (hitloc == 'X') hitloc = random_loc();

    transcribe_monster_actions(m);

    while (i<strlen(m->meleestr)) {
      if ((m->meleestr[i] == 'B') || (m->meleestr[i] == 'R')) {
	blocks = TRUE;
	if (hitloc == m->meleestr[i+1])
	  goodblocks++;
      }
      i+=2;
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
      if (Verbosity == VERBOSE) mprint(Str1);
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
  setgamestatus(SUPPRESS_PRINTING);
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
    for(i=1;i<MAXITEMS;i++) 
      if (used[i]) {
	Player.possessions[i]->used = TRUE;
	item_use(Player.possessions[i]);
      }
    calc_melee();
    showflags();
    dataprint();
    timeprint();
  }
  resetgamestatus(SUPPRESS_PRINTING);
}


void enter_site(site)
char site;
{
  switch(site) {
  case CITY: change_environment(E_CITY); break;
  case VILLAGE: change_environment(E_VILLAGE); break;
  case CAVES: change_environment(E_CAVES); break;
  case CASTLE: change_environment(E_CASTLE); break;
  case VOLCANO: change_environment(E_VOLCANO); break;
  case TEMPLE: change_environment(E_TEMPLE); break;
  case DRAGONLAIR: change_environment(E_DLAIR); break;
  case STARPEAK: change_environment(E_STARPEAK); break;
  case MAGIC_ISLE: change_environment(E_MAGIC_ISLE); break;
  default:print3("There's nothing to enter here!"); break;
  }
}



/* Switches context dungeon/countryside/city, etc */
void change_environment(new_environment)
char new_environment;
{
  int i,emerging = FALSE;
  pob o;

  Player.sx = -1; Player.sy = -1; /* reset sanctuary if there was one */

  resetgamestatus(FAST_MOVE);

  Last_Environment = Current_Environment;
  if (Last_Environment == E_COUNTRYSIDE) {
    LastCountryLocX = Player.x;
    LastCountryLocY = Player.y;
  }
  if (((Last_Environment == E_CITY) ||
       (Last_Environment == E_VILLAGE)) &&
      ((new_environment == E_MANSION) ||
       (new_environment == E_HOUSE) ||
       (new_environment == E_HOVEL) ||
       (new_environment == E_SEWERS) ||
       (new_environment == E_ARENA))) {
    LastTownLocX = Player.x;
    LastTownLocY = Player.y;
  }
  else if (((Last_Environment == E_MANSION) ||
	    (Last_Environment == E_HOUSE) ||
	    (Last_Environment == E_HOVEL) ||
	    (Last_Environment == E_SEWERS) ||
	    (Last_Environment == E_ARENA)) &&
	   ((new_environment == E_CITY) ||
	    (new_environment == E_VILLAGE))) {
    Player.x = LastTownLocX;
    Player.y = LastTownLocY;
    emerging = TRUE;
  }

  if (Last_Environment == E_ARENA)
    Arena_Victory = Arena_Monster->hp < 1;


  Current_Environment = new_environment;
/*    ScreenOffset = Player.y - (ScreenLength/2); */
  switch(new_environment) {
  case E_ARENA:
    LENGTH = 16;
    WIDTH = 64;
    Player.x = 5;
    Player.y = 7;
    setgamestatus(ARENA_MODE);
    locprint("The Rampart Arena");
    load_arena();
    erase_level();
    ScreenOffset = 0;
    show_screen();
    break;
  case E_ABYSS:
    LENGTH = 16;
    WIDTH = 64;
    Player.x = 32;
    Player.y = 15;
    locprint("The Adept's Challenge");
    load_abyss();
    abyss_file();
    lose_all_items();
    erase_level();
    ScreenOffset = 0;
    show_screen();
    break;
  case E_CIRCLE:
    LENGTH = 16;
    WIDTH = 64;
    Player.x = 32;
    Player.y = 14;
    locprint("The Astral Demesne of the Circle of Sorcerors");
    load_circle();
    if (find_item(&o,ARTIFACTID+21,-1)) {
      print1("A bemused voice says:");
      print2("'Why are you here? You already have the Star Gem!'");
      morewait();
    }
    else if (Player.rank[CIRCLE] > 0) {
      print1("You hear the voice of the Prime Sorceror:");
      print2("'Congratulations on your attainment of the Circle's Demesne.'");
      morewait();
      print1("For the honor of the Circle, you may take the Star Gem");
      print2("and destroy it on the acme of Star Peak.");
      morewait();
      print1("Beware the foul LawBringer who resides there...");
      print2("By the way, some of the members of the Circle seem to");
      morewait();
      print1("have become a bit jealous of your success --");
      print2("I'd watch out for them if I were you.");
      morewait();
    }
    else if (Player.alignment > 0) {
      print1("A mysterious ghostly image materializes in front of you.");
      print2("It speaks: 'Greetings, fellow abider in Law. I am called");
      morewait();
      print1("The LawBringer. If you wish to advance our cause, obtain");
      print2("the mystic Star Gem and return it to me on Star Peak.");
      morewait();
      print1("Beware the power of the evil Circle of Sorcerors and the");
      print2("forces of Chaos which guard the gem.'");
      morewait();
      print1("The strange form fades slowly.");
      morewait();
    }
    erase_level();
    ScreenOffset = 0;
    show_screen();
    break;
  case E_COURT:
    WIDTH = 64;
    LENGTH = 24;
    Player.x = 32;
    Player.y = 2;
    load_court();
    erase_level();
    ScreenOffset = 0;
    show_screen();
    break;
  case E_MANSION:
    WIDTH = 64;
    LENGTH = 16;
    Player.y = 8;
    Player.x = 2;
    load_house(E_MANSION);
    erase_level();
    ScreenOffset = 0;
    show_screen();
    break;
  case E_HOUSE:
    WIDTH = 64;
    LENGTH = 16;
    Player.y = 13;
    Player.x = 2;
    load_house(E_HOUSE);
    erase_level();
    ScreenOffset = 0;
    show_screen();
    break;
  case E_HOVEL:
    WIDTH = 64;
    LENGTH = 16;
    Player.y = 9;
    Player.x = 2;
    load_house(E_HOVEL);
    erase_level();
    ScreenOffset = 0;
    show_screen();
    break;
  case E_DLAIR:
    WIDTH = 64;
    LENGTH = 16;
    Player.y = 9;
    Player.x = 2;
    load_dlair(gamestatusp(KILLED_DRAGONLORD));
    erase_level();
    ScreenOffset = 0;
    show_screen();
    break;
  case E_STARPEAK:
    WIDTH = 64;
    LENGTH = 16;
    Player.y = 9;
    Player.x = 2;
    load_speak(gamestatusp(KILLED_LAWBRINGER));
    erase_level();
    ScreenOffset = 0;
    show_screen();
    break;
  case E_MAGIC_ISLE:
    WIDTH = 64;
    LENGTH = 16;
    Player.y = 15;
    Player.x = 63;
    load_misle(gamestatusp(KILLED_EATER));
    erase_level();
    ScreenOffset = 0;
    show_screen();
    break;
  case E_TEMPLE:
    WIDTH = 64;
    LENGTH = 16;
    load_temple(Country[Player.x][Player.y].aux);
    Player.y = 15;
    Player.x = 32;
    erase_level();
    ScreenOffset = 0;
    show_screen();
    break;
  case E_CITY:
    WIDTH = 64;
    LENGTH = 64;
    if (emerging) {
      print1("You emerge onto the street.");
      emerging = FALSE;
    }
    else {
      print1("You pass through the massive gates of Rampart, the city.");
      Player.x = 62;
      Player.y = 21;
    }
    if (City == NULL) load_city();
    Level = City;
    locprint("The City of Rampart.");
    erase_level();
    ScreenOffset = Player.y - (ScreenLength/2);
    show_screen();
    break;
  case E_VILLAGE:
    WIDTH = 64;
    LENGTH = 16;
    if (emerging) {
      print1("You emerge onto the street.");
      emerging = FALSE;
    }
    else {
      print1("You enter a small rural village.");
      /* different villages per different locations */
      switch(Country[Player.x][Player.y].aux) {
      case 1:
	Player.x = 0;
	Player.y = 6;
	Villagenum = 1;
	break;
      default: 
	print3("Very strange, a nonexistent village.");
      case 2:
	Player.x = 39;
	Player.y = 15;
	Villagenum = 2;
	break;
      case 3:
	Player.x = 63;
	Player.y = 8;
	Villagenum = 3;
	break;
      case 4: 
	Player.x = 32;
	Player.y = 15;
	Villagenum = 4;
	break;
      case 5: 
	Player.x = 2;
	Player.y = 8;
	Villagenum = 5;
	break;
      case 6: 
	Player.x = 2;
	Player.y = 2;
	Villagenum = 6;
	break;
      }
    }
    if ((! emerging) || (TempLevel == NULL)) load_village(Villagenum);
    else if (TempLevel->environment != E_VILLAGE) load_village(Villagenum);
    else Level = TempLevel;
    switch(Villagenum) {
      case 1: locprint("The Village of Star View."); break;
      case 2: locprint("The Village of Woodmere."); break;
      case 3: locprint("The Village of Stormwatch."); break;
      case 4: locprint("The Village of Thaumaris."); break;
      case 5: locprint("The Village of Skorch."); break;
      case 6: locprint("The Village of Whorfen."); break;
      default: locprint("A Deviant Village."); break;
      }
    erase_level();
    ScreenOffset = 0;
    show_screen();
    break;
  case E_CAVES:
    WIDTH = 64;
    LENGTH = 64;
    print1("You enter a dark cleft in a hillside;");
    print2("You note signs of recent passage in the dirt nearby.");
    if (gamestatusp(MOUNTED)) {
      morewait();
      print1("Seeing as you might not be coming back, you feel compelled");
      print2("to let your horse go, rather than keep him hobbled outside.");
      resetgamestatus(MOUNTED);
      calc_melee();
    }
    MaxDungeonLevels = CAVELEVELS;
    if (Current_Dungeon != E_CAVES) {
      free_dungeon();
      Dungeon = NULL;
      Level = NULL;
      Current_Dungeon = E_CAVES;
    }
    change_level(0,1,FALSE);
    break;
  case E_VOLCANO:
    WIDTH = 64;
    LENGTH = 64;
    print1("You pass down through the glowing crater.");
    if (gamestatusp(MOUNTED)) {
      morewait();
      print1("Seeing as you might not be coming back, you feel compelled");
      print2("to let your horse go, rather than keep him hobbled outside.");
      resetgamestatus(MOUNTED);
      calc_melee();
    }
    MaxDungeonLevels = VOLCANOLEVELS;
    if (Current_Dungeon != E_VOLCANO) {
      free_dungeon();
      Dungeon = NULL;
      Level = NULL;
      Current_Dungeon = E_VOLCANO;
    }
    change_level(0,1,FALSE);
    break;
  case E_ASTRAL:
    WIDTH = 64;
    LENGTH = 64;
    print1("You are in a weird flickery maze.");
    if (gamestatusp(MOUNTED)) {
      print2("Your horse doesn't seem to have made it....");
      resetgamestatus(MOUNTED);
      calc_melee();
    }
    MaxDungeonLevels = ASTRALLEVELS;
    if (Current_Dungeon != E_ASTRAL) {
      free_dungeon();
      Dungeon = NULL;
      Level = NULL;
      Current_Dungeon = E_ASTRAL;
    }
    change_level(0,1,FALSE);
    break;
  case E_CASTLE:
    WIDTH = 64;
    LENGTH = 64;
    print1("You cross the drawbridge. Strange forms move beneath the water.");
    if (gamestatusp(MOUNTED)) {
      morewait();
      print1("Seeing as you might not be coming back, you feel compelled");
      print2("to let your horse go, rather than keep him hobbled outside.");
      resetgamestatus(MOUNTED);
    }
    MaxDungeonLevels = CASTLELEVELS;
    if (Current_Dungeon != E_CASTLE) {
      free_dungeon();
      Dungeon = NULL;
      Level = NULL;
      Current_Dungeon = E_CASTLE;
    }
    change_level(0,1,FALSE);
    break;
  case E_SEWERS:
    WIDTH = 64;
    LENGTH = 64;
    print1("You pry open a manhole and descend into the sewers below.");
    if (gamestatusp(MOUNTED)) {
      print2("You horse waits patiently outside the sewer entrance....");
      dismount_steed();
    }
    MaxDungeonLevels = SEWERLEVELS;
    if (Current_Dungeon != E_SEWERS) {
      free_dungeon();
      Dungeon = NULL;
      Level = NULL;
      Current_Dungeon = E_SEWERS;
    }
    change_level(0,1,FALSE);
    break;
  case E_COUNTRYSIDE:
    WIDTH = 64;
    LENGTH = 64;
    print1("You return to the fresh air of the open countryside.");
    if (Last_Environment == E_CITY) {
      Player.x = 27;
      Player.y = 19;
    }
    else {
      Player.x = LastCountryLocX;
      Player.y = LastCountryLocY;
    }
    for(i=0;i<9;i++)
      Country[Player.x+Dirs[0][i]][Player.y+Dirs[1][i]].explored = TRUE;
    erase_level();
    ScreenOffset = Player.y - (ScreenLength/2);
    show_screen();
    break;
  case E_TACTICAL_MAP:
    WIDTH = 64;
    LENGTH = 16;
    print1("You are now on the tactical screen; exit off any side to leave");
    make_country_screen(Country[Player.x][Player.y].current_terrain_type);
    make_country_monsters(Country[Player.x][Player.y].current_terrain_type);
    Player.x = WIDTH/2;
    Player.y = LENGTH/2;
    erase_level();
    ScreenOffset = 0;
    show_screen();
    break;
  case E_NEVER_NEVER_LAND: default:
    print1("There must be some mistake. You don't look like Peter Pan.");
    print2("(But here you are in Never-Never Land)");
    erase_level();
    ScreenOffset = Player.y - (ScreenLength/2);
    show_screen();
    break;
  }
}



