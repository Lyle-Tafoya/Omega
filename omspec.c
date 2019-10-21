/* omega copyright (c) 1987,1988 by Laurence Raphael Brothers */
/* omspec.c */
/* monster special functions */

#include "oglob.h"


void m_sp_mp(m)
struct monster *m;
{
  if (m->attacked && (random_range(3) == 1)) {
    mprint("You feel cursed!");
    p_damage(10,UNSTOPPABLE,"a mendicant priest's curse");
    m_vanish(m);
  }
  else if (! m_statusp(m,NEEDY)) {
    mprint("The mendicant priest makes a mystical gesture....");
    mprint("You feel impressed...");
    Player.alignment += 5;
    if (Player.alignment > 20)
      Player.hp = max(Player.hp,Player.maxhp);
    m_vanish(m);
  }
}



void m_sp_ng(m)
struct monster *m;
{
  if (distance(m->x,m->y,Player.x,Player.y) < 2)
    if ((random_range(5) == 1) || (Player.status[VULNERABLE]>0)) {
      mprint("The night gaunt grabs you and carries you off!");
      mprint("Its leathery wings flap and flap, and it giggles insanely.");
      mprint("It tickles you cunningly to render you incapable of escape.");
      mprint("Finally, it deposits you in a strange place.");
      p_teleport(0);
    }
}





void m_sp_poison_cloud(m)
struct monster *m;
{
  if (distance(m->x,m->y,Player.x,Player.y) < 3) {
    mprint("A cloud of poison gas surrounds you!");
    if (Player.status[BREATHING] > 0)
      mprint("You can breathe freely, however.");
    else p_poison(7);
  }
}


void m_sp_explode(m)
struct monster *m;
{
  if ((distance(Player.x,Player.y,m->x,m->y)<2) &&
      (m-> hp > 0) && 
      (m->hp < Monsters[m->id].hp))
    fball(m->x,m->y,m->x,m->y,m->hp);
}



void m_sp_demon(m)
struct monster *m;
{
  int mid;

  if (random_range(2)) {
    if ((m->id != ML4+6) && /*succubi don't give fear */
	los_p(m->x,m->y,Player.x,Player.y) &&
	(random_range(30) > Player.level+10) &&
	(Player.status[AFRAID] == 0)) {
      mprint("You are stricken with fear!");
      if (! p_immune(FEAR)) Player.status[AFRAID] += m->level;
      else mprint("You master your reptile brain and stand fast.");
    }
    else m_sp_spell(m);
  }
  if ((m->hp < (m->level * 5)) && (m->hp > 1)) {
    mprint("The demon uses its waning lifeforce to summon help!");
    m->hp = 1;
    switch(m->level) {
    case 3: mid = ML2+1; break; /* night gaunt */
    case 4:
    case 5: mid = ML3+2; break; /* lesser frost demon */
    case 6: mid = ML5+4; break; /* frost demon */
    case 7: mid = ML5+12; break; /* outer circle demon */
    case 8: mid = ML6+10; break; /* demon serpent */
    case 9: mid = ML7+14; break; /* inner circle demon */
    }
    summon(-1,mid);
    summon(-1,mid);
  }
}


void m_sp_acid_cloud(m)
struct monster *m;
{
  if (m_statusp(m,HOSTILE) &&
      (distance(m->x,m->y,Player.x,Player.y) < 3))
    acid_cloud();
}

	  
void m_sp_escape(m)
struct monster *m;
{
  if (m_statusp(m,HOSTILE))
    m_vanish(m);
}


void m_sp_ghost(m)
struct monster *m;
{
  if (m_statusp(m,HOSTILE)) {
    mprint("The ghost moans horribly....");
    p_damage(1,FEAR,"a ghost-inspired heart attack");
    mprint("You've been terrorized!");
    if (! p_immune(FEAR)) Player.status[AFRAID] += m->level;
    else mprint("You master your reptile brain and stand fast.");
  }
}




