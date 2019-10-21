/* omega copyright (C) by Laurence Raphael Brothers, 1987,1988,1989 */
/* oabyss.c */
/* some functions to make the abyss level and run the final challenge */

#ifdef MSDOS
#include <sys/types.h>
#include <sys/timeb.h>
#include <dos.h>
#include <alloc.h> //
#endif
#include "oglob.h"


/* loads the abyss level into Level*/
void load_abyss()
{
  int i,j;
  char site;
  
  FILE *fd;
  
  TempLevel = Level;
  if (ok_to_free(TempLevel)) {
#ifndef MSDOS
    free((char *) TempLevel);
#endif
    TempLevel = NULL;
  }
#ifndef MSDOS
  Level = ((plv) malloc(sizeof(levtype)));
#else
  msdos_changelevel(TempLevel,0,-1);
  Level = &TheLevel;
#endif
  
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


#ifdef MSDOS
/* This stuff is in this file because the file was really small. */

msdos_init()
{
    int i;

    /* Allocate the inner level of pointers for TheLevel */
    for (i = 0; i < MAXWIDTH; i++)
    	TheLevel.site[i] = (plc)malloc(MAXLENGTH * sizeof(loctype));

    /* Remove old level files */
    kill_all_levels();
}

kill_all_levels()
{
    kill_levels("om*.lev");
}

kill_levels(str)
char *str;
{
//  int i;
//  struct find_t buf;
//
//  /* Remove old level files laying around */
//  sprintf(Str1,"%s%s",OMEGALIB,str);
//  for (i = _dos_findfirst(Str1,_A_NORMAL,&buf); !i; i = _dos_findnext(&buf))
//  {
//  	sprintf(Str2,"%s%s",OMEGALIB,buf.name);
//		remove(Str2);
//  }
}

#define MEM_CHECK_AMOUNT 0xf000
check_memory()
{
    char *mems[50];
    long amount = 0;
    int num_mems = 0;
    unsigned try;

//	sprintf(Str1,"_heapchk returned %d.",_heapchk());
	mprint(Str1);

    try = MEM_CHECK_AMOUNT;
    while (try > 10000)
    {
    	while (try > 0 && (mems[num_mems] = malloc(try)) == NULL)
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

#ifndef MSDOS
sleep(n)
int n;
{
    struct timeb otime,ntime;

    ftime(&otime);
    do
    	ftime(&ntime);
    while (((short)(ntime.time - otime.time)) * 1000 +
      (short)ntime.millitm - (short)otime.millitm < n * 1000);
}
#endif

static FILE *open_levfile(env,depth,rw)
int env,depth,rw;
{
    sprintf(Str1,"%som%03d%03d.lev",OMEGALIB,env,depth);
    return(fopen(Str1,(rw) ? "wb" : "rb"));
}

static free_objlist(pobjlist)
pol pobjlist;
{
    pol tmp;

    while (pobjlist)
    {
    	free((tmp = pobjlist)->thing);
	pobjlist = pobjlist->next;
	free(tmp);
    }
}

static free_mons_and_objs(mlist)
pml mlist;
{
    pml tmp;

    while (mlist)
    {
    	free_objlist((tmp = mlist)->m->possessions);
	free(tmp->m);
	mlist = mlist->next;
	free(tmp);
    }
}

/* Free up monsters and items */
free_levelstuff(oldlevel)
plv oldlevel;
{
    int i,j;

    free_mons_and_objs(oldlevel->mlist);
    for (i = 0; i < MAXWIDTH; i++)
    	for (j = 0; j < MAXLENGTH; j++)
	    free_objlist(oldlevel->site[i][j].things);
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
    	if (fp = open_levfile(oldlevel->environment,oldlevel->depth,1))
	{
	    save_level(fp,oldlevel);
	    fclose(fp);
	}
	else
	    mprint("Cannot save level!!!");
	/* Free up monsters and items */
	free_levelstuff(oldlevel);
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