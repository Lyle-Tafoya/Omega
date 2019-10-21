/* omega copyright (C) by Laurence Raphael Brothers, 1987,1988 */
/* otrap.c */
/* trap movement functions */

#include "oglob.h"


/* various traps */

/* note special function on different levels */
void l_trap_siren()
{
  pml ml;
  Level->site[Player.x][Player.y].locchar = TRAP;
  print1("A klaxon goes off!");
  print2("'Intruder Alert -- Intruder Alert -- Intruder Alert'");
  print3("You have the feeling you have been discovered....");
  morewait();
  clearmsg();
  if ((Current_Environment == E_HOUSE) ||
      (Current_Environment == E_MANSION)) {
    if (! gamestatusp(DESTROYED_ORDER)) {
      print1("The city guard arrives!");
      print2("You are apprehended....");
      morewait();
      send_to_jail();
    }
    else print1("Nobody answers the alarm.");
  }
  else if (Current_Environment == E_HOVEL) 
    print1("Nobody answers the alarm.");
  else {
    if (Current_Environment == E_CIRCLE) {
      summon(-1,ML9+7); /* prime circle demon */
      summon(-1,ML9+7);
      summon(-1,ML9+7);
    }
    for(ml=Level->mlist;ml!=NULL;ml=ml->next) {
      m_status_set(ml->m,AWAKE);
      ml->m->sense *= 2;
      if ((Current_Environment == E_CIRCLE) ||
	  ((Current_Environment == E_VILLAGE) && (ml->m->id == ML0+3)) ||
	  ((Current_Environment == E_CITY) && (ml->m->id == ML0+3)))
	m_status_set(ml->m,HOSTILE);
    }
  }
}

void l_trap_dart()
{
  Level->site[Player.x][Player.y].locchar = TRAP;
  if (gamestatusp(MOUNTED)) {
    mprint("A dart annoys your horse....");
  }
  else if (random_range(100) < Player.absorption) 
    mprint("A dart plinks off your armor");
  else {
    mprint("You were hit by a dart!");
    p_damage(difficulty(),NORMAL_DAMAGE,"a dart trap");
    if (random_range(3)==1) {
      mprint("It was poisoned!");
      p_poison(difficulty());
    }
  }
}

void l_trap_pit()
{
  Level->site[Player.x][Player.y].locchar = TRAP;
  if (gamestatusp(MOUNTED)) {
    mprint("Your horse stumbles into a pit and breaks a leg!");
    mprint("You are forced to put it out of its misery.");
    resetgamestatus(MOUNTED);
    showflags();
  }
  else if (Player.itemweight < ((int) (Player.maxweight / 2)))
    mprint("You nimbly dodge a pit trap.");
  else {
    mprint("You fell into a pit!");
    if (random_range(3) == 1) {
      mprint("And were impaled by spikes!");
      p_damage(difficulty()*5,NORMAL_DAMAGE,"a spiked pit");
    }
    else p_damage(difficulty()*2,NORMAL_DAMAGE,"a pit");
    Player.status[IMMOBILE] ++;
  }
}

void l_trap_door()
{
  if (Current_Environment != Current_Dungeon)
    mprint("You feel oddly lucky.");
  else {
    Level->site[Player.x][Player.y].locchar = TRAP;
    if (gamestatusp(MOUNTED)) {
      mprint("You and your horse fall through a trap door!");
      mprint("Your horse breaks its back. Snif.");
      if (Level->site[Player.x][Player.y].aux != S_DISINTEGRATE) {
	mprint("You're hit by a rockslide!");
	p_damage(Level->depth*difficulty(),UNSTOPPABLE,"a rockslide");
      }
      change_level(Level->depth,Level->depth+1,FALSE);
      resetgamestatus(MOUNTED);
      showflags();
      roomcheck();
    }
    else if (random_range(100) < Player.agi)
      mprint("You leap over a trap door.");
    else {
      mprint("You fell through a trap door!");
      p_damage(difficulty(),NORMAL_DAMAGE,"a trap door");
      if (Level->site[Player.x][Player.y].aux != S_DISINTEGRATE) {
	mprint("You're hit by a rockslide!");
	p_damage(Level->depth*difficulty(),UNSTOPPABLE,"a rockslide");
      }
      change_level(Level->depth,Level->depth+1,FALSE);
      roomcheck();
    }
  }
}

void l_trap_snare()
{
  Level->site[Player.x][Player.y].locchar = TRAP;
  if (gamestatusp(MOUNTED)) 
    mprint("Your horse steps out of a snare trap.");
  else if (random_range(100) < Player.agi)
    mprint("You agilely avoid a snare.");
  else {
    mprint("You were caught in a snare!");
    Player.status[IMMOBILE]+=difficulty();
  }
}

