/* omega copyright (c) 1987,1988,1989 by Laurence Raphael Brothers */
/* mmelee */
/* various functions to do with monster melee */

#include "glob.h"


void m_hit(m,dtype)
struct monster *m;
int dtype;
{
  if (m->uniqueness == COMMON) {
    strcpy(Str3,"a ");
    strcat(Str3,m->monstring);
  }
  else strcpy(Str3,m->monstring);
  if ((Player.status[DISPLACED] > 0) && (random_range(2) == 1))
    mprint("The attack was displaced!");
  else  p_damage(random_range(m->dmg),dtype,Str3);
}



/* execute monster attacks versus player */
void tacmonster(m)
struct monster *m;
{
  int i=0;
  drawvision(Player.x,Player.y);
  transcribe_monster_actions(m);
  while ((i < strlen(m->meleestr)) && (m->hp > 0)) {
    if (m->uniqueness == COMMON) {
      strcpy(Str4,"The ");
      strcat(Str4,m->monstring);
    }
    else strcpy(Str4,m->monstring);
    if (m->meleestr[i] == 'A') {
      strcat(Str4," attacks ");
      strcat(Str4,actionlocstr(m->meleestr[i+1]));
      if (Verbosity == VERBOSE) mprint(Str4);
      monster_melee(m,m->meleestr[i+1],0);
    }
    else if (m->meleestr[i] == 'L') {
      strcat(Str4," lunges ");
      strcat(Str4,actionlocstr(m->meleestr[i+1]));
      if (Verbosity == VERBOSE) mprint(Str4);
      monster_melee(m,m->meleestr[i+1],m->level);
    }
    i+=2;
  }
}



void monster_melee(m,hitloc,bonus)
struct monster *m;
char hitloc;
int bonus;
{
  if (player_on_sanctuary())
    print1("The aegis of your deity protects you!");
  else {
    /* It's lawful to wait to be attacked */
    if (m->attacked==0) Player.alignment++;
    m->attacked++;
    if (m->uniqueness == COMMON) {
      strcpy(Str2,"The ");
      strcat(Str2,m->monstring);
    }
    else strcpy(Str2,m->monstring);
    if (monster_hit(m,hitloc,bonus))
      switch(m->meleef) {
      case M_NO_OP: 
	strcat(Str2," touches you.");
	mprint(Str2);
	break;
      case M_MELEE_NORMAL:
	strcat(Str2," hits you.");
	mprint(Str2);
	m_hit(m,NORMAL_DAMAGE);
	break;
      case M_MELEE_NG:
	strcat(Str2," hits you.");
	mprint(Str2);
	m_hit(m,NORMAL_DAMAGE);
	if (random_range(5)==3) m_sp_ng(m);
	break;
      case M_MELEE_FIRE:
	strcat(Str2," blasts you with fire.");
	mprint(Str2);
	m_hit(m,FLAME);
	break;
      case M_MELEE_DRAGON:
	strcat(Str2," hits you and blasts you with fire.");
	mprint(Str2);
	m_hit(m,NORMAL_DAMAGE);
	m_hit(m,FLAME);
	break;
      case M_MELEE_ELEC:
	strcat(Str2," lashes you with electricity.");
	mprint(Str2);
	m_hit(m,ELECTRICITY);
	break;
      case M_MELEE_COLD:
	strcat(Str2," freezes you with cold.");
	mprint(Str2);
	m_hit(m,ELECTRICITY);
	break;
      case M_MELEE_POISON:
	strcat(Str2," hits you.");
	mprint(Str2);
	m_hit(m,NORMAL_DAMAGE);
	if (random_range(10) < m->level) {
	  mprint("You've been poisoned!");
	  p_poison(m->dmg);
	}
	break;
      case M_MELEE_GRAPPLE:
	strcat(Str2," grabs you.");
	mprint(Str2);
	m_hit(m,NORMAL_DAMAGE);
	Player.status[IMMOBILE]++;
	break;
      case M_MELEE_SPIRIT:
	strcat(Str2," touches you.");
	mprint(Str2);
	m_hit(m,NORMAL_DAMAGE);
	drain_life(m->level);
	break;
      case M_MELEE_DISEASE:
	strcat(Str2," hits you.");
	mprint(Str2);
	m_hit(m,NORMAL_DAMAGE);
	if (random_range(10) < m->level) {
	  mprint("You've been infected!");
	  disease(m->level);
	}
	break;
      case M_MELEE_SLEEP:
	strcat(Str2," hit you.");
	mprint(Str2);
	m_hit(m,NORMAL_DAMAGE);
	if (random_range(10) < m->level) {
	  mprint("You feel drowsy");
	  sleep_player(m->level);
	}
	break;
      }
    else {
      if (random_range(10)) strcat(Str2," missed you.");
      else {
	if (Verbosity == TERSE) {
	  switch(random_range(10)) {
	    case 0:
	      strcat(Str2," blundered severely.");
	      m_damage(m,m->dmg,UNSTOPPABLE); break;
	    case 1:
	      strcat(Str2," tripped while attacking.");m_dropstuff(m);break;
	    case 2:
	      strcat(Str2," seems seriously confused.");
	      m->speed = min(30,m->speed*2); break;
	    default:
	      strcat(Str2," missed you.");
	  }
	}
	switch(random_range(10)) {
	  case 0: strcat(Str2," flailed stupidly at you."); break;
	  case 1: strcat(Str2," made you laugh."); break;
	  case 2: strcat(Str2," blundered severely.");
	    m_damage(m,m->dmg,UNSTOPPABLE); break;
	  case 3: strcat(Str2," tripped while attacking.");m_dropstuff(m);break;
	  case 4: strcat(Str2," seems seriously confused.");
	    m->speed = min(30,m->speed*2); break;
	  case 5: strcat(Str2," is seriously ashamed."); break;
	  case 6: strcat(Str2," made a boo-boo."); break;
	  case 7: strcat(Str2," blundered."); break;
	  case 8: strcat(Str2," cries out in anger and frustration."); break;
	  case 9: strcat(Str2," curses your ancestry."); break;
	}
      }
      mprint(Str2);
    }
  }
}



