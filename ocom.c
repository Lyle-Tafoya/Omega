/* omega copyright (C) by Laurence Raphael Brothers, 1987 */
/* ocom.c */

/* this file should only include p_process, and functions called by */
/* p_process. functions called by those are in oaux */

#include <strings.h>
#include "oglob.h"

/* from omovef */
extern void p_movefunction();

/* from oeffect1 or 2 */
extern void wish(),level_drain();

/* from olev */
extern int save_game(),restore_game();

/* from ogen */
extern void create_level(),change_level();

/* from outil */
extern int random_range(),inbounds(),los_p(),distance();
extern void do_object_los();

/* from ocity */
extern void load_city();

/* from oscr */
extern void commanderror(),redraw(),commandlist(),helpfile();
extern void printm(),mprint(),menumorewait(),xredraw(),morewait();
extern void drawspot(),show_screen(),menuclear(),show_license();
extern void menuprint(),erase_level(),drawvision(),maddch(),clearmsg();
extern void menu_i_print(),drawscreen(),locprint(),endgraf(),mnumprint();
extern void display_quit();
extern char *msgscanstring();
extern char mgetc(),menugetc();
extern char ynq();
extern void screencheck();


/* from omon */
extern void monster_talk(),m_status_set();

/* from oitem */
extern int twohanded_p();
extern void item_use();

/* from oinv */
extern void p_drop_at(),drop_at(),drop_money(),give_money();

/* from oaux, probably 1, maybe 2 */
extern void pickup_at(),setspot(),foodcheck(),p_death(),tunnelcheck();
extern void calc_melee(),p_melee(),gain_experience(),p_damage();
extern void p_teleport(),tacmode(),darkenroom();
extern void searchat(),weapon_use(),damage_item(),roomcheck();
extern void givemonster(),conform_unused_object(),conform_lost_object();
extern void hand_wield(),optionset(),optionreset(),describe_player();
extern void conform_lost_objects(),lightroom(),surrender(),threaten();
extern char *itemid(),*mstatus_string(),*trapid();
extern int p_moveable(),cursed(),getitem(),getdir(),optionp(),goberserk();
extern int corridor_turn(),player_on_sanctuary();

/* ocom functions */
void pickup(),drop(),wwp(),magic(),disarm(),save(),restore();
void inventory(),opendoor(),closedoor(),fire(),fight();
void p_process(),eat(),version(),nap(),vault(),bash_item();
void search(),peruse(),quaff(),talk(),bash_location(),wizard();
void give(),rest(),examine(),help(),quit(),lastmprint();
void zapwand(),urt(),floor_inv(),callitem(), charid();
void moveplayer(),levelchange(),setoptions(),activate();
void pickpocket(),rename_player(),abortshadowform(),tunnel();



/* deal with a new player command */
void p_process()
{
  static int searchval=10; /* do a search for the first 10 moves */

  drawvision(Player.x,Player.y,Player.vision);

  Skipplayer = (Skipplayer ||
		((Player.status[BERSERK] > 0) && goberserk()));
  if (! Skipplayer) {
    if (searchval > 0) {
      searchval--;
      if (searchval == 0) Fastmove = FALSE;
    }
    if (! Fastmove) {
      dataprint();
      searchval = 0;
      Cmd = mgetc();
      clearmsg();
    }
    else if (! optionp(JUMPMOVE)) drawvision(Player.x,Player.y,Player.vision);
    switch (Cmd) {
     case ' ': 
     case 13: Skipmonsters = TRUE; break;  /* no op on space or return */
     case 6: abortshadowform(); /* ^f */
     case 7: wizard(); break; /* ^g */
     case 14: xredraw(); Skipmonsters = TRUE; break; /* ^l */
     case 16: lastmprint(); break; /* ^p */ 
     case 18: redraw();  Skipmonsters = TRUE; break; /* ^r */
     case 23: if (Wizard) drawscreen();  break; /* ^w */
     case 24: if (Wizard || Player.rank[ADEPT]) wish(1);  break; /* ^x */
     case 'a': zapwand();  break;
     case 'c': closedoor();  break;
     case 'd': drop();  break;
     case 'e': eat();  break;
     case 'f': fire();  break;
     case 'g': pickup();  break;
     case 'i': inventory();  Skipmonsters = TRUE; break;
     case 'm': magic();  break;
     case 'o': opendoor(); break;
     case 'p': pickpocket(); break;
     case 'q': quaff();  break;
     case 'r': peruse();  break;
     case 's': search(&searchval); break;
     case 't': talk(); break;
     case 'v': vault(); break;
     case 'w': wwp();  break; /* wield/wear/put on */
     case 'x': examine(); break;
     case 'z': bash_location();  break;
     case 'A': activate();  break;
     case 'C': callitem();  break;
     case 'D': disarm();  break;
     case 'F': fight();  break;
     case 'G': give();  break;
     case 'I': floor_inv(); break;
     case 'O': setoptions(); break;
     case 'P': show_license(); break; /* actually show_license is in oscr */
     case 'Q': quit(); break;
     case 'R': rename_player(); break;
     case 'S': save(); break;
     case 'T': tunnel(); break;
     case 'V': version(); break;
     case 'W': urt();  break;
     case 'Z': bash_item();  break;
     case '.': rest(); break;
     case ',': nap(); break;
     case '>': levelchange(Dungeon[Dlevel][Player.x][Player.y].aux,'>');break;
     case '<': levelchange(Dungeon[Dlevel][Player.x][Player.y].aux,'<');break;
     case '/': charid(); Skipmonsters = TRUE; break;
     case '?': help(); Skipmonsters = TRUE; break;
     case '4':	       
     case 'h': moveplayer(-1,0);  break;
     case '2':
     case 'j': moveplayer(0,1);  break;
     case '8':
     case 'k': moveplayer(0,-1);  break;
     case '6':
     case 'l': moveplayer(1,0);  break;
     case '1':
     case 'b': moveplayer(-1,1);  break;
     case '3':	      
     case 'n': moveplayer(1,1);  break;
     case '7':
     case 'y': moveplayer(-1,-1);  break;
     case '9':
     case 'u': moveplayer(1,-1);  break;
     case '5': 
       Skipmonsters = TRUE; /* don't do anything; this is a dummy turn */
       Cmd = mgetc();
       while ((Cmd != ESCAPE) &&
	      ((Cmd < '1') || (Cmd > '9') || (Cmd=='5'))) {
	 printm("\nRun in keypad direction [ESCAPE to abort]: ");
	 Cmd = mgetc();
       }
       if (Cmd != ESCAPE) Fastmove = TRUE;
       break;
     case 'H': Fastmove = TRUE; Cmd = 'h'; moveplayer(-1,0);  break; 
     case 'J': Fastmove = TRUE; Cmd = 'j'; moveplayer(0,1);  break;  
     case 'K': Fastmove = TRUE; Cmd = 'k'; moveplayer(0,-1);  break; 
     case 'L': Fastmove = TRUE; Cmd = 'l'; moveplayer(1,0);  break;  
     case 'B': Fastmove = TRUE; Cmd = 'b'; moveplayer(-1,1);  break; 
     case 'N': Fastmove = TRUE; Cmd = 'n'; moveplayer(1,1);  break;  
     case 'Y': Fastmove = TRUE; Cmd = 'y'; moveplayer(-1,-1);  break;
     case 'U': Fastmove = TRUE; Cmd = 'u'; moveplayer(1,-1);  break;
     default: commanderror();  Skipmonsters = TRUE; break;
    }
  }
}

