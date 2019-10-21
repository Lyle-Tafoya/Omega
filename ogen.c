/* omega copyright (c) 1987 by Laurence Raphael Brothers */
/* ogen.c */
/* level generator functions */

#include <stdio.h>
#include "oglob.h"


/* from outil */
extern int random_range(),inbounds(),sign();

/* from olev */
extern void populate_level(),stock_level(),load_city();
extern pmt make_creature(),m_create(),make_hiscore_npc();

/* from odepths */
extern void load_depths(); 

/* from oitem */
extern pob make_weapon(),create_object();

/* from omon */
extern void m_pickup();

/* from oscr */
extern void erase_level(),locprint(),drawvision();

void clear_level(),create_level(),graph_wheel(),build_wheel(),wheel_level();
void connect_room(),corridor_crawl(),build_room(),roomcenter();
void clear_dungeon(),init_dungeon(),install_traps(),straggle_corridor();
void room_connect_corridor(),make_central_room(),make_spoke_room();
void change_level(),cavern_level(),makedoor(),grid_level();
void install_specials(),make_stairs(),pond_level(),room_level();

int connected();
char *roomname();

void init_dungeon()
{
  clear_level(0);
  Dlevel = 0;
  erase_level();
  load_city();
  locprint("The City of Rampart.");
}

void clear_dungeon()
{
  int i;
  for(i=0;i<NUMLEVELS;i++) {
    Leveldata[i].generated = FALSE;
    Leveldata[i].tunnelled = 0;
    clear_level(i);
  }
}

/* erase the level */
void clear_level(level)
int level;
{
  int i,j;
  Mlist[level] = NULL;
  for(i=0;i<WIDTH;i++)
    for(j=0;j<LENGTH;j++) {
      Dungeon[level][i][j].locchar = WALL;
      Dungeon[level][i][j].showchar = ' ';
      Dungeon[level][i][j].creature = NULL;
      Dungeon[level][i][j].things = NULL;
      Dungeon[level][i][j].stopsrun = FALSE;
      Dungeon[level][i][j].aux = level*2; /* toughness of the rock */
      Dungeon[level][i][j].p_locf = L_NO_OP;
      Dungeon[level][i][j].seen = FALSE;
      Dungeon[level][i][j].secret = FALSE;
      Dungeon[level][i][j].roomnumber = RS_WALLSPACE;
    }
}

void create_level(fromlevel,tolevel,sitechar)
int fromlevel,tolevel;
char sitechar;     
{
  clear_level(tolevel);
  if (tolevel == 5) cavern_level(tolevel,1);
  else if (tolevel == 18) pond_level(tolevel,1);
  else if (tolevel == 27) pond_level(tolevel,2);
  else if (tolevel == 33) cavern_level(tolevel,2);
  else if (tolevel == 34) room_level(tolevel,1);
  else if (tolevel == 35) room_level(tolevel,2);
  else if (tolevel == 36) room_level(tolevel,3);
  else if (tolevel == 37) room_level(tolevel,4);
  else if (tolevel == 38) room_level(tolevel,5);
  else if (tolevel == NUMLEVELS-1) load_depths();
  else  switch(random_range(20)) {
    case 0:
    case 1:
    case 2: 
    case 3:
    case 4: cavern_level(tolevel,FALSE); break;
    case 5: 
    case 6: 
    case 7: grid_level(tolevel); break;
    case 8: 
    case 9:
    case 10:
    case 11:
    case 12: room_level(tolevel,FALSE); break;
    case 13:
    case 14: pond_level(tolevel,FALSE); break;
    default: wheel_level(tolevel); break;
  }
  install_traps(tolevel);
  install_specials(tolevel);
  if (tolevel != NUMLEVELS-1) make_stairs(fromlevel,tolevel,sitechar);
}


void wheel_level(level)
int level;
{
  int i,j;

  Leveldata[level].numrooms = 21;

  for (i=0;i<Leveldata[level].numrooms;i++) {
    Leveldata[level].rooms[i].rsi = random_range(NUMROOMNAMES)+ROOMBASE;
    Leveldata[level].rooms[i].numconnections = 0;
    for (j=0;j<MAXCONNECTIONS;j++)
      Leveldata[level].rooms[i].connections[j] = -1;
  }

  graph_wheel(level);
  build_wheel(level);

  populate_level(level,-1);
  stock_level(level);
}




