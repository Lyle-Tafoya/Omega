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

/* etc.c */
/* grab bag of random functions used in random places */

#include "glob.h"
#include "spell.h"

#include <array>
#include <string_view>

/* there are various ways for the player to receive one of these hints */
void hint()
{
  switch(random_range(96))
  {
    case 0:
      mprint("There is an entrance to the sewers in the Garden.");
      break;
    case 1:
      mprint("Statues can be dangerous.");
      break;
    case 2:
      mprint("Unidentified Artifacts can be dangerous.");
      break;
    case 3:
      mprint("The higher form of mercury is desirable.");
      break;
    case 4:
      mprint("A sense of unease is a good thing to have.");
      break;
    case 5:
      mprint("If you dig too much, you might cause a cave-in!");
      break;
    case 6:
      mprint("Be Lawful: Live and Let Live.");
      break;
    case 7:
      mprint("Be Chaotic: Live and Let Die.");
      break;
    case 8:
      mprint("The world doesn't slow down; you speed up.");
      break;
    case 9:
      mprint("Security is a sense of dislocation.");
      break;
    case 10:
      mprint("Tullimore Dew is a panacea.");
      break;
    case 11:
      mprint("Thieves hide behind closed doors.");
      break;
    case 12:
      mprint("`No jail is escapeproof' -- John Dillinger.");
      break;
    case 13:
      mprint("Oh, to have an apartment of your own!");
      break;
    case 14:
      mprint("Some homes have money and treasure.");
      break;
    case 15:
      mprint("Some homes are defended.");
      break;
    case 16:
      mprint("Sometimes you could just wish for Death.");
      break;
    case 17:
      mprint("A cursed wish can be fatal.");
      break;
    case 18:
      mprint("The way you play, you should wish for Skill.");
      break;
    case 19:
      mprint("A druid might wish for Balance.");
      break;
    case 20:
      mprint("Mages always wish for Knowledge.");
      break;
    case 21:
      mprint("Some fairies are good.");
      break;
    case 22:
      mprint("An affair with a demon can be heartbreaking.");
      break;
    case 23:
      mprint("The Explorer's Club knows a useful spell.");
      break;
    case 24:
      mprint("They say some famous people live in mansions.");
      break;
    case 25:
      mprint("Magic pools are totally random.");
      break;
    case 26:
      mprint("There are five elements, including Void.");
      break;
    case 27:
      mprint("Humans can be good or evil, lawful or chaotic.");
      break;
    case 28:
      mprint("There are many kinds of wishes. Case counts, you know.");
      break;
    case 29:
      mprint("There are caves due south of Rampart");
      break;
    case 30:
      mprint("Donaldson's Giants can withstand lava.");
      break;
    case 31:
      mprint("Ritual magic can have many different effects.");
      break;
    case 32:
      mprint("The Mercenaries are the best equipped fighters.");
      break;
    case 33:
      mprint("The Gladiators are the most skilled fighters.");
      break;
    case 34:
      mprint("Rent a flat and lose any bad stati you may have.");
      break;
    case 35:
      mprint("Some junk may be worth a fortune if identified.");
      break;
    case 36:
      mprint("Identify humans by talking to them.");
      break;
    case 37:
      mprint("They say the Duke has a treasure trove.");
      break;
    case 38:
      mprint("If you yield, your opponent will gain experience.");
      break;
    case 39:
      mprint("The Dragon Lord lives in the Waste of Time.");
      break;
    case 40:
      mprint("A full moon bodes well for the followers of Law.");
      break;
    case 41:
      mprint("A new moon omens evil for the Law-abiding.");
      break;
    case 42:
      mprint("Druids revere the half-moon.");
      break;
    case 43:
      mprint("Most grot is useless.");
      break;
    case 44:
      mprint("Cash can sometimes be found in the walls.");
      break;
    case 45:
      mprint("Pointy weapons break often but dig better.");
      break;
    case 46:
      mprint("The DREADED AQUAE MORTIS is invulnerable.");
      break;
    case 47:
      mprint("There must be *some* reason to worship Destiny!");
      break;
    case 48:
      mprint("Kill a trifid? A puzzle! Try a saline solution!");
      break;
    case 49:
      mprint("Beware! The Eater of Souls inhabits the abyss!");
      break;
    case 50:
      mprint("They say there's a red-light district in town.");
      break;
    case 51:
      mprint("The House of the Eclipse is behind a closed door.");
      break;
    case 52:
      mprint("The Orbs may be encountered on the Astral Plane.");
      break;
    case 53:
      mprint("The Champion should never refuse a challenge.");
      break;
    case 54:
      mprint("They say that the autoteller program is buggy.");
      break;
    case 55:
      mprint("It's better not to sleep on the ground.");
      break;
    case 56:
      mprint("Try ritual magic in different kinds of rooms.");
      break;
    case 57:
      mprint("Breaking down a wall by bashing it is a bad idea!");
      break;
    case 58:
      mprint("Follow the Oracle's advice; she is all-wise.");
      break;
    case 59:
      mprint("The ArchDruid lives in the northern forest.");
      break;
    case 60:
      mprint("A search of the mountains may reveal a secret pass.");
      break;
    case 61:
      mprint("Star Peak is to the far North-East.");
      break;
    case 62:
      mprint("The Archmage lives in the far North-West beyond a pass.");
      break;
    case 63:
      mprint("There is a volcano in the southern marshes.");
      break;
    case 64:
      mprint("The Demon Emperor resides in the Volcano.");
      break;
    case 65:
      mprint("The Lawgiver can be found at Star Peak.");
      break;
    case 66:
      mprint("The Temple of Athena is to the North-East.");
      break;
    case 67:
      mprint("The Temple of Set can be found in a desert.");
      break;
    case 68:
      mprint("The Temple of Hecate is in the swamp.");
      break;
    case 69:
      mprint("The Temple of Odin is to the South in some mountains.");
      break;
    case 70:
      mprint("There is a curious island off a promontory of the swamp.");
      break;
    case 71:
      mprint("The Eater of Magic can be found on an island.");
      break;
    case 72:
      mprint("The Temple of Destiny is practically inaccessible.");
      break;
    case 73:
      mprint("Each sect has its own main temple outside the city.");
      break;
    case 74:
      mprint("The aligned temples are dangerous to unbelievers.");
      break;
    case 75:
      mprint("If you are poor, maybe you should wish for Wealth.");
      break;
    case 76:
      mprint("Need mana? Wish for Power.");
      break;
    case 77:
      mprint("Wishing for Law, Balance, or Chaos alters alignment.");
      break;
    case 78:
      mprint("Feeling out of sorts? Wish for Health.");
      break;
    case 79:
      mprint("Challenge the abyss at the Temple of Destiny.");
      break;
    case 80:
      mprint("The Circle of Sorcerors has an Astral HQ");
      break;
    case 81:
      mprint("The Star Gem is the only way back from the Astral Plane.");
      break;
    case 82:
      mprint("The Star Gem is guarded by the Circle of Sorcerors.");
      break;
    case 83:
      mprint("The Star Gem is rightfully the property of the LawBringer.");
      break;
    case 84:
      mprint("They say the Demon Emperor owns the Amulet of the Planes.");
      break;
    case 85:
      mprint("An Amulet might get you to the Temple of Destiny.");
      break;
    case 86:
      mprint("A wish for Location might help you become Adept.");
      break;
    case 87:
      mprint("Some Artifacts may be used only once per day.");
      break;
    case 88:
      mprint("Overusing Artifacts can be a bad move.");
      break;
    case 89:
      mprint("You might starve in the Swamp or the Mountains!");
      break;
    case 90:
      mprint("You would have to be very chaotic to attack a guard!");
      break;
    case 91:
      mprint("You would have to be very foolhardy to attack a guard!");
      break;
    case 92:
      mprint("Only a master of chaos would kill all the city guards!");
      break;
    case 93:
      mprint("The Order depends on the force of the LawGiver.");
      break;
    case 94:
      mprint("City Guards are employees of the Order.");
      break;
    case 95:
      mprint("Disenchanted Artifacts can be restored.");
      break;
  }
}

