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
#include "level.h"

#include <memory>
#include <span>
#include <string>
#include <string_view>

// omega.cpp functions
void init_world();
void inititem(int);
void initrand(int, int);

// abyss.cpp functions
void load_abyss();

// aux1.cpp functions
std::string levelname(int);
std::string mstatus_string(const monster *);
std::string trapid(int);
int getdir();
bool goberserk();
bool p_country_moveable(int, int);
bool p_moveable(int, int);
bool player_on_sanctuary();
void calc_melee();
bool damage_item(std::unique_ptr<object> &);
std::string describe_player();
void foodcheck();
void gain_experience(int);
void p_damage(int, int, const std::string &);
void p_death(const std::string &);
void roomcheck();
void searchat(int, int);
void setspot(int &, int &);
void showroom(int);
void surrender(monster *);
void threaten(monster *);
void tunnelcheck();

// aux2.cpp functions
long item_value(const object *);
long true_item_value(const object *);
std::string actionlocstr(char);
bool p_immune(int);
int statmod(int);
void change_environment(char);
void enter_site(chtype);
void gain_level();
void minute_status_check();
void moon_check();
void movecursor(int &, int &, int, int);
void p_drown();
void p_hit(monster *, int, damage_type);
void p_win();
void tacplayer(monster *);
void tenminute_status_check();
void toggle_item_use(bool);
void torch_check();
void weapon_use(int, std::unique_ptr<object> &, monster *);

// aux3.cpp functions
std::string countryid(chtype);
char getlocation();
bool hostilemonstersnear();
bool magic_resist(int);
size_t maneuvers();
int parsecitysite();
bool stonecheck(int);
void alert_guards();
void countrysearch();
void hourly_check();
void tenminute_check();
void terrain_check(int);

// char.cpp functions
long calcmana();
int fixnpc(int);
void save_omegarc();

// city.cpp functions
void load_city(int);
void repair_jail();
void resurrect_guards();

// command1.cpp functions
void p_country_process();
void p_process();

// command2.cpp functions
void pickup();
void activate();
void bash_item();
void bash_location();
void callitem();
void closedoor();
void disarm();
void downstairs();
void drop_equipped_item();
void drop_pack_item();
void eat();
void give();
void magic();
void movepincountry(int, int);
void moveplayer(int, int);
void opendoor();
void peruse();
void quaff();
void rest();
void save(int);
void search(int *);
void setoptions();
void talk();
void upstairs();
void zapwand();

// command3.cpp functions
void abortshadowform();
void charid();
void city_move();
void dismount_steed();
void examine();
void fire();
void frobgamestatus();
void help();
void hunt(chtype);
void nap();
void pickpocket();
void quit(int = 0);
void rename_player();
void tacoptions();
void tunnel();
void vault();
void version();
void wizard();

// country.cpp functions
void load_country();
void load_dlair(int, int);
void load_misle(int, int);
void load_speak(int, int);
void load_temple(int, int);

// effect1.cpp functions
int random_item();
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
void i_chaos(std::unique_ptr<object> &);
void i_law(std::unique_ptr<object> &);
void invisible(int);
void knowledge(int);
void recover_stat(int);
void regenerate(int);
void sanctify(int);
void warp(int);

// effect3.cpp functions
int itemlist(int, int);
void acid_cloud();
void aggravate();
void amnesia();
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
void level_return();
void levitate(int);
void p_poison(int);
void p_teleport(int);
void polymorph(int);
void sanctuary();
void shadowform();
void sleep_monster(int);
void sleep_player(int);
void strategic_teleport(int);
void summon(int, int);
void truesight(int);

// env.cpp functions
void load_arena();
void load_circle(int);
void load_court(int);

// etc.cpp functions
std::string_view nameprint();
const std::string wordnum(int);
void hint();
void learnclericalspells(int, int);

// file.cpp functions
FILE *checkfopen(const std::string &, const std::string &);
int filecheck();
void abyss_file();
void adeptfile();
void cityguidefile();
void combat_help();
void commandlist();
void displayfile(const std::string &);
void extendlog(const std::string &, int);
void save_hiscore_npc(int);
void show_license();
void showmotd();
void theologyfile();
void user_intro();
void wishfile();

// gen1.cpp functions
const std::string roomname(int);
void build_room(int, int, int, char, int);
void cavern_level();
void change_level(char, char, char);
void clear_level(level *);
void free_dungeon();
void install_specials();
void install_traps();
void makedoor(int, int);
void sewer_level();
void straggle_corridor(int, int, int, int, chtype, char);

// gen2.cpp functions
void make_country_screen(chtype);
void make_stairs(int);
void maze_corridor(int, int, int, int, char, char);
void maze_level();
void room_level();

