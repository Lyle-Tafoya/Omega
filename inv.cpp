/* omega copyright (C) by Laurence Raphael Brothers, 1987,1988,1989 */
/* inv.c */
/* functions having to do with player item inventory */

#include "glob.h"
#include "interactive_menu.hpp"

#include <array>
#include <cassert>
#include <string>
#include <vector>

extern void item_equip(object *);
extern void item_unequip(object *);
extern void queue_message(const std::string &message);
extern void print_inventory_menu(Symbol item_type = NULL_ITEM);
extern interactive_menu *menu;

/* drops money, heh heh */
void drop_money()
{
  pob money;

  /* WDT HACK!  Let me guess -- this is yet another memory leak, right? */
  money = detach_money();
  if(money != NULL)
  {
    if(Current_Environment == E_CITY)
    {
      print1("As soon as the money leaves your hand,");
      print2("a horde of scrofulous beggars snatch it up and are gone!");
    }
    else
    {
      drop_at(Player.x, Player.y, money);
    }
  }
  else
  {
    setgamestatus(SKIP_MONSTERS, GameStatus);
  }
}

/* returns some money from player back into "money" item.
   for giving and dropping money */
pob detach_money()
{
  long c;
  pob  cash = NULL;
  c         = get_money(Player.cash);
  if(c != ABORT)
  {
    Player.cash -= c;
    cash = ((pob)checkmalloc(sizeof(objtype)));
    make_cash(cash, difficulty());
    cash->basevalue = c;
  }
  return (cash);
}

/* gets a legal amount of money or ABORT */
long get_money(long limit)
{
  long c;
  print1("How much? ");
  c = parsenum();
  if(c > limit)
  {
    print3("Forget it, buddy.");
    return (ABORT);
  }
  else
  {
    return (c);
  }
}

/* pick up from some location x,y */
/* Lift entire itemlist off ground, pass it to inventory control, which
   may drop things back onto the now null ground */
void pickup_at(int x, int y)
{
  resetgamestatus(FAST_MOVE, GameStatus);

  pol object_list = Level->site[x][y].things;
  Level->site[x][y].things = nullptr;
  if(object_list && !object_list->next)
  {
    gain_item(object_list->thing);
  }
  else
  {
    bool quit = false;
    while(!quit && object_list)
    {
      queue_message("Pick up: " + itemid(object_list->thing) + " [ynq] ");
      switch(ynq())
      {
        case 'y':
          gain_item(object_list->thing);
          break;
        case 'n':
          drop_at(x, y, object_list->thing);
          break;
        case 'q':
          drop_at(x, y, object_list->thing);
          quit = true;
          break;
      }
      pol tmp = object_list;
      object_list = object_list->next;
      free(tmp);
    }
  }
}

/* WDT -- convert from a char (keypress) to an item index in
 * player inventory */
/* Item identifiers, in this case the letters of the alphabet minus
 * any letters already used for commands.  Yes, there are more here
 * than could be needed, but I don't want to short myself for later.
 */
signed char inventory_keymap[] = "-abcfghimnoqruvwyz";
int         key_to_index(signed char key)
{
  assert(MAXITEMS > 0); /* must have room for an item, or this loop will
                         * die! */

  for(int i = 0; i < MAXITEMS; i++)
  {
    if(key == inventory_keymap[i])
    {
      return (signed char)i;
    }
  }
  return O_UP_IN_AIR;
}

signed char index_to_key(signed int index)
{
  if(index < MAXITEMS)
  {
    return inventory_keymap[index];
  }
  else
  {
    return '-';
  }
}

/* criteria for being able to put some item in some slot */
/* WDT -- why on earth does the 'slottable' function print stuff???? */
int aux_slottable(pob o, int slot)
{
  int ok = true;
  if(o == NULL)
  {
    ok = false;
  }
  else if(slot == O_ARMOR)
  {
    if(o->objchar != ARMOR)
    {
      ok = false;
    }
  }
  else if(slot == O_SHIELD)
  {
    if(o->objchar != SHIELD)
    {
      ok = false;
    }
  }
  else if(slot == O_BOOTS)
  {
    if(o->objchar != BOOTS)
    {
      ok = false;
    }
  }
  else if(slot == O_CLOAK)
  {
    if(o->objchar != CLOAK)
    {
      ok = false;
    }
  }
  else if(slot >= O_RING1)
  {
    if(o->objchar != RING)
    {
      ok = false;
    }
  }
  return (ok);
}

