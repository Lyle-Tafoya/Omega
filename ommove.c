/* omega copyright (c) 1987,1988 by Laurence Raphael Brothers */
/* ommove.c */
/* monster move functions */

#include "oglob.h"


/* like m_normal_move, but can open doors */
void m_smart_move(m)
struct monster *m;
{
  m_simple_move(m);
}

/* not very smart, but not altogether stupid movement */
void m_normal_move(m)
struct monster *m;
{
  m_simple_move(m);
}


/* used by both m_normal_move and m_smart_move */
void m_simple_move(m)
struct monster *m;
{
  int dx = sign(Player.x - m->x);
  int dy = sign(Player.y - m->y);
  erase_monster(m);
  if (m->hp < Monsters[m->id].hp/4) {
    dx = - dx;
    dy = - dy;
    m->movef = M_MOVE_SCAREDY;
    if (m->uniqueness == COMMON) {
      strcpy(Str2,"The ");
      strcat(Str2,m->monstring);
    }
    else strcpy(Str2,m->monstring);
    if (m->possessions != NULL) {
      strcat(Str2," drops its treasure and flees!");
      m_dropstuff(m);
    }
    else strcat(Str2," flees!");
    mprint(Str2);
    m->speed = min(2,m->speed-1);
  }
  if ((! m_statusp(m,HOSTILE)) || 
      (Player.status[INVISIBLE] > 0)) m_random_move(m);
  else {
    if (m_unblocked(m,m->x+dx,m->y+dy))
      movemonster(m,m->x+dx,m->y+dy);
    else if (dx == 0) {
      if (m_unblocked(m,m->x+1,m->y+dy))
	movemonster(m,m->x+1,m->y+dy);
      else if (m_unblocked(m,m->x-1,m->y+dy))
	movemonster(m,m->x-1,m->y+dy);
    }
    
    else if (dy == 0) {
      if (m_unblocked(m,m->x+dx,m->y+1))
	movemonster(m,m->x+dx,m->y+1);
      else if (m_unblocked(m,m->x+dx,m->y-1))
	movemonster(m,m->x+dx,m->y-1);
    }
    
    else {
      if (m_unblocked(m,m->x+dx,m->y))
	movemonster(m,m->x+dx,m->y);
      else if (m_unblocked(m,m->x,m->y+dy))
	movemonster(m,m->x,m->y+dy);
    }
  }
}




void m_move_animal(m)
struct monster *m;
{
  if (m_statusp(m,HOSTILE))
    m_normal_move(m);
  else m_scaredy_move(m);
}




/* same as simple move except run in opposite direction */
void m_scaredy_move(m)
struct monster *m;
{
  int dx = -sign(Player.x - m->x);
  int dy = -sign(Player.y - m->y);
  erase_monster(m);
  if (Player.status[INVISIBLE]) m_random_move(m);
  else {
    if (m_unblocked(m,m->x+dx,m->y+dy))
      movemonster(m,m->x+dx,m->y+dy);
    else if (dx == 0) {
      if (m_unblocked(m,m->x+1,m->y+dy))
	movemonster(m,m->x+1,m->y+dy);
      else if (m_unblocked(m,m->x-1,m->y+dy))
	movemonster(m,m->x-1,m->y+dy);
    }
    
    else if (dy == 0) {
      if (m_unblocked(m,m->x+dx,m->y+1))
	movemonster(m,m->x+dx,m->y+1);
      else if (m_unblocked(m,m->x+dx,m->y-1))
	movemonster(m,m->x+dx,m->y-1);
    }
    
    else {
      if (m_unblocked(m,m->x+dx,m->y))
	movemonster(m,m->x+dx,m->y);
      else if (m_unblocked(m,m->x,m->y+dy))
	movemonster(m,m->x,m->y+dy);
    }
  }
}






/* for spirits (and earth creatures) who can ignore blockages because
   either they are noncorporeal or they can move through stone */
void m_spirit_move(m)
struct monster *m;
{
  int dx = sign(Player.x - m->x);
  int dy = sign(Player.y - m->y);
  erase_monster(m);
  if (m->hp < Monsters[m->id].hp/6) {
    dx = -dx;
    dy = -dy;
  }

  if (Player.status[INVISIBLE] > 0) m_random_move(m);
  else movemonster(m,m->x+dx,m->y+dy);
}


  
/* fluttery dumb movement */
void m_flutter_move(m)
struct monster *m;
{
  int trange,range = distance(m->x,m->y,Player.x,Player.y);
  int i,tx,ty,nx=m->x,ny=m->y;
  erase_monster(m);
  if (Player.status[INVISIBLE] > 0) m_random_move(m);
  else {
    for (i=0;i<8;i++) {
      tx = m->x+Dirs[0][i];
      ty = m->y+Dirs[1][i];
      trange = distance(tx,ty,Player.x,Player.y);
      if (m->hp < Monsters[m->id].hp/6) {
	if ((trange > range) && m_unblocked(m,tx,ty)) {
	  range = trange;
	  nx = tx;
	  ny = ty;
	}
      }
      else if ((trange <= range) && m_unblocked(m,tx,ty)) {
	range = trange;
	nx = tx;
	ny = ty;
      }
    }
    movemonster(m,nx,ny);
  }
}


void m_follow_move(m)
struct monster *m;
{
  if (! m_statusp(m,HOSTILE))
    m_normal_move(m);
  else m_scaredy_move(m);
}



/* allows monsters to fall into pools, revealed traps, etc */
void m_confused_move(m)
struct monster *m;
{
  int i,nx,ny,done=FALSE;
  erase_monster(m);
  for (i=0;((i<8)&&(! done));i++) {
    nx = m->x+random_range(3)-1;
    ny = m->y+random_range(3)-1;
    if (unblocked(nx,ny) && 
	((nx != Player.x) || 
	 (ny != Player.y))) {
      done = TRUE;
      movemonster(m,nx,ny);
    }
  }
}

void m_random_move(m)
struct monster *m;
{
  int i,nx,ny,done=FALSE;
  erase_monster(m);
  for (i=0;((i<8)&&(! done));i++) {
    nx = m->x+random_range(3)-1;
    ny = m->y+random_range(3)-1;
    if (m_unblocked(m,nx,ny) && 
	((nx != Player.x) || 
	 (ny != Player.y))) {
      done = TRUE;
      movemonster(m,nx,ny);
    }
  }
}

    
/* monster removed from play */
void m_vanish(m)
struct monster *m;
{
  if (m->uniqueness == COMMON) {
    strcpy(Str2,"The ");
    strcat(Str2,m->monstring);
  }
  else strcpy(Str2,m->monstring);
  strcat(Str2," vanishes in the twinkling of an eye!");
  mprint(Str2);
  Level->site[m->x][m->y].creature = NULL;
  erase_monster(m);
  m->hp = -1; /* signals "death" -- no credit to payer, though */
}

/* monster still in play */
void m_teleport(m)
struct monster *m;
{
  erase_monster(m);
  if (m_statusp(m,AWAKE)) {
    Level->site[m->x][m->y].creature = NULL;
    putspot(m->x,m->y,getspot(m->x,m->y,FALSE));
    findspace(&(m->x),&(m->y),-1);
    Level->site[m->x][m->y].creature = m;
  }
}
