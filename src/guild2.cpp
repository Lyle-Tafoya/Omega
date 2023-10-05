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

/* guild2.c */
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

extern void item_unequip(object *);

void l_thieves_guild()
{
  int  fee, count, i, number, done = false, dues = 1000;
  char c, action;
  pob  lockpick;
  queue_message("You have penetrated to the Lair of the Thieves' Guild.");
  if(!nighttime())
  {
    queue_message("There aren't any thieves around in the daytime.");
  }
  else
  {
    if((Player.rank[THIEVES] == TMASTER) && (Player.level > Shadowlordlevel) &&
       find_and_remove_item(THINGID + 16, -1))
    {
      queue_message("You nicked the Justiciar's Badge!");
      queue_message("The Badge is put in a place of honor in the Guild Hall.");
      queue_message("You are now the Shadowlord of the Thieves' Guild!");
      queue_message("Who says there's no honor among thieves?");
      Shadowlord = Player.name;
      Shadowlordlevel    = Player.level;
      Shadowlordbehavior = fixnpc(4);
      save_hiscore_npc(7);
      queue_message("You learn the Spell of Shadowform.");
      spell::Spells[spell::SHADOWFORM].known = true;
      Player.rank[THIEVES]       = SHADOWLORD;
      Player.maxagi += 2;
      Player.maxdex += 2;
      Player.agi += 2;
      Player.dex += 2;
    }
    while(!done)
    {
      menuclear();
      if(Player.rank[THIEVES] == 0)
      {
        menuprint("a: Join the Thieves' Guild.\n");
      }
      else
      {
        menuprint("b: Raise your Guild rank.\n");
      }
      menuprint("c: Get an item identified.\n");
      if(Player.rank[THIEVES] > 0)
      {
        menuprint("d: Fence an item.\n");
      }
      menuprint("ESCAPE: Leave this Den of Iniquity.");
      showmenu();
      action = mgetc();
      if(action == ESCAPE)
      {
        done = true;
      }
      else if(action == 'a')
      {
        done = true;
        if(Player.rank[THIEVES] > 0)
        {
          queue_message("You are already a member!");
        }
        else if(Player.alignment > 10)
        {
          queue_message("You are too lawful to be a thief!");
        }
        else
        {
          dues += dues * (12 - Player.dex) / 9;
          dues += Player.alignment * 5;
          dues = std::max(100, dues);
          queue_message(std::format("Dues are {}Au. Pay it? [yn] ", dues));
          if(ynq() == 'y')
          {
            if(Player.cash < dues)
            {
              queue_message("You can't cheat the Thieves' Guild!");
              queue_message("... but the Thieves' Guild can cheat you....");
              Player.cash = 0;
            }
            else
            {
              queue_message(std::format("Shadowlord {} enters your name into the roll of the guild.", Shadowlord));
              queue_message("As a special bonus, you get a free lockpick.");
              queue_message("You are taught the spell of Object Detection.");
              spell::Spells[spell::OBJECT_DETECTION].known = true;
              lockpick                = new object;
              *lockpick               = Objects[THINGID + 2]; /* lock pick */
              gain_item(lockpick);
              Player.cash -= dues;
              dataprint();
              Player.guildxp[THIEVES] = 1;
              Player.rank[THIEVES]    = TMEMBER;
              Player.maxdex++;
              Player.dex++;
              Player.agi++;
              Player.maxagi++;
            }
          }
        }
      }
      else if(action == 'b')
      {
        if(Player.rank[THIEVES] == 0)
        {
          queue_message("You are not even a member!");
        }
        else if(Player.rank[THIEVES] == SHADOWLORD)
        {
          queue_message("You can't get any higher than this!");
        }
        else if(Player.rank[THIEVES] == TMASTER)
        {
          if(Player.level <= Shadowlordlevel)
          {
            queue_message("You are not experienced enough to advance.");
          }
          else
          {
            queue_message("You must bring back the Justiciar's Badge!");
          }
        }
        else if(Player.rank[THIEVES] == THIEF)
        {
          if(Player.guildxp[THIEVES] < 4000)
          {
            queue_message("You are not experienced enough to advance.");
          }
          else
          {
            queue_message("You are now a Master Thief of the Guild!");
            queue_message("You are taught the Spell of Apportation.");
            queue_message("To advance to the next level you must return with");
            queue_message("the badge of the Justiciar (cursed be his name).");
            queue_message("The Justiciar's office is just south of the gaol.");
            spell::Spells[spell::APPORTATION].known = true;
            Player.rank[THIEVES]   = TMASTER;
            Player.maxagi++;
            Player.maxdex++;
            Player.agi++;
            Player.dex++;
          }
        }
        else if(Player.rank[THIEVES] == ATHIEF)
        {
          if(Player.guildxp[THIEVES] < 1500)
          {
            queue_message("You are not experienced enough to advance.");
          }
          else
          {
            queue_message("You are now a ranking Thief of the Guild!");
            queue_message("You learn the Spell of Invisibility.");
            spell::Spells[spell::INVISIBILITY].known = true;
            Player.rank[THIEVES]      = THIEF;
            Player.agi++;
            Player.maxagi++;
          }
        }
        else if(Player.rank[THIEVES] == TMEMBER)
        {
          if(Player.guildxp[THIEVES] < 400)
          {
            queue_message("You are not experienced enough to advance.");
          }
          else
          {
            queue_message("You are now an Apprentice Thief!");
            queue_message("You are taught the Spell of Levitation.");
            spell::Spells[spell::LEVITATE].known = true;
            Player.rank[THIEVES]     = ATHIEF;
            Player.dex++;
            Player.maxdex++;
          }
        }
      }
      else if(action == 'c')
      {
        if(Player.rank[THIEVES] == 0)
        {
          queue_message("RTG, Inc, Appraisers. Identification Fee: 50Au/item.");
          fee = 50;
        }
        else
        {
          fee = 5;
          queue_message("The fee is 5Au per item.");
        }
        queue_message("Identify one item, or all possessions? [ip] ");
        if((char)mcigetc() == 'i')
        {
          if(Player.cash < fee)
          {
            queue_message("Try again when you have the cash.");
          }
          else
          {
            Player.cash -= fee;
            dataprint();
            identify(0);
          }
        }
        else
        {
          count = 0;
          for(i = 1; i < MAXITEMS; i++)
          {
            if(Player.possessions[i])
            {
              if(Player.possessions[i]->known < 2)
              {
                count++;
              }
            }
          }
          for(i = 0; i < Player.packptr; i++)
          {
            if(Player.pack[i])
            {
              if(Player.pack[i]->known < 2)
              {
                count++;
              }
            }
          }
          queue_message(std::format("The fee will be: {}Au. Pay it? [yn] ", std::max(count * fee, fee)));
          if(ynq() == 'y')
          {
            if(Player.cash < std::max(count * fee, fee))
            {
              queue_message("Try again when you have the cash.");
            }
            else
            {
              Player.cash -= std::max(count * fee, fee);
              dataprint();
              identify(1);
            }
          }
        }
      }
      else if(action == 'd')
      {
        if(Player.rank[THIEVES] == 0)
        {
          queue_message("Fence? Who said anything about a fence?");
        }
        else
        {
          queue_message("Fence one item or go through pack? [ip] ");
          if((char)mcigetc() == 'i')
          {
            i = getitem(NULL_ITEM);
            if(i == ABORT || !Player.possessions[i])
            {
              queue_message("Huh, Is this some kind of set-up?");
            }
            else if(Player.possessions[i]->blessing < 0)
            {
              queue_message("I don't want to buy a cursed item!");
            }
            else
            {
              long price = 2 * item_value(Player.possessions[i]) / 3;
              queue_message(std::format("I'll give you {} Au each. OK? [yn] ", price));
              if(ynq() == 'y')
              {
                number = getnumber(Player.possessions[i]->number);
                if((number >= Player.possessions[i]->number) && Player.possessions[i]->used)
                {
                  item_unequip(Player.possessions[i]);
                }
                Player.cash += number * price;
                /* Fenced artifacts could turn up anywhere, really... */
                if(Objects[Player.possessions[i]->id].uniqueness > UNIQUE_UNMADE)
                {
                  Objects[Player.possessions[i]->id].uniqueness = UNIQUE_UNMADE;
                }
                dispose_lost_objects(number, Player.possessions[i]);
                dataprint();
              }
              else
              {
                queue_message("Hey, gimme a break, it was a fair price!");
              }
            }
          }
          else
          {
            for(i = 0; i < Player.packptr; i++)
            {
              if(Player.pack[i]->blessing > -1)
              {
                long price = 2 * item_value(Player.pack[i]) / 3;
                queue_message(std::format("Sell {} for {} Au each? [ynq] ", itemid(Player.pack[i]), price));
                if((c = ynq()) == 'y')
                {
                  number = getnumber(Player.pack[i]->number);
                  Player.cash += number * price;
                  Player.pack[i]->number -= number;
                  if(Player.pack[i]->number < 1)
                  {
                    /* Fenced an artifact?  You just might see it again. */
                    if(Objects[Player.pack[i]->id].uniqueness > UNIQUE_UNMADE)
                    {
                      Objects[Player.pack[i]->id].uniqueness = UNIQUE_UNMADE;
                    }
                    delete Player.pack[i];
                    Player.pack[i] = nullptr;
                  }
                  calc_melee();
                  dataprint();
                }
                else if(c == 'q')
                {
                  break;
                }
              }
            }
            fixpack();
          }
        }
      }
    }
  }
  xredraw();
}

