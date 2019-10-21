/* omega copyright (c) 1987,1988 by Laurence Raphael Brothers */
/* This file is the header file for all omega modules */
/* odefs.h */

/* omega will NOT function unless the implementor sets the appropriate
definitions in the following section. */

/*--------------------------USER DEFINITIONS--------------------------*/

/* The average system V user should probably uncomment the following
two definitions. */

/* Implementor should uncomment the following if his system uses
string.h instead of strings.h (try man strings) */

/* #define STRING */

/* Implementor should uncomment the following if random and srandom 
   are not available  (try man random) */

/* #define NORANDOM */

/* Implementor should uncomment the following if omega appears to
redraw the screen excessively. */

/* #define EXCESSIVE_REDRAW */

/* The following definition is recommended. Remove it only if you have
huge amounts of disk space and are annoyed at waiting a few more seconds
on save and restore. */

#define COMPRESS_SAVE_FILES

/* Implementor should set the following three definitions: */

/* OMEGALIB is where all the data files reside. 
   Note the final / is necessary.
   This might usually be "/usr/games/lib/omegalib/" */

#define OMEGALIB "/grad/u4/brothers/omega/lib/"

/* set WIZARD to implementor's username */

#define WIZARD "brothers"

/* If CATCH_SIGNALS is set to 1, will not dump core, nicer for players. */
/* dbx still intercepts the signals first, so it's ok for debugging */

#define CATCH_SIGNALS 1


/*---------------------------SYSTEM DEFINITIONS---------------------------*/

/* Don't change anything from here on (unless you know what you're doing) */
#define VERSION 71
#define VERSIONSTRING "omega version 0.71 (beta)"

#define VACANT 0
#define ABORT -1
#define CASHVALUE -2

/* moderately arbitrary but probably cannot be easily changed */
#define MAXWIDTH 64
#define MAXLENGTH 64
#define SMALLSCREENLENGTH 16

/* number of slots in inventory. Cannot be changed without work */
#define MAXITEMS 16

/* number of slots in pack. Should be <= 26. */
#define MAXPACK 26

/* number of items in pawn shop. Should be <= 26 */
#define PAWNITEMS 10

/* Verbosity levels */
#define TERSE 0
#define MEDIUM 1
#define VERBOSE 2 

/* Arbitrary. Max of the levels in the dungeons */
#define MAXLEVELS 21

/* levels in each dungeon */
#define ASTRALLEVELS 5
#define SEWERLEVELS 18
#define CASTLELEVELS 16
#define CAVELEVELS 10
#define VOLCANOLEVELS 20

/* Overall Game Progress Vector Bits */
/* Long had BETTER have at least 32 bits.... */
#define SPOKE_TO_DRUID 1L
#define COMPLETED_CAVES 2L
#define COMPLETED_SEWERS 4L
#define COMPLETED_CASTLE 8L
#define COMPLETED_ASTRAL 16L
#define COMPLETED_VOLCANO 32L
#define KILLED_DRAGONLORD 64L
#define KILLED_EATER 128L
#define KILLED_LAWBRINGER 256L
#define COMPLETED_CHALLENGE 512L
#define SOLD_CONDO 1024L
#define FAST_MOVE 2048L
#define SKIP_PLAYER 4096L
#define SKIP_MONSTERS 8192L
#define MOUNTED 16384L
#define SUPPRESS_PRINTING 32768L
#define LOST 65536L
#define ARENA_MODE 131072L
#define CHEATED 262144L
#define BANK_BROKEN 524288L
#define CLUB_MEMBER 1048576L
#define PREPARED_VOID 2097152L
#define DESTROYED_ORDER 4194304L
#define GAVE_STARGEM 8388608L
#define ATTACKED_ORACLE 16777216L
#define UNDEAD_GUARDS 33554432L

/* general environment types */
#define E_NEVER_NEVER_LAND 0
#define E_COUNTRYSIDE 1
#define E_CITY 2
#define E_VILLAGE 3
#define E_TACTICAL_MAP 4
#define E_SEWERS 5
#define E_CASTLE 6
#define E_CAVES 7
#define E_VOLCANO 8
#define E_ASTRAL 9
#define E_ARENA 10
#define E_HOVEL 11
#define E_MANSION 12
#define E_HOUSE 13
#define E_DLAIR 14
#define E_ABYSS 15
#define E_STARPEAK 16
#define E_MAGIC_ISLE 17
#define E_TEMPLE 18
#define E_CIRCLE 19
#define E_COURT 20

/* player game status */
#define DEAD 1
#define QUIT 2
#define WIN 3
#define BIGWIN 4

