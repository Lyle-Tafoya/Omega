/* omega copyright (c) 1987,1988,1989 by Laurence Raphael Brothers */
/* gen2.c */
/* level generator functions */

#include "glob.h"



/* For each level, there should be one stairway going up and one down. 
fromlevel determines whether the player is placed on the up or the down
staircase. The aux value is currently unused elsewhere, but is set 
to the destination level. */

void make_stairs(fromlevel)
int fromlevel;
{
  int i,j;
  /* no stairway out of astral */
  if (Current_Environment != E_ASTRAL) {
    findspace(&i,&j,-1);
    Level->site[i][j].locchar = STAIRS_UP;
    Level->site[i][j].aux = Level->depth-1;
    lset(i,j,STOPS);
    if (fromlevel >= 0 && fromlevel < Level->depth) { 
      Player.x = i;
      Player.y = j;
    }
  }
  if (Level->depth < MaxDungeonLevels) {
    findspace(&i,&j,-1);
    Level->site[i][j].locchar = STAIRS_DOWN;
    Level->site[i][j].aux = Level->depth+1;
    lset(i,j,STOPS);
    if (fromlevel > Level->depth) { 
      Player.x = i;
      Player.y = j;
    }
  }
}





/* tactical map generating functions */


void make_country_screen(terrain)
Symbol terrain;
{
  int i,j;
  TempLevel = Level;
  if (ok_to_free(TempLevel)) {
#ifndef SAVE_LEVELS
    free_level(TempLevel);
#endif
    TempLevel = NULL;
  }
#ifndef SAVE_LEVELS
  Level = ((plv) checkmalloc(sizeof(levtype)));
#else
  msdos_changelevel(TempLevel,0,-1);
  Level = &TheLevel;
#endif
  clear_level(Level);
  Level->environment = E_TACTICAL_MAP;
  Level->generated = TRUE;
  switch(terrain) {
  case FOREST: make_forest(); break;
  case JUNGLE: make_jungle(); break;
  case SWAMP: make_swamp(); break;
  case RIVER: make_river(); break;
  case MOUNTAINS: case PASS: make_mountains(); break;
  case ROAD: make_road(); break;
  default: make_plains(); break;
  }
  if (nighttime()) {
    print3("Night's gloom shrouds your sight.");    
    for(i=0;i<WIDTH;i++)
      for(j=0;j<LENGTH;j++) {
	Level->site[i][j].showchar = SPACE;    
	Level->site[i][j].lstatus = 0;
      }
  }
}

void make_general_map(terrain)
char *terrain;
{
  int i, j;
  int size = strlen(terrain);
  char curr;

  for (i=0;i<WIDTH;i++)
    for (j=0;j<LENGTH;j++) {
      if ((i == 0 && j == 0) || !random_range(5))
	curr = terrain[random_range(size)];
      else if (j == 0 || (random_range(2) && i > 0))
	curr = Level->site[i - 1][j].locchar&0xff;
      else
	curr = Level->site[i][j - 1].locchar&0xff;
      switch (curr) {
	case (FLOOR&0xff):
	  Level->site[i][j].locchar = Level->site[i][j].showchar = FLOOR;
	  Level->site[i][j].p_locf = L_NO_OP;
	  break;
	case (HEDGE&0xff):
	  Level->site[i][j].locchar = Level->site[i][j].showchar = HEDGE;
	  Level->site[i][j].p_locf = L_HEDGE;
	  break;
	case (WATER&0xff):
	  Level->site[i][j].locchar = Level->site[i][j].showchar = WATER;
	  Level->site[i][j].p_locf = L_WATER;
	  break;
	case (RUBBLE&0xff):
	  Level->site[i][j].locchar = Level->site[i][j].showchar = RUBBLE;
	  Level->site[i][j].p_locf = L_RUBBLE;
	  break;
      }
      Level->site[i][j].lstatus = SEEN+LIT;
      Level->site[i][j].roomnumber = RS_COUNTRYSIDE;
      if ((i == 0) || (j == 0) || (i == WIDTH-1) || (j == LENGTH-1))
	Level->site[i][j].p_locf = L_TACTICAL_EXIT;
    }
}

