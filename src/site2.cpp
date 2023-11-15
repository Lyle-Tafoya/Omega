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

/* site2.c */
/* 2nd half of site functions and aux functions to them */

#include "glob.h"
#include "interactive_menu.hpp"
#include "scr.h"

#include <algorithm>
#include <format>
#include <string>
#include <vector>

extern interactive_menu *menu;

void l_condo()
{
  pol ol, prev = nullptr;
  int i, done = false, over = false, weeksleep = false;
  char response;

  if(!gamestatusp(SOLD_CONDO, GameStatus))
  {
    queue_message("Rampart Arms. Weekly Rentals and Purchases");
    append_message("Which are you interested in [r,p, or ESCAPE] ", true);
    response = mgetc();
    if(response == 'p')
    {
      queue_message("Only 50,000Au. Buy it? [yn] ");
      if(ynq() == 'y')
      {
        if(Player.cash < 50000)
        {
          queue_message("No mortgages, buddy.");
        }
        else
        {
          setgamestatus(SOLD_CONDO, GameStatus);
          Player.cash -= 50000;
          dataprint();
          queue_message("You are the proud owner of a luxurious condo penthouse.");
          Condoitems = nullptr;
        }
      }
    }
    else if(response == 'r')
    {
      queue_message("Weekly Rental, 1000Au. Pay for it? [yn] ");
      if(ynq() == 'y')
      {
        if(Player.cash < 1000)
        {
          queue_message("Hey, pay the rent or out you go....");
        }
        else
        {
          weeksleep = true;
          Player.cash -= 1000;
          dataprint();
        }
      }
    }
    else
    {
      queue_message("Please keep us in mind for your housing needs.");
    }
  }
  else
  {
    const std::vector<std::string> lines = {{"Home Sweet Home"},        {"a: Leave items in your safe."},
                                            {"b: Retrieve items."},     {"c: Take a week off to rest."},
                                            {"d: Retire permanently."}, {"ESCAPE: Leave this place."}};
    while(!done)
    {
      menu->load(lines);
      menu->print();
      response = static_cast<char>(menu->get_player_input());
      if(response == 'a')
      {
        i = getitem(NULL_ITEM);
        if(i != ABORT)
        {
          if(Player.possessions[i]->blessing < 0)
          {
            queue_message("The item just doesn't want to be stored away...");
          }
          else
          {
            ol         = new objectlist;
            ol->thing  = Player.possessions[i];
            ol->next   = Condoitems;
            Condoitems = ol;
            conform_unused_object(Player.possessions[i]);
            Player.possessions[i] = nullptr;
            queue_message(std::format("Item stored in safe: {}", itemid(ol->thing)));
          }
        }
      }
      else if(response == 'b')
      {
        ol = Condoitems;
        while(ol && !over)
        {
          queue_message("Retrieve ");
          queue_message(itemid(ol->thing));
          queue_message(" [ynq] ");
          response = (char)mcigetc();
          if(response == 'y')
          {
            gain_item(ol->thing);
            if(ol == Condoitems)
            {
              Condoitems = Condoitems->next;
            }
            else if(prev)
            {
              prev->next = ol->next;
            }
          }
          else if(response == 'q')
          {
            over = true;
          }
          prev = ol;
          ol   = ol->next;
        }
      }
      else if(response == 'c')
      {
        weeksleep = true;
        queue_message("You take a week off to rest...");
      }
      else if(response == 'd')
      {
        queue_message("You sure you want to retire, now? [yn] ");
        if(ynq() == 'y')
        {
          p_win();
        }
      }
      else if(response == ESCAPE)
      {
        done = true;
      }
    }
    xredraw();
  }
  if(weeksleep)
  {
    queue_message("Taking a week off to rest...");
    toggle_item_use(true);
    Player.hp  = Player.maxhp;
    Player.str = Player.maxstr;
    Player.agi = Player.maxagi;
    Player.con = Player.maxcon;
    Player.dex = Player.maxdex;
    Player.iq  = Player.maxiq;
    Player.pow = Player.maxpow;
    for(i = 0; i < NUMSTATI; i++)
    {
      if(Player.status[i] < 1000)
      {
        Player.status[i] = 0;
      }
    }
    toggle_item_use(false);
    Player.food = 36;
    queue_message("You're once again fit and ready to continue your adventure.");
    Time += 60 * 24 * 7;
    Date += 7;
    moon_check();
    timeprint();
  }
}