/* kind of arbitrary */
#define MAXROOMS 48
#define MAXCONNECTIONS 4 

/* some random characters */
#define ESCAPE 27
#define RETURN 13
#define LINEFEED 10
#define BACKSPACE 8
#define DELETE 127


/* tac mode action definitions */
/* have to remember to find where these are used, mostly unused, now! */
#define DISENGAGE 10
#define BLOCK 20
#define CUT 30
#define THRUST 40
#define MAGIC 50
#define LUNGE 60
#define RIPOSTE 70
#define WIELD 80
#define PICK_UP 90

/* as in attack low, block high, etc. */
/* These values may be added to the ones above to get things like
   block high, cut low, etc. CLEVER is only used by some monsters
   to cheat with.... */
#define LOW 1
#define CENTER 2
#define HIGH 3
#define CLEVER 4


/* weapon types */
#define CUTTING 1
#define THRUSTING 2
#define STRIKING 3
#define MISSILE 4

/* random aux constants */
/* aux field for private residences in city */
#define BURGLED 2
#define LOCKED 3
#define UNLOCKED 0

/* cannot use M command on site with this aux value */
#define NOCITYMOVE 666

/* bow and crossbow object aux fields */
#define LOADED 1
#define UNLOADED 0

/* alignment used randomly throughout*/
#define LAWFUL 1
#define CHAOTIC 2
#define NEUTRAL 3

/* spells */
#define NUMSPELLS 41

#define S_MON_DET 0
#define S_OBJ_DET 1
#define S_MISSILE 2
#define S_FIREBOLT 3
#define S_TELEPORT 4
#define S_LBALL 5
#define S_SLEEP 6
#define S_DISRUPT 7
#define S_DISINTEGRATE 8
#define S_POLYMORPH 9
#define S_HEAL 10
#define S_DISPEL 11
#define S_IDENTIFY 12
#define S_BREATHE 13
#define S_INVISIBLE 14
#define S_WARP 15
#define S_ENCHANT 16
#define S_BLESS 17
#define S_RESTORE 18
#define S_CURE 19
#define S_TRUESIGHT 20
#define S_HELLFIRE 21
#define S_KNOWLEDGE 22
#define S_HERO 23
#define S_RETURN 24
#define S_DESECRATE 25
#define S_HASTE 26
#define S_SUMMON 27
#define S_SANCTUARY 28
#define S_ACCURACY 29
#define S_RITUAL 30
#define S_FEAR 31
#define S_APPORT 32
#define S_SHADOWFORM 33
#define S_ALERT 34
#define S_REGENERATE 35
#define S_SANCTIFY 36
#define S_CLAIRVOYANCE 37
#define S_DRAIN 38
#define S_LEVITATE 39
#define S_WISH 40

/* ranks in guilds, etc */
#define NUMRANKS 9

#define LEGION 0
#define ARENA 1
#define COLLEGE 2
#define THIEVES 3
#define ORDER 4
#define CIRCLE 5
#define NOBILITY 6
#define PRIESTHOOD 7
#define ADEPT 8

#define LEGIONAIRE 1
#define CENTURION 2
#define FORCE_LEADER 3
#define COLONEL 4
#define COMMANDANT 5

#define TRAINEE 1
#define BESTIARIUS 2
#define RETIARIUS 3
#define GLADIATOR 4
#define CHAMPION 5

#define NOVICE 1
#define STUDENT 2
#define PRECEPTOR 3
#define MAGE 4
#define ARCHMAGE 5

#define TMEMBER 1
#define ATHIEF 2
#define THIEF 3
#define TMASTER 4
#define SHADOWLORD 5

#define GALLANT 1
#define GUARDIAN 2
#define CHEVALIER 3
#define PALADIN 4
#define JUSTICIAR 5

#define INITIATE 1
#define ENCHANTER 2
#define SORCEROR 3
#define HIGHSORCEROR 4
#define PRIME 5

#define COMMONER 1
#define ESQUIRE 2
#define KNIGHT 3
#define LORD 4
#define DUKE 5

#define LAY 1
#define ACOLYTE 2
#define PRIEST 3
#define SPRIEST 4
#define HIGHPRIEST 5

/* different priesthoods */
#define ODIN 1
#define SET 2
#define ATHENA 3
#define HECATE 4
#define DRUID 5
#define DESTINY 6



/* MONSTER STATUS/ABILITY BITS */
#define AWAKE 1
#define MOBILE 2
#define HOSTILE 4
#define WANDERING 16
#define HUNGRY 32
#define GREEDY 64
#define NEEDY  128
#define ONLYSWIM 256
#define FLYING 512
#define INTANGIBLE 1024
#define M_INVISIBLE 2048
#define SWIMMING 4096
#define POISONOUS 8192
#define EDIBLE 16384


