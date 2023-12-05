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

// site1.cpp
// 1st half of site functions and aux functions to them

#include "glob.h"
#include "scr.h"
#include "scrolling_buffer.hpp"
#include "spell.h"

#include <algorithm>
#include <chrono>
#include <string>
#include <thread>
#include <vector>

extern scrolling_buffer message_buffer;

// the bank; can be broken into (!)
void l_bank()
{
  long amount;
  char response;
  std::string passwd;
  queue_message("First Bank of Omega: Autoteller Carrel.");

  if(gamestatusp(BANK_BROKEN, GameStatus))
  {
    append_message("You see a damaged autoteller.", true);
  }
  else
  {
    append_message("The proximity sensor activates the autoteller as you approach.", true);
    bool done = false, valid = false;
    while(!done)
    {
      append_message(std::format("Current Balance: {}Au.", Balance), true);
      append_message("Enter command (? for help) > ", true);
      response = mgetc();
      if(response == '?')
      {
        std::vector<std::string> lines;
        lines.emplace_back("?: This List");
        if(Password.empty())
        {
          lines.emplace_back("O: Open an account");
        }
        else
        {
          lines.emplace_back("P: Enter password");
          lines.emplace_back("D: Deposit");
          lines.emplace_back("W: Withdraw");
        }
        lines.emplace_back("X: eXit");
        menu->load(lines);
        menu->print();
        continue;
      }
      else if(response == 'P' && !Password.empty())
      {
        append_message("Password: ", true);
        passwd = msgscanstring();
        valid  = (passwd == Password);
        if(!valid)
        {
          done                           = true;
          std::vector<std::string> lines = {
            {"Alert! Alert! Invalid Password!"},
            {"The police are being summoned!"},
            {"Please wait for the police to arrive...."},
            {""},
            {"----Hit space bar to continue----"}};
          menu->load(lines);
          response = menu->get_player_input();
          if(response == ' ')
          {
            Player.alignment += 5;
            xredraw();
            append_message("Ah ha! Trying to rob the bank, eh?", true);
            append_message("Take him away, boys!");
            send_to_jail();
          }
          else
          {
            Player.alignment -= 5;
            lines.clear();
            menu->load(lines);
            menu->print();
            doupdate();
            std::this_thread::sleep_for(std::chrono::seconds(2));
            lines.emplace_back("^@^@^@^@^@00AD1203BC0F0000FFFFFFFFFFFF");
            lines.emplace_back("Interrupt in _get_space. Illegal Character.");
            menu->load(lines);
            menu->print();
            doupdate();
            std::this_thread::sleep_for(std::chrono::seconds(2));
            lines.emplace_back("Aborting _police_alert.....");
            lines.emplace_back("Attempting reboot.....");
            menu->load(lines);
            menu->print();
            doupdate();
            std::this_thread::sleep_for(std::chrono::seconds(2));
            lines.emplace_back("Warning: Illegal shmop at _count_cash.");
            lines.emplace_back("Warning: Command Buffer NOT CLEARED");
            menu->load(lines);
            menu->print();
            doupdate();
            std::this_thread::sleep_for(std::chrono::seconds(2));
            lines.emplace_back("Reboot Complete. Execution Continuing.");
            lines.emplace_back("Withdrawing: 4294967297 Au.");
            lines.emplace_back("Warning: Arithmetic Overflow in _withdraw");
            menu->load(lines);
            menu->print();
            doupdate();
            std::this_thread::sleep_for(std::chrono::seconds(2));
            lines.emplace_back("Yo mama. Core dumped.");
            menu->load(lines);
            menu->print();
            doupdate();
            std::this_thread::sleep_for(std::chrono::seconds(2));
            queue_message("The cash machine begins to spew gold pieces!");
            queue_message("You pick up your entire balance and then some!");
            Player.cash += Balance + 1000 + random_range(3000);
            Balance = 0;
            setgamestatus(BANK_BROKEN, GameStatus);
          }
        }
        else
        {
          append_message("Password accepted. Working.", true);
        }
      }
      else if((response == 'D') && valid)
      {
        append_message("Amount: ", true);
        amount = get_money(Player.cash);
        if(amount < 1)
        {
          append_message("Transaction aborted.", true);
        }
        else if(amount > Player.cash)
        {
          append_message("Deposit too large -- transaction aborted.", true);
        }
        else
        {
          append_message("Transaction accomplished.", true);
          Balance += amount;
          Player.cash -= amount;
        }
      }
      else if((response == 'W') && valid)
      {
        append_message("Amount: ", true);
        amount = get_money(Balance);
        if(amount < 1)
        {
          append_message("Transaction aborted.", true);
        }
        else if(amount > Balance)
        {
          append_message("Withdrawal too large -- transaction aborted.", true);
        }
        else
        {
          append_message("Transaction accomplished.", true);
          Balance -= amount;
          Player.cash += amount;
        }
      }
      else if(response == 'X')
      {
        append_message("Bye!", true);
        done = true;
      }
      else if(response == 'O' && Password.empty())
      {
        append_message("Opening new account.", true);
        append_message(" Please enter new password: ");
        Password = msgscanstring();
        if(Password.empty())
        {
          append_message("Illegal to use null password -- aborted.", true);
          done = true;
        }
        else
        {
          append_message("Password validated; account saved.", true);
          valid = true;
        }
      }
      else
      {
        append_message("Illegal command.", true);
      }
      dataprint();
    }
  }
  xredraw();
}