void l_trap_blade()
{
  Level->site[Player.x][Player.y].locchar = TRAP;
  if (random_range(30) < Player.agi+Player.level)
    mprint("You duck under a scything blade!");
  else {
    if (gamestatusp(MOUNTED)) {
      mprint("Your horse is struck by a blade and killed instantly.");
      resetgamestatus(MOUNTED);
      showflags();
    }
    else {
      mprint("A heavy blade scythes across the room and hits you!");
      p_damage(random_range(difficulty()*3)+difficulty()-Player.absorption,
	       NORMAL_DAMAGE,
	       "a blade trap");
    }
  }
}

void l_trap_fire()
{
  if (gamestatusp(MOUNTED)) {
    mprint("Your horse is struck by a blast of fire and is charbroiled!");
    resetgamestatus(MOUNTED);
    showflags();
  }
  else if (random_range(50) < Player.agi+Player.level)
    mprint("You dodge a pillar of fire!");
  else {
    mprint("You were blasted by a fire trap!");
    p_damage(random_range((difficulty()+1)*5),FLAME,"a fire trap");
  }
  Level->site[Player.x][Player.y].locchar = FIRE;
  Level->site[Player.x][Player.y].p_locf = L_FIRE;
}

void l_trap_teleport()
{
  Level->site[Player.x][Player.y].locchar = TRAP;
  mprint("You experience a momentary disorientation....");
  if (random_range(10000) > difficulty()*difficulty()) p_teleport(0);
  else p_teleport(-1);
}

void l_trap_disintegrate()
{
  Level->site[Player.x][Player.y].locchar = TRAP;
  mprint("Oh, no! A disintegration trap!");
  if (gamestatusp(MOUNTED)) {
    mprint("Your horse falls apart into its component atoms...");
    resetgamestatus(MOUNTED);
    showflags();
  }
  disintegrate(Player.x,Player.y);
}

void l_trap_sleepgas()
{
  Level->site[Player.x][Player.y].locchar = TRAP;
  mprint("Hsssssssss....");
  morewait();
  mprint("You detect a strange odor....");
  sleep_player((difficulty()/5)+1);
}


void l_trap_acid()
{
  int i,k,itemdamage;
  Level->site[Player.x][Player.y].locchar = TRAP;
  if (Player.agi+Player.level < random_range(100)) {
    mprint("You are drenched by a spray of acid!");
    if (gamestatusp(MOUNTED)) {
      mprint("Your horse dies unpleasantly.");
      resetgamestatus(MOUNTED);
      showflags();
    }
    p_damage(random_range(difficulty()*5),ACID,"an acid trap");
    if (! p_immune(ACID)) {
      mprint("The acid seeps into your pack...");
      morewait();
      itemdamage = random_range(5);
      for(i=k=0;((i<MAXITEMS) && (k<itemdamage));i++)
	if (Player.possessions[i]!=NULL) {
	  k++;
	  damage_item(Player.possessions[i]);
	}
    }
  }
  else mprint("You somehow dodge a shower of hydroflouric acid!");
}

void l_trap_abyss()
{
  Level->site[Player.x][Player.y].locchar = ABYSS;
  if (gamestatusp(MOUNTED)) {
    mprint("You and your horse fall into the infinite abyss!");
    l_abyss();
  }
  if (Player.dex + Player.level < random_range(100)) {
    mprint("You stumble over a concealed entrance to the abyss!");
    l_abyss();
  }
  else mprint("You gingerly step around a concealed entrance to the abyss!");
}

void l_trap_manadrain()
{
  Level->site[Player.x][Player.y].locchar = TRAP;
  if (Player.mana==0) {
    if (random_range(10)==3) {
      mprint("That's strange.... You feel repowered!");
      mprint("The manadrain trap overloads -- positive feedback....");
      Level->site[Player.x][Player.y].locchar = FLOOR;
      Level->site[Player.x][Player.y].p_locf = L_NO_OP;
      Player.mana = calcmana();
    }
    else mprint("You feel strangely unaffected by the manadrain trap.");
  }
  else {
    Level->site[Player.x][Player.y].locchar = TRAP;
    mprint("A weird rainbow light seems to play over you....");
    mprint("You feel drained.");
    while (Player.mana > 1) {
      Player.mana /= 2;
      dataprint();
    }
    Player.mana = 0;
    dispel(-1);
  }
}
