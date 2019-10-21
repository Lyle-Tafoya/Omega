/* omega copyright (c) 1987,1988 by Laurence Raphael Brothers */
/* osave.c */

#include "oglob.h"

/*Various functions for doing game saves and restores */
/*The game remembers various player information, the city level,
the country level, and the last or current dungeon level */

/**************** SAVE FUNCTIONS */

/* Checks to see if save file already exists.
   Checks to see if save file can be opened for write.
   The player, the city level, and the current dungeon level are saved.
*/
    
int save_game(compress,savestr)
int compress;
char *savestr;
{
  int i,writeok=TRUE;
  char cmd[80];
  FILE *fd =fopen(savestr,"r");
  if (fd != NULL) {
    mprint(" Overwrite old file?");
    writeok = (ynq() == 'y');
    fclose(fd);
  }
  if (writeok) {
    fd=fopen(savestr,"w");
    if (fd == NULL) {
      writeok = FALSE;
      mprint(" Error opening file.");
    }
  }
  if (! writeok)
    print2("Save aborted.");
  else {

    print1("Saving Game....");

    /* write the version number */
    i = VERSION;
    fwrite((char *)&i,sizeof(int),1,fd);
    /* write game id to save file */

    save_player(fd);
    save_country(fd);
    save_level(fd,City);
    
    if ((Current_Environment != E_CITY) &&
	(Current_Environment != E_COUNTRYSIDE)) {
      i = TRUE;
      fwrite((char *)&i,sizeof(int),1,fd);
      save_level(fd,Level);
    }
    else {
      i = FALSE;
      fwrite((char *)&i,sizeof(int),1,fd);
    }
    fclose(fd);
    print2("Game Saved.");

#ifdef COMPRESS_SAVE_FILES
    if (compress) {
      morewait();
      clearmsg();
      print1("Compressing Save File....");
      strcpy(cmd,"compress ");
      strcat(cmd,savestr);
      system(cmd);
      if (strlen(savestr) < 13) {
	strcpy(cmd,"mv ");
	strcat(cmd,savestr);
	strcat(cmd,".Z ");
	strcat(cmd,savestr);
	system(cmd);
      }
    }
#endif

    morewait();
    clearmsg();
  }
  return(writeok);
}




/* saves game on SIGHUP */
/* no longer tries to compress, which hangs */
int signalsave()
{
  int i;
  FILE *fd=fopen("Omega.Sav","w");
  if (fd != NULL) {
    /* write the version number */
    i = VERSION;
    fwrite((char *)&i,sizeof(int),1,fd);
    /* write game id to save file */

    save_player(fd);
    save_country(fd);
    save_level(fd,City);
    
    if ((Current_Environment != E_CITY) &&
	(Current_Environment != E_COUNTRYSIDE)) {
      i = TRUE;
      fwrite((char *)&i,sizeof(int),1,fd);
      save_level(fd,Level);
    }
    else {
      i = FALSE;
      fwrite((char *)&i,sizeof(int),1,fd);
    }
    fclose(fd);
  }
  exit(0);
}


    

	





/* also saves some globals like Level->depth... */

