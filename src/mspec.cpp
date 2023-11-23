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

/* mspec.c */
/* monster special functions */

#include "glob.h"
#include "scr.h"

#include <algorithm>
#include <format>

void icebolt(int fx, int fy, int tx, int ty, int hit, int dmg);

void m_sp_mp(monster *m)
{
  if(m->attacked && (random_range(3) == 1))
  {
    queue_message("You feel cursed!");
    p_damage(10, UNSTOPPABLE, "a mendicant priest's curse");
    m_vanish(m);
  }
  else if(!m_statusp(*m, NEEDY))
  {
    queue_message("The mendicant priest makes a mystical gesture....");
    queue_message("You feel impressed...");
    Player.alignment += 5;
    if(Player.alignment > 20)
    {
      Player.hp = std::max(Player.hp, Player.maxhp);
    }
    m_vanish(m);
  }
}

void m_sp_ng(monster *m)
{
  if(distance(m->x, m->y, Player.x, Player.y) < 2)
  {
    if((random_range(5) == 1) || (Player.status[VULNERABLE] > 0))
    {
      queue_message("The night gaunt grabs you and carries you off!");
      queue_message("Its leathery wings flap and flap, and it giggles insanely.");
      queue_message("It tickles you cunningly to render you incapable of escape.");
      queue_message("Finally, it deposits you in a strange place.");
      p_teleport(0);
    }
  }
}

void m_sp_poison_cloud(monster *m)
{
  if(distance(m->x, m->y, Player.x, Player.y) < 3)
  {
    queue_message("A cloud of poison gas surrounds you!");
    if(Player.status[BREATHING] > 0)
    {
      queue_message("You can breathe freely, however.");
    }
    else
    {
      p_poison(7);
    }
  }
}

void m_sp_explode(monster *m)
{
  if((distance(Player.x, Player.y, m->x, m->y) < 2) && (m->hp > 0) && (m->hp < Monsters[m->id].hp))
  {
    fball(m->x, m->y, m->x, m->y, m->hp);
  }
}

void m_sp_demon(monster *m)
{
  int mid;

  if(random_range(2))
  {
    if((m->id != INCUBUS) && /*succubi don't give fear */
       los_p(m->x, m->y, Player.x, Player.y) && (random_range(30) > Player.level + 10) &&
       (Player.status[AFRAID] == 0))
    {
      queue_message("You are stricken with fear!");
      if(!p_immune(FEAR))
      {
        Player.status[AFRAID] += m->level;
      }
      else
      {
        queue_message("You master your reptile brain and stand fast.");
      }
    }
    else
    {
      m_sp_spell(m);
    }
  }
  if((m->hp < (m->level * 5)) && (m->hp > 1))
  {
    queue_message("The demon uses its waning lifeforce to summon help!");
    m->hp = 1;
    switch(m->level)
    {
      case 3:
        mid = NIGHT_GAUNT;
        break;
      case 4:
      case 5:
        mid = L_FDEMON;
        break; /* lesser frost demon */
      case 6:
        mid = FROST_DEMON;
        break;
      case 7:
        mid = OUTER_DEMON;
        break; /* outer circle demon */
      case 8:
        mid = DEMON_SERP;
        break; /* demon serpent */
      default:
        mid = INNER_DEMON;
        break; /* inner circle demon */
    }
    summon(-1, mid);
    summon(-1, mid);
  }
}

void m_sp_acid_cloud(monster *m)
{
  if(m_statusp(*m, HOSTILE) && (distance(m->x, m->y, Player.x, Player.y) < 3))
  {
    acid_cloud();
  }
}

void m_sp_escape(monster *m)
{
  if(m_statusp(*m, HOSTILE))
  {
    m_vanish(m);
  }
}

void m_sp_ghost(monster *m)
{
  if(m_statusp(*m, HOSTILE))
  {
    queue_message("The ghost moans horribly....");
    p_damage(1, FEAR, "a ghost-inspired heart attack");
    queue_message("You've been terrorized!");
    if(!p_immune(FEAR))
    {
      Player.status[AFRAID] += m->level;
    }
    else
    {
      queue_message("You master your reptile brain and stand fast.");
    }
  }
}