void buyfromstock(int base, int numitems)
{

  append_message("Purchase which item? [ESCAPE to quit] ", true);
  std::vector<std::string> lines;
  for(int i = 0; i < numitems; ++i)
  {
    lines.emplace_back(std::format("{}:{}", static_cast<char>(i + 'a'), Objects[base + i].objstr));
  }
  menu->load(lines);
  int player_input = ' ';
  while(player_input != ESCAPE && (player_input < 'a' || player_input >= 'a' + numitems))
  {
    player_input = menu->get_player_input();
  }
  if(player_input != ESCAPE)
  {
    int i          = player_input - 'a';
    auto newitem = std::make_unique<object>(Objects[base + i]);
    newitem->known = 2;
    long price     = 2 * true_item_value(newitem.get());
    append_message(std::format("I can let you have it for {} Au. Buy it? [yn] ", price), true);
    if(ynq() == 'y')
    {
      if(Player.cash < price)
      {
        append_message("Why not try again some time you have the cash?", true);
      }
      else
      {
        Player.cash -= price;
        dataprint();
        gain_item(std::move(newitem));
      }
    }
  }
}

void l_armorer()
{
  if(hour() == 12)
  {
    queue_message("Unfortunately, this is Julie's lunch hour -- try again later.");
  }
  else if(nighttime())
  {
    queue_message("It seems that Julie keeps regular business hours.");
  }
  else
  {
    bool done = false;
    while(!done)
    {
      queue_message("Julie's: Buy Armor, Weapons, or Leave [a,w,ESCAPE] ");
      int player_input = mgetc();
      if(player_input == ESCAPE)
      {
        done = true;
      }
      else if(player_input == 'a')
      {
        buyfromstock(ARMORID, 10);
      }
      else if(player_input == 'w')
      {
        buyfromstock(WEAPONID, 23);
      }
    }
  }
  xredraw();
}

void l_club()
{
#define hinthour club_hinthour
  char response;

  queue_message("Rampart Explorers' Club.");
  if(!gamestatusp(CLUB_MEMBER, GameStatus))
  {
    if(Player.level < 2)
    {
      append_message("Only reknowned adventurers need apply.", true);
    }
    else
    {
      append_message("Dues are 100Au. Pay it? [yn] ", true);
      if(ynq() == 'y')
      {
        if(Player.cash < 100)
        {
          append_message("Beat it, or we'll blackball you!", true);
        }
        else
        {
          append_message("Welcome to the club! You are taught the spell of Return.", true);
          append_message("When cast on the first level of a dungeon it", true);
          append_message("will transport you down to the lowest level");
          append_message("you have explored, and vice versa.");
          spell::Spells[spell::RETURN].known = true;
          Player.cash -= 100;
          setgamestatus(CLUB_MEMBER, GameStatus);
        }
      }
      else
      {
        append_message("OK, but you're missing out on our benefits....", true);
      }
    }
  }
  else
  {
    append_message("Shop at the club store or listen for rumors [sl] ", true);
    do
    {
      response = (char)mcigetc();
    } while((response != 's') && (response != 'l') && (response != ESCAPE));
    if(response == 'l')
    {
      if(hinthour == hour())
      {
        append_message("You don't hear anything useful.", true);
      }
      else
      {
        append_message("You overhear a conversation....", true);
        hint();
        hinthour = hour();
      }
    }
    else if(response == 's')
    {
      buyfromstock(THINGID + 7, 2);
      xredraw();
    }
    else if(response == ESCAPE)
    {
      append_message("Be seeing you, old chap!", true);
    }
  }
}

#undef hinthour

void l_gym()
{
  print_messages();
  std::vector<std::string> lines = {
    {"The Rampart Gymnasium"},
    {""},
    {""},
    {"Train for 2000 Au. Choose:"},
    {""},
    {"a: work out in the weight room"},
    {"b: use our gymnastics equipment"},
    {"c: take our new anaerobics course"},
    {"d: enroll in dance lessons"},
    {"ESCAPE: Leave this place"}};
  menu->load(lines);
  menu->print();
  append_message("The Rampart Gymnasium", true);
  bool done    = true;
  int trained = 0;
  do
  {
    if((Gymcredit > 0) || (Player.rank[ARENA]))
    {
      append_message(std::format("-- Credit: {} Au.", Gymcredit));
    }
    done = false;
    switch(mgetc())
    {
      case 'a':
        gymtrain(&(Player.maxstr), &(Player.str));
        break;
      case 'b':
        gymtrain(&(Player.maxdex), &(Player.dex));
        break;
      case 'c':
        gymtrain(&(Player.maxcon), &(Player.con));
        break;
      case 'd':
        gymtrain(&(Player.maxagi), &(Player.agi));
        break;
      case ESCAPE:
        if(trained == 0)
        {
          append_message("Well, it's your body you're depriving!", true);
        }
        else if(trained < 3)
        {
          append_message("You towel yourself off, and find the exit.", true);
        }
        else
        {
          append_message("A refreshing bath, and you're on your way.", true);
        }
        done = true;
        break;
      default:
        trained--;
        break;
    }
    trained++;
  } while(!done);
  xredraw();
  calc_melee();
}