/* designate room 0 focus; all rooms must be connected eventually to
   room 0. */
void graph_wheel(level)
int level;
{
  int i;

  connect_room(0,Leveldata[level].numrooms-1,level);

  for (i=1;i<Leveldata[level].numrooms-1;i++) 
    if (random_range(2) == 1)
      connect_room(i,0,level);

  for (i=1;i<Leveldata[level].numrooms-1;i++)
    if (! connected(i,0,level))
      connect_room(i,i+1,level);
}


void build_wheel(level)
int level;
{
  int i,j,fx,fy,tx,ty;

  make_central_room(level);
  build_room(0,level);

  for (i=1;i<Leveldata[level].numrooms;i++) {
    make_spoke_room(i,level);
    build_room(i,level);
  }

  for (i=0;i<Leveldata[level].numrooms;i++)
    for (j=0;j<Leveldata[level].rooms[i].numconnections;j++)
      room_connect_corridor(i,Leveldata[level].rooms[i].connections[j],level);
}




void make_central_room(level)
int level;
{
  Leveldata[level].rooms[0].left = ((int) (WIDTH/2)) - 6;
  Leveldata[level].rooms[0].right = Leveldata[level].rooms[0].left+12;
  Leveldata[level].rooms[0].top = ((int) (LENGTH/2)) - 4;
  Leveldata[level].rooms[0].bottom = Leveldata[level].rooms[0].top+4;
}




void make_spoke_room(room,level)
int room,level;
{
  int t,b,l,r;

  switch (room-1) {

    case 0: l = 2; t = 1; break;
    case 1: l = 18; t = 1; break; 
    case 2: l = 33; t = 1; break; 
    case 3: l = 48; t = 1; break; 

    case 4: l = 48; t = 9; break; 
    case 5: l = 48; t = 17; break; 

    case 6: l = 48; t = 57; break; 
    case 7: l = 18; t = 57; break; 
    case 8: l = 33; t = 57; break; 
    case 9: l = 2; t = 57; break; 

    case 10: l = 2; t = 25; break; 
    case 11: l = 2; t = 33; break; 
    case 12: l = 2; t = 41; break; 
    case 13: l = 2; t = 49; break; 

    case 14: l = 48; t = 25; break; 
    case 15: l = 48; t = 33; break; 
    case 16: l = 48; t = 41; break; 
    case 17: l = 48; t = 49; break; 

    case 18: l = 2; t = 9; break; 
    case 19: l = 2; t = 17; break; 

  }

  r = l + random_range(11)+4;
  b = t + random_range(4)+3;

  Leveldata[level].rooms[room].top = t;
  Leveldata[level].rooms[room].bottom = b;
  Leveldata[level].rooms[room].left = l;
  Leveldata[level].rooms[room].right = r;
}


