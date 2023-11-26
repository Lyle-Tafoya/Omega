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

// mtalk.cpp
// monster talk functions

#include "glob.h"
#include "scr.h"

#include <algorithm>
#include <format>

extern bool received_directions;

// The druid's altar is in the northern forest
void m_talk_druid(monster *m)
{
  if(!m_statusp(*m, HOSTILE))
  {
    queue_message("The Archdruid raises a hand in greeting.");
    if(!gamestatusp(SPOKE_TO_DRUID, GameStatus))
    {
      setgamestatus(SPOKE_TO_DRUID, GameStatus);
      queue_message("The Archdruid congratulates you on reaching his sanctum.");
      queue_message("You feel competent.");
      gain_experience(300);
      if(Player.patron == DRUID)
      {
        queue_message("The Archdruid conveys to you the wisdom of nature....");
        queue_message("You feel like a sage.");
        for(int i = 0; i < NUMRANKS; ++i)
        {
          if(Player.guildxp[i] > 0)
          {
            Player.guildxp[i] += 300;
          }
        }
      }
    }
    queue_message("Do you request a ritual of neutralization? [yn] ");
    if(ynq() == 'y')
    {
      if(Phase / 2 == 6 || Phase / 2 == 0)
      { // full or new moon
        queue_message("\"Unfortunately, I cannot perform a ritual of balance on");
        if(Phase / 2 == 6)
        {
          queue_message("this lawful day.\"");
        }
        else
        {
          queue_message("this chaotic day.\"");
        }
      }
      else if(Phase / 2 == 3 || Phase / 2 == 9)
      { // half moon
        queue_message("You take part in today's holy celebration of balance...");
        Player.alignment = 0;
        Player.mana      = calcmana();
        if(Player.patron == DRUID)
        {
          gain_experience(200); // if a druid wants to spend 2 days
        }
        Time += 60; // celebrating for 1600 xp, why not?
        hourly_check();
        Time += 60;
        hourly_check();
        Time += 60;
        hourly_check();
        Time += 60;
        hourly_check();
        Time += 60;
        hourly_check();
        Time += 60;
        hourly_check();
      }
      else
      {
        queue_message("The ArchDruid conducts a sacred rite of balance...");
        if(Player.patron == DRUID)
        {
          Player.alignment = 0;
          Player.mana      = calcmana();
        }
        else
        {
          Player.alignment -= Player.alignment * std::max(0, 10 - Player.level) / 10;
        }
        // the higher level the character is, the more set in his/her ways
        Time += 60;
        hourly_check();
      }
      dataprint();
    }
  }
  else
  {
    queue_message("The ArchDruid looks at you and cries: 'Unclean! Unclean!'");
    disrupt(Player.x, Player.y, 100);
    queue_message("This seems to have satiated his desire for vengeance.");
    queue_message("'Have you learned your lesson?' The ArchDruid asks. [yn] ");
    if(ynq())
    {
      queue_message("'I certainly hope so!' says the ArchDruid.");
      for(std::unique_ptr<monster> &level_monster : Level->mlist)
      {
        m_status_reset(*level_monster, HOSTILE);
      }
      m_vanish(m);
    }
    else
    {
      queue_message("'Idiot.' mutters the ArchDruid.");
      p_damage(500, UNSTOPPABLE, "the ArchDruid's Vengeance");
    }
  }
}

void m_talk_silent(monster *m)
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
  switch(random_range(4))
  {
    case 0:
      queue_message(std::format("{} does not reply.", monster_name));
      break;
    case 1:
      queue_message(std::format("{} shrugs silently.", monster_name));
      break;
    case 2:
      queue_message(std::format("{} holds a finger to his mouth.", monster_name));
      break;
    case 3:
      queue_message(std::format("{} glares at you but says nothing.", monster_name));
      break;
  }
}

void m_talk_stupid(monster *m)
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
  switch(random_range(4))
  {
    case 0:
      queue_message(std::format("{} looks at you with mute incomprehension.", monster_name));
      break;
    case 1:
      queue_message(std::format("{} growls menacingly and ignores you.", monster_name));
      break;
    case 2:
      queue_message(std::format("{} does not seem to have heard you.", monster_name));
      break;
    case 3:
      queue_message(std::format("{} tries to pretend it didn't hear you.", monster_name));
      break;
  }
}