void l_healer()
{
  std::vector<std::string> lines;
  lines.emplace_back("     Rampart Healers. Member RMA.");
  lines.emplace_back("a: Heal injuries (50 crowns).");
  lines.emplace_back("b: Cure disease (250 crowns).");
  lines.emplace_back("ESCAPE: Leave these antiseptic alcoves.");
  menu->load(lines);
  switch(menu->get_player_input())
  {
    case 'a':
      healforpay();
      break;
    case 'b':
      cureforpay();
      break;
    default:
      append_message("OK, but suppose you have Acute Satyriasis?", true);
      break;
  }
}

void statue_random(int x, int y)
{
  switch(random_range(difficulty() + 3) - 1)
  {
    default:
      l_statue_wake();
      break;
    case 0:
      queue_message("The statue crumbles with a clatter of gravel.");
      Level->site[x][y].locchar = RUBBLE;
      Level->site[x][y].p_locf  = L_RUBBLE;
      plotspot(x, y, true);
      lset(x, y, CHANGED, *Level);
      break;
    case 1:
      queue_message("The statue stoutly resists your attack.");
      break;
    case 2:
      queue_message("The statue crumbles with a clatter of gravel.");
      Level->site[x][y].locchar = RUBBLE;
      Level->site[x][y].p_locf  = L_RUBBLE;
      plotspot(x, y, true);
      lset(x, y, CHANGED, *Level);
      make_site_treasure(x, y, difficulty());
      break;
    case 3:
      queue_message("The statue hits you back!");
      p_damage(random_range(difficulty() * 5), UNSTOPPABLE, "a statue");
      break;
    case 4:
      queue_message("The statue looks slightly pained. It speaks:");
      hint();
      break;
    case 5:
      if((Current_Environment == Current_Dungeon) || (Current_Environment == E_CITY))
      {
        queue_message("You hear the whirr of some mechanism.");
        queue_message("The statue glides smoothly into the floor!");
        // WDT HACK: I shouldn't be making this choice on a level
        // where no stairs can be (or perhaps I should, and I should
        // implement a bonus level!).
        Level->site[x][y].locchar = STAIRS_DOWN;
        Level->site[x][y].p_locf  = L_NO_OP;
        lset(x, y, static_cast<lstatus_bit>(CHANGED | STOPS), *Level);
      }
      break;
    case 6:
      queue_message("The statue was covered with contact cement!");
      queue_message("You can't move....");
      Player.status[IMMOBILE] += random_range(6) + 2;
      break;
    case 7:
      queue_message("A strange radiation emanates from the statue!");
      dispel(-1);
      break;
    case 8: // I think this is particularly evil. Heh heh.
      if(Player.possessions[O_WEAPON_HAND])
      {
        queue_message("Your weapon sinks deeply into the statue and is sucked away!");
        std::unique_ptr<object> &weapon = Player.possessions[O_WEAPON_HAND];
        conform_unused_object(weapon);
        weapon->blessing = -1 - abs(weapon->blessing);
        drop_at(x, y, std::move(weapon));
      }
      break;
    case 9:
      queue_message("The statue extends an arm. Beams of light illuminate the level!");
      for(int i = 0; i < WIDTH; ++i)
      {
        for(int j = 0; j < LENGTH; ++j)
        {
          lset(i, j, SEEN, *Level);
          if(loc_statusp(i, j, SECRET, *Level))
          {
            lreset(i, j, SECRET, *Level);
            lset(i, j, CHANGED, *Level);
          }
        }
      }
      show_screen();
      break;
  }
}

void wake_statue(int x, int y, int first)
{
  if(Level->site[x][y].locchar == STATUE)
  {
    if(!first)
    {
      queue_message("Another statue awakens!");
    }
    else
    {
      queue_message("A statue springs to life!");
    }
    Level->site[x][y].locchar = FLOOR;
    lset(x, y, CHANGED, *Level);
    std::unique_ptr<monster> m = m_create(x, y, 0, difficulty() + 1);
    Level->site[x][y].creature = m.get();
    m_status_set(*m, HOSTILE);
    Level->mlist.emplace_front(std::move(m));
    for(int i = 0; i < 8; ++i)
    {
      wake_statue(x + Dirs[0][i], y + Dirs[1][i], false);
    }
  }
}

