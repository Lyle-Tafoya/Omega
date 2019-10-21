/* omega (c) 1987,1988,1989 by Laurence Raphael Brothers */
/* scr.c */
/* functions that use curses routines directly */
/* plus a few file i/o stuff */
/* also some in file.c */

#ifdef MSDOS_SUPPORTED_ANTIQUE
# include "curses.h"
#else
# ifdef AMIGA
#  include <curses210.h>
# else
#  include <curses.h>
# endif
# include <sys/types.h>
#endif

#if defined(MSDOS_SUPPORTED_ANTIQUE) || defined(AMIGA)
# define CHARATTR(c)	((c) >> 8)
#else
# define CHARATTR(c)	((c) & ~0xff)
#endif

#include "glob.h"

#ifdef EXCESSIVE_REDRAW
#undef wclear
#define wclear werase
#endif


/* note these variables are not exported to other files */

WINDOW *Levelw,*Dataw,*Flagw,*Timew,*Menuw,*Locw,*Morew,*Phasew;
WINDOW *Comwin,*Msg1w,*Msg2w,*Msg3w,*Msgw;
WINDOW *Showline[MAXITEMS];

void phaseprint()
{
  wclear(Phasew);
  wprintw(Phasew,"Moon's Phase:\n");
  switch(Phase/2) {
  case 0: wprintw(Phasew,"NEW"); break;
  case 1: case 11: wprintw(Phasew,"CRESCENT"); break;
  case 2: case 10: wprintw(Phasew,"1/4"); break;
  case 3: case 9: wprintw(Phasew,"HALF"); break;
  case 4: case 8: wprintw(Phasew,"3/4"); break;
  case 5: case 7: wprintw(Phasew,"GIBBOUS"); break;
  case 6: wprintw(Phasew,"FULL"); break;
  }
  wrefresh(Phasew);
}

void show_screen()
{
  int i,j,top,bottom;
  int last_attr = 0, c;

  wclear(Levelw);
  top = ScreenOffset;
  bottom = ScreenOffset + ScreenLength;
  top = max(0,top);
  bottom = min(bottom,LENGTH);
  if (Current_Environment != E_COUNTRYSIDE) 
    for (j=top;j<bottom;j++) {
      wmove(Levelw,screenmod(j),0);
      for (i=0;i<WIDTH;i++) {
	c = ((loc_statusp(i,j,SEEN)) ? getspot(i,j,FALSE):SPACE);
        if (optionp(SHOW_COLOUR) && CHARATTR(c) != last_attr) {
          last_attr = CHARATTR(c);
          wattrset(Levelw,last_attr);
        }
        waddch(Levelw,c&0xff);
      }
    }
  else for (j=top;j<bottom;j++)
      for (i=0;i<WIDTH;i++) {
	wmove(Levelw,screenmod(j),i);
        c = ((c_statusp(i,j,SEEN)) ? Country[i][j].current_terrain_type:SPACE);
        if (optionp(SHOW_COLOUR) && CHARATTR(c) != last_attr) {
          last_attr = CHARATTR(c);
          wattrset(Levelw,last_attr);
        }
        waddch(Levelw,c&0xff);
      }
  wrefresh(Levelw);
}



char mgetc()
{
  return(wgetch(Msgw));
}

/* case insensitive mgetc -- sends uppercase to lowercase */
int mcigetc()
{
  int c;

#ifdef MSDOS_SUPPORTED_ANTIQUE
#ifndef DJGPP
  keypad(Msgw,TRUE);
#endif
#endif
  c = wgetch(Msgw);
  if ((c>=(int)'A') && (c<=(int)'Z'))
    return(c+(int)('a'-'A'));
  else return(c);
}

char menugetc()
{
  return(wgetch(Menuw));
}


char lgetc()
{
  return(wgetch(Levelw));
}


int ynq()
{
  char p='*'; /* the user's choice; start with something impossible
               * to prevent a loop. */
  while ((p != 'n') && (p != 'y') && (p != 'q') && (p != ESCAPE) &&
         (p != EOF) && (p != ' '))
    p = wgetch(Msgw);
  switch (p) {
    case 'y': wprintw(Msgw,"yes. "); break;
    case 'n': wprintw(Msgw,"no. "); break;
    
    case ESCAPE: p = 'q'; /* fall through to 'q' */
    case ' ': p = 'q';    /* fall through to 'q' */
    case 'q': wprintw(Msgw,"quit. "); break;
    default: assert( p == EOF );
    }
  wrefresh(Msgw);
  return(p);
}


int ynq1()
{
  char p='*'; /* the user's choice; start with something impossible
               * to prevent a loop. */
  while ((p != 'n') && (p != 'y') && (p != 'q') && (p != ESCAPE) &&
          (p != ' ') && (p != EOF))
    p = wgetch(Msg1w);
  switch (p) {
    case 'y': wprintw(Msg1w,"yes. "); break;
    case 'n': wprintw(Msg1w,"no. "); break;
    
    case ESCAPE: p = 'q'; /* fall through to 'q' */
    case ' ': p = 'q';    /* fall through to 'q' */
    case 'q': wprintw(Msg1w,"quit. "); break;
    default: assert( p == EOF );
    }
  wrefresh(Msg1w);
  return(p);
}


int ynq2()
{
  char p='*'; /* the user's choice; start with something impossible
               * to prevent a loop. */
  while ((p != 'n') && (p != 'y') && (p != 'q') && (p != ESCAPE) &&
          (p != ' ') && (p != EOF))
    p = wgetch(Msg2w);
  switch (p) {
    case 'y': wprintw(Msg2w,"yes. "); break;
    case 'n': wprintw(Msg2w,"no. "); break;
    
    case ESCAPE: p = 'q'; /* fall through to 'q' */
    case ' ': p = 'q';    /* fall through to 'q' */
    case 'q': wprintw(Msg2w,"quit. "); break;
    default: assert( p == EOF );
    }
  wrefresh(Msg2w);
  return(p);
}
    
/* puts up a morewait to allow reading if anything in top two lines */
void checkclear()
{
  int x1,y,x2;
  getyx(Msg1w,x1,y);
  getyx(Msg2w,x2,y);  
  if ((x1 != 0) || (x2 != 0)) {
    morewait();
    wclear(Msg1w);
    wclear(Msg2w);
    wrefresh(Msg1w);
    wrefresh(Msg2w);
  }
}
  
