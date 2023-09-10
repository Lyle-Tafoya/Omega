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

/* extern.h -- function declarations */

#ifndef OMEGA_EXTERN_H_
#define OMEGA_EXTERN_H_

#include "defs.h"

#include <string>

/* omega.cpp functions */

int  main(int, char *[]);
bool game_restore(int, char *[]);
void init_world(void);
void inititem(int);
void initrand(int, int);
void signalexit(int = 0);

/* abyss.cpp functions */

void load_abyss(void);

/* aux1.cpp functions */

std::string levelname(int);
std::string mstatus_string(struct monster *);
std::string trapid(int);
int         getdir(void);
bool        goberserk(void);
int         p_country_moveable(int, int);
int         p_moveable(int, int);
int         player_on_sanctuary(void);
void        calc_melee(void);
int         damage_item(pob);
void        describe_player(void);
void        fight_monster(struct monster *);
void        foodcheck(void);
void        gain_experience(int);
void        p_damage(int, int, const std::string &);
void        p_death(const std::string &);
void        roomcheck(void);
void        searchat(int, int);
void        setspot(int *, int *);
void        showroom(int);
void        surrender(struct monster *);
void        threaten(struct monster *);
void        tunnelcheck(void);

/* aux2.cpp functions */

long        expval(int);
long        item_value(pob);
long        true_item_value(pob);
std::string actionlocstr(char);
int         p_immune(int);
int         player_hit(int, char, struct monster *);
int         statmod(int);
void        break_weapon(void);
void        change_environment(char);
void        drop_weapon(void);
void        enter_site(Symbol);
void        gain_level(void);
void        minute_status_check(void);
void        moon_check(void);
void        movecursor(int *, int *, int, int);
void        p_drown(void);
void        p_fumble(int);
void        p_hit(struct monster *, int, int);
void        p_win(void);
void        player_miss(struct monster *, int);
void        tacplayer(struct monster *);
void        tenminute_status_check(void);
void        toggle_item_use(bool);
void        torch_check(void);
void        weapon_use(int, pob, struct monster *);

/* aux3.cpp functions */

char  *citysiteid(int);
char  *countryid(Symbol);
char   getlocation(void);
int    hostilemonstersnear(void);
int    magic_resist(int);
size_t maneuvers(void);
int    parsecitysite(void);
int    stonecheck(int);
void   alert_guards(void);
void   countrysearch(void);
void   destroy_order(void);
void   hourly_check(void);
void   indoors_random_event(void);
void   outdoors_random_event(void);
void   tenminute_check(void);
void   terrain_check(int);

/* char.cpp functions */

long  calcmana(void);
FILE *omegarc_check(void);
int   competence_check(int);
int   fixnpc(int);
void  initplayer(void);
void  initstats(void);
void  omegan_character_stats(void);
void  save_omegarc(void);
void  user_character_stats(void);

/* city.cpp functions */

void assign_city_function(int, int);
void load_city(int);
void make_justiciar(int, int);
void make_major_undead(int, int);
void make_minor_undead(int, int);
void mazesite(int, int, int);
void randommazesite(int, int, int);
void repair_jail(void);
void resurrect_guards(void);

/* command1.cpp functions */

void p_country_process(void);
void p_process(void);

/* command2.cpp functions */

void pickup(void);
void activate(void);
void bash_item(void);
void bash_location(void);
void callitem(void);
void closedoor(void);
void disarm(void);
void downstairs(void);
void drop(void);
void eat(void);
void floor_inv(void);
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

/* command3.cpp functions */

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

/* country.cpp functions */

void load_country(void);
void load_dlair(int, int);
void load_misle(int, int);
void load_speak(int, int);
void load_temple(int, int);
void make_high_priest(int, int, int);
void random_temple_site(int, int, int, int);

/* effect1.cpp functions */

int  random_item(void);
void acquire(int);
void ball(int, int, int, int, int, int);
void bless(int);
void bolt(int, int, int, int, int, int, int);
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

/* effect2.cpp functions */

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

/* effect3.cpp functions */

int  itemlist(int, int);
int  monsterlist(void);
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
void level_drain(int, const char *);
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

/* env.cpp functions */

void load_arena(void);
void load_circle(int);
void load_court(int);
void make_archmage(int, int);
void make_prime(int, int);

/* etc.cpp functions */