/* PLAYER STATUS INDICES */
#define NUMSTATI 25

#define ACCURACY 0
#define BLINDED 1
#define SLOWED 2
#define DISPLACED 3
#define SLEPT 4
#define DISEASED 5
#define POISONED 6
#define HASTED 7
#define BREATHING 8
#define INVISIBLE 9
#define REGENERATING 10
#define VULNERABLE 11
#define BERSERK 12
#define IMMOBILE 13
#define ALERT 14
#define AFRAID 15
#define HERO 16
#define LEVITATING 17
#define ACCURATE 18
#define TRUESIGHT 19
#define SHADOWFORM 20
#define ILLUMINATION 21
#define DEFLECTION 22
#define PROTECTION 23
/* PROTECTION is deviant -- indicates protective value, not duration */
#define RETURNING 24
/* RETURNING is somewhat deviant--how many turns 'til RETURN spell goes off */

/* player immunity indices */
/* also monster immunity bits (2^n) */
/* also damage types */
#define NUMIMMUNITIES 14

#define UNSTOPPABLE 0 
#define NORMAL_DAMAGE 1
#define FLAME 2
#define COLD 3
#define ELECTRICITY 4
#define POISON 5
#define ACID 6
#define FEAR 7
#define SLEEP 8
#define NEGENERGY 9
#define OTHER_MAGIC 10
#define THEFT 11
#define GAZE 12
#define INFECTION 13
#define EVERYTHING -1


/* location lstatus bits */
#define SEEN 1
#define LIT 2
#define SECRET 4
#define STOPS 8


/* room string id */
/* for use in roomname() */
#define NUMROOMNAMES 30

/* normal room name indices start after the RS_ constants */
#define ROOMBASE 39
#define RS_COURT 38
#define RS_ZORCH 37
#define RS_CIRCLE 36
#define RS_MAGIC_ISLE 35
#define RS_STARPEAK 34
#define RS_VOLCANO 33
#define RS_HIGHASTRAL 32
#define RS_FIREPLANE 31
#define RS_AIRPLANE 30
#define RS_WATERPLANE 29
#define RS_EARTHPLANE 28
#define RS_LOWERASTRAL 27
#define RS_CLOSET 26
#define RS_SECRETPASSAGE 25
#define RS_DININGROOM 24
#define RS_BATHROOM 23
#define RS_BEDROOM 22
#define RS_KITCHEN 21
#define RS_DROWNED_SEWER 20
#define RS_DRAINED_SEWER 19
#define RS_SEWER_DUCT 18
#define RS_ARENA 17
#define RS_COUNTRYSIDE 16
#define RS_DRUID 15
#define RS_HECATE 14
#define RS_ATHENA 13
#define RS_SET 12
#define RS_ODIN 11
#define RS_DESTINY 10
#define RS_ADEPT 9
#define RS_WYRM 8
#define RS_OCEAN 7 
#define RS_PONDS 6
#define RS_DRAGONLORD 5
#define RS_GOBLINKING 4
#define RS_CAVERN 3
#define RS_CORRIDOR 2
#define RS_WALLSPACE 1

/* objects, locations, and terrain; characters to draw */
#define SPACE ' '
#define WALL '#'
#define PORTCULLIS '7'
#define OPEN_DOOR '|'
#define CLOSED_DOOR '-'
#define WHIRLWIND '6'
#define ABYSS '0'
#define LAVA '`'
#define HEDGE '\"'
#define WATER '~' 
#define FIRE ';'
#define TRAP '^'
#define LIFT '_'
#define UP '<'
#define DOWN '>'
#define FLOOR '.'
#define PLAYER '@'
#define CORPSE '+'
#define STATUE '1'
#define RUBBLE '4'
#define ALTAR '8'
#define CASH '$'      /* various kinds of money */
#define PILE '*'      /* several objects in one place */
#define FOOD '%'
#define WEAPON ')'
#define MISSILEWEAPON '('
#define SCROLL '?'
#define POTION '!'
#define ARMOR ']'
#define SHIELD '['
#define CLOAK '}'
#define BOOTS '{'
#define STICK '/'

#define RING '='
#define THING '\\'
#define ARTIFACT '&'

/* TERRAIN TYPES */
#define PLAINS '-'
#define TUNDRA '_'
#define ROAD '.'
#define MOUNTAINS '^'
#define PASS 'v'
#define RIVER '~'
#define CITY 'O'
#define VILLAGE 'o'
#define FOREST '('
#define JUNGLE ')'
#define SWAMP '='
#define VOLCANO '!'
#define CASTLE '%'
#define TEMPLE 'X'
#define CAVES '*'
#define DESERT '\"'
#define CHAOS_SEA '+'
#define STARPEAK '|'
#define DRAGONLAIR '$'
#define MAGIC_ISLE '&'

