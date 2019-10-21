/* omega copyright (C) by Laurence Raphael Brothers, 1987 */
/* odepths.c */
/* some functions to make the final level */

#include <stdio.h>
#include <strings.h>
#include "oglob.h"

/* from olev */
extern pmt make_creature();
extern void make_thing();

/* from oaux */
extern void drop_at(),tacmode();

/* from ofile */
extern FILE *checkfopen();

/* from outil */
extern int random_range();

/* from oscr */
extern void mprint(),drawomega(),morewait();

/* from omon */
extern void m_status_set();

/* from ocom */
extern void levelchange();

/* odepths functions */
void load_depths(),make_double(),final_duel(),makedepthsmonster();
void assign_depths_site();


/* loads the final level */
void load_depths()
{
  int i,j;
  char site,digit;
  pol item;
  
  FILE *fd;
  
  strcpy(Str1,OMEGALIB);
  strcat(Str1,"odepths.dat");
  fd = checkfopen(Str1,"r");
  Mlist[NUMLEVELS-1] = NULL;

  for(j=0;j<LENGTH;j++) {
    for(i=0;i<WIDTH;i++) {
      Dungeon[NUMLEVELS-1][i][j].roomnumber = RS_ADEPT;
      Dungeon[NUMLEVELS-1][i][j].things = NULL;
      Dungeon[NUMLEVELS-1][i][j].creature = NULL;
      Dungeon[NUMLEVELS-1][i][j].stopsrun = FALSE;
      site = getc(fd);
      if (site == 'S') {
	Dungeon[NUMLEVELS-1][i][j].locchar = FLOOR;
	Dungeon[NUMLEVELS-1][i][j].secret = TRUE;
      }
      else if (site == '^') {
	Dungeon[NUMLEVELS-1][i][j].locchar = FLOOR;
	Dungeon[NUMLEVELS-1][i][j].p_locf = TRAP_BASE+random_range(NUMTRAPS);
      }
      else if (site == HEDGE) {
	Dungeon[NUMLEVELS-1][i][j].locchar = HEDGE;
	Dungeon[NUMLEVELS-1][i][j].p_locf = L_TRIFID;
      }
      else if (site == WATER) {
	Dungeon[NUMLEVELS-1][i][j].locchar = WATER;
	Dungeon[NUMLEVELS-1][i][j].p_locf = L_WATER;
      }
      else if (site == FIRE) {
	Dungeon[NUMLEVELS-1][i][j].locchar = FIRE;
	Dungeon[NUMLEVELS-1][i][j].p_locf = L_FIRE;
      }
      else if (site == WHIRLWIND) {
	Dungeon[NUMLEVELS-1][i][j].locchar = WHIRLWIND;
	Dungeon[NUMLEVELS-1][i][j].p_locf = L_WHIRLWIND;
      }
      else if (site == LAVA) {
	Dungeon[NUMLEVELS-1][i][j].locchar = LAVA;
	Dungeon[NUMLEVELS-1][i][j].p_locf = L_LAVA;
      }
      else if (site == ABYSS) {
	Dungeon[NUMLEVELS-1][i][j].locchar = ABYSS;
	Dungeon[NUMLEVELS-1][i][j].p_locf = L_FINAL_ABYSS;
      }
      else if (site == 'Y') {
	Dungeon[NUMLEVELS-1][i][j].locchar = FLOOR;
	Dungeon[NUMLEVELS-1][i][j].p_locf = L_PORTCULLIS;
      }	  
      else if (site == 'X') {
	Dungeon[NUMLEVELS-1][i][j].locchar = FLOOR;
	Dungeon[NUMLEVELS-1][i][j].p_locf = L_PORTCULLIS_TRAP;
      }	  
      else if (site == 'M') {
	Dungeon[NUMLEVELS-1][i][j].locchar = FLOOR;
	Dungeon[NUMLEVELS-1][i][j].p_locf = L_WARNING;
      }
      else if (site == CLOSED_DOOR) {
	Dungeon[NUMLEVELS-1][i][j].locchar = CLOSED_DOOR;
	Dungeon[NUMLEVELS-1][i][j].aux = LOCKED;
	Dungeon[NUMLEVELS-1][i][j].p_locf = L_STATUE_WAKE;
      }
      else if (site == 'K') {
	Dungeon[NUMLEVELS-1][i][j].locchar = FLOOR;
	item = (pol) malloc(sizeof(oltype));
	item->thing = (pob) malloc(sizeof(objtype));
	item->next = NULL;
	make_thing(item->thing,7); /* key */
	Dungeon[NUMLEVELS-1][i][j].things = item;
      }
      else if (site == 'B') {
	Dungeon[NUMLEVELS-1][i][j].locchar = FLOOR;
	item = (pol) malloc(sizeof(oltype));
	item->thing = (pob) malloc(sizeof(objtype));
	item->next = NULL;
	make_thing(item->thing,6); /* salt water */
	Dungeon[NUMLEVELS-1][i][j].things = item;
      }
      else if (site == 'A') {
	Dungeon[NUMLEVELS-1][i][j].locchar = FLOOR;
	makedepthsmonster(i,j,ML10+6);
      }
      else if (site == 'E') {
	Dungeon[NUMLEVELS-1][i][j].locchar = FLOOR;
	makedepthsmonster(i,j,ML10+5);
      }
      else if (site == 'W') {
	Dungeon[NUMLEVELS-1][i][j].locchar = FLOOR;
	makedepthsmonster(i,j,ML10+7);
      }
      else if (site == 'F') {
	Dungeon[NUMLEVELS-1][i][j].locchar = FLOOR;
	makedepthsmonster(i,j,ML10+8);
      }
      else if (site == 'I') {
	Dungeon[NUMLEVELS-1][i][j].locchar = FLOOR;
	makedepthsmonster(i,j,ML0+2);
      }
      else if (site == 'o') {
	Dungeon[NUMLEVELS-1][i][j].locchar = PORTAL;
	Dungeon[NUMLEVELS-1][i][j].p_locf = L_ADEPT;
      }
      else if (site == 'G') {
	Dungeon[NUMLEVELS-1][i][j].locchar = UP;
	Dungeon[NUMLEVELS-1][i][j].p_locf = L_CHALLENGE;
      }
      else if (site == 'D') 
	assign_depths_site(i,j);
      else if (site == UP) {
	Dungeon[NUMLEVELS-1][i][j].locchar = UP;
	Dungeon[NUMLEVELS-1][i][j].aux = NUMLEVELS-2;
      }
      else if (site == WALL) {
	Dungeon[NUMLEVELS-1][i][j].locchar = WALL;
	Dungeon[NUMLEVELS-1][i][j].aux = 500;
      }
      else Dungeon[NUMLEVELS-1][i][j].locchar = site;
    }
    getc(fd);
  }
  Player.x = 31;
  Player.y = 3;
}