/* no op a turn.... */
void rest()
{
  if (random_range(20) == 1) {
    switch(random_range(10)) {
      case 0: mprint(" Time passes slowly.... "); break;
      case 1: mprint(" Tick. Tock. Tick. Tock. "); break;
      case 2: mprint(" Ho Hum. "); break;
      case 3: mprint(" Beauty Sleep. Well, in your case, Ugly Sleep. "); break;
      case 4: mprint(" And with Strange Aeons, even Death may die. "); break;
      case 5: mprint(" La Di Da. "); break;
      case 6: mprint(" Time keeps on tickin' tickin' -- into the future.... ");
  	      break;
      case 7: mprint(" Boooring! "); break;
      case 8: mprint(" You think I like watching you sleep? "); break;
      case 9: mprint(" You sure have an early bedtime! "); break;
    }
    morewait();
  }
}


/* read a scroll, book, tome, etc. */
void peruse()
{
  int index;
  struct object *obj;

  if (Player.status[BLINDED] > 0)
    mprint("You're blind -- you can't read!!!");
  else if (Player.status[AFRAID] > 0)
    mprint("You are too afraid to stop to read a scroll!");
  else {
    printm("\nRead -- ");
    index = getitem(SCROLL);
    if (index == ABORT)
      Skipmonsters = TRUE;
    else {
      obj = Player.possessions[index-'a'];
      if (obj->objchar != SCROLL) {
	printm("\nThere's nothing written on ");
	mprint(itemid(obj));
      }
      else {
	mprint("You carefully unfurl the scroll....");
	item_use(obj);
	conform_lost_objects(1,obj);
      }
    }
  }
}




void quaff()
{
  int index;
  struct object *obj;

  printm("\nQuaff --");
  index = getitem(POTION);
  if (index == ABORT)
    Skipmonsters = TRUE;
  else {
    obj = Player.possessions[index-'a'];
    if (obj->objchar != POTION) {
      printm("\nYou can't drink ");
      printm(itemid(obj));
    }
    else {
      mprint("You drink it down.... ");
      item_use(obj);
      conform_lost_objects(1,obj);
    }
  }
}




void activate()
{
  int index;
  struct object *obj;

  printm("\nActivate --");
  index = getitem(ARTIFACT);
  if (index == ABORT)
    Skipmonsters = TRUE;
  else {
    obj = Player.possessions[index-'a'];
    if (obj->objchar != ARTIFACT) {
      printm("\nYou can't activate ");
      printm(itemid(obj));
    }
    else {
      mprint("You activate the artifact.... ");
      item_use(obj);
    }
  }
}





void eat()
{
  int index;
  struct object *obj;

  printm("\nEat --");
  index = getitem(FOOD);
  if (index == ABORT)
    Skipmonsters = TRUE;
  else {
    obj = Player.possessions[index-'a'];
    if ((obj->objchar != FOOD)&&(obj->objchar != CORPSE)) {
      printm("\nYou can't eat ");
      printm(itemid(obj));
    }
    else {
      mprint("Munch Munch.... ");
      if (obj->objchar == FOOD) Player.food = max(0,Player.food+obj->aux);
      item_use(obj);
      conform_lost_objects(1,obj);
    }
  }
  foodcheck();
}




/* search all adjacent spots for secrecy */
void search(searchval)
int *searchval;
{
  int i;
  if (Player.status[AFRAID] > 0)
    mprint("You are too terror-stricken to stop to search for anything.");
  else {
    if (!Fastmove) {
      Fastmove = TRUE;
      *searchval = Searchnum;
    }
    for (i=0;i<8;i++) 
      searchat(Player.x+Dirs[0][i],Player.y+Dirs[1][i]);
    drawvision(Player.x,Player.y,Player.vision);
  }
}



/* pick up a thing where the player is */
 void pickup()
 {
   if (Dungeon[Dlevel][Player.x][Player.y].things == NULL)
     printm("\nThere's nothing there!");
   else pickup_at(Player.x,Player.y);
 }


/* floor inventory */
void floor_inv()
{
  pol ol = Dungeon[Dlevel][Player.x][Player.y].things;
  Skipmonsters = TRUE;
  menuclear();
  while (ol != NULL) {
    if (ol->thing == NULL) printf("***Error; null thing on things list***");
    else {
      menuprint(itemid(ol->thing));
      menuprint("\n");
    }
    ol = ol->next;
  }
  menumorewait();
  xredraw();
}









void drop()
{
  int index,n;
  
  printm("\nDrop --");
  index = getitem(NULL);
  if (index == ABORT)
    Skipmonsters = TRUE;
  else {
    if (index == CASHVALUE) drop_money();
    else if ((! Player.possessions[index-'a']->used) ||
	     (! cursed(Player.possessions[index-'a']))) {
      if (Player.possessions[index-'a']->number == 1) {
	drop_at(Player.x,Player.y,Player.possessions[index-'a']);
	conform_lost_objects(1,Player.possessions[index-'a']);
      }
      else {
	n = getnumber(Player.possessions[index-'a']->number);
	p_drop_at(Player.x,Player.y,n,Player.possessions[index-'a']);
	conform_lost_objects(n,Player.possessions[index-'a']);
      }
    }
    else {
      mprint("You can't seem to get rid of: ");
      mprint(itemid(Player.possessions[index-'a']));
    }
  }
  calc_melee();
}


