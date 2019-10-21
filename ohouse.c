/* omega copyright (C) by Laurence Raphael Brothers, 1987,1988 */
/* ovillage.c */
/* some functions to make the house levels */

#include "oglob.h"


/* loads the house level into Level*/
void load_house(kind)
int kind;
{
  int i,j;
  char site;
  
  FILE *fd;

  TempLevel = Level;
  if (ok_to_free(TempLevel)) {
    free((char *) TempLevel);
    TempLevel = NULL;
  }
  Level = ((plv) malloc(sizeof(levtype)));
  clear_level(Level);
  strcpy(Str3,OMEGALIB);
  switch(kind) {
  case E_HOUSE:
    strcat(Str3,"ohome1.dat");
    Level->environment = E_HOUSE;
    break;
  case E_MANSION:
    strcat(Str3,"ohome2.dat");
    Level->environment = E_MANSION;
    break;
  default:
  case E_HOVEL:
    strcat(Str3,"ohome3.dat");
    Level->environment = E_HOVEL;
    break;
  }
  fd = fopen(Str3,"r");
  for(j=0;j<LENGTH;j++) {
    for(i=0;i<WIDTH;i++) {
      if (kind == E_HOVEL) Level->site[i][j].lstatus = SEEN;
      else Level->site[i][j].lstatus = 0;
      Level->site[i][j].roomnumber = RS_CORRIDOR;
      Level->site[i][j].p_locf = L_NO_OP;
      site = getc(fd);
      switch(site) {
      case 'N':
	Level->site[i][j].locchar = FLOOR;
	Level->site[i][j].roomnumber = RS_BEDROOM;
	if (random_range(2)) make_house_npc(i,j);
	break;
      case 'H':
	Level->site[i][j].locchar = FLOOR;
	Level->site[i][j].roomnumber = RS_BEDROOM;
	if (random_range(2)) make_mansion_npc(i,j);
	break;
      case 'D':
	Level->site[i][j].locchar = FLOOR;
	Level->site[i][j].roomnumber = RS_DININGROOM;
	break;
      case '.':
	Level->site[i][j].locchar = FLOOR;
	break;
      case 'c':
	Level->site[i][j].locchar = FLOOR;
	Level->site[i][j].roomnumber = RS_CLOSET;
	break;
      case 'G':
	Level->site[i][j].locchar = FLOOR;
	Level->site[i][j].roomnumber = RS_BATHROOM;
	break;
      case 'B':
	Level->site[i][j].locchar = FLOOR;
	Level->site[i][j].roomnumber = RS_BEDROOM;
	break;
      case 'K':
	Level->site[i][j].locchar = FLOOR;
	Level->site[i][j].roomnumber = RS_KITCHEN;
	break;
      case 'S':
	Level->site[i][j].locchar = FLOOR;
	Level->site[i][j].showchar = WALL;
	lset(i,j,SECRET);
	Level->site[i][j].roomnumber = RS_SECRETPASSAGE;
	break;
      case '3':
	Level->site[i][j].locchar = SAFE;
	Level->site[i][j].showchar = WALL;
	lset(i,j,SECRET);
	Level->site[i][j].p_locf = L_SAFE;
	break;
      case '^':
	Level->site[i][j].locchar = FLOOR;
	Level->site[i][j].p_locf = TRAP_BASE+random_range(NUMTRAPS);
	break;
      case 'P':
	Level->site[i][j].locchar = PORTCULLIS;
	Level->site[i][j].p_locf = L_PORTCULLIS;
	break;
      case 'R':
	Level->site[i][j].locchar = FLOOR;
	Level->site[i][j].p_locf = L_RAISE_PORTCULLIS;
	break;
      case 'p':
	Level->site[i][j].locchar = FLOOR;
	Level->site[i][j].p_locf = L_PORTCULLIS;
	break;
      case 'T':
	Level->site[i][j].locchar = FLOOR;
	Level->site[i][j].p_locf = L_PORTCULLIS_TRAP;
	break;
      case 'X':
	Level->site[i][j].locchar = FLOOR;
	Level->site[i][j].p_locf = L_HOUSE_EXIT;
	break;
      case '#':
	Level->site[i][j].locchar = WALL;
	switch (kind) {
	case E_HOVEL: Level->site[i][j].aux = 10; break;
	case E_HOUSE: Level->site[i][j].aux = 50; break;
	case E_MANSION: Level->site[i][j].aux = 150; break;
	}
	break;
      case '|':
	Level->site[i][j].locchar = OPEN_DOOR;
	Level->site[i][j].roomnumber = RS_CORRIDOR;
	lset(i,j,STOPS);
	break;
      case '+':
	Level->site[i][j].locchar = CLOSED_DOOR;
	Level->site[i][j].roomnumber = RS_CORRIDOR;
	Level->site[i][j].aux = LOCKED;
	lset(i,j,STOPS);
	break;
      case 'd':
	Level->site[i][j].locchar = FLOOR;
	Level->site[i][j].roomnumber = RS_CORRIDOR;
	make_site_monster(i,j,ML4+10); /* dog */
	break;
      case 'a':
	Level->site[i][j].locchar = FLOOR;
	Level->site[i][j].roomnumber = RS_CORRIDOR;
	Level->site[i][j].p_locf = L_TRAP_SIREN;
	break;
      case 'A':
	Level->site[i][j].locchar = FLOOR;
	Level->site[i][j].roomnumber = RS_CORRIDOR;
	make_site_monster(i,j,ML4+2); /* automaton */
	break;
      }
      Level->site[i][j].showchar = ' ';
    }
    fscanf(fd,"\n");
  }
}


  
/* makes a log npc for houses and hovels */
void make_house_npc(i,j)
int i,j;
{
  pml ml = ((pml) malloc(sizeof(mltype)));
  pob ob;
  ml->m = ((pmt) malloc(sizeof(montype)));
  *(ml->m) = Monsters[NPC];
  make_log_npc(ml->m);
  if (ml->m->id == NPC) mprint("You detect signs of life in this house.");
  else mprint("An eerie shiver runs down your spine as you enter....");
  /* if not == NPC, then we got a ghost off the npc list */
  ml->m->x = i;
  ml->m->y = j;
  Level->site[i][j].creature = ml->m;
  ml->m->click = (Tick + 1) % 50;
  ml->next = Level->mlist;
  Level->mlist = ml;
  m_status_set(ml->m,HOSTILE);
  if (nighttime())
    m_status_reset(ml->m,AWAKE);
  else m_status_set(ml->m,AWAKE);
  if (ml->m->startthing > -1) {
    ob = ((pob) malloc(sizeof(objtype)));    
    *ob = Objects[ml->m->startthing];
    m_pickup(ml->m,ob);
  }
}





/* makes a hiscore npc for mansions */
void make_mansion_npc(i,j)
int i,j;
{
  pml ml = ((pml) malloc(sizeof(mltype)));
  ml->m = ((pmt) malloc(sizeof(montype)));
  *(ml->m) = Monsters[NPC];
  make_hiscore_npc(ml->m,random_range(14)+1);
  mprint("You detect signs of life in this house.");
  ml->m->x = i;
  ml->m->y = j;
  Level->site[i][j].creature = ml->m;
  ml->m->click = (Tick + 1) % 50;
  ml->next = Level->mlist;
  Level->mlist = ml;
  m_status_set(ml->m,HOSTILE);
  if (nighttime())
    m_status_reset(ml->m,AWAKE);
  else m_status_set(ml->m,AWAKE);
}