/* random spell cast by monster */
void m_sp_spell(m)
struct monster *m;
{
  char action[80];
  if (m_statusp(m,HOSTILE) && los_p(Player.x,Player.y,m->x,m->y)) {
    if (m->uniqueness == COMMON) strcpy(action,"The ");
    else strcpy(action,"");
    strcat(action,m->monstring);
    strcat(action," casts a spell...");
    mprint(action);
    if (! magic_resist(m->level)) switch (random_range(m->level+7)) {
    case 0: 
      nbolt(m->x,m->y,Player.x,Player.y,m->hit,10);
      break;
    case 1:
      mprint("It seems stronger..."); 
      m->hp += random_range(m->level*m->level);
      break;
    case 2:
      haste(-1);
      break;
    case 3:
      cure(-1);
      break;
    case 4:
      lball(m->x,m->y,Player.x,Player.y,20);
      break;
    case 5:
      enchant(-1);
    case 6:
      snowball(m->x,m->y,Player.x,Player.y,30);
      break;
    case 7:
      bless(0-m->level);
      break;
    case 8:
      p_poison(m->level);
      break;
    case 9:
      sleep_player(m->level/2);
      break;
    case 10:
      fbolt(m->x,m->y,Player.x,Player.y,m->hit*3,50);
      break;
    case 11:
      acquire(0-m->level);
      break;
    case 12:
      dispel(-1);
      break;
    case 13:
      disrupt(Player.x,Player.y,50);
      break;
    case 14:
      if (m->uniqueness == COMMON) {
	strcpy(Str2,"a ");
	strcat(Str2,m->monstring);
      }
      else strcpy(Str2,m->monstring);
      level_drain(m->level,Str2);
      break;
    case 15:
    case 16:
      disintegrate(Player.x,Player.y);
      break;
    }
  }
}



/* monsters with this have some way to hide, camouflage, etc until they 
   attack */
void m_sp_surprise(m)
struct monster *m;
{
  if (m->attacked) {
    if (m_statusp(m,HOSTILE) && 
	(! Player.status[TRUESIGHT]) &&
	m_statusp(m,M_INVISIBLE)) {
      m->monchar = Monsters[m->id].monchar;
      if (! Player.status[ALERT]) {
	switch(random_range(4)) {
	case 0: 
	  mprint("You are surprised by a sudden treacherous attack!");
	  break;
	case 1: 
	  mprint("You are shocked out of your reverie by the scream of battle!");
	  break;
	case 2: 
	  mprint("Suddenly, from out of the shadows, a surprise attack!");
	  break;
	case 3: 
	  mprint("A shriek of hatred causes you to momentarily freeze up!");
	  break;
	}
	morewait();
	setgamestatus(SKIP_PLAYER);
	m_status_reset(m,M_INVISIBLE);
      }
      else {
	mprint("You alertly sense the presence of an attacker!");
	m_status_reset(m,M_INVISIBLE);
      }	
    }
  }
}

void m_sp_whistleblower(m)
struct monster *m;
{
  pml ml;
  if (m_statusp(m,HOSTILE)) {
    alert_guards();
    m->specialf = M_MELEE_NORMAL;
  }
}


void m_sp_seductor(m)
struct monster *m;
{
  if (m_statusp(m,HOSTILE)) {
    if (m->uniqueness == COMMON) {
      strcpy(Str2,"The ");
      strcat(Str2,m->monstring);
    }
    else strcpy(Str2,m->monstring);
    strcat(Str2," runs away screaming for help....");
    mprint(Str2);
    m_vanish(m);
    summon(-1,-1);
    summon(-1,-1);
    summon(-1,-1);
  }
  else if (distance(Player.x,Player.y,m->x,m->y) < 2) 
    m_talk_seductor(m);

}


void m_sp_demonlover(m)
struct monster *m;
{
  if (distance(Player.x,Player.y,m->x,m->y) < 2) 
    m_talk_demonlover(m);
}

