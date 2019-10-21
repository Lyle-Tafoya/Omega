/* copyright (c) 1987 by Laurence Raphael Brothers */
/* outils.c */

#include <stdio.h>
#include "oglob.h"


/* from oscr */
extern void plotchar(),levelrefresh(),plotspot();
extern char ynq();

int inbounds(),random_range(),hitp(),sign(),max(),min(),distance();
int unblocked(),los_p(),screenmod(),offscreen(),pow2(),calc_points();
int bitp(),nighttime(),hour(),showhour(),day();
int view_unblocked(),m_unblocked();
int view_los_p(),confirmation();
char inversedir(),*getarticle(),*ordinal(),*month();
void initdirs(),do_los(),do_object_los(),bitset(),bitreset();
void findspace(); 

/* from oaux */
extern int item_value();

/* x and y on level? */
int inbounds(x,y)
int x,y;
{
  if ((x<0)||(y<0)||(x>WIDTH-1)||(y>LENGTH-1)) return(FALSE);
  else return(TRUE);
}

int random_range(k)
int k;
{
return( k==0 ? 0 : ((int) random()) % k ) ;
}


/* modify y coord relative to which part of level we are on */
int screenmod(y)
int y;
{
  switch(WhichScreen) {
  case 0:return(y+2);break;
  case 1:return(y+2-EDGE1);break;
  case 2:return(y+2-EDGE2);break;
  case 3:return(y+2-EDGE3);break;
  }
}


int offscreen(y)
int y;
{
  switch(WhichScreen) {
  case 0: return((y<0)||(y>EDGE1+1));
  case 1: return((y<EDGE1-1)||(y>EDGE2+1));
  case 2: return((y<EDGE2-1)||(y>EDGE3+1));
  case 3: return((y<EDGE3-1)||(y>BOTTOMEDGE));
  }
}

int hitp(hit,ac)
int hit,ac;
{
  int roll = random_range(20);
  if (roll == 0) return(TRUE);
  else if (roll == 19) return(FALSE);
else return((roll < (hit - ac)) ? TRUE : FALSE );
}



/* this function doesnt seem to exist by itself... */
int sign(n)
int n;
{
  if (n == 0) return(0);
  else return((n < 0) ? -1 : 1);
}


/* why load the math library.... */
int max(a,b)
int a,b;
{
  return((a>b) ? a : b);
}

/* why load the math library.... */
int min(a,b)
int a,b;
{
  return((a<b) ? a : b);
}


/* number of moves from x1,y1 to x2,y2 */
int distance(x1,y1,x2,y2)
int x1,y1,x2,y2;
{
  return(max(abs(x2-x1),abs(y2-y1)));
}


/* can you shoot, or move monsters through a spot? */
int unblocked(x,y)
int x,y;
{
  if ((! inbounds(x,y)) ||
      (Dungeon[Dlevel][x][y].creature != NULL) ||
      (Dungeon[Dlevel][x][y].locchar == WALL) ||
      (Dungeon[Dlevel][x][y].locchar == STATUE) ||
      (Dungeon[Dlevel][x][y].locchar == HEDGE) ||
      (Dungeon[Dlevel][x][y].locchar == WHIRLWIND) ||
      (Dungeon[Dlevel][x][y].locchar == CLOSED_DOOR) ||
      (Dungeon[Dlevel][x][y].secret) ||
      ((x==Player.x) && (y==Player.y)))
    return(FALSE);
  else 
    return(TRUE);
}


/* do monsters want to move through a spot */
int m_unblocked(m,x,y)
struct monster *m;
int x,y;
{
  if ((! inbounds(x,y)) ||
      (Dungeon[Dlevel][x][y].creature != NULL) ||
      (Dungeon[Dlevel][x][y].locchar == WALL) ||
      (Dungeon[Dlevel][x][y].locchar == STATUE) ||
      (Dungeon[Dlevel][x][y].locchar == WHIRLWIND) ||
      (Dungeon[Dlevel][x][y].secret) ||
      ((x==Player.x) && (y==Player.y)))
    return(FALSE);
  else if (m_statusp(m,SWIMMING) &&
	   (Dungeon[Dlevel][m->x][m->y].locchar==WATER) &&
	   (Dungeon[Dlevel][x][y].locchar==WATER))
    return(FALSE);
  else if (Dungeon[Dlevel][x][y].locchar == CLOSED_DOOR) {
    if ((m->movef==M_MOVE_SPIRIT) || (m->movef==M_MOVE_SMART))
      return(TRUE);
    else return(FALSE);
  }
  else if (m_statusp(m,FLYING)) return(TRUE);
  else if (Dungeon[Dlevel][x][y].locchar == LAVA)
    return(m_immunityp(m,FLAME) && m_statusp(m,SWIMMING));
  else if (Dungeon[Dlevel][x][y].locchar == WATER)
    return(m_statusp(m,SWIMMING));
  else if (Dungeon[Dlevel][x][y].locchar == FIRE)
    return(m_immunityp(m,FLAME));
  else if ((Dungeon[Dlevel][x][y].locchar == TRAP) ||
	   (Dungeon[Dlevel][x][y].locchar == HEDGE) ||
	   (Dungeon[Dlevel][x][y].locchar == ABYSS))
    return(FALSE);
  else return(TRUE);
}



