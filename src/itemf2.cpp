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

/* itemf2.c */

/* mostly ring, armor, and weapon functions */

#include "glob.h"
#include "scr.h"

#include <algorithm>

/* ring functions */
void i_perm_knowledge(std::unique_ptr<object> &o)
{
  if(o->known < 1)
  {
    o->known = 1;
  }
  if(o->blessing > -1)
  {
    Objects[o->id].known = 1;
  }
  if(o->used)
  {
    knowledge(o->blessing);
  }
}

void i_perm_strength(std::unique_ptr<object> &o)
{
  if(o->known < 1)
  {
    o->known = 1;
  }
  Objects[o->id].known = 1;
  if(o->used)
  {
    if(o->blessing > -1)
    {
      Player.str += abs(o->plus) + 1;
    }
    else
    {
      Player.str -= abs(o->plus) + 1;
    }
  }
  else
  {
    if(o->blessing > -1)
    {
      Player.str -= abs(o->plus) + 1;
    }
    else
    {
      Player.str += abs(o->plus) + 1;
    }
  }
  calc_melee();
}

void i_perm_burden(std::unique_ptr<object> &o)
{
  if(o->used)
  {
    o->weight = 1000;
    queue_message("You feel heavier.");
  }
  else
  {
    o->weight = 1;
    queue_message("Phew. What a relief.");
  }
  Player.itemweight = 0;
  for(int i = 0; i < MAXITEMS; ++i)
  {
    if(Player.possessions[i])
    {
      Player.itemweight += (Player.possessions[i]->weight * Player.possessions[i]->number);
    }
  }
}

void i_perm_gaze_immune(std::unique_ptr<object> &o)
{
  if(o->used)
  {
    Player.immunity[GAZE]++;
  }
  else
  {
    Player.immunity[GAZE]--;
  }
}

void i_perm_fire_resist(std::unique_ptr<object> &o)
{
  if(o->used)
  {
    Player.immunity[FLAME]++;
  }
  else
  {
    Player.immunity[FLAME]--;
  }
}

void i_perm_poison_resist(std::unique_ptr<object> &o)
{
  if(o->used)
  {
    if(o->blessing < 0)
    {
      Player.immunity[POISON] = 0;
      p_poison(100);
    }
    else
    {
      Player.immunity[POISON]++;
      if(Player.status[POISONED] > 0)
      {
        queue_message("You feel much better now.");
        Player.status[POISONED] = 0;
      }
    }
  }
  else
  {
    Player.status[POISONED] = 0;
    Player.immunity[POISON]--;
  }
}

void i_perm_regenerate(std::unique_ptr<object> &o)
{
  if(o->known < 1)
  {
    o->known = 1;
  }
  if(o->blessing > -1)
  {
    Objects[o->id].known = 1;
  }
  if(o->used)
  {
    queue_message("You seem abnormally healthy.");
    Player.status[REGENERATING] += 1500;
  }
  else
  {
    Player.status[REGENERATING] -= 1500;
    if(Player.status[REGENERATING] < 1)
    {
      queue_message("Your vitality is back to normal");
      Player.status[REGENERATING] = 0;
    }
  }
}

/* armor functions */

void i_normal_armor(std::unique_ptr<object> &o)
{
  if(o->used)
  {
    queue_message("You put on your suit of armor.");
  }
}

void i_perm_energy_resist(std::unique_ptr<object> &o)
{
  if(o->used)
  {
    Player.immunity[FLAME]++;
    Player.immunity[COLD]++;
    Player.immunity[ELECTRICITY]++;
  }
  else
  {
    Player.immunity[FLAME]--;
    Player.immunity[COLD]--;
    Player.immunity[ELECTRICITY]--;
  }
}

void i_perm_fear_resist(std::unique_ptr<object> &o)
{
  if(o->used)
  {
    Player.immunity[FEAR]++;
    if(o->blessing < 0)
    {
      Player.status[BERSERK] += 1500;
      queue_message("You feel blood-simple!");
    }
  }
  else
  {
    Player.immunity[FEAR]--;
    if(o->blessing < 0)
    {
      Player.status[BERSERK] -= 1500;
      if(Player.status[BERSERK] < 1)
      {
        queue_message("You feel less rabid now.");
        Player.status[BERSERK] = 0;
      }
    }
  }
}

