/* omega copyright (C) by Laurence Raphael Brothers, 1987,1988 */

/* oaux1.c */

/* auxiliary functions for those in ocom.c, also see oaux2.c 
   and oaux3.c */ 

#include "oglob.h"



/* check to see if too much tunneling has been done in this level */
void tunnelcheck()
{
  Level->tunnelled++;
  if ((Level->tunnelled) > LENGTH/4)
    mprint("Dust and stone fragments fall on you from overhead.");
  if ((Level->tunnelled) > LENGTH/2)
    mprint("You hear groaning and creaking noises.");
  if ((Level->tunnelled) > 3*LENGTH/4) 
    mprint("The floor trembles and you hear a loud grinding screech.");
  if ((Level->tunnelled) > LENGTH) {
    mprint("With a scream of tortured stone, the entire level caves in!!!");
    gain_experience(5000);
    mprint("You are flattened into an unpleasant jellylike substance.");
    p_death("dungeon cave-in");
  }
}

/* displays a room's name */
void showroom(i)
int i;
{
  strcpy(Str1,"");
  strcpy(Str2,"");
  switch(Current_Environment) {
  case E_MANSION:
    strcpy(Str2,"A luxurious mansion: ");
    break;
  case E_HOUSE:
    strcpy(Str2,"A house:");
    break;
  case E_HOVEL:
    strcpy(Str2,"A hovel:");
    break;
  case E_CITY:
    strcpy(Str2,"The City of Rampart");
    break;
  case E_VILLAGE:
    switch(Villagenum) {
    case 1: strcpy(Str2,"The Village of Star View"); break;
    case 2: strcpy(Str2,"The Village of Woodmere"); break;
    case 3: strcpy(Str2,"The Village of Stormwatch"); break;
    case 4: strcpy(Str2,"The Village of Thaumaris"); break;
    case 5: strcpy(Str2,"The Village of Skorch"); break;
    case 6: strcpy(Str2,"The Village of Whorfen"); break;
    }
    break;
  case E_CAVES:
    strcpy(Str2,"The Goblin Caves: ");
    break;
  case E_CASTLE:
    strcpy(Str2,"The Archmage's Castle: ");
    break;
  case E_ASTRAL:
    strcpy(Str2,"The Astral Plane: ");
    break;
  case E_VOLCANO:
    strcpy(Str2,"The Volcano: ");
    break;
  case E_SEWERS:
    strcpy(Str2,"The Sewers: ");
    break;
  case E_TACTICAL_MAP:
    strcpy(Str2,"The Tactical Map ");
    break;
  }
  if ((Current_Environment == E_MANSION) ||
      (Current_Environment == E_HOUSE)) 
    strcpy(Str1,roomname(i));
  else if (Current_Environment == Current_Dungeon) {
    strcpy(Str1,"Level ");
    if (Level->depth < 10) {
      Str1[6] = Level->depth + '0';
      Str1[7] = 0;
    }
    else {
      Str1[6] = (Level->depth / 10) + '0';
      Str1[7] = (Level->depth % 10) + '0';
      Str1[8] = 0;
    }
    strcat(Str1," (");
    strcat(Str1,roomname(i));
    strcat(Str1,")");
  }
  strcat(Str2,Str1);
  locprint(Str2);
}


int player_on_sanctuary()
{
  if ((Player.x==Player.sx) &&
      (Player.y==Player.sy))
    return(TRUE);
  else {
    if (Player.patron) {
      if ((Level->site[Player.x][Player.y].locchar == ALTAR) &&
	  (Level->site[Player.x][Player.y].aux == Player.patron))
	return(TRUE);
      else return(FALSE);
    }
    else return(FALSE);
  }
}