/* actually change Dungeon */
/* at moment, build rectangular rooms only */
void build_room(room,level)
int room,level;
{
  int i,j;
  pml head,tml;

  Leveldata[level].rooms[room].lighted = (random_range(100) > level*2);

  for(i=Leveldata[level].rooms[room].left+1;
      i<=Leveldata[level].rooms[room].right-1;
      i++)
    for (j=Leveldata[level].rooms[room].top+1;
	 j<=Leveldata[level].rooms[room].bottom-1;
	 j++) 
      Dungeon[level][i][j].locchar = FLOOR;
  for(i=Leveldata[level].rooms[room].left;
      i<=Leveldata[level].rooms[room].right;
      i++)
    for (j=Leveldata[level].rooms[room].top;
	 j<=Leveldata[level].rooms[room].bottom;
	 j++)
	Dungeon[level][i][j].roomnumber = room+ROOMBASE;

  /* party room */
  if ((random_range(1000) < 2*level) &&
      ((Leveldata[level].rooms[room].right -
	Leveldata[level].rooms[room].left) < 11) &&
      ((Leveldata[level].rooms[room].bottom -
	Leveldata[level].rooms[room].top) < 11)) {
    head = tml = ((pml) (malloc(sizeof(mltype))));
    for(i=Leveldata[level].rooms[room].left+1;
	i<=Leveldata[level].rooms[room].right-1;
	i++)
      for (j=Leveldata[level].rooms[room].top+1;
	   j<=Leveldata[level].rooms[room].bottom-1;
	   j++) 
	if ((Dungeon[level][i][j].locchar == FLOOR) &&
	    (Dungeon[level][i][j].creature == NULL)) {
	  Dungeon[level][i][j].things = ((pol) malloc(sizeof(oltype)));
	  Dungeon[level][i][j].things->thing = ((pob) create_object());
	  Dungeon[level][i][j].things->next = NULL;
	  
	  Dungeon[level][i][j].creature = (pmt) m_create(i,j,WANDERING,Dlevel);
	  Dungeon[level][i][j].creature->x = i;
	  Dungeon[level][i][j].creature->y = j;
	  
	  tml->next = ((pml) malloc(sizeof(mltype)));
	  tml->next->m = Dungeon[level][i][j].creature;
	  tml=tml->next;
	}

    if (Mlist[level] == NULL) {
      Mlist[level] = head->next;
      tml->next = NULL;
    }
    else {
      tml->next = Mlist[level];
      Mlist[level] = head->next;
    }
  }
}


/* keep going in one orthogonal direction or another until we hit our */
/* destination */

void straggle_corridor(fx,fy,tx,ty,level,loc)
int fx,fy,tx,ty,level;
char loc;
{
  int dx,dy;
  while ((fx != tx) || (fy != ty)) {
    dx = tx - fx;
    dy = ty - fy;
    if (random_range(abs(dx)+abs(dy)) < abs(dx))
      corridor_crawl(&fx,&fy,sign(dx),0,random_range(abs(dx))+1,level,loc);
    else corridor_crawl(&fx,&fy,0,sign(dy),random_range(abs(dy))+1,level,loc);
  }
}

/* now assumes rectangular rooms */
void room_connect_corridor(fromroom,toroom,level)
int fromroom,toroom,level;
{
  int fx,fy,tx,ty,sx,sy,fromdoor=FALSE,todoor=FALSE;
  roomcenter(fromroom,&fx,&fy,level);
  roomcenter(toroom,&tx,&ty,level);
  while ((! todoor) && ((tx != fx) || (ty != fy))) {
    sx = sign(tx - fx);
    sy = sign(ty - fy);
    fx += sx;
    fy += sy;
    if ((! fromdoor) && (Dungeon[level][fx][fy].locchar == WALL)) {
      makedoor(fx,fy,level);
      fromdoor = TRUE;
    }
    else if ((! todoor) && 
	     (Dungeon[level][fx][fy].roomnumber-ROOMBASE == toroom)) {
      makedoor(fx,fy,level);
      todoor = TRUE;
    }
    else {
      Dungeon[level][fx][fy].locchar = FLOOR;
      if(Dungeon[level][fx][fy].roomnumber < ROOMBASE)
	Dungeon[level][fx][fy].roomnumber = RS_CORRIDOR;
    }
  }      
}


void makedoor(x,y,level)
int x,y,level;
{
  if (random_range(20) <= level/10) {
    Dungeon[level][x][y].locchar = FLOOR;
    Dungeon[level][x][y].secret = TRUE;
  }
  else if (random_range(20)<=level/2) {
    Dungeon[level][x][y].locchar = CLOSED_DOOR;
    if (random_range(20) <= level/10) 
      Dungeon[level][x][y].secret = TRUE;
    if (random_range(40) <= level) 
      Dungeon[level][x][y].aux = LOCKED;
    else Dungeon[level][x][y].aux = UNLOCKED;
  }
  else {
    Dungeon[level][x][y].locchar = OPEN_DOOR;
    Dungeon[level][x][y].aux = UNLOCKED;
  }
  if (! Dungeon[level][x][y].secret) {
    Dungeon[level][x][y+1].stopsrun = TRUE;
    Dungeon[level][x+1][y].stopsrun = TRUE;
    Dungeon[level][x-1][y].stopsrun = TRUE;
    Dungeon[level][x][y-1].stopsrun = TRUE;
  }
}