/* random spell cast by monster */
void m_sp_spell(monster *m)
{
  if(m_statusp(*m, HOSTILE) && los_p(Player.x, Player.y, m->x, m->y))
  {
    if(m->uniqueness == COMMON)
    {
      queue_message(std::format("The {} casts a spell...", m->monstring));
    }
    else
    {
      queue_message(std::format("{} casts a spell...", m->monstring));
    }
    if(!magic_resist(m->level))
    {
      switch(random_range(m->level + 7))
      {
        case 0:
          nbolt(m->x, m->y, Player.x, Player.y, m->hit, 10);
          break;
        case 1:
          queue_message("It seems stronger...");
          m->hp += random_range(m->level * m->level);
          break;
        case 2:
          haste(-1);
          break;
        case 3:
          cure(-1);
          break;
        case 4:
          /* WDT: I'd like to make this (and "case 5" below) dependant on
         * the monster's IQ in some way -- dumb but powerful monsters
         * deserve what they get :).  No rush. */
          if(m_immunityp(*m, ELECTRICITY) || distance(m->x, m->y, Player.x, Player.y) > 2)
          {
            lball(m->x, m->y, Player.x, Player.y, 20);
          }
          else
          {
            lbolt(m->x, m->y, Player.x, Player.y, m->hit, 20);
          }
          break;
        case 5:
          if(m_immunityp(*m, COLD) || distance(m->x, m->y, Player.x, Player.y) > 2)
          {
            snowball(m->x, m->y, Player.x, Player.y, 30);
          }
          else
          {
            icebolt(m->x, m->y, Player.x, Player.y, m->hit, 30);
          }
          break;
        case 6:
          enchant(-1);
          break;
        case 7:
          bless(0 - m->level);
          break;
        case 8:
          p_poison(m->level);
          break;
        case 9:
          sleep_player(m->level / 2);
          break;
        case 10:
          fbolt(m->x, m->y, Player.x, Player.y, m->hit * 3, 50);
          break;
        case 11:
          acquire(0 - m->level);
          break;
        case 12:
          dispel(-1);
          break;
        case 13:
          disrupt(Player.x, Player.y, 50);
          break;
        case 14:
          if(m->uniqueness == COMMON)
          {
            level_drain(m->level, std::format("a {}", m->monstring).c_str());
          }
          else
          {
            level_drain(m->level, m->monstring);
          }
          break;
        case 15:
        case 16:
          disintegrate(Player.x, Player.y);
          break;
      }
    }
  }
}

/* monsters with this have some way to hide, camouflage, etc until they
   attack */
void m_sp_surprise(monster *m)
{
  if(m->attacked)
  {
    if(m_statusp(*m, HOSTILE) && (!Player.status[TRUESIGHT]) && m_statusp(*m, M_INVISIBLE))
    {
      m->monchar = Monsters[m->id].monchar;
      if(!Player.status[ALERT])
      {
        switch(random_range(4))
        {
          case 0:
            queue_message("You are surprised by a sudden treacherous attack!");
            break;
          case 1:
            queue_message("You are shocked out of your reverie by the scream of battle!");
            break;
          case 2:
            queue_message("Suddenly, from out of the shadows, a surprise attack!");
            break;
          case 3:
            queue_message("A shriek of hatred causes you to momentarily freeze up!");
            break;
        }
        setgamestatus(SKIP_PLAYER, GameStatus);
        m_status_reset(*m, M_INVISIBLE);
      }
      else
      {
        queue_message("You alertly sense the presence of an attacker!");
        m_status_reset(*m, M_INVISIBLE);
      }
    }
  }
}

void m_sp_whistleblower(monster *m)
{
  if(m_statusp(*m, HOSTILE))
  {
    alert_guards();
    m->specialf = M_MELEE_NORMAL;
  }
}

