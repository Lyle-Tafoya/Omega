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

/* effect2.c */

#include "glob.h"
#include "interactive_menu.hpp"
#include "scr.h"

#include <algorithm>
#include <format>
#include <string>
#include <vector>

extern interactive_menu *menu;

void knowledge(int blessing)
{
  if(blessing < 0)
  {
    queue_message("You feel ignorant.");
  }
  else
  {
    queue_message("You feel knowledgeable!");
    std::vector<std::string> lines;
    lines.emplace_back("Current Point Total: " + std::to_string(calc_points()));
    if(Player.alignment == 0)
    {
      lines.emplace_back("Alignment: Neutral, embodying the Cosmic Balance");
    }
    else if(abs(Player.alignment) < 10)
    {
      lines.emplace_back("Alignment: Neutral, tending toward ");
    }
    else if(abs(Player.alignment) < 50)
    {
      lines.emplace_back("Alignment: Neutral-");
    }
    else if(abs(Player.alignment) < 100)
    {
      lines.emplace_back("Alignment: ");
    }
    else if(abs(Player.alignment) < 200)
    {
      lines.emplace_back("Alignment: Servant of ");
    }
    else if(abs(Player.alignment) < 400)
    {
      lines.emplace_back("Alignment: Master of ");
    }
    else if(abs(Player.alignment) < 800)
    {
      lines.emplace_back("Alignment: The Essence of ");
    }
    else
    {
      lines.emplace_back("Alignment: The Ultimate Avatar of ");
    }
    if(Player.alignment < 0)
    {
      lines[lines.size() - 1] += "Chaos";
    }
    else if(Player.alignment > 0)
    {
      lines[lines.size() - 1] += "Law";
    }
    lines.emplace_back("");
    lines.emplace_back("Current stati:");
    if(Player.status[BLINDED])
    {
      lines.emplace_back("  Blinded");
    }
    if(Player.status[SLOWED])
    {
      lines.emplace_back("  Slowed");
    }
    if(Player.status[HASTED])
    {
      lines.emplace_back("  Hasted");
    }
    if(Player.status[DISPLACED])
    {
      lines.emplace_back("  Displaced");
    }
    if(Player.status[SLEPT])
    {
      lines.emplace_back("  Slept");
    }
    if(Player.status[DISEASED])
    {
      lines.emplace_back("  Diseased");
    }
    if(Player.status[POISONED])
    {
      lines.emplace_back("  Poisoned");
    }
    if(Player.status[BREATHING])
    {
      lines.emplace_back("  Breathing");
    }
    if(Player.status[INVISIBLE])
    {
      lines.emplace_back("  Invisible");
    }
    if(Player.status[REGENERATING])
    {
      lines.emplace_back("  Regenerating");
    }
    if(Player.status[VULNERABLE])
    {
      lines.emplace_back("  Vulnerable");
    }
    if(Player.status[BERSERK])
    {
      lines.emplace_back("  Berserk");
    }
    if(Player.status[IMMOBILE])
    {
      lines.emplace_back("  Immobile");
    }
    if(Player.status[ALERT])
    {
      lines.emplace_back("  Alert");
    }
    if(Player.status[AFRAID])
    {
      lines.emplace_back("  Afraid");
    }
    if(Player.status[ACCURATE])
    {
      lines.emplace_back("  Accurate");
    }
    if(Player.status[HERO])
    {
      lines.emplace_back("  Heroic");
    }
    if(Player.status[LEVITATING])
    {
      lines.emplace_back("  Levitating");
    }
    if(Player.status[TRUESIGHT])
    {
      lines.emplace_back("  Sharp");
    }
    if(Player.status[SHADOWFORM])
    {
      lines.emplace_back("  Shadowy");
    }
    if(Player.status[ILLUMINATION])
    {
      lines.emplace_back("  Glowing");
    }
    if(Player.status[DEFLECTION])
    {
      lines.emplace_back("  Buffered");
    }
    if(Player.status[RETURNING])
    {
      lines.emplace_back("  Returning");
    }
    lines.emplace_back("");
    lines.emplace_back("Immunities:");
    if(p_immune(NORMAL_DAMAGE))
    {
      lines.emplace_back("  Normal Damage");
    }
    if(p_immune(FLAME))
    {
      lines.emplace_back("  Flame");
    }
    if(p_immune(ELECTRICITY))
    {
      lines.emplace_back("  Electricity");
    }
    if(p_immune(COLD))
    {
      lines.emplace_back("  Cold");
    }
    if(p_immune(POISON))
    {
      lines.emplace_back("  Poison");
    }
    if(p_immune(ACID))
    {
      lines.emplace_back("  Acid");
    }
    if(p_immune(FEAR))
    {
      lines.emplace_back("  Fear");
    }
    if(p_immune(SLEEP))
    {
      lines.emplace_back("  Sleep");
    }
    if(p_immune(NEGENERGY))
    {
      lines.emplace_back("  Negative Energies");
    }
    if(p_immune(THEFT))
    {
      lines.emplace_back("  Theft");
    }
    if(p_immune(GAZE))
    {
      lines.emplace_back("  Gaze");
    }
    if(p_immune(INFECTION))
    {
      lines.emplace_back("  Infection");
    }
    lines.emplace_back("");
    lines.emplace_back("Ranks:");
    switch(Player.rank[LEGION])
    {
      case COMMANDANT:
        lines.emplace_back("  Commandant of the Legion");
        break;
      case COLONEL:
        lines.emplace_back("  Colonel of the Legion");
        break;
      case FORCE_LEADER:
        lines.emplace_back("  Force Leader of the Legion");
        break;
      case CENTURION:
        lines.emplace_back("  Centurion of the Legion");
        break;
      case LEGIONAIRE:
        lines.emplace_back("  Legionaire");
        break;
    }
    if(Player.rank[LEGION] > 0)
    {
      lines[lines.size() - 1] += std::format(" ({} XP)", Player.guildxp[LEGION]);
    }
    switch(Player.rank[ARENA])
    {
      case -1:
        lines.emplace_back("  Ex-gladiator");
        break;
      case CHAMPION:
        lines.emplace_back("  Gladiator Champion");
        break;
      case GLADIATOR:
        lines.emplace_back("  Gladiator of the Arena");
        break;
      case RETIARIUS:
        lines.emplace_back("  Retiarius of the Arena");
        break;
      case BESTIARIUS:
        lines.emplace_back("  Bestiarius of the Arena");
        break;
      case TRAINEE:
        lines.emplace_back("  Gladiator Trainee of the Arena");
        break;
    }
    if(Player.rank[ARENA] > 0)
    {
      lines[lines.size() - 1] += std::format(" (Opponent {})", Arena_Opponent);
    }
    switch(Player.rank[COLLEGE])
    {
      case ARCHMAGE:
        lines.emplace_back("  Archmage of the Collegium Magii");
        break;
      case MAGE:
        lines.emplace_back("  Collegium Magii: Mage");
        break;
      case PRECEPTOR:
        lines.emplace_back("  Collegium Magii: Preceptor");
        break;
      case STUDENT:
        lines.emplace_back("  Collegium Magii: Student");
        break;
      case NOVICE:
        lines.emplace_back("  Collegium Magii: Novice");
        break;
    }
    if(Player.rank[COLLEGE] > 0)
    {
      lines[lines.size() - 1] += std::format(" ({} XP)", Player.guildxp[COLLEGE]);
    }
    switch(Player.rank[NOBILITY])
    {
      case DUKE:
        lines.emplace_back("  Duke of Rampart");
        break;
      case LORD:
        lines.emplace_back("  Peer of the Realm");
        break;
      case KNIGHT:
        lines.emplace_back("  Order of the Knights of Rampart");
        break;
      case ESQUIRE:
        lines.emplace_back("  Squire of Rampart");
        break;
      case COMMONER:
        lines.emplace_back("  Commoner");
        break;
      default:
        lines.emplace_back("  Lowly Commoner");
        break;
    }
    if(Player.rank[NOBILITY] > 1)
    {
      lines[lines.size() - 1] += std::format(
        " ({}{} Quest Completed)", Player.rank[NOBILITY] - 1, ordinal(Player.rank[NOBILITY] - 1)
      );
    }
    else if(Player.rank[NOBILITY] == 1)
    {
      lines[lines.size() - 1] += " (1st Quest Undertaken)";
    }
    switch(Player.rank[CIRCLE])
    {
      case -1:
        lines.emplace_back("  Former member of the Circle.");
        break;
      case PRIME:
        lines.emplace_back("  Prime Sorceror of the Inner Circle");
        break;
      case HIGHSORCEROR:
        lines.emplace_back("  High Sorceror of the Inner Circle");
        break;
      case SORCEROR:
        lines.emplace_back("  Member of the Circle of Sorcerors");
        break;
      case ENCHANTER:
        lines.emplace_back("  Member of the Circle of Enchanters");
        break;
      case INITIATE:
        lines.emplace_back("  Member of the Circle of Initiates");
        break;
    }
    if(Player.rank[CIRCLE] > 0)
    {
      lines[lines.size() - 1] += std::format(" ({} XP)", Player.guildxp[CIRCLE]);
    }
    switch(Player.rank[ORDER])
    {
      case -1:
        lines.emplace_back("  Washout from the Order of Paladins");
        break;
      case JUSTICIAR:
        lines.emplace_back("  Justiciar of the Order of Paladins");
        break;
      case PALADIN:
        lines.emplace_back("  Paladin of the Order");
        break;
      case CHEVALIER:
        lines.emplace_back("  Chevalier of the Order");
        break;
      case GUARDIAN:
        lines.emplace_back("  Guardian of the Order");
        break;
      case GALLANT:
        lines.emplace_back("  Gallant of the Order");
        break;
    }
    if(Player.rank[ORDER] > 0)
    {
      lines[lines.size() - 1] += std::format(" ({} XP)", Player.guildxp[ORDER]);
    }
    switch(Player.rank[THIEVES])
    {
      case SHADOWLORD:
        lines.emplace_back("  Guild of Thieves: Shadowlord");
        break;
      case TMASTER:
        lines.emplace_back("  Guild of Thieves: Master Thief");
        break;
      case THIEF:
        lines.emplace_back("  Guild of Thieves: Thief");
        break;
      case ATHIEF:
        lines.emplace_back("  Guild of Thieves: Apprentice Thief");
        break;
      case TMEMBER:
        lines.emplace_back("  Guild of Thieves: Candidate Member");
        break;
    }
    if(Player.rank[THIEVES] > 0)
    {
      lines[lines.size() - 1] += std::format(" ({} XP)", Player.guildxp[THIEVES]);
    }
    switch(Player.rank[PRIESTHOOD])
    {
      case LAY:
        lines.emplace_back("  A lay devotee of ");
        break;
      case ACOLYTE:
        lines.emplace_back("  An Acolyte of ");
        break;
      case PRIEST:
        lines.emplace_back("  A Priest of ");
        break;
      case SPRIEST:
        lines.emplace_back("  A Senior Priest of ");
        break;
      case HIGHPRIEST:
        lines.emplace_back("  The High Priest of ");
        break;
    }
    switch(Player.patron)
    {
      case ODIN:
        lines[lines.size() - 1] += "Odin";
        break;
      case SET:
        lines[lines.size() - 1] += "Set";
        break;
      case ATHENA:
        lines[lines.size() - 1] += "Athena";
        break;
      case HECATE:
        lines[lines.size() - 1] += "Hecate";
        break;
      case DRUID:
        lines[lines.size() - 1] += "Druidism";
        break;
      case DESTINY:
        lines[lines.size() - 1] += "the Lords of Destiny";
        break;
    }
    if(Player.rank[PRIESTHOOD] > 0)
    {
      lines[lines.size() - 1] += std::format(" ({} XP)", Player.guildxp[PRIESTHOOD]);
    }
    if(Player.rank[MONKS] > 0)
    {
      switch(Player.rank[MONKS])
      {
        case MONK_GRANDMASTER:
          lines.emplace_back("Tholian Monks: Grandmaster");
          break;
        case MONK_MASTER_TEARS:
          lines.emplace_back("Tholian Monks: Master of Tears");
          break;
        case MONK_MASTER_PAINS:
          lines.emplace_back("Tholian Monks: Master of Pain");
          break;
        case MONK_MASTER_SIGHS:
          lines.emplace_back("Tholian Monks: Master of Sighs");
          break;
        case MONK_MASTER:
          lines.emplace_back("Tholian Monks: Master");
          break;
        case MONK_MONK:
          lines.emplace_back("Tholian Monks: Monk");
          break;
        case MONK_TRAINEE:
          lines.emplace_back("Tholian Monks: Trainee");
          break;
      }
      if(Player.rank[MONKS] > 0)
      {
        lines.back() += std::format(" ({} XP)", Player.guildxp[MONKS]);
      }
    }
    if(Player.rank[ADEPT] > 0)
    {
      lines.emplace_back("");
      lines.emplace_back("**************");
      lines.emplace_back("*Omegan Adept*");
      lines.emplace_back("**************");
    }
    menu->load(lines);
    menu->get_player_input();
    xredraw();
  }
}