char       *nameprint(void);
const char *wordnum(int);
void        hint(void);
void        learnclericalspells(int, int);

/* file.cpp functions */

void  lock_score_file(void);
void  unlock_score_file(void);
FILE *checkfopen(const std::string &, const std::string &);
int   filecheck(void);
bool  test_file_access(const std::string &, char);
void  abyss_file(void);
void  adeptfile(void);
void  checkhigh(const std::string &, int);
void  cityguidefile(void);
void  combat_help(void);
void  commandlist(void);
void  displayfile(const std::string &);
void  displaycryptfile(const std::string &);
void  extendlog(const std::string &, int);
void  filescanstring(FILE *, char *); /* in util.c if MSDOS defined */
void  inv_help(void);
void  save_hiscore_npc(int);
void  show_license(void);
void  showmotd(void);
void  showscores(void);
void  theologyfile(void);
void  user_intro(void);
void  wishfile(void);

/* gen1.cpp functions */

const char *roomname(int);
plv         findlevel(struct level *, char);
void        build_room(int, int, int, char, int);
void        build_square_room(int, int, int, char, int);
void        cavern_level(void);
void        change_level(char, char, char);
void        clear_level(struct level *);
void        corridor_crawl(int *, int *, int, int, int, Symbol, char);
void        find_stairs(char, char);
void        free_dungeon(void);
void        install_specials(void);
void        install_traps(void);
void        makedoor(int, int);
void        sewer_corridor(int, int, int, int, Symbol);
void        sewer_level(void);
void        straggle_corridor(int, int, int, int, Symbol, char);

/* gen2.cpp functions */

void make_country_screen(Symbol);
void make_general_map(char *);
void make_forest(void);
void make_jungle(void);
void make_mountains(void);
void make_plains(void);
void make_river(void);
void make_road(void);
void make_stairs(int);
void make_swamp(void);
void maze_corridor(int, int, int, int, char, char);
void maze_level(void);
void room_corridor(int, int, int, int, int);
void room_level(void);

/* guild1.cpp functions */

void l_arena(void);
void l_castle(void);
void l_merc_guild(void);

/* guild2.cpp functions */

void l_college(void);
void l_order(void);
void l_sorcerors(void);
void l_thieves_guild(void);

/* house.cpp functions */

void load_house(int, int);
void make_house_npc(int, int);
void make_mansion_npc(int, int);

/* inv.cpp functions */

void        do_inventory_control(void);
int         key_to_index(signed char);
signed char index_to_key(int);
long        get_money(long);
const char *cashstr(void);
std::string itemid(pob);
bool        cursed(pob);
int         find_and_remove_item(int, int);
int         find_item(pob *, int, int);
int         get_item_number(pob);
int         getitem(Symbol);
bool        item_useable(pob, int);
int         objequal(struct object *, struct object *);
int         slottable(pob, int);
pob         detach_money(void);
pob         split_item(int, pob);
void        conform_lost_object(pob);
void        conform_lost_objects(int, pob);
void        conform_unused_object(pob);
void        dispose_lost_objects(int, pob);
void        drop_at(int, int, pob);
void        drop_from_slot(int);
void        drop_money(void);
void        fixpack(void);
void        gain_item(pob);
void        give_money(struct monster *);
void        givemonster(struct monster *, struct object *);
void        lose_all_items(void);
void        p_drop_at(int, int, int, pob);
void        pack_extra_items(pob);
void        pickup_at(int, int);

/* item.cpp functions */

void        shuffle(int[], int);
const char *bootname(int);
const char *cloakname(int);
const char *grotname(void);
const char *potionname(int);
const char *ringname(int);
const char *scrollname(int);
const char *stickname(int);
int         itemblessing(void);
int         itemcharge(void);
int         itemplus(void);
int         twohandedp(int);
pob         create_object(int);
void        make_armor(pob, int);
void        make_artifact(pob, int);
void        make_boots(pob, int);
void        make_cash(pob, int);
void        make_cloak(pob, int);
void        make_corpse(pob, struct monster *);
void        make_food(pob, int);
void        make_potion(pob, int);
void        make_ring(pob, int);
void        make_scroll(pob, int);
void        make_shield(pob, int);
void        make_stick(pob, int);
void        make_thing(pob, int);
void        make_weapon(pob, int);

