/* omega copyright (C) by Laurence Raphael Brothers, 1987,1988,1989 */
/* abyss.c */
/* some functions to make the abyss level and run the final challenge */

#ifdef MSDOS_SUPPORTED_ANTIQUE
#include <sys/types.h>
#include <malloc.h>
#ifdef SAVE_LEVELS
#include <sys/timeb.h>
#include <dos.h>
#endif
#endif

#include "glob.h"

/* loads the abyss level into Level*/
void load_abyss()
{
  int i,j;
  char site;
  
  FILE *fd;
  
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

  strcpy(Str3,Omegalib);
  strcat(Str3,"abyss.dat");
  fd = checkfopen(Str3,"rb");
  site = cryptkey("abyss.dat");
  for(j=0;j<LENGTH;j++) {
    for(i=0;i<WIDTH;i++) {
      site = getc(fd)^site;
      Level->site[i][j].roomnumber = RS_ADEPT;
      switch(site) {
      case '0':
	Level->site[i][j].locchar = VOID_CHAR;
	Level->site[i][j].p_locf = L_VOID;
	break;
      case 'V':
	Level->site[i][j].locchar = VOID_CHAR;
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
    site = getc(fd)^site;
  }
  fclose(fd);
}


#ifdef SAVE_LEVELS
/* This stuff is in this file because the file was really small. */

void msdos_init()
{
    int i;

    /* Allocate the inner level of pointers for TheLevel */
    for (i = 0; i < MAXWIDTH; i++)
    	TheLevel.site[i] = (plc)checkmalloc(MAXLENGTH * sizeof(loctype));

    /* Remove old level files */
    kill_all_levels();
}

void kill_all_levels()
{
    kill_levels("om*.lev");
}

void kill_levels(str)
char *str;
{
    int i;
    struct find_t buf;

    /* Remove old level files laying around */
    sprintf(Str1,"%s%s",Omegalib,str);
    for (i = _dos_findfirst(Str1,_A_NORMAL,&buf); !i; i = _dos_findnext(&buf))
    {
    	sprintf(Str2,"%s%s",Omegalib,buf.name);
	remove(Str2);
    }
}

#define MEM_CHECK_AMOUNT 0xf000
void check_memory()
{
    char *mems[50];
    long amount = 0;
    int num_mems = 0;
    unsigned try;

    sprintf(Str1,"Heapchk returned %d.",_heapchk());
    mprint(Str1);

    try = MEM_CHECK_AMOUNT;
    while (try > 10000)
    {
    	while (try > 0 && (mems[num_mems] = checkmalloc(try)) == NULL)
	    try -= 0x400;
	amount += try;
	num_mems++;
    }
    while (--num_mems >= 0)
    	if (mems[num_mems] != NULL)
	    free(mems[num_mems]);

    sprintf(Str1,"Free mem approx %dK",(int)(amount / 0x400));
    mprint(Str1);
}

static FILE *open_levfile(env,depth,rw)
int env,depth,rw;
{
    sprintf(Str1,"%som%03d%03d.lev",Omegalib,env,depth);
    return(fopen(Str1,(rw) ? "wb" : "rb"));
}

/* Saves oldlevel (unless NULL), and reads in the new level,
   unless depth is < 0. */
plv msdos_changelevel(oldlevel,newenv,newdepth)
plv oldlevel;
int newenv,newdepth;
{
    FILE *fp;

    if (oldlevel != NULL)
    {
    	if (oldlevel->environment == newenv &&
	  oldlevel->depth == newdepth)
	    return(oldlevel);
    	if ((fp = open_levfile(oldlevel->environment,oldlevel->depth,1)) != NULL)
	{
	    save_level(fp,oldlevel);
	    fclose(fp);
	}
	else
	    mprint("Cannot save level!!!");
	/* Free up monsters and items */
	free_level(oldlevel);
    }
    if (newdepth >= 0)
    {
    	if ((fp = open_levfile(newenv,newdepth,0)) == NULL)
	    return(NULL);
	restore_level(fp);
	fclose(fp);
	return(Level);
    }
    return(NULL);
}

#endif

#ifdef DJGPP

void check_memory()
{
  clear_screen();
  print1("There should be over 300 K free on the drive.");  
  print2("Save _before_ the free space gets below 300 K.");
  morewait();
  system("dir");
  morewait();
  clear_screen();
  xredraw();
}
#endif
