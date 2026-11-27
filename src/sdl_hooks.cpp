/*
Game-state queries for the SDL frontend: what to draw in each map cell and
what the context button should do.

This file is part of Omega (GPL-3.0-or-later).
*/
#include "sdl/frontend.hpp"

#include "defs.h"
#include "extern.h"
#include "glob.h"
#include "scr.h"

#include <SDL.h>

#include <string>
#include <unistd.h>
#include <vector>

// Omega looks for ./lib relative to the working directory. Launched from Steam
// or a desktop shortcut that's somewhere random, so start next to the binary.
static const bool chdir_to_binary = [] {
  if(!SDL_getenv("OMEGA_NO_CHDIR"))
  {
    if(char *base = SDL_GetBasePath())
    {
      if(chdir(base) != 0)
      {
        // fall back to the current directory
      }
      SDL_free(base);
    }
  }
  return true;
}();

namespace
{
struct terrain_name
{
  chtype ch;
  const char *key;
};

const terrain_name DUNGEON_TERRAIN[] = {
  {WALL, "wall"}, {FLOOR, "floor"}, {PORTCULLIS, "portcullis"}, {OPEN_DOOR, "open_door"},
  {CLOSED_DOOR, "closed_door"}, {WHIRLWIND, "whirlwind"}, {ABYSS, "abyss"}, {LAVA, "lava"},
  {HEDGE, "hedge"}, {WATER, "water"}, {FIRE, "fire"}, {TRAP, "trap"}, {LIFT, "lift"},
  {STAIRS_UP, "stairs_up"}, {STAIRS_DOWN, "stairs_down"}, {STATUE, "statue"}, {RUBBLE, "rubble"},
  {ALTAR, "altar"}, {CHAIR, "chair"}, {SAFE, "safe"}, {FURNITURE, "furniture"}, {BED, "bed"},
};

const terrain_name COUNTRY_TERRAIN[] = {
  {PLAINS, "c_plains"}, {TUNDRA, "c_tundra"}, {ROAD, "c_road"}, {MOUNTAINS, "c_mountains"},
  {PASS, "c_pass"}, {RIVER, "c_river"}, {CITY, "c_city"}, {VILLAGE, "c_village"},
  {FOREST, "c_forest"}, {JUNGLE, "c_jungle"}, {SWAMP, "c_swamp"}, {VOLCANO, "c_volcano"},
  {CASTLE, "c_castle"}, {TEMPLE, "c_temple"}, {CAVES, "c_caves"}, {DESERT, "c_desert"},
  {CHAOS_SEA, "c_chaos_sea"}, {STARPEAK, "c_starpeak"}, {DRAGONLAIR, "c_dragonlair"},
  {MAGIC_ISLE, "c_magic_isle"},
};

constexpr chtype GLYPH_MASK = A_CHARTEXT | A_COLOR;

template <size_t N>
const char *lookup(const terrain_name (&table)[N], chtype c)
{
  for(const auto &t : table)
  {
    if((t.ch & GLYPH_MASK) == (c & GLYPH_MASK))
    {
      return t.key;
    }
  }
  return nullptr;
}

const char *env_name(int env)
{
  switch(env)
  {
    case E_CITY: return "city";
    case E_VILLAGE: return "village";
    case E_SEWERS: return "sewers";
    case E_CASTLE: return "castle";
    case E_CAVES: return "caves";
    case E_VOLCANO: return "volcano";
    case E_ASTRAL: return "astral";
    case E_ARENA: return "arena";
    case E_HOVEL: return "hovel";
    case E_MANSION: return "mansion";
    case E_HOUSE: return "house";
    case E_DLAIR: return "dlair";
    case E_ABYSS: return "abyss";
    case E_TEMPLE: return "temple";
    default: return "";
  }
}

std::string object_key(const object *o)
{
  chtype c = o->objchar;
  const char *pool = nullptr;
  if(c == POTION) pool = "potion";
  else if(c == SCROLL) pool = "scroll";
  else if(c == RING) pool = "ring";
  else if(c == STICK) pool = "stick";
  else if(c == CLOAK) pool = "cloak";
  else if(c == BOOTS) pool = "boots";
  if(pool)
  {
    // keyed by the unidentified name, so the art never gives the game away
    return std::string("pool:") + pool + ":" + o->objstr;
  }
  return "o" + std::to_string(o->id);
}

bool same_glyph(chtype a, chtype b)
{
  return (a & A_CHARTEXT) == (b & A_CHARTEXT);
}

bool game_running()
{
  if(Current_Environment == E_COUNTRYSIDE)
  {
    return true;
  }
  return Level != nullptr && Current_Environment != E_NEVER_NEVER_LAND;
}
} // namespace

