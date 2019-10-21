/* omega copyright (c) 1987 by Laurence Raphael Brothers */
/* olev.c */

#include <stdio.h>
#include <strings.h>
#include "oglob.h"


/* from oinitmon's */
extern void oinitmon0to3(),oinitmon4to7(),oinitmon8to10();

/* from outil */
extern int random_range();
extern void findspace();

/* from omon */
extern void m_status_set(),make_log_npc(),m_pickup();

/* from oitem */
extern pob create_object();
extern void make_cash();

/* from ogen */
extern void clear_level(),clear_dungeon();

/* from oscr */
extern void erase_level(),mprint(),morewait(),show_screen();
extern char ynq();
extern void screencheck(),drawvision(),showflags();

/* from ofile */
extern void filescanstring();

/* olev functions */
/* level creation functions */
void populate_level(),stock_level(),wandercheck();
pmt m_create(),make_creature();

/* save/restore functions */
void itemusecheck();
int save_game(),restore_game(),gameidcheck();

void save_player(),save_player_items(),save_player_spells();
void save_player_item_knowledge(),save_level_monsters();
void save_level(),save_item(),save_itemlist();

void restore_player(),restore_player_items(),restore_player_spells();
void restore_player_item_knowledge(),restore_level_monsters();
void restore_level(),write_int();
int read_int();
pob restore_item();
pol restore_itemlist();


/* If monster is -1, use random monsters, otherwise use that monster id to
   populate level */
void populate_level(level,monster)
int level;
int monster;
{
  pml head,tml;
  int i,j,k,nummonsters=(random_range(level/5)+1)*3+8;
  head = tml = ((pml) malloc(sizeof(mltype)));
  if (monster > -1) nummonsters *=2;
  for(k=0;k<nummonsters;k++) {
    findspace(&i,&j);
    if (monster > -1) {
      Dungeon[level][i][j].creature = make_creature(monster);
      Dungeon[level][i][j].creature->x = i;
      Dungeon[level][i][j].creature->y = j;
      if (m_statusp(Dungeon[level][i][j].creature,SWIMMING)) {
	Dungeon[level][i][j].locchar = WATER;
	Dungeon[level][i][j].p_locf = L_WATER;
      }
    }
    else Dungeon[level][i][j].creature = m_create(i,j,FALSE,level);
    tml->next = ((pml) malloc(sizeof(mltype)));
    tml->next->m = Dungeon[level][i][j].creature;
    tml = tml->next;
  }
  if (Mlist[level]==NULL) {
    tml->next = NULL;
    Mlist[level] = head->next;
  }
  else {
    tml->next = Mlist[level];
    Mlist[level] = head->next;
  }
}

void wandercheck()
{
  int x,y;
  pml tml;
  if (random_range(NUMLEVELS) < Dlevel) {
    findspace(&x,&y);
    tml = ((pml) malloc(sizeof(mltype)));
    tml->next = Mlist[Dlevel];
    tml->m = Dungeon[Dlevel][x][y].creature = m_create(x,y,WANDERING,Dlevel);
    Mlist[Dlevel] = tml;    
  }
}

/* make and return an appropriate monster */
/* eventually to be more intelligent */
pmt m_create(x,y,kind,level)
int x,y,kind,level;
{
  pmt newmonster;
  int monster_range;

  /* dungeon level --> monster level */
  level = level/3 + 1;

  switch(level) {
    case 0:monster_range = ML1; break;
    case 1:monster_range = ML2; break;
    case 2:monster_range = ML3; break;
    case 3:monster_range = ML4; break;
    case 4:monster_range = ML5; break;
    case 5:monster_range = ML6; break;
    case 6:monster_range = ML7; break;
    case 7:monster_range = ML8; break;
    case 8:monster_range = ML9; break;
    case 9:monster_range = ML10; break;
    default:monster_range = NUMMONSTERS; break;
  }
  do 
    newmonster = make_creature(random_range(monster_range));
  while (Monsters[newmonster->id].uniqueness != 0); 
  /*only nonunique with this */
  if (kind == WANDERING) m_status_set(newmonster,WANDERING);
  newmonster->x = x;
  newmonster->y = y;
  return(newmonster);
}

