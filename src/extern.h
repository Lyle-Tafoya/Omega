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

// extern.h -- function declarations

#ifndef OMEGA_EXTERN_H_
#define OMEGA_EXTERN_H_

#include "defs.h"

#include <string>
#include <string_view>

// omega.cpp functions
void init_world(void);
void inititem(int);
void initrand(int, int);

// abyss.cpp functions
void load_abyss(void);

// aux1.cpp functions
std::string levelname(int);
std::string mstatus_string(monster *);
std::string trapid(int);
int         getdir(void);
bool        goberserk(void);
int         p_country_moveable(int, int);
int         p_moveable(int, int);
int         player_on_sanctuary(void);
void        calc_melee(void);
int         damage_item(pob);
std::string describe_player(void);
void        foodcheck(void);
void        gain_experience(int);
void        p_damage(int, int, const std::string &);
void        p_death(const std::string &);
void        roomcheck(void);
void        searchat(int, int);
void        setspot(int &, int &);
void        showroom(int);
void        surrender(monster *);
void        threaten(monster *);
void        tunnelcheck(void);

// aux2.cpp functions
long        item_value(pob);
long        true_item_value(pob);
std::string actionlocstr(char);
int         p_immune(int);
int         statmod(int);
void        change_environment(char);
void        enter_site(Symbol);
void        gain_level(void);
void        minute_status_check(void);
void        moon_check(void);
void        movecursor(int &, int &, int, int);
void        p_drown(void);
void        p_hit(monster *, int, damage_type);
void        p_win(void);
void        tacplayer(monster *);
void        tenminute_status_check(void);
void        toggle_item_use(bool);
void        torch_check(void);
void        weapon_use(int, pob, monster *);

// aux3.cpp functions
std::string countryid(Symbol);
char   getlocation(void);
int    hostilemonstersnear(void);
int    magic_resist(int);
size_t maneuvers(void);
int    parsecitysite(void);
int    stonecheck(int);
void   alert_guards(void);
void   countrysearch(void);
void   hourly_check(void);
void   tenminute_check(void);
void   terrain_check(int);

// char.cpp functions
long  calcmana(void);
int   fixnpc(int);
void  save_omegarc(void);

// city.cpp functions
void load_city(int);
void repair_jail(void);
void resurrect_guards(void);

// command1.cpp functions
void p_country_process(void);
void p_process(void);

// command2.cpp functions
void pickup(void);
void activate(void);
void bash_item(void);
void bash_location(void);
void callitem(void);
void closedoor(void);
void disarm(void);
void downstairs(void);
void drop_equipped_item(void);
void drop_pack_item(void);
void eat(void);
void give(void);
void magic(void);
void movepincountry(int, int);
void moveplayer(int, int);
void opendoor(void);
void peruse(void);
void quaff(void);
void rest(void);
void save(int);
void search(int *);
void setoptions(void);
void talk(void);
void upstairs(void);
void zapwand(void);

// command3.cpp functions
void abortshadowform(void);
void charid(void);
void city_move(void);
void dismount_steed(void);
void examine(void);
void fire(void);
void frobgamestatus(void);
void help(void);
void hunt(Symbol);
void nap(void);
void pickpocket(void);
void quit(int = 0);
void rename_player(void);
void tacoptions(void);
void tunnel(void);
void vault(void);
void version(void);
void wizard(void);

// country.cpp functions
void load_country(void);
void load_dlair(int, int);
void load_misle(int, int);
void load_speak(int, int);
void load_temple(int, int);

// effect1.cpp functions
int  random_item(void);
void acquire(int);
void bless(int);
void enchant(int);
void fball(int, int, int, int, int);
void fbolt(int, int, int, int, int, int);
void heal(int);
void identify(int);
void lball(int, int, int, int, int);
void lbolt(int, int, int, int, int, int);
void manastorm(int, int, int);
void mondet(int);
void nbolt(int, int, int, int, int, int);
void objdet(int);
void snowball(int, int, int, int, int);
void wish(int);

// effect2.cpp functions
void accuracy(int);
void alert(int);
void augment(int);
void breathe(int);
void displace(int);
void flux(int);
void haste(int);
void i_chaos(pob);
void i_law(pob);
void invisible(int);
void knowledge(int);
void recover_stat(int);
void regenerate(int);
void sanctify(int);
void warp(int);