// guild1.cpp functions
void l_arena();
void l_castle();
void l_merc_guild();

// guild2.cpp functions
void l_college();
void l_monastery();
void l_order();
void l_sorcerors();
void l_thieves_guild();

// house.cpp functions
void load_house(int, int);

// inv.cpp functions
const std::string cashstr();
void conform_unused_object(std::unique_ptr<object> &);
bool cursed(const object *);
void dispose_lost_objects(int, std::unique_ptr<object> &);
void do_inventory_control();
void drop_at(int, int, std::unique_ptr<object>);
void drop_money();
bool find_and_remove_item(int, int);
bool find_item(object **, int, int);
void fixpack();
void gain_item(std::unique_ptr<object>);
int getitem(chtype);
long get_money(long);
void give_money(monster *);
void givemonster(monster *, std::unique_ptr<object>);
char index_to_key(int);
std::string itemid(object *);
void lose_all_items();
bool merge_item(std::span<std::unique_ptr<object>>, const object *, int);
void p_drop_at(int, int, int, object *);
void pickup_at(int, int);
bool slottable(const object *, int);
std::unique_ptr<object> split_item(int, const object *);

// item.cpp functions
void shuffle(int[], int);
const std::string bootname(int);
const std::string cloakname(int);
const std::string potionname(int);
const std::string ringname(int);
const std::string scrollname(int);
const std::string stickname(int);
void item_equip(std::unique_ptr<object> &);
void item_unequip(std::unique_ptr<object> &);
void item_use(std::unique_ptr<object> &);
bool twohandedp(int);
std::unique_ptr<object> create_object(int);
void make_armor(object *, int);
void make_artifact(object *, int);
void make_boots(object *, int);
void make_cash(object *, int);
void make_cloak(object *, int);
void make_corpse(object *, const monster *);
void make_food(object *, int);
void make_potion(object *, int);
void make_ring(object *, int);
void make_scroll(object *, int);
void make_shield(object *, int);
void make_stick(object *, int);
void make_thing(object *, int);
void make_weapon(object *, int);

// itemf1.cpp functions
void i_accuracy(std::unique_ptr<object> &);
void i_acquire(std::unique_ptr<object> &);
void i_alert(std::unique_ptr<object> &);
void i_augment(std::unique_ptr<object> &);
void i_azoth(std::unique_ptr<object> &);
void i_bless(std::unique_ptr<object> &);
void i_breathing(std::unique_ptr<object> &);
void i_charge(std::unique_ptr<object> &);
void i_clairvoyance(std::unique_ptr<object> &);
void i_corpse(std::unique_ptr<object> &);
void i_cure(std::unique_ptr<object> &);
void i_deflect(std::unique_ptr<object> &);
void i_displace(std::unique_ptr<object> &);
void i_enchant(std::unique_ptr<object> &);
void i_fear_resist(std::unique_ptr<object> &);
void i_flux(std::unique_ptr<object> &);
void i_food(std::unique_ptr<object> &);
void i_heal(std::unique_ptr<object> &);
void i_hero(std::unique_ptr<object> &);
void i_id(std::unique_ptr<object> &);
void i_illuminate(std::unique_ptr<object> &);
void i_immune(std::unique_ptr<object> &);
void i_invisible(std::unique_ptr<object> &);
void i_jane_t(std::unique_ptr<object> &);
void i_key(std::unique_ptr<object> &);
void i_knowledge(std::unique_ptr<object> &);
void i_lembas(std::unique_ptr<object> &);
void i_levitate(std::unique_ptr<object> &);
void i_mondet(std::unique_ptr<object> &);
void i_neutralize_poison(std::unique_ptr<object> &);
void i_no_op(std::unique_ptr<object> &);
void i_nothing(std::unique_ptr<object> &);
void i_objdet(std::unique_ptr<object> &);
void i_pepper_food(std::unique_ptr<object> &);
void i_perm_accuracy(std::unique_ptr<object> &);
void i_perm_agility(std::unique_ptr<object> &);
void i_perm_displace(std::unique_ptr<object> &);
void i_perm_hero(std::unique_ptr<object> &);
void i_perm_illuminate(std::unique_ptr<object> &);
void i_perm_invisible(std::unique_ptr<object> &);
void i_perm_levitate(std::unique_ptr<object> &);
void i_perm_negimmune(std::unique_ptr<object> &);
void i_perm_protection(std::unique_ptr<object> &);
void i_perm_speed(std::unique_ptr<object> &);
void i_perm_truesight(std::unique_ptr<object> &);
void i_pick(std::unique_ptr<object> &);
void i_poison_food(std::unique_ptr<object> &);
void i_pow(std::unique_ptr<object> &);
void i_raise_portcullis(std::unique_ptr<object> &);
void i_regenerate(std::unique_ptr<object> &);
void i_restore(std::unique_ptr<object> &);
void i_sleep_self(std::unique_ptr<object> &);
void i_speed(std::unique_ptr<object> &);
void i_spells(std::unique_ptr<object> &);
void i_stim(std::unique_ptr<object> &);
void i_teleport(std::unique_ptr<object> &);
void i_torch_use(std::unique_ptr<object> &);
void i_torch_unequip(std::unique_ptr<object> &);
void i_trap(std::unique_ptr<object> &);
void i_truesight(std::unique_ptr<object> &);
void i_warp(std::unique_ptr<object> &);
void i_wish(std::unique_ptr<object> &);