void m_sp_eater(m)
struct monster *m;
{
  int i;
  if (Player.rank[COLLEGE]) m_status_set(m,HOSTILE);
  if (m_statusp(m,HOSTILE))
    if (los_p(m->x,m->y,Player.x,Player.y)) {
      mprint("A strange numbing sensation comes over you...");
      morewait();
      Player.mana = Player.mana / 2;
      if (random_range(4)) enchant(-1);
      else dispel(-1);
      Player.pow--;
      if (--Player.pow < 1) p_death("The Eater of Magic");
    }
  if (m->hp < 10) {
    mprint("The Eater explodes in a burst of mana!");
    for(i=0;i<9;i++) 
      manastorm(m->x+Dirs[0][i],m->y+Dirs[0][i],100);
  }
}


void m_sp_dragonlord(m)
struct monster *m;
{
  if (m_statusp(m,HOSTILE)) {
    if (distance(m->x,m->y,Player.x,Player.y)<2) {
      if (! Player.status[IMMOBILE]) {
	mprint("A gust of wind from the Dragonlord's wings knocks you down!");
	p_damage(25,NORMAL_DAMAGE,"a gust of wind");
	setgamestatus(SKIP_PLAYER);
	Player.status[IMMOBILE]+=2;
      }
      else if (! Constriction) {
	mprint("The Dragonlord grabs you with his tail!");
	Constriction = 25;
	Player.status[IMMOBILE]+=1;
      }
      else if (random_range(2)) {
	mprint("The coils squeeze tighter and tighter...");
	p_damage(Constriction,NORMAL_DAMAGE,"The Dragonlord");
	Player.status[IMMOBILE]+=1;
	Constriction *=2;
      }
      else {
	mprint("The dragonlord hurls you to the ground!");
	p_damage(2*Constriction,NORMAL_DAMAGE,"The Dragonlord");
	Constriction = 0;
      }
      m_sp_spell(m);
    }
    else {
      Constriction = 0;
      if (view_los_p(m->x,m->y,Player.x,Player.y)) {
	if ((! Player.immunity[FEAR]) && (! Player.status[AFRAID])) {
	  mprint("You are awestruck at the sight of the Dragonlord.");
	  Player.status[AFRAID]+=5;
	}
	if (random_range(3)) {
	  m_sp_spell(m);
	  m_sp_spell(m);
	}
      }
    }
  }
  else if (distance(m->x,m->y,Player.x,Player.y)<2)
    mprint("You are extremely impressed at the sight of the Dragonlord.");
}


void m_sp_blackout(m)
struct monster *m;
{
  if ((distance(m->x,m->y,Player.x,Player.y) < 4) &&
      (Player.status[BLINDED] == 0)) {
    mprint("The fungus emits a burst of black spores. You've been blinded!");
    if (Player.status[TRUESIGHT] > 0) mprint("The blindness quickly passes.");
    else Player.status[BLINDED]+=4;
  }
  if (loc_statusp(m->x,m->y,LIT)) {
    mprint("The fungus chirps.... ");
    mprint("The area is plunged into darkness.");
    torch_check();torch_check();torch_check();
    torch_check();torch_check();torch_check();
    spreadroomdark(m->x,m->y,Level->site[m->x][m->y].roomnumber);
    levelrefresh();
  }
}


void m_sp_bogthing(m)
struct monster *m;
{
  if (Player.status[IMMOBILE] && 
      (distance(Player.x,Player.y,m->x,m->y) < 2)) {
    if (! Player.status[AFRAID]) {
      mprint("As the bogthing touches you, you feel a frisson of terror....");
      if (Player.immunity[FEAR]) mprint("which you shake off.");
      else Player.status[AFRAID]+=2;
    }
    else {
      mprint("The bogthing's touch causes you scream in agony!");
      p_damage(50,UNSTOPPABLE,"fright");
      mprint("Your struggles grow steadily weaker....");
      Player.con--;
      Player.str--;
      if ((Player.con < 3) || (Player.str < 3))
	p_death("congestive heart failure");
    }
  }
}


