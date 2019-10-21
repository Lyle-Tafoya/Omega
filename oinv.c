/* omega copyright (C) by Laurence Raphael Brothers, 1987,1988 */
/* oinv.c */
/* functions having to do with player item inventory */

#include "oglob.h"


/* drops money, heh heh */
void drop_money()
{
  pob money;

  money = detach_money();
  if (money != NULL) {
    if (Current_Environment == E_CITY) {
      print1("As soon as the money leaves your hand,");
      print2("a horde of scrofulous beggars snatch it up and are gone!");
    }
    else drop_at(Player.x,Player.y,money);
  }
  else setgamestatus(SKIP_MONSTERS);
}



/* returns some money from player back into "money" item.
   for giving and dropping money */
pob detach_money()
{
  int c;
  pob cash=NULL;
  c = get_money(Player.cash);
  if (c != ABORT) {
    Player.cash -= c;
    cash = ((pob) malloc(sizeof(objtype)));
    make_cash(cash,difficulty());
    cash->basevalue = c;
  }
  return(cash);
}


/* gets a legal amount of money or ABORT */
int get_money(limit)
int limit;
{
  int c;
  print1("How much? ");
  c = parsenum();
  if (c > limit) {
    print3("Forget it, buddy.");
    return(ABORT);
  }
  else return(c);
}



/* pick up from some location x,y */
/* Lift entire itemlist off ground, pass it to inventory control, which
   may drop things back onto the now null ground */
void pickup_at(x,y)
int x,y;
{
  int quit = FALSE;
  char response;
  pol ol = Level->site[x][y].things;
  pol temp;

  resetgamestatus(FAST_MOVE);

  Level->site[x][y].things = NULL;

  while (ol != NULL) {
    if (! quit) {
      clearmsg1();
      print1("Pick up: ");
      nprint1(itemid(ol->thing));
      nprint1(" [ynq]: ");
      response = ynq1();
      quit = (response == 'q');
    }
    if (response == 'y') gain_item(ol->thing);
    else drop_at(Player.x,Player.y,ol->thing);
    temp = ol;
    ol = ol->next;
    temp->thing = NULL;
    temp->next = NULL;
    free((char *) temp);
  }
}


/* put all of o on objlist at x,y on Level->depth */
/* Not necessarily dropped by character; just dropped... */
void drop_at(x,y,o)
int x,y;
pob o;
{
  pol tmp;
  pob cpy; 
  
  if (Current_Environment != E_COUNTRYSIDE) 
    if ((Level->site[x][y].locchar != ' ') &&
	(Level->site[x][y].locchar != '0')) {
      cpy = ((pob) malloc(sizeof(objtype)));
      tmp = ((pol) malloc(sizeof(oltype)));
      *cpy = *o;
      cpy->used = FALSE;
      tmp->thing = cpy;
      tmp->next = Level->site[x][y].things;
      Level->site[x][y].things = tmp;
    }
  else if (Level->site[x][y].p_locf == L_VOID_STATION)
    setgamestatus(PREPARED_VOID);
}  

/* put n of o on objlist at x,y on Level->depth */
void p_drop_at(x,y,n,o)
int x,y;
int n;
pob o;
{
  pol tmp; 
  if (Current_Environment != E_COUNTRYSIDE) 
    if ((Level->site[x][y].locchar != ' ') &&
	(Level->site[x][y].locchar != '0')) {
      tmp = ((pol) malloc(sizeof(oltype)));
      tmp->thing = ((pob) malloc(sizeof(objtype)));
      *(tmp->thing) = *o;
      tmp->thing->used = FALSE;
      tmp->thing->number = n;
      print2("Dropped ");
      nprint2(itemid(tmp->thing));
      morewait();
      tmp->next = Level->site[x][y].things;
      Level->site[x][y].things = tmp;
    }
    else if (Level->site[x][y].p_locf == L_VOID_STATION)
      setgamestatus(PREPARED_VOID);
}  


/* returns a string for identified items */
char *itemid(obj)
pob obj;
{
  char tstr[80];
  if (obj->objchar==CASH){
    strcpy(Str4,obj->truename);
    return(Str4);
  }
  else {
    if (Objects[obj->id].known > obj->known)
      obj->known = Objects[obj->id].known;
    
    setnumstr(obj,tstr);
    strcpy(Str4,tstr);
    if (obj->known == 0)
      strcat(Str4,obj->objstr);
    else if (obj->known == 1)
      strcat(Str4,obj->truename);
    else {
      if (obj->blessing < 0) {
	strcat(Str4, "cursed ");
	strcat(Str4, obj->cursestr);
      }
      else if (obj->blessing > 0) {
	strcat(Str4, "blessed ");
	strcat(Str4, obj->truename);
      }
      else strcat(Str4,obj->truename);
      if (obj->number > 1) strcat(Str4,"s");
      switch (obj->objchar) {
      case STICK: 
        setchargestr(obj,tstr);
	strcat(Str4,tstr);
	break;
      case MISSILEWEAPON:
      case ARMOR:
      case RING:
      case SHIELD:
      case WEAPON: 
	setplustr(obj,tstr);
	strcat(Str4, tstr);
	break;
      default: strcat(Str4,""); break;
      }
    }
    return(Str4);
  }
}