/* wield/wear/put-on */
void wwp()
{
  int index;
  struct object *obj;
  int unuseable = FALSE;

  printm("\nUse --");
  index = getitem(NULL);
  if (index == ABORT)
    Skipmonsters = TRUE;
  else if (index == CASHVALUE) mprint("Can't use cash!");
  else if (Player.possessions[index-'a']->used)
    printm("\nYou already are using that item!");
  else {
    morewait();
    obj = Player.possessions[index-'a'];
    switch (obj->objchar) {
      case SHIELD:
        if ((Player.primary != NULL) && twohanded_p(Player.primary->id))
	  mprint("You can't use a shield with a two-handed weapon.");
	else if (! cursed(Player.secondary)) {
	  if (Player.secondary != NULL) {
	    mprint("You stop using your old shield.");
	    morewait();
	    conform_unused_object(Player.secondary);
	  }
	  Player.secondary = obj;
	  obj->used = TRUE;
	  printm("\nNow blocking with ");
	  mprint(itemid(obj));
	  item_use(obj);
        }
        else mprint("You can't take your shield off!");
	break;
      case WEAPON:
      case MISSILEWEAPON:
        if (twohanded_p(obj->id) && (Player.secondary != NULL))
	  mprint("You'll have to stop using that shield, first.");
	else if (! cursed(Player.primary)) {
	  if (Player.primary != NULL) {
	    mprint("You put away your old weapon.");
	    morewait();
	    conform_unused_object(Player.primary);
	  }
	  Player.primary = obj;
	  obj->used = TRUE;
	  printm("\nNow wielding ");
	  mprint(itemid(obj));
	  item_use(obj);
        }
        else mprint("Your weapon seems stuck to your hand!");
	break;
      case ARMOR:
	if (! cursed(Player.armor)) {
	  if (Player.armor != NULL) {
	    mprint("You take off your old suit of armor");
	    morewait();
	    conform_unused_object(Player.armor);
	  }
	  Player.armor = obj;
	  obj->used = TRUE;
	  printm("\nNow armored in ");
	  mprint(itemid(obj));	  
	  item_use(obj);
        }
        else mprint("You can't seem to take your armor off!");		   
	break;
      case BOOTS:
	if (! cursed(Player.boots)) {
	  if (Player.boots != NULL) {
	    mprint("You take off your old pair of boots.");
	    morewait();
	    conform_unused_object(Player.boots);
	  }
	  Player.boots = obj;
	  obj->used = TRUE;
	  printm("\nNow shod in ");
	  mprint(itemid(obj));
	  item_use(obj);
	}
        else mprint("Your boots are stuck on your feet!");
	break;
      case CLOAK:
	if (! cursed(Player.cloak)) {
	  if (Player.cloak != NULL) {
	    mprint("You take off your old cloak.");
	    morewait();
	    conform_unused_object(Player.cloak);
	  }
	  Player.cloak = obj;
	  obj->used = TRUE;
	  printm("\nNow garbed in ");
	  mprint(itemid(obj));
	  item_use(obj);
	}
	else mprint("Your cloak refuses to come off!");
	break;
      case RING:
	if (Player.numrings == 8)
	  mprint("Too many rings! Can't wear ");
	else {
	  Player.numrings++;
	  obj->used = TRUE;
	  printm("\nNow wearing: ");
	  item_use(obj);
	}
	mprint(itemid(obj));
	break;
      case THING:
	if (obj->usef == I_NO_OP) {
	  mprint("You can't use:");
	  mprint(itemid(obj));
	}
	else {
	  obj->used = TRUE;
	  printm("\nUsing: ");
	  mprint(itemid(obj));
	  item_use(obj);
	}
	break;
      default:
	printm("\nNo way you can employ ");
	mprint(itemid(obj));
	unuseable = TRUE;
	break;
      }	     
  }
  calc_melee();
}
      




/* unwield/remove/take-off */
void urt()
{
  int index;
  printm("\nStop using --");
  index = getitem(NULL);
  if (index == ABORT)
    Skipmonsters = TRUE;
  else if (index == CASHVALUE) printm("\nNot using cash!");
  else if (! Player.possessions[index-'a']->used) {
    printm("\nYou're not using that item!");
    Skipmonsters = TRUE;
  }
  else if (! cursed(Player.possessions[index-'a'])) {
    morewait();
    clearmsg();
    switch (Player.possessions[index-'a']->objchar) {
      case WEAPON:
        mprint("No longer wielding ");
	break;
      case ARMOR:
	mprint("Took off ");
	break;
      case SHIELD:
	mprint("Stopped blocking with ");
	break;
      case BOOTS:
	mprint("Took off ");
	break;
      case CLOAK:
	mprint("Took off ");
	break;
      case RING:
	mprint("Removed ");
	Player.numrings--;
	break;
      case THING:
	mprint("Stopped using: ");
	break;
      }
    mprint(itemid(Player.possessions[index-'a']));
    conform_unused_object(Player.possessions[index-'a']);
  }
  else {
    mprint("You can't seem to stop using: ");
    mprint(itemid(Player.possessions[index-'a']));
  }
  calc_melee();
}

void inventory()
{
  int i;

  menuclear();
  menuprint("Cash: ");
  menunumprint(Player.cash);
  menuprint("Au.\n");
  for (i=0;i<MAXITEMS;i++)
    if (Player.possessions[i] != NULL)
      menu_i_print(i);
  menumorewait();
  xredraw();
}




/* talk to the animals -- learn their languages.... */
void talk()
{
  int dir;
  int dx,dy,index=0;
  char response;
  struct monster *m;
  printm("\nTalk:");
  index = getdir();

  if (index == ABORT)
    Skipmonsters = TRUE;
  else {
    dx = Dirs[0][index];
    dy = Dirs[1][index];
    
    if ((! inbounds(Player.x+dx, Player.y+dy)) ||
	(Dungeon[Dlevel][Player.x+dx][Player.y+dy].creature == NULL)) {
      printm("\nThere's nothing there to talk to!!!");
      Skipmonsters = TRUE;
    }
    else {
      m = Dungeon[Dlevel][Player.x+dx][Player.y+dy].creature;
      menuclear();
      strcpy(Str1,"     Talk to ");
      strcat(Str1,m->monstring);
      strcat(Str1,":");
      menuprint(Str1);
      menuprint("\na: Greet.");
      menuprint("\nb: Threaten.");
      menuprint("\nc: Surrender.");
      menuprint("\nESCAPE: Clam up.");
      do response = menugetc();
      while ((response != 'a') &&
	     (response != 'b') &&
	     (response != 'c') &&
	     (response != ESCAPE));
      switch(response) {
      case 'a': monster_talk(m); break;
      case 'b': threaten(m); break;
      case 'c': surrender(m); break;
      default: Skipmonsters = TRUE; break;
      }
    }
  }
  xredraw();
}

