/* omega copyright (C) by Laurence Raphael Brothers, 1987 */

/* oaux1.c */

/* auxiliary functions for those in ocom.c, also see oaux2.c */ 
/* Only functions called in ocom.c should be here */

#include <strings.h>
#include "oglob.h"

/* from oitem */
extern pob create_object();

/* from ospell */
extern char *spellid();

/* from ogen */
extern char *roomname();

/* from oeffect 1 or 2 */
extern void manastorm();

/* from oitemf */
extern void weapon_normal_hit(),weapon_demonblade(),weapon_lightsabre();
extern void weapon_mace_disrupt(),weapon_bare_hands();
extern void weapon_vorpal(),weapon_desecrate(),weapon_firestar();
extern void weapon_defend(),weapon_victrix(),weapon_tangle();
extern void weapon_arrow(),weapon_bolt();

/* from oitem */
extern void item_use();

/* from oinv */
extern void pickup_at(),drop_at(),givemonster(),conform_lost_objects();
extern void add_item_to_pack();
extern char *itemid();
extern int getitem(),badobject(),cursed();

/* from outil */
extern int random_range();
extern int inbounds(),confirmation();
extern char inversedir();
extern char *getarticle();
extern int unblocked(),bitp();
extern void bitset(),bitreset();

/* from oetc */
extern char *wordnum();

/* from oscr */
extern void showcursor(),refreshmsg(),drawspot(),describe_player();
extern void mprint(),endgraf(),showflags(),dataprint(),menuaddch();
extern void locprint(),xredraw(),putspot(),blankoutspot(),levelrefresh();
extern void drawvision(),display_death(),morewait(),menumorewait(),printm();
extern void menuclear(),menuprint(),display_win(),mnumprint(),menunumprint();
extern void screencheck();
extern char mgetc(),lgetc(),ynq(),getspot();


/* from ocom */
void p_process();

/* from omon */
extern void m_status_set(),m_status_reset(),m_death(),m_pickup();
extern void tacmonster(),m_teleport(),m_dropstuff(),m_vanish();
extern int m_statusp();

/* from oaux2 */
extern char *actionlocstr();
extern void break_weapon(),drop_weapon(),gain_experience(),gain_level();
extern void p_fumble(),p_win(),p_hit(),p_miss(),addring();
extern void top_level(),printactions(),movecursor(),p_drown();
extern void tacplayer(),weapon_use(),move_status_check(),hour_status_check();
extern int expval(),player_hit(),statmod(),item_value(),true_item_value();
extern int p_immune();

/* oaux1 functions */
void pickup_at(),setspot(),foodcheck(),p_death(),hourly_check();
void calc_melee(),p_melee(),gain_experience(),p_damage();
void p_process(),p_teleport(),tacplayer(),roomcheck(),tunnelcheck();
void searchat(),showroom(),weapon_use(),damage_item(),tacmode();
void hand_wield(),optionset(),optionreset(),describe_player();
void lightroom(),surrender(),threaten(),darkenroom();
char *itemid(),*mstatus_string(),*trapid();
int p_moveable(),cursed(),getitem(),getdir(),optionp(),goberserk();
int player_on_sanctuary();


/* check to see if too much tunneling has been done in this level */
void tunnelcheck()
{
  Leveldata[Dlevel].tunnelled++;
  if ((Leveldata[Dlevel].tunnelled) > WIDTH/4)
    mprint("Dust and stone fragments fall on you from overhead.");
  if ((Leveldata[Dlevel].tunnelled) > WIDTH/2)
    mprint("You hear groaning and creaking noises.");
  if ((Leveldata[Dlevel].tunnelled) > 3*WIDTH/4) 
    mprint("The floor trembles and you hear a loud grinding screech.");
  if ((Leveldata[Dlevel].tunnelled) > WIDTH) {
    mprint("With a scream of tortured stone, the entire level caves in!!!");
    gain_experience(5000);
    mprint("You are flattened into an unpleasant jellylike substance.");
    p_death("dungeon cave-in");
  }
}

/* displays a room's name */
void showroom(i)
int i;
{
  if (Dlevel == 0) locprint("The City of Rampart.");
  else {
    strcpy(Str1,"Level ");
    if (Dlevel < 10) {
      Str1[6] = Dlevel + '0';
      Str1[7] = 0;
    }
    else {
      Str1[6] = ((int) Dlevel / 10) + '0';
      Str1[7] = (Dlevel % 10) + '0';
      Str1[8] = 0;
    }
    strcat(Str1," -- ");
    strcat(Str1,roomname(i));
    locprint(Str1);
  }
}