char *cashstr()
{
  if (difficulty() < 3) return("copper pieces");
  else if (difficulty() < 5) return("silver pieces");
  else if (difficulty() < 7) return("gold pieces");
  else if (difficulty() < 8) return("semiprecious gems");
  else if (difficulty() < 9) return("mithril pieces");
  else if (difficulty() < 10) return("precious gems"); 
  else return("orichalc pieces");
}

/* return an object's plus as a string */
void setplustr(obj,pstr)
pob obj;
char *pstr;
{
  pstr[0] = ' ';
  pstr[1] = (obj->plus < 0 ? '-' : '+' );
  if (abs(obj->plus) < 10) {
    pstr[2] = '0' + abs(obj->plus);
    pstr[3] = 0;
  }
  else {
    pstr[2] = '0' + abs(obj->plus / 10);
    pstr[3] = '0' + abs(obj->plus % 10);
    pstr[4] = 0;
  }
}

/* return an object's number as a string */
void setnumstr(obj,nstr)
pob obj;
char *nstr;
{
  if (obj->number < 2)
    nstr[0] = 0;
  else if (obj->number < 10) {
    nstr[0] = '0' + obj->number;
    nstr[1] = 'x';
    nstr[2] = ' ';
    nstr[3] = 0;
  }
  else if (obj->number < 41) {
    nstr[0] = '0' + ((int)(obj->number / 10));
    nstr[1] = '0' + (obj->number % 10);
    nstr[2] = 'x';
    nstr[3] = ' ';
    nstr[4] = 0;
  }
  else strcpy(nstr,"lots of ");
}




/* return object with charges */
void setchargestr(obj,cstr)
pob obj;
char *cstr;
{
  cstr[0] = ' ';
  cstr[1] = '[';
  if (obj->charge < 0) {
    cstr[2]='d';
    cstr[3]='e';
    cstr[4]='a';
    cstr[5]='d';
    cstr[6]=']';
    cstr[7]=0;
  }
  else if (obj->charge < 10) {
    cstr[2] = '0' + obj->charge;
    cstr[3] = ']';
    cstr[4] = 0;
  }
  else {
    cstr[2] = '0' + ((int)(obj->charge / 10));
    cstr[3] = '0' + (obj->charge % 10);
    cstr[4] = ']';
    cstr[5] = 0;
  }
}


void give_money(m)
struct monster *m;
{
  pob cash;

  cash = detach_money();
  if (cash == NULL)
    setgamestatus(SKIP_MONSTERS);
  else givemonster(m,cash);
}
    
    

void givemonster(m,o)
struct monster *m;
struct object *o;
{
  /* special case -- give gem to LawBringer */
  if ((m->id == ML10+2) && (o->id == ARTIFACTID+21)) {
    clearmsg();
    print1("The LawBringer accepts the gem reverently.");
    print2("He raises it above his head, where it bursts into lambent flame!");
    morewait();
    print1("You are bathed in a shimmering golden light.");
    print2("You feel embedded in an infinite matrix of ordered energy.");
    morewait();
    if (Imprisonment > 0)
      Imprisonment = 0;
    if (Player.rank[ORDER] == -1) {
      print2("You have been forgiven. You feel like a Paladin....");
      Player.rank[ORDER] = 1;
    }
    Player.alignment += 200;
    Player.pow = Player.maxpow = Player.pow * 2;
    gain_experience(2000);
    setgamestatus(GAVE_STARGEM);
  }
  else {
    if (m->uniqueness == COMMON) {
      strcpy(Str3,"The ");
      strcat(Str3,m->monstring);
    }
    else strcpy(Str3,m->monstring);
    
    if (m_statusp(m,GREEDY) || m_statusp(m,NEEDY)) {
      m_pickup(m,o);
      strcat(Str3," takes your gift");
      print1(Str3);
      Player.alignment++;
      if (m_statusp(m,GREEDY) && (true_item_value(o) < m->level*100))
      nprint1("...but does not appear satisfied.");
      else if (m_statusp(m,NEEDY) && 
	       (true_item_value(o) < Level->depth*Level->depth))
	nprint1("...and looks chasteningly at you.");
      else {
	nprint1("...and seems happy with it.");
	m_status_reset(m,HOSTILE);
	m_status_reset(m,GREEDY);
	m_status_reset(m,NEEDY);
      }
    }
    else if (m_statusp(m,HUNGRY)) {
      
      if (((m->id == HORSE) && (o->id == FOODID+15)) || /* grain */
	  ((m->id != HORSE) &&
	   ((o->usef == I_FOOD) || (o->usef == I_POISON_FOOD)))) {
	strcat(Str3," wolfs down your food ... ");
	print1(Str3);
	m_status_reset(m,HUNGRY);
	m_status_reset(m,HOSTILE);
	if  (o->usef == I_POISON_FOOD) {
	  Player.alignment -= 2;
	  nprint1("...and chokes on the poisoned ration!");
	  morewait();
	  m_status_set(m,HOSTILE);
	  m_damage(m,100,POISON);
	}
	else nprint1("...and now seems satiated.");
	morewait();
	free((char *)o);
      }
      else {
	strcat(Str3," spurns your offering and leaves it on the ground.");
	print1(Str3);
	drop_at(m->x,m->y,o);
      }
    }
    else {
      strcat(Str3," doesn't care for your offering and drops it.");
      print1(Str3);
      drop_at(m->x,m->y,o);
    }
  }
}




