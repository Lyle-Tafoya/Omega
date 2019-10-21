/* omega copyright (C) by Laurence Raphael Brothers, 1987 */
/* ocity.c */
/* some functions to make the city level */

#include <stdio.h>
#include <strings.h>
#include "oglob.h"


/* from olev */
extern pmt m_create(),make_creature();
extern pob create_object();

/* from omon */
extern void m_status_reset();

/* ocity functions */
void load_city(),makecityguard(),makecitymonster(),makecitytreasure();
void mysterycitysite(),assign_city_function();

/* loads the city level */
void load_city()
{
  int i,j,seen=TRUE;
  char site,locsite,digit;
  
  FILE *fd;
  
  strcpy(Str3,OMEGALIB);
  strcat(Str3,"ocity.dat");
  fd = fopen(Str3,"r");
  Mlist[0] = NULL;

  for(j=0;j<LENGTH;j++) {
    for(i=0;i<WIDTH;i++) {
      Dungeon[0][i][j].seen = seen;
      site = getc(fd);

      switch(site) {
      case 'g':
	Dungeon[0][i][j].locchar = FLOOR;
	Dungeon[0][i][j].p_locf = L_GARDEN;
	seen = ! seen;
	break;
      case 'z':
	Dungeon[0][i][j].locchar = FLOOR;
	Dungeon[0][i][j].p_locf = L_MAZE;
	break;
      case 'y':
	Dungeon[0][i][j].locchar = FLOOR;
	Dungeon[0][i][j].p_locf = L_CEMETARY;
	break;
      case 'x':
	assign_city_function(i,j);
	break;
      case 'T':
	Dungeon[0][i][j].locchar = FLOOR;
	Dungeon[0][i][j].p_locf = L_TEMPLE;
	break;
      case 'C':
	Dungeon[0][i][j].locchar = OPEN_DOOR;
	Dungeon[0][i][j].p_locf = L_COLLEGE;
	break;
      case 's':
	Dungeon[0][i][j].locchar = OPEN_DOOR;
	Dungeon[0][i][j].p_locf = L_SORCERORS;
	break;
      case 'M':
	Dungeon[0][i][j].locchar = OPEN_DOOR;
	Dungeon[0][i][j].p_locf = L_MERC_GUILD;
	break;
      case 'c':
	Dungeon[0][i][j].locchar = OPEN_DOOR;
	Dungeon[0][i][j].p_locf = L_CASTLE;
	break;
      case '?':
	mysterycitysite(i,j);
	break;
      case 'J':
	Dungeon[0][i][j].locchar = CLOSED_DOOR;
	Dungeon[0][i][j].p_locf = L_JAIL;
	break;
      case 'A':
	Dungeon[0][i][j].locchar = OPEN_DOOR;
	Dungeon[0][i][j].p_locf = L_ARENA;
	break;
      case 'B':
	Dungeon[0][i][j].locchar = OPEN_DOOR;
	Dungeon[0][i][j].p_locf = L_BANK;
	Dungeon[0][i][j+1].stopsrun = TRUE;
	Dungeon[0][i+1][j].stopsrun = TRUE;
	Dungeon[0][i-1][j].stopsrun = TRUE;
	Dungeon[0][i][j-1].stopsrun = TRUE;
	break;
      case 'X':
	Dungeon[0][i][j].locchar = FLOOR;
	Dungeon[0][i][j].p_locf = L_CITYGATE;
	break;
      case 'Y':
	Dungeon[0][i][j].locchar = CLOSED_DOOR;
	Dungeon[0][i][j].p_locf = L_CHAOSSTORM;
	Dungeon[0][i][j].aux = LOCKED;
	break;
      case 'V':
	Dungeon[0][i][j].locchar = FLOOR;
	Dungeon[0][i][j].p_locf = L_VAULT;
	break;
      case 'S':
	Dungeon[0][i][j].locchar = FLOOR;
	Dungeon[0][i][j].secret = TRUE;
	break;
      case 'G':
	Dungeon[0][i][j].locchar = FLOOR;
	makecityguard(i,j);
	break;
      case '$':
	Dungeon[0][i][j].locchar = FLOOR;
	makecitytreasure(i,j);
	break;
      case '2':
	Dungeon[0][i][j].locchar = ALTAR;
	Dungeon[0][i][j].p_locf = L_ALTAR;
	Dungeon[0][i][j].aux = ODIN;
	break;
      case '3':
	Dungeon[0][i][j].locchar = ALTAR;
	Dungeon[0][i][j].p_locf = L_ALTAR;
	Dungeon[0][i][j].aux = SET;
	break;
      case '4':
	Dungeon[0][i][j].locchar = ALTAR;
	Dungeon[0][i][j].p_locf = L_ALTAR;
	Dungeon[0][i][j].aux = ATHENA;
	break;
      case '5':
	Dungeon[0][i][j].locchar = ALTAR;
	Dungeon[0][i][j].p_locf = L_ALTAR;
	Dungeon[0][i][j].aux = HECATE;
	break;
      case '6':
	Dungeon[0][i][j].locchar = ALTAR;
	Dungeon[0][i][j].p_locf = L_ALTAR;
	Dungeon[0][i][j].aux = DESTINY;
	break;
      case 'D':
	Dungeon[0][i][j].locchar = ALTAR;
	Dungeon[0][i][j].p_locf = L_DRUID;
	Dungeon[0][i][j].aux = DRUID;
	break;
      case '^':
	Dungeon[0][i][j].locchar = FLOOR;
	Dungeon[0][i][j].p_locf = TRAP_BASE+random_range(NUMTRAPS);
	break;
      case '"':
	Dungeon[0][i][j].locchar = HEDGE;
	if (random_range(20))
	  Dungeon[0][i][j].p_locf = L_HEDGE;
	else
	  Dungeon[0][i][j].p_locf = L_TRIFID;
	break;
      case '\'':
	Dungeon[0][i][j].locchar = HEDGE;
	if (random_range(20))
	  Dungeon[0][i][j].p_locf = L_HEDGE;
	else
	  Dungeon[0][i][j].p_locf = L_TRIFID;
	seen = ! seen;
	break;
      case '~':
	Dungeon[0][i][j].locchar = WATER;
	Dungeon[0][i][j].p_locf = L_WATER;
	break;
      case '&':
	Dungeon[0][i][j].locchar = WALL;
	Dungeon[0][i][j].aux = 100;
	seen = ! seen;
	break;
      case '>':
	Dungeon[0][i][j].locchar = DOWN;
	Dungeon[0][i][j].p_locf = L_SEWER;
	Dungeon[0][i][j].aux = 1;
	break;
      case '*':
	Dungeon[0][i][j].locchar = WALL;
	Dungeon[0][i][j].aux = 10;
	break;
      case '#':
	Dungeon[0][i][j].locchar = WALL;
	Dungeon[0][i][j].aux = 100;
	break;
      default:
	Dungeon[0][i][j].locchar = site;
	break;
      }

      if (Dungeon[0][i][j].seen) {
	if (Dungeon[0][i][j].secret)
	  Dungeon[0][i][j].showchar = '#';
	else Dungeon[0][i][j].showchar = Dungeon[0][i][j].locchar;
      }
    }
    fscanf(fd,"\n");
  }
}

