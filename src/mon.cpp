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

/* mon.c */
/* various functions to do with monsters */

#include "glob.h"

#include <algorithm>
#include <cstring>
#include <format>

extern bool merge_item_with_list(objectlist *l, object *o, int n);
extern void queue_message(const std::string &message);

/*               Revised function                   */
/* WDT: code contributed by David J. Robertson */
/* consider one monster's action */
void m_pulse(struct monster *m)
{
  int range  = distance(m->x, m->y, Player.x, Player.y);
  int STRIKE = false;
  pol prev;

  if(Time % 10 == 0)
  {
    if(m->hp < Monsters[m->id].hp)
    {
      m->hp++;
    }
  }

  if((!m_statusp(*m, AWAKE)) && (range <= m->wakeup))
  {
    m_status_set(*m, AWAKE);
    resetgamestatus(FAST_MOVE, GameStatus);
  }

  if(m_statusp(*m, AWAKE))
  {
    if(m_statusp(*m, WANDERING))
    {
      if(m_statusp(*m, MOBILE))
      {
        m_random_move(m);
      }
      if(range <= m->sense && (m_statusp(*m, HOSTILE) || m_statusp(*m, NEEDY)))
      {
        m_status_reset(*m, WANDERING);
      }
    }
    else /* not wandering */
    {
      if(m_statusp(*m, HOSTILE))
      {
        if((range > 2) && (range < m->sense) && (random_range(2) == 1))
        {
          if(los_p(m->x, m->y, Player.x, Player.y) && (Player.status[INVISIBLE] == 0))
          {
            STRIKE = true;
            monster_strike(m);
          }
        }
      }

      if((m_statusp(*m, HOSTILE) || m_statusp(*m, NEEDY)) && (range > 1) && m_statusp(*m, MOBILE) &&
         (!STRIKE || (random_range(2) == 1)))
      {
        monster_move(m);
      }
      else if(m_statusp(*m, HOSTILE) && (range == 1))
      {
        resetgamestatus(FAST_MOVE, GameStatus);
        tacmonster(m);
      }
    }
    /* if monster is greedy, picks up treasure it finds */
    if(m_statusp(*m, GREEDY) && (m->hp > 0))
    {
      while(Level->site[m->x][m->y].things)
      {
        m_pickup(m, Level->site[m->x][m->y].things->thing);
        prev                           = Level->site[m->x][m->y].things;
        Level->site[m->x][m->y].things = Level->site[m->x][m->y].things->next;
        free(prev);
      }
    }
    /* prevents monsters from casting spells from other side of dungeon */
    if((range < std::max(5, m->level)) && (m->hp > 0) && (random_range(2) == 1))
    {
      monster_special(m);
    }
  }
}

/* actually make a move */
void movemonster(struct monster *m, int newx, int newy)
{
  if(Level->site[newx][newy].creature)
  {
    return;
  }
  if(Level->site[m->x][m->y].creature == m)
  {
    Level->site[m->x][m->y].creature = nullptr;
  }
  m->x                             = newx;
  m->y                             = newy;
  Level->site[m->x][m->y].creature = m;
  m_movefunction(m, Level->site[m->x][m->y].p_locf);
}

/* give object o to monster m */
void m_pickup(struct monster *m, struct object *o)
{
  pol tmp        = ((pol)checkmalloc(sizeof(oltype)));
  tmp->thing     = o;
  tmp->next      = m->possessions;
  m->possessions = tmp;
}

void m_dropstuff(struct monster *m)
{
  objectlist *drop_pile = Level->site[m->x][m->y].things;
  for(objectlist *possessions = m->possessions; possessions;)
  {
    objectlist *tmp = possessions->next;
    if(merge_item_with_list(drop_pile, possessions->thing, possessions->thing->number))
    {
      free(possessions->thing);
      free(possessions);
    }
    else
    {
      possessions->next = drop_pile;
      drop_pile         = possessions;
    }
    possessions = tmp;
  }
  m->possessions                 = nullptr;
  Level->site[m->x][m->y].things = drop_pile;
}

void m_damage(struct monster *m, int dmg, int dtype)
{
  m_status_set(*m, AWAKE);
  m_status_set(*m, HOSTILE);
  if(m_immunityp(*m, dtype))
  {
    if(los_p(Player.x, Player.y, m->x, m->y))
    {
      if(m->uniqueness != COMMON)
      {
        queue_message(std::format("{} ignores the attack!", m->monstring));
      }
      else
      {
        queue_message(std::format("The {} ignores the attack!", m->monstring));
      }
    }
  }
  else if((m->hp -= dmg) < 1)
  {
    m_death(m);
  }
}

