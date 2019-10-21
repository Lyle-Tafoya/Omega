/* omega copyright (C) by Laurence Raphael Brothers, 1987 */
/* oinv.c */
/* functions having to do with player item inventory */

#include <strings.h>
#include "oglob.h"

/* from oitem */
extern void item_use();

/* from oaux */
extern void calc_melee();

/* from outil */
extern int random_range();
extern int inbounds();
extern int unblocked();

/* from oscr */
extern void printm(),mprint(),drawvision(),xredraw(),menuprint();
extern void menuclear(),menu_i_print();
extern void refreshmsg(),dataprint();
extern char mgetc(),ynq();

/* from omon */
extern void m_damage(),m_status_set(),m_status_reset(),m_pickup();
extern int m_statusp();


/* oinv functions */
void pickup_at(),drop_at(),givemonster(),conform_lost_object();
void conform_unused_object(),add_item_to_pack(),drop_money();
void setplustr(),setchargestr(), setnumstr(),p_drop_at(),give_money();
void conform_lost_objects();
pob detach_money();
int get_money();
char *itemid(),*cashstr();
int getitem(),badobject(),cursed(),inpack(),itemcompare();


/* drops money, heh heh */
void drop_money()
{
  pob money;

  money = detach_money();
  if (money != NULL) {
    if (Dlevel == 0) {
      mprint("As soon as the money leaves your hand,");
      mprint("a horde of scrofulous beggars snatch it up and are gone!");
    }
    else drop_at(Player.x,Player.y,money);
  }
  else Skipmonsters = TRUE;
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
    make_cash(cash,Dlevel);
    cash->basevalue = c;
  }
  return(cash);
}


/* gets a legal amount of money or ABORT */
int get_money(limit)
int limit;
{
  int c;
  mprint("How much? ");
  c = parsenum();
  if (c > limit) {
    printm("\nForget it, buddy.");
    return(ABORT);
  }
  else return(c);
}



/* pick up from some location x,y */
void pickup_at(x,y)
int x,y;
{
  pol ol = Dungeon[Dlevel][x][y].things;
  pol prev = NULL;
  int i,spaceleft,done = FALSE;
  char ans;

  Fastmove = FALSE;
  drawvision(Player.x,Player.y,Player.vision);
  while ((ol != NULL) && (! done)) {
    spaceleft = FALSE;
    for (i=0;i<MAXITEMS;i++)
      spaceleft |= (Player.possessions[i]==NULL);
    printm("\nPick up ");
    printm(itemid(ol->thing));
    printm(" [ynq]: ");
    done = ((ans = ynq()) == 'q');
    if (ans == 'y') {
      if ((! spaceleft) && (ol->thing->objchar != CASH)) {
	mprint("You're carrying too many items!");
	done = TRUE;
      }
      else if (Player.itemweight + ol->thing->weight >= Player.maxweight) {
	printm("\nYou're carrying enough weight already!");
	done = TRUE;
      }
      else {

	add_item_to_pack(ol->thing);

	/* if was first item */
	if (ol == Dungeon[Dlevel][x][y].things) 
	  Dungeon[Dlevel][x][y].things =
	    Dungeon[Dlevel][x][y].things->next;
	/* else for items in middle of list */
	else prev->next = prev->next->next;
      }
    }
    else prev = ol;
    ol = ol->next;
  }
}




int badobject(index)
char index;     
{
  return ((index < 'a') ||
	  (index > 'z') ||
	  (Player.possessions[index-'a'] == NULL));
}


/* put all of o on objlist at x,y on Dlevel */
void drop_at(x,y,o)
int x,y;
pob o;
{
  pol tmp = ((pol) malloc(sizeof(oltype)));
  pob cpy = ((pob) malloc(sizeof(objtype)));
  *cpy = *o;
  cpy->used = FALSE;
  tmp->thing = cpy;
  tmp->next = Dungeon[Dlevel][x][y].things;
  Dungeon[Dlevel][x][y].things = tmp;
}  

/* put n of o on objlist at x,y on Dlevel */
void p_drop_at(x,y,n,o)
int x,y;
int n;
pob o;
{
  pol tmp = ((pol) calloc(1,sizeof(oltype)));
  tmp->thing = ((pob) calloc(1,sizeof(objtype)));
  *(tmp->thing) = *o;
  tmp->thing->used = FALSE;
  tmp->thing->number = n;
  printm("\nDropped ");
  mprint(itemid(tmp->thing));
  tmp->next = Dungeon[Dlevel][x][y].things;
  Dungeon[Dlevel][x][y].things = tmp;
}  