void l_college()
{
  char action;
  int  done = false, enrolled = false;
  queue_message("The Collegium Magii. Founded 16937, AOF.");
  if(nighttime())
  {
    queue_message("The Registration desk is closed at night....");
  }
  else
  {
    while(!done)
    {
      if((Player.rank[COLLEGE] == MAGE) && (Player.level > Archmagelevel) &&
         find_and_remove_item(CORPSEID, EATER))
      {
        queue_message("You brought back the heart of the Eater of Magic!");
        queue_message("The Heart is sent to the labs for analysis.");
        queue_message("The Board of Trustees appoints you Archmage!");
        Archmage = Player.name;
        Archmagelevel        = Player.level;
        Player.rank[COLLEGE] = ARCHMAGE;
        Player.maxiq += 5;
        Player.iq += 5;
        Player.maxpow += 5;
        Player.pow += 5;
        Archmagebehavior = fixnpc(4);
        save_hiscore_npc(9);
      }
      menuclear();
      menuprint("May we help you?\n\n");
      menuprint("a: Enroll in the College.\n");
      menuprint("b: Raise your College rank.\n");
      menuprint("c: Do spell research.\n");
      menuprint("ESCAPE: Leave these hallowed halls.\n");
      showmenu();
      action = mgetc();
      if(action == ESCAPE)
      {
        done = true;
      }
      else if(action == 'a')
      {
        if(Player.rank[COLLEGE] > 0)
        {
          queue_message("You are already enrolled!");
        }
        else if(Player.iq < 13)
        {
          queue_message("Your low IQ renders you incapable of being educated.");
        }
        else if(Player.rank[CIRCLE] > 0)
        {
          queue_message("Sorcery and our Magic are rather incompatable, no?");
        }
        else if(Player.rank[MONKS] > 0)
        {
          queue_message("Meditation will not leave you enough time for studies.");
        }
        else
        {
          if(Player.iq > 17)
          {
            queue_message("You are given a scholarship!");
            enrolled = true;
          }
          else
          {
            queue_message("Tuition is 1000Au. ");
            queue_message("Pay it? [yn] ");
            if(ynq() == 'y')
            {
              if(Player.cash < 1000)
              {
                queue_message("You don't have the funds!");
              }
              else
              {
                Player.cash -= 1000;
                enrolled = true;
                dataprint();
              }
            }
          }
          if(enrolled)
          {
            queue_message("Archmage ");
            queue_message(Archmage);
            queue_message(" greets you and congratulates you on your acceptance.");
            queue_message("You are now enrolled in the Collegium Magii!");
            queue_message("You are now a Novice.");
            queue_message("You may research 1 spell, for your intro class.");
            Spellsleft              = 1;
            Player.rank[COLLEGE]    = INITIATE;
            Player.guildxp[COLLEGE] = 1;
            Player.maxiq += 1;
            Player.iq += 1;
            Player.maxpow += 1;
            Player.pow += 1;
          }
        }
      }
      else if(action == 'b')
      {
        if(Player.rank[COLLEGE] == 0)
        {
          queue_message("You have not even been initiated, yet!");
        }
        else if(Player.rank[COLLEGE] == ARCHMAGE)
        {
          queue_message("You are at the pinnacle of mastery in the Collegium.");
        }
        else if(Player.rank[COLLEGE] == MAGE)
        {
          if(Player.level <= Archmagelevel)
          {
            queue_message("You are not experienced enough to advance.");
          }
          else
          {
            queue_message("You must return with the heart of the Eater of Magic!");
          }
        }
        else if(Player.rank[COLLEGE] == PRECEPTOR)
        {
          if(Player.guildxp[COLLEGE] < 4000)
          {
            queue_message("You are not experienced enough to advance.");
          }
          else
          {
            queue_message("You are now a Mage of the Collegium Magii!");
            queue_message("You may research 6 spells for postdoctoral research.");
            Spellsleft += 6;
            queue_message("To become Archmage, you must return with the");
            queue_message("heart of the Eater of Magic");
            queue_message("The Eater may be found on a desert isle somewhere.");
            Player.rank[COLLEGE] = MAGE;
            Player.maxiq += 2;
            Player.iq += 2;
            Player.maxpow += 2;
            Player.pow += 2;
          }
        }
        else if(Player.rank[COLLEGE] == STUDENT)
        {
          if(Player.guildxp[COLLEGE] < 1500)
          {
            queue_message("You are not experienced enough to advance.");
          }
          else
          {
            queue_message("You are now a Preceptor of the Collegium Magii!");
            queue_message("You are taught the basics of ritual magic.");
            queue_message("Your position allows you to research 4 spells.");
            Spellsleft += 4;
            spell::Spells[spell::RITUAL_MAGIC].known = true;
            Player.rank[COLLEGE]   = PRECEPTOR;
            Player.maxiq += 1;
            Player.iq += 1;
            Player.maxpow += 1;
            Player.pow += 1;
          }
        }
        else if(Player.rank[COLLEGE] == NOVICE)
        {
          if(Player.guildxp[COLLEGE] < 400)
          {
            queue_message("You are not experienced enough to advance.");
          }
          else
          {
            queue_message("You are now a Student at the Collegium Magii!");
            queue_message("You are taught the spell of identification.");
            queue_message("Thesis research credit is 2 spells.");
            Spellsleft += 2;
            spell::Spells[spell::IDENTIFICATION].known = true;
            Player.rank[COLLEGE]     = STUDENT;
            Player.maxiq += 1;
            Player.iq += 1;
            Player.maxpow += 1;
            Player.pow += 1;
          }
        }
      }
      else if(action == 'c')
      {
        if(Spellsleft > 0)
        {
          queue_message(std::format("Research permitted: {} Spells.", Spellsleft));
        }
        if(Spellsleft < 1)
        {
          queue_message("Extracurricular Lab fee: 2000 Au. ");
          queue_message("Pay it? [yn] ");
          if(ynq() == 'y')
          {
            if(Player.cash < 2000)
            {
              queue_message("Try again when you have the cash.");
            }
            else
            {
              Player.cash -= 2000;
              dataprint();
              Spellsleft = 1;
            }
          }
        }
        if(Spellsleft > 0)
        {
          learnspell(0);
          Spellsleft--;
        }
      }
    }
  }
  xredraw();
}

