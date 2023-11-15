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

/* spell.c */
/* functions having to do with spellcasting */

#include "spell.h"

#include "extern.h"
#include "glob.h"
#include "interactive_menu.hpp"
#include "scr.h"
#include "scrolling_buffer.hpp"

#include <algorithm>
#include <array>
#include <format>
#include <optional>
#include <string>
#include <vector>

extern scrolling_buffer message_buffer;
extern interactive_menu *menu;

void cast_spell(spell::spell_id id)
{
  spell::Spells[id].cast();
}

void s_wish()
{
  if(random_range(100) > Player.iq + Player.pow + Player.level)
  {
    queue_message("Your concentration is flawed!");
    queue_message("The spell energy backfires!");
    p_damage(random_range(spell::Spells[spell::WISH].powerdrain), UNSTOPPABLE, "a backfired wish spell");
  }
  else
  {
    wish(0);
    if(spell::Spells[spell::WISH].known)
    {
      queue_message("The power of the spell is too much for you to withstand!");
      queue_message("All memory of the spell is expunged from your brain.");
      spell::Spells[spell::WISH].known = false;
    }
  }
}

void s_firebolt()
{
  int x = Player.x, y = Player.y;
  setspot(x, y);
  fbolt(Player.x, Player.y, x, y, Player.dex * 2 + Player.level, Player.level * 10 + 10);
}

void s_missile()
{
  int x = Player.x, y = Player.y;
  setspot(x, y);
  nbolt(Player.x, Player.y, x, y, Player.dex * 2 + Player.level, Player.level * 3 + 3);
}

void s_teleport()
{
  p_teleport(0);
}

void s_disrupt()
{
  int x = Player.x, y = Player.y;
  setspot(x, y);
  disrupt(x, y, Player.level * 10 + 25);
}

void s_disintegrate()
{
  int x = Player.x, y = Player.y;
  setspot(x, y);
  disintegrate(x, y);
}

void s_sleep()
{
  sleep_monster(0);
}

void s_heal()
{
  heal(3);
}

void s_dispel()
{
  dispel((Player.level + Player.maxpow) / 10);
}

void s_breathe()
{
  breathe(0);
}

void s_invisible()
{
  invisible(0);
}

void s_warp()
{
  warp(1);
}

void s_enchant()
{
  enchant(1);
}

void s_bless()
{
  bless(0);
}

void s_restore()
{
  recover_stat(0);
}

void s_cure()
{
  cure(0);
}

void s_truesight()
{
  truesight(0);
}

void s_hellfire()
{
  int x = Player.x, y = Player.y;
  setspot(x, y);
  hellfire(x, y, 0);
}

void s_knowledge()
{
  knowledge(0);
}

void s_hero()
{
  hero(0);
}

/* spell takes longer and longer to work deeper into dungeon */
void s_return()
{
  queue_message("You hear a whine as your spell begins to charge up.");
  Player.status[RETURNING] = ((Current_Environment == Current_Dungeon) ? difficulty() : 1);
}

void s_desecrate()
{
  sanctify(-1);
}

void s_haste()
{
  haste(0);
}

void s_summon()
{
  summon(0, -1);
}

void s_sanctuary()
{
  sanctuary();
}

void s_sanctify()
{
  sanctify(1);
}

void s_accuracy()
{
  accuracy(0);
}

void s_fear()
{
  int x = Player.x, y = Player.y;
  setspot(x, y);
  inflict_fear(x, y);
}

/* Has all kinds of effects in different circumstances.
   Eventually will be more interesting */
