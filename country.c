/* omega copyright (C) by Laurence Raphael Brothers, 1987,1988,1989 */
/* country.c */
/* load_country(), and all load_ functions for places which are */
/* accessible from the country and don't have their own files */

#include "glob.h"

/* loads the countryside level from the data file */
void load_country()
{
  int i,j;
  char site;
  
  FILE *fd;
  
  strcpy(Str3,Omegalib);
  strcat(Str3,"country.dat");
  fd = checkfopen(Str3,"rb");
  site = cryptkey("country.dat");

  for(j=0;j<LENGTH;j++) {
    for(i=0;i<WIDTH;i++) {
      site = getc(fd)^site;
      Country[i][j].aux = 0;
      Country[i][j].status = 0;
      switch (site) {
	case (PASS&0xff):
	  Country[i][j].base_terrain_type = PASS;
	  Country[i][j].current_terrain_type = MOUNTAINS;
	  break;
	case (CASTLE&0xff):
	  Country[i][j].base_terrain_type = CASTLE;
	  Country[i][j].current_terrain_type = MOUNTAINS;
	  break;
	case (STARPEAK&0xff):
	  Country[i][j].base_terrain_type = STARPEAK;
	  Country[i][j].current_terrain_type = MOUNTAINS;
	  break;
	case (CAVES&0xff):
	  Country[i][j].base_terrain_type = CAVES;
	  Country[i][j].current_terrain_type = MOUNTAINS;
	  break;
	case (VOLCANO&0xff):
	  Country[i][j].base_terrain_type = VOLCANO;
	  Country[i][j].current_terrain_type = MOUNTAINS;
	  break;
	case (DRAGONLAIR&0xff):
	  Country[i][j].base_terrain_type = DRAGONLAIR;
	  Country[i][j].current_terrain_type = DESERT;
	  break;
	case (MAGIC_ISLE&0xff):
	  Country[i][j].base_terrain_type = MAGIC_ISLE;
	  Country[i][j].current_terrain_type = CHAOS_SEA;
	  break;
	case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
	  Country[i][j].current_terrain_type =
	    Country[i][j].base_terrain_type = VILLAGE;
	  Country[i][j].aux = 1+site-'a';
	  break;
	case '1': case '2': case '3': case '4': case '5': case '6':
	  Country[i][j].current_terrain_type =
	    Country[i][j].base_terrain_type = TEMPLE;
	  Country[i][j].aux = site-'0';
	  break;
	case (PLAINS&0xff):
	  Country[i][j].current_terrain_type =
	      Country[i][j].base_terrain_type = PLAINS;
	  break;
	case (TUNDRA&0xff):
	  Country[i][j].current_terrain_type =
	      Country[i][j].base_terrain_type = TUNDRA;
	  break;
	case (ROAD&0xff):
	  Country[i][j].current_terrain_type =
	      Country[i][j].base_terrain_type = ROAD;
	  break;
	case (MOUNTAINS&0xff):
	  Country[i][j].current_terrain_type =
	      Country[i][j].base_terrain_type = MOUNTAINS;
	  break;
	case (RIVER&0xff):
	  Country[i][j].current_terrain_type =
	      Country[i][j].base_terrain_type = RIVER;
	  break;
	case (CITY&0xff):
	  Country[i][j].current_terrain_type =
	      Country[i][j].base_terrain_type = CITY;
	  break;
	case (FOREST&0xff):
	  Country[i][j].current_terrain_type =
	      Country[i][j].base_terrain_type = FOREST;
	  break;
	case (JUNGLE&0xff):
	  Country[i][j].current_terrain_type =
	      Country[i][j].base_terrain_type = JUNGLE;
	  break;
	case (SWAMP&0xff):
	  Country[i][j].current_terrain_type =
	      Country[i][j].base_terrain_type = SWAMP;
	  break;
	case (DESERT&0xff):
	  Country[i][j].current_terrain_type =
	      Country[i][j].base_terrain_type = DESERT;
	  break;
	case (CHAOS_SEA&0xff):
	  Country[i][j].current_terrain_type =
	      Country[i][j].base_terrain_type = CHAOS_SEA;
	  break;
      }
    }
    site = getc(fd)^site;
  }
  fclose(fd);
}







