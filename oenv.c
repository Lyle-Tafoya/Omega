/* omega copyright (c) 1987,1988 by Laurence Raphael Brothers */
/* oenv.c */
/* some load_* routines for special environments */

#include "oglob.h"

/* loads the arena level into Level*/
void load_arena()
{
  int i,j;
  char site;
  pob box=((pob)malloc(sizeof(objtype)));
  FILE *fd;

  *box = Objects[THINGID+0];

  TempLevel = Level;
  if (ok_to_free(TempLevel)) {
    free((char *) TempLevel);
    TempLevel = NULL;
  }
  Level = ((plv) malloc(sizeof(levtype)));
  clear_level(Level);
  Level->environment = E_ARENA;
  strcpy(Str3,OMEGALIB);
  strcat(Str3,"oarena.dat");
  fd = fopen(Str3,"r");
  for(j=0;j<LENGTH;j++) {
    for(i=0;i<WIDTH;i++) {
      Level->site[i][j].lstatus = SEEN+LIT;
      Level->site[i][j].roomnumber = RS_ARENA;
      site = getc(fd);
      switch(site) {
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
      case '7':
	Level->site[i][j].locchar = PORTCULLIS;
	Level->site[i][j].p_locf = L_PORTCULLIS;
	break;
      case 'T':
	Level->site[i][j].locchar = FLOOR;
	Level->site[i][j].p_locf = L_DROP_EVERY_PORTCULLIS;
	break;
      case 'X':
	Level->site[i][j].locchar = FLOOR;
	Level->site[i][j].p_locf = L_ARENA_EXIT;
	break;
      default:
	Level->site[i][j].locchar = site;
	Level->site[i][j].p_locf = L_NO_OP;
	break;
      }
      Level->site[i][j].showchar = Level->site[i][j].locchar;
    }
    fscanf(fd,"\n");

  }
  Level->site[60][7].creature = Arena_Monster;
  Arena_Monster->x = 60;
  Arena_Monster->y = 7;
  Arena_Monster->sense = 50;
  m_pickup(Arena_Monster,box);
  m_status_set(Arena_Monster,AWAKE);
  Level->mlist = (pml) malloc(sizeof(mltype));
  Level->mlist->m = Arena_Monster;
  Level->mlist->next = NULL;
  /* hehehehe cackled the dungeon master.... */
  mprint("Your opponent holds the only way you can leave!");
  Arena_Monster->hp += Arena_Monster->level*10;
  Arena_Monster->hit += Arena_Monster->hit;
  Arena_Monster->dmg += Arena_Monster->dmg/2;
}

/* loads the sorcereror's circle into Level*/
void load_circle()
{
  int i,j;
  int safe = (Player.rank[CIRCLE] > 0);
  char site;
  FILE *fd;

  TempLevel = Level;
  if (ok_to_free(TempLevel)) {
    free((char *) TempLevel);
    TempLevel = NULL;
  }
  Level = ((plv) malloc(sizeof(levtype)));
  clear_level(Level);
  Level->environment = E_CIRCLE;
  strcpy(Str3,OMEGALIB);
  strcat(Str3,"ocircle.dat");
  fd = fopen(Str3,"r");
  for(j=0;j<LENGTH;j++) {
    for(i=0;i<WIDTH;i++) {
      Level->site[i][j].lstatus = 0;
      Level->site[i][j].roomnumber = RS_CIRCLE;
      Level->site[i][j].p_locf = L_NO_OP;
      site = getc(fd);
      switch(site) {
      case 'P':
	Level->site[i][j].locchar = FLOOR;
	make_prime(i,j); /* prime sorceror */
	Level->site[i][j].creature->specialf = M_SP_PRIME;
	if (! safe) m_status_set(Level->site[i][j].creature,HOSTILE);
	break;
      case 'D':
	Level->site[i][j].locchar = FLOOR;
	make_site_monster(i,j,ML9+7); /* prime circle demon */
	if (safe) m_status_reset(Level->site[i][j].creature,HOSTILE);
	Level->site[i][j].creature->specialf = M_SP_PRIME;
	break;
      case 's':
	Level->site[i][j].locchar = FLOOR;
	make_site_monster(i,j,ML4+13); /* servant of chaos */
	Level->site[i][j].creature->specialf = M_SP_COURT;
	if (safe) m_status_reset(Level->site[i][j].creature,HOSTILE);
	break;
      case 'e':
	Level->site[i][j].locchar = FLOOR;
	make_site_monster(i,j,ML2+7); /* enchanter */
	Level->site[i][j].creature->specialf = M_SP_COURT;
	if (safe) m_status_reset(Level->site[i][j].creature,HOSTILE);
	break;
      case 'n':
	Level->site[i][j].locchar = FLOOR;
	make_site_monster(i,j,ML5+6); /* necromancer */
	Level->site[i][j].creature->specialf = M_SP_COURT;
	if (safe) m_status_reset(Level->site[i][j].creature,HOSTILE);
	break;
      case 'T':
	Level->site[i][j].locchar = FLOOR;
	make_site_monster(i,j,ML9+4); /* High Thaumaturgist */
	Level->site[i][j].creature->specialf = M_SP_COURT;
	if (safe) m_status_reset(Level->site[i][j].creature,HOSTILE);
	break;
      case '#':
	Level->site[i][j].locchar = WALL;
	Level->site[i][j].aux = 1000; 
	break;
      case 'L':
	Level->site[i][j].locchar = FLOOR;
	Level->site[i][j].p_locf = L_CIRCLE_LIBRARY;
	break;
      case '?':
	Level->site[i][j].locchar = FLOOR;
	Level->site[i][j].p_locf = L_TOME1;
	break;
      case '!':
	Level->site[i][j].locchar = FLOOR;
	Level->site[i][j].p_locf = L_TOME2;
	break;
      case 'S':
	Level->site[i][j].locchar = FLOOR;
	lset(i,j,SECRET);
	break;
      case '.':
	Level->site[i][j].locchar = FLOOR;
	break;
      case '-':
	Level->site[i][j].locchar = CLOSED_DOOR;
	break;
      }
    }
    fscanf(fd,"\n");
  }
  fclose(fd);
}