void save_player(fd)
FILE *fd;
{
  int i;

  /* Save random global state information */

  fwrite((char *)&Player,sizeof(Player),1,fd);
  fprintf(fd,"%s\n",Password);
  fprintf(fd,"%s\n",Player.name);
  fwrite((char *)CitySiteList,sizeof(CitySiteList),1,fd);
  fwrite((char *)&GameStatus,sizeof(long),1,fd);
  fwrite((char *)&Current_Environment,sizeof(int),1,fd);
  fwrite((char *)&Last_Environment,sizeof(int),1,fd);
  fwrite((char *)&Current_Dungeon,sizeof(int),1,fd);
  fwrite((char *)&Villagenum,sizeof(int),1,fd);
  fwrite((char *)&Verbosity,sizeof(char),1,fd);
  fwrite((char *)&Time,sizeof(int),1,fd);
  fwrite((char *)&Tick,sizeof(int),1,fd);
  fwrite((char *)&Searchnum,sizeof(int),1,fd);
  fwrite((char *)&Phase,sizeof(int),1,fd);
  fwrite((char *)&Date,sizeof(int),1,fd);
  fwrite((char *)&Spellsleft,sizeof(int),1,fd);
  fwrite((char *)&SymbolUseHour,sizeof(int),1,fd);
  fwrite((char *)&ViewHour,sizeof(int),1,fd);
  fwrite((char *)&HelmHour,sizeof(int),1,fd);
  fwrite((char *)&Constriction,sizeof(int),1,fd);
  fwrite((char *)&Blessing,sizeof(int),1,fd);
  fwrite((char *)&LastDay,sizeof(int),1,fd);
  fwrite((char *)&RitualHour,sizeof(int),1,fd);
  fwrite((char *)&Lawstone,sizeof(int),1,fd);
  fwrite((char *)&Chaostone,sizeof(int),1,fd);
  fwrite((char *)&Mindstone,sizeof(int),1,fd);
  fwrite((char *)&Arena_Opponent,sizeof(int),1,fd);
  fwrite((char *)&Imprisonment,sizeof(int),1,fd);
  fwrite((char *)&Gymcredit,sizeof(int),1,fd);
  fwrite((char *)&Balance,sizeof(int),1,fd);
  fwrite((char *)&StarGemUse,sizeof(int),1,fd);
  fwrite((char *)&HiMagicUse,sizeof(int),1,fd);
  fwrite((char *)&HiMagic,sizeof(int),1,fd);
  fwrite((char *)&FixedPoints,sizeof(int),1,fd);
  fwrite((char *)&LastCountryLocX,sizeof(int),1,fd);
  fwrite((char *)&LastCountryLocY,sizeof(int),1,fd);
  fwrite((char *)&LastTownLocX,sizeof(int),1,fd);
  fwrite((char *)&LastTownLocY,sizeof(int),1,fd);
  fwrite((char *)&Pawndate,sizeof(int),1,fd);


  fwrite((char *)Spells,sizeof(Spells),1,fd);

  /* Save player possessions */

  for(i=0;i<MAXITEMS;i++) save_item(fd,Player.possessions[i]);
  for(i=0;i<MAXPACK;i++) save_item(fd,Player.pack[i]);
  for(i=0;i<PAWNITEMS;i++) save_item(fd,Pawnitems[i]);

  /* Save items in condo vault */
  save_itemlist(fd,Condoitems);

  /* Save player item knowledge */
  for (i=0;i<TOTALITEMS;i++)
    fwrite((char *)&(Objects[i].known),sizeof(Objects[i].known),1,fd);
}


/* Save whatever is pointed to by level */
void save_level(fd,level)
FILE *fd;
plv level;
{
  int i,j;

  fwrite((char *)level,sizeof(levtype),1,fd);
  save_monsters(fd,level->mlist);
  for(i=0;i<MAXWIDTH;i++)
    for(j=0;j<MAXLENGTH;j++)
      save_itemlist(fd,level->site[i][j].things);
}


void save_monsters(fd,ml)
FILE *fd;
pml ml;
{
  pml tml;
  int nummonsters=0;

  /* First count monsters */
  for(tml=ml;tml!=NULL;tml=tml->next) 
    if (tml->m->hp > 0) nummonsters++;

  fwrite((char *)&nummonsters,sizeof(int),1,fd);

  /* Now save monsters */
  for(tml=ml;tml!=NULL;tml=tml->next) {
    if (tml->m->hp > 0) {
      fwrite((char *)tml->m,sizeof(montype),1,fd);
      fprintf(fd,"%s\n",tml->m->monstring);
      save_itemlist(fd,tml->m->possessions);
    }
  }
}


/* Save o unless it's null, then save a null item instead */
void save_item(fd,o)
FILE *fd;
pob o;
{
  objtype nullobject;
  nullobject.id = -1;
  if (o == NULL) {
    fwrite((char *)&nullobject,sizeof(objtype),1,fd);
    fprintf(fd,"Null Object. Report if you see this!\n");
  }
  else {
    fwrite((char *)o,sizeof(objtype),1,fd);
    fprintf(fd,"%s\n",o->truename);
  }
}

void save_itemlist(fd,ol)
FILE *fd;
pol ol;
{
  int numitems = 0;
  pol tol;
  for(tol=ol;tol!=NULL;tol=tol->next) numitems++;
  fwrite((char *)&numitems,sizeof(int),1,fd);
  for(tol=ol;tol!=NULL;tol=tol->next) 
    save_item(fd,tol->thing);
}


