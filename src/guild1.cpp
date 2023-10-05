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

/* guild1.c */
/* L_ functions  */

/* These functions implement the various guilds. */
/* They are all l_ functions since they are basically activated*/
/* at some site or other. */

#include "glob.h"
#include "scr.h"
#include "spell.h"

#include <algorithm>
#include <format>
#include <string>

void l_merc_guild()
{
  pob newitem;

  queue_message("Legion of Destiny, Mercenary Guild, Inc.");
  if(nighttime())
  {
    append_message("The barracks are under curfew right now.", true);
  }
  else
  {
    append_message("You enter Legion HQ,", true);
    if(Player.rank[LEGION] == COMMANDANT)
    {
      append_message("Your aide follows you to the staff room.");
    }
    if(Player.rank[LEGION] > 0)
    {
      append_message("and report to your commander.");
    }
    switch(Player.rank[LEGION])
    {
      case 0:
        append_message("and see the Recruiting Centurion.");
        append_message("Do you wish to join the Legion? [yn] ", true);
        if(ynq() == 'y')
        {
          if(Player.rank[ARENA] > 0)
          {
            append_message("The Centurion checks your record, and gets angry:", true);
            append_message("'The legion don't need any Arena Jocks. Git!'");
          }
          else if(Player.rank[ORDER] > 0)
          {
            append_message("The Centurion checks your record, and is amused:", true);
            append_message("'A paladin in the ranks? You must be joking.'");
          }
          else if(Player.con < 12)
          {
            append_message("The Centurion looks you over, sadly.", true);
            append_message("You are too fragile to join the legion.");
          }
          else if(Player.str < 10)
          {
            append_message("The Centurion looks at you contemptuously.", true);
            append_message("Your strength is too low to pass the physical!");
          }
          else
          {
            append_message("You are tested for strength and stamina...", true);
            append_message("and you pass!");
            append_message(std::format("Commandant {} shakes your hand.", Commandant), true);
            append_message("The Legion pays you a 500Au induction fee.", true);
            append_message("You are also issued a shortsword and leather.");
            append_message("You are now a Legionaire.", true);
            newitem  = new object;
            *newitem = Objects[WEAPONID + 1]; /* shortsword */
            gain_item(newitem);
            newitem  = new object;
            *newitem = Objects[ARMORID + 1]; /* leather */
            gain_item(newitem);
            Player.cash += 500;
            Player.rank[LEGION]    = LEGIONAIRE;
            Player.guildxp[LEGION] = 1;
            Player.str++;
            Player.con++;
            Player.maxstr++;
            Player.maxcon++;
          }
        }
        break;
      case COMMANDANT:
        queue_message("You find the disposition of your forces satisfactory.");
        break;
      case COLONEL:
        if((Player.level > Commandantlevel) && find_and_remove_item(CORPSEID, DEMON_EMP))
        {
          queue_message("You liberated the Demon Emperor's Regalia!");
          queue_message("The Legion is assembled in salute to you!");
          queue_message("The Regalia is held high for all to see and admire.");
          queue_message("Commandant ");
          queue_message(Commandant);
          queue_message(" promotes you to replace him,");
          queue_message("and announces his own overdue retirement.");
          queue_message("You are the new Commandant of the Legion!");
          queue_message("The Emperor's Regalia is sold for a ridiculous sum.");
          Commandant = Player.name;
          Commandantlevel    = Player.level;
          Commandantbehavior = fixnpc(4);
          save_hiscore_npc(8);
          queue_message("You now know the Spell of Regeneration.");
          spell::Spells[spell::REGENERATE].known = true;
          Player.rank[LEGION]        = COMMANDANT;
          Player.maxstr += 2;
          Player.str += 2;
          Player.maxcon += 2;
          Player.con += 2;
          queue_message("Your training is complete. You get top salary.");
          Player.cash += 20000;
        }
        else if(Player.level <= Commandantlevel)
        {
          queue_message("Your CO expresses satisfaction with your progress.");
          queue_message("But your service record does not yet permit promotion.");
        }
        else
        {
          queue_message("Why do you come empty handed?");
          queue_message("You must return with the Regalia of the Demon Emperor!");
        }
        break;
      case FORCE_LEADER:
        queue_message("Your CO expresses satisfaction with your progress.");
        if(Player.guildxp[LEGION] < 4000)
        {
          queue_message("But your service record does not yet permit promotion.");
        }
        else
        {
          queue_message("You have been promoted to Legion Colonel!");
          queue_message("Your next promotion is contingent on the return of");
          queue_message("the Regalia of the Demon Emperor.");
          queue_message("The Demon Emperor holds court at the base of a volcano");
          queue_message("to the far south, in the heart of a swamp.");
          queue_message("You have been taught the spell of heroism!");
          spell::Spells[spell::HEROISM].known = true;
          Player.rank[LEGION]  = COLONEL;
          Player.maxstr++;
          Player.str++;
          Player.maxcon++;
          Player.con++;
          queue_message("You are given advanced training, and a raise.");
          Player.cash += 10000;
        }
        break;
      case CENTURION:
        queue_message("Your CO expresses satisfaction with your progress.");
        if(Player.guildxp[LEGION] < 1500)
        {
          queue_message("But your service record does not yet permit promotion.");
        }
        else
        {
          queue_message("You are now a Legion Force-Leader!");
          Player.rank[LEGION] = FORCE_LEADER;
          Player.maxstr++;
          Player.str++;
          queue_message("You receive more training, and bonus pay.");
          Player.cash += 5000;
        }
        break;
      case LEGIONAIRE:
        queue_message("Your CO expresses satisfaction with your progress.");
        if(Player.guildxp[LEGION] < 400)
        {
          queue_message("But your service record does not yet permit promotion.");
        }
        else
        {
          queue_message("You are promoted to Legion Centurion!");
          queue_message("You get advanced training, and a higher salary.");
          Player.rank[LEGION] = CENTURION;
          Player.maxcon++;
          Player.con++;
          Player.cash += 2000;
        }
        break;
    }
  }
}