void corridor_crawl(fx,fy,sx,sy,n,level,loc)
int *fx,*fy,sx,sy,n,level;
char loc;
{
  int i;
  for (i=0;i<n;i++) {
    *fx += sx;
    *fy += sy;
    Dungeon[level][*fx][*fy].locchar = loc;
    if (loc==WATER) {
      Dungeon[level][*fx][*fy].p_locf = L_WATER;
      if (Dungeon[level][*fx][*fy].roomnumber < ROOMBASE) 
	Dungeon[level][*fx][*fy].roomnumber = RS_PONDS;
    }
    else if (loc==FLOOR) {
      Dungeon[level][*fx][*fy].p_locf = L_NO_OP;
      if (Dungeon[level][*fx][*fy].roomnumber < ROOMBASE) 
	Dungeon[level][*fx][*fy].roomnumber = RS_CORRIDOR;
    }
  }
}





char *roomname(index)
int index;
{
  switch(index) {
  case RS_DESTINY: return("The Halls of Fate");
  case RS_DRUID: return("The Great Henge");
  case RS_HECATE: return("The Church of the Far Side");
  case RS_SET: return("The Temple of the Black Hand");
  case RS_ATHENA: return("The Parthenon");
  case RS_ODIN: return("The Shrine of the Noose");
  case RS_ADEPT: return("Labyrinth of The Adept's Challenge."); break;
  case RS_WYRM: return("The Sunken Cavern of the Great Wyrm."); break;
  case RS_OCEAN: return("The Underground Ocean."); break;
  case RS_PONDS: return("A series of subterranean pools and streams.");
  case RS_DRAGONLORD: return("The Lair of the DragonLord."); break;
  case RS_GOBLINKING: return("The Caves of the Goblins."); break;
  case RS_CAVERN: return("A vast natural cavern."); break;
  case RS_CORRIDOR: return("A dimly lit corridor."); break;
  case RS_WALLSPACE: return("A niche hollowed out of the wall.");
  case ROOMBASE+0: return("An abandoned garderobe."); break;
  case ROOMBASE+1: return("A dungeon cell."); break;
  case ROOMBASE+2: return("A tiled chamber."); break;
  case ROOMBASE+3: return("A crystal cavern."); break;
  case ROOMBASE+4: return("Someone's bedroom."); break;
  case ROOMBASE+5: return("An old storeroom."); break;
  case ROOMBASE+6: return("A room with charred walls."); break;
  case ROOMBASE+7: return("A marble hall."); break;
  case ROOMBASE+8: return("An eerie cave."); break;
  case ROOMBASE+9: return("A ransacked treasure-chamber."); break;
  case ROOMBASE+10: return("A smoke-filled room."); break;
  case ROOMBASE+11: return("A well-appointed apartment."); break;
  case ROOMBASE+12: return("An antechamber."); break;
  case ROOMBASE+13: return("An unoccupied harem."); break;
  case ROOMBASE+14: return("A multi-purpose room."); break;
  case ROOMBASE+15: return("A room filled with stalactites."); break;
  case ROOMBASE+16: return("An underground greenhouse."); break;
  case ROOMBASE+17: return("A water closet."); break;
  case ROOMBASE+18: return("A study."); break;
  case ROOMBASE+19: return("A living room."); break;
  case ROOMBASE+20: return("A comfortable den."); break;
  case ROOMBASE+21: return("An abatoir."); break;
  case ROOMBASE+22: return("A boudoir.");break;
  case ROOMBASE+23: return("A star chamber.");break;
  case ROOMBASE+24: return("A manmade cavern."); break;
  case ROOMBASE+25: return("A sewer control room");break;
  default: return("A room of mystery and allure."); break;
  }
}

/* is a room connected to a lower number room? */
int connected(fromroom,toroom,level)
{
  int i,connected=FALSE;

  for (i=0;
       ((i<Leveldata[level].rooms[fromroom].numconnections) &&
	(! connected));
       i++)
    connected != (Leveldata[level].rooms[fromroom].connections[i] == toroom);
  return(connected);
}