/* check a move attempt, maybe attack something, return TRUE if ok to move. */
/* x y is the proposed place to move to */
int p_moveable(x,y)
int x,y;
{
  setgamestatus(SKIP_MONSTERS);
  if (! inbounds(x,y)) return (FALSE);
  else if (Player.status[SHADOWFORM]) {
    resetgamestatus(SKIP_MONSTERS);
    return(TRUE);
  }
  else if (loc_statusp(x,y,SECRET)) {
    if (!gamestatusp(FAST_MOVE)) print3("Ouch!");
    return(FALSE);
  }
  else if (Level->site[x][y].creature != NULL) {
    if (! gamestatusp(FAST_MOVE)) {
      fight_monster(Level->site[x][y].creature);
      resetgamestatus(SKIP_MONSTERS);
      return(FALSE);
    }
    else return(FALSE);
  }
  else if ((Level->site[x][y].locchar == WALL) ||
	   (Level->site[x][y].locchar == STATUE) ||
	   (Level->site[x][y].locchar == PORTCULLIS) ||
	   (Level->site[x][y].locchar == CLOSED_DOOR) ||
	   (gamestatusp(FAST_MOVE) &&
	    ((Level->site[x][y].locchar == HEDGE) ||
	     (Level->site[x][y].locchar == LAVA) ||
	     (Level->site[x][y].locchar == ABYSS) ||
	     (Level->site[x][y].locchar == FIRE) ||
	     (Level->site[x][y].locchar == WATER) ||
	     (Level->site[x][y].locchar == LIFT) ||
	     (Level->site[x][y].locchar == TRAP)))) {
    if (! gamestatusp(FAST_MOVE)) print3("Ouch!");
    return(FALSE);
  }
  else if (optionp(CONFIRM)) {
    if ((Level->site[x][y].locchar == HEDGE) ||
	(Level->site[x][y].locchar == LAVA) ||
	(Level->site[x][y].locchar == FIRE) ||
	(Level->site[x][y].locchar == ABYSS) ||
	(Level->site[x][y].locchar == WATER) ||
	(Level->site[x][y].locchar == RUBBLE) ||
	(Level->site[x][y].locchar == LIFT) ||
	(Level->site[x][y].locchar == TRAP)) {
      /* horses WILL go into water... */
      if (gamestatusp(MOUNTED)) {
	if (Level->site[x][y].locchar != WATER) {
	  print1("You can't convince your steed to continue.");
	  setgamestatus(SKIP_MONSTERS);
	  return(FALSE);
	}
	else return(TRUE);
      }
      else if (confirmation()) resetgamestatus(SKIP_MONSTERS);
      else setgamestatus(SKIP_MONSTERS);
      return(!gamestatusp(SKIP_MONSTERS));
    }
    else {
      resetgamestatus(SKIP_MONSTERS);
      return(TRUE);
    }
  }
  else {
    resetgamestatus(SKIP_MONSTERS);
    return(TRUE);
  }
}



/* check a move attempt in the countryside */
int p_country_moveable(x,y)
int x,y;
{
  if (! inbounds(x,y)) return (FALSE);
  else if (optionp(CONFIRM)) {
    if ((Country[x][y].current_terrain_type == CHAOS_SEA) ||
	(Country[x][y].current_terrain_type == MOUNTAINS))
      return(confirmation());
    else return(TRUE);
  }
  else return(TRUE);
}





/* search once particular spot */
void searchat(x,y)
int x,y;
{
  int i;
  if (inbounds(x,y) && (random_range(3) || Player.status[ALERT])) {
    if (loc_statusp(x,y,SECRET)) {
      lreset(x,y,SECRET);
      if ((Level->site[x][y].locchar==OPEN_DOOR) ||
	  (Level->site[x][y].locchar==CLOSED_DOOR))
	mprint("You find a secret door!");
      else mprint("You find a secret passage!");
      for(i=0;i<8;i++)
	lset(x+Dirs[0][i],y+Dirs[1][i],STOPS);
      drawvision(Player.x,Player.y);
    }
    if ((Level->site[x][y].p_locf >= TRAP_BASE) &&
	(Level->site[x][y].locchar != TRAP) &&
	(Level->site[x][y].p_locf <= TRAP_BASE+NUMTRAPS)) {
      Level->site[x][y].locchar = TRAP;
      mprint("You find a trap!");
      drawvision(Player.x,Player.y);
      resetgamestatus(FAST_MOVE);
    }
  }
}