/* returns a string for identified items */
char *itemid(obj)
pob obj;
{
  char tstr[80];
  int i;
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
	strcat(Str4, obj->truename);
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

char *cashstr(o)
pob o;
{
  if (Dlevel < 3) return("copper pieces");
  else if (Dlevel < 7) return("silver pieces");
  else if (Dlevel < 12) return("gold pieces");
  else if (Dlevel < 20) return("semiprecious gems");
  else if (Dlevel < 30) return("mithril pieces");
  else if (Dlevel < 35) return("precious gems"); 
  else return("orichalc pieces");
}

/* return an object's plus as a string */
void setplustr(obj,pstr)
pob obj;
char pstr[72];
{
  pstr[0] = ' ';
  pstr[1] = (obj->plus < 0 ? '-' : '+' );
  if (abs(obj->plus) < 10) {
    pstr[2] = '0' + abs(obj->plus);
    pstr[3] = 0;
  }
  else {
    pstr[2] = '0' + ((int)(abs(obj->plus / 10)));
    pstr[3] = '0' + abs((obj->plus % 10));
    pstr[4] = 0;
  }
}

/* return an object's number as a string */
void setnumstr(obj,nstr)
pob obj;
char nstr[10];
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
char cstr[72];
{
  cstr[0] = ' ';
  cstr[1] = '[';

  if (obj->charge < 10) {
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
  int amount;
  pob cash;

  cash = detach_money();
  if (cash == NULL)
    Skipmonsters = TRUE;
  else givemonster(m,cash);
}
    
    

void givemonster(m,o)
struct monster *m;
struct object *o;
{
  conform_lost_objects(1,o);

  printm("\nThe ");
  printm(m->monstring);

  if (m_statusp(m,GREEDY) || m_statusp(m,NEEDY)) {
    m_pickup(m,o);
    printm(" takes your offering ");
    Player.alignment++;
    if (m_statusp(m,GREEDY) && (true_item_value(o) < m->level*100))
      mprint(" but does not appear satisfied.");
    else if (m_statusp(m,NEEDY) && (true_item_value(o) < Dlevel*Dlevel))
      mprint(" and looks chasteningly at you.");
    else {
      mprint(" and seems happy with it.");
      m_status_reset(m,HOSTILE);
      m_status_reset(m,GREEDY);
      m_status_reset(m,NEEDY);
    }
  }
  else if (m_statusp(m,HUNGRY)) {
    if ((o->usef == I_FOOD) || (o->usef == I_POISON_FOOD)) {
      mprint(" wolfs down your food ... ");
      m_status_reset(m,HUNGRY);
      m_status_reset(m,HOSTILE);
      if  (o->usef == I_POISON_FOOD) {
	Player.alignment -= 2;
	mprint("and chokes on the poisoned ration!");
	m_status_set(m,HOSTILE);
	m_damage(m,100,POISON);
      }
      else mprint("and now seems satiated.");
      free((char *) o);
    }
    else {
      mprint(" ignores your offering.");
      drop_at(m->x,m->y,o);
    }
  }
  else {
    printm(" does not seem interested in your offer");
    drop_at(m->x,m->y,o);
  }
}



/* clears lost possession from pack, by searching for it */
/* will clear all, not just one */
void conform_lost_object(obj)
pob obj;
{
  int i;

  Player.itemweight -= obj->weight*obj->number;
  for(i=0;i<MAXITEMS;i++) 
    if (Player.possessions[i] == obj) {
      Player.possessions[i] = NULL;
      conform_unused_object(obj);
    }
}

void conform_lost_objects(n,obj)
int n;
pob obj;
{
  int i;
  Player.itemweight -= n*obj->weight;
  for(i=0;i<MAXITEMS;i++) 
    if (Player.possessions[i] == obj) {
      obj->number -= n;
      if (obj->number < 1) {
	Player.possessions[i] = NULL;
	conform_unused_object(obj);
      }
    }
}

/* clears unused possession */
void conform_unused_object(obj)
pob obj;
{
  if (obj->used) {
    obj->used = FALSE;
    switch(obj->objchar) {
      case SHIELD: 
      case WEAPON: item_use(obj);
		   if (Player.primary == obj)
		     Player.primary = NULL;
	           if (Player.secondary == obj)
		     Player.secondary = NULL;
		   break;
      case ARMOR:  item_use(obj);
		   Player.armor = NULL; break;
      case CLOAK:  item_use(obj);
		   Player.cloak = NULL; break;
      case BOOTS:  item_use(obj);
		   Player.boots = NULL; break;
      case RING:   item_use(obj);
		   Player.boots = NULL; break;
    }
  }
  calc_melee();
}


/* select an item from inventory */
int getitem(itype)
char itype;
{
  char invstr[64];
  char item[2];
  int i,index,ok=FALSE,drewmenu=FALSE,found=FALSE;

  found = ((itype == NULL) && (Player.cash > 0));
  item[1] = 0;
  strcpy(invstr,"Select an item [");
  if (itype == NULL) strcat(invstr,"$");
  for(i=0;i<MAXITEMS;i++)
    if ((Player.possessions[i] != NULL) &&
	((itype == NULL) ||
	 (Player.possessions[i]->objchar == itype))) {
	   found = TRUE;
	   item[0] = 'a'+i;
	   strcat(invstr,item);
	 }
  strcat(invstr,",?,ESCAPE]:");
  if (found == FALSE) {
    if (itype == FOOD) return(getitem(CORPSE));
    else {
      mprint("Nothing appropriate.");
      return(ABORT);
    }
  }
  else {
    mprint(invstr);
    while (! ok) {
      index = mgetc();
      if (index == '?') {
	drewmenu = TRUE;
	menuclear();
	if (itype == NULL) {
	  menuprint("$: ");
	  menunumprint(Player.cash);
	  menuprint("Au.\n");
	}
	for (i=0;i<MAXITEMS;i++)
	  if (Player.possessions[i] != NULL)
	    if ((itype == NULL) || 
		(Player.possessions[i]->objchar == itype))
	      menu_i_print(i);
	menuprint("\n---More---");
	refreshmsg();
      }
      else if ((index != ESCAPE) && (index != '$') && badobject(index))
	printm("\nBad choice! Try again [? for inventory, ESCAPE to quit]:");
      else ok = TRUE;
    }
    if (drewmenu) xredraw();
    if (index == ESCAPE) return(ABORT);
    else if (index == '$') return(CASHVALUE);
    else return(index);
  }
}


  
  
int itemcompare(a,b)
pob a,b;
{
  return((a->id == b->id) &&
	 (a->plus == b->plus) &&
	 (a->blessing == b->blessing) &&
	 (a->charge == 0) &&
	 (b->charge == 0) &&
	 (a->aux == b->aux) &&
	 (a->known == b->known) &&
	 (! a->used) &&
	 (! b->used));
}

void add_item_to_pack(o)
struct object *o;
{
  int i,gotten=FALSE;
  if (o->objchar == CASH)
    Player.cash += o->basevalue;
  else {
    for(i=0;((i<MAXITEMS) && (!gotten));i++)
      if (Player.possessions[i] != NULL)
	if (itemcompare(Player.possessions[i],o)) {
	  Player.itemweight += o->weight*o->number;
	  gotten = TRUE;
	  Player.possessions[i]->number+=o->number;
	}
    for(i=0;((i<MAXITEMS) && (!gotten));i++)
      if (Player.possessions[i] == NULL) {
	Player.itemweight += o->weight*o->number;
	gotten = TRUE;
	Player.possessions[i] = o;
      }
    if (! gotten) {
      mprint("The item falls to the ground.");
      drop_at(Player.x,Player.y,o);
    }
  }
  calc_melee();
  dataprint();
}


int cursed(obj)
pob obj;
{
  return((obj == NULL) ? FALSE : (obj->blessing < 0));
}


/* if object with id and auxval is inpack, return index, else return
ABORT (-1) -- if auxval is -1, ignore it */
int inpack(id,auxval)
int id,auxval;
{
  int i,found=FALSE;

  for(i=0;((i<MAXITEMS)&&(!found));i++)
    found = ((Player.possessions[i] != NULL) &&
	     (Player.possessions[i]->id == id) &&
	     ((auxval == -1) ||
	      (Player.possessions[i]->aux == auxval)));
  return(found ? i-1 : ABORT);
}
