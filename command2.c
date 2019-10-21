/* omega copyright (C) by Laurence Raphael Brothers, 1987,1988,1989 */
/* command2.c */

/* This file contains toplevel commands called from command1.c */

#ifndef MSDOS_SUPPORTED_ANTIQUE
#include <unistd.h>
#include <ctype.h>
#endif

#include "glob.h"

/* no op a turn.... */
void rest()
{
  if (random_range(20) == 1) {
    switch(random_range(10)) {
      case 0: print3(" Time passes slowly.... "); break;
      case 1: print3(" Tick. Tock. Tick. Tock. "); break;
      case 2: print3(" Ho Hum. "); break;
      case 3: print3(" Beauty Sleep. Well, in your case, Ugly Sleep. "); break;
      case 4: print3(" And with Strange Aeons, even Death may die. "); break;
      case 5: print3(" La Di Da. "); break;
      case 6: print3(" Time keeps on tickin' tickin' -- into the future.... ");
  	      break;
      case 7: print3(" Boooring! "); break;
      case 8: print3(" You think I like watching you sleep? "); break;
      case 9: print3(" You sure have an early bedtime! "); break;
    }
    morewait();
  }
}


/* read a scroll, book, tome, etc. */
void peruse()
{
  int index;
  struct object *obj;

  clearmsg();

  if (Player.status[BLINDED] > 0)
    print3("You're blind -- you can't read!!!");
  else if (Player.status[AFRAID] > 0)
    print3("You are too afraid to stop to read a scroll!");
  else {
    print1("Read -- ");
    index = getitem(SCROLL);
    if (index == ABORT)
      setgamestatus(SKIP_MONSTERS);
    else {
      obj = Player.possessions[index];
      if (obj->objchar != SCROLL) {
	print3("There's nothing written on ");
	nprint3(itemid(obj));
      }
      else {
	nprint1("You carefully unfurl the scroll....");
	morewait();
	item_use(obj);
	dispose_lost_objects(1,obj);
      }
    }
  }
}




void quaff()
{
  int index;
  struct object *obj;
  clearmsg();
  print1("Quaff --");
  index = getitem(POTION);
  if (index == ABORT)
    setgamestatus(SKIP_MONSTERS);
  else {
    obj = Player.possessions[index];
    if (obj->objchar != POTION) {
      print3("You can't drink ");
      nprint3(itemid(obj));
    }
    else {
      print1("You drink it down.... ");
      item_use(obj);
      morewait();
      dispose_lost_objects(1,obj);
    }
  }
}




void activate()
{
  int index;
  char response;

  clearmsg();

  print1("Activate -- item [i] or artifact [a] or quit [ESCAPE]?");
  do response = (char) mcigetc();
  while ((response != 'i') && (response != 'a') && (response != ESCAPE));
  if (response != ESCAPE) {
    if (response == 'i')
      index = getitem(THING);
    else if (response == 'a')
      index = getitem(ARTIFACT);
    if (index != ABORT) {
      clearmsg();
      print1("You activate it.... ");
      morewait();
      item_use(Player.possessions[index]);
    }
    else setgamestatus(SKIP_MONSTERS);
  }
  else setgamestatus(SKIP_MONSTERS);
}