/* This is to be called whenever anything might change player performance in
   melee, such as changing weapon, statistics, etc. */
void calc_melee()
{
  calc_weight();

  Player.maxweight = (Player.str * Player.agi * 10);
  Player.absorption = Player.status[PROTECTION];
  Player.defense = 2 * statmod(Player.agi)+(Player.level/2);
  Player.hit = Player.level + statmod(Player.dex)+1;
  Player.dmg = statmod(Player.str)+3;
  Player.speed = 5 - min(4,(statmod(Player.agi)/2));
  if (Player.status[HASTED] > 0) Player.speed = Player.speed / 2;
  if (Player.status[SLOWED] > 0) Player.speed = Player.speed * 2;
  if (Player.itemweight > 0)
    switch(Player.maxweight / Player.itemweight) {
    case 0: Player.speed+=6; break;
    case 1: Player.speed+=3; break;
    case 2: Player.speed+=2; break;
    case 3: Player.speed+=1; break;
    }
  
  if (Player.status[ACCURATE]) Player.hit+=20;
  if (Player.status[HERO]) Player.hit+=Player.dex;
  if (Player.status[HERO]) Player.dmg+=Player.str;
  if (Player.status[HERO]) Player.defense+=Player.agi;
  if (Player.status[HERO]) Player.speed=Player.speed / 2;

  Player.speed = max(1,min(25,Player.speed));

  if (gamestatusp(MOUNTED)) {
    Player.speed = 3;
    Player.hit += 10;
    Player.dmg += 10;
  }

  /* weapon */
  /* have to check for used since it could be a 2h weapon just carried
     in one hand */
  if (Player.possessions[O_WEAPON_HAND] != NULL) 
    if (Player.possessions[O_WEAPON_HAND]->used &&
	((Player.possessions[O_WEAPON_HAND]->objchar==WEAPON)||
	 (Player.possessions[O_WEAPON_HAND]->objchar==MISSILEWEAPON))) {
      Player.hit += 
	Player.possessions[O_WEAPON_HAND]->hit + 
	  Player.possessions[O_WEAPON_HAND]->plus;
      Player.dmg += 
	Player.possessions[O_WEAPON_HAND]->dmg + 
	  Player.possessions[O_WEAPON_HAND]->plus;
    }
  
  /* shield or defensive weapon */
  if (Player.possessions[O_SHIELD] != NULL) {
    Player.defense += 
      Player.possessions[O_SHIELD]->aux + 
	Player.possessions[O_SHIELD]->plus;
  }
  
  /* armor */
  if (Player.possessions[O_ARMOR] != NULL) {
    Player.absorption += Player.possessions[O_ARMOR]->dmg;
    Player.defense += 
      Player.possessions[O_ARMOR]->plus - 
	Player.possessions[O_ARMOR]->aux;
  }

  if (strlen(Player.meleestr) > 2*maneuvers())
    default_maneuvers();
  comwinprint();
  showflags();
  dataprint();
}


/* player attacks monster m */
void fight_monster(m)
struct monster *m;
{
  int hitmod = 0;
  int reallyfight = TRUE;

  if (Player.status[AFRAID]) {
    print3("You are much too afraid to fight!");
    reallyfight = FALSE;
  }
  else if (player_on_sanctuary()) {
    print3("You restrain yourself from desecrating this holy place.");
    reallyfight = FALSE;
  }
  else if (Player.status[SHADOWFORM]) {
    print3("Your attack has no effect in your shadowy state.");
    reallyfight = FALSE;
  }
  else if ((Player.status[BERSERK]<1) && (! m_statusp(m,HOSTILE))) {
    if (optionp(BELLICOSE)) reallyfight = TRUE;
    else reallyfight = confirmation();
  }
  else reallyfight = TRUE;
  