void makecityguard(i,j)
int i,j;
{
  pml tml = ((pml) (calloc(1,sizeof(mltype))));
  tml->m = (Dungeon[0][i][j].creature = make_creature(ML0+3));
  tml->m->x = i;
  tml->m->y = j;
  tml->next = Mlist[0];
  Mlist[0] = tml;
}


void makecitymonster(i,j)
int i,j;
{
  pml tml = ((pml) (calloc(1,sizeof(mltype))));
  tml->m = (Dungeon[0][i][j].creature = m_create(i,j,FALSE,-1));
  m_status_reset(tml->m,AWAKE);
  tml->m->wakeup = 1;
  tml->next = Mlist[0];
  Mlist[0] = tml;
}

void makecitytreasure(i,j)
int i,j;
{
  pol tol = ((pol) (calloc(1,sizeof(oltype))));

  tol->next=NULL;
  Dungeon[0][i][j].things = tol;
  tol->thing = NULL;
  tol->thing = create_object();
  Dungeon[0][i][j].things = tol;
}

void assign_city_function(x,y)
int x,y;
{
  static int setup=0;
  static int next=0;
  static int permutation[64]; /* number of x's in city map */
  int i,j,k,l;

  Dungeon[0][x][y+1].stopsrun = TRUE;
  Dungeon[0][x+1][y].stopsrun = TRUE;
  Dungeon[0][x-1][y].stopsrun = TRUE;
  Dungeon[0][x][y-1].stopsrun = TRUE;


  if (setup == 0) {
    setup = 1;
    for(i=0;i<64;i++)
      permutation[i] = i;
    for(i=0;i<500;i++) {
      j = random_range(64);
      k = random_range(64);
      l = permutation[j];
      permutation[j] = permutation[k];
      permutation[k] = l;
    }
  }
  if (next > 63) { /* in case someone changes the no. of x's */
    Dungeon[0][x][y].locchar = CLOSED_DOOR;
    Dungeon[0][x][y].p_locf = L_RESIDENCE;
    if(random_range(5)) Dungeon[0][x][y].aux = LOCKED;
  }
  else switch(permutation[next]) {
  case 0:
    Dungeon[0][x][y].locchar = OPEN_DOOR;
    Dungeon[0][x][y].p_locf = L_ARMORER;
    break;
  case 1:
    Dungeon[0][x][y].locchar = OPEN_DOOR;
    Dungeon[0][x][y].p_locf = L_CLUB;
    break;
  case 2:
    Dungeon[0][x][y].locchar = OPEN_DOOR;
    Dungeon[0][x][y].p_locf = L_GYM;
    break;
  case 3:
    Dungeon[0][x][y].locchar = CLOSED_DOOR;
    Dungeon[0][x][y].p_locf = L_THIEVES_GUILD;
    Dungeon[0][x][y].aux = LOCKED;
    break;
  case 4:
    Dungeon[0][x][y].locchar = OPEN_DOOR;
    Dungeon[0][x][y].p_locf = L_HEALER;
    break;
  case 5:
    Dungeon[0][x][y].locchar = OPEN_DOOR;
    Dungeon[0][x][y].p_locf = L_CASINO;
    break;
  case 6: case 7: case 8: case 9: case 20: case 21:
    Dungeon[0][x][y].locchar = OPEN_DOOR;
    Dungeon[0][x][y].p_locf = L_TAVERN;
    break;
  case 10:
    Dungeon[0][x][y].locchar = OPEN_DOOR;
    Dungeon[0][x][y].p_locf = L_ALCHEMIST;
    break;
  case 11:
    Dungeon[0][x][y].locchar = OPEN_DOOR;
    Dungeon[0][x][y].p_locf = L_DPW;
    break;
  case 12:
    Dungeon[0][x][y].locchar = OPEN_DOOR;
    Dungeon[0][x][y].p_locf = L_LIBRARY;
    break;
  case 13:
    Dungeon[0][x][y].locchar = OPEN_DOOR;
    Dungeon[0][x][y].p_locf = L_PAWN_SHOP;
    break;
  case 14:
    Dungeon[0][x][y].locchar = OPEN_DOOR;
    Dungeon[0][x][y].p_locf = L_CONDO;
    break;
  case 15:
    Dungeon[0][x][y].locchar = CLOSED_DOOR;
    Dungeon[0][x][y].p_locf = L_BROTHEL;
    break;
  default:
    Dungeon[0][x][y].locchar = CLOSED_DOOR;
    Dungeon[0][x][y].p_locf = L_RESIDENCE;
    if(random_range(5)) Dungeon[0][x][y].aux = LOCKED;
    break;
  }
  next++;
}

  

void mysterycitysite(i,j)
int i,j;
{
  switch(random_range(7)) {
  case 0: case 1:
    Dungeon[0][i][j].locchar = FLOOR;
    Dungeon[0][i][j].p_locf = TRAP_BASE+random_range(NUMTRAPS);
    break;
  case 2: case 3:
    Dungeon[0][i][j].locchar = FLOOR;
    makecitymonster(i,j);
    break;
  case 4: case 5:
    Dungeon[0][i][j].locchar = FLOOR;
    makecitytreasure(i,j);
    break;
  default:
    Dungeon[0][i][j].locchar = FLOOR;    
  }
}