void gymtrain(int *maxstat, int *stat)
{
  if(Gymcredit + Player.cash < 2000)
  {
    append_message("You can't afford our training!", true);
  }
  else
  {
    if(Gymcredit > 2000)
    {
      Gymcredit -= 2000;
    }
    else
    {
      Player.cash -= (2000 - Gymcredit);
      Gymcredit = 0;
    }
    append_message("Sweat. Sweat. ", true);
    if((*maxstat < 30) && ((*maxstat < random_range(30)) || (random_range(3) == 1)))
    {
      append_message("The training pays off!");
      (*maxstat)++;
      (*stat)++;
    }
    else
    {
      append_message("You feel the healthy glow of a good workout.");
      if(*stat < *maxstat)
      {
        (*stat)++;
        append_message("A feeling of rehabilitation washes through you.", true);
      }
    }
  }
  dataprint();
}

void healforpay()
{
  if(Player.cash < 50)
  {
    queue_message("You can't afford to be healed!");
  }
  else
  {
    Player.cash -= 50;
    Player.hp += 20 + random_range(20);
    if(Player.hp > Player.maxhp)
    {
      Player.hp = Player.maxhp;
    }
    queue_message("Another medical marvel....");
  }
  calc_melee();
}

void cureforpay()
{
  if(Player.cash < 250)
  {
    queue_message("You can't afford to be cured!");
  }
  else
  {
    Player.cash -= 250;
    Player.status[DISEASED] = 0;
    queue_message("Quarantine lifted....");
    showflags();
  }
}

void pacify_guards()
{
  pml ml;

  for(ml = Level->mlist; ml; ml = ml->next)
  {
    if((ml->m->id == GUARD) || /*guard*/
       ((ml->m->id == HISCORE_NPC) && (ml->m->aux2 == 15)))
    { /* justiciar */
      m_status_reset(*ml->m, HOSTILE);
      ml->m->specialf = M_NO_OP;
      if(ml->m->id == GUARD && ml->m->hp > 0 && ml->m->aux1 > 0)
      {
        if(Level->site[ml->m->x][ml->m->y].creature == ml->m)
        {
          Level->site[ml->m->x][ml->m->y].creature = nullptr;
        }
        ml->m->x                                 = ml->m->aux1;
        ml->m->y                                 = ml->m->aux2;
        Level->site[ml->m->x][ml->m->y].creature = ml->m;
      }
      else if(ml->m->id == HISCORE_NPC && ml->m->hp > 0 && Current_Environment == E_CITY)
      {
        if(Level->site[ml->m->x][ml->m->y].creature == ml->m)
        {
          Level->site[ml->m->x][ml->m->y].creature = nullptr;
        }
        ml->m->x                                 = 40;
        ml->m->y                                 = 62;
        Level->site[ml->m->x][ml->m->y].creature = ml->m;
      }
    }
  }
  if(Current_Environment == E_CITY)
  {
    Level->site[40][60].p_locf = L_ORDER; /* undoes action in alert_guards */
  }
}

