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

/* priest.c */
/* functions for clerics. */

#include "glob.h"
#include "scr.h"
#include "spell.h"

#include <algorithm>
#include <string>

extern void item_equip(object *);
extern void item_unequip(object *);

/* prayer occurs at altars, hence name of function */
void l_altar()
{
  int  i, deity;
  char response;

  if(Current_Environment == E_COUNTRYSIDE)
  {
    deity = DRUID;
  }
  else
  {
    deity = Level->site[Player.x][Player.y].aux;
  }

  switch(deity)
  {
    default:
      queue_message("This rude altar has no markings.");
      break;
    case ODIN:
      queue_message("This granite altar is graven with a gallows.");
      break;
    case SET:
      queue_message("This sandstone altar has a black hand drawn on it.");
      break;
    case HECATE:
      queue_message("This silver altar is inlaid with a black crescent moon.");
      break;
    case ATHENA:
      queue_message("This golden altar is inscribed with an owl.");
      break;
    case DESTINY:
      queue_message("This crystal altar is in the form of an omega.");
      break;
    case DRUID:
      queue_message("This oaken altar is ornately engraved with leaves.");
      break;
  }
  bool confirm;
  if(optionp(PARANOID_CONFIRM, Player))
  {
    queue_message("Worship at this altar? (yes) [no] ");
    confirm = (msgscanstring() == "yes");
  }
  else
  {
    queue_message("Worship at this altar? [yn] ");
    confirm = (ynq() == 'y');
  }
  if(confirm)
  {
    if(Player.rank[MONKS] > 0)
    {
      queue_message("A monk seeks truth within.");
    }
    else if(Player.rank[PRIESTHOOD] == 0)
    {
      increase_priest_rank(deity);
    }
    else if(!check_sacrilege(deity))
    {
      if(Blessing)
      {
        queue_message("You have a sense of immanence.");
      }
      queue_message("Request a Blessing, Sacrifice an item, or just Pray [b,s,p] ");
      do
      {
        response = (char)mcigetc();
      } while((response != 'b') && (response != 's') && (response != 'p') && (response != ESCAPE));
      if(response == 'b')
      {
        queue_message("You beg a heavenly benefice.");
        queue_message("You hear a gong resonating throughout eternity....");
        if(Blessing)
        {
          queue_message("A shaft of lucent radiance lances down from the heavens!");
          queue_message("You feel uplifted....");
          gain_experience(Player.rank[PRIESTHOOD] * Player.rank[PRIESTHOOD] * 50);
          cleanse(1);
          heal(10);
          bless(1);
          Blessing = false;
          increase_priest_rank(deity);
        }
        else
        {
          queue_message("Your ardent plea is ignored.");
          queue_message("You feel ashamed.");
          Player.xp -= (Player.xp / 4);
        }
        calc_melee();
      }
      else if(response == 's')
      {
        queue_message("Which item to Sacrifice?");
        i = getitem(NULL_ITEM);
        if(i == ABORT)
        {
          i = 0;
        }
        if(!Player.possessions[i])
        {
          queue_message("You have insulted your deity!");
          queue_message("Not a good idea, as it turns out...");
          dispel(-1);
          p_damage(Player.hp - 1, UNSTOPPABLE, "a god's pique");
        }
        else if(true_item_value(Player.possessions[i]) >
                (long)(Player.rank[PRIESTHOOD] * Player.rank[PRIESTHOOD] * Player.rank[PRIESTHOOD] * 50))
        {
          queue_message("With a burst of blue flame, your offering vanishes!");
          dispose_lost_objects(1, Player.possessions[i]);
          queue_message("A violet nimbus settles around your head and slowly fades.");
          Blessing = true;
        }
        else
        {
          queue_message("A darkling glow envelopes your offering!");
          queue_message("The glow slowly fades....");
          setgamestatus(SUPPRESS_PRINTING, GameStatus);
          if(Player.possessions[i]->used)
          {
            item_unequip(Player.possessions[i]);
            Player.possessions[i]->blessing = -1 - abs(Player.possessions[i]->blessing);
            item_equip(Player.possessions[i]);
          }
          else
          {
            Player.possessions[i]->blessing = -1 - abs(Player.possessions[i]->blessing);
          }
          resetgamestatus(SUPPRESS_PRINTING, GameStatus);
        }
      }
      else if(response == 'p')
      {
        if(deity != Player.patron)
        {
          queue_message("Nothing seems to happen.");
        }
        else if(!increase_priest_rank(deity))
        {
          answer_prayer();
        }
      }
    }
  }
}