void m_sp_seductor(monster *m)
{
  if(m_statusp(*m, HOSTILE))
  {
    if(m->uniqueness == COMMON)
    {
      queue_message(std::format("The {} runs away screaming for help....", m->monstring));
    }
    else
    {
      queue_message(std::format("{} runs away screaming for help....", m->monstring));
    }
    m_vanish(m);
    summon(-1, -1);
    summon(-1, -1);
    summon(-1, -1);
  }
  else if(distance(Player.x, Player.y, m->x, m->y) < 2)
  {
    m_talk_seductor(m);
  }
}

void m_sp_demonlover(monster *m)
{
  if(distance(Player.x, Player.y, m->x, m->y) < 2)
  {
    m_talk_demonlover(m);
  }
}

void m_sp_eater(monster *m)
{
  if(Player.rank[COLLEGE])
  {
    m_status_set(*m, HOSTILE);
  }
  if(m_statusp(*m, HOSTILE))
  {
    if(los_p(m->x, m->y, Player.x, Player.y))
    {
      queue_message("A strange numbing sensation comes over you...");
      Player.mana = Player.mana / 2;
      if(random_range(4))
      {
        enchant(-1);
      }
      else
      {
        dispel(-1);
      }
      Player.pow--;
      if(--Player.pow < 1)
      {
        p_death("the Eater of Magic");
      }
    }
  }
  if(m->hp < 10)
  {
    queue_message("The Eater explodes in a burst of mana!");
    manastorm(m->x, m->y, 1000);
  }
}

void m_sp_dragonlord(monster *m)
{
  if(m_statusp(*m, HOSTILE))
  {
    if(distance(m->x, m->y, Player.x, Player.y) < 2)
    {
      if(!Player.status[IMMOBILE])
      {
        queue_message("A gust of wind from the Dragonlord's wings knocks you down!");
        p_damage(25, NORMAL_DAMAGE, "a gust of wind");
        setgamestatus(SKIP_PLAYER, GameStatus);
        Player.status[IMMOBILE] += 2;
      }
      else if(!Constriction)
      {
        queue_message("The Dragonlord grabs you with his tail!");
        Constriction = 25;
        Player.status[IMMOBILE] += 1;
      }
      else if(random_range(2))
      {
        queue_message("The coils squeeze tighter and tighter...");
        p_damage(Constriction, NORMAL_DAMAGE, "the Dragonlord");
        Player.status[IMMOBILE] += 1;
        Constriction *= 2;
      }
      else
      {
        queue_message("The Dragonlord hurls you to the ground!");
        p_damage(2 * Constriction, NORMAL_DAMAGE, "the Dragonlord");
        Constriction = 0;
      }
      m_sp_spell(m);
    }
    else
    {
      Constriction = 0;
      if(view_los_p(m->x, m->y, Player.x, Player.y))
      {
        if((!Player.immunity[FEAR]) && (!Player.status[AFRAID]))
        {
          queue_message("You are awestruck at the sight of the Dragonlord.");
          Player.status[AFRAID] += 5;
        }
        if(random_range(3))
        {
          m_sp_spell(m);
          m_sp_spell(m);
        }
      }
    }
  }
  else if(distance(m->x, m->y, Player.x, Player.y) < 2)
  {
    queue_message("You are extremely impressed at the sight of the Dragonlord.");
  }
}

void m_sp_blackout(monster *m)
{
  if((distance(m->x, m->y, Player.x, Player.y) < 4) && (Player.status[BLINDED] == 0))
  {
    queue_message("The fungus emits a burst of black spores. You've been blinded!");
    if(Player.status[TRUESIGHT] > 0)
    {
      queue_message("The blindness quickly passes.");
    }
    else
    {
      Player.status[BLINDED] += 4;
    }
  }
  if(loc_statusp(m->x, m->y, LIT, *Level))
  {
    queue_message("The fungus chirps.... ");
    queue_message("The area is plunged into darkness.");
    torch_check();
    torch_check();
    torch_check();
    torch_check();
    torch_check();
    torch_check();
    spreadroomdark(m->x, m->y, Level->site[m->x][m->y].roomnumber);
    levelrefresh();
  }
}