void m_sp_were(m)
struct monster *m;
{
  int mid;
  if (m_statusp(m,HOSTILE) || (Phase == 6)) {
    do mid = random_range(ML9-NML_0)+ML1;
    while ((Monsters[mid].uniqueness != COMMON) ||
	   (! m_statusp(&(Monsters[mid]),MOBILE)) ||
	   (! m_statusp(&(Monsters[mid]),HOSTILE)));
    m->id = Monsters[mid].id;
    m->hp += Monsters[mid].hp;
    m->status |= Monsters[mid].status;
    m->ac = Monsters[mid].ac;
    m->dmg = Monsters[mid].dmg;
    m->speed = Monsters[mid].speed;
    m->immunity |= Monsters[mid].immunity;
    m->xpv += Monsters[mid].xpv;
    m->corpseweight = Monsters[mid].corpseweight;
    m->monchar = Monsters[mid].monchar;
    m->talkf = Monsters[mid].talkf;
    m->meleef = Monsters[mid].meleef;
    m->strikef = Monsters[mid].strikef;
    m->specialf = Monsters[mid].specialf;
    strcpy(Str1,"were-");
    strcat(Str1,Monsters[mid].monstring);
    strcpy(Str2,"dead were-");
    strcat(Str2,Monsters[mid].monstring);
    m->monstring = salloc(Str1);
    m->corpsestr = salloc(Str2);
    m->immunity += pow2(NORMAL_DAMAGE);
    if (los_p(m->x,m->y,Player.x,Player.y)) 
      mprint("You witness a hideous transformation!");
    else mprint("You hear a distant howl.");
  }
}    


void m_sp_leash(m)
struct monster *m;
{
  if (m->aux1 < 0) {
    m->aux1 = m->x;
    m->aux2 = m->y;
  }
  else if (distance(m->x,m->y,m->aux1,m->aux2) > 5) {
    Level->site[m->x][m->y].creature = NULL;
    m->x = m->aux1;
    m->y = m->aux2;
    Level->site[m->x][m->y].creature = m;
    if (los_p(Player.x,Player.y,m->x,m->y)) 
      mprint("You see the dog jerked back by its chain!");
    else mprint("You hear a strangled sort of yelp!");
  }
}

void m_sp_servant(m)
struct monster *m;
{
  pml ml;
  if ((m->id == ML4+12) && (Player.alignment < 0))
    m_status_set(m,HOSTILE);
  else if ((m->id == ML4+13) && (Player.alignment > 0))
    m_status_set(m,HOSTILE);
}


void m_sp_av(m)
struct monster *m;
{
  if (Player.mana > 0) {
    mprint("You feel a sudden loss of mana!");
    Player.mana -= (max(0,10-distance(m->x,m->y,Player.x,Player.y)));
    dataprint();
  }
}

void m_sp_lw(m)
struct monster *m;
{
  if (random_range(2)) {
    if (Level->site[m->x][m->y].locchar == FLOOR) {
      Level->site[m->x][m->y].locchar = LAVA;
      Level->site[m->x][m->y].p_locf = L_LAVA;
    }
    else if (Level->site[m->x][m->y].locchar == WATER) {
      Level->site[m->x][m->y].locchar = FLOOR;
      Level->site[m->x][m->y].p_locf = L_NO_OP;
    }
  }
}