bool merge_item_with_list(objectlist *l, object *o, int n)
{
  for(objectlist *pile = l; pile; pile = pile->next)
  {
    if(!pile->thing)
    {
      continue;
    }
    if(o->objchar == CASH && pile->thing->objchar == CASH)
    {
      pile->thing->basevalue += o->basevalue;
      return true;
    }
    else if(objequal(pile->thing, o) && pile->thing->objchar != STICK)
    {
      pile->thing->number += n;
      return true;
    }
  }
  return false;
}

/* put all of o on objlist at x,y on Level->depth */
/* Not necessarily dropped by character; just dropped... */
void drop_at(int x, int y, pob o)
{
  pol tmp;
  pob cpy;

  if(Current_Environment != E_COUNTRYSIDE)
  {
    if((Level->site[x][y].locchar != VOID_CHAR) && (Level->site[x][y].locchar != ABYSS))
    {
      if(merge_item_with_list(Level->site[x][y].things, o, o->number))
      {
        return;
      }
      cpy                      = ((pob)checkmalloc(sizeof(objtype)));
      tmp                      = ((pol)checkmalloc(sizeof(oltype)));
      *cpy                     = *o;
      cpy->used                = false;
      tmp->thing               = cpy;
      tmp->next                = Level->site[x][y].things;
      Level->site[x][y].things = tmp;
    }
    else if(Level->site[x][y].p_locf == L_VOID_STATION)
    {
      setgamestatus(PREPARED_VOID, GameStatus);
    }
  }
}

/* put n of o on objlist at x,y on Level->depth */
void p_drop_at(int x, int y, int n, pob o)
{
  if(Current_Environment != E_COUNTRYSIDE)
  {
    if((Level->site[x][y].locchar != VOID_CHAR) && (Level->site[x][y].locchar != ABYSS))
    {
      print2("Dropped ");
      nprint2(itemid(o));
      if(merge_item_with_list(Level->site[x][y].things, o, n))
      {
        return;
      }
      pol tmp                  = ((pol)checkmalloc(sizeof(oltype)));
      tmp->thing               = ((pob)checkmalloc(sizeof(objtype)));
      *(tmp->thing)            = *o;
      tmp->thing->used         = false;
      tmp->thing->number       = n;
      tmp->next                = Level->site[x][y].things;
      Level->site[x][y].things = tmp;
    }
    else if(Level->site[x][y].p_locf == L_VOID_STATION)
    {
      setgamestatus(PREPARED_VOID, GameStatus);
    }
  }
}

// return an object's plus as a string
std::string getplusstr(pob obj)
{
  if(obj->plus < 0)
  {
    return std::to_string(obj->plus);
  }
  else
  {
    return "+" + std::to_string(obj->plus);
  }
}

// return object with charges
std::string getchargestr(pob obj)
{
  std::string chargestr = " [";
  if(obj->charge < 0)
  {
    chargestr += "dead]";
  }
  else
  {
    chargestr += std::to_string(obj->charge) + "]";
  }
  return chargestr;
}


/* return an object's number as a string */
std::string getnumstr(pob obj)
{
  std::string numstr;
  if(obj->number > 1 && obj->number < 41)
  {
    numstr += std::to_string(obj->number) + "x ";
  }
  else if(obj->number > 40)
  {
    numstr = "lots of ";
  }
  return numstr;
}