/* try to deactivate a trap */
void disarm()
{
  int dir;
  int x,y,index=0;
  pob o;

  index = getdir();

  if (index == ABORT)
    Skipmonsters = TRUE;
  else {
    x = Dirs[0][index]+Player.x;
    y = Dirs[1][index]+Player.y;
    
    if (! inbounds(x,y))
      mprint("Whoa, off the map...");
    else if (Dungeon[Dlevel][x][y].locchar != TRAP)
      mprint("You can't see a trap there!");
    else {
      if (random_range(50+Dlevel*2) < 
	  Player.dex+Player.level+Player.rank[THIEVES]*10) {
	mprint("You disarmed the trap!");
	if (random_range(100) < Player.dex+Player.rank[THIEVES]*10) {
	  o = ((pob) malloc(sizeof(objtype)));
	  switch(Dungeon[Dlevel][x][y].p_locf) {
	  case L_TRAP_DART: 
	    *o=Objects[THINGID+17];
	    break;
	  case L_TRAP_DISINTEGRATE:
	    *o=Objects[THINGID+23];
	    break;
	  case L_TRAP_SLEEP_GAS:
	    *o=Objects[THINGID+22];
	    break;
	  case L_TRAP_TELEPORT:
	    *o=Objects[THINGID+21];
	    break;
	  case L_TRAP_ABYSS:
	    *o=Objects[THINGID+24];
	    break;
	  case L_TRAP_FIRE:
	    *o=Objects[THINGID+20];
	    break;
	  case L_TRAP_SNARE:
	    *o=Objects[THINGID+19];
	    break;
	  case L_TRAP_ACID:
	    *o=Objects[THINGID+18];
	    break;
	  case L_TRAP_MANADRAIN:
	    *o=Objects[THINGID+25];
	    break;
	  default:
	    o = NULL;
	    break;
	  }
	  if (o != NULL) {
	    mprint("You manage to retrieve the trap components!");
	    Objects[o->id].known = 1;
	    o->known = 1;
	    add_item_to_pack(o);
	    gain_experience(25);
	  }
	}
	Dungeon[Dlevel][x][y].p_locf = L_NO_OP;
	Dungeon[Dlevel][x][y].locchar = FLOOR;
	gain_experience(5);
      }
      else if (random_range(10+Dlevel) > Player.dex) {
	mprint("You accidentally set off the trap!");
	Player.x = x; Player.y = y;
	p_movefunction(Dungeon[Dlevel][x][y].p_locf);
      }
      else mprint("You failed to disarm the trap.");
    }
  }
}

/* is it more blessed to give, or receive? */
void give()
{
  int index;
  int dx,dy,dindex=0;
  struct monster *m;
  pob obj;

  printm("\nGive to monster --");
  dindex = getdir();
  if (dindex == ABORT) 
    Skipmonsters = TRUE;
  else {
    dx = Dirs[0][dindex];
    dy = Dirs[1][dindex];
    if ((! inbounds(Player.x+dx, Player.y+dy)) ||
	(Dungeon[Dlevel][Player.x+dx][Player.y+dy].creature == NULL)) {
      printm("\nThere's nothing there to give something to!!!");
      Skipmonsters = TRUE;
    }
    else {
      m = Dungeon[Dlevel][Player.x+dx][Player.y+dy].creature;
      printm("\nItem to give away: "); 
      index = getitem(NULL);
      if (index == ABORT)
	Skipmonsters = TRUE;
      else if (index == CASHVALUE) give_money(m);
      else if (! cursed(Player.possessions[index-'a'])) {
	  obj = ((pob) calloc(1,sizeof(objtype)));
	  *obj = *(Player.possessions[index-'a']);
	  obj->number = 1;
	  givemonster(m,obj);
	  printm("\nGiven: ");
	  mprint(itemid(obj));
	  morewait();
	  clearmsg();
	  calc_melee();
	}
	else {
	  mprint("You can't even give away: ");
	  mprint(itemid(Player.possessions[index-'a']));
	}
      }
    }
}




/* zap a wand, of course */
void zapwand()
{
  int index;
  struct object *obj;

  if (Player.status[AFRAID] > 0)
    mprint("You are so terror-stricken you can't hold a wand straight!");
  else {
    printm("\nZap --");
    index = getitem(STICK);
    if (index == ABORT)
      Skipmonsters = TRUE;
    else {
      obj = Player.possessions[index-'a'];
      if (obj->objchar != STICK) {
	printm("\nYou can't zap: ");
	printm(itemid(obj));
      }
      else 
	if (obj->charge < 1) 
	  printm("\nFizz.... Pflpt. Out of charges. ");
	else {
	  obj->charge--;
	  item_use(obj);
	}
    }
  }
}

/* cast a spell */
void magic()
{
  int index,drain;
  if (Player.status[AFRAID] > 0)
    mprint("You are too afraid to concentrate on a spell!");
  else {
    index = getspell();
    xredraw();
    if (index == ABORT)
      Skipmonsters = TRUE;
    else {
      drain = Spells[index].powerdrain;
      if (Lunarity == 1) drain = drain / 2;
      else if (Lunarity == -1) drain = drain *2;
      if (drain > Player.mana)
	mprint("You lack the power for that spell! ");
      else {
	Player.mana -= drain;
	(*(Spells[index].castf))();
      }
    }
  }
}

void levelchange(newlevel,sitechar)
int newlevel;
char sitechar;
{
  char reverse=FLOOR;

  if (sitechar==UP) reverse = DOWN;
  else if (sitechar==DOWN) reverse = UP;

  if (Dungeon[Dlevel][Player.x][Player.y].locchar != sitechar)
    mprint("Not here!");
  else {
    Dungeon[Dlevel][Player.x][Player.y].showchar = 
      Dungeon[Dlevel][Player.x][Player.y].locchar;
    if (newlevel < 0) {
      mprint("You appear high above the ground!");
      p_damage(abs(newlevel)*10,UNSTOPPABLE,"a fall from a height");
      newlevel = 0;
      }
    else if ((newlevel == NUMLEVELS-1) && (newlevel-Dlevel > 1)) {
      if (Player.patron != DESTINY) {
	mprint("Some mysterious force changes your destination...");
	newlevel = Dlevel+1;
      }
      else mprint("Your long hours of devotion are finally paid off!");
    }
    else if (newlevel >= NUMLEVELS) {
      mprint("You appear deep underground!!!");
      p_death("molten magma");
    }
    morewait();
    if (newlevel > Dlevel+1) {
      printm("\nYou descend ");
      mnumprint(newlevel-Dlevel);
      mprint(" levels...");
    }
    else if (newlevel == Dlevel+1)
      printm("\nYou descend a level.");
    else if (newlevel+1 == Dlevel)
      printm("\nYou ascend a level.");
    else if (newlevel+1 < Dlevel) {
      printm("\nYou ascend ");
      mnumprint(Dlevel-newlevel);
      mprint(" levels...");
    }
    
    if (newlevel == 0) {
      Dlevel = 0;
      erase_level();
      if (! Leveldata[0].generated) load_city();
      Player.x = 32;
      Player.y = 2;
      drawvision(Player.x,Player.y,Player.vision);
      Leveldata[0].generated = TRUE;
      mprint("You find yourself on a familiar street corner.");
      locprint("The City of Rampart.");
      WhichScreen = 0;
      show_screen(WhichScreen);
    }
    else {
      Deepest = max(Deepest,newlevel);
      erase_level();
      if (Leveldata[newlevel].generated)
	change_level(newlevel,reverse); /* sets player location */
      else create_level(Dlevel,newlevel,reverse); /* ditto */
      Leveldata[newlevel].generated = TRUE;

      Dlevel = newlevel;

      screencheck(Player.y);
      roomcheck();
    }
  }
}