void l_statue_wake()
{
  int x = Player.x, y = Player.y;
  for(int i = 0; i < 9; ++i)
  {
    wake_statue(x + Dirs[0][i], y + Dirs[1][i], true);
  }
}

void l_casino()
{
  queue_message("Rampart Mithril Nugget Casino.");
  if(random_range(10) == 1)
  {
    append_message("Casino closed due to Grand Jury investigation.", true);
  }
  else
  {
    std::vector<std::string> lines;
    lines.emplace_back("     Rampart Mithril Nugget Casino.");
    lines.emplace_back("a: Drop 100Au in the slots.");
    lines.emplace_back("b: Risk 1000Au at roulette.");
    lines.emplace_back("ESCAPE: Leave this green baize hall.");
    menu->load(lines);
    for(bool done = false; !done;)
    {
      switch(menu->get_player_input())
      {
        case 'a':
          if(Player.cash < 100)
          {
            append_message("No credit, jerk.", true);
          }
          else
          {
            Player.cash -= 100;
            dataprint();
            print_messages();
            const std::string REEL_VALUES[] = {
              "<Slime Mold>", "<Lemon>", "<Copper>", "<Nymph>", "<Sword>",
              "<Shield>",     "<Chest>", "<Bar>",    "<Orb>",   "<Mithril Nugget>",
            };
            append_message("", true);
            int a, b, c;
            for(int i = 0; i < 20; ++i)
            {
              doupdate();
              if(i == 19)
              {
                std::this_thread::sleep_for(std::chrono::seconds(1));
              }
              else
              {
                std::this_thread::sleep_for(std::chrono::milliseconds(250));
              }
              a = random_range(10);
              b = random_range(10);
              c = random_range(10);
              message_buffer.replace_last(REEL_VALUES[a] + " " + REEL_VALUES[b] + " " + REEL_VALUES[c]);
              print_messages();
            }
            if(winnings > 0)
            {
              do
              {
                a = random_range(10);
                b = random_range(10);
                c = random_range(10);
              } while((a == b) || (a == c) || (b == c));
            }
            else
            {
              a = random_range(10);
              b = random_range(10);
              c = random_range(10);
            }
            message_buffer.replace_last(REEL_VALUES[a] + " " + REEL_VALUES[b] + " " + REEL_VALUES[c]);
            print_messages();
            if((a == b) && (a == c))
            {
              append_message("Jackpot Winner!", true);
              long reward = (a + 2) * (b + 2) * (c + 2) * 5;
              winnings += reward;
              Player.cash += reward;
              dataprint();
            }
            else if(a == b)
            {
              append_message("Winner!", true);
              long reward = (a + 2) * (b + 2) * 5;
              winnings += reward;
              Player.cash += reward;
              dataprint();
            }
            else if(a == c)
            {
              append_message("Winner!", true);
              long reward = (a + 2) * (c + 2) * 5;
              winnings += reward;
              Player.cash += reward;
              dataprint();
            }
            else if(c == b)
            {
              append_message("Winner!", true);
              long reward = (c + 2) * (b + 2) * 5;
              winnings += reward;
              Player.cash += reward;
              dataprint();
            }
            else
            {
              append_message("Loser!", true);
              winnings -= 100;
            }
          }
          break;
        case 'b':
          if(Player.cash < 1000)
          {
            append_message("No credit, jerk.", true);
          }
          else
          {
            Player.cash -= 1000;
            dataprint();
            append_message("Red or Black? [rb] ", true);
            int response;
            do
            {
              response = static_cast<char>(mcigetc());
            } while((response != 'r') && (response != 'b'));
            int match = (response == 'r' ? 0 : 1);
            append_message("", true);
            int a, b;
            for(int i = 0; i < 20; ++i)
            {
              doupdate();
              if(i == 19)
              {
                std::this_thread::sleep_for(std::chrono::seconds(1));
              }
              else
              {
                std::this_thread::sleep_for(std::chrono::milliseconds(250));
              }
              a = random_range(37);
              b = a % 2;
              if(a == 0)
              {
                message_buffer.replace_last("0 ");
              }
              else if(a == 1)
              {
                message_buffer.replace_last("0 - 0 ");
              }
              else
              {
                message_buffer.replace_last((b == 0 ? "Red " : "Black ") + std::to_string(a - 1));
              }
              print_messages();
            }
            if(winnings > 0)
            {
              do
              {
                a = random_range(37);
                b = a % 2;
              } while(b == match);
            }
            else
            {
              a = random_range(37);
              b = a % 2;
            }
            if(a == 0)
            {
              message_buffer.replace_last("0 ");
            }
            else if(a == 1)
            {
              message_buffer.replace_last("0 - 0 ");
            }
            else
            {
              message_buffer.replace_last((b == 0 ? "Red " : "Black ") + std::to_string(a - 1));
            }
            print_messages();
            if((a > 1) && (b == match))
            {
              append_message("Winner!", true);
              winnings += 1000;
              Player.cash += 2000;
              dataprint();
            }
            else
            {
              append_message("Loser!", true);
              winnings -= 1000;
              dataprint();
            }
          }
          break;
        case ESCAPE:
          done = true;
          break;
      }
    }
  }
}