void save_country(fd)
FILE *fd;
{
  fwrite((char *)Country,sizeof(Country),1,fd);
}




/* read player data, city level, dungeon level,
   check on validity of save file, etc.
   return TRUE if game restored, FALSE otherwise */

int restore_game(savestr)
char *savestr;
{
  int i,version; 
  char cmd[80];
  FILE *fd;


#ifdef COMPRESS_SAVE_FILES
  print1("Uncompressing Save File....");
  if (strlen(savestr) < 13) {
    strcpy(cmd,"mv ");
    strcat(cmd,savestr);
    strcat(cmd," ");
    strcat(savestr,".Z");
    strcat(cmd,savestr);
    system(cmd);
  }
  strcpy(cmd,"uncompress ");
  strcat(cmd,savestr);
  system(cmd);
  print2("Save file uncompressed.");
  morewait();
  if ((savestr[strlen(savestr)-1] == 'Z') &&
      (savestr[strlen(savestr)-2] == '.'))
    savestr[strlen(savestr)-2] = 0;
#endif
  
  fd = fopen(savestr,"r");

  if (fd == NULL) {
    print1("Error restoring game -- aborted.");
    print2("File name was: ");
    nprint2(savestr);
    morewait();
    return(FALSE);
  }
  else {
    print1("Restoring...");

    fread((char *)&version,sizeof(int),1,fd);

    if (VERSION != version) {
      print2(" Sorry, I can't restore an outdated save file!");
      morewait();
      return(FALSE);
    }
    restore_player(fd);
    restore_country(fd);
    restore_level(fd); /* the city level */
    fread((char *)&i,sizeof(int),1,fd);
    if (i == TRUE)
      restore_level(fd);
    print3("Restoration complete.");
    ScreenOffset = -1000;
    screencheck(Player.y);
    drawvision(Player.x,Player.y);
    setgamestatus(SKIP_MONSTERS);
    dataprint();
    showflags();
    fclose(fd);
    return(TRUE);
  }
}



void restore_player(fd)
FILE *fd;
{
  int i;
  fread((char *)&Player,sizeof(Player),1,fd);
  filescanstring(fd,Password);
  filescanstring(fd,Player.name);
  fread((char *)CitySiteList,sizeof(CitySiteList),1,fd);
  fread((char *)&GameStatus,sizeof(long),1,fd);
  fread((char *)&Current_Environment,sizeof(int),1,fd);
  fread((char *)&Last_Environment,sizeof(int),1,fd);
  fread((char *)&Current_Dungeon,sizeof(int),1,fd);
  fread((char *)&Villagenum,sizeof(int),1,fd);
  switch(Current_Dungeon) {
  case E_ASTRAL: MaxDungeonLevels = ASTRALLEVELS; break;
  case E_SEWERS: MaxDungeonLevels = SEWERLEVELS; break;
  case E_CASTLE: MaxDungeonLevels = CASTLELEVELS; break;
  case E_CAVES: MaxDungeonLevels = CAVELEVELS; break;
  case E_VOLCANO: MaxDungeonLevels = VOLCANOLEVELS; break;
  }
  fread((char *)&Verbosity,sizeof(char),1,fd);
  fread((char *)&Time,sizeof(int),1,fd);
  fread((char *)&Tick,sizeof(int),1,fd);
  fread((char *)&Searchnum,sizeof(int),1,fd);
  fread((char *)&Phase,sizeof(int),1,fd);
  fread((char *)&Date,sizeof(int),1,fd);
  fread((char *)&Spellsleft,sizeof(int),1,fd);
  fread((char *)&SymbolUseHour,sizeof(int),1,fd);
  fread((char *)&ViewHour,sizeof(int),1,fd);
  fread((char *)&HelmHour,sizeof(int),1,fd);
  fread((char *)&Constriction,sizeof(int),1,fd);
  fread((char *)&Blessing,sizeof(int),1,fd);
  fread((char *)&LastDay,sizeof(int),1,fd);
  fread((char *)&RitualHour,sizeof(int),1,fd);
  fread((char *)&Lawstone,sizeof(int),1,fd);
  fread((char *)&Chaostone,sizeof(int),1,fd);
  fread((char *)&Mindstone,sizeof(int),1,fd);
  fread((char *)&Arena_Opponent,sizeof(int),1,fd);
  fread((char *)&Imprisonment,sizeof(int),1,fd);
  fread((char *)&Gymcredit,sizeof(int),1,fd);
  fread((char *)&Balance,sizeof(int),1,fd);
  fread((char *)&StarGemUse,sizeof(int),1,fd);
  fread((char *)&HiMagicUse,sizeof(int),1,fd);
  fread((char *)&HiMagic,sizeof(int),1,fd);
  fread((char *)&FixedPoints,sizeof(int),1,fd);
  fread((char *)&LastCountryLocX,sizeof(int),1,fd);
  fread((char *)&LastCountryLocY,sizeof(int),1,fd);
  fread((char *)&LastTownLocX,sizeof(int),1,fd);
  fread((char *)&LastTownLocY,sizeof(int),1,fd);
  fread((char *)&Pawndate,sizeof(int),1,fd);
  fread((char *)Spells,sizeof(Spells),1,fd);
  for(i=0;i<MAXITEMS;i++) 
    Player.possessions[i] = restore_item(fd);
  for(i=0;i<MAXPACK;i++) 
    Player.pack[i] = restore_item(fd);
  for(i=0;i<PAWNITEMS;i++) 
    Pawnitems[i] = restore_item(fd);
  Condoitems = restore_itemlist(fd);
  for (i=0;i<TOTALITEMS;i++) 
    fread((char *)&(Objects[i].known),sizeof(int),1,fd);
  calc_melee();
}