/* set various player options */
void setoptions()
{
  char response=' ';

  printm("\nOption BELLICOSE [TF]: ");
  printm( optionp(BELLICOSE) ? "(now T) " : "(now F) ");
  while ((response != 'T') && (response != 'F'))
    response = mgetc();
  maddch(response);
  if (response == 'T') optionset(BELLICOSE);
  else optionreset(BELLICOSE);
  response = ' ';
  printm("\nOption JUMPMOVE [TF]: ");
  printm( optionp(JUMPMOVE) ? "(now T) " : "(now F) ");
  while ((response != 'T') && (response != 'F'))
    response = mgetc();
  maddch(response);
  if (response == 'T') optionset(JUMPMOVE);
  else optionreset(JUMPMOVE);
  response = ' ';
  printm("\nOption PICKUP [TF]: ");
  printm( optionp(PICKUP) ? "(now T) " : "(now F) ");
  while ((response != 'T') && (response != 'F'))
    response = mgetc();
  maddch(response);
  if (response == 'T') optionset(PICKUP);
  else optionreset(PICKUP);
  response = ' ';
  printm("\nOption RUNSTOP [TF]: ");
  printm( optionp(RUNSTOP) ? "(now T) " : "(now F) ");
  while ((response != 'T') && (response != 'F'))
    response = mgetc();
  maddch(response);
  if (response == 'T') optionset(RUNSTOP);
  else optionreset(RUNSTOP);
  response = ' ';
  printm("\nOption CONFIRM [TF]: ");
  printm( optionp(CONFIRM) ? "(now T) " : "(now F) ");
  while ((response != 'T') && (response != 'F'))
    response = mgetc();
  maddch(response);
  if (response == 'T') optionset(CONFIRM);
  else optionreset(CONFIRM);
  printm("\nOption SEARCHNUM [0>x>10]: ");
  printm("(now ");
  maddch(Searchnum+'0');
  printm(") ");
  Searchnum = parsenum();
  if (Searchnum < 1) {
    printm("\nSet minimum value: 1");
    Searchnum = 1;
  }
  else if (Searchnum > 9) {
    printm("\nSet maximum value: 9");
    Searchnum = 9;
  }
}


/* name an item */
void callitem()
{
  int index;
  pob obj;

  Skipmonsters = TRUE;
  printm("\nCall --");
  index = getitem(NULL);
  if (index == CASHVALUE) mprint("Can't rename cash!");
  else if (index != ABORT) {
    obj = Player.possessions[index-'a'];
    if (obj->known)
      printm("\nThat item is already identified!");
    else {
      printm("\nCall it:");
      strcpy(obj->objstr,msgscanstring());
      clearmsg();
      printm("\nAlso call by that name all similar items? [yn]");
      if (ynq() == 'y') strcpy(Objects[obj->id].objstr,obj->objstr);
    }
  }
}
      



/* open a door */
void opendoor()
{
  int dir;
  int ox,oy;

  printm("\nOpen door");
  dir = getdir();
  if (dir == ABORT)
    Skipmonsters = TRUE;
  else {
    ox = Player.x + Dirs[0][dir];
    oy = Player.y + Dirs[1][dir];
    if (Dungeon[Dlevel][ox][oy].locchar == OPEN_DOOR) {
      mprint("That door is already open!");
      Skipmonsters = TRUE;
    }
    else if ((Dungeon[Dlevel][ox][oy].locchar != CLOSED_DOOR) || 
	Dungeon[Dlevel][ox][oy].secret) {
	  mprint("You can't open that!");
	  Skipmonsters = TRUE;
	}
    else if (Dungeon[Dlevel][ox][oy].aux == LOCKED) 
      mprint("That door seems to be locked.");
    else Dungeon[Dlevel][ox][oy].locchar = OPEN_DOOR;
  }
}



/* Try to destroy some location */
void bash_location()
{
  int dir;
  int ox,oy;

  printm("\nBashing location:");
  dir = getdir();
  if (dir == ABORT)
    Skipmonsters = TRUE;
  else {
    ox = Player.x + Dirs[0][dir];
    oy = Player.y + Dirs[1][dir];
    if (Dungeon[Dlevel][ox][oy].locchar == WALL) {
      mprint("You hurl yourself at the wall!");
      p_damage(Player.str,NORMAL_DAMAGE,"a suicidal urge");
    }
    else if (Dungeon[Dlevel][ox][oy].locchar == OPEN_DOOR) {
      mprint("You hurl yourself through the open door!");
      mprint("Yaaaaah! ... thud.");
      Player.x = ox;
      Player.y = oy;
      p_damage(3,UNSTOPPABLE,"silliness");
      p_movefunction(Dungeon[Dlevel][Player.x][Player.y].p_locf);
      Skipmonsters = TRUE; /* monsters are surprised... */
    }
    else if (Dungeon[Dlevel][ox][oy].locchar == CLOSED_DOOR) {
      if (Dungeon[Dlevel][ox][oy].secret) {
	mprint("You found a secret door!");
	Dungeon[Dlevel][ox][oy].secret = FALSE;
      }
      if (Dungeon[Dlevel][ox][oy].aux == LOCKED) {
	if (Dlevel == NUMLEVELS-1) {
	  mprint("Ouch! The door was made out of adamant!");
	  p_damage(Player.str,UNSTOPPABLE,"a door");
	}
	else if (random_range(100)+Dlevel < Player.str) {
	  Player.x = ox;
	  Player.y = oy;
	  mprint("You blast the door off its hinges!");
	  p_movefunction(Dungeon[Dlevel][Player.x][Player.y].p_locf);
	  Dungeon[Dlevel][ox][oy].locchar = FLOOR;
	  Skipmonsters = TRUE; /* monsters are surprised... */
	}
	else {
	  mprint("Crash! The door holds.");
	  if (random_range(30) > Player.str)
	    p_damage(max(1,statmod(Player.str)),UNSTOPPABLE,"a door");
	}
      }
      else {
	Player.x = ox;
	Player.y = oy;
	mprint("You bash open the door!");
	if (random_range(30) > Player.str)
	  p_damage(1,UNSTOPPABLE,"a door");
	p_movefunction(Dungeon[Dlevel][Player.x][Player.y].p_locf);
	Dungeon[Dlevel][ox][oy].locchar = OPEN_DOOR;
	Skipmonsters = TRUE; /* monsters are surprised... */
      }
    }
    else if (Dungeon[Dlevel][ox][oy].locchar == STATUE){
      mprint("you shouldn't of ought to have done that....");
      l_statue_wake();
    }
    else {
      mprint("You restrain yourself from total silliness.");
      Skipmonsters = TRUE;
    }
  }
}