/* will clear all, not just one of an object. */
void conform_lost_object(obj)
pob obj;
{
  if (obj != NULL) conform_lost_objects(obj->number,obj);
}



/* removes n of object from inventory; frees object if appropriate. */

void dispose_lost_objects(n,obj)
int n;
pob obj;
{
  int i,conformed=FALSE,subtracted=FALSE,freetrue,freecurse;
  if (obj != NULL) for(i=0;i<MAXITEMS;i++) 
    if (Player.possessions[i] == obj) {
      if (! subtracted) {
	obj->number -= n;
	subtracted = TRUE;
      }
      if (obj->number < 1) {
	if (!conformed) {
	  conform_unused_object(obj);
	  conformed = TRUE;
	}
	Player.possessions[i] = NULL;
      }
    }
  if (obj->number < 1) {
    freetrue=(obj->objstr != obj->truename);
    freecurse=((obj->objstr != obj->cursestr)&&
	       (obj->cursestr != obj->truename));
    free(obj->objstr);
    if (freetrue) free(obj->truename);
    if (freecurse) free(obj->cursestr);
    free((char *) obj);
  }
}




/* removes n of object from inventory without freeing object.
   Removes all instances of pointer (might be 2 handed weapon, etc) */
void conform_lost_objects(n,obj)
int n;
pob obj;
{
  int i,conformed=FALSE,subtracted=FALSE;
  if (obj != NULL) for(i=0;i<MAXITEMS;i++) 
    if (Player.possessions[i] == obj) {
      if (! subtracted) {
	obj->number -= n;
	subtracted = TRUE;
      }
      if (obj->number < 1) {
	if (!conformed) {
	  conform_unused_object(obj);
	  conformed = TRUE;
	}
	Player.possessions[i] = NULL;
      }
    }
}


/* clears unused possession */
void conform_unused_object(obj)
pob obj;
{
  if (obj->used) {
    obj->used = FALSE;
    item_use(obj);
  }
  calc_melee();
}


/* select an item from inventory */
/* if itype is NULL, any kind of item is acceptable.
   if itype is CASHVALUE, any kind of item or '$' (cash) is acceptable.
   if itype is FOOD, CORPSE or FOOD is acceptable, but only FOOD is
listed in the possibilities.
   if itype is any other object type (eg SCROLL, POTION, etc.), only
that type of item is acceptable or is listed */