void m_talk_greedy(monster *m)
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
  switch(random_range(4))
  {
    case 0:
      queue_message(std::format("{} says: Give me a treasure....", monster_name));
      break;
    case 1:
      queue_message(std::format("{} says: Stand and deliver, knave!", monster_name));
      break;
    case 2:
      queue_message(std::format(" says: Your money or your life!", monster_name));
      break;
    case 3:
      queue_message(std::format("{} says: Yield or Die!", monster_name));
      break;
  }
}

void m_talk_hungry(monster *m)
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
  switch(random_range(4))
  {
    case 0:
      queue_message(std::format("{} says: I hunger, foolish adventurer!", monster_name));
      break;
    case 1:
      queue_message(std::format("{} drools menacingly at you.", monster_name));
      break;
    case 2:
      queue_message(std::format(" says: You're invited to be lunch!", monster_name));
      break;
    case 3:
      queue_message(std::format("{} says: Feeed Meee!", monster_name));
      break;
  }
}

void m_talk_guard(monster *m)
{
  if(m_statusp(*m, HOSTILE))
  {
    queue_message("'Surrender in the name of the Law!'");
    queue_message("Do it? [yn] ");
    if(ynq() == 'y')
    {
      Player.alignment++;
      if(Current_Environment == E_CITY)
      {
        queue_message("Go directly to jail. Do not pass go, do not collect 200Au.");
        queue_message("You are taken to the city gaol.");
        send_to_jail();
        drawvision(Player.x, Player.y);
      }
      else
      {
        queue_message("Mollified, the guard disarms you and sends you away.");
        dispose_lost_objects(1, Player.possessions[O_WEAPON_HAND]);
        pacify_guards();
      }
    }
    else
    {
      queue_message("All right, you criminal scum, you asked for it!");
    }
  }
  else if(!received_directions && Current_Environment == E_CITY)
  {
    queue_message("The guard offers to show you a map of Rampart.");
    queue_message("Look at the map? [yn]  ");
    if(ynq() == 'y')
    {
      for(int i = 0; i < NUMCITYSITES; ++i)
      {
        int site = CITYSITEBASE + i;
        if(site != L_THIEVES_GUILD && site != L_ORACLE && site != L_BROTHEL)
        {
          CitySiteList[i][0] = true;
        }
      }
      queue_message("You feel more knowledgeable!");
    }
    else
    {
      queue_message("Next time you need help, you can ask someone else!");
    }
    received_directions = true;
  }
  else if(Player.rank[ORDER] > 0)
  {
    queue_message("'Greetings comrade! May you always tread the paths of Law.'");
  }
  else
  {
    queue_message("Move it right along, stranger!");
  }
}

void m_talk_mp(monster *)
{
  queue_message("The mendicant priest asks you to spare some treasure for the needy");
}

void m_talk_titter(monster *m)
{
  if(m->uniqueness == COMMON)
  {
    queue_message(std::format("The {} titters obscenely at you.", m->monstring));
  }
  else
  {
    queue_message(std::format("{} titters obscenely at you.", m->monstring));
  }
}

void m_talk_ninja(monster *)
{
  queue_message("The black-garbed figure says apologetically:");
  queue_message("'Situree simasita, wakarimasen.'");
}

void m_talk_thief(monster *m)
{
  if(Player.rank[THIEVES])
  {
    if(m->level == 2)
    {
      m->monstring = "sneak thief";
    }
    else
    {
      m->monstring = "master thief";
    }
    queue_message("The cloaked figure makes a gesture which you recognize...");
    queue_message("...the thieves' guild recognition signal!");
    queue_message("'Sorry, mate, thought you were a mark....'");
    m_vanish(m);
  }
  else
  {
    m_talk_man(m);
  }
}

void m_talk_assassin(monster *m)
{
  m->monstring = "master assassin";
  queue_message("The ominous figure does not reply, but hands you an embossed card:");
  queue_message("'Guild of Assassins Ops are forbidden to converse with targets.'");
}

void m_talk_im(monster *m)
{
  if(m->monstring != "itinerant merchant")
  {
    m->monstring = "itinerant merchant";
  }
  if(m->possessions.empty())
  {
    queue_message("The merchant says: Alas! I have nothing to sell!");
  }
  else
  {
    m->possessions.front()->known = 2;
    long price                    = std::max(10l, 4 * true_item_value(m->possessions.front().get()));
    queue_message(std::format(
      "I have a fine {} for only {} Au. Want it? [yn] ", itemid(m->possessions.front().get()), price
    ));
    if(ynq() == 'y')
    {
      if(Player.cash < price)
      {
        if(Player.alignment > 10)
        {
          queue_message("Well, I'll let you have it for what you've got.");
          Player.cash = 0;
          gain_item(std::move(m->possessions.front()));
          m->possessions.pop_front();
        }
        else
        {
          queue_message("Beat it, you deadbeat!");
        }
      }
      else
      {
        queue_message("Here you are. Have a good day.");
        Player.cash -= price;
        gain_item(std::move(m->possessions.front()));
        m->possessions.pop_front();
      }
    }
    else
    {
      queue_message("Well then, I must be off. Good day.");
    }
    m_vanish(m);
  }
}