/* attempt destroy an item */
void bash_item()
{
  int item;
  pob obj;
  printm("\nDestroy an item: ");
  morewait();
  item = getitem(NULL);
  if (item == CASHVALUE) mprint("Can't destroy cash!");
  else if (item != ABORT) {
    obj = Player.possessions[item-'a'];
    if (Player.str+random_range(20) > obj->fragility+random_range(20)) {
      if (Player.alignment < 0) {
	mprint("That was fun....");
	gain_experience(obj->level * obj->level * 5);
      }
      damage_item(obj);
    }
    else {
      if (obj->objchar == WEAPON) {
	mprint("The weapon turned in your hand -- you hit yourself!");
	p_damage(random_range(obj->dmg+abs(obj->plus)),
		 NORMAL_DAMAGE,
		 "a failure at vandalism");
      }
      else if (obj->objchar == ARTIFACT) {
	mprint("Uh Oh -- Now you've gotten it angry....");
	p_damage(obj->level*10,
		 UNSTOPPABLE,
		 "an enraged artifact");
      }
      else {
	mprint("Ouch! Damn thing refuses to break...");
	p_damage(1,UNSTOPPABLE,"a failure at vandalism");
      }
    }
  }
}


/* guess what this does */
void save()
{
  printm("\nEnter filename to save character to: ");
  strcpy(Str1,msgscanstring());
  if (save_game(Str1)) {
    endgraf();
    printf("Bye!\n");
    exit(0);
  }
  else Skipmonsters = TRUE;
}


/* close a door */
void closedoor()
{
  int dir;
  int ox,oy;

  printm("\nClose door");
  dir = getdir();
  if (dir == ABORT)
    Skipmonsters = TRUE;
  else {
    ox = Player.x + Dirs[0][dir];
    oy = Player.y + Dirs[1][dir];
    if (Dungeon[Dlevel][ox][oy].locchar == CLOSED_DOOR) {
      mprint("That door is already closed!");
      Skipmonsters = TRUE;
    }
    else if (Dungeon[Dlevel][ox][oy].locchar != OPEN_DOOR) {
      mprint("You can't close that!");
      Skipmonsters = TRUE;
    }
    else Dungeon[Dlevel][ox][oy].locchar = CLOSED_DOOR;
  }
}

/* handle a h,j,k,l, etc. */
void moveplayer(dx,dy)
int dx,dy;
{
  struct location *p;

  if (p_moveable(Player.x+dx,Player.y+dy)) {
    if (Player.status[IMMOBILE] > 0) {
      Fastmove = FALSE;
      mprint("You are unable to move");
    }
    else {
      Player.x += dx;
      Player.y += dy;
      p_movefunction(Dungeon[Dlevel][Player.x][Player.y].p_locf);
      if (Fastmove)
	Fastmove =
	  ! ((Dungeon[Dlevel][Player.x][Player.y].things != NULL) ||
	     (optionp(RUNSTOP) && 
	      Dungeon[Dlevel][Player.x][Player.y].stopsrun));
      if ((Dungeon[Dlevel][Player.x][Player.y].things != NULL) &&
	  (optionp(PICKUP)))
	pickup();
      screencheck(Player.y);
      roomcheck();
    }
  }
  else if (Fastmove == TRUE) {
    drawvision(Player.x,Player.y,Player.vision);
    Fastmove = FALSE;
  }
}



/* look at some spot */
void examine()
{
  pol ol;
  int x=Player.x,y=Player.y;

  Skipmonsters = TRUE;
  mprint("Examine:");
  setspot(&x,&y);
  if (! los_p(Player.x,Player.y,x,y))
    mprint("I refuse to examine something I can't see.");
  else {
    printm("\nYou see:");
    menuclear();
    if (Dungeon[Dlevel][x][y].creature != NULL) {
      strcpy(Str3,mstatus_string(Dungeon[Dlevel][x][y].creature));
      menuprint(Str3);
      menuprint("\n---------------------------\n");
    }
    else if ((Player.x == x) && (Player.y ==y)) {
      describe_player();
      menuprint("\n---------------------------\n");
    }
    if (Dungeon[Dlevel][x][y].things != NULL) {
      ol = Dungeon[Dlevel][x][y].things;
      while (ol != NULL) {
	menuprint(itemid(ol->thing));
	menuprint("\n");
	ol = ol->next;
      }
      menuprint("---------------------------\n");
    }
    if (Dungeon[Dlevel][x][y].secret)
      menuprint("An age-worn stone wall.\n");
    else
      switch (Dungeon[Dlevel][x][y].locchar) {
        case ABYSS: menuprint("An entrance to the infinite abyss\n"); break;
        case PORTAL: menuprint("A strange glowing portal\n"); break;
        case FLOOR:menuprint("A dirty stone floor.\n"); break;
        case WALL:
	  if (Dungeon[Dlevel][x][y].aux < 10)
	    menuprint("A pitted concrete wall.\n");
	  else if (Dungeon[Dlevel][x][y].aux < 30)
	    menuprint("An age-worn sandstone wall.\n");
	  else if (Dungeon[Dlevel][x][y].aux < 50)
	    menuprint("A smooth basalt wall.\n");
	  else if (Dungeon[Dlevel][x][y].aux < 70)
	    menuprint("A solid granite wall.\n");
	  else if (Dungeon[Dlevel][x][y].aux < 90)
	    menuprint("A reinforced plasteel wall.\n");
	  else if (Dungeon[Dlevel][x][y].aux < 110)
	    menuprint("A thick wall of Rampart bluestone\n");
	  else menuprint("A wall of Puppeteer hull metal.\n");
	  break;
        case CLOSED_DOOR:menuprint("A solid oaken door, now closed.\n"); break;
        case OPEN_DOOR:menuprint("A solid oaken door, now open.\n"); break;
        case STATUE:menuprint("A strange-looking statue.\n"); break;
        case UP:menuprint("A stairway leading up.\n"); break;
        case DOWN:menuprint("A stairway leading down....\n"); break;
        case TRAP:menuprint(trapid(Dungeon[Dlevel][x][y].p_locf));
		   menuprint("\n"); break;
        case HEDGE:menuprint("A brambly, thorny hedge.\n"); break;
        case LAVA:menuprint("A bubbling pool of lava.\n"); break;
        case LIFT:menuprint("A strange glowing disk.\n"); break;
        case ALTAR:menuprint("An (un?)holy altar.\n"); break;
        case WHIRLWIND: case WHIRLWIND2:
	  menuprint("A small electrical storm.\n"); break;
        case WATER: 
	  if (Dungeon[Dlevel][x][y].p_locf == L_WATER)
	    menuprint("A deep pool of water.\n");
	  else menuprint("An eerie pool of water.\n");
	  break;
        case FIRE:menuprint("A curtain of fire.\n"); break;
      }
    menumorewait();
    xredraw();
  }
}


void help()
{
  char c;

  printm("\nh for help, c for command list, ESCAPE to quit: ");
  do 
    c = mgetc();
  while ((c != 'h') && (c != 'c') && (c != ESCAPE));

  if (c == 'h') helpfile();
  else if (c == 'c') commandlist();
}  


void version()
{
  Skipmonsters=TRUE;
  printm("\n");
  printm(VERSIONSTRING);
}