/* for external call */
void clearmsg()
{
  wclear(Msg1w);
  wclear(Msg2w);
  wclear(Msg3w);
  Msgw = Msg1w;
  wrefresh(Msg1w);
  wrefresh(Msg2w);
  wrefresh(Msg3w);
}

void clearmsg3()
{
  wclear(Msg3w);
  wrefresh(Msg3w);
}

void clearmsg1()
{
  wclear(Msg1w);
  wclear(Msg2w);
  Msgw = Msg1w;
  wrefresh(Msg1w);
  wrefresh(Msg2w);
}


void erase_level()
{
  wclear(Levelw);
  wrefresh(Levelw);
}

/* direct print to first msg line */
void print1(s)
char *s;
{
  if (! gamestatusp(SUPPRESS_PRINTING)) {
    buffercycle(s);
    wclear(Msg1w);
    wprintw(Msg1w,s);
    wrefresh(Msg1w);
  }
}

/* for run on-messages -- print1 clears first.... */
void nprint1(s)
char *s;
{
  if (! gamestatusp(SUPPRESS_PRINTING)) {
    if (bufferappend(s)) {
      wprintw(Msg1w,s);
      wrefresh(Msg1w);
    }
  }
}




/* direct print to second msg line */
void print2(s)
char *s;
{
  if (! gamestatusp(SUPPRESS_PRINTING)) {
    buffercycle(s);
    wclear(Msg2w);
    wprintw(Msg2w,s);
    wrefresh(Msg2w);
  }
}

/* for run on-messages -- print2 clears first.... */
void nprint2(s)
char *s;
{
  if (! gamestatusp(SUPPRESS_PRINTING)) {
    if (bufferappend(s)) {
      wprintw(Msg2w,s);
      wrefresh(Msg2w);
    }
  }
}




/* msg line 3 is not part of the region that mprint or printm can reach */
/* typical use of print3 is for "you can't do that" type error messages */
void print3(s)
char *s;
{
  if (! gamestatusp(SUPPRESS_PRINTING)) {
    buffercycle(s);
    wclear(Msg3w);
    wprintw(Msg3w,s);
    wrefresh(Msg3w);
  }
}

/* for run on-messages -- print3 clears first.... */
void nprint3(s)
char *s;
{
  if (! gamestatusp(SUPPRESS_PRINTING)) {
    if (bufferappend(s)) {
      wprintw(Msg3w,s);
      wrefresh(Msg3w);
    }
  }
}



/* prints wherever cursor is in window, but checks to see if
it should morewait and clear window */
void mprint(s)
char *s;
{
  int x,y;
  if (! gamestatusp(SUPPRESS_PRINTING)) {
    getyx(Msgw,y,x);
    if (x+strlen(s) >= WIDTH) {
      buffercycle(s);
      if (Msgw == Msg1w) {
	wclear(Msg2w);
	Msgw = Msg2w;
      }
      else {
	morewait();
	wclear(Msg1w);
	wclear(Msg2w);
	wrefresh(Msg2w);
	Msgw = Msg1w;
      }
    }
    else if (x > 0)
      bufferappend(s);
    else
      buffercycle(s);
    wprintw(Msgw,s);
    waddch(Msgw,' '); 
    wrefresh(Msgw);
  }
}




void omega_title()
{
  showmotd();
  clear();
  touchwin(stdscr);
  refresh();  
/*  showscores();*/ /* DG */
}





/* blanks out ith line of Menuw or Levelw */
void hide_line(i)
int i;
{
  wclear(Showline[i]);
  touchwin(Showline[i]);
  wrefresh(Showline[i]);
}



/* initialize, screen, windows */
void initgraf()
{
  int i;
  initscr();
#ifndef MSDOS_SUPPORTED_ANTIQUE
  start_color();
# ifndef AMIGA
  clrgen_init();
# endif
#endif
  if (LINES < 24 || COLS < 80) {
    printf("Minimum Screen Size: 24 Lines by 80 Columns.");
    exit(0);
  }
#ifdef AMIGA
  init_color(1, 800, 800, 800); /* white */
  init_color(2, 644, 164, 164); /* brown */
  init_color(3, 800, 800, 0); /* yellow */
  init_color(4, 200, 200, 200); /* grey */
  init_color(5, 0, 1000, 0); /* green */
  init_color(6, 0, 0, 1000); /* blue */
  init_color(7, 1000, 0, 0); /* red */
  LINES -= 2;	/* ugly, but neccessary with this curses package... */
#endif
  ScreenLength = LINES - 6;
  Msg1w = newwin(1,80,0,0);
  scrollok(Msg1w, 0);	/* DJGPP curses defaults to scrollable new windows */
  Msg2w = newwin(1,80,1,0);
  scrollok(Msg2w, 0);
  Msg3w = newwin(1,80,2,0);
  scrollok(Msg3w, 0);
  Msgw = Msg1w;
  Morew = newwin(1,15,3,65);  
  scrollok(Morew, 0);
  Locw = newwin(1,80,ScreenLength+3,0);
  scrollok(Locw, 0);
  Levelw = newwin(ScreenLength,64,3,0);
  scrollok(Levelw, 0);
  for(i=0;i<MAXITEMS;i++) {
    Showline[i] = newwin(1,64,i+3,0);
    scrollok(Showline[i], 0);
    wclear(Showline[i]);
  }
  Menuw = newwin(ScreenLength,64,3,0);
  scrollok(Menuw, 0);
  Dataw = newwin(2,80,ScreenLength+4,0);
  scrollok(Dataw, 0);
  Timew = newwin(2,15,4,65);
  scrollok(Timew, 0);
  Phasew = newwin(2,15,6,65);
  scrollok(Phasew, 0);
  Flagw = newwin(4,15,9,65);
  scrollok(Flagw, 0);
  Comwin = newwin(8,15,14,65);
  scrollok(Comwin, 0);

  noecho();
  crmode();

  clear();
  touchwin(stdscr);
/*  omega_title();*/
/*  clear();*/
/*  touchwin(stdscr);*/
/*  refresh();*/          /* DG */
}