pob restore_item(fd)
FILE *fd;
{
  char tempstr[80];
  pob obj = ((pob) malloc(sizeof(objtype)));
  fread((char *)obj,sizeof(objtype),1,fd);
  filescanstring(fd,tempstr);
  obj->truename = salloc(tempstr);
  if (obj->id < 0) {
    free((char *) obj);
    return(NULL);
  }
  else {
    obj->objstr = Objects[obj->id].objstr;
    obj->cursestr = Objects[obj->id].cursestr;
    return(obj);
  }
}

pol restore_itemlist(fd)
FILE *fd;
{
  pol ol=NULL,cur=NULL,new=NULL;
  int i,numitems,firsttime=TRUE;
  fread((char *)&numitems,sizeof(int),1,fd);
  for(i=0;i<numitems;i++) {
    new = ((pol) malloc(sizeof(oltype)));
    new->thing = restore_item(fd);
    new->next = NULL;
    if (firsttime==TRUE) {
      ol = cur = new;
      firsttime = FALSE;
    }
    else {
      cur->next = new;
      cur = new;
    }
  }
  return(ol);
}








void restore_level(fd)
FILE *fd;
{
  int i,j;

  Level = ((plv) malloc(sizeof(levtype)));
  
  fread((char *)Level,sizeof(levtype),1,fd);

  Level->next = NULL;

  restore_monsters(fd,Level);
  for(i=0;i<MAXWIDTH;i++)
    for(j=0;j<MAXLENGTH;j++)
      Level->site[i][j].things = restore_itemlist(fd);
  if (Level->environment == E_CITY) City = Level;
  else if (Level->environment == Current_Dungeon) Dungeon = Level;
}




void restore_monsters(fd,level)
FILE *fd;
plv level;
{
  pml ml=NULL;
  int i,nummonsters;
  char tempstr[80];

  level->mlist = NULL;

  fread((char *)&nummonsters,sizeof(int),1,fd);
  
  for(i=0;i<nummonsters;i++) {
    ml = ((pml) malloc(sizeof(mltype)));
    ml->m = ((pmt) malloc(sizeof(montype)));
    ml->next = NULL;
    fread((char *)ml->m,sizeof(montype),1,fd);
    filescanstring(fd,tempstr);
    ml->m->monstring = salloc(tempstr);
    ml->m->corpsestr = salloc(Monsters[ml->m->id].corpsestr);
    ml->m->meleestr = salloc(Monsters[ml->m->id].meleestr);
    level->site[ml->m->x][ml->m->y].creature = ml->m;
    ml->m->possessions = restore_itemlist(fd);
    ml->next = level->mlist;
    level->mlist = ml;
  }
}



void restore_country(fd)
FILE *fd;
{
  fread((char *)Country,sizeof(Country),1,fd);
}