  if (reallyfight) {

    if (Lunarity == 1) hitmod += Player.level;
    else if (Lunarity == -1) hitmod -= (Player.level / 2);

    if (! m->attacked) Player.alignment -= 2; /* chaotic action */
    m_status_set(m,AWAKE);
    m_status_set(m,HOSTILE);
    m->attacked = TRUE;
    Player.hit += hitmod;
    tacplayer(m);
    Player.hit -= hitmod;
  }
}




/* Attempt to break an object o */
void damage_item(o)
pob o;     
{
  int i;
  /* special case -- break star gem */
  if (o->id == ARTIFACTID+21) {
    print1("The Star Gem shatters into a million glistening shards....");
    if (Current_Environment == E_STARPEAK) {
      if (! gamestatusp(KILLED_LAWBRINGER))
	print2("You hear an agonizing scream of anguish and despair.");
      morewait();
      print1("A raging torrent of energy escapes in an explosion of magic!");
      print2("The energy flows to the apex of Star Peak where there is");
      morewait();
      clearmsg();
      print1("an enormous explosion!");
      morewait();
      annihilate(1);
      print3("You seem to gain strength in the chaotic glare of magic!");
      Player.str = Player.maxstr + 5;
      Player.pow = Player.maxpow + 5;
      Player.alignment -= 200;
      dispose_lost_objects(1,o);
    }
    else {
      morewait();
      print1("The shards coalesce back together again, and vanish");
      print2("with a muted giggle.");
      dispose_lost_objects(1,o);
    }
  }
  else {
    if (o->fragility < random_range(30)) {
      if (o->objchar == STICK) {
	strcpy(Str1,"Your ");
	strcat(Str1,(o->blessing >= 0 ? o->truename : o->cursestr));
	strcat(Str1," explodes!");
	print1(Str1);
	morewait();
	if (o->charge < 1)
	  nprint1(" Fzzz... Out of Power... Oh well...");
	else {
	  nprint1(" Ka-Blamm!!!");
	  /* general case. Some sticks will eventually do special things */
	  morewait();
	for(i=0;i<9;i++)
	  manastorm(Player.x+Dirs[0][i],
		    Player.y+Dirs[1][i],
		    o->charge*o->level*10);
	  dispose_lost_objects(1,o);
	}
      }
      else if ((o->blessing > 0) && (o->level >= random_range(10))) {
	strcpy(Str1,"Your ");
	strcat(Str1,itemid(o));
	strcat(Str1," glows strongly.");
	print1(Str1);
      }
      else if ((o->blessing < -1) && (o->level >= random_range(10))) {
	strcpy(Str1,"You hear an evil giggle from your ");
	strcat(Str1,itemid(o));
	print1(Str1);
      }
      else if (o->plus > 0) {
	strcpy(Str1,"Your ");
	strcat(Str1,itemid(o));
	strcat(Str1," glows and then fades.");
	print1(Str1);
	o->plus--;
      }
      else {
	if (o->blessing > 0) print1("You hear a faint despairing cry!");
	else if (o->blessing < 0) print1("You hear an agonized scream!");
	strcpy(Str1,"Your ");
	strcat(Str1,itemid(o));
	strcat(Str1," shatters in a thousand lost fragments!");
	print2(Str1);
	dispose_lost_objects(1,o);
      }
    }
  }
}
      
      





/* do dmg points of damage of type dtype, from source fromstring */
void p_damage(dmg,dtype,fromstring)
int dmg,dtype;
char *fromstring;
{
  if (! p_immune(dtype)) {
    if (gamestatusp(FAST_MOVE)) {
      drawvision(Player.x,Player.y);
      resetgamestatus(FAST_MOVE);
    }
    if (dtype == NORMAL_DAMAGE) Player.hp -= max(1,(dmg-Player.absorption));
    else Player.hp -= dmg;
    if (Player.hp < 1) p_death(fromstring);
  }
  else mprint("You resist the effects!");
  dataprint();
}

