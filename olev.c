/* omega copyright (c) 1987,1988 by Laurence Raphael Brothers */
/* olev.c */

#include "oglob.h"

/* Functions dealing with dungeon and country levels aside from actual
level structure generation */


/* monsters for tactical encounters */
void make_country_monsters(terrain)
char terrain;
{
  pml tml,ml=NULL;
  static int plains[10] = 
    {BUNNY,BUNNY,BLACKSNAKE,HAWK,IMPALA,WOLF,LION,BRIGAND,RANDOM};
  static int forest[10] =
   {BUNNY,QUAIL,HAWK,BADGER,DEER,DEER,WOLF,BEAR,BRIGAND,RANDOM};
  static int jungle[10] =
    {ANTEATER,PARROT,MAMBA,ANT,ANT,HYENA,HYENA,ELEPHANT,LION,RANDOM};
  static int river[10] =
    {QUAIL,TROUT,TROUT,MANOWAR,BASS,BASS,CROC,CROC,BRIGAND,RANDOM};
  static int swamp[10] =
    {BASS,BASS,CROC,CROC,BOGTHING,ANT,ANT,RANDOM,RANDOM,RANDOM};
  static int desert[10] =
    {HAWK,HAWK,CAMEL,CAMEL,HYENA,HYENA,LION,LION,RANDOM,RANDOM};
  static int tundra[10] =
    {WOLF,WOLF,BEAR,BEAR,DEER,DEER,RANDOM,RANDOM,RANDOM,RANDOM};
  static int mountain[10] =
    {BUNNY,SHEEP,WOLF,WOLF,HAWK,HAWK,HAWK,RANDOM,RANDOM,RANDOM};
  int *monsters,i,nummonsters;

  nummonsters = (random_range(5)+1) * (random_range(3)+1);

  switch(terrain) {
  case PLAINS: monsters = plains; break;
  case FOREST: monsters = forest; break;
  case JUNGLE: monsters = jungle; break;
  case RIVER: monsters = river; break;
  case SWAMP: monsters = swamp; break;
  case MOUNTAINS: case PASS: case VOLCANO: monsters = mountain; break;
  case DESERT: monsters = desert; break;
  case TUNDRA: monsters = tundra; break;
  default: monsters = NULL;
  }
  for(i=0;i<nummonsters;i++) {
    tml = ((pml) malloc(sizeof(mltype)));
    tml->m = ((pmt) malloc(sizeof(montype)));
    if (monsters == NULL) tml->m = 
      m_create(random_range(WIDTH),random_range(LENGTH),TRUE,difficulty());
    else {
      tml->m = make_creature(*(monsters+random_range(10)));
      tml->m->x = random_range(WIDTH);
      tml->m->y = random_range(LENGTH);
    }
    Level->site[tml->m->x][tml->m->y].creature = tml->m;
    tml->m->sense = WIDTH;
    if (m_statusp(tml->m,ONLYSWIM)) {
      Level->site[tml->m->x][tml->m->y].locchar = WATER;
      Level->site[tml->m->x][tml->m->y].p_locf = L_WATER;
    }

    tml->next = ml;
    ml = tml;
  }
  Level->mlist = ml;
}