void m_talk_man(monster *m)
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
  switch(random_range(5))
  {
    case 0:
      queue_message(std::format("{} asks you for the way home.", monster_name));
      break;
    case 1:
      queue_message(std::format("{} wishes you a pleasant day.", monster_name));
      break;
    case 2:
      queue_message(std::format("{} sneers at you contemptuously.", monster_name));
      break;
    case 3:
      queue_message(std::format("{} smiles and nods.", monster_name));
      break;
    case 4:
      queue_message(std::format("{} tells you a joke.", monster_name));
      break;
  }
}

void m_talk_evil(monster *m)
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
  switch(random_range(14))
  {
    case 0:
      queue_message(std::format("{} says: 'THERE CAN BE ONLY ONE!'", monster_name));
      break;
    case 1:
      queue_message(std::format("{} says: 'Prepare to die, Buckwheat!'", monster_name));
      break;
    case 2:
      queue_message(std::format("{} says: 'Time to die!'", monster_name));
      break;
    case 3:
      queue_message(std::format("{} says: 'There will be no mercy.'", monster_name));
      break;
    case 4:
      queue_message(std::format("{} insults your mother-in-law.", monster_name));
      break;
    case 5:
      queue_message(std::format("{} says: 'Kurav tu ando mul!'", monster_name));
      break;
    case 6:
      queue_message(std::format("{} says: '!va al infierno!'", monster_name));
      break;
    case 7:
      queue_message(std::format("{} says: 'dame desu, nee.'", monster_name));
      break;
    case 8:
      queue_message(std::format("{} spits on your rug and calls your cat a bastard.", monster_name));
      break;
    case 9:
      queue_message(std::format("{} snickers malevolently and draws a weapon.", monster_name));
      break;
    case 10:
      queue_message(std::format("{} sends 'rm -r *' to your shell!", monster_name));
      break;
    case 11:
      queue_message(std::format("{} tweaks your nose and cackles evilly.", monster_name));
      break;
    case 12:
      queue_message(std::format("{} thumbs you in the eyes.", monster_name));
      break;
    case 13:
      queue_message(std::format("{} kicks you in the groin.", monster_name));
      break;
  }
}

void m_talk_robot(monster *m)
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
  switch(random_range(4))
  {
    case 0:
      queue_message(std::format("{} says: 'exterminate...Exterminate...EXTERMINATE!!!'", monster_name));
      break;
    case 1:
      queue_message(std::format("{} says: 'Kill ... Crush ... Destroy'", monster_name));
      break;
    case 2:
      queue_message(std::format("{} says: 'Danger -- Danger'", monster_name));
      break;
    case 3:
      queue_message(std::format("{} says: 'Yo Mama -- core dumped.'", monster_name));
      break;
  }
}

void m_talk_slithy(monster *)
{
  queue_message("It can't talk -- it's too slithy!");
}

void m_talk_mimsy(monster *)
{
  queue_message("It can't talk -- it's too mimsy!");
}

void m_talk_burble(monster *m)
{
  if(m->uniqueness == COMMON)
  {
    queue_message(std::format("The {} burbles hatefully at you.", m->monstring));
  }
  else
  {
    queue_message(std::format("{} burbles hatefully at you.", m->monstring));
  }
}

void m_talk_beg(monster *m)
{
  if(m->uniqueness == COMMON)
  {
    queue_message(std::format("The {} asks you for alms.", m->monstring));
  }
  else
  {
    queue_message(std::format("{} asks you for alms.", m->monstring));
  }
}

void m_talk_hint(monster *m)
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
  if(m_statusp(*m, HOSTILE))
  {
    queue_message(std::format("{} only sneers at you.", monster_name));
  }
  else
  {
    queue_message(std::format("{} whispers in your ear: ", monster_name));
    hint();
    m->talkf = M_TALK_SILENT;
  }
}