#define CHAIR '5'
#define SAFE '3'
#define FURNITURE '2'
#define BED '9'

/* wow, all characters used! */


/* total number of player options */
#define NUMOPTIONS 9

/* number of options with TRUE/FALSE values */
#define NUMTFOPTIONS 7

/* The slot number of the two options not in Player.options */
#define VERBOSITY_LEVEL 8
#define SEARCH_DURATION 9

/* Player.options bits */
#define BELLICOSE 1 
#define JUMPMOVE 2
#define RUNSTOP 4
#define PICKUP 8
#define CONFIRM 16
#define TOPINV 32
#define PACKADD 64

/* This has to be changed whenever an item is added */
#define NUMSCROLLS 24
#define NUMPOTIONS 18
#define NUMFOODS 16
#define NUMTHINGS 26
#define NUMWEAPONS 41
#define NUMARMOR 17
#define NUMSHIELDS 8
#define NUMCLOAKS 7
#define NUMBOOTS 7
#define NUMRINGS 10
#define NUMSTICKS 17
#define NUMARTIFACTS 24

/* running sum of itemtypes, for indexing into Objects array */
#define THINGID 0 
#define FOODID NUMTHINGS
#define SCROLLID (FOODID + NUMFOODS)
#define POTIONID (SCROLLID + NUMSCROLLS)
#define WEAPONID (POTIONID + NUMPOTIONS)
#define ARMORID (WEAPONID + NUMWEAPONS)
#define SHIELDID (ARMORID + NUMARMOR)
#define CLOAKID (SHIELDID + NUMSHIELDS)
#define BOOTID (CLOAKID + NUMCLOAKS)
#define RINGID (BOOTID + NUMBOOTS)
#define STICKID (RINGID + NUMRINGS)
#define ARTIFACTID (STICKID + NUMSTICKS)
#define CASHID (ARTIFACTID+NUMARTIFACTS)
/* Corpse's aux field is monster id */
#define CORPSEID (CASHID+1)

#define TOTALITEMS (CORPSEID+1) 

/* describing unique items and monsters */
#define COMMON 0
#define UNIQUE_UNMADE 1
#define UNIQUE_MADE 2

/* general item function id's */
#define I_NO_OP 0
#define I_NOTHING 1

  /* note some of these functions are for other types of items too */

    /* scroll functions */
#define I_BLESS 101
#define I_ACQUIRE 102
#define I_ENCHANT 103
#define I_TELEPORT 104
#define I_WISH 105
#define I_CLAIRVOYANCE 106
#define I_DISPLACE 107
#define I_ID 108
#define I_FIREFLASH 109
#define I_SPELLS 110
#define I_JANE_T 111
#define I_ALERT 112
#define I_FLUX 113
#define I_CHARGE 114
#define I_WARP 115
#define I_KNOWLEDGE 116
#define I_LAW 117
#define I_HINT 118
#define I_HERO 119
#define I_TRUESIGHT 120
#define I_ILLUMINATE 121
#define I_DEFLECT 122

    /* potion functions */
#define I_HEAL 201
#define I_OBJDET 202
#define I_MONDET 203
#define I_SLEEP_SELF 204
#define I_RESTORE 205
#define I_NEUTRALIZE_POISON 206
#define I_SPEED 207
#define I_AZOTH 208
#define I_REGENERATE 210
#define I_INVISIBLE 211
#define I_BREATHING 212
#define I_FEAR_RESIST 213
#define I_AUGMENT 214
#define I_CHAOS 215
#define I_ACCURACY 216
#define I_LEVITATION 217
#define I_CURE 218

    /* stick functions */
#define I_FIREBOLT 301
#define I_LBOLT 302
#define I_MISSILE 303
#define I_SLEEP_OTHER 304
#define I_FIREBALL 305
#define I_LBALL 306
#define I_SUMMON 307
#define I_HIDE 308
#define I_DISRUPT 309
#define I_DISINTEGRATE 310
#define I_SNOWBALL 311
#define I_APPORT 312
#define I_DISPEL 313
#define I_POLYMORPH 314
#define I_FEAR 315

    /* food functions */
#define I_FOOD 401
#define I_LEMBAS 402
#define I_STIM 403
#define I_POW 404
#define I_IMMUNE 405
#define I_POISON_FOOD 406
#define I_CORPSE 407
#define I_PEPPER_FOOD 408
#define I_CANNIBAL 409
#define I_INEDIBLE 410

    /* boots functions */
