/* omega (c) 1987 by Laurence Raphael Brothers */
/* oscr.c */
/* functions that use curses routines directly */
/* plus a few file i/o stuff */
/* also some in ofile.c */

#include <curses.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/timeb.h>
#include "oglob.h"

/* from ofile */
extern void showscores(),extendlog(),showfile();



/* from outil */
extern int los_p(),view_los_p();
extern int inbounds();
extern int screenmod();
extern int offscreen();
extern int distance();
extern int calc_points();
extern int hour(),showhour();
extern int day();
extern char *month(),*ordinal();

/* from oplay */
extern char *itemid();
extern int optionp();

/* from ospell */
extern char *spellid();

WINDOW *Levelw,*Dataw,*Flagw,*Timew,*Menuw1,*Menuw2,*Msgw,*Locw,*Morew,*Phasew;
WINDOW *CurrentMenu,*Rightside,*Comwin;
     
void redraw(),drawspot(),drawvision(),morewait(),drawmonsters();
void mprint(),printm(),title(),sitclear(),initgraf(),xredraw();
void plotchar(),show_screen(),blankoutspot(),show_license();
void menuclear(),showcursor(),mnumprint(),menumorewait(),menunumprint();
void menu_i_print(),clearmsg(),menuprint(),menu_item_print();
void stdmorewait(),draw_explosion(),dataprint(),comwinprint(),timeprint();
void menuspellprint(),erase_level(),levelrefresh();
void refreshmsg(),menuaddch(),drawscreen(),maddch(),putspot();
void menuprintitem(),display_quit(),showflags();
void drawplayer(),displaystats(),display_death(),plotmon();
void dobackspace(),drawandrefreshspot(),selectmenu();
void drawomega(),screencheck(),blotspot(),plotspot();
int parsenum(),getnumber(),litroom();
char getspot(),mgetc(),lgetc(),menugetc(),*msgscanstring();
char mcigetc();

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
}

void selectmenu()
{
  int y,x;
  getyx(CurrentMenu,y,x);
  if (y>17) {
    if (CurrentMenu == Menuw1)
      CurrentMenu = Menuw2;
    else {
      menumorewait();
      menuclear();
      CurrentMenu = Menuw1;
    }
  }
}

void show_screen(y)
int y;
{
  int i,j,seen,top,bottom;
  wclear(Levelw);
  switch(y) {
  case 0:
    top = TOPEDGE;
    bottom = EDGE1+2;
    break;
  case 1:
    top = EDGE1-1;
    bottom = EDGE2+1;
    break;
  case 2:
    top = EDGE2-1;
    bottom = EDGE3+1;
    break;
  case 3:
    top = EDGE3-2;
    bottom = BOTTOMEDGE;
    break;
  }
  for (j=top;j<=bottom;j++) 
    for (i=0;i<WIDTH;i++) {
      seen = Dungeon[Dlevel][i][j].seen;
      wmove(Levelw,screenmod(j),i);
      waddch(Levelw,(seen ? Dungeon[Dlevel][i][j].showchar : ' '));
    }
  wrefresh(Levelw);
}





void refreshmsg()
{
  wrefresh(Msgw);
}


char mgetc()
{
  return(wgetch(Msgw));
}

/* case insensitive mgetc -- sends uppercase to lowercase */
char mcigetc()
{
  char c = wgetch(Msgw);
  if ((c>='A') && (c<='Z'))
    return(c+('a'-'A'));
  else return(c);
}

char menugetc()
{
  return(wgetch(Menuw1));
}


char lgetc()
{
  return(wgetch(Levelw));
}


char ynq()
{
  char p=' ';
  while ((p != 'n') && (p != 'y') && (p != 'q') && (p != ESCAPE))
    p = wgetch(Msgw);
  switch (p) {
    case 'y': wprintw(Msgw,"yes. "); break;
    case 'n': wprintw(Msgw,"no. "); break;
    case ESCAPE:
    case 'q': wprintw(Msgw,"quit. "); break;
    }
  wrefresh(Msgw);
  return(p);
}
    

/* for external call */
void clearmsg()
{
  wclear(Msgw);
  wrefresh(Msgw);
}


void erase_level()
{
  wclear(Levelw);
}