/* itemf1.cpp functions */

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

/* itemf2.cpp functions */

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
void weapon_acidwhip(int, pob, struct monster *);
void weapon_arrow(int, pob, struct monster *);
void weapon_bare_hands(int, struct monster *);
void weapon_bolt(int, pob, struct monster *);
void weapon_defend(int, pob, struct monster *);
void weapon_demonblade(int, pob, struct monster *);
void weapon_desecrate(int, pob, struct monster *);
void weapon_firestar(int, pob, struct monster *);
void weapon_lightsabre(int, pob, struct monster *);
void weapon_mace_disrupt(int, pob, struct monster *);
void weapon_normal_hit(int, pob, struct monster *);
void weapon_scythe(int, pob, struct monster *);
void weapon_tangle(int, pob, struct monster *);
void weapon_victrix(int, pob, struct monster *);
void weapon_vorpal(int, pob, struct monster *);

/* itemf3.cpp functions */

int  orbcheck(char);
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

/* lev.cpp functions */

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

/* mmelee.cpp functions */

char random_loc(void);
bool monster_hit(struct monster *, char, int);
void m_hit(struct monster *, int);
void monster_melee(struct monster *, char, int);
void tacmonster(struct monster *);
void transcribe_monster_actions(struct monster *);

/* mmove.cpp functions */

void m_confused_move(struct monster *);
void m_flutter_move(struct monster *);
void m_follow_move(struct monster *);
void m_move_animal(struct monster *);
void m_normal_move(struct monster *);
void m_random_move(struct monster *);
void m_scaredy_move(struct monster *);
void m_simple_move(struct monster *);
void m_smart_move(struct monster *);
void m_spirit_move(struct monster *);
void m_teleport(struct monster *);
void m_vanish(struct monster *);
void m_move_leash(struct monster *);

/* mon.cpp functions */

const char *mantype(void);
void        determine_npc_behavior(pmt, int, int);
void        m_abyss(struct monster *);
void        m_altar(struct monster *);
void        m_damage(struct monster *, int, int);
void        m_death(struct monster *);
void        m_dropstuff(struct monster *);
void        m_fire(struct monster *);
void        m_lava(struct monster *);
void        m_no_op(struct monster *);
void        m_pickup(struct monster *, struct object *);
void        m_pulse(struct monster *);
void        m_trap_abyss(struct monster *);
void        m_trap_acid(struct monster *);
void        m_trap_blade(struct monster *);
void        m_trap_dart(struct monster *);
void        m_trap_disintegrate(struct monster *);
void        m_trap_door(struct monster *);
void        m_trap_fire(struct monster *);
void        m_trap_manadrain(struct monster *);
void        m_trap_pit(struct monster *);
void        m_trap_sleepgas(struct monster *);
void        m_trap_snare(struct monster *);
void        m_trap_teleport(struct monster *);
void        m_water(struct monster *);
void        make_hiscore_npc(pmt, int);
void        make_log_npc(struct monster *);
void        monster_action(struct monster *, int);
void        monster_move(struct monster *);
void        monster_special(struct monster *);
void        monster_strike(struct monster *);
void        monster_talk(struct monster *);
void        movemonster(struct monster *, int, int);
void        strengthen_death(struct monster *);

/* move.cpp functions */

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
void stationcheck(void);

/* movef.cpp functions */

void m_movefunction(struct monster *, int);
void p_movefunction(int);
void sign_print(int, int, int); /* this function in site1.c if MSDOS defined */

/* mspec.cpp functions */

void m_aggravate(struct monster *);
void m_huge_sounds(struct monster *);
void m_illusion(struct monster *);
void m_sp_acid_cloud(struct monster *);
void m_sp_angel(struct monster *);
void m_sp_av(struct monster *);
void m_sp_blackout(struct monster *);
void m_sp_bogthing(struct monster *);
void m_sp_court(struct monster *);
void m_sp_demon(struct monster *);
void m_sp_demonlover(struct monster *);
void m_sp_dragonlord(struct monster *);
void m_sp_eater(struct monster *);
void m_sp_escape(struct monster *);
void m_sp_explode(struct monster *);
void m_sp_ghost(struct monster *);
void m_sp_lair(struct monster *);
void m_sp_lw(struct monster *);
void m_sp_mb(struct monster *);
void m_sp_merchant(struct monster *);
void m_sp_mirror(struct monster *);
void m_sp_mp(struct monster *);
void m_sp_ng(struct monster *);
void m_sp_poison_cloud(struct monster *);
void m_sp_prime(struct monster *);
void m_sp_raise(struct monster *);
void m_sp_seductor(struct monster *);
void m_sp_servant(struct monster *);
void m_sp_spell(struct monster *);
void m_sp_surprise(struct monster *);
void m_sp_swarm(struct monster *);
void m_sp_were(struct monster *);
void m_sp_whistleblower(struct monster *);
void m_summon(struct monster *);
void m_thief_f(struct monster *);