void s_ritual()
{
  object *symbol;
  int roomno;
  int x, y;

  queue_message("You begin your ritual....");
  queue_message("You enter a deep trance. Time Passes...");
  setgamestatus(SKIP_PLAYER, GameStatus);
  time_clock(false);
  setgamestatus(SKIP_PLAYER, GameStatus);
  time_clock(false);
  setgamestatus(SKIP_PLAYER, GameStatus);
  time_clock(false);
  setgamestatus(SKIP_PLAYER, GameStatus);
  time_clock(false);
  setgamestatus(SKIP_PLAYER, GameStatus);
  time_clock(false);
  if(RitualDay == day() && RitualHour == hour())
  {
    queue_message("Your mental fatigue prevents from completing the ritual!");
  }
  else if(random_range(100) > Player.iq + Player.pow + Player.level)
  {
    queue_message("Your concentration was broken -- the ritual fails!");
  }
  else
  {
    queue_message("You charge the ritual with magical energy and focus your will.");
    queue_message("Time Passes...");
    setgamestatus(SKIP_PLAYER, GameStatus);
    time_clock(false);
    setgamestatus(SKIP_PLAYER, GameStatus);
    time_clock(false);
    setgamestatus(SKIP_PLAYER, GameStatus);
    time_clock(false);
    setgamestatus(SKIP_PLAYER, GameStatus);
    time_clock(false);
    setgamestatus(SKIP_PLAYER, GameStatus);
    time_clock(false);
    RitualDay  = day();
    RitualHour = hour();
    /* set of random conditions for different ritual effects */
    if(Current_Environment == E_CITY)
    {
      queue_message("Flowing waves of mystical light congeal all around you.");
      queue_message("'Like wow, man! Colors!'");
      queue_message("Appreciative citizens throw you spare change.");
      Player.cash += random_range(50);
    }
    else if((roomno = Level->site[Player.x][Player.y].roomnumber) >= 0)
    {
      if(RitualRoom == roomno)
      {
        queue_message("For some reason the ritual doesn't work this time...");
      }
      else
      {
        RitualRoom = roomno;
        switch(RitualRoom)
        {
          case ROOMBASE + 9: /* ransacked treasure chamber */
            queue_message("Your spell sets off frenetic growth all around you!");
            for(int i = 0; i < 8; ++i)
            {
              Level->site[Player.x + Dirs[0][i]][Player.y + Dirs[1][i]].locchar = HEDGE;
              Level->site[Player.x + Dirs[0][i]][Player.y + Dirs[1][i]].p_locf  = L_TRIFFID;
              lset(Player.x + Dirs[0][i], Player.y + Dirs[1][i], CHANGED, *Level);
            }
            break;
          case ROOMBASE + 13: /* harem */
          case ROOMBASE + 22: /* boudoir */
            queue_message("A secret panel opens next to the bed....");
            if(random_range(2))
            {
              summon(0, INCUBUS); /* succubus/incubus */
            }
            else
            {
              summon(0, SATYR); /* satyr/nymph */
            }
            break;
          case ROOMBASE + 26: /*shrine to high magic */
            queue_message("A storm of mana coaelesces around you.");
            queue_message("You are buffeted by bursts of random magic.");
            p_damage(random_range(Player.pow), UNSTOPPABLE, "high magic");
            queue_message("Continue ritual? Could be dangerous.... [yn] ");
            if(ynq() == 'y')
            {
              s_wish();
            }
            else
            {
              queue_message("The mana fades away to nothingness.");
            }
            x = Player.x;
            y = Player.y;
            while(x >= 0 && Level->site[x - 1][y].roomnumber == ROOMBASE + 26)
            {
              --x;
            }
            while(y >= 0 && Level->site[x][y - 1].roomnumber == ROOMBASE + 26)
            {
              --y;
            }
            for(int i = 0; Level->site[x][y].roomnumber == ROOMBASE + 26;)
            {
              Level->site[x][y].roomnumber = RS_ZORCH;
              ++x;
              ++i;
              if(Level->site[x][y].roomnumber != ROOMBASE + 26)
              {
                x -= i;
                i = 0;
                ++y;
              }
            }
            lset(Player.x, Player.y, CHANGED, *Level);
            break;
          case ROOMBASE + 27: /* magician's lab */
            queue_message("Your magical activity sets off a latent spell in the lab!");
            cast_spell(static_cast<spell::spell_id>(random_range(spell::NUM_SPELLS)));
            break;
          case ROOMBASE + 28: /* pentagram room */
            queue_message("A smoky form begins to coalesce....");
            summon(-1, -1);
            queue_message("Fortunately, it seems confined to the pentagram.");
            m_status_reset(*Level->mlist->m, MOBILE);
            break;
          case ROOMBASE + 29: /* blue omega room */
            queue_message("The Lords of Destiny look upon you....");
            if(Player.level > 10)
            {
              queue_message("A curtain of blue flames leaps up from the omega.");
              l_adept();
            }
            else
            {
              if(Player.patron == DESTINY)
              {
                queue_message("Your patrons take pity on you.");
                if((Player.rank[PRIESTHOOD] < SPRIEST) && (!find_item(&symbol, ARTIFACTID + 19, -1)))
                {
                  symbol         = new object;
                  *symbol        = Objects[ARTIFACTID + 19];
                  symbol->known  = 2;
                  symbol->charge = 17;
                  gain_item(symbol);
                  queue_message("You feel uplifted.");
                }
                else
                {
                  gain_experience(std::min(1000l, Player.xp));
                }
              }
              else if(random_range(3) == 1)
              {
                queue_message("You feel Fated.");
                gain_experience(Player.level * Player.level * 10);
                Player.hp = std::max(Player.hp, Player.maxhp);
              }
              else if(random_range(2))
              {
                queue_message("You feel Doomed.");
                Player.hp   = 1;
                Player.mana = 0;
                Player.xp   = 0;
              }
              else
              {
                queue_message("The Lords of Destiny laugh at you!");
              }
            }
            break;
          default:
            queue_message("Well, not much effect. Chalk it up to experience.");
            gain_experience(Player.level * 5);
            break;
        }
      }
    }
    else
    {
      if(RitualRoom == Level->site[Player.x][Player.y].roomnumber)
      {
        queue_message("The ritual fails for some unexplainable reason.");
      }
      else
      {
        queue_message("The ritual seems to be generating some spell effect.");
        RitualRoom = Level->site[Player.x][Player.y].roomnumber;
        switch(RitualRoom)
        {
          case RS_WALLSPACE:
            shadowform();
            break;
          case RS_CORRIDOR:
            haste(0);
            break;
          case RS_PONDS:
            breathe(0);
            break;
          case RS_ADEPT:
            hero(1);
            break;
          default:
            queue_message("The ritual doesn't seem to produce any tangible results...");
            gain_experience(Player.level * 6);
        }
      }
    }
  }
}