/* possible moreprint to message window, otherwise just a printm */
void mprint(s)
char *s;
{
  int x,y;
  if (! SuppressPrinting) {
    strcpy(Laststring,s);
    getyx(Msgw,y,x);
    if (x+strlen(s) > 64) {
      morewait();
      wclear(Msgw);
    }
    wprintw(Msgw,s);
    waddch(Msgw,' '); 
    wrefresh(Msgw);
  }
}



/* no new line */
void printm(s)
char *s;
{
  if (! SuppressPrinting) {
    wprintw(Msgw,s);
    waddch(Msgw,' ');
    wrefresh(Msgw);
  }
}  

void title()
{
  FILE *fd;
  strcpy(Str1,OMEGALIB);
  strcat(Str1,"omega.motd");
  fd = fopen(Str1,"r");
  showfile(fd);
  /* Pseudo Random Seed */
  Seed = (int) time(0);
  clear();
  refresh();  
  showscores();
}










/* initialize, screen, windows */
void initgraf()
{
  initscr();

  Rightside = newwin(20,15,1,65);
  Msgw = newwin(1,80,0,0);  
  Morew = newwin(1,15,1,65);  
  Locw = newwin(1,80,21,0);
  Levelw = newwin(20,64,1,0);
  Menuw1 = newwin(20,64,1,0);
  Menuw2 = newwin(20,40,1,40);  
  Dataw = newwin(2,80,22,0);
  Timew = newwin(2,15,3,65);
  Phasew = newwin(2,15,6,65);
  Flagw = newwin(3,15,9,65);
  Comwin = newwin(8,15,13,65);
  CurrentMenu = Menuw1;

  noecho();
  crmode();

  scrollok(Msgw,TRUE);

  wclear(Msgw);
  wclear(Locw);
  wclear(Levelw);
  wclear(Timew);
  wclear(Flagw);
  wclear(Phasew);
  wclear(Dataw);
  wclear(Menuw1);
  wclear(Menuw2);
  wclear(Comwin);

  clear();
  refresh();
  title();
  clear();
  refresh();
}

void drawplayer()
{
  static int lastx=0,lasty=0;
  plotspot(lastx,lasty,(Player.status[BLINDED]>0 ? FALSE : TRUE));
  wmove(Levelw,screenmod(Player.y),Player.x);
  if ((! Player.status[INVISIBLE]) || Player.status[TRUESIGHT])
    waddch(Levelw,PLAYER);
  lastx = Player.x;
  lasty = Player.y;
}

int litroom(x,y)
int x,y;
{
  if (Dungeon[Dlevel][x][y].roomnumber < ROOMBASE) return(FALSE);
  else 
    return(Leveldata[Dlevel].rooms
	   [Dungeon[Dlevel][x][y].roomnumber-ROOMBASE].lighted ||
	   Player.status[ILLUMINATION]);
}

void drawvision(x,y,v)
int x,y,v;
{
  static int oldx,oldy,oldv,oldroom;
  int i,j;
  if (Dlevel != 0)
    for (i=0-oldv;i<oldv+1;i++)
      for (j=0-oldv;j<oldv+1;j++)
	if (inbounds(oldx+i,oldy+j))
	  if ((oldroom != Dungeon[Dlevel][x][y].roomnumber) ||
	      (! litroom(oldx,oldy)))
	    blankoutspot(oldx+i,oldy+j);
  if (Player.status[BLINDED]) {
    drawspot(oldx,oldy);
    drawspot(x,y);
    drawplayer();
    v = 0;
  }
  else {
    for (i=0-v;i<v+1;i++)
      for (j=0-v;j<v+1;j++)
	if (inbounds(x+i,y+j))
	  drawspot(x+i,y+j);
    drawplayer();
    drawmonsters(TRUE);
  }
  if ((! Fastmove) || (! optionp(JUMPMOVE)))
    showcursor(Player.x,Player.y);
  oldx = x;
  oldy = y;
  oldv = v;
  oldroom = Dungeon[Dlevel][x][y].roomnumber;
}


void showcursor(x,y)
int x,y;
{
  wmove(Levelw,screenmod(y),x);
  wrefresh(Levelw);
}

void levelrefresh()
{
  wrefresh(Levelw);
}

/* this one refreshes immediately and also ignores monsters */
void drawandrefreshspot(x,y)
int x,y;
{
  char c = getspot(x,y,FALSE);
  int l;
  if (c != Dungeon[Dlevel][x][y].showchar)
    if (view_los_p(Player.x,Player.y,x,y)) {
	Dungeon[Dlevel][x][y].seen = TRUE;
	Dungeon[Dlevel][x][y].showchar = c;
	putspot(x,y,c);
      }
  wrefresh(Levelw);
}