// returns a string for identified items
std::string itemid(pob obj)
{
  std::string item_name;
  if(obj->objchar == CASH)
  {
    return obj->truename;
  }
  else
  {
    if(Objects[obj->id].known > obj->known)
    {
      obj->known = Objects[obj->id].known;
    }

    item_name += getnumstr(obj);
    if(obj->known == 0)
    {
      item_name += obj->objstr;
    }
    else if(obj->known == 1)
    {
      if(obj->id == ARTIFACTID + 8 || obj->id == ARTIFACTID + 20 || obj->id == ARTIFACTID + 21)
      {
        item_name += obj->objstr;
      }
      item_name += obj->truename;
    }
    else
    {
      if(obj->id == ARTIFACTID + 8 || obj->id == ARTIFACTID + 20 || obj->id == ARTIFACTID + 21)
      {
        item_name += "the ";
      }
      if(obj->on_use == I_NOTHING && Objects[obj->id].on_use != I_NOTHING &&
         obj->on_equip == I_NOTHING && Objects[obj->id].on_equip != I_NOTHING &&
         obj->on_unequip == I_NOTHING && Objects[obj->id].on_unequip != I_NOTHING)
      {
        item_name += "disenchanted ";
      }
      if(obj->blessing < 0)
      {
        item_name += "cursed " + std::string(obj->cursestr);
      }
      else if(obj->blessing > 0)
      {
        item_name += "blessed " + std::string(obj->truename);
      }
      else
      {
        item_name += obj->truename;
      }
      if(obj->number > 1)
      {
        item_name += "s";
      }
      switch(obj->objchar)
      {
        case STICK:
          item_name += getchargestr(obj);
          break;
        case MISSILEWEAPON:
        case ARMOR:
        case RING:
        case SHIELD:
        case WEAPON:
          item_name += " " + getplusstr(obj);
          break;
        default:
          break;
      }
      if(obj->objchar == WEAPON)
      {
        item_name += " (" + std::to_string(obj->hit + obj->plus) + "," +
          std::to_string(obj->dmg + obj->plus) + ")";
      }
      else if(obj->objchar == ARMOR)
      {
        item_name += " [" + std::to_string(obj->plus - obj->aux) + "," + std::to_string(obj->dmg) + "]";
      }
      else if(obj->objchar == SHIELD)
      {
        item_name += " [" + std::to_string(obj->plus + obj->aux) + ",0]";
      }
    }
    return item_name;
  }
}

const char *cashstr()
{
  if(difficulty() < 3)
  {
    return "copper pieces";
  }
  else if(difficulty() < 5)
  {
    return "silver pieces";
  }
  else if(difficulty() < 7)
  {
    return "gold pieces";
  }
  else if(difficulty() < 8)
  {
    return "semiprecious gems";
  }
  else if(difficulty() < 9)
  {
    return "mithril pieces";
  }
  else if(difficulty() < 10)
  {
    return "precious gems";
  }
  else
  {
    return "orichalc pieces";
  }
}

void give_money(struct monster *m)
{
  pob cash;

  cash = detach_money();
  if(cash == NULL)
  {
    setgamestatus(SKIP_MONSTERS, GameStatus);
  }
  else
  {
    givemonster(m, cash);
  }
}

void givemonster(struct monster *m, struct object *o)
{
  /* special case -- give gem to LawBringer */
  if((m->id == LAWBRINGER) && (o->id == ARTIFACTID + 21))
  {
    print1("The LawBringer accepts the gem reverently.");
    print2("He raises it above his head, where it bursts into lambent flame!");
    print1("You are bathed in a shimmering golden light.");
    print2("You feel embedded in an infinite matrix of ordered energy.");
    if(Imprisonment > 0)
    {
      Imprisonment = 0;
    }
    if(Player.rank[ORDER] == -1)
    {
      print2("You have been forgiven. You feel like a Paladin....");
      Player.rank[ORDER] = 1;
    }
    Player.alignment += 200;
    Player.pow = Player.maxpow = Player.pow * 2;
    gain_experience(2000);
    setgamestatus(GAVE_STARGEM, GameStatus);
    /* WDT HACK!!!  Where else would this ever get freed?? */
    free(o);
  }
  else
  {
    if(m->uniqueness == COMMON)
    {
      strcpy(Str3, "The ");
      strcat(Str3, m->monstring);
    }
    else
    {
      strcpy(Str3, m->monstring);
    }

    if(m_statusp(*m, GREEDY) || m_statusp(*m, NEEDY))
    {
      m_pickup(m, o);
      strcat(Str3, " takes your gift");
      print1(Str3);
      Player.alignment++;
      if(m_statusp(*m, GREEDY) && (true_item_value(o) < (long)m->level * 100))
      {
        nprint1("...but does not appear satisfied.");
      }
      else if(m_statusp(*m, NEEDY) && (true_item_value(o) < (long)Level->depth * Level->depth))
      {
        nprint1("...and looks chasteningly at you.");
      }
      else
      {
        nprint1("...and seems happy with it.");
        m_status_reset(*m, HOSTILE);
        m_status_reset(*m, GREEDY);
        m_status_reset(*m, NEEDY);
      }
    }
    else if(m_statusp(*m, HUNGRY))
    {
      if(((m->id == HORSE) && (o->id == FOODID + 15)) || /* grain */
         ((m->id != HORSE) && ((o->on_use == I_FOOD) || (o->on_use == I_POISON_FOOD))))
      {
        strcat(Str3, " wolfs down your food ... ");
        print1(Str3);
        m_status_reset(*m, HUNGRY);
        m_status_reset(*m, HOSTILE);
        if(o->on_use == I_POISON_FOOD)
        {
          Player.alignment -= 2;
          nprint1("...and chokes on the poisoned ration!");
          m_status_set(*m, HOSTILE);
          m_damage(m, 100, POISON);
        }
        else
        {
          nprint1("...and now seems satiated.");
        }
        free((char *)o);
      }
      else
      {
        strcat(Str3, " spurns your offering and leaves it on the ground.");
        print1(Str3);
        drop_at(m->x, m->y, o);
      }
    }
    else
    {
      strcat(Str3, " doesn't care for your offering and drops it.");
      print1(Str3);
      drop_at(m->x, m->y, o);
    }
  }
}