void m_sp_bogthing(monster *m)
{
  if(Player.status[IMMOBILE] && (distance(Player.x, Player.y, m->x, m->y) < 2))
  {
    if(!Player.status[AFRAID])
    {
      queue_message("As the bogthing touches you, you feel a frisson of terror....");
      if(Player.immunity[FEAR])
      {
        queue_message("which you shake off.");
      }
      else
      {
        Player.status[AFRAID] += 2;
      }
    }
    else
    {
      queue_message("The bogthing's touch causes you scream in agony!");
      p_damage(50, UNSTOPPABLE, "fright");
      queue_message("Your struggles grow steadily weaker....");
      Player.con--;
      Player.str--;
      if((Player.con < 3) || (Player.str < 3))
      {
        p_death("congestive heart failure");
      }
    }
  }
}

void m_sp_were(monster *m)
{
  int mid;
  if(m_statusp(*m, HOSTILE) || (Phase == 6))
  {
    do
    {
      mid = random_range(ML9 - NML_0) + ML1;
      /* log npc, 0th level npc, high score npc or were-creature */
    } while(mid == NPC || mid == ZERO_NPC || mid == HISCORE_NPC || mid == WEREHUMAN ||
            (Monsters[mid].uniqueness != COMMON) || (!m_statusp(Monsters[mid], MOBILE)) ||
            (!m_statusp(Monsters[mid], HOSTILE)));
    m->id = Monsters[mid].id;
    m->hp += Monsters[mid].hp;
    m->status |= Monsters[mid].status;
    m->ac    = Monsters[mid].ac;
    m->dmg   = Monsters[mid].dmg;
    m->speed = Monsters[mid].speed;
    m->immunity |= Monsters[mid].immunity;
    m->xpv += Monsters[mid].xpv;
    m->corpseweight = Monsters[mid].corpseweight;
    m->monchar      = Monsters[mid].monchar;
    m->talkf        = Monsters[mid].talkf;
    m->meleef       = Monsters[mid].meleef;
    m->strikef      = Monsters[mid].strikef;
    m->specialf     = Monsters[mid].specialf;
    m->monstring    = std::format("were-{}", Monsters[mid].monstring);
    m->corpsestr    = std::format("dead were-{}", Monsters[mid].monstring);
    m->immunity += pow2(NORMAL_DAMAGE);
    if(los_p(m->x, m->y, Player.x, Player.y))
    {
      queue_message("You witness a hideous transformation!");
    }
    else
    {
      queue_message("You hear a distant howl.");
    }
  }
}

void m_sp_servant(monster *m)
{
  if((m->id == SERV_LAW) && (Player.alignment < 0))
  {
    m_status_set(*m, HOSTILE);
  }
  else if((m->id == SERV_CHAOS) && (Player.alignment > 0))
  {
    m_status_set(*m, HOSTILE);
  }
}

void m_sp_av(monster *m)
{
  if(Player.mana > 0)
  {
    queue_message("You feel a sudden loss of mana!");
    Player.mana -= (std::max(0, 10 - distance(m->x, m->y, Player.x, Player.y)));
    dataprint();
  }
}

void m_sp_lw(monster *m)
{
  if(random_range(2))
  {
    if(Level->site[m->x][m->y].locchar == FLOOR)
    {
      Level->site[m->x][m->y].locchar = LAVA;
      Level->site[m->x][m->y].p_locf  = L_LAVA;
      lset(m->x, m->y, CHANGED, *Level);
    }
    else if(Level->site[m->x][m->y].locchar == WATER)
    {
      Level->site[m->x][m->y].locchar = FLOOR;
      Level->site[m->x][m->y].p_locf  = L_NO_OP;
      lset(m->x, m->y, CHANGED, *Level);
    }
  }
}