void l_castle()
{
  pob o;
  int x, y;

  if(Player.level < 3)
  {
    queue_message("You can't possibly enter the castle, you nobody!");
    queue_message("Come back when you are famous.");
  }
  else
  {
    queue_message("You are ushered into the castle.");
    if(Player.rank[NOBILITY] < DUKE)
    {
      queue_message("His Grace, ");
      queue_message(Duke);
      queue_message("-- Duke of Rampart! <fanfare>");
    }
    if(Player.rank[NOBILITY] == 0)
    {
      queue_message("Well, sirrah, wouldst embark on a quest? [yn] ");
      if(ynq() == 'y')
      {
        queue_message("Splendid. Bring me the head of the Goblin King.");
        Player.rank[NOBILITY] = COMMONER;
      }
      else
      {
        queue_message("You scoundrel! Guards! Take this blackguard away!");
        p_damage(25, UNSTOPPABLE, "castle guards for lese majeste");
        send_to_jail();
      }
    }
    else if(Player.rank[NOBILITY] == COMMONER)
    {
      if(find_and_remove_item(CORPSEID, GOBLIN_KING))
      {
        queue_message("Good job, sirrah! I promote you to the rank of esquire.");
        Player.rank[NOBILITY] = ESQUIRE;
        gain_experience(100);
        queue_message("Now that you have proved yourself true, another quest!");
        queue_message("Bring to me a Holy Defender!");
        queue_message("One is said to be in the possession of the Great Wyrm");
        queue_message("in the depths of the sewers below the city.");
      }
      else
      {
        queue_message("Do not return until you achieve the quest, caitiff!");
      }
    }
    else if(Player.rank[NOBILITY] == ESQUIRE)
    {
      if(find_and_remove_item(WEAPONID + 34, -1))
      {
        queue_message("My thanks, squire. In return, I dub thee knight!");
        Player.rank[NOBILITY] = KNIGHT;
        gain_experience(1000);
        queue_message("If thou wouldst please me further...");
        queue_message("Bring me a suit of dragonscale armor.");
        queue_message("You might have to kill a dragon to get one....");
      }
      else
      {
        queue_message("Greetings, squire. My sword? What, you don't have it?");
      }
    }
    else if(Player.rank[NOBILITY] == KNIGHT)
    {
      if(find_and_remove_item(ARMORID + 12, -1))
      {
        queue_message("Thanks, good sir knight.");
        queue_message("Here are letters patent to a peerage!");
        Player.rank[NOBILITY] = LORD;
        gain_experience(10000);
        queue_message("If you would do me a final service...");
        queue_message("I require the Orb of Mastery. If you would be so kind...");
        queue_message("By the way, you might find the Orb in the possession");
        queue_message("Of the Elemental Master on the Astral Plane");
      }
      else
      {
        queue_message("Your quest is not yet complete, sir knight.");
      }
    }
    else if(Player.rank[NOBILITY] == LORD)
    {
      if(find_item(&o, ARTIFACTID + 0, -1))
      {
        queue_message("My sincerest thanks, my lord.");
        queue_message("You have proved yourself a true paragon of chivalry");
        queue_message("I abdicate the Duchy in your favor....");
        queue_message("Oh, you can keep the Orb, by the way....");
        Player.rank[NOBILITY] = DUKE;
        gain_experience(10000);
        Duke = Player.name;
        Dukebehavior = fixnpc(4);
        save_hiscore_npc(12);
        for(y = 52; y < 63; y++)
        {
          for(x = 2; x < 52; x++)
          {
            if(Level->site[x][y].p_locf == L_TRAP_SIREN)
            {
              Level->site[x][y].p_locf = L_NO_OP;
              lset(x, y, CHANGED, *Level);
            }
            if(x >= 12 && loc_statusp(x, y, SECRET, *Level))
            {
              lreset(x, y, SECRET, *Level);
              lset(x, y, CHANGED, *Level);
            }
            if(x >= 20 && x <= 23 && y == 56)
            {
              Level->site[x][y].locchar = FLOOR;
              lset(x, y, CHANGED, *Level);
            }
          }
        }
      }
      else
      {
        queue_message("I didn't really think you were up to the task....");
      }
    }
  }
}