/* make creature # mid */
pmt make_creature(mid)
int mid;
{
  pmt newmonster = ((pmt) malloc(sizeof(montype)));
  pob ob;
  int i,treasures;

  *newmonster = Monsters[mid];

  if (mid == ML0+4)
    make_log_npc(newmonster);
  else {
    if (newmonster->sleep < random_range(100))
      m_status_set(newmonster,AWAKE);
    treasures = random_range(newmonster->treasure);
    for(i=0;i<treasures;i++) {
      ob = ((pob) malloc(sizeof(objtype)));    
      do ob = ((pob) (create_object())); while (ob->uniqueness != 0);
      m_pickup(newmonster,ob);
    }
  }
  newmonster->click = (Tick + 1) % 50;
  return(newmonster);
}




/* drop treasures randomly onto level */
void stock_level(level)
int level;
{
  int i,j,k=0,numtreasures=2*random_range(Dlevel/4)+4;

  /* put cash anywhere, including walls, put other treasures only on floor */
  for (k=0;k<numtreasures;k++) {
    do {
      i = random_range(WIDTH);
      j = random_range(LENGTH);
    } while (Dungeon[level][i][j].locchar != FLOOR);
    Dungeon[level][i][j].things = ((pol) malloc(sizeof(oltype)));
    Dungeon[level][i][j].things->thing = ((pob) create_object());
    Dungeon[level][i][j].things->next = NULL;
    i = random_range(WIDTH);
    j = random_range(LENGTH);
    Dungeon[level][i][j].things = ((pol) malloc(sizeof(oltype)));
    Dungeon[level][i][j].things->thing = ((pob) malloc(sizeof(objtype)));
    make_cash(Dungeon[level][i][j].things->thing,level);
    Dungeon[level][i][j].things->next = NULL;
  }
}


/**************** SAVE AND RESTORE FUNCTIONS ***************/
/**************** SAVE FUNCTIONS */

/* writes integers to file using putc instead of fprintf */
void write_int(fd,d)
FILE *fd;
int d;
{
  int sign;
  if (d > 0) sign = 0;
  else {
    sign = 64;
    d = -d;
  }
  while (d > 63) {
    putc(((char) (d % 128)),fd);
    d = d/128;
  }
  putc(((char) (128+sign+d)),fd);
}



/* Checks to see if save file already exists.
   Checks to see if save file can be opened for write.
   Checks to see if gameid in file is same as gameid in omega.saves;
 if it isn't, only WIZARD can restore game. If it is, and
  DESTRUCTIVE_RESTORE is set, then that gameid is removed from
  omega.saves (done in gameidcheck()).
   After all, this, the player, the city level, and the player's
  current level are saved. All other levels will be regenerated.
*/
    
int save_game(savestr)
char *savestr;
{
  int writeok=TRUE,gameid;
  FILE *fd=fopen(savestr,"r");
  FILE *fd2;
  if (fd != NULL) {
    mprint("Overwrite old file?");
    writeok = (ynq() == 'y');
    fclose(fd);
  }
  if (writeok) {
    fd=fopen(savestr,"w");
    if (fd == NULL) {
      writeok = FALSE;
      mprint("Error opening file.");
    }
  }
  if (! writeok)
    mprint("Save aborted.");
  else {
    /* keep track of player saves, use a game id */
    strcpy(Str3,OMEGALIB);
    strcat(Str3,"omega.saves");
    fd2 = fopen(Str3,"a");
    gameid = random_range(32000)+random_range(32000);
    write_int(fd2,gameid);
    fclose(fd2);

    /* write the version number */
    fprintf(fd,"%d\n",VERSION);
    /* write game id to save file */
    fprintf(fd,"%d\n",gameid);
    save_player(fd);
    save_level(fd,0);
    save_level(fd,Dlevel);
    fclose(fd);
    mprint("Game Saved.");
    morewait();
  }
  return(writeok);
}


    

	