void m_sp_angel(monster *m)
{
  int mid, hostile = false;
  switch(m->aux1)
  {
    case ATHENA:
    case ODIN:
      hostile = ((Player.patron == HECATE) || (Player.patron == SET));
      break;
    case SET:
    case HECATE:
      hostile = ((Player.patron == ODIN) || (Player.patron == ATHENA));
      break;
    case DESTINY:
      hostile = (Player.patron != DESTINY);
      break;
  }
  if(hostile)
  {
    m_status_set(*m, HOSTILE);
  }
  if(m_statusp(*m, HOSTILE))
  {
    queue_message("The angel summons a heavenly host!");
    switch(m->level)
    {
      case 9:
        mid = HIGH_ANGEL;
        break;
      case 8:
        mid = ANGEL;
        break;
      default:
      case 6:
        mid = PHANTOM;
        break;
    }
    summon(-1, mid);
    summon(-1, mid);
    summon(-1, mid);
    /* prevent angel from summoning infinitely */
    m->specialf = M_NO_OP;
  }
}

/* Could completely fill up level */
void m_sp_swarm(monster *m)
{
  if(random_range(5) == 1)
  {
    if(view_los_p(m->x, m->y, Player.x, Player.y))
    {
      queue_message("The swarm expands!");
    }
    else
    {
      queue_message("You hear an aggravating humming noise.");
    }
    summon(-1, SWARM);
  }
}

// raise nearby corpses from the dead....
void m_sp_raise(monster *m)
{
  for(int x = m->x - 2; x <= m->x + 2; ++x)
  {
    for(int y = m->y - 2; y <= m->y + 2; ++y)
    {
      if(inbounds(x, y))
      {
        if(!Level->site[x][y].things.empty())
        {
          if(Level->site[x][y].things.front()->id == CORPSEID)
          {
            queue_message("The Zombie Overlord makes a mystical gesture...");
            summon(-1, Level->site[x][y].things.front()->charge);
            Level->site[x][y].things.pop_front();
          }
        }
      }
    }
  }
}

void m_sp_mb(monster *m)
{
  if(distance(m->x, m->y, Player.x, Player.y) == 1)
  {
    queue_message("The manaburst explodes!");
    if(m_statusp(*m, HOSTILE))
    {
      queue_message("You get blasted!");
      p_damage(random_range(100), UNSTOPPABLE, "a manaburst");
      queue_message("You feel cold all over!");
      Player.pow -= 3;
      Player.iq--;
      Player.con--;
      Player.str -= 2;
      Player.dex--;
      Player.agi--;
      dispel(-1);
    }
    else
    {
      queue_message("You feel toasty warm inside!");
      Player.pow++;
      Player.mana = std::max(Player.mana, calcmana());
      Player.hp   = std::max(Player.hp, ++Player.maxhp);
    }
    m->hp = 0;
  }
}

void m_sp_mirror(monster *m)
{
  int i, x, y;
  if(view_los_p(m->x, m->y, Player.x, Player.y))
  {
    if(random_range(20) + 6 < m->level)
    {
      summon(-1, m->id);
      queue_message("You hear the sound of a mirror shattering!");
    }
    else
    {
      for(i = 0; i < 5; i++)
      {
        x = m->x + random_range(13) - 6;
        y = m->y + random_range(13) - 6;
        if(inbounds(x, y))
        {
          Level->site[x][y].showchar = m->monchar;
          putspot(x, y, m->monchar);
        }
      }
    }
  }
}

void m_illusion(monster *m)
{
  int i = random_range(NUMMONSTERS);
  if(i == NPC || i == HISCORE_NPC || i == ZERO_NPC)
  {
    i = m->id; /* can't imitate NPC */
  }
  if(Player.status[TRUESIGHT])
  {
    m->monchar   = Monsters[m->id].monchar;
    m->monstring = Monsters[m->id].monstring;
  }
  else if(random_range(5) == 1)
  {
    m->monchar   = Monsters[i].monchar;
    m->monstring = Monsters[i].monstring;
  }
}

void m_huge_sounds(monster *m)
{
  if(m_statusp(*m, AWAKE) && (!los_p(m->x, m->y, Player.x, Player.y)) && (random_range(10) == 1))
  {
    queue_message("The dungeon shakes!");
  }
}