void send_to_jail()
{
  if(Player.rank[ORDER] > 0)
  {
    queue_message("A member of the Order of Paladins sent to jail!");
    queue_message("It cannot be!");
    queue_message("You are immediately expelled permanently from the Order!");
    queue_message("Your name is expunged from the records....");
    Player.rank[ORDER] = -1;
  }
  else if(gamestatusp(DESTROYED_ORDER, GameStatus))
  {
    queue_message("The destruction of the Order of Paladins has negated the law!");
  }
  else if((Current_Environment != E_CITY) && (Last_Environment != E_CITY))
  {
    queue_message("Fortunately, there is no jail around here, so you are freed!");
  }
  else
  {
    pacify_guards();
    if(((Current_Environment == E_HOUSE) || (Current_Environment == E_MANSION) || (Current_Environment == E_HOVEL)) && (Last_Environment == E_CITY))
    {
      setgamestatus(SUPPRESS_PRINTING, GameStatus);
      change_environment(E_CITY);
      resetgamestatus(SUPPRESS_PRINTING, GameStatus);
    }
    if(Current_Environment == E_CITY)
    {
      if(gamestatusp(UNDEAD_GUARDS, GameStatus))
      {
        queue_message("You are taken to a weirdly deserted chamber where an undead");
        queue_message("Magistrate presides over a court of ghosts and haunts.");
        queue_message("'Mr. Foreman, what is the verdict?'");
        queue_message("'Guilty as charged, your lordship.'");
        queue_message("'Guilty...");
        queue_message("Guilty...");
        queue_message("Guilty...");
        queue_message("Guilty...'");
        queue_message("The members of the court close in around, fingers pointing.");
        queue_message("You feel insubstantial hands closing around your throat....");
        queue_message("You feel your life draining away!");
        while(Player.level > 0)
        {
          Player.level--;
          Player.xp /= 2;
          Player.hp /= 2;
          dataprint();
        }
        Player.maxhp = Player.maxcon;
        queue_message("You are finally released, a husk of your former self....");
        Player.x = 58;
        Player.y = 40;
        screencheck(Player.x, Player.y);
      }
      else if(Player.alignment + random_range(200) < 0)
      {
        queue_message("Luckily for you, a smooth-tongued advocate from the");
        queue_message("Rampart Chaotic Liberties Union gets you off!");
        Player.x = 58;
        Player.y = 40;
        screencheck(Player.x, Player.y);
      }
      else
      {
        switch(Imprisonment++)
        {
          case 0:
            queue_message("The Magistrate sternly reprimands you.");
            queue_message("As a first-time offender, you are given probation.");
            Player.y = 58;
            Player.x = 40;
            screencheck(Player.x, Player.y);
            break;
          case 1:
            queue_message("The Magistrate expresses shame for your conduct.");
            queue_message("You are thrown in jail!");
            repair_jail();
            Player.y = 54;
            Player.x = 37 + (2 * random_range(4));
            screencheck(Player.x, Player.y);
            l_portcullis_trap();
            break;
          default:
            queue_message("The Magistrate renders summary judgement.");
            queue_message("You are sentenced to prison!");
            queue_message("The guards recognize you as a 'three-time-loser'");
            queue_message("...and beat you up a little to teach you a lesson.");
            p_damage(random_range(Imprisonment * 10), UNSTOPPABLE, "police brutality");
            repair_jail();
            Player.y = 54;
            Player.x = 37 + (2 * random_range(4));
            screencheck(Player.x, Player.y);
            l_portcullis_trap();
        }
      }
    }
  }
}

void l_adept()
{
  queue_message("You see a giant shimmering gate in the form of an omega.");
  if(!gamestatusp(ATTACKED_ORACLE, GameStatus))
  {
    if(Player.str + Player.con + Player.iq + Player.pow < 100)
    {
      queue_message("A familiar female voice says: I would not advise this now....");
    }
    else
    {
      queue_message("A familiar female voice says: Go for it!");
    }
  }
  queue_message("Enter the mystic portal? [yn] ");
  if(ynq() != 'y')
  {
    if(Player.level > 100)
    {
      queue_message("The Lords of Destiny spurn your cowardice....");
      Player.xp    = 0;
      Player.level = 0;
      Player.hp = Player.maxhp = Player.con;
      Player.mana              = calcmana();
      queue_message("You suddenly feel very inexperienced.");
      dataprint();
    }
  }
  else
  {
    queue_message("You pass through the portal.");
    drawomega();
    queue_message("Like wow man! Colors! ");
    if(Player.patron != DESTINY)
    {
      queue_message("Strange forces try to tear you apart!");
      p_damage(random_range(200), UNSTOPPABLE, "a vortex of chaos");
    }
    else
    {
      queue_message("Some strange force shields you from a chaos vortex!");
    }
    queue_message("Your head spins for a moment....");
    queue_message("and clears....");
    Player.hp   = Player.maxhp;
    Player.mana = calcmana();
    change_environment(E_ABYSS);
  }
}

