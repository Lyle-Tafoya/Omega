/* omega copyright (c) 1987,1988,1989 by Laurence Raphael Brothers */
/* extern.h -- function declarations */

#ifdef __STDC__
#define ARGS(x) x
#else
#define ARGS(x) ()
#endif

/* omega.c functions */

int main ARGS((int,char *[]));
int game_restore ARGS((int,char *[]));
void init_world ARGS((void));
void inititem ARGS((int));
void initrand ARGS((int, int));
void signalexit ARGS((void));
void signalsave ARGS((void));

/* abyss.c functions */

#ifdef MSDOS_SUPPORTED_ANTIQUE
void check_memory ARGS((void));
void kill_all_levels ARGS((void));
void kill_levels ARGS((char *));
void msdos_init ARGS((void));
plv msdos_changelevel ARGS((plv,int,int));
#endif
void load_abyss ARGS((void));

/* aux1.c functions */

char *levelname ARGS((int));
char *mstatus_string ARGS((struct monster *));
char *trapid ARGS((int));
int getdir ARGS((void));
int goberserk ARGS((void));
int p_country_moveable ARGS((int,int));
int p_moveable ARGS((int,int));
int player_on_sanctuary ARGS((void));
void calc_melee ARGS((void));
int damage_item ARGS((pob));
void describe_player ARGS((void));
void fight_monster ARGS((struct monster *));
void foodcheck ARGS((void));
void gain_experience ARGS((int));
void p_damage ARGS((int,int,char *));
void p_death ARGS((char *));
void roomcheck ARGS((void));
void searchat ARGS((int,int));
void setspot ARGS((int *,int *));
void showroom ARGS((int));
void surrender ARGS((struct monster *));
void threaten ARGS((struct monster *));
void tunnelcheck ARGS((void));

/* aux2.c functions */

long expval ARGS((int));
long item_value ARGS((pob));
long true_item_value ARGS((pob));
char *actionlocstr ARGS((char));
int p_immune ARGS((int));
int player_hit ARGS((int,char,struct monster *));
int statmod ARGS((int));
void break_weapon ARGS((void));
void change_environment ARGS((char));
void drop_weapon ARGS((void));
void enter_site ARGS((Symbol));
void gain_level ARGS((void));
void minute_status_check ARGS((void));
void moon_check ARGS((void));
void movecursor ARGS((int *,int *,int,int));
void p_drown ARGS((void));
void p_fumble ARGS((int));
void p_hit  ARGS((struct monster *,int,int));
void p_win ARGS((void));
void player_miss ARGS((struct monster *,int));
void tacplayer ARGS((struct monster *));
void tenminute_status_check ARGS((void));
void toggle_item_use ARGS((int));
void torch_check ARGS((void));
void weapon_use ARGS((int,pob,struct monster *));

/* aux3.c functions */

char *citysiteid ARGS((int));
char *countryid ARGS((Symbol));
char getlocation ARGS((void));
int hostilemonstersnear ARGS((void));
int magic_resist ARGS((int));
int maneuvers ARGS((void));
int parsecitysite ARGS((void));
int stonecheck ARGS((int));
void alert_guards ARGS((void));
void countrysearch ARGS((void));
void default_maneuvers ARGS((void));
void destroy_order ARGS((void));
void hourly_check ARGS((void));
void indoors_random_event ARGS((void));
void outdoors_random_event ARGS((void));
void showknownsites ARGS((int, int));
void tenminute_check ARGS((void));
void terrain_check ARGS((int));

/* char.c functions */

long calcmana ARGS((void));
FILE *omegarc_check ARGS((void));
int competence_check ARGS((int));
int fixnpc ARGS((int));
void initplayer ARGS((void));
void initstats ARGS((void));
void omegan_character_stats ARGS((void));
void save_omegarc ARGS((void));
void user_character_stats ARGS((void));

/* city.c functions */

void assign_city_function ARGS((int,int));
void load_city ARGS((int));
void make_justiciar ARGS((int,int));
void make_major_undead ARGS((int,int));
void make_minor_undead ARGS((int,int));
void mazesite ARGS((int,int,int));
void randommazesite ARGS((int,int,int));
void repair_jail ARGS((void));
void resurrect_guards ARGS((void));

/* command1.c functions */

void p_country_process ARGS((void));
void p_process ARGS((void));

/* command2.c functions */

 void pickup ARGS((void));
void activate ARGS((void));
void bash_item ARGS((void));
void bash_location ARGS((void));
void callitem ARGS((void));
void closedoor ARGS((void));
void disarm ARGS((void));
void downstairs ARGS((void));
void drop ARGS((void));
void eat ARGS((void));
void floor_inv ARGS((void));
void give ARGS((void));
void magic ARGS((void));
void movepincountry ARGS((int,int));
void moveplayer ARGS((int,int));
void opendoor ARGS((void));
void peruse ARGS((void));
void quaff ARGS((void));
void rest ARGS((void));
void save ARGS((int, int));
void search ARGS((int *));
void setoptions ARGS((void));
void talk ARGS((void));
void upstairs ARGS((void));
void zapwand ARGS((void));