#define I_PERM_SPEED 501
#define I_PERM_HERO 502
#define I_PERM_LEVITATE 503
#define I_PERM_AGILITY 504
#define I_BOOTS_JUMPING 505
#define I_BOOTS_7LEAGUE 506

    /* cloak functions */
#define I_PERM_DISPLACE 601
#define I_PERM_NEGIMMUNE 602
#define I_PERM_INVISIBLE 603
#define I_PERM_ACCURACY 604
#define I_PERM_PROTECTION 605
#define I_PERM_TRUESIGHT 606

    /* ring functions */
#define I_PERM_VISION 701
#define I_PERM_BURDEN 702
#define I_PERM_STRENGTH 703
#define I_PERM_GAZE_IMMUNE 704
#define I_PERM_FIRE_RESIST 705
#define I_PERM_POISON_RESIST 706
#define I_PERM_REGENERATE 707
#define I_PERM_KNOWLEDGE 708

    /* armor functions */
#define I_PERM_ENERGY_RESIST 801
#define I_PERM_BREATHING 802
#define I_PERM_FEAR_RESIST 803
#define I_NORMAL_ARMOR 804

/* artifact functions */
#define I_ORBFIRE 901
#define I_ORBWATER 902
#define I_ORBEARTH 903
#define I_ORBAIR 904
#define I_ORBMASTERY 905
#define I_ORBDEAD 906
#define I_CRYSTAL 907
#define I_ANTIOCH 908
#define I_KOLWYNIA 909
#define I_DEATH 910
#define I_ENCHANTMENT 911
#define I_HELM 912
#define I_LIFE 913
#define I_JUGGERNAUT 914
#define I_SYMBOL 915
#define I_STARGEM 916
#define I_SCEPTRE 917
#define I_PLANES 918

/* weapons functions */
#define I_NORMAL_WEAPON 1001
#define I_LIGHTSABRE 1002
#define I_DEMONBLADE 1003
#define I_MACE_DISRUPT 1004
#define I_TANGLE 1005
#define I_ARROW 1006
#define I_BOLT 1007
#define I_VORPAL 1008
#define I_DESECRATE 1009
#define I_FIRESTAR 1010
#define I_DEFEND 1011
#define I_VICTRIX 1012
#define I_EMPIRE 1013
#define I_SCYTHE 1014
#define I_ACIDWHIP 1015

/* thing functions */
#define I_PICK 1101
#define I_KEY 1102
#define I_SHOVEL 1103 /* unused */
#define I_EXCAVATOR 1104 /* unused */
#define I_PERM_ILLUMINATE 1105
#define I_TRAP 1106
#define I_RAISE_PORTCULLIS 1107

/* shield functions */
#define I_PERM_DEFLECT 1201
#define I_NORMAL_SHIELD 1202

/* monster function ids */
/* Its conceivable for a function of one type to be called when another
would usually occur. A monster's special function may be an extra move,
for example. */

#define M_NO_OP -1

/* talk functions */
#define M_TALK_STUPID 101
#define M_TALK_SILENT 102
#define M_TALK_HUNGRY 103
#define M_TALK_GREEDY 104
#define M_TALK_TITTER 105
#define M_TALK_MAN 106
#define M_TALK_ROBOT 107
#define M_TALK_EVIL 108
#define M_TALK_BURBLE 109
#define M_TALK_SLITHY 110
#define M_TALK_MIMSY 111
#define M_TALK_SEDUCTOR 112
#define M_TALK_MP 113
#define M_TALK_IM 114
#define M_TALK_GUARD 115
#define M_TALK_GHOST 116
#define M_TALK_HINT 117
#define M_TALK_BEG 118
#define M_TALK_EF 119
#define M_TALK_GF 120
#define M_TALK_MORGON 121
#define M_TALK_LB 122
#define M_TALK_DEMONLOVER 123
#define M_TALK_ASSASSIN 124
#define M_TALK_NINJA 125
#define M_TALK_THIEF 126
#define M_TALK_MERCHANT 127
#define M_TALK_HORSE 128
#define M_TALK_PARROT 129
#define M_TALK_ANIMAL 130
#define M_TALK_HYENA 131
#define M_TALK_SERVANT 132
#define M_TALK_SCREAM 133
#define M_TALK_DRUID 134
#define M_TALK_ARCHMAGE 135
#define M_TALK_PRIME 136