void l_monastery()
{
  bool enrolled = false;
  queue_message("Tholian Monastery of Rampart. Founded 12031, AOF.");
  queue_message("Welcome to our humble hovel.");
  if(nighttime())
  {
    queue_message("The monastery doors are sealed until dawn.");
  }
  else
  {
    bool done=false;
    while(!done)
    {
      menuclear();
      menuprint("Find your True Course:\n\n");
      menuprint("a: Discover the Way.\n");
      menuprint("b: Meditate on the Path.\n");
      menuprint("c: Meditate on Knowledge.\n");
      menuprint("d: Take an extended Meditation.\n");
      menuprint("ESCAPE: Re-enter the World.\n");
      showmenu();
      char action = mgetc();
      if(action == ESCAPE)
      {
        done = true;
        calc_melee();
      }
      else if(action == 'a')
      {
        if(Player.rank[MONKS] > 0)
        {
          queue_message("You are already initiated, Brother.");
        }
        else if(Player.con < 13)
        {
          queue_message("Your body is too fragile to walk the Path, child.");
        }
        else if(Player.pow < 13)
        {
          queue_message("Your mind is too fragile to follow the Path, child.");
        }
        else if(Player.rank[COLLEGE] > 0)
        {
          queue_message("The Collegium has corruputed your spirit, child.");
        }
        else if(Player.rank[CIRCLE] > 0)
        {
          queue_message("The Circle has corruputed your spirit, child.");
        }
        else
        {
          if(Player.pow > 17)
          {
            queue_message("Your spirit is strong. You may study the Path with us.");
            enrolled=true;
          }
          else
          {
            queue_message("A substantial cash sacrifice will cleanse your spirit.. ");
            queue_message("Donate your worldly goods? [yn] ");
            if(ynq() =='y')
            {
              if (Player.cash < 1000)
              {
                // WDT HACK! I'd rather the monks have some other criteria for joining
                queue_message("You have not much to give.");
              }
              else
              {
                Player.cash = 0;
                enrolled = true;
                dataprint();
              }
            }
          }
          if(enrolled)
          {
            queue_message(std::format("Grandmaster {} welcomes you to the Brotherhood.", Grandmaster));
            queue_message("You are now a Tholian Monk trainee!");
            queue_message("You may study the Way with us.");
            Studiesleft = 1;
            Player.rank[MONKS] = MONK_TRAINEE;
            Player.guildxp[MONKS] = 1;
            Player.maxpow += 1;
            Player.pow += 1;
          }
        }
      }
      else if(action == 'b')
      {
        if(Player.rank[MONKS] == 0)
        {
          queue_message("You are not yet initiated, child.");
        }
        else if(Player.rank[MONKS]==MONK_GRANDMASTER)
        {
          queue_message("Your advancement lies within, Grandmaster.");
        }
        else if(Player.rank[MONKS]==MONK_MASTER_TEARS)
        {
          if(Player.level <= Grandmasterlevel)
          {
            queue_message("Seek more experience, Master.");
          }
          else if(Player.rank[MONKS]==MONK_MASTER_TEARS && Player.level > Grandmasterlevel && find_and_remove_item(CORPSEID,EATER))
          {
            queue_message("You brought back the heart of the Eater of Magic!");
            queue_message("The Heart is sent to the placed in the kitchen cauldron.");
            queue_message("The Grandmaster steps down. You are the new Grandmaster.");
            Grandmaster = Player.name;
            Grandmasterlevel = Player.level;
            Player.rank[MONKS] = MONK_GRANDMASTER;
            Player.maxhp += (Player.maxpow * 3);
            Player.maxiq += 5;
            Player.iq+= 5;
            Player.maxpow += 3;
            Player.pow += 3;
            Player.maxstr += 3;
            Player.str += 3;
            Grandmasterbehavior = fixnpc(4);
            save_hiscore_npc(16);
          }
          else
          {
            queue_message("You must return with the heart of the Eater of Magic!");
          }

        }
        else if(Player.rank[MONKS]==MONK_MASTER_PAINS)
        {
          if(Player.guildxp[MONKS] < 40000)
          {
            queue_message("Seek more experience, Master.");
          }
          else
          {
            queue_message("You have travelled far, Master.");
            queue_message("You are now Master of Tears.");
            Studiesleft += 6;
            queue_message("To become Grandmaster, you must return with the");
            queue_message("heart of the Eater of Magic");
            queue_message("The Eater may be found on a desert isle somewhere.");
            spell::Spells[spell::REGENERATE].known = true;
            Player.rank[MONKS] = MONK_MASTER_TEARS;
            Player.maxhp += (Player.maxpow * 2);
            Player.maxpow += 2;
            Player.pow += 2;
            Player.maxstr += 2;
            Player.str += 2;
            Player.maxagi += 2;
            Player.agi += 2;
          }
        }
        else if(Player.rank[MONKS]==MONK_MASTER_SIGHS)
        {
          if(Player.guildxp[MONKS] < 15000)
          {
            queue_message("Seek more experience, Master.");
          }
          else
          {
            queue_message("The Path is long, Master.");
            queue_message("You are now Master of Pain.");
            queue_message("You feel enlightened.");
            Studiesleft +=4;
            spell::Spells[spell::RITUAL_MAGIC].known = true;
            spell::Spells[spell::RESTORATION].known = true;
            Player.status[ILLUMINATION] = 1500;  // enlightened
            Player.rank[MONKS] = MONK_MASTER_PAINS;
            Player.maxhp += Player.maxpow;
            Player.maxcon += 1;
            Player.con += 1;
            Player.maxdex += 2;
            Player.dex += 2;
          }
        }
        else if (Player.rank[MONKS]==MONK_MASTER)
        {
          if(Player.guildxp[MONKS] < 9000)
          {
            queue_message("Seek more experience, Master.");
          }
          else {
            queue_message("Drink, weary Master.");
            queue_message("You are now Master of Sighs.");
            Studiesleft +=2;
            spell::Spells[spell::HASTE].known = true;
            Player.rank[MONKS] = MONK_MASTER_SIGHS;
            Player.maxhp += Player.maxpow;
            Player.maxcon += 1;
            Player.con += 1;
            Player.maxiq += 2;
            Player.iq += 2;
          }
        }
        else if(Player.rank[MONKS]==MONK_MONK)
        {
          if(Player.guildxp[MONKS] < 3000)
          {
            queue_message("Seek more experience, Brother.");
          }
          else
          {
            queue_message("A thousand steps on the path, Brother.");
            queue_message("You are now a Master.");
            Studiesleft +=2;
            spell::Spells[spell::HEALING].known = true;
            Player.rank[MONKS] = MONK_MASTER;
            Player.maxhp += Player.maxpow;
            Player.maxcon += 1;
            Player.con += 1;
            Player.maxpow += 2;
            Player.pow += 5;
          }
        }
        else if(Player.rank[MONKS]==MONK_TRAINEE)
        {
          if(Player.guildxp[MONKS] < 1500)
          {
            queue_message("Seek more experience, Brother.");
          }
          else
          {
            queue_message("You have sought wisdom, Brother.");
            queue_message("You are now a Tholian Monk.");
            Studiesleft +=2;
            spell::Spells[spell::CURING].known = true;
            Player.rank[MONKS] = MONK_MONK;
            Player.maxhp += Player.maxpow;
            Player.maxcon += 1;
            Player.con += 1;
            Player.maxpow += 1;
            Player.pow += 1;
          }
        }
      }
      else if(action == 'c')
      {
        if(Studiesleft > 0)
        {
          queue_message(std::format("Studies permitted: {} Studies.", Studiesleft));
        }
        if(Studiesleft < 1)
        {
          queue_message("Sacrifice clears a cluttered heart. ");
          queue_message("Donate your worldly cash? [yn] ");
          if(ynq()=='y')
          {
            if(Player.cash < 2000)
            {
              if(Player.rank[MONKS] >= MONK_GRANDMASTER)
                queue_message("You have not much to give, Grandmaster.");
              else if(Player.rank[MONKS] >= MONK_MASTER)
              {
                queue_message("You have not much to give, Master.");
              }
              else
              {
                queue_message("You have not much to give, Brother.");
              }
            }
            else
            {
              Player.cash = 0;
              dataprint();
              Studiesleft = 1;
            }
          }
        }
        if(Studiesleft > 0)
        {
          learnspell(0);
          --Studiesleft;
        }
      }
      else if(action == 'd')
      {
        if (Player.rank[MONKS] < MONK_MASTER)
        {
          queue_message("Only Masters can achieve extended meditation, child.");
        }
        else
        {
          queue_message("Seeking inner truth...");
          toggle_item_use(true);
          Player.cash = 0;
          Player.hp = Player.maxhp;
          Player.str = Player.maxstr;
          Player.agi = Player.maxagi;
          Player.con = Player.maxcon;
          Player.dex = Player.maxdex;
          Player.iq = Player.maxiq;
          Player.pow = Player.maxpow;
          for(int i=0; i < NUMSTATI; ++i)
          {
            if(Player.status[i] < 1000)
            {
              Player.status[i]=0;
            }
          }
          toggle_item_use(false);
          Player.food = 43;
          queue_message("Your body and mind are whole.");
          if(random_range(2))
          {
            queue_message("Extra whole!");
            Player.maxhp += 1;
          }
        }
        Time += 60*24*7;
        Date += 7;
        moon_check();
        timeprint();
      }
    }
  }
  xredraw();
}

