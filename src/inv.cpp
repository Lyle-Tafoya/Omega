/*
Omega copyright (C) by Laurence Raphael Brothers, 1987,1988,1989
Modifications copyright (C) by Lyle Tafoya, 2019, 2021-2023

This file is part of Omega.

Omega is free software: you can redistribute it and/or modify it under the terms
of the GNU General Public License as published by the Free Software Foundation,
either version 3 of the License, or (at your option) any later version.

Omega is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
Omega. If not, see <https://www.gnu.org/licenses/>.
*/

/* inv.c */
/* functions having to do with player item inventory */

#include "glob.h"
#include "interactive_menu.hpp"
#include "scr.h"

#include <array>
#include <cassert>
#include <format>
#include <string>
#include <vector>

extern void item_equip(object *);
extern void item_unequip(object *);
extern interactive_menu *menu;

// returns some money from player back into "money" item.
// for giving and dropping money
object *detach_money()
{
  long c;
  object *cash = nullptr;
  c        = get_money(Player.cash);
  if(c != ABORT)
  {
    Player.cash -= c;
    cash = new object;
    make_cash(cash, difficulty());
    cash->basevalue = c;
  }
  return (cash);
}

/* drops money, heh heh */
void drop_money()
{
  object *money;

  /* WDT HACK!  Let me guess -- this is yet another memory leak, right? */
  money = detach_money();
  if(money)
  {
    if(Current_Environment == E_CITY)
    {
      queue_message("As soon as the money leaves your hand,");
      queue_message("a horde of scrofulous beggars snatch it up and are gone!");
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

/* gets a legal amount of money or ABORT */
long get_money(long limit)
{
  long c;
  queue_message("How much? ");
  c = parsenum();
  if(c > limit)
  {
    queue_message("Forget it, buddy.");
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

  std::forward_list<object *> &object_list = Level->site[x][y].things;
  if(!object_list.empty() && std::next(object_list.begin()) == object_list.end())
  {
    gain_item(object_list.front());
    object_list.pop_front();
  }
  else
  {
    for(auto prev = object_list.before_begin(); std::next(prev) != object_list.end();)
    {
      auto it = std::next(prev);
      queue_message(std::format("Pick up: {} [ynq] ", itemid(*it)));
      char player_input = ynq();
      if(player_input == 'y')
      {
        gain_item(*it);
        object_list.erase_after(prev);
      }
      else if(player_input == 'q')
      {
        break;
      }
      else
      {
        ++prev;
      }
    }
  }
}

/* criteria for being able to put some item in some slot */
/* WDT -- why on earth does the 'slottable' function print stuff???? */
int aux_slottable(object *o, int slot)
{
  int ok = true;
  if(!o)
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

// are two objects equal except for their number field?
// returns false if either object is null
int objequal(object *o, object *p)
{
  if(!o || !p)
  {
    return false;
  }
  else
  {
    return (
      o->id == p->id && o->weight == p->weight && o->plus == p->plus && o->charge == p->charge &&
      o->dmg == p->dmg && o->hit == p->hit && o->aux == p->aux && o->fragility == p->fragility &&
      o->basevalue == p->basevalue && o->known == p->known && o->blessing == p->blessing &&
      o->on_use == p->on_use && o->on_equip == p->on_equip && o->on_unequip == p->on_unequip
    );
  }
}

bool merge_item_with_list(std::forward_list<object *> &l, object *o, int n)
{
  for(object *list_object : l)
  {
    if(!list_object)
    {
      continue;
    }
    if(o->objchar == CASH && list_object->objchar == CASH)
    {
      list_object->basevalue += o->basevalue;
      return true;
    }
    else if(objequal(list_object, o) && list_object->objchar != STICK)
    {
      list_object->number += n;
      return true;
    }
  }
  return false;
}

// put all of o on objlist at x,y on Level->depth
// Not necessarily dropped by character; just dropped...
void drop_at(int x, int y, object *o)
{
  if(Current_Environment != E_COUNTRYSIDE)
  {
    if((Level->site[x][y].locchar != VOID_CHAR) && (Level->site[x][y].locchar != ABYSS))
    {
      if(merge_item_with_list(Level->site[x][y].things, o, o->number))
      {
        return;
      }
      object *cpy = new object{*o};
      cpy->used   = false;
      Level->site[x][y].things.push_front(cpy);
    }
    else if(Level->site[x][y].p_locf == L_VOID_STATION)
    {
      setgamestatus(PREPARED_VOID, GameStatus);
    }
  }
}

// put n of o on objlist at x,y on Level->depth
void p_drop_at(int x, int y, int n, object *o)
{
  if(Current_Environment != E_COUNTRYSIDE)
  {
    if((Level->site[x][y].locchar != VOID_CHAR) && (Level->site[x][y].locchar != ABYSS))
    {
      queue_message(std::format("Dropped {}", itemid(o)));
      if(merge_item_with_list(Level->site[x][y].things, o, n))
      {
        return;
      }
      object *cpy = new object{*o};
      cpy->used   = false;
      cpy->number = n;
      Level->site[x][y].things.push_front(cpy);
    }
    else if(Level->site[x][y].p_locf == L_VOID_STATION)
    {
      setgamestatus(PREPARED_VOID, GameStatus);
    }
  }
}

// return an object's plus as a string
std::string getplusstr(object *obj)
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
std::string getchargestr(object *obj)
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
std::string getnumstr(object *obj)
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
std::string itemid(object *obj)
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
        item_name += "cursed " + obj->cursestr;
      }
      else if(obj->blessing > 0)
      {
        item_name += "blessed " + obj->truename;
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
        item_name += std::format(" ({},{})", obj->hit + obj->plus, obj->dmg + obj->plus);
      }
      else if(obj->objchar == ARMOR)
      {
        item_name += std::format(" [{},{}]", obj->plus - obj->aux, obj->dmg);
      }
      else if(obj->objchar == SHIELD)
      {
        item_name += std::format(" [{},0]", obj->plus + obj->aux);
      }
    }
    return item_name;
  }
}

const std::string cashstr()
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

void give_money(monster *m)
{
  object *cash;

  cash = detach_money();
  if(!cash)
  {
    setgamestatus(SKIP_MONSTERS, GameStatus);
  }
  else
  {
    givemonster(m, cash);
  }
}

void givemonster(monster *m, object *o)
{
  /* special case -- give gem to LawBringer */
  if((m->id == LAWBRINGER) && (o->id == ARTIFACTID + 21))
  {
    queue_message("The LawBringer accepts the gem reverently.");
    queue_message("He raises it above his head, where it bursts into lambent flame!");
    queue_message("You are bathed in a shimmering golden light.");
    queue_message("You feel embedded in an infinite matrix of ordered energy.");
    if(Imprisonment > 0)
    {
      Imprisonment = 0;
    }
    if(Player.rank[ORDER] == -1)
    {
      queue_message("You have been forgiven. You feel like a Paladin....");
      Player.rank[ORDER] = 1;
    }
    Player.alignment += 200;
    Player.pow = Player.maxpow = Player.pow * 2;
    gain_experience(2000);
    setgamestatus(GAVE_STARGEM, GameStatus);
    /* WDT HACK!!!  Where else would this ever get freed?? */
    delete o;
  }
  else
  {
    std::string monster_name;
    if(m->uniqueness == COMMON)
    {
      monster_name = std::format("The {}", m->monstring);
    }
    else
    {
      monster_name = m->monstring;
    }

    if(m_statusp(*m, GREEDY) || m_statusp(*m, NEEDY))
    {
      m_pickup(m, o);
      queue_message(std::format("{} takes your gift.", monster_name));
      Player.alignment++;
      if(m_statusp(*m, GREEDY) && (true_item_value(o) < (long)m->level * 100))
      {
        append_message("...but does not appear satisfied.");
      }
      else if(m_statusp(*m, NEEDY) && (true_item_value(o) < (long)Level->depth * Level->depth))
      {
        append_message("...and looks chasteningly at you.");
      }
      else
      {
        append_message("...and seems happy with it.");
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
        queue_message(std::format("{} wolfs dwn your food ...", monster_name));
        m_status_reset(*m, HUNGRY);
        m_status_reset(*m, HOSTILE);
        if(o->on_use == I_POISON_FOOD)
        {
          Player.alignment -= 2;
          append_message("...and chokes on the poisoned ration!");
          m_status_set(*m, HOSTILE);
          m_damage(m, 100, POISON);
        }
        else
        {
          append_message("...and now seems satiated.");
        }
        delete o;
      }
      else
      {
        queue_message(std::format("{} spurns your offering and leaves it on the ground.", monster_name));
        drop_at(m->x, m->y, o);
      }
    }
    else
    {
      queue_message(std::format("{} doesn't care for your offering and drops it.", monster_name));
      drop_at(m->x, m->y, o);
    }
  }
}

/* will clear all, not just one of an object. */
void conform_lost_object(object *obj)
{
  if(obj)
  {
    conform_lost_objects(obj->number, obj);
  }
}

/* removes n of object from inventory; frees object if appropriate. */

void dispose_lost_objects(int n, object *obj)
{
  int i, conformed = false, subtracted = false;

  if(!obj)
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
        Player.possessions[i] = nullptr;
      }
    }
  }
  if(obj->number < 1)
  {
    delete obj;
  }
}