void s_apport()
{
  apport(0);
}

void s_shadowform()
{
  shadowform();
}

void s_alert()
{
  alert(0);
}

void s_regenerate()
{
  regenerate(0);
}

void s_clairvoyance()
{
  clairvoyance(10);
}

void s_drain()
{
  drain(0);
}

void s_levitate()
{
  levitate(0);
}

void s_polymorph()
{
  polymorph(0);
}

/* lball spell */
void s_lball()
{
  int x = Player.x, y = Player.y;
  setspot(x, y);
  lball(Player.x, Player.y, x, y, Player.level * 10 + 10);
}

void s_identify()
{
  identify(0);
}

void s_objdet()
{
  objdet(1);
}

void s_mondet()
{
  mondet(1);
}

std::array<spell, spell::NUM_SPELLS> spell::Spells{
  {{spell::ACCURACY, 20, s_accuracy},
   {spell::ALERTNESS, 15, s_alert},
   {spell::APPORTATION, 15, s_apport},
   {spell::BALL_LIGHTNING, 25, s_lball},
   {spell::BLESSING, 30, s_bless},
   {spell::BREATHING, 20, s_breathe},
   {spell::CLAIRVOYANCE, 10, s_clairvoyance},
   {spell::CURING, 20, s_cure},
   {spell::DESECRATION, 50, s_desecrate},
   {spell::DISINTEGRATE, 40, s_disintegrate},
   {spell::DISPELLING, 40, s_dispel},
   {spell::DISRUPT, 30, s_disrupt},
   {spell::ENCHANTMENT, 30, s_enchant},
   {spell::ENERGY_DRAIN, 40, s_drain},
   {spell::FEAR, 10, s_fear},
   {spell::FIREBOLT, 20, s_firebolt},
   {spell::HASTE, 15, s_haste},
   {spell::HEALING, 15, s_heal},
   {spell::HELLFIRE, 90, s_hellfire},
   {spell::HEROISM, 20, s_hero},
   {spell::IDENTIFICATION, 10, s_identify},
   {spell::INVISIBILITY, 15, s_invisible},
   {spell::LEVITATE, 25, s_levitate},
   {spell::MAGIC_MISSILE, 10, s_missile},
   {spell::MONSTER_DETECTION, 3, s_mondet},
   {spell::OBJECT_DETECTION, 3, s_objdet},
   {spell::POLYMORPH, 30, s_polymorph},
   {spell::REGENERATE, 20, s_regenerate},
   {spell::RESTORATION, 20, s_restore},
   {spell::RETURN, 10, s_return},
   {spell::RITUAL_MAGIC, 50, s_ritual},
   {spell::SANCTIFICATION, 75, s_sanctify},
   {spell::SANCTUARY, 75, s_sanctuary},
   {spell::SELF_KNOWLEDGE, 10, s_knowledge},
   {spell::SHADOWFORM, 50, s_shadowform},
   {spell::SLEEP, 15, s_sleep},
   {spell::SUMMONING, 20, s_summon},
   {spell::TELEPORT, 20, s_teleport},
   {spell::THE_WARP, 50, s_warp},
   {spell::TRUESIGHT, 20, s_truesight},
   {spell::WISH, 100, s_wish}}
};