int lastx= -1,lasty= -1;

void drawplayer()
{
  int c;

  if (Current_Environment == E_COUNTRYSIDE) {
    if (inbounds(lastx,lasty) && !offscreen(lasty)) {
	wmove(Levelw,screenmod(lasty),lastx);
        c = Country[lastx][lasty].current_terrain_type;
        if (optionp(SHOW_COLOUR))
          wattrset(Levelw, CHARATTR(c));
        waddch(Levelw,(c&0xff));
    }
    wmove(Levelw,screenmod(Player.y),Player.x);
    if (optionp(SHOW_COLOUR))
      wattrset(Levelw, CHARATTR(PLAYER));
    waddch(Levelw,(PLAYER&0xff));
  }
  else {
    if (inbounds(lastx,lasty) && !offscreen(lasty))
      plotspot(lastx,lasty,(Player.status[BLINDED]>0 ? FALSE : TRUE));
    wmove(Levelw,screenmod(Player.y),Player.x);
    if ((! Player.status[INVISIBLE]) || Player.status[TRUESIGHT]) {
      if (optionp(SHOW_COLOUR))
        wattrset(Levelw, CHARATTR(PLAYER));
      waddch(Levelw,(PLAYER&0xff));
    }
  }
  lastx = Player.x;
  lasty = Player.y;
}

void setlastxy(new_x, new_y) /* used when changing environments */
int new_x, new_y;
{
    lastx = new_x;
    lasty = new_y;
}

int litroom(x,y)
int x,y;
{
  if (Level->site[x][y].roomnumber < ROOMBASE) return(FALSE);
  else return(loc_statusp(x,y,LIT) ||
	      Player.status[ILLUMINATION]);
}

void drawvision(x,y)
int x,y;
{
  static int oldx = -1,oldy = -1;
  int i,j,c;

  if (Current_Environment != E_COUNTRYSIDE) {
    if (Player.status[BLINDED]) {
      drawspot(oldx,oldy);
      drawspot(x,y);
      drawplayer();
    }
    else {
      if (Player.status[ILLUMINATION] > 0) {
	for (i= -2;i<3;i++)
	  for (j= -2;j<3;j++)
	    if (inbounds(x+i,y+j))
	      if (view_los_p(x+i,y+j,Player.x,Player.y))
		dodrawspot(x+i,y+j);
      }
      else {
	for (i= -1;i<2;i++)
	  for (j= -1;j<2;j++)
	    if (inbounds(x+i,y+j))
	      dodrawspot(x+i,y+j);
      }
      drawplayer();
      drawmonsters(FALSE); /* erase all monsters */
      drawmonsters(TRUE);  /* draw those now visible */
    }
    if ((! gamestatusp(FAST_MOVE)) || (! optionp(JUMPMOVE)))
      omshowcursor(Player.x,Player.y);
    oldx = x;
    oldy = y;
  }
  else {
    for (i= -1;i<2;i++)
      for (j= -1;j<2;j++)
	if (inbounds(x+i,y+j)) {
	  c_set(x+i, y+j, SEEN);
	  if (!offscreen(y+j)) {
	    wmove(Levelw,screenmod(y+j),x+i);
            c = Country[x+i][y+j].current_terrain_type;
            if (optionp(SHOW_COLOUR))
              wattrset(Levelw, CHARATTR(c));
            waddch(Levelw,(c&0xff));
	  }
	}
    drawplayer();
    omshowcursor(Player.x,Player.y);
  }
}


void omshowcursor(x,y)
int x,y;
{
  if (! offscreen(y)) {
    wmove(Levelw,screenmod(y),x);
    wrefresh(Levelw);
  }
}

void levelrefresh()
{
  wrefresh(Levelw);
}


/* draws a particular spot under if in line-of-sight */
void drawspot(x,y)
int x,y;
{
  Symbol c;
  if (inbounds(x,y)) {
    c = getspot(x,y,FALSE);
    if (c != Level->site[x][y].showchar)
      if (view_los_p(Player.x,Player.y,x,y)) {
	lset(x,y,SEEN);
	Level->site[x][y].showchar = c;
	putspot(x,y,c);
      }
  }
}



/* draws a particular spot regardless of line-of-sight */
void dodrawspot(x,y)
int x,y;
{
  Symbol c;
  if (inbounds(x,y)) {
    c = getspot(x,y,FALSE);
    if (c != Level->site[x][y].showchar) {
      lset(x,y,SEEN);
      Level->site[x][y].showchar = c;
      putspot(x,y,c);
    }
  }
}

/* write a blank to a spot if it is floor */
void blankoutspot(i,j)
int i,j;
{
  if (inbounds(i,j)) {
    lreset(i,j,LIT);
    lset(i, j, CHANGED);
    if (Level->site[i][j].locchar == FLOOR)  {
      Level->site[i][j].showchar = SPACE;
      putspot(i,j,SPACE);
    }
  }
}

/* blank out a spot regardless */
void blotspot(i,j)
int i,j;
{
  if (inbounds(i,j)) {
    lreset(i,j,SEEN);
    Level->site[i][j].showchar = SPACE;
    if (! offscreen(j)) {
      wmove(Levelw,screenmod(j),i);
      wattrset(Levelw, CHARATTR(SPACE));
      waddch(Levelw, SPACE&0xff);
    }
  }
}


/* for displaying activity specifically at some point */
void plotspot(x,y,showmonster)
int x,y,showmonster;
{
  if (loc_statusp(x,y,SEEN))
    putspot(x,y,getspot(x,y,showmonster));
  else 
    putspot(x,y,SPACE);
}


/* Puts c at x,y on screen. No fuss, no bother. */
void putspot(x,y,c)
int x,y;
Symbol c;
{
  if (! offscreen(y)) {
    wmove(Levelw,screenmod(y),x);
    if (optionp(SHOW_COLOUR))
      wattrset(Levelw, CHARATTR(c));
    waddch(Levelw,(0xff&c));
  }
}


/* regardless of line of sight, etc, draw a monster */
void plotmon(m)
struct monster *m;
{
  if (! offscreen(m->y)) {
    wmove(Levelw,screenmod(m->y),m->x);
    if (optionp(SHOW_COLOUR))
      wattrset(Levelw, CHARATTR(m->monchar));
    waddch(Levelw,(m->monchar&0xff));
  }
}
  
