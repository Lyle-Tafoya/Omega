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

#ifndef OMEGA_SPELLS_H_
#define OMEGA_SPELLS_H_

#include <array>
#include <cstdint>
#include <functional>
#include <optional>
#include <string>

typedef uint16_t spell_id_type;

struct spell
{
  enum spell_id : spell_id_type
  {
    ACCURACY = 0,
    ALERTNESS,
    APPORTATION,
    BALL_LIGHTNING,
    BLESSING,
    BREATHING,
    CLAIRVOYANCE,
    CURING,
    DESECRATION,
    DISINTEGRATE,
    DISPELLING,
    DISRUPT,
    ENCHANTMENT,
    ENERGY_DRAIN,
    FEAR,
    FIREBOLT,
    HASTE,
    HEALING,
    HELLFIRE,
    HEROISM,
    IDENTIFICATION,
    INVISIBILITY,
    LEVITATE,
    MAGIC_MISSILE,
    MONSTER_DETECTION,
    OBJECT_DETECTION,
    POLYMORPH,
    REGENERATE,
    RESTORATION,
    RETURN,
    RITUAL_MAGIC,
    SANCTIFICATION,
    SANCTUARY,
    SELF_KNOWLEDGE,
    SHADOWFORM,
    SLEEP,
    SUMMONING,
    TELEPORT,
    THE_WARP,
    TRUESIGHT,
    WISH
  };

  spell_id id;
  int powerdrain;
  std::function<void()> cast;
  bool known = false;

  static constexpr spell_id_type NUM_SPELLS = spell::WISH + 1;
  static std::array<spell, NUM_SPELLS> Spells;
  static const std::array<std::string, NUM_SPELLS> spell_names;
};

extern void cast_spell(spell::spell_id id);
extern std::optional<spell::spell_id> getspell();

#endif