void l_triffid()
{
  int damage = 0, stuck = true;
  queue_message("The hedge comes alive with a surge of alien growth!");
  while(stuck)
  {
    dataprint();
    damage += Level->depth / 2 + 1;
    queue_message("Razor-edged vines covered in suckers attach themselves to you.");
    if(find_and_remove_item(THINGID + 6, -1))
    {
      queue_message("Thinking fast, you toss salt water on the triffid...");
      queue_message("The triffid disintegrates with a frustrated sigh.");
      Level->site[Player.x][Player.y].locchar = FLOOR;
      Level->site[Player.x][Player.y].p_locf  = L_NO_OP;
      lset(Player.x, Player.y, CHANGED, *Level);
      gain_experience(1000);
      stuck = false;
    }
    else
    {
      p_damage(damage, UNSTOPPABLE, "a triffid");
      queue_message("You are entangled in tendrils...");
      std::vector<std::string> lines = {
        {"a: Try to break free."},
        {"b: Hang limp and hope the tendrils uncoil."},
        {"c: Pray for assistance."},
        {"d: Attempt to bargain with the hedge."},
        {"e: Click your heels together and wish for escape."},
        {"ANYTHING ELSE: writhe and scream hopelessly."}};
      menu->load(lines);
      menu->print();
      switch(menugetc())
      {
        case 'a':
          if(Player.str > random_range(200))
          {
            queue_message("Amazing! You're now free.");
            queue_message("The triffid writhes hungrily at you.");
            stuck = false;
          }
          else
          {
            queue_message("Well, THAT didn't work.");
          }
          break;
        case 'b':
          queue_message("Well, at least you're facing your fate with dignity.");
          break;
        case 'c':
          if((Player.patron == DRUID) && (Player.rank[PRIESTHOOD] > random_range(5)))
          {
            queue_message("A shaft of golden light bathes the alien plant");
            queue_message("which grudginly lets you go....");
            stuck = false;
          }
          else
          {
            queue_message("You receive no divine aid as yet.");
          }
          break;
        case 'd':
          queue_message("The hedge doesn't answer your entreaties.");
          break;
        case 'e':
          queue_message("You forgot your ruby slippers, stupid.");
          break;
        default:
          queue_message("The hedge enjoys your camp play-acting....");
          break;
      }
    }
  }
  xredraw();
}

void l_vault()
{
  queue_message("You come to a thick vault door with a complex time lock.");
  if((hour() == 23))
  {
    queue_message("The door is open.");
    Level->site[12][56].locchar = FLOOR;
  }
  else
  {
    queue_message("The door is closed.");
    Level->site[12][56].locchar = WALL;
    queue_message("Try to crack it? [yn] ");
    if(ynq() == 'y')
    {
      if(random_range(100) < Player.rank[THIEVES] * Player.rank[THIEVES])
      {
        queue_message("The lock clicks open!!!");
        gain_experience(5000);
        Level->site[12][56].locchar = FLOOR;
      }
      else
      {
        queue_message("Uh, oh, set off the alarm.... The castle guard arrives....");
        if(Player.rank[NOBILITY] == DUKE)
        {
          queue_message("\"Ah, just testing us, your Grace?  I hope we're up to "
                        "scratch.\"");
        }
        else
        {
          send_to_jail();
        }
      }
    }
    else
    {
      queue_message("Good move.");
    }
  }
}