void i_perm_breathing(std::unique_ptr<object> &o)
{
  if(o->known < 1)
  {
    o->known = 1;
  }
  if(o->blessing > -1)
  {
    Objects[o->id].known = 1;
  }

  if(o->blessing > -1)
  {
    if(o->used)
    {
      queue_message("Your breath is energized!");
      Player.status[BREATHING] += 1500;
    }
    else
    {
      Player.status[BREATHING] -= 1500;
      if(Player.status[BREATHING] < 1)
      {
        queue_message("Your breathing is back to normal.");
        Player.status[BREATHING] = 0;
      }
    }
  }
  else if(o->used)
  {
    Player.status[BREATHING] = 0;
    p_drown();
    queue_message("Water pours from the broken suit.");
  }
}

// weapons functions

void weapon_hit(object *weapon, monster *m, int damage_modifier, damage_type damage_type)
{
  if(weapon->used)
  {
    p_hit(m, Player.dmg + damage_modifier, damage_type);
  }
  else
  {
    object *weapon_hand_object = Player.possessions[O_WEAPON_HAND].get();
    if(weapon_hand_object && weapon_hand_object->used)
    {
      p_hit(
        m,
        Player.dmg - weapon_hand_object->dmg - weapon_hand_object->plus + damage_modifier + weapon->dmg +
          weapon->plus,
        damage_type
      );
    }
    else
    {
      p_hit(m, Player.dmg + damage_modifier + weapon->dmg + weapon->plus, damage_type);
    }
  }
}

void weapon_acidwhip(int dmgmod, std::unique_ptr<object> &o, monster *m)
{
  if((random_range(2) == 1) && (!m_immunityp(*m, NORMAL_DAMAGE)))
  {
    queue_message("You entangle the monster!");
    m_status_reset(*m, MOBILE);
  }
  weapon_hit(o.get(), m, dmgmod, ACID);
}

void weapon_scythe(int, std::unique_ptr<object> &, monster *m)
{
  queue_message("Slice!");
  m_death(m);
  if(!Player.rank[ADEPT])
  {
    queue_message("Ooops!");
    queue_message("You accidentally touch yourself on the backswing....");
    p_death("the Scythe of Death");
  }
}

void weapon_demonblade(int dmgmod, std::unique_ptr<object> &o, monster *m)
{
  if(o->blessing > -1)
  {
    queue_message("Demonblade disintegrates with a soft sigh.");
    queue_message("You stop foaming at the mouth.");
    Player.status[BERSERK] = 0;
    dispose_lost_objects(o->number, o);
  }
  else if(m->specialf == M_SP_DEMON)
  {
    queue_message("The demon flees in terror before your weapon!");
    m_vanish(m);
  }
  else if(m->meleef != M_MELEE_SPIRIT)
  {
    if(m->level > random_range(10))
    {
      if(Player.hp < Player.maxhp)
      {
        Player.hp = std::min(Player.maxhp, Player.hp + m->hp);
      }
      Player.str++;
      if(Player.pow < Player.maxpow)
      {
        Player.pow = std::min(Player.maxpow, Player.pow + m->level);
      }
      m_death(m);
      queue_message("You feel a surge of raw power from Demonblade!");
    }
    else
    {
      weapon_hit(o.get(), m, dmgmod, NORMAL_DAMAGE);
    }
  }
  else
  {
    queue_message("Demonblade howls as it strikes the spirit!");
    if(random_range(10) == 1)
    {
      queue_message("... and shatters into a thousand lost fragments!");
      p_damage(50, UNSTOPPABLE, "Demonblade exploding");
      dispose_lost_objects(o->number, o);
    }
    else
    {
      queue_message("You feel your lifeforce draining....");
      p_damage(25, UNSTOPPABLE, "a backlash of negative energies");
      Player.str -= 3;
      Player.con -= 3;
      if(Player.str < 1 || Player.con < 1)
      {
        p_death("a backlash of negative energies");
      }
    }
  }
}

void weapon_lightsabre(int, std::unique_ptr<object> &o, monster *m)
{
  if(!o->known)
  {
    queue_message("Fumbling with the cylinder, you press the wrong stud....");
    p_damage(100, UNSTOPPABLE, "fumbling with a lightsabre");
    o->known = 1;
  }
  else
  {
    /* test prevents confusing immunity messages.... */
    if(!m_immunityp(*m, NORMAL_DAMAGE))
    {
      queue_message("Vzzzzmmm!");
      m_damage(m, 20, NORMAL_DAMAGE);
    }
    if((m->hp > 0) && (!m_immunityp(*m, FLAME)))
    {
      queue_message("Zzzzap!");
      m_damage(m, 20, FLAME);
    }
  }
}