// effect3.cpp functions
int  itemlist(int, int);
void acid_cloud(void);
void aggravate(void);
void amnesia(void);
void annihilate(int);
void apport(int);
void clairvoyance(int);
void cleanse(int);
void cure(int);
void deflection(int);
void disease(int);
void disintegrate(int, int);
void dispel(int);
void disrupt(int, int, int);
void drain(int);
void drain_life(int);
void hellfire(int, int, int);
void hero(int);
void hide(int, int);
void illuminate(int);
void inflict_fear(int, int);
void learnspell(int);
void level_drain(int, const std::string &);
void level_return(void);
void levitate(int);
void p_poison(int);
void p_teleport(int);
void polymorph(int);
void sanctuary(void);
void shadowform(void);
void sleep_monster(int);
void sleep_player(int);
void strategic_teleport(int);
void summon(int, int);
void truesight(int);

// env.cpp functions
void load_arena(void);
void load_circle(int);
void load_court(int);

// etc.cpp functions
std::string_view nameprint(void);
const std::string wordnum(int);
void        hint(void);
void        learnclericalspells(int, int);

// file.cpp functions
FILE *checkfopen(const std::string &, const std::string &);
int   filecheck(void);
void  abyss_file(void);
void  adeptfile(void);
void  cityguidefile(void);
void  combat_help(void);
void  commandlist(void);
void  displayfile(const std::string &);
void  extendlog(const std::string &, int);
void  save_hiscore_npc(int);
void  show_license(void);
void  showmotd(void);
void  theologyfile(void);
void  user_intro(void);
void  wishfile(void);

// gen1.cpp functions
const std::string roomname(int);
void        build_room(int, int, int, char, int);
void        cavern_level(void);
void        change_level(char, char, char);
void        clear_level(level *);
void        free_dungeon(void);
void        install_specials(void);
void        install_traps(void);
void        makedoor(int, int);
void        sewer_level(void);
void        straggle_corridor(int, int, int, int, Symbol, char);

// gen2.cpp functions
void make_country_screen(Symbol);
void make_stairs(int);
void maze_corridor(int, int, int, int, char, char);
void maze_level(void);
void room_level(void);

// guild1.cpp functions
void l_arena(void);
void l_castle(void);
void l_merc_guild(void);

// guild2.cpp functions
void l_college(void);
void l_monastery();
void l_order(void);
void l_sorcerors(void);
void l_thieves_guild(void);

// house.cpp functions
void load_house(int, int);

// inv.cpp functions
void        do_inventory_control(void);
char        index_to_key(int);
long        get_money(long);
const std::string cashstr(void);
std::string itemid(pob);
bool        cursed(pob);
int         find_and_remove_item(int, int);
int         find_item(pob *, int, int);
int         getitem(Symbol);
int         slottable(pob, int);
pob         split_item(int, pob);
void        conform_lost_object(pob);
void        conform_lost_objects(int, pob);
void        conform_unused_object(pob);
void        dispose_lost_objects(int, pob);
void        drop_at(int, int, pob);
void        drop_money(void);
void        fixpack(void);
void        gain_item(pob);
void        give_money(monster *);
void        givemonster(monster *, object *);
void        lose_all_items(void);
void        p_drop_at(int, int, int, pob);
void        pickup_at(int, int);

// item.cpp functions
void        shuffle(int[], int);
const std::string bootname(int);
const std::string cloakname(int);
const std::string potionname(int);
const std::string ringname(int);
const std::string scrollname(int);
const std::string stickname(int);
int         twohandedp(int);
pob         create_object(int);
void        make_armor(pob, int);
void        make_artifact(pob, int);
void        make_boots(pob, int);
void        make_cash(pob, int);
void        make_cloak(pob, int);
void        make_corpse(pob, monster *);
void        make_food(pob, int);
void        make_potion(pob, int);
void        make_ring(pob, int);
void        make_scroll(pob, int);
void        make_shield(pob, int);
void        make_stick(pob, int);
void        make_thing(pob, int);
void        make_weapon(pob, int);