void m_sp_angel(m)
struct monster *m;
{
  int mid,hostile = FALSE;
  switch(m->aux1) {
  case ATHENA:
  case ODIN: 
    hostile = ((Player.patron == HECATE) || (Player.patron == SET));
    break;
  case SET:
  case HECATE: 
    hostile = ((Player.patron == ODIN) || (Player.patron == ATHENA));
    break;
  case DESTINY:
    hostile = (Player.patron != DESTINY);
    break;
  }
  if (hostile)
    m_status_set(m,HOSTILE);
  if (m_statusp(m,HOSTILE)) {
    mprint("The angel summons a heavenly host!");
    switch(m->level) {
    case 9: mid = ML8+11; break; /* high angel */
    case 8: mid = ML6+11; break; /* angel */
    default:
    case 6: mid = ML3+4; break; /* phantom */
    }
    summon(-1,mid);
    summon(-1,mid);
    summon(-1,mid);
    /* prevent angel from summoning infinitely */
    m->specialf = M_NO_OP;
  }
}


/* Could completely fill up level */
void m_sp_swarm(m)
struct monster *m;
{
  if (random_range(5)==1) {
    if (view_los_p(m->x,m->y,Player.x,Player.y))
      mprint("The swarm expands!");
    else mprint("You hear an aggravating humming noise.");
    summon(-1,ML4+14);
  }
}




/* raise nearby corpses from the dead.... */
void m_sp_raise(m)
struct monster *m;
{
  int x,y;
  pol t;
  for(x=m->x-2;x<=m->x+2;x++)
    for(y=m->y-2;y<=m->y+2;y++)
      if (inbounds(x,y)) 
	if (Level->site[x][y].things != NULL)
	  if (Level->site[x][y].things->thing->id == CORPSEID) {
	    mprint("The Zombie Overlord makes a mystical gesture...");
	    summon(-1,Level->site[x][y].things->thing->charge);
	    t = Level->site[x][y].things;
	    Level->site[x][y].things = Level->site[x][y].things->next;
	    free((char *) t);
	  }
}




void m_sp_mb(m)
struct monster *m;
{
  if (distance(m->x,m->y,Player.x,Player.y)==1) {
    mprint("The manaburst explodes!");
    if (m_statusp(m,HOSTILE)) {
      mprint("You get blasted!");
      p_damage(random_range(100),UNSTOPPABLE,"a manaburst");
      mprint("You feel cold all over!");
      Player.pow-=3;
      Player.iq--;
      Player.con--;
      Player.str-=2;
      Player.dex--;
      Player.agi--;
      dispel(-1);
    }
    else {
      mprint("You feel toasty warm inside!");
      Player.pow++;
      Player.mana = max(Player.mana,calcmana());
      Player.hp = max(Player.hp,++Player.maxhp);
    }
    m->hp = 0;
  }
}


void m_sp_mirror(m)
struct monster *m;
{
  int i,x,y;
  if (view_los_p(m->x,m->y,Player.x,Player.y)) {
    if (random_range(20)+6 < m->level) {
      summon(-1,m->id);
      mprint("You hear the sound of a mirror shattering!");
    }
    else for(i=0;i<5;i++) {
      x = m->x + random_range(13)-6;
      y = m->y + random_range(13)-6;
      if (inbounds(x,y)) {
	Level->site[x][y].showchar = m->monchar;
	putspot(x,y,m->monchar);
      }
    }
  }
}



void m_illusion(m)
struct monster *m;
{
  int i = random_range(NUMMONSTERS);
  if (i==ML0+4) i = m->id; /* can't imitate NPC */
  if (Player.status[TRUESIGHT]) {
    m->monchar = Monsters[m->id].monchar;
    m->monstring = Monsters[m->id].monstring;
  }
  else  if (random_range(5) == 1) {
    m->monchar = Monsters[i].monchar;
    m->monstring = Monsters[i].monstring;
  }
}




void m_huge_sounds(m)
struct monster *m;
{
  if (m_statusp(m,AWAKE) &&
      (! los_p(m->x,m->y,Player.x,Player.y)) &&
      (random_range(10) == 1))
    mprint("The dungeon shakes!");
}