/* command3.c functions */

void abortshadowform ARGS((void));
void charid ARGS((void));
void city_move ARGS((void));
void dismount_steed ARGS((void));
void examine ARGS((void));
void fire ARGS((void));
void frobgamestatus ARGS((void));
void help ARGS((void));
void hunt ARGS((Symbol));
void nap ARGS((void));
void pickpocket ARGS((void));
void quit ARGS((void));
void rename_player ARGS((void));
void tacoptions ARGS((void));
void tunnel ARGS((void));
void vault ARGS((void));
void version ARGS((void));
void wizard ARGS((void));

/* country.c functions */

void load_country ARGS((void));
void load_dlair ARGS((int,int));
void load_misle ARGS((int,int));
void load_speak ARGS((int,int));
void load_temple ARGS((int,int));
void make_high_priest ARGS((int,int,int));
void random_temple_site ARGS((int,int,int,int));

/* effect1.c functions */

int random_item ARGS((void));
void acquire ARGS((int));
void ball ARGS((int,int,int,int,int,int));
void bless ARGS((int));
void bolt ARGS((int,int,int,int,int,int,int));
void enchant ARGS((int));
void fball ARGS((int,int,int,int,int));
void fbolt ARGS((int,int,int,int,int,int));
void heal ARGS((int));
void identify ARGS((int));
void lball ARGS((int,int,int,int,int));
void lbolt ARGS((int,int,int,int,int,int));
void manastorm ARGS((int,int,int));
void mondet ARGS((int));
void nbolt ARGS((int,int,int,int,int,int));
void objdet ARGS((int));
void snowball ARGS((int,int,int,int,int));
void wish ARGS((int));

/* effect2.c functions */

void accuracy ARGS((int));
void alert ARGS((int));
void augment ARGS((int));
void breathe ARGS((int));
void displace ARGS((int));
void flux ARGS((int));
void haste ARGS((int));
void i_chaos ARGS((pob));
void i_law ARGS((pob));
void invisible ARGS((int));
void knowledge ARGS((int));
void recover_stat ARGS((int));
void regenerate ARGS((int));
void sanctify ARGS((int));
void warp ARGS((int));

/* effect3.c functions */

int itemlist ARGS((int,int));
int monsterlist ARGS((void));
void acid_cloud ARGS((void));
void aggravate ARGS((void));
void amnesia ARGS((void));
void annihilate ARGS((int));
void apport ARGS((int));
void clairvoyance ARGS((int));
void cleanse ARGS((int));
void cure ARGS((int));
void deflection ARGS((int));
void disease ARGS((int));
void disintegrate ARGS((int,int));
void dispel ARGS((int));
void disrupt ARGS((int,int,int));
void drain ARGS((int));
void drain_life ARGS((int));
void hellfire ARGS((int,int,int));
void hero ARGS((int));
void hide ARGS((int,int));
void illuminate ARGS((int));
void inflict_fear ARGS((int,int));
void learnspell ARGS((int));
void level_drain ARGS((int,char *));
void level_return ARGS((void));
void levitate ARGS((int));
void p_poison ARGS((int));
void p_teleport ARGS((int));
void polymorph ARGS((int));
void sanctuary ARGS((void));
void shadowform ARGS((void));
void sleep_monster ARGS((int));
void sleep_player ARGS((int));
void strategic_teleport ARGS((int));
void summon ARGS((int,int));
void truesight ARGS((int));

/* env.c functions */

void load_arena ARGS((void));
void load_circle ARGS((int));
void load_court ARGS((int));
void make_archmage ARGS((int,int));
void make_prime ARGS((int,int));

/* etc.c functions */

char *nameprint ARGS((void));
char *slotstr ARGS((int));
char *wordnum ARGS((int));
void hint ARGS((void));
void learnclericalspells ARGS((int,int));

/* file.c functions */

#ifndef MSDOS
void lock_score_file ARGS((void));
void unlock_score_file ARGS((void));
#endif
FILE *checkfopen ARGS((char *,char *));
int filecheck ARGS((void));
int test_file_access ARGS((char *, char));
void abyss_file ARGS((void));
void adeptfile ARGS((void));
void checkhigh ARGS((char *,int));
void cityguidefile ARGS((void));
void combat_help ARGS((void));
void commandlist ARGS((void));
void copyfile ARGS((char *));
void displayfile ARGS((char *));
void displaycryptfile ARGS((char *));
void extendlog ARGS((char *,int));
void filescanstring ARGS((FILE *,char *));	/* in util.c if MSDOS defined */
void inv_help ARGS((void));
void save_hiscore_npc ARGS((int));
void show_license ARGS((void));
void showmotd ARGS((void));
void showscores ARGS((void));
void theologyfile ARGS((void));
void user_intro ARGS((void));
void wishfile ARGS((void));