// itemf1.cpp functions
void i_accuracy(pob);
void i_acquire(pob);
void i_alert(pob);
void i_augment(pob);
void i_azoth(pob);
void i_bless(pob);
void i_breathing(pob);
void i_charge(pob);
void i_clairvoyance(pob);
void i_corpse(pob);
void i_cure(pob);
void i_deflect(pob);
void i_displace(pob);
void i_enchant(pob);
void i_fear_resist(pob);
void i_flux(pob);
void i_food(pob);
void i_heal(pob);
void i_hero(pob);
void i_id(pob);
void i_illuminate(pob);
void i_immune(pob);
void i_invisible(pob);
void i_jane_t(pob);
void i_key(pob);
void i_knowledge(pob);
void i_lembas(pob);
void i_levitate(pob);
void i_mondet(pob);
void i_neutralize_poison(pob);
void i_no_op(pob);
void i_nothing(pob);
void i_objdet(pob);
void i_pepper_food(pob);
void i_perm_accuracy(pob);
void i_perm_agility(pob);
void i_perm_displace(pob);
void i_perm_hero(pob);
void i_perm_illuminate(pob);
void i_perm_invisible(pob);
void i_perm_levitate(pob);
void i_perm_negimmune(pob);
void i_perm_protection(pob);
void i_perm_speed(pob);
void i_perm_truesight(pob);
void i_pick(pob);
void i_poison_food(pob);
void i_pow(pob);
void i_raise_portcullis(pob);
void i_regenerate(pob);
void i_restore(pob);
void i_sleep_self(pob);
void i_speed(pob);
void i_spells(pob);
void i_stim(pob);
void i_teleport(pob);
void i_torch_use(pob);
void i_torch_unequip(pob);
void i_trap(pob);
void i_truesight(pob);
void i_warp(pob);
void i_wish(pob);

// itemf2.cpp functions
void i_defend(pob);
void i_demonblade(pob);
void i_desecrate(pob);
void i_lightsabre(pob);
void i_mace_disrupt(pob);
void i_normal_armor(pob);
void i_normal_shield(pob);
void i_normal_weapon(pob);
void i_perm_breathing(pob);
void i_perm_burden(pob);
void i_perm_deflect(pob);
void i_perm_energy_resist(pob);
void i_perm_fear_resist(pob);
void i_perm_fire_resist(pob);
void i_perm_gaze_immune(pob);
void i_perm_knowledge(pob);
void i_perm_poison_resist(pob);
void i_perm_regenerate(pob);
void i_perm_strength(pob);
void i_victrix(pob);
void weapon_acidwhip(int, pob, monster *);
void weapon_arrow(int, pob, monster *);
void weapon_bare_hands(int, monster *);
void weapon_bolt(int, pob, monster *);
void weapon_defend(int, pob, monster *);
void weapon_demonblade(int, pob, monster *);
void weapon_desecrate(int, pob, monster *);
void weapon_firestar(int, pob, monster *);
void weapon_lightsabre(int, pob, monster *);
void weapon_mace_disrupt(int, pob, monster *);
void weapon_normal_hit(int, pob, monster *);
void weapon_scythe(int, pob, monster *);
void weapon_tangle(int, pob, monster *);
void weapon_victrix(int, pob, monster *);
void weapon_vorpal(int, pob, monster *);

// itemf3.cpp functions
void i_antioch(pob);
void i_apport(pob);
void i_crystal(pob);
void i_death(pob);
void i_disintegrate(pob);
void i_dispel(pob);
void i_disrupt(pob);
void i_enchantment(pob);
void i_fear(pob);
void i_fireball(pob);
void i_firebolt(pob);
void i_helm(pob);
void i_hide(pob);
void i_juggernaut(pob);
void i_kolwynia(pob);
void i_lball(pob);
void i_lbolt(pob);
void i_life(pob);
void i_missile(pob);
void i_orbair(pob);
void i_orbdead(pob);
void i_orbearth(pob);
void i_orbfire(pob);
void i_orbmastery(pob);
void i_orbwater(pob);
void i_planes(pob);
void i_polymorph(pob);
void i_sceptre(pob);
void i_sleep_other(pob);
void i_snowball(pob);
void i_stargem(pob);
void i_summon(pob);
void i_symbol(pob);

// lev.cpp functions
int  difficulty(void); /* this function in util.c if MSDOS defined */
pmt  m_create(int, int, int, int);
pmt  make_creature(int);
void make_country_monsters(Symbol);
void make_site_monster(int, int, int);
void make_site_treasure(int, int, int);
void make_specific_treasure(int, int, int);
void populate_level(int);
void stock_level(void);
void wandercheck(void);