void l_commandant()
{
  int num;
  queue_message("Commandant Sonder's Rampart-fried Lyzzard partes. Open 24 hrs.");
  append_message("Buy a bucket! Only 5 Au. Make a purchase? [yn] ", true);
  if(ynq() == 'y')
  {
    append_message("How many? ", true);
    num = (int)parsenum();
    if(num < 1)
    {
      queue_message("Cute. Real cute.");
    }
    else if(num * 5 > Player.cash)
    {
      queue_message("No handouts here, mac!");
    }
    else
    {
      Player.cash -= num * 5;
      auto food    = std::make_unique<object>(Objects[FOODID + 0]); // food ration
      food->number = num;
      if(num == 1)
      {
        append_message("There you go, mac! One Lyzzard Bucket, coming up.", true);
      }
      else
      {
        append_message("A passel of Lyzzard Buckets, for your pleasure.", true);
      }
      gain_item(std::move(food));
    }
  }
  else
  {
    append_message("Don't blame the Commandant if you starve!", true);
  }
}

void l_diner()
{
  queue_message("The Rampart Diner. All you can eat, 25Au.");
  queue_message("Place an order? [yn] ");
  if(ynq() == 'y')
  {
    if(Player.cash < 25)
    {
      queue_message("TANSTAAFL! Now git!");
    }
    else
    {
      Player.cash -= 25;
      dataprint();
      Player.food = 44;
      foodcheck();
    }
  }
}

void l_crap()
{
  queue_message("Les Crapeuleaux. (****) ");
  if((hour() < 17) || (hour() > 23))
  {
    queue_message("So sorry, we are closed 'til the morrow...");
  }
  else
  {
    queue_message("May I take your order? [yn] ");
    if(ynq() == 'y')
    {
      if(Player.cash < 1000)
      {
        queue_message("So sorry, you have not the funds for dinner.");
      }
      else
      {
        queue_message("Hope you enjoyed your tres expensive meal, m'sieur...");
        Player.cash -= 1000;
        dataprint();
        Player.food += 8;
        foodcheck();
      }
    }
  }
}

void l_tavern()
{
#define hinthour tavern_hinthour
  char response;
  queue_message("The Centaur and Nymph -- J. Riley, prop.");
  if(nighttime())
  {
    std::vector<std::string> lines = {
      {"Riley says: Whataya have?"},
      {""},
      {"a: Pint of Riley's ultra-dark 1Au"},
      {"b: Shot of Tullimore Dew 10Au"},
      {"c: Round for the House. 100Au"},
      {"d: Bed and Breakfast. 25Au"},
      {"ESCAPE: Leave this comfortable haven."}};
    menu->load(lines);
    menu->print();
    do
    {
      response = (char)mcigetc();
    } while((response != 'a') && (response != 'b') && (response != 'c') && (response != 'd') &&
            (response != ESCAPE));
    switch(response)
    {
      case 'a':
        if(Player.cash < 1)
        {
          queue_message("Aw hell, have one on me.");
        }
        else
        {
          Player.cash -= 1;
          dataprint();
          if(hinthour != hour())
          {
            if(random_range(3))
            {
              queue_message("You overhear a rumor...");
              hint();
            }
            else
            {
              queue_message("You don't hear much of interest.");
            }
            hinthour = hour();
          }
          else
          {
            queue_message("You just hear the same conversations again.");
          }
        }
        break;
      case 'b':
        if(Player.cash < 10)
        {
          queue_message("I don't serve the Dew on no tab, buddy!");
        }
        else
        {
          Player.cash -= 10;
          queue_message("Ahhhhh....");
          if(Player.status[POISONED] || Player.status[DISEASED])
          {
            queue_message("Phew! That's, er, smooth stuff!");
          }
          Player.status[POISONED] = 0;
          Player.status[DISEASED] = 0;
          showflags();
        }
        break;
      case 'c':
        if(Player.cash < 100)
        {
          queue_message("Whatta feeb!");
          queue_message("Outta my establishment.... Now!");
          p_damage(random_range(20), UNSTOPPABLE, "Riley's right cross");
        }
        else
        {
          Player.cash -= 100;
          dataprint();
          queue_message("'What a guy!'");
          queue_message("'Hey, thanks, fella.'");
          queue_message("'Make mine a double...'");
          switch(random_range(4))
          {
            case 0:
              if(Player.alignment < 0 && !CitySiteList[L_THIEVES_GUILD - CITYSITEBASE][0])
              {
                queue_message("A shadowy figure nods his head at you and hands you a note.");
                queue_message("It contains directions to the thieves' guild");
                CitySiteList[L_THIEVES_GUILD - CITYSITEBASE][0] = true;
              }
              else
              {
                queue_message("'You're a real pal. Say, have you heard.... ");
                hint();
              }
              break;
            case 1:
              queue_message("A wandering priest of Dionysus blesses you...");
              if((Player.patron == ODIN) || (Player.patron == ATHENA))
              {
                Player.alignment++;
              }
              else if((Player.patron == HECATE) || (Player.patron == SET))
              {
                Player.alignment--;
              }
              else if(Player.alignment > 0)
              {
                Player.alignment--;
              }
              else
              {
                Player.alignment++;
              }
              break;
            case 2:
              queue_message("A thirsty bard promises to put your name in a song!");
              gain_experience(20);
              break;
            case 3:
              queue_message("Riley draws you a shot of his 'special reserve'");
              queue_message("Drink it [yn]?");
              if(ynq() == 'y')
              {
                if(Player.con < random_range(20))
                {
                  queue_message("<cough> Quite a kick!");
                  queue_message("You feel a fiery warmth in your tummy....");
                  Player.con++;
                  Player.maxcon++;
                }
                else
                {
                  queue_message("You toss it back nonchalantly.");
                }
              }
          }
        }
        break;
      case 'd':
        if(Player.cash < 25)
        {
          queue_message("Pay in advance, mac!");
        }
        else
        {
          Player.cash -= 25;
          queue_message("How about a shot o' the dew for a nightcap?");
          Time += (6 + random_range(4)) * 60;
          Player.status[POISONED] = 0;
          Player.status[DISEASED] = 0;
          Player.food             = 40;
          // reduce temporary stat gains to max stat levels
          toggle_item_use(true);
          Player.str = std::min(Player.str, Player.maxstr);
          Player.con = std::min(Player.con, Player.maxcon);
          Player.agi = std::min(Player.agi, Player.maxagi);
          Player.dex = std::min(Player.dex, Player.maxdex);
          Player.iq  = std::min(Player.iq, Player.maxiq);
          Player.pow = std::min(Player.pow, Player.maxpow);
          toggle_item_use(false);
          timeprint();
          dataprint();
          showflags();
          queue_message("The next day.....");
          if(hour() > 10)
          {
            queue_message("Oh my! You overslept!");
          }
        }
        break;
      default:
        queue_message("So? Just looking? Go on!");
        break;
    }
  }
  else
  {
    queue_message("The pub don't open til dark, fella.");
  }
  xredraw();
}