int check_sacrilege(int deity)
{
  int sacrilege = false;
  if((Player.patron != deity) && (Player.patron > 0))
  {
    sacrilege = true;
    Player.pow--;
    Player.maxpow--;
    switch(Player.patron)
    {
      case ODIN:
        queue_message("Odin notices your lack of faith! ");
        if(deity == ATHENA)
        {
          queue_message("However, Athena intercedes on your behalf.");
          sacrilege = false;
        }
        else
        {
          queue_message("You are struck by a thunderbolt!");
          p_damage(Player.level * 5, UNSTOPPABLE, "Odin's wrath");
          if(Player.hp > 0)
          {
            queue_message("The bolt warps your feeble frame....");
            Player.maxcon = Player.maxcon / 2;
            Player.con    = std::min(Player.con, Player.maxcon);
            Player.maxstr = Player.maxstr / 2;
            Player.con    = std::min(Player.str, Player.maxstr);
          }
        }
        break;
      case SET:
        queue_message("Set notices your lack of faith! ");
        if(deity == HECATE)
        {
          queue_message("But since you pray to a friendly deity,");
          queue_message("Set decides not to punish you.");
          sacrilege = false;
        }
        else
        {
          queue_message("You are blasted by a shaft of black fire!");
          p_damage(Player.level * 5, UNSTOPPABLE, "Set's anger");
          if(Player.hp > 0)
          {
            queue_message("You are wreathed in clouds of smoke.");
            for(int i = 0; i < MAXITEMS; ++i)
            {
              if(Player.possessions[i] && Player.possessions[i]->blessing > -1)
              {
                conform_lost_object(Player.possessions[i]);
              }
            }
            queue_message("You feel Set's Black Hand on your heart....");
            Player.con = Player.maxcon = Player.maxcon / 4;
          }
        }
        break;
      case HECATE:
        queue_message("Hecate notices your lack of faith! ");
        if(deity == SET)
        {
          queue_message("But ignores the affront since she likes Set.");
          sacrilege = false;
        }
        else
        {
          queue_message("You are zapped by dark moonbeams!");
          p_damage(Player.level * 5, UNSTOPPABLE, "Hecate's malice");
          if(Player.hp > 0)
          {
            queue_message("The beams leach you of magical power!");
            Player.maxpow = Player.maxpow / 5;
            Player.pow    = std::min(Player.pow, Player.maxpow);
            for(int i = 0; i < spell::NUM_SPELLS; ++i)
            {
              spell::Spells[i].known = false;
            }
          }
        }
        break;
      case ATHENA:
        queue_message("Athena notices your lack of faith! ");
        if(deity == ODIN)
        {
          queue_message("But lets you off this time since Odin is also Lawful.");
          sacrilege = false;
        }
        else
        {
          queue_message("You are zorched by godsfire!");
          if(Player.hp > 0)
          {
            queue_message("The fire burns away your worldly experience!");
            Player.level = 0;
            Player.xp    = 0;
            Player.maxhp = Player.hp = Player.con;
            queue_message("Your power is reduced by the blast!!!");
            Player.pow = Player.maxpow = Player.maxpow / 3;
            Player.mana                = std::min(Player.mana, calcmana());
          }
        }
        break;
      case DESTINY:
        queue_message("The Lords of Destiny ignore your lack of faith.");
        sacrilege = false;
        break;
      case DRUID:
        queue_message("Your treachery to the ArchDruid has been noted.");
        if(random_range(2) == 1)
        {
          Player.alignment += 40;
        }
        else
        {
          Player.alignment -= 40;
        }
        break;
    }
    if(sacrilege)
    {
      Player.patron           = 0;
      Player.rank[PRIESTHOOD] = 0;
    }
  }
  return (sacrilege);
}