// mmelee.cpp functions
char random_loc(void);
void tacmonster(monster *);
void transcribe_monster_actions(monster *);

// mmove.cpp functions
void m_confused_move(monster *);
void m_flutter_move(monster *);
void m_follow_move(monster *);
void m_move_animal(monster *);
void m_normal_move(monster *);
void m_random_move(monster *);
void m_scaredy_move(monster *);
void m_simple_move(monster *);
void m_smart_move(monster *);
void m_spirit_move(monster *);
void m_teleport(monster *);
void m_vanish(monster *);
void m_move_leash(monster *);

// mon.cpp functions
const char *mantype(void);
void        determine_npc_behavior(pmt, int, int);
void        m_abyss(monster *);
void        m_altar(monster *);
void        m_damage(monster *, int, damage_type);
void        m_death(monster *);
void        m_dropstuff(monster *);
void        m_fire(monster *);
void        m_lava(monster *);
void        m_no_op(monster *);
void        m_pickup(monster *, object *);
void        m_pulse(monster *);
void        m_trap_abyss(monster *);
void        m_trap_acid(monster *);
void        m_trap_blade(monster *);
void        m_trap_dart(monster *);
void        m_trap_disintegrate(monster *);
void        m_trap_door(monster *);
void        m_trap_fire(monster *);
void        m_trap_manadrain(monster *);
void        m_trap_pit(monster *);
void        m_trap_sleepgas(monster *);
void        m_trap_snare(monster *);
void        m_trap_teleport(monster *);
void        m_water(monster *);
void        make_hiscore_npc(pmt, int);
void        make_log_npc(monster *);
void        monster_talk(monster *);
void        movemonster(monster *, int, int);

// move.cpp functions
void l_abyss(void);
void l_air_station(void);
void l_arena_exit(void);
void l_balancestone(void);
void l_chaos(void);
void l_chaostone(void);
void l_circle_library(void);
void l_drop_every_portcullis(void);
void l_earth_station(void);
void l_enter_circle(void);
void l_enter_court(void);
void l_escalator(void);
void l_fire(void);
void l_fire_station(void);
void l_hedge(void);
void l_house_exit(void);
void l_lava(void);
void l_lawstone(void);
void l_lift(void);
void l_magic_pool(void);
void l_mindstone(void);
void l_no_op(void);
void l_portcullis_trap(void);
void l_raise_portcullis(void);
void l_rubble(void);
void l_sacrificestone(void);
void l_tactical_exit(void);
void l_temple_warning(void);
void l_throne(void);
void l_tome1(void);
void l_tome2(void);
void l_voice1(void);
void l_voice2(void);
void l_voice3(void);
void l_void(void);
void l_void_station(void);
void l_voidstone(void);
void l_water(void);
void l_water_station(void);
void l_whirlwind(void);

// movef.cpp functions
void m_movefunction(monster *, int);
void p_movefunction(int);
void sign_print(int, int, int); /* this function in site1.c if MSDOS defined */

// mspec.cpp functions
void m_aggravate(monster *);
void m_huge_sounds(monster *);
void m_illusion(monster *);
void m_sp_acid_cloud(monster *);
void m_sp_angel(monster *);
void m_sp_av(monster *);
void m_sp_blackout(monster *);
void m_sp_bogthing(monster *);
void m_sp_court(monster *);
void m_sp_demon(monster *);
void m_sp_demonlover(monster *);
void m_sp_dragonlord(monster *);
void m_sp_eater(monster *);
void m_sp_escape(monster *);
void m_sp_explode(monster *);
void m_sp_ghost(monster *);
void m_sp_lair(monster *);
void m_sp_lw(monster *);
void m_sp_mb(monster *);
void m_sp_merchant(monster *);
void m_sp_mirror(monster *);
void m_sp_mp(monster *);
void m_sp_ng(monster *);
void m_sp_poison_cloud(monster *);
void m_sp_prime(monster *);
void m_sp_raise(monster *);
void m_sp_seductor(monster *);
void m_sp_servant(monster *);
void m_sp_spell(monster *);
void m_sp_surprise(monster *);
void m_sp_swarm(monster *);
void m_sp_were(monster *);
void m_sp_whistleblower(monster *);
void m_summon(monster *);
void m_thief_f(monster *);

