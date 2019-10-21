/* o.c functions */
void initrand(),inititem(),init_world();
int game_restore(),signalexit(),main();


/* oabyss functions */
void load_abyss();


/* oaux1 and oaux3 functions */
void pickup_at(),setspot(),foodcheck(),p_death(),hourly_check();
void calc_melee(),gain_experience(),p_damage(),minute_check();
void p_process(),p_teleport(),tacplayer(),roomcheck(),tunnelcheck();
void searchat(),showroom(),weapon_use(),damage_item(),tacmode();
void hand_wield(),optionset(),optionreset(),describe_player();
void lightroom(),surrender(),threaten(),darkenroom(),fight_monster();
void indoors_random_event(),outdoors_random_event(),tenminute_check();
char getlocation(),*itemid(),*mstatus_string(),*trapid(),*levelname();
int p_moveable(),cursed(),getdir(),optionp(),goberserk();
int player_on_sanctuary(),maneuvers();


/* oaux2 and oaux3 functions */
char *actionlocstr();
void break_weapon(),drop_weapon(),moon_check(),toggle_item_use();
void p_fumble(),p_win(),p_hit(),player_miss(),addring(),torch_check();
void top_level(),printactions(),movecursor(),p_drown(),enter_site();
void tacexecute(),weapon_use(),minute_status_check(),tenminute_status_check();
void expandsiteabbrev(),gain_level(),terrain_check();
void countrysearch(),default_maneuvers(),alert_guards();
int expval(),player_hit(),statmod(),item_value(),true_item_value();
int parsecitysite(),expandsite(),hostilemonstersnear(),magic_resist();
char *countryid(),*citysiteid();

/* ochar functions */
void initplayer(),initstats(),save_omegarc(),omegan_character_stats();
void user_character_stats();
int calcmana(),fixnpc(),competence_check();
FILE *omegarc_check();


/* ocity functions */
void load_city(),randommazesite(),assign_city_function(),make_justiciar();
void resurrect_guards(),mazesite(),make_minor_undead(),make_major_undead();
void repair_jail();

/* ocom1 functions */
void p_country_process(),p_process();

/* ocom3 and ocom2 functions */
void pickup(),drop(),magic(),disarm(),save(),restore();
void inventory(),opendoor(),closedoor(),fire(),fight(),city_move();
void eat(),version(),nap(),vault(),bash_item(),frobgamestatus();
void search(),peruse(),quaff(),talk(),bash_location(),wizard();
void give(),rest(),examine(),help();
void zapwand(),floor_inv(),callitem(), charid();
void moveplayer(),upstairs(),downstairs(),setoptions(),activate();
void pickpocket(),rename_player(),abortshadowform(),tunnel();
void change_environment();
void movepincountry(),tacoptions(),hunt(),dismount_steed();
int quit();

/* oeffect1 functions */
void bless(),identify(),bolt(),ball(),amnesia(),p_poison();
void heal(),mondet(),objdet(),enchant(),wish(),manastorm();
void acquire(),cleanse(),annihilate(),sleep_monster(),apport();
void sleep_player(),summon(),hide(),clairvoyance(),aggravate();
void fbolt(),lbolt(),lball(),fball(),nbolt(),learnspell(),level_drain();
void disintegrate(),disrupt(),snowball(),p_teleport(),acid_cloud();
void strategic_teleport();
int random_item(),itemlist(),monsterlist();


/* oeffect2 functions */
void knowledge(),flux(),displace(),breathe(),invisible(),warp(),alert();
void haste(),regenerate(),recover_stat(),augment(),truesight(),disease();
void i_chaos(),i_law(),sanctify(),hero(),level_return(),cure(),dispel();
void polymorph(),hellfire(),illuminate(),drain_life(),drain();
void inflict_fear(),levitate(),accuracy(),sanctuary(),shadowform();
void invulnerable(),deflection();

/* oetc functions */
void hint(),learnclericalspells(),add_new_npc();
char *slotstr(),*wordnum(),*nameprint();


/* ofile functions */
void commandlist(),helpfile(),showfile(),showscores(),showmotd();
void extendlog(),filescanstring(),checkhigh(),user_intro();
void theologyfile(),cityguidefile(),combat_help(),inv_help();
void abyss_file(),wishfile(),adeptfile(),copyfile(),displayfile();
int filecheck();
FILE *checkfopen();


/* ogen functions */
int loc_statusp(),mazeok();
void lset(),lreset(),load_arena(),make_country_screen(),room_corridor();
void clear_level(),create_level(),build_square_room(),build_diamond_room();
void connect_room(),corridor_crawl(),build_room(),sewer_corridor();
void clear_dungeon(),init_dungeon(),install_traps(),straggle_corridor();
void room_connect_corridor(),sewer_level(),room_level(),make_prime();
void change_level(),cavern_level(),makedoor(),make_archmage();
void maze_level(),maze_corridor();
void install_specials(),make_stairs();
void make_plains(),make_road(),make_forest(),make_jungle();
void make_river(),make_swamp(),make_mountains(),load_circle(),load_court();
void find_stairs();
char *roomname();
plv findlevel();