int player_on_sanctuary()
{
  if ((Player.slevel==Dlevel) &&
      (Player.x==Player.sx) &&
      (Player.y==Player.sy))
    return(TRUE);
  else {
    if (Player.patron) {
      if ((Dungeon[Dlevel][Player.x][Player.y].locchar == ALTAR) &&
	  (Dungeon[Dlevel][Player.x][Player.y].aux == Player.patron))
	return(TRUE);
      else return(FALSE);
    }
    else return(FALSE);
  }
}


/* check a move attempt, maybe attack something, return TRUE if ok to move. */
/* x y is the proposed place to move to */
 int p_moveable(x,y)
 int x,y;
 {
   Skipmonsters = TRUE;
   if (! inbounds(x,y)) return (FALSE);
   else if (Player.status[SHADOWFORM]) {
     Skipmonsters = FALSE;
     return(TRUE);
   }
   else if (Dungeon[Dlevel][x][y].secret) {
     if (!Fastmove) mprint("Ouch!");
     return(FALSE);
   }
   else if (Dungeon[Dlevel][x][y].creature != NULL) {
     if ((! Fastmove) || optionp(BELLICOSE)) {
	 p_melee(Dungeon[Dlevel][x][y].creature);
	 Skipmonsters = FALSE;
	 return(FALSE);
       }
     else return(FALSE);
   }
   else if ((Dungeon[Dlevel][x][y].locchar == WALL) ||
	    (Dungeon[Dlevel][x][y].locchar == STATUE) ||
	    (Dungeon[Dlevel][x][y].locchar == CLOSED_DOOR) ||
	    (Fastmove &&
	     ((Dungeon[Dlevel][x][y].locchar == HEDGE) ||
	      (Dungeon[Dlevel][x][y].locchar == LAVA) ||
	      (Dungeon[Dlevel][x][y].locchar == ABYSS) ||
	      (Dungeon[Dlevel][x][y].locchar == FIRE) ||
	      (Dungeon[Dlevel][x][y].locchar == WATER) ||
	      (Dungeon[Dlevel][x][y].locchar == WHIRLWIND) ||
	      (Dungeon[Dlevel][x][y].locchar == LIFT) ||
	      (Dungeon[Dlevel][x][y].locchar == TRAP)))) {
     if (! Fastmove) mprint("Ouch!");
     return(FALSE);
   }
   else if (optionp(CONFIRM)) {
     if ((Dungeon[Dlevel][x][y].locchar == HEDGE) ||
	 (Dungeon[Dlevel][x][y].locchar == LAVA) ||
	 (Dungeon[Dlevel][x][y].locchar == FIRE) ||
	 (Dungeon[Dlevel][x][y].locchar == ABYSS) ||
	 (Dungeon[Dlevel][x][y].locchar == WATER) ||
	 (Dungeon[Dlevel][x][y].locchar == WHIRLWIND) ||
	 (Dungeon[Dlevel][x][y].locchar == LIFT) ||
	 (Dungeon[Dlevel][x][y].locchar == TRAP)) {
       return(! (Skipmonsters = ! confirmation()) );
     }
     else {
       Skipmonsters = FALSE;
       return(TRUE);
     }
   }
   else {
     Skipmonsters = FALSE;
     return(TRUE);
   }
 }


/* search once particular spot */
void searchat(x,y)
int x,y;
{
  int i;
  if (inbounds(x,y) && (random_range(3) || Player.status[ALERT])) {
    if (Dungeon[Dlevel][x][y].secret) {
      Dungeon[Dlevel][x][y].secret = FALSE;
      if ((Dungeon[Dlevel][x][y].locchar==OPEN_DOOR) ||
	  (Dungeon[Dlevel][x][y].locchar==CLOSED_DOOR))
	mprint("You find a secret door!");
      else mprint("You find a secret passage!");
      for(i=0;i<8;i++)
	Dungeon[Dlevel][x+Dirs[0][i]][y+Dirs[1][i]].stopsrun = TRUE;
      drawvision(Player.x,Player.y,Player.vision);
    }
    if ((Dungeon[Dlevel][x][y].p_locf >= TRAP_BASE) &&
	(Dungeon[Dlevel][x][y].locchar != TRAP) &&
	(Dungeon[Dlevel][x][y].p_locf <= TRAP_BASE+NUMTRAPS)) {
      Dungeon[Dlevel][x][y].locchar = TRAP;
      mprint("You find a trap!");
      if (Fastmove == TRUE) {
	drawvision(Player.x,Player.y,Player.vision);
	Fastmove = FALSE;
      }
    }
  }
}

/* This is called whenever anything might change player performance in
   melee, such as changing weapon, statistics, etc. */