void weapon_tangle(int dmgmod, std::unique_ptr<object> &o, monster *m)
{
  if((random_range(2) == 1) && (!m_immunityp(*m, NORMAL_DAMAGE)))
  {
    queue_message("You entangle the monster!");
    m_status_reset(*m, MOBILE);
  }
  weapon_hit(o.get(), m, dmgmod, NORMAL_DAMAGE);
}

/* if wielding a bow, add bow damage to arrow damage */
void weapon_arrow(int dmgmod, std::unique_ptr<object> &o, monster *m)
{
  if(Player.possessions[O_WEAPON_HAND] && (Player.possessions[O_WEAPON_HAND]->id == WEAPONID + 26))
  { /* ie, using a bow */
    p_hit(m, Player.dmg + o->plus + o->dmg + dmgmod, NORMAL_DAMAGE);
  }
  else
  {
    p_hit(m, o->plus + o->dmg + dmgmod, NORMAL_DAMAGE);
  }
}

/* if wielding a crossbow, add bow damage to arrow damage */
void weapon_bolt(int dmgmod, std::unique_ptr<object> &o, monster *m)
{
  if(Player.possessions[O_WEAPON_HAND] &&
     (Player.possessions[O_WEAPON_HAND]->id == WEAPONID + 27) && /*ie using a crossbow */
     (Player.possessions[O_WEAPON_HAND]->aux == LOADED))
  {
    p_hit(m, Player.dmg + o->plus + o->dmg + dmgmod, NORMAL_DAMAGE);
    Player.possessions[O_WEAPON_HAND]->aux = UNLOADED;
  }
  else
  {
    p_hit(m, o->plus + o->dmg, NORMAL_DAMAGE);
  }
}

void weapon_mace_disrupt(int dmgmod, std::unique_ptr<object> &o, monster *m)
{
  if(m->meleef == M_MELEE_SPIRIT)
  {
    queue_message("The monster crumbles away to dust!");
    m_death(m);
  }
  else
  {
    weapon_hit(o.get(), m, dmgmod, UNSTOPPABLE);
  }
}

void weapon_normal_hit(int dmgmod, std::unique_ptr<object> &o, monster *m)
{
  weapon_hit(o.get(), m, dmgmod, NORMAL_DAMAGE);
}

/* will be updated eventually */
void weapon_bare_hands(int dmgmod, monster *m)
{
  p_hit(m, Player.dmg + dmgmod, NORMAL_DAMAGE);
}

void i_demonblade(std::unique_ptr<object> &o)
{
  if(o->used)
  {
    o->known = 2;
    queue_message("Demonblade's fangs open and bury themselves in your wrist!");
    queue_message("You hear evil laughter in the distance....");
    queue_message("You begin to foam at the mouth!");
    Player.status[BERSERK] = 1500;
  }
  else
  {
    queue_message("You hear a sound like a demon cursing.");
    queue_message("You feel less like biting your shield.");
    Player.status[BERSERK] = 0;
  }
}

void i_normal_weapon(std::unique_ptr<object> &o)
{
  if(o->used)
  {
    queue_message("You ready your weapon for battle.");
  }
}

void i_lightsabre(std::unique_ptr<object> &o)
{
  if(o->used)
  {
    queue_message("You feel one with the Force.");
  }
  else
  {
    queue_message("You feel out of touch with the Force.");
  }
}

void i_mace_disrupt(std::unique_ptr<object> &)
{
  queue_message("That's a damned heavy mace!");
}

void weapon_vorpal(int dmgmod, std::unique_ptr<object> &o, monster *m)
{
  if((random_range(10) < 3) && (!m_immunityp(*m, NORMAL_DAMAGE)))
  {
    o->known = 2;
    if(random_range(2) == 1)
    {
      queue_message("One Two! One Two! And through and through!");
    }
    else
    {
      queue_message("Your vorpal blade goes snicker-snack!");
    }
    m_death(m);
  }
  else
  {
    weapon_normal_hit(dmgmod, o, m);
  }
}