/* can you see through a spot? */
int view_unblocked(x,y)
int x,y;
{
  if (! inbounds(x,y)) return(FALSE);
  else if ((Dungeon[Dlevel][x][y].locchar == WALL) ||
	   (Dungeon[Dlevel][x][y].locchar == STATUE) ||
	   (Dungeon[Dlevel][x][y].locchar == HEDGE) ||
	   (Dungeon[Dlevel][x][y].locchar == FIRE) ||
	   (Dungeon[Dlevel][x][y].locchar == WHIRLWIND) ||
	   (Dungeon[Dlevel][x][y].locchar == CLOSED_DOOR) ||
	   (Dungeon[Dlevel][x][y].secret))
    return(FALSE);
  else 
    return(TRUE);
}


/* 8 moves in Dirs */
void initdirs()
{
  Dirs[0][0] = 1;
  Dirs[0][1] = 1;
  Dirs[0][2] = -1;
  Dirs[0][3] = -1;
  Dirs[0][4] = 1;
  Dirs[0][5] = -1;
  Dirs[0][6] = 0;
  Dirs[0][7] = 0;
  Dirs[0][8] = 0;
  Dirs[1][0] = 1;
  Dirs[1][1] = -1;
  Dirs[1][2] = 1;
  Dirs[1][3] = -1;
  Dirs[1][4] = 0;
  Dirs[1][5] = 0;
  Dirs[1][6] = 1;
  Dirs[1][7] = -1;
  Dirs[1][8] = 0;
}





/* do_los moves pyx along a lineofsight from x1 to x2 */
/* x1 and x2 are pointers because as a side effect they are changed */
/* to the final location of the pyx */
void do_los(pyx,x1,y1,x2,y2)
char pyx;
int *x1,*y1,x2,y2;
{
  int ox,oy,sx,sy,v;

  do {
    ox = *x1;
    oy = *y1;
    sx=sign(x2-*x1);
    sy=sign(y2-*y1);
    if (abs(x2-*x1) > abs(y2-*y1))
      *x1 += sx;
    else if (abs(x2-*x1) < abs(y2-*y1))
      *y1 += sy;
    else {
      *x1 += sx;
      *y1 += sy;
    }
    Dungeon[Dlevel][*x1][*y1].showchar = pyx;
    /* delay enough to make pyx visible */
    for(v=1;v<10;v++) plotchar(pyx,*x1,*y1);
    plotspot(ox,oy,TRUE);
  } while(unblocked(*x1,*y1) && ((*x1 != x2) || (*y1 != y2)));
  
  plotspot(*x1,*y1,TRUE);
  levelrefresh();
}


/* This is the same as do_los, except we stop before hitting nonliving
obstructions */
void do_object_los(pyx,x1,y1,x2,y2)
char pyx;
int *x1,*y1,x2,y2;
{
  int ox,oy,sx,sy,v;

  do {
    ox = *x1;
    oy = *y1;
    sx=sign(x2-*x1);
    sy=sign(y2-*y1);
    if (abs(x2-*x1) > abs(y2-*y1))
      *x1 += sx;
    else if (abs(x2-*x1) < abs(y2-*y1))
      *y1 += sy;
    else {
      *x1 += sx;
      *y1 += sy;
    }
    if (unblocked(*x1,*y1)) {
      for(v=1;v<10;v++) plotchar(pyx,*x1,*y1);
      Dungeon[Dlevel][*x1][*y1].showchar = pyx;
    }
    plotspot(ox,oy,TRUE);
  } while(unblocked(*x1,*y1) && ((*x1 != x2) || (*y1 != y2)));
  
  if ((! unblocked(*x1,*y1)) && (Dungeon[Dlevel][*x1][*y1].creature == NULL)) {
    *x1 = ox;
    *y1 = oy;
  }
  plotspot(*x1,*y1,TRUE);
  levelrefresh();
}
      
    


/* los_p checks to see whether there is an unblocked los from x1,y1 to x2,y2 */
int los_p(x1,y1,x2,y2)
int x1,y1,x2,y2;
{
  int sx,sy;
  do {
    sx=sign(x2-x1);
    sy=sign(y2-y1);
    if (abs(x2-x1) > abs(y2-y1))
      x1 += sx;
    else if (abs(x2-x1) < abs(y2-y1))
      y1 += sy;
    else {
      x1 += sx;
      y1 += sy;
    }
  } while (unblocked(x1,y1) && ((x1 != x2) || (y1 != y2)));
  return((x1==x2) && (y1==y2));
}