#undef hinthour

void l_alchemist()
{
  queue_message("Ambrosias' Potions et cie.");
  if(nighttime())
  {
    queue_message("Ambrosias doesn't seem to be in right now.");
  }
  else
  {
    std::vector<std::string> lines;
    lines.emplace_back("     Ambrosias' Potions et cie.");
    lines.emplace_back("a: Sell monster components.");
    lines.emplace_back("b: Pay for transformation.");
    lines.emplace_back("ESCAPE: Leave this place.");
    menu->load(lines);
    for(bool done = false; !done;)
    {
      int slot;
      switch(menu->get_player_input())
      {
        case 'a':
          done = true;
          slot = getitem(CORPSE);
          if(slot != ABORT && Player.possessions[slot])
          {
            std::unique_ptr<object> &obj = Player.possessions[slot];
            if(Monsters[obj->charge].transformid == -1)
            {
              queue_message("I don't want such a thing.");
              if(obj->basevalue > 0)
              {
                queue_message("You might be able to sell it to someone else, though.");
              }
            }
            else
            {
              long sell_price = obj->basevalue / 3;
              queue_message(std::format("I'll give you {}Au each. Take it? [yn]", sell_price));
              if(ynq() == 'y')
              {
                int n = getnumber(obj->number);
                Player.cash += sell_price * n;
                dispose_lost_objects(n, slot);
              }
              else
              {
                queue_message("Well, keep the smelly old thing, then!");
              }
            }
          }
          else
          {
            queue_message("So nu?");
          }
          break;
        case 'b':
          done = true;
          slot = getitem(CORPSE);
          if(slot != ABORT && Player.possessions[slot])
          {
            object *obj = Player.possessions[slot].get();
            if(Monsters[obj->charge].transformid == -1)
            {
              queue_message("Oy vey! You want me to transform such a thing?");
            }
            else if(obj->number > 1 && Objects[Monsters[obj->charge].transformid].objchar == STICK)
            {
              queue_message("I can only work with one of these at a time.");
            }
            else
            {
              int mlevel           = Monsters[obj->charge].level;
              long transform_price = std::max(10l, obj->basevalue * 2 * obj->number);
              queue_message(std::format(
                "It'll cost you {} Au for the transformation. Pay it? [yn] ", transform_price
              ));
              if(ynq() == 'y')
              {
                if(Player.cash < transform_price)
                {
                  queue_message("You can't afford it!");
                }
                else
                {
                  queue_message("Voila! A tap of the Philosopher's Stone...");
                  Player.cash -= transform_price;
                  int n       = obj->number;
                  *obj        = Objects[Monsters[obj->charge].transformid];
                  obj->number = n;
                  if((obj->id >= STICKID) && (obj->id < STICKID + NUMSTICKS))
                  {
                    obj->charge = 20;
                  }
                  if(obj->plus == 0)
                  {
                    obj->plus = mlevel;
                  }
                  if(obj->blessing == 0)
                  {
                    obj->blessing = 1;
                  }
                }
              }
              else
              {
                queue_message("I don't need your business, anyhow.");
              }
            }
          }
          else
          {
            queue_message("So nu?");
          }
          break;
        case ESCAPE:
          done = true;
          break;
        default:
          break;
      }
    }
  }
}