/* will clear all, not just one of an object. */
void conform_lost_object(pob obj)
{
  if(obj != NULL)
  {
    conform_lost_objects(obj->number, obj);
  }
}

/* removes n of object from inventory; frees object if appropriate. */

void dispose_lost_objects(int n, pob obj)
{
  int i, conformed = false, subtracted = false;

  if(obj == NULL)
  {
    return;
  }
  for(i = 0; i < MAXITEMS; i++)
  {
    if(Player.possessions[i] == obj)
    {
      if(!subtracted)
      {
        obj->number -= n;
        subtracted = true;
      }
      if(obj->number < 1)
      {
        if(!conformed)
        {
          conform_unused_object(obj);
          conformed = true;
        }
        Player.possessions[i] = NULL;
      }
    }
  }
  if(obj->number < 1)
  {
    free((char *)obj);
  }
}

/* removes n of object from inventory without freeing object.
   Removes all instances of pointer (might be 2 handed weapon, etc) */
void conform_lost_objects(int n, pob obj)
{
  int i, conformed = false, subtracted = false;
  if(obj != NULL)
  {
    for(i = 0; i < MAXITEMS; i++)
    {
      if(Player.possessions[i] == obj)
      {
        if(!subtracted)
        {
          obj->number -= n;
          subtracted = true;
        }
        if(obj->number < 1)
        {
          if(!conformed)
          {
            conform_unused_object(obj);
            conformed = true;
          }
          Player.possessions[i] = NULL;
        }
      }
    }
  }
}