int getitem(itype)
char itype;
{
  char invstr[64];
  char index;
  int i,k=0,ok=FALSE,drewmenu=FALSE,found=FALSE;

  found = ((itype == NULL) || ((itype == CASHVALUE) && (Player.cash > 0)));
  invstr[0]=0;
  for(i=1;i<MAXITEMS;i++)
    if (Player.possessions[i] != NULL)
      if ((itype == NULL) ||
	  (itype == CASHVALUE) ||
	  (Player.possessions[i]->objchar == itype) ||
	  ((itype == FOOD) && (Player.possessions[i]->objchar == CORPSE))) {
	   found = TRUE;
	   invstr[k++] = 'a'+i-1;
	   invstr[k] = 0;
	 }
  if ((itype == CASHVALUE) && found) {
    invstr[k++] = '$';
    invstr[k] = 0;
  }
  if (! found) {
    print3("Nothing appropriate.");
    return(ABORT);
  }
  else {
    print2("Select an item [");
    nprint2(invstr);
    nprint2(",?] ");
    while (! ok) {
      index = mcigetc();
      if (index == '?') {
	drewmenu = TRUE;
	menuclear();
	for (i=1;i<MAXITEMS;i++)
	  if (Player.possessions[i] != NULL)
	    if ((itype == NULL) || 
		(itype == CASHVALUE) ||
		(Player.possessions[i]->objchar == itype) ||
		((itype == FOOD) && 
		 (Player.possessions[i]->objchar == CORPSE)))
	      display_inventory_slot(i,FALSE);
      }
      else if (index == ESCAPE) ok = TRUE;
      else if (index == CASH) {
	if (itype == CASHVALUE) ok = TRUE;
	else {
	  print3("You cannot select cash now.");
	  ok = FALSE;
	}
      }
      else if (badobject(index) || (! strmem(index,invstr)))
	print3("Nope! Try again [? for inventory, ESCAPE to quit]:");
      else ok = TRUE;
    }
    if (drewmenu) xredraw();
    if (index == ESCAPE) return(ABORT);
    else if (index == CASH) return(CASHVALUE);
    else return(index+1-'a');
  }
}


/* true if the numerical index based on 'a' == 1 turns out to be either
out of the range of the possessions array or a null item */
int badobject(slotchar)
char slotchar;
{
  int slot = slotchar + 1 - 'a';
  if ((slot<1) || (slot >= MAXITEMS)) return(TRUE);
  else return(Player.possessions[slot] == NULL);
}


/* this takes the numerical index directly for the same effect as badobject*/
int baditem(slotnum)
int slotnum;
{
  if ((slotnum<1) || (slotnum >= MAXITEMS)) return(TRUE);
  else return(Player.possessions[slotnum] == NULL);
}
    


/* formerly add_item_to_pack */
void gain_item(o)
struct object *o;
{
  if (o->objchar == CASH) {
    print2("You gained some cash.");
    Player.cash += o->basevalue;
    dataprint();
  }
  else if (optionp(PACKADD)) {
    if (! get_to_pack(o)) {
      Player.possessions[O_UP_IN_AIR] = o;
      top_inventory_control();
    }
  }
  else {
    Player.possessions[O_UP_IN_AIR] = o;
    top_inventory_control();
  }
}


/* Adds item to pack list */
void add_to_pack(o)
pob o;
{
  if (Player.packptr >= MAXPACK) {
    print3("Your pack is full. The item drops to the ground.");
    drop_at(Player.x,Player.y,o);
  }
  else {
    Player.pack[Player.packptr++] = o;
    print3("Putting item in pack.");
  }
}


/* Adds item to pack list, maybe going into inventory mode if pack is full */
int get_to_pack(o)
pob o;
{
  if (Player.packptr >= MAXPACK) {
    print3("Your pack is full.");
    morewait();
    return(FALSE);
  }
  else {
    Player.pack[Player.packptr++] = o;
    print3("Putting item in pack.");
    return(TRUE);
  }
}



/* takes something from pack, puts to slot, 
or to 'up-in-air', one of which at least must be empty */
void take_from_pack(slot,display)
int slot,display;
{
  char response;
  int i,quit = FALSE,ok=TRUE,displayed=FALSE;
  pob item;
  if (Player.possessions[slot] != NULL) 
    slot = O_UP_IN_AIR;
  if (Player.possessions[slot] != NULL) 
    print3("slot is not empty!");
  else if (Player.packptr == 0)
    print3("Pack is empty!");
  else {
    do {
      ok = TRUE;
      print1("Enter pack slot letter, or ? to show pack, or ESCAPE to quit.");
      response = mgetc();
      if (response == '?') {
	display_pack();
	displayed = TRUE;
	ok = FALSE;
      }
      else if (response == ESCAPE) quit = TRUE;
      else{
	ok = ((response >= 'A') && (response < 'A'+Player.packptr));
	if (ok) ok = slottable(Player.pack[response-'A'],slot);
      }
    } while (! ok);
    if (! quit) {
      if (response - 'A' > 10) {
	print1("You begin to rummage through your pack.");
	morewait();
	Command_Duration += 10;
      }
      if (response - 'A' > 5) {
	print1("You search your pack for the item.");
	Command_Duration += 5;
	morewait();
      }
      print1("You take the item from your pack.");
      morewait();
      Command_Duration += 2;
      item = Player.possessions[slot] = Player.pack[response-'A'];
      for(i=response-'A';i<Player.packptr-1;i++)
	Player.pack[i] = Player.pack[i+1];
      Player.pack[--Player.packptr] = NULL;
      
      if (item_useable(item,slot)) {
	item->used = TRUE;
	item_use(item);
	morewait();
	if (item->number > 1) pack_extra_items(item);
      }
    }
  }
  if (displayed) {
    xredraw();
    if (display) display_possessions();
  }
}