/* monstertype is more or less Current_Dungeon */
/* The caves and sewers get harder as you penetrate them; the castle
is completely random, but also gets harder as it is explored;
the astral and the volcano just stay hard... */
void populate_level(monstertype)
int monstertype;
{
  pml head,tml;
  int i,j,k,monsterid,nummonsters=(random_range(difficulty()/3)+1)*3+8;

  if (monstertype == E_CASTLE) nummonsters += 10;
  else if (monstertype == E_ASTRAL) nummonsters += 10;
  else if (monstertype == E_VOLCANO) nummonsters += 20;

  head = tml = ((pml) malloc(sizeof(mltype)));

  for(k=0;k<nummonsters;k++) {

    findspace(&i,&j,-1);

    switch(monstertype) {
    case E_CAVES:
      if (Level->depth*10+random_range(100) > 150) 
	monsterid = ML3+7; /* Goblin Shaman*/
      else if (Level->depth*10+random_range(100) > 100) 
	monsterid = ML2+9; /* Goblin Chieftain */
      else if (random_range(100) > 50) monsterid = ML1+6; /* Goblin */
      else monsterid = -1; /* IE, random monster */
      break;
    case E_SEWERS:
      if (! random_range(3)) monsterid = -1;
      else switch(random_range(Level->depth+3)) {
      case 0: monsterid = ML1+3; break; /* sewer rat */
      case 1: monsterid = ML1+4; break; /* aggravator fungus */
      case 2: monsterid = ML1+5; break; /* blipper rat */
      case 3: monsterid = ML2+1; break; /* night gaunt */
      case 4: monsterid = ML2+5; break; /* transparent nasty */
      case 5: monsterid = ML2+8; break; /* murk fungus */
      case 6: monsterid = ML3+1; break; /* catoblepas */
      case 7: monsterid = ML3+3; break; /* acid cloud */
      case 8: monsterid = ML4+3; break; /* denebian slime devil */
      case 9: monsterid = ML4+8; break; /* giant crocodile */
      case 10: monsterid = ML5+1; break; /* tesla monster */
      case 11: monsterid = ML5+7; break; /* shadow spirit */
      case 12: monsterid = ML5+8; break; /* bogthing */
      case 13: monsterid = ML6+2; break; /* water elemental */
      case 14: monsterid = ML6+6; break; /* triton */
      case 15: monsterid = ML7+3; break; /* ROUS */
      default: monsterid = -1; break;
      }
      break;
    case E_ASTRAL:
      if (random_range(2)) /* random astral creatures */
	switch(random_range(12)) {
	case 0: monsterid = ML3+14; break; /* thought form */
	case 1: monsterid = ML4+11; break; /* astral fuzzy */
	case 2: monsterid = ML4+15; break; /* ban sidhe */
	case 3: monsterid = ML4+16; break; /* astral grue */
	case 4: monsterid = ML5+7; break; /* shadow spirit */
	case 5: monsterid = ML5+9; break; /* astral vampire */
	case 6: monsterid = ML5+11; break; /* manaburst */
	case 7: monsterid = ML6+9; break; /* rakshasa */
	case 8: monsterid = ML7+4; break; /* illusory fiend */
	case 9: monsterid = ML7+9; break; /* mirror master */
	case 10: monsterid = ML7+10; break; /* elder etheric grue */
	case 11: monsterid = ML8+8; break; /* shadow slayer */
	}
      else if (random_range(2) && (Level->depth == 1)) /* plane of earth */
	monsterid = ML6+3; /* earth elemental */
      else if (random_range(2) && (Level->depth == 2)) /* plane of air */
	monsterid = ML6+1;  /* air elemental */
      else if (random_range(2) && (Level->depth == 3)) /* plane of water */
	monsterid = ML6+2;  /* water elemental */
      else if (random_range(2) && (Level->depth == 4)) /* plane of fire */
	monsterid = ML6+0;  /* fire elemental */
      else if (random_range(2) && (Level->depth == 5)) /* deep astral */
	switch (random_range(12)) {
	case 0:monsterid = ML2+1; break; /* night gaunt */
	case 1:monsterid = ML4+12; break; /* servant of law */
	case 2:monsterid = ML4+13; break; /* servant of chaos */
	case 3:monsterid = ML5+4; break; /* lesser frost demon */
	case 4:monsterid = ML5+12; break; /* outer circle demon */
	case 5:monsterid = ML6+10; break; /* demon serpent */
	case 6:monsterid = ML6+11; break; /* angel */
	case 7:monsterid = ML7+14; break; /* inner circle demon */
	case 8:monsterid = ML8+5; break; /* frost demon lord */
	case 9:monsterid = ML8+11; break; /* high angel */
	case 10:monsterid = ML9+7; break; /* prime circle demon */
	case 11:monsterid = ML9+6; break; /* archangel */
	}
      else monsterid = -1;
      break;
    case E_VOLCANO:
      if (random_range(2)) {
	do monsterid = random_range(ML10-ML4)+ML4;
	while (Monsters[monsterid].uniqueness != COMMON);
      }
      else switch(random_range(Level->depth/2+2)) { /* evil & fire creatures */
      case 0: monsterid = ML4+5; break;
      case 1: monsterid = ML4+6; break;
      case 2: monsterid = ML5+0; break;
      case 3: monsterid = ML5+4; break;
      case 4: monsterid = ML5+5; break;
      case 5: monsterid = ML5+10; break;
      case 6: monsterid = ML6+0; break;
      case 7: monsterid = ML6+5; break;
      case 8: monsterid = ML6+9; break;
      case 9: monsterid = ML6+10; break;
      case 10: monsterid = ML7+1; break;
      case 11: monsterid = ML7+6; break;
      case 12: monsterid = ML7+11; break;
      case 13: monsterid = ML7+12; break;
      case 14: monsterid = ML7+14; break;
      case 15: monsterid = ML7+3; break; 
      case 16: monsterid = ML8+3; break; 
      case 17: monsterid = ML8+5; break; 
      case 18: monsterid = ML8+8; break; 
      case 19: monsterid = ML7+3; break; 
      case 20: monsterid = ML9+5; break; 
      case 21: monsterid = ML9+7; break; 
      default: monsterid = -1; break;
      }
      break;      
    case E_CASTLE: 
      if (random_range(4)==1) {
	if (difficulty() < 5)
	  monsterid = ML2+7;
	else if (difficulty() < 6)
	  monsterid = ML5+6;
	else if (difficulty() < 8)
	  monsterid = ML6+0;
	else monsterid = ML9+4;
      }
      else monsterid = -1;
    break;

    default: monsterid = -1; break;
    }
    
    if (monsterid > -1)
      Level->site[i][j].creature = make_creature(monsterid);
    else Level->site[i][j].creature = m_create(i,j,TRUE,difficulty());


    Level->site[i][j].creature->x = i;
    Level->site[i][j].creature->y = j;
    
    if (m_statusp(Level->site[i][j].creature,ONLYSWIM)) {
      Level->site[i][j].locchar = WATER;
      Level->site[i][j].p_locf = L_WATER;
    }
    
    tml->next = ((pml) malloc(sizeof(mltype)));
    tml->next->m = Level->site[i][j].creature;
    tml = tml->next;
  }
  
  if (Level->mlist==NULL) {
    tml->next = NULL;
    Level->mlist = head->next;
  }
  else {
    tml->next = Level->mlist;
    Level->mlist = head->next;
  }
}



