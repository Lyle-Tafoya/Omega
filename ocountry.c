/* omega copyright (C) by Laurence Raphael Brothers, 1987,1988 */
/* ocountry.c */
/* load_country(), and all load_ functions for places which are
/* accessible from the country and don't have their own files */

#include "oglob.h"

/* loads the countryside level from the data file */
void load_country()
{
  int i,j;
  char site;
  
  FILE *fd;
  
  strcpy(Str3,OMEGALIB);
  strcat(Str3,"ocountry.dat");
  fd = fopen(Str3,"r");

  for(j=0;j<LENGTH;j++) {
    for(i=0;i<WIDTH;i++) {
      site = getc(fd);
      Country[i][j].base_terrain_type = site;
      Country[i][j].aux = 0;
      if ((site == PASS) ||
	  (site == CASTLE) ||
	  (site == STARPEAK) ||
	  (site == CAVES) ||
	  (site == VOLCANO)) Country[i][j].current_terrain_type = MOUNTAINS;
      else if (site == DRAGONLAIR) Country[i][j].current_terrain_type = DESERT;
      else if (site == MAGIC_ISLE) 
	Country[i][j].current_terrain_type = CHAOS_SEA;
      else if ((site >= 'a') && (site <= 'f')) {
	Country[i][j].current_terrain_type =
	  Country[i][j].base_terrain_type = 'o';
	Country[i][j].aux = 1+site-'a';
      }
      else if ((site >= '1') && (site <= '6')) {
	Country[i][j].current_terrain_type =
	  Country[i][j].base_terrain_type = TEMPLE;
	Country[i][j].aux = site-'0';
      }
      else Country[i][j].current_terrain_type = site;
      Country[i][j].explored = FALSE;
    }
    fscanf(fd,"\n");
  }
  fclose(fd);
}