void l_brothel()
{
  char response;
  queue_message("You come to a heavily reinforced inner door.");
  queue_message("A sign reads `The House of the Eclipse'");
  queue_message("Try to enter? [yn] ");
  if(ynq() == 'y')
  {
    std::vector<std::string> lines = {
      {"a:knock on the door."},
      {"b:try to pick the lock."},
      {"c:bash down the door."},
      {"ESCAPE: Leave this house of ill repute."}};
    menu->load(lines);
    menu->print();
    do
    {
      response = menugetc();
    } while((response != 'a') && (response != 'b') && (response != 'c') && (response != ESCAPE));
    xredraw();
    if(response == 'a')
    {
      if(!nighttime())
      {
        queue_message("There is no reponse.");
      }
      else
      {
        queue_message("A window opens in the door.");
        queue_message("`500Au, buddy. For the night.' pay it? [yn] ");
        if(ynq() == 'y')
        {
          if(Player.cash < 500)
          {
            queue_message("`What, no roll?!'");
            queue_message("The bouncer bounces you a little and lets you go.");
            p_damage(25, UNSTOPPABLE, "da bouncer");
          }
          else
          {
            Player.cash -= 500;
            queue_message("Would you like to see a male or female worker? [mf] ");
            char preference;
            do
            {
              preference = static_cast<char>(mcigetc());
            } while(preference != 'm' && preference != 'f' && preference != 'n');
            queue_message("You are ushered into an opulently appointed hall.");
            queue_message("After an expensive dinner (takeout from Les Crapuleux)");
            if(preference == 'n')
            {
              switch(random_range(4))
              {
                case 0:
                  append_message("you spend the evening playing German Whist with");
                  break;
                case 1:
                  append_message("you spend the evening discussing philosophy with");
                  break;
                case 2:
                  append_message("you spend the evening playing chess against");
                  break;
                case 3:
                  append_message("you spend the evening telling your adventures to");
              }
              append_message("various employees of the House of the Eclipse.");
            }
            else
            {
              queue_message("you spend an enjoyable and educational evening with");
              if(preference == 'm' || (preference == 'b' && random_range(2)))
              {
                switch(random_range(4))
                {
                  case 0:
                    append_message("Skarn the Insatiable, a satyr.");
                    break;
                  case 1:
                    append_message("Dryden the Defanged, an incubus.");
                    break;
                  case 2:
                    append_message("Gorgar the Equipped, a centaur.");
                    break;
                  case 3:
                    append_message("Hieronymus, the mendicant priest of Eros.");
                    break;
                }
              }
              else
              {
                switch(random_range(4))
                {
                  case 0:
                    append_message("Noreen the Nymph (omaniac)");
                    break;
                  case 1:
                    append_message("Angelface, a recanted succubus.");
                    break;
                  case 2:
                    append_message("Corporal Sue of the City Guard (moonlighting).");
                    break;
                  case 3:
                    append_message("Sheena the Queena the Jungle, a wereleopard.");
                    break;
                }
              }
            }
            if(hour() > 12)
            {
              Time += ((24 - hour()) + 8) * 60;
            }
            else
            {
              Time += ((9 - hour()) * 60);
              Date++;
            }
            Player.food             = 40;
            Player.status[DISEASED] = 0;
            Player.status[POISONED] = 0;
            Player.hp               = Player.maxhp;
            /* reduce temporary stat gains to max stat levels */
            toggle_item_use(true);
            Player.str = std::min(Player.str, Player.maxstr);
            Player.con = std::min(Player.con, Player.maxcon);
            Player.agi = std::min(Player.agi, Player.maxagi);
            Player.dex = std::min(Player.dex, Player.maxdex);
            Player.iq  = std::min(Player.iq, Player.maxiq);
            Player.pow = std::min(Player.pow, Player.maxpow);
            toggle_item_use(false);
            if(preference == 'n')
            {
              Player.iq++; /* whatever :-) */
            }
            else
            {
              Player.con++;
            }
            gain_experience(100);
            timeprint();
            dataprint();
            showflags();
            if(preference == 'n')
            {
              queue_message("You arise refreshed the next morning...");
            }
            else
            {
              queue_message("You arise, tired but happy, the next morning...");
            }
          }
        }
        else
        {
          queue_message("What are you, some kinda prude?");
        }
      }
    }
    else if(response == 'b')
    {
      if(nighttime())
      {
        queue_message("As you fumble at the lock, the door opens....");
        queue_message("The bouncer tosses you into the street.");
      }
      else
      {
        queue_message("The door appears to be bolted and barred from behind.");
      }
    }
    else if(response == 'c')
    {
      if(nighttime())
      {
        queue_message("As you charge toward the door it opens....");
        queue_message("Yaaaaah! Thud!");
        queue_message("You run past the startled bouncer into a wall.");
        p_damage(20, UNSTOPPABLE, "a move worthy of Clouseau");
        queue_message("The bouncer tosses you into the street.");
      }
      else
      {
        queue_message("Ouch! The door resists your efforts.");
        p_damage(1, UNSTOPPABLE, "a sturdy door");
        queue_message("You hear an irritated voice from inside:");
        queue_message("'Keep it down out there! Some of us are trying to sleep!'");
      }
    }
  }
}