void m_talk_gf(monster *m)
{
  queue_message("The good fairy glints: Would you like a wish?");
  if(ynq() == 'y')
  {
    queue_message("The good fairy glows: Are you sure?");
    if(ynq() == 'y')
    {
      queue_message("The good fairy radiates: Really really sure?");
      if(ynq() == 'y')
      {
        queue_message("The good fairy beams: I mean, like, sure as sure can be?");
        if(ynq() == 'y')
        {
          queue_message("The good fairy dazzles: You don't want a wish, right?");
          if(ynq() == 'y')
          {
            queue_message("The good fairy laughs: I thought not.");
          }
          else
          {
            wish(0);
          }
        }
      }
    }
  }
  queue_message("In a flash of sweet-smelling light, the fairy vanishes....");
  Player.hp   = std::max(Player.hp, Player.maxhp);
  Player.mana = std::max(Player.mana, calcmana());
  queue_message("You feel mellow.");
  m_vanish(m);
}

void m_talk_ef(monster *m)
{
  queue_message("The evil fairy roils: Eat my pixie dust!");
  queue_message("She waves her black-glowing wand, which screams thinly....");
  m->movef    = M_MOVE_SMART;
  m->meleef   = M_MELEE_POISON;
  m->specialf = M_SP_THIEF;
  acquire(-1);
  bless(-1);
  sleep_player(m->level / 2);
  summon(-1, -1);
  summon(-1, -1);
  summon(-1, -1);
  summon(-1, -1);
}

void m_talk_seductor(monster *m)
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
  queue_message(std::format("{} beckons seductively", monster_name));
  queue_message("Flee? [yn] ");
  if(ynq() == 'y')
  {
    queue_message("You feel stupid.");
  }
  else
  {
    queue_message(std::format("{} shows you a good time....", monster_name));
    gain_experience(500);
    Player.con++;
  }
  m_vanish(m);
}

void m_talk_demonlover(monster *m)
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
  queue_message(std::format("{} beckons seductively", monster_name));
  queue_message("Flee? [yn] ");
  if(ynq() == 'y')
  {
    queue_message("You feel fortunate....");
  }
  else
  {
    queue_message(std::format("{} shows you a good time....", monster_name));
    queue_message("You feel your life energies draining...");
    level_drain(random_range(3) + 1, "a demon's kiss");
  }
  m->talkf    = M_TALK_EVIL;
  m->meleef   = M_MELEE_SPIRIT;
  m->specialf = M_SP_DEMON;

  if((m->monchar & 0xff) == 's')
  {
    m->monchar   = 'I' | CLR(RED);
    m->monstring = "incubus";
  }
  else
  {
    m->monchar   = 'S' | CLR(RED);
    m->monstring = "succubus";
  }
  queue_message(std::format("{} laughs insanely.", monster_name));
  queue_message("You now notice the fangs, claws, batwings...");
}

void m_talk_horse(monster *m)
{
  if(m_statusp(*m, HOSTILE))
  {
    queue_message("The horse neighs angrily at you.");
  }
  else if(m_statusp(*m, HUNGRY))
  {
    queue_message("The horse noses curiously at your pack.");
  }
  else if(gamestatusp(MOUNTED, GameStatus))
  {
    queue_message("The horse and your steed don't seem to get along.");
  }
  else if(Current_Environment == Current_Dungeon)
  {
    queue_message("The horse shies; maybe he doesn't like the dungeon air....");
  }
  else
  {
    queue_message("The horse lets you pat his nose. Want to ride him? [yn] ");
    if(ynq() == 'y')
    {
      m->hp                            = -1;
      Level->site[m->x][m->y].creature = nullptr;
      putspot(m->x, m->y, getspot(m->x, m->y, false));
      setgamestatus(MOUNTED, GameStatus);
      calc_melee();
      queue_message("You are now equitating!");
    }
  }
}

void m_talk_hyena(monster *)
{
  queue_message("The hyena only laughs at you...");
}

void m_talk_parrot(monster *)
{
  queue_message("Polly wanna cracker?");
}

void m_talk_servant(monster *m)
{
  int target, x = Player.x, y = Player.y;
  if(m->id == SERV_LAW)
  {
    target = SERV_CHAOS;
    queue_message("The Servant of Law pauses in thought for a moment.");
    queue_message("You are asked: Are there any Servants of Chaos hereabouts? [yn] ");
  }
  else
  {
    target = SERV_LAW;
    queue_message("The Servant of Chaos grins mischievously at you.");
    queue_message("You are asked: Are there any Servants of Law hereabouts? [yn] ");
  }
  if(ynq() == 'y')
  {
    queue_message("Show me.");
    show_screen();
    drawmonsters(true);
    setspot(x, y);
    if(Level->site[x][y].creature)
    {
      if(Level->site[x][y].creature->id == target)
      {
        queue_message("The Servant launches itself towards its enemy.");
        queue_message("In a blaze of combat, the Servants annihilate each other!");
        gain_experience(m->xpv);
        m_death(Level->site[x][y].creature);
        Level->site[m->x][m->y].creature = nullptr;
        m->x                             = x;
        m->y                             = y;
        Level->site[x][y].creature       = m;
        m_death(Level->site[x][y].creature);
      }
      else
      {
        queue_message("Right. Tell me about it. Idiot!");
      }
    }
    else
    {
      queue_message("Right. Tell me about it. Idiot!");
    }
  }
  else
  {
    queue_message("The servant shrugs and turns away.");
  }
}