/* ability functions */
#define M_SP_SURPRISE 201
#define M_SP_MP 202
#define M_SP_THIEF 203
#define M_SP_AGGRAVATE 204
#define M_SP_POISON_CLOUD 205
#define M_SP_HUGE 206
#define M_SP_SUMMON 207
#define M_SP_ILLUSION 208
#define M_SP_FLUTTER 209
#define M_SP_ESCAPE 210
#define M_SP_SPELL 211
#define M_SP_EXPLODE 212
#define M_SP_DEMON 213
#define M_SP_ACID_CLOUD 214
#define M_SP_WHIRL 215
#define M_SP_GHOST 216
#define M_SP_WHISTLEBLOWER 217
#define M_SP_EATER 218
#define M_SP_LAWBRINGER 219
#define M_SP_DRAGONLORD 220
#define M_SP_DE 221
#define M_SP_DEMONLOVER 222
#define M_SP_SEDUCTOR 223
#define M_SP_MASTER 224
#define M_SP_WYRM 225
#define M_SP_BLACKOUT 226
#define M_SP_BOG 227
#define M_SP_MERCHANT 228
#define M_SP_WERE 229
#define M_SP_LEASH 230
#define M_SP_SERVANT 231
#define M_SP_AV 232
#define M_SP_LW 233
#define M_SP_SWARM 234
#define M_SP_ANGEL 235
#define M_SP_MB 236
#define M_SP_MIRROR 237
#define M_SP_RAISE 238
#define M_SP_DEATH 239
#define M_SP_COURT 240
#define M_SP_LAIR 241
#define M_SP_PRIME 242

/* rangestrike functions */
#define M_STRIKE_MISSILE 301
#define M_STRIKE_FBOLT 302
#define M_STRIKE_LBALL 303
#define M_STRIKE_FBALL 304
#define M_STRIKE_BLIND 305
#define M_STRIKE_SNOWBALL 306
#define M_STRIKE_MASTER 307
#define M_STRIKE_SONIC 308

/* combat functions */
#define M_MELEE_NORMAL 401
#define M_MELEE_FIRE 402
#define M_MELEE_DRAGON 403
#define M_MELEE_MP 404
#define M_MELEE_ELEC 405
#define M_MELEE_POISON 406
#define M_MELEE_NG 407
#define M_MELEE_SUCCUBUS 408
#define M_MELEE_SPIRIT 409
#define M_MELEE_DISEASE 410
#define M_MELEE_SLEEP 411
#define M_MELEE_COLD 412
#define M_MELEE_MASTER 413
#define M_MELEE_GRAPPLE 414
#define M_MELEE_DEATH 415

/* movement functions */
#define M_MOVE_NORMAL 501
#define M_MOVE_FLUTTER 502
#define M_MOVE_TELEPORT 503
#define M_MOVE_FOLLOW 504
#define M_MOVE_RANDOM 505
#define M_MOVE_SMART 506
#define M_MOVE_SPIRIT 507
#define M_MOVE_SCAREDY 508
#define M_MOVE_CONFUSED 509
#define M_MOVE_ANIMAL 510

/* MLx -> index to Monsters starting for level x */
/* MLx -> number of monsters of level x or less */
/* NML_x -> number of monsters of level x */
/* NML-X must be changed whenever a monster is added */
#define ML0 0
#define NML_0 9
#define ML1 (ML0 + NML_0)
#define NML_1 22
#define ML2 (ML1 + NML_1)
#define NML_2 14
#define ML3 (ML2 + NML_2)
#define NML_3 15
#define ML4 (ML3 + NML_3)
#define NML_4 18
#define ML5 (ML4 + NML_4)
#define NML_5 14
#define ML6 (ML5 + NML_5)
#define NML_6 13
#define ML7 (ML6 + NML_6)
#define NML_7 15
#define ML8 (ML7 + NML_7)
#define NML_8 12
#define ML9 (ML8 + NML_8)
#define NML_9 8
#define ML10 (ML9 + NML_9)
#define NML_10 10

#define NUMMONSTERS (ML10 + NML_10)

/* Some monster ID's : (Those that are explicitly named in code) */
/* Actually, there are still many magic constants floating around. */
/* Eventually I'll get around to making each monster's id a constant.... */
#define RANDOM -1
#define NPC (ML0+4)
#define HISCORE_NPC (ML0+8)
#define BUNNY (ML0+5)
#define BLACKSNAKE (ML1+14)
#define HAWK (ML1+13)
#define IMPALA (ML1+19)
#define WOLF (ML2+10)
#define LION (ML3+8)
#define BRIGAND (ML3+9)
#define QUAIL (ML1+11)
#define BADGER (ML1+12)
#define DEER (ML1+14)
#define BEAR (ML3+10)
#define ANTEATER (ML1+16)
#define PARROT (ML1+20)
#define MAMBA (ML3+11)
#define ANT (ML2+11)
#define HYENA (ML1+21)
#define ELEPHANT (ML2+12)
#define TROUT (ML1+18)
#define BASS (ML1+19)
#define MANOWAR (ML3+12)
#define CROC (ML4+8)
#define BOGTHING (ML5+8)
#define CAMEL (ML1+15)
#define SHEEP (ML0+5)
#define GHOST (ML2+6)
#define HAUNT (ML4+5)
#define SPECTRE (ML5+5)
#define LICHE (ML6+5)
#define HORSE (ML2+13)