void calc_melee()
{
  Player.maxweight = (Player.str * Player.agi * 10);
  Player.absorption = Player.status[PROTECTION];
  Player.defense = 2 * statmod(Player.agi)+(Player.level/2);
  Player.hit = Player.level + statmod(Player.dex)+1;
  Player.dmg = statmod(Player.str)+3;
  Player.speed = 5 - min(4,(statmod(Player.agi)/2));
  if (Player.status[HASTED]) Player.speed = Player.speed / 2;
  if (Player.status[SLOWED]) Player.speed = Player.speed * 2;
  if (Player.itemweight > 0)
    switch(Player.maxweight / Player.itemweight) {
    case 0: Player.speed+=6; break;
    case 1: Player.speed+=3; break;
    case 2: Player.speed+=2; break;
    case 3: Player.speed+=1; break;
    }
  
  if (Player.status[ACCURATE]) Player.hit+=20;
  if (Player.status[HERO]) Player.hit+=Player.dex;
  if (Player.status[HERO]) Player.dmg+=Player.str;
  if (Player.status[HERO]) Player.defense+=Player.agi;
  if (Player.status[HERO]) Player.speed=Player.speed / 2;

  Player.speed = max(1,min(25,Player.speed));

  /* weapon */
  if (Player.primary != NULL) {
    Player.hit += Player.primary->hit + Player.primary->plus;
    Player.dmg += Player.primary->dmg + Player.primary->plus;
  }

  /* shield */
  if (Player.secondary != NULL) 
    Player.defense += Player.secondary->aux + Player.secondary->plus;

  /* armor */
  if (Player.armor != NULL) {
    Player.absorption += Player.armor->dmg;
    Player.defense = Player.defense + Player.armor->plus - Player.armor->aux;
  }
  comwinprint();
}
	

/* player attacks monster m */
void p_melee(m)
struct monster *m;
{
  int hitmod = 0;
  if (Lunarity == 1) hitmod += Player.level;
  else if (Lunarity == -1) hitmod -= (Player.level / 2);
  if (Player.status[AFRAID])
    mprint("You are much too afraid to fight!");
  else if (player_on_sanctuary())
    mprint("You restrain yourself from descrating this holy place.");
  else if (Player.status[SHADOWFORM])
    mprint("Your attack has no effect in your shadowy state.");
  else {
    if (! m->attacked) Player.alignment--; /* chaotic action */
    m_status_set(m,AWAKE);
    m_status_set(m,HOSTILE);
    m->attacked = TRUE;
    if (hitp(Player.hit+hitmod,m->ac))
      weapon_use(0,Player.primary,m);
    else p_miss(m,NORMAL_DAMAGE);
  }
}

/* Attempt to break an object o */
void damage_item(o)
pob o;     
{
  int i;
  if (o->objchar == STICK) {
    strcpy(Str1,"Your ");
    strcat(Str1,(o->blessing >= 0 ? o->truename : o->cursestr));
    strcat(Str1," explodes!");
    mprint(Str1);
    morewait();
    if (o->charge < 1)
      mprint("Fzzz... Out of Power... Oh well...");
    else {
      mprint("Ka-Blamm!!!");
      /* general case. Some sticks will eventually do special things */
      for(i=0;i<9;i++)
	manastorm(Player.x+Dirs[0][i],
		  Player.y+Dirs[1][i],
		  o->charge*o->level*10);
      conform_lost_objects(1,o);
    }
  }
  else if (o->blessing > 0) {
    strcpy(Str1,"Your ");
    strcat(Str1,itemid(o));
    strcat(Str1," glows strongly.");
    mprint(Str1);
  }
  else if (o->blessing < -1) {
    strcpy(Str1,"You hear an evil giggle from your ");
    strcat(Str1,itemid(o));
    mprint(Str1);
  }
  else if (o->plus > 0) {
    strcpy(Str1,"Your ");
    strcat(Str1,itemid(o));
    strcat(Str1," glows and then fades.");
    mprint(Str1);
    o->plus--;
  }
  else {
    strcpy(Str1,"Your ");
    strcat(Str1,itemid(o));
    strcat(Str1," shatters in a thousand lost fragments!");
    conform_lost_objects(1,o);
    mprint(Str1);
  }
}





/* do dmg points of damage of type dtype, from source fromstring */
void p_damage(dmg,dtype,fromstring)
int dmg,dtype;
char *fromstring;
{
  if (! p_immune(dtype)) {
    if (Fastmove == TRUE) {
      drawvision(Player.x,Player.y,Player.vision);
      Fastmove = FALSE;
    }
    if (dtype == NORMAL_DAMAGE) Player.hp -= max(1,(dmg-Player.absorption));
    else Player.hp -= dmg;
    if (Player.hp < 1) p_death(fromstring);
  }
  else mprint("You resist the effects!");
}