void m_death(struct monster *m)
{
  pob corpse;
  pml ml;
  int x, y, found = false;
  pol curr, prev = nullptr;

  m->hp = -1;
  if(los_p(Player.x, Player.y, m->x, m->y))
  {
    gain_experience(m->xpv);
    calc_melee();
    if(m->uniqueness != COMMON)
    {
      queue_message(std::format("{} is dead!", m->monstring));
    }
    else
    {
      queue_message(std::format("The {} is dead!", m->monstring));
    }
  }
  m_dropstuff(m);
  if(m->id == DEATH)
  { /* Death */
    mprint("Death lies sprawled out on the ground......");
    mprint("Death laughs ironically and gets back to its feet.");
    mprint("It gestures and another scythe appears in its hands.");
    switch(random_range(10))
    {
      case 0:
        mprint("Death performs a little bow and goes back on guard.");
        break;
      case 1:
        mprint("'A hit! A palpable hit!' Death goes back on the attack.");
        break;
      case 2:
        mprint("'Ah, if only it could be so simple!' snickers Death.");
        break;
      case 3:
        mprint("'You think Death can be slain?  What a jest!' says Death.");
        break;
      case 4:
        mprint("'Your point is well taken.' says Death, attacking again.");
        break;
      case 5:
        mprint("'Oh, come now, stop delaying the inevitable.' says Death.");
        break;
      case 6:
        mprint("'Your destiny ends here with me.' says Death, scythe raised.");
        break;
      case 7:
        mprint("'I almost felt that.' says Death, smiling.");
        break;
      case 8:
        mprint("'Timeo Mortis?' asks Death quizzically, 'Not me!'");
        break;
      case 9:
        mprint("Death sighs theatrically. 'They never learn.'");
        break;
    }
    strengthen_death(m);
  }
  else
  {
    Level->site[m->x][m->y].creature = nullptr;
    if(m == Arena_Monster)
    {
      Arena_Victory = true; /* won this round of arena combat */
    }
    if(random_range(2) || (m->uniqueness != COMMON))
    {
      corpse = ((pob)checkmalloc(sizeof(objtype)));
      make_corpse(corpse, m);
      drop_at(m->x, m->y, corpse);
    }
    plotspot(m->x, m->y, false);
    switch(m->id)
    {
      case HISCORE_NPC:
        switch(m->aux2)
        {
          case 0:
            mprint("You hear a faroff dirge. You feel a sense of triumph.");
            break;
          case 1:
          case 2:
          case 3:
          case 4:
          case 5:
          case 6:
            mprint("You hear a faroff sound like angels crying....");
            strcpy(Priest[m->aux2], std::string{nameprint()}.c_str());
            Priestbehavior[m->aux2] = 2933;
            break;
          case 7:
            mprint("A furtive figure dashes out of the shadows, takes a look at");
            mprint("the corpse, and runs away!");
            strcpy(Shadowlord, std::string{nameprint()}.c_str());
            Shadowlordbehavior = 2912;
            break;
          case 8:
            mprint("An aide-de-camp approaches, removes the corpse's insignia,");
            mprint("and departs.");
            strcpy(Commandant, std::string{nameprint()}.c_str());
            Commandantbehavior = 2912;
            break;
          case 9:
            mprint("An odd glow surrounds the corpse, and slowly fades.");
            strcpy(Archmage, std::string{nameprint()}.c_str());
            Archmagebehavior = 2933;
            break;
          case 10:
            mprint("A demon materializes, takes a quick look at the corpse,");
            mprint("and teleports away with a faint popping noise.");
            strcpy(Prime, std::string{nameprint()}.c_str());
            Primebehavior = 2932;
            break;
          case 11:
            mprint("A sports columnist rushes forward and takes a quick photo");
            mprint("of the corpse and rushes off muttering about a deadline.");
            strcpy(Champion, std::string{nameprint()}.c_str());
            Championbehavior = 2913;
            break;
          case 12:
            mprint("You hear a fanfare in the distance, and feel dismayed.");
            strcpy(Duke, std::string{nameprint()}.c_str());
            Dukebehavior = 2911;
            break;
          case 13:
            if(Player.alignment > 10)
            {
              mprint("You feel smug.");
            }
            else if(Player.alignment < 10)
            {
              mprint("You feel ashamed.");
            }
            strcpy(Chaoslord, std::string{nameprint()}.c_str());
            Chaoslordbehavior = 2912;
            break;
          case 14:
            if(Player.alignment < 10)
            {
              mprint("You feel smug.");
            }
            else if(Player.alignment > 10)
            {
              mprint("You feel ashamed.");
            }
            strcpy(Lawlord, std::string{nameprint()}.c_str());
            Lawlordbehavior = 2911;
            break;
          case 15:
            /* just a tad complicated. Promote a new justiciar if any
           guards are left in the city, otherwise Destroy the Order! */
            Player.alignment -= 100;
            if(!gamestatusp(DESTROYED_ORDER, GameStatus))
            {
              curr = Level->site[m->x][m->y].things;
              while(curr && curr->thing->id != THINGID + 16)
              {
                prev = curr;
                curr = curr->next;
              }
              strcpy(Justiciar, std::string{nameprint()}.c_str());
              Justiciarbehavior = 2911;
              mprint("In the distance you hear a trumpet. A Servant of Law");
              /* promote one of the city guards to be justiciar */
              ml = City->mlist;
              while(!found && ml)
              {
                found = ((ml->m->id == GUARD) && (ml->m->hp > 0));
                if(!found)
                {
                  ml = ml->next;
                }
              }
              if(ml)
              {
                if(curr)
                {
                  mprint("materializes, sheds a tear, picks up the badge, and "
                         "leaves.");
                  m_pickup(ml->m, curr->thing);
                  if(prev)
                  {
                    prev->next = curr->next;
                  }
                  else
                  {
                    Level->site[m->x][m->y].things = curr->next;
                  }
                  free(curr);
                }
                else
                {
                  mprint("materializes, sheds a tear, and leaves.");
                }
                mprint("A new justiciar has been promoted!");
                x = ml->m->x;
                y = ml->m->y;
                make_hiscore_npc(ml->m, 15);
                ml->m->x     = x;
                ml->m->y     = y;
                ml->m->click = (Tick + 1) % 60;
                m_status_reset(*ml->m, AWAKE);
                m_status_reset(*ml->m, HOSTILE);
              }
              else
              {
                mprint("materializes, sheds a tear, and leaves.");
              }
              alert_guards();
              /* will cause order to be destroyed if no guards or justiciar*/
            }
            else
            {
              mprint("A Servant of Chaos materializes, grabs the corpse,");
              mprint("snickers a bit, and vanishes.");
            }
            break;
        }
        save_hiscore_npc(m->aux2);
        break;
      case GUARD: /* guard */
        Player.alignment -= 10;
        if((Current_Environment == E_CITY) || (Current_Environment == E_VILLAGE))
        {
          alert_guards();
        }
        break;
      case GOBLIN_KING:
        if(!gamestatusp(ATTACKED_ORACLE, GameStatus))
        {
          mprint("You seem to hear a woman's voice from far off:");
          mprint("'Well done! Come to me now....'");
        }
        setgamestatus(COMPLETED_CAVES, GameStatus);
        break; /* gob king */
      case GREAT_WYRM:
        if(!gamestatusp(ATTACKED_ORACLE, GameStatus))
        {
          mprint("A female voice sounds from just behind your ear:");
          mprint("'Well fought! I have some new advice for you....'");
        }
        setgamestatus(COMPLETED_SEWERS, GameStatus);
        break; /*grt worm */
      case EATER:
        setgamestatus(KILLED_EATER, GameStatus);
        break;
      case LAWBRINGER:
        setgamestatus(KILLED_LAWBRINGER, GameStatus);
        break;
      case DRAGON_LORD:
        setgamestatus(KILLED_DRAGONLORD, GameStatus);
        break;
      case DEMON_EMP:
        setgamestatus(COMPLETED_VOLCANO, GameStatus);
        if(!gamestatusp(ATTACKED_ORACLE, GameStatus))
        {
          mprint("You feel a soft touch on your shoulder...");
          mprint("You turn around but there is no one there!");
          mprint("You turn back and see a note: 'See me soon.'");
          mprint("The note vanishes in a burst of blue fire!");
        }
        break;
      case ELEM_MASTER:
        if(!gamestatusp(ATTACKED_ORACLE, GameStatus))
        {
          mprint("Words appear before you, traced in blue flame!");
          mprint("'Return to the Prime Plane via the Circle of Sorcerors....'");
        }
        break; /* elem mast */
    }
    switch(m->specialf)
    {
      case M_SP_COURT:
      case M_SP_LAIR:
        m_status_set(*m, HOSTILE);
        monster_action(m, m->specialf);
    }
  }
}