/* loads the dragon's lair into Level*/
void load_dlair(empty)
int empty;
{
  int i,j;
  char site;
  
  FILE *fd;

  if (empty) mprint("The Lair is now devoid of inhabitants and treasure.");

  TempLevel = Level;
  if (ok_to_free(TempLevel)) {
    free((char *) TempLevel);
    TempLevel = NULL;
  }
  Level = ((plv) malloc(sizeof(levtype)));
  clear_level(Level);
  Level->environment = E_DLAIR;
  strcpy(Str3,OMEGALIB);
  strcat(Str3,"odlair.dat");
  fd = fopen(Str3,"r");
  for(j=0;j<LENGTH;j++) {
    for(i=0;i<WIDTH;i++) {
      Level->site[i][j].lstatus = 0;
      Level->site[i][j].roomnumber = RS_CAVERN;
      Level->site[i][j].p_locf = L_NO_OP;
      site = getc(fd);
      switch(site) {
      case 'D':
	Level->site[i][j].locchar = FLOOR;
	if (! empty)
	  make_site_monster(i,j,ML10+3); /* dlord */
	Level->site[i][j].creature->specialf = M_SP_LAIR;
	break;
      case 'd':
	Level->site[i][j].locchar = FLOOR;
	if (! empty)
	  make_site_monster(i,j,ML8+3); /* elite dragons */
	Level->site[i][j].creature->specialf = M_SP_LAIR;
	Level->site[i][j].creature->hit *= 2;
	Level->site[i][j].creature->dmg *= 2;
	break;
      case 'W':
	Level->site[i][j].locchar = FLOOR;
	if (! empty)
	  make_site_monster(i,j,ML9+2);
	break;
      case 'M':
	Level->site[i][j].locchar = FLOOR;
	if (! empty)
	  make_site_monster(i,j,-1);
	break;
      case 'S':
	Level->site[i][j].locchar = FLOOR;
	Level->site[i][j].showchar = WALL;
	if (! empty)
	  lset(i,j,SECRET);
	Level->site[i][j].roomnumber = RS_SECRETPASSAGE;
	break;
      case '$':
	Level->site[i][j].locchar = FLOOR;
	Level->site[i][j].roomnumber = RS_DRAGONLORD;
	if (! empty)
	  make_site_treasure(i,j,10);
	break;
      case 's':
	Level->site[i][j].locchar = FLOOR;
	Level->site[i][j].p_locf = L_TRAP_SIREN;
	break;
      case '7':
	if (! empty)
	  Level->site[i][j].locchar = PORTCULLIS;
	else Level->site[i][j].locchar = FLOOR;
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
	if (! empty)
	  Level->site[i][j].p_locf = L_PORTCULLIS_TRAP;
	break;
      case 'X':
	Level->site[i][j].locchar = FLOOR;
	Level->site[i][j].p_locf = L_TACTICAL_EXIT;
	break;
      case '#':
	Level->site[i][j].locchar = WALL;
	Level->site[i][j].aux = 150; 
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





/* loads the star peak into Level*/
void load_speak(empty)
int empty;
{
  int i,j,safe = Player.alignment > 0;
  char site;
  
  FILE *fd;

  if (empty) mprint("The peak is now devoid of inhabitants and treasure.");

  TempLevel = Level;
  if (ok_to_free(TempLevel)) {
    free((char *) TempLevel);
    TempLevel = NULL;
  }
  Level = ((plv) malloc(sizeof(levtype)));
  clear_level(Level);
  Level->environment = E_STARPEAK;
  strcpy(Str3,OMEGALIB);
  strcat(Str3,"ospeak.dat");
  fd = fopen(Str3,"r");
  for(j=0;j<LENGTH;j++) {
    for(i=0;i<WIDTH;i++) {
      Level->site[i][j].lstatus = 0;
      Level->site[i][j].roomnumber = RS_STARPEAK;
      Level->site[i][j].p_locf = L_NO_OP;
      site = getc(fd);
      switch(site) {
      case 'S':
	Level->site[i][j].locchar = FLOOR;
	Level->site[i][j].showchar = WALL;
	lset(i,j,SECRET);
	Level->site[i][j].roomnumber = RS_SECRETPASSAGE;
	break;
      case 'L':
	Level->site[i][j].locchar = FLOOR;
	if (! empty)
	  make_site_monster(i,j,ML10+2); /* lawbringer */
	if (safe) m_status_reset(Level->site[i][j].creature,HOSTILE);
	break;
      case 's':
	Level->site[i][j].locchar = FLOOR;
	if (! empty)
	  make_site_monster(i,j,ML4+12); /* servant of law */
	if (safe) m_status_reset(Level->site[i][j].creature,HOSTILE);
	break;
      case 'M':
	Level->site[i][j].locchar = FLOOR;
	if (! empty)
	  make_site_monster(i,j,-1);
	if (safe) m_status_reset(Level->site[i][j].creature,HOSTILE);
	break;
      case '$':
	Level->site[i][j].locchar = FLOOR;
	if (! empty)
	  make_site_treasure(i,j,10);
	break;
      case '7':
	if (! empty)
	  Level->site[i][j].locchar = PORTCULLIS;
	else Level->site[i][j].locchar = FLOOR;
	Level->site[i][j].p_locf = L_PORTCULLIS;
	break;
      case 'R':
	Level->site[i][j].locchar = FLOOR;
	Level->site[i][j].p_locf = L_RAISE_PORTCULLIS;
	break;
      case '-':
	Level->site[i][j].locchar = CLOSED_DOOR;
	break;
      case '|':
	Level->site[i][j].locchar = OPEN_DOOR;
	break;
      case 'p':
	Level->site[i][j].locchar = FLOOR;
	Level->site[i][j].p_locf = L_PORTCULLIS;
	break;
      case 'T':
	Level->site[i][j].locchar = FLOOR;
	if (! empty)
	  Level->site[i][j].p_locf = L_PORTCULLIS_TRAP;
	break;
      case 'X':
	Level->site[i][j].locchar = FLOOR;
	Level->site[i][j].p_locf = L_TACTICAL_EXIT;
	break;
      case '#':
	Level->site[i][j].locchar = WALL;
	Level->site[i][j].aux = 150; 
	break;
      case '4':
	Level->site[i][j].locchar = RUBBLE;
	Level->site[i][j].p_locf = L_RUBBLE;
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



/* loads the magic isle into Level*/
void load_misle(empty)
int empty;
{
  int i,j;
  char site;
  
  FILE *fd;

  if (empty) mprint("The isle is now devoid of inhabitants and treasure.");

  TempLevel = Level;
  if (ok_to_free(TempLevel)) {
    free((char *) TempLevel);
    TempLevel = NULL;
  }
  Level = ((plv) malloc(sizeof(levtype)));
  clear_level(Level);
  Level->environment = E_MAGIC_ISLE;
  strcpy(Str3,OMEGALIB);
  strcat(Str3,"omisle.dat");
  fd = fopen(Str3,"r");
  for(j=0;j<LENGTH;j++) {
    for(i=0;i<WIDTH;i++) {
      Level->site[i][j].lstatus = 0;
      Level->site[i][j].roomnumber = RS_MAGIC_ISLE;
      Level->site[i][j].p_locf = L_NO_OP;
      site = getc(fd);
      switch(site) {
      case 'E':
	Level->site[i][j].locchar = FLOOR;
	if (! empty)
	  make_site_monster(i,j,ML10+1); /* eater of magic */
	break;
      case 'm':
	Level->site[i][j].locchar = FLOOR;
	if (! empty)
	  make_site_monster(i,j,ML8+9); /* militant priest */
	break;
      case 'n':
	Level->site[i][j].locchar = FLOOR;
	if (! empty)
	  make_site_monster(i,j,ML7+1); /* nazgul */
	break;
      case 'X':
	Level->site[i][j].locchar = FLOOR;
	Level->site[i][j].p_locf = L_TACTICAL_EXIT;
	break;
      case '#':
	Level->site[i][j].locchar = WALL;
	Level->site[i][j].aux = 150; 
	break;
      case '4':
	Level->site[i][j].locchar = RUBBLE;
	Level->site[i][j].p_locf = L_RUBBLE;
	break;
      case '~':
	Level->site[i][j].locchar = WATER;
	Level->site[i][j].p_locf = L_CHAOS;
	break;
      case '=':
	Level->site[i][j].locchar = WATER;
	Level->site[i][j].p_locf = L_MAGIC_POOL;
	break;
      case '-':
	Level->site[i][j].locchar = CLOSED_DOOR;
	break;
      case '|':
	Level->site[i][j].locchar = OPEN_DOOR;
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


/* loads a temple into Level*/
void load_temple(deity)
int deity;
{
  int i,j;
  char site;
  pml ml;
  FILE *fd;

  TempLevel = Level;
  if (ok_to_free(TempLevel)) {
    free((char *) TempLevel);
    TempLevel = NULL;
  }
  Level = ((plv) malloc(sizeof(levtype)));
  clear_level(Level);
  Level->environment = E_TEMPLE;
  strcpy(Str3,OMEGALIB);
  strcat(Str3,"otemple.dat");
  fd = fopen(Str3,"r");
  for(j=0;j<LENGTH;j++) {
    for(i=0;i<WIDTH;i++) {
      switch(deity) {
      case ODIN: Level->site[i][j].roomnumber = RS_ODIN; break;
      case SET: Level->site[i][j].roomnumber = RS_SET; break;
      case HECATE: Level->site[i][j].roomnumber = RS_HECATE; break;
      case ATHENA: Level->site[i][j].roomnumber = RS_ATHENA; break;
      case DRUID: Level->site[i][j].roomnumber = RS_DRUID; break;
      case DESTINY: Level->site[i][j].roomnumber = RS_DESTINY; break;
      }
      site = getc(fd);
      switch(site) {
      case '8':
	Level->site[i][j].locchar = ALTAR;
	Level->site[i][j].p_locf = L_ALTAR;
	Level->site[i][j].aux = deity;
	break;
      case 'H':
	Level->site[i][j].locchar = FLOOR;
	if (strcmp(Player.name,Priest[Player.patron]) != 0) 
	  make_high_priest(i,j,deity);
	break;
      case 'S':
	Level->site[i][j].locchar = FLOOR;
	if (strcmp(Player.name,Priest[Player.patron]) != 0) 
	  lset(i,j,SECRET);
	break;
      case 'W':
	Level->site[i][j].locchar = FLOOR;
	if ((deity != Player.patron) &&
	    ((deity == ODIN) ||
	     (deity == SET) ||
	     (deity == HECATE) ||
	     (deity == ATHENA) ||
	     (deity == DESTINY)))
	  Level->site[i][j].p_locf = L_TEMPLE_WARNING;
	break;
      case 'm':
	Level->site[i][j].locchar = FLOOR;
	make_site_monster(i,j,ML8+9); /* militant priest */
	break;
      case 'd':
	Level->site[i][j].locchar = FLOOR;
	make_site_monster(i,j,ML4+10); /* doberman death hound */
	break;
      case 'X':
	Level->site[i][j].locchar = FLOOR;
	Level->site[i][j].p_locf = L_TACTICAL_EXIT;
	break;
      case '#':
	if (deity != DRUID) {
	  Level->site[i][j].locchar = WALL;
	  Level->site[i][j].aux = 150; 
	}
	else {
	  Level->site[i][j].locchar = HEDGE;
	  Level->site[i][j].p_locf = L_HEDGE; 
	}
	break;
      case '.':
	Level->site[i][j].locchar = FLOOR;
	break;
      case 'x':
	Level->site[i][j].locchar = FLOOR;
	random_temple_site(i,j,deity);
	break;
      case '?':
	if (deity != DESTINY) 
	  Level->site[i][j].locchar = FLOOR;
	else {
	  Level->site[i][j].locchar = ABYSS;
	  Level->site[i][j].p_locf = L_ADEPT;
	}
	break;
      case '-':
	Level->site[i][j].locchar = CLOSED_DOOR;
	break;
      case '|':
	Level->site[i][j].locchar = OPEN_DOOR;
	break;
      }
    }
    fscanf(fd,"\n");
  }
  /* Main Temple is peaceful for player of same sect,druids always peaceful. */
  if ((Player.patron == deity) || (deity == DRUID))
    for(ml=Level->mlist;ml!=NULL;ml=ml->next) 
      m_status_reset(ml->m,HOSTILE);
  fclose(fd);
}

void random_temple_site(i,j,deity)
int i,j,deity;
{
  switch(random_range(12)) {
  case 0:
    make_site_monster(i,j,ML0+1); break; /* mendicant priest */
  case 1:
    Level->site[i][j].locchar = WATER;
    Level->site[i][j].p_locf = L_MAGIC_POOL;
  case 2: make_site_monster(i,j,ML7+14); break; /* inner circle demon */
  case 3: 
    make_site_monster(i,j,ML6+11);
    Level->site[i][j].creature->aux1 = deity;
    break; /* angel of apropriate sect */
  case 4: 
    make_site_monster(i,j,ML8+11);
    Level->site[i][j].creature->aux1 = deity;
    break; /* archangel of apropriate sect */
  case 5: 
    make_site_monster(i,j,ML9+6);
    Level->site[i][j].creature->aux1 = deity;
    break; /* archangel of apropriate sect */
  }
}

void make_high_priest(i,j,deity)
int i,j,deity;
{
  pml ml = ((pml) malloc(sizeof(mltype)));
  pmt m = ((pmt) malloc(sizeof(montype)));
  make_hiscore_npc(m,deity);
  m->x = i;
  m->y = j;
  Level->site[i][j].creature = m;
  ml->m = m;
  ml->next = Level->mlist;
  Level->mlist = ml;
}