/* game over, you lose! */
void p_death(fromstring)
char *fromstring;
{
  Player.hp = -1;
  print3("You died!");
  morewait();
  display_death(fromstring);
  endgraf();
  exit(0);
}


/* move the cursor around, like for firing a wand, sets x and y to target */
void setspot(x,y)
int *x,*y;
{
  char c = ' ';
  mprint("Targeting.... ? for help");
  showcursor(*x,*y);
  while ((c != '.') && (c != ESCAPE)) {
    c = lgetc();
    switch(c) {
      case 'h':case '4': movecursor(x,y,-1,0);  break;
      case 'j':case '2': movecursor(x,y,0,1);  break;
      case 'k':case '8': movecursor(x,y,0,-1);  break;
      case 'l':case '6': movecursor(x,y,1,0);  break;
      case 'b':case '1': movecursor(x,y,-1,1);  break;
      case 'n':case '3': movecursor(x,y,1,1);  break;
      case 'y':case '7': movecursor(x,y,-1,-1);  break;
      case 'u':case '9': movecursor(x,y,1,-1);  break;
      case '?':
	clearmsg();
	mprint("Use vi keys or numeric keypad to move cursor to target.");
	mprint("Hit the '.' key when done, or ESCAPE to abort.");
	break;
    }
  }
  if (c==ESCAPE) *x = *y= ABORT;
  screencheck(Player.y);
}


/* get a direction: return index into Dirs array corresponding to direction */
int getdir()
{
  int ok = TRUE;
  do {
    mprint("Select direction [hjklyubn, ESCAPE to quit]: ");
    switch (mgetc()) {
    case '4':
    case 'h':
    case 'H': return(5); break;
    case '2':
    case 'j':
    case 'J': return(6); break;
    case '8':
    case 'k':
    case 'K': return(7); break;
    case '6':
    case 'l':
    case 'L': return(4); break;
    case '7':
    case 'y':
    case 'Y': return(3); break;
    case '9':
    case 'u':
    case 'U': return(1); break;
    case '1':
    case 'b':
    case 'B': return(2); break;
    case '3':
    case 'n':
    case 'N': return(0); break;
    case ESCAPE: return(ABORT); break;
    default: print3("That's not a direction! "); ok = FALSE; break;
    }
  } while (! ok);
}




/* bitwise access functions to Player.options */

int optionp(o)
int o;
{
  return(Player.options&o);
}

void optionset(o)
int o;
{
  Player.options |= o;
}

void optionreset(o)
int o;
{
  Player.options &= ~o;
}



/* functions describes monster m's state for examine function */      
char *mstatus_string(m)
struct monster *m;
{
  if (m->uniqueness == COMMON) {
    if (m->hp < Monsters[m->id].hp / 3)
      strcpy(Str2,"a grievously injured ");
    else if (m->hp < Monsters[m->id].hp / 2)
      strcpy(Str2,"a severely injured ");
    else if (m->hp < Monsters[m->id].hp)
      strcpy(Str2,"an injured ");
    else strcpy(Str2,getarticle(m->monstring));
    if (m->level > Monsters[m->id].level) {
      strcat(Str2," (level ");
      strcat(Str2,wordnum(m->level+1-Monsters[m->id].level));
      strcat(Str2,") ");
    }
    strcat(Str2,m->monstring);
  }
  else {
    strcpy(Str2,m->monstring);
    if (m->hp < Monsters[m->id].hp / 3)
      strcat(Str2," who is grievously injured ");
    else if (m->hp < Monsters[m->id].hp / 2)
      strcat(Str2," who is severely injured ");
    else if (m->hp < Monsters[m->id].hp)
      strcat(Str2," who is injured ");
  }
  return(Str2);
}