/* Recreates the current level */
void flux(int)
{
  queue_message("The universe warps around you!");
  if(Current_Environment == E_CITY)
  {
    queue_message("Sensing dangerous high order magic, the Collegium Magii");
    queue_message("and the Circle of Sorcerors join forces to negate the spell.");
    queue_message("You are zapped by an antimagic ray!!!");
    dispel(-1);
    queue_message("The universe unwarps itself....");
  }
  else if(Current_Environment != Current_Dungeon)
  {
    queue_message("Odd.... No effect!");
  }
  else
  {
    queue_message("You stagger as the very nature of reality warps!");
    erase_level();
    Level->generated = false;
    queue_message("The fabric of spacetime reknits....");
    change_level(Level->depth - 1, Level->depth, true);
  }
}

/*Turns on displacement status for the player */
void displace(int blessing)
{
  if(blessing > -1)
  {
    queue_message("You feel a sense of dislocation.");
    Player.status[DISPLACED] = blessing + random_range(6);
  }
  else
  {
    queue_message("You feel vulnerable");
    Player.status[VULNERABLE] += random_range(6) - blessing;
  }
}

void invisible(int blessing)
{
  if(blessing > -1)
  {
    queue_message("You feel transparent!");
    Player.status[INVISIBLE] += 2 + 5 * blessing;
  }
  else
  {
    queue_message("You feel dangerous!");
    Player.status[VULNERABLE] += random_range(10) + 1;
  }
}