void fire()
{
  int index,x1,y1,x2,y2;
  pob obj;
  struct monster *m;

  printm("\nFire/Throw --");
  index = getitem(NULL);
  if (index == ABORT)
    Skipmonsters = TRUE;
  else if (index == CASHVALUE) mprint("Can't fire money at something!");
  else if (cursed(Player.possessions[index-'a']))
    mprint("You can't seem to get rid of it!");
  else if (Player.possessions[index-'a']->used)
    mprint("You'd better stop using it first.");
  else if ((Player.primary != NULL) &&  /* load a crossbow */
	   (Player.primary->id == WEAPONID+27) && /* crossbow id */
	   (Player.primary->aux != LOADED) &&
	   (Player.possessions[index-'a']->id == WEAPONID+29)) { /* bolt id */
	     mprint("You crank back the crossbow and load a bolt.");
	     Player.primary->aux = LOADED;
	   }
  else {
    obj = Player.possessions[index-'a'];
    x1 = x2 = Player.x;
    y1 = y2 = Player.y;
    setspot(&x2,&y2);
    do_object_los(obj->objchar,&x1,&y1,x2,y2);
    if ((m=Dungeon[Dlevel][x1][y1].creature) != NULL) {
      if (obj->dmg == 0) {
	if (m->treasure > 0) { /* ie, the monster can have treasure/objects */
	  strcpy(Str1,"The ");
	  strcat(Str1,m->monstring);
	  strcat(Str1," catches your thrown offering!");
	  mprint(Str1);
	  givemonster(m,obj);
	}
	else {
	  strcpy(Str1,"Your thrown offering bounces off the ");
	  strcat(Str1,m->monstring);
	  mprint(Str1);
	  drop_at(x1,y1,obj);
	}
      }
      else if (hitp(Player.hit,m->ac)) /* ok already, hit the damn thing */
	weapon_use(2*statmod(Player.str),obj,m);
      else {
	strcpy(Str1,"Your ");
	strcat(Str1,itemid(obj));
	strcat(Str1," misses the ");
	strcat(Str1,m->monstring);
	mprint(Str1);
	p_drop_at(x1,y1,1,obj);
	conform_lost_objects(1,obj);
      }
    }
    else {
      p_drop_at(x1,y1,1,obj);
      conform_lost_objects(1,obj);
    }
  }
}

void quit()
{
  mprint("Quit: Are you sure? [yn] ");
  if (ynq()=='y') {
    display_quit();
    endgraf();
    exit(0);
  }
  else Skipmonsters=TRUE;
}


void lastmprint()
{
  clearmsg();
  printm(Laststring);
  Skipmonsters = TRUE;
}

void nap()
{
  static int naptime;
  if (Fastmove) {
    if (naptime-- < 1) {
      Fastmove = FALSE;
      drawvision(Player.x,Player.y,Player.vision);
    }
  }
  else {
    mprint("Rest for how long? (in turns) ");
    naptime = parsenum();
    if (naptime > 1000) {
      mprint("You can only sleep up to 10 hours (1000 turns)");
      naptime = 1000;
    }
    if (naptime > 1) {
      Fastmove = TRUE;
      mprint("Resting.... ");
    }
  }
}


void charid()
{
  char id;
  
  printm("\nCharacter to identify: ");
  id = mgetc();
  if (((id >= 'a') && (id <= 'z')) || ((id >= 'A') && (id <= 'Z'))) 
    printm("A monster or NPC -- examine (x) to find out exactly.");
  else switch(id) {
    case SPACE:
      printm(" : Empty space or unknown region");
      break;
    case WALL:
      printm(" : An (impenetrable?) wall");
      break;
    case PORTAL:
      printm(" : A strange glowing portal");
      break;
    case OPEN_DOOR:
      printm(" : An open door");
      break;
    case CLOSED_DOOR:
      printm(" : A closed (possibly locked) door");
      break;
    case LAVA:
      printm(" : A pool of lava");
      break;
    case HEDGE:
      printm(" : A dense hedge");
      break;
    case WATER:
      printm(" : A deep body of water");
      break;
    case WHIRLWIND:
      printm(" : A mysterious storm");
      break;
    case FIRE:
      printm(" : A curtain of fire");
      break;
    case TRAP:
      printm(" : An uncovered trap");
      break;
    case UP:
      printm(" : A stairway leading up");
      break;
    case DOWN:
      printm(" : A stairway leading down");
      break;
    case FLOOR:
      printm(" : The dungeon floor");
      break;
    case ABYSS:
      printm(" : An entrance to the infinite abyss");
      break;
    case PLAYER:
      printm(" : You, the player");
      break;
    case CORPSE:
      printm(" : The remains of some creature");
      break;
    case THING:
      printm(" : Some random miscellaneous object");
      break;
    case STATUE:
      printm(" : A statue");
      break;
    case ALTAR:
      printm(" : A (un?)holy altar");
      break;
    case CASH:
      printm(" : Bills, specie, gems: cash");
      break;
    case PILE:
      printm(" : A pile of objects");
      break;
    case FOOD:
      printm(" : Something edible");
      break;
    case WEAPON:
      printm(" : Some kind of weapon");
      break;
    case MISSILEWEAPON:
      printm(" : Some kind of missile");
      break;
    case SCROLL:
      printm(" : Something readable");
      break;
    case POTION:
      printm(" : Something drinkable");
      break;
    case ARMOR:
      printm(" : A suit of armor");
      break;
    case SHIELD:
      printm(" : A shield");
      break;
    case CLOAK:
      printm(" : A cloak");
      break;
    case BOOTS:
      printm(" : A pair of boots");
      break;
    case STICK:
      printm(" : A stick");
      break;
    case RING:
      printm(" : A ring");
      break;
    case ARTIFACT:
      printm(" : An artifact");
      break;
    default:
      printm("That character is unused.");
      break;
    }
}

void wizard()
{
  Skipmonsters = TRUE;
  mprint("Really try to toggle wizard mode? [yn]");
  if (ynq()=='y') {
    if (strcmp(getlogin(),WIZARD)==0) {
      if  (! Wizard) {
	Cheated = TRUE;
	mprint("Wizard mode set.");
	Wizard = TRUE;
      }
      else {
	mprint("Wizard mode reset.");
	Wizard = FALSE;
      }
    }
    else {
      mprint("There is a shrieking sound, as of reality being distorted.");
      strcpy(Str1,WIZARD);
      strcat(Str1,", the Wizard of omega appears before you....");
      mprint(Str1);
      mprint("'Do not meddle in the affairs of Wizards --");
      if (random_range(2)) mprint("it makes them soggy and hard to light.'");
      else mprint("for they are subtle, and swift to anger!'");
    }
  }
}


