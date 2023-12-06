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

// inv.cpp
// functions having to do with player item inventory

#include "glob.h"
#include "scr.h"

#include <array>
#include <cassert>
#include <format>
#include <span>
#include <string>
#include <vector>

// returns some money from player back into "money" item.
// for giving and dropping money
std::unique_ptr<object> detach_money()
{
  long c = get_money(Player.cash);
  if(c == ABORT)
  {
    return nullptr;
  }
  else
  {
    Player.cash -= c;
    auto cash = std::make_unique<object>();
    make_cash(cash.get(), difficulty());
    cash->basevalue = c;
    return cash;
  }
}

// drops money, heh heh
void drop_money()
{

  std::unique_ptr<object> money = detach_money();
  if(money)
  {
    if(Current_Environment == E_CITY)
    {
      queue_message("As soon as the money leaves your hand,");
      queue_message("a horde of scrofulous beggars snatch it up and are gone!");
    }
    else
    {
      drop_at(Player.x, Player.y, std::move(money));
    }
  }
  else
  {
    setgamestatus(SKIP_MONSTERS, GameStatus);
  }
}

// gets a legal amount of money or ABORT
long get_money(long limit)
{
  queue_message("How much? ");
  long c = parsenum();
  if(c > limit || c <= 0)
  {
    queue_message("Forget it, buddy.");
    return ABORT;
  }
  else
  {
    return c;
  }
}

// pick up from some location x,y
void pickup_at(int x, int y)
{
  resetgamestatus(FAST_MOVE, GameStatus);

  std::vector<std::unique_ptr<object>> &items = Level->site[x][y].things;
  if(items.size() == 1)
  {
    std::unique_ptr<object> tmp = std::move(items.back());
    items.pop_back();
    gain_item(std::move(tmp));
  }
  else
  {
    for(auto i = items.size(); i-- > 0;)
    {
      std::unique_ptr<object> &item = items[i];
      queue_message(std::format("Pick up: {} [ynq] ", itemid(item.get())));
      char player_input = ynq();
      if(player_input == 'y')
      {
        std::unique_ptr<object> tmp = std::move(item);
        items.erase(items.begin() + i);
        gain_item(std::move(tmp));
      }
      else if(player_input == 'q')
      {
        break;
      }
    }
  }
}

// criteria for being able to put some item in some slot
int aux_slottable(const object *o, int slot)
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
  return ok;
}

// are two objects equal except for their number field?
// returns false if either object is null
int objequal(const object *o, const object *p)
{
  if(!o || !p)
  {
    return false;
  }
  else
  {
    return o->id == p->id && o->weight == p->weight && o->plus == p->plus &&
      o->charge == p->charge && o->dmg == p->dmg && o->hit == p->hit &&
      o->aux == p->aux && o->fragility == p->fragility &&
      o->basevalue == p->basevalue&& o->known == p->known &&
      o->blessing == p->blessing && o->on_use == p->on_use &&
      o->on_equip == p->on_equip && o->on_unequip == p->on_unequip;
  }
}

bool merge_item(std::span<std::unique_ptr<object>> items, const object *o, int n)
{
  for(std::unique_ptr<object> &item : items)
  {
    if(!item)
    {
      continue;
    }
    if(o->objchar == CASH && item->objchar == CASH)
    {
      item->basevalue += o->basevalue;
      return true;
    }
    else if(objequal(item.get(), o) && item->objchar != STICK)
    {
      item->number += n;
      return true;
    }
  }
  return false;
}

// put all of o on objlist at x,y on Level->depth
// Not necessarily dropped by character; just dropped...
void drop_at(int x, int y, std::unique_ptr<object> o)
{
  if(Current_Environment != E_COUNTRYSIDE)
  {
    if((Level->site[x][y].locchar != VOID_CHAR) && (Level->site[x][y].locchar != ABYSS))
    {
      if(merge_item(Level->site[x][y].things, o.get(), o->number))
      {
        return;
      }
      o->used = false;
      Level->site[x][y].things.emplace_back(std::move(o));
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
      if(merge_item(Level->site[x][y].things, o, n))
      {
        return;
      }
      auto cpy = std::make_unique<object>(*o);
      cpy->used   = false;
      cpy->number = n;
      Level->site[x][y].things.emplace_back(std::move(cpy));
    }
    else if(Level->site[x][y].p_locf == L_VOID_STATION)
    {
      setgamestatus(PREPARED_VOID, GameStatus);
    }
  }
}

// return an object's plus as a string
std::string getplusstr(const object *obj)
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
std::string getchargestr(const object *obj)
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