/* if display, displays monsters, otherwise erases them */
void drawmonsters(display)
int display;
{
  pml ml;
  for (ml=Level->mlist;ml!=NULL;ml=ml->next) {
    if (ml->m->hp > 0) {
      if (display) {
	if (view_los_p(Player.x,Player.y,ml->m->x,ml->m->y)) {
	  if (Player.status[TRUESIGHT] || (! m_statusp(ml->m,M_INVISIBLE))) {
	    if (!optionp(SHOW_COLOUR) &&
	        (ml->m->level > 5) &&
		((ml->m->monchar&0xff) != '@') &&
		((ml->m->monchar&0xff) != '|')) wstandout(Levelw);
	    putspot(ml->m->x,ml->m->y,ml->m->monchar);
	    if (!optionp(SHOW_COLOUR))
	      wstandend(Levelw);
	  }
	}
      }
      else erase_monster(ml->m);
    }
  }
}

/* replace monster with what would be displayed if monster weren't there */
void erase_monster(m)
struct monster *m;
{
  if (loc_statusp(m->x,m->y,SEEN))
    putspot(m->x,m->y,getspot(m->x,m->y,FALSE));
  else blotspot(m->x,m->y);
}

/* find apt char to display at some location */
Symbol getspot(x,y,showmonster)
int x,y,showmonster;
{
  if (loc_statusp(x,y,SECRET)) return(WALL);
  else switch (Level->site[x][y].locchar) {
  case WATER:
    if (Level->site[x][y].creature == NULL) 
      return(WATER);
    else if (m_statusp(Level->site[x][y].creature,SWIMMING))
      return(WATER);
    else if (showmonster)
      return(Level->site[x][y].creature->monchar);
    else return(WATER);
  /* these sites never show anything but their location char's */
  case CLOSED_DOOR:
  case LAVA:
  case FIRE:
  case ABYSS:
    return(Level->site[x][y].locchar);
  /* rubble and hedge don't show items on their location */
  case RUBBLE:
  case HEDGE:
    if (showmonster && (Level->site[x][y].creature != NULL)) {
      if ((m_statusp(Level->site[x][y].creature,M_INVISIBLE)) &&
	  (! Player.status[TRUESIGHT]))
	return(getspot(x,y,FALSE));
      else return (Level->site[x][y].creature->monchar);
    }
    else return(Level->site[x][y].locchar);
  /* everywhere else, first try to show monster, next show items, next show
     location char */
  default:
    if (showmonster && (Level->site[x][y].creature != NULL)) {
      if ((m_statusp(Level->site[x][y].creature,M_INVISIBLE)) &&
	  (! Player.status[TRUESIGHT]))
	return(getspot(x,y,FALSE));
      else return (Level->site[x][y].creature->monchar);
    }
    else if (Level->site[x][y].things != NULL) {
      if (Level->site[x][y].things->next != NULL)
	return(PILE);
      else return(Level->site[x][y].things->thing->objchar);
    }
    else return(Level->site[x][y].locchar);
  }
}

void commanderror()
{
  wclear(Msg3w);
  wprintw(Msg3w,"%c : unknown command",Cmd);
  wrefresh(Msg3w);
}

void timeprint()
{
  wclear(Timew);
  wprintw(Timew,"%d:%d",showhour(),showminute());
  if (showminute()==0) waddch(Timew,'0');
  wprintw(Timew,hour()>11 ? " PM \n" : " AM \n");
  wprintw(Timew,month());
  wprintw(Timew," the %d",day());
  wprintw(Timew,ordinal(day()));
  wrefresh(Timew);
}


void comwinprint()
{
  wclear(Comwin);
  wprintw(Comwin,"Hit: %d  \n",Player.hit);
  wprintw(Comwin,"Dmg: %d  \n",Player.dmg);
  wprintw(Comwin,"Def: %d  \n",Player.defense);
  wprintw(Comwin,"Arm: %d  \n",Player.absorption);
  wprintw(Comwin,"Spd: %d.%d  \n", 5/Player.speed, 500/Player.speed%100);
  wrefresh(Comwin);
}

void dataprint()
{
  wclear(Dataw);
  /* WDT HACK: I should make these fields spaced and appropriately justified.
   * Maybe I don't feel like it right now. */
  wprintw(Dataw,"Hp:%d/%d Mana:%ld/%ld Au:%ld Level:%d/%ld Carry:%d/%d \n",
	  Player.hp,Player.maxhp,Player.mana,Player.maxmana,Player.cash,
	  Player.level,Player.xp,Player.itemweight,Player.maxweight);
  wprintw(Dataw,"Str:%d/%d Con:%d/%d Dex:%d/%d Agi:%d/%d Int:%d/%d Pow:%d/%d   ",
	  Player.str,Player.maxstr,Player.con,Player.maxcon,
	  Player.dex,Player.maxdex,Player.agi,Player.maxagi,
	  Player.iq,Player.maxiq,Player.pow,Player.maxpow);
  wrefresh(Dataw);
}


/* redraw everything currently displayed */
void redraw()
{
  touchwin(curscr);
  wrefresh(curscr);
}  

/* redraw each permanent window */
void xredraw()
{
  touchwin(Msgw);
  touchwin(Msg3w);
  touchwin(Levelw);
  touchwin(Timew);
  touchwin(Flagw);
  touchwin(Dataw);
  touchwin(Locw);
  touchwin(Morew);
  touchwin(Phasew);
  touchwin(Comwin); 
  wrefresh(Msgw);
  wrefresh(Msg3w);
  wrefresh(Levelw);
  wrefresh(Timew);
  wrefresh(Flagw);
  wrefresh(Dataw);
  wrefresh(Locw);
  wrefresh(Morew);
  wrefresh(Phasew);
  wrefresh(Comwin);
}



void menuaddch(c)
char c;
{
  waddch(Menuw,c);
  wrefresh(Menuw);
}