/* General interface to inventory */
void item_inventory(topline)
int topline;
{
  if (topline) {
    menuclear();
    display_possessions();
    inventory_control();
  }
  else top_inventory_control();
}





/* inventory_control assumes a few setup things have been done,
   like displaying the slots, loading the O_UP_IN_AIR item, etc.

   Each action uses up a little time. If only inspection actions
   are taken, no time is used up. */


void inventory_control()
{
  int slot = 0,done=FALSE;
  char response;
  clearmsg3();
  do {
    checkclear();
    print1("Action [^l,d,e,p,s,t,x,>,<,?,ESCAPE]:");
    display_inventory_slot(slot,FALSE);
    if (slot == O_WEAPON_HAND)
      display_inventory_slot(O_READY_HAND,FALSE);
    else if (slot == O_READY_HAND)
      display_inventory_slot(O_WEAPON_HAND,FALSE);
    display_inventory_slot(O_UP_IN_AIR,FALSE);
    move_slot(slot,slot,MAXITEMS);
    response = mcigetc();

    switch(response) {
    case 12:
    case 18: /* ^l, ^r */
      menuclear();
      display_possessions();
      break;
    case 'd':
      if (Player.possessions[O_UP_IN_AIR] != NULL) 
	drop_from_slot(O_UP_IN_AIR);
      else if (Player.possessions[slot] != NULL) 
	drop_from_slot(slot);
      else print3("Nothing in selected slot!");
      Command_Duration++;
      break; 
    case 'p': 
      if (Player.possessions[slot] != NULL) 
	put_to_pack(slot);
      Command_Duration+=5;
      break;
    case 's':
      display_pack();
      morewait();
      xredraw();
      display_possessions();
      Command_Duration+=5;
      break;
    case 't': 
      take_from_pack(slot,TRUE); 
      Command_Duration+=5;
      break;
    case 'e':
      switch_to_slot(slot);
      Command_Duration+=2;
      break;
    case 'x':
      switch_to_slot(slot);
      Command_Duration+=2;
      done = (Player.possessions[O_UP_IN_AIR] == NULL);
      break;
    case 'j':
    case '>':
      slot = move_slot(slot,slot+1,MAXITEMS);
      break;
    case 'k':
    case '<':
      slot = move_slot(slot,slot-1,MAXITEMS);
      break;
    case '?':
      inv_help();
      display_possessions();
      break;
    case ESCAPE:
      if (Player.possessions[O_UP_IN_AIR] != NULL) {
	drop_at(Player.x,Player.y,Player.possessions[O_UP_IN_AIR]);
	Player.possessions[O_UP_IN_AIR] = NULL;
	print3("Object 'up in air' dropped.");
      }
      done = TRUE;
      break;
    }
    calc_melee();
    dataprint();
  } while (! done);
  xredraw();
}







/* same as inventory_control, but only uses msg window for i/o*/


void top_inventory_control()
{
  int slot = 0,done=FALSE,usedmenu=FALSE;
  char response;
  clearmsg3();
  do {
    clearmsg1();
    print1("Action [d,e,p,s,t,x,~,?,ESCAPE]:");
    print2("'Up in air': ");
    if (Player.possessions[O_UP_IN_AIR] == NULL) nprint2("NOTHING");
    else nprint2(itemid(Player.possessions[O_UP_IN_AIR]));
    response = mcigetc();

    switch(response) {
    case 'd':
      if (Player.possessions[O_UP_IN_AIR] != NULL) 
	drop_from_slot(O_UP_IN_AIR);
      else {
	slot = get_inventory_slot();
	if (Player.possessions[slot] != NULL) 
	  drop_from_slot(slot);
	else print3("Nothing in selected slot!");
      }
      Command_Duration++;
      break; 
    case 'p': 
      if (Player.possessions[O_UP_IN_AIR] == NULL)
	slot = get_inventory_slot();
      else slot = O_UP_IN_AIR;
      put_to_pack(slot); 
      Command_Duration+=5;
      break;
    case 's':
      display_pack();
      usedmenu = TRUE;      
      Command_Duration+=5;
      break;
    case 't': 
      slot = get_inventory_slot();
      take_from_pack(slot,FALSE); 
      Command_Duration+=5;
      break;
    case 'e':
      slot = get_inventory_slot();
      switch_to_slot(slot);
      Command_Duration+=2;
      break;
    case 'x':
      slot = get_inventory_slot();
      switch_to_slot(slot);
      Command_Duration+=2;
      done = (Player.possessions[O_UP_IN_AIR] == NULL);
      break;
    case '~':
      menuclear();
      display_possessions();
      inventory_control();
      usedmenu = TRUE;
      done = TRUE;
      break;
    case '?':
      inv_help();
      usedmenu=TRUE;
      break;
    case ESCAPE:
      if (Player.possessions[O_UP_IN_AIR] != NULL) {
	drop_at(Player.x,Player.y,Player.possessions[O_UP_IN_AIR]);
	Player.possessions[O_UP_IN_AIR] = NULL;
	print3("Object 'up in air' dropped.");
      }
      done = TRUE;
      break;
    }
    calc_melee();
    dataprint();
  } while (! done);
  if (usedmenu) 
    xredraw();
}