/* mstrike.cpp functions */

void m_blind_strike(struct monster *);
void m_fireball(struct monster *);
void m_firebolt(struct monster *);
void m_lball(struct monster *);
void m_nbolt(struct monster *);
void m_snowball(struct monster *);
void m_strike_sonic(struct monster *);

/* mtalk.cpp functions */

void m_talk_animal(struct monster *);
void m_talk_archmage(struct monster *);
void m_talk_assassin(struct monster *);
void m_talk_beg(struct monster *);
void m_talk_burble(struct monster *);
void m_talk_demonlover(struct monster *);
void m_talk_druid(struct monster *);
void m_talk_ef(struct monster *);
void m_talk_evil(struct monster *);
void m_talk_gf(struct monster *);
void m_talk_greedy(struct monster *);
void m_talk_guard(struct monster *);
void m_talk_hint(struct monster *);
void m_talk_horse(struct monster *);
void m_talk_hungry(struct monster *);
void m_talk_hyena(struct monster *);
void m_talk_im(struct monster *);
void m_talk_man(struct monster *);
void m_talk_merchant(struct monster *);
void m_talk_mimsy(struct monster *);
void m_talk_mp(struct monster *);
void m_talk_ninja(struct monster *);
void m_talk_parrot(struct monster *);
void m_talk_prime(struct monster *);
void m_talk_robot(struct monster *);
void m_talk_scream(struct monster *);
void m_talk_seductor(struct monster *);
void m_talk_servant(struct monster *);
void m_talk_silent(struct monster *);
void m_talk_slithy(struct monster *);
void m_talk_stupid(struct monster *);
void m_talk_thief(struct monster *);
void m_talk_titter(struct monster *);

/* priest.cpp functions */

int  check_sacrilege(int);
void answer_prayer(void);
void hp_req_print(void);
void hp_req_test(void);
int  increase_priest_rank(int);
void l_altar(void);
void make_hp(pob);

/* save.cpp functions */

bool ok_outdated(int);
bool restore_game(char *);
void restore_hiscore_npc(pmt, int);
pob  restore_item(FILE *, int);
pol  restore_itemlist(FILE *, int);
void restore_country(FILE *, int);
void restore_level(FILE *, int);
void restore_monsters(FILE *, plv, int);
void restore_player(FILE *, int);
bool save_game(const char *);
bool save_country(FILE *);
bool save_item(FILE *, pob);
bool save_itemlist(FILE *, pol);
bool save_level(FILE *, plv);
bool save_monsters(FILE *, pml);
bool save_player(FILE *);
void signalsave(int = 0);

/* scr.cpp functions */