void monster_move(struct monster *m)
{
  monster_action(m, m->movef);
}

void monster_strike(struct monster *m)
{
  if(player_on_sanctuary())
  {
    print1("The aegis of your deity protects you!");
  }
  else
  {
    /* It's lawful to wait to be attacked */
    if(m->attacked == 0)
    {
      Player.alignment++;
    }
    m->attacked++;
    monster_action(m, m->strikef);
  }
}

void monster_special(struct monster *m)
{
  /* since many special functions are really attacks, cancel them
     all if on sanctuary */
  if(!player_on_sanctuary())
  {
    monster_action(m, m->specialf);
  }
}

void monster_talk(struct monster *m)
{
  monster_action(m, m->talkf);
}

void monster_action(struct monster *m, int action)
{
  int meleef;
  if((action >= M_MELEE_NORMAL) && (action < M_MOVE_NORMAL))
  {
    /* kluge allows multiple attack forms */
    if(distance(m->x, m->y, Player.x, Player.y) < 2)
    {
      meleef    = m->meleef;
      m->meleef = action;
      tacmonster(m);
      m->meleef = meleef;
    }
  }
  else
  {
    switch(action)
    {
      case M_NO_OP:
        m_no_op(m);
        break;

      case M_MOVE_NORMAL:
        m_normal_move(m);
        break;
      case M_MOVE_FLUTTER:
        m_flutter_move(m);
        break;
      case M_MOVE_FOLLOW:
        m_follow_move(m);
        break;
      case M_MOVE_TELEPORT:
        m_teleport(m);
        break;
      case M_MOVE_RANDOM:
        m_random_move(m);
        break;
      case M_MOVE_SMART:
        m_smart_move(m);
        break;
      case M_MOVE_SPIRIT:
        m_spirit_move(m);
        break;
      case M_MOVE_CONFUSED:
        m_confused_move(m);
        break;
      case M_MOVE_SCAREDY:
        m_scaredy_move(m);
        break;
      case M_MOVE_ANIMAL:
        m_move_animal(m);
        break;
      case M_MOVE_LEASH:
        m_move_leash(m);
        break;

      case M_STRIKE_MISSILE:
        m_nbolt(m);
        break;
      case M_STRIKE_FBOLT:
        m_firebolt(m);
        break;
      case M_STRIKE_LBALL:
        m_lball(m);
        break;
      case M_STRIKE_FBALL:
        m_fireball(m);
        break;
      case M_STRIKE_SNOWBALL:
        m_snowball(m);
        break;
      case M_STRIKE_BLIND:
        m_blind_strike(m);
        break;
      case M_STRIKE_SONIC:
        m_strike_sonic(m);
        break;

      case M_TALK_HORSE:
        m_talk_horse(m);
        break;
      case M_TALK_THIEF:
        m_talk_thief(m);
        break;
      case M_TALK_STUPID:
        m_talk_stupid(m);
        break;
      case M_TALK_SILENT:
        m_talk_silent(m);
        break;
      case M_TALK_HUNGRY:
        m_talk_hungry(m);
        break;
      case M_TALK_GREEDY:
        m_talk_greedy(m);
        break;
      case M_TALK_TITTER:
        m_talk_titter(m);
        break;
      case M_TALK_MP:
        m_talk_mp(m);
        break;
      case M_TALK_IM:
        m_talk_im(m);
        break;
      case M_TALK_MAN:
        m_talk_man(m);
        break;
      case M_TALK_ROBOT:
        m_talk_robot(m);
        break;
      case M_TALK_EVIL:
        m_talk_evil(m);
        break;
      case M_TALK_GUARD:
        m_talk_guard(m);
        break;
      case M_TALK_MIMSY:
        m_talk_mimsy(m);
        break;
      case M_TALK_SLITHY:
        m_talk_slithy(m);
        break;
      case M_TALK_BURBLE:
        m_talk_burble(m);
        break;
      case M_TALK_BEG:
        m_talk_beg(m);
        break;
      case M_TALK_HINT:
        m_talk_hint(m);
        break;
      case M_TALK_EF:
        m_talk_ef(m);
        break;
      case M_TALK_GF:
        m_talk_gf(m);
        break;
      case M_TALK_SEDUCTOR:
        m_talk_seductor(m);
        break;
      case M_TALK_DEMONLOVER:
        m_talk_demonlover(m);
        break;
      case M_TALK_NINJA:
        m_talk_ninja(m);
        break;
      case M_TALK_ASSASSIN:
        m_talk_assassin(m);
        break;
      case M_TALK_SERVANT:
        m_talk_servant(m);
        break;
      case M_TALK_ANIMAL:
        m_talk_animal(m);
        break;
      case M_TALK_SCREAM:
        m_talk_scream(m);
        break;
      case M_TALK_PARROT:
        m_talk_parrot(m);
        break;
      case M_TALK_HYENA:
        m_talk_hyena(m);
        break;
      case M_TALK_DRUID:
        m_talk_druid(m);
        break;
      case M_TALK_ARCHMAGE:
        m_talk_archmage(m);
        break;
      case M_TALK_MERCHANT:
        m_talk_merchant(m);
        break;
      case M_TALK_PRIME:
        m_talk_prime(m);
        break;

      case M_SP_BOG:
        m_sp_bogthing(m);
        break;
      case M_SP_WERE:
        m_sp_were(m);
        break;
      case M_SP_WHISTLEBLOWER:
        m_sp_whistleblower(m);
        break;
      case M_SP_MERCHANT:
        m_sp_merchant(m);
        break;
      case M_SP_SURPRISE:
        m_sp_surprise(m);
        break;
      case M_SP_MP:
        m_sp_mp(m);
        break;
      case M_SP_THIEF:
        m_thief_f(m);
        break;
      case M_SP_DEMONLOVER:
        m_sp_demonlover(m);
        break;
      case M_SP_AGGRAVATE:
        m_aggravate(m);
        break;
      case M_SP_POISON_CLOUD:
        m_sp_poison_cloud(m);
        break;
      case M_SP_HUGE:
        m_huge_sounds(m);
        break;
      case M_SP_SUMMON:
        m_summon(m);
        break;
      case M_SP_ILLUSION:
        m_illusion(m);
        break;
      case M_SP_ESCAPE:
        m_sp_escape(m);
        break;
      case M_SP_FLUTTER:
        m_flutter_move(m);
        break;
      case M_SP_EXPLODE:
        m_sp_explode(m);
        break;
      case M_SP_DEMON:
        m_sp_demon(m);
        break;
      case M_SP_ACID_CLOUD:
        m_sp_acid_cloud(m);
        break;
      case M_SP_GHOST:
        m_sp_ghost(m);
        break;
      case M_SP_SPELL:
        m_sp_spell(m);
        break;
      case M_SP_SEDUCTOR:
        m_sp_seductor(m);
        break;
      case M_SP_EATER:
        m_sp_eater(m);
        break;
      case M_SP_DRAGONLORD:
        m_sp_dragonlord(m);
        break;
      case M_SP_BLACKOUT:
        m_sp_blackout(m);
        break;
      case M_SP_SWARM:
        m_sp_swarm(m);
        break;
      case M_SP_ANGEL:
        m_sp_angel(m);
        break;
      case M_SP_SERVANT:
        m_sp_servant(m);
        break;
      case M_SP_AV:
        m_sp_av(m);
        break;
      case M_SP_LW:
        m_sp_lw(m);
        break;
      case M_SP_MB:
        m_sp_mb(m);
        break;
      case M_SP_RAISE:
        m_sp_raise(m);
        break;
      case M_SP_MIRROR:
        m_sp_mirror(m);
        break;
      case M_SP_COURT:
        m_sp_court(m);
        break;
      case M_SP_LAIR:
        m_sp_lair(m);
        break;
      case M_SP_PRIME:
        m_sp_prime(m);
        break;
    }
  }
}