/* outil functions */
int inbounds(),random_range(),hitp(),sign(),max(),min(),distance();
int unblocked(),los_p(),screenmod(),offscreen(),calc_points();
int bitp(),nighttime(),hour(),showhour(),day(),showminute();
int view_unblocked(),m_unblocked(),strmem();
int view_los_p(),confirmation(),gamestatusp();
char inversedir(),*getarticle(),*ordinal(),*month(),*salloc();
void initdirs(),do_los(),do_object_los(),bitset(),bitreset();
void findspace(),setgamestatus(),resetgamestatus(),calc_weight();


/* otime functions */
void time_clock();


/* ospell functions */
void s_firebolt(),s_lball(),s_sleep(),s_objdet(),s_mondet(),s_identify();
void s_teleport(),s_disrupt(),s_disintegrate(),s_missile();
void s_heal(),s_dispel(),s_breathe(),s_invisible(),s_warp(),s_enchant();
void s_bless(),s_truesight(),s_hellfire(),s_knowledge(),s_hero();
void s_restore(),s_cure(),s_return(),s_desecrate(),s_summon();
void s_accuracy(),s_ritual(),s_apport(),s_shadowform(),s_alert();
void s_regenerate(),s_sanctify(),s_clairvoyance(),s_drain(),s_polymorph();
void s_invulnerable(),cast_spell(),s_fear(),s_wish();
void initspells(),expandspellabbrevs();
int getspell(),expandspell();
char *spellid();


/* osite functions */
void l_temple(),l_armorer(),l_club(),l_gym(),sign_print(),l_castle();
void l_healer(),statue_random(),l_statue_wake(),l_casino(),l_bank();
void l_sewer(),l_tavern(),l_alchemist(),l_vault(),l_oracle(),l_commandant();
void l_countryside(),l_dpw(),l_library(),l_pawn_shop(),l_diner();
void l_condo(),l_cemetary(),l_garden(),l_house(),l_hovel(),l_mansion();
void l_jail(),l_adept(),l_trifid(),l_warning(),l_brothel(),l_crap();
void l_safe(),l_cartographer(),l_charity(),l_mindstone();
void healforpay(),cureforpay(),gymtrain(),buyfromstock(),wake_statue();
void send_to_jail();


/* oscr functions */     
void redraw(),drawspot(),drawvision(),morewait(),drawmonsters(),locprint();
void mprint(),title(),sitclear(),initgraf(),xredraw(),erase_monster();
void plotchar(),show_screen(),blankoutspot(),show_license(),phaseprint();
void menuclear(),showcursor(),mnumprint(),menunumprint(),display_pack();
void clearmsg(),menuprint(),print3(),nprint3(),print1(),print2(),nprint1();
void clearmsg1(),clearmsg3(),commanderror(),nprint2(),deathprint();
void stdmorewait(),draw_explosion(),dataprint(),comwinprint(),timeprint();
void erase_level(),levelrefresh(),display_options(),display_option_slot();
void refreshmsg(),menuaddch(),drawscreen(),maddch(),putspot(),checkclear();
void display_quit(),showflags(),spreadroomdark(),display_win();
void drawplayer(),displaystats(),display_death(),plotmon(),display_bigwin();
void dobackspace(),drawandrefreshspot(),spreadroomlight(),bufferprint();
void drawomega(),screencheck(),blotspot(),plotspot(),dodrawspot();
void display_inventory_slot(),endgraf(),menuspellprint(),hide_line();
void display_possessions(),clear_if_necessary(),buffercycle(),lightspot();
int parsenum(),getnumber(),litroom(),move_slot(),stillonblock();
char getspot(),mgetc(),lgetc(),menugetc(),*msgscanstring();
char mcigetc(),ynq(),ynq1(),ynq2();


/* omove functions */
void p_movefunction();
void l_water(),l_hedge(),l_lava(),l_lift(),l_magic_pool(),l_whirlwind();
void l_abyss(),l_no_op(),l_tactical_exit(),l_rubble(),l_raise_portcullis();
void l_portcullis(),l_portcullis_trap(),l_arena_exit(),l_house_exit();
void l_drop_every_portcullis(),l_earth_station(),l_water_station();
void l_fire_station(),l_air_station(),l_void(),stationcheck(),l_whirlwind();
void l_voice1(),l_voice2(),l_voice3(),l_whirlwind(),l_void_station();
void l_chaos(),l_tome1(),l_tome2(),l_enter_circle(),l_circle_library();
void l_temple_warning(),l_throne(),l_enter_court(),l_escalator(),l_fire();
void l_voidstone(),l_chaostone(),l_lawstone(),l_balancestone();
void l_sacrificestone();