void final_duel()
{
  struct monster *m;
  m = (pmt) (calloc(1,sizeof(montype)));
  morewait();
  mprint("You alight on a glowing blue disk floating in an endless void.");
  mprint("Inscribed in the disk is a giant omega.");
  mprint("On the far side of the disk, you find another adventurer.");
  mprint("The stranger looks familiar.... Just like you!");
  mprint("Your doppelganger moves to the attack.");
  mprint("After a moment of ego-imbalance, you move to defend yourself.");
  morewait();
  drawomega();
  make_double(m);
  if (Player.patron == DESTINY) {
    mprint("As you cross the image of the omega, you suddenly feel stronger.");
    mprint("You imagine the Lords of Destiny radiating approval....");
    Player.str += Player.rank[PRIESTHOOD]*2;
    Player.dex += Player.rank[PRIESTHOOD]*2;
    Player.agi += Player.rank[PRIESTHOOD]*2;
    Player.hp += Player.rank[PRIESTHOOD]*20;
    Player.mana += Player.rank[PRIESTHOOD]*20;
  }
  Final = TRUE;
  tacmode(m);
  Final = FALSE;
  if (Player.hp > 0) {
    mprint("You feel infused with power....");
    mprint("You feel like you can completely control your environment.");
    mprint("[ie, try hitting control-x]");
    Player.rank[ADEPT]=TRUE;
    mprint("Your vision blurs....");
    levelchange(0,ABYSS);
  }
}