/* for the examine function */
void describe_player()
{
  if (Player.hp < (Player.maxhp /5))
    print1("A grievously injured ");
  else if (Player.hp < (Player.maxhp /2))
    print1("A seriously wounded ");
  else if (Player.hp < Player.maxhp)
    print1("A somewhat bruised ");
  else print1("A fit ");

  nprint1(levelname(Player.level));
  nprint1(" named ");
  nprint1(Player.name);
}


/* access to player experience... */
/* share out experience among guild memberships */
void gain_experience(amount)
int amount;
{
  int i,count=0,share;
  Player.xp += amount;
  gain_level(); /* actually, check to see if should gain level */
  for(i=0;i<NUMRANKS;i++)
    if (Player.guildxp[i] > 0) count++;
  share = amount/(max(count,1));
  for(i=0;i<NUMRANKS;i++)
    if (Player.guildxp[i] > 0) Player.guildxp[i]+=share;
}


/* try to hit a monster in an adjacent space. If there are none
   return FALSE. Note if you're berserk you get to attack ALL
   adjacent monsters! */
int goberserk()
{
  int wentberserk=FALSE,i;
  char meleestr[80];
  strcpy(meleestr,Player.meleestr);
  strcpy(Player.meleestr,"ALACAH");
  for(i=0;i<8;i++)
    if (Level->site
	[Player.x+Dirs[0][i]]
	[Player.y+Dirs[1][i]]
	.creature
	!= NULL) {
      wentberserk=TRUE;
      fight_monster(Level->site
		    [Player.x+Dirs[0][i]]
		    [Player.y+Dirs[1][i]]
		    .creature);
      morewait();
    }
  strcpy(Player.meleestr,meleestr);
  return(wentberserk);
}

/* identifies a trap for examine() by its aux value */
char *trapid(trapno)
int trapno;
{
  switch (trapno) {
  case L_TRAP_SIREN:return("A siren trap");break;
  case L_TRAP_DART:return("A dart trap");break;
  case L_TRAP_PIT:return("A pit");break;
  case L_TRAP_SNARE:return("A snare");break;
  case L_TRAP_BLADE:return("A blade trap");break;
  case L_TRAP_FIRE:return("A fire trap");break;
  case L_TRAP_TELEPORT:return("A teleport trap");break;
  case L_TRAP_DISINTEGRATE:return("A disintegration trap");break;
  case L_TRAP_DOOR:return("A trap door");break;
  case L_TRAP_MANADRAIN:return("A manadrain trap");break;
  case L_TRAP_ACID:return("An acid shower trap");break;
  case L_TRAP_SLEEP_GAS:return("A sleep gas trap");break;
  case L_TRAP_ABYSS:return("A concealed entrance to the abyss");break;
  default: return("A completely inoperative trap.");break;
  }
}


/* checks current food status of player, every hour, and when food is eaten */
void foodcheck()
{
  if (Player.food > 48) {
    print3("You vomit up your huge meal.");
    Player.food = 12;
  }
  else if (Player.food == 30) 
    print3("Time for a smackerel of something.");
  else if (Player.food == 20) 
    print3("You feel hungry.");
  else if (Player.food == 12) 
    print3("You are ravenously hungry.");
  else if (Player.food == 3) {
    print3("You feel weak.");
    if (gamestatusp(FAST_MOVE)) {
      drawvision(Player.x,Player.y);
      resetgamestatus(FAST_MOVE);
    }
  }
  else if (Player.food < 0) {
    if (gamestatusp(FAST_MOVE)) {
      drawvision(Player.x,Player.y);
      resetgamestatus(FAST_MOVE);
    }
    print3("You're starving!");
    p_damage(-5*Player.food,UNSTOPPABLE,"starvation");
  }
  showflags();
}