void l_dpw()
{
  queue_message("Rampart Department of Public Works.");
  if(Date - LastDay < 7)
  {
    append_message("G'wan! Get a job!", true);
  }
  else if(Player.cash < 100)
  {
    append_message("Do you want to go on the dole? [yn] ", true);
    if(ynq() == 'y')
    {
      append_message("Well, ok, but spend it wisely.", true);
      append_message("Please enter your name for our records:");
      std::string name = msgscanstring();
      name[0] = std::toupper(name[0]);
      if(name.empty())
      {
        append_message("Maybe you should come back when you've learned to write.", true);
      }
      else if(name != Player.name)
      {
        append_message("Aha! Welfare Fraud! It's off to gaol for you, lout!", true);
        send_to_jail();
      }
      else
      {
        append_message("Here's your handout, layabout!", true);
        LastDay     = Date;
        Player.cash = 99;
        dataprint();
      }
    }
  }
  else
  {
    queue_message("You're too well off for us to help you!");
  }
}

void l_library()
{
  queue_message("Rampart Public Library.");
  if(nighttime())
  {
    queue_message("CLOSED");
  }
  else
  {
    queue_message("Library Research Fee: 1000Au.", true);
    int fee = 1000;
    if(Player.maxiq < 18)
    {
      queue_message("The Rampart student aid system has arranged a grant!", true);
      fee = std::max(50, 1000 - (18 - Player.maxiq) * 125);
      queue_message(std::format("Your revised fee is: {}Au.", fee), true);
    }
    bool studied = false, done = false;
    while(!done)
    {
      queue_message("Pay the fee? [yn] ", true);
      if(ynq() == 'y')
      {
        if(Player.cash < fee)
        {
          queue_message("No payee, No studee.", true);
          done = true;
        }
        else
        {
          Player.cash -= fee;
          do
          {
            studied = true;
            dataprint();
            std::vector<std::string> lines = {
              {"Peruse a scroll:"},
              {"a: Omegan Theology"},
              {"b: Guide to Rampart"},
              {"c: High Magick"},
              {"d: Odd Uncatalogued Document"},
              {"e: Attempt Advanced Research"},
              {"ESCAPE: Leave this font of learning."}};
            menu->load(lines);
            menu->print();
            char response = (char)mcigetc();
            if(response == 'a')
            {
              queue_message("You unfurl an ancient, yellowing scroll...", true);
              more_wait();
              theologyfile();
            }
            else if(response == 'b')
            {
              queue_message("You unroll a slick four-color document...", true);
              more_wait();
              cityguidefile();
            }
            else if(response == 'c')
            {
              queue_message("This scroll is written in a strange magical script...", true);
              more_wait();
              wishfile();
            }
            else if(response == 'd')
            {
              queue_message("You find a strange document, obviously misfiled", true);
              queue_message("under the heading 'acrylic fungus painting technique'");
              more_wait();
              adeptfile();
            }
            else if(response == 'e')
            {
              if(random_range(30) > Player.iq)
              {
                queue_message("You feel more knowledgeable!", true);
                Player.iq++;
                Player.maxiq++;
                dataprint();
                if(Player.maxiq < 19 && fee != std::max(50, 1000 - (18 - Player.maxiq) * 125))
                {
                  fee = std::max(50, 1000 - (18 - Player.maxiq) * 125);
                  queue_message(std::format("Your revised fee is: {}Au.", fee), true);
                }
              }
              else
              {
                queue_message("You find advice in an ancient tome: ", true);
                hint();
              }
            }
            else if(response == ESCAPE)
            {
              done = true;
              queue_message("That was an expensive browse...", true);
            }
            else
            {
              studied = false;
            }
          } while(!studied);
        }
        xredraw();
      }
      else
      {
        done = true;
        if(studied)
        {
          queue_message("Come back anytime we're open, 7am to 8pm.", true);
        }
        else
        {
          queue_message("You philistine!", true);
        }
      }
    }
  }
}