/* Look for incidence of game id in omega.saves, if it appears zero or one
times it is ok to restore. Twice and it is invalid to restore if not
implementor. If it is ok to restore, and DESTUCTIVE_RESTORE is #defined
to be true, the game id is appended enough times to make precisely two in 
the file. Note that this allows the file owner simply to delete it,
thus validating all saved games. */

int gameidcheck(gameid)
int gameid;
{
  FILE *fd;
  int i,id,found=0;

  strcpy(Str2,OMEGALIB);
  strcat(Str2,"omega.saves");
  fd = fopen(Str2,"r");
  if (fd != NULL) {
    /* note read_int returns -1 on EOF */
    while ((id = read_int(fd)) > -1)
      if (gameid == id) found++;
    fclose(fd);

    if (DESTRUCTIVE_RESTORE && found < 2) {
      fd = fopen(Str2,"a");
      for(i=0;i<2-found;i++)
	write_int(fd,gameid);
      return((found<2) || (strcmp(getlogin(),WIZARD)==0));
    }
  }
  else {
    mprint("No omega.saves file! But I'll restore your game anyhow.");
    return(TRUE);
  }
}
      
  


/* also saves some globals like Dlevel... */

void save_player(fd)
FILE *fd;
{
  int i;
  fprintf(fd,"%s\n",Player.name);
  fprintf(fd,"%s\n",Password);
  putc(Player.preference,fd);
  write_int(fd,Dlevel);
  write_int(fd,Wizard);
  write_int(fd,Cheated);
  write_int(fd,Time);
  write_int(fd,Tick);
  write_int(fd,Phase);
  write_int(fd,Date);
  write_int(fd,Gymcredit);
  write_int(fd,Balance);
  write_int(fd,Bank_Broken);
  write_int(fd,Deepest);
  write_int(fd,Clubmember);
  write_int(fd,Sawdruid);
  write_int(fd,Soldcondo);
  write_int(fd,Player.str);
  write_int(fd,Player.con);
  write_int(fd,Player.dex);
  write_int(fd,Player.agi);
  write_int(fd,Player.iq);
  write_int(fd,Player.pow);
  write_int(fd,Player.hp);
  write_int(fd,Player.maxstr);
  write_int(fd,Player.maxcon);
  write_int(fd,Player.maxdex);
  write_int(fd,Player.maxagi);
  write_int(fd,Player.maxiq);
  write_int(fd,Player.maxpow);
  write_int(fd,Player.maxhp);
  write_int(fd,Player.xp);
  write_int(fd,Player.x);
  write_int(fd,Player.y);
  write_int(fd,Player.click);
  write_int(fd,Player.level);
  write_int(fd,Player.vision);
  write_int(fd,Player.food);
  write_int(fd,abs(Player.alignment));
  putc(Player.alignment > 0 ? '+' : '-',fd);
  write_int(fd,Player.mana);
  write_int(fd,Player.cash);
  write_int(fd,Player.patron);
  write_int(fd,Player.sx);
  write_int(fd,Player.sy);
  write_int(fd,Player.slevel);
  write_int(fd,Player.options);
  for(i=0;i<NUMIMMUNITIES;i++)
    write_int(fd,Player.immunity[i]);
  for(i=0;i<NUMSTATI;i++)
    write_int(fd,Player.status[i]);
  for(i=0;i<NUMRANKS;i++)
    write_int(fd,Player.rank[i]);

  save_itemlist(fd,Condoitems);
  save_player_items(fd);
  save_player_item_knowledge(fd);
  save_player_spells(fd);
}