void morewait()
{
  int display=TRUE;
  int c;
  if (gamestatusp(SUPPRESS_PRINTING))
    return;
  do {
    wclear(Morew);
    if (display) wprintw(Morew,"***  MORE  ***");
    else wprintw(Morew,"+++  MORE  +++");
    display = ! display;
    wrefresh(Morew);
    c = wgetch(Msgw);
  } while ((c != ' ') && (c != RETURN) && (c != EOF));
  wclear(Morew);
  wrefresh(Morew);
}

int stillonblock()
{
  int display=TRUE;
  int c;
  do {
    wclear(Morew);
    if (display) wprintw(Morew,"<<<STAY?>>>");
    else wprintw(Morew,">>>STAY?<<<");
    display = ! display;
    wrefresh(Morew);
    c = wgetch(Msgw);
  } while ((c != ' ') && (c != ESCAPE) && (c != EOF));
  wclear(Morew);
  wrefresh(Morew);
  return (c == ' ');
}

void menuclear()
{
  wclear(Menuw);
  touchwin(Menuw);
  wrefresh(Menuw);
}

void menuspellprint(i)
int i;
{
  int x,y;
  getyx(Menuw,y,x);
  if (y >= ScreenLength - 2) {
    wrefresh(Menuw);
    morewait();
    wclear(Menuw);
    touchwin(Menuw);
  }
  wprintw(Menuw,spellid(i));
  wprintw(Menuw,"(%d)\n",Spells[i].powerdrain);
}  

void menuprint(s)
char *s;
{
  int x,y;
  getyx(Menuw,y,x);
  if (y >= ScreenLength - 2) {
    wrefresh(Menuw);
    morewait();
    wclear(Menuw);
    touchwin(Menuw);
  }
  wprintw(Menuw,s);
}

void showmenu()
{
  wrefresh(Menuw);
}


void endgraf()
{
  clear();
  touchwin(stdscr);
  refresh();
  endwin();
}


void plotchar(pyx,x,y)
Symbol pyx;
int x,y;
{
  if (! offscreen(y)) {
    wmove(Levelw,screenmod(y),x);
    if (optionp(SHOW_COLOUR))
      wattrset(Levelw, CHARATTR(pyx));
    waddch(Levelw,(pyx&0xff));
    wrefresh(Levelw);
  }
}



void draw_explosion(pyx,x,y)
Symbol pyx;
int x,y;
{
  int i,j;
  
  for(j=0;j<3;j++) {
    for(i=0;i<9;i++) 
      plotchar(pyx,x+Dirs[0][i],y+Dirs[1][i]);
    usleep(150000);
    for(i=0;i<9;i++)
      plotchar(SPACE,x+Dirs[0][i],y+Dirs[1][i]);
    usleep(150000);
  }
  for(i=0;i<9;i++) 
    plotspot(x+Dirs[0][i],y+Dirs[1][i],TRUE);
  wrefresh(Levelw);
}

char *msgscanstring()
{
  static char instring[80],byte='x';
  int i=0;

  instring[0]=0;
  byte = mgetc();
  while (byte != '\n') {
    if ((byte == 8) || (byte == 127)) { /* ^h or delete */
      if (i>0){ 
	i--;
	dobackspace();
      }
      instring[i] = 0;
    }
    else {
      instring[i] = byte;
      waddch(Msgw,byte);
      wrefresh(Msgw);
      i++;
      instring[i] = 0;
    }
    byte = mgetc();
  } 
  return(instring);
}


void locprint(s)
char *s;
{
  wclear(Locw);
  wprintw(Locw,s);
  wrefresh(Locw);
}

/* draw everything whether visible or not */
void drawscreen()
{
  int i,j;
  if (Current_Environment == E_COUNTRYSIDE)
    for (i=0;i<WIDTH;i++)
      for(j=0;j<LENGTH;j++)
	c_set(i, j, SEEN);
  else for (i=0;i<WIDTH;i++)
    for(j=0;j<LENGTH;j++)
      lset(i,j,SEEN);
  if (Current_Environment == E_CITY)
    for (i = 0; i < NUMCITYSITES; i++)
      CitySiteList[i][0] = 1;
  show_screen();
}

/*selects a number up to range */

int getnumber(range)
int range;
{
  int done=FALSE,value=1;
  int atom;

  if (range==1) return(1);
  else while (! done) {
    clearmsg();
    wprintw(Msg1w,"How many? Change with < or >, ESCAPE to select:");
    mnumprint(value);
#ifndef MSDOS
    do atom=mcigetc();
    while ((atom != '<')&&(atom != '>')&&(atom!=ESCAPE));
    if ((atom=='>') && (value < range)) value++;
    else if ((atom=='<') && (value > 1)) value--;
    else if (atom==ESCAPE) done = TRUE;
#else
    atom=mcigetc();
    switch (atom)
    {
      case '>':
      case 'k':
#ifdef KEY_UP
      case KEY_UP:
#endif
        if (value < range)
	  value++;
	break;
      case '<':
      case 'j':
#ifdef KEY_DOWN
      case KEY_DOWN:
#endif
        if (value > 1)
	  value--;
	break;
#ifdef KEY_HOME
      case KEY_HOME:
#endif
        value = 1;
        break;
#ifdef KEY_LL
      case KEY_LL:
#endif
        value = range;
        break;
      case ESCAPE:
      	done = TRUE;
	break;
    }
#endif
  }
  return(value);
}

/* reads a positive number up to 999999 */
long parsenum()
{
  int number[8];
  int place = -1;
  int i,x,y,mult=1;
  long num=0;
  char byte=' ';

  while ((byte != ESCAPE) && (byte != '\n')) {
    byte = mgetc();
    if ((byte == BACKSPACE) || (byte == DELETE)) {
      if (place > -1) {
	number[place] = 0;
	place--;
	getyx(Msgw,y,x);
	wmove(Msgw,y,x-1);
	waddch(Msgw,' ');
	wmove(Msgw,y,x-1);
	wrefresh(Msgw);
      }
    }
    else if ((byte <= '9') && (byte >= '0') && (place < 7)) {
      place++;
      number[place]=byte;
      waddch(Msgw,byte);
      wrefresh(Msgw);
    }
  }
  waddch(Msgw,' ');
  if (byte == ESCAPE) return(ABORT);
  else {
    for (i=place;i>=0;i--) {
      num += mult*(number[i]-'0');
      mult*=10;
    }
    return(num);
  }
}


    
void maddch(c)
char c;
{
  waddch(Msgw,c);
  wrefresh(Msgw);
}