/* draws a particular spot under if in line-of-sight */
void drawspot(x,y)
int x,y;
{
  char c = getspot(x,y,FALSE);
  if (c != Dungeon[Dlevel][x][y].showchar)
    if (view_los_p(Player.x,Player.y,x,y)) {
	Dungeon[Dlevel][x][y].seen = TRUE;
	Dungeon[Dlevel][x][y].showchar = c;
	putspot(x,y,c);
      }
}

/* write a blank to a spot if it is floor */
void blankoutspot(i,j)
int i,j;
{
  if (Dungeon[Dlevel][i][j].showchar == FLOOR) 
    blotspot(i,j);
}

/* blank out a spot regardless */
void blotspot(i,j)
int i,j;
{
  Dungeon[Dlevel][i][j].seen = FALSE;
  Dungeon[Dlevel][i][j].showchar = SPACE;
  if (! offscreen(j)) {
    wmove(Levelw,screenmod(j),i);
    wdelch(Levelw);
    winsch(Levelw,SPACE);
  }
}


/* for displaying activity specifically at some point */
void plotspot(x,y,showmonster)
int x,y,showmonster;
{
  if (Dungeon[Dlevel][x][y].seen)
    putspot(x,y,getspot(x,y,showmonster));
  else 
    putspot(x,y,' ');
}

void putspot(x,y,c)
int x,y;
char c;
{
  if (! offscreen(y)) {
    wmove(Levelw,screenmod(y),x);
    waddch(Levelw,c);
  }
}


/* regardless of line of sight, etc, draw a monster */
void plotmon(m)
struct monster *m;
{
  if (! offscreen(m->y)) {
    wmove(Levelw,screenmod(m->y),m->x);
    waddch(Levelw,m->monchar);
  }
}
  
/* if display, displays monsters, otherwise erases them */
void drawmonsters(display)
int display;
{
  pml ml;
  for (ml=Mlist[Dlevel];ml!=NULL;ml=ml->next) {
    if (display) {
      if (view_los_p(Player.x,Player.y,ml->m->x,ml->m->y))
	if (Player.status[TRUESIGHT] || (! m_statusp(ml->m,M_INVISIBLE)))
	  putspot(ml->m->x,ml->m->y,ml->m->monchar);
    }
    else if (Dungeon[Dlevel][ml->m->x][ml->m->y].seen)
      putspot(ml->m->x,
	      ml->m->y,
	      Dungeon[Dlevel][ml->m->x][ml->m->y].showchar);
    else blotspot(ml->m->x,ml->m->y);
  }
}

/* find apt char to display at some location */
char getspot(x,y,showmonster)
int x,y,showmonster;
{
  if (Dungeon[Dlevel][x][y].secret) return(WALL);
  switch (Dungeon[Dlevel][x][y].locchar) {
  case WHIRLWIND: 
    return((Time % 2 == 1) ? WHIRLWIND2 : WHIRLWIND);
    break;
  case WATER:
    if (Dungeon[Dlevel][x][y].creature == NULL) 
      return(WATER);
    else if (m_statusp(Dungeon[Dlevel][x][y].creature,SWIMMING))
      return(WATER);
    else return(Dungeon[Dlevel][x][y].creature->monchar);
    break;
  case CLOSED_DOOR:
  case LAVA:
  case HEDGE:
  case FIRE:
  case PORTAL:
  case ABYSS:
    return(Dungeon[Dlevel][x][y].locchar);
    break;
  default:
    if (showmonster && (Dungeon[Dlevel][x][y].creature != NULL)) {
      if ((m_statusp(Dungeon[Dlevel][x][y].creature,M_INVISIBLE)) &&
	  (! Player.status[TRUESIGHT]))
	return(getspot(x,y,FALSE));
      else return (Dungeon[Dlevel][x][y].creature->monchar);
    }
    else if (Dungeon[Dlevel][x][y].things != NULL) {
      if (Dungeon[Dlevel][x][y].things->next != NULL)
	return(PILE);
      else return(Dungeon[Dlevel][x][y].things->thing->objchar);
    }
    else return(Dungeon[Dlevel][x][y].locchar);
    break;
  }
}

void commanderror()
{
  wclear(Msgw);
  wrefresh(Msgw);
  wprintw(Msgw,"%c : unknown command",Cmd);
  wrefresh(Msgw);
}