/* for when a deity teaches spells to a devotee */
void learnclericalspells(int deity, int level)
{
  mprint("With your new clerical rank comes knowledge of magic...");
  Player.pow += level;
  Player.maxpow += level;
  switch(level)
  {
    case LAY:
      if(deity == ODIN)
      {
        spell::Spells[spell::MAGIC_MISSILE].known = true;
      }
      else if(deity == SET)
      {
        spell::Spells[spell::INVISIBILITY].known = true;
      }
      else if(deity == ATHENA)
      {
        spell::Spells[spell::IDENTIFICATION].known = true;
      }
      else if(deity == HECATE)
      {
        spell::Spells[spell::ENERGY_DRAIN].known = true;
      }
      else if(deity == DRUID)
      {
        spell::Spells[spell::SELF_KNOWLEDGE].known = true;
        spell::Spells[spell::MONSTER_DETECTION].known   = true;
      }
      break;
    case ACOLYTE:
      if(deity == ODIN)
      {
        spell::Spells[spell::BALL_LIGHTNING].known     = true;
        spell::Spells[spell::TRUESIGHT].known = true;
      }
      else if(deity == SET)
      {
        spell::Spells[spell::SUMMONING].known   = true;
        spell::Spells[spell::FIREBOLT].known = true;
      }
      else if(deity == ATHENA)
      {
        spell::Spells[spell::HEALING].known      = true;
        spell::Spells[spell::SANCTUARY].known = true;
      }
      else if(deity == HECATE)
      {
        spell::Spells[spell::SLEEP].known  = true;
        spell::Spells[spell::DISPELLING].known = true;
      }
      else if(deity == DRUID)
      {
        spell::Spells[spell::HEALING].known = true;
        spell::Spells[spell::CURING].known = true;
      }
      else if(deity == DESTINY)
      {
        mprint("An acolyte of the Lords of Destiny. Gee whiz.");
      }
      break;
    case PRIEST:
      spell::Spells[spell::SANCTIFICATION].known = true;
      if(deity == ODIN)
      {
        spell::Spells[spell::HEROISM].known = true;
        spell::Spells[spell::HEALING].known = true;
      }
      else if(deity == SET)
      {
        spell::Spells[spell::INVISIBILITY].known = true;
        spell::Spells[spell::DISPELLING].known    = true;
      }
      else if(deity == ATHENA)
      {
        spell::Spells[spell::REGENERATE].known = true;
        spell::Spells[spell::ACCURACY].known   = true;
      }
      else if(deity == HECATE)
      {
        spell::Spells[spell::SHADOWFORM].known = true;
        spell::Spells[spell::CURING].known       = true;
      }
      else if(deity == DRUID)
      {
        spell::Spells[spell::DISRUPT].known      = true;
        spell::Spells[spell::ALERTNESS].known        = true;
        spell::Spells[spell::CLAIRVOYANCE].known = true;
      }
      else if(deity == DESTINY)
      {
        mprint("How useless, a new priest of the Lords of Destiny.");
      }
      break;
    case SPRIEST:
      spell::Spells[spell::BLESSING].known = true;
      if(deity == ODIN)
      {
        spell::Spells[spell::ACCURACY].known = true;
      }
      else if(deity == SET)
      {
        spell::Spells[spell::SHADOWFORM].known = true;
      }
      else if(deity == ATHENA)
      {
        spell::Spells[spell::HEROISM].known = true;
      }
      else if(deity == HECATE)
      {
        spell::Spells[spell::POLYMORPH].known = true;
      }
      else if(deity == DRUID)
      {
        spell::Spells[spell::POLYMORPH].known = true;
        spell::Spells[spell::LEVITATE].known  = true;
      }
      else if(deity == DESTINY)
      {
        mprint("Wow, a new senior priest of the Lords of Destiny.");
      }
      break;
    case HIGHPRIEST:
      if(deity == ODIN)
      {
        spell::Spells[spell::RESTORATION].known = true;
      }
      else if(deity == SET)
      {
        spell::Spells[spell::HELLFIRE].known = true;
      }
      else if(deity == ATHENA)
      {
        spell::Spells[spell::HELLFIRE].known = true;
      }
      else if(deity == HECATE)
      {
        spell::Spells[spell::DESECRATION].known = true;
      }
      else if(deity == DRUID)
      {
        spell::Spells[spell::DISINTEGRATE].known = true;
        spell::Spells[spell::HEROISM].known         = true;
      }
      else if(deity == DESTINY)
      {
        mprint("So you're now the high priest of the Lords of Destiny.");
        mprint("You didn't think you were going to get anything, did you?");
      }
  }
}