void display_death(source)
char *source;
{
  clear();
  touchwin(stdscr);
  printw("\n\n\n\n");
  printw("Requiescat In Pace, ");
  printw(Player.name);
  printw(" (%ld points)",calc_points());
  strcpy(Str4,"Killed by ");
  strcat(Str4,source);
  printw("\n");
  printw(Str4);
  printw(".");
  printw("\n\n\n\n\nHit 'c' to continue.");
  refresh();
  while (wgetch(stdscr) != 'c')
	;
  clear();
  touchwin(stdscr);
  refresh();
  extendlog(Str4,DEAD);
}


void display_win()
{
  clear();
  touchwin(stdscr);
  printw("\n\n\n\n");
  printw(Player.name);
  if (Player.rank[ADEPT]) {
    printw(" is a total master of omega with %ld points!",FixedPoints);
    strcpy(Str4,"A total master of omega");
  }
  else {
    strcpy(Str4,"retired a winner");
    printw(" triumphed in omega with %ld points!",calc_points());
  }
  printw("\n\n\n\n\nHit 'c' to continue.");
  refresh();
  while (wgetch(stdscr) != 'c')
	;
  clear();
  touchwin(stdscr);
  refresh();
  if (Player.rank[ADEPT])
    extendlog(Str4,BIGWIN);
  else extendlog(Str4,WIN);
}

void display_quit()
{
  clear();
  touchwin(stdscr);
  printw("\n\n\n\n");
  printw(Player.name);
  strcpy(Str4,"A quitter.");
  printw(" wimped out with %ld points!",calc_points());
  printw("\n\n\n\n\nHit 'c' to continue.");
  refresh();
  while (wgetch(stdscr) != 'c')
	;
  clear();
  touchwin(stdscr);
  refresh();
  extendlog(Str4,QUIT);
}


void display_bigwin()
{
  clear();
  touchwin(stdscr);
  printw("\n\n\n\n");
  printw(Player.name);
  strcpy(Str4,"retired, an Adept of Omega.");
  printw(" retired, an Adept of Omega with %ld points!",FixedPoints);
  printw("\n\n\n\n\nHit 'c' to continue.");
  refresh();
  while (wgetch(stdscr) != 'c')
	;
  clear();
  touchwin(stdscr);
  refresh();
  extendlog(Str4,BIGWIN);
}


void mnumprint(n)
int n;
{
  char numstr[20];
  sprintf(numstr,"%d",n);
  bufferappend(numstr);
  wprintw(Msgw,"%d",n);
  wrefresh(Msgw);
}

void mlongprint(n)
long n;
{
  char numstr[20];
  sprintf(numstr,"%ld",n);
  bufferappend(numstr);
  wprintw(Msgw,"%ld",n);
  wrefresh(Msgw);
}


void menunumprint(n)
int n;
{
  int x,y;
  getyx(Menuw,y,x);
  if (y >= ScreenLength - 2) {
    wrefresh(Menuw);
    morewait();
    wclear(Menuw);
    touchwin(Menuw);
  }
  wprintw(Menuw,"%d",n);
}

void menulongprint(n)
long n;
{
  int x,y;
  getyx(Menuw,y,x);
  if (y >= ScreenLength - 2) {
    wrefresh(Menuw);
    morewait();
    wclear(Menuw);
    touchwin(Menuw);
  }
  wprintw(Menuw,"%ld",n);
}


void dobackspace()
{
  int x,y;

  getyx(Msgw,y,x);
  if (x > 0) {
    waddch(Msgw,' ');
    wmove(Msgw,y,x-1);
    waddch(Msgw,' ');
    wmove(Msgw,y,x-1);
  }
  wrefresh(Msgw);
}


void showflags()
{

  phaseprint();
  wclear(Flagw);
  if (Player.food < 0)
    wprintw(Flagw,"Starving\n");
  else if (Player.food <= 3)
    wprintw(Flagw,"Weak\n");
  else if (Player.food <= 10)
    wprintw(Flagw,"Ravenous\n");
  else if (Player.food <= 20)
    wprintw(Flagw,"Hungry\n");
  else if (Player.food <= 30)
    wprintw(Flagw,"A mite peckish\n");
  else if (Player.food <= 36)
    wprintw(Flagw,"Content\n");
  else if (Player.food <= 44)
    wprintw(Flagw,"Satiated\n");
  else wprintw(Flagw,"Bloated\n");


  if (Player.status[POISONED]>0)
    wprintw(Flagw,"Poisoned\n");
  else wprintw(Flagw,"Vigorous\n");

  if (Player.status[DISEASED]>0)
    wprintw(Flagw,"Diseased\n");
  else wprintw(Flagw,"Healthy\n");

  if (gamestatusp(MOUNTED)) wprintw(Flagw,"Mounted\n");
  else if (Player.status[LEVITATING]) wprintw(Flagw,"Levitating\n");
  else wprintw(Flagw,"Afoot\n");

  wrefresh(Flagw);
}