int get_inventory_slot()
{
  int ok;
  char response;
  do {
    clearmsg1();
    print1("Which inventory slot [a..o,*='up-in-air' slot]?");
    response = mcigetc(); 
    ok = ((response == '*') ||
	  ((response >= 'a') && (response < 'a' + MAXITEMS - 1)));
  } while (! ok);
  return((response == '*') ? 0 : ((int) (response + 1 - 'a')));
}
    


/* returns some number between 0 and o->number */
int get_item_number(o)
pob o;
{
  int n=0;
  do {
    print1("How many? -- max");
    mnumprint(o->number);
    nprint1(" :");
    n = parsenum();
    if (n>o->number) print3("Too many!");
    else if (n<1) n = 0;
  } while (n > o->number);
  if (n < 1) n = 0;
  return(n);
}
  
void drop_from_slot(slot)
int slot;
{
  int n,waitflag;
  if (Player.possessions[slot] != NULL) {
    n = Player.possessions[slot]->number;
    if (n > 1) n = get_item_number(Player.possessions[slot]);
    if (n > 0) {
      p_drop_at(Player.x,Player.y,n,Player.possessions[slot]);
      waitflag = (Player.possessions[slot]->used && 
		  (Player.possessions[slot]->number == n));
      conform_lost_objects(n,Player.possessions[slot]);
      if (waitflag) morewait();
    }
    else print3("Didn't drop anything.");
  }
  else print3("Didn't drop anything.");
}


void put_to_pack(slot)
int slot;
{
  int waitflag,num = 1;
  pob temp,oslot = Player.possessions[slot];
  if (oslot == NULL) 
    print3("Slot is empty!");
  else if (cursed(oslot) == TRUE+TRUE)
    print3("Item is cursed!");
  else {
    if (oslot->number > 1)
      num = get_item_number(oslot);
    if (num > 0) {
      temp = split_item(num,oslot);
      waitflag = (oslot->used && (oslot->number == num));
      conform_lost_objects(num,oslot);
      if (waitflag) morewait();
      add_to_pack(temp);
    }
  }
}


/* splits num off of item to make newitem which is returned */
/* something else (conform_lost_objects) has to reduce the actual
   number value of item and Player.itemweight */
pob split_item(num,item)
int num;
pob item;
{
  pob newitem=NULL;
  if (item != NULL) {
    newitem = ((pob) malloc(sizeof(objtype)));
    *newitem = *item;
    if (num <= item->number)
      newitem->number = num;
    /* else num > item->number, so return NULL on this error condition */
  }
  return(newitem);
}



/* Trades contents of "up in air" slot with selected slot. One or both
may be null. If both slots are 'objequal' merges two groups into one
in the selected slot. If one slot is null and the number of the other
is greater than one, requests how many to move. */