/* makes one of the highscore npcs */
void make_hiscore_npc(pmt npc, int npcid)
{
  int st = -1;
  pob ob;
  *npc      = Monsters[HISCORE_NPC];
  npc->aux2 = npcid;
  /* each of the high score npcs can be created here */
  std::string npc_name;
  switch(npcid)
  {
    case 0:
      npc_name = Hiscorer;
      determine_npc_behavior(npc, Hilevel, Hibehavior);
      break;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
      npc_name = Priest[npcid];
      determine_npc_behavior(npc, Priestlevel[npcid], Priestbehavior[npcid]);
      st                     = ARTIFACTID + 13 + npcid; /* appropriate holy symbol... */
      Objects[st].uniqueness = UNIQUE_MADE;
      if(npcid == DRUID)
      {
        npc->talkf = M_TALK_DRUID;
      }
      if(Player.patron == npcid)
      {
        m_status_reset(*npc, HOSTILE);
      }
      break;
    case 7:
      npc_name = Shadowlord;
      determine_npc_behavior(npc, Shadowlordlevel, Shadowlordbehavior);
      break;
    case 8:
      npc_name = Commandant;
      determine_npc_behavior(npc, Commandantlevel, Commandantbehavior);
      if(Player.rank[LEGION])
      {
        m_status_reset(*npc, HOSTILE);
      }
      break;
    case 9:
      npc_name = Archmage;
      determine_npc_behavior(npc, Archmagelevel, Archmagebehavior);
      st         = ARTIFACTID + 9; /* kolwynia */
      npc->talkf = M_TALK_ARCHMAGE;
      m_status_reset(*npc, WANDERING);
      m_status_reset(*npc, HOSTILE);
      break;
    case 10:
      npc_name = Prime;
      determine_npc_behavior(npc, Primelevel, Primebehavior);
      npc->talkf    = M_TALK_PRIME;
      npc->specialf = M_SP_PRIME;
      if(Player.alignment < 0)
      {
        m_status_reset(*npc, HOSTILE);
      }
      break;
    case 11:
      npc_name = Champion;
      determine_npc_behavior(npc, Championlevel, Championbehavior);
      if(Player.rank[ARENA])
      {
        m_status_reset(*npc, HOSTILE);
      }
      break;
    case 12:
      npc_name = Duke;
      determine_npc_behavior(npc, Dukelevel, Dukebehavior);
      break;
    case 13:
      npc_name = Chaoslord;
      determine_npc_behavior(npc, Chaoslordlevel, Chaoslordbehavior);
      if(Player.alignment < 0 && random_range(2))
      {
        m_status_reset(*npc, HOSTILE);
      }
      break;
    case 14:
      npc_name = Lawlord;
      determine_npc_behavior(npc, Lawlordlevel, Lawlordbehavior);
      if(Player.alignment > 0)
      {
        m_status_reset(*npc, HOSTILE);
      }
      break;
    case 15:
      npc_name = Justiciar;
      determine_npc_behavior(npc, Justiciarlevel, Justiciarbehavior);
      st            = THINGID + 16; /* badge */
      npc->talkf    = M_TALK_GUARD;
      npc->specialf = M_SP_WHISTLEBLOWER;
      m_status_reset(*npc, WANDERING);
      m_status_reset(*npc, HOSTILE);
      break;
  }
  if(st > -1 && Objects[st].uniqueness == UNIQUE_MADE)
  {
    ob  = ((pob)checkmalloc(sizeof(objtype)));
    *ob = Objects[st];
    m_pickup(npc, ob);
  }
  npc->monstring = salloc(npc_name.c_str());
  npc->corpsestr = salloc(std::format("The body of {}", npc_name).c_str());
}