// itemf2.cpp functions
void i_defend(std::unique_ptr<object> &);
void i_demonblade(std::unique_ptr<object> &);
void i_desecrate(std::unique_ptr<object> &);
void i_lightsabre(std::unique_ptr<object> &);
void i_mace_disrupt(std::unique_ptr<object> &);
void i_normal_armor(std::unique_ptr<object> &);
void i_normal_shield(std::unique_ptr<object> &);
void i_normal_weapon(std::unique_ptr<object> &);
void i_perm_breathing(std::unique_ptr<object> &);
void i_perm_burden(std::unique_ptr<object> &);
void i_perm_deflect(std::unique_ptr<object> &);
void i_perm_energy_resist(std::unique_ptr<object> &);
void i_perm_fear_resist(std::unique_ptr<object> &);
void i_perm_fire_resist(std::unique_ptr<object> &);
void i_perm_gaze_immune(std::unique_ptr<object> &);
void i_perm_knowledge(std::unique_ptr<object> &);
void i_perm_poison_resist(std::unique_ptr<object> &);
void i_perm_regenerate(std::unique_ptr<object> &);
void i_perm_strength(std::unique_ptr<object> &);
void i_victrix(std::unique_ptr<object> &);
void weapon_acidwhip(int, std::unique_ptr<object> &, monster *);
void weapon_arrow(int, std::unique_ptr<object> &, monster *);
void weapon_bare_hands(int, monster *);
void weapon_bolt(int, std::unique_ptr<object> &, monster *);
void weapon_defend(int, std::unique_ptr<object> &, monster *);
void weapon_demonblade(int, std::unique_ptr<object> &, monster *);
void weapon_desecrate(int, std::unique_ptr<object> &, monster *);
void weapon_firestar(int, std::unique_ptr<object> &, monster *);
void weapon_lightsabre(int, std::unique_ptr<object> &, monster *);
void weapon_mace_disrupt(int, std::unique_ptr<object> &, monster *);
void weapon_normal_hit(int, std::unique_ptr<object> &, monster *);
void weapon_scythe(int, std::unique_ptr<object> &, monster *);
void weapon_tangle(int, std::unique_ptr<object> &, monster *);
void weapon_victrix(int, std::unique_ptr<object> &, monster *);
void weapon_vorpal(int, std::unique_ptr<object> &, monster *);

// itemf3.cpp functions
void i_antioch(std::unique_ptr<object> &);
void i_apport(std::unique_ptr<object> &);
void i_crystal(std::unique_ptr<object> &);
void i_death(std::unique_ptr<object> &);
void i_disintegrate(std::unique_ptr<object> &);
void i_dispel(std::unique_ptr<object> &);
void i_disrupt(std::unique_ptr<object> &);
void i_enchantment(std::unique_ptr<object> &);
void i_fear(std::unique_ptr<object> &);
void i_fireball(std::unique_ptr<object> &);
void i_firebolt(std::unique_ptr<object> &);
void i_helm(std::unique_ptr<object> &);
void i_hide(std::unique_ptr<object> &);
void i_juggernaut(std::unique_ptr<object> &);
void i_kolwynia(std::unique_ptr<object> &);
void i_lball(std::unique_ptr<object> &);
void i_lbolt(std::unique_ptr<object> &);
void i_life(std::unique_ptr<object> &);
void i_missile(std::unique_ptr<object> &);
void i_orbair(std::unique_ptr<object> &);
void i_orbdead(std::unique_ptr<object> &);
void i_orbearth(std::unique_ptr<object> &);
void i_orbfire(std::unique_ptr<object> &);
void i_orbmastery(std::unique_ptr<object> &);
void i_orbwater(std::unique_ptr<object> &);
void i_planes(std::unique_ptr<object> &);
void i_polymorph(std::unique_ptr<object> &);
void i_sceptre(std::unique_ptr<object> &);
void i_sleep_other(std::unique_ptr<object> &);
void i_snowball(std::unique_ptr<object> &);
void i_stargem(std::unique_ptr<object> &);
void i_summon(std::unique_ptr<object> &);
void i_symbol(std::unique_ptr<object> &);