void timeprint()
{
  wclear(Timew);
  wprintw(Timew,"%d",showhour());
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
  wprintw(Comwin,"Spd: %.2f  \n",5.0/Player.speed);
  wrefresh(Comwin);
}

void dataprint()
{
  wclear(Dataw);
  wprintw(Dataw,"HP:%d/%d MANA:%d/%d AU:%d LEVEL:%d/%d CARRY:%d/%d \n",
	  Player.hp,Player.maxhp,Player.mana,Player.maxmana,Player.cash,
	  Player.level,Player.xp,Player.itemweight,Player.maxweight);
  wprintw(Dataw,"STR:%d/%d CON:%d/%d DEX:%d/%d AGI:%d/%d INT:%d/%d POW:%d/%d   ",
	  Player.str,Player.maxstr,Player.con,Player.maxcon,
	  Player.dex,Player.maxdex,Player.agi,Player.maxagi,
	  Player.iq,Player.maxiq,Player.pow,Player.maxpow);
  wrefresh(Dataw);
}


void displaystats(statpoints)
int statpoints;
{
wclear(Dataw);
wprintw(Dataw,"       Statistic Points Left: %d \n",statpoints);
wprintw(Dataw,"STR:%d  CON:%d  DEX:%d  AGI:%d  INT:%d  POW:%d  ",
	Player.maxstr,Player.maxcon,Player.maxdex,Player.maxagi,Player.maxiq,
	Player.maxpow);
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
  wclear(Rightside);
  wrefresh(Rightside);
  scrollok(Msgw,FALSE);
  touchwin(Msgw);
  touchwin(Levelw);
  touchwin(Timew);
  touchwin(Flagw);
  touchwin(Dataw);
  touchwin(Locw);
  touchwin(Morew);
  touchwin(Phasew);
  touchwin(Comwin);
  wrefresh(Msgw);
  wrefresh(Levelw);
  wrefresh(Timew);
  wrefresh(Flagw);
  wrefresh(Dataw);
  wrefresh(Locw);
  wrefresh(Morew);
  wrefresh(Phasew);
  wrefresh(Comwin);
  scrollok(Msgw,TRUE);
}



void menuaddch(c)
char c;
{
  selectmenu();
  waddch(CurrentMenu,c);
  wrefresh(CurrentMenu);
}

void stdmorewait()
{
  printw("\n---More---");
  refresh();
  while (wgetch(stdscr) != SPACE) { }
}

void menumorewait()
{
  wprintw(Menuw2,"\n ---More---");
  wrefresh(Menuw2);
  while (wgetch(Menuw2) != SPACE) { }
}


/* Neato the way it doesn't matter what display is set to, huh */
void morewait()
{
  int display;
  do {
    wclear(Morew);
    if (display) wprintw(Morew,"***  MORE  ***");
    else wprintw(Morew,"+++  MORE  +++");
    display = ! display;
    wrefresh(Morew);
  } while (wgetch(Msgw) != SPACE);
  wclear(Morew);
  wrefresh(Morew);
}

void menuclear()
{
  wclear(Menuw1);
  wclear(Menuw2);
  wrefresh(Menuw1);
  wrefresh(Menuw2);
  CurrentMenu = Menuw1;
}

void menuspellprint(i)
int i;
{
  selectmenu();
  wprintw(CurrentMenu,spellid(i));
  wprintw(CurrentMenu,"(%d)\n",Spells[i].powerdrain);
  wrefresh(CurrentMenu);
}  

void menuprint(s)
char *s;
{
  selectmenu();
  wprintw(CurrentMenu,s);
  wrefresh(CurrentMenu);
}

void menu_i_print(i)
int i;
{
  menu_item_print(i,Player.possessions[i]);
}

void menuprintitem(i,s)
int i;
char *s;
{
  selectmenu();
  wprintw(CurrentMenu,"%d",i);
  waddch(CurrentMenu,':');
  waddch(CurrentMenu,' ');
  wprintw(CurrentMenu,s);
  wprintw(CurrentMenu,"\n");
  wrefresh(CurrentMenu);
}

void menu_item_print(i,item)
int i;
pob item;
{
  selectmenu();
  waddch(CurrentMenu,((char) ('a' + i)));
  waddch(CurrentMenu,':');
  waddch(CurrentMenu,' ');
  waddch(CurrentMenu,item->objchar);
  waddch(CurrentMenu,' ');
  wprintw(CurrentMenu,itemid(item));
  if (item->used) wprintw(CurrentMenu," (in use)");
  wprintw(CurrentMenu,"\n");
  wrefresh(CurrentMenu);
}