/* establishes a connection from room to a lower numbered room */
void connect_room(fromroom,toroom,level)
int fromroom,toroom,level;
{
  /* What a mess! */
  Leveldata[level].rooms[fromroom].connections[(Leveldata[level].rooms[fromroom].numconnections)++] = toroom;
}



void roomcenter(room,x,y,level)
int room,*x,*y,level;
{
  *x = (Leveldata[level].rooms[room].left +
	Leveldata[level].rooms[room].right) / 2;
  *y = (Leveldata[level].rooms[room].top + 
	Leveldata[level].rooms[room].bottom) / 2;
}



void change_level(level,sitechar)
int level;
char sitechar;
{
  int i,j,found=FALSE;
  for(i=0;i<WIDTH;i++)
    for(j=0;j<LENGTH;j++) 
      if ((Dungeon[level][i][j].locchar == sitechar) && (! found)){ 
	found = TRUE;
	Player.x = i;
	Player.y = j;
	break;
      }
}


void install_traps(level)
int level;
{
  int i,j;
  for(i=0;i<WIDTH;i++)
    for(j=0;j<LENGTH;j++)
      if ((Dungeon[level][i][j].locchar == FLOOR) &&
	  (Dungeon[level][i][j].p_locf == L_NO_OP) &&
	  random_range(500) <= ((int)(level/6)))
	Dungeon[level][i][j].p_locf = TRAP_BASE+random_range(NUMTRAPS);
}


void cavern_level(level,special_level)
int level;
int special_level;
{
  int i,j,fx,fy,tx,ty;
  pob tob;

  Leveldata[level].numrooms = 1;

  if (special_level==1) 
    Leveldata[level].rooms[0].rsi = RS_GOBLINKING;
  else if (special_level==2)
    Leveldata[level].rooms[0].rsi = RS_DRAGONLORD;
  else Leveldata[level].rooms[0].rsi = RS_CAVERN;
  Leveldata[level].rooms[0].top = random_range((int)(LENGTH/2));
  Leveldata[level].rooms[0].left = random_range((int)(WIDTH/2));
  Leveldata[level].rooms[0].right = Leveldata[level].rooms[0].left +
    random_range((int)(WIDTH/4)) +
      ((int)(WIDTH/4));
  Leveldata[level].rooms[0].bottom = Leveldata[level].rooms[0].top +
    random_range((int)(LENGTH/4)) +
      ((int)(LENGTH/4));

  build_room(0,level);
  
  
  for (i=0;i<24;i++) {
    do {
      tx = random_range(WIDTH);
      ty = random_range(LENGTH);
    } while (Dungeon[level][tx][ty].locchar != FLOOR);
    fx = random_range(WIDTH-2)+1;
    fy = random_range(LENGTH-2)+1;
    straggle_corridor(fx,fy,tx,ty,level,FLOOR);
  }
  while (random_range(3)==1) {
    do {
      tx = random_range(WIDTH);
      ty = random_range(LENGTH);
    } while (Dungeon[level][tx][ty].locchar != FLOOR);
    fx = random_range(WIDTH-2)+1;
    fy = random_range(LENGTH-2)+1;
    straggle_corridor(fx,fy,tx,ty,level,WATER);
  }
  if (special_level==1) {
    populate_level(level,ML1+6); /* ML1+6 == goblin */
    do {
      tx = random_range(WIDTH);
      ty = random_range(LENGTH);
    } while (Dungeon[level][tx][ty].locchar != FLOOR);
    
    Mlist[level]->m = 
      Dungeon[level][tx][ty].creature = 
	((pmt) make_creature(ML3+5));
    Mlist[level]->m->x = tx;
    Mlist[level]->m->y = ty;
  }
  else if (special_level==2) { /* dragonslair */
    populate_level(level,ML8+3); /* dragon */
    do {
      tx = random_range(WIDTH);
      ty = random_range(LENGTH);
    } while (Dungeon[level][tx][ty].locchar != FLOOR);
    Mlist[level]->m = 
      Dungeon[level][tx][ty].creature = 
	((pmt) make_creature(ML10+3)); /*dragonlord*/
    Mlist[level]->m->x = tx;
    Mlist[level]->m->y =ty;
  }
  else populate_level(level,-1);
  stock_level(level);
}