void switch_to_slot(slot)
int slot;
{
  pob oslot = Player.possessions[slot];
  pob oair = Player.possessions[O_UP_IN_AIR];
  pob otemp = NULL;
  int slotnull,airnull,num=1,trade=FALSE,put=FALSE,take=FALSE,merge=FALSE;
  int s2h=FALSE,a2h=FALSE;

  /* ie, is cursed and in use */
  if (slot == O_UP_IN_AIR)
    print3("This action makes no sense!");
  else if (cursed(oslot)==TRUE+TRUE)
    print3("The object in that slot is cursed -- you can't get rid of it!");
  else {

    slotnull = (oslot == NULL); 
    airnull = (oair == NULL);

    if  (!slotnull)
      s2h = (Player.possessions[O_READY_HAND] ==
	     Player.possessions[O_WEAPON_HAND]);
    
    if (! airnull)
      a2h = (twohandedp(oair->id) && 
	     ((slot == O_READY_HAND) || (slot == O_WEAPON_HAND)));
    
    
    /* figure out which action to take */

    /* merge if both are same kind of object */
    merge = objequal(oslot,oair);
    
    take = ((!merge) && (!slotnull) && airnull);
    
    put = ((!merge) && slotnull && (!airnull));
    
    trade = ((!merge) && (!slotnull) && (!airnull) && slottable(oair,slot));
    
    if (merge) merge_item(slot);
    
    else if (put) {

      /* deal with a 2-handed weapon */
      if (a2h) {
	if (Player.possessions[O_READY_HAND] == NULL)
	  Player.possessions[O_READY_HAND] = oair;
	if (Player.possessions[O_WEAPON_HAND] == NULL)
	  Player.possessions[O_WEAPON_HAND] = oair;
      }
      else Player.possessions[slot] = oair;
      Player.possessions[O_UP_IN_AIR] = NULL;
      if (item_useable(oair,slot)) {
	oair->used = TRUE;
	item_use(oair);
	morewait();
	if (oair->number > 1) pack_extra_items(oair);
      }
      Player.possessions[O_UP_IN_AIR] = NULL;
    }

    else if (take) {
      if (oslot->number > 1) {
	num = get_item_number(oslot);
      }
      if (num > 0) {
	otemp = split_item(num,oslot);
	dispose_lost_objects(num,oslot);
	Player.possessions[O_UP_IN_AIR] = otemp;
	otemp->used = FALSE;
      }
    }

    else if (trade) {
      
      /* first remove item from slot */
      num = oslot->number;
      conform_lost_objects(oslot->number,oslot);
      oslot->number = num;
      
      Player.possessions[O_UP_IN_AIR] = oslot;

      Player.possessions[slot] = oair;
      
      if (a2h) {
	if (Player.possessions[O_READY_HAND] == NULL)
	  Player.possessions[O_READY_HAND] = oair;
	if (Player.possessions[O_WEAPON_HAND] == NULL)
	  Player.possessions[O_WEAPON_HAND] = oair;
      }
      
      if (item_useable(oair,slot)) {
	oair->used = TRUE;
	item_use(oair);
	morewait();
	if (oair->number > 1) pack_extra_items(oair);
      }
    }
  }
}




/* merges the up-in-air items into the selected items */

void merge_item(slot)
int slot;
{
  Player.possessions[slot]->number +=
    Player.possessions[O_UP_IN_AIR]->number;
  Player.possessions[O_UP_IN_AIR] = NULL;
}


/* are two objects equal except for their number field? */
/* returns false if either object is null */
int objequal(o,p)
struct object *o,*p;
{
  if ((o == NULL) || (p == NULL)) return(FALSE);
  else return(
	 (o->id == p->id) &&
	 (o->plus == p->plus) &&
	 (o->charge == 0) &&
	 (p->charge == 0) &&
	 (o->dmg == p->dmg) &&
	 (o->hit == p->hit) &&
	 (o->aux == p->aux) &&
	 (o->known == p->known) &&
	 (o->blessing == p->blessing) &&
	 (o->usef == p->usef)
	 );

}

/* criteria for being able to put some item in some slot */
int slottable(o,slot)
pob o;
int slot;
{
  int ok = TRUE;
  if (o == NULL) ok = FALSE;
  else if (slot == O_ARMOR) {
    if (o->objchar != ARMOR) {
      print3("Only armor can go in the armor slot!");
      ok = FALSE;
    }
  }
  else if (slot == O_SHIELD) {
    if (o->objchar != SHIELD) {
      print3("Only a shield can go in the shield slot!");
      ok = FALSE;
    }
  }
  else if (slot == O_BOOTS) {
    if (o->objchar != BOOTS) {
      print3("Only boots can go in the boots slot!");
      ok = FALSE;
    }
  }
  else if (slot == O_CLOAK) {
    if (o->objchar != CLOAK) {
      print3("Only a cloak can go in the cloak slot!");
      ok = FALSE;
    }
  }
  else if (slot >= O_RING1) {
    if (o->objchar != RING) {
      print3("Only a ring can go in a ring slot!");
      ok = FALSE;
    }
  }
  return(ok);
}



/* whether or not an item o can be used in a slot. Assumes o can in
   fact be placed in the slot. */
int item_useable(o,slot)
pob o;
int slot;
{
  /* don't have to check the object in the first if since only armor
  can go in armor slot, cloak in cloak slot, etc */

  if ((slot == O_ARMOR) || 
      (slot == O_CLOAK) ||
      (slot == O_SHIELD) ||
      (slot == O_BOOTS) ||
      (slot >= O_RING1))
    return(TRUE);

  /* weapon is useable if it is put in weapon hand or if it is two-handed
     and put in either hand when the other also holds the weapon */

  else if ((o->objchar == WEAPON) ||
	   (o->objchar == MISSILEWEAPON)) {
    if (twohandedp(o->id) && 
	((slot==O_READY_HAND)||(slot==O_WEAPON_HAND))) {
      if (Player.possessions[O_READY_HAND] == 
	  Player.possessions[O_WEAPON_HAND]) {
	print1("You heft the weapon and find you must use both hands.");
	morewait();
	return(TRUE);
      }
      else {
	print1("This weapon is two-handed, so at the moment, ");
	print2("you are just lugging it around....");
	morewait();
	return(FALSE);
      }	
    }
    else return(slot == O_WEAPON_HAND);
  }
  else return(FALSE);
}