void save_player_items(fd)
FILE *fd;
{
  int i;
  for(i=0;i<MAXITEMS;i++) save_item(fd,Player.possessions[i]);
}


void save_player_item_knowledge(fd)
FILE *fd;
{
  int i;
  for (i=0;i<TOTALITEMS;i++)
    putc(((char)Objects[i].known),fd);
}

void save_player_spells(fd)
FILE *fd;
{
  int i;
  for (i=0;i<NUMSPELLS;i++){
    putc(((char)Spells[i].known),fd);
    putc(((char)Spells[i].powerdrain),fd);
  }
}


void save_level(fd,level)
FILE *fd;
int level;
{
  int i,j;

  write_int(fd,level);
  write_int(fd,Leveldata[level].numrooms);
  write_int(fd,Leveldata[level].tunnelled);  

  for(i=0;i<Leveldata[level].numrooms;i++) {
    write_int(fd,Leveldata[level].rooms[i-1].lighted);
    write_int(fd,Leveldata[level].rooms[i-1].rsi);
    write_int(fd,Leveldata[level].rooms[i-1].top);
    write_int(fd,Leveldata[level].rooms[i-1].bottom);
    write_int(fd,Leveldata[level].rooms[i-1].left);
    write_int(fd,Leveldata[level].rooms[i-1].right);
  }
  
  for(j=0;j<LENGTH;j++)
    for(i=0;i<WIDTH;i++) {
      if ((Dungeon[level][i][j].locchar == WALL) &&
	  (Dungeon[level][i][j].aux == level*2) &&
	  (Dungeon[level][i][j].seen == TRUE) &&
	  (Dungeon[level][i][j].showchar == WALL) &&
	  (Dungeon[level][i][j].p_locf == L_NO_OP) &&
	  (Dungeon[level][i][j].stopsrun == FALSE) &&
	  (Dungeon[level][i][j].secret == FALSE) &&
	  (Dungeon[level][i][j].roomnumber == RS_WALLSPACE))
	putc('2',fd);
      else if ((Dungeon[level][i][j].locchar == WALL) &&
	  (Dungeon[level][i][j].seen == FALSE) &&
	  (Dungeon[level][i][j].aux == level*2) &&
	  (Dungeon[level][i][j].showchar == SPACE) &&
	  (Dungeon[level][i][j].p_locf == L_NO_OP) &&
	  (Dungeon[level][i][j].stopsrun == FALSE) &&
	  (Dungeon[level][i][j].secret == FALSE) &&
	  (Dungeon[level][i][j].roomnumber == RS_WALLSPACE))
	putc('3',fd);
      else {
	putc(Dungeon[level][i][j].locchar,fd);
	putc(Dungeon[level][i][j].seen,fd);
	putc(Dungeon[level][i][j].showchar,fd);
	putc(Dungeon[level][i][j].p_locf,fd);
	putc(Dungeon[level][i][j].stopsrun,fd);
	putc(Dungeon[level][i][j].secret,fd);
	putc(Dungeon[level][i][j].roomnumber,fd);
	write_int(fd,Dungeon[level][i][j].aux);
      }
      save_itemlist(fd,Dungeon[level][i][j].things);
    }
  save_level_monsters(fd,level);
}
  

void save_level_monsters(fd,level)
FILE *fd;
int level;
{
  pml ml;
  int nummonsters = 0;
  
  for(ml=Mlist[level];ml!=NULL;ml=ml->next)nummonsters++;

  write_int(fd,nummonsters);

  for(ml=Mlist[level];ml!=NULL;ml=ml->next) {
    write_int(fd,ml->m->id);
    write_int(fd,ml->m->hp);
    write_int(fd,ml->m->status);
    write_int(fd,ml->m->x);
    write_int(fd,ml->m->y);
    write_int(fd,ml->m->click);
    write_int(fd,ml->m->attacked);
    write_int(fd,ml->m->talkf);
    write_int(fd,ml->m->movef);
    write_int(fd,ml->m->meleef);
    write_int(fd,ml->m->specialf);
    save_itemlist(fd,ml->m->possessions);
  }
}