void install_specials(level)
int level;
{
  int i,j,x,y;

  for(x=0;x<WIDTH;x++)
    for(y=0;y<LENGTH;y++)
      if ((Dungeon[level][x][y].locchar == FLOOR) &&
	  (Dungeon[level][x][y].p_locf == L_NO_OP) &&
	  random_range(500) <= ((int)(level/4))) {
	i = random_range(100);
	if (i < 10) {
	  Dungeon[level][x][y].locchar = ALTAR;
	  Dungeon[level][x][y].p_locf = L_ALTAR;
	  Dungeon[level][x][y].aux = random_range(10);
	}
	else if (i < 25) {
	  Dungeon[level][x][y].locchar = WATER;
	  Dungeon[level][x][y].p_locf = L_MAGIC_POOL;
	}
	else if (i < 35) {
	  Dungeon[level][x][y].locchar = LAVA;
	  Dungeon[level][x][y].p_locf = L_LAVA;
	}
	else if (i < 45) {
	  Dungeon[level][x][y].locchar = FIRE;
	  Dungeon[level][x][y].p_locf = L_FIRE;
	}
	else if (i < 50) {
	  Dungeon[level][x][y].locchar = LIFT;
	  Dungeon[level][x][y].p_locf = L_LIFT;
	}
	else if (i < 55) {
	  Dungeon[level][x][y].locchar = HEDGE;
	  Dungeon[level][x][y].p_locf = L_HEDGE;
	}
	else if (i < 57) {
	  Dungeon[level][x][y].locchar = HEDGE;
	  Dungeon[level][x][y].p_locf = L_TRIFID;
	}
	else if (i< 65) {
	  Dungeon[level][x][y].locchar = STATUE;
	  if (random_range(100) < Dlevel) 
	    for (j=0;j<8;j++) {
	      if (Dungeon[level][x+Dirs[0][j]][y+Dirs[1][j]].p_locf != L_NO_OP)
		Dungeon[level][x+Dirs[0][j]][y+Dirs[1][j]].locchar = FLOOR;
	      Dungeon[level][x+Dirs[0][j]][y+Dirs[1][j]].p_locf = 
		L_STATUE_WAKE;
	    }
	}
	else if (i<70) {
	  Dungeon[level][x][y].locchar = WHIRLWIND;
	  Dungeon[level][x][y].p_locf = L_WHIRLWIND;
	}
	else {
	  Dungeon[level][x][y].locchar = WATER;
	  Dungeon[level][x][y].p_locf = L_WATER;
	}
      }
}


void grid_level(level)
int level;
{
  int i,j,k,l,fx,fy,tx,ty,x,y;

  Leveldata[level].numrooms = 0;


  for (i=1;i<LENGTH-1;i+=random_range(4)+4) {
    for (l=1;l<WIDTH-1;l++) {
      Dungeon[level][l][i].locchar = FLOOR;
      Dungeon[level][l][i].roomnumber = RS_CORRIDOR;
    }
  }
  for (i=1;i<WIDTH-1;i+=random_range(6)+3) {
    for (l=1;l<LENGTH-1;l++) {
      Dungeon[level][i][l].locchar = FLOOR;
      Dungeon[level][i][l].roomnumber = RS_CORRIDOR;
    }
  }
  
  Leveldata[level].numrooms = random_range(4)+4;

  for (i=0;i<Leveldata[level].numrooms;i++) {
    Leveldata[level].rooms[i].rsi = random_range(NUMROOMNAMES)+ROOMBASE;
    Leveldata[level].rooms[i].top = random_range(LENGTH-8)+1;
    Leveldata[level].rooms[i].left = random_range(WIDTH-16)+1;
    Leveldata[level].rooms[i].right = 
      Leveldata[level].rooms[i].left +
	random_range(8)+8;
    Leveldata[level].rooms[i].bottom =
      Leveldata[level].rooms[i].top + 
	random_range(4)+4;
    build_room(i,level);
  }

  while (random_range(3)==1) {
    fx = random_range(WIDTH-2)+1;
    fy = random_range(LENGTH-2)+1;
    tx = random_range(WIDTH-2)+1;
    ty = random_range(LENGTH-2)+1;
    straggle_corridor(fx,fy,tx,ty,level,WATER);
  }

  populate_level(level,-1);
  stock_level(level);
}