/* gen1.c functions */

char *roomname ARGS((int));
plv findlevel ARGS((struct level *,char));
void build_room ARGS((int,int,int,char,int));
void build_square_room ARGS((int,int,int,char,int));
void cavern_level ARGS((void));
void change_level ARGS((char,char,char));
void clear_level ARGS((struct level *));
void corridor_crawl ARGS((int *,int *,int,int,int,Symbol,char));
void find_stairs ARGS((char,char));
void free_dungeon ARGS((void));
void install_specials ARGS((void));
void install_traps ARGS((void));
void makedoor ARGS((int,int));
void sewer_corridor ARGS((int,int,int,int,Symbol));
void sewer_level ARGS((void));
void straggle_corridor ARGS((int,int,int,int,Symbol,char));

/* gen2.c functions */

void make_country_screen ARGS((Symbol));
void make_general_map ARGS((char *));
void make_forest ARGS((void));
void make_jungle ARGS((void));
void make_mountains ARGS((void));
void make_plains ARGS((void));
void make_river ARGS((void));
void make_road ARGS((void));
void make_stairs ARGS((int));
void make_swamp ARGS((void));
void maze_corridor ARGS((int,int,int,int,char,char));
void maze_level ARGS((void));
void room_corridor ARGS((int,int,int,int,int));
void room_level ARGS((void));

/* guild1.c functions */

void l_arena ARGS((void));
void l_castle ARGS((void));
void l_merc_guild ARGS((void));

/* guild2.c functions */

void l_college ARGS((void));
void l_order ARGS((void));
void l_sorcerors ARGS((void));
void l_thieves_guild ARGS((void));

/* house.c functions */

void load_house ARGS((int,int));
void make_house_npc ARGS((int,int));
void make_mansion_npc ARGS((int,int));

/* inv.c functions */

void do_inventory_control ARGS((void));
int  key_to_index ARGS((signed char));
signed char index_to_key ARGS((int));
long get_money ARGS((long));
char *cashstr ARGS((void));
char *itemid ARGS((pob));
int baditem ARGS((int));
int badobject ARGS((char));
int cursed ARGS((pob));
int find_and_remove_item ARGS((int,int));
int find_item ARGS((pob *,int,int));
int get_inventory_slot ARGS((void));
int get_item_number ARGS((pob));
int get_to_pack ARGS((pob));
int getitem ARGS((Symbol));
int item_useable ARGS((pob,int));
int objequal ARGS((struct object *,struct object *));
int slottable ARGS((pob,int));
int take_from_pack ARGS((int,int));
pob detach_money ARGS((void));
pob split_item ARGS((int,pob));
void add_to_pack ARGS((pob));
void conform_lost_object ARGS((pob));
void conform_lost_objects ARGS((int,pob));
void conform_unused_object ARGS((pob));
void dispose_lost_objects ARGS((int,pob));
void drop_at ARGS((int,int,pob));
void drop_from_slot ARGS((int));
void drop_money ARGS((void));
void fixpack ARGS((void));
void gain_item ARGS((pob));
void give_money ARGS((struct monster *));
void givemonster ARGS((struct monster *, struct object *));
void inventory_control ARGS((void));
void item_inventory ARGS((int));
void lose_all_items ARGS((void));
void merge_item ARGS((int));
void p_drop_at ARGS((int,int,int,pob));
void pack_extra_items ARGS((pob));
void pickup_at ARGS((int,int));
void put_to_pack ARGS((int));
void setchargestr ARGS((pob,char *));
void setnumstr ARGS((pob,char *));
void setplustr ARGS((pob,char *));
void show_inventory_slot ARGS((int,int));
void switch_to_slot ARGS((int));
void top_inventory_control ARGS((void));

/* item.c functions */

void shuffle ARGS((int [], int));
char *bootname ARGS((int));
char *cloakname ARGS((int));
char *grotname ARGS((void));
char *potionname ARGS((int));
char *ringname ARGS((int));
char *scrollname ARGS((int));
char *stickname ARGS((int));
int itemblessing ARGS((void));
int itemcharge ARGS((void));
int itemplus ARGS((void));
int twohandedp ARGS((int));
pob create_object ARGS((int));
void item_use ARGS((struct object *));
void make_armor ARGS((pob, int));
void make_artifact ARGS((pob, int));
void make_boots ARGS((pob, int));
void make_cash ARGS((pob, int));
void make_cloak ARGS((pob, int));
void make_corpse ARGS((pob, struct monster *));
void make_food ARGS((pob, int));
void make_potion ARGS((pob, int));
void make_ring ARGS((pob, int));
void make_scroll ARGS((pob, int));
void make_shield ARGS((pob, int));
void make_stick ARGS((pob, int));
void make_thing ARGS((pob, int));
void make_weapon ARGS((pob, int));