/* removes n of object from inventory without freeing object.
   Removes all instances of pointer (might be 2 handed weapon, etc) */
void conform_lost_objects(int n, object *obj)
{
  int i, conformed = false, subtracted = false;
  if(obj)
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
          Player.possessions[i] = nullptr;
        }
      }
    }
  }
}

/* clears unused possession */
void conform_unused_object(object *obj)
{
  if(obj->used)
  {
    item_unequip(obj);
  }
  calc_melee();
}

// Item identifiers, in this case the letters of the alphabet minus
// any letters already used for commands.  Yes, there are more here
// than could be needed, but I don't want to short myself for later.
char inventory_keymap[] = "-abcfghimnoqruvwyz";

// WDT -- convert from a char (keypress) to an item index in player inventory
int key_to_index(signed char key)
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

char index_to_key(signed int index)
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

/* select an item from inventory */
/* if itype is NULL_ITEM, any kind of item is acceptable.
   if itype is CASH, any kind of item or '$' (cash) is acceptable.
   if itype is FOOD, CORPSE or FOOD is acceptable, but only FOOD is
listed in the possibilities.
   if itype is any other object type (eg SCROLL, POTION, etc.), only
that type of item is acceptable or is listed */

int getitem(chtype itype)
{
  std::string invstr;
  char key;
  bool ok = false, drewmenu = false, found = false;

  found     = (itype == NULL_ITEM || (itype == CASH && Player.cash > 0));
  for(int i = 1; i < MAXITEMS; ++i)
  {
    if(Player.possessions[i])
    {
      if(itype == NULL_ITEM || itype == CASH || Player.possessions[i]->objchar == itype ||
         (itype == FOOD && Player.possessions[i]->objchar == CORPSE))
      {
        found       = true;
        invstr += index_to_key(i);
      }
    }
  }
  if((itype == CASH) && found)
  {
    invstr += '$';
  }
  if(!found)
  {
    queue_message("Nothing appropriate.");
    return (ABORT);
  }
  else
  {
    queue_message("Select an item [");
    queue_message(invstr);
    queue_message(",?] ");
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
          queue_message("You cannot select cash now.");
          ok = false;
        }
      }
      else if(!invstr.contains(key) || key_to_index(key) == -1)
      {
        queue_message("Nope! Try again [? for inventory, ESCAPE to quit]:");
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
      return ABORT;
    }
    else if(key == (CASH & 0xff))
    {
      return CASHVALUE;
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
      delete o;
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
      delete o;
      return true;
    }
  }
  return false;
}