/* For each level,
   Make one stairway back to wherever player came from. 
   There are always four stairways. One goes up one level, one down one
   level, one goes up multiple levels, and one goes down multiple levels
*/
void make_stairs(fromlevel,level,sitechar)
int fromlevel;
int level;
char sitechar;
{
  int i,j,k;
  do {
    i = random_range(WIDTH);
    j = random_range(LENGTH);
  } while (Dungeon[level][i][j].locchar != FLOOR);
  Player.x = i;
  Player.y = j;
  Dungeon[level][Player.x][Player.y].locchar = sitechar;
  Dungeon[level][Player.x][Player.y].aux = fromlevel;
  Dungeon[level][Player.x][Player.y].stopsrun = TRUE;
  if (fromlevel != level - 1) {
    do {
      i = random_range(WIDTH);
      j = random_range(LENGTH);
    } while (Dungeon[level][i][j].locchar != FLOOR);
    Player.x = i;
    Player.y = j;
    Dungeon[level][Player.x][Player.y].locchar = UP;
    Dungeon[level][Player.x][Player.y].aux = level - 1;
    Dungeon[level][Player.x][Player.y].stopsrun = TRUE;
  }
  
  /* only one final portal... */
  if ((fromlevel != level + 1) && (level != NUMLEVELS-2)) {
    do {
      i = random_range(WIDTH);
      j = random_range(LENGTH);
    } while (Dungeon[level][i][j].locchar != FLOOR);
    Dungeon[level][i][j].locchar = DOWN;
    Dungeon[level][i][j].stopsrun = TRUE;
    Dungeon[level][i][j].aux = level + 1;
  }


  if (fromlevel >= level - 1) {
    do {
      i = random_range(WIDTH);
      j = random_range(LENGTH);
    } while (Dungeon[level][i][j].locchar != FLOOR);
    Dungeon[level][i][j].locchar = UP;
    Dungeon[level][i][j].stopsrun = TRUE;
    Dungeon[level][i][j].aux = max(0,level - (random_range(3)+2));
  }

  if (fromlevel <= level + 1) {
    do {
      i = random_range(WIDTH);
      j = random_range(LENGTH);
    } while (Dungeon[level][i][j].locchar != FLOOR);
    Dungeon[level][i][j].locchar = DOWN;
    Dungeon[level][i][j].stopsrun = TRUE;
    Dungeon[level][i][j].aux = level + (random_range(3)+2);
    if (Dungeon[level][i][j].aux > NUMLEVELS -10)
      Dungeon[level][i][j].aux = level+1;
    if (level == NUMLEVELS-2) 
      Dungeon[level][i][j].p_locf = L_WARNING;
  }
}



void pond_level(level,special_level)
int level;
int special_level;
{
  int i,l,r,t,b,k,j,fx,fy,tx,ty,rsi;
  pob tob;

  if (special_level==1) 
    rsi = RS_OCEAN;
  else if (special_level==2)
    rsi = RS_WYRM;
  else rsi = RS_PONDS;


  for(i=1;i<WIDTH-1;i++)
    for(j=1;j<LENGTH-1;j++) {
      Dungeon[level][i][j].roomnumber = rsi;
      Dungeon[level][i][j].p_locf = L_WATER;
      Dungeon[level][i][j].locchar = WATER;
    }

  tx = random_range(WIDTH-2)+1;
  ty = random_range(LENGTH-2)+1;
  for (i=0;i<24;i++) {
    fx = tx;
    fy = ty;
    tx = random_range(WIDTH-2)+1;
    ty = random_range(LENGTH-2)+1;
    straggle_corridor(fx,fy,tx,ty,level,FLOOR);
  }

  if (special_level==1) {
    populate_level(level,ML4+8); /* giant croc */
    do {
      i = random_range(WIDTH);
      j = random_range(LENGTH);
    }while (Dungeon[level][i][j].locchar != WATER);
    Mlist[level]->m = 
      Dungeon[level][i][j].creature = 
	((pmt) make_creature(ML6+6)); /* triton */
    Mlist[level]->m->x = i;
    Mlist[level]->m->y = j;
  }
  else if (special_level==2) {
    populate_level(level,ML8+6); /* tigershark */
    do {
      i = random_range(WIDTH);
      j = random_range(LENGTH);
    }while (Dungeon[level][i][j].locchar != WATER);
    Mlist[level]->m = 
      Dungeon[level][i][j].creature = 
	((pmt) make_creature(ML9+6)); /*great wyrm*/
    Mlist[level]->m->x = i;
    Mlist[level]->m->y = j;
  }
  else populate_level(level,-1);
  stock_level(level);

}