/* make the prime sorceror */
void make_prime(i,j)
int i,j;
{
  pml ml = ((pml) malloc(sizeof(mltype)));
  pmt m = ((pmt) malloc(sizeof(montype)));
  pol ol;
  pob o;
  make_hiscore_npc(m,10); /* 10 is index for prime */
  m->x = i;
  m->y = j;
  Level->site[i][j].creature = m;
  ml->m = m;
  ml->next = Level->mlist;
  Level->mlist = ml;
  
  if (! gamestatusp(COMPLETED_ASTRAL)) {
    ol  = ((pol) malloc(sizeof(oltype)));
    o  = ((pob) malloc(sizeof(objtype)));
    *o = Objects[ARTIFACTID+21];
    ol->thing = o;
    ol->next = NULL;
    m->possessions = ol;
  }

}






/* loads the court of the archmage into Level*/
void load_court()
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
  Level->environment = E_COURT;
  strcpy(Str3,OMEGALIB);
  strcat(Str3,"ocourt.dat");
  fd = fopen(Str3,"r");
  for(j=0;j<LENGTH;j++) {
    for(i=0;i<WIDTH;i++) {
      Level->site[i][j].lstatus = 0;
      Level->site[i][j].roomnumber = RS_COURT;
      Level->site[i][j].p_locf = L_NO_OP;
      site = getc(fd);
      switch(site) {
      case '5':
	Level->site[i][j].locchar = CHAIR;
	Level->site[i][j].p_locf = L_THRONE;
	make_specific_treasure(i,j,ARTIFACTID+22);
	make_archmage(i,j);
	m_status_reset(Level->site[i][j].creature,HOSTILE);
	m_status_reset(Level->site[i][j].creature,MOBILE);
	break;
      case 'e':
	Level->site[i][j].locchar = FLOOR;
	make_site_monster(i,j,ML2+7); /* enchanter */
	m_status_reset(Level->site[i][j].creature,HOSTILE);
	Level->site[i][j].creature->specialf = M_SP_COURT;
	break;
      case 'n':
	Level->site[i][j].locchar = FLOOR;
	make_site_monster(i,j,ML5+6); /* necromancer */
	m_status_reset(Level->site[i][j].creature,HOSTILE);
	Level->site[i][j].creature->specialf = M_SP_COURT;
	break;
      case 'T':
	Level->site[i][j].locchar = FLOOR;
	make_site_monster(i,j,ML9+4); /* High Thaumaturgist */
	m_status_reset(Level->site[i][j].creature,HOSTILE);
	Level->site[i][j].creature->specialf = M_SP_COURT;
	break;
      case '#':
	Level->site[i][j].locchar = WALL;
	Level->site[i][j].aux = 1000; 
	break;
      case 'G':
	Level->site[i][j].locchar = FLOOR;
	Level->site[i][j].locchar = FLOOR;
	make_site_monster(i,j,ML0+3); /* guard */
	m_status_reset(Level->site[i][j].creature,HOSTILE);
	break;
      case '<':
	Level->site[i][j].locchar = UP;
	Level->site[i][j].p_locf = L_ESCALATOR;
	break;
      case '.':
	Level->site[i][j].locchar = FLOOR;
	break;
      }
    }
    fscanf(fd,"\n");
  }
  fclose(fd);
}



/* make the archmage */
void make_archmage(i,j)
int i,j;
{
  pml ml = ((pml) malloc(sizeof(mltype)));
  pmt m = ((pmt) malloc(sizeof(montype)));
  make_hiscore_npc(m,9); /* 10 is index for archmage */
  m->x = i;
  m->y = j;
  Level->site[i][j].creature = m;
  ml->m = m;
  ml->next = Level->mlist;
  Level->mlist = ml;
  m->specialf = M_SP_COURT;
}