/* sets npc behavior given level and behavior code */
void determine_npc_behavior(pmt npc, int level, int behavior)
{
  int combatype, competence, talktype;
  npc->hp     = (level + 1) * 20;
  npc->status = AWAKE + MOBILE + WANDERING;
  combatype   = (behavior % 100) / 10;
  competence  = (behavior % 1000) / 100;
  talktype    = behavior / 1000;
  npc->level  = competence;
  if(npc->level < 2 * difficulty())
  {
    npc->status += HOSTILE;
  }
  npc->xpv = npc->level * 20;
  switch(combatype)
  {
    case 1: /* melee */
      npc->meleef = M_MELEE_NORMAL;
      npc->dmg    = competence * 5;
      npc->hit    = competence * 3;
      npc->speed  = 3;
      break;
    case 2: /*missile*/
      npc->meleef  = M_MELEE_NORMAL;
      npc->strikef = M_STRIKE_MISSILE;
      npc->dmg     = competence * 3;
      npc->hit     = competence * 2;
      npc->speed   = 4;
      break;
    case 3: /* spellcasting */
      npc->meleef   = M_MELEE_NORMAL;
      npc->dmg      = competence;
      npc->hit      = competence;
      npc->specialf = M_SP_SPELL;
      npc->speed    = 6;
      break;
    case 4: /* thievery */
      npc->meleef   = M_MELEE_NORMAL;
      npc->dmg      = competence;
      npc->hit      = competence;
      npc->specialf = M_SP_THIEF;
      npc->speed    = 3;
      break;
    case 5: /* flee */
      npc->dmg      = competence;
      npc->hit      = competence;
      npc->meleef   = M_MELEE_NORMAL;
      npc->specialf = M_MOVE_SCAREDY;
      npc->speed    = 3;
      break;
  }
  if(npc->talkf == M_TALK_MAN)
  {
    switch(talktype)
    {
      case 1:
        npc->talkf = M_TALK_EVIL;
        break;
      case 2:
        npc->talkf = M_TALK_MAN;
        break;
      case 3:
        npc->talkf = M_TALK_HINT;
        break;
      case 4:
        npc->talkf = M_TALK_BEG;
        break;
      case 5:
        npc->talkf = M_TALK_SILENT;
        break;
      default:
        mprint("Say Whutt? (npc talk weirdness)");
        break;
    }
  }
  npc->uniqueness = UNIQUE_MADE;
}

