/* omega copyright (c) 1987,1988,1989 by Laurence Raphael Brothers */
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
#ifdef MSDOS
  int tmpdepth;
#endif
  int i,writeok=TRUE;
  char cmd[80];
#ifndef MSDOS
  FILE *fd =fopen(savestr,"r");
#else
  FILE *fd =fopen(savestr,"rb");
#endif
  if (fd != NULL) {
    mprint(" Overwrite old file?");
    writeok = (ynq() == 'y');
    fclose(fd);
  }
  if (writeok) {
#ifndef MSDOS
    fd=fopen(savestr,"w");
#else
    fd=fopen(savestr,"wb");
#endif
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
#ifdef MSDOS
    tmpdepth = Level->depth;
    City = msdos_changelevel(Level,E_CITY,0);
#endif
    save_level(fd,City);
    
    if ((Current_Environment != E_CITY) &&
	(Current_Environment != E_COUNTRYSIDE)) {
      i = TRUE;
      fwrite((char *)&i,sizeof(int),1,fd);
#ifdef MSDOS
      Level = msdos_changelevel(NULL,Current_Environment,tmpdepth);
#endif
      save_level(fd,Level);
    }
    else {
      i = FALSE;
      fwrite((char *)&i,sizeof(int),1,fd);
    }
    fclose(fd);
    print1("Game Saved.");

#ifdef COMPRESS_SAVE_FILES
    if (compress) {
      print2("Compressing Save File....");
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
#ifndef MSDOS
  FILE *fd=fopen("Omega.Sav","w");
#else
  FILE *fd=fopen("Omega.Sav","wb");
#endif
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
  int i,twohand;

  twohand =
    ((Player.possessions[O_READY_HAND] == Player.possessions[O_WEAPON_HAND])
     && (Player.possessions[O_READY_HAND] != NULL));

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
#ifndef MSDOS
  fwrite((char *)&Time,sizeof(int),1,fd);
#else
  fwrite((char *)&Time,sizeof(long),1,fd);
#endif
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
#ifndef MSDOS
  fwrite((char *)&Gymcredit,sizeof(int),1,fd);
#else
  fwrite((char *)&Gymcredit,sizeof(long),1,fd);
#endif
#ifndef MSDOS
  fwrite((char *)&Balance,sizeof(int),1,fd);
#else
  fwrite((char *)&Balance,sizeof(long),1,fd);
#endif
  fwrite((char *)&StarGemUse,sizeof(int),1,fd);
  fwrite((char *)&HiMagicUse,sizeof(int),1,fd);
  fwrite((char *)&HiMagic,sizeof(int),1,fd);
#ifndef MSDOS
  fwrite((char *)&FixedPoints,sizeof(int),1,fd);
#else
  fwrite((char *)&FixedPoints,sizeof(long),1,fd);
#endif
  fwrite((char *)&LastCountryLocX,sizeof(int),1,fd);
  fwrite((char *)&LastCountryLocY,sizeof(int),1,fd);
  fwrite((char *)&LastTownLocX,sizeof(int),1,fd);
  fwrite((char *)&LastTownLocY,sizeof(int),1,fd);
  fwrite((char *)&Pawndate,sizeof(int),1,fd);

  fwrite((char *)&twohand,sizeof(int),1,fd);

  fwrite((char *)Spells,sizeof(Spells),1,fd);

  fwrite((char *)&Command_Duration,sizeof(Command_Duration),1,fd);
  fwrite((char *)&Precipitation,sizeof(Precipitation),1,fd);
  fwrite((char *)&Lunarity,sizeof(Lunarity),1,fd);
  fwrite((char *)&ZapHour,sizeof(ZapHour),1,fd);
  fwrite((char *)&RitualRoom,sizeof(RitualRoom),1,fd);

  /* High score stuff */
  fwrite((char *)&Shadowlordbehavior,sizeof(Shadowlordbehavior),1,fd);
  fwrite((char *)&Archmagebehavior,sizeof(Archmagebehavior),1,fd);
  fwrite((char *)&Primebehavior,sizeof(Primebehavior),1,fd);
  fwrite((char *)&Justiciarbehavior,sizeof(Justiciarbehavior),1,fd);
  fwrite((char *)&Commandantbehavior,sizeof(Commandantbehavior),1,fd);
  fwrite((char *)&Chaoslordbehavior,sizeof(Chaoslordbehavior),1,fd);
  fwrite((char *)&Lawlordbehavior,sizeof(Lawlordbehavior),1,fd);
  fwrite((char *)&Championbehavior,sizeof(Championbehavior),1,fd);
  fwrite((char *)Priestbehavior,sizeof(Priestbehavior),1,fd);
  fwrite((char *)&Hibehavior,sizeof(Hibehavior),1,fd);
  fwrite((char *)&Dukebehavior,sizeof(Dukebehavior),1,fd);
  fwrite((char *)Shadowlord,sizeof(Shadowlord),1,fd);
  fwrite((char *)Archmage,sizeof(Archmage),1,fd);
  fwrite((char *)Prime,sizeof(Prime),1,fd);
  fwrite((char *)Commandant,sizeof(Commandant),1,fd);
  fwrite((char *)Duke,sizeof(Duke),1,fd);
  fwrite((char *)Champion,sizeof(Champion),1,fd);
  fwrite((char *)Priest,sizeof(Priest),1,fd);
  fwrite((char *)Hiscorer,sizeof(Hiscorer),1,fd);
  fwrite((char *)Hidescrip,sizeof(Hidescrip),1,fd);
  fwrite((char *)Chaoslord,sizeof(Chaoslord),1,fd);
  fwrite((char *)Lawlord,sizeof(Lawlord),1,fd);
  fwrite((char *)Justiciar,sizeof(Justiciar),1,fd);
  fwrite((char *)&Shadowlordlevel,sizeof(Shadowlordlevel),1,fd);
  fwrite((char *)&Archmagelevel,sizeof(Archmagelevel),1,fd);
  fwrite((char *)&Primelevel,sizeof(Primelevel),1,fd);
  fwrite((char *)&Commandantlevel,sizeof(Commandantlevel),1,fd);
  fwrite((char *)&Dukelevel,sizeof(Dukelevel),1,fd);
  fwrite((char *)&Championlevel,sizeof(Championlevel),1,fd);
  fwrite((char *)Priestlevel,sizeof(Priestlevel),1,fd);
  fwrite((char *)&Hiscore,sizeof(Hiscore),1,fd);
  fwrite((char *)&Hilevel,sizeof(Hilevel),1,fd);
  fwrite((char *)&Justiciarlevel,sizeof(Justiciarlevel),1,fd);
  fwrite((char *)&Chaoslordlevel,sizeof(Chaoslordlevel),1,fd);
  fwrite((char *)&Lawlordlevel,sizeof(Lawlordlevel),1,fd);
  fwrite((char *)&Chaos,sizeof(Chaos),1,fd);
  fwrite((char *)&Law,sizeof(Law),1,fd);

  /* stuff which used to be statics */
  fwrite((char *)&twiddle,sizeof(twiddle),1,fd);
  fwrite((char *)&saved,sizeof(saved),1,fd);
  fwrite((char *)&onewithchaos,sizeof(onewithchaos),1,fd);
  fwrite((char *)&club_hinthour,sizeof(club_hinthour),1,fd);
  fwrite((char *)&winnings,sizeof(winnings),1,fd);
  fwrite((char *)&tavern_hinthour,sizeof(tavern_hinthour),1,fd);
  fwrite((char *)scroll_ids,sizeof(scroll_ids),1,fd);
  fwrite((char *)potion_ids,sizeof(potion_ids),1,fd);
  fwrite((char *)stick_ids,sizeof(stick_ids),1,fd);
  fwrite((char *)ring_ids,sizeof(ring_ids),1,fd);
  fwrite((char *)cloak_ids,sizeof(cloak_ids),1,fd);
  fwrite((char *)boot_ids,sizeof(boot_ids),1,fd);
  fwrite((char *)deepest,sizeof(deepest),1,fd);

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
#ifdef MSDOS
  for (i = 0; i < MAXWIDTH; i++)
      fwrite((char *)level->site[i],MAXLENGTH * sizeof(loctype),1,fd);
#endif
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
  
#ifndef MSDOS
  fd = fopen(savestr,"r");
#else
  fd = fopen(savestr,"rb");
#endif

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
#ifndef MSDOS
      restore_level(fd);
#else
    {
      msdos_changelevel(Level,0,-1);
      restore_level(fd);
    }
#endif
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
  int i,twohand;
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
#ifndef MSDOS
  fread((char *)&Time,sizeof(int),1,fd);
#else
  fread((char *)&Time,sizeof(long),1,fd);
#endif
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
#ifndef MSDOS
  fread((char *)&Gymcredit,sizeof(int),1,fd);
#else
  fread((char *)&Gymcredit,sizeof(long),1,fd);
#endif
#ifndef MSDOS
  fread((char *)&Balance,sizeof(int),1,fd);
#else
  fread((char *)&Balance,sizeof(long),1,fd);
#endif
  fread((char *)&StarGemUse,sizeof(int),1,fd);
  fread((char *)&HiMagicUse,sizeof(int),1,fd);
  fread((char *)&HiMagic,sizeof(int),1,fd);
#ifndef MSDOS
  fread((char *)&FixedPoints,sizeof(int),1,fd);
#else
  fread((char *)&FixedPoints,sizeof(long),1,fd);
#endif
  fread((char *)&LastCountryLocX,sizeof(int),1,fd);
  fread((char *)&LastCountryLocY,sizeof(int),1,fd);
  fread((char *)&LastTownLocX,sizeof(int),1,fd);
  fread((char *)&LastTownLocY,sizeof(int),1,fd);
  fread((char *)&Pawndate,sizeof(int),1,fd);

  fread((char *)&twohand,sizeof(int),1,fd);

  fread((char *)Spells,sizeof(Spells),1,fd);

  fread((char *)&Command_Duration,sizeof(Command_Duration),1,fd);
  fread((char *)&Precipitation,sizeof(Precipitation),1,fd);
  fread((char *)&Lunarity,sizeof(Lunarity),1,fd);
  fread((char *)&ZapHour,sizeof(ZapHour),1,fd);
  fread((char *)&RitualRoom,sizeof(RitualRoom),1,fd);

  /* High score stuff */
  fread((char *)&Shadowlordbehavior,sizeof(Shadowlordbehavior),1,fd);
  fread((char *)&Archmagebehavior,sizeof(Archmagebehavior),1,fd);
  fread((char *)&Primebehavior,sizeof(Primebehavior),1,fd);
  fread((char *)&Justiciarbehavior,sizeof(Justiciarbehavior),1,fd);
  fread((char *)&Commandantbehavior,sizeof(Commandantbehavior),1,fd);
  fread((char *)&Chaoslordbehavior,sizeof(Chaoslordbehavior),1,fd);
  fread((char *)&Lawlordbehavior,sizeof(Lawlordbehavior),1,fd);
  fread((char *)&Championbehavior,sizeof(Championbehavior),1,fd);
  fread((char *)Priestbehavior,sizeof(Priestbehavior),1,fd);
  fread((char *)&Hibehavior,sizeof(Hibehavior),1,fd);
  fread((char *)&Dukebehavior,sizeof(Dukebehavior),1,fd);
  fread((char *)Shadowlord,sizeof(Shadowlord),1,fd);
  fread((char *)Archmage,sizeof(Archmage),1,fd);
  fread((char *)Prime,sizeof(Prime),1,fd);
  fread((char *)Commandant,sizeof(Commandant),1,fd);
  fread((char *)Duke,sizeof(Duke),1,fd);
  fread((char *)Champion,sizeof(Champion),1,fd);
  fread((char *)Priest,sizeof(Priest),1,fd);
  fread((char *)Hiscorer,sizeof(Hiscorer),1,fd);
  fread((char *)Hidescrip,sizeof(Hidescrip),1,fd);
  fread((char *)Chaoslord,sizeof(Chaoslord),1,fd);
  fread((char *)Lawlord,sizeof(Lawlord),1,fd);
  fread((char *)Justiciar,sizeof(Justiciar),1,fd);
  fread((char *)&Shadowlordlevel,sizeof(Shadowlordlevel),1,fd);
  fread((char *)&Archmagelevel,sizeof(Archmagelevel),1,fd);
  fread((char *)&Primelevel,sizeof(Primelevel),1,fd);
  fread((char *)&Commandantlevel,sizeof(Commandantlevel),1,fd);
  fread((char *)&Dukelevel,sizeof(Dukelevel),1,fd);
  fread((char *)&Championlevel,sizeof(Championlevel),1,fd);
  fread((char *)Priestlevel,sizeof(Priestlevel),1,fd);
  fread((char *)&Hiscore,sizeof(Hiscore),1,fd);
  fread((char *)&Hilevel,sizeof(Hilevel),1,fd);
  fread((char *)&Justiciarlevel,sizeof(Justiciarlevel),1,fd);
  fread((char *)&Chaoslordlevel,sizeof(Chaoslordlevel),1,fd);
  fread((char *)&Lawlordlevel,sizeof(Lawlordlevel),1,fd);
  fread((char *)&Chaos,sizeof(Chaos),1,fd);
  fread((char *)&Law,sizeof(Law),1,fd);

  /* stuff which used to be statics */
  fread((char *)&twiddle,sizeof(twiddle),1,fd);
  fread((char *)&saved,sizeof(saved),1,fd);
  fread((char *)&onewithchaos,sizeof(onewithchaos),1,fd);
  fread((char *)&club_hinthour,sizeof(club_hinthour),1,fd);
  fread((char *)&winnings,sizeof(winnings),1,fd);
  fread((char *)&tavern_hinthour,sizeof(tavern_hinthour),1,fd);
  fread((char *)scroll_ids,sizeof(scroll_ids),1,fd);
  fread((char *)potion_ids,sizeof(potion_ids),1,fd);
  fread((char *)stick_ids,sizeof(stick_ids),1,fd);
  fread((char *)ring_ids,sizeof(ring_ids),1,fd);
  fread((char *)cloak_ids,sizeof(cloak_ids),1,fd);
  fread((char *)boot_ids,sizeof(boot_ids),1,fd);
  fread((char *)deepest,sizeof(deepest),1,fd);

  /* Set up the strings for the id's */
  inititem(0);

  for(i=0;i<MAXITEMS;i++) 
    Player.possessions[i] = restore_item(fd);
  if (twohand) {
    free((char *)Player.possessions[O_READY_HAND]);
    Player.possessions[O_READY_HAND] = 
      Player.possessions[O_WEAPON_HAND];
  }
  for(i=0;i<MAXPACK;i++) 
    Player.pack[i] = restore_item(fd);
  for(i=0;i<PAWNITEMS;i++) 
    Pawnitems[i] = restore_item(fd);
  Condoitems = restore_itemlist(fd);
  for (i=0;i<TOTALITEMS;i++) 
    fread((char *)&(Objects[i].known),sizeof(Objects[i].known),1,fd);
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

#ifndef MSDOS
  Level = ((plv) malloc(sizeof(levtype)));
  
  fread((char *)Level,sizeof(levtype),1,fd);
#else
  plc *tmp = (plc *)malloc(MAXWIDTH * sizeof(plc));

  Level = &TheLevel;
  for (i = 0; i < MAXWIDTH; i++)
    tmp[i] = Level->site[i];
  fread((char *)Level,sizeof(levtype),1,fd);
  for (i = 0; i < MAXWIDTH; i++)
    fread((char *)(Level->site[i] = tmp[i]),MAXLENGTH * sizeof(loctype),1,fd);
  free(tmp);
#endif

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
}