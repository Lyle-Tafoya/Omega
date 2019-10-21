/* copyright (c) 1987,1988 by Laurence Raphael Brothers */
/* outils.c */

/* Random utility functions called from all over */

#include "oglob.h"

/* x and y on level? */
int inbounds(x,y)
int x,y;
{
  return((x>=0)&&(y>=0)&&(x<WIDTH)&&(y<LENGTH));
}

/* RANDFUNCTION is defined in odefs.h */
int random_range(k)
int k;
{
  return( k==0 ? 0 : (int) RANDFUNCTION % k ) ;
}


/* modify absolute y coord relative to which part of level we are on */
int screenmod(y)
int y;
{
  return(y-ScreenOffset);
}


int offscreen(y)
int y;
{
  return((y<0)||
	 (y<ScreenOffset) ||
	 (y>ScreenOffset+ScreenLength-1) ||
	 (y>LENGTH));
}


/* always hit on a natural 0; never hit on a natural 19 */
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
      (Level->site[x][y].creature != NULL) ||
      (Level->site[x][y].locchar == WALL) ||
      (Level->site[x][y].locchar == PORTCULLIS) ||
      (Level->site[x][y].locchar == STATUE) ||
      (Level->site[x][y].locchar == HEDGE) ||
      (Level->site[x][y].locchar == CLOSED_DOOR) ||
      loc_statusp(x,y,SECRET) ||
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
  if ((! inbounds(x,y)) || ((x==Player.x) && (y==Player.y)))
    return(FALSE);
  else if ((Level->site[x][y].creature != NULL) ||
	   (Level->site[x][y].locchar == SPACE)) return(FALSE);
  else if (m_statusp(m,ONLYSWIM)) 
    return(Level->site[x][y].locchar == WATER);
  else if ((Level->site[x][y].locchar == FLOOR) ||
	   (Level->site[x][y].locchar == OPEN_DOOR))
    return(TRUE);
  else if ((Level->site[x][y].locchar == PORTCULLIS) ||
	   (Level->site[x][y].locchar == WALL) ||
	   (Level->site[x][y].locchar == STATUE) ||
	   loc_statusp(x,y,SECRET))
    return(m_statusp(m,INTANGIBLE));
  else if (Level->site[x][y].locchar==WATER)
    return(m_statusp(m,SWIMMING) || 
	   m_statusp(m,ONLYSWIM) ||
	   m_statusp(m,FLYING));
  else if (Level->site[x][y].locchar == CLOSED_DOOR) {
    if (m->movef==M_MOVE_SMART) {
      mprint("You hear a door creak open.");
      Level->site[x][y].locchar = OPEN_DOOR;
      return(TRUE);
    }
    else if (random_range(m->dmg) > random_range(100)) {
      mprint("You hear a door shattering.");
      Level->site[x][y].locchar = RUBBLE;
      return(TRUE);
    }
    else return(m_statusp(m,INTANGIBLE));
  }
  else if (Level->site[x][y].locchar == LAVA)
    return((m_immunityp(m,FLAME) && 
	    m_statusp(m,SWIMMING)) ||
	   m_statusp(m,FLYING));
  else if (Level->site[x][y].locchar == FIRE)
    return(m_immunityp(m,FLAME));
  else if ((Level->site[x][y].locchar == TRAP) ||
	   (Level->site[x][y].locchar == HEDGE) ||
	   (Level->site[x][y].locchar == ABYSS))
    return((m->movef == M_MOVE_CONFUSED) ||
	   m_statusp(m,FLYING));
  else return(TRUE);
}