constexpr std::array names
{
  "Orion Splash",
  "Gorgar",
  "Hieronymous",
  "Quantifor Quotron",
  "Leon",
  "Joyce",
  "Leticia Smiley",
  "Ogilvy the Grim",
  "Salara Storn",
  "Murgo",
  "Jonathan Atwilder",
  "Xylos the Tan",
  "Terence",
  "Toronado",
  "Kelly",
  "Cantinflas",
  "Ixel",
  "Toto",
  "Frost",
  "Aliera Erinyes",
  "Godel",
  "Kerst Blackblade",
  "Ebenezer",
  "Jeremiah",
  "Triskelion Shadow",
  "Eleskir Eremar",
  "Tyron",
  "Morgon",
  "Achmed",
  "Chin",
  "Fujimoto",
  "Dos Santos",
  "Federico",
  "Jaime",
  "Siobhan",
  "Hans",
  "Gurkov",
  "Krilos the Slayer",
  "Oxxblud",
  "Dorian"
};

/* random names for various uses */
std::string_view nameprint()
{
  return names[random_range(names.size())];
}

/* returns english string equivalent of number */
const std::string wordnum(int num)
{
  switch(num)
  {
    case 0:
      return "zero ";
    case 1:
      return "one ";
    case 2:
      return "two ";
    case 3:
      return "three ";
    case 4:
      return "four ";
    case 5:
      return "five ";
    case 6:
      return "six ";
    case 7:
      return "seven ";
    case 8:
      return "eight ";
    case 9:
      return "nine ";
    case 10:
      return "ten ";
    default:
      return "";
  }
}