// lev.cpp functions
int difficulty();
std::unique_ptr<monster> m_create(int, int, int, int);
std::unique_ptr<monster> make_creature(int);
void make_country_monsters(chtype);
void make_site_monster(int, int, int);
void make_site_treasure(int, int, int);
void make_specific_treasure(int, int, int);
void populate_level(int);
void stock_level();
void wandercheck();

// mmelee.cpp functions
char random_loc();
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
const char *mantype();
void determine_npc_behavior(monster *, int, int);
void m_abyss(monster *);
void m_altar(monster *);
void m_damage(monster *, int, damage_type);
void m_death(monster *);
void m_dropstuff(monster *);
void m_fire(monster *);
void m_lava(monster *);
void m_no_op(monster *);
void m_pickup(monster *, std::unique_ptr<object>);
void m_pulse(monster *);
void m_trap_abyss(monster *);
void m_trap_acid(monster *);
void m_trap_blade(monster *);
void m_trap_dart(monster *);
void m_trap_disintegrate(monster *);
void m_trap_door(monster *);
void m_trap_fire(monster *);
void m_trap_manadrain(monster *);
void m_trap_pit(monster *);
void m_trap_sleepgas(monster *);
void m_trap_snare(monster *);
void m_trap_teleport(monster *);
void m_water(monster *);
void make_hiscore_npc(monster *, int);
void make_log_npc(monster *);
void monster_talk(monster *);
void movemonster(monster *, int, int);

// move.cpp functions
void l_abyss();
void l_air_station();
void l_arena_exit();
void l_balancestone();
void l_chaos();
void l_chaostone();
void l_circle_library();
void l_drop_every_portcullis();
void l_earth_station();
void l_enter_circle();
void l_enter_court();
void l_escalator();
void l_fire();
void l_fire_station();
void l_hedge();
void l_house_exit();
void l_lava();
void l_lawstone();
void l_lift();
void l_magic_pool();
void l_mindstone();
void l_no_op();
void l_portcullis_trap();
void l_raise_portcullis();
void l_rubble();
void l_sacrificestone();
void l_tactical_exit();
void l_temple_warning();
void l_throne();
void l_tome1();
void l_tome2();
void l_voice1();
void l_voice2();
void l_voice3();
void l_void();
void l_void_station();
void l_voidstone();
void l_water();
void l_water_station();
void l_whirlwind();

// movef.cpp functions
void m_movefunction(monster *, int);
void p_movefunction(int);

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
void l_altar();

// save.cpp functions
bool restore_game(const std::string &);
bool save_game(const std::string &);
void signalsave(int = 0);

// site1.cpp functions
void l_alchemist();
void l_armorer();
void l_bank();
void l_casino();
void l_club();
void l_commandant();
void l_crap();
void l_diner();
void l_dpw();
void l_gym();
void l_healer();
void l_library();
void l_pawn_shop();
void l_statue_wake();
void l_tavern();
void statue_random(int, int);

// site2.cpp functions
void cureforpay();
void gymtrain(int *, int *);
void healforpay();
void l_adept();
void l_brothel();
void l_cartographer();
void l_charity();
void l_condo();
void l_countryside();
void l_house();
void l_hovel();
void l_mansion();
void l_oracle();
void l_safe();
void l_triffid();
void l_vault();
void pacify_guards();
void send_to_jail();
void sign_print(int, int, int);

// time.cpp functions
void time_clock(int);

// trap.cpp functions
void l_trap_abyss();
void l_trap_acid();
void l_trap_blade();
void l_trap_dart();
void l_trap_disintegrate();
void l_trap_door();
void l_trap_fire();
void l_trap_manadrain();
void l_trap_pit();
void l_trap_siren();
void l_trap_sleepgas();
void l_trap_snare();
void l_trap_teleport();

// util.cpp functions
long calc_points();
const std::string getarticle(const std::string &);
const std::string ordinal(int);
bool confirmation();
char cryptkey(const std::string &);
int day();
int distance(int, int, int, int);
bool hitp(int, int);
int hour();
bool inbounds(int, int);
bool los_p(int, int, int, int);
bool m_unblocked(const monster *, int, int);
bool nighttime();
bool offscreen(int, int);
bool ok_to_free(level *);
void free_level(level *);
int random_range(int);
int screenmod(int);
int screenmod_horizontal(int);
int showhour();
int showminute();
bool unblocked(int, int);
bool view_los_p(int, int, int, int);
bool view_unblocked(int, int);
void calc_weight();
void do_los(chtype, int *, int *, int, int);
void do_object_los(chtype, int *, int *, int, int);
bool findspace(int *, int *, int);
void initdirs();

// village.cpp functions
void load_village(int, int);

#endif