/* can you see through a spot? */
int view_unblocked(x,y)
int x,y;
{
  if (! inbounds(x,y)) return(FALSE);
  else if ((Level->site[x][y].locchar == WALL) ||
	   (Level->site[x][y].locchar == STATUE) ||
	   (Level->site[x][y].locchar == HEDGE) ||
	   (Level->site[x][y].locchar == FIRE) ||
	   (Level->site[x][y].locchar == CLOSED_DOOR) ||
	   loc_statusp(x,y,SECRET))
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
    Level->site[*x1][*y1].showchar = pyx;
    /* delay enough to make pyx visible */
    for(v=1;v<10;v++) plotchar(pyx,*x1,*y1);
    plotspot(ox,oy,FALSE);
  } while(unblocked(*x1,*y1) && ((*x1 != x2) || (*y1 != y2)));
  
  plotspot(*x1,*y1,FALSE);
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
      Level->site[*x1][*y1].showchar = pyx;
    }
    plotspot(ox,oy,TRUE);
  } while(unblocked(*x1,*y1) && ((*x1 != x2) || (*y1 != y2)));
  
  if ((! unblocked(*x1,*y1)) && (Level->site[*x1][*y1].creature == NULL)) {
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



int gamestatusp(flag)
long flag;
{
  return(GameStatus&flag);
}

void setgamestatus(flag)
long flag;
{
  GameStatus |= flag;
}

void resetgamestatus(flag)
long flag;
{
  GameStatus &= ~flag;
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
  
  if (gamestatusp(SPOKE_TO_DRUID)) points += 50;
  if (gamestatusp(COMPLETED_CAVES)) points += 100;
  if (gamestatusp(COMPLETED_SEWERS)) points += 200;
  if (gamestatusp(COMPLETED_CASTLE)) points += 300;
  if (gamestatusp(COMPLETED_ASTRAL)) points += 400;
  if (gamestatusp(COMPLETED_VOLCANO)) points += 500;
  if (gamestatusp(KILLED_DRAGONLORD)) points += 100;
  if (gamestatusp(KILLED_EATER)) points += 100;
  if (gamestatusp(KILLED_LAWBRINGER)) points += 100;

  points += Player.xp/50;

  points += Player.cash/500;

  for (i=0;i<MAXITEMS;i++) 
    if (Player.possessions[i] != NULL)
      points += Player.possessions[i]->level*(Player.possessions[i]->known+1);

  for (i=0;i<MAXPACK;i++) 
    if (Player.pack[i] != NULL)
      points += Player.pack[i]->level*(Player.pack[i]->known+1);

  for (i=0;i<NUMRANKS;i++) {
    if (Player.rank[i] == 5) points += 500;
    else points += 20*Player.rank[i];
  }
  
  if (Player.hp < 1)
    points = (points / 2);
  
  else if (Player.rank[ADEPT])
    points *= 10;
  
  return(points);
}


/* returns the 24 hour clock hour */
int hour()
{
  return(((Time+720) / 60) % 24);
}

/* returns 0, 10, 20, 30, 40, or 50 */
int showminute()
{
  return(((Time % 60)/10)*10);
}

/* returns the 12 hour clock hour */
int showhour()
{
  int showtime;
  if ((hour() == 0) || (hour() == 12)) showtime = 12;
  else showtime = (hour() % 12);
  return(showtime);
}

/* nighttime is defined from 9 PM to 6AM */
int nighttime()
{
  return((hour() > 20) || (hour() < 7));
}

char *getarticle(str)
char *str;
{
  if ((str[0]=='a') || (str[0]=='A') ||
      (str[0]=='e') || (str[0]=='E') ||
      (str[0]=='i') || (str[0]=='I') ||
      (str[0]=='o') || (str[0]=='O') ||
      (str[0]=='u') || (str[0]=='U') ||
      (((str[0]=='h') || (str[0]=='H')) && 
       ((str[1]=='i') || (str[1]=='e'))))
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


/* finds floor space on level with buildaux not equal to baux, 
sets x,y there. There must *be* floor space somewhere on level.... */

void findspace(x,y,baux)
int *x,*y;
{
  int i,j,k,l,done=FALSE;
  i = k = random_range(WIDTH);
  j = l = random_range(LENGTH);
  do {
    i++;
    if (i >= WIDTH) {
      i = 0;
      j++;
      if (j > LENGTH)
	j = 0;
      done = ((i == k) && (j == l));
    }
    done = done || 
      ((Level->site[i][j].locchar == FLOOR) &&
       (Level->site[i][j].creature == NULL) &&
       (Level->site[i][j].buildaux != baux));
  } while (! done);
  *x = i;
  *y = j;
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
  case 0:  mprint("Are you sure? [yn] "); break;
  case 1:  mprint("Certain about that? [yn] "); break;
  case 2:  mprint("Do you really mean it? [yn] "); break;
  case 3:  mprint("Confirm that, would you? [yn] "); break;
  }
  return(ynq()=='y');
}


/* is character c a member of string s */
int strmem(c,s)
char c;
char *s;
{
  int i,found=FALSE;
  for(i=0;((i<strlen(s)) && (! found));i++)
    found = (s[i] == c);
  return(found);
}


/* returns true if its ok to get rid of a level */
int ok_to_free(level)
plv level;
{
  if (level == NULL) return(FALSE);
  else return((level->environment != E_CITY) &&
	      (level->environment != E_VILLAGE) &&
	      (level->environment != Current_Dungeon));
}


void calc_weight()
{
  int i,weight=0;

  for(i=1;i<MAXITEMS;i++) 
    if (Player.possessions[i] != NULL)
      weight += Player.possessions[i]->weight *
	Player.possessions[i]->number;

  if ((Player.possessions[O_WEAPON_HAND] != NULL) &&
      (Player.possessions[O_READY_HAND] == Player.possessions[O_WEAPON_HAND]))
    weight -= Player.possessions[O_READY_HAND]->weight *
      Player.possessions[O_READY_HAND]->number;
      
  for(i=1;i<MAXPACK;i++) 
    if (Player.pack[i] != NULL)
      weight += Player.pack[i]->weight *
	Player.pack[i]->number;

  Player.itemweight = weight;

  dataprint();
}
	  
/* alloc just enough string space for str, strcpy, and return pointer */
char *salloc(str)
char *str;
{
  char *s=calloc((unsigned)(strlen(str)+1),sizeof(char));
  strcpy(s,str);
  return(s);
}