/* game over, you lose! */
void p_death(fromstring)
char *fromstring;
{
  Player.hp = -1;
  mprint("You died!");
  morewait();
  display_death(fromstring);
  endgraf();
  exit(0);
}


/* move the cursor around, like for firing a wand, sets x and y to target */
void setspot(x,y)
int *x,*y;
{
  char c = ' ';
  mprint("Targeting.... ? for help");
  showcursor(*x,*y);
  while ((c != '.') && (c != ESCAPE)) {
    c = lgetc();
    switch(c) {
      case 'h':case '4': movecursor(x,y,-1,0);  break;
      case 'j':case '2': movecursor(x,y,0,1);  break;
      case 'k':case '8': movecursor(x,y,0,-1);  break;
      case 'l':case '6': movecursor(x,y,1,0);  break;
      case 'b':case '1': movecursor(x,y,-1,1);  break;
      case 'n':case '3': movecursor(x,y,1,1);  break;
      case 'y':case '7': movecursor(x,y,-1,-1);  break;
      case 'u':case '9': movecursor(x,y,1,-1);  break;
      case '?':
	menuclear();
	menuprint("Use vi keys or numeric keypad\n");
        menuprint("to move cursor to target.\n");
	menuprint("Hit the '.' key when done,\n");
	menuprint("or ESCAPE to abort.");
	menumorewait();
	xredraw();
	break;
    }
  }
  if (c==ESCAPE) *x = *y= ABORT;
  screencheck(Player.y);
}


/* get a direction: return index into Dirs array corresponding to direction */
int getdir()
{
  mprint("Select direction [hjklyubn, ESCAPE to quit]: ");
  switch (mgetc()) {
  case '4':
  case 'h':
  case 'H': return(5); break;
  case '2':
  case 'j':
  case 'J': return(6); break;
  case '8':
  case 'k':
  case 'K': return(7); break;
  case '6':
  case 'l':
  case 'L': return(4); break;
  case '7':
  case 'y':
  case 'Y': return(3); break;
  case '9':
  case 'u':
  case 'U': return(1); break;
  case '1':
  case 'b':
  case 'B': return(2); break;
  case '3':
  case 'n':
  case 'N': return(0); break;
  case ESCAPE: return(ABORT); break;
  default: mprint("That's not a direction! "); return(getdir()); break;
  }
}




/* bitwise access functions to Player.options */

int optionp(o)
int o;
{
  return(bitp(Player.options,o));
}

void optionset(o)
int o;
{
  bitset(&(Player.options),o);
}

void optionreset(o)
int o;
{
  bitreset(&(Player.options),o);
}



/* functions describes monster m's state for examine function */      
char *mstatus_string(m)
struct monster *m;
{
  if (m->uniqueness == COMMON) {
    if (m->hp < Monsters[m->id].hp / 3)
      strcpy(Str2,"a grievously injured");
    else if (m->hp < Monsters[m->id].hp / 2)
      strcpy(Str2,"a severely injured");
    else if (m->hp < Monsters[m->id].hp)
      strcpy(Str2,"an injured ");
    else strcpy(Str2,getarticle(m->monstring));
    if (m->level > Monsters[m->id].level) {
      strcat(Str2," (level ");
      strcat(Str2,wordnum(m->level+1-Monsters[m->id].level));
      strcat(Str2,") ");
    }
    strcat(Str2,m->monstring);
  }
  else {
    strcpy(Str2,m->monstring);
    if (m->hp < Monsters[m->id].hp / 3)
      strcat(Str2,"who is grievously injured");
    else if (m->hp < Monsters[m->id].hp / 2)
      strcat(Str2,"who is severely injured");
    else if (m->hp < Monsters[m->id].hp)
      strcat(Str2,"who is injured ");
  }
  return(Str2);
}




/* for the examine function */
void describe_player()
{
  if (Player.hp < (Player.maxhp /5))
    menuprint("\nA grievously injured ");
  else if (Player.hp < (Player.maxhp /2))
    menuprint("\nA seriously wounded ");
  else if (Player.hp < Player.maxhp)
    menuprint("\n somewhat bruised ");
  else menuprint("\nA fit ");

  if (Player.level == 0)
    menuprint("\nbeginning adventurer named\n");
  else if (Player.level < 4)
    menuprint("\nnovice adventurer named\n");
  else if (Player.level < 8)
    menuprint("\nexperienced adventurer named\n");
  else if (Player.level < 16)
    menuprint("\nmaster adventurer named\n");
  else menuprint("\ngodlike adventurer named\n");
  menuprint(Player.name);
}


/* access to player experience... */
void gain_experience(amount)
int amount;
{
  Player.xp += amount;
  gain_level();
}


/* try to hit a monster in an adjacent space. If there are none
   return FALSE */