void l_pawn_shop()
{
  int number;
  bool done = false;

  if(nighttime())
  {
    queue_message("Shop Closed: Have a Nice (K)Night");
  }
  else
  {
    int limit    = std::min(5, Date - Pawndate);
    Pawndate = Date;
    for(int k = 0; k < limit; ++k)
    {
      if(Pawnitems[0])
      {
        if(Objects[Pawnitems[0]->id].uniqueness > UNIQUE_UNMADE)
        {
          Objects[Pawnitems[0]->id].uniqueness = UNIQUE_UNMADE;
        }
        // could turn up anywhere, really :)
      }
      for(int i = 0; i < PAWNITEMS - 1; ++i)
      {
        Pawnitems[i] = std::move(Pawnitems[i + 1]);
      }
      for(int i = 0; i < PAWNITEMS; ++i)
      {
        if(!Pawnitems[i])
        {
          do
          {
            Pawnitems[i]        = create_object(5);
            Pawnitems[i]->known = 2;
          } while(Pawnitems[i]->objchar == CASH || Pawnitems[i]->objchar == ARTIFACT ||
                  true_item_value(Pawnitems[i].get()) <= 0);
        }
      }
    }
    queue_message("Knight's Pawn Shop:");
    while(!done)
    {
      append_message("Buy item, Sell item, sell Pack contents, Leave [b,s,p,ESCAPE] ", true);
      std::vector<std::string> lines;
      for(int i = 0; i < PAWNITEMS; ++i)
      {
        if(Pawnitems[i])
        {
          lines.emplace_back(std::format("{}:{}", static_cast<char>(i + 'a'), itemid(Pawnitems[i].get())));
        }
      }
      menu->load(lines);
      int player_input = menu->get_player_input();
      if(player_input == ESCAPE)
      {
        done = true;
      }
      else if(player_input == 'b')
      {
        append_message("Purchase which item? [ESCAPE to quit] ", true);
        player_input = ' ';
        while(player_input != ESCAPE && (player_input < 'a' || player_input >= 'a' + PAWNITEMS))
        {
          player_input = menu->get_player_input();
        }
        if(player_input != ESCAPE)
        {
          int i = player_input - 'a';
          if(!Pawnitems[i])
          {
            append_message("No such item!", true);
          }
          else if(true_item_value(Pawnitems[i].get()) <= 0)
          {
            append_message("Hmm, how did that junk get on my shelves?", true);
            append_message("I'll just remove it.");
            Pawnitems[i].reset();
          }
          else
          {
            long price = Pawnitems[i]->number * true_item_value(Pawnitems[i].get());
            append_message(std::format("The low, low, cost is: {}Au. Buy it? [ynq] ", price), true);
            if(ynq() == 'y')
            {
              if(Player.cash < price)
              {
                append_message("No credit! Gwan, Beat it!", true);
              }
              else
              {
                Player.cash -= price;
                Objects[Pawnitems[i]->id].known = 1;
                gain_item(std::move(Pawnitems[i]));
              }
            }
          }
        }
      }
      else if(player_input == 's')
      {
        menuclear();
        queue_message("Sell which item: ");
        int slot = getitem(NULL_ITEM);
        if(slot != ABORT && Player.possessions[slot])
        {
          if(cursed(Player.possessions[slot].get()))
          {
            queue_message("No loans on cursed items! I been burned before....");
          }
          else if(true_item_value(Player.possessions[slot].get()) <= 0)
          {
            queue_message("That looks like a worthless piece of junk to me.");
          }
          else
          {
            long price = item_value(Player.possessions[slot].get()) / 2;
            queue_message(std::format("You can get {} Au each. Sell? [yn] ", price));
            if(ynq() == 'y')
            {
              number = getnumber(Player.possessions[slot]->number);
              if((number >= Player.possessions[slot]->number) && Player.possessions[slot]->used)
              {
                item_unequip(Player.possessions[slot]);
              }
              Player.cash += number * price;
              for(int i = 0; i < PAWNITEMS - 1; ++i)
              {
                Pawnitems[i] = std::move(Pawnitems[i + 1]);
              }
              Pawnitems[PAWNITEMS - 1]         = std::make_unique<object>(*Player.possessions[slot]);
              Pawnitems[PAWNITEMS - 1]->number = number;
              Pawnitems[PAWNITEMS - 1]->known  = 2;
              dispose_lost_objects(number, slot);
              dataprint();
            }
          }
        }
      }
      else if(player_input == 'p')
      {
        for(size_t i = Player.pack.size(); --i > 0;)
        {
          std::unique_ptr<object> &item = Player.pack[i];
          if(item->blessing > -1 && true_item_value(item.get()) > 0)
          {
            long price = item_value(item.get()) / 2;
            queue_message(std::format("Sell {} for {} Au each? [ynq] ", itemid(item.get()), price));
            player_input = ynq();
            if(player_input == 'y')
            {
              number = getnumber(item->number);
              if(number > 0)
              {
                Player.cash += number * price;
                for(int j = 0; j < PAWNITEMS - 1; ++j)
                {
                  Pawnitems[j] = std::move(Pawnitems[j + 1]);
                }
                Pawnitems[PAWNITEMS - 1]         = std::make_unique<object>(*item);
                Pawnitems[PAWNITEMS - 1]->number = number;
                Pawnitems[PAWNITEMS - 1]->known  = 2;
                item->number -= number;
                if(item->number < 1)
                {
                  Player.pack.erase(Player.pack.begin() + i);
                }
                dataprint();
              }
            }
            else if(player_input == 'q')
            {
              break;
            }
          }
        }
      }
    }
  }
  calc_melee();
  xredraw();
}