void eat()
{
  int index;
  struct object *obj;

  clearmsg();

  print1("Eat --");
  index = getitem(FOOD);
  if (index == ABORT)
    setgamestatus(SKIP_MONSTERS);
  else {
    obj = Player.possessions[index];
    if ((obj->objchar != FOOD)&&(obj->objchar != CORPSE)) {
      print3("You can't eat ");
      nprint3(itemid(obj));
    }
    else {
      if (obj->usef == I_FOOD) Player.food = max(0,Player.food+obj->aux);
      item_use(obj);
      dispose_lost_objects(1,obj);
      if (Current_Dungeon == E_COUNTRYSIDE) {
	Time += 100;
	hourly_check();
      }
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
    print3("You are too terror-stricken to stop to search for anything.");
  else {
    if (!gamestatusp(FAST_MOVE)) {
      setgamestatus(FAST_MOVE);
      *searchval = Searchnum;
    }
    for (i=0;i<9;i++) 
      searchat(Player.x+Dirs[0][i],Player.y+Dirs[1][i]);
    drawvision(Player.x,Player.y);
  }
}



/* pick up a thing where the player is */
 void pickup()
 {
   if (Level->site[Player.x][Player.y].things == NULL)
     print3("There's nothing there!");
   else if (Player.status[SHADOWFORM])
     print3("You can't really interact with the real world in your shadowy state.");
   else
     pickup_at(Player.x,Player.y);
 }


#ifndef MSDOS_SUPPORTED_ANTIQUE
/* floor inventory */
void floor_inv()
{
  pol ol = Level->site[Player.x][Player.y].things;
  setgamestatus(SKIP_MONSTERS);
  menuclear();
  while (ol != NULL) {
    if (ol->thing == NULL) print3("***Error; null thing on things list***");
    else {
      menuprint(itemid(ol->thing));
      menuprint("\n");
    }
    ol = ol->next;
  }
  showmenu();
  morewait();
  xredraw();
}
#endif









void drop()
{
  int index,n;
  
  clearmsg();

  print1("Drop --");
  index = getitem(CASH);
  if (index == ABORT)
    setgamestatus(SKIP_MONSTERS);
  else {
    if (index == CASHVALUE) drop_money();
    else if ((! Player.possessions[index]->used) ||
	     (! cursed(Player.possessions[index]))) {
      if (Player.possessions[index]->number == 1) {
	p_drop_at(Player.x,Player.y,1,Player.possessions[index]);
	conform_lost_objects(1,Player.possessions[index]);
      }
      else {
	n = getnumber(Player.possessions[index]->number);
	p_drop_at(Player.x,Player.y,n,Player.possessions[index]);
	conform_lost_objects(n,Player.possessions[index]);
      }
    }
    else {
      print3("You can't seem to get rid of: ");
      nprint3(itemid(Player.possessions[index]));
    }
  }
  calc_melee();
}


      






/* talk to the animals -- learn their languages.... */
void talk()
{
  int dx,dy,index=0;
  char response;
  struct monster *m;

  clearmsg();

  print1("Talk --");
  index = getdir();

  if (index == ABORT)
    setgamestatus(SKIP_MONSTERS);
  else {
    dx = Dirs[0][index];
    dy = Dirs[1][index];
    
    if ((! inbounds(Player.x+dx, Player.y+dy)) ||
	(Level->site[Player.x+dx][Player.y+dy].creature == NULL)) {
      print3("There's nothing there to talk to!!!");
      setgamestatus(SKIP_MONSTERS);
    }
    else {
      m = Level->site[Player.x+dx][Player.y+dy].creature;
      menuclear();
      strcpy(Str1,"     Talk to ");
      strcat(Str1,m->monstring);
      strcat(Str1,":");
      menuprint(Str1);
      menuprint("\na: Greet.");
      menuprint("\nb: Threaten.");
      menuprint("\nc: Surrender.");
      menuprint("\nESCAPE: Clam up.");
      showmenu();
      do response = menugetc();
      while ((response != 'a') &&
	     (response != 'b') &&
	     (response != 'c') &&
	     (response != ESCAPE));
      switch(response) {
      case 'a': monster_talk(m); break;
      case 'b': threaten(m); break;
      case 'c': surrender(m); break;
      default: setgamestatus(SKIP_MONSTERS); break;
      }
    }
  }
  xredraw();
}

/* try to deactivate a trap */
void disarm()
{
  int x,y,index=0;
  pob o;

  clearmsg();
  print1("Disarm -- ");

  index = getdir();

  if (index == ABORT)
    setgamestatus(SKIP_MONSTERS);
  else {
    x = Dirs[0][index]+Player.x;
    y = Dirs[1][index]+Player.y;
    
    if (! inbounds(x,y))
      print3("Whoa, off the map...");
    else if (Level->site[x][y].locchar != TRAP)
      print3("You can't see a trap there!");
    else {
      if (random_range(50+difficulty()*5) < 
	  Player.dex*2+Player.level*3+Player.rank[THIEVES]*10) {
	print1("You disarmed the trap!");
	if (random_range(100) < Player.dex+Player.rank[THIEVES]*10) {
	  o = ((pob) checkmalloc(sizeof(objtype)));
	  switch(Level->site[x][y].p_locf) {
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
	    free(o);
	    o = NULL;
	    break;
	  }
	  if (o != NULL) {
	    print2("You manage to retrieve the trap components!");
	    morewait();
	    Objects[o->id].known = 1;
	    o->known = 1;
	    gain_item(o);
	    gain_experience(25);
	  }
	}
	Level->site[x][y].p_locf = L_NO_OP;
	Level->site[x][y].locchar = FLOOR;
	lset(x, y, CHANGED);
	gain_experience(5);
      }
      else if (random_range(10+difficulty()*2) > Player.dex) {
	print1("You accidentally set off the trap!");
	Player.x = x; Player.y = y;
	p_movefunction(Level->site[x][y].p_locf);
      }
      else print1("You failed to disarm the trap.");
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

  clearmsg();

  print1("Give to monster --");
  dindex = getdir();
  if (dindex == ABORT) 
    setgamestatus(SKIP_MONSTERS);
  else {
    dx = Dirs[0][dindex];
    dy = Dirs[1][dindex];
    if (! inbounds(Player.x+dx, Player.y+dy))
      print3("Whoa, off the map...");
    else if (Level->site[Player.x+dx][Player.y+dy].creature == NULL) {
      print3("There's nothing there to give something to!!!");
      setgamestatus(SKIP_MONSTERS);
    }
    else {
      m = Level->site[Player.x+dx][Player.y+dy].creature;
      clearmsg();
      print1("Give what? "); 
      index = getitem(CASH);
      if (index == ABORT)
	setgamestatus(SKIP_MONSTERS);
      else if (index == CASHVALUE) give_money(m);
      else if (! cursed(Player.possessions[index])) {
	obj = ((pob) checkmalloc(sizeof(objtype)));
	*obj = *(Player.possessions[index]);
	obj->used = FALSE;
	conform_lost_objects(1,Player.possessions[index]);
	obj->number = 1;
	print2("Given: ");
	nprint2(itemid(obj));
	morewait();
        /* WDT bug fix: I moved the print above the givemonster
         * call.  If that turns out looking ugly, I should change it to
         * a sprintf or strcat.  At any rate, it was wrong before because
         * it was accessing an object which had already been freed as part
         * of givemonster. */
	givemonster(m,obj);
	calc_melee();
      }
      else {
	print3("You can't even give away: ");
	nprint3(itemid(Player.possessions[index]));
      }
    }
  }
}




/* zap a wand, of course */
void zapwand()
{
  int index;
  struct object *obj;

  clearmsg();

  if (Player.status[AFRAID] > 0)
    print3("You are so terror-stricken you can't hold a wand straight!");
  else {
    print1("Zap --");
    index = getitem(STICK);
    if (index == ABORT)
      setgamestatus(SKIP_MONSTERS);
    else {
      obj = Player.possessions[index];
      if (obj->objchar != STICK) {
	print3("You can't zap: ");
	nprint3(itemid(obj));
      }
      else 
	if (obj->charge < 1) 
	  print3("Fizz.... Pflpt. Out of charges. ");
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
  clearmsg();
  if (Player.status[AFRAID] > 0)
    print3("You are too afraid to concentrate on a spell!");
  else {
    index = getspell();
    xredraw();
    if (index == ABORT)
      setgamestatus(SKIP_MONSTERS);
    else {
      drain = Spells[index].powerdrain;
      if (Lunarity == 1) drain = drain / 2;
      else if (Lunarity == -1) drain = drain *2;
      if (drain > Player.mana)
	if (Lunarity == -1 && Player.mana >= drain/2)
	  print3("The contrary moon has made that spell too draining! ");
	else
	  print3("You lack the power for that spell! ");
      else {
	Player.mana -= drain;
	cast_spell(index);
      }
    }
  }
  dataprint();
}


/* go upstairs ('<' command) */
void upstairs()
{
  if (Level->site[Player.x][Player.y].locchar != STAIRS_UP)
    print3("Not here!");
  else if (Level->site[Player.x][Player.y].p_locf == L_ESCALATOR)
    p_movefunction(Level->site[Player.x][Player.y].p_locf);
  else {
    if (gamestatusp(MOUNTED))
      print2("You manage to get your horse upstairs.");
    print1("You ascend a level.");
    if (Level->depth <= 1) {
      if (Level->environment == E_SEWERS)
	change_environment(E_CITY);
      else change_environment(E_COUNTRYSIDE);
    }
    else change_level(Level->depth,Level->depth-1,FALSE);
    roomcheck();
  }
  setgamestatus(SKIP_MONSTERS);
}



/* go downstairs ('>' command) */
void downstairs()
{
  if (Level->site[Player.x][Player.y].locchar != STAIRS_DOWN)
    print3("Not here!");
  else if (Level->site[Player.x][Player.y].p_locf == L_ENTER_CIRCLE ||
    Level->site[Player.x][Player.y].p_locf == L_ENTER_COURT)
    p_movefunction(Level->site[Player.x][Player.y].p_locf);
  else {
    if (gamestatusp(MOUNTED))
      print2("You manage to get your horse downstairs.");
    if (Current_Environment == Current_Dungeon) {
      print1("You descend a level.");
      change_level(Level->depth,Level->depth+1,FALSE);
      roomcheck();
    }
    else if ((Current_Environment == E_CITY) ||
	     (Last_Environment == E_CITY))
      change_environment(E_SEWERS);
    else if (Current_Environment != Current_Dungeon)
      print3("This stairway is deviant. You can't use it.");
  }
  setgamestatus(SKIP_MONSTERS);
}




/* set various player options */
/* have to redefine in odefs for next full recompile */
void setoptions()
{
  int slot = 1,to,done = FALSE;
  int response;

  clearmsg();
  menuclear();

  display_options();

  move_slot(1,1,NUMOPTIONS);
  clearmsg();
  print1("Currently selected option is preceded by highlit >>");
  print2("Move selected option with '>' and '<', ESCAPE to quit.");
  do {
    response = mcigetc();
    switch(response) {
    case 'j':
    case '>':
#ifdef KEY_DOWN
    case KEY_DOWN:
#endif
      to = slot + 1;
#ifndef COMPRESS_SAVE_FILES
      if (to == 8)	/* COMPRESS_OPTION */
	to = 9;
#endif
      slot = move_slot(slot,to,NUMOPTIONS+1);
      break;
    case 'k':
    case '<':
#ifdef KEY_UP
    case KEY_UP:
#endif
      to = slot - 1;
#ifndef COMPRESS_SAVE_FILES
      if (to == 8)	/* COMPRESS_OPTION */
	to = 7;
#endif
      if (to > 0)
	slot = move_slot(slot,to,NUMOPTIONS+1); 
      break;
#ifdef KEY_HOME
    case KEY_HOME:
      slot = move_slot(slot,1,NUMOPTIONS+1);
      break;
#endif
#ifdef KEY_LL
    case KEY_LL:
      slot = move_slot(slot,NUMOPTIONS,NUMOPTIONS+1);
      break;
#endif
    case 't':
      if (slot <= NUMTFOPTIONS) 
	optionset(pow2(slot-1));
      else if (slot == VERBOSITY_LEVEL)
	Verbosity = TERSE;
      else print3("'T' is meaningless for this option.");
      break;
    case 'f':
      if (slot <= NUMTFOPTIONS) 
	optionreset(pow2(slot-1));
      else print3("'F' is meaningless for this option.");
      break;
    case 'm':
      if (slot == VERBOSITY_LEVEL) 
	Verbosity = MEDIUM;
      else print3("'M' is meaningless for this option.");
      break;
    case 'v':
      if (slot == VERBOSITY_LEVEL) 
	Verbosity = VERBOSE;
      else print3("'V' is meaningless for this option.");
      break;
    case '1':case '2':case '3':case '4':case '5':
    case '6':case '7':case '8':case'9':
      if (slot == SEARCH_DURATION)
	Searchnum = response - '0';
      else print3("A number is meaningless for this option.");
      break;
    case ESCAPE:
      done = TRUE;
      break;
    default: print3("That response is meaningless for this option."); break;
    }
    display_option_slot(slot);
    move_slot(slot,slot,NUMOPTIONS+1);
  } while (! done);
  if (optionp(SHOW_COLOUR))
    colour_on();
  else
    colour_off();
#if !defined(MSDOS_SUPPORTED_ANTIQUE) && !defined(AMIGA)
  xredraw();
#endif
}



/* name an item */
void callitem()
{
  int index;
  pob obj;

  clearmsg();
  setgamestatus(SKIP_MONSTERS);
  print1("Call --");
  index = getitem(NULL_ITEM);
  if (index == CASHVALUE) print3("Can't rename cash!");
  else if (index != ABORT) {
    obj = Player.possessions[index];
    if (obj->known)
      print3("That item is already identified!");
    else {
      print1("Call it:");
      obj->objstr = salloc(msgscanstring());
      clearmsg();
      print2("Also call all similar items by that name? [yn] ");
      if (ynq2() == 'y') {
	Objects[obj->id].objstr = obj->objstr;
      }
    }
  }
}
      



/* open a door */
void opendoor()
{
  int dir;
  int ox,oy;

  clearmsg();
  print1("Open --");
  dir = getdir();
  if (dir == ABORT)
    setgamestatus(SKIP_MONSTERS);
  else {
    ox = Player.x + Dirs[0][dir];
    oy = Player.y + Dirs[1][dir];
    if (Level->site[ox][oy].locchar == OPEN_DOOR) {
      print3("That door is already open!");
      setgamestatus(SKIP_MONSTERS);
    }
    else if (Level->site[ox][oy].locchar == PORTCULLIS) {
      print1("You try to lift the massive steel portcullis....");
      if (random_range(100) < Player.str) {
	print2("Incredible. You bust a gut and lift the portcullis.");
	Level->site[ox][oy].locchar = FLOOR;
	lset(ox, oy, CHANGED);
      }
      else {
	print2("Argh. You ruptured yourself.");
	p_damage(Player.str,UNSTOPPABLE,"a portcullis");
      }
    }
    else if ((Level->site[ox][oy].locchar != CLOSED_DOOR) || 
	     loc_statusp(ox,oy,SECRET)) {
      print3("You can't open that!");
      setgamestatus(SKIP_MONSTERS);
    }
    else if (Level->site[ox][oy].aux == LOCKED) 
      print3("That door seems to be locked.");
    else {
      Level->site[ox][oy].locchar = OPEN_DOOR;
      lset(ox, oy, CHANGED);
    }
  }
}



/* Try to destroy some location */
void bash_location()
{
  int dir;
  int ox,oy;

  clearmsg();
  print1("Bashing --");
  dir = getdir();
  if (dir == ABORT)
    setgamestatus(SKIP_MONSTERS);
  else {
    ox = Player.x + Dirs[0][dir];
    oy = Player.y + Dirs[1][dir];
    if ((Current_Environment == E_CITY) &&
	(ox == 0) &&
	(oy == 0)) {
      print1("Back Door WIZARD Mode!");
      print2("You will invalidate your score if you proceed.");
      morewait();
      print1("Enable WIZARD Mode? [yn] ");
      if (ynq1()=='y') {
	print2("You feel like a cheater.");
	setgamestatus(CHEATED);
      }
      else print2("A sudden tension goes out of the air....");
    }
    else {
      if (Level->site[ox][oy].locchar == WALL) {
	print1("You hurl yourself at the wall!");
	p_damage(Player.str,NORMAL_DAMAGE,"a suicidal urge");
      }
      else if (Level->site[ox][oy].locchar == OPEN_DOOR) {
	print1("You hurl yourself through the open door!");
	print2("Yaaaaah! ... thud.");
	morewait();
	Player.x = ox;
	Player.y = oy;
	p_damage(3,UNSTOPPABLE,"silliness");
	p_movefunction(Level->site[Player.x][Player.y].p_locf);
	setgamestatus(SKIP_MONSTERS); /* monsters are surprised... */
      }
      else if (Level->site[ox][oy].locchar == CLOSED_DOOR) {
	if (loc_statusp(ox,oy,SECRET)) {
	  print1("You found a secret door!");
	  lreset(ox,oy,SECRET);
	  lset(ox, oy, CHANGED);
	}
	if (Level->site[ox][oy].aux == LOCKED) {
	  if (random_range(50+difficulty()*10) < Player.str) {
	    Player.x = ox;
	    Player.y = oy;
	    print2("You blast the door off its hinges!");
	    Level->site[ox][oy].locchar = FLOOR;
	    lset(ox, oy, CHANGED);
	    p_movefunction(Level->site[Player.x][Player.y].p_locf);
	    setgamestatus(SKIP_MONSTERS); /* monsters are surprised... */
	  }
	  else {
	    print1("Crash! The door holds.");
	    if (random_range(30) > Player.str)
	      p_damage(max(1,statmod(Player.str)),UNSTOPPABLE,"a door");
	  }
	}
	else {
	  Player.x = ox;
	  Player.y = oy;
	  print2("You bash open the door!");
	  if (random_range(30) > Player.str)
	    p_damage(1,UNSTOPPABLE,"a door");
	  Level->site[ox][oy].locchar = OPEN_DOOR;
	  lset(ox, oy, CHANGED);
	  p_movefunction(Level->site[Player.x][Player.y].p_locf);
	  setgamestatus(SKIP_MONSTERS); /* monsters are surprised... */
	}
      }
      else if (Level->site[ox][oy].locchar == STATUE) {
	statue_random(ox,oy);
      }
      else if (Level->site[ox][oy].locchar == PORTCULLIS) {
	print1("Really, you don't have a prayer.");
	if (random_range(1000) < Player.str) {
	  print2("The portcullis flies backwards into a thousand fragments.");
	  print3("Wow. What a stud.");
	  gain_experience(100);
	  Level->site[ox][oy].locchar = FLOOR;
	  Level->site[ox][oy].p_locf = L_NO_OP;
	  lset(ox, oy, CHANGED);
	}
	else {
	  print2("You only hurt yourself on the 3'' thick steel bars.");
	  p_damage(Player.str,UNSTOPPABLE,"a portcullis");
	}
      }
      else if (Level->site[ox][oy].locchar == ALTAR) {
	if ((Player.patron > 0)&&(Level->site[ox][oy].aux == Player.patron)) {
	  print1("You have a vision! An awesome angel hovers over the altar.");
	  print2("The angel says: 'You twit, don't bash your own altar!'");
	  print3("The angel slaps you upside the head for your presumption.");
	  p_damage(Player.hp-1,UNSTOPPABLE,"an annoyed angel");
	}
	else if (Level->site[ox][oy].aux == 0) {
	  print1("The feeble powers of the minor godling are not enough to");
	  print2("protect his altar! The altar crumbles away to dust.");
	  print3("You feel almost unbearably smug.");
	  Level->site[ox][oy].locchar = RUBBLE;
	  Level->site[ox][oy].p_locf = L_RUBBLE;
	  lset(ox, oy, CHANGED);
	  gain_experience(5);
	}
	else {
	  print1("You have successfully annoyed a major deity. Good job.");
	  print2("Zzzzap! A bolt of godsfire strikes!");
	  if (Player.rank[PRIESTHOOD] > 0) 
	    print3("Your own deity's aegis defends you from the bolt!");
	  p_damage(max(0,random_range(100)-Player.rank[PRIESTHOOD]*20),
		   UNSTOPPABLE,
		   "a bolt of godsfire");
	  if (Player.rank[PRIESTHOOD]*20+Player.pow+Player.level >
	      random_range(200)) {
	    morewait();
	    print1("The altar crumbles...");
	    Level->site[ox][oy].locchar = RUBBLE;
	    Level->site[ox][oy].p_locf = L_RUBBLE;
	    lset(ox, oy, CHANGED);
	    morewait();
	    if (Player.rank[PRIESTHOOD]) {
	      print2("You sense your deity's pleasure with you.");
	      morewait();
	      print3("You are surrounded by a golden glow.");
	      cleanse(1);
	      heal(10);
	    }
	    gain_experience(500);
	  }
	}
      }
      else {
	print3("You restrain yourself from total silliness.");
	setgamestatus(SKIP_MONSTERS);
      }
    }
  }
}


/* attempt destroy an item */
void bash_item()
{
  int item;
  pob obj;

  clearmsg();
  print1("Destroy an item --");
  item = getitem(NULL_ITEM);
  if (item == CASHVALUE) print3("Can't destroy cash!");
  else if (item != ABORT) {
    obj = Player.possessions[item];
    if (Player.str+random_range(20) > obj->fragility+random_range(20)) {
      if (damage_item(obj) && Player.alignment < 0) {
	print2("That was fun....");
	gain_experience(obj->level * obj->level * 5);
      }
    }
    else {
      if (obj->objchar == WEAPON) {
	print2("The weapon turned in your hand -- you hit yourself!");
	p_damage(random_range(obj->dmg+abs(obj->plus)),
		 NORMAL_DAMAGE,
		 "a failure at vandalism");
      }
      else if (obj->objchar == ARTIFACT) {
	print2("Uh Oh -- Now you've gotten it angry....");
	p_damage(obj->level*10,
		 UNSTOPPABLE,
		 "an enraged artifact");
      }
      else {
	print2("Ouch! Damn thing refuses to break...");
	p_damage(1,UNSTOPPABLE,"a failure at vandalism");
      }
    }
  }
}


/* guess what this does */
/* if force is true, exiting due to some problem - don't bomb out */
void save(compress, force)
int compress, force;
{
  char fname[100];
  int pos, ok = TRUE;

  clearmsg();
  if (gamestatusp(ARENA_MODE)) {
    if (force) {
      resetgamestatus(ARENA_MODE);
      change_environment(E_CITY);
    }
    else {
      print3("Can't save the game in the arena!");
      setgamestatus(SKIP_MONSTERS);
      ok = FALSE;
    }
  }
  else if (Current_Environment == E_ABYSS) {
    if (force)
      change_environment(E_COUNTRYSIDE);
    else {
      print3("Can't save the game in the Adept's Challenge!");
      setgamestatus(SKIP_MONSTERS);
      ok = FALSE;
    }
  }
  else if (Current_Environment == E_TACTICAL_MAP) {
    if (force)
      change_environment(E_COUNTRYSIDE);
    else {
      print3("Can't save the game in the tactical map!");
      setgamestatus(SKIP_MONSTERS);
      ok = FALSE;
    }
  }
  if (!force && ok) {
    print1("Confirm Save? [yn] ");
    ok = (ynq1() == 'y');
  }
  if (force || ok) {
    print1("Enter savefile name: ");
    strcpy(fname,msgscanstring());
    if (fname[0] == '\0') {
      print1("No save file entered - save aborted.");
      ok = FALSE;
    }
#ifdef MSDOS
    for (pos = 0; fname[pos] && isalnum(fname[pos]); pos++)
      ;
#else
    for (pos = 0; fname[pos] && isprint(fname[pos]) && !isspace(fname[pos]);
      pos++)
      ;
#endif
    if (fname[pos]) {
      sprintf(Str1, "Illegal character '%c' in filename - Save aborted.", fname[pos]);
      print1(Str1);
      ok = FALSE;
    }
#ifdef MSDOS
    if (strlen(fname) > 7)
    {
      print1("Save name longer than 7 characters - Save aborted.");
      ok = FALSE;
    }
#else
# ifdef SYSV
    if (strlen(fname) > 14 - EXT_LENGTH - 1)
    {
      sprintf(Str1, "Save name longer than %d characters - Save aborted.",
	14 - EXT_LENGTH - 1);
      print1(Str1);
      ok = FALSE;
    }
# endif
#endif
    if (ok)
      if (save_game(compress,fname)) {
	print3("Bye!");
	sleep(2);
	endgraf();
	exit(0);
      }
      else
	print1("Save Aborted.");
  }
  if (force) {
    morewait();
    clearmsg();
    print1("The game is quitting - you will lose your character.");
    print2("Try to save again? ");
    if (ynq2() == 'y')
      save(compress, force);
  }
  setgamestatus(SKIP_MONSTERS);	/* if we get here, we failed to save */
}

/* close a door */
void closedoor()
{
  int dir;
  int ox,oy;

  clearmsg();

  print1("Close --");
  dir = getdir();
  if (dir == ABORT)
    setgamestatus(SKIP_MONSTERS);
  else {
    ox = Player.x + Dirs[0][dir];
    oy = Player.y + Dirs[1][dir];
    if (Level->site[ox][oy].locchar == CLOSED_DOOR) {
      print3("That door is already closed!");
      setgamestatus(SKIP_MONSTERS);
    }
    else if (Level->site[ox][oy].locchar != OPEN_DOOR) {
      print3("You can't close that!");
      setgamestatus(SKIP_MONSTERS);
    }
    else Level->site[ox][oy].locchar = CLOSED_DOOR;
    lset(ox, oy, CHANGED);
  }
}

/* handle a h,j,k,l, etc. */
void moveplayer(dx,dy)
int dx,dy;
{
  if (p_moveable(Player.x+dx,Player.y+dy)) {
      
    if (Player.status[IMMOBILE] > 0) {
      resetgamestatus(FAST_MOVE);
      print3("You are unable to move");
    }
    else if ((Player.maxweight < Player.itemweight) && 
	     random_range(2) &&
	     (! Player.status[LEVITATING])) {
      if (gamestatusp(MOUNTED)) {
	print1("Your horse refuses to carry you and your pack another step!");
	print2("Your steed bucks wildly and throws you off!");
	p_damage(10,UNSTOPPABLE,"a cruelly abused horse");
	resetgamestatus(MOUNTED);
	summon(-1,HORSE);
      }
      else {
	p_damage(1,UNSTOPPABLE,"a rupture");
	print3("The weight of your pack drags you down. You can't move.");
      }
    }
    else {
      Player.x += dx;
      Player.y += dy;
      p_movefunction(Level->site[Player.x][Player.y].p_locf);
      
      /* causes moves to take effectively 30 seconds in town without
         monsters being sped up compared to player */
      if ((Current_Environment == E_CITY) ||
	  (Current_Environment == E_VILLAGE)) {
	twiddle = ! twiddle;
	if (twiddle) {
	  Time++;
	  if (Time % 10 == 0) tenminute_check();
	  else minute_status_check();
	}
      }

      /* this test protects against player entering countryside and still
      having effects from being on the Level, a kluge, but hey,... */

      if (Current_Environment != E_COUNTRYSIDE) {
	if (gamestatusp(FAST_MOVE))
	  if ((Level->site[Player.x][Player.y].things != NULL) ||
	      (optionp(RUNSTOP) && 
	       loc_statusp(Player.x,Player.y,STOPS)))
	    resetgamestatus(FAST_MOVE);
	if ((Level->site[Player.x][Player.y].things != NULL) &&
	    (optionp(PICKUP)))
	  pickup();
      }
    }
  }
  else if (gamestatusp(FAST_MOVE)) {
    drawvision(Player.x,Player.y);
    resetgamestatus(FAST_MOVE);
  }
}


/* handle a h,j,k,l, etc. */
void movepincountry(dx,dy)
int dx,dy;
{
  int i,takestime = TRUE;
  if ((Player.maxweight < Player.itemweight) && 
      random_range(2) &&
      (! Player.status[LEVITATING])) {
    if (gamestatusp(MOUNTED)) {
      print1("Your horse refuses to carry you and your pack another step!");
      print2("Your steed bucks wildly and throws you off!");
      p_damage(10,UNSTOPPABLE,"a cruelly abused horse");
      resetgamestatus(MOUNTED);
      morewait();
      print1("With a shrill neigh of defiance, your former steed gallops");
      print2("off into the middle distance....");
      if (Player.packptr != 0) {
	morewait();
	print1("You remember (too late) that the contents of your pack");
	print2("were kept in your steed's saddlebags!");
	for(i=0;i<MAXPACK;i++) {
	  if (Player.pack[i] != NULL)
	    free((char *) Player.pack[i]);
	  Player.pack[i] = NULL;
	}
	Player.packptr = 0;
	calc_melee();
      }
    }
    else {
      p_damage(1,UNSTOPPABLE,"a rupture");
      print3("The weight of your pack drags you down. You can't move.");
    }
  }
  else {
    if (gamestatusp(LOST)) {
      print3("Being lost, you strike out randomly....");
      morewait();
      dx = random_range(3)-1;
      dy = random_range(3)-1;
    }
    if (p_country_moveable(Player.x+dx,Player.y+dy)) {
      if (Player.status[IMMOBILE] > 0) 
	print3("You are unable to move");
      else {
	Player.x += dx;
	Player.y += dy;
	if ((! gamestatusp(MOUNTED))&&(Player.possessions[O_BOOTS] != NULL)) {
	  if (Player.possessions[O_BOOTS]->usef == I_BOOTS_7LEAGUE) {
	    takestime = FALSE;
	    if (Player.possessions[O_BOOTS]->blessing < 0) {
	      print1("Whooah! -- Your boots launch you into the sky....");
	      print2("You come down in a strange location....");
	      Player.x = random_range(WIDTH);
	      Player.y = random_range(LENGTH);
	      morewait();
	      clearmsg();
	      print1("Your boots disintegrate with a malicious giggle...");
	      dispose_lost_objects(1,Player.possessions[O_BOOTS]);
	    }
	    else if (Player.possessions[O_BOOTS]->known != 2) {
	      print1("Wow! Your boots take you 7 leagues in a single stride!");
	      Player.possessions[O_BOOTS]->known = 2;
	    }
	  }
	}
	if (gamestatusp(LOST) && (Precipitation < 1) &&
	  c_statusp(Player.x, Player.y, SEEN)) {
	  print3("Ah! Now you know where you are!");
	  morewait();
	  resetgamestatus(LOST);
	}
	else if (gamestatusp(LOST)) {
	  print3("You're still lost.");
	  morewait();
	}
	if (Precipitation > 0) Precipitation--;
	c_set(Player.x, Player.y, SEEN);
	terrain_check(takestime);
      }
    }
  }
}