void l_sorcerors()
{
  char action;
  int  done = false, fee = 3000;
  long total;
  queue_message("The Circle of Sorcerors.");
  if(Player.rank[CIRCLE] == -1)
  {
    queue_message("Fool! Didn't we tell you to go away?");
    Player.mana = 0;
    dataprint();
  }
  else
  {
    while(!done)
    {
      if((Player.rank[CIRCLE] == HIGHSORCEROR) && (Player.level > Primelevel) &&
         find_and_remove_item(CORPSEID, LAWBRINGER))
      {
        queue_message("You obtained the Crown of the Lawgiver!");
        queue_message("The Crown is ritually sacrificed to the Lords of Chaos.");
        queue_message("You are now the Prime Sorceror of the Inner Circle!");
        Prime = Player.name;
        Primelevel    = Player.level;
        Primebehavior = fixnpc(4);
        save_hiscore_npc(10);
        queue_message("You learn the Spell of Disintegration!");
        spell::Spells[spell::DISINTEGRATE].known = true;
        Player.rank[CIRCLE]          = PRIME;
        Player.maxpow += 10;
        Player.pow += 10;
      }
      menuclear();
      menuprint("May we help you?\n\n");
      menuprint("a: Become an Initiate of the Circle.\n");
      menuprint("b: Raise your rank in the Circle.\n");
      menuprint("c: Restore mana points\n");
      menuprint("ESCAPE: Leave these Chambers of Power.\n");
      showmenu();
      action = mgetc();
      if(action == ESCAPE)
      {
        done = true;
      }
      else if(action == 'a')
      {
        if(Player.rank[CIRCLE] > 0)
        {
          queue_message("You are already an initiate!");
        }
        else if(Player.alignment > 0)
        {
          queue_message("You may not join -- you reek of Law!");
        }
        else if(Player.rank[COLLEGE] != 0)
        {
          queue_message("Foolish Mage!  You don't have the right attitude to Power!");
        }
        else if(Player.rank[MONKS] > 0)
        {
          queue_message("Stupid monk. Go meditate on this!");
        }
        else
        {
          fee += Player.alignment * 100;
          fee += fee * (12 - Player.pow) / 9;
          fee = std::max(100, fee);
          queue_message(std::format("For you, there is an initiation fee of {} Au.", fee));
          queue_message("Pay it? [yn] ");
          if(ynq() == 'y')
          {
            if(Player.cash < fee)
            {
              queue_message("Try again when you have the cash!");
            }
            else
            {
              queue_message(std::format("Prime Sorceror {} conducts your initiation into the circle of novices.", Prime));
              queue_message("You learn the Spell of Magic Missiles.");
              spell::Spells[spell::MAGIC_MISSILE].known = true;
              Player.cash -= fee;
              dataprint();
              Player.rank[CIRCLE]    = INITIATE;
              Player.guildxp[CIRCLE] = 1;
              Player.maxpow++;
              Player.pow++;
            }
          }
        }
      }
      else if(action == 'b')
      {
        if(Player.rank[CIRCLE] == 0)
        {
          queue_message("You have not even been initiated, yet!");
        }
        else if(Player.alignment > -1)
        {
          queue_message("Ahh! You have grown too lawful!!!");
          queue_message("You are hereby blackballed from the Circle!");
          Player.rank[CIRCLE] = -1;
          queue_message("A pox upon thee!");
          if(!Player.immunity[INFECTION])
          {
            Player.status[DISEASED] += 100;
          }
          queue_message("And a curse on your possessions!");
          acquire(-1);
          enchant(-1);
          bless(-1);
          queue_message("Die, false sorceror!");
          p_damage(25, UNSTOPPABLE, "a sorceror's curse");
          done = true;
        }
        else if(Player.rank[CIRCLE] == PRIME)
        {
          queue_message("You are at the pinnacle of mastery in the Circle.");
        }
        else if(Player.rank[CIRCLE] == HIGHSORCEROR)
        {
          if(Player.level <= Primelevel)
          {
            queue_message("You are not experienced enough to advance.");
          }
          else
          {
            queue_message("You must return with the Crown of the LawBringer!");
          }
        }
        else if(Player.rank[CIRCLE] == SORCEROR)
        {
          if(Player.guildxp[CIRCLE] < 4000)
          {
            queue_message("You are not experienced enough to advance.");
          }
          else
          {
            queue_message("You are now a High Sorceror of the Inner Circle!");
            queue_message("You learn the Spell of Disruption!");
            queue_message("To advance you must return with the LawBringer's Crown!");
            queue_message("The LawBringer resides on Star Peak.");
            spell::Spells[spell::DISRUPT].known = true;
            Player.rank[CIRCLE]     = HIGHSORCEROR;
            Player.maxpow += 5;
            Player.pow += 5;
          }
        }
        else if(Player.rank[CIRCLE] == ENCHANTER)
        {
          if(Player.guildxp[CIRCLE] < 1500)
          {
            queue_message("You are not experienced enough to advance.");
          }
          else
          {
            queue_message("You are now a member of the Circle of Sorcerors!");
            queue_message("You learn the Spell of Ball Lightning!");
            spell::Spells[spell::BALL_LIGHTNING].known = true;
            Player.rank[CIRCLE]   = SORCEROR;
            Player.maxpow += 2;
            Player.pow += 2;
          }
        }
        else if(Player.rank[CIRCLE] == INITIATE)
        {
          if(Player.guildxp[CIRCLE] < 400)
          {
            queue_message("You are not experienced enough to advance.");
          }
          else
          {
            queue_message("You are now a member of the Circle of Enchanters!");
            queue_message("You learn the Spell of Firebolts.");
            spell::Spells[spell::FIREBOLT].known = true;
            Player.rank[CIRCLE]      = ENCHANTER;
            Player.maxpow += 2;
            Player.pow += 2;
          }
        }
      }
      else if(action == 'c')
      {
        done = true;
        fee  = Player.level * 100;
        if(Player.rank[CIRCLE])
        {
          fee = fee / 2;
        }
        queue_message(std::format("That will be: {} Au. Pay it? [yn] ", fee));
        if(ynq() == 'y')
        {
          if(Player.cash < fee)
          {
            queue_message("Begone, deadbeat, or face the wrath of the Circle!");
          }
          else
          {
            Player.cash -= fee;
            total = calcmana();
            while(Player.mana < total)
            {
              Player.mana++;
              dataprint();
            }
            queue_message("Have a sorcerous day, now!");
          }
        }
        else
        {
          queue_message("Be seeing you!");
        }
      }
    }
  }
  xredraw();
}