void make_plains()
{
  make_general_map(".");
}

void make_road()
{
  int x, y;
  make_general_map("\"\"~4....");
  for (x = WIDTH/2 - 3; x <= WIDTH/2 + 3; x++)
    for (y = 0; y < LENGTH; y++) {
      Level->site[x][y].locchar = Level->site[x][y].showchar = FLOOR;
      if (y != 0 && y != LENGTH - 1)
	Level->site[x][y].p_locf = L_NO_OP;
    }
}



void make_forest()
{
  make_general_map("\".");
  straggle_corridor(0,random_range(LENGTH),WIDTH,random_range(LENGTH),
    WATER,RS_COUNTRYSIDE);
}


void make_jungle()
{
  make_general_map("\"\".");
}


void make_river()
{
  int i,y,y1;
  make_general_map("\".......");
  y = random_range(LENGTH);
  y1 = random_range(LENGTH);
  straggle_corridor(0,y,WIDTH,y1,WATER,RS_COUNTRYSIDE);
  for(i=0;i<7;i++) {
    if (y > LENGTH/2) y--;
    else y++;
    if (y1 > LENGTH/2) y1--;
    else y1++;
    straggle_corridor(0,y,WIDTH,y1,WATER,RS_COUNTRYSIDE);
  }
}


void make_mountains()
{
  int i,x,y,x1,y1;
  make_general_map("4...");
  x = 0;
  y = random_range(LENGTH);
  x1 = WIDTH;
  y1 = random_range(LENGTH);
  straggle_corridor(x,y,x1,y1,WATER,RS_COUNTRYSIDE);
  for(i=0;i<7;i++) {
    x = random_range(WIDTH); 
    x1 = random_range(WIDTH);
    y = 0;
    y1 = LENGTH;
    straggle_corridor(x,y,x1,y1,WATER,RS_COUNTRYSIDE);
  }
}



void make_swamp()
{
  make_general_map("~~\".");
}






/* builds a room. Then, for each successive room, sends off at least one
corridor which is guaranteed to connect up to another room, thus guaranteeing
fully connected level. */