const std::array<std::string, spell::NUM_SPELLS> spell::spell_names{
  "accuracy",
  "alertness",
  "apportation",
  "ball lightning",
  "blessing",
  "breathing",
  "clairvoyance",
  "curing",
  "desecration",
  "disintegrate",
  "dispelling",
  "disrupt",
  "enchantment",
  "energy drain",
  "fear",
  "firebolt",
  "haste",
  "healing",
  "hellfire",
  "heroism",
  "identification",
  "invisibility",
  "levitate",
  "magic missile",
  "monster detection",
  "object detection",
  "polymorph",
  "regeneration",
  "restoration",
  "return",
  "ritual magic",
  "sanctification",
  "sanctuary",
  "self knowledge",
  "shadow form",
  "sleep",
  "summoning",
  "teleport",
  "the warp",
  "true sight",
  "wishing"};

std::vector<std::string> known_spells(int first, int last)
{
  std::vector<std::string> lines;
  lines.emplace_back("Possible Spells:");

  bool printed = false;
  for(int i = first; i <= last; ++i)
  {
    if(spell::Spells[i].known)
    {
      printed = true;
      lines.emplace_back(
        std::format("  {} ({} mana)", spell::spell_names[i], spell::Spells[i].powerdrain)
      );
    }
  }
  if(!printed)
  {
    lines.emplace_back("No spells match that prefix!");
  }
  return lines;
}

std::optional<spell::spell_id> spellparse()
{
  spell_id_type first = 0;
  while(first < spell::NUM_SPELLS && !spell::Spells[first].known)
  {
    ++first;
  }
  if(first == spell::NUM_SPELLS)
  {
    append_message("You don't know any spells!", true);
    return {};
  }
  std::string prefix;
  bool found         = false;
  spell_id_type last = spell::NUM_SPELLS - 1;
  bool menu_shown    = false;
  int player_input;
  append_message("", true);
  do
  {
    if(menu_shown)
    {
      menu->load(known_spells(first, last));
      player_input = menu->get_player_input();
    }
    else
    {
      player_input = mgetc();
    }
    switch(player_input)
    {
      case KEY_BACKSPACE:
      case KEY_DC:
      case DELETE:
      case '\b':
        if(!prefix.empty())
        {
          prefix.pop_back();
          for(int f = first; f >= 0 && spell::spell_names[f].starts_with(prefix); --f)
          {
            if(spell::Spells[f].known)
            {
              first = f;
            }
          }
          for(int l = last; l < spell::NUM_SPELLS && spell::spell_names[l].starts_with(prefix); ++l)
          {
            if(spell::Spells[l].known)
            {
              last = l;
            }
          }
          if(found)
          {
            found = 0;
          }
          message_buffer.replace_last(prefix);
        }
        if(prefix.empty())
        {
          first = 0;
          last  = spell::NUM_SPELLS - 1;
          found = 0;
        }
        break;
      case ESCAPE:
        message_buffer.replace_last("_ Spell cancelled.");
        xredraw();
        return {};
      case '?':
        menu_shown = true;
        break;
      case '\n':
        break;
      default:
        if(std::isupper(player_input))
        {
          player_input = std::tolower(player_input);
        }
        if(found)
        {
          continue;
        }
        int f = first;
        while(f < spell::NUM_SPELLS &&
              (!spell::Spells[f].known || spell::spell_names[f].length() < prefix.length() ||
               spell::spell_names[f][prefix.length()] < player_input))
        {
          ++f;
        }
        int l = last;
        while(l >= 0 && (!spell::Spells[l].known || spell::spell_names[l].length() < prefix.length() ||
                         spell::spell_names[l][prefix.length()] > player_input))
        {
          --l;
        }
        if(l < f)
        {
          continue;
        }
        prefix += player_input;
        message_buffer.replace_last(prefix);
        first = f;
        last  = l;
        if(first == last && !found) // unique name
        {
          found = 1;
          message_buffer.replace_last(spell::spell_names[first]);
        }
        break;
    }
  } while(player_input != '\n');
  xredraw();
  if(found)
  {
    return static_cast<spell::spell_id>(first);
  }
  else
  {
    append_message("That is an ambiguous abbreviation!", true);
    return {};
  }
}

/* select a spell to cast */
std::optional<spell::spell_id> getspell()
{
  queue_message("Cast Spell: [type spell abbrev, ?, or ESCAPE]: ");
  return spellparse();
}