void drawomega()
{
  int i;
  clear();
  touchwin(stdscr);
  for(i=0;i<7;i++) {
      move(1, 1);
      if (optionp(SHOW_COLOUR))
        wattrset(stdscr, CHARATTR(CLR(LIGHT_BLUE)));
      printw("\n\n\n");
      printw("\n                                    *****");
      printw("\n                               ******   ******");
      printw("\n                             ***             ***");
      printw("\n                           ***                 ***");
      printw("\n                          **                     **");
      printw("\n                         ***                     ***");
      printw("\n                         **                       **");
      printw("\n                         **                       **");
      printw("\n                         ***                     ***");
      printw("\n                          ***                   ***");
      printw("\n                            **                 **");
      printw("\n                       *   ***                ***   *");
      printw("\n                        ****                    ****");
      refresh();
      usleep(200000);
      move(1, 1);
      if (optionp(SHOW_COLOUR))
        wattrset(stdscr, CHARATTR(CLR(CYAN)));
      printw("\n\n\n");
      printw("\n                                    +++++");
      printw("\n                               ++++++   ++++++");
      printw("\n                             +++             +++");
      printw("\n                           +++                 +++");
      printw("\n                          ++                     ++");
      printw("\n                         +++                     +++");
      printw("\n                         ++                       ++");
      printw("\n                         ++                       ++");
      printw("\n                         +++                     +++");
      printw("\n                          +++                   +++");
      printw("\n                            ++                 ++");
      printw("\n                       +   +++                +++   +");
      printw("\n                        ++++                    ++++");
      refresh();
      usleep(200000);
      move(1, 1);
      if (optionp(SHOW_COLOUR))
        wattrset(stdscr, CHARATTR(CLR(BLUE)));
      printw("\n\n\n");
      printw("\n                                    .....");
      printw("\n                               ......   ......");
      printw("\n                             ...             ...");
      printw("\n                           ...                 ...");
      printw("\n                          ..                     ..");
      printw("\n                         ...                     ...");
      printw("\n                         ..                       ..");
      printw("\n                         ..                       ..");
      printw("\n                         ...                     ...");
      printw("\n                          ...                   ...");
      printw("\n                            ..                 ..");
      printw("\n                       .   ...                ...   .");
      printw("\n                        ....                    ....");
      refresh();
      usleep(200000);
  }
  wattrset(stdscr, CHARATTR(CLR(WHITE)));
}

/* y is an absolute coordinate */
/* ScreenOffset is the upper left hand corner of the current screen
   in absolute coordinates */

void screencheck(y)
int y;
{
  if (((y-ScreenOffset) < (ScreenLength/8)) ||
      ((y-ScreenOffset) > (7*ScreenLength/8))) {
    ScreenOffset = y - (ScreenLength/2);
    show_screen();
    if (Current_Environment != E_COUNTRYSIDE) 
      drawmonsters(TRUE);
    if (!offscreen(Player.y))
      drawplayer();
  }
}




void spreadroomlight(x,y,roomno)
int x,y,roomno;
{
  if (inbounds(x,y) && !loc_statusp(x,y,LIT) &&
      Level->site[x][y].roomnumber == roomno) {
    lightspot(x,y);
    spreadroomlight(x+1,y,roomno);
    spreadroomlight(x,y+1,roomno);
    spreadroomlight(x-1,y,roomno);
    spreadroomlight(x,y-1,roomno);
  }
}

/* illuminate one spot at x y */
void lightspot(x,y)
int x,y;
{ 
  Symbol c;
  lset(x,y,LIT);
  lset(x,y,SEEN);
  lset(x, y, CHANGED);
  c = getspot(x,y,FALSE);
  Level->site[x][y].showchar = c;
  putspot(x,y,c);
}



void spreadroomdark(x,y,roomno)
int x,y,roomno;
{
  if (inbounds(x,y))
    if (loc_statusp(x,y,LIT) && (Level->site[x][y].roomnumber == roomno)) {
      blankoutspot(x,y);
      spreadroomdark(x+1,y,roomno);
      spreadroomdark(x,y+1,roomno);
      spreadroomdark(x-1,y,roomno);
      spreadroomdark(x,y-1,roomno);
    }
}




void display_pack()
{
  int i;
  if (Player.packptr < 1) print3("Pack is empty.");
  else {
    menuclear();
    menuprint("Items in Pack:\n");
    for(i=0;i<Player.packptr;i++) {
      sprintf(Str1, "  %c: %s\n", i + 'A', itemid(Player.pack[i]));
      menuprint(Str1);
    }
    showmenu();
  }
}


void display_possessions()
{
  int i;
  for(i=0;i<MAXITEMS;i++) 
    display_inventory_slot(i,FALSE);
}


void display_inventory_slot(slotnum,topline)
int slotnum;
int topline;
{
  WINDOW *W;
  char usechar = ')', idchar = '-';
  if (Player.possessions[slotnum] != NULL)
    if (Player.possessions[slotnum]->used)
      usechar = '>';
  if (topline) W = Msg3w;
  else {
    W = Showline[slotnum];
    hide_line(slotnum);
  }
  idchar = index_to_key(slotnum);
  touchwin(W);
  wclear(W);
  switch(slotnum) {
  case O_UP_IN_AIR:
    wprintw(W,"-- Object 'up in air':",usechar);
    break;
  case O_READY_HAND:
    wprintw(W,"-- %c%c ready hand: ",idchar,usechar);
    break;
  case O_WEAPON_HAND:
    wprintw(W,"-- %c%c weapon hand: ",idchar,usechar);
    break;
  case O_LEFT_SHOULDER:
    wprintw(W,"-- %c%c left shoulder: ",idchar,usechar);
    break;
  case O_RIGHT_SHOULDER:
    wprintw(W,"-- %c%c right shoulder: ",idchar,usechar);
    break;
  case O_BELT1:
    wprintw(W,"-- %c%c belt: ",idchar,usechar);
    break;
  case O_BELT2:
    wprintw(W,"-- %c%c belt: ",idchar,usechar);
    break;
  case O_BELT3:
    wprintw(W,"-- %c%c belt: ",idchar,usechar);
    break;
  case O_SHIELD:
    wprintw(W,"-- %c%c shield: ",idchar,usechar);
    break;
  case O_ARMOR:
    wprintw(W,"-- %c%c armor: ",idchar,usechar);
    break;
  case O_BOOTS:
    wprintw(W,"-- %c%c boots: ",idchar,usechar);
    break;
  case O_CLOAK:
    wprintw(W,"-- %c%c cloak: ",idchar,usechar);
    break;
  case O_RING1:
    wprintw(W,"-- %c%c finger: ",idchar,usechar);
    break;
  case O_RING2:
    wprintw(W,"-- %c%c finger: ",idchar,usechar);
    break;
  case O_RING3:
    wprintw(W,"-- %c%c finger: ",idchar,usechar);
    break;
  case O_RING4:
    wprintw(W,"-- %c%c finger: ",idchar,usechar);
    break;
  }
  if (Player.possessions[slotnum]== NULL)
    wprintw(W,"(slot vacant)");
  else wprintw(W,itemid(Player.possessions[slotnum]));
  wrefresh(W);
}