/* otrap functions */
void l_trap_dart(),l_trap_pit(),l_trap_door(),l_trap_snare(),l_trap_siren();
void l_trap_blade(),l_trap_fire(),l_trap_disintegrate(),l_trap_teleport();
void l_trap_acid(),l_trap_manadrain(),l_trap_sleepgas(),l_trap_abyss();



/* ohouse functions */
void load_house(),make_dog(),make_automaton(),make_house_npc();
void make_mansion_npc();

/* omonf functions */
void m_normal_move(),m_no_op();
void m_smart_move(),m_simple_move(),m_spirit_move(),m_vanish();
void m_firebolt(),m_talk_guard(),m_random_move(),m_sp_morgon();
void m_flutter_move(),m_nbolt(),m_sp_surprise(),m_talk_druid();
void m_talk_stupid(),m_talk_hungry(),m_talk_silent(),m_talk_greedy();
void m_talk_mp(),m_follow_move(),m_sp_mp(),m_talk_evil(),m_talk_prime();
void m_lball(),m_fireball(),m_talk_titter(),m_talk_thief();
void m_sp_ng(),m_sp_poison_cloud(),m_huge_sounds(),m_talk_robot();
void m_blind_strike(),m_talk_im(),m_confused_move(),m_talk_beg();
void m_thief_f(),m_talk_man(),m_summon(),m_animal_talk(),m_move_animal();
void m_teleport(),m_illusion(),m_aggravate(),m_talk_hint();
void m_sp_explode(),m_sp_acid_cloud(),m_sp_bogthing(),m_talk_animal();
void m_sp_whirl(),m_sp_escape(),m_sp_ghost(),m_sp_demonlover();
void m_talk_mimsy(),m_talk_slithy(),m_talk_burble(),m_sp_spell();
void m_scaredy_move(),m_talk_gf(),m_talk_ef(),m_sp_demon(),m_sp_av();
void m_sp_seductor(),m_talk_demonlover(),m_talk_seductor(),m_sp_lw();
void m_sp_eater(),m_sp_dragonlord(),m_talk_ninja(),m_talk_assassin();
void m_sp_blackout(),m_sp_leash(),m_sp_were(),m_sp_angel(),m_sp_servant();
void m_sp_swarm(),m_talk_servant(),m_talk_horse(),m_snowball(),m_sp_prime();
void m_sp_mb(),m_sp_mirror(),m_sp_raise(),m_talk_scream(),m_strike_sonic();
void m_talk_parrot(),m_talk_hyena(),m_talk_archmage(),m_talk_merchant();
void m_sp_merchant(),m_sp_court(),m_sp_lair(),m_sp_whistleblower();

char *mantype();

/* omon functions */
int m_statusp(),m_immunityp(),tacticalp();
void m_pulse(),m_status_set(),m_damage(),monster_action();
void m_death(),m_hit(),m_miss(),m_pickup(),m_dropstuff();
void m_status_reset(),monster_strike(),tacmonster(),free_mlist();
void monster_talk(),monster_melee(),monster_move(),monster_special();
void movemonster(),make_log_npc(),m_abyss(),transcribe_monster_actions();
void m_movefunction(),trasnscribe_monster_actions(),determine_npc_behavior();
void m_water(),m_lava(),m_altar(),m_whirlwind(),m_fire(),make_hiscore_npc();
void m_trap_dart(),m_trap_pit(),m_trap_door(),m_trap_snare();
void m_trap_blade(),m_trap_fire(),m_trap_disintegrate(),m_trap_teleport();
void m_trap_manadrain(),m_trap_acid(),m_trap_sleepgas(),m_trap_abyss();
void strengthen_death(),fix_phantom(),destroy_order();
char random_loc();
int corpsevalue(),monster_hit();


/* olev functions */
void populate_level(),stock_level(),wandercheck(),make_site_monster();
void free_dungeon(),tactical_map(),make_country_monsters();
void make_site_treasure(),make_specific_treasure();
int difficulty();
pmt m_create(),make_creature();

/* osave functions */
int save_game(),restore_game(), signalsave();
void save_player(),save_player_items(),save_player_spells();
void save_player_item_knowledge(),save_monsters(),save_country();
void save_level(),save_item(),save_itemlist();
void restore_player(),restore_player_items(),restore_player_spells();
void restore_player_item_knowledge(),restore_monsters();
void restore_level(),restore_country();
pob restore_item();
pol restore_itemlist();




