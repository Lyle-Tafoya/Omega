/* omega copyright (C) by Laurence Raphael Brothers, 1987,1988 */
/* omovef.c */
/* the movefunctions switch functions, for player and monster*/

#include "oglob.h"



void p_movefunction(movef)
int movef;
{
  /* loc functs above traps should be activated whether levitating or not */
  if (movef != L_NO_OP) clearmsg();
  sign_print(Player.x,Player.y,FALSE);
  if ((! Player.status[LEVITATING]) || 
      gamestatusp(MOUNTED) ||
      (Cmd == '@') || /* @ command activates all effects under player */
      (movef < LEVITATION_AVOIDANCE)) {

    switch(movef) {
      
      /* miscellaneous */
    case L_NO_OP:l_no_op(); break;
    case L_HEDGE:l_hedge(); break;
    case L_WATER:l_water(); break;
    case L_LIFT:l_lift(); break;
    case L_LAVA:l_lava(); break;
    case L_FIRE:l_fire(); break;
    case L_WHIRLWIND:l_whirlwind(); break;
    case L_RUBBLE:l_rubble(); break;
    case L_MAGIC_POOL:l_magic_pool(); break;
    case L_ABYSS:l_abyss(); break;
      
    case L_PORTCULLIS_TRAP:l_portcullis_trap(); break;
    case L_RAISE_PORTCULLIS:l_raise_portcullis(); break;
    case L_DROP_EVERY_PORTCULLIS:l_drop_every_portcullis(); break;
    case L_ARENA_EXIT:l_arena_exit(); break;
    case L_TRIFID:l_trifid(); break;
    case L_ENTER_COURT:l_enter_court(); break;
    case L_ESCALATOR:l_escalator(); break;
    case L_THRONE:l_throne(); break;

    case L_TRAP_DART:l_trap_dart(); break;
    case L_TRAP_SIREN:l_trap_siren(); break;
    case L_TRAP_PIT:l_trap_pit(); break;
    case L_TRAP_DOOR:l_trap_door(); break;
    case L_TRAP_SNARE:l_trap_snare(); break;
    case L_TRAP_BLADE:l_trap_blade(); break;
    case L_TRAP_FIRE:l_trap_fire(); break;
    case L_TRAP_TELEPORT:l_trap_teleport(); break;
    case L_TRAP_DISINTEGRATE:l_trap_disintegrate(); break;
    case L_TRAP_SLEEP_GAS:l_trap_sleepgas(); break;
    case L_TRAP_MANADRAIN:l_trap_manadrain(); break;
    case L_TRAP_ACID:l_trap_acid(); break;
    case L_TRAP_ABYSS:l_trap_abyss(); break;

   /*door functions */
    case L_BANK:l_bank(); break;
    case L_ARMORER:l_armorer(); break;
    case L_CLUB:l_club(); break;
    case L_GYM:l_gym(); break;
    case L_BROTHEL:l_brothel(); break;
    case L_THIEVES_GUILD:l_thieves_guild(); break;
    case L_COLLEGE:l_college(); break;
    case L_HEALER:l_healer(); break;
    case L_STATUE_WAKE:l_statue_wake(); break;
    case L_CASINO:l_casino(); break;
    case L_COMMANDANT:l_commandant(); break;
    case L_DINER:l_diner(); break;
    case L_CRAP:l_crap(); break;
    case L_TAVERN:l_tavern(); break;
    case L_MERC_GUILD:l_merc_guild(); break;
    case L_ALCHEMIST:l_alchemist(); break;
    case L_SORCERORS:l_sorcerors(); break;
    case L_CASTLE:l_castle(); break;
    case L_ARENA:l_arena(); break;
    case L_VAULT:l_vault(); break;
    case L_DPW:l_dpw(); break;
    case L_LIBRARY:l_library(); break;
    case L_PAWN_SHOP:l_pawn_shop(); break;
    case L_CONDO:l_condo(); break;
    case L_ALTAR:l_altar(); break;
    case L_TACTICAL_EXIT:l_tactical_exit(); break;
    case L_HOUSE_EXIT:l_house_exit(); break;
    case L_SAFE: l_safe(); break;
    case L_HOUSE: l_house(); break;
    case L_HOVEL: l_hovel(); break;
    case L_MANSION: l_mansion(); break;
    case L_COUNTRYSIDE:l_countryside(); break;
    case L_ORACLE:l_oracle(); break;
    case L_ORDER:l_order(); break;
    case L_CARTOGRAPHER:l_cartographer(); break;

    case L_TEMPLE_WARNING:l_temple_warning(); break;
    case L_ENTER_CIRCLE:l_enter_circle(); break;
    case L_CIRCLE_LIBRARY:l_circle_library(); break;
    case L_TOME1: l_tome1(); break;
    case L_TOME2: l_tome2(); break;

    case L_CHARITY: l_charity(); break;

    case L_CHAOSTONE:l_chaostone();break;
    case L_VOIDSTONE:l_voidstone();break;
    case L_BALANCESTONE:l_balancestone();break;
    case L_LAWSTONE:l_lawstone();break;
    case L_SACRIFICESTONE:l_sacrificestone(); break;
    case L_MINDSTONE:l_mindstone(); break;

    /* challenge functions */
    case L_ADEPT:l_adept(); break;
    case L_VOICE1:l_voice1();break;
    case L_VOICE2:l_voice2();break;
    case L_VOICE3:l_voice3();break;
    case L_VOID:l_void();break;
    case L_FIRE_STATION:l_fire_station();break;
    case L_EARTH_STATION:l_earth_station();break;
    case L_WATER_STATION:l_water_station();break;
    case L_AIR_STATION:l_air_station();break;
    case L_VOID_STATION:l_void_station();break;
    }
    if (movef != L_NO_OP) {
      resetgamestatus(FAST_MOVE);
      dataprint();
    }
  }
}





/* execute some move function for a monster */
void m_movefunction(m,movef)
struct monster *m;
int movef;
{
  /* loc functs above traps should be activated whether levitating or not */
  if (! m_statusp(m,FLYING))
    switch(movef) {

    /* miscellaneous */
    case L_NO_OP:m_no_op(m); break;
    case L_WATER:m_water(m); break;
    case L_LAVA:m_lava(m); break;
    case L_FIRE:m_fire(m); break;
    case L_MAGIC_POOL:m_water(m); break;
    case L_ABYSS: m_abyss(m); break;

    case L_TRAP_DART:m_trap_dart(m); break;
    case L_TRAP_PIT:m_trap_pit(m); break;
    case L_TRAP_DOOR:m_trap_door(m); break;
    case L_TRAP_SNARE:m_trap_snare(m); break;
    case L_TRAP_BLADE:m_trap_blade(m); break;
    case L_TRAP_FIRE:m_trap_fire(m); break;
    case L_TRAP_TELEPORT:m_trap_teleport(m); break;
    case L_TRAP_DISINTEGRATE:m_trap_disintegrate(m); break;
    case L_TRAP_MANADRAIN:m_trap_manadrain(m); break;
    case L_TRAP_SLEEP_GAS:m_trap_sleepgas(m); break;
    case L_TRAP_ACID:m_trap_acid(m); break;
    case L_TRAP_ABYSS:m_trap_abyss(m);break;

    case L_ALTAR:m_altar(m); break;
    }
}