int goberserk()
{
  int wentberserk=FALSE,i;

  for(i=0;i<8;i++)
    if (Dungeon[Dlevel][Player.x+Dirs[0][i]][Player.y+Dirs[1][i]].creature
	!= NULL) {
	  wentberserk=TRUE;
	  p_melee(Dungeon
		  [Dlevel]
		  [Player.x+Dirs[0][i]]
		  [Player.y+Dirs[1][i]].creature);
	}

  return(wentberserk);
}

/* identifies a trap for examine() by its aux value */
char *trapid(trapno)
int trapno;
{
  switch (trapno) {
    case L_TRAP_DART:return("A dart trap");
    case L_TRAP_PIT:return("A pit");
    case L_TRAP_SNARE:return("A snare");
    case L_TRAP_BLADE:return("A blade trap");
    case L_TRAP_FIRE:return("A fire trap");
    case L_TRAP_TELEPORT:return("A teleport trap");
    case L_TRAP_DISINTEGRATE:return("A disintegration trap");
    case L_TRAP_DOOR:return("A trap door");
    case L_TRAP_MANADRAIN:return("A manadrain trap");
    case L_TRAP_ACID:return("An acid shower trap");
    case L_TRAP_SLEEP_GAS:return("A sleep gas trap");
    case L_TRAP_ABYSS:return("A concealed entrance to the abyss");
  }
}


/* checks current food status of player, every hour, and when food is eaten */
void foodcheck()
{
  if (Player.food > 24) {
    mprint("You vomit up your huge meal.");
    Player.food = 4;
  }
  else if (Player.food == 3) mprint("You feel hungry.");
  else if (Player.food == 1) {
    mprint("You feel weak.");
    if (Fastmove == TRUE) {
      drawvision(Player.x,Player.y,Player.vision);
      Fastmove = FALSE;
    }
  }
  else if (Player.food < 0) {
    if (Fastmove == TRUE) {
      drawvision(Player.x,Player.y,Player.vision);
      Fastmove = FALSE;
    }
    mprint("You're starving!");
    p_damage(-5*Player.food,UNSTOPPABLE,"starvation");
  }
  showflags();
}


/* tactical fighting mode, top-level routine */
void tacplayer(p_actions,pnumactions,m_actions,mnumactions,m)
int p_actions[10],*pnumactions,m_actions[10],mnumactions;
struct monster *m;
{
  int i,actionsleft,index,maneuvers,done;
  float times;

  times = (Player.status[HASTED] ? 2.0 : 1.0);
  times *= (Player.status[SLOWED] ? 0.5 : 1.0);