void weapon_desecrate(int dmgmod, std::unique_ptr<object> &o, monster *m)
{
  o->known = 2;
  if(Player.alignment < 0)
  {
    queue_message("Your opponent screams in agony!");
    weapon_hit(o.get(), m, dmgmod, UNSTOPPABLE);
    Player.alignment--;
    if(Player.hp < Player.maxhp)
    {
      queue_message("You feel a thrill of power surging up your blade!");
      Player.hp = std::min(Player.maxhp, Player.hp + Player.dmg + dmgmod);
    }
  }
  else
  {
    queue_message("Your blade turns in your hands and hits you!");
    queue_message("You hear evil laughter....");
    level_drain(Player.dmg, "the sword Desecrator");
    Player.alignment -= 10;
    queue_message("A strange force spreads from the wound throughout your body...");
    queue_message("You feel much more chaotic now.");
  }
}

void weapon_firestar(int dmgmod, std::unique_ptr<object> &o, monster *m)
{
  if(random_range(3) == 1)
  {
    o->known = 2;
    fball(Player.x, Player.y, Player.x, Player.y, std::max(Player.dmg, 25));
  }
  if(m->hp > 0)
  {
    weapon_normal_hit(dmgmod, o, m);
  }
}

void weapon_defend(int dmgmod, std::unique_ptr<object> &o, monster *m)
{
  if((Player.alignment < 0) && (o->blessing > 0))
  {
    queue_message("The Holy Defender screams in your hands....");
    queue_message("You stagger before the sound of its rage....");
    p_damage(50, UNSTOPPABLE, "a pissed-off Holy Defender");
    queue_message("The weapon finally quiets. It seems less holy now.");
    o->truename = o->cursestr;
    Player.status[PROTECTION] -= (o->hit);
    o->plus     = 0 - abs(o->plus);
    o->blessing = -1;
  }
  if((o->blessing > 0) && ((m->specialf == M_SP_DEMON) || (m->meleef == M_MELEE_SPIRIT)))
  {
    queue_message("Your opponent shies back before your holy weapon!");
    m->hit = 0;
    m->speed *= 2;
  }
  weapon_normal_hit(dmgmod, o, m);
}

void weapon_victrix(int dmgmod, std::unique_ptr<object> &o, monster *m)
{
  if(m->meleef == M_MELEE_SPIRIT)
  {
    queue_message("Your opponent dissipates in a harmless cloud of vapors...");
    m_death(m);
  }
  else
  {
    weapon_normal_hit(dmgmod, o, m);
  }
}

void i_defend(std::unique_ptr<object> &o)
{
  o->known = 2;
  if(o->used)
  {
    queue_message("You feel under an aegis!");
    Player.status[PROTECTION] += o->hit;
  }
  else
  {
    Player.status[PROTECTION] -= o->hit;
  }
}

void i_victrix(std::unique_ptr<object> &o)
{
  o->known    = 2;
  o->blessing = abs(o->blessing);
  if(o->used)
  {
    Player.immunity[POISON]++;
    Player.immunity[FEAR]++;
    Player.immunity[INFECTION]++;
  }
  else
  {
    Player.immunity[POISON]--;
    Player.immunity[FEAR]--;
    Player.immunity[INFECTION]--;
  }
}

void i_desecrate(std::unique_ptr<object> &o)
{
  if(o->known < 1)
  {
    o->known = 2;
  }
  if(o->blessing > 0)
  {
    queue_message("How weird, a blessed desecrator... ");
    queue_message("The structure of reality cannot permit such a thing....");
    dispose_lost_objects(1, o);
  }
  else if(Level->site[Player.x][Player.y].locchar == ALTAR)
  {
    sanctify(-1);
  }
}

/* shield functions */
void i_normal_shield(std::unique_ptr<object> &o)
{
  if(o->used)
  {
    queue_message("You sling your shield across a forearm.");
  }
}

void i_perm_deflect(std::unique_ptr<object> &o)
{
  if(o->known < 1)
  {
    o->known = 2;
  }
  if(o->blessing > -1)
  {
    if(o->used)
    {
      queue_message("You feel buffered.");
      Player.status[DEFLECTION] += 1500;
    }
    else
    {
      Player.status[DEFLECTION] -= 1500;
      if(Player.status[DEFLECTION] < 1)
      {
        queue_message("You feel less defended");
        Player.status[DEFLECTION] = 0;
      }
    }
  }
  else
  {
    if(o->used)
    {
      queue_message("You feel naked.");
      Player.status[VULNERABLE] += 1500;
      Player.status[DEFLECTION] = 0;
    }
    else
    {
      Player.status[VULNERABLE] -= 1500;
      if(Player.status[VULNERABLE] < 1)
      {
        queue_message("You feel less vulnerable");
        Player.status[VULNERABLE] = 0;
      }
    }
  }
}
