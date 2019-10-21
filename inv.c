/* omega copyright (C) by Laurence Raphael Brothers, 1987,1988,1989 */
/* inv.c */
/* functions having to do with player item inventory */

#ifdef MSDOS_SUPPORTED_ANTIQUE
# include "curses.h"
#else
# ifdef AMIGA
#  include <curses210.h>
# else
#  include <curses.h>
# endif
#endif

#include "glob.h"

/* drops money, heh heh */
void drop_money()
{
  pob money;

  /* WDT HACK!  Let me guess -- this is yet another memory leak, right? */
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
  long c;
  pob cash=NULL;
  c = get_money(Player.cash);
  if (c != ABORT) {
    Player.cash -= c;
    cash = ((pob) checkmalloc(sizeof(objtype)));
    make_cash(cash,difficulty());
    cash->basevalue = c;
  }
  return(cash);
}


/* gets a legal amount of money or ABORT */
long get_money(limit)
long limit;
{
  long c;
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
    else drop_at(x,y,ol->thing);
    temp = ol;
    ol = ol->next;
    temp->thing = NULL;
    temp->next = NULL;
    free((char *) temp);
  }
}


/* WDT -- convert from a char (keypress) to an item index in
 * player inventory */
/* Item identifiers, in this case the letters of the alphabet minus
 * any letters already used for commands.  Yes, there are more here
 * than could be needed, but I don't want to short myself for later.
 */
signed char inventory_keymap[] = "-abcfghimnoqruvwyz";
int key_to_index(key)
signed char key;
{
  int i;
  assert( MAXITEMS>0 ); /* must have room for an item, or this loop will
                         * die! */
  
  for (i=0; i<MAXITEMS; i++) {
    if (key == inventory_keymap[i])
      return (signed char)i;
  }
  return O_UP_IN_AIR;
}

signed char index_to_key(index)
signed int index;
{
  if ( index < MAXITEMS )
    return inventory_keymap[index];
  else return '-';
}


/* criteria for being able to put some item in some slot */
/* WDT -- why on earth does the 'slottable' function print stuff???? */
int aux_slottable(o,slot)
pob o;
int slot;
{
  int ok = TRUE;
  if (o == NULL) ok = FALSE;
  else if (slot == O_ARMOR) {
    if (o->objchar != ARMOR) {
      ok = FALSE;
    }
  }
  else if (slot == O_SHIELD) {
    if (o->objchar != SHIELD) {
      ok = FALSE;
    }
  }
  else if (slot == O_BOOTS) {
    if (o->objchar != BOOTS) {
      ok = FALSE;
    }
  }
  else if (slot == O_CLOAK) {
    if (o->objchar != CLOAK) {
      ok = FALSE;
    }
  }
  else if (slot >= O_RING1) {
    if (o->objchar != RING) {
      ok = FALSE;
    }
  }
  return(ok);
}

/* put all of o on objlist at x,y on Level->depth */
/* Not necessarily dropped by character; just dropped... */
void drop_at(x,y,o)
int x,y;
pob o;
{
  pol tmp;
  pob cpy; 
  
  if (Current_Environment != E_COUNTRYSIDE) {
    if ((Level->site[x][y].locchar != VOID_CHAR) &&
	(Level->site[x][y].locchar != ABYSS)) {
      cpy = ((pob) checkmalloc(sizeof(objtype)));
      tmp = ((pol) checkmalloc(sizeof(oltype)));
      *cpy = *o;
      cpy->used = FALSE;
      tmp->thing = cpy;
      tmp->next = Level->site[x][y].things;
      Level->site[x][y].things = tmp;
    }
    else if (Level->site[x][y].p_locf == L_VOID_STATION)
      setgamestatus(PREPARED_VOID);
  }
}  