void room_level()
{
  int i,fx,fy,tx,ty,t,l,e;
  char rsi;

  Level->numrooms = random_range(8)+9;

  do {
    t = random_range(LENGTH-10)+1;
    l = random_range(WIDTH-10)+1;
    e = 4+random_range(5);
  } while ((Level->site[l][t].roomnumber != RS_WALLSPACE) ||
	   (Level->site[l+e][t].roomnumber != RS_WALLSPACE) ||
	   (Level->site[l][t+e].roomnumber != RS_WALLSPACE) ||
	   (Level->site[l+e][t+e].roomnumber != RS_WALLSPACE));
  if (Current_Dungeon == E_SEWERS) {
    if (random_range(2)) rsi = ROOMBASE+25;
    else rsi = ROOMBASE+random_range(NUMROOMNAMES);
  }
  else rsi = ROOMBASE+random_range(NUMROOMNAMES);
  build_room(l,t,e,rsi,1);


  for (i=2;i<=Level->numrooms;i++) {
    do {
      t = random_range(LENGTH-10)+1;
      l = random_range(WIDTH-10)+1;
      e = 4+random_range(5);
    } while ((Level->site[l][t].roomnumber != RS_WALLSPACE) ||
	     (Level->site[l+e][t].roomnumber != RS_WALLSPACE) ||
	     (Level->site[l][t+e].roomnumber != RS_WALLSPACE) ||
	     (Level->site[l+e][t+e].roomnumber != RS_WALLSPACE));
    if (Current_Dungeon == E_SEWERS) {
      if (random_range(2)) rsi = ROOMBASE+25;
      else rsi = ROOMBASE+random_range(NUMROOMNAMES);
    }
    else rsi = ROOMBASE+random_range(NUMROOMNAMES);
    build_room(l,t,e,rsi,i);
    

    /* corridor which is guaranteed to connect */
    findspace(&tx,&ty,i);

    /* figure out where to start corridor from */
    if ((ty <= t) && (tx <= l+e)) {
      fx = l+1+random_range(e-1);
      fy = t;
    }
    else if ((tx >= l+e) && (ty <= t+e)) {
      fx = l+e;
      fy = t+1+random_range(e-1);
    }
    else if ((ty >= t+e) && (tx >= l)) {
      fx = l+1+random_range(e-1);
      fy = t+e;
    }
    else {
      fx = l;
      fy = t+1+random_range(e-1);
    }

    room_corridor(fx,fy,tx,ty,i);


    /* corridor which may not go anywhere */
    if (random_range(2)) {
      findspace(&tx,&ty,i);
      if ((ty <= t) && (tx <= l+e)) {
	fx = l+1+random_range(e-1);
	fy = t;
      }
      else if ((tx >= l+e) && (ty <= t+e)) {
	fx = l+e;
	fy = t+1+random_range(e-1);
      }
      else if ((ty >= t+e) && (tx >= l)) {
	fx = l+1+random_range(e-1);
	fy = t+e;
      }
      else {
	fx = l;
	fy = t+1+random_range(e-1);
      }
      room_corridor(fx,fy,tx,ty,i);
    }
  }

  if (Current_Dungeon == E_SEWERS) {
    if (Level->depth == SEWERLEVELS) {
      findspace(&tx,&ty,-1);
      Level->mlist = ((pml) checkmalloc(sizeof(mltype)));
      Level->mlist->next = NULL;
      Level->mlist->m = 
	Level->site[tx][ty].creature = 
	  ((pmt) make_creature(GREAT_WYRM)); /* The Great Wyrm */
      Level->mlist->m->x = tx;
      Level->mlist->m->y = ty;
    }
  }
  else if (Current_Environment == E_CASTLE) {
    if (Level->depth == CASTLELEVELS) {
      findspace(&tx,&ty,-1);
      Level->site[tx][ty].locchar = STAIRS_DOWN;
      Level->site[tx][ty].p_locf = L_ENTER_COURT;
    }
  }
  else if (Current_Environment == E_VOLCANO) {
    if (Level->depth == VOLCANOLEVELS && !gamestatusp(COMPLETED_VOLCANO)) {
      findspace(&tx,&ty,-1);
      Level->mlist = ((pml) checkmalloc(sizeof(mltype)));
      Level->mlist->next = NULL;
      Level->mlist->m = 
	Level->site[tx][ty].creature = 
	  ((pmt) make_creature(DEMON_EMP)); /* The demon emp */
      Level->mlist->m->x = tx;
      Level->mlist->m->y = ty;
    }
  }
}



/* goes from f to t unless it hits a site which is not a wall and doesn't
   have buildaux field == baux */
void room_corridor(fx,fy,tx,ty,baux)
int fx,fy,tx,ty,baux;
{
  int dx,dy,continuing = TRUE;

  dx = sign(tx-fx);
  dy = sign(ty-fy);

  makedoor(fx,fy);

  fx+=dx;
  fy+=dy;

  while(continuing) {
    Level->site[fx][fy].locchar = FLOOR;
    Level->site[fx][fy].roomnumber = RS_CORRIDOR;
    Level->site[fx][fy].buildaux = baux;
    dx = sign(tx-fx);
    dy = sign(ty-fy);
    if ((dx != 0) && (dy != 0)) {
      if (random_range(2)) dx = 0;
      else if (random_range(2)) dy = 0;
    }
    fx+=dx;
    fy+=dy;
    continuing = (((fx != tx) || (fy != ty)) &&
		  ((Level->site[fx][fy].buildaux == 0) ||
		   (Level->site[fx][fy].buildaux == baux)));
  }
  makedoor(fx,fy);
}