/* Jump, that is */
void vault()
{
  static int lastvault = 0;
  int x=Player.x,y=Player.y,jumper=0;
  
  if (Player.boots != NULL)
    if (Player.boots->usef = I_BOOTS_JUMPING)
      jumper = 2;
  
  if (lastvault+5 > Time) 
    mprint("You're still recovering your wind from the last jump...");
  else if (Player.status[IMMOBILE] > 0) {
    Fastmove = FALSE;
    mprint("You are unable to move");
  }
  else {
    Skipmonsters = TRUE;
    mprint("Jump where?");
    setspot(&x,&y);
    if (! los_p(x,y,Player.x,Player.y))
      mprint("The way is obstructed.");
    else if (distance(x,y,Player.x,Player.y) > 
	     max(2,statmod(Player.agi)+2)+jumper)
      mprint("The jump is too far for you.");
    else if (Dungeon[Dlevel][x][y].creature != NULL)
      mprint("You can't jump on another creature.");
    else if (! p_moveable(x,y))
      mprint("You can't jump there.");
    else {
      lastvault = Time;
      Skipmonsters = FALSE;
      Player.x = x;
      Player.y = y;
      if ((! jumper) && (random_range(30) > Player.agi)) {
	mprint("Oops -- took a tumble.");
	Skipplayer = TRUE;
	p_damage((Player.itemweight/250),UNSTOPPABLE,"clumsiness");
      }
      p_movefunction(Dungeon[Dlevel][Player.x][Player.y].p_locf);
      if ((Dungeon[Dlevel][Player.x][Player.y].things != NULL) &&
	  (optionp(PICKUP)))
	pickup();
      screencheck(Player.y);
      roomcheck();
    }
  }
}



/* fight an adjacent monster */
void fight()
{
  int dir;
  int ox,oy;
  int level=Dlevel,x=Player.x,y=Player.y;
  struct monster *m;
  
  if (Player.status[AFRAID] > 0)
    mprint("You are much too afraid to fight!");
  else if (player_on_sanctuary())
    mprint("You dare not profane this holy place.");
  else {
    printm("\nFight");
    dir = getdir();
    if (dir == ABORT)
      Skipmonsters = TRUE;
    else {
      ox = Player.x + Dirs[0][dir];
      oy = Player.y + Dirs[1][dir];
      if ((m=Dungeon[Dlevel][ox][oy].creature) == NULL) {
	mprint("There's no enemy there!");
	Skipmonsters = TRUE;
      }
      else {
	if (m->attacked == 0) Player.alignment--; /* chaotic action */
	tacmode(m);
	Tacmode = FALSE; /* just making sure */
	xredraw();
      }      
    }
  }
}


/* Do the Artful Dodger trick */
void pickpocket()
{
  int dir;
  int dx,dy,index=0;
  struct monster *m;

  index = getdir();

  if (index == ABORT)
    Skipmonsters = TRUE;
  else {
    dx = Dirs[0][index];
    dy = Dirs[1][index];
    
    if ((! inbounds(Player.x+dx, Player.y+dy)) ||
	(Dungeon[Dlevel][Player.x+dx][Player.y+dy].creature == NULL)) {
      printm("\There's nothing there to steal from!!!");
      Skipmonsters = TRUE;
    }
    else {
      m = Dungeon[Dlevel][Player.x+dx][Player.y+dy].creature;
      if (m->id == ML0+3) {
	mprint("Trying to steal from a guardsman, eh?");
	mprint("Go directly to jail. Do not pass go, do not collect 200Au.");
	erase_level();
	Dlevel=0;
	if (! Leveldata[0].generated) load_city();
	mprint("You are taken to the city gaol.");
	send_to_jail();
	locprint("The City of Rampart.");
	drawvision(Player.x,Player.y,Player.vision);
      }
      else if (m->possessions == NULL) {
	mprint("You couldn't find anything worth taking!");
	mprint("But you managed to annoy it...");
	m_status_set(m,HOSTILE);
      }
      else if (Player.dex*5+Player.rank[THIEVES]*20+random_range(100) >
	       random_range(100)+m->level*20) {
	mprint("You successfully complete your crime!");
	mprint("You stole:");
	mprint(itemid(m->possessions->thing));
	Player.alignment--;
	gain_experience(m->level*m->level);
	add_item_to_pack(m->possessions->thing);
	m->possessions = m->possessions->next;
      }
    }
  }
}



void rename_player()
{
  Skipmonsters = TRUE;
  printm("\nRename Character:");
  strcpy(Player.name,msgscanstring());
}


void abortshadowform()
{
  Skipmonsters = TRUE;
  if (Player.status[SHADOWFORM] && (Player.status[SHADOWFORM]<1000)) {
    mprint("You abort your spell of Shadow Form.");
    Player.immunity[NORMAL_DAMAGE]--;
    Player.immunity[ACID]--;
    Player.immunity[THEFT]--;
    Player.immunity[INFECTION]--;
    mprint("You feel less shadowy now.");
    Player.status[SHADOWFORM] = 0;
  }
}

void tunnel()
{
  int dir,ox,oy;

  printm("\Tunnel");
  dir = getdir();
  if (dir == ABORT)
    Skipmonsters = TRUE;
  else {
    ox = Player.x + Dirs[0][dir];
    oy = Player.y + Dirs[1][dir];
    if (Dungeon[Dlevel][ox][oy].secret)
      mprint("You have no success as yet.");
    else if (Dungeon[Dlevel][ox][oy].locchar != WALL) {
      mprint("You can't tunnel through that!");
      Skipmonsters = TRUE;
    }
    else {
      if (random_range(20)==1){
	if (Player.primary == NULL) {
	  mprint("Ouch! broke a fingernail...");
	  p_damage(Player.str / 6,UNSTOPPABLE,"A broken fingernail");
	}
	else if ((Player.primary->type == THRUSTING) ||
		 ((Player.primary->type != STRIKING) &&
		  (Player.primary->fragility < random_range(20)))) {
	  mprint("Clang! Uh oh...");
	  damage_item(Player.primary);
	}
	else mprint("Your digging implement shows no sign of breaking.");
      }
      if (Player.primary == NULL) {
	if ((Player.str/3)+random_range(100) >
	    Dungeon[Dlevel][ox][oy].aux*2) {
	  mprint("You carve a tunnel through the stone!");
	  tunnelcheck();
	  Dungeon[Dlevel][ox][oy].locchar = FLOOR;
	}
      }
      else if (Player.primary->type == THRUSTING) {
	if (Player.primary->dmg*2+random_range(100) >
	    Dungeon[Dlevel][ox][oy].aux) {
	  mprint("You carve a tunnel through the stone!");
	  tunnelcheck();
	  Dungeon[Dlevel][ox][oy].locchar = FLOOR;
	}
      }
      else if (Player.primary->dmg+random_range(100) >
	       Dungeon[Dlevel][ox][oy].aux) {
	mprint("You carve a tunnel through the stone!");
	tunnelcheck();
	Dungeon[Dlevel][ox][oy].locchar = FLOOR;
      }
      else mprint("You have no success as yet.");
    }
  }
}