void m_thief_f(monster *m)
{
  int i = random_item();
  if(random_range(3) == 1)
  {
    if(distance(Player.x, Player.y, m->x, m->y) < 2)
    {
      if(p_immune(THEFT) || (Player.level > (m->level * 2) + random_range(20)))
      {
        queue_message("You feel secure.");
      }
      else
      {
        if(i == ABORT)
        {
          queue_message("You feel fortunate.");
        }
        else if((Player.possessions[i]->used) || (Player.dex < m->level * random_range(10)))
        {
          queue_message("You feel a sharp tug.... You hold on!");
        }
        else
        {
          queue_message("You feel uneasy for a moment.");
          if(m->uniqueness == COMMON)
          {
            queue_message(std::format("The {} suddenly runs away for some reason.", m->monstring));
          }
          else
          {
            queue_message(std::format("{} suddenly runs away for some reason.", m->monstring));
          }
          m_teleport(m);
          m->movef    = M_MOVE_SCAREDY;
          m->specialf = M_MOVE_SCAREDY;
          conform_unused_object(Player.possessions[i]);
          m_pickup(m, std::move(Player.possessions[i]));
        }
      }
    }
  }
}

void m_summon(monster *m)
{
  if((distance(Player.x, Player.y, m->x, m->y) < 2) && (random_range(3) == 1))
  {
    summon(0, -1);
    summon(0, -1);
  }
}

void m_aggravate(monster *m)
{
  if(m_statusp(*m, HOSTILE))
  {
    if(m->uniqueness == COMMON)
    {
      queue_message(std::format("The {} emits an irritating humming sound.", m->monstring));
    }
    else
    {
      queue_message(std::format("{} emits an irritating humming sound.", m->monstring));
    }
    aggravate();
    m_status_reset(*m, HOSTILE);
  }
}

void m_sp_merchant(monster *m)
{
  if(m_statusp(*m, HOSTILE))
  {
    if(Current_Environment == E_VILLAGE)
    {
      queue_message("The merchant screams: 'Help! Murder! Guards! Help!'");
      queue_message("You hear the sound of police whistles and running feet.");
      for(std::unique_ptr<monster> &level_monster : Level->mlist)
      {
        m_status_set(*level_monster, AWAKE);
        m_status_set(*level_monster, HOSTILE);
      }
      m->specialf = M_NO_OP;
    }
  }
}

/* The special function of the various people in the court of the archmage */
/* and the sorcerors' circle */
void m_sp_court(monster *m)
{
  if(m_statusp(*m, HOSTILE))
  {
    queue_message("A storm of spells hits you!");
    for(std::unique_ptr<monster> &level_monster : Level->mlist)
    {
      m_status_set(*level_monster, HOSTILE);
      m_sp_spell(level_monster.get());
      if(level_monster->specialf == M_SP_COURT)
      {
        level_monster->specialf = M_SP_SPELL;
      }
    }
  }
}

/* The special function of the dragons in the dragons' lair */
void m_sp_lair(monster *m)
{
  if(m_statusp(*m, HOSTILE))
  {
    queue_message("You notice a number of dragons waking up....");
    queue_message("You are struck by a quantity of firebolts.");
    for(std::unique_ptr<monster> &level_monster : Level->mlist)
    {
      if(level_monster->hp > 0 && level_monster->specialf == M_SP_LAIR)
      {
        m_status_set(*level_monster, HOSTILE);
        fbolt(level_monster->x, level_monster->y, Player.x, Player.y, 100, 100);
        if(level_monster->id == DRAGON_LORD)
        {
          level_monster->specialf = M_SP_DRAGONLORD;
        }
        else
        {
          level_monster->specialf = M_STRIKE_FBOLT;
        }
      }
    }
  }
}

void m_sp_prime(monster *m)
{
  if(m_statusp(*m, HOSTILE))
  {
    queue_message("The prime sorceror gestures and a pentacular gate opens!");
    queue_message("You are surrounded by demons!");
    summon(-1, DEMON_PRINCE);
    summon(-1, DEMON_PRINCE);
    summon(-1, DEMON_PRINCE);
    summon(-1, DEMON_PRINCE);
  }
  m->specialf = M_SP_SPELL;
}