/* checks to see if player hits with hitmod vs. monster m at location hitloc */
int monster_hit(m,hitloc,bonus)
struct monster *m;
char hitloc;
int bonus;
{
  int i=0,blocks=FALSE,goodblocks=0,hit,riposte=FALSE;
  while (i<strlen(Player.meleestr)) {
    if ((Player.meleestr[i] == 'B') || (Player.meleestr[i] == 'R')) {
      blocks = TRUE;
      if (hitloc == Player.meleestr[i+1]) {
	goodblocks++;
	if (Player.meleestr[i] == 'R') riposte = TRUE;
      }
    }
    i+=2;
  }
  if (! blocks) goodblocks = -1;
  hit = hitp(m->hit+bonus,Player.defense+goodblocks*10);
  if ((! hit) && (goodblocks > 0)) {
    if (Verbosity == VERBOSE) mprint("You blocked it!");
    if (riposte) {
      if (Verbosity != TERSE) mprint("You got a riposte!");
      if (hitp(Player.hit,m->ac)) {
	mprint("You hit!");
	weapon_use(0,Player.possessions[O_WEAPON_HAND],m);
      }
      else mprint("You missed.");
    }
  }
  return(hit);
}



/* decide monster actions in tactical combat mode */
/* if monster is skilled, it can try see the player's attacks coming and
   try to block appropriately. */

void transcribe_monster_actions(m)
struct monster *m;
{
  int i;
  char attack_loc,block_loc;
  static char mmstr[80];
  
  int p_blocks[3];
  int p_attacks[3];

  for(i=0;i<3;i++)p_blocks[i] = p_attacks[i] = 0;

  /* Find which area player blocks and attacks least in */
  i = 0;
  while (i<strlen(Player.meleestr)) {
    if ((Player.meleestr[i] == 'B') ||
	(Player.meleestr[i] == 'R')) {
      if (Player.meleestr[i+1] == 'H') p_blocks[0]++;
      if (Player.meleestr[i+1] == 'C') p_blocks[1]++;
      if (Player.meleestr[i+1] == 'L') p_blocks[2]++;
    }
    else if ((Player.meleestr[i] == 'A') ||
	     (Player.meleestr[i] == 'L')) {
      if (Player.meleestr[i+1] == 'H') p_attacks[0]++;
      if (Player.meleestr[i+1] == 'C') p_attacks[1]++;
      if (Player.meleestr[i+1] == 'L') p_attacks[2]++;
    }
    i+=2;
  }

  if ((p_blocks[2] <= p_blocks[1]) &&
      (p_blocks[2] <= p_blocks[0]))
    attack_loc = 'L';
  else if ((p_blocks[1] <= p_blocks[2]) &&
	   (p_blocks[1] <= p_blocks[0]))
    attack_loc = 'C'; /* DG 12/31/98 */
  else attack_loc = 'H';
  if ((p_attacks[2] <= p_attacks[1]) &&
      (p_attacks[2] <= p_attacks[0]))
    block_loc = 'L';
  else if ((p_attacks[1] <= p_attacks[2]) &&
	   (p_attacks[1] <= p_attacks[0]))
    block_loc = 'C';
  else block_loc = 'H';

  m->meleestr = mmstr;

  if (m->id != NPC)
    strcpy(m->meleestr,Monsters[m->id].meleestr);
  else {
    strcpy(m->meleestr,"");
    for(i=0;i<m->level;i+=2) 
      strcat(m->meleestr,"L?R?");
  }

  i = 0;
  while (i<strlen(m->meleestr)) {
    if ((m->meleestr[i] == 'A') || (m->meleestr[i] == 'L')) {
      if (m->meleestr[i+1] == '?') {
	if (m->level+random_range(30) > Player.level+random_range(20))
	  m->meleestr[i+1] = attack_loc;
	else m->meleestr[i+1] = random_loc();
      }
      else if (m->meleestr[i+1] == 'X') m->meleestr[i+1] = random_loc();
    }
    else if ((m->meleestr[i] == 'B') || (m->meleestr[i] == 'R')) {
      if (m->meleestr[i+1] == '?') { 
	if (m->level+random_range(30) > Player.level+random_range(20))
	  m->meleestr[i+1] = block_loc;
	else m->meleestr[i+1] = random_loc();
      }
      else if (m->meleestr[i+1] == 'X') m->meleestr[i+1] = random_loc();
    }
    i+=2;
  }
}

char random_loc()
{
  switch(random_range(3)) {
  case 0:return('H');
  case 1:return('C');
  default:return('L');
  }
}