  done = FALSE;
  *pnumactions = 0;
  maneuvers = 2 + Player.level/10;
  if (Player.rank[ARENA]) maneuvers++;
  maneuvers = ((int) (maneuvers*times));
  maneuvers = min(8,max(1,maneuvers));
  actionsleft = maneuvers;
  printm("\n");
  do {
    menuclear();
    if (Arena) 
      menuprint("Arena Combat: [abgwlrd,BACKSPACE/DELETE,CR/ESC,?]");
    else if (Final)
      menuprint("Final Duel: [abgwlrm,BACKSPACE/DELETE,CR/ESC,?]");
    else 
      menuprint("Tactical Combat: [abgwlrmd,BACKSPACE/DELETE,CR/ESC,?]");
    printactions(*pnumactions,p_actions);
    menuprint("\n");
    menunumprint(actionsleft);
    menuprint(" Maneuvers left.\n");
    switch(mgetc()) {
    case '?':
      if (Arena)
	menuprint("ARENA COMBAT\n\n");
      else if (Final) menuprint("THE FINAL DUEL\n\n");
      else menuprint("TACTICAL COMBAT\n\n");
      menuprint("\nOpponent: ");
      menuprint(mstatus_string(m));
      menuprint("\n\nSelect maneuvers:");
      menuprint("\n[a] attack (1 maneuver)");
      menuprint("\n[b] block (1 maneuver)");
      menuprint("\n[g] pick up (all maneuvers)");
      menuprint("\n[w] wield (all maneuvers)");
      menuprint("\n[l] lunge (2 maneuvers)");
      menuprint("\n[r] riposte (2 maneuvers)");
      if (! Arena) 
	menuprint("\n[m] magic (all maneuvers)");
      if (! Final) menuprint("\n[d] disengage (all maneuvers)");
      menuprint("\nBACKSPACE/DELETE clears choices");
      menuprint("\nESCAPE/RETURN when done");
      menuprint("\n? for this list");
      menumorewait();
      break;
    case 'a':
      if (actionsleft>0) {
	if (Player.status[AFRAID])
	  menuprint("You are too afraid!");
	else {
	  if (Player.primary == NULL) p_actions[*pnumactions] = CUT;
	  else if ((Player.primary->type == THRUSTING) || 
		   (Player.primary->type == MISSILE))
	    p_actions[*pnumactions] = THRUST;
	  else p_actions[*pnumactions] = CUT;
	  menuprint("\nAttack");
	  p_actions[*pnumactions]+=getlocation();
	  (*pnumactions)++;
	  actionsleft--;
	}
      }
      else {
	menuprint("\nNo more maneuvers!");
	morewait();
      }
      break;
    case 'b':
      if (actionsleft>0) {
	p_actions[*pnumactions] = BLOCK;
	menuprint("\nBlock ");
	p_actions[*pnumactions]+= getlocation();
	(*pnumactions)++;
	actionsleft--;
      }
      else {
	menuprint("\nNo more maneuvers!.");
	morewait();
      }
      break;
    case 'l':
      if (actionsleft>1) {
	if (Player.status[AFRAID]) {
	  menuprint("You are too afraid!");
	  morewait();
	}
	else if (Player.primary != NULL) {
	  if (Player.primary->type != MISSILE) {
	    p_actions[*pnumactions] = LUNGE;
	    p_actions[*pnumactions]+=getlocation();
	    (*pnumactions)++;
	    actionsleft -= 2;
	    menuprint("\nLunge ");
	  }
	  else {
	    menuprint("\nNot with a missile weapon!");
	    morewait();
	  }
	}
	else {
	  menuprint("\Not without a weapon!");
	  morewait();
	}
      }
      else {
	menuprint("\nNo more maneuvers!");
	morewait();
      }
      break;
    case 'r':
      if (actionsleft>1) {
	if (Player.status[AFRAID]) {
	  menuprint("You are too afraid!");
	  morewait();
	}
	else if (Player.primary != NULL) {
	  if (Player.primary->type == THRUSTING) {
	    p_actions[*pnumactions] = RIPOSTE;
	    menuprint("\nRiposte ");
	    p_actions[*pnumactions]+=getlocation();
	    (*pnumactions)++;
	    actionsleft -= 2;
	  }
	  else {
	    menuprint("\nNot without a thrusting weapon!");
	    morewait();
	  }
	}
	else {
	  menuprint("\nNot without a weapon!");
	  morewait();
	}
      }
      else {
	menuprint("\nNo more maneuvers!!");
	morewait();
      }
      break;
    case 'm':
      if (Arena) mprint("Magic is not allowed in the Arena.");
      else if (actionsleft == maneuvers) {
	if (Player.status[AFRAID]) {
	  menuprint("\nYou are too afraid!");
	  morewait();
	}
	else {
	  index = getspell();
	  if (index == ABORT)
	    Skipmonsters = TRUE;
	  else if (Spells[index].powerdrain > Player.mana) {
	    menuprint("\nYou lack the power! ");
	    morewait();
	  }
	  else {
	    Player.mana -= Spells[index].powerdrain;
	    p_actions[0] = MAGIC;
	    p_actions[1] = index;
	    *pnumactions = 1;
	    actionsleft = 0;
	  }
	  xredraw();
	}
      }
      else {
	menuprint("No more maneuvers!");
	morewait();
      }
      break;
    case 'g':
      if (Dungeon[Dlevel][Player.x][Player.y].things == NULL) {
	menuprint("Nothing to pick up!");
	morewait();
      }
      else if (actionsleft == maneuvers) {
	p_actions[0] = PICK_UP;
	*pnumactions = 1;
	actionsleft = 0;
      }
      else {
	menuprint("No more maneuvers!");
	morewait();
      }
      break;
    case 'w':
      if (actionsleft == maneuvers) {
	p_actions[0] = WIELD;
	*pnumactions = 1;
	actionsleft = 0;
      }
      else {
	menuprint("No more maneuvers!");
	morewait();
      }
      break;
    case 'd':
      if (Final) {
	menuprint("There's nowhere to run....");
	morewait();
      }
      else if (actionsleft == maneuvers) {
	p_actions[0] = DISENGAGE;
	*pnumactions = 1;
	actionsleft = 0;
      }
      else {
	menuprint("No more manuevers!");
	morewait();
      }
      break;
    case BACKSPACE:
    case DELETE:
      *pnumactions=0;
      actionsleft=maneuvers;
      break;
    case RETURN:
    case LINEFEED:
    case ESCAPE:
      done = TRUE;
      break;
    }
  } while (! done);
  menuclear();
  dataprint();
}