int move_slot(oldslot,newslot,maxslot)
int oldslot,newslot,maxslot;
{
  if ((newslot >= 0) && (newslot < maxslot)){
    wmove(Showline[oldslot],0,0);
    waddstr(Showline[oldslot],"--");
    wrefresh(Showline[oldslot]);
    wmove(Showline[newslot],0,0);
    wstandout(Showline[newslot]);
    waddstr(Showline[newslot],">>");
    wstandend(Showline[newslot]);
    wrefresh(Showline[newslot]);
    return(newslot);
  }
  else return(oldslot);
}

void colour_on()
{
}

void colour_off()
{
  wattrset(Levelw, CHARATTR(CLR(WHITE)));
}

void display_option_slot(slot)
int slot;
{
  hide_line(slot);
  wclear(Showline[slot]);
  switch(slot) {
  case 1:
    wprintw(Showline[slot],"-- Option BELLICOSE [TF]: ");
    wprintw(Showline[slot], optionp(BELLICOSE) ? "(now T) " : "(now F) ");
    break;
  case 2:
    wprintw(Showline[slot],"-- Option JUMPMOVE [TF]: ");
    wprintw(Showline[slot], optionp(JUMPMOVE) ? "(now T) " : "(now F) ");
    break;
  case 3:
    wprintw(Showline[slot],"-- Option RUNSTOP [TF]: ");
    wprintw(Showline[slot], optionp(RUNSTOP) ? "(now T) " : "(now F) ");
    break;
  case 4:
    wprintw(Showline[slot],"-- Option PICKUP [TF]: ");
    wprintw(Showline[slot], optionp(PICKUP) ? "(now T) " : "(now F) ");
    break;
  case 5:
    wprintw(Showline[slot],"-- Option CONFIRM [TF]: ");
    wprintw(Showline[slot], optionp(CONFIRM) ? "(now T) " : "(now F) ");
    break;
  case 6:
    wprintw(Showline[slot],"-- Option TOPINV [TF]: ");
    wprintw(Showline[slot], optionp(TOPINV) ? "(now T) " : "(now F) ");
    break;
  case 7:
    wprintw(Showline[slot],"-- Option PACKADD [TF]: ");
    wprintw(Showline[slot], optionp(PACKADD) ? "(now T) " : "(now F) ");
    break;
  case 8:
#ifdef COMPRESS_SAVE_FILES
    wprintw(Showline[slot],"-- Option COMPRESS [TF]: ");
    wprintw(Showline[slot], optionp(COMPRESS_OPTION) ? "(now T) " : "(now F) ");
#endif
    break;
  case 9:
    wprintw(Showline[slot],"-- Option COLOUR [TF]: ");
    wprintw(Showline[slot], optionp(SHOW_COLOUR) ? "(now T) " : "(now F) ");
    break;
  case VERBOSITY_LEVEL:
    wprintw(Showline[slot],
	    "-- Option VERBOSITY [(T)erse,(M)edium,(V)erbose]: (now ");
    if (Verbosity == VERBOSE) wprintw(Showline[slot],"Verbose)");
    else if (Verbosity == MEDIUM) wprintw(Showline[slot],"Medium)");
    else wprintw(Showline[slot],"Terse)");
    break;
  case SEARCH_DURATION:
    wprintw(Showline[slot],"-- Option SEARCHNUM [0>x>10]: (now %d)",Searchnum);
    break;
  }
  wrefresh(Showline[slot]);
}


void display_options()
{
  int i;
  menuclear();
  hide_line(0);
  for(i=1;i<=NUMOPTIONS;i++)
    display_option_slot(i);
}


/* nya ha ha ha ha haaaa.... */
void deathprint()
{
  mgetc();
  waddch(Msgw,'D');
  wrefresh(Msgw);
  mgetc();
  waddch(Msgw,'e');
  wrefresh(Msgw);
  mgetc();
  waddch(Msgw,'a');
  wrefresh(Msgw);
  mgetc();
  waddch(Msgw,'t');
  wrefresh(Msgw);
  mgetc();
  waddch(Msgw,'h');
  wrefresh(Msgw);
  mgetc();
}
  
void clear_if_necessary()
{
  int x,y;
  getyx(Msg1w,y,x);

  if (x != 0) {
    wclear(Msg1w);
    wrefresh(Msg1w);
  }  

  getyx(Msg2w,y,x);

  if (x != 0) {
    wclear(Msg2w);
    wrefresh(Msg2w);
  }  

  getyx(Msg3w,y,x);

  if (x != 0) {
    wclear(Msg3w);
    wrefresh(Msg3w);
  }

}

int bufferpos = 0;

void buffercycle(s)
char *s;
{
  strcpy(Stringbuffer[bufferpos++],s);
  if (bufferpos >= STRING_BUFFER_SIZE)
    bufferpos = 0;
}

int bufferappend(s)
char *s;
{
  int pos = bufferpos - 1;

  if (pos < 0)
    pos = STRING_BUFFER_SIZE - 1;
  if (strlen(Stringbuffer[pos]) + strlen(s) < 80 - 1) {
    strcat(Stringbuffer[pos],s);
    return 1;
  }
  else
    return 0;
}


void bufferprint()
{
  int i = bufferpos - 1, c, finished = 0;
  clearmsg();
#ifndef MSDOS_SUPPORTED_ANTIQUE
  wprintw(Msg1w,"^p for previous message, ^n for next, anything else to quit.");
#else
  wprintw(Msg1w,"^o for last message, ^n for next, anything else to quit.");
#endif
  wrefresh(Msg1w);
  do {
    if (i >= STRING_BUFFER_SIZE) i = 0;
    if (i < 0) i = STRING_BUFFER_SIZE - 1;
    wclear(Msg2w);
    wprintw(Msg2w,Stringbuffer[i]);
    wrefresh(Msg2w);
    c = mgetc();
#ifndef MSDOS_SUPPORTED_ANTIQUE
    if (c == 16)	/* ^p */
#else
    if (c == 15)	/* ^o */
#endif
      i--;
    else if (c == 14)	/* ^n */
      i++;
    else
      finished = 1;
  } while (!finished);
  clearmsg();
  omshowcursor(Player.x,Player.y);
}

void clear_screen()
{
  clear();
  touchwin(stdscr);
  refresh();
}