/* location functions */
#define L_NO_OP 0

/* random sites */
#define L_LIFT 1
#define L_BALANCESTONE 2
#define L_FIRE 3
#define L_WHIRLWIND 4
#define L_VOIDSTONE 5
#define L_WARNING 6
#define L_ARENA_EXIT 7
#define L_HOUSE_EXIT 8
#define L_SAFE 9

/* city level shop and guild functions */
/* following are those in CitySiteList */
#define NUMCITYSITES 26
#define CITYSITEBASE 10
#define L_CHARITY 10
#define L_ARMORER 11
#define L_CLUB 12
#define L_GYM 13
#define L_THIEVES_GUILD 14
#define L_COLLEGE 15
#define L_HEALER 16
#define L_CASINO 17
#define L_TAVERN 18
#define L_MERC_GUILD 19
#define L_ALCHEMIST 20
#define L_SORCERORS 21
#define L_CASTLE 22
#define L_ARENA 23
#define L_DPW 24
#define L_LIBRARY 25
#define L_PAWN_SHOP 26
#define L_BANK 27
#define L_CONDO 28
#define L_ORACLE 29
#define L_ORDER 30
#define L_DINER 31
#define L_COMMANDANT 32
#define L_CRAP 33
#define L_TEMPLE 34
#define L_COUNTRYSIDE 35
/* end of city sites */

/* random sites */
#define L_JAIL 36
#define L_TEMPLE_WARNING 37
#define L_LAWSTONE 38
#define L_CHAOSTONE 39

/* final abyss sites ignore levitation*/
#define L_EARTH_STATION 40
#define L_FIRE_STATION 41
#define L_WATER_STATION 42
#define L_AIR_STATION 43
#define L_VOID_STATION 44
#define L_VOID 45
#define L_VOICE1 46
#define L_VOICE2 47
#define L_VOICE3 48

#define L_SACRIFICESTONE 49

/* circle hq sites */
#define L_TOME1 50
#define L_TOME2 51
#define L_ENTER_CIRCLE 52
#define L_CIRCLE_LIBRARY 53

/* other site functions */
#define L_BROTHEL 54
#define L_DRUID 55
#define L_ALTAR 56

#define L_GARDEN 57
#define L_ADEPT 58
#define L_SEWER 59

#define L_OMEGA 60
#define L_CARTOGRAPHER 61
#define L_STABLES 62
#define L_COMMONS 63
#define L_GRANARY 64
#define L_MAZE 65
#define L_HOVEL 66
#define L_HOUSE 67
#define L_MANSION 68
#define L_OCCUPIED_HOUSE 69
#define L_TACTICAL_EXIT 70
#define L_VAULT 71
#define L_CEMETARY 72
#define L_THRONE 73
#define L_ESCALATOR 74
#define L_ENTER_COURT 75

#define L_TRIFID 76
#define L_FINAL_ABYSS 77
#define L_RAISE_PORTCULLIS 78

#define L_MINDSTONE 79

/* above LEVITATION_AVOIDANCE, no effect if player is levitating */
#define LEVITATION_AVOIDANCE 80

/* random sites */
#define L_CHAOS 81
#define L_WATER 82
#define L_LAVA 83
#define L_MAGIC_POOL 84
#define L_PORTCULLIS_TRAP 85
#define L_DROP_EVERY_PORTCULLIS 87
#define L_PORTCULLIS 88


/* traps */
#define NUMTRAPS 13
#define TRAP_BASE 89

/* traps */
#define L_TRAP_DART 89
#define L_TRAP_PIT 90
#define L_TRAP_DOOR 91
#define L_TRAP_SNARE 92
#define L_TRAP_BLADE 93
#define L_TRAP_FIRE 94
#define L_TRAP_TELEPORT 95
#define L_TRAP_DISINTEGRATE 96
#define L_TRAP_SLEEP_GAS 97
#define L_TRAP_ACID 98
#define L_TRAP_MANADRAIN 99
#define L_TRAP_ABYSS 100
#define L_TRAP_SIREN 101

/* more random sites */
#define L_STATUE_WAKE 102
#define L_STATUE_RANDOM 103

#define L_HEDGE 104
#define L_RUBBLE 105