/* draw a bunch of random non-intersecting rooms and draw corridors between 
   them */
void room_level(level,special_level)
int level,special_level;
{
  int roomsmade[30][30]; /* 30 is an arbitrary large enough number.... */
  int i,j,k,l,x,y,templetype,left,right,top,bottom;
  pml ml;

  if (special_level) Leveldata[level].numrooms = 23;
  else Leveldata[level].numrooms = 10+random_range(20);
  for(i=0;i<30;i++)
    for(j=0;j<30;j++)
      roomsmade[i][j] = FALSE;
  for (i=0;i<Leveldata[level].numrooms;i++) {
    do {
      k = random_range(6); /* 64 / 10, 10 is max room width */
      l = random_range(6); /* 64 / 10, 10 is max room length */
    } while(roomsmade[k][l]);
    roomsmade[k][l]=TRUE;
    Leveldata[level].rooms[i].top = (l * 10) + 1;
    Leveldata[level].rooms[i].bottom = (l * 10) + 5 + random_range(5);
    Leveldata[level].rooms[i].left = (k * 10) + 1;
    Leveldata[level].rooms[i].right = (k * 10) + 5 + random_range(5);
    Leveldata[level].rooms[i].rsi = random_range(NUMROOMNAMES)+ROOMBASE;
    build_room(i,level);
  }
  for (i=0;i<Leveldata[level].numrooms-1;i++) 
    room_connect_corridor(i,i+1,level);

  if (special_level) {
    do {
      x = random_range(WIDTH);
      y = random_range(LENGTH);
    } while ((Dungeon[level][x][y].locchar != FLOOR) &&
	     (Dungeon[level][x][y].roomnumber < ROOMBASE));
    Dungeon[level][x][y].locchar = ALTAR;
    Dungeon[level][x][y].p_locf = L_ALTAR;
    Dungeon[level][x][y].aux = special_level;
    ml = (pml) malloc(sizeof(mltype));
    ml->next = NULL;
    ml->m = 
      Dungeon[level][x][y].creature = 
	((pmt) make_hiscore_npc(special_level));
    ml->m->x = x;
    ml->m->y = y;
    Mlist[level] = ml;
    populate_level(level,ML8+9);
    left = 
      Leveldata[level].rooms[Dungeon[level][x][y].roomnumber-ROOMBASE].left;
    right = 
      Leveldata[level].rooms[Dungeon[level][x][y].roomnumber-ROOMBASE].right;
    top = 
      Leveldata[level].rooms[Dungeon[level][x][y].roomnumber-ROOMBASE].top;
    bottom = 
      Leveldata[level].rooms[Dungeon[level][x][y].roomnumber-ROOMBASE].bottom;
    switch(special_level) {
    case ODIN: templetype = RS_ODIN; break;
    case ATHENA: templetype = RS_ATHENA; break;
    case SET: templetype = RS_SET; break;
    case HECATE: templetype = RS_HECATE; break;
    case DRUID: templetype = RS_DRUID; break;
    case DESTINY: templetype = RS_DESTINY; break;
    }
    for(k=left;k<=right;k++)
      for(l=top;l<bottom;l++)
	Dungeon[level][k][l].roomnumber = templetype;
  }
  populate_level(level,-1);
  stock_level(level);
}