int increase_priest_rank(int deity)
{
  if(Player.rank[PRIESTHOOD] == 0)
  {
    switch(deity)
    {
      default:
        queue_message("Some nameless god blesses you....");
        Player.hp = std::max(Player.hp, Player.maxhp);
        queue_message("The altar crumbles to dust and blows away.");
        Level->site[Player.x][Player.y].locchar = FLOOR;
        Level->site[Player.x][Player.y].p_locf  = L_NO_OP;
        lset(Player.x, Player.y, CHANGED, *Level);
        break;
      case ODIN:
        if(Player.alignment > 0)
        {
          queue_message("Odin hears your prayer!");
          queue_message(Priest[ODIN] + " personally blesses you.");
          queue_message("You are now a lay devotee of Odin.");
          Player.patron              = ODIN;
          Player.rank[PRIESTHOOD]    = LAY;
          Player.guildxp[PRIESTHOOD] = 1;
          learnclericalspells(ODIN, LAY);
        }
        else
        {
          queue_message("Odin ignores you.");
        }
        break;
      case SET:
        if(Player.alignment < 0)
        {
          queue_message("Set hears your prayer!");
          queue_message(Priest[SET] + " personally blesses you.");
          queue_message("You are now a lay devotee of Set.");
          Player.patron              = SET;
          Player.rank[PRIESTHOOD]    = LAY;
          Player.guildxp[PRIESTHOOD] = 1;
          learnclericalspells(SET, LAY);
        }
        else
        {
          queue_message("Set ignores you.");
        }
        break;
      case ATHENA:
        if(Player.alignment > 0)
        {
          queue_message("Athena hears your prayer!");
          queue_message(Priest[ATHENA] + " personally blesses you.");
          queue_message("You are now a lay devotee of Athena.");
          Player.patron              = ATHENA;
          Player.rank[PRIESTHOOD]    = LAY;
          Player.guildxp[PRIESTHOOD] = 1;
          learnclericalspells(ATHENA, LAY);
        }
        else
        {
          queue_message("Athena ignores you.");
        }
        break;
      case HECATE:
        if(Player.alignment < 0)
        {
          queue_message("Hecate hears your prayer!");
          queue_message(Priest[HECATE] + " personally blesses you.");
          queue_message("You are now a lay devotee of Hecate.");
          Player.patron              = HECATE;
          Player.rank[PRIESTHOOD]    = LAY;
          Player.guildxp[PRIESTHOOD] = 1;
          learnclericalspells(HECATE, LAY);
        }
        else
        {
          queue_message("Hecate ignores you.");
        }
        break;
      case DRUID:
        if(abs(Player.alignment) < 10)
        {
          queue_message(Priest[DRUID] + " personally blesses you.");
          queue_message("You are now a lay devotee of the Druids.");
          Player.patron              = DRUID;
          Player.rank[PRIESTHOOD]    = LAY;
          Player.guildxp[PRIESTHOOD] = 1;
          learnclericalspells(DRUID, LAY);
        }
        else
        {
          queue_message("You hear a voice....");
          queue_message("'Only those who embody the Balance may become Druids.'");
        }
        break;
      case DESTINY:
        queue_message("The Lords of Destiny could hardly care less.");
        queue_message("But you can consider yourself now to be a lay devotee.");
        Player.patron              = DESTINY;
        Player.rank[PRIESTHOOD]    = LAY;
        Player.guildxp[PRIESTHOOD] = 1;
        break;
    }
  }
  else if(deity == Player.patron)
  {
    if((((deity == ODIN) || (deity == ATHENA)) && (Player.alignment < 1)) ||
       (((deity == SET) || (deity == HECATE)) && (Player.alignment > 1)) ||
       ((deity == DRUID) && (abs(Player.alignment) > 10)))
    {
      queue_message("You have swerved from the One True Path!");
      queue_message("Your deity is greatly displeased...");
      Player.xp -= Player.level * Player.level;
      Player.xp = std::max(0l, Player.xp);
    }
    else if(Player.rank[PRIESTHOOD] == HIGHPRIEST)
    {
      return 0;
    }
    else if(Player.rank[PRIESTHOOD] == SPRIEST)
    {
      if(Player.level > Priestlevel[deity])
      {
        hp_req_test();
      }
      else
      {
        return 0;
      }
    }
    else if(Player.rank[PRIESTHOOD] == PRIEST)
    {
      if(Player.guildxp[PRIESTHOOD] >= 4000)
      {
        queue_message("An heavenly fanfare surrounds you!");
        queue_message("Your deity raises you to the post of Senior Priest.");
        hp_req_print();
        Player.rank[PRIESTHOOD] = SPRIEST;
        learnclericalspells(deity, SPRIEST);
      }
      else
      {
        return 0;
      }
    }
    else if(Player.rank[PRIESTHOOD] == ACOLYTE)
    {
      if(Player.guildxp[PRIESTHOOD] >= 1500)
      {
        queue_message("A trumpet sounds in the distance.");
        queue_message("Your deity raises you to the post of Priest.");
        Player.rank[PRIESTHOOD] = PRIEST;
        learnclericalspells(deity, PRIEST);
      }
      else
      {
        return 0;
      }
    }
    else if(Player.rank[PRIESTHOOD] == LAY)
    {
      if(Player.guildxp[PRIESTHOOD] >= 400)
      {
        queue_message("A mellifluous chime sounds from above the altar.");
        queue_message("Your deity raises you to the post of Acolyte.");
        Player.rank[PRIESTHOOD] = ACOLYTE;
        learnclericalspells(deity, ACOLYTE);
      }
      else
      {
        return 0;
      }
    }
  }
  return 1;
}