/* itemf1.c functions */

void i_accuracy ARGS((pob));
void i_acquire ARGS((pob));
void i_alert ARGS((pob));
void i_augment ARGS((pob));
void i_azoth ARGS((pob));
void i_bless ARGS((pob));
void i_breathing ARGS((pob));
void i_charge ARGS((pob));
void i_clairvoyance ARGS((pob));
void i_corpse ARGS((pob));
void i_cure ARGS((pob));
void i_deflect ARGS((pob));
void i_displace ARGS((pob));
void i_enchant ARGS((pob));
void i_fear_resist ARGS((pob));
void i_flux ARGS((pob));
void i_food ARGS((pob));
void i_heal ARGS((pob));
void i_hero ARGS((pob));
void i_id ARGS((pob));
void i_illuminate ARGS((pob));
void i_immune ARGS((pob));
void i_invisible ARGS((pob));
void i_jane_t ARGS((pob));
void i_key ARGS((pob));
void i_knowledge ARGS((pob));
void i_lembas ARGS((pob));
void i_levitate ARGS((pob));
void i_mondet ARGS((pob));
void i_neutralize_poison ARGS((pob));
void i_no_op ARGS((pob));
void i_nothing ARGS((pob));
void i_objdet ARGS((pob));
void i_pepper_food ARGS((pob));
void i_perm_accuracy ARGS((pob));
void i_perm_agility ARGS((pob));
void i_perm_displace ARGS((pob));
void i_perm_hero ARGS((pob));
void i_perm_illuminate ARGS((pob));
void i_perm_invisible ARGS((pob));
void i_perm_levitate ARGS((pob));
void i_perm_negimmune ARGS((pob));
void i_perm_protection ARGS((pob));
void i_perm_speed ARGS((pob));
void i_perm_truesight ARGS((pob));
void i_pick ARGS((pob));
void i_poison_food ARGS((pob));
void i_pow ARGS((pob));
void i_raise_portcullis ARGS((pob));
void i_regenerate ARGS((pob));
void i_restore ARGS((pob));
void i_sleep_self ARGS((pob));
void i_speed ARGS((pob));
void i_spells ARGS((pob));
void i_stim ARGS((pob));
void i_teleport ARGS((pob));
void i_trap ARGS((pob));
void i_truesight ARGS((pob));
void i_warp ARGS((pob));
void i_wish ARGS((pob));

/* itemf2.c functions */

void i_defend ARGS((pob));
void i_demonblade ARGS((pob));
void i_desecrate ARGS((pob));
void i_lightsabre ARGS((pob));
void i_mace_disrupt ARGS((pob));
void i_normal_armor ARGS((pob));
void i_normal_shield ARGS((pob));
void i_normal_weapon ARGS((pob));
void i_perm_breathing ARGS((pob));
void i_perm_burden ARGS((pob));
void i_perm_deflect ARGS((pob));
void i_perm_energy_resist ARGS((pob));
void i_perm_fear_resist ARGS((pob));
void i_perm_fire_resist ARGS((pob));
void i_perm_gaze_immune ARGS((pob));
void i_perm_knowledge ARGS((pob));
void i_perm_poison_resist ARGS((pob));
void i_perm_regenerate ARGS((pob));
void i_perm_strength ARGS((pob));
void i_victrix ARGS((pob));
void weapon_acidwhip ARGS((int, pob, struct monster *));
void weapon_arrow ARGS((int, pob, struct monster *));
void weapon_bare_hands ARGS((int, struct monster *));
void weapon_bolt ARGS((int, pob, struct monster *));
void weapon_defend ARGS((int, pob, struct monster *));
void weapon_demonblade ARGS((int, pob, struct monster *));
void weapon_desecrate ARGS((int, pob, struct monster *));
void weapon_firestar ARGS((int, pob, struct monster *));
void weapon_lightsabre ARGS((int, pob, struct monster *));
void weapon_mace_disrupt ARGS((int, pob, struct monster *));
void weapon_normal_hit ARGS((int, pob, struct monster *));
void weapon_scythe ARGS((int, pob, struct monster *));
void weapon_tangle ARGS((int, pob, struct monster *));
void weapon_victrix ARGS((int, pob, struct monster *));
void weapon_vorpal ARGS((int, pob, struct monster *));

/* itemf3.c functions */