/* if signp is true, always print message, otherwise do so only sometimes */
void sign_print(int x, int y, int signp)
{
  if((Level->site[x][y].p_locf >= CITYSITEBASE) && (Level->site[x][y].p_locf < CITYSITEBASE + NUMCITYSITES))
  {
    CitySiteList[Level->site[x][y].p_locf - CITYSITEBASE][0] = true;
  }
  switch(Level->site[x][y].p_locf)
  {
    case L_CHARITY:
      queue_message("You notice a sign: The Rampart Orphanage And Hospice For The Needy.");
      break;
    case L_MANSION:
      queue_message("You notice a sign:");
      queue_message("This edifice protected by DeathWatch Devices, Ltd.");
      break;
    case L_GRANARY:
      queue_message("You notice a sign:");
      queue_message("Public Granary: Entrance Strictly Forbidden.");
      break;
    case L_PORTCULLIS:
      if(Level->site[x][y].locchar == FLOOR)
      {
        queue_message("You see a groove in the floor and slots above you.");
      }
      break;
    case L_STABLES:
      queue_message("You notice a sign:");
      queue_message("Village Stables");
      break;
    case L_COMMONS:
      queue_message("You notice a sign:");
      queue_message("Village Commons: No wolves allowed.");
      break;
    case L_MAZE:
      queue_message("You notice a sign:");
      queue_message("Hedge maze closed for triffid extermination.");
      break;
    case L_BANK:
      if(signp)
      {
        queue_message("You notice a sign:");
        queue_message("First Bank of Omega: Autoteller Carrel.");
      }
      break;
    case L_TEMPLE:
      queue_message("You see the ornate portico of the Rampart Pantheon");
      break;
    case L_ARMORER:
      if(signp)
      {
        queue_message("You notice a sign:");
        queue_message("Julie's Armor of Proof and Weapons of Quality");
      }
      break;
    case L_CLUB:
      if(signp)
      {
        queue_message("You notice a sign:");
        queue_message("Rampart Explorers' Club.");
      }
      break;
    case L_GYM:
      if(signp)
      {
        queue_message("You notice a sign:");
        queue_message("The Rampart Gymnasium, (affil. Rampart Coliseum).");
      }
      break;
    case L_HEALER:
      if(signp)
      {
        queue_message("You notice a sign:");
        queue_message("Rampart Healers. Member RMA.");
      }
      break;
    case L_CASINO:
      if(signp)
      {
        queue_message("You notice a sign:");
        queue_message("Rampart Mithril Nugget Casino.");
      }
      break;
    case L_SEWER:
      queue_message("A sewer entrance. You don't want to go down THERE, do you?");
      break;
    case L_COMMANDANT:
      if(signp)
      {
        queue_message("You notice a sign:");
        queue_message("Commandant Sonder's Rampart-fried Lyzzard partes. Open 24 hrs.");
      }
      break;
    case L_DINER:
      if(signp)
      {
        queue_message("You notice a sign:");
        queue_message("The Rampart Diner. All you can eat, 25Au.");
      }
      break;
    case L_CRAP:
      if(signp)
      {
        queue_message("You notice a sign:");
        queue_message("Les Crapeuleaux. (****)");
      }
      break;
    case L_TAVERN:
      if(signp)
      {
        queue_message("You notice a sign:");
        queue_message("The Centaur and Nymph -- J. Riley, prop.");
      }
      break;
    case L_ALCHEMIST:
      if(signp)
      {
        queue_message("You notice a sign:");
        queue_message("Ambrosias' Potions et cie.");
      }
      break;
    case L_DPW:
      if(signp)
      {
        queue_message("You notice a sign:");
        queue_message("Rampart Department of Public Works.");
      }
      break;
    case L_LIBRARY:
      if(signp)
      {
        queue_message("You notice a sign:");
        queue_message("Rampart Public Library.");
      }
      break;
    case L_CONDO:
      if(signp)
      {
        queue_message("You notice a sign:");
        if(gamestatusp(SOLD_CONDO, GameStatus))
        {
          queue_message("Home Sweet Home");
        }
        else
        {
          queue_message("Luxury Condominium For Sale; Inquire Within");
        }
      }
      break;
    case L_PAWN_SHOP:
      if(signp)
      {
        queue_message("You notice a sign:");
        queue_message("Knight's Pawn Shop.");
      }
      break;
    case L_CEMETARY:
      queue_message("You notice a sign:");
      queue_message("Rampart City Cemetary. Closed -- Full.");
      break;
    case L_GARDEN:
      queue_message("You notice a sign:");
      queue_message("Rampart Botanical Gardens---Do not deface statues.");
      break;
    case L_JAIL:
      queue_message("You notice a sign:");
      queue_message("Rampart City Gaol -- always room for more.");
      break;
    case L_MONASTERY:
      queue_message("You notice a sign:");
      queue_message("Tholian Monastery. Truthseekers welcome.");
      break;
    case L_ORACLE:
      queue_message("You notice a sign:");
      queue_message("The Oracle of the Cyan Flames");
      break;
  }
}

void l_countryside()
{
  if(optionp(CONFIRM, Player))
  {
    queue_message("Do you really want to return to the countryside? ");
    if(ynq() != 'y')
    {
      return;
    }
  }
  change_environment(E_COUNTRYSIDE);
}