/* put n of o on objlist at x,y on Level->depth */
void p_drop_at(x,y,n,o)
int x,y;
int n;
pob o;
{
  pol tmp; 
  if (Current_Environment != E_COUNTRYSIDE) 
    if ((Level->site[x][y].locchar != VOID_CHAR) &&
	(Level->site[x][y].locchar != ABYSS)) {
      tmp = ((pol) checkmalloc(sizeof(oltype)));
      tmp->thing = ((pob) checkmalloc(sizeof(objtype)));
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
    else if (obj->known == 1) {
      if (obj->id == ARTIFACTID + 8 || obj->id == ARTIFACTID + 20 ||
	obj->id == ARTIFACTID + 21)
	strcat(Str4, "the ");
      strcat(Str4,obj->truename);
    }
    else {
      if (obj->id == ARTIFACTID + 8 || obj->id == ARTIFACTID + 20 ||
	obj->id == ARTIFACTID + 21)
	strcat(Str4, "the ");
      if (obj->usef == I_NOTHING && Objects[obj->id].usef != I_NOTHING)
	strcat(Str4, "disenchanted ");
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
  if ((m->id == LAWBRINGER) && (o->id == ARTIFACTID+21)) {
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
    /* WDT HACK!!!  Where else would this ever get freed?? */
    free(o);
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
      if (m_statusp(m,GREEDY) && (true_item_value(o) < (long) m->level*100))
      nprint1("...but does not appear satisfied.");
      else if (m_statusp(m,NEEDY) && 
	       (true_item_value(o) < (long) Level->depth*Level->depth))
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
  int i,conformed=FALSE,subtracted=FALSE;

  if (obj == NULL)
    return;
  for(i=0;i<MAXITEMS;i++) 
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
  if (obj->number < 1)
    free((char *) obj);
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
/* if itype is NULL_ITEM, any kind of item is acceptable.
   if itype is CASH, any kind of item or '$' (cash) is acceptable.
   if itype is FOOD, CORPSE or FOOD is acceptable, but only FOOD is
listed in the possibilities.
   if itype is any other object type (eg SCROLL, POTION, etc.), only
that type of item is acceptable or is listed */

int getitem(itype)
Symbol itype;
{
  char invstr[64];
  char key;
  int i,k=0,ok=FALSE,drewmenu=FALSE,found=FALSE;

  found = ((itype == NULL_ITEM) || ((itype == CASH) && (Player.cash > 0)));
  invstr[0]=0;
  for(i=1;i<MAXITEMS;i++)
    if (Player.possessions[i] != NULL)
      if ((itype == NULL_ITEM) ||
	  (itype == CASH) ||
	  (Player.possessions[i]->objchar == itype) ||
	  ((itype == FOOD) && (Player.possessions[i]->objchar == CORPSE))) {
	   found = TRUE;
	   invstr[k++] = index_to_key(i);
	   invstr[k] = 0;
	 }
  if ((itype == CASH) && found) {
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
      key = (char) mcigetc();
      if (key == '?') {
	drewmenu = TRUE;
	for (i=1;i<MAXITEMS;i++)
	  if (Player.possessions[i] != NULL)
	    if ((itype == NULL_ITEM) || 
		(itype == CASH) ||
		(Player.possessions[i]->objchar == itype) ||
		((itype == FOOD) && 
		 (Player.possessions[i]->objchar == CORPSE)))
	      display_inventory_slot(i,FALSE);
      }
      else if (key == ESCAPE) ok = TRUE;
      else if (key == (CASH&0xff)) {
	if (itype == CASH) ok = TRUE;
	else {
	  print3("You cannot select cash now.");
	  ok = FALSE;
	}
      }
      else if ( !strmem(key,invstr) || key_to_index(key)==(signed char)-1 )
	print3("Nope! Try again [? for inventory, ESCAPE to quit]:");
      else ok = TRUE;
    }
    if (drewmenu) xredraw();
    if (key == ESCAPE) return(ABORT);
    else if (key == (CASH&0xff)) return(CASHVALUE);
    else return key_to_index(key);
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


#ifndef MSDOS_SUPPORTED_ANTIQUE
/* this takes the numerical index directly for the same effect as badobject*/
int baditem(slotnum)
int slotnum;
{
  if ((slotnum<1) || (slotnum >= MAXITEMS)) return(TRUE);
  else return(Player.possessions[slotnum] == NULL);
}
#endif
    


/* formerly add_item_to_pack */
void gain_item(o)
struct object *o;
{
  if (o->uniqueness == UNIQUE_MADE)
    Objects[o->id].uniqueness = UNIQUE_TAKEN;
  if (o->objchar == CASH) {
    print2("You gained some cash.");
    Player.cash += o->basevalue;
    free((char *)o);
    dataprint();
  }
  else if (optionp(PACKADD)) {
    if (! get_to_pack(o)) {
      Player.possessions[O_UP_IN_AIR] = o;
      do_inventory_control();
    }
  }
  else {
    Player.possessions[O_UP_IN_AIR] = o;
    do_inventory_control();
  }
}

/* inserts the item at the start of the pack array */
void push_pack(o)
pob o;
{
  int i;
  for (i = Player.packptr; i > 0; i--)
    Player.pack[i] = Player.pack[i-1];
  Player.pack[0] = o;
  Player.packptr++;
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
    push_pack(o);
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
    push_pack(o);
    print3("Putting item in pack.");
    return(TRUE);
  }
}

int pack_item_cost(index)
{
  int cost;
  if (index > 20) {
    cost = 17;
  }
  else if (index > 15) {
    cost = 7;
  }
  else cost = 2;
  return cost;
}

/* WDT -- 'response' must be an index into the pack. */
int use_pack_item(response,slot)
int response,slot;
{
  pob item; int i;
  i = pack_item_cost(response);
  if (i > 10) {
    print1("You begin to rummage through your pack.");
    morewait();
  }
  if (i > 5) {
    print1("You search your pack for the item.");
    morewait();
  }
  print1("You take the item from your pack.");
  morewait();
  Command_Duration += i;
  item = Player.possessions[slot] = Player.pack[response];
  for(i=response;i<Player.packptr-1;i++)
      Player.pack[i] = Player.pack[i+1];
  Player.pack[--Player.packptr] = NULL;
  
  if ((slot == O_READY_HAND || slot == O_WEAPON_HAND) &&
      twohandedp(item->id))
  {
    if (Player.possessions[O_READY_HAND] == NULL)
        Player.possessions[O_READY_HAND] = item;
    if (Player.possessions[O_WEAPON_HAND] == NULL)
        Player.possessions[O_WEAPON_HAND] = item;
  }
  if (item_useable(item,slot)) {
    item->used = TRUE;
    item_use(item);
    morewait();
    if (item->number > 1) pack_extra_items(item);
  }
}

/* WDT HACK!  This ought to be in scr.c, along with its companion.  However,
 * right now it's only used in the function directly below. */
int aux_display_pack(start_item,slot)
int start_item,slot;
{
  int i=start_item, items;
  char *depth_string;
  if (Player.packptr < 1) print3("Pack is empty.");
  else if (Player.packptr <= start_item) print3("You see the leather at the bottom of the pack.");
  else {
    menuclear();
    items = 0;
    for(i=start_item;i<Player.packptr && items<ScreenLength-5;i++) {
      if ( aux_slottable(Player.pack[i],slot) ) {
	if (pack_item_cost(i) > 10)
          depth_string = "**";
        else if (pack_item_cost(i) > 5)
          depth_string = "* ";
        else depth_string = "  ";
        sprintf(Str1, "  %c: %s %s\n", i + 'a', depth_string,
		itemid(Player.pack[i]));
	if (items == 0)
	  menuprint("Items in Pack:\n");
        menuprint(Str1);
        items++;
      }
    }
    if (items == 0)
      menuprint("You see nothing useful for that slot in the pack.");
    else {
      menuprint("\n*: Takes some time to reach; **: buried very deeply.");
    }
    showmenu();
  }
  return i;
}

/* takes something from pack, puts to slot, 
or to 'up-in-air', one of which at least must be empty */
int aux_take_from_pack(slot)
int slot;
{
  char response,pack_item, last_item;
  int i,quit = FALSE,ok=TRUE;
  if (Player.possessions[slot] != NULL) 
    slot = O_UP_IN_AIR;
  if (Player.possessions[slot] != NULL) 
    print3("slot is not empty!");
  else if (Player.packptr < 1)
    print3("Pack is empty!");
  else {
    pack_item = 0;
    do {
      ok = TRUE;
      last_item = aux_display_pack(pack_item,slot);
      if (last_item == Player.packptr && pack_item == 0 )
        print1("Enter pack slot letter or ESCAPE to quit.");
      else if (last_item == Player.packptr)
        print1("Enter pack slot letter, - to go back, or ESCAPE to quit.");
      else if (pack_item == 0)
        print1("Enter pack slot letter, + to see more, or ESCAPE to quit.");
      else
        print1("Enter pack slot letter, + or - to see more, or ESCAPE to quit.");
      response = mcigetc();
      if (response == '?') {
	/* WDT HACK -- display some help instead. */
        print1("Help not implemented (sorry).");
	morewait();
	ok = FALSE;
      }
      else if (response == ESCAPE) quit = TRUE;
      else if (response == '+') {
	if (last_item < Player.packptr)
	  pack_item = last_item;
        ok = FALSE;
      }
      else if (response == '-') {
	/* WDT HACK: this _should_ make us page up.  Sadly,
	 * I have no way of calculating how much I'll be paging up.
	 * This is fixable, but I have no idea how much work... */
        pack_item = 0;
	ok = FALSE;
      }
      else{
	ok = ((response >= 'a') && (response < 'a'+Player.packptr));
	if (ok) ok = slottable(Player.pack[response-'a'],slot);
      }
    } while (! ok);
    if (! quit) {
      use_pack_item(response - 'a',slot);
    }
  }
  if ( optionp(TOPINV) )
    display_possessions();
  return slot;
}

/* takes something from pack, puts to slot, 
or to 'up-in-air', one of which at least must be empty */
int aux_top_take_from_pack(slot,display)
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
      response = mcigetc();
      if (response == '?') {
	display_pack();
	displayed = TRUE;
	ok = FALSE;
      }
      else if (response == ESCAPE) quit = TRUE;
      else{
	ok = ((response >= 'a') && (response < 'a'+Player.packptr));
	if (ok) ok = slottable(Player.pack[response-'a'],slot);
      }
    } while (! ok);
    if (! quit) use_pack_item(response - 'a',slot);
  }
  if (displayed) {
    if (display)
      display_possessions();
    else
      xredraw();
  }
  return slot;
}

int take_from_pack(slot,display)
int slot,display;
{
  if (optionp(TOPINV)) aux_top_take_from_pack(slot,display);
  else aux_take_from_pack(slot);
}


#ifndef MSDOS_SUPPORTED_ANTIQUE
/* General interface to inventory */
void item_inventory(topline)
int topline;
{
  if (topline) {
    display_possessions();
    inventory_control();
  }
  else top_inventory_control();
}
#endif


void do_inventory_control()
{
  if (optionp(TOPINV)) top_inventory_control();
  else {
    menuclear();
    display_possessions();
    inventory_control();
  }
}

/* inventory_control assumes a few setup things have been done,
   like displaying the slots, loading the O_UP_IN_AIR item, etc.

   Each action uses up a little time. If only inspection actions
   are taken, no time is used up. */


void inventory_control()
{
  int slot = 0,done=FALSE;
  int response;
  char letter;
#ifdef MSDOS_SUPPORTED_ANTIQUE
  int simple = 0;
#endif
  clearmsg3();
  checkclear();
  print1("Action [d,e,l,p,s,t,x,>,<,?,ESCAPE]:");
  show_inventory_slot(slot,FALSE);
  display_inventory_slot(O_UP_IN_AIR,FALSE);
  do {
    move_slot(slot,slot,MAXITEMS);
    response = mcigetc();

    switch(response) {
    case 12:
    case 18: /* ^l, ^r */
      display_possessions();
      break;
    case 'd':
      if (Player.possessions[O_UP_IN_AIR] != NULL) 
      {
	drop_from_slot(O_UP_IN_AIR);
	display_inventory_slot(O_UP_IN_AIR, FALSE);
      }
      else if (Player.possessions[slot] != NULL) 
      {
	drop_from_slot(slot);
	show_inventory_slot(slot, FALSE);
      }
      else print3("Nothing in selected slot!");
      Command_Duration++;
      break; 
    case 'l':
      Str1[0] = '\0';
      if (Player.possessions[slot] != NULL) {
	if (!strcmp(itemid(Player.possessions[slot]),
	  Player.possessions[slot]->objstr))
	  print3("You notice nothing new about it.");
	else {
	  if (Player.possessions[slot]->uniqueness == COMMON)
	    strcat(Str1, "Your ");
	  strcat(Str1, itemid(Player.possessions[slot]));
	  if (Player.possessions[slot]->objchar == BOOTS)
	    strcat(Str1, " look like ");
	  else {
	    strcat(Str1, " looks like a");
	    letter = Player.possessions[slot]->objstr[0];
	    if (letter == 'a' || letter == 'A' || letter == 'e' ||
	      letter == 'E' || letter == 'i' || letter == 'I' ||
	      letter == 'o' || letter == 'O' || letter == 'u' || letter == 'U')
	      strcat(Str1, "n ");
	    else
	      strcat(Str1, " ");
	  }
	  strcat(Str1, Player.possessions[slot]->objstr);
	  print3(Str1);
	}
      }
      else print3("Nothing in selected slot!");
      break;
    case 'p': 
      if (Player.possessions[slot] != NULL) 
      {
	put_to_pack(slot);
	show_inventory_slot(slot, FALSE);
      }
      Command_Duration+=5;
      break;
    case 's':
      display_pack();
      morewait();
      display_possessions();
      Command_Duration+=5;
      break;
    case 't': 
      show_inventory_slot(take_from_pack(slot,TRUE), FALSE);
      Command_Duration+=5;
      break;
    case 'e':
      switch_to_slot(slot);
      show_inventory_slot(O_UP_IN_AIR,FALSE);
      show_inventory_slot(slot,FALSE);
      Command_Duration+=2;
      break;
    case '\n':
    case 'x':
      switch_to_slot(slot);
      show_inventory_slot(O_UP_IN_AIR,FALSE);
      show_inventory_slot(slot,FALSE);
      Command_Duration+=2;
      done = (Player.possessions[O_UP_IN_AIR] == NULL);
      break;
    case 'j':
    case '>':
    case '2':
#if defined(KEY_DOWN)
    case KEY_DOWN:
#endif
      slot = move_slot(slot,slot+1,MAXITEMS);
      break;
    case 'k':
    case '<':
    case '8':
#if defined(KEY_UP)
    case KEY_UP:
#endif
      slot = move_slot(slot,slot-1,MAXITEMS);
      break;
#ifdef KEY_HOME
    case KEY_HOME:
#endif
    case '-':
      slot = move_slot(slot,0,MAXITEMS);
      break;
#ifdef KEY_LL
    case KEY_LL:
#endif
    case '+':
      slot = move_slot(slot,MAXITEMS-1,MAXITEMS);
      break;
    case '?':
      menuclear();
      menuprint("d:\tDrop up-in-air or current item\n");
      menuprint("e:\tExchange current slot with up-in-air slot\n");
      menuprint("l:\tLook at current item\n");
      menuprint("p:\tPut up-in-air or current item in pack\n");
      menuprint("s:\tShow contents of pack\n");
      menuprint("t:\tTake something from pack into the\n\tcurrent or up-in-air slot\n");
      menuprint("x:\tAs 'e', but exit if up-in-air slot finishes empty\n");
      menuprint(">:\tMove down one slot/item\n");
      menuprint("<:\tMove up one slot/item\n");
      menuprint("?:\tDisplay help (this message + help file)\n");
      menuprint("ESCAPE:\texit\n");
      showmenu();
      clearmsg();
      print1("Display full help? (y/n)");
      if (ynq1() == 'y')
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
    default:
      if (key_to_index(response) > 0) {
        slot = move_slot(slot,key_to_index(response),MAXITEMS);
        if (Player.possessions[slot] == NULL
            &&
            Player.possessions[O_UP_IN_AIR] == NULL) {
          show_inventory_slot(take_from_pack(slot,TRUE), FALSE);
          Command_Duration+=5;
        }
        else {
          switch_to_slot(slot);
          show_inventory_slot(slot,FALSE);
	  slot = O_UP_IN_AIR;
          show_inventory_slot(slot,FALSE);
          Command_Duration+=2;
        }
      }
    }
    calc_melee();
  } while (! done);
  xredraw();
}







/* same as inventory_control, but only uses msg window for i/o*/


void top_inventory_control()
{
  int slot = 0,done=FALSE,usedmenu=FALSE;
  char response, letter;
  clearmsg3();
  do {
    clearmsg1();
    print1("Action [d,e,l,p,s,t,x,~,?,ESCAPE]:");
    print2("'Up in air': ");
    if (Player.possessions[O_UP_IN_AIR] == NULL) nprint2("NOTHING");
    else nprint2(itemid(Player.possessions[O_UP_IN_AIR]));
    response = (char) mcigetc();

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
    case 'l':
      Str1[0] = '\0';
      slot = get_inventory_slot();
      if (Player.possessions[slot] != NULL) {
	if (!strcmp(itemid(Player.possessions[slot]),
	  Player.possessions[slot]->objstr))
	  print3("You notice nothing new about it.");
	else {
	  if (Player.possessions[slot]->uniqueness == COMMON)
	    strcat(Str1, "Your ");
	  strcat(Str1, itemid(Player.possessions[slot]));
	  if (Player.possessions[slot]->objchar == BOOTS)
	    strcat(Str1, " look like ");
	  else {
	    strcat(Str1, " looks like a");
	    letter = Player.possessions[slot]->objstr[0];
	    if (letter == 'a' || letter == 'A' || letter == 'e' ||
	      letter == 'E' || letter == 'i' || letter == 'I' ||
	      letter == 'o' || letter == 'O' || letter == 'u' || letter == 'U')
	      strcat(Str1, "n ");
	    else
	      strcat(Str1, " ");
	  }
	  strcat(Str1, Player.possessions[slot]->objstr);
	  print3(Str1);
	}
      }
      else print3("Nothing in selected slot!");
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
      (void) take_from_pack(slot,FALSE); 
      Command_Duration+=5;
      break;
    case 'e':
      slot = get_inventory_slot();
      if ( slot == O_UP_IN_AIR ) break;
      switch_to_slot(slot);
      Command_Duration+=2;
      break;
    case 'x':
      slot = get_inventory_slot();
      if ( slot == O_UP_IN_AIR ) break;
      switch_to_slot(slot);
      Command_Duration+=2;
      done = (Player.possessions[O_UP_IN_AIR] == NULL);
      break;
    case '~':
      display_possessions();
      inventory_control();
      usedmenu = TRUE;
      done = TRUE;
      break;
    case '?':
      menuclear();
      menuprint("d:\tDrop an item\n");
      menuprint("e:\tExchange a slot with up-in-air slot\n");
      menuprint("l:\tLook at an item\n");
      menuprint("p:\tPut an item in pack\n");
      menuprint("s:\tShow contents of pack\n");
      menuprint("t:\tTake something from pack into a slot\n");
      menuprint("x:\tAs 'e', above, exit if up-in-air slot finishes empty\n");
      menuprint("~:\tEnter full-screen inventory mode\n");
      menuprint("?:\tDisplay help (this message + help file)\n");
      menuprint("ESCAPE:\texit\n");
      showmenu();
      clearmsg();
      print1("Display full help? (y/n)");
      if (ynq1() == 'y')
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
  } while (! done);
  if (usedmenu) 
    xredraw();
}



/* Let the user select a slot. */
int get_inventory_slot()
{
  signed char response;
  do {
    clearmsg1();
    print1("Which inventory slot ['-'='up-in-air' slot]?");
    response = (signed char)mcigetc();
    if ( response == ESCAPE || response == '-' )
      return O_UP_IN_AIR;
    else response = key_to_index(response);
  } while (response != O_UP_IN_AIR);
  return response;
}


/* returns some number between 0 and o->number */
int get_item_number(o)
pob o;
{
  int n=0;
  if (o->number == 1)
    return 1;
  do {
    clearmsg();
    print1("How many? -- max ");
    mnumprint(o->number);
    nprint1(" :");
    n = (int) parsenum();
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
    if(cursed(Player.possessions[slot]) == TRUE + TRUE)
      print3("It sticks to your fingers!");
    else {
      n = get_item_number(Player.possessions[slot]);
      if (n > 0) {
	p_drop_at(Player.x,Player.y,n,Player.possessions[slot]);
	waitflag = (Player.possessions[slot]->used && 
		    (Player.possessions[slot]->number == n));
	conform_lost_objects(n,Player.possessions[slot]);
	if (waitflag) morewait();
      }
      else print3("Didn't drop anything.");
    }
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
    newitem = ((pob) checkmalloc(sizeof(objtype)));
    *newitem = *item;
    if (num <= item->number)
      newitem->number = num;
    /* else num > item->number, so return newitem with number = item->number */
    newitem->used = FALSE;	/* whether the original item was used or not */
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
    
    put = ((!merge) && slotnull && (!airnull) && slottable(oair,slot));
    
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
      num = get_item_number(oslot);
      if (num > 0) {
	otemp = split_item(num,oslot);
	dispose_lost_objects(num,oslot);
	Player.possessions[O_UP_IN_AIR] = otemp;
      }
      if (s2h) {
        if (Player.possessions[O_READY_HAND] == oslot)
          Player.possessions[O_READY_HAND] = NULL;
        if (Player.possessions[O_WEAPON_HAND] == oslot)
          Player.possessions[O_WEAPON_HAND] = NULL;
      }
    }

    else if (trade) {
      
      /* first remove item from slot */
      num = oslot->number;
      conform_lost_objects(oslot->number,oslot);
      oslot->number = num;
      
      Player.possessions[O_UP_IN_AIR] = oslot;

      Player.possessions[slot] = oair;

      if (s2h) {
        if (Player.possessions[O_READY_HAND] == oslot)
          Player.possessions[O_READY_HAND] = NULL;
        if (Player.possessions[O_WEAPON_HAND] == oslot)
          Player.possessions[O_WEAPON_HAND] = NULL;
      }
      
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


/* ->;WDT HACK: this is bad factoring.  I want to use this, but it's
 * printing SILLY stuff out. */
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
	conform_lost_objects(1, o);
	found = TRUE;
      }
  if (! found) for(i=0;((i<Player.packptr)&&(! found));i++)
    if (Player.pack[i] != NULL)
      if ((Player.pack[i]->id == id) &&
	  ((chargeval == -1) ||
	   (Player.pack[i]->charge == chargeval))) {
	Player.pack[i]->number--;
	if (Player.pack[i]->number == 0) {
	  free((char *)Player.pack[i]);
	  Player.pack[i] = NULL;
	}
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
  pob extra=((pob) checkmalloc(sizeof(objtype)));
  *extra = *item;
  extra->number = item->number-1;
  extra->used = FALSE;
  item->number = 1;
  if (Player.packptr < MAXPACK) {
    print3("Putting extra items back in pack.");
    morewait();
    push_pack(extra);
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


/* show slots, with appropriate additional displays if two-handed weapons */
/* are involved */
void show_inventory_slot(slotnum,topline)
int slotnum;
int topline;
{
  if (!topline)
    if (Player.possessions[O_READY_HAND] == Player.possessions[O_WEAPON_HAND] &&
      (slotnum == O_READY_HAND || slotnum == O_WEAPON_HAND))
    {
      display_inventory_slot(O_READY_HAND, topline);
      display_inventory_slot(O_WEAPON_HAND, topline);
    }
    else if (slotnum == O_UP_IN_AIR && Player.possessions[O_UP_IN_AIR] &&
      twohandedp(Player.possessions[O_UP_IN_AIR]->id))
    {
      display_inventory_slot(O_READY_HAND, topline);
      display_inventory_slot(O_WEAPON_HAND, topline);
      display_inventory_slot(slotnum, topline);
    }
    else
      display_inventory_slot(slotnum, topline);
  else
    display_inventory_slot(slotnum, topline);
}