int    mcigetc(void);
long   parsenum(void);
std::string msgscanstring();
Symbol getspot(int, int, int);
char   menugetc(void);
int    mgetc(void);
int    ynq(void);
int    ynq1(void);
int    ynq2(void);
int    getnumber(int);
int    litroom(int, int);
int    move_slot(int, int, int);
int    stillonblock(void);
void   blankoutspot(int, int);
void   blotspot(int, int);
void   bufferprint(void);
void   clear_screen(void);
void   colour_on(void);
void   colour_off(void);
void   commanderror(void);
void   dataprint(void);
void   deathprint(void);
void   display_bigwin(void);
void   display_death(const std::string &);
void   display_option_slot(int);
void   display_options(void);
void   display_pack(void);
void   display_quit(void);
void   display_win(void);
void   dobackspace(void);
void   dodrawspot(int, int);
void   draw_explosion(Symbol, int, int);
void   drawmonsters(int);
void   drawomega(void);
void   drawplayer(void);
void   drawscreen(void);
void   drawspot(int, int);
void   drawvision(int, int);
void   endgraf(void);
void   erase_level(void);
void   erase_monster(struct monster *);
void   hide_line(int);
void   initgraf(void);
void   levelrefresh(void);
void   lightspot(int, int);
void   locprint(const std::string &);
void   maddch(char);
void   menuaddch(char);
void   menuclear(void);
void   menuprint(const std::string &);
void   showmenu(void);
void   mprint(const std::string &);
void   nprint1(const std::string &);
void   nprint2(const std::string &);
void   nprint3(const std::string &);
void   phaseprint(void);
void   plotchar(Symbol, int, int);
void   plotmon(struct monster *);
void   plotspot(int, int, int);
void   print1(const std::string &);
void   print2(const std::string &);
void   print3(const std::string &);
void   putspot(int, int, Symbol);
void   redraw(void);
void   screencheck(int, int);
void   setlastxy(int, int);
void   show_screen(void);
void   omshowcursor(int, int);
void   showflags(void);
void   spreadroomdark(int, int, int);
void   spreadroomlight(int, int, int);
void   timeprint(void);
void   title(void);
void   xredraw(void);

/* site1.cpp functions */

void buyfromstock(int, int);
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
void wake_statue(int, int, int);

/* site2.cpp functions */

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
void l_trifid(void);
void l_vault(void);
void pacify_guards(void);
void send_to_jail(void);

/* spell.cpp functions */

const char *spellid(int);
int         getspell(void);
int         spellparse(void);
void        cast_spell(int);
void        initspells(void);
void        s_accuracy(void);
void        s_alert(void);
void        s_apport(void);
void        s_bless(void);
void        s_breathe(void);
void        s_clairvoyance(void);
void        s_cure(void);
void        s_desecrate(void);
void        s_disintegrate(void);
void        s_dispel(void);
void        s_disrupt(void);
void        s_drain(void);
void        s_enchant(void);
void        s_fear(void);
void        s_firebolt(void);
void        s_haste(void);
void        s_heal(void);
void        s_hellfire(void);
void        s_hero(void);
void        s_identify(void);
void        s_invisible(void);
void        s_knowledge(void);
void        s_lball(void);
void        s_levitate(void);
void        s_missile(void);
void        s_mondet(void);
void        s_objdet(void);
void        s_polymorph(void);
void        s_regenerate(void);
void        s_restore(void);
void        s_return(void);
void        s_ritual(void);
void        s_sanctify(void);
void        s_sanctuary(void);
void        s_shadowform(void);
void        s_sleep(void);
void        s_summon(void);
void        s_teleport(void);
void        s_truesight(void);
void        s_warp(void);
void        s_wish(void);
void        showknownspells(int, int);

/* time.cpp functions */

void fix_phantom(struct monster *);
void time_clock(int);

/* trap.cpp functions */

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

/* util.cpp functions */

long        calc_points(void);
const char *getarticle(const std::string &);
const char *month(void);
const char *ordinal(int);
const char *salloc(const char *);
char        inversedir(int);
int         confirmation(void);
char        cryptkey(const std::string &);
int         day(void);
int         distance(int, int, int, int);
bool        hitp(int, int);
int         hour(void);
int         inbounds(int, int);
int         los_p(int, int, int, int);
int         m_unblocked(struct monster *, int, int);
int         nighttime(void);
int         offscreen(int, int);
int         ok_to_free(plv);
void        free_level(plv);
void        free_mons_and_objs(pml);
void        free_objlist(pol);
void       *checkmalloc(unsigned int);
int         random_range(int);
int         screenmod(int);
int         screenmod_horizontal(int);
int         showhour(void);
int         showminute(void);
int         strmem(char, const std::string &);
int         strprefix(const std::string &, const std::string &);
int         unblocked(int, int);
int         view_los_p(int, int, int, int);
int         view_unblocked(int, int);
void        calc_weight(void);
void        do_los(Symbol, int *, int *, int, int);
void        do_object_los(Symbol, int *, int *, int, int);
void        findspace(int *, int *, int);
void        initdirs(void);

/* village.cpp functions */

void assign_village_function(int, int, int);
void load_village(int, int);
void make_food_bin(int, int);
void make_guard(int, int);
void make_horse(int, int);
void make_merchant(int, int);
void make_sheep(int, int);
void special_village_site(int, int, int);

#endif