/* oitemf functions (1 to 3)*/
void weapon_bare_hands(),weapon_normal_hit(),weapon_demonblade();
void weapon_lightsabre(),weapon_mace_disrupt(),weapon_tangle();
void weapon_vorpal(),weapon_desecrate(),weapon_firestar(),weapon_defend();
void weapon_victrix(),weapon_bolt(),weapon_arrow(),weapon_scythe();
void weapon_acidwhip();
void i_no_op(),i_nothing(),i_stargem(),i_sceptre(),i_deflect();
void i_spells(),i_bless(),i_enchant(),i_acquire(),i_jane_t();
void i_flux(),i_breathing(),i_invisible(),i_perm_invisible();
void i_teleport(),i_wish(),i_clairvoyance(),i_displace(),i_id(),i_augment();
void i_heal(),i_objdet(),i_mondet(),i_sleep_self(),i_neutralize_poison();
void i_speed(),i_restore(),i_azoth(),i_regenerate(),i_demonblade();
void i_firebolt(),i_lbolt(),i_missile(),i_fireball(),i_lball(),i_snowball();
void i_sleep_other(),i_summon(),i_hide(),i_disrupt(),i_disintegrate();
void i_perm_vision(),i_perm_burden(),i_perm_fire_resist(),i_charge();
void i_perm_poison_resist(),i_perm_strength(),i_perm_gaze_immune();
void i_perm_energy_resist(), i_perm_speed(),i_perm_breathing();
void i_perm_displace(),i_perm_negimmune(),i_mace_disrupt(),i_warp();
void i_food(),i_poison_food(),i_stim(),i_immune(),i_lembas(),i_pow();
void i_orbfire(),i_orbwater(),i_orbair(),i_orbearth(),i_orbdead();
void i_orbmastery(),i_lightsabre(),i_alert(),i_knowledge(),i_pick();
void i_perm_knowledge(),i_fear_resist(),i_perm_fear_resist(),i_key();
void i_defend(),i_victrix(),i_apport(),i_corpse(),i_accuracy();
void i_perm_accuracy(),i_hero(),i_perm_hero(),i_levitate();
void i_perm_regenerate(),i_perm_levitate(),i_perm_protection();
void i_perm_agility(),i_truesight(),i_perm_truesight(),i_cure();
void i_dispel(),i_pepper_food(),i_enchantment(),polymorph();
void i_crystal(),i_kolwynia(),i_death(),i_life(),i_helm(),i_antioch();
void i_illuminate(),i_perm_illuminate(),i_desecrate();
void i_trap(),i_juggernaut(),i_raise_portcullis(),i_planes();
void i_fear(),i_polymorph(),i_symbol(),i_perm_deflect();
void i_normal_armor(),i_normal_weapon(),i_normal_shield();

/* oitem functions */
void item_use();
pob create_object();
void make_shield(),make_armor(),make_weapon();
void make_scroll(),make_potion(),make_food();
void make_thing(),make_ring(),make_artifact(),make_corpse();
void make_boots(),make_cloak(),make_stick(),make_cash();
int itemplus(),itemcharge(),itemblessing(),twohandedp();
char *scrollname(),*potionname(),*stickname(),*ringname(),*cloakname();
char *bootname(),*grotname();


/* oinv functions */
void pickup_at(),drop_at(),givemonster(),conform_lost_object();
void conform_unused_object(),gain_item(),drop_money(),add_to_pack();
void setplustr(),setchargestr(), setnumstr(),p_drop_at(),give_money();
void conform_lost_objects(),itemlist_inventory(),take_from_pack();
void merge_item(),take_from_slot(),inventory_control(),item_inventory();
void drop_from_slot(),put_to_pack(),switch_to_slot(),top_inventory_control();
void lose_all_items(),dispose_lost_objects(),pack_extra_items(),fixpack();
int get_inventory_slot(),getitem();
int objequal(),slottable(),item_useable(),find_and_remove_item();
int getitem(),badobject(),cursed(),get_money(),get_item_number(),find_item();
pob detach_money(),split_item();
char *itemid(),*cashstr();

/* oguild functions */
void l_druid(),l_thieves_guild(),l_college(),l_merc_guild(),l_sorcerors();
void l_altar(),l_arena(),l_order();

/* opriest functions */
void make_hp(),hp_req_test(),hp_req_print(),increase_priest_rank();
void answer_prayer();
int check_sacrilege();

/* ocountry functions */
void load_country(),load_dlair(),load_speak(),load_misle(),load_temple();
void make_high_priest(),random_temple_site();

/* ovillage functions */
void load_village(),make_horse(),make_sheep();
void make_guard(),make_sheep(),make_merchant(),make_food_bin();
void assign_village_function(),special_village_site();