/* clears unused possession */
void conform_unused_object(pob obj)
{
  if(obj->used)
  {
    item_unequip(obj);
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

int getitem(Symbol itype)
{
  char invstr[64];
  char key;
  int  i, k = 0, ok = false, drewmenu = false, found = false;

  found     = ((itype == NULL_ITEM) || ((itype == CASH) && (Player.cash > 0)));
  invstr[0] = 0;
  for(i = 1; i < MAXITEMS; i++)
  {
    if(Player.possessions[i] != NULL)
    {
      if((itype == NULL_ITEM) || (itype == CASH) || (Player.possessions[i]->objchar == itype) ||
         ((itype == FOOD) && (Player.possessions[i]->objchar == CORPSE)))
      {
        found       = true;
        invstr[k++] = index_to_key(i);
        invstr[k]   = 0;
      }
    }
  }
  if((itype == CASH) && found)
  {
    invstr[k++] = '$';
    invstr[k]   = 0;
  }
  if(!found)
  {
    print3("Nothing appropriate.");
    return (ABORT);
  }
  else
  {
    print2("Select an item [");
    nprint2(invstr);
    nprint2(",?] ");
    while(!ok)
    {
      key = (char)mcigetc();
      if(key == '?')
      {
        drewmenu = true;
        print_inventory_menu(itype);
      }
      else if(key == ESCAPE)
      {
        ok = true;
      }
      else if(key == (CASH & 0xff))
      {
        if(itype == CASH)
        {
          ok = true;
        }
        else
        {
          print3("You cannot select cash now.");
          ok = false;
        }
      }
      else if(!strmem(key, invstr) || key_to_index(key) == (signed char)-1)
      {
        print3("Nope! Try again [? for inventory, ESCAPE to quit]:");
      }
      else
      {
        ok = true;
      }
    }
    if(drewmenu)
    {
      xredraw();
    }
    if(key == ESCAPE)
    {
      return (ABORT);
    }
    else if(key == (CASH & 0xff))
    {
      return (CASHVALUE);
    }
    else
    {
      return key_to_index(key);
    }
  }
}

bool merge_item_with_pack(object *o)
{
  if(!o || o->objchar == STICK)
  {
    return false;
  }
  for(int i = 0; i < MAXPACK; ++i)
  {
    object *pack_item = Player.pack[i];
    if(pack_item && objequal(o, pack_item))
    {
      pack_item->number += o->number;
      free(o);
      return true;
    }
  }
  return false;
}

bool merge_item_with_inventory(object *o)
{
  if(!o && o->objchar == STICK)
  {
    return false;
  }
  const std::array slots{O_LEFT_SHOULDER, O_RIGHT_SHOULDER, O_BELT1, O_BELT2, O_BELT3};
  for(int slot : slots)
  {
    object *pack_item = Player.possessions[slot];
    if(pack_item && objequal(o, pack_item))
    {
      pack_item->number += o->number;
      free(o);
      return true;
    }
  }
  return false;
}

/* inserts the item at the start of the pack array */
void push_pack(pob o)
{
  for(int i = Player.packptr; i > 0; i--)
  {
    Player.pack[i] = Player.pack[i - 1];
  }
  Player.pack[0] = o;
  Player.packptr++;
}

void add_to_pack(pob o)
{
  if(merge_item_with_pack(o))
  {
    return;
  }
  if(Player.packptr >= MAXPACK)
  {
    print3("Your pack is full. The item drops to the ground.");
    drop_at(Player.x, Player.y, o);
  }
  else
  {
    push_pack(o);
    print3("Putting item in pack.");
  }
}

int get_to_pack(pob o)
{
  if(merge_item_with_pack(o))
  {
    return true;
  }
  if(Player.packptr >= MAXPACK)
  {
    print3("Your pack is full.");
    return false;
  }
  else
  {
    push_pack(o);
    print3("Putting item in pack.");
    return true;
  }
}

void gain_item(object *o)
{
  if(o->uniqueness == UNIQUE_MADE)
  {
    Objects[o->id].uniqueness = UNIQUE_TAKEN;
  }
  if(o->objchar == CASH)
  {
    print2("You gained some cash.");
    Player.cash += o->basevalue;
    free((char *)o);
    dataprint();
  }
  else if(merge_item_with_inventory(o))
  {
    queue_message("You add it to the stack in your inventory");
  }
  else if(!get_to_pack(o))
  {
    p_drop_at(Player.x, Player.y, o->number, o);
  }
  calc_melee();
}

int pack_item_cost(int index)
{
  int cost;
  if(index > 20)
  {
    cost = 17;
  }
  else if(index > 15)
  {
    cost = 7;
  }
  else
  {
    cost = 2;
  }
  return cost;
}

/* WDT -- 'response' must be an index into the pack. */
void use_pack_item(int response, int slot)
{
  pob item;
  int i;
  i = pack_item_cost(response);
  if(i > 10)
  {
    print1("You begin to rummage through your pack.");
  }
  if(i > 5)
  {
    print1("You search your pack for the item.");
  }
  print1("You take the item from your pack.");
  Command_Duration += i;
  item = Player.possessions[slot] = Player.pack[response];
  for(i = response; i < Player.packptr - 1; i++)
  {
    Player.pack[i] = Player.pack[i + 1];
  }
  Player.pack[--Player.packptr] = NULL;

  if((slot == O_READY_HAND || slot == O_WEAPON_HAND) && twohandedp(item->id))
  {
    if(Player.possessions[O_READY_HAND] == NULL)
    {
      Player.possessions[O_READY_HAND] = item;
    }
    if(Player.possessions[O_WEAPON_HAND] == NULL)
    {
      Player.possessions[O_WEAPON_HAND] = item;
    }
  }
  if(item_useable(item, slot))
  {
    item_equip(item);
    if(item->number > 1)
    {
      pack_extra_items(item);
    }
  }
}

/* WDT HACK!  This ought to be in scr.c, along with its companion.  However,
 * right now it's only used in the function directly below. */
int aux_display_pack(int start_item, int slot)
{
  int         i = start_item, items;
  const char *depth_string;
  if(Player.packptr < 1)
  {
    queue_message("Pack is empty.");
  }
  else if(Player.packptr <= start_item)
  {
    queue_message("You see the leather at the bottom of the pack.");
  }
  else
  {
    menuclear();
    items = 0;
    for(i = start_item; i < Player.packptr && items < ScreenLength - 5; i++)
    {
      if(aux_slottable(Player.pack[i], slot))
      {
        if(pack_item_cost(i) > 10)
        {
          depth_string = "**";
        }
        else if(pack_item_cost(i) > 5)
        {
          depth_string = "* ";
        }
        else
        {
          depth_string = "  ";
        }
        sprintf(Str1, "  %c: %s %s\n", i + 'a', depth_string, itemid(Player.pack[i]).c_str());
        if(items == 0)
        {
          menuprint("Items in Pack:\n");
        }
        menuprint(Str1);
        items++;
      }
    }
    if(items == 0)
    {
      menuprint("You see nothing useful for that slot in the pack.");
    }
    else
    {
      menuprint("\n*: Takes some time to reach; **: buried very deeply.");
    }
    showmenu();
  }
  return i;
}

// takes something from pack, puts to slot
void take_from_pack(int slot)
{
  char pack_item, last_item;
  int  quit = false, ok = true;
  int response;
  if(Player.packptr < 1)
  {
    print3("Pack is empty!");
  }
  else
  {
    pack_item = 0;
    do
    {
      ok        = true;
      last_item = aux_display_pack(pack_item, slot);
      if(last_item == Player.packptr && pack_item == 0)
      {
        print1("Enter pack slot letter or ESCAPE to quit.");
      }
      else if(last_item == Player.packptr)
      {
        print1("Enter pack slot letter, - to go back, or ESCAPE to quit.");
      }
      else if(pack_item == 0)
      {
        print1("Enter pack slot letter, + to see more, or ESCAPE to quit.");
      }
      else
      {
        print1("Enter pack slot letter, + or - to see more, or ESCAPE to quit.");
      }
      response = mcigetc();
      if(response == '?')
      {
        /* WDT HACK -- display some help instead. */
        print1("Help not implemented (sorry).");
        ok = false;
      }
      else if(response == ESCAPE)
      {
        quit = true;
      }
      else if(response == '+')
      {
        if(last_item < Player.packptr)
        {
          pack_item = last_item;
        }
        ok = false;
      }
      else if(response == '-')
      {
        /* WDT HACK: this _should_ make us page up.  Sadly,
         * I have no way of calculating how much I'll be paging up.
         * This is fixable, but I have no idea how much work... */
        pack_item = 0;
        ok        = false;
      }
      else
      {
        ok = ((response >= 'a') && (response < 'a' + Player.packptr));
        if(ok)
        {
          ok = slottable(Player.pack[response - 'a'], slot);
        }
      }
    } while(!ok);
    if(!quit)
    {
      use_pack_item(response - 'a', slot);
    }
  }
  print_inventory_menu();
}

void put_to_pack(int slot)
{
  object *oslot = Player.possessions[slot];
  if(!oslot)
  {
    print3("Slot is empty!");
  }
  else if(oslot->blessing < 0 && oslot->used)
  {
    print3("Item is cursed!");
  }
  else
  {
    int num = get_item_number(oslot);
    bool twohanded = (slot == O_READY_HAND || slot == O_WEAPON_HAND) && twohandedp(oslot->id);
    if(num >= oslot->number)
    {
      if(twohanded && oslot &&
          Player.possessions[O_READY_HAND] == Player.possessions[O_WEAPON_HAND])
      {
        Player.possessions[O_READY_HAND] = nullptr;
        Player.possessions[O_WEAPON_HAND] = nullptr;
      }
      else
      {
        Player.possessions[slot] = nullptr;
      }
      conform_unused_object(oslot);
      add_to_pack(oslot);
    }
    else if(num > 0)
    {
      object *temp = split_item(num, oslot);
      conform_lost_objects(num, oslot);
      add_to_pack(temp);
    }
  }
}

void do_inventory_control()
{
  print_inventory_menu();
  for(bool done = false; !done;)
  {
    int response = mcigetc();
    switch(response)
    {
      case 12:
      case 18: /* ^l, ^r */
        print_inventory_menu();
        break;
      case ESCAPE:
        done = true;
        break;
      default:
        if(key_to_index(response) > 0)
        {
          int slot = key_to_index(response);
          if(!Player.possessions[slot])
          {
            take_from_pack(slot);
            Command_Duration += 5;
          }
          else
          {
            put_to_pack(slot);
            Command_Duration += 2;
          }
          print_inventory_menu();
        }
    }
    calc_melee();
    print_inventory_menu();
  }
  xredraw();
}

/* returns some number between 0 and o->number */
int get_item_number(pob o)
{
  int n = 0;
  if(o->number == 1)
  {
    return 1;
  }
  do
  {
    print1("How many? -- max " + std::to_string(o->number) + " :");
    n = (int)parsenum();
    if(n > o->number)
    {
      print3("Too many!");
    }
    else if(n < 1)
    {
      n = 0;
    }
  } while(n > o->number);
  if(n < 1)
  {
    n = 0;
  }
  return (n);
}

void drop_from_slot(int slot)
{
  int n;
  if(Player.possessions[slot] != NULL)
  {
    if(cursed(Player.possessions[slot]) == true + true)
    {
      print3("It sticks to your fingers!");
    }
    else
    {
      n = get_item_number(Player.possessions[slot]);
      if(n > 0)
      {
        p_drop_at(Player.x, Player.y, n, Player.possessions[slot]);
        conform_lost_objects(n, Player.possessions[slot]);
      }
      else
      {
        print3("Didn't drop anything.");
      }
    }
  }
  else
  {
    print3("Didn't drop anything.");
  }
}

/* splits num off of item to make newitem which is returned */
/* something else (conform_lost_objects) has to reduce the actual
   number value of item and Player.itemweight */
pob split_item(int num, pob item)
{
  pob newitem = NULL;
  if(item != NULL)
  {
    newitem  = ((pob)checkmalloc(sizeof(objtype)));
    *newitem = *item;
    if(num <= item->number)
    {
      newitem->number = num;
    }
    /* else num > item->number, so return newitem with number = item->number */
    newitem->used = false; /* whether the original item was used or not */
  }
  return (newitem);
}


/* are two objects equal except for their number field? */
/* returns false if either object is null */
int objequal(struct object *o, struct object *p)
{
  if(!o || !p)
  {
    return false;
  }
  else
  {
    return (o->id == p->id && o->weight == p->weight && o->plus == p->plus && o->charge == p->charge &&
            o->dmg == p->dmg && o->hit == p->hit && o->aux == p->aux && o->fragility == p->fragility &&
            o->basevalue == p->basevalue && o->known == p->known && o->blessing == p->blessing &&
            o->on_use == p->on_use && o->on_equip == p->on_equip && o->on_unequip == p->on_unequip);
  }
}

/* criteria for being able to put some item in some slot */
int slottable(pob o, int slot)
{
  int ok = true;
  if(o == NULL)
  {
    ok = false;
  }
  else if(slot == O_ARMOR)
  {
    if(o->objchar != ARMOR)
    {
      print3("Only armor can go in the armor slot!");
      ok = false;
    }
  }
  else if(slot == O_SHIELD)
  {
    if(o->objchar != SHIELD)
    {
      print3("Only a shield can go in the shield slot!");
      ok = false;
    }
  }
  else if(slot == O_BOOTS)
  {
    if(o->objchar != BOOTS)
    {
      print3("Only boots can go in the boots slot!");
      ok = false;
    }
  }
  else if(slot == O_CLOAK)
  {
    if(o->objchar != CLOAK)
    {
      print3("Only a cloak can go in the cloak slot!");
      ok = false;
    }
  }
  else if(slot >= O_RING1)
  {
    if(o->objchar != RING)
    {
      print3("Only a ring can go in a ring slot!");
      ok = false;
    }
  }
  return (ok);
}

// whether or not an item o can be used in a slot. Assumes o can in fact be placed in the slot.
bool item_useable(pob o, int slot)
{
  if(slot == O_ARMOR || slot == O_CLOAK || slot == O_SHIELD || slot == O_BOOTS || slot >= O_RING1)
  {
    return true;
  }
  else if(o->objchar == WEAPON || o->objchar == MISSILEWEAPON)
  {
    if(twohandedp(o->id) && (slot == O_READY_HAND || slot == O_WEAPON_HAND))
    {
      if(Player.possessions[O_READY_HAND] == Player.possessions[O_WEAPON_HAND])
      {
        print1("You heft the weapon and find you must use both hands.");
        return true;
      }
      else
      {
        print1("This weapon is two-handed, so at the moment, ");
        print2("you are just lugging it around....");
        return false;
      }
    }
    else
    {
      return slot == O_WEAPON_HAND;
    }
  }
  else if((slot == O_READY_HAND || slot == O_WEAPON_HAND) && o->id == 8)
  {
    return true;
  }
  else
  {
    return false;
  }
}

/* returns false if not cursed, true if cursed but not used,
   true + true if cursed and used */
int cursed(pob obj)
{
  return ((obj == NULL) ? false : ((obj->blessing < 0) ? (obj->used == true) + true : false));
}

/* returns true if item with id and charge is found in pack or in
   inventory slot. charge is used to differentiate
   corpses instead of aux, which is their food value. */
int find_item(pob *o, int id, int chargeval)
{
  int i, found = false;
  *o = NULL;
  for(i = 1; ((i < MAXITEMS) && (!found)); i++)
  {
    if(Player.possessions[i] != NULL)
    {
      if((Player.possessions[i]->id == id) &&
         ((chargeval == -1) || (Player.possessions[i]->charge == chargeval)))
      {
        *o    = Player.possessions[i];
        found = true;
      }
    }
  }
  if(!found)
  {
    for(i = 0; ((i < Player.packptr) && (!found)); i++)
    {
      if(Player.pack[i] != NULL)
      {
        if((Player.pack[i]->id == id) && ((chargeval == -1) || (Player.pack[i]->charge == chargeval)))
        {
          *o    = Player.pack[i];
          found = true;
        }
      }
    }
  }
  return (found);
}

/* returns true if item with id and charge is found in pack or in
   inventory slot. Destroys item. charge is used to differentiate
   corpses instead of aux, which is their food value. */
int find_and_remove_item(int id, int chargeval)
{
  int i, found = false;
  pob o = NULL;

  for(i = 1; ((i < MAXITEMS) && (!found)); i++)
  {
    if(Player.possessions[i] != NULL)
    {
      if((Player.possessions[i]->id == id) &&
         ((chargeval == -1) || (Player.possessions[i]->charge == chargeval)))
      {
        o = Player.possessions[i];
        conform_lost_objects(1, o);
        found = true;
      }
    }
  }
  if(!found)
  {
    for(i = 0; ((i < Player.packptr) && (!found)); i++)
    {
      if(Player.pack[i] != NULL)
      {
        if((Player.pack[i]->id == id) && ((chargeval == -1) || (Player.pack[i]->charge == chargeval)))
        {
          Player.pack[i]->number--;
          if(Player.pack[i]->number == 0)
          {
            free((char *)Player.pack[i]);
            Player.pack[i] = NULL;
          }
          found = true;
        }
      }
    }
  }
  fixpack();
  return (found);
}

void lose_all_items()
{
  int i;
  print1("You notice that you are completely devoid of all possessions.");
  for(i = 0; i < MAXITEMS; i++)
  {
    if(Player.possessions[i] != NULL)
    {
      dispose_lost_objects(Player.possessions[i]->number, Player.possessions[i]);
      Player.possessions[i] = NULL;
    }
  }
  for(i = 0; i < MAXPACK; i++)
  {
    if(Player.pack[i] != NULL)
    {
      free((char *)Player.pack[i]);
    }
    Player.pack[i] = NULL;
  }
  Player.packptr = 0;
  calc_melee();
}

/* prevents people from wielding 3 short swords, etc. */
void pack_extra_items(pob item)
{
  pob extra     = ((pob)checkmalloc(sizeof(objtype)));
  *extra        = *item;
  extra->number = item->number - 1;
  extra->used   = false;
  item->number  = 1;
  if(Player.packptr < MAXPACK)
  {
    print3("Putting extra items back in pack.");
    push_pack(extra);
  }
  else
  {
    print3("No room for extra copies of item -- dropping them.");
    drop_at(Player.x, Player.y, extra);
  }
  calc_melee();
}

/* makes sure Player.pack is OK, (used after sale from pack) */
void fixpack()
{
  pob tpack[MAXPACK];
  int i, tctr = 0;
  for(i = 0; i < MAXPACK; i++)
  {
    tpack[i] = NULL;
  }
  for(i = 0; i < MAXPACK; i++)
  {
    if(Player.pack[i] != NULL)
    {
      tpack[tctr++] = Player.pack[i];
    }
  }
  for(i = 0; i < MAXPACK; i++)
  {
    Player.pack[i] = tpack[i];
  }
  Player.packptr = tctr;
}