/* Add a wandering monster possibly */
void wandercheck()
{
  int x,y;
  pml tml;
  if (random_range(MaxDungeonLevels) < difficulty()) {
    findspace(&x,&y,-1);
    tml = ((pml) malloc(sizeof(mltype)));
    tml->next = Level->mlist;
    tml->m = Level->site[x][y].creature = m_create(x,y,WANDERING,difficulty());
    Level->mlist = tml;    
  }
}



/* call make_creature and place created monster on Level->mlist and Level */
void make_site_monster(i,j,mid)
int i,j,mid;
{
  pml ml = ((pml) malloc(sizeof(mltype)));
  pmt m;
  if (mid > -1)  Level->site[i][j].creature = (m = make_creature(mid));
  else Level->site[i][j].creature = (m = m_create(i,j,WANDERING,difficulty()));
  m->x = i;
  m->y = j;
  ml->m = m;
  ml->next = Level->mlist;
  Level->mlist = ml;
}


/* make and return an appropriate monster for the level and depth*/
/* called by populate_level, doesn't actually add to mlist for some reason*/
/* eventually to be more intelligent */
pmt m_create(x,y,kind,level)
int x,y,kind,level;
{
  pmt newmonster;
  int monster_range;
  int mid;

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
    mid = random_range(monster_range);
  while (Monsters[mid].uniqueness != COMMON); 
  newmonster = make_creature(mid);

  /* no duplicates of unique monsters */
  if (kind == WANDERING) m_status_set(newmonster,WANDERING);
  newmonster->x = x;
  newmonster->y = y;
  return(newmonster);
}



/* make creature # mid, totally random if mid == -1 */
/* make creature allocates space for the creature */
pmt make_creature(mid)
int mid;
{
  pmt newmonster = ((pmt) malloc(sizeof(montype)));
  pob ob;
  int i,treasures;

  if (mid == -1) mid = random_range(ML9);
  *newmonster = Monsters[mid];
  if ((mid == ML6+11) || (mid == ML8+11) || (mid == ML9+6)) {
    /* aux1 field of an angel is its deity */
    newmonster->aux1 = random_range(6)+1;
    newmonster->corpsestr = salloc(Monsters[mid].corpsestr);
    strcpy(Str3,Monsters[mid].monstring);
    switch(newmonster->aux1) {
    case ODIN: strcat(Str3," of Odin"); break;
    case SET: strcat(Str3," of Set"); break;
    case HECATE: strcat(Str3," of Hecate"); break;
    case ATHENA: strcat(Str3," of Athena"); break;
    case DESTINY: strcat(Str3," of Destiny"); break;
    case DRUID: strcat(Str3," of the Balance"); break;
    }
    newmonster->monstring = salloc(Str3);
  }
  else if (mid == ML0+7) { 
    /* generic 0th level human */
    newmonster->monstring = salloc(mantype());
    strcpy(Str1,"dead ");
    strcat(Str1,newmonster->monstring);
    newmonster->corpsestr = salloc(Str1);
  }
  else if (newmonster->monchar == '!') {
    /* the nymph/satyr and incubus/succubus */
    if (Player.preference == 'f') {
      newmonster->monchar = 'n';
      newmonster->monstring = salloc("nymph");
      newmonster->corpsestr = salloc("dead nymph");
    }
    else {
      newmonster->monchar = 's';
      newmonster->monstring = salloc("satyr");
      newmonster->corpsestr = salloc("dead satyr");
    }
    if (newmonster->id == ML4+6) {
      if (Player.preference == 'f') 
	newmonster->corpsestr = salloc("dead succubus");
      else newmonster->corpsestr = salloc("dead incubus");
    }
  }
  if (mid == NPC)
    make_log_npc(newmonster);
  else {
    if (newmonster->sleep < random_range(100))
      m_status_set(newmonster,AWAKE);
    if (newmonster->startthing > -1) {
      ob = ((pob) malloc(sizeof(objtype)));    
      *ob = Objects[newmonster->startthing];
      m_pickup(newmonster,ob);
    }
    treasures = random_range(newmonster->treasure);
    for(i=0;i<treasures;i++) {
      ob = ((pob) malloc(sizeof(objtype)));    
      do ob = ((pob) (create_object(newmonster->level)));
      while (ob->uniqueness != 0);
      m_pickup(newmonster,ob);
    }
  }
  newmonster->click = (Tick + 1) % 50;
  return(newmonster);
}