void m_talk_animal(monster *m)
{
  if(m->uniqueness == COMMON)
  {
    queue_message(std::format("The {}", m->monstring));
  }
  else
  {
    queue_message(std::format("{}", m->monstring));
  }
  queue_message("shows you a scholarly paper by Dolittle, D. Vet.");
  queue_message("which demonstrates that animals don't have speech centers");
  queue_message("complex enough to communicate in higher languages.");
  queue_message("It giggles softly to itself and takes back the paper.");
}

void m_talk_scream(monster *m)
{
  queue_message("A thinly echoing scream reaches your ears....");
  queue_message("You feel doomed....");
  queue_message("A bird appears and flies three times widdershins around your head.");
  summon(-1, QUAIL);
  m->talkf = M_TALK_EVIL;
}

void m_talk_archmage(monster *m)
{
  if(m_statusp(*m, HOSTILE))
  {
    queue_message("The Archmage ignores your attempt at conversation");
    queue_message("and concentrates on his spellcasting....");
  }
  else if(Current_Environment == E_COURT)
  {
    queue_message("The Archmage congratulates you on getting this far.");
    queue_message("He invites you to attempt the Throne of High Magic");
    queue_message("but warns you that it is important to wield the Sceptre");
    queue_message("before sitting on the throne.");
    if(Level->site[m->x][m->y].p_locf == L_THRONE)
    {
      queue_message("The Archmage smiles and makes an arcane gesture....");
      m_vanish(m);
    }
  }
  else
  {
    queue_message("The Archmage tells you to find him again in his");
    queue_message("Magical Court at the base of his castle in the mountains");
    queue_message("of the far North-East; if you do he will give you some");
    queue_message("important information.");
  }
}

void m_talk_merchant(monster *m)
{
  if(!m_statusp(*m, HOSTILE))
  {
    if(Current_Environment == E_VILLAGE)
    {
      queue_message("The merchant asks you if you want to buy a horse for 250GP.");
      queue_message("Pay the merchant? [yn] ");
      if(ynq() == 'y')
      {
        if(Player.cash < 250)
        {
          queue_message("The merchant says: 'Come back when you've got the cash!'");
        }
        else
        {
          Player.cash -= 250;
          queue_message("The merchant takes your money and tells you to select");
          queue_message("any horse you want in the stables.");
          queue_message("He says: 'You'll want to get to know him before trying to");
          queue_message("ride him. By the way, food for the horse is not included.'");
          queue_message("The merchant runs off toward the bank, cackling gleefully.");
          m_vanish(m);
        }
      }
      else
      {
        queue_message("The merchant tells you to stop wasting his time.");
      }
    }
    else
    {
      queue_message("The merchant tells you to visit his stables at his village");
      queue_message("for a great deal on a horse.");
    }
  }
  else
  {
    queue_message("The merchant ignores you and screams:");
    queue_message("'Help! Help! I'm being oppressed!'");
  }
}

void m_talk_prime(monster *m)
{
  if(!m_statusp(*m, HOSTILE))
  {
    if(Current_Environment == E_CIRCLE)
    {
      queue_message("The Prime nods brusquely at you, removes a gem from his");
      queue_message("sleeve, places it on the floor, and vanishes wordlessly.");
      m_dropstuff(m);
      m_vanish(m);
    }
    else
    {
      queue_message("The Prime makes an intricate gesture, which leaves behind");
      queue_message("glowing blue sparks... He winks mischievously at you....");
      if(Player.rank[CIRCLE] > 0)
      {
        queue_message("The blue sparks strike you! You feel enhanced!");
        queue_message("You feel more experienced....");
        Player.pow += Player.rank[CIRCLE];
        Player.mana += calcmana();
        gain_experience(1000);
        m_vanish(m);
      }
    }
  }
  else
  {
    m_talk_evil(m);
  }
}