// return an object's number as a string
std::string getnumstr(const object *obj)
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
std::string itemid(object *obj, int known)
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
    if(known < 0)
    {
      known = obj->known;
    }

    item_name += getnumstr(obj);
    if(known == 0)
    {
      item_name += obj->objstr;
    }
    else if(known == 1)
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
  std::unique_ptr<object> cash = detach_money();
  if(!cash)
  {
    setgamestatus(SKIP_MONSTERS, GameStatus);
  }
  else
  {
    givemonster(m, std::move(cash));
  }
}

void givemonster(monster *m, std::unique_ptr<object> o)
{
  // special case -- give gem to LawBringer
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
      long value = true_item_value(o.get());
      m_pickup(m, std::move(o));
      queue_message(std::format("{} takes your gift.", monster_name));
      Player.alignment++;
      if(m_statusp(*m, GREEDY) && (value < (long)m->level * 100))
      {
        append_message("...but does not appear satisfied.");
      }
      else if(m_statusp(*m, NEEDY) && (value < (long)Level->depth * Level->depth))
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
      if(((m->id == HORSE) && (o->id == FOODID + 15)) || // grain
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
      }
      else
      {
        queue_message(std::format("{} spurns your offering and leaves it on the ground.", monster_name));
        drop_at(m->x, m->y, std::move(o));
      }
    }
    else
    {
      queue_message(std::format("{} doesn't care for your offering and drops it.", monster_name));
      drop_at(m->x, m->y, std::move(o));
    }
  }
}

// removes n of object from inventory; frees object if appropriate
void dispose_lost_objects(int n, int slot)
{
  std::unique_ptr<object> &item = Player.possessions[slot];
  if(!item)
  {
    return;
  }
  item->number -= n;
  if(item->number < 1)
  {
    conform_unused_object(item);
    item.reset();
  }
}

// clears unused possession
void conform_unused_object(std::unique_ptr<object> &obj)
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
  assert(MAXITEMS > 0); // must have room for an item, or this loop will die!

  for(int i = 0; i < MAXITEMS; ++i)
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

// select an item from inventory
// if itype is NULL_ITEM, any kind of item is acceptable.
// if itype is CASH, any kind of item or '$' (cash) is acceptable.
// if itype is FOOD, CORPSE or FOOD is acceptable, but only FOOD is
// listed in the possibilities.
// if itype is any other object type (eg SCROLL, POTION, etc.), only
// that type of item is acceptable or is listed