/* loads the dragon's lair into Level*/
void load_dlair(empty, populate)
int empty;
int populate;
{
  int i,j;
  char site;
  
  FILE *fd;

  if (empty) {
    mprint("The Lair is now devoid of inhabitants and treasure.");
    morewait();
  }

  if (!populate)
    empty = TRUE;
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
  Level->environment = E_DLAIR;
  strcpy(Str3,Omegalib);
  strcat(Str3,"dlair.dat");
  fd = checkfopen(Str3,"rb");
  site = cryptkey("dlair.dat");
  for(j=0;j<LENGTH;j++) {
    for(i=0;i<WIDTH;i++) {
      Level->site[i][j].lstatus = 0;
      if (i < 48)
	Level->site[i][j].roomnumber = RS_CAVERN;
      else
	Level->site[i][j].roomnumber = RS_DRAGONLORD;
      Level->site[i][j].p_locf = L_NO_OP;
      site = getc(fd)^site;
      switch(site) {
      case 'D':
	Level->site[i][j].locchar = FLOOR;
	if (! empty) {
	  make_site_monster(i,j,DRAGON_LORD);
	  Level->site[i][j].creature->specialf = M_SP_LAIR;
	}
	break;
      case 'd':
	Level->site[i][j].locchar = FLOOR;
	if (! empty) {
	  make_site_monster(i,j,DRAGON); /* elite dragons, actually */
	  Level->site[i][j].creature->specialf = M_SP_LAIR;
	  Level->site[i][j].creature->hit *= 2;
	  Level->site[i][j].creature->dmg *= 2;
	}
	break;
      case 'W':
	Level->site[i][j].locchar = FLOOR;
	if (! empty)
	  make_site_monster(i,j,KING_WYV);
	break;
      case 'M':
	Level->site[i][j].locchar = FLOOR;
	if (! empty)
	  make_site_monster(i,j,RANDOM);
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
    site = getc(fd)^site;
  }
  fclose(fd);
}





/* loads the star peak into Level*/
void load_speak(empty, populate)
int empty;
int populate;
{
  int i,j,safe = Player.alignment > 0;
  char site;
  
  FILE *fd;

  if (empty) {
    mprint("The peak is now devoid of inhabitants and treasure.");
    morewait();
  }

  if (!populate)
    empty = TRUE;

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
  Level->environment = E_STARPEAK;
  strcpy(Str3,Omegalib);
  strcat(Str3,"speak.dat");
  fd = checkfopen(Str3,"rb");
  site = cryptkey("speak.dat");
  for(j=0;j<LENGTH;j++) {
    for(i=0;i<WIDTH;i++) {
      Level->site[i][j].lstatus = 0;
      Level->site[i][j].roomnumber = RS_STARPEAK;
      Level->site[i][j].p_locf = L_NO_OP;
      site = getc(fd)^site;
      switch(site) {
      case 'S':
	Level->site[i][j].locchar = FLOOR;
	Level->site[i][j].showchar = WALL;
	lset(i,j,SECRET);
	Level->site[i][j].roomnumber = RS_SECRETPASSAGE;
	break;
      case 'L':
	Level->site[i][j].locchar = FLOOR;
	if (! empty) {
	  make_site_monster(i,j,LAWBRINGER);
	  if (safe) m_status_reset(Level->site[i][j].creature,HOSTILE);
	}
	break;
      case 's':
	Level->site[i][j].locchar = FLOOR;
	if (! empty) {
	  make_site_monster(i,j,SERV_LAW); /* servant of law */
	  if (safe) m_status_reset(Level->site[i][j].creature,HOSTILE);
	}
	break;
      case 'M':
	Level->site[i][j].locchar = FLOOR;
	if (! empty) {
	  make_site_monster(i,j,-1);
	  if (safe) m_status_reset(Level->site[i][j].creature,HOSTILE);
	}
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
    site = getc(fd)^site;
  }
  fclose(fd);
}



/* loads the magic isle into Level*/
void load_misle(empty, populate)
int empty;
int populate;
{
  int i,j;
  char site;
  
  FILE *fd;

  if (empty) {
    mprint("The isle is now devoid of inhabitants and treasure.");
    morewait();
  }

  if (!populate)
    empty = TRUE;

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
  Level->environment = E_MAGIC_ISLE;
  strcpy(Str3,Omegalib);
  strcat(Str3,"misle.dat");
  fd = checkfopen(Str3,"rb");
  site = cryptkey("misle.dat");
  for(j=0;j<LENGTH;j++) {
    for(i=0;i<WIDTH;i++) {
      Level->site[i][j].lstatus = 0;
      Level->site[i][j].roomnumber = RS_MAGIC_ISLE;
      Level->site[i][j].p_locf = L_NO_OP;
      site = getc(fd)^site;
      switch(site) {
      case 'E':
	Level->site[i][j].locchar = FLOOR;
	if (! empty)
	  make_site_monster(i,j,EATER); /* eater of magic */
	break;
      case 'm':
	Level->site[i][j].locchar = FLOOR;
	if (! empty)
	  make_site_monster(i,j,MIL_PRIEST); /* militant priest */
	break;
      case 'n':
	Level->site[i][j].locchar = FLOOR;
	if (! empty)
	  make_site_monster(i,j,NAZGUL);
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
    site = getc(fd)^site;
  }
  fclose(fd);
}


/* loads a temple into Level*/
void load_temple(deity, populate)
int deity;
int populate;
{
  int i,j;
  char site;
  pml ml;
  FILE *fd;
  
  /* WDT HACK: I don't know why this is wrong.  Shrug.  David Givens
   * suggested removing it, and he has more experience with Omega
   * than I, so... */
/*  initrand(Current_Environment, deity); */ /* FIXED! 12/30/98 */
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
  Level->environment = E_TEMPLE;
  strcpy(Str3,Omegalib);
  strcat(Str3,"temple.dat");
  fd = checkfopen(Str3,"rb");
  site = cryptkey("temple.dat");
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
      site = getc(fd)^site;
      switch(site) {
      case '8':
	Level->site[i][j].locchar = ALTAR;
	Level->site[i][j].p_locf = L_ALTAR;
	Level->site[i][j].aux = deity;
	break;
      case 'H':
	Level->site[i][j].locchar = FLOOR;
	if (populate && (!Player.patron ||
	    strcmp(Player.name,Priest[Player.patron]) ||
	    Player.rank[PRIESTHOOD] != HIGHPRIEST))
	  make_high_priest(i,j,deity);
	break;
      case 'S':
	Level->site[i][j].locchar = FLOOR;
	if (!Player.patron ||
	    strcmp(Player.name,Priest[Player.patron]) ||
	    Player.rank[PRIESTHOOD] != HIGHPRIEST)
	  lset(i,j,SECRET);
	break;
      case 'W':
	Level->site[i][j].locchar = FLOOR;
	if (deity != Player.patron && deity != DRUID)
	  Level->site[i][j].p_locf = L_TEMPLE_WARNING;
	break;
      case 'm':
	Level->site[i][j].locchar = FLOOR;
	if (populate)
	  make_site_monster(i,j,MIL_PRIEST); /* militant priest */
	break;
      case 'd':
	Level->site[i][j].locchar = FLOOR;
	if (populate)
	  make_site_monster(i,j,DOBERMAN); /* doberman death hound */
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
	random_temple_site(i,j,deity,populate);
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
    site = getc(fd)^site;
  }
  /* Main Temple is peaceful for player of same sect,druids always peaceful. */
  if ((Player.patron == deity) || (deity == DRUID))
    for(ml=Level->mlist;ml!=NULL;ml=ml->next) 
      m_status_reset(ml->m,HOSTILE);
  fclose(fd);
/*  initrand(-2, 0); */ /* FIXED! 12/30/98 */
}

void random_temple_site(i,j,deity,populate)
int i,j,deity,populate;
{
  switch(random_range(12)) {
  case 0:
    if (populate)
      make_site_monster(i,j,MEND_PRIEST);
    break; /* mendicant priest */
  case 1:
    Level->site[i][j].locchar = WATER;
    Level->site[i][j].p_locf = L_MAGIC_POOL;
  case 2:
    if (populate)
      make_site_monster(i,j,INNER_DEMON);
    break; /* inner circle demon */
  case 3: 
    if (populate)
      make_site_monster(i,j,ANGEL);
    break; /* angel of apropriate sect */
  case 4: 
    if (populate)
      make_site_monster(i,j,HIGH_ANGEL);
    break; /* high angel of apropriate sect */
  case 5: 
    if (populate)
      make_site_monster(i,j,ARCHANGEL);
    break; /* archangel of apropriate sect */
  }
}

void make_high_priest(i,j,deity)
int i,j,deity;
{
  pml ml = ((pml) checkmalloc(sizeof(mltype)));
  pmt m = ((pmt) checkmalloc(sizeof(montype)));
  make_hiscore_npc(m,deity);
  m->x = i;
  m->y = j;
  Level->site[i][j].creature = m;
  ml->m = m;
  ml->next = Level->mlist;
  Level->mlist = ml;
}