void save_item(fd,o)
FILE *fd;
pob o;
{
  if (o != NULL) {
    putc('*',fd);
    write_int(fd,o->id);
    write_int(fd,o->plus);
    write_int(fd,o->used);
    write_int(fd,o->usef);
    write_int(fd,o->known);
    write_int(fd,o->charge);
    write_int(fd,o->blessing);
    write_int(fd,o->aux);
    write_int(fd,o->number);
  }
  else putc('#',fd);
}



void save_itemlist(fd,ol)
FILE *fd;
pol ol;
{
  while (ol != NULL) {
    putc('$',fd);
    save_item(fd,ol->thing);
    ol = ol->next;
  }
  putc('&',fd);
}






/************************ RESTORE FUNCTIONS *************************
/* reads integers from getc's */
/* explicitly returns -1 on EOF */
int read_int(fd)
FILE *fd;
{
  int d,e,mult=1;
  d=0;
  while (((e = getc(fd)) < 128) && (e != EOF)) {
    d = d+mult*e;	
    mult=mult*128;
  }
  if (e == EOF)
    return(-1);
  else {
   e = e-128;
   if (e > 63) return(-((e-64)*mult+d));
   else return(e*mult+d);
 }
}



/* read player data, city level, dungeon level,
   check on validity of save file, etc.
   return TRUE if game restored, FALSE otherwise */

int restore_game(savestr)
char *savestr;
{
  int i,version,gameid; 
  FILE *fd = fopen(savestr,"r");

  if (fd == NULL) {
    mprint("Error restoring game -- aborted.");
    return(FALSE);
  }
  else {
    mprint("Restoring....");

    fscanf(fd,"%d\n",&version);
    fscanf(fd,"%d\n",&gameid);
    if (VERSION != version) {
      mprint("Sorry, I can't restore an outdated save file!");
      return(FALSE);
    }
    else if (! gameidcheck(gameid)) 
      mprint("Sorry. Only the omega WIZARD can restore this file.");
    else {
      for(i=0;i<NUMLEVELS;i++)
	Leveldata[i].generated = FALSE;
      restore_player(fd);
      initmon0to3();
      initmon4to7();
      initmon8to10();
      restore_level(fd);
      restore_level(fd);
      mprint("Restoration complete");
      WhichScreen = -1;
      screencheck(Player.y);
      drawvision(Player.x,Player.y,Player.vision);
      dataprint();
      showflags();
      fclose(fd);
      return(TRUE);
    }
  }
}



void restore_player(fd)
FILE *fd;
{
  int i;
  filescanstring(fd,Player.name);
  filescanstring(fd,Password);
  Player.preference=((char) getc(fd));
  Dlevel = read_int(fd);
  Wizard =  read_int(fd);
  Cheated = read_int(fd);
  Time = read_int(fd);
  Tick = read_int(fd);
  Phase = read_int(fd);
  Date = read_int(fd);
  Gymcredit = read_int(fd);
  Balance = read_int(fd);
  Bank_Broken = read_int(fd);
  Deepest = read_int(fd);
  Clubmember = read_int(fd);
  Sawdruid = read_int(fd);
  Soldcondo = read_int(fd);
  Player.str = read_int(fd);
  Player.con = read_int(fd);
  Player.dex = read_int(fd);
  Player.agi = read_int(fd);
  Player.iq = read_int(fd);
  Player.pow = read_int(fd);
  Player.hp = read_int(fd);
  Player.maxstr = read_int(fd);
  Player.maxcon = read_int(fd);
  Player.maxdex = read_int(fd);
  Player.maxagi = read_int(fd);
  Player.maxiq = read_int(fd);
  Player.maxpow = read_int(fd);
  Player.maxhp = read_int(fd);
  Player.xp = read_int(fd);
  Player.x = read_int(fd);
  Player.y = read_int(fd);
  Player.click = read_int(fd);
  Player.level = read_int(fd);
  Player.vision = read_int(fd);
  Player.food = read_int(fd);
  Player.alignment = read_int(fd);
  Player.alignment *= (((char) getc(fd)) == '+' ? 1 : -1);
  Player.mana = read_int(fd);
  Player.maxmana = calcmana();
  Player.cash = read_int(fd);
  Player.patron = read_int(fd);
  Player.sx = read_int(fd);
  Player.sy = read_int(fd);
  Player.slevel = read_int(fd);
  Player.options = read_int(fd);
  for(i=0;i<NUMIMMUNITIES;i++)
    Player.immunity[i] = read_int(fd);
  for(i=0;i<NUMSTATI;i++)
    Player.status[i] = read_int(fd);
  for(i=0;i<NUMRANKS;i++)
    Player.rank[i] = read_int(fd);

  Condoitems = restore_itemlist(fd);
  restore_player_items(fd);
  restore_player_item_knowledge(fd);
  itemusecheck();
  restore_player_spells(fd);
  calc_melee();
}



