/* omega copyright (c) 1987,1988 by Laurence Raphael Brothers */
/* omstrike.c */
/* monster strike functions */

#include "oglob.h"




void m_firebolt(m)
struct monster *m;
{
  fbolt(m->x,m->y,Player.x,Player.y,m->hit,m->dmg);
}

void m_nbolt(m)
struct monster *m;
{
  nbolt(m->x,m->y,Player.x,Player.y,m->hit,m->dmg);
}


void m_lball(m)
struct monster *m;
{
  lball(m->x,m->y,Player.x,Player.y,m->dmg);
}

void m_fireball(m)
struct monster *m;
{
  fball(m->x,m->y,Player.x,Player.y,m->dmg);
}


void m_snowball(m)
struct monster *m;
{
  snowball(m->x,m->y,Player.x,Player.y,m->dmg);
}
    
void m_blind_strike(m)
struct monster *m;
{

  pml ml;
  if ((Player.status[BLINDED] == 0) &&
      los_p(m->x,m->y,Player.x,Player.y) &&
      (distance(m->x,m->y,Player.x,Player.y) < 5)) {
    if (m->uniqueness == COMMON) {
      strcpy(Str2,"The ");
      strcat(Str2,m->monstring);
    }
    else strcpy(Str2,m->monstring);
    strcat(Str2," gazes at you menacingly");
    mprint(Str2);
    if (! p_immune(GAZE)) {
      mprint("You've been blinded!");
      Player.status[BLINDED] = random_range(4)+1;
      for(ml=Level->mlist;ml!=NULL;ml=ml->next) 
	plotspot(ml->m->x,ml->m->y,FALSE);
    }
    else mprint("You gaze steadily back....");
  }
}




void m_strike_sonic(m)
struct monster *m;
{
  if (m->uniqueness == COMMON) {
    strcpy(Str2,"The ");
    strcat(Str2,m->monstring);
  }
  else strcpy(Str2,m->monstring);
  strcat(Str2,"screams at you!");
  mprint(Str2);
  p_damage(m->dmg,OTHER_MAGIC,"a sonic blast");
}