/* makes an ordinary npc (maybe undead) */
void make_log_npc(struct monster *npc)
{
  int   i;
  int   behavior, status, level;

  /* in case the log file is null */
  behavior = 2718;
  level    = 1;
  status   = 2;
  strcpy(Str2, "Malaprop the Misnamed");

  FILE *fd = checkfopen(std::format("{}omega.log", Omegalib), "r");
  int n  = 1;
  while(fgets(Str1, STRING_LEN, fd))
  {
    if(random_range(n) == 0)
    { /* this algo. from Knuth 2 - cute, eh? */
      sscanf(Str1, "%d %d %d", &status, &level, &behavior);
      for(i = 0; (Str1[i] < 'a' || Str1[i] > 'z') && (Str1[i] < 'A' || Str1[i] > 'Z'); i++)
      {
        ;
      }
      strcpy(Str2, Str1 + i);
      Str2[strlen(Str2) - 1] = '\0'; /* 'cos fgets reads in the \n */
    }
    n++;
  }
  fclose(fd);
  npc->hp = level * 20;
  std::string npc_name;
  if(status == 1)
  {
    if(level < 3)
    {
      *npc = Monsters[GHOST];
      npc_name = "ghost named ";
    }
    else if(level < 7)
    {
      *npc = Monsters[HAUNT];
      npc_name = "haunt named ";
    }
    else if(level < 12)
    {
      *npc = Monsters[SPECTRE];
      npc_name = "spectre named ";
    }
    else
    {
      *npc = Monsters[LICHE];
      npc_name = "lich named ";
    }
    npc->monstring = salloc(std::format("{}{}", npc_name, Str2).c_str());
    npc->corpsestr = salloc(std::format("the mortal remains of {}", Str2).c_str());
  }
  else
  {
    npc->monstring = salloc(Str2);
    npc->corpsestr = salloc(std::format("the corpse of {}", Str2).c_str());
  }
  determine_npc_behavior(npc, level, behavior);
}

void m_trap_dart(struct monster *m)
{
  if(los_p(m->x, m->y, Player.x, Player.y))
  {
    if(m->uniqueness != COMMON)
    {
      queue_message(std::format("{} was hit by a dart!", m->monstring));
    }
    else
    {
      queue_message(std::format("The {} was hit by a dart!", m->monstring));
    }
    Level->site[m->x][m->y].locchar = TRAP;
    lset(m->x, m->y, CHANGED, *Level);
  }
  m_damage(m, difficulty() * 2, NORMAL_DAMAGE);
}

void m_trap_pit(struct monster *m)
{
  if(los_p(m->x, m->y, Player.x, Player.y))
  {
    if(m->uniqueness != COMMON)
    {
      queue_message(std::format("{} fell into a pit!", m->monstring));
    }
    else
    {
      queue_message(std::format("The {} fell into a pit!", m->monstring));
    }
    Level->site[m->x][m->y].locchar = TRAP;
    lset(m->x, m->y, CHANGED, *Level);
  }
  if(!m_statusp(*m, INTANGIBLE))
  {
    m_status_reset(*m, MOBILE);
  }
  m_damage(m, difficulty() * 5, NORMAL_DAMAGE);
}

void m_trap_door(struct monster *m)
{
  if(los_p(m->x, m->y, Player.x, Player.y))
  {
    if(m->uniqueness != COMMON)
    {
      queue_message(std::format("{} fell into a trap door!", m->monstring));
    }
    else
    {
      queue_message(std::format("The {} fell into a trap door!", m->monstring));
    }
    Level->site[m->x][m->y].locchar = TRAP;
    lset(m->x, m->y, CHANGED, *Level);
  }
  m_vanish(m);
}