/* drop treasures randomly onto level */
void stock_level()
{
  int i,j,k,numtreasures=2*random_range(difficulty()/4)+4;

  /* put cash anywhere, including walls, put other treasures only on floor */
  for (k=0;k<numtreasures+10;k++) {
    do {
      i = random_range(WIDTH);
      j = random_range(LENGTH);
    } while (Level->site[i][j].locchar != FLOOR);
    make_site_treasure(i,j,difficulty());
    i = random_range(WIDTH);
    j = random_range(LENGTH);
    Level->site[i][j].things = ((pol) malloc(sizeof(oltype)));
    Level->site[i][j].things->thing = ((pob) malloc(sizeof(objtype)));
    make_cash(Level->site[i][j].things->thing,difficulty());
    Level->site[i][j].things->next = NULL;
    /* caves have more random cash strewn around */
    if (Current_Dungeon == E_CAVES) {
      i = random_range(WIDTH);
      j = random_range(LENGTH);
      Level->site[i][j].things = ((pol) malloc(sizeof(oltype)));
      Level->site[i][j].things->thing = ((pob) malloc(sizeof(objtype)));
      make_cash(Level->site[i][j].things->thing,difficulty());
      Level->site[i][j].things->next = NULL;
      i = random_range(WIDTH);
      j = random_range(LENGTH);
      Level->site[i][j].things = ((pol) malloc(sizeof(oltype)));
      Level->site[i][j].things->thing = ((pob) malloc(sizeof(objtype)));
      make_cash(Level->site[i][j].things->thing,difficulty());
      Level->site[i][j].things->next = NULL;
    }
  }
}


/* make a new object (of at most level itemlevel) at site i,j on level*/
void make_site_treasure(i,j,itemlevel)
int i,j,itemlevel;
{
  pol tmp = ((pol) malloc(sizeof(oltype)));
  tmp->thing = ((pob) create_object(itemlevel));
  tmp->next = Level->site[i][j].things;
  Level->site[i][j].things = tmp;
}

/* make a specific new object at site i,j on level*/
void make_specific_treasure(i,j,itemid)
int i,j,itemid;
{
  pol tmp = ((pol) malloc(sizeof(oltype)));
  tmp->thing = ((pob) malloc(sizeof(objtype)));
  *(tmp->thing) = Objects[itemid];
  tmp->next = Level->site[i][j].things;
  Level->site[i][j].things = tmp;
}



/* returns a "level of difficulty" based on current environment
   and depth in dungeon. Is somewhat arbitrary. value between 1 and 10.
   May not actually represent real difficulty, but instead level
   of items, monsters encountered.    */
int difficulty()
{
  int depth = 1;
  if (Level != NULL) depth = Level->depth;
  switch(Current_Environment) {
  case E_COUNTRYSIDE: return(7); break;
  case E_CITY: return(3); break;
  case E_VILLAGE: return(1); break;
  case E_TACTICAL_MAP: return(7); break;
  case E_SEWERS: return(depth/6)+3;break;
  case E_CASTLE: return(depth/4)+4;break;
  case E_CAVES: return(depth/3)+1;break;
  case E_VOLCANO: return(depth/4)+5;break;
  case E_ASTRAL: return(8);break;
  case E_ARENA: return(5);break;
  case E_HOVEL: return(3);break;
  case E_MANSION: return(7);break;
  case E_HOUSE: return(5);break;
  case E_DLAIR: return(9);break;
  case E_ABYSS: return(10);break;
  case E_STARPEAK: return(9);break;
  case E_CIRCLE: return(8);break;
  case E_MAGIC_ISLE: return(8);break;
  case E_TEMPLE: return(8);break;
  default: return(3); break;
  }
}