void l_order()
{
  pob newitem;
  pml ml;
  queue_message("The Headquarters of the Order of Paladins.");
  if((Player.rank[ORDER] == PALADIN) && (Player.level > Justiciarlevel) &&
     gamestatusp(GAVE_STARGEM, GameStatus) && Player.alignment > 300)
  {
    queue_message("You have succeeded in your quest!");
    queue_message("The previous Justiciar steps down in your favor.");
    queue_message("You are now the Justiciar of Rampart and the Order!");
    Justiciar = Player.name;
    for(ml = Level->mlist; ml && (ml->m->id != HISCORE_NPC || ml->m->aux2 != 15); ml = ml->next)
    {
      /* just scan for current Justicar */;
    }
    if(ml)
    {
      Level->site[ml->m->x][ml->m->y].creature = nullptr;
      erase_monster(ml->m);
      ml->m->hp = -1; /* signals "death" -- no credit to player, though */
    }
    Justiciarlevel    = Player.level;
    Justiciarbehavior = fixnpc(4);
    save_hiscore_npc(15);
    queue_message("You are awarded a blessed shield of deflection!");
    newitem           = new object;
    *newitem          = Objects[SHIELDID + 7]; /* shield of deflection */
    newitem->blessing = 9;
    gain_item(newitem);
    Player.rank[ORDER] = JUSTICIAR;
    Player.maxstr += 5;
    Player.str += 5;
    Player.maxpow += 5;
    Player.pow += 5;
  }
  if(Player.alignment < 1)
  {
    if(Player.rank[ORDER] > 0)
    {
      queue_message("You have been tainted by chaos!");
      queue_message("You are stripped of your rank in the Order!");
      Player.rank[ORDER] = -1;
      send_to_jail();
    }
    else
    {
      queue_message("Get thee hence, minion of chaos!");
    }
  }
  else if(Player.rank[ORDER] == -1)
  {
    queue_message("Thee again?  Get thee hence, minion of chaos!");
  }
  else if(Player.rank[ORDER] == 0)
  {
    if(Player.rank[ARENA] != 0)
    {
      queue_message("We do not accept bloodstained gladiators into our Order.");
    }
    else if(Player.rank[LEGION] != 0)
    {
      queue_message("Go back to your barracks, mercenary!");
    }
    else
    {
      queue_message("Dost thou wish to join our Order? [yn] ");
      if(ynq() == 'y')
      {
        queue_message("Justiciar ");
        queue_message(Justiciar);
        queue_message(" welcomes you to the Order.");
        queue_message("'Mayest thou always follow the sublime path of Law.'");
        queue_message("You are now a Gallant in the Order.");
        queue_message("You are given a horse and a blessed spear.");
        Player.rank[ORDER]    = GALLANT;
        Player.guildxp[ORDER] = 1;
        setgamestatus(MOUNTED, GameStatus);
        newitem           = new object;
        *newitem          = Objects[WEAPONID + 19]; /* spear */
        newitem->blessing = 9;
        newitem->plus     = 1;
        newitem->known    = 2;
        gain_item(newitem);
      }
    }
  }
  else
  {
    queue_message("'Welcome back, Paladin.'");
    if(!gamestatusp(MOUNTED, GameStatus))
    {
      queue_message("You are given a new steed.");
      setgamestatus(MOUNTED, GameStatus);
    }
    if((Player.hp < Player.maxhp) || (Player.status[DISEASED]) || (Player.status[POISONED]))
    {
      queue_message("Your wounds are treated by a medic.");
    }
    cleanse(0);
    Player.hp   = Player.maxhp;
    Player.food = 40;
    queue_message("You get a hot meal from the refectory.");
    if(Player.rank[ORDER] == PALADIN)
    {
      if(Player.level <= Justiciarlevel)
      {
        queue_message("You are not experienced enough to advance.");
      }
      else if(Player.alignment < 300)
      {
        queue_message("You are not sufficiently Lawful as yet to advance.");
      }
      else
      {
        queue_message("You must give the Star Gem to the LawBringer.");
      }
    }
    else if(Player.rank[ORDER] == CHEVALIER)
    {
      if(Player.guildxp[ORDER] < 4000)
      {
        queue_message("You are not experienced enough to advance.");
      }
      else if(Player.alignment < 200)
      {
        queue_message("You are not sufficiently Lawful as yet to advance.");
      }
      else
      {
        queue_message("You are made a Paladin of the Order!");
        queue_message("You learn the Spell of Heroism and get Mithril Plate!");
        newitem           = new object;
        *newitem          = Objects[ARMORID + 11]; /* mithril plate armor */
        newitem->blessing = 9;
        newitem->known    = 2;
        gain_item(newitem);
        queue_message("To advance you must rescue the Star Gem and return it");
        queue_message("to its owner, the LawBringer, who resides on Star Peak.");
        queue_message("The Star Gem was stolen by the cursed Prime Sorceror,");
        queue_message("whose headquarters may be found beyond the Astral Plane.");
        queue_message("The Oracle will send you to the Astral Plane if you");
        queue_message("prove yourself worthy to her.");
        spell::Spells[spell::HEROISM].known = true;
        Player.rank[ORDER]   = PALADIN;
      }
    }
    else if(Player.rank[ORDER] == GUARDIAN)
    {
      if(Player.guildxp[ORDER] < 1500)
      {
        queue_message("You are not experienced enough to advance.");
      }
      else if(Player.alignment < 125)
      {
        queue_message("You are not yet sufficiently Lawful to advance.");
      }
      else
      {
        Player.rank[ORDER] = CHEVALIER;
        queue_message("You are made a Chevalier of the Order!");
        queue_message("You are given a Mace of Disruption!");
        newitem        = new object;
        *newitem       = Objects[WEAPONID + 25]; /* mace of disruption */
        newitem->known = 2;
        gain_item(newitem);
      }
    }
    else if(Player.rank[ORDER] == GALLANT)
    {
      if(Player.guildxp[ORDER] < 400)
      {
        queue_message("You are not experienced enough to advance.");
      }
      else if(Player.alignment < 50)
      {
        queue_message("You are not Lawful enough to advance.");
      }
      else
      {
        queue_message("You are made a Guardian of the Order of Paladins!");
        queue_message("You are given a Holy Hand Grenade (of Antioch).");
        queue_message("You hear a nasal monotone in the distance....");
        queue_message("'...and the number of thy counting shall be 3...'");
        Player.rank[ORDER] = GUARDIAN;
        newitem            = new object;
        *newitem           = Objects[ARTIFACTID + 7]; /* holy hand grenade. */
        newitem->known     = 2;
        gain_item(newitem);
      }
    }
  }
}