void restore_player_items(fd)
FILE *fd;
{
  int i;
  Player.itemweight = 0;
  for(i=0;i<MAXITEMS;i++) {
    Player.possessions[i] = restore_item(fd);
    if (Player.possessions[i] != NULL)
      Player.itemweight += 
	Player.possessions[i]->number * Player.possessions[i]->weight;
  }
}




void itemusecheck()
{
  int i;
  
  for(i=0;i<MAXITEMS;i++)
    if (Player.possessions[i] != NULL)
      if (Player.possessions[i]->used)
      switch(Player.possessions[i]->objchar) {
      case ARMOR:
	Player.armor = Player.possessions[i];
	break;
      case WEAPON: case MISSILE:
	Player.primary = Player.possessions[i];
	break;
      case SHIELD:
	Player.secondary = Player.possessions[i];
	break;
      case CLOAK:
	Player.cloak = Player.possessions[i];
	break;
      case BOOTS:
	Player.boots = Player.possessions[i];
	break;
      }
}


void restore_player_item_knowledge(fd)
FILE *fd;
{
  int i;
  char c;
  for (i=0;i<TOTALITEMS;i++) {
    Objects[i].known =  getc(fd);
  }
}



void restore_player_spells(fd)
FILE *fd;
{
  int i;
  char c,d;
  for (i=0;i<NUMSPELLS;i++) {
    Spells[i].known =  getc(fd);
    Spells[i].powerdrain = getc(fd);
  }
}