void m_thief_f(m)
struct monster *m;
{
  int i = random_item();
  if (random_range(3) == 1) {
    if (distance(Player.x,Player.y,m->x,m->y) < 2) {
      if (p_immune(THEFT) || (Player.level > (m->level*2)+random_range(20))) 
	mprint("You feel secure.");
      else {
	if (i == ABORT)
	  mprint("You feel fortunate.");
	else if ((Player.possessions[i]->used) || 
		 (Player.dex < m->level*random_range(10))) {
	  mprint("You feel a sharp tug.... You hold on!");
	}
	else {
	  mprint("You feel uneasy for a moment.");
	  if (m->uniqueness == COMMON) {
	    strcpy(Str2,"The ");
	    strcat(Str2,m->monstring);
	  }
	  else strcpy(Str2,m->monstring);
	  strcat(Str2,"suddenly runs away for some reason.");
	  mprint(Str2);
	  m_teleport(m);
	  m->movef = M_MOVE_SCAREDY;
	  m->specialf = M_MOVE_SCAREDY;
	  m_pickup(m,Player.possessions[i]);
	  conform_lost_object(Player.possessions[i]);
	}
      }
    }
  }
}


void m_summon(m)
struct monster *m;
{
  if ((distance(Player.x,Player.y,m->x,m->y) < 2) &&
      (random_range(3) == 1)) {
	summon(0,-1);
	summon(0,-1);
      }
}	  


void m_aggravate(m)
struct monster *m;
{

  if (m_statusp(m,HOSTILE)) {
    if (m->uniqueness == COMMON) {
      strcpy(Str2,"The ");
      strcat(Str2,m->monstring);
    }
    else strcpy(Str2,m->monstring);
    strcat(Str2," emits an irritating humming sound.");
    mprint(Str2);
    aggravate();
    m_status_reset(m,HOSTILE);
  }
}



void m_sp_merchant(m)
struct monster *m;
{
  pml ml;
  if (m_statusp(m,HOSTILE))
    if (Current_Environment == E_VILLAGE) {
      mprint("The merchant screams: 'Help! Murder! Guards! Help!'");
      mprint("You hear the sound of police whistles and running feet.");
      for (ml=Level->mlist;ml!=NULL;ml=ml->next) {
	m_status_set(ml->m,AWAKE);
	m_status_set(ml->m,HOSTILE);
      }
      m->specialf = M_NO_OP;
    }
}

/* The special function of the various people in the court of the archmage */
/* and the sorcerors' circle */
void m_sp_court(m)
struct monster *m;
{
  pml ml;
  if (m_statusp(m,HOSTILE)) {
    mprint("A storm of spells hits you!");
    for(ml=Level->mlist;ml!=NULL;ml=ml->next) {
      m_status_set(ml->m,HOSTILE);
      m_sp_spell(ml->m);
      if (ml->m->specialf == M_SP_COURT)
	ml->m->specialf = M_SP_SPELL;
    }
  }
}


/* The special function of the dragons in the dragons' lair */
void m_sp_lair(m)
struct monster *m;
{
  pml ml;
  if (m_statusp(m,HOSTILE)) {
    if (m->id == ML10+3) m->specialf = M_SP_DRAGONLORD;
    mprint("You notice a number of dragons waking up....");
    mprint("You are struck by a quantity of firebolts.");
    for(ml=Level->mlist;ml!=NULL;ml=ml->next) {
      m_status_set(ml->m,HOSTILE);
      fbolt(ml->m->x,ml->m->y,Player.x,Player.y,100,100);
      if (ml->m->specialf == M_SP_LAIR)
	ml->m->specialf = M_STRIKE_FBOLT;
    }
  }
}


void m_sp_prime(m)
struct monster *m;
{
  if (m_statusp(m,HOSTILE)) {
    mprint("The prime sorceror gestures and a pentacular gate opens!");
    mprint("You are surrounded by demons!");
    summon(-1,ML9+7);
    summon(-1,ML9+7);
    summon(-1,ML9+7);
    summon(-1,ML9+7);
  }
  m->specialf = M_SP_SPELL;
}