int orbcheck ARGS((char));
void i_antioch ARGS((pob));
void i_apport ARGS((pob));
void i_crystal ARGS((pob));
void i_death ARGS((pob));
void i_disintegrate ARGS((pob));
void i_dispel ARGS((pob));
void i_disrupt ARGS((pob));
void i_enchantment ARGS((pob));
void i_fear ARGS((pob));
void i_fireball ARGS((pob));
void i_firebolt ARGS((pob));
void i_helm ARGS((pob));
void i_hide ARGS((pob));
void i_juggernaut ARGS((pob));
void i_kolwynia ARGS((pob));
void i_lball ARGS((pob));
void i_lbolt ARGS((pob));
void i_life ARGS((pob));
void i_missile ARGS((pob));
void i_orbair ARGS((pob));
void i_orbdead ARGS((pob));
void i_orbearth ARGS((pob));
void i_orbfire ARGS((pob));
void i_orbmastery ARGS((pob));
void i_orbwater ARGS((pob));
void i_planes ARGS((pob));
void i_polymorph ARGS((pob));
void i_sceptre ARGS((pob));
void i_sleep_other ARGS((pob));
void i_snowball ARGS((pob));
void i_stargem ARGS((pob));
void i_summon ARGS((pob));
void i_symbol ARGS((pob));

/* lev.c functions */

int difficulty ARGS((void));	/* this function in util.c if MSDOS defined */
pmt m_create ARGS((int,int,int,int));
pmt make_creature ARGS((int));
void make_country_monsters ARGS((Symbol));
void make_site_monster ARGS((int,int,int));
void make_site_treasure ARGS((int,int,int));
void make_specific_treasure ARGS((int,int,int));
void populate_level ARGS((int));
void stock_level ARGS((void));
void wandercheck ARGS((void));

/* mmelee.c functions */

char random_loc ARGS((void));
int monster_hit ARGS((struct monster *,char,int));
void m_hit ARGS((struct monster *,int));
void monster_melee ARGS((struct monster *,char,int));
void tacmonster ARGS((struct monster *));
void transcribe_monster_actions ARGS((struct monster *));

/* mmove.c functions */

void m_confused_move ARGS((struct monster *));
void m_flutter_move ARGS((struct monster *));
void m_follow_move ARGS((struct monster *));
void m_move_animal ARGS((struct monster *));
void m_normal_move ARGS((struct monster *));
void m_random_move ARGS((struct monster *));
void m_scaredy_move ARGS((struct monster *));
void m_simple_move ARGS((struct monster *));
void m_smart_move ARGS((struct monster *));
void m_spirit_move ARGS((struct monster *));
void m_teleport ARGS((struct monster *));
void m_vanish ARGS((struct monster *));
void m_move_leash ARGS((struct monster *));

/* mon.c functions */

char *mantype ARGS((void));
void determine_npc_behavior ARGS((pmt,int,int));
void m_abyss ARGS((struct monster *));
void m_altar ARGS((struct monster *));
void m_damage ARGS((struct monster *,int,int));
void m_death ARGS((struct monster *));
void m_dropstuff ARGS((struct monster *));
void m_fire ARGS((struct monster *));
void m_lava ARGS((struct monster *));
void m_no_op ARGS((struct monster *));
void m_pickup ARGS((struct monster *,struct object *));
void m_pulse ARGS((struct monster *));
void m_trap_abyss ARGS((struct monster *));
void m_trap_acid ARGS((struct monster *));
void m_trap_blade ARGS((struct monster *));
void m_trap_dart ARGS((struct monster *));
void m_trap_disintegrate ARGS((struct monster *));
void m_trap_door ARGS((struct monster *));
void m_trap_fire ARGS((struct monster *));
void m_trap_manadrain ARGS((struct monster *));
void m_trap_pit ARGS((struct monster *));
void m_trap_sleepgas ARGS((struct monster *));
void m_trap_snare ARGS((struct monster *));
void m_trap_teleport ARGS((struct monster *));
void m_water ARGS((struct monster *));
void make_hiscore_npc ARGS((pmt,int));
void make_log_npc ARGS((struct monster *));
void monster_action ARGS((struct monster *,int));
void monster_move ARGS((struct monster *));
void monster_special ARGS((struct monster *));
void monster_strike ARGS((struct monster *));
void monster_talk ARGS((struct monster *));
void movemonster ARGS((struct monster *,int,int));
void strengthen_death ARGS((struct monster *));

/* move.c functions */

void l_abyss ARGS((void));
void l_air_station ARGS((void));
void l_arena_exit ARGS((void));
void l_balancestone ARGS((void));
void l_chaos ARGS((void));
void l_chaostone ARGS((void));
void l_circle_library ARGS((void));
void l_drop_every_portcullis ARGS((void));
void l_earth_station ARGS((void));
void l_enter_circle ARGS((void));
void l_enter_court ARGS((void));
void l_escalator ARGS((void));
void l_fire ARGS((void));
void l_fire_station ARGS((void));
void l_hedge ARGS((void));
void l_house_exit ARGS((void));
void l_lava ARGS((void));
void l_lawstone ARGS((void));
void l_lift ARGS((void));
void l_magic_pool ARGS((void));
void l_mindstone ARGS((void));
void l_no_op ARGS((void));
void l_portcullis_trap ARGS((void));
void l_raise_portcullis ARGS((void));
void l_rubble ARGS((void));
void l_sacrificestone ARGS((void));
void l_tactical_exit ARGS((void));
void l_temple_warning ARGS((void));
void l_throne ARGS((void));
void l_tome1 ARGS((void));
void l_tome2 ARGS((void));
void l_voice1 ARGS((void));
void l_voice2 ARGS((void));
void l_voice3 ARGS((void));
void l_void ARGS((void));
void l_void_station ARGS((void));
void l_voidstone ARGS((void));
void l_water ARGS((void));
void l_water_station ARGS((void));
void l_whirlwind ARGS((void));
void stationcheck ARGS((void));