/* view_los_p sees through monsters */
int view_los_p(x1,y1,x2,y2)
int x1,y1,x2,y2;
{
  int sx,sy;
  do {
    sx=sign(x2-x1);
    sy=sign(y2-y1);
    if (abs(x2-x1) > abs(y2-y1))
      x1 += sx;
    else if (abs(x2-x1) < abs(y2-y1))
      y1 += sy;
    else {
      x1 += sx;
      y1 += sy;
    }
  } while (view_unblocked(x1,y1) && ((x1 != x2) || (y1 != y2)));
  return((x1==x2) && (y1==y2));
}




/* why load the math library.... */
int pow2(n)
int n;
{
  switch(n) {
    case 0: return(1);break;
    case 1: return(2);break;
    case 2: return(4);break;
    case 3: return(8);break;
    case 4: return(16);break;
    case 5: return(32);break;
    case 6: return(64);break;
    case 7: return(128);break;
    case 8: return(256);break;
    case 9: return(512);break;
    case 10: return(1024);break;
    case 11: return(2048);break;
    case 12: return(4096);break;
    case 13: return(8192);break;
    case 14: return(16384);break;
    case 15: return(32768);break;
    default: return(-1);break;
  }
}


int bitp(bv,flag)
int bv,flag;
{
  return(bv & flag);
}

void bitset(pbv,flag)
int *pbv,flag;
{
  *pbv |= flag;
}

void bitreset(pbv,flag)
int *pbv,flag;
{
  *pbv -= ( bitp(*pbv,flag) ? flag : 0);
}




/* returns the command direction from the index into Dirs */
char inversedir(dirindex)
int dirindex;	  
{
  switch (dirindex) {
    case 0:return('n');break;
    case 1:return('u');break;
    case 2:return('b');break;
    case 3:return('y');break;
    case 4:return('l');break;
    case 5:return('h');break;
    case 6:return('j');break;
    case 7:return('k');break;
  }
}


int calc_points()
{
  int i,points=0;
  
  for(i=0;i<MAXITEMS;i++)
    if (Player.possessions[i] != NULL)
      points += Player.possessions[i]->level;
  points += Deepest*5;
  points += 10*Player.level;
  for (i=0;i<NUMRANKS;i++)
    points += 10*Player.rank[i];
  if (Player.hp < 0)
    points = (points / 2);
  else if (Player.rank[ADEPT])
    points *= 10;
  return(points);
}

int hour()
{
  return(((int) ((Time+1200) / 100)) % 24);
}

int showhour()
{
  int showtime;
  if ((hour() == 0) || (hour() == 12)) showtime = 12;
  else showtime = (hour() % 12);
  return(showtime);
}

int nighttime()
{
  return((hour() > 20) || (hour() < 7));
}

char *getarticle(str)
char *str;
{
  if ((str[0]=='a') ||
      (str[0]=='e') ||
      (str[0]=='i') ||
      (str[0]=='o') ||
      (str[0]=='u') ||
      (str[0]=='h'))
    return("an ");
  else return("a ");
}

int day()
{
  return ((Date % 30) + 1);
}

char *ordinal(number)
int number;
{
  if ((number == 11) || (number == 12) || (number == 13)) return("th");
  else  switch(number % 10) {
    case 1:return("st");break;
    case 2:return("nd");break;
    case 3:return("rd");break;
    default: return("th");break;
  }
}
	   
char *month()
{
  switch((Date % 360) / 30) {
    case 0: return("Freeze"); break;
    case 1: return("Ice"); break;
    case 2: return("Mud"); break;
    case 3: return("Storm"); break;
    case 4: return("Breeze"); break;
    case 5: return("Light"); break;
    case 6: return("Flame"); break;
    case 7: return("Broil"); break;
    case 8: return("Cool"); break;
    case 9: return("Haunt"); break;
    case 10: return("Chill"); break;
    case 11: return("Dark"); break;
    case 12: return("Twixt"); break;
  }
}


/* finds an empty space, sets x,y */
void findspace(x,y)
int *x,*y;
{
  do {
    *x = random_range(WIDTH);
    *y = random_range(LENGTH);
  } while ((Dungeon[Dlevel][*x][*y].locchar != FLOOR) ||
	   (Dungeon[Dlevel][*x][*y].creature != NULL));
}

/* is prefix a prefix of s? */
int strprefix(prefix,s)
char *prefix,*s;
{
  int i=0,matched=TRUE;
  if (strlen(prefix) > strlen(s)) return(FALSE);
  else {
    while (matched && (i<strlen(prefix))) {
      matched = (prefix[i] == s[i]);
      i++;
    }
    return(matched);
  }
}

int confirmation()
{
  switch(random_range(4)) {
  case 0:  mprint("Are you sure? [yn]"); break;
  case 1:  mprint("Certain about that? [yn]"); break;
  case 2:  mprint("Do you really mean it? [yn]"); break;
  case 3:  mprint("Confirm that, would you? [yn]"); break;
  }
  return(ynq()=='y');
}