void l_oracle()
{
  char response;
  if(gamestatusp(ATTACKED_ORACLE, GameStatus) && (!gamestatusp(COMPLETED_ASTRAL, GameStatus)))
  {
    queue_message("You come before a blue crystal dais. You see a broken mirror.");
    queue_message("Look in the mirror? [yn] ");
    if(ynq() == 'y')
    {
      queue_message("A strange force rips you from your place....");
      Player.hp = 1;
      queue_message("You feel drained....");
      dataprint();
      queue_message("You find yourself in a weird flickery maze.");
      change_environment(E_ASTRAL);
    }
  }
  else
  {
    queue_message("You come before a blue crystal dais. There is a bell and a mirror.");
    queue_message("Ring the bell [b], look in the mirror [m], or leave [ESCAPE] ");
    do
    {
      response = (char)mcigetc();
    } while((response != 'b') && (response != 'm') && (response != ESCAPE));
    if(response == 'b')
    {
      queue_message("The ringing note seems to last forever.");
      queue_message("You notice a robed figure in front of you....");
      queue_message("The oracle doffs her cowl. Her eyes glitter with blue fire!");
      queue_message("Attack her? [yn] ");
      if(ynq() == 'y')
      {
        setgamestatus(ATTACKED_ORACLE, GameStatus);
        queue_message("The oracle deftly avoids your attack.");
        queue_message("She sneers at you and vanishes.");
      }
      else
      {
        queue_message("She stares at you...and speaks:");
        if(!gamestatusp(SPOKE_TO_DRUID, GameStatus))
        {
          queue_message("'The ArchDruid speaks wisdom in his forest shrine.'");
        }
        else if(!gamestatusp(COMPLETED_CAVES, GameStatus))
        {
          queue_message("'Thou mayest find aught of interest in the caves to the "
                        "South.'");
        }
        else if(!gamestatusp(COMPLETED_SEWERS, GameStatus))
        {
          queue_message("'Turn thy attention to the abyssal depths of the city.'");
        }
        else if(!gamestatusp(COMPLETED_CASTLE, GameStatus))
        {
          queue_message("'Explorest thou the depths of the Castle of the ArchMage.'");
        }
        else if(!gamestatusp(COMPLETED_ASTRAL, GameStatus))
        {
          queue_message("'Journey to the Astral Plane and meet the Gods' servants.'");
          queue_message("The oracle holds out her hand. Do you take it? [yn] ");
          if(ynq() == 'y')
          {
            queue_message("'Beware: Only the Star Gem can escape the Astral Plane.'");
            queue_message("A magic portal opens behind the oracle. She leads you");
            queue_message("through a sequence of special effects that would have");
            queue_message("IL&M technicians cursing in awe and deposits you in an");
            queue_message("odd looking room whose walls seem strangely insubstantial.");
            gain_experience(5000);
            change_environment(E_ASTRAL);
          }
          else
          {
            queue_message("You detect the hint of a sneer from the oracle.");
          }
        }
        else if(!gamestatusp(COMPLETED_VOLCANO, GameStatus))
        {
          queue_message("'The infernal maw may yield its secrets to thee now.'");
        }
        else if(!gamestatusp(COMPLETED_CHALLENGE, GameStatus))
        {
          queue_message("'The challenge of adepthood yet awaits thee.'");
        }
        else
        {
          queue_message("'My lord: Thou hast surpassed my tutelage forever.");
          queue_message("Fare thee well.'");
          queue_message("The oracle replaces her hood and seems to fade away....");
        }
      }
    }
    else if(response == 'm')
    {
      queue_message("You seem to see yourself. Odd....");
      knowledge(1);
    }
    else
    {
      queue_message("You leave this immanent place.");
    }
  }
}

void l_mansion()
{
  queue_message("Enter the mansion? [yn] ");
  if(ynq() == 'y')
  {
    change_environment(E_MANSION);
  }
}

void l_house()
{
  queue_message("Enter the house? [yn] ");
  if(ynq() == 'y')
  {
    change_environment(E_HOUSE);
  }
}

void l_hovel()
{
  queue_message("Enter the hovel? [yn] ");
  if(ynq() == 'y')
  {
    change_environment(E_HOVEL);
  }
}