/* see whether room should be illuminated */
void roomcheck()
{
  static int oldroomno = -1;
  static plv oldlevel = NULL;
  int roomno = Level->site[Player.x][Player.y].roomnumber;

  if ((roomno == RS_CAVERN) ||
      (roomno == RS_SEWER_DUCT) || 
      (roomno == RS_KITCHEN) ||
      (roomno == RS_BATHROOM) ||
      (roomno == RS_BEDROOM) ||
      (roomno == RS_DININGROOM) ||
      (roomno == RS_CLOSET) ||
      (roomno > ROOMBASE))
    if ((! loc_statusp(Player.x,Player.y,LIT)) && 
	(! Player.status[BLINDED]) &&
	(Player.status[ILLUMINATION] || (difficulty() < 6))) {
      showroom(Level->site[Player.x][Player.y].roomnumber);
      spreadroomlight(Player.x,Player.y,roomno);
      levelrefresh();
    }
  if ((oldroomno != roomno) || (oldlevel != Level)) {
    showroom(roomno);
    oldroomno = roomno;
    oldlevel = Level;
  }
}





/* ask for mercy */
void surrender(m)
struct monster *m;
{
  int i,bestitem,bestvalue;
  pml ml;
  switch(random_range(4)) {
  case 0: print1("You grovel at the monster's feet..."); break;
  case 1: print1("You cry 'uncle'!"); break;
  case 2: print1("You beg for mercy."); break;
  case 3: print1("You yield to the monster."); break;
  }
  if ((m->id == ML0+3) && (! m_statusp(m,HOSTILE))) {
    print2("The guard (bored): Have you broken a law? [yn] ");
    if (ynq2() == 'y') {
      print2("The guard grabs you, and drags you to court.");
      for(ml=Level->mlist;ml!=NULL;ml=ml->next)
	if (ml->m->id == ML0+3)
	  m_status_reset(ml->m,HOSTILE);
      morewait();
      send_to_jail();
    }
    else print2("Then don't bother me. Scat!");
  }
  else if ((m->talkf==M_NO_OP) ||
	   (m->talkf==M_TALK_STUPID))
    print3("Your plea is ignored.");
  else  {
    morewait();
    print1("Your surrender is accepted.");
    if (Player.cash > 0) nprint1(" All your gold is taken....");
    Player.cash = 0;
    bestvalue = 0;
    bestitem = ABORT;
    for (i=1;i<MAXITEMS;i++)
      if (Player.possessions[i] != NULL)
	if (bestvalue < true_item_value(Player.possessions[i])) {
          bestitem = i;
          bestvalue = true_item_value(Player.possessions[i]);
        }
    if (bestitem != ABORT) {
      print2("You also give away your best item... ");
      nprint2(itemid(Player.possessions[bestitem]));
      nprint2(".");
      morewait();
      givemonster(m,Player.possessions[bestitem]);
      morewait(); /* msgs come from givemonster */
      conform_lost_object(Player.possessions[bestitem]);
    }
    print2("You feel less experienced... ");
    Player.xp = max(0,Player.xp - m->xpv);
    nprint2("The monster seems more experienced!");
    m->level = (min(10,m->level+1));
    m->hp += m->level*20;
    m->hit += m->level;
    m->dmg += m->level;
    m->ac += m->level;
    m->xpv += m->level*10;
    morewait();
    clearmsg();
    if ((m->talkf == M_TALK_EVIL) && random_range(10)) {
      print1("It continues to attack you, laughing evilly!");
      m_status_set(m,HOSTILE);
      m_status_reset(m,GREEDY);
    }
    else if (m->id == ML0+0 || m->id == ML0+3)
      print1("It continues to attack you. ");
    else {
      print1("The monster leaves, chuckling to itself....");
      m_teleport(m);
    }
  }
  dataprint();
}