int getitem(chtype itype)
{
  std::string invstr;
  bool found = itype == NULL_ITEM || (itype == CASH && Player.cash > 0);
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
  if(itype == CASH && found)
  {
    invstr += '$';
  }
  if(!found)
  {
    queue_message("Nothing appropriate.");
    return ABORT;
  }
  else
  {
    queue_message("Select an item [");
    queue_message(invstr);
    queue_message(",?] ");
    bool drewmenu = false;
    char key;
    for(bool ok = false; !ok;)
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

bool merge_item_with_pack(const object *o)
{
  if(!o || o->objchar == STICK)
  {
    return false;
  }
  for(std::unique_ptr<object> &item : Player.pack)
  {
    if(item && objequal(o, item.get()))
    {
      item->number += o->number;
      return true;
    }
  }
  return false;
}

bool merge_item_with_inventory(const object *o)
{
  if(!o && o->objchar == STICK)
  {
    return false;
  }
  const std::array slots{O_LEFT_SHOULDER, O_RIGHT_SHOULDER, O_BELT1, O_BELT2, O_BELT3};
  for(int slot : slots)
  {
    object *pack_item = Player.possessions[slot].get();
    if(pack_item && objequal(o, pack_item))
    {
      pack_item->number += o->number;
      return true;
    }
  }
  return false;
}

// inserts the item at the end of the pack array
void push_pack(std::unique_ptr<object> o)
{
  Player.pack.emplace_back(std::move(o));
}

void add_to_pack(std::unique_ptr<object> o)
{
  if(merge_item_with_pack(o.get()))
  {
    return;
  }
  if(Player.pack.size() >= MAXPACK)
  {
    queue_message("Your pack is full. The item drops to the ground.");
    drop_at(Player.x, Player.y, std::move(o));
  }
  else
  {
    push_pack(std::move(o));
    queue_message("Putting item in pack.");
  }
}

void get_to_pack(std::unique_ptr<object> o)
{
  if(merge_item_with_pack(o.get()))
  {
    return;
  }
  if(Player.pack.size() >= MAXPACK)
  {
    queue_message("Your pack is full.");
    p_drop_at(Player.x, Player.y, o->number, o.get());
  }
  else
  {
    queue_message("Putting item in pack.");
    push_pack(std::move(o));
  }
}

void gain_item(std::unique_ptr<object> o)
{
  if(o->uniqueness == UNIQUE_MADE)
  {
    Objects[o->id].uniqueness = UNIQUE_TAKEN;
  }
  if(o->objchar == CASH)
  {
    queue_message("You gained some cash.");
    Player.cash += o->basevalue;
    dataprint();
  }
  else if(merge_item_with_inventory(o.get()))
  {
    queue_message("You add it to the stack in your inventory");
  }
  else
  {
    get_to_pack(std::move(o));
  }
  calc_melee();
}

int pack_item_cost(size_t index)
{
  index = Player.pack.size() - 1 - index;
  if(index > 20)
  {
    return 17;
  }
  else if(index > 15)
  {
    return 7;
  }
  else
  {
    return 2;
  }
}

bool is_two_handed(const object *o)
{
  if(o && twohandedp(o->id))
  {
    return true;
  }
  else
  {
    return false;
  }
}

// whether or not an item o can be used in a slot. Assumes o can in fact be placed in the slot.
bool item_useable(const object *o, int slot)
{
  if(slot == O_ARMOR || slot == O_CLOAK || slot == O_SHIELD || slot == O_BOOTS || slot >= O_RING1)
  {
    return true;
  }
  else if(o->objchar == WEAPON || o->objchar == MISSILEWEAPON)
  {
    if(is_two_handed(o) && (slot == O_WEAPON_HAND || slot == O_READY_HAND))
    {
      if(is_two_handed(Player.possessions[O_WEAPON_HAND].get()) && !Player.possessions[O_READY_HAND])
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
  auto extra = std::make_unique<object>(*item);
  extra->number = item->number - 1;
  extra->used   = false;
  item->number  = 1;
  if(Player.pack.size() < MAXPACK)
  {
    queue_message("Putting extra items back in pack.");
    push_pack(std::move(extra));
  }
  else
  {
    queue_message("No room for extra copies of item -- dropping them.");
    drop_at(Player.x, Player.y, std::move(extra));
  }
  calc_melee();
}

// WDT -- 'response' must be an index into the pack
void use_pack_item(size_t response, int slot)
{
  int duration = pack_item_cost(response);
  if(duration > 10)
  {
    queue_message("You begin to rummage through your pack.");
  }
  if(duration > 5)
  {
    queue_message("You search your pack for the item.");
  }
  queue_message("You take the item from your pack.");
  Command_Duration += duration;

  Player.possessions[slot] = std::move(Player.pack[response]);
  Player.pack.erase(Player.pack.begin() + response);

  if(item_useable(Player.possessions[slot].get(), slot))
  {
    item_equip(Player.possessions[slot]);
    if(Player.possessions[slot]->number > 1)
    {
      pack_extra_items(Player.possessions[slot].get());
    }
  }
}

// WDT HACK!  This ought to be in scr.c, along with its companion.  However,
// right now it's only used in the function directly below.
size_t aux_display_pack(size_t start_item, int slot)
{
  size_t i = Player.pack.size() - start_item;
  if(Player.pack.empty())
  {
    queue_message("Pack is empty.");
  }
  else if(Player.pack.size() <= start_item)
  {
    queue_message("You see the leather at the bottom of the pack.");
  }
  else
  {
    menuclear();
    int items = 0;
    while(i-- > 0  && items < ScreenLength - 5)
    {
      if(aux_slottable(Player.pack[i].get(), slot))
      {
        std::string depth_string;
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
          std::format("  {}: {} {}\n", static_cast<char>('a' + Player.pack.size() - 1 - i), depth_string, itemid(Player.pack[i].get()))
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
  return Player.pack.size() - 1 - i;
}

// takes something from pack, puts to slot
void take_from_pack(int slot)
{
  if(Player.pack.empty())
  {
    queue_message("Pack is empty!");
  }
  else
  {
    int response;
    size_t pack_item = 0;
    bool quit = false, ok;
    do
    {
      ok        = true;
      size_t last_item = aux_display_pack(pack_item, slot);
      if(last_item == Player.pack.size() && pack_item == 0)
      {
        queue_message("Enter pack slot letter or ESCAPE to quit.");
      }
      else if(last_item == Player.pack.size())
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
        // WDT HACK -- display some help instead.
        queue_message("Help not implemented (sorry).");
        ok = false;
      }
      else if(response == ESCAPE)
      {
        quit = true;
      }
      else if(response == '+')
      {
        if(last_item < Player.pack.size())
        {
          pack_item = last_item;
        }
        ok = false;
      }
      else if(response == '-')
      {
        // WDT HACK: this _should_ make us page up.  Sadly,
        // I have no way of calculating how much I'll be paging up.
        // This is fixable, but I have no idea how much work...
        pack_item = 0;
        ok        = false;
      }
      else
      {
        size_t pack_index = response - 'a';
        ok = response >= 'a' && response <= 'z' && pack_index < Player.pack.size();
        if(ok)
        {
          ok = slottable(Player.pack[Player.pack.size() - 1 - pack_index].get(), slot);
        }
      }
    } while(!ok);
    if(!quit)
    {
      use_pack_item(Player.pack.size() - 1 - (response - 'a'), slot);
    }
  }
  print_inventory_menu();
}

// returns some number between 0 and o->number
int get_item_number(const object *o)
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
  return n;
}

void put_to_pack(int slot)
{
  std::unique_ptr<object> &inventory_item = Player.possessions[slot];
  if(!inventory_item)
  {
    queue_message("Slot is empty!");
  }
  else if(inventory_item->blessing < 0 && inventory_item->used)
  {
    queue_message("Item is cursed!");
  }
  else
  {
    int num = get_item_number(inventory_item.get());
    if(num >= inventory_item->number)
    {
      if(is_two_handed(Player.possessions[O_WEAPON_HAND].get()) &&
        (slot == O_READY_HAND && !Player.possessions[O_READY_HAND]))
      {
        conform_unused_object(Player.possessions[O_WEAPON_HAND]);
        add_to_pack(std::move(Player.possessions[O_WEAPON_HAND]));
      }
      else
      {
        conform_unused_object(inventory_item);
        add_to_pack(std::move(inventory_item));
      }
    }
    else if(num > 0)
    {
      std::unique_ptr<object> o = split_item(num, inventory_item.get());
      add_to_pack(std::move(o));
      dispose_lost_objects(num, slot);
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
      case 18: // ^l, ^r
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
            if(slot == O_READY_HAND && is_two_handed(Player.possessions[O_WEAPON_HAND].get()))
            {
              put_to_pack(O_WEAPON_HAND);
              Command_Duration += 2;
            }
            else
            {
              take_from_pack(slot);
              Command_Duration += 5;
            }
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

// splits num off of item to make newitem which is returned
// something else (dispose_lost_objects) has to reduce the actual
// number value of item and Player.itemweight
std::unique_ptr<object> split_item(int num, const object *item)
{
  if(item)
  {
    auto o = std::make_unique<object>(*item);
    if(num <= item->number)
    {
      o->number = num;
    }
    o->used = false;
    return o;
  }
  else
  {
    return nullptr;
  }
}

// criteria for being able to put some item in some slot
bool slottable(const object *o, int slot)
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

bool cursed(const object *obj)
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
bool find_item(object *&o, int id, int chargeval)
{
  o = nullptr;
  for(std::unique_ptr<object> &item : Player.possessions)
  {
    if(item)
    {
      if(item->id == id && (chargeval == -1 || item->charge == chargeval))
      {
        o = item.get();
        return true;
      }
    }
  }
  for(std::unique_ptr<object> &item : Player.pack)
  {
    if(item)
    {
      if(item->id == id && (chargeval == -1 || item->charge == chargeval))
      {
        o = item.get();
        return true;
      }
    }
  }
  return false;
}

// returns true if item with id and charge is found in pack or in
// inventory slot. Destroys item. charge is used to differentiate
// corpses instead of aux, which is their food value.
bool find_and_remove_item(int id, int chargeval)
{
  for(int i = 1; i < MAXITEMS; ++i)
  {
    std::unique_ptr<object> &item = Player.possessions[i];
    if(item)
    {
      if(item->id == id && (chargeval == -1 || item->charge == chargeval))
      {
        dispose_lost_objects(1, i);
        return true;
      }
    }
  }
  for(auto it = Player.pack.begin(); it != Player.pack.end(); ++it)
  {
    std::unique_ptr<object> &item = *it;
    if(item)
    {
      if(item->id == id && (chargeval == -1 || item->charge == chargeval))
      {
        if(--item->number == 0)
        {
          Player.pack.erase(it);
        }
        return true;
      }
    }
  }
  return false;
}

void lose_all_items()
{
  queue_message("You notice that you are completely devoid of all possessions.");
  for(int i = 0; i < MAXITEMS; ++i)
  {
    if(Player.possessions[i])
    {
      dispose_lost_objects(Player.possessions[i]->number, i);
    }
  }
  Player.pack.clear();
  calc_melee();
}