/* movef.c functions */

void m_movefunction ARGS((struct monster *,int));
void p_movefunction ARGS((int));
void sign_print ARGS((int,int,int));	/* this function in site1.c if MSDOS defined */

/* mspec.c functions */

void m_aggravate ARGS((struct monster *));
void m_huge_sounds ARGS((struct monster *));
void m_illusion ARGS((struct monster *));
void m_sp_acid_cloud ARGS((struct monster *));
void m_sp_angel ARGS((struct monster *));
void m_sp_av ARGS((struct monster *));
void m_sp_blackout ARGS((struct monster *));
void m_sp_bogthing ARGS((struct monster *));
void m_sp_court ARGS((struct monster *));
void m_sp_demon ARGS((struct monster *));
void m_sp_demonlover ARGS((struct monster *));
void m_sp_dragonlord ARGS((struct monster *));
void m_sp_eater ARGS((struct monster *));
void m_sp_escape ARGS((struct monster *));
void m_sp_explode ARGS((struct monster *));
void m_sp_ghost ARGS((struct monster *));
void m_sp_lair ARGS((struct monster *));
void m_sp_lw ARGS((struct monster *));
void m_sp_mb ARGS((struct monster *));
void m_sp_merchant ARGS((struct monster *));
void m_sp_mirror ARGS((struct monster *));
void m_sp_mp ARGS((struct monster *));
void m_sp_ng ARGS((struct monster *));
void m_sp_poison_cloud ARGS((struct monster *));
void m_sp_prime ARGS((struct monster *));
void m_sp_raise ARGS((struct monster *));
void m_sp_seductor ARGS((struct monster *));
void m_sp_servant ARGS((struct monster *));
void m_sp_spell ARGS((struct monster *));
void m_sp_surprise ARGS((struct monster *));
void m_sp_swarm ARGS((struct monster *));
void m_sp_were ARGS((struct monster *));
void m_sp_whistleblower ARGS((struct monster *));
void m_summon ARGS((struct monster *));
void m_thief_f ARGS((struct monster *));

/* mstrike.c functions */

void m_blind_strike ARGS((struct monster *));
void m_fireball ARGS((struct monster *));
void m_firebolt ARGS((struct monster *));
void m_lball ARGS((struct monster *));
void m_nbolt ARGS((struct monster *));
void m_snowball ARGS((struct monster *));
void m_strike_sonic ARGS((struct monster *));

/* mtalk.c functions */

void m_talk_animal ARGS((struct monster *));
void m_talk_archmage ARGS((struct monster *));
void m_talk_assassin ARGS((struct monster *));
void m_talk_beg ARGS((struct monster *));
void m_talk_burble ARGS((struct monster *));
void m_talk_demonlover ARGS((struct monster *));
void m_talk_druid ARGS((struct monster *));
void m_talk_ef ARGS((struct monster *));
void m_talk_evil ARGS((struct monster *));
void m_talk_gf ARGS((struct monster *));
void m_talk_greedy ARGS((struct monster *));
void m_talk_guard ARGS((struct monster *));
void m_talk_hint ARGS((struct monster *));
void m_talk_horse ARGS((struct monster *));
void m_talk_hungry ARGS((struct monster *));
void m_talk_hyena ARGS((struct monster *));
void m_talk_im ARGS((struct monster *));
void m_talk_man ARGS((struct monster *));
void m_talk_merchant ARGS((struct monster *));
void m_talk_mimsy ARGS((struct monster *));
void m_talk_mp ARGS((struct monster *));
void m_talk_ninja ARGS((struct monster *));
void m_talk_parrot ARGS((struct monster *));
void m_talk_prime ARGS((struct monster *));
void m_talk_robot ARGS((struct monster *));
void m_talk_scream ARGS((struct monster *));
void m_talk_seductor ARGS((struct monster *));
void m_talk_servant ARGS((struct monster *));
void m_talk_silent ARGS((struct monster *));
void m_talk_slithy ARGS((struct monster *));
void m_talk_stupid ARGS((struct monster *));
void m_talk_thief ARGS((struct monster *));
void m_talk_titter ARGS((struct monster *));

/* priest.c functions */

int check_sacrilege ARGS((int));
void answer_prayer ARGS((void));
void hp_req_print ARGS((void));
void hp_req_test ARGS((void));
int increase_priest_rank ARGS((int));
void l_altar ARGS((void));
void make_hp ARGS((pob));