void tacmode(m)
struct monster *m;
{
  int p_actions[10],pnumactions,m_actions[10],mnumactions;
  int px=Player.x,py=Player.y,mx=m->x,my=m->y,level=Dlevel;
  Tacmode = TRUE;
  m_status_set(m,AWAKE);
  m_status_set(m,HOSTILE);
  m->attacked = TRUE;
  while (Tacmode) {
    tacplayer(p_actions,&pnumactions,m_actions,mnumactions,m);
    morewait();
    tacmonster(p_actions,pnumactions,m_actions,&mnumactions,m);
    tacexecute(p_actions,pnumactions,m_actions,mnumactions,m);
    if ((px!=Player.x) ||
	(py!=Player.y) ||
	(mx!=m->x) ||
	(my!=m->y) ||
	(level != Dlevel) ||
	(Player.hp < 1) ||
	(m->hp < 1))
      Tacmode = FALSE;
  }
  if ((px!=Player.x) ||
      (py!=Player.y) ||
      (mx!=m->x) ||
      (level != Dlevel) ||
      (my!=m->y))
    mprint("Close combat broken off by disengagement.");
}

void roomcheck()
{
  static int oldroom = -1;
  int roomno;
  
  if (oldroom != 
      (roomno = Dungeon[Dlevel][Player.x][Player.y].roomnumber)) {
    if (oldroom >= ROOMBASE) darkenroom(oldroom);
    if (roomno >= ROOMBASE) {
      lightroom(roomno);
      showroom(Leveldata[Dlevel].rooms[roomno-ROOMBASE].rsi);
    }
    else showroom(roomno);
  }
  oldroom = roomno;
}


/* illuminate room when entered */
void lightroom(roomno)
int roomno;
{
  int i,j;
  struct level_data *l;
  struct room *r;
  char c;

  l = &(Leveldata[Dlevel]);

  r = &(l->rooms[roomno-ROOMBASE]);

  if (roomno > -1) {
    if (Fastmove == TRUE) {
      drawvision(Player.x,Player.y,Player.vision);
      Fastmove = FALSE;
    }
    if ((r->lighted) || Player.status[ILLUMINATION])
      for(i=r->left;i<=r->right;i++)
	for(j=r->top;j<=r->bottom;j++) {
	  c = getspot(i,j,FALSE);
	  Dungeon[Dlevel][i][j].seen = TRUE;
	  Dungeon[Dlevel][i][j].showchar = c;
	  putspot(i,j,c);
	}
  }
}

void darkenroom(roomno)
int roomno;
{
  int i,j;
  struct room *r;

  if (roomno >= ROOMBASE) {
    r = &(Leveldata[Dlevel].rooms[roomno-ROOMBASE]);
      for(i=r->left-1;i<=r->right+1;i++)
	for(j=r->top-1;j<=r->bottom+1;j++) 
	  blankoutspot(i,j);
  }
  levelrefresh();
}



/* ask for mercy */
void surrender(m)
  struct monster *m;
{
  int i,bestitem,bestvalue;
  switch(random_range(4)) {
  case 0: printm("\nYou grovel at the monster's feet..."); break;
  case 1: printm("\nYou cry 'uncle'!");
  case 2: printm("\nYou beg for mercy.");
  case 3: printm("\nYou yield to the monster.");
  }
  if ((m->talkf==M_NO_OP) ||
      (m->talkf==M_TALK_STUPID))
    mprint("Your plea is ignored.");
  else  {
    mprint("Your surrender is accepted.");
    mprint("All your gold is taken....");
    Player.cash = 0;
    bestvalue = 0;
    bestitem = ABORT;
    for (i=0;i<MAXITEMS;i++)
      if (Player.possessions[i] != NULL)
	if (bestvalue < true_item_value(Player.possessions[i]))
	  bestitem = i;
    if (bestitem != ABORT) {
      mprint("You also give away your best item....");
      mprint(itemid(Player.possessions[bestitem]));
      givemonster(m,Player.possessions[bestitem]);
    }
    mprint("You feel less experienced...");
    Player.xp = max(0,Player.xp - m->xpv);
    mprint("The monster seems more experienced!");
    m->level = (min(10,m->level+1));
    m->hp += m->level*20;
    m->hit += m->level;
    m->dmg += m->level;
    m->ac += m->level;
    m->xpv += m->level*10;
    m->actions++;
    if ((m->talkf == M_TALK_EVIL) && random_range(10)) {
      mprint("It continues to attack you, laughing evilly!");
      m_status_set(m,HOSTILE);
      m_status_reset(m,GREEDY);
    }
    else {
      mprint("The monster leaves, chuckling to itself....");
      m_teleport(m);
    }
  }
}