// mstrike.cpp functions
void m_blind_strike(monster *);
void m_fireball(monster *);
void m_firebolt(monster *);
void m_lball(monster *);
void m_nbolt(monster *);
void m_snowball(monster *);
void m_strike_sonic(monster *);

// mtalk.cpp functions
void m_talk_animal(monster *);
void m_talk_archmage(monster *);
void m_talk_assassin(monster *);
void m_talk_beg(monster *);
void m_talk_burble(monster *);
void m_talk_demonlover(monster *);
void m_talk_druid(monster *);
void m_talk_ef(monster *);
void m_talk_evil(monster *);
void m_talk_gf(monster *);
void m_talk_greedy(monster *);
void m_talk_guard(monster *);
void m_talk_hint(monster *);
void m_talk_horse(monster *);
void m_talk_hungry(monster *);
void m_talk_hyena(monster *);
void m_talk_im(monster *);
void m_talk_man(monster *);
void m_talk_merchant(monster *);
void m_talk_mimsy(monster *);
void m_talk_mp(monster *);
void m_talk_ninja(monster *);
void m_talk_parrot(monster *);
void m_talk_prime(monster *);
void m_talk_robot(monster *);
void m_talk_scream(monster *);
void m_talk_seductor(monster *);
void m_talk_servant(monster *);
void m_talk_silent(monster *);
void m_talk_slithy(monster *);
void m_talk_stupid(monster *);
void m_talk_thief(monster *);
void m_talk_titter(monster *);

// priest.cpp functions
void l_altar(void);

// save.cpp functions
bool restore_game(const std::string &);
bool save_game(const std::string &);
void signalsave(int = 0);

// site1.cpp functions
void l_alchemist(void);
void l_armorer(void);
void l_bank(void);
void l_casino(void);
void l_club(void);
void l_commandant(void);
void l_crap(void);
void l_diner(void);
void l_dpw(void);
void l_gym(void);
void l_healer(void);
void l_library(void);
void l_pawn_shop(void);
void l_statue_wake(void);
void l_tavern(void);
void statue_random(int, int);

// site2.cpp functions
void cureforpay(void);
void gymtrain(int *, int *);
void healforpay(void);
void l_adept(void);
void l_brothel(void);
void l_cartographer(void);
void l_charity(void);
void l_condo(void);
void l_countryside(void);
void l_house(void);
void l_hovel(void);
void l_mansion(void);
void l_oracle(void);
void l_safe(void);
void l_triffid(void);
void l_vault(void);
void pacify_guards(void);
void send_to_jail(void);

// time.cpp functions
void time_clock(int);

// trap.cpp functions
void l_trap_abyss(void);
void l_trap_acid(void);
void l_trap_blade(void);
void l_trap_dart(void);
void l_trap_disintegrate(void);
void l_trap_door(void);
void l_trap_fire(void);
void l_trap_manadrain(void);
void l_trap_pit(void);
void l_trap_siren(void);
void l_trap_sleepgas(void);
void l_trap_snare(void);
void l_trap_teleport(void);

// util.cpp functions
long        calc_points(void);
const std::string getarticle(const std::string &);
const std::string ordinal(int);
int         confirmation(void);
char        cryptkey(const std::string &);
int         day(void);
int         distance(int, int, int, int);
bool        hitp(int, int);
int         hour(void);
int         inbounds(int, int);
int         los_p(int, int, int, int);
int         m_unblocked(monster *, int, int);
int         nighttime(void);
int         offscreen(int, int);
int         ok_to_free(plv);
void        free_level(plv);
void        free_objlist(pol);
int         random_range(int);
int         screenmod(int);
int         screenmod_horizontal(int);
int         showhour(void);
int         showminute(void);
int         strmem(char, const std::string &);
int         unblocked(int, int);
int         view_los_p(int, int, int, int);
int         view_unblocked(int, int);
void        calc_weight(void);
void        do_los(Symbol, int *, int *, int, int);
void        do_object_los(Symbol, int *, int *, int, int);
void        findspace(int *, int *, int);
void        initdirs(void);

// village.cpp functions
void load_village(int, int);

#endif