/* inserts the item at the start of the pack array */
void push_pack(object *o)
{
  for(int i = Player.packptr; i > 0; i--)
  {
    Player.pack[i] = Player.pack[i - 1];
  }
  Player.pack[0] = o;
  Player.packptr++;
}

void add_to_pack(object *o)
{
  if(merge_item_with_pack(o))
  {
    return;
  }
  if(Player.packptr >= MAXPACK)
  {
    queue_message("Your pack is full. The item drops to the ground.");
    drop_at(Player.x, Player.y, o);
  }
  else
  {
    push_pack(o);
    queue_message("Putting item in pack.");
  }
}

int get_to_pack(object *o)
{
  if(merge_item_with_pack(o))
  {
    return true;
  }
  if(Player.packptr >= MAXPACK)
  {
    queue_message("Your pack is full.");
    return false;
  }
  else
  {
    push_pack(o);
    queue_message("Putting item in pack.");
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
    queue_message("You gained some cash.");
    Player.cash += o->basevalue;
    delete o;
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

// whether or not an item o can be used in a slot. Assumes o can in fact be placed in the slot.
bool item_useable(object *o, int slot)
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
        queue_message("You heft the weapon and find you must use both hands.");
        return true;
      }
      else
      {
        queue_message("This weapon is two-handed, so at the moment, ");
        queue_message("you are just lugging it around....");
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

// prevents people from wielding 3 short swords, etc.
void pack_extra_items(object *item)
{
  object *extra     = new object;
  *extra        = *item;
  extra->number = item->number - 1;
  extra->used   = false;
  item->number  = 1;
  if(Player.packptr < MAXPACK)
  {
    queue_message("Putting extra items back in pack.");
    push_pack(extra);
  }
  else
  {
    queue_message("No room for extra copies of item -- dropping them.");
    drop_at(Player.x, Player.y, extra);
  }
  calc_melee();
}

/* WDT -- 'response' must be an index into the pack. */
void use_pack_item(int response, int slot)
{
  object *item;
  int i;
  i = pack_item_cost(response);
  if(i > 10)
  {
    queue_message("You begin to rummage through your pack.");
  }
  if(i > 5)
  {
    queue_message("You search your pack for the item.");
  }
  queue_message("You take the item from your pack.");
  Command_Duration += i;
  item = Player.possessions[slot] = Player.pack[response];
  for(i = response; i < Player.packptr - 1; i++)
  {
    Player.pack[i] = Player.pack[i + 1];
  }
  Player.pack[--Player.packptr] = nullptr;

  if((slot == O_READY_HAND || slot == O_WEAPON_HAND) && twohandedp(item->id))
  {
    if(!Player.possessions[O_READY_HAND])
    {
      Player.possessions[O_READY_HAND] = item;
    }
    if(!Player.possessions[O_WEAPON_HAND])
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
  int i = start_item, items;
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
        if(items == 0)
        {
          menuprint("Items in Pack:\n");
        }
        menuprint(
          std::format("  {}: {} {}\n", static_cast<char>('a' + i), depth_string, itemid(Player.pack[i]))
        );
        ++items;
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
  int quit = false, ok = true;
  int response;
  if(Player.packptr < 1)
  {
    queue_message("Pack is empty!");
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
        queue_message("Enter pack slot letter or ESCAPE to quit.");
      }
      else if(last_item == Player.packptr)
      {
        queue_message("Enter pack slot letter, - to go back, or ESCAPE to quit.");
      }
      else if(pack_item == 0)
      {
        queue_message("Enter pack slot letter, + to see more, or ESCAPE to quit.");
      }
      else
      {
        queue_message("Enter pack slot letter, + or - to see more, or ESCAPE to quit.");
      }
      response = mcigetc();
      if(response == '?')
      {
        /* WDT HACK -- display some help instead. */
        queue_message("Help not implemented (sorry).");
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

// returns some number between 0 and o->number
int get_item_number(object *o)
{
  int n = 0;
  if(o->number == 1)
  {
    return 1;
  }
  do
  {
    queue_message(std::format("How many? -- max {}:", o->number));
    n = (int)parsenum();
    if(n > o->number)
    {
      queue_message("Too many!");
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

void put_to_pack(int slot)
{
  object *oslot = Player.possessions[slot];
  if(!oslot)
  {
    queue_message("Slot is empty!");
  }
  else if(oslot->blessing < 0 && oslot->used)
  {
    queue_message("Item is cursed!");
  }
  else
  {
    int num        = get_item_number(oslot);
    bool twohanded = (slot == O_READY_HAND || slot == O_WEAPON_HAND) && twohandedp(oslot->id);
    if(num >= oslot->number)
    {
      if(twohanded && oslot && Player.possessions[O_READY_HAND] == Player.possessions[O_WEAPON_HAND])
      {
        Player.possessions[O_READY_HAND]  = nullptr;
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

/* splits num off of item to make newitem which is returned */
/* something else (conform_lost_objects) has to reduce the actual
   number value of item and Player.itemweight */
object *split_item(int num, object *item)
{
  object *newitem = nullptr;
  if(item)
  {
    newitem  = new object;
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

// criteria for being able to put some item in some slot
bool slottable(object *o, int slot)
{
  bool ok = true;
  if(!o)
  {
    ok = false;
  }
  else if(slot == O_ARMOR)
  {
    if(o->objchar != ARMOR)
    {
      queue_message("Only armor can go in the armor slot!");
      ok = false;
    }
  }
  else if(slot == O_SHIELD)
  {
    if(o->objchar != SHIELD)
    {
      queue_message("Only a shield can go in the shield slot!");
      ok = false;
    }
  }
  else if(slot == O_BOOTS)
  {
    if(o->objchar != BOOTS)
    {
      queue_message("Only boots can go in the boots slot!");
      ok = false;
    }
  }
  else if(slot == O_CLOAK)
  {
    if(o->objchar != CLOAK)
    {
      queue_message("Only a cloak can go in the cloak slot!");
      ok = false;
    }
  }
  else if(slot >= O_RING1)
  {
    if(o->objchar != RING)
    {
      queue_message("Only a ring can go in a ring slot!");
      ok = false;
    }
  }
  return ok;
}

bool cursed(object *obj)
{
  if(!obj)
  {
    return false;
  }
  else
  {
    return obj->blessing < 0;
  }
}

// returns true if item with id and charge is found in pack or in
// inventory slot. charge is used to differentiate
// corpses instead of aux, which is their food value.
bool find_item(object **o, int id, int chargeval)
{
  int i;
  bool found = false;
  *o = nullptr;
  for(i = 1; ((i < MAXITEMS) && (!found)); i++)
  {
    if(Player.possessions[i])
    {
      if((Player.possessions[i]->id == id) && ((chargeval == -1) || (Player.possessions[i]->charge == chargeval)))
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
      if(Player.pack[i])
      {
        if((Player.pack[i]->id == id) && ((chargeval == -1) || (Player.pack[i]->charge == chargeval)))
        {
          *o    = Player.pack[i];
          found = true;
        }
      }
    }
  }
  return found;
}

// returns true if item with id and charge is found in pack or in
// inventory slot. Destroys item. charge is used to differentiate
// corpses instead of aux, which is their food value.
bool find_and_remove_item(int id, int chargeval)
{
  int i;
  bool found = false;
  object *o = nullptr;

  for(i = 1; ((i < MAXITEMS) && (!found)); i++)
  {
    if(Player.possessions[i])
    {
      if((Player.possessions[i]->id == id) && ((chargeval == -1) || (Player.possessions[i]->charge == chargeval)))
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
      if(Player.pack[i])
      {
        if((Player.pack[i]->id == id) && ((chargeval == -1) || (Player.pack[i]->charge == chargeval)))
        {
          Player.pack[i]->number--;
          if(Player.pack[i]->number == 0)
          {
            delete Player.pack[i];
            Player.pack[i] = nullptr;
          }
          found = true;
        }
      }
    }
  }
  fixpack();
  return found;
}

void lose_all_items()
{
  int i;
  queue_message("You notice that you are completely devoid of all possessions.");
  for(i = 0; i < MAXITEMS; i++)
  {
    if(Player.possessions[i])
    {
      dispose_lost_objects(Player.possessions[i]->number, Player.possessions[i]);
      Player.possessions[i] = nullptr;
    }
  }
  for(i = 0; i < MAXPACK; i++)
  {
    if(Player.pack[i])
    {
      delete Player.pack[i];
    }
    Player.pack[i] = nullptr;
  }
  Player.packptr = 0;
  calc_melee();
}

/* makes sure Player.pack is OK, (used after sale from pack) */
void fixpack()
{
  object *tpack[MAXPACK];
  int i, tctr = 0;
  for(i = 0; i < MAXPACK; i++)
  {
    tpack[i] = nullptr;
  }
  for(i = 0; i < MAXPACK; i++)
  {
    if(Player.pack[i])
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