/* save.c functions */

int ok_outdated ARGS((int));
int restore_game ARGS((char *));
void restore_hiscore_npc ARGS((pmt, int));
pob restore_item ARGS((FILE *, int));
pol restore_itemlist ARGS((FILE *, int));
void restore_country ARGS((FILE *, int));
void restore_level ARGS((FILE *, int));
void restore_monsters ARGS((FILE *,plv, int));
void restore_player ARGS((FILE *, int));
int save_game ARGS((int,char *));
int save_country ARGS((FILE *));
int save_item ARGS((FILE *,pob));
int save_itemlist ARGS((FILE *,pol));
int save_level ARGS((FILE *,plv));
int save_monsters ARGS((FILE *,pml));
int save_player ARGS((FILE *));
void signalsave ARGS((void));

/* scr.c functions */

int mcigetc ARGS((void));
long parsenum ARGS((void));
char *msgscanstring ARGS((void));
Symbol getspot ARGS((int,int,int));
char lgetc ARGS((void));
char menugetc ARGS((void));
char mgetc ARGS((void));
int ynq ARGS((void));
int ynq1 ARGS((void));
int ynq2 ARGS((void));
int getnumber ARGS((int));
int litroom ARGS((int,int));
int move_slot ARGS((int,int,int));
int stillonblock ARGS((void));
void blankoutspot ARGS((int,int));
void blotspot ARGS((int,int));
void buffercycle ARGS((char *));
int bufferappend ARGS((char *));
void bufferprint ARGS((void));
void checkclear ARGS((void));
void clear_if_necessary ARGS((void));
void clear_screen ARGS((void));
void clearmsg ARGS((void));
void clearmsg1 ARGS((void));
void clearmsg3 ARGS((void));
void colour_on ARGS((void));
void colour_off ARGS((void));
void commanderror ARGS((void));
void comwinprint ARGS((void));
void dataprint ARGS((void));
void deathprint ARGS((void));
void display_bigwin ARGS((void));
void display_death ARGS((char *));
void display_inventory_slot ARGS((int,int));
void display_option_slot ARGS((int));
void display_options ARGS((void));
void display_pack ARGS((void));
void display_possessions ARGS((void));
void display_quit ARGS((void));
void display_win ARGS((void));
void dobackspace ARGS((void));
void dodrawspot ARGS((int,int));
void draw_explosion ARGS((Symbol,int,int));
void drawmonsters ARGS((int));
void drawomega ARGS((void));
void drawplayer ARGS((void));
void drawscreen ARGS((void));
void drawspot ARGS((int,int));
void drawvision ARGS((int,int));
void endgraf ARGS((void));
void erase_level ARGS((void));
void erase_monster ARGS((struct monster *));
void hide_line ARGS((int));
void initgraf ARGS((void));
void levelrefresh ARGS((void));
void lightspot ARGS((int,int));
void locprint ARGS((char *));
void maddch ARGS((char));
void menuaddch ARGS((char));
void menuclear ARGS((void));
void menulongprint ARGS((long));
void menunumprint ARGS((int));
void menuprint ARGS((char *));
void menuspellprint ARGS((int));
void showmenu ARGS((void));
void mlongprint ARGS((long));
void mnumprint ARGS((int));
void morewait ARGS((void));
void mprint ARGS((char *));
void nprint1 ARGS((char *));
void nprint2 ARGS((char *));
void nprint3 ARGS((char *));
void phaseprint ARGS((void));
void plotchar ARGS((Symbol,int,int));
void plotmon ARGS((struct monster *));
void plotspot ARGS((int,int,int));
void print1 ARGS((char *));
void print2 ARGS((char *));
void print3 ARGS((char *));
void putspot ARGS((int,int,Symbol));
void redraw ARGS((void));
void screencheck ARGS((int));
void setlastxy ARGS((int, int));
void show_screen ARGS((void));
void omshowcursor ARGS((int,int));
void showflags ARGS((void));
void spreadroomdark ARGS((int,int,int));
void spreadroomlight ARGS((int,int,int));
void timeprint ARGS((void));
void title ARGS((void));
void xredraw ARGS((void));

/* site1.c functions */

void buyfromstock ARGS((int,int));
void l_alchemist ARGS((void));
void l_armorer ARGS((void));
void l_bank ARGS((void));
void l_casino ARGS((void));
void l_club ARGS((void));
void l_commandant ARGS((void));
void l_crap ARGS((void));
void l_diner ARGS((void));
void l_dpw ARGS((void));
void l_gym ARGS((void));
void l_healer ARGS((void));
void l_library ARGS((void));
void l_pawn_shop ARGS((void));
void l_statue_wake ARGS((void));
void l_tavern ARGS((void));
void statue_random ARGS((int,int));
void wake_statue ARGS((int,int,int));

/* site2.c functions */

