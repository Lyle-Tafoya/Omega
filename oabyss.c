/* omega copyright (C) by Laurence Raphael Brothers, 1987,1988 */
/* oabyss.c */
/* some functions to make the abyss level and run the final challenge */

#include "oglob.h"


/* loads the abyss level into Level*/
void load_abyss()
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
  strcat(Str3,"oabyss.dat");
  fd = fopen(Str3,"r");
  for(j=0;j<LENGTH;j++) {
    for(i=0;i<WIDTH;i++) {
      site = getc(fd);
      switch(site) {
      case '0':
	Level->site[i][j].locchar = ' ';
	Level->site[i][j].p_locf = L_VOID;
	break;
      case 'V':
	Level->site[i][j].locchar = ' ';
	Level->site[i][j].p_locf = L_VOID_STATION;
	break;
      case '1':
	Level->site[i][j].locchar = FLOOR;
	Level->site[i][j].p_locf = L_VOICE1;
	break;
      case '2':
	Level->site[i][j].locchar = FLOOR;
	Level->site[i][j].p_locf = L_VOICE2;
	break;
      case '3':
	Level->site[i][j].locchar = FLOOR;
	Level->site[i][j].p_locf = L_VOICE3;
	break;
      case '~':
	Level->site[i][j].locchar = WATER;
	Level->site[i][j].p_locf = L_WATER_STATION;
	break;
      case ';':
	Level->site[i][j].locchar = FIRE;
	Level->site[i][j].p_locf = L_FIRE_STATION;
	break;
      case '"':
	Level->site[i][j].locchar = HEDGE;
	Level->site[i][j].p_locf = L_EARTH_STATION;
	break;
      case '6':
	Level->site[i][j].locchar = WHIRLWIND;
	Level->site[i][j].p_locf = L_AIR_STATION;
	break;
      case '#':
	Level->site[i][j].locchar = WALL;
	break;
      case '.':
	Level->site[i][j].locchar = FLOOR;
	break;
      }
    }
    fscanf(fd,"\n");
  }
}