void answer_prayer()
{
  switch(random_range(12))
  {
    case 0:
      queue_message("You have a revelation!");
      break;
    case 1:
      queue_message("You feel pious.");
      break;
    case 2:
      queue_message("A feeling of sanctity comes over you.");
      break;
    default:
      queue_message("Nothing unusual seems to happen.");
      break;
  }
}

void hp_req_test()
{
  pob o;
  switch(Player.patron)
  {
    case ODIN:
      if(find_item(&o, ARTIFACTID + 15, -1))
      {
        make_hp(o);
      }
      else
      {
        hp_req_print();
      }
      break;
    case SET:
      if(find_item(&o, ARTIFACTID + 14, -1))
      {
        make_hp(o);
      }
      else
      {
        hp_req_print();
      }
      break;
    case ATHENA:
      if(find_item(&o, ARTIFACTID + 17, -1))
      {
        make_hp(o);
      }
      else
      {
        hp_req_print();
      }
      break;
    case HECATE:
      if(find_item(&o, ARTIFACTID + 16, -1))
      {
        make_hp(o);
      }
      else
      {
        hp_req_print();
      }
      break;
    case DRUID:
      if(find_item(&o, ARTIFACTID + 14, -1))
      {
        make_hp(o);
      }
      else if(find_item(&o, ARTIFACTID + 15, -1))
      {
        make_hp(o);
      }
      else if(find_item(&o, ARTIFACTID + 16, -1))
      {
        make_hp(o);
      }
      else if(find_item(&o, ARTIFACTID + 17, -1))
      {
        make_hp(o);
      }
      else
      {
        hp_req_print();
      }
      break;
    case DESTINY:
      if(find_item(&o, ARTIFACTID + 19, -1))
      {
        make_hp(o);
      }
      else
      {
        hp_req_print();
      }
      break;
  }
}

void hp_req_print()
{
  queue_message("To advance further, you must obtain the Holy Symbol of ");
  switch(Player.patron)
  {
    case ODIN:
      queue_message(Priest[SET]);
      queue_message("who may be found in the main Temple of Set.");
      break;
    case SET:
      queue_message(Priest[ODIN]);
      queue_message("who may be found in the main Temple of Odin.");
      break;
    case ATHENA:
      queue_message(Priest[HECATE]);
      queue_message("who may be found in the main Temple of Hecate.");
      break;
    case HECATE:
      queue_message(Priest[ATHENA]);
      queue_message("who may be found in the main Temple of Athena.");
      break;
    case DRUID:
      queue_message("any of the aligned priests who may be found in their main Temples.");
      break;
    case DESTINY:
      queue_message(Priest[DESTINY]);
      queue_message("who may be found in the main Temple of Destiny.");
      break;
  }
}

void make_hp(pob o)
{
  queue_message("A full-scale heavenly choir chants 'Hallelujah' all around you!");
  queue_message("You notice a change in the symbol you carry....");
  switch(Player.patron)
  {
    case ODIN:
      *o = Objects[ARTIFACTID + 14];
      break;
    case SET:
      *o = Objects[ARTIFACTID + 15];
      break;
    case ATHENA:
      *o = Objects[ARTIFACTID + 16];
      break;
    case HECATE:
      *o = Objects[ARTIFACTID + 17];
      break;
    case DRUID:
      *o = Objects[ARTIFACTID + 18];
      break;
    case DESTINY:
      *o = Objects[ARTIFACTID + 19];
      break;
  }
  o->known  = 2;
  o->charge = 17; /* random hack to convey bit that symbol is functional */
  if(Player.patron == DRUID)
  {
    queue_message("Your deity raises you to the post of ArchDruid!");
  }
  else
  {
    queue_message("Your deity raises you to the post of High Priest!");
  }
  queue_message("You feel holy.");
  Priest[Player.patron] = Player.name;
  Priestlevel[Player.patron]    = Player.level;
  Player.rank[PRIESTHOOD]       = HIGHPRIEST;
  Priestbehavior[Player.patron] = fixnpc(4);
  save_hiscore_npc(Player.patron);
  learnclericalspells(Player.patron, HIGHPRIEST);
}