void cureforpay ARGS((void));
void gymtrain ARGS((int *,int *));
void healforpay ARGS((void));
void l_adept ARGS((void));
void l_brothel ARGS((void));
void l_cartographer ARGS((void));
void l_charity ARGS((void));
void l_condo ARGS((void));
void l_countryside ARGS((void));
void l_house ARGS((void));
void l_hovel ARGS((void));
void l_mansion ARGS((void));
void l_oracle ARGS((void));
void l_safe ARGS((void));
void l_trifid ARGS((void));
void l_vault ARGS((void));
void pacify_guards ARGS((void));
void send_to_jail ARGS((void));

/* spell.c functions */

char *spellid ARGS((int));
int getspell ARGS((void));
int spellparse ARGS((void));
void cast_spell ARGS((int));
void initspells ARGS((void));
void s_accuracy ARGS((void));
void s_alert ARGS((void));
void s_apport ARGS((void));
void s_bless ARGS((void));
void s_breathe ARGS((void));
void s_clairvoyance ARGS((void));
void s_cure ARGS((void));
void s_desecrate ARGS((void));
void s_disintegrate ARGS((void));
void s_dispel ARGS((void));
void s_disrupt ARGS((void));
void s_drain ARGS((void));
void s_enchant ARGS((void));
void s_fear ARGS((void));
void s_firebolt ARGS((void));
void s_haste ARGS((void));
void s_heal ARGS((void));
void s_hellfire ARGS((void));
void s_hero ARGS((void));
void s_identify ARGS((void));
void s_invisible ARGS((void));
void s_knowledge ARGS((void));
void s_lball ARGS((void));
void s_levitate ARGS((void));
void s_missile ARGS((void));
void s_mondet ARGS((void));
void s_objdet ARGS((void));
void s_polymorph ARGS((void));
void s_regenerate ARGS((void));
void s_restore ARGS((void));
void s_return ARGS((void));
void s_ritual ARGS((void));
void s_sanctify ARGS((void));
void s_sanctuary ARGS((void));
void s_shadowform ARGS((void));
void s_sleep ARGS((void));
void s_summon ARGS((void));
void s_teleport ARGS((void));
void s_truesight ARGS((void));
void s_warp ARGS((void));
void s_wish ARGS((void));
void showknownspells ARGS((int, int));

/* time.c functions */

void fix_phantom ARGS((struct monster *));
void time_clock ARGS((int));

/* trap.c functions */

void l_trap_abyss ARGS((void));
void l_trap_acid ARGS((void));
void l_trap_blade ARGS((void));
void l_trap_dart ARGS((void));
void l_trap_disintegrate ARGS((void));
void l_trap_door ARGS((void));
void l_trap_fire ARGS((void));
void l_trap_manadrain ARGS((void));
void l_trap_pit ARGS((void));
void l_trap_siren ARGS((void));
void l_trap_sleepgas ARGS((void));
void l_trap_snare ARGS((void));
void l_trap_teleport ARGS((void));

/* util.c functions */

long calc_points ARGS((void));
char *getarticle ARGS((char *));
char *month ARGS((void));
char *ordinal ARGS((int));
char *salloc ARGS((char *));
char inversedir ARGS((int));
void init_perms ARGS((void));
int confirmation ARGS((void));
char cryptkey ARGS((char*));
int day ARGS((void));
int distance ARGS((int,int,int,int));
int hitp ARGS((int,int));
int hour ARGS((void));
int inbounds ARGS((int,int));
int los_p ARGS((int,int,int,int));
int m_unblocked ARGS((struct monster *,int,int));
int nighttime ARGS((void));
int offscreen ARGS((int));
int ok_to_free ARGS((plv));
void free_level ARGS((plv));
void free_mons_and_objs ARGS((pml));
void free_objlist ARGS((pol));
void *checkmalloc ARGS((unsigned int));
int random_range ARGS((int));
int screenmod ARGS((int));
int showhour ARGS((void));
int showminute ARGS((void));
int strmem ARGS((char,char *));
int strprefix ARGS((char *,char *));
int unblocked ARGS((int,int));
int view_los_p ARGS((int,int,int,int));
int view_unblocked ARGS((int,int));
void calc_weight ARGS((void));
void change_to_game_perms ARGS((void));
void change_to_user_perms ARGS((void));
void do_los ARGS((Symbol,int *,int *,int,int));
void do_object_los ARGS((int,int *,int *,int,int));
void findspace ARGS((int *,int *,int));
void initdirs ARGS((void));
#ifdef NO_USLEEP
void usleep ARGS((int));
#endif

/* village.c functions */

void assign_village_function ARGS((int,int,int));
void load_village ARGS((int, int));
void make_food_bin ARGS((int,int));
void make_guard ARGS((int,int));
void make_horse ARGS((int,int));
void make_merchant ARGS((int,int));
void make_sheep ARGS((int,int));
void special_village_site ARGS((int,int,int));