void make_double(m)
struct monster *m;
{
  int i,numspells=0;
  strcpy(m->monstring,Player.name);
  strcpy(m->corpsestr,"Your corpse");
  m->id = 0;
  m->hit = Player.hit;
  m->sense = Player.vision;
  m->sleep = 0;
  m->treasure = 0;
  m->corpseweight = 1800;
  m->attacked = 1;
  m->uniqueness = 2;
  m->monchar = PLAYER;
  m->hitloc = 0;
  m->possessions = 0;
  m->hp = Player.hp;
  m->ac = (Player.defense+Player.absorption)/2;
  m->actions = 2+Player.level/10;
  if (Player.rank[ARENA]) m->actions++;
  m->tactics = MM_GOOD;
  m->dmg = Player.dmg;
  for (i=0;i<NUMSPELLS;i++)
    if (Spells[i].known) numspells++;
  m->xpv = Player.xp;
  m->level = min(10,numspells/3);
  m->talkf = M_TALK_SILENT;
  m->movef = M_MOVE_SMART;
  m->meleef = M_MELEE_NORMAL;
  m->strikef = M_NO_OP;
  m->specialf = M_SP_SPELL;
}


void makedepthsmonster(i,j,id)
int i,j,id;
{
  pml tml = ((pml) (calloc(1,sizeof(mltype))));
  Dungeon[NUMLEVELS-1][i][j].creature = make_creature(id);
  tml->m = Dungeon[NUMLEVELS-1][i][j].creature;
  tml->m->x = i;
  tml->m->y = j;
  tml->next = Mlist[NUMLEVELS-1];
  Mlist[NUMLEVELS-1] = tml;
  if (Monsters[id].uniqueness == UNIQUE_UNMADE)
    Monsters[id].uniqueness = UNIQUE_MADE;
}

void assign_depths_site(x,y)
int x,y;
{
  static int setup=0;
  static int next=0;
  static int permutation[6];
  pol item;

  int i,j,k,l;

  if (setup == 0) {
    setup = 1;
    for(i=0;i<6;i++)
      permutation[i] = i;
    for(i=0;i<100;i++) {
      j = random_range(6);
      k = random_range(6);
      l = permutation[j];
      permutation[j] = permutation[k];
      permutation[k] = l;
    }
  }

  Dungeon[NUMLEVELS-1][x][y].locchar = FLOOR;
  item = (pol) malloc(sizeof(oltype));
  item->thing = (pob) malloc(sizeof(objtype));
  item->next = NULL;
  make_thing(item->thing,7); /* key */
  Dungeon[NUMLEVELS-1][x][y].things = item;

  if (next > 5) { /* in case someone changes the no. of D's */
    Dungeon[NUMLEVELS-1][x][y].locchar = ABYSS;
    Dungeon[NUMLEVELS-1][x][y].p_locf = L_ABYSS;
  }
  else {
    Dungeon[NUMLEVELS-1][x][y].locchar = FLOOR;
    Dungeon[NUMLEVELS-1][x][y].p_locf = L_GUARDIAN;
    switch(permutation[next]) {
    case 0:
      if (Monsters[ML10+0].uniqueness == UNIQUE_UNMADE) {
	makedepthsmonster(x,y,ML10+0);
	if (Player.rank[THIEVES]) 
	  m_status_set(Dungeon[NUMLEVELS-1][x][y].creature,HOSTILE);
      }
      break;
    case 1:
      if (Monsters[ML10+1].uniqueness == UNIQUE_UNMADE) {
	makedepthsmonster(x,y,ML10+1);
	if (Player.rank[COLLEGE]) 
	  m_status_set(Dungeon[NUMLEVELS-1][x][y].creature,HOSTILE);
      }
      break;
    case 2:
      if (Monsters[ML10+2].uniqueness == UNIQUE_UNMADE) {
	makedepthsmonster(x,y,ML10+2);
	if (Player.rank[CIRCLE]) 
	  m_status_set(Dungeon[NUMLEVELS-1][x][y].creature,HOSTILE);
      }
      break;
    case 3:
      if (Monsters[ML10+4].uniqueness == UNIQUE_UNMADE) {
	makedepthsmonster(x,y,ML10+4);
	if (Player.rank[LEGION]) 
	  m_status_set(Dungeon[NUMLEVELS-1][x][y].creature,HOSTILE);
      }
      break;
    default:
      makedepthsmonster(x,y,ML9+random_range(NML_9));
      m_status_set(Dungeon[NUMLEVELS-1][x][y].creature,HOSTILE);
      break;
    }
    next++;
  }
}