void endgraf()
{
clear();
refresh();
endwin();
}


void plotchar(pyx,x,y)
char pyx;
int x,y;
{
  if (! offscreen(y)) {
    wmove(Levelw,screenmod(y),x);
    waddch(Levelw,pyx);
    wrefresh(Levelw);
  }
}



void draw_explosion(pyx,x,y)
char pyx;
int x,y;
{
  int i,j;
  
  for(j=0;j<3;j++) {
    for(i=0;i<9;i++) 
      plotchar(pyx,x+Dirs[0][i],y+Dirs[1][i]);
    for(i=0;i<9;i++)
      plotchar(' ',x+Dirs[0][i],y+Dirs[1][i]);
  }
  for(i=0;i<9;i++) 
    plotspot(x+Dirs[0][i],y+Dirs[1][i],TRUE);
  wrefresh(Levelw);
}

char *msgscanstring()
{
  char instring[80],byte='x';
  int i=0;

  instring[0]=0;
  byte = mgetc();
  while (byte != '\n') {
    if ((byte == 8) || (byte == 127)) { /* ^h or delete */
      instring[i] = 0;
      if (i>0){ 
	i--;
	dobackspace();
      }
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

  for (i=0;i<WIDTH;i++)
    for(j=0;j<LENGTH;j++)
      putspot(i,j,Dungeon[Dlevel][i][j].locchar);
  wrefresh(Levelw);
  morewait();
  xredraw();
}

/*selects a number up to range */

int getnumber(range)
{
  int done=FALSE,value=1;
  char atom;

  if (range==1) return(1);
  else while (! done) {
    printm("\nHow many? Change with < or >, ESCAPE to select:");
    mnumprint(value);
    do atom=mgetc();
    while ((atom != '<')&&(atom != '>')&&(atom!=ESCAPE));
    if ((atom=='>') && (value < range)) value++;
    else if ((atom=='<') && (value > 1)) value--;
    else if (atom==ESCAPE) done = TRUE;
  }
  return(value);
}

/* reads a positive number up to 99999 */
int parsenum()
{
  int number[5];
  int place = -1;
  int done=FALSE;
  int i,x,y,num=0,mult=1;
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
    else if ((byte <= '9') && (byte >= '0') && (place < 4)) {
      place++;
      number[place]=byte;
      waddch(Msgw,byte);
      wrefresh(Msgw);
    }
	   }
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
  printw("\n\n\n\n");
  printw("Requiescat In Pace, ");
  printw(Player.name);
  printw(" (%d points)",calc_points());
  strcpy(Str4,"Killed by ");
  strcat(Str4,source);
  printw("\n");
  printw(Str4);
  printw(".");
  printw("\n\n\n\n\n\Hit any key to continue.");
  refresh();
  wgetch(stdscr);
  clear();
  refresh();
  extendlog(Str4,DEAD);
}


void display_win()
{
  clear();
  printw("\n\n\n\n");
  printw(Player.name);
  if (Player.status[ADEPT]) {
    printw("is a total master of omega with %d points!",calc_points());
    strcpy(Str4,"A total master of omega");
  }
  else {
    strcpy(Str4,"retired a winner");
    printw(" triumphed in omega with %d points!",calc_points());
    printw("\nDeepest penetration was level %d.",Deepest);
  }
  printw("\n\n\n\n\n\Hit any key to continue.");
  refresh();
  wgetch(stdscr);
  clear();
  refresh();
  if (Player.status[ADEPT])
    extendlog(Str4,BIGWIN);
  else extendlog(Str4,WIN);
}

void display_quit()
{
  clear();
  printw("\n\n\n\n");
  printw(Player.name);
  strcpy(Str4,"A quitter.");
  printw(" wimped out on level %d with %d points!",Dlevel,calc_points());
  printw("\nDeepest penetration was level %d.",Deepest);
  printw("\n\n\n\n\n\Hit any key to continue.");
  refresh();
  wgetch(stdscr);
  clear();
  refresh();
  extendlog(Str4,QUIT);
}


void mnumprint(n)
int n;
{
  wprintw(Msgw,"%d",n);
  wrefresh(Msgw);
}

void menunumprint(n)
int n;
{
  wprintw(CurrentMenu,"%d",n);
  wrefresh(CurrentMenu);
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
  if (Player.food > 20)
    wprintw(Flagw,"Satiated\n");
  else if (Player.food < 0)
    wprintw(Flagw,"Starving\n");
  else if (Player.food < 2)
    wprintw(Flagw,"Weak\n");
  else if (Player.food < 4)
    wprintw(Flagw,"Hungry\n");
  else if (Player.food < 8)
    wprintw(Flagw,"A mite peckish\n");
  else wprintw(Flagw,"Content\n");

  if (Player.status[POISONED]>0)
    wprintw(Flagw,"Poisoned\n");
  else wprintw(Flagw,"Vigorous\n");

  if (Player.status[DISEASED]>0)
    wprintw(Flagw,"Diseased\n");
  else wprintw(Flagw,"Healthy\n");

  wrefresh(Flagw);
}

void drawomega()
{
  int i;
  for(i=0;i<7;i++) {
  clear();
  printw("\n                          *****");
  printw("\n                     ******   ******");
  printw("\n                   ***             ***");
  printw("\n                 ***                 ***");
  printw("\n                **                     **");
  printw("\n               ***                     ***");
  printw("\n               **                       **");
  printw("\n               **                       **");
  printw("\n               ***                     ***");
  printw("\n                ***                   ***");
  printw("\n                  **                 **");
  printw("\n             *   ***                ***   *");
  printw("\n              ****                    ****");
  refresh();
  clear();
  printw("\n                          +++++");
  printw("\n                     ++++++   ++++++");
  printw("\n                   +++             +++");
  printw("\n                 +++                 +++");
  printw("\n                ++                     ++");
  printw("\n               +++                     +++");
  printw("\n               ++                       ++");
  printw("\n               ++                       ++");
  printw("\n               +++                     +++");
  printw("\n                +++                   +++");
  printw("\n                  ++                 ++");
  printw("\n             +   +++                +++   +");
  printw("\n              ++++                    ++++");
  refresh();
  clear();
  printw("\n                          .....");
  printw("\n                     ......   ......");
  printw("\n                   ...             ...");
  printw("\n                 ...                 ...");
  printw("\n                ..                     ..");
  printw("\n               ...                     ...");
  printw("\n               ..                       ..");
  printw("\n               ..                       ..");
  printw("\n               ...                     ...");
  printw("\n                ...                   ...");
  printw("\n                  ..                 ..");
  printw("\n             .   ...                ...   .");
  printw("\n              ....                    ....");
  refresh();
  }
  xredraw();
}               


void screencheck(y)
int y;
{
  if (y/16 != WhichScreen) {
    WhichScreen = y/16;
    show_screen(WhichScreen);
  }
}



void show_license()
{
  Skipmonsters = TRUE;
  clear();
  printw("General Public License:\n\n");
  printw("omega is copyright (C) 1987 by:\n");
  printw("Laurence R. Brothers (brothers@paul.rutgers.edu)\n\n");
  printw("All rights are reserved, save the following permissions:\n\n");
  printw("Free copying: Anyone may make a copy of omega, and distribute\n");
  printw("the copy, so long as this license remains accessible\n");
  printw("(via the 'P' command). No charge will be levied for such copying\n");
  printw("or distribution. The author will gladly accept any remuneration\n");
  printw("that users may wish to provide, however.\n\n");
  printw("Modification: No one may modify omega except inasmuch\n");
  printw("as may be necessary to cause omega to function in any given\n");
  printw("computer environment. 'Porting' is therefore freely allowed. Any\n");
  printw("such modifications will be considered 'works for hire'\n");
  printw("and the modifiers will have no rights of ownership over\n");
  printw("such modifications. I specifically note that no compensation\n");
  printw("in any form will be rendered to those who make such allowed\n");
  printw("changes to omega. All other modifications are in violation of\n");
  printw("this license.\n\n");
  stdmorewait();
  clear();
  printw("Warranty: No warranty is made for omega's operation.\n");
  printw("Any damages incurred in the use of omega or in its\n");
  printw("implementation are solely the responsibility of the end-user.\n\n");
  printw("Coverage: Anyone who compiles, runs, copies, or in any other way\n");
  printw("manipulates any copy of omega's code, programs, or files shall\n");
  printw("considered to be bound by this license.\n");
  refresh();
  stdmorewait();
  xredraw();
}