void warp(int blessing)
{
  int newlevel;
  if(Current_Environment != Current_Dungeon)
  {
    queue_message("How strange! No effect....");
  }
  else
  {
    queue_message("Warp to which level? ");
    newlevel = (int)parsenum();
    if(newlevel >= MaxDungeonLevels || blessing < 0 || newlevel < 1)
    {
      queue_message("You have been deflected!");
      newlevel = random_range(MaxDungeonLevels - 1) + 1;
    }
    queue_message("You dematerialize...");
    change_level(Level->depth, newlevel, false);
  }
  roomcheck();
}

void alert(int blessing)
{
  if(blessing > -1)
  {
    queue_message("You feel on-the-ball.");
    Player.status[ALERT] += 4 + (5 * blessing);
  }
  else
  {
    sleep_player(abs(blessing) + 3);
  }
}

void regenerate(int blessing)
{
  if(blessing < 0)
  {
    heal(blessing * 10);
  }
  else
  {
    queue_message("You feel abnormally healthy.");
    Player.status[REGENERATING] += (blessing + 1) * 50;
  }
}

void haste(int blessing)
{
  if(blessing > -1)
  {
    if(!Player.status[HASTED])
    {
      queue_message("The world slows down!");
    }
    else
    {
      queue_message("Nothing much happens.");
    }
    if(Player.status[SLOWED])
    {
      Player.status[SLOWED] = 0;
    }
    Player.status[HASTED] += (blessing * 100) + random_range(250);
  }
  else
  {
    queue_message("You feel slower.");
    if(Player.status[HASTED] > 0)
    {
      queue_message("...but the feeling quickly fades.");
    }
    else
    {
      Player.status[SLOWED] += random_range(250) + 250;
    }
  }
}