void maze_level()
{
  int i,j,tx,ty,mid;
  char rsi;
  if (Current_Environment == E_ASTRAL)
    switch(Level->depth){
    case 1: rsi = RS_EARTHPLANE; break;
    case 2: rsi = RS_AIRPLANE; break;
    case 3: rsi = RS_WATERPLANE; break;
    case 4: rsi = RS_FIREPLANE; break;
    case 5: rsi = RS_HIGHASTRAL; break;
    }
  else rsi = RS_VOLCANO;
  maze_corridor(random_range(WIDTH-1)+1,
		random_range(LENGTH-1)+1,
		random_range(WIDTH-1)+1,
		random_range(LENGTH-1)+1,
		rsi,0);
  if (Current_Dungeon == E_ASTRAL) {
    for(i=0;i<WIDTH;i++) 
      for(j=0;j<LENGTH;j++) 
	if (Level->site[i][j].locchar == WALL)
	  switch(Level->depth){
	  case 1: Level->site[i][j].aux = 500; break;
	  case 2: 
	    Level->site[i][j].locchar = WHIRLWIND;
	    Level->site[i][j].p_locf = L_WHIRLWIND;
	    break;
	  case 3: 
	    Level->site[i][j].locchar = WATER;
	    Level->site[i][j].p_locf = L_WATER;
	    break;
	  case 4:
	    Level->site[i][j].locchar = FIRE;
	    Level->site[i][j].p_locf = L_FIRE;
	    break;
	  case 5: 
	    Level->site[i][j].locchar = ABYSS;
	    Level->site[i][j].p_locf = L_ABYSS;
	    break;
	  }
    switch(Level->depth) {
    case 1: mid = LORD_EARTH; break; /* Elemental Lord of Earth */
    case 2: mid = LORD_AIR; break; /* Elemental Lord of Air */
    case 3: mid = LORD_WATER; break; /* Elemental Lord of Water */
    case 4: mid = LORD_FIRE; break; /* Elemental Lord of Fire */
    case 5: mid = ELEM_MASTER; break; /* Elemental Master */
    }
    if (Level->depth == 5) {
      findspace(&tx,&ty,-1);
      Level->site[tx][ty].p_locf = L_ENTER_CIRCLE;
      Level->site[tx][ty].locchar = STAIRS_DOWN;
    }
    if (! gamestatusp(COMPLETED_ASTRAL)) {
      findspace(&tx,&ty,-1);
      Level->mlist = ((pml) checkmalloc(sizeof(mltype)));
      Level->mlist->next = NULL;
      Level->mlist->m = 
	Level->site[tx][ty].creature = 
	  ((pmt) make_creature(mid)); 
      Level->mlist->m->x = tx;
      Level->mlist->m->y = ty;
    }
  }
  else if (Current_Environment == E_VOLCANO) {
    if (Level->depth == VOLCANOLEVELS && !gamestatusp(COMPLETED_VOLCANO)) {
      findspace(&tx,&ty,-1);
      Level->mlist = ((pml) checkmalloc(sizeof(mltype)));
      Level->mlist->next = NULL;
      Level->mlist->m = 
	Level->site[tx][ty].creature = 
	  ((pmt) make_creature(DEMON_EMP)); /* The demon emp */
      Level->mlist->m->x = tx;
      Level->mlist->m->y = ty;
    }
  }
}


/* keep drawing corridors recursively for 2^5 endpoints */
void maze_corridor(fx,fy,tx,ty,rsi,num)
int fx,fy,tx,ty;
char rsi,num;
{
  if (num < 6) {
    straggle_corridor(fx,fy,tx,ty,FLOOR,rsi);
    maze_corridor(tx,ty,
		  random_range(WIDTH-1)+1,
		  random_range(LENGTH-1)+1,
		  rsi,num+1);
    
  }
}