/* threaten a monster */
void threaten(m)
struct monster *m;
{
  char response;
  switch(random_range(4)) {
  case 0:mprint("You demand that your opponent surrender!"); break;
  case 1:mprint("You threaten to do bodily harm to it."); break;
  case 2:mprint("You attempt to bluster it into submission."); break;
  case 3:mprint("You try to cow it with your awesome presence."); break;
  }
  if (! m_statusp(m,HOSTILE)) {
    print3("You only annoy it with your futile demand.");
    m_status_set(m,HOSTILE);
  }
  else if (((m->level*2 > Player.level) && (m->hp > Player.dmg)) || 
	   (m->uniqueness != COMMON))
    print1("It sneers contemptuously at you.");
  else if ((m->talkf != M_TALK_GREEDY) &&
	   (m->talkf != M_TALK_HUNGRY) &&
	   (m->talkf != M_TALK_EVIL) &&
	   (m->talkf != M_TALK_MAN) &&
	   (m->talkf != M_TALK_BEG) &&
	   (m->talkf != M_TALK_THIEF) &&
	   (m->talkf != M_TALK_MERCHANT) &&
	   (m->talkf != M_TALK_IM)) 
    print1("Your demand is ignored");
  else {
    print1("It yields to your mercy.");
    Player.alignment+=3;
    print2("Kill it, rob it, or free it? [krf] ");
    do response = mcigetc();
    while ((response != 'k')&&(response != 'r')&&(response !='f'));
    if (response == 'k') {
      m_death(m);
      print2("You treacherous rogue!");
      Player.alignment -= 13;
    }
    else if (response == 'r') {
      Player.alignment-=2;
      print2("It drops its treasure and flees.");
      m_dropstuff(m);
      m->hp = -1;
      Level->site[m->x][m->y].creature = NULL;
      putspot(m->x,m->y,getspot(m->x,m->y,FALSE));
    }
    else {
      Player.alignment+=2;
      print2("'If you love something set it free ... '");
      if (random_range(100)==13) {
	morewait();
	print2("'...If it doesn't come back, hunt it down and kill it.'");
      }
      print3("It departs with a renewed sense of its own mortality.");
      m->hp = -1;
      Level->site[m->x][m->y].creature = NULL;
      putspot(m->x,m->y,getspot(m->x,m->y,FALSE));
    }
  }
}

/* name of the player's experience level */
char *levelname(level)
{
  switch(level) {
  case 0:strcpy(Str3,"neophyte");break;
  case 1:strcpy(Str3,"beginner");break;
  case 2:strcpy(Str3,"tourist");break;
  case 3:strcpy(Str3,"traveller");break;
  case 4:strcpy(Str3,"wayfarer");break;
  case 5:strcpy(Str3,"peregrinator");break;
  case 6:strcpy(Str3,"wanderer");break;
  case 7:strcpy(Str3,"hunter");break;
  case 8:strcpy(Str3,"scout");break;
  case 9:strcpy(Str3,"trailblazer");break;
  case 10:strcpy(Str3,"discoverer");break;
  case 11:strcpy(Str3,"explorer");break;
  case 12:strcpy(Str3,"senior explorer");break;
  case 13:strcpy(Str3,"ranger");break;
  case 14:strcpy(Str3,"ranger captain");break;
  case 15:strcpy(Str3,"ranger knight");break;
  case 16:strcpy(Str3,"adventurer");break;
  case 17:strcpy(Str3,"experienced adventurer");break;
  case 18:strcpy(Str3,"skilled adventurer");break;
  case 19:strcpy(Str3,"master adventurer");break;
  case 20:strcpy(Str3,"hero");break;
  case 21:strcpy(Str3,"superhero");break;
  case 22:strcpy(Str3,"demigod");break;
  default:
    if (level < 100) {
      strcpy(Str3,"Order ");
      Str3[6] = ((level/10)-2) + '0';
      Str3[7] = 0;
      strcat(Str3," Master of Omega");
    }
    else strcpy(Str3,"Ultimate Master of Omega");
    break;
  }
  return(Str3);
}