/* returns FALSE if not cursed, TRUE if cursed but not used,
   TRUE + TRUE if cursed and used */
int cursed(obj)
pob obj;
{
  return((obj == NULL) ? 
	 FALSE : 
	 ((obj->blessing < 0) ? 
	  (obj->used == TRUE) + TRUE : 
	  FALSE));
}




/* returns true if item with id and charge is found in pack or in
   inventory slot. charge is used to differentiate
   corpses instead of aux, which is their food value. */
int find_item(o,id,chargeval)
int id,chargeval;
pob *o;
{
  int i,found=FALSE;
  *o=NULL;
  for(i=1;((i<MAXITEMS)&&(! found));i++)
    if (Player.possessions[i] != NULL)
      if ((Player.possessions[i]->id == id) &&
	  ((chargeval == -1) ||
	   (Player.possessions[i]->charge == chargeval))) {
	*o = Player.possessions[i];
	found = TRUE;
      }
  if (! found)
    for(i=0;((i<Player.packptr)&&(! found));i++)
      if (Player.pack[i] != NULL)
	if ((Player.pack[i]->id == id) &&
	    ((chargeval == -1) ||
	     (Player.pack[i]->charge == chargeval))) {
	  *o = Player.pack[i];
	  found = TRUE;
	}
  return(found);
}



/* returns true if item with id and charge is found in pack or in
   inventory slot. Destroys item. charge is used to differentiate
   corpses instead of aux, which is their food value. */
int find_and_remove_item(id,chargeval)
int id,chargeval;
{
  int i,found=FALSE;
  pob o=NULL;

  for(i=1;((i<MAXITEMS)&&(! found));i++)
    if (Player.possessions[i] != NULL)
      if ((Player.possessions[i]->id == id) &&
	  ((chargeval == -1) ||
	   (Player.possessions[i]->charge == chargeval))) {
	o = Player.possessions[i];
	conform_lost_object(o);
	found = TRUE;
      }
  if (! found) for(i=0;((i<Player.packptr)&&(! found));i++)
    if (Player.pack[i] != NULL)
      if ((Player.pack[i]->id == id) &&
	  ((chargeval == -1) ||
	   (Player.pack[i]->charge == chargeval))) {
	free((char *)Player.pack[i]);
	Player.pack[i] = NULL;
	found = TRUE;
      }
  fixpack();
  return(found);
}




void lose_all_items()
{
  int i;
  print1("You notice that you are completely devoid of all possessions.");
  morewait();
  for(i=0;i<MAXITEMS;i++)
    if (Player.possessions[i] != NULL) {
      dispose_lost_objects(Player.possessions[i]->number,
			   Player.possessions[i]);
      Player.possessions[i] = NULL;
    }
  for(i=0;i<MAXPACK;i++) {
    if (Player.pack[i] != NULL) 
      free((char *) Player.pack[i]);
    Player.pack[i] = NULL;
  }
  Player.packptr = 0;
  calc_melee();
  morewait();
}


/* prevents people from wielding 3 short swords, etc. */
void pack_extra_items(item)
pob item;
{
  pob extra=((pob) malloc(sizeof(objtype)));
  if (Player.packptr < MAXPACK) {
    print3("Putting extra items back in pack.");
    morewait();
    *extra = *item;
    extra->number = item->number-1;
    item->number = 1;
    Player.pack[Player.packptr++] = extra;
  }
  else if (Player.possessions[O_UP_IN_AIR] == NULL) {
    print3("Extra copies of item are 'up in the air'");
    Player.possessions[O_UP_IN_AIR] = extra;
  }
  else {
    print3("No room for extra copies of item -- dropping them.");
    drop_at(Player.x,Player.y,extra);
  }
  calc_melee();
}


/* makes sure Player.pack is OK, (used after sale from pack) */
void fixpack()
{
  pob tpack[MAXPACK];
  int i,tctr=0;
  for(i=0;i<MAXPACK;i++) tpack[i] = NULL;
  for(i=0;i<MAXPACK;i++)
    if (Player.pack[i]!=NULL) 
      tpack[tctr++] = Player.pack[i];
  for(i=0;i<MAXPACK;i++)
    Player.pack[i]=tpack[i];
  Player.packptr = tctr;

}