void recover_stat(int blessing)
{
  if(blessing < 0)
  {
    queue_message("You feel a cold surge!");
    switch(random_range(6))
    {
      case 0:
        Player.str = std::min(Player.str - 1, Player.maxstr - 1);
        break;
      case 1:
        Player.con = std::min(Player.con - 1, Player.maxcon - 1);
        break;
      case 2:
        Player.dex = std::min(Player.dex - 1, Player.maxdex - 1);
        break;
      case 3:
        Player.agi = std::min(Player.agi - 1, Player.maxagi - 1);
        break;
      case 4:
        Player.iq = std::min(Player.iq - 1, Player.maxiq - 1);
        break;
      case 5:
        Player.pow = std::min(Player.pow - 1, Player.maxpow - 1);
        break;
    }
  }
  else
  {
    queue_message("You feel a warm tingle!");
    Player.str = std::max(Player.str, Player.maxstr);
    Player.con = std::max(Player.con, Player.maxcon);
    Player.dex = std::max(Player.dex, Player.maxdex);
    Player.agi = std::max(Player.agi, Player.maxagi);
    Player.iq  = std::max(Player.iq, Player.maxiq);
    Player.pow = std::max(Player.pow, Player.maxpow);
  }
  calc_melee();
}

void augment(int blessing)
{
  if(blessing < 0)
  {
    queue_message("You feel a cold surge!");
    switch(random_range(6))
    {
      case 0:
        Player.str = std::min(Player.str - 1, Player.maxstr - 1);
        break;
      case 1:
        Player.con = std::min(Player.con - 1, Player.maxcon - 1);
        break;
      case 2:
        Player.dex = std::min(Player.dex - 1, Player.maxdex - 1);
        break;
      case 3:
        Player.agi = std::min(Player.agi - 1, Player.maxagi - 1);
        break;
      case 4:
        Player.iq = std::min(Player.iq - 1, Player.maxiq - 1);
        break;
      case 5:
        Player.pow = std::min(Player.pow - 1, Player.maxpow - 1);
        break;
    }
  }
  else if(blessing == 0)
  {
    queue_message("You feel a warm tingle!");
    switch(random_range(6))
    {
      case 0:
        Player.str = std::max(Player.str + 1, Player.maxstr + 1);
        break;
      case 1:
        Player.con = std::max(Player.con + 1, Player.maxcon + 1);
        break;
      case 2:
        Player.dex = std::max(Player.dex + 1, Player.maxdex + 1);
        break;
      case 3:
        Player.agi = std::max(Player.agi + 1, Player.maxagi + 1);
        break;
      case 4:
        Player.iq = std::max(Player.iq + 1, Player.maxiq + 1);
        break;
      case 5:
        Player.pow = std::max(Player.pow + 1, Player.maxpow + 1);
        break;
    }
  }
  else
  {
    queue_message("You feel a hot flash!");
    Player.str = std::max(Player.str + 1, Player.maxstr + 1);
    Player.con = std::max(Player.con + 1, Player.maxcon + 1);
    Player.dex = std::max(Player.dex + 1, Player.maxdex + 1);
    Player.agi = std::max(Player.agi + 1, Player.maxagi + 1);
    Player.iq  = std::max(Player.iq + 1, Player.maxiq + 1);
    Player.pow = std::max(Player.pow + 1, Player.maxpow + 1);
  }
  calc_melee();
}