void m_trap_abyss(struct monster *m)
{
  if(los_p(m->x, m->y, Player.x, Player.y))
  {
    if(m->uniqueness != COMMON)
    {
      queue_message(std::format("{} fell into the infinite abyss!", m->monstring));
    }
    else
    {
      queue_message(std::format("The {} fell into the infinite abyss!", m->monstring));
    }
    Level->site[m->x][m->y].locchar = ABYSS;
    lset(m->x, m->y, CHANGED, *Level);
    Level->site[m->x][m->y].p_locf = L_ABYSS;
    lset(m->x, m->y, CHANGED, *Level);
  }
  setgamestatus(SUPPRESS_PRINTING, GameStatus);
  m_vanish(m);
  resetgamestatus(SUPPRESS_PRINTING, GameStatus);
}

void m_trap_snare(struct monster *m)
{
  Level->site[m->x][m->y].locchar = TRAP;
  lset(m->x, m->y, CHANGED, *Level);
  if(los_p(m->x, m->y, Player.x, Player.y))
  {
    if(m->uniqueness != COMMON)
    {
      queue_message(std::format("{} was caught in a snare!", m->monstring));
    }
    else
    {
      queue_message(std::format("The {} was caught in a snare!", m->monstring));
    }
  }
  if(!m_statusp(*m, INTANGIBLE))
  {
    m_status_reset(*m, MOBILE);
  }
}

void m_trap_blade(struct monster *m)
{
  Level->site[m->x][m->y].locchar = TRAP;
  lset(m->x, m->y, CHANGED, *Level);
  if(los_p(m->x, m->y, Player.x, Player.y))
  {
    if(m->uniqueness != COMMON)
    {
      queue_message(std::format("{} was was hit by a blade trap!", m->monstring));
    }
    else
    {
      queue_message(std::format("The {} was was hit by a blade trap!", m->monstring));
    }
  }
  m_damage(m, (difficulty() + 1) * 7 - Player.defense, NORMAL_DAMAGE);
}

void m_trap_fire(struct monster *m)
{
  Level->site[m->x][m->y].locchar = TRAP;
  lset(m->x, m->y, CHANGED, *Level);
  if(los_p(m->x, m->y, Player.x, Player.y))
  {
    if(m->uniqueness != COMMON)
    {
      queue_message(std::format("{} was was hit by a fire trap!", m->monstring));
    }
    else
    {
      queue_message(std::format("The {} was was hit by a fire trap!", m->monstring));
    }
  }
  m_damage(m, (difficulty() + 1) * 5, FLAME);
}

void m_fire(struct monster *m)
{
  if(los_p(m->x, m->y, Player.x, Player.y))
  {
    if(m->uniqueness != COMMON)
    {
      queue_message(std::format("{} was was blasted by fire!", m->monstring));
    }
    else
    {
      queue_message(std::format("The {} was was blasted by fire!", m->monstring));
    }
  }
  m_damage(m, random_range(100), FLAME);
}

void m_trap_teleport(struct monster *m)
{
  Level->site[m->x][m->y].locchar = TRAP;
  lset(m->x, m->y, CHANGED, *Level);
  if(los_p(m->x, m->y, Player.x, Player.y))
  {
    if(m->uniqueness != COMMON)
    {
      queue_message(std::format("{} walked into teleport trap!", m->monstring));
    }
    else
    {
      queue_message(std::format("The {} walked into teleport trap!", m->monstring));
    }
  }
  m_teleport(m);
}

void m_trap_disintegrate(struct monster *m)
{
  if(los_p(m->x, m->y, Player.x, Player.y))
  {
    if(m->uniqueness != COMMON)
    {
      queue_message(std::format("{} walked into a disintegration trap!", m->monstring));
    }
    else
    {
      queue_message(std::format("The {} walked into a disintegration trap!", m->monstring));
    }
    Level->site[m->x][m->y].locchar = TRAP;
    lset(m->x, m->y, CHANGED, *Level);
  }
  disintegrate(m->x, m->y);
}

void m_trap_sleepgas(struct monster *m)
{
  if(los_p(m->x, m->y, Player.x, Player.y))
  {
    if(m->uniqueness != COMMON)
    {
      queue_message(std::format("{} walked into a sleepgas trap!", m->monstring));
    }
    else
    {
      queue_message(std::format("The {} walked into a sleepgas trap!", m->monstring));
    }
    Level->site[m->x][m->y].locchar = TRAP;
    lset(m->x, m->y, CHANGED, *Level);
  }
  if(!m_immunityp(*m, SLEEP))
  {
    m_status_reset(*m, AWAKE);
  }
}

void m_trap_acid(struct monster *m)
{
  if(los_p(m->x, m->y, Player.x, Player.y))
  {
    if(m->uniqueness != COMMON)
    {
      queue_message(std::format("{} walked into an acid bath trap!", m->monstring));
    }
    else
    {
      queue_message(std::format("The {} walked into an acid bath trap!", m->monstring));
    }
    Level->site[m->x][m->y].locchar = TRAP;
    lset(m->x, m->y, CHANGED, *Level);
  }
  m_damage(m, random_range(difficulty() * difficulty()), ACID);
}

void m_trap_manadrain(struct monster *m)
{
  if(los_p(m->x, m->y, Player.x, Player.y))
  {
    if(m->uniqueness != COMMON)
    {
      queue_message(std::format("{} walked into a manadrain trap!", m->monstring));
    }
    else
    {
      queue_message(std::format("The {} walked into a manadrain trap!", m->monstring));
    }
    Level->site[m->x][m->y].locchar = TRAP;
    lset(m->x, m->y, CHANGED, *Level);
  }
  if(m->specialf == M_SP_SPELL)
  {
    m->specialf = M_NO_OP;
  }
}