void l_safe()
{
  char response;
  pob newitem;
  int attempt = 0;
  queue_message("You have discovered a safe!");
  queue_message("Pick the lock [p], Force the door [f], or ignore [ESCAPE]");
  do
  {
    response = (char)mcigetc();
  } while((response != 'p') && (response != 'f') && (response != ESCAPE));
  if(response == 'p')
  {
    attempt = (2 * Player.dex + Player.rank[THIEVES] * 10 - random_range(100)) / 10;
  }
  else if(response == 'f')
  {
    attempt = (Player.dmg - random_range(100)) / 10;
  }
  if(attempt > 0)
  {
    Player.alignment -= 4;
    gain_experience(50);
    queue_message("The door springs open!");
    Level->site[Player.x][Player.y].locchar = FLOOR;
    Level->site[Player.x][Player.y].p_locf  = L_NO_OP;
    lset(Player.x, Player.y, CHANGED, *Level);
    if(random_range(2) == 1)
    {
      queue_message("You find:");
      do
      {
        newitem = create_object(difficulty());
        queue_message(itemid(newitem));
        gain_item(newitem);
      } while(random_range(3) == 1);
    }
    else
    {
      queue_message("The safe was empty (awwwww....)");
    }
  }
  else
  {
    queue_message("Your attempt at burglary failed!");
    if(attempt == -1)
    {
      queue_message("A siren goes off! You see flashing red lights everywhere!");
      if(Last_Environment == E_CITY)
      {
        queue_message("The city guard shows up! They collar you in no time flat!");
        change_environment(E_CITY);
        send_to_jail();
      }
    }
    else if(attempt == -2)
    {
      queue_message("There is a sudden flash!");
      p_damage(random_range(25), FLAME, "a safe");
      queue_message("The safe has self-destructed.");
      Level->site[Player.x][Player.y].locchar = RUBBLE;
      Level->site[Player.x][Player.y].p_locf  = L_RUBBLE;
      lset(Player.x, Player.y, CHANGED, *Level);
    }
    else if(attempt == -3)
    {
      queue_message("The safe jolts you with electricity!");
      lball(Player.x, Player.y, Player.x, Player.y, 30);
    }
    else if(attempt < -3)
    {
      queue_message("You are hit by an acid spray!");
      if(Player.possessions[O_CLOAK])
      {
        queue_message("Your cloak is destroyed!");
        conform_lost_object(Player.possessions[O_CLOAK]);
        p_damage(10, ACID, "a safe");
      }
      else if(Player.possessions[O_ARMOR])
      {
        queue_message("Your armor corrodes!");
        Player.possessions[O_ARMOR]->dmg -= 3;
        Player.possessions[O_ARMOR]->hit -= 3;
        Player.possessions[O_ARMOR]->aux -= 3;
        p_damage(10, ACID, "a safe");
      }
      else
      {
        queue_message("The acid hits your bare flesh!");
        p_damage(random_range(100), ACID, "a safe");
      }
    }
  }
}

void l_cartographer()
{
  int i, j, x, y;
  queue_message("Ye Olde Mappe Shoppe.");
  queue_message("Map of the local area: 500Au. Buy it? [yn] ");
  if(ynq() == 'y')
  {
    if(Player.cash < 500)
    {
      queue_message("Cursed be cheapskates! May you never find an aid station....");
    }
    else
    {
      queue_message("You now have the local area mapped.");
      Player.cash -= 500;
      dataprint();
      switch(Villagenum)
      {
        case 1:
          x = 56;
          y = 5;
          break;
        default:
        case 2:
          x = 35;
          y = 11;
          break;
        case 3:
          x = 10;
          y = 40;
          break;
        case 4:
          x = 7;
          y = 6;
          break;
        case 5:
          x = 40;
          y = 43;
          break;
        case 6:
          x = 20;
          y = 41;
          break;
      }
      for(i = x - 15; i <= x + 15; i++)
      {
        for(j = y - 15; j <= y + 15; j++)
        {
          if((i >= 0) && (i < 64) && (j >= 0) && (j < 64))
          {
            if(Country[i][j].current_terrain_type != Country[i][j].base_terrain_type)
            {
              c_set(i, j, CHANGED, Country);
              Country[i][j].current_terrain_type = Country[i][j].base_terrain_type;
            }
            c_set(i, j, SEEN, Country);
          }
        }
      }
    }
  }
  else
  {
    queue_message("Don't blame me if you get lost....");
  }
}

void l_charity()
{
  queue_message("'Greetings, friend. Do you wish to make a donation?' [yn] ");
  if(ynq() != 'y')
  {
    append_message("'Pinchpurse!'", true);
  }
  else
  {
    append_message("How much can you give? ", true);
    long donation = parsenum();
    if(donation < 1)
    {
      append_message("'Go stick your head in a pig.'", true);
    }
    else if(donation > Player.cash)
    {
      append_message("'I'm afraid you're charity is bigger than your purse!'", true);
    }
    else if(donation < std::max(100, Player.level * Player.level * 100))
    {
      append_message("'Oh, can't you do better than that?'", true);
      append_message("'Well, I guess we'll take it....'", true);
      if(Player.alignment < 10)
      {
        Player.alignment++;
      }
      Player.cash -= donation;
    }
    else
    {
      append_message("'Oh thank you kindly, friend, and bless you!'", true);
      Player.cash -= donation;
      Player.alignment += 5;
    }
  }
  dataprint();
}