void breathe(int blessing)
{
  if(blessing > -1)
  {
    queue_message("Your breath is energized!");
    Player.status[BREATHING] += 6 + blessing;
  }
  else
  {
    queue_message("You choke as your lungs fill with water!");
    p_damage(50, UNSTOPPABLE, "drowning");
  }
}

void i_chaos(object *)
{
  if(Player.alignment < 0)
  {
    Player.alignment -= random_range(20);
    queue_message("You feel deliciously chaotic!");
    gain_experience(abs(Player.alignment) * 10);
  }
  else
  {
    queue_message("You feel a sense of inner turmoil!");
    Player.alignment -= random_range(20);
  }
}

void i_law(object *)
{
  if(Player.alignment > 0)
  {
    Player.alignment += random_range(20);
    queue_message("You feel wonderfully lawful!");
    gain_experience(Player.alignment * 10);
  }
  else
  {
    queue_message("You feel a sense of inner constraint!");
    Player.alignment += random_range(20);
  }
}

void sanctify(int blessing)
{
  if(blessing > -1)
  {
    if(Level->environment == E_TEMPLE)
    {
      queue_message("Odd, the spell has no effect. I wonder why.");
    }
    else if(Level->site[Player.x][Player.y].locchar == ALTAR)
    {
      queue_message("This site can't get any holier!");
    }
    else if(Player.patron == 0)
    {
      queue_message("The gods are angered!");
      Level->site[Player.x][Player.y].locchar = LAVA;
      Level->site[Player.x][Player.y].p_locf  = L_LAVA;
      lset(Player.x, Player.y, CHANGED, *Level);
      p_movefunction(L_LAVA);
    }
    else
    {
      Level->site[Player.x][Player.y].locchar = ALTAR;
      Level->site[Player.x][Player.y].aux     = Player.patron;
      Level->site[Player.x][Player.y].p_locf  = L_ALTAR;
      lset(Player.x, Player.y, CHANGED, *Level);
      queue_message("You are standing on sacred ground!");
    }
  }
  else
  {
    if(Level->site[Player.x][Player.y].locchar == ALTAR)
    {
      queue_message("The altar crumbles before your unholy blast....");
      Level->site[Player.x][Player.y].locchar = FLOOR;
      Level->site[Player.x][Player.y].p_locf  = L_NO_OP;
      lset(Player.x, Player.y, CHANGED, *Level);
      if(Level->site[Player.x][Player.y].aux == Player.patron)
      {
        queue_message("Your deity is not amused....");
        p_damage(Player.hp - 1, UNSTOPPABLE, "Divine Wrath");
      }
      else if((Player.patron == ATHENA) || (Player.patron == ODIN))
      {
        if((Level->site[Player.x][Player.y].aux == SET) || (Level->site[Player.x][Player.y].aux == HECATE))
        {
          queue_message("Your deity applauds the eradication of Chaos' taint");
          gain_experience(1000);
        }
        else
        {
          queue_message("Your deity approves of your action.");
          gain_experience(100);
        }
      }
      else if((Player.patron == SET) || (Player.patron == HECATE))
      {
        if((Level->site[Player.x][Player.y].aux == ODIN) || (Level->site[Player.x][Player.y].aux == ATHENA))
        {
          queue_message("Your deity applauds the obliteration of Law");
          gain_experience(1000);
        }
        else
        {
          queue_message("Your deity approves of your action.");
          gain_experience(100);
        }
      }
      else if(Player.patron == DRUID)
      {
        queue_message("Your attempt to maintain the Balance is applauded....");
        gain_experience(250);
      }
      else
      {
        queue_message("Nothing much happens");
      }
    }
    else
    {
      queue_message("You feel an aura of unholiness arising from this spot....");
    }
  }
}

void accuracy(int blessing)
{
  if(blessing > -1)
  {
    queue_message("Everything seems covered in bullseyes!");
    Player.status[ACCURACY] += random_range(5) + 1 + blessing * 5;
    calc_melee();
  }
  else
  {
    Player.status[ACCURACY] = 0;
    calc_melee();
    queue_message("Your vision blurs...");
  }
}