#define L_ABYSS 106

/* player possession slots */
/* slot 0 should not be filled when out of inventory_control() */

#define O_UP_IN_AIR 0
#define O_READY_HAND 1
#define O_WEAPON_HAND 2
#define O_LEFT_SHOULDER 3
#define O_RIGHT_SHOULDER 4
#define O_BELT1 5
#define O_BELT2 6
#define O_BELT3 7
#define O_SHIELD 8 
#define O_ARMOR 9
#define O_BOOTS 10
#define O_CLOAK 11
#define O_RING1 12
#define O_RING2 13
#define O_RING3 14
#define O_RING4 15

/* structure definitions */

struct room {
  int lighted; 
  int left,right,top,bottom;
  int rsi; /* index into roomname switch */
};



struct spell {
  char known;
  char id;
  char powerdrain;
} ;


struct monster {
  struct objectlist *possessions;
  int attacked,aux1,aux2,x,y,click;
  int id,hp,hit,ac,dmg,sense,wakeup,level,speed,sleep,treasure;
  int xpv,corpseweight,corpsevalue,transformid,startthing,uniqueness;
  int talkf,movef,meleef,strikef,specialf;
  long status,immunity;
  char monchar;
  char *monstring,*corpsestr,*meleestr;
};


struct monsterlist {
  struct monster *m;
  struct monsterlist *next;
};




struct player {
  int str,con,dex,agi,iq,pow,maxstr,maxcon,maxdex,maxagi,maxiq,maxpow;
  int xp,level,hp,maxhp,hit,dmg,absorption,speed,click;
  int defense,food,alignment,mana,maxmana;
  int cash,patron,birthday;
  char preference;
  int sx,sy; /* sanctuary coordinates */
  int x,y; /* current player coordinates */
  int itemweight,maxweight;
  int immunity[NUMIMMUNITIES];
  int status[NUMSTATI];
  long options;
  int rank[NUMRANKS];
  int guildxp[NUMRANKS];
  char name[64];
  char meleestr[64];
  struct object *possessions[MAXITEMS];
  struct object *pack[MAXPACK];
  int packptr;
};



struct object {
  int id,weight,plus,charge,dmg,hit,aux,number,fragility;
  int basevalue, known, used, blessing,type,uniqueness,usef,level;
  char objchar,*objstr,*truename,*cursestr;
};



struct objectlist {
  struct object *thing;
  struct objectlist *next;
};



/* terrain locations */
struct terrain {
  char base_terrain_type;
  char current_terrain_type;
  char aux;
  char explored;
};

/* dungeon locations */
struct location {
  char p_locf; /* function executed when moved on */
  int lstatus; /* seen,stopsrun,lit,secret, */
  char roomnumber; /* so room can be named */
  char locchar; /* terrain type */
  char showchar; /*char instantaneously drawn for site */
  int aux; /* signifies various things */
  int buildaux; /* used in constructing level */
  struct objectlist *things; 
  struct monster *creature;
 };


struct level {
  char depth; /* which level is this */
  struct level *next; /* pointer to next level in dungeon */
  struct location site[MAXWIDTH][MAXLENGTH]; /* dungeon data */
  char generated; /* has the level been made (visited) yet? */
  char numrooms; /* number of rooms on level */
  char tunnelled; /* amount of tunnelling done on this level */
  struct monsterlist *mlist; /* List of monsters on level */
  int environment; /* where kind of level is this? */
  int deepest; /*If level is dungeon head, deepest level made */
};

 
/* random typedef's */

typedef struct monsterlist mltype;
typedef mltype *pml;

typedef struct monster montype;
typedef montype *pmt;

typedef struct location loctype;
typedef loctype *plc;

typedef struct level levtype;
typedef levtype *plv;

typedef struct object objtype;
typedef objtype *pob;

typedef struct objectlist oltype;
typedef oltype *pol;

/* random  function declarations from system libraries */

char *malloc(),*calloc(),*getlogin();
int free();
long time();

#ifdef NORANDOM
int rand();
int srand();
#define RANDFUNCTION rand()
#define SRANDFUNCTION srand((int)(time((long *)NULL)+Seed))
#endif

#ifndef NORANDOM
long random();
int srandom();
#define RANDFUNCTION random()
#define SRANDFUNCTION srandom((int)(time((long *)NULL)+Seed))
#endif


#define pow2(n) (1 << n)


/* systemV for some reason uses string.h instead of strings.h */
/* Also, random and srandom are unlikely to be found on system V... */

#ifdef STRING
#include <string.h>
#endif

#ifndef STRING
#include <strings.h>
#endif

#include <stdio.h>

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