void restore_level(fd)
FILE *fd;
{
  int i,j,level;
  char c,d;

  level = read_int(fd);
  Leveldata[level].numrooms = read_int(fd);
  Leveldata[level].tunnelled = read_int(fd);

  Leveldata[level].generated = TRUE;
  
  for(i=0;i<Leveldata[level].numrooms;i++) {
    Leveldata[level].rooms[i-1].lighted = read_int(fd);
    Leveldata[level].rooms[i-1].rsi = read_int(fd);
    Leveldata[level].rooms[i-1].top = read_int(fd);
    Leveldata[level].rooms[i-1].bottom = read_int(fd);
    Leveldata[level].rooms[i-1].left = read_int(fd);
    Leveldata[level].rooms[i-1].right = read_int(fd);
  }
  
  for(j=0;j<LENGTH;j++) 
    for(i=0;i<WIDTH;i++) {
      Dungeon[level][i][j].locchar = ((char) getc(fd));
      if (Dungeon[level][i][j].locchar == '2') {
	Dungeon[level][i][j].locchar = WALL;
	Dungeon[level][i][j].aux = level*2;
	Dungeon[level][i][j].seen = TRUE;
	Dungeon[level][i][j].showchar = WALL;
	Dungeon[level][i][j].p_locf = L_NO_OP;
	Dungeon[level][i][j].stopsrun = FALSE;
	Dungeon[level][i][j].secret = FALSE;
	Dungeon[level][i][j].roomnumber = RS_WALLSPACE;
      }
      else if (Dungeon[level][i][j].locchar == '3') {
	Dungeon[level][i][j].locchar = WALL;
	Dungeon[level][i][j].aux = level*2;
	Dungeon[level][i][j].seen = FALSE;
	Dungeon[level][i][j].showchar = SPACE;
	Dungeon[level][i][j].p_locf = L_NO_OP;
	Dungeon[level][i][j].stopsrun = FALSE;
	Dungeon[level][i][j].secret = FALSE;
	Dungeon[level][i][j].roomnumber = RS_WALLSPACE;
      }
      else {
	Dungeon[level][i][j].seen = ((char) getc(fd));
	Dungeon[level][i][j].showchar = ((char) getc(fd));
	Dungeon[level][i][j].p_locf = ((char) getc(fd));
	Dungeon[level][i][j].stopsrun = ((char) getc(fd));
	Dungeon[level][i][j].secret = ((char) getc(fd));
	Dungeon[level][i][j].roomnumber = ((char) getc(fd));
	Dungeon[level][i][j].aux = read_int(fd);
      }
      Dungeon[level][i][j].things = restore_itemlist(fd);
    }
  restore_level_monsters(fd,level);
}




void restore_level_monsters(fd,level)
FILE *fd;
int level;
{
  pml ml;
  pmt m;
  int i,nummonsters;
  char c;

  Mlist[level] = NULL;

  nummonsters = read_int(fd);
  
  for(i=0;i<nummonsters;i++) {
    ml = ((pml) malloc(sizeof(mltype)));
    m = ((pmt) malloc(sizeof(montype)));
    ml->m = m;
    ml->next = NULL;
    ml->m->id = read_int(fd);
    *(ml->m) = Monsters[ml->m->id];
    ml->m->hp = read_int(fd);
    ml->m->status = read_int(fd);
    ml->m->x = read_int(fd);
    ml->m->y = read_int(fd);
    ml->m->click = read_int(fd);
    ml->m->attacked = read_int(fd);
    ml->m->talkf = read_int(fd);
    ml->m->movef = read_int(fd);
    ml->m->meleef = read_int(fd);
    ml->m->specialf = read_int(fd);
    ml->m->possessions = restore_itemlist(fd);
    ml->next = Mlist[level];
    Mlist[level] = ml;
  }
}



pob restore_item(fd)
FILE *fd;
{
  pob o;
  char c;
  if (((char) getc(fd)) != '*') return(NULL);
  else {
    o = ((pob) malloc(sizeof(objtype)));
    o->id = read_int(fd);
    *o = Objects[o->id];
    o->plus = read_int(fd);
    o->used = read_int(fd);
    o->usef = read_int(fd);
    o->known = read_int(fd);
    o->charge = read_int(fd);
    o->blessing = read_int(fd);
    o->aux = read_int(fd);
    o->number = read_int(fd);
    if (o->id == CORPSEID) {
      strcpy(o->objstr,Monsters[o->aux].corpsestr);
      strcpy(o->truename,Monsters[o->aux].corpsestr);
      strcpy(o->cursestr,Monsters[o->aux].corpsestr);
    }
    return(o);
  }
}



pol restore_itemlist(fd)
FILE *fd;
{
  pol head=NULL;
  pol ol=NULL;

  while (((char) getc(fd)) == '$') {
    if (ol == NULL) {
      ol = ((pol) malloc(sizeof(oltype)));
      head = ol;
    }
    else {
      ol->next = ((pol) malloc(sizeof(oltype)));
      ol = ol->next;
    }
    ol->thing = restore_item(fd);
    ol->next = NULL;
  }
  return(head);
}