void omega_map_state(fe_map_state &s)
{
  s       = {};
  s.valid = game_running();
  if(!s.valid)
  {
    return;
  }
  s.country = Current_Environment == E_COUNTRYSIDE;
  s.env     = s.country ? "country" : env_name(Current_Environment);
  s.hoff    = HorizontalOffset;
  s.voff    = ScreenOffset;
  s.px      = Player.x;
  s.py      = Player.y;
  s.world_w = WIDTH;
  s.world_h = LENGTH;
}

void omega_cell_keys(int x, int y, chtype shown, std::vector<std::string> &out)
{
  out.clear();
  if(x < 0 || y < 0 || x >= WIDTH || y >= LENGTH)
  {
    out.push_back("glyph");
    return;
  }
  bool is_player = x == Player.x && y == Player.y && (shown & A_CHARTEXT) == '@';

  if(Current_Environment == E_COUNTRYSIDE)
  {
    const char *t = lookup(COUNTRY_TERRAIN, Country[x][y].current_terrain_type);
    if(t)
    {
      out.push_back(t);
    }
    if(is_player)
    {
      out.push_back("player");
    }
    else if(!t || !same_glyph(shown, Country[x][y].current_terrain_type))
    {
      out.push_back("glyph"); // something we don't know (spell effects etc.)
    }
    return;
  }

  const location &site = Level->site[x][y];
  chtype ground        = loc_statusp(x, y, SECRET, *Level) ? WALL : site.locchar;
  const char *t        = lookup(DUNGEON_TERRAIN, ground);
  if(!t)
  {
    t = "floor";
  }
  out.push_back(t);

  if(is_player)
  {
    out.push_back("player");
    return;
  }
  if(site.creature && same_glyph(shown, site.creature->monchar))
  {
    out.push_back("m" + std::to_string(site.creature->id));
    return;
  }
  if(!site.things.empty())
  {
    const object *top = site.things.back().get();
    if(same_glyph(shown, PILE) && site.things.size() > 1)
    {
      out.push_back("pile");
      return;
    }
    if(same_glyph(shown, top->objchar))
    {
      out.push_back(site.things.size() > 1 ? "pile" : object_key(top));
      return;
    }
  }
  if(same_glyph(shown, ground))
  {
    return; // plain terrain
  }
  // remembered or transient glyphs that don't match current state: trust the glyph
  if(const char *other = lookup(DUNGEON_TERRAIN, shown))
  {
    out.back() = other;
    return;
  }
  out.push_back("glyph");
}

int omega_context_key()
{
  if(!game_running())
  {
    return ' ';
  }
  if(Current_Environment == E_COUNTRYSIDE)
  {
    chtype t = Country[Player.x][Player.y].base_terrain_type;
    if(t == CITY || t == VILLAGE || t == CASTLE || t == TEMPLE || t == CAVES || t == VOLCANO ||
       t == DRAGONLAIR || t == STARPEAK || t == MAGIC_ISLE)
    {
      return '>';
    }
    return 's';
  }
  const location &here = Level->site[Player.x][Player.y];
  if(!here.things.empty())
  {
    return 'g';
  }
  if(here.locchar == STAIRS_DOWN)
  {
    return '>';
  }
  if(here.locchar == STAIRS_UP)
  {
    return '<';
  }
  if(here.p_locf != L_NO_OP)
  {
    return '@';
  }
  for(int dx = -1; dx <= 1; ++dx)
  {
    for(int dy = -1; dy <= 1; ++dy)
    {
      int x = Player.x + dx, y = Player.y + dy;
      if((dx || dy) && inbounds(x, y) && Level->site[x][y].locchar == CLOSED_DOOR)
      {
        return 'o';
      }
    }
  }
  return 's';
}

std::string omega_context_label()
{
  switch(omega_context_key())
  {
    case '>': return Current_Environment == E_COUNTRYSIDE ? "Enter" : "Go down";
    case '<': return "Go up";
    case 'g': return "Pick up";
    case '@': return "Use spot";
    case 'o': return "Open door";
    case 's': return "Search";
    default: return "";
  }
}

void omega_window_closed()
{
  if(game_running())
  {
    signalsave(0); // saves to Omega.sav and exits
  }
  endgraf();
  exit(0);
}

void omega_debug_change_env(int env)
{
  change_environment(env);
}