void m_water(struct monster *m)
{
  if((!m_statusp(*m, INTANGIBLE)) && (!m_statusp(*m, SWIMMING)) && (!m_statusp(*m, ONLYSWIM)))
  {
    if(los_p(m->x, m->y, Player.x, Player.y))
    {
      if(m->uniqueness != COMMON)
      {
        queue_message(std::format("{} drowned!", m->monstring));
      }
      else
      {
        queue_message(std::format("The {} drowned!", m->monstring));
      }
    }
    m_death(m);
  }
}

void m_abyss(struct monster *m)
{
  if(los_p(m->x, m->y, Player.x, Player.y))
  {
    if(m->uniqueness != COMMON)
    {
      queue_message(std::format("{} fell into the infinite abyss!", m->monstring));
    }
    else
    {
      queue_message(std::format("The {} fell into the infinite abyss!", m->monstring));
    }
  }
  m_vanish(m);
}

void m_lava(struct monster *m)
{
  if((!m_immunityp(*m, FLAME)) || ((!m_statusp(*m, SWIMMING)) && (!m_statusp(*m, ONLYSWIM))))
  {
    if(los_p(m->x, m->y, Player.x, Player.y))
    {
      if(m->uniqueness != COMMON)
      {
        queue_message(std::format("{} died in a pool of lava!", m->monstring));
      }
      else
      {
        queue_message(std::format("The {} died in a pool of lava!", m->monstring));
      }
    }
    m_death(m);
  }
}

void m_altar(struct monster *m)
{
  int visible  = view_los_p(Player.x, Player.y, m->x, m->y);
  int reaction = 0;
  int altar    = Level->site[m->x][m->y].aux;

  if(visible)
  {
    if(m->uniqueness != COMMON)
    {
      queue_message(std::format("{} walks next to an altar...", m->monstring));
    }
    else
    {
      queue_message(std::format("The {} walks next to an altar...", m->monstring));
    }
  }
  if(!m_statusp(*m, HOSTILE))
  {
    reaction = 0;
  }
  else if(m->id == HISCORE_NPC && m->aux2 == altar)
  {
    reaction = 1; /* high priest of same deity */
  }
  else if((m->id == ANGEL || m->id == HIGH_ANGEL || m->id == ARCHANGEL) && m->aux1 == altar)
  {
    reaction = 1; /* angel of same deity */
  }
  else if(altar == Player.patron)
  {
    reaction = -1; /* friendly deity will zap hostile monster */
  }
  else if(((Player.patron == ODIN || Player.patron == ATHENA) && (altar == SET || altar == HECATE)) ||
          ((Player.patron == SET || Player.patron == HECATE) && (altar == ODIN || altar == ATHENA)))
  {
    reaction = 1; /* hostile deity will help hostile monster */
  }
  switch(reaction)
  {
    case -1:
      if(visible)
      {
        mprint("Your deity is angry!");
        mprint("A bolt of godsfire strikes the monster....");
      }
      disrupt(m->x, m->y, Player.rank[PRIESTHOOD] * 50);
      break;
    case 1:
      if(visible)
      {
        mprint("The deity of the altar smiles on the monster....");
        mprint("A shaft of light zaps the altar...");
      }
      m->hp = Monsters[m->id].hp * 2;
      break;
    default:
      if(visible)
      {
        mprint("but nothing much seems to happen");
      }
      break;
  }
}

const char *mantype()
{
  switch(random_range(20))
  {
    case 0:
      return "janitor";
    case 1:
      return "beggar";
    case 2:
      return "barbarian";
    case 3:
      return "hairdresser";
    case 4:
      return "accountant";
    case 5:
      return "lawyer";
    case 6:
      return "indian chief";
    case 7:
      return "tinker";
    case 8:
      return "tailor";
    case 9:
      return "soldier";
    case 10:
      return "spy";
    case 11:
      return "doctor";
    case 12:
      return "miner";
    case 13:
      return "noble";
    case 14:
      return "serf";
    case 15:
      return "neer-do-well";
    case 16:
      return "vendor";
    case 17:
      return "dilettante";
    case 18:
      return "surveyor";
    default:
    case 19:
      return "jongleur";
  }
}

void strengthen_death(struct monster *m)
{
  pol ol     = ((pol)checkmalloc(sizeof(oltype)));
  pob scythe = ((pob)checkmalloc(sizeof(objtype)));
  m->xpv += std::min(10000l, m->xpv + 1000);
  m->hit += std::min(1000, m->hit + 10);
  m->dmg = std::min(10000, m->dmg * 2);
  m->ac += std::min(1000, m->ac + 10);
  m->speed       = std::max(m->speed - 1, 1);
  m->movef       = M_MOVE_SMART;
  m->hp          = std::min(100000, 100 + m->dmg * 10);
  *scythe        = Objects[WEAPONID + 39];
  ol->thing      = scythe;
  ol->next       = nullptr;
  m->possessions = ol;
}

void m_no_op(struct monster *) {}