void l_arena()
{
  char  response;
  pob   newitem;
  int   i, prize, monsterlevel;

  queue_message("Rampart Coliseum");
  if(Player.rank[ARENA] == 0)
  {
    queue_message("Enter the games, or Register as a Gladiator? [e,r,ESCAPE] ");
    do
    {
      response = (char)mcigetc();
    } while((response != 'e') && (response != 'r') && (response != ESCAPE));
  }
  else
  {
    queue_message("Enter the games? [yn] ");
    response = ynq();
    if(response == 'y')
    {
      response = 'e';
    }
    else
    {
      response = ESCAPE;
    }
  }
  if(response == 'r')
  {
    if(Player.rank[ARENA] > 0)
    {
      queue_message("You're already a gladiator....");
    }
    else if(Player.rank[ORDER] > 0)
    {
      queue_message("We don't let Paladins into our Guild.");
    }
    else if(Player.rank[LEGION] > 0)
    {
      queue_message("We don't train no stinkin' mercs!");
    }
    else if(Player.str < 13)
    {
      queue_message("Yer too weak to train!");
    }
    else if(Player.agi < 12)
    {
      queue_message("Too clumsy to be a gladiator!");
    }
    else
    {
      queue_message("Ok, yer now an Arena Trainee.");
      queue_message("Here's a wooden sword, and a shield");
      newitem  = new object;
      *newitem = Objects[WEAPONID + 17]; /* club */
      gain_item(newitem);
      newitem  = new object;
      *newitem = Objects[SHIELDID + 2]; /* shield */
      gain_item(newitem);
      Player.rank[ARENA] = TRAINEE;
      Arena_Opponent     = 3;
      queue_message("You've got 5000Au credit at the Gym.");
      Gymcredit += 5000;
    }
  }
  else if(response == 'e')
  {
    queue_message("OK, we're arranging a match....");
    Arena_Monster = new monster;
    Arena_Victory = false;
    switch(Arena_Opponent)
    {
      case 0:
        *Arena_Monster = Monsters[GEEK];
        break;
      case 1:
        *Arena_Monster = Monsters[HORNET];
        break;
      case 2:
        *Arena_Monster = Monsters[HYENA];
        break;
      case 3:
        *Arena_Monster = Monsters[GOBLIN];
        break;
      case 4:
        *Arena_Monster = Monsters[GRUNT];
        break;
      case 5:
        *Arena_Monster = Monsters[TOVE];
        break;
      case 6:
        *Arena_Monster = Monsters[APPR_NINJA];
        break;
      case 7:
        *Arena_Monster = Monsters[SALAMANDER];
        break;
      case 8:
        *Arena_Monster = Monsters[ANT];
        break;
      case 9:
        *Arena_Monster = Monsters[MANTICORE];
        break;
      case 10:
        *Arena_Monster = Monsters[SPECTRE];
        break;
      case 11:
        *Arena_Monster = Monsters[BANDERSNATCH];
        break;
      case 12:
        *Arena_Monster = Monsters[LICHE];
        break;
      case 13:
        *Arena_Monster = Monsters[AUTO_MAJOR];
        break;
      case 14:
        *Arena_Monster = Monsters[JABBERWOCK];
        break;
      case 15:
        *Arena_Monster = Monsters[JOTUN];
        break;
      default:
        if((Player.rank[ARENA] < 6) && (Player.rank[ARENA] > 0))
        {
          *Arena_Monster = Monsters[HISCORE_NPC];
        }
        else
        {
          do
          {
            i = random_range(ML9 - ML0) + ML0;
          } while(i == NPC || i == HISCORE_NPC || i == ZERO_NPC || (Monsters[i].uniqueness != COMMON) ||
                  (Monsters[i].dmg == 0));
          *Arena_Monster = Monsters[i];
        }
        break;
    }

    monsterlevel = Arena_Monster->level;
    if(Arena_Monster->id == HISCORE_NPC)
    {
      std::string name = std::format("{}, the arena champion", Champion);
      Arena_Monster->monstring = name;
      Arena_Monster->corpsestr = std::format("The corse of {}", name);
      Arena_Monster->level     = 20;
      Arena_Monster->hp        = Championlevel * Championlevel * 5;
      Arena_Monster->hit       = Championlevel * 4;
      Arena_Monster->ac        = Championlevel * 3;
      Arena_Monster->dmg       = 100 + Championlevel * 2;
      Arena_Monster->xpv       = Championlevel * Championlevel * 5;
      Arena_Monster->speed     = 3;
      for(i = 0; i < Championlevel / 5; i++)
      {
        Arena_Monster->meleestr += "L?R?";
      }
      m_status_set(*Arena_Monster, MOBILE);
      m_status_set(*Arena_Monster, HOSTILE);
    }
    else
    {
      std::string name{std::format("{} the {}", nameprint(), Arena_Monster->monstring)};
      Arena_Monster->monstring = name;
      Arena_Monster->corpsestr = std::format("The corpse of {}", name);
    }
    Arena_Monster->uniqueness = UNIQUE_MADE;
    queue_message("You have a challenger: ");
    queue_message(Arena_Monster->monstring);
    Arena_Monster->attacked = true;
    m_status_set(*Arena_Monster, HOSTILE);
    change_environment(E_ARENA);
    queue_message("Let the battle begin....");

    time_clock(true);
    while(Current_Environment == E_ARENA)
    {
      time_clock(false);
    }

    /* WDT -- Sheldon Simms points out that these objects are not
     * wastes of space; on the contrary, they can be carried out of the
     * arena.  Freeing them was causing subtle and hard to find problems.
     * However, not freeing them is causing a couple of tiny memory leaks.
     * This should be fixed, probably by modifying the object destruction
     * procedures to account for this case.  I'm not really concerned. */
    /* David Given has proposed a nicer solution, but it still causes a
     * memory leak.  Obviously, we need a special field just for names
     * in the monster struct.  Yadda yadda -- I'll mmark this with a
     * HACK!, and comme back to it later. */
    /* can not free the corpse string... it is referenced in the */
    /* corpse string of the corpse object.  */
    /* Unfortunately, this will cause a memory leak, but I don't see */
    /* any way to avoid it.  This fixes the munged arena corpse names */
    /* problem. -DAG */
    /* free(corpse); */

    if(!Arena_Victory)
    {
      queue_message("The crowd boos your craven behavior!!!");
      if(Player.rank[ARENA] > 0)
      {
        queue_message("You are thrown out of the Gladiator's Guild!");
        if(Gymcredit > 0)
        {
          queue_message("Your credit at the gym is cut off!");
        }
        Gymcredit          = 0;
        Player.rank[ARENA] = -1;
      }
    }
    else
    {
      Arena_Opponent++;
      if(monsterlevel == 20)
      {
        queue_message("The crowd roars its approval!");
        if(Player.rank[ARENA])
        {
          queue_message("You are the new Arena Champion!");
          Championlevel = Player.level;
          Champion = Player.name;
          Player.rank[ARENA] = 5;
          Championbehavior   = fixnpc(4);
          save_hiscore_npc(11);
          queue_message("You are awarded the Champion's Spear: Victrix!");
          newitem  = new object;
          *newitem = Objects[WEAPONID + 35];
          gain_item(newitem);
        }
        else
        {
          queue_message("As you are not an official gladiator,");
          queue_message("you are not made Champion.");
        }
      }
      prize = std::max(25, monsterlevel * 50);
      if(Player.rank[ARENA] > 0)
      {
        prize *= 2;
      }
      queue_message(std::format("Good fight! Your prize is: {}Au.", prize));
      Player.cash += prize;
      if((Player.rank[ARENA] < 4) && (Arena_Opponent > 5) && (Arena_Opponent % 3 == 0))
      {
        if(Player.rank[ARENA] > 0)
        {
          Player.rank[ARENA]++;
          queue_message("You've been promoted to a stronger class!");
          queue_message("You are also entitled to additional training.");
          Gymcredit += Arena_Opponent * 1000;
        }
      }
    }
    xredraw();
  }
}