/* threaten a monster */
void threaten(m)
struct monster *m;
{
  char response;
  switch(random_range(4)) {
  case 0:mprint("You demand that your opponent surrender!"); break;
  case 1:mprint("You threaten to do bodily harm to it."); break;
  case 2:mprint("You attempt to bluster it into submission."); break;
  case 3:mprint("You try to cow it with your awesome presence."); break;
  }
  if ((m->talkf == M_NO_OP) || (m->talkf == M_TALK_STUPID))
    mprint("Your demand is ignored!");
  else if (! m_statusp(m,HOSTILE)) {
    mprint("You only annoy it with your futile demand.");
    m_status_set(m,HOSTILE);
  }
  else if (m->level*3 > Player.level)
    mprint("It sneers contemptuously at you.");
  else if ((m->level*2 < Player.level) || (m->hp < Player.dmg)) {
    mprint("It yields to your mercy.");
    Player.alignment++;
    mprint("Kill it, rob it, or free it? [krf] ");
    do response = mgetc();
    while ((response != 'k')&&(response != 'r')&&(response !='f'));
    if (response == 'k') {
      m_death(m);
      mprint("You treacherous rogue!");
      Player.alignment -= 10;
    }
    else if (response == 'r') {
      mprint("It drops its treasure and flees.");
      m_dropstuff(m);
      m_vanish(m);
    }
    else {
      Player.alignment+=5;
      mprint("'If you love something set it free ... '");
      if (random_range(100)==13)
	mprint("'...If it doesn't come back, hunt it down and kill it.'");
      mprint("It departs with a renewed sense of its own mortality.");
      m_status_reset(m,HOSTILE);
      m_teleport(m);
    }
  }
}
  



/* Various things that get looked at every hour. Food/hunger status,
   random events, hourly status checks, wandering monster generation,
   player healing */
void hourly_check()
{
  int i;
  pol ol;
  pml ml;

  Player.food--;
  foodcheck();
  wandercheck();
  hour_status_check();
  if ((Player.status[DISEASED] == 0) && (Player.hp < Player.maxhp))
    Player.hp = min(Player.maxhp,Player.hp+Player.level+1);

  /* random events, just general weirdness for the heck of it */
  switch(random_range(1000)) {
    case 0: 
      mprint("You feel an unexplainable elation.");
      morewait();
      break;
    case 1:
      mprint("You hear a distant rumbling.");
      morewait();
      break;
    case 2:
      mprint("You realize your fly is open.");
      morewait();
      break;
    case 3:
      mprint("You have a sudden craving for a pecan twirl.");
      morewait();
      break;
    case 4:
      mprint("A mysterious healing flux settles over the level.");
      morewait();
      for (ml=Mlist[Dlevel];ml!=NULL;ml=ml->next)
	ml->m->hp = Monsters[ml->m->id].hp;
      Player.hp = max(Player.hp,Player.maxhp);
      break;
    case 5:
      mprint("You discover an itch just where you can't scratch it.");
      morewait();
      break;
    case 6:
      mprint("A cosmic ray strikes!");
      p_damage(10,UNSTOPPABLE,"a cosmic ray");
      morewait();
      break;
    case 7:
      mprint("You catch your second wind....");
      Player.hp = ++Player.maxhp;
      Player.mana = max(Player.mana, calcmana());
      morewait();
      break;
    case 8:
      mprint("You find some spare change in a hidden pocket.");
      Player.cash += Player.level*Player.level+1;
      morewait();
      break;
    case 9:
      if (Player.primary != NULL) {
	mprint("The warranty runs out on your weapon...");
	damage_item(Player.primary);
      }
      else mprint("You feel strangely lucky.");
      morewait();
      break;
    case 10:
      if (Dungeon[Dlevel][Player.x][Player.y].locchar == FLOOR) {
	Dungeon[Dlevel][Player.x][Player.y].locchar = TRAP;
	Dungeon[Dlevel][Player.x][Player.y].p_locf = L_TRAP_PIT;
	mprint("You fall into a pit -- filled with treasure!");
	morewait();
	for (i=0;i<5;i++) {
	  ol = ((pol) calloc(1,sizeof(oltype)));
	  ol->thing = create_object();
	  ol->next = Dungeon[Dlevel][Player.x][Player.y].things;
	  Dungeon[Dlevel][Player.x][Player.y].things = ol;
	}
	pickup();
      }
      else mprint("You feel like fortune has passed you by....");
      break;
    case 11: 
      mprint("A mysterious voice echoes all around you....");
      hint();
      morewait();
      break;
    case 12:
      if (Balance > 0) {
	mprint("You get word of the failure of your bank!");
	Balance = 0;
      }
      else mprint("You feel lucky.");
      break;
    case 13:
      if (Balance > 0) {
	mprint("You get word of a bank error in your favor!");
	Balance += 5000;
      }
      else mprint("You feel unlucky.");
      break;
    }
  showflags();
}


